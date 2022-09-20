#ifndef PYTHIA_TARGETS_BMV2_CMD_STEPPER_H_
#define PYTHIA_TARGETS_BMV2_CMD_STEPPER_H_

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
#include "backends/pythia/pythia/targets/bmv2/program_info.h"

namespace Pythia {

namespace P4Pythia {

namespace Bmv2 {

class BMv2_V1ModelCmdStepper : public CmdStepper {
 protected:
    std::string getClassName() override { return "BMv2_V1ModelCmdStepper"; }

    const BMv2_V1ModelProgramInfo& getProgramInfo() const override;

    void initializeTargetEnvironment(ExecutionState* nextState) const override;

    boost::optional<const Constraint*> startParser_impl(const IR::P4Parser* parser,
                                                        ExecutionState* state) const override;

    std::map<Continuation::Exception, Continuation> getExceptionHandlers(
        const IR::P4Parser* parser, Continuation::Body normalContinuation,
        const ExecutionState* state) const override;

 public:
    BMv2_V1ModelCmdStepper(ExecutionState& state, AbstractSolver& solver,
                           const ProgramInfo& programInfo);
};

}  // namespace Bmv2

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_BMV2_CMD_STEPPER_H_ */
