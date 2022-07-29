#pragma once

#include "abstract/RegValue.h"

#include <set>

class CharChoiceValue : public RegValue {
public:
    CharChoiceValue(Interpreter& interpreter, std::set<int> characters)
        : RegValue(interpreter), m_characters(characters) { }
    ~CharChoiceValue() { }

    bool can_be_matched() { return true; }

    std::string generate_regex();
    bool needs_parens() { return false; }

private:
    std::set<int> m_characters;
};
