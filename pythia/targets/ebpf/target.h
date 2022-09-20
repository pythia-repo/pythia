#ifndef PYTHIA_TARGETS_EBPF_TARGET_H_
#define PYTHIA_TARGETS_EBPF_TARGET_H_

#include <stdint.h>

#include <boost/filesystem.hpp>
#include <boost/optional/optional.hpp>

#include "backends/pythia/common/core/solver.h"
#include "ir/ir.h"

#include "backends/pythia/pythia/core/exploration_strategy/exploration_strategy.h"
#include "backends/pythia/pythia/core/program_info.h"
#include "backends/pythia/pythia/core/target.h"
#include "backends/pythia/pythia/lib/execution_state.h"
#include "backends/pythia/pythia/targets/ebpf/cmd_stepper.h"
#include "backends/pythia/pythia/targets/ebpf/expr_stepper.h"
#include "backends/pythia/pythia/targets/ebpf/program_info.h"
#include "backends/pythia/pythia/targets/ebpf/test_backend.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

class EBPFPythiaTarget : public PythiaTarget {
 public:
    /// Registers this target.
    static void make();

 protected:
    const EBPFProgramInfo* initProgram_impl(
        const IR::P4Program* program, const IR::Declaration_Instance* mainDecl) const override;

    int getPortNumWidth_bits_impl() const override;

    EBPFTestBackend* getTestBackend_impl(const ProgramInfo& programInfo,
                                         ExplorationStrategy& symbex,
                                         const boost::filesystem::path& testPath,
                                         boost::optional<uint32_t> seed) const override;

    EBPFCmdStepper* getCmdStepper_impl(ExecutionState& state, AbstractSolver& solver,
                                       const ProgramInfo& programInfo) const override;

    EBPFExprStepper* getExprStepper_impl(ExecutionState& state, AbstractSolver& solver,
                                         const ProgramInfo& programInfo) const override;

    const ArchSpec* getArchSpecImpl() const override;

 private:
    EBPFPythiaTarget();

    static const ArchSpec archSpec;
};

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_EBPF_TARGET_H_ */
