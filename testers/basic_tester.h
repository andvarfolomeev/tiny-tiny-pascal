#ifndef BASIC_TESTER_H
#define BASIC_TESTER_H

#include <set>

#include "test_report.h"

namespace testers {
class BasicTester {
   private:
    std::string tests_path;

   public:
    explicit BasicTester(const std::string &tests_path)
        : tests_path(tests_path) {}
    TestReport run_tests();

   protected:
    std::set<std::string> get_paths_to_test_files();
    virtual bool run_test(const std::string &input_file_path,
                          const std::string &output_file_path);

    static std::string read_file(std::string_view path);
};
}  // namespace testers

#endif  // BASIC_TESTER_H
