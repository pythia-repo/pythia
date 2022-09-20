# Common configuration across all Pythia projects.

# Use ccache if available.
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  message(STATUS "Enabling ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

# Use C++11.
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use libgc.
find_package(LibGc 7.2.0 REQUIRED)

# Helper for defining a pythia executable target.
function(add_pythia_executable target source)
  add_executable(${target} ${source} ${ARGN})
  target_include_directories(
    ${target}
    PUBLIC "${CMAKE_SOURCE_DIR}"
    PUBLIC "${CMAKE_BINARY_DIR}"
  )
  install(TARGETS ${target} RUNTIME DESTINATION ${P4C_RUNTIME_OUTPUT_DIRECTORY})
endfunction(add_pythia_executable)

# Helper for defining a pythia library target.
function(add_pythia_library target)
  add_library(${target} ${ARGN})
  target_include_directories(
    ${target}
    PUBLIC "${CMAKE_SOURCE_DIR}"
    PUBLIC "${CMAKE_BINARY_DIR}"
  )
endfunction(add_pythia_library)

# Helper for adding a subproject and configuring it for linting.
function(add_and_lint_subdirectory dir cpplint_dir)
  add_subdirectory(${dir})
  add_test(
    NAME "cpplint-${dir}"
    COMMAND sh -c "find '${dir}' \
        -iname \\*.h -o -iname \\*.hpp -o -iname \\*.c -o -iname \\*.cpp \
      | xargs python3 ${cpplint_dir}/cpplint.py \
          --quiet "
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  )
endfunction(add_and_lint_subdirectory)
