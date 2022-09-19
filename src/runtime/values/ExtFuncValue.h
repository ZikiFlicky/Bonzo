#pragma once

#include "abstract/CallableValue.h"
#include "utils/ErrorOr.h"

class ExtFuncValue : public CallableValue {
public:
    typedef ErrorOr<std::shared_ptr<Value>> (Interpreter::*FuncDef)(CallInfo& info);

    ExtFuncValue(Interpreter& interpreter, std::string name, FuncDef func)
        : CallableValue(interpreter, name), m_func(func) { }

    ErrorOr<std::shared_ptr<Value>> call(CallInfo info) override;

    bool try_match(MatchState& state) override { (void)state; assert(0); }

private:
    FuncDef m_func;
};