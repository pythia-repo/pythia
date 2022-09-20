include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/TestUtils.cmake)

# Add eBPF tests from the p4c submodule
set(PyTHIA_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(P4PYTHIA_DIR ${CMAKE_SOURCE_DIR}/build/pythia)
set(P4PYTHIA_DRIVER "${PyTHIA_BINARY_DIR}/p4check pythia")

# This file defines how we write the tests we generate.
set(TEMPLATE_FILE ${CMAKE_CURRENT_LIST_DIR}/STFTestTemplate.cmake)

if(NOT PyTHIA_EBPF_PATH)
  set(PyTHIA_EBPF_PATH ${CMAKE_HOME_DIRECTORY}/build)
endif()

if(NOT NIGHTLY)
  set(EXTRA_OPTS "--print-traces --seed 1000 --max-tests 10")
else()
  set(EXTRA_OPTS "--print-traces --max-tests 10")
endif()

set(EBPF_SEARCH_PATTERNS "include.*ebpf_model.p4")
set(P4TESTDATA ${P4C_SOURCE_DIR}/../p4c/testdata)
set(P4TESTS_FOR_EBPF "${P4TESTDATA}/p4_16_samples/*.p4")
p4c_find_tests("${P4TESTS_FOR_EBPF}" EBPF_TESTS INCLUDE "${EBPF_SEARCH_PATTERNS}" EXCLUDE "")
pythia_find_tests("${EBPF_TESTS}" ebpftests EXCLUDE "")


# Add ebpf tests from p4c
set(P4C_EBPF_TEST_SUITES_P416 ${ebpftests})

pythia_add_tests(
  ${TEMPLATE_FILE}
  "pythia-p4c-ebpf" ${P4PYTHIA_DRIVER} "${P4C_EBPF_TEST_SUITES_P416}"
  "" "ebpf" "ebpf" "p4-16" TRUE "-I${P4C_BINARY_DIR}/p4include --test-backend STF ${EXTRA_OPTS} "
)

#############################################################################
# TEST PROPERTIES
#############################################################################

include(${CMAKE_CURRENT_LIST_DIR}/EBPFXfail.cmake)
