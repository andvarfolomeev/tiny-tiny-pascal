#include <vector>

#include "tester.h"

int main() {
    auto res = tester::ScannerTester("../tests/scanner/").run_tests();
    res += tester::ParserTester("../tests/parser/").run_tests();
    res += tester::SemanticTester("../tests/semantic/").run_tests();
    std::cout << res;
    return EXIT_SUCCESS;
}
