# CMake file for common components of Pythia.
cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

include(common)

project(pythia-common)

# We need a fairly recent version of Z3.
set(Z3_MIN_VERSION "4.8.14")
find_package(Z3 ${Z3_MIN_VERSION} REQUIRED)

if(NOT DEFINED Z3_VERSION_STRING OR ${Z3_VERSION_STRING} VERSION_LESS ${Z3_MIN_VERSION})
  message(FATAL_ERROR "The minimum required Z3 version is ${Z3_MIN_VERSION}. Has ${Z3_VERSION_STRING}.")
endif()

# Boost filesystem is required for path handling.
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem system iostreams)

# Store the current git commit hash as part of version information.
execute_process(
  COMMAND git log --pretty=format:-%h -n 1
  OUTPUT_VARIABLE PyTHIA_VERSION_HASH
  ERROR_QUIET
)

if(NOT "${PyTHIA_VERSION_HASH}" STREQUAL "")
  message(STATUS "Current git refpoint: ${PyTHIA_VERSION_HASH}")
endif()

# Generate version information.
configure_file(version.h.in version.h)

# Source files for pythia-common.
set(
  P4C_TOOLS_COMMON_SOURCES
  options.cpp
  version.cpp

  compiler/compiler_target.cpp
  compiler/convert_errors.cpp
  compiler/convert_varbits.cpp
  compiler/copy_headers.cpp
  compiler/hs_index_simplify.cpp
  compiler/midend.cpp

  core/target.cpp
  core/z3_solver.cpp

  lib/coverage.cpp
  lib/format_bin_hex.cpp
  lib/formulae.cpp
  lib/ir.cpp
  lib/model.cpp
  lib/saturation_elim.cpp
  lib/symbolic_env.cpp
  lib/taint.cpp
  lib/timer.cpp
  lib/trace_events.cpp
  lib/util.cpp
  lib/zombie.cpp
)

add_pythia_library(pythia-common ${P4C_TOOLS_COMMON_SOURCES})

target_link_libraries(
  pythia-common
  PUBLIC gsl-lite
  PUBLIC ${Boost_SYSTEM_LIBRARY}
  PUBLIC ${Boost_FILESYSTEM_LIBRARY}
  PUBLIC ${Z3_LIBRARY}
)

target_include_directories(
  pythia-common
  PUBLIC "${CMAKE_BINARY_DIR}/common"
  PUBLIC "${P4C_SOURCE_DIR}"
  PUBLIC "${P4C_SOURCE_DIR}/test/frameworks/gtest/googletest/include"
  PUBLIC "${P4C_BINARY_DIR}"
)

add_dependencies(pythia-common genIR frontend)
if(ENABLE_IWYU)
  set_property(TARGET pythia-common PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()
