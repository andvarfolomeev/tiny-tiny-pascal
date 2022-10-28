#include <algorithm>
#include <iostream>

#include "lexer/scanner.h"
#include "lexer/token_type.h"
#include "tests/lexer.h"
#include "tests/test_report.h"

/**
 * Get value of parameter. Check exist of parameter before use get_param
 * @param begin begin of array
 * @param end enf of array
 * @return value of parameter
 */
std::string get_param(char **begin, char **end, const std::string &option) {
  char **itr = std::find(begin, end, option);
  if (itr == end || ++itr == end) {
    throw std::runtime_error("Parameter doesnt exist");
  }
  std::string result(*itr);
  return result;
}

/**
 *
 * @param begin begin of array
 * @param end enf of array
 * @param option name of parameter
 * @return is parameter exist in array
 */
bool check_param(char **begin, char **end, const std::string &option) {
  return std::find(begin, end, option) != end;
}

/**
 * Print how use program
 */
void print_usage() {
  std::cout << "Usage: tiny_tiny_compiler.exe [options]\n"
            << "Options:\n"
            << "\t --tests - run tests\n"
            << "\t --file [file path]";
}

int main(int argc, char *argv[]) {
  // Only one parameter - path to execute file
  if (argc == 1) {
    print_usage();
  }

  if (check_param(argv, argv + argc, "--tests")) {
    TestReport test_report = run_lexer_tests();
    std::cout << test_report;
    return test_report.get_failed() != 0;
  } else if (check_param(argv, argv + argc, "--file")) {
    auto filename = get_param(argv, argv + argc, "--file");
    std::ifstream file(filename);

    if (!file.good()) {
      std::cout << "Error: file " << filename << " doesnt exist "
                << "\n";
      return EXIT_FAILURE;
    } else {
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
    }
  }

  return EXIT_SUCCESS;
}