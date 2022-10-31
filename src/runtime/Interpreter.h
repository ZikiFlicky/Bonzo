#pragma once

#include "Scope.h"
#include <ast/instructions/Instruction.h>
#include <utils/ErrorOr.h>
#include <utils/TextPosition.h>
#include <runtime/values/abstract/CallableValue.h>
#include <runtime/values/ExtFuncValue.h>
#include <runtime/RuntimeManager.h>

#include <string>
#include <vector>
#include <stack>

class Expr;

class Interpreter final {
public:
    Interpreter();
    Interpreter(std::string compare_text);
    ~Interpreter();

    void set_instructions(std::vector<std::shared_ptr<Instruction>>& instructions) {
        m_input_type = InstructionsInput;
        m_instructions = instructions;
    }
    void set_expr(std::shared_ptr<Expr> expr) {
        m_input_type = ExprInput;
        m_expr = expr;
    }

    bool has_errored() { return m_rtm.has_errored(); }
    RuntimeError error() { return m_rtm.error(); }
    void show_error();
    std::string output_buffer() { return m_rtm.output_buffer(); }
    void set_buffered() { m_rtm.set_buffered(); }
    std::vector<TextSnippet>& match_snippets() { return m_rtm.match_snippets(); }

    ErrorOr<void> run();

private:
    enum InputType {
        InstructionsInput = 1,
        ExprInput
    };

    RuntimeManager m_rtm;
    InputType m_input_type { InstructionsInput };
    std::vector<std::shared_ptr<Instruction>> m_instructions;
    std::shared_ptr<Expr> m_expr;

    void set_base_variables();

    ErrorOr<void> run_instructions();
    ErrorOr<void> run_expr();
};
