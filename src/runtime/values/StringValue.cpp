#include "StringValue.h"
#include <runtime/MatchHandler.h>

#include <iostream>

ErrorOr<std::shared_ptr<Value>> StringValue::add_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs,
    TextPosition operator_position, RuntimeManager& rtm) {
    if (rhs->is_string()) {
        auto rhs_string = rhs->string();
        return std::shared_ptr<Value>(new StringValue(string() + rhs_string->string()));
    }
    return Value::add_with(shared_this, rhs, operator_position, rtm);
}

std::string StringValue::generate_regex() {
    std::string generated = { };
    for (auto c : string()) {
        switch (c) {
        case '.':
        case '*':
        case '\\':
        case '(':
        case ')':
        case '{':
        case '}':
        case '?':
        case '+':
        case '|':
        case '[':
        case ']':
        case '^':
        case '!':
        case '=':
        case ',':
        case '/':
            generated += "\\";
            break;
        }
        generated += c;
    }
    return generated;
}

bool StringValue::try_match(MatchState& state) {
    if (state.index() > 0)
        return false;
    auto backtrack = state.position();
    for (auto c : string()) {
        if (c != state.matcher().get()) {
            state.matcher().set_position(backtrack);
            return false;
        }
    }
    state.inc_index();
    return true;
}
