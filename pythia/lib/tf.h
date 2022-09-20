#ifndef PYTHIA_LIB_TF_H_
#define PYTHIA_LIB_TF_H_

#include <cstddef>

#include <boost/optional/optional.hpp>

#include "lib/cstring.h"

#include "backends/pythia/pythia/lib/test_spec.h"

namespace Pythia {

namespace P4Pythia {

/// THe default base class for the various test frameworks (TF). Every test framework has a test
/// name and a seed associated with it.
class TF {
 protected:
    /// The @testName to be used in test case generation.
    const cstring testName;

    /// The seed used by the pythia.
    boost::optional<unsigned int> seed;

    /// Creates a generic test framework.
    explicit TF(cstring, boost::optional<unsigned int>);

 public:
    /// The method used to output the test case to be implemented by
    /// all the test frameworks (eg. STF, PTF, etc.).
    /// @param spec the testcase specification to be outputted
    /// @param selectedBranches string describing branches selected for this testcase
    /// @param testIdx testcase unique number identifier
    /// @param currentCoverage current coverage ratio (between 0.0 and 1.0)
    // attaches arbitrary string data to the test preamble.
    virtual void outputTest(const TestSpec* spec, cstring selectedBranches, size_t testIdx,
                            float currentCoverage) = 0;
};

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_LIB_TF_H_ */
