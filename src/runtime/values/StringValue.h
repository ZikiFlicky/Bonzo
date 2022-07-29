#pragma once

#include "abstract/RegValue.h"

#include <string>

class StringValue : public RegValue {
public:
    StringValue(Interpreter& interpreter, std::string string)
        : RegValue(interpreter), m_string(string) { }
    ~StringValue() override { }

    std::string string() { return m_string; }

    ErrorOr<std::shared_ptr<RegValue>> add_with(std::shared_ptr<RegValue> shared_this, std::shared_ptr<RegValue> rhs) override;

    bool can_be_matched() override { return true; }
    std::string generate_regex() override;
    bool needs_parens() override { return false; }

    virtual bool is_string() override { return true; }

private:
    std::string m_string;
};
