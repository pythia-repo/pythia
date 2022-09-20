#ifndef PYTHIA_TARGETS_EBPF_TABLE_STEPPER_H_
#define PYTHIA_TARGETS_EBPF_TABLE_STEPPER_H_

#include <map>
#include <vector>

#include "ir/ir.h"
#include "lib/cstring.h"

#include "backends/pythia/pythia/core/small_step/table_stepper.h"
#include "backends/pythia/pythia/lib/execution_state.h"
#include "backends/pythia/pythia/lib/test_spec.h"
#include "backends/pythia/pythia/targets/ebpf/expr_stepper.h"
#include "backends/pythia/pythia/targets/ebpf/test_spec.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

class EBPFTableStepper : public TableStepper {
 private:
    /// Specifies the type of the table implementation:
    /// standard: standard implementation - use normal control plane entries.
    /// constant: The table is constant - no control entries are possible.
    /// skip: Skip the implementation and just use the default entry (no entry at all).
    enum class TableImplementation { standard, selector, profile, constant, skip };

    /// eBPF specific table properties.
    struct EBPFProperties {
        /// The type of the table implementation.
        TableImplementation implementaton;
    } EBPFProperties;

 protected:
    const IR::Expression* computeTargetMatchType(ExecutionState* nextState,
                                                 const KeyProperties& keyProperties,
                                                 std::map<cstring, const FieldMatch>* matches,
                                                 const IR::Expression* hitCondition) override;

    void checkTargetProperties(
        const std::vector<const IR::ActionListElement*>& tableActionList) override;

    void evalTargetTable(const std::vector<const IR::ActionListElement*>& tableActionList) override;

 public:
    explicit EBPFTableStepper(EBPFExprStepper* stepper, const IR::P4Table* table);
};

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_EBPF_TABLE_STEPPER_H_ */
