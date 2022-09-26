#include <parsing/Lexer.h>
#include <parsing/Parser.h>
#include <runtime/Interpreter.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

static bool read_file(std::string filename, std::string& out) {
    std::ifstream stream(filename);
    if (!stream.is_open()) return false;
    std::stringstream buffer;
    buffer << stream.rdbuf();
    out = buffer.str();
    return true;
}

static void show_help() {
    std::cout << "Welcome to the Bonzo regex programming language" << std::endl;
    std::cout << "bonzo [OPTION]... [FILE] [OPTION]..." << std::endl;
    std::cout << "Options and arguments:" << std::endl;
    std::cout << "  -h, --help:            show help message" << std::endl;
    std::cout << "  -i, --input-file=FILE: make match statements match against the input file" << std::endl;
}

int main(int argc, char* argv[]) {
    bool has_filename { false };
    bool has_match_against_filename { false };
    std::string filename, match_against_filename;

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-h" || arg == "--help") {
            show_help();
            return 0;
        }
        if (arg.starts_with("-i=") || arg.starts_with("--input-file=")) {
            auto filename_start_index = arg.find_first_of('=') + 1;
            match_against_filename = arg.substr(filename_start_index);
            has_match_against_filename = true;
            continue;
        }
        if (has_filename) {
            std::cerr << "Redefinition of filename '" << filename << "\'" << std::endl;
            return 1;
        } else {
            filename = arg;
            has_filename = true;
        }
    }

    if (!has_filename) {
        show_help();
        return 1;
    }

    std::string stream;
    if (!read_file(filename, stream)) {
        std::cerr << "Failed to open file '" << filename << "'" << std::endl;
        return 1;
    }

    std::string match_against_stream;
    if (has_match_against_filename) {
        if (!read_file(match_against_filename, match_against_stream)) {
            std::cerr << "Failed to open file '" << match_against_filename << "'" << std::endl;
            return 1;
        }
    }

    Parser parser(stream);
    // If an error was returned
    if (parser.parse_all().is_error()) {
        parser.show_error();
        return 1;
    }
    if (has_match_against_filename) {
        // Match against a filename
        Interpreter interpreter(parser.instructions(), std::string(match_against_stream));
        if (interpreter.run().is_error()) {
            interpreter.show_error();
            return 1;
        }
    } else {
        // Generate regex
        Interpreter interpreter(parser.instructions());
        if (interpreter.run().is_error()) {
            interpreter.show_error();
            return 1;
        }
    }
    return 0;
}
