#include "ArbitraryLengthValue.h"

std::string ArbitraryLengthValue::generate_regex() {
    return m_value->generate_regex_as_child() + "*";
}
