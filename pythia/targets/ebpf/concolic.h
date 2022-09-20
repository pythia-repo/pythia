#ifndef PYTHIA_TARGETS_EBPF_CONCOLIC_H_
#define PYTHIA_TARGETS_EBPF_CONCOLIC_H_

#include <cstddef>
#include <functional>
#include <vector>

#include "backends/pythia/common/lib/model.h"
#include "ir/ir.h"
#include "lib/gmputil.h"

#include "backends/pythia/pythia/lib/concolic.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

class EBPFConcolic : public Concolic {
 private:
    /// This is the list of concolic functions that are implemented in this class.
    static const ConcolicMethodImpls::ImplList EBPFConcolicMethodImpls;

 public:
    /// @returns the concolic  functions that are implemented for this particular target.
    static const ConcolicMethodImpls::ImplList* getEBPFConcolicMethodImpls();
};

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_EBPF_CONCOLIC_H_ */
