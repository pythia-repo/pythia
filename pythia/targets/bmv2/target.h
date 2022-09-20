#ifndef PYTHIA_TARGETS_BMV2_TARGET_H_
#define PYTHIA_TARGETS_BMV2_TARGET_H_

#include <stdint.h>

#include <boost/filesystem.hpp>
#include <boost/optional/optional.hpp>

#include "backends/pythia/common/core/solver.h"
#include "ir/ir.h"

#include "backends/pythia/pythia/core/exploration_strategy/exploration_strategy.h"
#include "backends/pythia/pythia/core/program_info.h"
#include "backends/pythia/pythia/core/target.h"
#include "backends/pythia/pythia/lib/execution_state.h"
#include "backends/pythia/pythia/targets/bmv2/cmd_stepper.h"
#include "backends/pythia/pythia/targets/bmv2/expr_stepper.h"
#include "backends/pythia/pythia/targets/bmv2/program_info.h"
#include "backends/pythia/pythia/targets/bmv2/test_backend.h"

namespace Pythia {

namespace P4Pythia {

namespace Bmv2 {

class BMv2_V1ModelPythiaTarget : public PythiaTarget {
 public:
    /// Registers this target.
    static void make();

 protected:
    const BMv2_V1ModelProgramInfo* initProgram_impl(
        const IR::P4Program* program, const IR::Declaration_Instance* mainDecl) const override;

    int getPortNumWidth_bits_impl() const override;

    Bmv2TestBackend* getTestBackend_impl(const ProgramInfo& programInfo,
                                         ExplorationStrategy& symbex,
                                         const boost::filesystem::path& testPath,
                                         boost::optional<uint32_t> seed) const override;

    BMv2_V1ModelCmdStepper* getCmdStepper_impl(ExecutionState& state, AbstractSolver& solver,
                                               const ProgramInfo& programInfo) const override;

    BMv2_V1ModelExprStepper* getExprStepper_impl(ExecutionState& state, AbstractSolver& solver,
                                                 const ProgramInfo& programInfo) const override;

    const ArchSpec* getArchSpecImpl() const override;

 private:
    BMv2_V1ModelPythiaTarget();

    static const ArchSpec archSpec;
};

}  // namespace Bmv2

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_BMV2_TARGET_H_ */
