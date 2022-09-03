#pragma once

#include <runtime/values/abstract/Value.h>

#include <string>
#include <vector>
#include <stack>
#include <iostream>

class TextState {
public:
    TextState(size_t line, size_t column, size_t index)
        : m_line(line), m_column(column), m_index(index) { }
    TextState() { }

    size_t line() { return m_line; }
    size_t column() { return m_column; }
    size_t index() { return m_index; }

    std::string to_string() { return "line " + std::to_string(line()) + " column " + std::to_string(column()); }

private:
    size_t m_line { 1 }, m_column { 1 }, m_index { 0 };
};

class TextSnippet {
public:
    TextSnippet(TextState start, TextState end)
        : m_start(start), m_end(end) { }

    TextState& start() { return m_start; }
    TextState& end() { return m_end; }
    size_t length();

private:
    TextState m_start, m_end;
};

class MatchHandler {
public:
    explicit MatchHandler(std::string string) : m_string(string) { }
    ~MatchHandler() { }

    TextState& text_state() { return m_text_state; }

    void set_text_state(TextState text_state) { m_text_state = text_state; }

    char get();

private:
    TextState m_text_state;
    std::string m_string;
};

class MatchState {
public:
    explicit MatchState(MatchHandler& matcher, std::shared_ptr<Value> value)
        : m_matcher(matcher), m_value(value), m_text_state(matcher.text_state()) { }
    ~MatchState() { }

    MatchHandler& matcher() { return m_matcher; }
    TextState text_state() { return m_text_state; }
    size_t index() { return m_index; }

    void set_text_state(TextState state) { m_text_state = state; }

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
    TextState m_text_state;
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
