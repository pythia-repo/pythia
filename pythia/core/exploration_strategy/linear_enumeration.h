#ifndef PYTHIA_CORE_EXPLORATION_STRATEGY_LINEAR_ENUMERATION_H_
#define PYTHIA_CORE_EXPLORATION_STRATEGY_LINEAR_ENUMERATION_H_

#include <cstdint>
#include <ctime>
#include <functional>
#include <iosfwd>
#include <list>
#include <map>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include <boost/none.hpp>
#include <boost/optional/optional.hpp>

#include "backends/pythia/common/core/solver.h"
#include "backends/pythia/common/lib/formulae.h"
#include "gsl/gsl-lite.hpp"
#include "ir/ir.h"

#include "backends/pythia/pythia/core/exploration_strategy/exploration_strategy.h"
#include "backends/pythia/pythia/core/program_info.h"
#include "backends/pythia/pythia/core/small_step/small_step.h"
#include "backends/pythia/pythia/lib/execution_state.h"
#include "backends/pythia/pythia/lib/final_state.h"

namespace Pythia {

namespace P4Pythia {

/// Visits all possible explorable branches and stores them in a list,
/// which will be picked randomly to produce a test. This feature is still
/// *experimetnal* and it can take a long time to produce tests depending on
// the chosen max bound.
class LinearEnumeration : public ExplorationStrategy {
 public:
    /// Callbacks are invoked when the P4 program terminates. If the callback returns true,
    /// execution halts. Otherwise, execution of the P4 program continues on a different random
    /// path.
    using Callback = std::function<bool(const FinalState&)>;

    using Branch = ExplorationStrategy::Branch;
    using StepResult = ExplorationStrategy::StepResult;

    /// Executes the P4 program along a randomly chosen path. When the program terminates, the
    /// given callback is invoked. If the callback returns true, then the executor terminates.
    /// Otherwise, execution of the P4 program continues on a different random path.
    void run(const Callback& callBack) override;

    /// Constructor for this strategy, considering inheritance.
    LinearEnumeration(AbstractSolver& solver, const ProgramInfo& programInfo,
                      boost::optional<uint32_t> seed, int linearEnumeration);

 protected:
    /// The max size for the exploredBranches vector.
    uint64_t maxBound;

    /// A vector, wherein each element (a Branch) is from a terminal state, and therefore
    /// can generate a test case.
    std::vector<Branch> exploredBranches;

    /// Receives a branch and an evaluator, checks if the branch has a next state, then
    /// recursively steps into the states, gets the sucessors and calls itself again.
    ///
    /// It will recursively map all terminal states, or at least fill the exploredBranches
    /// vector until it reaches the maxBoud.
    void mapBranch(Branch& branch);
};

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_CORE_EXPLORATION_STRATEGY_LINEAR_ENUMERATION_H_ */
