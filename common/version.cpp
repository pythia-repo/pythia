#include "backends/pythia/common/version.h"

#include <iostream>

void printVersion(const char* binaryName) {
    std::cout << binaryName << std::endl
              << "Version"
              << " " << PyTHIA_VERSION_MAJOR << "." << PyTHIA_VERSION_MINOR << "."
              << PyTHIA_VERSION_PATCH << PyTHIA_VERSION_HASH << std::endl;
}
