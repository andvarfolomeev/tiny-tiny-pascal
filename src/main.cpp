#include <argparse/argparse.hpp>
#include <iostream>

#include "lexer/scanner.h"
#include "lexer/token_type.h"

/**
 * Print how use program
 */
std::string help() {
    return "Usage: tiny_tiny_compiler.exe [options]\n"
           "Options:\n"
           "\t --tests - run tests\n"
           "\t --file [file path]";
}

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("tiny_tiny_pascal");

    program.add_argument("file").help("path to source file");
    program.add_argument("--lexer")
        .help("run lexer")
        .default_value(false)
        .implicit_value(true);

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return EXIT_FAILURE;
    }

    auto path_to_source = program.get<std::string>("file");
    std::ifstream file(path_to_source);

    if (!file.good()) {
        std::cout << "file " << path_to_source << " doesnt exist "
                  << "\n";
    }

    std::cout << path_to_source << std::endl;

    auto run_lexer = program.get<bool>("--lexer");
    if (run_lexer) {
        try {
            lexer::Scanner scanner(file);
            for (;;) {
                auto token = scanner.next_token();
                std::cout << token << "\n";
                if (token.get_type() == lexer::TokenType::eof) {
                    break;
                }
            }
            return EXIT_SUCCESS;
        } catch (const lexer::ScannerException& ex) {
            std::cout << ex.what();
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}