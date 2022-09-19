#include "RuleInstruction.h"

#include <iostream>

RuleInstruction::RuleInstruction(std::string name, std::shared_ptr<Expr> expr, TextSnippet snippet)
    : Instruction(snippet), m_name(name), m_expr(expr) {
}

RuleInstruction::~RuleInstruction() {
}

ErrorOr<void> RuleInstruction::run(Interpreter& interpreter) {
    auto maybe_value = m_expr->eval(interpreter);
    if (maybe_value.is_error())
        return false;
    interpreter.set_variable(m_name, maybe_value.value());
    return true; // Ran successfully
}
