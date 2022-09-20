# Pythia - Testing Tools For P4 Targets

## Directory Structure

```
pythia
 ├─ cmake       ── common CMake modules
 ├─ common      ── C++ source: common code for the various components of p4check
 ├─ p4check     ── C++ source: the main entry point for p4check
 ├─ scripts
 │   ├─ hooks   ── git hooks
 │   └─ tools   ── development-support scripts (e.g., cpplint)
 ├─ submodules  ── external dependencies
 └─ pythia     ── C++ source: pythia
```

## Pythia
Pythia is a collection of tools that make testing P4 targets and programs a little easier. So far the platform supports the following tools and projects:

- [Pythia](https://github.com/p4lang/p4c/tree/main/backends/pythia/pythia): An input-output test case generator for P4.

## Building

Pythia can be built using the following CMAKE configuration in the P4C repository.

```
mkdir build
cd build
cmake .. -DENABLE_GMP=OFF -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DENABLE_TEST_TOOLS=ON
make
```

## Dependencies
* [inja](https://github.com/pantor/inja) template engine for testcase generation.
* [gsl-lite](https://github.com/gsl-lite/gsl-lite) C++ Core Guidelines Support Library, used to
support and enforce C++ best practices.

## Development Style
Currently, each C++ source directory has a few subdirectories, including:
* `core`, containing the core functionality of the submodule; and
* `lib`, containing supporting data structures.

The distinction between the two can be fuzzy. Here are some guiding principles
for where to find/put what:
* If it depends on anything in `core`, it belongs in `core`.
* If it's something that resembles a general-purpose data structure (e.g., an
  environment or a symbol table), it's probably in `lib`.

### C++ Coding style

We generally follow the [Google C++ Style
Guide](https://google.github.io/styleguide/cppguide.html). This is partially
enforced by `cpplint` and `clang-format` and their respective configuration files. Both tools run in a git hook and as part of CI. To run `clang-format` on Ubuntu 20.04, install it with `pip3 install --user clang-format`.

Some deviations from the Style Guide are highlighted below. While this
repository depends heavily on the main P4 compiler ([P4C](https://github.com/p4lang/p4c/)), the code there doesn't always follow our style; don't follow its precedent!

* Comments are important. The Style Guide's [section on
  comments](https://google.github.io/styleguide/cppguide.html#Comments) is
  required reading.
    * Classes, methods, and fields are documented with triple-slash
      Doxygen-style comments:
      ```
      /// An example class demonstrating a documentation comment.
      class C {};
      ```
    * We do not use copyright headers or license boilerplate in our source
      files. Where needed, these will be auto-generated during release
      packaging.
* Lines are wrapped at 100 characters.
* Indents are four spaces. Tab characters should not be used for indenting.
* Generally prefer a single class declaration per `.h` file, unless providing a
  library of related classes. Multiple classes may be declared in a `.cpp`
  file.
* Include headers in the following order: related header, C system headers, C++
  system headers, other library headers, `Pythia` headers. Each class of
  headers should be sorted alphabetically and separated by a blank line. Files
  from `Pythia` should be listed relative to the project root. Files from
  `p4c` should be listed relative to the submodule's root.

  For example, in `pythia/lib/foo.cpp`, format your includes as follows.
  (Note: the comments below are for explanatory purposes, and should be elided
  in the actual file.)
  ```
  // Related header
  #include "backends/pythia/lib/foo.h"

  // C system headers
  #include <stdlib.h>
  #include <time.h>

  // C++ system headers
  #include <boost/optional.hpp>
  #include <set>
  #include <vector>

  // Other library headers
  #include "lib/error_type.h"
  #include "gsl/gsl-lite.hpp"
  #include "ir.h"

  // Headers from Pythia folders
  #include "backends/pythia/pythia/common/lib/model.h"
  #include "backends/pythia/pythia/lib/symbolic_env.h"
  ```



