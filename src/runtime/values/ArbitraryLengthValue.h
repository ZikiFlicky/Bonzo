#pragma once

#include "abstract/RegValue.h"

class ArbitraryLengthValue : public RegValue {
public:
    ArbitraryLengthValue(Interpreter& interpreter, std::shared_ptr<RegValue> value)
        : RegValue(interpreter), m_value(value) { };

    bool can_be_matched() { return m_value->can_be_matched(); }

    std::string generate_regex();

private:
    std::shared_ptr<RegValue> m_value;

    std::shared_ptr<RegValue> value() { return m_value; }
};
