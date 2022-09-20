#ifndef PYTHIA_PYTHIA_H_
#define PYTHIA_PYTHIA_H_

#include "backends/pythia/common/p4ctool.h"
#include "ir/ir.h"
#include "lib/cstring.h"

#include "backends/pythia/pythia/lib/final_state.h"
#include "backends/pythia/pythia/options.h"

namespace Pythia {

namespace P4Pythia {

/// This is main implementation of the P4Pythia tool.
class Pythia : public AbstractP4cTool<PythiaOptions> {
 protected:
    void registerTarget() override;

    int mainImpl(const IR::P4Program* program) override;
};

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_PYTHIA_H_ */
