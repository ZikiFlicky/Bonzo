#pragma once

#include <runtime/Interpreter.h>
#include <ast/exprs/abstract/Expr.h>
#include <parsing/Lexer.h>


class TestInterface {
public:
    TestInterface() { }

    void expect_output(std::string stream, std::string expect);
    void expect_parse_error(std::string stream, ParsingError error);
    void expect_runtime_error(std::string stream, std::string expect, RuntimeError error);

    void run(size_t test);

private:
    enum TestType {
        TestOutput = 1,
        TestParseError,
        TestRuntimeError
    };

    struct TestDef {
        TestDef(std::string code, std::string expected)
            : type(TestOutput), code(code), expected(expected) { }
        TestDef(std::string code, ParsingError error)
            : type(TestParseError), code(code), expected(""), parse_error(error) { }
        TestDef(std::string code, std::string expected, RuntimeError error)
            : type(TestRuntimeError), code(code), expected(expected), runtime_error(error) { }
        TestDef(const TestDef& def) {
            type = def.type;
            switch (type) {
            case TestOutput:
                code = def.code;
                expected = def.expected;
                break;
            case TestParseError:
                code = def.code;
                parse_error = def.parse_error;
                break;
            case TestRuntimeError:
                code = def.code;
                expected = def.expected;
                runtime_error = def.runtime_error;
                break;
            default:
                assert(0);
            }
        }
        ~TestDef() { }

        TestType type;
        std::string code { };
        std::string expected { };
        union {
            ParsingError parse_error;
            RuntimeError runtime_error;
        };
    };
    std::vector<TestDef> m_test_defs;
    std::vector<size_t> m_failed_tests;
    size_t m_test_number, m_run_mode { 0 };

    size_t amount_tests() { return m_test_defs.size(); }
    size_t amount_fail() { return m_failed_tests.size(); }
    size_t amount_succeed() { return amount_tests() - amount_fail(); }

    void show_test_success();
    void show_test_no_match(std::string output, std::string expect);
    void show_test_no_match_parse_error(ParsingError error, ParsingError expected);
    void show_test_no_match_parse_error(std::string output, ParsingError expected);
    void show_test_no_match_runtime_error(ParsingError parse_error, std::string expected_output, RuntimeError expected_error);
    void show_test_no_match_runtime_error(std::string output, std::string expected_output, RuntimeError expected_error);
    void show_test_no_match_runtime_error(std::string output, RuntimeError error, std::string expected_output, RuntimeError expected_error);

    void run_output_test(std::string stream, std::string expect);
    void run_parse_error_test(std::string stream, ParsingError expect);
    void run_runtime_error_test(std::string stream, std::string expect_output, RuntimeError expect_error);

    void run_test(TestDef& def);
};
