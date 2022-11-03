#include <iostream>

#include "scanner_tester.h"
#include "simple_parser_tester.h"
#include "test_report.h"

int main() {
    testers::TestReport report;

    std::string tests_dir_path = "../tests/";

    testers::ScannerTester scanner_tester(tests_dir_path + "scanner/");
    testers::SimpleParserTester simple_parser_tester(tests_dir_path +
                                                     "simple_parser/");

    report += scanner_tester.run_tests();
    report += simple_parser_tester.run_tests();

    std::cout << report << "\n";
    if (report.get_failed() != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}