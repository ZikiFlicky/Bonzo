#include "FuncInstruction.h"
#include <runtime/values/FuncValue.h>

#include <iostream>

FuncInstruction::FuncInstruction(std::string name, std::vector<std::string> parameters, std::shared_ptr<Expr> expr, TextSnippet snippet)
    : Instruction(snippet), m_name(name), m_parameters(parameters), m_expr(expr) {
}

FuncInstruction::~FuncInstruction() {
}

ErrorOr<void> FuncInstruction::run(RuntimeManager& rtm) {
    auto value = std::make_shared<FuncValue>(m_name, m_parameters, m_expr);
    rtm.set_variable(m_name, value);
    return true; // Ran successfully
}
