#include "Value.h"
#include <runtime/values/OrValue.h>
#include <runtime/values/SumValue.h>
#include <runtime/Interpreter.h>

ErrorOr<std::shared_ptr<Value>> Value::or_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs,
        TextPosition operator_position, RuntimeManager& rtm) {
    if (!can_be_matched() || !rhs->can_be_matched()) {
        rtm.set_error("values could not be or'ed together", operator_position);
        return { };
    }
    return std::shared_ptr<Value>(new OrValue({ shared_this, rhs }));
}

ErrorOr<std::shared_ptr<Value>> Value::add_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs,
        TextPosition operator_position, RuntimeManager& rtm) {
    if (!can_be_matched() || !rhs->can_be_matched()) {
        rtm.set_error("values could not be added together", operator_position);
        return { };
    }
    return std::shared_ptr<Value>(new SumValue({ shared_this, rhs }));
}

std::string Value::generate_regex_as_child() {
    std::string generated = generate_regex();
    if (needs_parens())
        generated = "(" + generated + ")";
    return generated;
}
