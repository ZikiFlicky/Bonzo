#include "SpecialValue.h"

std::string SpecialValue::generate_regex() {
    switch (m_type) {
    case Character:
        return "\\w";
    case Whitespace:
        return "\\s";
    case Newline:
        return "\\n";
    default:
        assert(0);
    }
}
