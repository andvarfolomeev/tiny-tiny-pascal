#ifndef TESTS_TEST_REPORT_H_
#define TESTS_TEST_REPORT_H_

#include <sstream>

class TestReport {
  unsigned int success, failed;

public:
  TestReport();

  void inc_success();
  void inc_failed();

  unsigned int get_success();
  unsigned int get_failed();

  TestReport &operator+=(const TestReport &report);
  friend std::ostream &operator<<(std::ostream &os, const TestReport &report);
};

#endif // TESTS_TEST_REPORT_H_