#pragma once

#include "Instruction.h"
#include <ast/exprs/abstract/Expr.h>

class MatchInstruction : public Instruction {
public:
    MatchInstruction(std::shared_ptr<Expr> expr, TextSnippet snippet);
    ~MatchInstruction() override;

    ErrorOr<void> run(Interpreter& interpreter) override;

private:
    std::shared_ptr<Expr> m_expr;
};
