#include "ExtFuncValue.h"
#include <runtime/Interpreter.h>

ErrorOr<std::shared_ptr<Value>> ExtFuncValue::call(CallInfo& info) {
    auto result = (interpreter().*m_func)(info);
    if (result.is_error())
        return { };
    return result;
}
