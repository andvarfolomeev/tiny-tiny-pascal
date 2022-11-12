#include "simple_parser_tester.h"

#include <iostream>

#include "../src/scanner/scanner.h"
#include "../src/simple_parser/simple_parser.h"

bool testers::SimpleParserTester::run_test(
    const std::string& input_file_path, const std::string& output_file_path) {
    bool success = true;
    std::ifstream out_file;
    out_file.open(output_file_path);

    std::ifstream in_file;
    in_file.open(input_file_path);
    std::cout << input_file_path << std::endl;

    scanner::Scanner scanner(in_file);
    simpleparser::SimpleParser simple_parser(scanner);

    std::ostringstream simple_parser_answer;

    try {
        simple_parser.parse_expression()->draw(simple_parser_answer, 0);
    } catch (const simpleparser::SyntaxException& ex) {
        simple_parser_answer << "Exception: " << ex.what();
    } catch (const scanner::ScannerException& ex) {
        simple_parser_answer << "Exception: " << ex.what();
    }

    if (!out_file) {
        std::ofstream out_file_new;
        out_file_new.open(output_file_path);
        std::cout << "Out file doesnt exist" << std::endl;
        out_file_new << simple_parser_answer.str();
        return success;
    }

    auto output_file_content = read_file(output_file_path);
    if (simple_parser_answer.str() != output_file_content) {
        std::cout << "FAILED"
                  << "\n\tExpected: \t" << output_file_content << ";"
                  << std::endl
                  << "\tTaken:\t\t" << simple_parser_answer.str() << ";"
                  << std::endl;
        success = false;
    }
    return success;
}
