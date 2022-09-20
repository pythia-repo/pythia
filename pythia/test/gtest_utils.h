#ifndef PYTHIA_TEST_GTEST_UTILS_H_
#define PYTHIA_TEST_GTEST_UTILS_H_

#include <string>

#include <boost/optional/optional.hpp>

#include "frontends/common/options.h"
#include "ir/ir.h"
#include "test/frameworks/gtest/googletest/include/gtest/gtest.h"

namespace Test {

/// Encapsulates functionality for running the front- and mid-ends of the compiler in a test.
class PythiaTestCase {
 public:
    /// Factory method for producing a test case from a P4 program source.
    static boost::optional<const PythiaTestCase> create(
        std::string deviceName, std::string archName, CompilerOptions::FrontendVersion langVersion,
        const std::string& source);

    /// Factory method for producing a test case from a P4_14 program source.
    static boost::optional<const PythiaTestCase> create_14(std::string deviceName,
                                                            std::string archName,
                                                            const std::string& source);

    /// Factory method for producing a test case from a P4_16 program source.
    static boost::optional<const PythiaTestCase> create_16(std::string deviceName,
                                                            std::string archName,
                                                            const std::string& source);

    /// The output of the compiler's mid end.
    const IR::P4Program* program;

 private:
    /// Ensures target plug-ins are initialized.
    static void ensureInit();
};

/// GTest for P4 Tools tests.
class PythiaTest : public ::testing::Test {};

}  // namespace Test

#endif /* PYTHIA_TEST_GTEST_UTILS_H_ */
