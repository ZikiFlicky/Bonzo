#pragma once

#include <runtime/values/abstract/Value.h>

class OptionalValue : public Value {
public:
    OptionalValue(Interpreter &interpreter, std::shared_ptr<Value> optional)
        : Value(interpreter), m_optional(optional) {}

    std::string generate_regex() override {
        return m_optional->generate_regex_as_child() + "?";
    }

    bool can_be_matched() override { return true; }

private:
    std::shared_ptr<Value> m_optional;
};
