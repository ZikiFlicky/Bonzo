#pragma once

#include "abstract/Expr.h"

#include <vector>
#include <cassert>

class InvocationExpr : public Expr {
public:
    InvocationExpr(TextSnippet snippet, std::shared_ptr<Expr> invoked, std::vector<std::shared_ptr<Expr>> arguments);
    ~InvocationExpr() override;

    ErrorOr<std::shared_ptr<Value>> eval(RuntimeManager& rtm) override;

private:
    std::shared_ptr<Expr> m_invoked;
    std::vector<std::shared_ptr<Expr>> m_arguments;
};
