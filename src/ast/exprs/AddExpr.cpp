#include "AddExpr.h"

ErrorOr<std::shared_ptr<Value>> AddExpr::eval(RuntimeManager& rtm) {
    if (eval_lhs_and_rhs(rtm).is_error())
        return { };
    return lhs_value()->add_with(lhs_value(), rhs_value(), snippet().start(), rtm);
}
