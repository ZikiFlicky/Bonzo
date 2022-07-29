#pragma once

#include <cstddef>
#include <string>

class State {
public:
    State(std::string* stream, size_t index, size_t line, size_t column) 
        : m_stream(stream), m_index(index), m_line(line), m_column(column) { }
    State() { }
    ~State() { }

    std::string* stream() const { return m_stream; }
    size_t index() const { return m_index; }
    size_t line() const { return m_line; }
    size_t column() const { return m_column; }

    State& operator=(const State& state) = default;

private:
    std::string* m_stream { nullptr };
    size_t m_index { 0 }, m_line { 1 }, m_column { 1 };
};
