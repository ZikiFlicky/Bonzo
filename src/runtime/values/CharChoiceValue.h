#pragma once

#include "abstract/Value.h"

#include <set>

class CharChoiceValue : public Value {
public:
    CharChoiceValue(Interpreter& interpreter, std::set<int> characters)
        : Value(interpreter), m_characters(characters) { }
    ~CharChoiceValue() { }

    bool can_be_matched() override { return true; }

    std::string generate_regex() override;
    bool needs_parens() override { return false; }
    bool try_match(MatchState& state) override;

private:
    std::set<int> m_characters;
};
