#pragma once

#include <string>

class TextPosition {
public:
    TextPosition(std::string* stream, size_t index, size_t line, size_t column) 
        : m_stream(stream), m_index(index), m_line(line), m_column(column) { }
    TextPosition() : m_valid(false) { }
    ~TextPosition() { }

    bool valid() { return m_valid; }
    std::string* stream() const { return m_stream; }
    size_t index() const { return m_index; }
    size_t line() const { return m_line; }
    size_t column() const { return m_column; }

    std::string to_string() { return "line " + std::to_string(line()) + " column " + std::to_string(column()); }

    TextPosition& operator=(const TextPosition& state) = default;
    bool operator==(TextPosition position) {
        return position.stream() == stream() && position.index() == index();
    }

private:
    bool m_valid { true };
    std::string* m_stream { nullptr };
    size_t m_index { 0 }, m_line { 1 }, m_column { 1 };
};
