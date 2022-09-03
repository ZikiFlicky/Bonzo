#pragma once

#include <runtime/values/abstract/Value.h>

#include <string>
#include <vector>
#include <stack>
#include <iostream>

class TextPosition {
public:
    TextPosition(size_t line, size_t column, size_t index)
        : m_line(line), m_column(column), m_index(index) { }
    TextPosition() { }

    size_t line() { return m_line; }
    size_t column() { return m_column; }
    size_t index() { return m_index; }

    std::string to_string() { return "line " + std::to_string(line()) + " column " + std::to_string(column()); }

private:
    size_t m_line { 1 }, m_column { 1 }, m_index { 0 };
};

class TextSnippet {
public:
    TextSnippet(TextPosition start, TextPosition end)
        : m_start(start), m_end(end) { }

    TextPosition& start() { return m_start; }
    TextPosition& end() { return m_end; }
    size_t length();

private:
    TextPosition m_start, m_end;
};

class MatchHandler {
public:
    explicit MatchHandler(std::string string) : m_string(string) { }
    ~MatchHandler() { }

    TextPosition& position() { return m_position; }

    void set_position(TextPosition position) { m_position = position; }

    char get();

private:
    TextPosition m_position;
    std::string m_string;
};

class MatchState {
public:
    explicit MatchState(MatchHandler& matcher, std::shared_ptr<Value> value)
        : m_matcher(matcher), m_value(value), m_position(matcher.position()) { }
    ~MatchState() { }

    MatchHandler& matcher() { return m_matcher; }
    TextPosition position() { return m_position; }
    size_t index() { return m_index; }

    void set_position(TextPosition state) { m_position = state; }

    void inc_index() { ++m_index; }
    void dec_index() { assert(m_index > 0); --m_index; }
    void reset_index() { m_index = 0; }
    bool try_match() { return m_value->try_match(*this); }
    bool has_states() { return m_states.size() > 0; }
    void push_state(MatchState state) { m_states.push(state); }
    void pop_state();
    MatchState& top_state();

private:
    MatchHandler& m_matcher;
    std::shared_ptr<Value> m_value;
    TextPosition m_position;
    size_t m_index { 0 };
    std::stack<MatchState> m_states;
};

class SearchProvider {
public:
    explicit SearchProvider(std::string string) : m_match_against_string(string) { }
    ~SearchProvider() { }

    std::vector<TextSnippet> find_from_value(std::shared_ptr<Value> value);

private:
    std::string m_match_against_string;
};
