#include "OrExpr.h"
#include <runtime/values/OrValue.h>

ErrorOr<std::shared_ptr<Value>> OrExpr::eval(Interpreter& interpreter) {
    (void)interpreter;
    if (eval_lhs_and_rhs(interpreter).is_error())
        return { };
    return lhs_value()->or_with(lhs_value(), rhs_value(), snippet().start(), interpreter);
}
