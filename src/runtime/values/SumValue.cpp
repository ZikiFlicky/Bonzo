#include "SumValue.h"

#include <runtime/Interpreter.h>

ErrorOr<std::shared_ptr<RegValue>> SumValue::add_with(std::shared_ptr<RegValue> shared_this, std::shared_ptr<RegValue> rhs) {
    (void)shared_this;
    if (!rhs->can_be_matched()) {
        interpreter().set_error("values could not be added together");
        return { };
    }
    // Copy with the new value
    std::vector<std::shared_ptr<RegValue>> new_values = values();
    new_values.push_back(rhs);
    return std::shared_ptr<RegValue>(new SumValue(interpreter(), new_values));
}

std::string SumValue::generate_regex() {
    std::string generated = { };
    for (auto value : values())
        generated += value->generate_regex_as_child();
    return generated;
}
