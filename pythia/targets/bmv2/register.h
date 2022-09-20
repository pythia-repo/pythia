#ifndef PYTHIA_TARGETS_BMV2_REGISTER_H_
#define PYTHIA_TARGETS_BMV2_REGISTER_H_

#include "backends/pythia/common/p4ctool.h"

#include "backends/pythia/pythia/options.h"
#include "backends/pythia/pythia/targets/bmv2/bmv2.h"
#include "backends/pythia/pythia/targets/bmv2/target.h"
#include "backends/pythia/pythia/pythia.h"

namespace Pythia {

namespace P4Pythia {

/// Register the BMv2 compiler target with the tools framework.
void bmv2_registerCompilerTarget() { Bmv2::BMv2_V1ModelCompilerTarget::make(); }

/// Register the BMv2 pythia target with the pythia framework.
void bmv2_registerPythiaTarget() { Bmv2::BMv2_V1ModelPythiaTarget::make(); }

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_BMV2_REGISTER_H_ */
