#pragma once

#include "utils/ErrorOr.h"
#include "RegValue.h"

#include <vector>

class CallableValue : public RegValue {
public:
    CallableValue(Interpreter& interpreter)
        : RegValue(interpreter) { }

    bool is_callable() override { return true; }

    virtual ErrorOr<std::shared_ptr<RegValue>> call(std::vector<std::shared_ptr<RegValue>> arguments) = 0;
};
