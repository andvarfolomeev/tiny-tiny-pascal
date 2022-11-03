#include "lexer.h"
#include "test_report.h"

int main() {
    TestReport report;
    report += run_lexer_tests();

    std::cout << report << "\n";
    if (report.get_failed() != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}