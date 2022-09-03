#include "CharChoiceValue.h"
#include <runtime/MatchHandler.h>

std::string CharChoiceValue::generate_regex() {
    std::string generated = "[";
    for (auto c : m_characters)
        generated += c; // FIXME: Should be escaped!
    generated += "]";
    return generated;
}

bool CharChoiceValue::try_match(MatchState& state) {
    if (state.index() > 0)
        return false;
    auto backtrack = state.matcher().text_state();
    auto character = state.matcher().get();
    for (auto c : m_characters) {
        if (c == character) {
            state.inc_index();
            return true;
        }
    }
    state.matcher().set_text_state(backtrack);
    return false;
}
