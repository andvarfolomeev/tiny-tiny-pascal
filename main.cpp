#include <iostream>

#include "tests/lexer.h"
#include "tests/test_report.h"

int main() {
  TestReport test_report = run_lexer_tests();
  std::cout << test_report;

  return 0;
}