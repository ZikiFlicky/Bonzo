#pragma once

#include <string>
#include <cassert>

#include <iostream>

class TextPosition {
public:
    TextPosition(std::string stream, size_t index, size_t line, size_t column)
        : m_stream(stream), m_index(index), m_line(line), m_column(column) { }
    TextPosition() : m_valid(false) { }
    ~TextPosition() { }

    bool valid() { return m_valid; }
    const std::string& stream() const { return m_stream; }
    size_t index() const { return m_index; }
    size_t line() const { return m_line; }
    size_t column() const { return m_column; }

    TextPosition previous_position() {
        assert(valid() && index() > 0);
        size_t idx = index() - 1;
        size_t ln, col;
        if (stream()[idx] == '\n') {
            ln = line() - 1;
            col = 1;
            // Calculate column by looping back until the start of the file or the last occurrence of a newline
            while (col < idx && stream()[idx - col] != '\n')
                ++col;
        } else {
            ln = line();
            col = column() - 1;
        }
        return { stream(), idx, ln, col };
    }

    std::string to_string() { return "[" + std::to_string(line()) + "," + std::to_string(column()) + "]"; }

    TextPosition& operator=(const TextPosition& state) = default;
    bool operator==(TextPosition position) {
        return position.stream() == stream() && position.index() == index();
    }

private:
    bool m_valid { true };
    std::string m_stream;
    size_t m_index { 0 }, m_line { 1 }, m_column { 1 };
};
