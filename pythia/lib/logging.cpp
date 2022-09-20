
#include "backends/pythia/pythia/lib/logging.h"

#include "lib/log.h"

namespace Pythia {

namespace P4Pythia {

void enableTraceLogging() { Log::addDebugSpec("test_traces:4"); }

void enableInformationLogging() { Log::addDebugSpec("test_info:4"); }

void enableStepLogging() { Log::addDebugSpec("small_step:4"); }

void enableCoverageLogging() { Log::addDebugSpec("coverage:4"); }

void enablePerformanceLogging() { Log::addDebugSpec("performance:4"); }

}  // namespace P4Pythia

}  // namespace Pythia
