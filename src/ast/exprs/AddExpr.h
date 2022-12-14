#pragma once

#include "abstract/BinExpr.h"

class AddExpr : public BinExpr {
public:
    AddExpr(TextSnippet snippet, std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs)
        : BinExpr(snippet, lhs, rhs) { }

    ErrorOr<std::shared_ptr<Value>> eval(RuntimeManager& rtm);
};
