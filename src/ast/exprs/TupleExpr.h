#pragma once

#include "abstract/Expr.h"

#include <vector>
#include <cassert>

class TupleExpr : public Expr {
public:
    TupleExpr(StreamSnippet snippet, std::vector<std::shared_ptr<Expr>> arguments);

    ErrorOr<std::shared_ptr<Value>> eval(Interpreter& interpreter);

private:
    std::vector<std::shared_ptr<Expr>> m_arguments;
};
