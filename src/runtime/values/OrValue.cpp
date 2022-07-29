#include "OrValue.h"

#include <runtime/Interpreter.h>

ErrorOr<std::shared_ptr<RegValue>> OrValue::or_with(std::shared_ptr<RegValue> shared_this, std::shared_ptr<RegValue> rhs) {
    (void)shared_this;
    if (!rhs->can_be_matched()) {
        interpreter().set_error("values could not be or'ed together");
        return { };
    }
    auto new_values = values();
    new_values.push_back(rhs);
    return std::shared_ptr<RegValue>(new OrValue(interpreter(), new_values));
}

std::string OrValue::generate_regex() {
    std::string generated = { };
    for (size_t i = 0; i < values().size(); ++i) {
        if (i != 0)
            generated += "|";
        generated += values()[i]->generate_regex_as_child();
    }
    return generated;
}
