#include "OptionalValue.h"

std::string OptionalValue::generate_regex() {
    return m_optional->generate_regex_as_child() + "?";
}

bool OptionalValue::try_match(MatchState& state) {
    switch (state.index()) {
    case 0:
        state.inc_index();
        return true;
    case 1: {
        // If not matched before
        if (!state.has_states()) {
            MatchState optional_state(state.matcher(), m_optional);
            if (!optional_state.try_match()) {
                state.inc_index();
                return false;
            }
            state.push_state(optional_state);
            return true;
        }
        // If we matched before and can match again, return true
        auto& main_state = state.top_state();
        if (main_state.try_match())
            return true;
        // If we couldn't find anything return false and switch to mode 2 to automatically return false
        state.pop_state();
        state.inc_index();
        return false;
    }
    default:
        return false;
    }
}
