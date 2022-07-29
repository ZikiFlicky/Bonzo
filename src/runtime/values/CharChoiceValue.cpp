#include "CharChoiceValue.h"

std::string CharChoiceValue::generate_regex() {
    std::string generated = "[";
    for (auto c : m_characters)
        generated += c;
    generated += "]";
    return generated;
}
