#include "tests/lexer.h"

int main() {
  int status_code = 0;
  status_code |= run_lexer_tests();
  return status_code;
}