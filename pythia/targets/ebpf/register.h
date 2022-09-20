#ifndef PYTHIA_TARGETS_EBPF_REGISTER_H_
#define PYTHIA_TARGETS_EBPF_REGISTER_H_

#include "backends/pythia/common/p4ctool.h"

#include "backends/pythia/pythia/options.h"
#include "backends/pythia/pythia/targets/ebpf/ebpf.h"
#include "backends/pythia/pythia/targets/ebpf/target.h"
#include "backends/pythia/pythia/pythia.h"

namespace Pythia {

namespace P4Pythia {

/// Register the ebpf compiler target with the tools framework.
void ebpf_registerCompilerTarget() { EBPF::EBPFCompilerTarget::make(); }

/// Register the ebpf pythia target with the pythia framework.
void ebpf_registerPythiaTarget() { EBPF::EBPFPythiaTarget::make(); }

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_EBPF_REGISTER_H_ */
