#include "StringExpr.h"
#include <runtime/values/StringValue.h>
#include <runtime/values/abstract/Value.h>

#include <iostream>

ErrorOr<std::shared_ptr<Value>> StringExpr::eval(Interpreter& interpreter) {
    (void)interpreter;
    return std::shared_ptr<Value>(new StringValue(m_value));
}
