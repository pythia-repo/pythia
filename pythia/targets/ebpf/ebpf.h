#ifndef PYTHIA_TARGETS_EBPF_EBPF_H_
#define PYTHIA_TARGETS_EBPF_EBPF_H_

#include "backends/pythia/common/compiler/compiler_target.h"
#include "backends/pythia/common/compiler/midend.h"
#include "frontends/common/options.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

class EBPFCompilerTarget : public CompilerTarget {
 public:
    /// Registers this target.
    static void make();

 private:
    MidEnd mkMidEnd(const CompilerOptions& options) const override;

    EBPFCompilerTarget();
};

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_EBPF_EBPF_H_ */
