#include "test_report.h"

namespace tester {
TestReport::TestReport() : success(0), failed(0) {}

void TestReport::inc_success() { ++success; }

void TestReport::inc_failed() { ++failed; }

unsigned int TestReport::get_success() const { return success; }

unsigned int TestReport::get_failed() const { return failed; }

TestReport &TestReport::operator+=(const TestReport &report) {
    success += report.success;
    failed += report.failed;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const TestReport &report) {
    os << "Success: " << report.get_success()
       << "; Failed: " << report.get_failed();
    return os;
}
}  // namespace tester