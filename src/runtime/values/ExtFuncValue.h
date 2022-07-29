#pragma once

#include "abstract/CallableValue.h"
#include "utils/ErrorOr.h"

class ExtFuncValue : public CallableValue {
public:
    typedef ErrorOr<std::shared_ptr<RegValue>> (Interpreter::*FuncDef)(std::vector<std::shared_ptr<RegValue>> arguments);

    ExtFuncValue(Interpreter& interpreter, FuncDef func)
        : CallableValue(interpreter), m_func(func) { }

    ErrorOr<std::shared_ptr<RegValue>> call(std::vector<std::shared_ptr<RegValue>> arguments);

private:
    FuncDef m_func;
};