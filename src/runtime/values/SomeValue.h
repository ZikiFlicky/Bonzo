#pragma once

#include <runtime/values/abstract/RegValue.h>

class SomeValue : public RegValue {
public:
    SomeValue(Interpreter& interpreter, std::shared_ptr<RegValue> value)
        : RegValue(interpreter), m_value(value) { }

    std::string generate_regex() override {
        return m_value->generate_regex_as_child() + "+";
    }

    bool can_be_matched() override { return true; }

private:
    std::shared_ptr<RegValue> m_value;
};
