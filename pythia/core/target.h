#ifndef PYTHIA_CORE_TARGET_H_
#define PYTHIA_CORE_TARGET_H_

#include <stddef.h>
#include <stdint.h>

#include <map>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/optional/optional.hpp>

#include "backends/pythia/common/core/solver.h"
#include "backends/pythia/common/core/target.h"
#include "ir/ir.h"
#include "lib/cstring.h"

#include "backends/pythia/pythia/core/arch_spec.h"
#include "backends/pythia/pythia/core/exploration_strategy/exploration_strategy.h"
#include "backends/pythia/pythia/core/program_info.h"
#include "backends/pythia/pythia/core/small_step/cmd_stepper.h"
#include "backends/pythia/pythia/core/small_step/expr_stepper.h"
#include "backends/pythia/pythia/lib/execution_state.h"
#include "backends/pythia/pythia/lib/namespace_context.h"
#include "backends/pythia/pythia/lib/test_backend.h"

namespace Pythia {

namespace P4Pythia {

class PythiaTarget : public Target {
 public:
    /// @returns the singleton instance for the current target.
    static const PythiaTarget& get();

    /// Produces a @ProgramInfo for the given P4 program.
    ///
    /// @returns nullptr if the program is not supported by this target.
    static const ProgramInfo* initProgram(const IR::P4Program* program) {
        return get().initProgram_impl(program);
    }

    /// Returns the test back end associated with this P4Pythia target.
    static TestBackEnd* getTestBackend(const ProgramInfo& programInfo, ExplorationStrategy& symbex,
                                       const boost::filesystem::path& testPath,
                                       boost::optional<uint32_t> seed) {
        return get().getTestBackend_impl(programInfo, symbex, testPath, seed);
    }

    /// The width of a port number, in bits.
    static int getPortNumWidth_bits() { return get().getPortNumWidth_bits_impl(); }

    /// Provides a CmdStepper implementation for this target.
    static CmdStepper* getCmdStepper(ExecutionState& state, AbstractSolver& solver,
                                     const ProgramInfo& programInfo) {
        return get().getCmdStepper_impl(state, solver, programInfo);
    }

    /// Provides a ExprStepper implementation for this target.
    static ExprStepper* getExprStepper(ExecutionState& state, AbstractSolver& solver,
                                       const ProgramInfo& programInfo) {
        return get().getExprStepper_impl(state, solver, programInfo);
    }

    /// A vector that maps the architecture parameters of each pipe to the corresponding
    /// global architecture variables. For example, this map specifies which parameter of each pipe
    /// refers to the input header.
    // The arch map needs to be public to be subclassed.
    /// @returns a reference to the architecture map defined in this target
    static const ArchSpec* getArchSpec() { return get().getArchSpecImpl(); }

 protected:
    /// @see @initProgram.
    const ProgramInfo* initProgram_impl(const IR::P4Program* program) const;

    /// @see @initProgram.
    virtual const ProgramInfo* initProgram_impl(const IR::P4Program* program,
                                                const IR::Declaration_Instance* mainDecl) const = 0;

    /// @see getPortNumWidth_bits.
    virtual int getPortNumWidth_bits_impl() const = 0;

    /// @see getTestBackend.
    virtual TestBackEnd* getTestBackend_impl(const ProgramInfo& programInfo,
                                             ExplorationStrategy& symbex,
                                             const boost::filesystem::path& testPath,
                                             boost::optional<uint32_t> seed) const = 0;

    /// @see getCmdStepper.
    virtual CmdStepper* getCmdStepper_impl(ExecutionState& state, AbstractSolver& solver,
                                           const ProgramInfo& programInfo) const = 0;

    /// @see getExprStepper.
    virtual ExprStepper* getExprStepper_impl(ExecutionState& state, AbstractSolver& solver,
                                             const ProgramInfo& programInfo) const = 0;

    /// @see getArchSpec
    virtual const ArchSpec* getArchSpecImpl() const = 0;

    /// Utility function. Converts the list of arguments @inputArgs to a list of type declarations
    ///  and appends the result to @v. Any names appearing in the arguments are
    /// resolved with @ns.
    //
    static void argumentsToTypeDeclarations(const NamespaceContext* ns,
                                            const IR::Vector<IR::Argument>* inputArgs,
                                            std::vector<const IR::Type_Declaration*>& resultDecls);

    explicit PythiaTarget(std::string deviceName, std::string archName);

 private:
};

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_CORE_TARGET_H_ */
