#include <algorithm>
#include <iostream>

#include "lexer/scanner.h"
#include "tests/lexer.h"
#include "tests/test_report.h"

char *get_param(char **begin, char **end, const std::string &option) {
  char **itr = std::find(begin, end, option);
  if (itr != end && ++itr != end) {
    return *itr;
  }
  return nullptr;
}

bool check_param(char **begin, char **end, const std::string &option) {
  return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[]) {
  if (check_param(argv, argv + argc, "--tests")) {
    TestReport test_report = run_lexer_tests();
    std::cout << test_report;
    return test_report.get_failed() != 0;
  }

  auto filename = get_param(argv, argv + argc, "--file");

  if (filename) {
    std::ifstream file(filename);
    if (file.good()) {
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
      } catch (const lexer::ScannerException &ex) {
        std::cout << ex.what();
        return EXIT_FAILURE;
      }
    } else {
      std::cout << "Error: file " << filename << " doesnt exist "
                << "\n";
      return EXIT_FAILURE;
    }
  }

  std::cout << "Usage: tiny_tiny_compiler.exe [options]\n"
            << "Options:\n"
            << "\t --tests - run tests\n"
            << "\t --file [file path]";

  return EXIT_SUCCESS;
}