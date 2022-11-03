#ifndef TESTS_TEST_REPORT_H_
#define TESTS_TEST_REPORT_H_

#include <sstream>

namespace testers {
class TestReport {
    unsigned int success, failed;

   public:
    TestReport();
    void inc_success();
    void inc_failed();
    [[nodiscard]] unsigned int get_success() const;
    [[nodiscard]] unsigned int get_failed() const;
    TestReport &operator+=(const TestReport &report);
    friend std::ostream &operator<<(std::ostream &os, const TestReport &report);
};
}  // namespace testers

#endif  // TESTS_TEST_REPORT_H_