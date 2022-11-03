#include "basic_tester.h"

#include <filesystem>
#include <fstream>
#include <iostream>

std::set<std::string> testers::BasicTester::get_paths_to_test_files() {
    std::set<std::string> result;

    for (const auto& file_path :
         std::filesystem::recursive_directory_iterator(tests_path)) {
        std::string file_name = file_path.path().filename().string();
        size_t index_of_separator = file_name.find_last_of('.');
        std::string file_name_without_ext =
            file_name.substr(0, index_of_separator);
        result.insert(tests_path + file_name_without_ext);
    }

    return result;
}

bool testers::BasicTester::run_test(const std::string& input_file_path,
                                    const std::string& output_file_path) {
    return false;
}
testers::TestReport testers::BasicTester::run_tests() {
    TestReport report;
    auto tests_file_paths = get_paths_to_test_files();

    for (const auto& test_file_path : tests_file_paths) {
        auto input_file_path = test_file_path + ".in";
        auto output_file_path = test_file_path + ".out";

        if (run_test(input_file_path, output_file_path)) {
            std::cout << "OK\n";
            report.inc_success();
        } else {
            std::cout << "OK\n";
            report.inc_failed();
        }
    }
    return report;
}

std::string testers::BasicTester::read_file(std::string_view path) {
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
