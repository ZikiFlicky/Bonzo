#pragma once

#include "abstract/Expr.h"

#include <cassert>

class VariableExpr : public Expr {
public:
    VariableExpr(StreamSnippet snippet, std::string name)
        : Expr(snippet), m_name(name) { }
    ~VariableExpr() { }

    ErrorOr<std::shared_ptr<RegValue>> eval(Interpreter& interpreter);

private:
    std::string m_name;
};
