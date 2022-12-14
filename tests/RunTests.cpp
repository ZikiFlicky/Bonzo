#include "Interface.h"

static void show_help() {
    std::cout << "Bonzo Test suite" << std::endl;
    std::cout << "Usage: ./runtests [--test TEST_NUMBER]" << std::endl;
    std::cout << "Options and arguments:" << std::endl;
    std::cout << "  --help:             show this help message" << std::endl;
    std::cout << "  --test TEST_NUMBER: run a specific test" << std::endl;
}

static bool string_to_positive_number(char* string, size_t& number) {
    char* number_end;
    long result = std::strtol(string, &number_end, 10);
    assert(number_end);
    // If number_end is not the end of the string
    // or if it's the start of the string, then the string is invalid
    if (number_end != string && *number_end == '\0' && result > 0) {
        number = result;
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    size_t test_to_run = 0; // 0 is default and just means we run all of the files
    if (argc > 1) {
        std::string arg1 = argv[1];
        if (arg1 == "--help") {
            show_help();
            return 0;
        } else if (arg1 == "--test") {
            if (argc > 2) {
                auto arg2 = argv[2];
                if (!string_to_positive_number(arg2, test_to_run)) {
                    std::cout << "Invalid 2nd argument, expected unsigned integer > 0" << std::endl;
                    return 1;
                }
            } else {
                std::cout << "Expected number after '--test' flag" << std::endl;
                return 1;
            }
        }
    }

    TestInterface i;

    // Basic
    i.expect_output("", "");
    i.expect_output("\n", "");
    i.expect_output("\n\n", "");
    i.expect_output("match ''", "Generated: \n");
    i.expect_output(" match ''", "Generated: \n");
    i.expect_output("\tmatch ''", "Generated: \n");
    i.expect_output("match 'string'", "Generated: string\n");
    i.expect_output("\nmatch ''\n", "Generated: \n");
    i.expect_output("match ''\n", "Generated: \n");
    i.expect_output("match ''\n\nmatch ''", "Generated: \nGenerated: \n");
    i.expect_parse_error("Match ''", { "unexpected token", { nullptr, 0, 1, 1 } });
    i.expect_output("a", "ParsingError [1,1]: unexpected token\n");
    i.expect_parse_error("a", { "unexpected token", { nullptr, 0, 1, 1 } });
    i.expect_parse_error("match '", { "unexpected newline", { nullptr, 0, 1, 8 } });
    i.expect_parse_error("match '\n", { "unexpected newline", { nullptr, 0, 1, 8 } });

    // Rules
    i.expect_output("rule var { 'value' }", "");
    i.expect_output("rule var_name { 'value' }", "");
    i.expect_output("rule var { 'value' }\nmatch var", "Generated: value\n");
    i.expect_parse_error("rule var { 'value' 'value2' }\nmatch var", { "expected '}'", { nullptr, 0, 1, 19 } });
    i.expect_parse_error("rule var ", { "expected '{'", { nullptr, 0, 1, 10 } });
    i.expect_parse_error("rule var { ", { "expected expression", { nullptr, 0, 1, 12 } });
    i.expect_parse_error("rule var { }", { "expected expression", { nullptr, 0, 1, 11 } });
    i.expect_parse_error("rule var { 'value'", { "expected '}'", { nullptr, 0, 1, 19 } });

    // Funcs
    i.expect_output("func f [ ] { 'value' }", "");
    i.expect_output("func f [ ] { 'value' }\nmatch f { }", "Generated: value\n");
    i.expect_output("func f [ ] { 'value' }\nmatch f { }", "Generated: value\n");
    i.expect_runtime_error("func f [ ] { 'value' }\nmatch f", "", { "input not generable/matchable", { nullptr, 0, 2, 1 }, { } });
    i.expect_output("func f [ param ] { param + '1234' }\nmatch f { 'string' }", "Generated: string1234\n");
    i.expect_output("func add [ param1 param2 ] { param1 + param2 }\nmatch add { 'string' '123' }", "Generated: string123\n");
    i.expect_runtime_error("func add [ param1 param2 ] { param1 + param2 }\nmatch add { 'string' }", "",
        { "expected 2 arguments but got 1 instead", { nullptr, 0, 2, 6 }, { { { nullptr, 0, 2, 6 }, "add" } } });
    i.expect_parse_error("func \n", { "expected identifier", { nullptr, 0, 1, 6 } });
    i.expect_parse_error("func f \n", { "expected '['", { nullptr, 0, 1, 7 } });
    i.expect_parse_error("func f [ \n", { "expected identifier or ']'", { nullptr, 0, 1, 10 } });
    i.expect_parse_error("func f [ param \n", { "expected identifier or ']'", { nullptr, 0, 1, 16 } });
    i.expect_parse_error("func f [ param ] \n", { "expected '{'", { nullptr, 0, 1, 18 } });
    i.expect_parse_error("func f [ param ] { \n", { "expected expression", { nullptr, 0, 1, 20 } });
    i.expect_parse_error("func f [ param ] { param + '1234' 'another string' \n", { "expected '}'", { nullptr, 0, 1, 35 } });
    i.expect_parse_error("func f [ param ] { param + '1234' \n", { "expected '}'", { nullptr, 0, 1, 34 } });

    // Match
    i.expect_parse_error("match ", { "expected expression", { nullptr, 0, 1, 7 } });
    i.expect_parse_error("match 'string' 'another string'", { "expected newline", { nullptr, 0, 1, 16 } });

    // Addition
    i.expect_search_find("'1' + '2'", "12", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 2 } } } );
    i.expect_search_find("'1' + '2'", "121212", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 2 } }, { { nullptr, 0, 1, 3 }, { nullptr, 0, 1, 4 } }, { { nullptr, 0, 1, 5 }, { nullptr, 0, 1, 6 } } } );
    i.expect_search_find("'1' + '2' + '3'", " 123", { { { nullptr, 0, 1, 2 }, { nullptr, 0, 1, 4 } } } );

    // Piping
    i.expect_search_find("'1' | '2'", "1", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } } );
    i.expect_search_find("'1' | '2'", "2", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } } );
    i.expect_search_find("'1' | '2' | '3'", "3", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } } );

    // Special characters
    i.expect_search_find("Spacing", " ", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } } );
    i.expect_search_find("Spacing", "\t", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } } );
    i.expect_search_find("Character", "a", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } } );
    i.expect_search_find("Character", "A", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } } );
    i.expect_search_find("Character", "z", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } } );
    i.expect_search_find("Character", "Z", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } } );
    i.expect_search_find("Character", " ", { });
    i.expect_search_find("Newline", "\n", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } });

    // ArbitraryLength
    i.expect_search_find("ArbitraryLength { '1' }", "", { });
    i.expect_search_find("ArbitraryLength { '1' }", "1", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } });
    i.expect_search_find("ArbitraryLength { '1' }", "111", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 3 } } });
    i.expect_search_find("ArbitraryLength { '11' }", "111", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 2 } } });
    i.expect_search_find("ArbitraryLength { '12' }", "12121", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 4 } } });
    i.expect_search_find("'1' + ArbitraryLength { '1' }", "1", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } });
    i.expect_search_find("ArbitraryLength { '1' } + '1'", "1", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } });

    // Some
    i.expect_search_find("Some { '1' }", "", { });
    i.expect_search_find("'1' + Some { '1' }", "1", { });
    i.expect_search_find("'1' + Some { '12' }", "11212", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 5 } } });
    i.expect_search_find("Some { '12' } + '1'", "12121", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 5 } } });

    // Any
    i.expect_search_find("Any { [ '1' '2' '32' ] }", "1", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } });
    i.expect_search_find("Any { [ '1' '2' '32' ] }", "2", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } } });
    i.expect_search_find("Any { [ '1' '2' '32' ] }", "32", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 2 } } });

    // Separated
    i.expect_search_find("Separated { [ ] Some { Spacing } }", " Stuff ", { });
    i.expect_search_find("Separated { [ 'This' 'is' 'a' 'sentence' ] Some { Spacing } }", " This is a\t sentence", { { { nullptr, 0, 1, 2 }, { nullptr, 0, 1, 20 } } });

    // Map
    i.expect_search_find("Separated { Map { Some [ '+' '-' ] } '' }", "+++--", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 5 } } });

    // NoCase
    i.expect_search_find("NoCase { 'AbAb' }", "abAB", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 4 } } });

    // Optional
    i.expect_search_find("Optional { '123' } + '3'", "3", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 1 } }});
    i.expect_search_find("Optional { '123' } + '3'", "1233", { { { nullptr, 0, 1, 1 }, { nullptr, 0, 1, 4 } }});

    i.run(test_to_run);
    return 0;
}
