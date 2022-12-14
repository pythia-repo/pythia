#include "backends/pythia/pythia/pythia.h"

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/none.hpp>
#include <boost/optional/optional.hpp>

#include "backends/pythia/common/core/z3_solver.h"
#include "backends/pythia/common/lib/timer.h"
#include "backends/pythia/common/lib/util.h"
#include "frontends/common/parser_options.h"
#include "lib/error.h"

#include "backends/pythia/pythia/core/exploration_strategy/exploration_strategy.h"
#include "backends/pythia/pythia/core/exploration_strategy/incremental_stack.h"
#include "backends/pythia/pythia/core/exploration_strategy/linear_enumeration.h"
#include "backends/pythia/pythia/core/exploration_strategy/random_access_stack.h"
#include "backends/pythia/pythia/core/exploration_strategy/selected_branches.h"
#include "backends/pythia/pythia/core/target.h"
#include "backends/pythia/pythia/lib/logging.h"
#include "backends/pythia/pythia/lib/test_backend.h"
#include "backends/pythia/pythia/register.h"

namespace fs = boost::filesystem;

namespace Pythia {

namespace P4Pythia {

void Pythia::registerTarget() {
    // Register all available compiler targets.
    // These are discovered by CMAKE, which fills out the register.h.in file.
    registerCompilerTargets();
}

int Pythia::mainImpl(const IR::P4Program* program) {
    // Register all available pythia targets.
    // These are discovered by CMAKE, which fills out the register.h.in file.
    registerPythiaTargets();

    const auto* programInfo = PythiaTarget::initProgram(program);
    if (programInfo == nullptr) {
        ::error("Program not supported by target device and architecture.");
        return EXIT_FAILURE;
    }
    if (::errorCount() > 0) {
        ::error("Pythia: Encountered errors during preprocessing. Exiting");
        return EXIT_FAILURE;
    }

    // Print basic information for each test.
    enableInformationLogging();

    auto const inputFile = P4CContext::get().options().file;
    cstring testDirStr = PythiaOptions::get().outputDir;
    auto seed = PythiaOptions::get().seed;

    // Get the basename of the input file and remove the extension
    // This assumes that inputFile is not null.
    auto programName = fs::path(inputFile).filename().replace_extension("");
    // Create the directory, if the directory string is valid and if it does not exist.
    auto testPath = programName;
    if (!testDirStr.isNullOrEmpty()) {
        auto testDir = fs::path(testDirStr);
        fs::create_directories(testDir);
        testPath = fs::path(testDir) / testPath;
    }

    if (seed != boost::none) {
        // Initialize the global seed for randomness.
        PythiaUtils::setRandomSeed(*seed);
        printFeature("test_info", 4, "============ Program seed %1% =============\n", *seed);
    }

    Z3Solver solver;

    auto symExec = [&solver, &programInfo, seed]() -> ExplorationStrategy* {
        if (PythiaOptions::get().popLevel > 1) {
            return new RandomAccessStack(solver, *programInfo, seed,
                                         PythiaOptions::get().popLevel);
        }
        if (!PythiaOptions::get().selectedBranches.empty()) {
            std::string selectedBranchesStr = PythiaOptions::get().selectedBranches;
            return new SelectedBranches(solver, *programInfo, seed, selectedBranchesStr);
        }
        if (PythiaOptions::get().linearEnumeration > 1) {
            return new LinearEnumeration(solver, *programInfo, seed,
                                         PythiaOptions::get().linearEnumeration);
        }
        return new IncrementalStack(solver, *programInfo, seed);
    }();

    // Define how to handle the final state for each test. This is target defined.
    auto* testBackend = PythiaTarget::getTestBackend(*programInfo, *symExec, testPath, seed);
    ExplorationStrategy::Callback callBack =
        std::bind(&TestBackEnd::run, testBackend, std::placeholders::_1);

    try {
        // Run the symbolic executor with given exploration strategy.
        symExec->run(callBack);
    } catch (...) {
        if (PythiaOptions::get().trackBranches) {
            // Print list of the selected branches and store all information into
            // dumpFolder/selectedBranches.txt file.
            // This printed list could be used for repeat this bug in arguments of --input-branches
            // command line. For example, --input-branches "1,1".
            symExec->printCurrentTraceAndBranches(std::cerr);
        }
        throw;
    }

    return ::errorCount() == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

}  // namespace P4Pythia

}  // namespace Pythia
