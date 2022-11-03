#include "scanner_tester.h"

#include <iostream>

#include "../src/scanner/scanner.h"

namespace testers {

bool ScannerTester::run_test(const std::string& input_file_path,
                             const std::string& output_file_path) {
    bool success = true;
    std::ifstream out_file;
    out_file.open(output_file_path);

    std::ifstream in_file;
    in_file.open(input_file_path);
    std::cout << input_file_path << std::endl;
    scanner::Scanner scanner(in_file);

    if (!out_file) {
        std::ofstream out_file_new;
        out_file_new.open(output_file_path);
        std::cout << "Out file doesnt exist" << std::endl;
        while (!scanner.eof()) {
            try {
                auto token = scanner.next_token();
                std::cout << token << std::endl;
                out_file_new << token;
                if (token.get_type() != scanner::TokenType::eof) {
                    out_file_new << std::endl;
                }
            } catch (const scanner::ScannerException& ex) {
                std::string lexer_message = "Exception: ";
                lexer_message += ex.what();
                out_file_new << lexer_message;
                return success;
            }
        }
        return success;
    }

    std::string line;
    while (!scanner.eof() && !out_file.eof()) {
        getline(out_file, line);
        try {
            auto token = scanner.next_token();
            if (token.to_string() != line) {
                std::cout << "FAILED"
                          << "\n\tExpected: \t" << line << ";" << std::endl
                          << "\tTaken:\t\t" << token.to_string() << ";"
                          << std::endl;
                success = false;
            }
        } catch (const scanner::ScannerException& ex) {
            std::string lexer_message = "Exception: ";
            lexer_message += ex.what();
            if (line != lexer_message) {
                std::cout << "FAILED"
                          << "\n\tExpected: \t" << line << ";" << std::endl
                          << "\tTaken:\t\t" << lexer_message << ";"
                          << std::endl;
            }
            return (line == lexer_message);
        }
    }
    while (!out_file.eof()) {
        success = false;
        std::cout
            << "FAILED: Stream of scanner ended before the stream of out file"
            << std::endl;

        getline(out_file, line);
        std::cout << "\t" << line << std::endl;
    }

    while (!scanner.eof()) {
        success = false;
        std::cout
            << "FAILED: Stream of out file ended before the stream of scanner"
            << std::endl;

        auto token = scanner.next_token();
        std::cout << "\t" << token << std::endl;
    }
    return success;
}
}  // namespace testers