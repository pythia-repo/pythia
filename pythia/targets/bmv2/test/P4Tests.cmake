include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/TestUtils.cmake)

# Add v1model tests from the p4c submodule
set(PyTHIA_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(P4PYTHIA_DIR ${CMAKE_SOURCE_DIR}/build/pythia)
set(P4PYTHIA_DRIVER "${PyTHIA_BINARY_DIR}/p4check pythia")

# This file defines how we write the tests we generate.
set(TEMPLATE_FILE ${CMAKE_CURRENT_LIST_DIR}/STFTestTemplate.cmake)

if(NOT PyTHIA_BMV2_PATH)
  set(PyTHIA_BMV2_PATH ${CMAKE_HOME_DIRECTORY}/build)
endif()

if(NOT NIGHTLY)
  set(EXTRA_OPTS "--print-traces --seed 1000 --max-tests 10")
else()
  set(EXTRA_OPTS "--print-traces --max-tests 10")
endif()

set(V1_SEARCH_PATTERNS "include.*v1model.p4" "main|common_v1_test")
set(P4TESTDATA ${P4C_SOURCE_DIR}/../p4c/testdata)
set(P4TESTS_FOR_BMV2 "${P4TESTDATA}/p4_16_samples/*.p4")
p4c_find_tests("${P4TESTS_FOR_BMV2}" P4_16_V1_TESTS INCLUDE "${V1_SEARCH_PATTERNS}" EXCLUDE "")
pythia_find_tests("${P4_16_V1_TESTS}" v1tests EXCLUDE "")

set(
  PYTHIA_BMV2_P416_TESTS
  "${CMAKE_CURRENT_LIST_DIR}/p4-programs/*.p4"
)

p4c_find_tests("${PYTHIA_BMV2_P416_TESTS}" BMV2_P4_16_V1_TESTS INCLUDE "${V1_SEARCH_PATTERNS}" EXCLUDE "")
pythia_find_tests("${BMV2_P4_16_V1_TESTS}" bmv2v1tests EXCLUDE "")

# Add bmv2 tests from p4c and from pythia/test/p4-programs/bmv2
set(P4C_V1_TEST_SUITES_P416 ${v1tests} ${bmv2v1tests})

pythia_add_tests(
  ${TEMPLATE_FILE}
  "pythia-p4c-bmv2" ${P4PYTHIA_DRIVER} "${P4C_V1_TEST_SUITES_P416}"
  "" "bmv2" "v1model" "p4-16" TRUE "-I${P4C_BINARY_DIR}/p4include --test-backend STF ${EXTRA_OPTS} "
)

#############################################################################
# TEST PROPERTIES
#############################################################################

# Add bmv2 PTF tests from p4c and from pythia/test/p4-programs/bmv2
set(P4C_V1_TEST_SUITES_P416_PTF ${bmv2v1tests})

pythia_add_tests(
  ${TEMPLATE_FILE}
  "pythia-p4c-bmv2-ptf" ${P4PYTHIA_DRIVER} "${P4C_V1_TEST_SUITES_P416_PTF}"
  "" "bmv2" "v1model" "p4-16" TRUE "-I${P4C_BINARY_DIR}/p4include --test-backend PTF-P4 ${EXTRA_OPTS}"
)

pythia_add_tests(
  ${TEMPLATE_FILE}
  "pythia-p4c-bmv2-protobuf" ${P4PYTHIA_DRIVER} "${P4C_V1_TEST_SUITES_P416}"
  "" "bmv2" "v1model" "p4-16" FALSE "-I${P4C_BINARY_DIR}/p4include --test-backend Protobuf ${EXTRA_OPTS}"
)

include(${CMAKE_CURRENT_LIST_DIR}/BMV2Xfail.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/BMV2PTFXfail.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/BMV2ProtobufXfail.cmake)
