#include <exception>
#include <iostream>
#include <utility>

#include <boost/optional/optional.hpp>

#include "backends/pythia/common/version.h"
#include "backends/pythia/p4check/commands.h"
#include "lib/crash.h"
#include "lib/exceptions.h"
#include "lib/gc.h"

#include "backends/pythia/pythia/pythia.h"

int main(int argc, char** argv) {
    setup_gc_logging();
    setup_signals();

    // Catch all exceptions here.
    try {
        // Process command-line options to find the command being requested and hand off to that
        // command.
        if (const auto pair = Pythia::P4CheckCommands::get().process(argc, argv)) {
            const auto& command = pair->first;
            const auto& args = pair->second;

            switch (command) {
                case Pythia::HELP:
                    Pythia::P4CheckCommands::get().usage(argv[0]);
                    return 0;

                case Pythia::VERSION:
                    printVersion(argv[0]);
                    return 0;

                case Pythia::PYTHIA:
                    return Pythia::P4Pythia::Pythia().main(args);

                case Pythia::VERIFY:
                    std::cerr << "Verify is not implemented yet." << std::endl;
                    return 1;

                case Pythia::MUTATE:
                    std::cerr << "Mutate is not implemented yet." << std::endl;
                    return 1;

                case Pythia::SMITH:
                    std::cerr << "Smith is not implemented yet." << std::endl;
                    return 1;

                default:
                    std::cerr << "Unknown or unimplemented command: \"" << command << "\"."
                              << std::endl;
                    return 1;
            }
        } else {
            // Processing failed.
            return 1;
        }
    } catch (const Util::CompilerBug& e) {
        std::cerr << "Internal error: " << e.what() << std::endl;
        std::cerr << "Please submit a bug report with your code." << std::endl;
        return 1;
    } catch (const Util::CompilerUnimplemented& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const Util::CompilationError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Internal error: " << e.what() << std::endl;
        std::cerr << "Please submit a bug report with your code." << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Internal error. Please submit a bug report with your code." << std::endl;
        return 1;
    }
}
