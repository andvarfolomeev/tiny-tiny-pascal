#include "tester.h"

#include <filesystem>
#include <fstream>

#include "../src/scanner/scanner.h"
#include "../src/simple_parser/simple_parser.h"
#include "../src/parser/parser.h"

namespace tester {

TestReport Tester::run_tests() {
    TestReport report;
    auto tests_file_paths = get_path_to_files();

    for (const auto& test_file_path : tests_file_paths) {
        auto input_file_path = test_file_path + ".in";
        auto output_file_path = test_file_path + ".out";

        std::cout << input_file_path << std::endl;

        if (!file_exist(output_file_path)) {
            std::ofstream output_stream(output_file_path);
            try {
                auto answer = get_answer(input_file_path);
                output_stream << answer;
            } catch (TinyPascalException& ex) {
                output_stream << ex.what();
            }
            continue;
        }

        auto answer_in_out = read_file(output_file_path);

        try {
            auto answer = get_answer(input_file_path);
            if (answer == answer_in_out) {
                report.inc_success();
            } else {
                report.inc_failed();
                std::cout << "\tExpected:" << std::endl;
                std::cout << "\t" << answer_in_out << std::endl;
                std::cout << "\tTaken: " << std::endl;
                std::cout << "\t" << answer << std::endl;
            }
        } catch (TinyPascalException& ex) {
            if (answer_in_out == ex.what()) {
                report.inc_success();
            } else {
                report.inc_failed();
                std::cout << "\tExpected:" << std::endl;
                std::cout << "\t" << answer_in_out << std::endl;
                std::cout << "\tTaken: " << std::endl;
                std::cout << "\t" << ex.what() << std::endl;
            }
        }
    }
    return report;
}

std::set<std::string> Tester::get_path_to_files() {
    std::set<std::string> result;
    for (const auto& file_path :
         std::filesystem::recursive_directory_iterator(path_to_files)) {
        std::string file_name = file_path.path().filename().string();
        size_t index_of_separator = file_name.find_last_of('.');
        std::string file_name_without_ext =
            file_name.substr(0, index_of_separator);
        result.insert(path_to_files + file_name_without_ext);
    }
    return result;
}

std::string Tester::read_file(const std::string& path) {
    constexpr auto read_size = std::size_t(4096);
    auto stream = std::ifstream(path.data());
    stream.exceptions(std::ios_base::badbit);

    auto out = std::string();
    auto buf = std::string(read_size, '\0');
    while (stream.read(&buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}

bool Tester::file_exist(std::string& path) {
    std::ifstream f(path);
    return f.good();
}

std::string ScannerTester::get_answer(const std::string& path_in) {
    auto stream = std::ifstream(path_in);
    scanner::Scanner scanner(stream);

    std::stringstream answer;

    for (;;) {
        auto token = scanner.next_token();

        answer << token;

        if (token == scanner::TokenType::eof)
            break;
        else
            answer << std::endl;
    }
    return answer.str();
}

std::string SimpleParserTester::get_answer(const std::string& path_in) {
    auto stream = std::ifstream(path_in);
    scanner::Scanner scanner(stream);
    simpleparser::SimpleParser parser(scanner);
    std::stringstream answer;
    parser.parse_expression()->draw(answer, 0);
    return answer.str();
}

std::string ParserTester::get_answer(const std::string& path_in) {
    auto stream = std::ifstream(path_in);
    scanner::Scanner scanner(stream);
    parser::Parser parser(scanner);
    std::stringstream answer;
    parser.program()->draw(answer, 0);
    return answer.str();
}
}  // namespace tester
