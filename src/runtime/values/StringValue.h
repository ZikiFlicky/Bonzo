#pragma once

#include "abstract/Value.h"

#include <string>

class StringValue : public Value {
public:
    StringValue(Interpreter& interpreter, std::string string)
        : Value(interpreter), m_string(string) { }
    ~StringValue() override { }

    std::string string() { return m_string; }

    ErrorOr<std::shared_ptr<Value>> add_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs) override;

    bool can_be_matched() override { return true; }
    std::string generate_regex() override;
    bool needs_parens() override { return m_string.size() != 1; }

    virtual bool is_string() override { return true; }

private:
    std::string m_string;
};
