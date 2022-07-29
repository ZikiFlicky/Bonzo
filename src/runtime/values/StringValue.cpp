#include "StringValue.h"

#include <iostream>

ErrorOr<std::shared_ptr<Value>> StringValue::add_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs) {
    if (rhs->is_string()) {
        auto rhs_string = rhs->string();
        return std::shared_ptr<Value>(new StringValue(interpreter(), string() + rhs_string->string()));
    }
    return Value::add_with(shared_this, rhs);
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
            generated += "\\";
            break;
        }
        generated += c;
    }
    return generated;
}
