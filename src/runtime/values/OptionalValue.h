#pragma once

#include <runtime/values/abstract/RegValue.h>

class OptionalValue : public RegValue {
public:
    OptionalValue(Interpreter &interpreter, std::shared_ptr<RegValue> optional)
        : RegValue(interpreter), m_optional(optional) {}

    std::string generate_regex() override {
        return m_optional->generate_regex_as_child() + "?";
    }

    bool can_be_matched() override { return true; }

private:
    std::shared_ptr<RegValue> m_optional;
};
