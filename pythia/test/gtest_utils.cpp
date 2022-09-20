#include "backends/pythia/pythia/test/gtest_utils.h"

#include <boost/none.hpp>

#include "backends/pythia/common/compiler/compiler_target.h"
#include "backends/pythia/common/core/target.h"
#include "frontends/common/options.h"
#include "frontends/common/parser_options.h"
#include "lib/compile_context.h"
#include "lib/exceptions.h"

#include "backends/pythia/pythia/register.h"

namespace Test {

boost::optional<const PythiaTestCase> PythiaTestCase::create(
    std::string deviceName, std::string archName, CompilerOptions::FrontendVersion langVersion,
    const std::string& source) {
    // Initialize the target.
    ensureInit();
    BUG_CHECK(Pythia::Target::init(deviceName, archName), "Target %1%/%2% not supported",
              deviceName, archName);

    // Set up the compilation context and set the source language.
    AutoCompileContext autoCompileContext(Pythia::CompilerTarget::makeContext());
    P4CContext::get().options().langVersion = langVersion;

    auto program = Pythia::CompilerTarget::runCompiler(source);
    if (!program) {
        return boost::none;
    }
    return PythiaTestCase{*program};
}

boost::optional<const PythiaTestCase> PythiaTestCase::create_14(std::string deviceName,
                                                                  std::string archName,
                                                                  const std::string& source) {
    return create(deviceName, archName, CompilerOptions::FrontendVersion::P4_14, source);
}

boost::optional<const PythiaTestCase> PythiaTestCase::create_16(std::string deviceName,
                                                                  std::string archName,
                                                                  const std::string& source) {
    return create(deviceName, archName, CompilerOptions::FrontendVersion::P4_16, source);
}

void PythiaTestCase::ensureInit() {
    static bool initialized = false;
    if (initialized) {
        return;
    }
    // Register supported compiler targets.
    Pythia::P4Pythia::registerCompilerTargets();

    // Register supported Pythia targets.
    Pythia::P4Pythia::registerPythiaTargets();

    initialized = true;
}

}  // namespace Test
