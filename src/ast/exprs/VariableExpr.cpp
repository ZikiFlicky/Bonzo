#include "VariableExpr.h"

ErrorOr<std::shared_ptr<Value>> VariableExpr::eval(RuntimeManager& rtm) {
    auto value = rtm.get_variable(m_name);
    if (!value) {
        rtm.set_error("could not find a variable named '" + m_name + "'", snippet().start());
        return { };
    }
    return value;
}
