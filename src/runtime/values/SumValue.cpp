#include "SumValue.h"
#include <runtime/Interpreter.h>
#include <runtime/MatchHandler.h>

#include <stack>

ErrorOr<std::shared_ptr<Value>> SumValue::add_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs, TextPosition operator_position) {
    (void)shared_this;
    if (!rhs->can_be_matched()) {
        interpreter().set_error("values could not be added together", operator_position);
        return { };
    }
    // Copy with the new value
    std::vector<std::shared_ptr<Value>> new_values = values();
    new_values.push_back(rhs);
    return std::shared_ptr<Value>(new SumValue(interpreter(), new_values));
}

std::string SumValue::generate_regex() {
    std::string generated = { };
    for (auto value : values())
        generated += value->generate_regex_as_child();
    return generated;
}

bool SumValue::try_match(MatchState& state) {
    if (values().size() == 0) {
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
        } else if (state.index() == values().size()) {
            return true;
        } else {
            MatchState value_state(state.matcher(), values()[state.index()]);
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
