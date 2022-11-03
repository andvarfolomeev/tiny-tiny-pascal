#ifndef SIMPLE_PARSER_TESTER_H
#define SIMPLE_PARSER_TESTER_H

#include "basic_tester.h"

namespace testers {
class SimpleParserTester : public BasicTester {
   public:
    explicit SimpleParserTester(const std::string &tests_path)
        : BasicTester(tests_path) {}

   private:
    bool run_test(const std::string &input_file_path,
                  const std::string &output_file_path) override;
};
}  // namespace testers

#endif  // SIMPLE_PARSER_TESTER_H
