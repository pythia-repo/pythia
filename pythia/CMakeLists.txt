# CMake file for Barefoot p4pythia.
cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

include(common)

project(p4pythia)

# Source files for p4pythia.
set(
  PYTHIA_SOURCES
  options.cpp
  pythia.cpp


  core/arch_spec.cpp
  core/externs.cpp
  core/program_info.cpp
  core/small_step/abstract_stepper.cpp
  core/small_step/cmd_stepper.cpp
  core/small_step/expr_stepper.cpp
  core/small_step/extern_stepper.cpp
  core/small_step/table_stepper.cpp
  core/small_step/small_step.cpp
  core/exploration_strategy/incremental_stack.cpp
  core/exploration_strategy/selected_branches.cpp
  core/exploration_strategy/random_access_stack.cpp
  core/exploration_strategy/linear_enumeration.cpp
  core/exploration_strategy/exploration_strategy.cpp
  core/target.cpp

  lib/concolic.cpp
  lib/continuation.cpp
  lib/execution_state.cpp
  lib/final_state.cpp
  lib/gen_eq.cpp
  lib/logging.cpp
  lib/namespace_context.cpp
  lib/test_backend.cpp
  lib/test_spec.cpp
  lib/tf.cpp
)

# GTest source files for p4pythia.
set(
  PYTHIA_GTEST_SOURCES
  #  # XXX These should be in a library.
  ${P4C_SOURCE_DIR}/test/gtest/helpers.cpp
  ${P4C_SOURCE_DIR}/test/gtest/gtestp4c.cpp

  test/gtest_utils.cpp
  test/lib/taint.cpp
  test/small-step/binary.cpp
  test/small-step/unary.cpp
  test/small-step/util.cpp
  test/small-step/value.cpp
  test/transformations/saturation_arithm.cpp
  test/z3-solver/asrt_model.cpp
  test/z3-solver/expressions.cpp
)

# Pythia libraries.
set(
  PYTHIA_LIBS
  pythia-common
  inja
)

file(GLOB tools_targets RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/targets ${CMAKE_CURRENT_SOURCE_DIR}/targets/*)
foreach(ext ${tools_targets})
  if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/targets/${ext}/CMakeLists.txt)
    # Generate an option that makes it possible to disable this extension.
    string(MAKE_C_IDENTIFIER ${ext} EXT_AS_IDENTIFIER)
    string(TOUPPER ${EXT_AS_IDENTIFIER} EXT_AS_OPTION_NAME)
    string(CONCAT ENABLE_EXT_OPTION "ENABLE_PYTHIA_TARGET_" ${EXT_AS_OPTION_NAME})
    string(CONCAT EXT_HELP_TEXT "Build the " ${ext} " target")
    option(${ENABLE_EXT_OPTION} ${EXT_HELP_TEXT} ON)
    if(${ENABLE_EXT_OPTION})
      message("-- Enabling target ${ext}")
      add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/targets/${ext})
      set(include_statements "${include_statements}#include \"backends/pythia/pythia/targets/${ext}/register.h\"\n")
      set(compiler_targets "${compiler_targets}    ${ext}_registerCompilerTarget();\n")
      set(pythia_targets "${pythia_targets}    ${ext}_registerPythiaTarget();\n")
    endif()
  endif()
endforeach(ext)

# Convert the list of files into #includes
foreach(include_file ${include_files})
endforeach()

# Fill the template
configure_file(register.h.in register.h)

add_pythia_library(p4pythia ${PYTHIA_SOURCES})

target_link_libraries(
  p4pythia
  ${PYTHIA_LIBS}
)

if(ENABLE_GTESTS)
  add_executable(gtest-p4pythia ${PYTHIA_GTEST_SOURCES})
  target_include_directories(
    gtest-p4pythia
    PRIVATE ${P4C_SOURCE_DIR}/test/frameworks/gtest/googlemock/include
    PRIVATE ${P4C_SOURCE_DIR}/test/frameworks/gtest/googletest/include
  )
  target_link_libraries(
    gtest-p4pythia
    PRIVATE p4pythia
    PRIVATE gtest
  )

  add_custom_target(
    linkgtest
    # Link P4 include files in a more convenient location.
    COMMAND
    for incl in p4include p4_14include \; do
    ${CMAKE_COMMAND} -E create_symlink
    ${P4C_BINARY_DIR}/\$$incl ${CMAKE_CURRENT_BINARY_DIR}/\$$incl \;
    done
  )
  add_dependencies(gtest-p4pythia linkgtest)

  if(ENABLE_TESTING)
    add_definitions("-DGTEST_HAS_PTHREAD=0")
    add_test(NAME gtest-p4pythia COMMAND gtest-p4pythia)
    set_tests_properties(gtest-p4pythia PROPERTIES LABELS "gtest-pythia")
  endif()

  if(ENABLE_IWYU)
    set_property(TARGET gtest-p4pythia PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
  endif()
endif()

if(ENABLE_IWYU)
  set_property(TARGET p4pythia PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()
