#pragma once

#include <runtime/values/abstract/Value.h>
#include <runtime/MatchHandler.h>

class OptionalValue : public Value {
public:
    OptionalValue(std::shared_ptr<Value> optional)
        : Value(), m_optional(optional) {}

    bool can_be_matched() override { return true; }
    std::string generate_regex() override;
    bool try_match(MatchState& state) override;

private:
    std::shared_ptr<Value> m_optional;
};
