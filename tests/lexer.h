#ifndef TESTS_LEXER_H_
#define TESTS_LEXER_H_

#include <iostream>

#include "test_report.h"

const std::string test_path = "../tests/lexer/";

TestReport run_lexer_tests();

bool run_test(const std::string &input_file_path,
              const std::string &output_file_path);

#endif  // TESTS_LEXER_H_