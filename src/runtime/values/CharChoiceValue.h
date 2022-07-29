#pragma once

#include "abstract/Value.h"

#include <set>

class CharChoiceValue : public Value {
public:
    CharChoiceValue(Interpreter& interpreter, std::set<int> characters)
        : Value(interpreter), m_characters(characters) { }
    ~CharChoiceValue() { }

    bool can_be_matched() { return true; }

    std::string generate_regex();
    bool needs_parens() { return false; }

private:
    std::set<int> m_characters;
};
