#pragma once

#include "Lexer.h"
#include <ast/exprs/abstract/Expr.h>
#include <ast/instructions/Instruction.h>

#include <vector>

class Parser final {
public:
    Parser(Lexer& lexer)
    : m_error_state({ lexer.stream(), 0, 1, 1 }), m_lexer(lexer) { }
    ~Parser() {}

    bool has_errored() { return m_has_errored; }
    std::vector<std::shared_ptr<Instruction>>& instructions() { return m_instructions; }

    ErrorOr<bool> parse_all();
    void show_error();

private:
    std::string m_error_message { };
    TextPosition m_error_state;
    bool m_has_errored { false };

    Lexer& m_lexer;
    std::vector<std::shared_ptr<Instruction>> m_instructions { };

    TextPosition position() { return m_lexer.position(); }
    Token& token() { return m_lexer.token(); }
    bool is_eof() { return m_lexer.is_eof(); }

    void set_error(std::string error_message);

    ErrorOr<bool> lex();
    void load_state(const TextPosition& state) { m_lexer.load_state(state); }
    ErrorOr<bool> match_token(Token::Type type);
    ErrorOr<bool> match_newline(bool do_error = true);
    ErrorOr<bool> parse_enclosed_arguments(Token::Type start_token, Token::Type end_token, std::vector<std::shared_ptr<Expr>>* arguments_out);
    ErrorOr<std::shared_ptr<Expr>> parse_string();
    ErrorOr<std::shared_ptr<Expr>> parse_variable_expr();
    ErrorOr<std::shared_ptr<Expr>> parse_tuple();
    ErrorOr<std::shared_ptr<Expr>> parse_parens();
    ErrorOr<std::shared_ptr<Expr>> parse_single();
    ErrorOr<std::shared_ptr<Expr>> parse_invocation();
    ErrorOr<std::shared_ptr<Expr>> parse_binary_expr();
    ErrorOr<std::shared_ptr<Expr>> parse_expr();
    ErrorOr<std::shared_ptr<Instruction>> parse_match_instruction();
    ErrorOr<std::shared_ptr<Instruction>> parse_rule_instruction();
    ErrorOr<std::shared_ptr<Instruction>> parse_func_instruction();
    ErrorOr<std::shared_ptr<Instruction>> parse_instruction();
};
