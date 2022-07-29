#pragma once

#include "abstract/CollectionValue.h"

#include <vector>

class TupleValue : public CollectionValue {
public:
    TupleValue(Interpreter& interpreter, std::vector<std::shared_ptr<RegValue>> values)
        : CollectionValue(interpreter, values) { }

    virtual bool is_tuple() override { return true; }
};
