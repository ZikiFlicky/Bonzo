#include "RuntimeManager.h"
#include <runtime/values/ArbitraryLengthValue.h>
#include <runtime/values/SomeValue.h>
#include <runtime/values/OrValue.h>
#include <runtime/values/SeparatedValue.h>
#include <runtime/values/TupleValue.h>
#include <runtime/values/CharChoiceValue.h>
#include <runtime/values/StringValue.h>
#include <runtime/values/SumValue.h>
#include <runtime/values/OptionalValue.h>


ErrorOr<std::shared_ptr<Value>> RuntimeManager::builtin_function_arbitrary_length(CallInfo& info) {
    if (expect_arguments_size(info, 1).is_error())
        return { };
    auto arg = info.arguments[0];
    if (verify_matchable(arg).is_error())
        return { };
    return std::shared_ptr<Value>(new ArbitraryLengthValue(arg.value));
}

ErrorOr<std::shared_ptr<Value>> RuntimeManager::builtin_function_some(CallInfo& info) {
    if (expect_arguments_size(info, 1).is_error())
        return { };
    if (verify_matchable(info.arguments[0]).is_error())
        return { };
    return std::shared_ptr<Value>(new SomeValue(info.arguments[0].value));
}

ErrorOr<std::shared_ptr<Value>> RuntimeManager::builtin_function_any(CallInfo& info) {
    if (expect_arguments_size(info, 1).is_error())
        return { };
    auto arg1 = info.arguments[0];
    if (!arg1.value->is_tuple()) {
        set_error("expected tuple", arg1.snippet.start());
        return { };
    }
    auto tuple = arg1.value->tuple();
    if (verify_matchable(tuple->values()).is_error())
        return { };
    return std::shared_ptr<Value>(new OrValue(tuple->values()));
}

ErrorOr<std::shared_ptr<Value>> RuntimeManager::builtin_function_separated(CallInfo& info) {
    if (expect_arguments_size(info, 2).is_error())
        return { };
    auto& arg1 = info.arguments[0];
    if (!arg1.value->is_tuple()) {
        set_error("expected tuple argument", arg1.snippet.start());
        return { };
    }
    auto tuple = arg1.value->tuple();
    if (verify_matchable(tuple->values()).is_error())
        return { };
    auto& arg2 = info.arguments[1];
    if (verify_matchable(arg2).is_error())
        return { };
    return std::shared_ptr<Value>(new SeparatedValue(tuple->values(), arg2.value));
}

ErrorOr<std::shared_ptr<Value>> RuntimeManager::builtin_function_map(CallInfo& info) {
    if (expect_arguments_size(info, 2).is_error())
        return { };
    auto& arg1 = info.arguments[0];
    if (!arg1.value->is_callable()) {
        // FIXME: Should be the argument's snippet
        set_error("expected callable argument", arg1.snippet.start());
        return { };
    }
    auto callable = arg1.value->callable();
    auto& arg2 = info.arguments[1];
    if (!arg2.value->is_tuple()) {
        set_error("expected tuple argument", arg2.snippet.start());
        return { };
    }
    auto tuple = arg2.value->tuple();
    std::vector<std::shared_ptr<Value>> mapped_values;
    for (auto value : tuple->values()) {
        CallInfo call_info = { { { value, { } } }, { }, *this };
        auto maybe_mapped = callable->call(call_info);
        if (maybe_mapped.is_error())
            return { };
        mapped_values.push_back(maybe_mapped.value());
    }
    // FIXME: Verify arguments are valid
    return std::shared_ptr<Value>(new TupleValue(mapped_values));
}

ErrorOr<std::shared_ptr<Value>> RuntimeManager::builtin_function_nocase(CallInfo& info) {
    if (expect_arguments_size(info, 1).is_error())
        return { };

    auto& arg1 = info.arguments[0];
    // Verify we got a string
    if (!arg1.value->is_string()) {
        set_error("expected string value", info.call_snippet.start());
        return { };
    }
    auto string = arg1.value->string();

    std::vector<std::shared_ptr<Value>> summed_values;
    for (size_t i = 0; i < string->string().length(); ++i) {
        std::shared_ptr<Value> value;
        auto c = string->string()[i];
        if (isalpha(c)) {
            value = std::shared_ptr<Value>(new CharChoiceValue({ tolower(c), toupper(c) }));
        } else {
            // Create substring until next alpha character
            auto start_index = i;
            for(; i < string->string().length(); ++i) {
                if (isalpha(string->string()[i]))
                    break;
            }
            value = std::shared_ptr<Value>(new StringValue(string->string().substr(start_index, i - start_index)));
        }
        summed_values.push_back(value);
    }

    // Return sum value
    return std::shared_ptr<Value>(new SumValue(summed_values));
}

ErrorOr<std::shared_ptr<Value>> RuntimeManager::builtin_function_optional(CallInfo& info) {
    if (expect_arguments_size(info, 1).is_error())
        return { };

    auto& arg1 = info.arguments[0];
    if (verify_matchable(arg1).is_error())
        return { };
    auto matchable = arg1.value;

    // Return sum value
    return std::shared_ptr<Value>(new OptionalValue(matchable));
}

ErrorOr<void> RuntimeManager::match_against(ValueSnippetPair pair) {
    if (!pair.value->can_be_matched()) {
        set_error("input not generable", pair.snippet.start());
        return false;
    }
    SearchProvider provider(compare_text());
    m_match_snippets = provider.find_from_value(pair.value);
    return true;
}

void RuntimeManager::set_error(std::string error_message, TextPosition position) {
    assert(!has_errored());
    m_has_error = true;
    // Create vector call trace
    auto call_trace_stack_copy = m_call_trace;
    std::vector<CallTraceItem> call_trace;
    while (!call_trace_stack_copy.empty()) {
        call_trace.push_back(call_trace_stack_copy.top());
        call_trace_stack_copy.pop();
    }
    m_error = { error_message, position, call_trace };
}

void RuntimeManager::add_call_trace(CallTraceItem call_trace) {
    m_call_trace.push(call_trace);
}

void RuntimeManager::remove_call_trace() {
    m_call_trace.pop();
}

void RuntimeManager::enter_new_scope() {
    m_top_scope = new Scope(m_top_scope);
}

void RuntimeManager::leave_scope() {
    auto prev = m_top_scope->prev();
    delete m_top_scope;
    m_top_scope = prev;
}

void RuntimeManager::set_variable(std::string name, std::shared_ptr<Value> value) {
    // Loop scopes and if you find the variable name replace it with the value
    for (auto scope = m_top_scope; scope; scope = scope->prev()) {
        if (scope->contains_variable(name)) {
            scope->set_variable(name, value);
            return;
        }
    }
    // If nothing was found set in the top scope
    m_top_scope->set_variable(name, value);
}

void RuntimeManager::set_local_variable(std::string name, std::shared_ptr<Value> value) {
    m_top_scope->set_variable(name, value);
}

std::shared_ptr<Value> RuntimeManager::get_variable(std::string name) {
    // Loop scopes and if you find the variable name return the value
    for (auto scope = m_top_scope; scope; scope = scope->prev()) {
        if (scope->contains_variable(name))
            return scope->get_variable(name);
    }
    return nullptr;
}

ErrorOr<void> RuntimeManager::expect_arguments_size(CallInfo& info, size_t size) {
    if (info.arguments.size() != size) {
        set_error("expected " + std::to_string(size) + " arguments but got " + std::to_string(info.arguments.size()) + " instead",
            info.call_snippet.start());
        return false;
    }
    return true;
}

ErrorOr<std::shared_ptr<Value>> RuntimeManager::call_builtin(ExtFuncValue::FuncDef func, CallInfo& info) {
    return (this->*func)(info);
}

ErrorOr<void> RuntimeManager::verify_matchable(ValueSnippetPair value) {
    if (!value.value->can_be_matched()) {
        // TODO: Better error
        set_error("not matchable", value.snippet.start());
        return false;
    }
    return true;
}

ErrorOr<void> RuntimeManager::verify_matchable(std::vector<std::shared_ptr<Value>> values) {
    for (auto value : values) {
        ValueSnippetPair pair = { value, {} };
        if (verify_matchable(pair).is_error())
            return false;
    }
    return true;
}

void RuntimeManager::print(std::string string) {
    if (m_is_output_buffered)
        m_output_buffer += string;
    else
        std::cout << string;
}

std::string RuntimeError::to_string() {
    std::string error_string = "RuntimeError at " + error_position.to_string() + ": " + error_message + "\n";
    TextPosition position;
    for (size_t i = 0; i < call_trace.size(); ++i) {
        auto trace = call_trace[call_trace.size() - i - 1];
        // If this is the first one, call it a "trigger"
        if (position.valid()) {
            error_string += "  Called from `" + trace.func_name + "` at " + position.to_string();
        } else {
            error_string += "  Triggered in `" + trace.func_name + "`";
        }
        error_string += "\n";
        position = trace.position;
    }
    if (position.valid())
        error_string += "  Called from " + position.to_string() + "\n";
    return error_string;
}
