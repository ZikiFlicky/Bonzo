#pragma once

#include <runtime/values/abstract/Value.h>
#include <runtime/MatchHandler.h>

class SomeValue : public Value {
public:
    SomeValue(std::shared_ptr<Value> value)
        : Value(), m_value(value) { }

    bool can_be_matched() override { return true; }
    std::string generate_regex() override;
    bool try_match(MatchState& state) override;

private:
    std::shared_ptr<Value> m_value;
};
