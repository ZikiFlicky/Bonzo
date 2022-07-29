#include "SeparatedValue.h"

std::string SeparatedValue::generate_regex() {
    std::string generated = { };
    std::string stringed_separator = seperator()->generate_regex();
    for (size_t i = 0; i < values().size(); ++i) {
        if (i != 0)
            generated += stringed_separator;
        generated += values()[i]->generate_regex_as_child();
    }
    return generated;
}
