#pragma once

#include "abstract/CollectionValue.h"

#include <vector>

class TupleValue : public CollectionValue {
public:
    TupleValue(Interpreter& interpreter, std::vector<std::shared_ptr<Value>> values)
        : CollectionValue(interpreter, values) { }

    virtual bool is_tuple() override { return true; }
    bool try_match(MatchState& state) override { (void)state; assert(0); }
};
