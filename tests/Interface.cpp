#include "Interface.h"
#include <parsing/Parser.h>

void TestInterface::run_output_test(std::string stream, std::string expect) {
    Parser parser(stream);
    std::string output;
    if (parser.parse_all().is_error()) {
        output = parser.error().to_string();
    } else {
        Interpreter interpreter;
        interpreter.set_buffered();
        interpreter.set_instructions(parser.instructions());
        interpreter.run();
        output = interpreter.output_buffer();
        if (interpreter.has_errored())
            output += interpreter.error().to_string();
    }
    if (output == expect)
        show_test_success();
    else
        show_test_no_match(output, expect);
}

void TestInterface::run_parse_error_test(std::string stream, ParsingError expect) {
    Parser parser(stream);
    if (parser.parse_all().is_error()) {
        // Checks if the error message will look the same
        if (parser.error().similar_to(expect))
            show_test_success();
        else
            show_test_no_match_parse_error(parser.error(), expect);
    } else {
        Interpreter interpreter;
        interpreter.set_buffered();
        interpreter.set_instructions(parser.instructions());
        interpreter.run();
        std::string output = interpreter.output_buffer();
        if (interpreter.has_errored())
            output += interpreter.error().to_string();
        show_test_no_match_parse_error(output, expect);
    }
}

void TestInterface::run_runtime_error_test(std::string stream, std::string expect_output, RuntimeError& expect_error) {
    Parser parser(stream);
    if (parser.parse_all().is_error()) {
        show_test_no_match_runtime_error(parser.error(), expect_output, expect_error);
    } else {
        // Create buffered interpreter and run it
        Interpreter interpreter;
        interpreter.set_buffered();
        interpreter.set_instructions(parser.instructions());
        interpreter.run();
        std::string output = interpreter.output_buffer();
        if (!interpreter.has_errored()) {
            show_test_no_match_runtime_error(output, expect_output, expect_error);
        } else {
            auto error = interpreter.error();
            if (output == expect_output && error.similar_to(expect_error)) {
                show_test_success();
            } else {
                show_test_no_match_runtime_error(output, error, expect_output, expect_error);
            }
        }
    }
}

void TestInterface::run_search_test(std::string stream, std::string match_against, std::vector<TextSnippet> expected_snippets) {
    Parser parser(stream, Parser::ParseType::ParseExpr);
    if (parser.parse_all().is_error()) {
        show_test_no_match_snippets(parser.error(), stream);
    } else {
        // Create buffered interpreter and run it
        Interpreter interpreter(match_against);
        interpreter.set_expr(parser.expr());
        interpreter.run();
        if (interpreter.has_errored()) {
            show_test_no_match_snippets(interpreter.error(), stream);
        } else {
            // TODO: Have new TextSnippet class which is just for matched snippets
            // Check if snippets match
            auto& found_snippets = interpreter.match_snippets();
            if (snippets_match(found_snippets, expected_snippets))
                show_test_success();
            else
                show_test_no_match_snippets(found_snippets, stream, match_against, expected_snippets);
        }
    }
}

void TestInterface::show_test_success() {
    std::cout << "Test " << m_test_number << "/" << amount_tests() << " passed" << std::endl;
}

void TestInterface::show_test_no_match(std::string output, std::string expect) {
    std::cout << "Test " << m_test_number << "/" << amount_tests() << " failed" << std::endl \
        << "expected:" << std::endl \
        << expect << std::endl \
        << "But got this instead:" << std::endl \
        << output << std::endl;
    m_failed_tests.push_back(m_test_number);
}

void TestInterface::show_test_no_match_parse_error(ParsingError error, ParsingError expected) {
    std::cout << "Test " << m_test_number << "/" << amount_tests() << " failed" << std::endl \
        << "expected:" << std::endl \
        << expected.to_string() << std::endl \
        << "But got this error instead:" << std::endl \
        << error.to_string() << std::endl;
    m_failed_tests.push_back(m_test_number);
}

void TestInterface::show_test_no_match_parse_error(std::string output, ParsingError expected) {
    std::cout << "Test " << m_test_number << "/" << amount_tests() << " failed" << std::endl \
        << "expected:" << std::endl \
        << expected.to_string() << std::endl \
        << "But got this instead:" << std::endl \
        << output << std::endl;
    m_failed_tests.push_back(m_test_number);
}

void TestInterface::show_test_no_match_runtime_error(ParsingError parse_error, std::string expected_output, RuntimeError expected_error) {
    std::cout << "Test " << m_test_number << "/" << amount_tests() << " failed" << std::endl \
        << "expected:" << std::endl \
        << expected_output << expected_error.to_string() << std::endl \
        << "But got parsing error instead:" << std::endl \
        << parse_error.to_string() << std::endl;
    m_failed_tests.push_back(m_test_number);
}

void TestInterface::show_test_no_match_runtime_error(std::string output, std::string expected_output, RuntimeError expected_error) {
    std::cout << "Test " << m_test_number << "/" << amount_tests() << " failed" << std::endl \
        << "expected:" << std::endl \
        << expected_output << expected_error.to_string() << std::endl \
        << "But didn't get runtime error:" << std::endl \
        << output << std::endl;
    m_failed_tests.push_back(m_test_number);
}

void TestInterface::show_test_no_match_runtime_error(std::string output, RuntimeError error, std::string expected_output, RuntimeError expected_error) {
    std::cout << "Test " << m_test_number << "/" << amount_tests() << " failed" << std::endl \
        << "expected:" << std::endl \
        << expected_output + expected_error.to_string() << std::endl \
        << "But didn't match:" << std::endl \
        << output << error.to_string() << std::endl;
    m_failed_tests.push_back(m_test_number);
}

void TestInterface::show_test_no_match_snippets(ParsingError error, std::string expression) {
    std::cout << "Test " << m_test_number << "/" << amount_tests() << " failed" << std::endl \
        << "Expression " << expression << std::endl \
        << "errored with:" << std::endl \
        << error.to_string() << std::endl;
    m_failed_tests.push_back(m_test_number);
}

void TestInterface::show_test_no_match_snippets(RuntimeError error, std::string expression) {
    std::cout << "Test " << m_test_number << "/" << amount_tests() << " failed" << std::endl \
        << "Expression " << expression << std::endl \
        << "errored with:" << std::endl \
        << error.to_string() << std::endl;
    m_failed_tests.push_back(m_test_number);
}

void TestInterface::show_test_no_match_snippets(std::vector<TextSnippet>& output, std::string expression, std::string match_against, std::vector<TextSnippet>& expected) {
    // Align end of snippets to their prev position to look like the the printed snippets
    std::vector<TextSnippet> aligned_output_snippets;
    for (auto& s : output)
        aligned_output_snippets.push_back({ s.start(), s.end().previous_position() });

    std::cout << "Test " << m_test_number << "/" << amount_tests() << " failed" << std::endl \
        << "Expression " << expression << std::endl \
        << "against:" << std::endl \
        << match_against << std::endl \
        << "expected matches:" << std::endl \
        << matches_to_string(expected) << std::endl \
        << "but got:" << std::endl \
        << matches_to_string(aligned_output_snippets) << std::endl;
    m_failed_tests.push_back(m_test_number);
}

void TestInterface::run_test(TestDef& def) {
    switch (def.type) {
    case TestOutput:
        run_output_test(def.code, def.expected);
        break;
    case TestParseError:
        run_parse_error_test(def.code, def.parse_error);
        break;
    case TestRuntimeError:
        run_runtime_error_test(def.code, def.expected, def.runtime_error);
        break;
    case TestMatch:
        run_search_test(def.code, def.match_against, def.expected_snippets);
        break;
    default:
        assert(0);
    }
}

void TestInterface::expect_output(std::string stream, std::string expect) {
    m_test_defs.push_back({ stream, expect });
}

void TestInterface::expect_parse_error(std::string stream, ParsingError error) {
    m_test_defs.push_back({ stream, error });
}

void TestInterface::expect_runtime_error(std::string stream, std::string expect, RuntimeError error) {
    m_test_defs.push_back({ stream, expect, error });
}

void TestInterface::expect_search_find(std::string stream, std::string match_against, std::vector<TextSnippet> snippets) {
    m_test_defs.push_back({ stream, match_against, snippets });
}

void TestInterface::run(size_t test) {
    m_failed_tests = {};
    m_test_number = 0;
    m_run_mode = test;
    if (m_run_mode == 0) {
        std::cout << "Running " << amount_tests() << " tests" << std::endl;
        for (auto& def : m_test_defs) {
            ++m_test_number;
            run_test(def);
        }
        std::cout << amount_succeed() << "/" << amount_tests() << " tests passed" << std::endl;
        if (amount_fail() > 0) {
            std::cout << "Failing tests: ";
            for (size_t i = 0; i < amount_fail(); ++i) {
                if (i > 0)
                    std::cout << ", ";
                std::cout << m_failed_tests[i];
            }
            std::cout << std::endl;
        }
    } else if (test > amount_tests()) {
        std::cout << "Invalid test number" << std::endl;
    } else {
        m_test_number = test;
        run_test(m_test_defs[test - 1]);
    }
}

std::string TestInterface::matches_to_string(std::vector<TextSnippet>& snippets) {
    std::string as_string { };
    for (size_t i = 0; i < snippets.size(); ++i) {
        if (i != 0)
            as_string += "\n";
        auto start = snippets[i].start();
        auto end = snippets[i].end();
        as_string += start.to_string() + " -> " + end.to_string();
    }
    return as_string;
}

bool TestInterface::snippets_match(std::vector<TextSnippet>& snippets, std::vector<TextSnippet>& expect) {
    if (snippets.size() != expect.size())
        return false;

    auto amount_snippets = snippets.size();
    for (size_t i = 0; i < amount_snippets; ++i) {
        // Realign to look like the expected_snippet
        TextSnippet realigned_found_snippet = { snippets[i].start(), snippets[i].end().previous_position() };
        assert(realigned_found_snippet.start().similar_to(expect[i].start()));
        if (!realigned_found_snippet.similar_to(expect[i]))
            return false;
    }

    return true;
}
