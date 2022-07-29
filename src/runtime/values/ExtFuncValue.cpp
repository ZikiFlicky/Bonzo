#include "ExtFuncValue.h"
#include <runtime/Interpreter.h>

ErrorOr<std::shared_ptr<Value>> ExtFuncValue::call(std::vector<std::shared_ptr<Value>> arguments) {
    auto result = (interpreter().*m_func)(arguments);
    if (result.is_error())
        return { };
    return result;
}
