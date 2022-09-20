#ifndef PYTHIA_TARGETS_BMV2_BMV2_H_
#define PYTHIA_TARGETS_BMV2_BMV2_H_

#include "backends/pythia/common/compiler/compiler_target.h"
#include "backends/pythia/common/compiler/midend.h"
#include "frontends/common/options.h"

namespace Pythia {

namespace P4Pythia {

namespace Bmv2 {

class BMv2_V1ModelCompilerTarget : public CompilerTarget {
 public:
    /// Registers this target.
    static void make();

 private:
    MidEnd mkMidEnd(const CompilerOptions& options) const override;

    BMv2_V1ModelCompilerTarget();
};

}  // namespace Bmv2

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_BMV2_BMV2_H_ */
