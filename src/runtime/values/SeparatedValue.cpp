#include "SeparatedValue.h"
#include <runtime/MatchHandler.h>

#include <stack>

std::string SeparatedValue::generate_regex() {
    std::string generated = "";
    std::string stringed_separator = seperator()->generate_regex();
    for (size_t i = 0; i < values().size(); ++i) {
        if (i != 0)
            generated += stringed_separator;
        generated += values()[i]->generate_regex_as_child();
    }
    return generated;
}

bool SeparatedValue::try_match(MatchState& state) {
    size_t amount_values = values().size() == 0 ? 0 : values().size() * 2 - 1;
    if (amount_values == 0) {
        if (state.index() == 0) {
            state.inc_index();
            return true;
        } else {
            return false;
        }
    }
    // If this was already matched we need to start by rematching
    bool need_rematch = state.has_states();
    for (;;) {
        if (need_rematch) {
            // If we got to the beginning, it means we have no more possible matches
            if (!state.has_states())
                return false;
            auto& top_state = state.top_state();
            if (!top_state.try_match()) {
                if (state.index() == 0)
                    return false;
                state.pop_state();
                state.dec_index();
            } else {
                // If we matched the value again, we can start matching new states
                need_rematch = false;
            }
        } else if (state.index() == amount_values) {
            return true;
        } else {
            // Decide which value to use
            auto value = state.index() % 2 == 0 ? values()[state.index() / 2] : seperator();
            MatchState value_state(state.matcher(), value);
            if (value_state.try_match()) {
                state.push_state(value_state);
                state.inc_index();
            } else {
                if (state.index() == 0)
                    return false;
                need_rematch = true;
            }
        }
    }
}
