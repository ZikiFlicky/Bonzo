#include "MatchInstruction.h"
#include <runtime/Interpreter.h>

#include <iostream>


MatchInstruction::MatchInstruction(std::shared_ptr<Expr> expr)
    : Instruction(), m_expr(expr) {
}

MatchInstruction::~MatchInstruction() {
}

ErrorOr<void> MatchInstruction::run(Interpreter& interpreter) {
    auto maybe_value = m_expr->eval(interpreter);
    if (maybe_value.is_error())
        return false;
    auto value = maybe_value.value();
    if (!value->can_be_matched()) {
        interpreter.set_error("unable to generate regex from input");
        return false;
    }
    std::cout << "Generated: " << value->generate_regex() << std::endl;
    return true; // Ran successfully
}
