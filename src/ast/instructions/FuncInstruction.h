#pragma once

#include "Instruction.h"
#include <ast/exprs/abstract/Expr.h>

#include <vector>

class FuncInstruction : public Instruction {
public:
    FuncInstruction(std::string name, std::vector<std::string> parameters, std::shared_ptr<Expr> expr);
    ~FuncInstruction() override;

    ErrorOr<void> run(Interpreter& interpreter) override;

private:
    std::string m_name;
    std::vector<std::string> m_parameters;
    std::shared_ptr<Expr> m_expr;
};
