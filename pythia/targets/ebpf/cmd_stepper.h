#ifndef PYTHIA_TARGETS_EBPF_CMD_STEPPER_H_
#define PYTHIA_TARGETS_EBPF_CMD_STEPPER_H_

#include <map>
#include <string>

#include <boost/optional/optional.hpp>

#include "backends/pythia/common/core/solver.h"
#include "backends/pythia/common/lib/formulae.h"
#include "ir/ir.h"

#include "backends/pythia/pythia/core/program_info.h"
#include "backends/pythia/pythia/core/small_step/cmd_stepper.h"
#include "backends/pythia/pythia/lib/continuation.h"
#include "backends/pythia/pythia/lib/execution_state.h"
#include "backends/pythia/pythia/targets/ebpf/program_info.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

class EBPFCmdStepper : public CmdStepper {
 protected:
    std::string getClassName() override { return "EBPFCmdStepper"; }

    const EBPFProgramInfo& getProgramInfo() const override;

    void initializeTargetEnvironment(ExecutionState* nextState) const override;

    boost::optional<const Constraint*> startParser_impl(const IR::P4Parser* parser,
                                                        ExecutionState* state) const override;

    std::map<Continuation::Exception, Continuation> getExceptionHandlers(
        const IR::P4Parser* parser, Continuation::Body normalContinuation,
        const ExecutionState* state) const override;

 public:
    EBPFCmdStepper(ExecutionState& state, AbstractSolver& solver, const ProgramInfo& programInfo);
};

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_EBPF_CMD_STEPPER_H_ */
