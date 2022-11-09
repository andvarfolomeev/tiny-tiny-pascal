#include <argparse/argparse.hpp>
#include <iostream>

#include "scanner/scanner.h"
#include "simple_parser/simple_parser.h"

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("tiny_tiny_pascal");

    program.add_argument("file").help("path to source file");
    program.add_argument("--scanner")
        .help("run scanner")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("--simple-parser")
        .help("run simple parser")
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

    auto run_lexer = program.get<bool>("--scanner");
    if (run_lexer) {
        try {
            scanner::Scanner scanner(file);
            for (;;) {
                auto token = scanner.next_token();
                std::cout << token << "\n";
                if (token.get_type() == scanner::TokenType::eof) {
                    break;
                }
            }
        } catch (const scanner::ScannerException& ex) {
            std::cout << ex.what();
            return EXIT_FAILURE;
        }

        // reopen file
        file.close();
        file.open(path_to_source);
    }

    auto run_simple_parser = program.get<bool>("--simple-parser");
    if (run_simple_parser) {
        try {
            scanner::Scanner scanner(file);
            simpleparser::SimpleParser simple_parser(scanner);
            auto node = simple_parser.parse_expression();
            node->draw_tree(std::cout);
            delete node;
        } catch (const scanner::ScannerException& ex) {
            std::cout << ex.what();
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}