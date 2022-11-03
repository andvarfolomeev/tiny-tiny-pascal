#include "simple_parser.h"

void run_test_parser() {
    std::ifstream stream("../tests/simple_parser/2.in");
    scanner::Scanner scanner(stream);
    simpleparser::SimpleParser simple_parser(scanner);
    auto expression = simple_parser.parse_expression();

    std::cout << expression->calc();
}
