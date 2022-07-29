#include "abstract/RegValue.h"
#include <runtime/Interpreter.h>

class SpecialValue : public RegValue {
public:
    enum Type {
        Character = 1,
        Whitespace,
        Newline
    };

    SpecialValue(Interpreter& interpreter, Type type)
        : RegValue(interpreter), m_type(type) { }

    bool can_be_matched() { return true; }

    std::string generate_regex();
    bool needs_parens() { return false; }

private:
    Type m_type;
};