#include "Parser.h"
#include <ast/exprs/StringExpr.h>
#include <ast/exprs/VariableExpr.h>
#include <ast/exprs/OrExpr.h>
#include <ast/exprs/AddExpr.h>
#include <ast/exprs/InvocationExpr.h>
#include <ast/exprs/TupleExpr.h>
#include <ast/instructions/MatchInstruction.h>
#include <ast/instructions/RuleInstruction.h>
#include <ast/instructions/FuncInstruction.h>
#include <utils/ErrorOr.h>
#include <utils/TextSnippet.h>

#include <cassert>
#include <iostream>

ErrorOr<bool> Parser::lex() {
    auto maybe_result = m_lexer.lex();
    if (maybe_result.is_error())
        return { };
    // Return whether we actually managed to lex
    return maybe_result.value();
}

void Parser::set_error(std::string error_message) {
    m_error = { error_message, position() };
    m_has_errored = true;
}

ErrorOr<bool> Parser::match_token(Token::Type type) {
    auto backtrack = position();
    auto maybe_result = lex();

    if (maybe_result.is_error())
        return { };

    // If not found any token
    if (!maybe_result.value())
        return false;

    // If did find token, but it's type wasn't the requested one
    if (token().type() != type) {
        load_state(backtrack);
        return false;
    }
    return true;
}

ErrorOr<bool> Parser::match_newline(bool do_error) {
    auto backtrack = position();
    auto maybe_result = lex();

    if (maybe_result.is_error())
        return { };

    // If not found token, we're at the end of the file, and an end-of-file can be treated as a newline
    if (!maybe_result.value())
        return true;

    // If found the a token 
    if (token().type() == Token::Type::Newline) {
        return true;
    } else {
        load_state(backtrack);
        if (do_error) {
            set_error("expected newline");
            return { };
        } else {
            return false;
        }
    }
}

ErrorOr<bool> Parser::parse_enclosed_arguments(Token::Type start_token, Token::Type end_token, std::vector<std::shared_ptr<Expr>>* arguments_out) {
    // Try to match start token
    ErrorOr<bool> maybe_matched = match_token(start_token);
    if (maybe_matched.is_error())
        return { };
    // If not matched
    if (!maybe_matched.value())
        return false;

    // Read all arguments
    std::vector<std::shared_ptr<Expr>> arguments;
    bool read_arguments = true;
    do {
        // Try to match end token
        auto maybe_matched = match_token(end_token);
        if (maybe_matched.is_error())
            return { };
        // If managed to match end token, stop
        if (maybe_matched.value()) {
            read_arguments = false;
        } else {
            // Try to parse argument
            auto maybe_parsed = parse_expr();
            if (maybe_parsed.is_error())
                return { };
            // See if an argument wasn't found, error
            std::shared_ptr<Expr> argument;
            if (!(argument = maybe_parsed.value())) {
                set_error("expected argument or '" + Token::type_to_string(end_token) + "'");
                return { };
            }
            arguments.push_back(argument);
        }
    } while (read_arguments);

    *arguments_out = arguments;
    return true;
}

ErrorOr<std::shared_ptr<Expr>> Parser::parse_string() {
    auto maybe_result = match_token(Token::Type::String);
    if (maybe_result.is_error())
        return { };
    // If not matched the string token
    if (!maybe_result.value())
        return std::shared_ptr<Expr>(nullptr);

    auto start_state = token().start_state();
    assert(start_state.stream());
    auto& stream = *start_state.stream();
    auto start_offset = token().index();
    auto string_length = token().length();
    size_t index = 0;
    std::string string { };
    while (index < string_length) {
        if (stream[start_offset + index] == '\\') {
            string += stream[start_offset + index + 1];
            index += 2;
        } else {
            string += stream[start_offset + index];
            ++index;
        }
    }
    return std::shared_ptr<Expr>(new StringExpr({ start_state, position() }, string));
}

ErrorOr<std::shared_ptr<Expr>> Parser::parse_variable_expr() {
    auto maybe_result = match_token(Token::Type::Identifier);
    if (maybe_result.is_error())
        return { };
    // If not matched the string token
    if (!maybe_result.value())
        return std::shared_ptr<Expr>(nullptr);

    auto start_state = token().start_state();
    return std::shared_ptr<Expr>(new VariableExpr({ start_state, position() },
        token().to_string()));
}

ErrorOr<std::shared_ptr<Expr>> Parser::parse_tuple() {
    auto backtrack = position(); // Start state
    std::vector<std::shared_ptr<Expr>> arguments;
    auto maybe_arguments = parse_enclosed_arguments(Token::Type::LeftBracket, Token::Type::RightBracket, &arguments);
    if (maybe_arguments.is_error())
        return { };
    // If failed to parse argument list
    if (!maybe_arguments.value())
        return std::shared_ptr<Expr>(nullptr);

    return std::shared_ptr<Expr>(new TupleExpr({ backtrack, position() }, arguments));
}

ErrorOr<std::shared_ptr<Expr>> Parser::parse_parens() {
    // Try to match start token
    auto maybe_matched = match_token(Token::Type::LeftParen);
    if (maybe_matched.is_error())
        return { };
    // If not matched
    if (!maybe_matched.value())
        return std::shared_ptr<Expr>(nullptr);

    auto maybe_expr = parse_expr();
    if (maybe_expr.is_error())
        return { };
    std::shared_ptr<Expr> inner;
    if (!(inner = maybe_expr.value())) {
        set_error("expected expr after '('");
        return { };
    }

    maybe_matched = match_token(Token::Type::RightParen);
    if (maybe_matched.is_error()) {
        return { };
    }
    if (!maybe_matched.value()) {
        set_error("expected a ')' after expression");
        return { };
    }

    return inner;
}

ErrorOr<std::shared_ptr<Expr>> Parser::parse_single() {
    std::shared_ptr<Expr> expr;

    ErrorOr<std::shared_ptr<Expr>> maybe_result = parse_string();
    if (maybe_result.is_error())
        return { };
    // If we found a string
    if ((expr = maybe_result.value()))
        return expr;

    maybe_result = parse_variable_expr();
    if (maybe_result.is_error())
        return { };
    // If we found a variable name
    if ((expr = maybe_result.value()))
        return expr;

    maybe_result = parse_tuple();
    if (maybe_result.is_error())
        return { };
    // If we found a tuple
    if ((expr = maybe_result.value()))
        return expr;

    maybe_result = parse_parens();
    if (maybe_result.is_error())
        return { };
    // If we found a paren expression
    if ((expr = maybe_result.value()))
        return expr;

    return std::shared_ptr<Expr>(nullptr);
}

ErrorOr<std::shared_ptr<Expr>> Parser::parse_invocation() {
    auto full_backtrack = position(); // Store start state
    auto maybe_parsed = parse_single();
    if (maybe_parsed.is_error())
        return { };
    std::shared_ptr<Expr> expr;
    if (!(expr = maybe_parsed.value()))
        return std::shared_ptr<Expr>(nullptr);

    for (;;) {
        std::vector<std::shared_ptr<Expr>> arguments;
        auto maybe_arguments = parse_enclosed_arguments(Token::Type::LeftBrace, Token::Type::RightBrace, &arguments);
        if (maybe_arguments.is_error()) {
            return { };
        }
        // If not found argument list, stop looping
        if (!maybe_arguments.value())
            break;
        // Create the new expression
        expr = std::shared_ptr<Expr>(new InvocationExpr({ full_backtrack, position() }, expr, arguments));
    }

    return expr;
}

ErrorOr<std::shared_ptr<Expr>> Parser::parse_binary_expr() {
    auto maybe_result = parse_invocation();
    std::shared_ptr<Expr> expr;

    if (maybe_result.is_error())
        return { };

    if (!(expr = maybe_result.value()))
        return std::shared_ptr<Expr>(nullptr);

    for (;;) {
        auto backtrack = position();
        auto maybe_lex_result = lex();

        if (maybe_lex_result.is_error())
            return { };

        // If failed to lex
        if (!maybe_lex_result.value())
            break;

        auto token_type = token().type();
        bool token_valid;

        switch (token_type) {
        case Token::Type::Pipe:
        case Token::Type::Plus:
            token_valid = true;
            break;
        default:
            token_valid = false;
        }
        if (!token_valid) {
            load_state(backtrack);
            break;
        }

        auto maybe_parse_result = parse_invocation();
        if (maybe_parse_result.is_error())
            return { };
        std::shared_ptr<Expr> rhs;
        if (!(rhs = maybe_parse_result.value())) {
            set_error("expected expression after '" + Token::type_to_string(token_type) + "'");
            return { };
        }
        switch (token_type) {
        case Token::Type::Pipe:
            expr = std::shared_ptr<Expr>(new OrExpr({ backtrack, position() }, expr, rhs));
            break;
        case Token::Type::Plus:
            expr = std::shared_ptr<Expr>(new AddExpr({ backtrack, position() }, expr, rhs));
            break;
        default:
            // TODO: BZ_UNEACHABLE();
            assert(0);
        }
    }

    return expr;
}

ErrorOr<std::shared_ptr<Expr>> Parser::parse_expr() {
    return parse_binary_expr();
}

ErrorOr<std::shared_ptr<Instruction>> Parser::parse_match_instruction() {
    auto start_position = position();
    auto maybe_result = match_token(Token::Type::Match);
    if (maybe_result.is_error())
        return { };

    // If failed to match the token "match"
    if (!maybe_result.value())
        return std::shared_ptr<Instruction>(nullptr);

    auto maybe_expr_result = parse_expr();

    if (maybe_expr_result.is_error())
        return { };

    std::shared_ptr<Expr> expr;
    if (!(expr = maybe_expr_result.value())) {
        set_error("expected expression");
        return { };
    }

    return std::shared_ptr<Instruction>(new MatchInstruction(expr, { start_position, position() }));
}

ErrorOr<std::shared_ptr<Instruction>> Parser::parse_rule_instruction() {
    auto start_position = position();
    ErrorOr<bool> maybe_matched = match_token(Token::Type::Rule);
    if (maybe_matched.is_error())
        return { };
    // If failed to match the token "rule"
    if (!maybe_matched.value())
        return std::shared_ptr<Instruction>(nullptr);

    maybe_matched = match_token(Token::Type::Identifier);
    if (maybe_matched.is_error())
        return { };
    // If failed to match an identifier
    if (!maybe_matched.value()) {
        set_error("expected identifier");
        return { };
    }

    auto rule_name = token().to_string();

    maybe_matched = match_token(Token::Type::LeftBrace);
    if (maybe_matched.is_error())
        return { };
    // If failed to match a '{' identifier
    if (!maybe_matched.value()) {
        set_error("expected '{'");
        return { };
    }

    auto maybe_parsed = parse_expr();
    std::shared_ptr<Expr> rule_expr;
    if (maybe_parsed.is_error())
        return { };
    // If not found rule expression
    if (!(rule_expr = maybe_parsed.value())) {
        set_error("expected expression");
        return { };
    }

    maybe_matched = match_token(Token::Type::RightBrace);
    if (maybe_matched.is_error())
        return { };
    // If not matched '}'
    if (!maybe_matched.value()) {
        set_error("expected '}'");
        return { };
    }

    return std::shared_ptr<Instruction>(new RuleInstruction(rule_name, rule_expr, { start_position, position() }));
}

ErrorOr<std::shared_ptr<Instruction>> Parser::parse_func_instruction() {
    auto start_position = position();
    ErrorOr<bool> maybe_matched = match_token(Token::Type::Func);
    if (maybe_matched.is_error())
        return { };
    // If failed to match the token "func"
    if (!maybe_matched.value())
        return std::shared_ptr<Instruction>(nullptr);

    maybe_matched = match_token(Token::Type::Identifier);
    if (maybe_matched.is_error())
        return { };
    // If failed to match an identifier
    if (!maybe_matched.value()) {
        set_error("expected identifier");
        return { };
    }

    auto func_name = token().to_string();

    maybe_matched = match_token(Token::Type::LeftBracket);
    if (maybe_matched.is_error())
        return { };
    // If failed to match a '[' identifier
    if (!maybe_matched.value()) {
        set_error("expected '['");
        return { };
    }

    std::vector<std::string> parameters;
    bool read_parameters = true;
    do {
        auto backtrack = position();
        auto maybe_lexed = lex();
        if (maybe_lexed.is_error())
            return { };
        // If managed to lex
        if (maybe_lexed.value()) {
            switch (token().type()) {
            case Token::Type::RightBracket:
                read_parameters = false;
                break;
            case Token::Type::Identifier:
                parameters.push_back(token().to_string());
                break;
            default:
                load_state(backtrack);
                set_error("expected identifier parameter");
                return { };
            }
        }
    } while (read_parameters);

    maybe_matched = match_token(Token::Type::LeftBrace);
    if (maybe_matched.is_error())
        return { };
    // If failed to match a '{' identifier
    if (!maybe_matched.value()) {
        set_error("expected '{'");
        return { };
    }

    auto maybe_parsed = parse_expr();
    std::shared_ptr<Expr> func_expr;
    if (maybe_parsed.is_error())
        return { };
    // If not found func expression
    if (!(func_expr = maybe_parsed.value())) {
        set_error("expected expression");
        return { };
    }

    maybe_matched = match_token(Token::Type::RightBrace);
    if (maybe_matched.is_error())
        return { };
    // If not matched '}'
    if (!maybe_matched.value()) {
        set_error("expected '}'");
        return { };
    }

    return std::shared_ptr<Instruction>(new FuncInstruction(func_name, parameters, func_expr,
        { start_position, position() }));
}

ErrorOr<std::shared_ptr<Instruction>> Parser::parse_instruction() {
    typedef ErrorOr<std::shared_ptr<Instruction>> (Parser::*ParseInstructionMethod)();
    static ParseInstructionMethod parse_methods[] = {
        &Parser::parse_match_instruction,
        &Parser::parse_rule_instruction,
        &Parser::parse_func_instruction
    };

    for (auto method : parse_methods) {
        auto maybe_result = (this->*method)();
        if (maybe_result.is_error())
            return { };

        std::shared_ptr<Instruction> instruction;
        if ((instruction = maybe_result.value())) {
            auto maybe_match_result = match_newline();
            // If not found a newline
            if (maybe_match_result.is_error())
                return { };
            // If we found a newline
            return instruction;
        }
    }

    return std::shared_ptr<Instruction>(nullptr);
}

ErrorOr<void> Parser::parse_all() {
    // We don't care whether there actually was a newline
    if (match_newline(false).is_error())
        return false;

    std::shared_ptr<Instruction> instruction;
    for (;;) {
        auto maybe_instruction = parse_instruction();
        if (maybe_instruction.is_error())
            return false;
        if (!(instruction = maybe_instruction.value()))
            break;
        m_instructions.push_back(instruction);
    }

    // If not reached eof but finished parsing, we have some trailing nonsense
    if (!is_eof()) {
        set_error("unexpected token");
        return false;
    }

    return true;
}

ParsingError Parser::error() {
    if (m_lexer.has_errored())
        return m_lexer.error();
    assert(has_errored());
    return m_error;
}

void Parser::show_error() {
    std::cout << error().to_string() << std::endl;
}
