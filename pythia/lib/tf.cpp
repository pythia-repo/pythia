#include "backends/pythia/pythia/lib/tf.h"

#include <boost/none.hpp>

namespace Pythia {

namespace P4Pythia {

TF::TF(cstring testName, boost::optional<unsigned int> seed = boost::none)
    : testName(testName), seed(seed) {}

}  // namespace P4Pythia

}  // namespace Pythia
