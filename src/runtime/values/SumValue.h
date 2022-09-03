#pragma once

#include "abstract/CollectionValue.h"

class SumValue : public CollectionValue {
public:
    SumValue(Interpreter& interpreter, std::vector<std::shared_ptr<Value>> values)
        : CollectionValue(interpreter, values) { };

    bool can_be_matched() override { return can_values_be_matched(); }
    ErrorOr<std::shared_ptr<Value>> add_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs) override;

    std::string generate_regex() override;
    bool needs_parens() override { return values().size() > 1; }
    bool try_match(MatchState& state) override;
};
