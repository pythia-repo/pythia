#include "backends/pythia/pythia/targets/ebpf/ebpf.h"

#include <string>

#include "frontends/common/options.h"
#include "ir/ir.h"

#include "backends/pythia/pythia/options.h"

namespace Pythia {

namespace P4Pythia {

namespace EBPF {

EBPFCompilerTarget::EBPFCompilerTarget() : CompilerTarget("ebpf", "ebpf") {}

void EBPFCompilerTarget::make() {
    static EBPFCompilerTarget* INSTANCE = nullptr;
    if (INSTANCE == nullptr) {
        INSTANCE = new EBPFCompilerTarget();
    }
}

MidEnd EBPFCompilerTarget::mkMidEnd(const CompilerOptions& options) const {
    MidEnd midEnd(options);
    midEnd.addPasses({});
    midEnd.addDefaultPasses();

    return midEnd;
}

}  // namespace EBPF

}  // namespace P4Pythia

}  // namespace Pythia
