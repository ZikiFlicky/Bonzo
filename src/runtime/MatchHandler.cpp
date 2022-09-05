#include "MatchHandler.h"
#include <utils/TextSnippet.h>

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
    // Set new position
    m_position = { &m_string, index, line, column };
    return c;
}

TextSnippet MatchState::try_match_largest_snippet() {
    if (try_match()) {
        auto max_position = matcher().position();
        while (try_match()) {
            auto new_position = matcher().position();
            if (new_position.index() > max_position.index())
                max_position = new_position;
        }
        TextSnippet snippet(position(), max_position);
        // If we got a worthy (length > 0) snippet, return it
        if (snippet.length() > 0)
            matcher().set_position(max_position);
        else // Backtrack position to start
            matcher().set_position(position());
        return snippet;
    }
    return { };
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
    MatchHandler matcher(m_match_against_string);
    // Loop while there is at least one character to match against
    while (matcher.position().index() < m_match_against_string.size()) {
        // Create a state for parsing
        MatchState state(matcher, value);
        TextSnippet snippet = state.try_match_largest_snippet();
        // Push snippet to vector if it's valuable, otherwise increment read index
        if (snippet.length() > 0)
            snippets.push_back(snippet);
        else
            matcher.get();
    }
    return snippets;
}
