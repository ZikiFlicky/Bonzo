#pragma once

#include "abstract/Value.h"

class ArbitraryLengthValue : public Value {
public:
    ArbitraryLengthValue(std::shared_ptr<Value> value)
        : Value(), m_value(value) { };

    bool can_be_matched() override { return m_value->can_be_matched(); }

    std::string generate_regex() override;
    bool try_match(MatchState& state) override;

private:
    std::shared_ptr<Value> m_value;

    std::shared_ptr<Value> value() { return m_value; }
};
