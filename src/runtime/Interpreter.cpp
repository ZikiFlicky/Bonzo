#include "Interpreter.h"
#include <runtime/RuntimeManager.h>
#include <runtime/values/SpecialValue.h>
#include <ast/exprs/abstract/Expr.h>

#include <iostream>

Interpreter::Interpreter()
    : m_rtm() {
    set_base_variables();
}

Interpreter::Interpreter(std::string compare_text)
    : m_rtm(compare_text) {
    set_base_variables();
}

Interpreter::~Interpreter() {
    m_rtm.leave_scope();
}

void Interpreter::set_base_variables() {
    struct {
        std::string name;
        ExtFuncValue::FuncDef def;
    } func_defs[] =  {
        { "ArbitraryLength", &RuntimeManager::builtin_function_arbitrary_length },
        { "Some", &RuntimeManager::builtin_function_some },
        { "Any", &RuntimeManager::builtin_function_any },
        { "Separated", &RuntimeManager::builtin_function_separated },
        { "Map", &RuntimeManager::builtin_function_map },
        { "NoCase", &RuntimeManager::builtin_function_nocase },
        { "Optional", &RuntimeManager::builtin_function_optional }
    };
    // Set base functions
    for (auto def : func_defs) {
        auto func = std::make_shared<ExtFuncValue>(def.name, def.def);
        m_rtm.set_variable(def.name, func);
    }
    // Set base variables
    m_rtm.set_variable("Spacing",
        std::make_shared<SpecialValue>(SpecialValue::Type::Whitespace)
    );
    m_rtm.set_variable("Character",
        std::make_shared<SpecialValue>(SpecialValue::Type::Character)
    );
    m_rtm.set_variable("Newline",
        std::make_shared<SpecialValue>(SpecialValue::Type::Newline)
    );
}

void Interpreter::show_error() {
    assert(has_errored());
    m_rtm.print(error().to_string());
}

ErrorOr<void> Interpreter::run_instructions() {
    for (auto instruction : m_instructions) {
        // Quit if errored
        if (instruction->run(m_rtm).is_error())
            return false;
    }
    return true;
}

ErrorOr<void> Interpreter::run_expr() {
    assert(m_rtm.operation_type() == MatchAgainst);
    // Evaluate expr into a value
    auto maybe_value = m_expr->eval(m_rtm);
    if (maybe_value.is_error())
        return false;
    // Try to match
    auto value = maybe_value.value();
    if (m_rtm.match_against({ value, m_expr->snippet() }).is_error())
        return false;
    return true;
}

ErrorOr<void> Interpreter::run() {
    switch (m_input_type) {
    case InstructionsInput:
        return run_instructions();
    case ExprInput:
        return run_expr();
    default:
        assert(0);
    }
}
