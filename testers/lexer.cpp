#include "lexer.h"

#include <filesystem>
#include <iostream>
#include <set>

#include "../src/scanner/scanner.h"
#include "test_report.h"

namespace fs = std::filesystem;

std::set<std::string> get_paths_to_text_files() {
    std::set<std::string> result;

    for (const auto &file_path : fs::recursive_directory_iterator(test_path)) {
        std::string file_name = file_path.path().filename().string();
        size_t index_of_separator = file_name.find_last_of('.');
        std::string file_name_without_ext =
            file_name.substr(0, index_of_separator);
        result.insert(test_path + file_name_without_ext);
    }

    return result;
}

TestReport run_lexer_tests() {
    TestReport report;
    std::set<std::string> tests_file_paths = get_paths_to_text_files();

    for (const auto &test_file_path : tests_file_paths) {
        auto input_file_path = test_file_path + ".in";
        auto output_file_path = test_file_path + ".out";

        if (run_test(input_file_path, output_file_path)) {
            std::cout << "OK\n";
            report.inc_success();
        } else {
            report.inc_failed();
        }
    }
    return report;
}

bool run_test(const std::string &input_file_path,
              const std::string &output_file_path) {
    int success = true;
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
            } catch (const scanner::ScannerException &ex) {
                std::string lexer_message = "Exception: ";
                lexer_message += ex.what();
                out_file_new << lexer_message;
                return EXIT_SUCCESS;
            }
        }
        return EXIT_SUCCESS;
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
        } catch (const scanner::ScannerException &ex) {
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