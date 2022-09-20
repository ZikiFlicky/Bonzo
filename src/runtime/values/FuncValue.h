#pragma once

#include "abstract/CallableValue.h"
#include <ast/exprs/abstract/Expr.h>

#include <string>

class FuncValue : public CallableValue {
public:
    FuncValue(std::string name, std::vector<std::string> parameter_names, std::shared_ptr<Expr> eval_expr);
    ~FuncValue() override;

    bool is_callable() override { return true; }
    bool try_match(MatchState& state) override { (void)state; assert(0); }

    ErrorOr<std::shared_ptr<Value>> call(CallInfo& info) override;

private:
    std::vector<std::string> m_parameter_names;
    std::shared_ptr<Expr> m_eval_expr;

    size_t amount_params() { return m_parameter_names.size(); }
};
