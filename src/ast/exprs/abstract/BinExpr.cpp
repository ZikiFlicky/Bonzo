#include "BinExpr.h"

BinExpr::BinExpr(TextSnippet snippet, std::shared_ptr<Expr> lhs, std::shared_ptr<Expr> rhs)
    : Expr(snippet), m_lhs(lhs), m_rhs(rhs) {
}

BinExpr::~BinExpr() {
}

ErrorOr<void> BinExpr::eval_lhs_and_rhs(RuntimeManager& rtm) {
    auto maybe_lhs = lhs()->eval(rtm);
    if (maybe_lhs.is_error())
        return false;
    auto maybe_rhs = rhs()->eval(rtm);
    if (maybe_rhs.is_error())
        return false;
    m_lhs_value = maybe_lhs.value();
    m_rhs_value = maybe_rhs.value();
    return true;
}
