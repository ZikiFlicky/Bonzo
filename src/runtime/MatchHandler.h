#pragma once

#include <runtime/values/abstract/Value.h>
#include <utils/TextPosition.h>

#include <string>
#include <vector>
#include <stack>
#include <iostream>

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

    void inc_index() { ++m_index; }
    void dec_index() { assert(m_index > 0); --m_index; }
    void reset_index() { m_index = 0; }
    bool try_match() { return m_value->try_match(*this); }
    TextSnippet try_match_largest_snippet();
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
    explicit SearchProvider(std::string& string) : m_match_against_string(string) { }
    ~SearchProvider() { }

    std::vector<TextSnippet> find_from_value(std::shared_ptr<Value> value);

private:
    std::string& m_match_against_string;
};
