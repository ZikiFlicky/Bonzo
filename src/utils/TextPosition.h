#pragma once

#include <string>
#include <cassert>

#include <iostream>

class TextPosition {
public:
    TextPosition(std::string* stream, size_t index, size_t line, size_t column)
        : m_stream(stream), m_index(index), m_line(line), m_column(column) { }
    TextPosition() : m_valid(false) { }

    bool valid() const { return m_valid; }
    std::string* stream() const { return m_stream; }
    size_t index() const { return m_index; }
    size_t line() const { return m_line; }
    size_t column() const { return m_column; }

    // Are the two positions equal, depending on the attributes that the user cares about
    bool similar_to(const TextPosition& position) {
        return valid() && position.valid() && line() == position.line() && column() == position.column();
    }

    TextPosition previous_position() {
        assert(valid() && index() > 0);
        size_t idx = index() - 1;
        size_t ln, col;
        if ((*stream())[idx] == '\n') {
            ln = line() - 1;
            col = 1;
            // Calculate column by looping back until the start of the file or the last occurrence of a newline
            while (col < idx && (*stream())[idx - col] != '\n')
                ++col;
        } else {
            ln = line();
            col = column() - 1;
        }
        return { stream(), idx, ln, col };
    }

    std::string to_string() { return "[" + std::to_string(line()) + "," + std::to_string(column()) + "]"; }

    bool operator==(const TextPosition& position) const {
        return position.stream() == stream() && position.index() == index();
    }

private:
    bool m_valid { true };
    std::string* m_stream; // FIXME: This should be a reference
    size_t m_index { 0 }, m_line { 1 }, m_column { 1 };
};
