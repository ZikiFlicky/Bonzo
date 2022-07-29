#pragma once

#include "abstract/CollectionValue.h"

class OrValue : public CollectionValue {
public:
    OrValue(Interpreter& interpreter, std::vector<std::shared_ptr<RegValue>> values)
        : CollectionValue(interpreter, values) { };

    bool can_be_matched() { return can_values_be_matched(); }
    ErrorOr<std::shared_ptr<RegValue>> or_with(std::shared_ptr<RegValue> shared_this, std::shared_ptr<RegValue> rhs);

    std::string generate_regex();
    bool needs_parens() { return values().size() > 1; }
};
