#pragma once

#include "abstract/BinExpr.h"

class OrExpr : public BinExpr {
public:
    OrExpr(StreamSnippet snippet, std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs)
        : BinExpr(snippet, lhs, rhs) { }

    ErrorOr<std::shared_ptr<RegValue>> eval(Interpreter& interpreter);
};
