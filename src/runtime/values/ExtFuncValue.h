#pragma once

#include "abstract/CallableValue.h"
#include "utils/ErrorOr.h"

class ExtFuncValue : public CallableValue {
public:
    typedef ErrorOr<std::shared_ptr<Value>> (Interpreter::*FuncDef)(std::vector<std::shared_ptr<Value>> arguments);

    ExtFuncValue(Interpreter& interpreter, FuncDef func)
        : CallableValue(interpreter), m_func(func) { }

    ErrorOr<std::shared_ptr<Value>> call(std::vector<std::shared_ptr<Value>> arguments);

private:
    FuncDef m_func;
};