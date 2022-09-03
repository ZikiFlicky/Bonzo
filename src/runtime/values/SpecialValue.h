#include "abstract/Value.h"
#include <runtime/Interpreter.h>

class SpecialValue : public Value {
public:
    enum Type {
        Character = 1,
        Whitespace,
        Newline
    };

    SpecialValue(Interpreter& interpreter, Type type)
        : Value(interpreter), m_type(type) { }

    bool can_be_matched() override { return true; }

    std::string generate_regex() override;
    bool needs_parens() override { return false; }
    bool try_match(MatchState& state) override;

private:
    Type m_type;
};
