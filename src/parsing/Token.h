#pragma once

#include "stream/State.h"
#include <cstddef>

class Token {
public:
    enum Type {
        Unknown, // Needed for empty constructor
        Newline,
        Match,
        Rule,
        Func,
        String,
        Pipe,
        Plus,
        LeftParen,
        RightParen,
        LeftBracket,
        RightBracket,
        LeftBrace,
        RightBrace,
        Identifier
    };

    Token(Type type, State start_state, size_t index, size_t length)
        : m_type(type), m_start_state(start_state), m_index(index), m_length(length) { }
    Token()
        : m_type(Token::Type::Unknown), m_index(0), m_length(0) { }
    ~Token() { }

    Type type() { return m_type; }
    State start_state() { return m_start_state; }
    size_t index() { return m_index; }
    size_t length() { return m_length; }

    std::string to_string();

    static std::string type_to_string(Type type);

protected:
    // TODO: Use StreamSnippet
    Type m_type;
    State m_start_state { };
    size_t m_index, m_length;
};
