#ifndef SCANNER_TESTER_H
#define SCANNER_TESTER_H

#include "basic_tester.h"

namespace testers {
class ScannerTester : public BasicTester {
   public:
    explicit ScannerTester(const std::string &tests_path)
        : BasicTester(tests_path) {}

   private:
    bool run_test(const std::string &input_file_path,
                  const std::string &output_file_path) override;
};
}  // namespace testers

#endif  // SCANNER_TESTER_H
