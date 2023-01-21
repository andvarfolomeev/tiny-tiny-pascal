#ifndef TESTER_H
#define TESTER_H

#include <iostream>
#include <set>
#include <utility>

#include "test_report.h"

namespace tester {
class Tester {
   public:
    explicit Tester(std::string path_to_files)
        : path_to_files(std::move(path_to_files)) {}

    TestReport run_tests();

   protected:
    std::string path_to_files;

    std::set<std::string> get_path_to_files();

    static std::string read_file(const std::string &path);

    static bool file_exist(std::string &path);

    virtual std::string get_answer(const std::string &path_in) = 0;
};

class ScannerTester : public Tester {
   public:
    explicit ScannerTester(std::string path_to_files)
        : Tester(std::move(path_to_files)) {}

   protected:
    std::string get_answer(const std::string &path_in) override;
};

class ParserTester : public Tester {
   public:
    explicit ParserTester(std::string path_to_files)
        : Tester(std::move(path_to_files)) {}

   protected:
    std::string get_answer(const std::string &path_in) override;
};

class SemanticTester : public Tester {
   public:
    explicit SemanticTester(std::string path_to_files)
        : Tester(std::move(path_to_files)) {}

   protected:
    std::string get_answer(const std::string &path_in) override;
};

class GeneratorTester : public Tester {
   public:
    explicit GeneratorTester(std::string path_to_files)
        : Tester(std::move(path_to_files)) {}

   protected:
    std::string get_answer(const std::string &path_in) override;
};
}  // namespace tester

#endif  // TESTER_H
