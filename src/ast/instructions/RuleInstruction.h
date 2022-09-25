#pragma once

#include "Instruction.h"
#include <ast/exprs/abstract/Expr.h>

class RuleInstruction : public Instruction {
public:
    RuleInstruction(std::string name, std::shared_ptr<Expr> expr, TextSnippet snippet);
    ~RuleInstruction();

    ErrorOr<void> run(RuntimeManager& rtm);

private:
    std::string m_name;
    std::shared_ptr<Expr> m_expr;
};
