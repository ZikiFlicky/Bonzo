#include "ArbitraryLengthValue.h"
#include <runtime/MatchHandler.h>

std::string ArbitraryLengthValue::generate_regex() {
    return m_value->generate_regex_as_child() + "*";
}

bool ArbitraryLengthValue::try_match(MatchState& state) {
    if (state.index() == 0) {
        state.inc_index();
        return true;
    }
    bool need_rematch = false;
    for (;;) {
        // If we already had a value and it didn't change our position, we are matching an empty value
        if (state.has_states() && state.position() == state.matcher().position())
            return false;
        MatchState val_state(state.matcher(), value());
        if (!need_rematch && val_state.try_match()) {
            state.push_state(val_state);
            state.inc_index();
            return true;
        } else {
            if (!state.has_states())
                return false;            
            // If you can rematch the top state return true
            auto& top_state = state.top_state();
            if (top_state.try_match())
                return true;
            // Go to the previous state
            state.pop_state();
            state.dec_index();
            need_rematch = true;
        }
    }
}
