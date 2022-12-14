#ifndef PYTHIA_TARGETS_BMV2_TEST_BACKEND_H_
#define PYTHIA_TARGETS_BMV2_TEST_BACKEND_H_

#include <cstdint>
#include <string>
#include <vector>

#include <boost/filesystem.hpp>
#include <boost/optional/optional.hpp>

#include "backends/pythia/common/lib/model.h"
#include "backends/pythia/common/lib/trace_events.h"
#include "gsl/gsl-lite.hpp"
#include "ir/ir.h"
#include "lib/gmputil.h"

#include "backends/pythia/pythia/core/exploration_strategy/exploration_strategy.h"
#include "backends/pythia/pythia/core/program_info.h"
#include "backends/pythia/pythia/lib/execution_state.h"
#include "backends/pythia/pythia/lib/test_backend.h"
#include "backends/pythia/pythia/targets/bmv2/test_spec.h"

namespace Pythia {

namespace P4Pythia {

namespace Bmv2 {

class Bmv2TestBackend : public TestBackEnd {
 private:
    /// These three constants are used for a special case. When the output packet is 0 in BMv2, the
    /// packet is not dropped, instead you receive garbage (02000000) to be precise.
    /// These values model this output.
    ///  See also: https://github.com/p4lang/behavioral-model/issues/977
    static const int ZERO_PKT_WIDTH = 32;
    static const big_int ZERO_PKT_VAL;
    static const big_int ZERO_PKT_MAX;
    /// List of the supported back ends.
    static const std::vector<std::string> SUPPORTED_BACKENDS;

 public:
    explicit Bmv2TestBackend(const ProgramInfo& programInfo, ExplorationStrategy& symbex,
                             const boost::filesystem::path& testPath,
                             boost::optional<uint32_t> seed);

    TestBackEnd::TestInfo produceTestInfo(
        const ExecutionState* executionState, const Model* completedModel,
        const IR::Expression* outputPacketExpr, const IR::Expression* outputPortExpr,
        const std::vector<gsl::not_null<const TraceEvent*>>* programTraces) override;

    const TestSpec* createTestSpec(const ExecutionState* executionState,
                                   const Model* completedModel, const TestInfo& testInfo) override;
};

}  // namespace Bmv2

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_TARGETS_BMV2_TEST_BACKEND_H_ */
