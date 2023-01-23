#include <argparse/argparse.hpp>
#include <iostream>

#include "generator/generator.h"
#include "parser/parser.h"
#include "scanner/scanner.h"
#include "visitor/generator_visitor.h"
#include "visitor/printer_visitor.h"
#include "visitor/semantic_visitor.h"

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("tiny_tiny_pascal");

    program.add_argument("file").help("path to source file");
    program.add_argument("--scanner")
        .help("run scanner")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("--parser")
        .help("run parser")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("--semantic")
        .help("run semantic")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("--generation")
        .help("run generator")
        .default_value(false)
        .implicit_value(true);
    program.add_argument("--out")
        .help("path to outfile")
        .default_value(std::string{"a.out"})
        .required()
        .nargs(1)
        .metavar("PATH");

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
                if (token == scanner::TokenType::eof) {
                    break;
                }
            }
        } catch (const TinyPascalException& ex) {
            std::cout << ex.what();
            return EXIT_FAILURE;
        }

        // reopen file
        file.close();
        file.open(path_to_source);
    }

    auto run_parser = program.get<bool>("--parser");
    auto run_semantic = program.get<bool>("--semantic");
    if (run_parser) {
        try {
            if (run_semantic) {
                scanner::Scanner scanner(file);
                parser::Parser p(scanner);
                auto head = p.program();
                auto semantic_visitor =
                    std::make_shared<visitor::SemanticVisitor>();
                semantic_visitor->visit(head.get());
                auto printer_visitor =
                    std::make_shared<visitor::PrinterVisitor>(std::cout);
                printer_visitor->visit(head.get());
                std::cout << "\n";
                semantic_visitor->get_sym_table_stack()->draw(std::cout);
            } else {
                scanner::Scanner scanner(file);
                parser::Parser p(scanner);
                auto head = p.program();
                auto printer_visitor =
                    std::make_shared<visitor::PrinterVisitor>(std::cout);
                printer_visitor->visit(head.get());
            }
        } catch (const TinyPascalException& ex) {
            std::cout << ex.what();
            return EXIT_FAILURE;
        }
    }

    auto out_file_path = program.get<std::string>("--out");
    auto run_generation = program.get<bool>("--generation");
    if (run_generation) {
        try {
            scanner::Scanner scanner(file);
            parser::Parser p(scanner);
            auto head = p.program();
            auto semantic_visitor =
                std::make_shared<visitor::SemanticVisitor>();
            semantic_visitor->visit(head.get());
            Generator g;
            auto generation_visitor =
                std::make_shared<visitor::GeneratorVisitor>(g);
            generation_visitor->visit(head.get());
            std::ofstream out_file(out_file_path);
            g.write(out_file);
        } catch (const TinyPascalException& ex) {
            std::cout << ex.what();
            return EXIT_FAILURE;
        } catch (const std::exception& ex) {
            std::cout << ex.what();
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
