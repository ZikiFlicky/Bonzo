#pragma once

#include "Lexer.h"
#include <ast/exprs/abstract/Expr.h>
#include <ast/instructions/Instruction.h>

#include <vector>

class Parser final {
public:
    enum ParseType {
        ParseInstructions = 1,
        ParseExpr
    };

    Parser(std::string& stream, ParseType input_type = ParseInstructions)
        : m_lexer(stream), m_parse_type(input_type) { }
    ~Parser() {}

    bool has_errored() { return m_has_errored; }
    std::vector<std::shared_ptr<Instruction>>& instructions() {
        assert(m_parse_type == ParseInstructions);
        return m_instructions;
    }
    std::shared_ptr<Expr>& expr() {
        assert(m_parse_type == ParseExpr);
        return m_expr;
    }

    ErrorOr<void> parse_all();
    ParsingError error();
    void show_error();

private:
    Lexer m_lexer;
    ParseType m_parse_type;
    std::shared_ptr<Expr> m_expr;
    std::vector<std::shared_ptr<Instruction>> m_instructions { };

    ParsingError m_error;
    bool m_has_errored { false };

    TextPosition position() { return m_lexer.position(); }
    Token& token() { return m_lexer.token(); }
    bool is_eof() { return m_lexer.is_eof(); }
    ErrorOr<bool> tokens_left();

    void set_error(std::string error_message);

    ErrorOr<bool> lex();
    // TODO: Rename to load_position
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

    // Parse according to the two parse modes
    ErrorOr<void> parse_instructions_stream();
    ErrorOr<void> parse_expr_stream();
};
