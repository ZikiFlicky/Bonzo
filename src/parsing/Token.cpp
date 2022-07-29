#include "Token.h"

std::string Token::type_to_string(Type type) {
    switch (type) {
    case Token::Type::Pipe:
        return "|";
    case Token::Type::Plus:
        return "+";
    case LeftParen:
        return "(";
    case RightParen:
        return ")";
    case LeftBracket:
        return "[";
    case RightBracket:
        return "]";
    case LeftBrace:
        return "{";
    case RightBrace:
        return "}";
    default:
        return "???";
    }
}

std::string Token::to_string() {
    return m_start_state.stream()->substr(start_state().index(), length());
}
