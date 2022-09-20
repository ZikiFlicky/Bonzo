#include "OrValue.h"

#include <runtime/Interpreter.h>
#include <runtime/MatchHandler.h>

ErrorOr<std::shared_ptr<Value>> OrValue::or_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs,
    TextPosition operator_position, Interpreter& interpreter) {
    (void)shared_this;
    if (!rhs->can_be_matched()) {
        interpreter.set_error("values could not be or'ed together", operator_position);
        return { };
    }
    auto new_values = values();
    new_values.push_back(rhs);
    return std::shared_ptr<Value>(new OrValue(new_values));
}

std::string OrValue::generate_regex() {
    std::string generated = { };
    for (size_t i = 0; i < values().size(); ++i) {
        if (i != 0)
            generated += "|";
        generated += values()[i]->generate_regex_as_child();
    }
    return generated;
}

bool OrValue::try_match(MatchState& state) {
    while (state.index() < values().size()) {
        // If we already have a state try to match it differently
        if (state.has_states()) {
            auto& top_state = state.top_state();
            if (top_state.try_match())
                return true;
            state.pop_state();
        }
        MatchState val_state(state.matcher(), values()[state.index()]);
        state.inc_index();
        if (val_state.try_match()) {
            state.push_state(val_state);
            return true;
        }
    }
    return false;
}
