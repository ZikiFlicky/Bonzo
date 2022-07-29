#pragma once

#include "abstract/Expr.h"

class StringExpr : public Expr {
public:
    StringExpr(StreamSnippet snippet, std::string value)
        : Expr(snippet), m_value(value) { }
    ~StringExpr() { }

    ErrorOr<std::shared_ptr<RegValue>> eval(Interpreter& interpreter);

private:
    std::string m_value;
};
