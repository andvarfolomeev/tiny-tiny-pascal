#include "tester.h"

int main() {
    auto res = tester::ScannerTester("../tests/scanner/").run_tests();
    res += tester::SimpleParserTester("../tests/simple_parser/").run_tests();
    std::cout << res;
    return EXIT_SUCCESS;
}