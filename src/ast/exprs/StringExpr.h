#pragma once

#include "abstract/Expr.h"

class StringExpr : public Expr {
public:
    StringExpr(TextSnippet snippet, std::string value)
        : Expr(snippet), m_value(value) { }
    ~StringExpr() { }

    ErrorOr<std::shared_ptr<Value>> eval(RuntimeManager& rtm);

private:
    std::string m_value;
};
