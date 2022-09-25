#pragma once

#include "abstract/Value.h"

#include <string>

class StringValue : public Value {
public:
    StringValue(std::string string)
        : Value(), m_string(string) { }
    ~StringValue() override { }

    std::string string() { return m_string; }

    ErrorOr<std::shared_ptr<Value>> add_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs,
        TextPosition operator_position, RuntimeManager& rtm) override;

    bool can_be_matched() override { return true; }
    std::string generate_regex() override;
    bool needs_parens() override { return m_string.size() != 1; }
    bool try_match(MatchState& state) override;

    virtual bool is_string() override { return true; }

private:
    std::string m_string;
};
