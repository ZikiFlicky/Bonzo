#pragma once

#include "abstract/Value.h"

class ArbitraryLengthValue : public Value {
public:
    ArbitraryLengthValue(Interpreter& interpreter, std::shared_ptr<Value> value)
        : Value(interpreter), m_value(value) { };

    bool can_be_matched() { return m_value->can_be_matched(); }

    std::string generate_regex();

private:
    std::shared_ptr<Value> m_value;

    std::shared_ptr<Value> value() { return m_value; }
};
