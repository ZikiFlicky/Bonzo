#include "StringExpr.h"
#include <runtime/values/StringValue.h>
#include <runtime/values/abstract/Value.h>

#include <iostream>

ErrorOr<std::shared_ptr<Value>> StringExpr::eval(RuntimeManager& rtm) {
    (void)rtm;
    return std::shared_ptr<Value>(new StringValue(m_value));
}
