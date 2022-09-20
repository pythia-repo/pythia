#ifndef PYTHIA_TARGETS_EBPF_EXPR_STEPPER_H_
#define PYTHIA_TARGETS_EBPF_EXPR_STEPPER_H_

#include <string>

#include "backends/pythia/common/core/solver.h"
#include "ir/ir.h"

#include "backends/pythia/pythia/core/program_info.h"
#include "backends/pythia/pythia/core/small_step/expr_stepper.h"
#include "backends/pythia/pythia/lib/execution_state.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

class EBPFExprStepper : public ExprStepper {
 protected:
    std::string getClassName() override { return "EBPFExprStepper"; }

 public:
    EBPFExprStepper(ExecutionState& state, AbstractSolver& solver, const ProgramInfo& programInfo);

    void evalExternMethodCall(const IR::MethodCallExpression* call, const IR::Expression* receiver,
                              IR::ID name, const IR::Vector<IR::Argument>* args,
                              ExecutionState& state) override;

    bool preorder(const IR::P4Table* /*table*/) override;
};
}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_EBPF_EXPR_STEPPER_H_ */
