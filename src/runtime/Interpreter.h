#pragma once

#include "Scope.h"
#include <ast/instructions/Instruction.h>
#include <utils/ErrorOr.h>
#include <utils/TextPosition.h>
#include <runtime/values/abstract/CallableValue.h>
#include <runtime/values/ExtFuncValue.h>

#include <string>
#include <vector>
#include <stack>

class Interpreter;

struct CallTraceItem {
    TextPosition position;
    std::string func_name;
};

enum OperationType {
    GenerateRegex = 1,
    MatchAgainst
};

class RuntimeManager final {
    friend class Interpreter;
public:
    RuntimeManager(Interpreter& interpreter) : m_interpreter(interpreter), m_operation_type(OperationType::GenerateRegex) { }
    RuntimeManager(Interpreter& interpreter, std::string compare_text)
        : m_interpreter(interpreter), m_operation_type(OperationType::MatchAgainst), m_compare_text(compare_text) { }
    ~RuntimeManager() = default;

    OperationType operation_type() { return m_operation_type; }
    std::string& compare_text() { return m_compare_text; }
    void set_error(std::string error_message, TextPosition position);
    bool has_errored() { return m_has_error; }
    void add_call_trace(CallTraceItem position);
    void remove_call_trace();
    void enter_new_scope();
    void leave_scope();
    void set_variable(std::string name, std::shared_ptr<Value> value);
    void set_local_variable(std::string name, std::shared_ptr<Value> value);
    std::shared_ptr<Value> get_variable(std::string name);
    ErrorOr<std::shared_ptr<Value>> call_builtin(ExtFuncValue::FuncDef func, CallInfo& info);

    ErrorOr<void> expect_arguments_size(CallInfo& info, size_t size);
    ErrorOr<void> verify_matchable(ValueSnippetPair value);
    ErrorOr<void> verify_matchable(std::vector<std::shared_ptr<Value>> values);

private:
    Interpreter& m_interpreter;
    OperationType m_operation_type;
    std::stack<CallTraceItem> m_call_trace;
    bool m_has_error { false };
    std::string m_error_message { };
    TextPosition m_error_position;

    Scope* m_top_scope { new Scope };
    std::string m_compare_text;
};

class Interpreter final {
public:
    Interpreter(std::vector<std::shared_ptr<Instruction>>& instructions);
    Interpreter(std::vector<std::shared_ptr<Instruction>>& instructions, std::string compare_text);
    ~Interpreter();

    bool has_errored() { return m_rtm.has_errored(); }
    void show_error();

    ErrorOr<void> run();

private:
    RuntimeManager m_rtm;
    std::vector<std::shared_ptr<Instruction>>& m_instructions;

    void set_base_variables();

    ErrorOr<std::shared_ptr<Value>> builtin_function_arbitrary_length(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_some(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_any(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_separated(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_map(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_nocase(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_optional(CallInfo& info);
};
