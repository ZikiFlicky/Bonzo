#include "FuncValue.h"

FuncValue::FuncValue(std::string name, std::vector<std::string> parameter_names, std::shared_ptr<Expr> eval_expr)
    : CallableValue(name), m_parameter_names(parameter_names), m_eval_expr(eval_expr) {
}

FuncValue::~FuncValue() {
}

ErrorOr<std::shared_ptr<Value>> FuncValue::call(CallInfo& info) {
    if (info.rtm.expect_arguments_size(info, amount_params()).is_error())
        return { };
    info.rtm.enter_new_scope();
    // Set variables
    for (size_t i = 0; i < amount_params(); ++i)
        info.rtm.set_local_variable(m_parameter_names[i], info.arguments[i].value);
    auto evaluated = m_eval_expr->eval(info.rtm);
    info.rtm.leave_scope();
    return evaluated;
}
