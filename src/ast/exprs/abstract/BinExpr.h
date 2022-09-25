#pragma once

#include "Expr.h"
#include <utils/TextSnippet.h>

class BinExpr : public Expr {
public:
    BinExpr(TextSnippet snippet, std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs);
    virtual ~BinExpr();

    ErrorOr<std::shared_ptr<Value>> eval(RuntimeManager& rtm) = 0;

protected:
    std::shared_ptr<Expr> lhs() { return m_lhs; }
    std::shared_ptr<Expr> rhs() { return m_rhs; }

    ErrorOr<void> eval_lhs_and_rhs(RuntimeManager& rtm);

    std::shared_ptr<Value> lhs_value() { return m_lhs_value; }
    std::shared_ptr<Value> rhs_value() { return m_rhs_value; }

private:
    std::shared_ptr<Expr> m_lhs, m_rhs;
    std::shared_ptr<Value> m_lhs_value { nullptr }, m_rhs_value { nullptr };
};
