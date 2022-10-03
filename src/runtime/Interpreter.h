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

    // Are the two traces equal, depending on only the attributes that can be user-defined
    bool similar_to(const CallTraceItem& trace) {
        return func_name == trace.func_name && position.similar_to(trace.position);
    }
};

enum OperationType {
    GenerateRegex = 1,
    MatchAgainst
};

struct RuntimeError {
    std::string error_message { };
    TextPosition error_position { };
    std::vector<CallTraceItem> call_trace { };

    std::string to_string();
    bool similar_to(const RuntimeError& error) {
        if (error_message != error.error_message)
            return false;
        if (!error_position.similar_to(error.error_position))
            return false;
        if (call_trace.size() != error.call_trace.size())
            return false;
        size_t trace_size = call_trace.size();
        for (size_t i = 0; i < trace_size; ++i) {
            if (!call_trace[i].similar_to(error.call_trace[i]))
                return false;
        }
        return true;
    }
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
    RuntimeError error() {
        assert(has_errored());
        return m_error;
    }
    void print(std::string string);

    ErrorOr<void> expect_arguments_size(CallInfo& info, size_t size);
    ErrorOr<void> verify_matchable(ValueSnippetPair value);
    ErrorOr<void> verify_matchable(std::vector<std::shared_ptr<Value>> values);

private:
    Interpreter& m_interpreter;
    bool m_is_output_buffered { false };
    std::string m_output_buffer;
    OperationType m_operation_type;

    std::stack<CallTraceItem> m_call_trace;
    RuntimeError m_error;
    bool m_has_error { false };

    Scope* m_top_scope { new Scope };
    std::string m_compare_text;

    std::string output_buffer() { return m_output_buffer; }
    void set_buffered() { m_is_output_buffered = true; }
};

class Interpreter final {
public:
    Interpreter();
    Interpreter(std::string compare_text);
    ~Interpreter();

    void set_instructions(std::vector<std::shared_ptr<Instruction>> instructions) { m_instructions = instructions; }

    bool has_errored() { return m_rtm.has_errored(); }
    RuntimeError error() { return m_rtm.error(); }
    void show_error();
    std::string output_buffer() { return m_rtm.output_buffer(); }
    void set_buffered() { m_rtm.set_buffered(); }

    ErrorOr<void> run();

private:
    RuntimeManager m_rtm;
    std::vector<std::shared_ptr<Instruction>> m_instructions;

    void set_base_variables();

    ErrorOr<std::shared_ptr<Value>> builtin_function_arbitrary_length(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_some(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_any(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_separated(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_map(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_nocase(CallInfo& info);
    ErrorOr<std::shared_ptr<Value>> builtin_function_optional(CallInfo& info);
};
