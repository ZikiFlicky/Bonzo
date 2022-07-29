#pragma once

#include "abstract/CollectionValue.h"

class SeparatedValue : public CollectionValue {
public:
    SeparatedValue(Interpreter& interpreter, std::vector<std::shared_ptr<Value>> values, std::shared_ptr<Value> separator)
        : CollectionValue(interpreter, values), m_separator(separator) { };

    bool can_be_matched() { return can_values_be_matched() && m_separator->can_be_matched(); }

    std::string generate_regex();
    bool needs_parens() { return values().size() > 1; }

private:
    std::shared_ptr<Value> m_separator;

    std::shared_ptr<Value> seperator() { return m_separator; }
};
