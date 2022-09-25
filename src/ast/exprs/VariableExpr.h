#pragma once

#include "abstract/Expr.h"

#include <cassert>

class VariableExpr : public Expr {
public:
    VariableExpr(TextSnippet snippet, std::string name)
        : Expr(snippet), m_name(name) { }
    ~VariableExpr() { }

    ErrorOr<std::shared_ptr<Value>> eval(RuntimeManager& rtm);

private:
    std::string m_name;
};
