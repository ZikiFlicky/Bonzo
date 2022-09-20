#include "AddExpr.h"

ErrorOr<std::shared_ptr<Value>> AddExpr::eval(Interpreter& interpreter) {
    (void)interpreter;
    if (eval_lhs_and_rhs(interpreter).is_error())
        return { };
    return lhs_value()->add_with(lhs_value(), rhs_value(), snippet().start(), interpreter);
}
