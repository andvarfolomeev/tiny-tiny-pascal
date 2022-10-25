#include "test_report.h"

TestReport::TestReport() : success(0), failed(0) {}

void TestReport::inc_success() { ++this->success; }

void TestReport::inc_failed() { ++this->failed; }

unsigned int TestReport::get_success() { return this->success; }

unsigned int TestReport::get_failed() { return this->failed; }

TestReport &TestReport::operator+=(const TestReport &report) {
  this->success += report.success;
  this->failed += report.failed;
  return *this;
}

std::ostream &operator<<(std::ostream &os, const TestReport &report) {
  os << "Success: " << report.success << "; Failed: " << report.failed;
  return os;
}
