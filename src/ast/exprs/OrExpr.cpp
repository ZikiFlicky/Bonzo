#include "OrExpr.h"
#include <runtime/values/OrValue.h>

ErrorOr<std::shared_ptr<Value>> OrExpr::eval(RuntimeManager& rtm) {
    (void)rtm;
    if (eval_lhs_and_rhs(rtm).is_error())
        return { };
    return lhs_value()->or_with(lhs_value(), rhs_value(), snippet().start(), rtm);
}
