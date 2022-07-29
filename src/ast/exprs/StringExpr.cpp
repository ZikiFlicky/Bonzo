#include "StringExpr.h"
#include <runtime/values/StringValue.h>
#include <runtime/values/abstract/RegValue.h>

#include <iostream>

ErrorOr<std::shared_ptr<RegValue>> StringExpr::eval(Interpreter& interpreter) {
    (void)interpreter;
    return std::shared_ptr<RegValue>(new StringValue(interpreter, m_value));
}
