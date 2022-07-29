#include "RegValue.h"
#include <runtime/values/OrValue.h>
#include <runtime/values/SumValue.h>
#include <runtime/Interpreter.h>

ErrorOr<std::shared_ptr<RegValue>> RegValue::or_with(std::shared_ptr<RegValue> shared_this, std::shared_ptr<RegValue> rhs) {
    if (!can_be_matched() || !rhs->can_be_matched()) {
        interpreter().set_error("values could not be or'ed together");
        return { };
    }
    return std::shared_ptr<RegValue>(new OrValue(interpreter(), { shared_this, rhs }));
}

ErrorOr<std::shared_ptr<RegValue>> RegValue::add_with(std::shared_ptr<RegValue> shared_this, std::shared_ptr<RegValue> rhs) {
    if (!can_be_matched() || !rhs->can_be_matched()) {
        interpreter().set_error("values could not be added together");
        return { };
    }
    return std::shared_ptr<RegValue>(new SumValue(interpreter(), { shared_this, rhs }));
}

std::string RegValue::generate_regex_as_child() {
    std::string generated = generate_regex();
    if (needs_parens())
        generated = "(" + generated + ")";
    return generated;
}
