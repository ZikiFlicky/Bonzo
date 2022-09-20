#include "FuncValue.h"

FuncValue::FuncValue(std::string name, std::vector<std::string> parameter_names, std::shared_ptr<Expr> eval_expr)
    : CallableValue(name), m_parameter_names(parameter_names), m_eval_expr(eval_expr) {
}

FuncValue::~FuncValue() {
}

ErrorOr<std::shared_ptr<Value>> FuncValue::call(CallInfo& info) {
    if (info.interpreter.expect_arguments_size(info, amount_params()).is_error()) {
        info.interpreter.set_error("wrong amount of arguments", info.call_snippet.start());
        return { };
    }
    info.interpreter.enter_new_scope();
    // Set variables
    for (size_t i = 0; i < amount_params(); ++i)
        info.interpreter.set_local_variable(m_parameter_names[i], info.arguments[i].value);
    auto evaluated = m_eval_expr->eval(info.interpreter);
    info.interpreter.leave_scope();
    return evaluated;
}
