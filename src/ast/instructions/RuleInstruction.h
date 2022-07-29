#pragma once

#include "Instruction.h"
#include <ast/exprs/abstract/Expr.h>

class RuleInstruction : public Instruction {
public:
    RuleInstruction(std::string name, std::shared_ptr<Expr> expr);
    ~RuleInstruction();

    ErrorOr<void> run(Interpreter& interpreter);

private:
    std::string m_name;
    std::shared_ptr<Expr> m_expr;
};
