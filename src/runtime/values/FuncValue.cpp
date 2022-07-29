#include "FuncValue.h"

FuncValue::FuncValue(Interpreter& interpreter, std::vector<std::string> parameter_names, std::shared_ptr<Expr> eval_expr)
    : CallableValue(interpreter), m_parameter_names(parameter_names), m_eval_expr(eval_expr) {
}

FuncValue::~FuncValue() {
}

ErrorOr<std::shared_ptr<RegValue>> FuncValue::call(std::vector<std::shared_ptr<RegValue>> arguments) {
    if (!interpreter().expect_arguments_size(arguments, amount_params())) {
        interpreter().set_error("wrong amount of arguments");
        return { };
    }
    interpreter().enter_new_scope();
    // Set variables
    for (size_t i = 0; i < m_parameter_names.size(); ++i)
        interpreter().set_local_variable(m_parameter_names[i], arguments[i]);
    auto evaluated = m_eval_expr->eval(interpreter());
    interpreter().leave_scope();
    return evaluated;
}
