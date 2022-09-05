#include "Lexer.h"

// TODO: Rename this
static const struct {
    std::string keyword;
    Token::Type type;
} all_keywords[] = {
    { "match", Token::Type::Match },
    { "rule", Token::Type::Rule },
    { "func", Token::Type::Func }
};

void Lexer::set_error(std::string error_message) {
    m_error_message = error_message;
    m_error_state = position();
    m_has_errored = true;
}

void Lexer::advance(size_t amount) {
    m_index += amount;
}

bool Lexer::remove_comment() {
    if (remaining() < 2)
        return false;
    if (get_char() != ';' || get_char(1) != ';')
        return false;

    advance(2);
    for (;;) {
        auto backtrack = position();
        if (read_newline(nullptr)) {
            // Unadvance the newline so when can lex it in the future
            load_state(backtrack);
            break;
        } else {
            advance();
        }
    }
    return true;
}

void Lexer::remove_whitespace() {
    bool found_whitespace = true;
    do {
        if (is_eof() || remove_comment()) {
            // You can have nothing but newline/eof after comment
            found_whitespace = false;
        } else {
            switch (get_char()) {
            case ' ':
            case '\t':
                advance();
                ++m_column;
                break;
            default:
                found_whitespace = false;
                break;
            }
        }
    } while (found_whitespace);
}

bool Lexer::read_newline(size_t* length_out) {
    bool had_newlines = false;
    size_t length = 0;
    for (;;) {
        if (is_eof()) {
            had_newlines = true;
            break;
        } else if (get_char() == '\n') {
            advance();
            ++length;
            ++m_line;
            m_column = 1;
            remove_whitespace();
        } else {
            had_newlines = length > 0;
            break;
        }
    }
    if (length_out)
        *length_out = length;
    return had_newlines;
}

ErrorOr<bool> Lexer::lex_keyword(const std::string& keyword, Token::Type type) {
    if (remaining() < keyword.length())
        return false;
    size_t start_index = m_index;
    TextPosition backtrack = position();
    for (size_t i = 0; i < keyword.length(); ++i) {
        if (keyword[i] != get_char(i))
            return false;
    }
    bool end_of_file = remaining() == keyword.length();
    // If the last char is not a keyword char the keyword is valid
    if (end_of_file || !Lexer::is_identifier_char(get_char(keyword.length()))) {
        advance(keyword.length());
        m_column += keyword.length();
        set_token({ type, backtrack, start_index, keyword.length() });
        return true;
    } else {
        return false;
    }
}

ErrorOr<bool> Lexer::lex_string() {
    if (get_char() != '\'')
        return false;
    advance();
    ++m_column;

    size_t start_index = m_index;
    size_t length = 0;
    auto start_state = position();

    bool loop_string = true;
    while (loop_string) {
        auto backtrack = position();
        // If found a newline (or end-of-file) before terminating the string, set an error
        if (read_newline(nullptr)) {
            load_state(backtrack);
            set_error("unexpected newline");
            return { };
        } else {
            switch (get_char()) {
            case '\'':
                ++m_column;
                advance();
                loop_string = false;
                break;
            case '\\': {
                // Make sure we don't have an eol/eof
                advance();
                auto after_escape_backtrack = position();
                if (read_newline(nullptr)) {
                    load_state(after_escape_backtrack);
                    set_error("unexpected newline");
                    return { };
                }
                length += 2;
                m_column += 2;
                advance();
                break;
            }
            default:
                ++length;
                ++m_column;
                advance();
            }
        }
    }
    set_token({ Token::Type::String, start_state, start_index, length });
    return true;
}

ErrorOr<bool> Lexer::lex_any_keyword() {
    for (auto& keyword_def : all_keywords) {
        auto maybe_result = lex_keyword(keyword_def.keyword, keyword_def.type);
        if (maybe_result.is_error())
            return { };
        // If we managed to lex the keyword
        if (maybe_result.value())
            return true;
    }
    return false;
}

ErrorOr<bool> Lexer::lex_single_character() {
    Token::Type type;
    auto backtrack = position();
    switch (get_char()) {
    case '|':
        type = Token::Type::Pipe;
        break;
    case '+':
        type = Token::Type::Plus;
        break;
    case '(':
        type = Token::Type::LeftParen;
        break;
    case ')':
        type = Token::Type::RightParen;
        break;
    case '[':
        type = Token::Type::LeftBracket;
        break;
    case ']':
        type = Token::Type::RightBracket;
        break;
    case '{':
        type = Token::Type::LeftBrace;
        break;
    case '}':
        type = Token::Type::RightBrace;
        break;
    default:
        return false;
    }
    advance();
    ++m_column;

    set_token({ type, backtrack, backtrack.index(), 1 });
    return true;
}

ErrorOr<bool> Lexer::lex_newline() {
    size_t length;
    size_t start_index = m_index;
    auto start_state = position();

    // If not found a newline
    if (!read_newline(&length))
        return false;

    set_token({ Token::Type::Newline, start_state, start_index, length });
    return true;
}

ErrorOr<bool> Lexer::lex_identifier() {
    auto backtrack = position();

    if (!Lexer::is_identifier_start_char(get_char()))
        return false;
    advance();
    ++m_column;

    size_t length = 1;
    // Loop rest of characters
    while (!is_eof() && Lexer::is_identifier_char(get_char())) {
        advance();
        ++m_column;
        ++length;
    }

    set_token({ Token::Type::Identifier, backtrack, backtrack.index(), length });
    return true;
}

TextPosition Lexer::position() {
    return TextPosition(&m_stream, m_index, m_line, m_column);
}

void Lexer::load_state(const TextPosition& state) {
    m_stream = *state.stream();
    m_index = state.index();
    m_line = state.line();
    m_column = state.column();
}

ErrorOr<bool> Lexer::lex() {
    typedef ErrorOr<bool> (Lexer::*LexMethod)();
    static LexMethod lex_funcs[] = {
        &Lexer::lex_string,
        &Lexer::lex_any_keyword,
        &Lexer::lex_single_character,
        &Lexer::lex_newline,
        &Lexer::lex_identifier
    };

    remove_whitespace();
    if (is_eof())
        return false;
    for (auto method: lex_funcs) {
        auto maybe_result = (this->*method)();
        if (maybe_result.is_error())
            return { };
        // If not error and managed to lex
        if (maybe_result.value())
            return true;
    }

    set_error("no token detected");
    return { };
}
