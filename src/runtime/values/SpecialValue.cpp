#include "SpecialValue.h"
#include <runtime/MatchHandler.h>

std::string SpecialValue::generate_regex() {
    switch (m_type) {
    case Character:
        return "\\w";
    case Whitespace:
        return "\\s";
    case Newline:
        return "\\n";
    default:
        assert(0);
    }
}

bool SpecialValue::try_match(MatchState& state) {
    if (state.index() > 0)
        return false;
    auto backtrack = state.position();
    auto c = state.matcher().get();
    bool success;
    switch (m_type) {
    case Character:
        success = isalnum(c);
        break;
    case Whitespace:
        success = c == ' ' || c == '\t';
        break;
    case Newline:
        success = c == '\n';
        break;
    default:
        assert(0);
    }
    if (!success) {
        state.matcher().set_position(backtrack);
        return false;
    }
    state.inc_index();
    return true;
}
