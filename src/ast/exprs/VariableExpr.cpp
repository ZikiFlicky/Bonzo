#include "VariableExpr.h"

ErrorOr<std::shared_ptr<RegValue>> VariableExpr::eval(Interpreter& interpreter) {
    auto value = interpreter.get_variable(m_name);
    if (!value) {
        interpreter.set_error("could not find a variable named '" + m_name + "'");
        return { };
    }
    return value;
}
