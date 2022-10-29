#include "lexer.h"

int main() { TestReport report;

    report += run_lexer_tests();

    if (report.get_failed() != 0) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}