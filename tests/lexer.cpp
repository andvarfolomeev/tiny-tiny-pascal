#include <filesystem>
#include <iostream>
#include <set>

#include "../lexer/scanner.hpp"
#include "lexer.h"

namespace fs = std::filesystem;

std::set<std::string> get_paths_to_text_files() {
  std::set<std::string> result;

  for (const auto &file_path : fs::recursive_directory_iterator(test_path)) {
    std::string file_name = file_path.path().filename().string();
    size_t index_of_separator = file_name.find_last_of('.');
    std::string file_name_without_ext = file_name.substr(0, index_of_separator);
    result.insert(test_path + file_name_without_ext);
  }

  return result;
}

int run_lexer_tests() {
  int status_code = 0;
  std::set<std::string> tests_file_paths = get_paths_to_text_files();

  for (const auto &test_file_path : tests_file_paths) {
    auto path_of_in_file = test_file_path + ".in";
    auto path_of_out_file = test_file_path + ".out";

    std::ifstream out_file;
    out_file.open(path_of_out_file);

    std::ifstream in_file;
    in_file.open(path_of_in_file);
    std::cout << path_of_in_file << "\t" << path_of_out_file << "\n";
    lexer::Scanner scanner(in_file);

    std::string line;
    while (!scanner.eof() && !out_file.eof()) {
      getline(out_file, line);
      auto token = scanner.next_token();
      if (token.to_string() == line) {
        std::cout << "OK"
                  << "\t" << line << std::endl;
      } else {
        std::cout << "FAILED" << std::endl;
        std::cout << "\t"
                  << "Expected: \t" << line << ";" << std::endl
                  << "\tTaken:\t\t" << token.to_string() << ";" << std::endl;
        status_code = 1;
      }
    }
    while (!out_file.eof()) {
      std::cout << "FAILED: Stream of lexer ended before the stream of out file"
                << std::endl;

      getline(out_file, line);
      std::cout << "\t" << line << std::endl;
    }
    while (!scanner.eof()) {
      std::cout << "FAILED: Stream of out file ended before the stream of lexer"
                << std::endl;

      auto token = scanner.next_token();
      std::cout << "\t" << token << std::endl;
    }
  }
  return status_code;
}
