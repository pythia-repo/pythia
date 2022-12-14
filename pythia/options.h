#ifndef PYTHIA_OPTIONS_H_
#define PYTHIA_OPTIONS_H_

#include <string>

#include "backends/pythia/common/options.h"
#include "lib/cstring.h"

namespace Pythia {

/// Encapsulates and processes command-line options for p4pythia.
class PythiaOptions : public AbstractP4cToolOptions {
 public:
    /// Whether to produce just the input packet for each test.
    bool inputPacketOnly = false;

    /// Maximum number of tests to be generated. Defaults to 1.
    int maxTests = 1;

    /// Fixed packet size in bits. Defaults to 0, which implies no sizing.
    int packetSize = 0;

    /// Level of multiPop step. A good value is 10, namely, 10 per cent of
    /// the size of the unexploredBranches. The smaller the number,
    /// the bigger the step; e.g. unexploredBranches size == 100
    /// then this variable calculates 100/10 or 100/2 for the pop level.
    /// Defaults to 0, so we invoke a single pop()
    int popLevel = 0;

    /// Activates LinearEnumeration explorations trategy, and specifies
    /// the max bound of the buffer vector collecting all terminal
    /// branches. Defaults to 0, which means this strategy is not activated.
    int linearEnumeration = 0;

    /// @returns the singleton instance of this class.
    static PythiaOptions& get();

    /// Directory for generated tests. Defaults to PWD.
    cstring outputDir = nullptr;

    /// Do not fail on unimplemented features. Instead, try the next branch.
    bool permissive = false;

    /// The test back end that P4Pythia will generate test for. Examples, STF, PTF or Protobuf.
    cstring testBackend;

    /// String of selected branches separated by comma.
    std::string selectedBranches;

    /// Track the branches that are executed in the symbolic executor. This can be used for
    /// deterministic replay of an execution trace.
    bool trackBranches = false;

    const char* getIncludePath() override;

 private:
    PythiaOptions();
};

}  // namespace Pythia

#endif /* PYTHIA_OPTIONS_H_ */
