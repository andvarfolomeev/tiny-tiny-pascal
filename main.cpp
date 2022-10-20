#include <fstream>
#include <iostream>
#include <regex>

#include "./lexer/scanner.hpp"
#include "./lexer/tokens/token.hpp"

int main() {
  lexer::Token token(1, 1, "Integer", "17", "$11");
  std::cout << token << std::endl;
  std::ifstream in("filename.ext", std::ios_base::in);
  lexer::Scanner scanner(in);
  scanner.start();
  return 0;
}
