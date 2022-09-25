#pragma once

#include "Instruction.h"
#include <ast/exprs/abstract/Expr.h>

#include <vector>

class FuncInstruction : public Instruction {
public:
    FuncInstruction(std::string name, std::vector<std::string> parameters, std::shared_ptr<Expr> expr, TextSnippet snippet);
    ~FuncInstruction() override;

    ErrorOr<void> run(RuntimeManager& rtm) override;

private:
    std::string m_name;
    std::vector<std::string> m_parameters;
    std::shared_ptr<Expr> m_expr;
};
