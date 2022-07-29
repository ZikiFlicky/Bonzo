#include <parsing/Lexer.h>
#include <parsing/Parser.h>
#include <runtime/Interpreter.h>

#include <iostream>
#include <fstream>

static bool read_file(std::string filename, std::string* out) {
    std::ifstream file(filename);
    if (!file.is_open())
        return false;
    file.seekg(0, std::ios_base::seekdir::_S_end);
    auto length = file.tellg();
    file.seekg(0);
    char* dump = new char[length];
    file.read(dump, length);
    *out = std::string(dump, length);
    delete[] dump;
    file.close();
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Expected filename as argument" << std::endl;
        return 1;
    }
    std::string stream;
    if (!read_file(argv[1], &stream)) {
        std::cerr << "Failed to open file '" << argv[1] << "'" << std::endl;
        return 1;
    }
    Lexer lexer(stream);
    Parser parser(lexer);
    // If an error was returned
    if (parser.parse_all().is_error()) {
        parser.show_error();
        return 1;
    }
    std::string test_text = "Test text";
    Interpreter interpreter(parser.instructions(), test_text);
    if (interpreter.run().is_error()) {
        interpreter.show_error();
        return 1;
    }

    return 0;
}
