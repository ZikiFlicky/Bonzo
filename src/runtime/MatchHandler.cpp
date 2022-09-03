#include "MatchHandler.h"

size_t TextSnippet::length() {
    size_t start_index = m_start.index();
    size_t end_index = m_end.index();
    assert(end_index >= start_index);
    return end_index - start_index;
}

char MatchHandler::get() {
    auto index = position().index();
    auto line = position().line();
    auto column = position().column();
    if (index == m_string.size())
        return '\0';
    auto c = m_string[index++];
    switch (c) {
    case '\r':
        break;
    case '\n':
        ++line;
        column = 1;
        break;
    default:
        ++column;
    }
    m_position = TextPosition(line, column, index);
    return c;
}

void MatchState::pop_state() {
    assert(m_states.size() > 0);
    auto& state = m_states.top();
    matcher().set_position(state.position());
    m_states.pop();
}

MatchState& MatchState::top_state() {
    assert(has_states());
    return m_states.top();
}

std::vector<TextSnippet> SearchProvider::find_from_value(std::shared_ptr<Value> value) {
    std::vector<TextSnippet> snippets;
    // Create base matcher that can advance through all starting positions
    MatchHandler base_matcher(m_match_against_string);
    for (size_t string_index = 0; string_index < m_match_against_string.size(); ++string_index) {
        // Copy matcher
        MatchHandler matcher = base_matcher;
        // Create a state for parsing
        MatchState state(matcher, value);
        while (state.try_match()) {
            // Create snippet from start to end
            TextSnippet snippet(state.position(), matcher.position());
            // Ignore snippets of length 0
            if (snippet.length() > 0)
                snippets.push_back(snippet);
        }
        base_matcher.get();
    }
    return snippets;
}
