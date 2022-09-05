#pragma once

#include <parsing/Token.h>
#include <utils/ErrorOr.h>
#include <utils/TextPosition.h>

#include <cstddef>
#include <string>
#include <functional>

class Lexer final {
public:
    Lexer(std::string& stream)
        : m_error_state({ &stream, 0, 1, 1 }), m_stream(stream) { }
    ~Lexer() { }

    inline bool is_eof() { return remaining() == 0; }
    bool has_errored() { return m_has_errored; }
    Token& token() { return m_token; }
    std::string* stream() { return &m_stream; }
    std::string error_message() { return m_error_message; }
    TextPosition error_state() { return m_error_state; }

    void set_token(Token the) { m_token = the; }

    void advance(size_t amount = 1);
    TextPosition position();
    ErrorOr<bool> lex();

    void load_state(const TextPosition& state);

    static inline bool is_identifier_start_char(char c) { return isalpha(c) || c == '_'; }
    static inline bool is_identifier_char(char c) { return is_identifier_start_char(c) || isdigit(c); }

private:
    bool m_has_errored { false };
    std::string m_error_message { };
    TextPosition m_error_state;

    std::string& m_stream;
    size_t m_index { 0 };
    size_t m_line { 1 };
    size_t m_column { 1 };
    Token m_token { Token::Type::Unknown, {}, 0, 0 };

    void set_error(std::string error_message);
    bool remove_comment();
    void remove_whitespace();
    bool read_newline(size_t *length_out);
    ErrorOr<bool> lex_keyword(const std::string& keyword, Token::Type type);
    ErrorOr<bool> lex_string();
    ErrorOr<bool> lex_any_keyword();
    ErrorOr<bool> lex_single_character();
    ErrorOr<bool> lex_newline();
    ErrorOr<bool> lex_identifier();

    inline char get_char(size_t index = 0) { return m_stream[m_index + index]; }
    inline size_t remaining() { return m_stream.length() - m_index; }
};
