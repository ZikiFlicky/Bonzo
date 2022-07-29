#pragma once

#include "utils/ErrorOr.h"
#include "Value.h"

#include <vector>

class CallableValue : public Value {
public:
    CallableValue(Interpreter& interpreter)
        : Value(interpreter) { }

    bool is_callable() override { return true; }

    virtual ErrorOr<std::shared_ptr<Value>> call(std::vector<std::shared_ptr<Value>> arguments) = 0;
};
