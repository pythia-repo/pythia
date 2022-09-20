#ifndef PYTHIA_LIB_EXCEPTIONS_H_
#define PYTHIA_LIB_EXCEPTIONS_H_

#include "lib/exceptions.h"

namespace Pythia {

namespace P4Pythia {

/// This class indicates a feature that is not implemented in P4Pythia.
/// Paths with this unimplemented feature should be skipped
class PythiaUnimplemented final : public Util::P4CExceptionBase {
 public:
    template <typename... T>
    explicit PythiaUnimplemented(const char* format, T... args)
        : P4CExceptionBase(format, args...) {
        // Check if output is redirected and if so, then don't color text so that
        // escape characters are not present
        message = cstring(Util::cerr_colorize(Util::ANSI_BLUE)) + "Not yet implemented" +
                  Util::cerr_clear_colors() + ":\n" + message;
    }

    template <typename... T>
    PythiaUnimplemented(int line, const char* file, const char* format, T... args)
        : P4CExceptionBase(format, args...) {
        message = cstring("In file: ") + file + ":" + Util::toString(line) + "\n" +
                  Util::cerr_colorize(Util::ANSI_BLUE) + "Unimplemented compiler support" +
                  Util::cerr_clear_colors() + ": " + message;
    }
};

#define PYTHIA_UNIMPLEMENTED(...)                                   \
    do {                                                             \
        throw PythiaUnimplemented(__LINE__, __FILE__, __VA_ARGS__); \
    } while (0)

}  // namespace P4Pythia

}  // namespace Pythia

#endif /* PYTHIA_LIB_EXCEPTIONS_H_ */
