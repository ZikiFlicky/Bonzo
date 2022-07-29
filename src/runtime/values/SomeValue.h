#pragma once

#include <runtime/values/abstract/Value.h>

class SomeValue : public Value {
public:
    SomeValue(Interpreter& interpreter, std::shared_ptr<Value> value)
        : Value(interpreter), m_value(value) { }

    std::string generate_regex() override {
        return m_value->generate_regex_as_child() + "+";
    }

    bool can_be_matched() override { return true; }

private:
    std::shared_ptr<Value> m_value;
};
