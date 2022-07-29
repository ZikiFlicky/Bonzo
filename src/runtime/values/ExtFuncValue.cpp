#include "ExtFuncValue.h"
#include <runtime/Interpreter.h>

ErrorOr<std::shared_ptr<RegValue>> ExtFuncValue::call(std::vector<std::shared_ptr<RegValue>> arguments) {
    auto result = (interpreter().*m_func)(arguments);
    if (result.is_error())
        return { };
    return result;
}
