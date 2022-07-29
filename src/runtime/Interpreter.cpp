#include "Interpreter.h"
#include <runtime/values/ExtFuncValue.h>
#include <runtime/values/TupleValue.h>
#include <runtime/values/OrValue.h>
#include <runtime/values/ArbitraryLengthValue.h>
#include <runtime/values/SeparatedValue.h>
#include <runtime/values/SpecialValue.h>
#include <runtime/values/StringValue.h>
#include <runtime/values/SumValue.h>
#include <runtime/values/CharChoiceValue.h>
#include <runtime/values/OptionalValue.h>
#include <runtime/values/SomeValue.h>

#include <iostream>

Interpreter::Interpreter(std::vector<std::shared_ptr<Instruction>>& instructions, std::string& compare_text)
    : m_instructions(instructions), m_compare_text(compare_text) {
    // Set base functions
    set_variable("ArbitraryLength",
        std::make_shared<ExtFuncValue>(*this, &Interpreter::builtin_function_arbitrary_length)
    );
    set_variable("Some",
        std::make_shared<ExtFuncValue>(*this, &Interpreter::builtin_function_some)
    );
    set_variable("Any",
        std::make_shared<ExtFuncValue>(*this, &Interpreter::builtin_function_any)
    );
    set_variable("Separated",
        std::make_shared<ExtFuncValue>(*this, &Interpreter::builtin_function_separated)
    );
    set_variable("Map",
        std::make_shared<ExtFuncValue>(*this, &Interpreter::builtin_function_map)
    );
    set_variable("NoCase",
        std::make_shared<ExtFuncValue>(*this, &Interpreter::builtin_function_nocase)
    );
    set_variable("Optional",
        std::make_shared<ExtFuncValue>(*this, &Interpreter::builtin_function_optional)
    );
    // Set base variables
    set_variable("Spacing",
        std::make_shared<SpecialValue>(*this, SpecialValue::Type::Whitespace)
    );
    set_variable("Character",
        std::make_shared<SpecialValue>(*this, SpecialValue::Type::Character)
    );
    set_variable("Newline",
        std::make_shared<SpecialValue>(*this, SpecialValue::Type::Newline)
    );
}

Interpreter::~Interpreter() {
    leave_scope();
}

void Interpreter::set_error(std::string error_message) {
    m_error_message = error_message;
    m_has_errored = true;
}

void Interpreter::show_error() {
    std::cerr << "RuntimeError: " << m_error_message << std::endl;
}

void Interpreter::enter_new_scope() {
    m_top_scope = new Scope(m_top_scope);
}

void Interpreter::leave_scope() {
    auto prev = m_top_scope->prev();
    delete m_top_scope;
    m_top_scope = prev;
}

void Interpreter::set_variable(std::string name, std::shared_ptr<RegValue> value) {
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

void Interpreter::set_local_variable(std::string name, std::shared_ptr<RegValue> value) {
    m_top_scope->set_variable(name, value);
}

std::shared_ptr<RegValue> Interpreter::get_variable(std::string name) {
    // Loop scopes and if you find the variable name return the value
    for (auto scope = m_top_scope; scope; scope = scope->prev()) {
        if (scope->contains_variable(name))
            return scope->get_variable(name);
    }
    return nullptr;
}

bool Interpreter::expect_arguments_size(std::vector<std::shared_ptr<RegValue>> arguments, size_t size) {
    if (arguments.size() != size) {
        set_error("expected " + std::to_string(size) + " arguments but got " + std::to_string(arguments.size()) + " instead");
        return false;
    }
    return true;
}

ErrorOr<std::shared_ptr<RegValue>> Interpreter::builtin_function_arbitrary_length(std::vector<std::shared_ptr<RegValue>> arguments) {
    if (!expect_arguments_size(arguments, 1))
        return { };
    auto arg = arguments[0];
    if (!verify_matchable(arg))
        return { };
    return std::shared_ptr<RegValue>(new ArbitraryLengthValue(*this, arg));
}

ErrorOr<std::shared_ptr<RegValue>> Interpreter::builtin_function_some(std::vector<std::shared_ptr<RegValue>> arguments) {
    if (!expect_arguments_size(arguments, 1))
        return { };
    if (!verify_matchable(arguments[0]))
        return { };
    return std::shared_ptr<RegValue>(new SomeValue(*this, arguments[0]));
}

ErrorOr<std::shared_ptr<RegValue>> Interpreter::builtin_function_any(std::vector<std::shared_ptr<RegValue>> arguments) {
    if (!expect_arguments_size(arguments, 1))
        return { };
    if (!arguments[0]->is_tuple()) {
        set_error("expected tuple");
        return { };
    }
    auto tuple = arguments[0]->tuple();
    if (!verify_matchable(tuple->values()))
        return { };
    return std::shared_ptr<RegValue>(new OrValue(*this, tuple->values()));
}

ErrorOr<std::shared_ptr<RegValue>> Interpreter::builtin_function_separated(std::vector<std::shared_ptr<RegValue>> arguments) {
    if (!expect_arguments_size(arguments, 2))
        return { };
    if (!arguments[0]->is_tuple()) {
        set_error("expected tuple argument");
        return { };
    }
    auto tuple = arguments[0]->tuple();
    if (!verify_matchable(tuple->values()))
        return { };
    if (!verify_matchable(arguments[1]))
        return { };
    return std::shared_ptr<RegValue>(new SeparatedValue(*this, tuple->values(), arguments[1]));
}

ErrorOr<std::shared_ptr<RegValue>> Interpreter::builtin_function_map(std::vector<std::shared_ptr<RegValue>> arguments) {
    if (!expect_arguments_size(arguments, 2))
        return { };
    if (!arguments[0]->is_callable()) {
        set_error("expected callable argument");
        return { };
    }
    auto callable = arguments[0]->callable();
    if (!arguments[1]->is_tuple()) {
        set_error("expected tuple argument");
        return { };
    }
    auto tuple = arguments[1]->tuple();
    std::vector<std::shared_ptr<RegValue>> mapped_values;
    for (auto value : tuple->values()) {
        auto maybe_mapped = callable->call({ value });
        if (maybe_mapped.is_error())
            return { };
        mapped_values.push_back(maybe_mapped.value());
    }
    // FIXME: Verify arguments are valid
    return std::shared_ptr<RegValue>(new TupleValue(*this, mapped_values));
}

ErrorOr<std::shared_ptr<RegValue>> Interpreter::builtin_function_nocase(std::vector<std::shared_ptr<RegValue>> arguments) {
    if (!expect_arguments_size(arguments, 1))
        return { };

    // Verify we got a string
    if (!arguments[0]->is_string()) {
        set_error("expected string value");
        return { };
    }
    auto string = arguments[0]->string();

    std::vector<std::shared_ptr<RegValue>> summed_values;
    for (size_t i = 0; i < string->string().length(); ++i) {
        std::shared_ptr<RegValue> value;
        auto c = string->string()[i];
        if (isalpha(c)) {
            value = std::shared_ptr<RegValue>(new CharChoiceValue(*this, { tolower(c), toupper(c) }));
        } else {
            // Create substring until next alpha character
            auto start_index = i;
            for(; i < string->string().length(); ++i) {
                if (isalpha(string->string()[i]))
                    break;
            }
            value = std::shared_ptr<RegValue>(new StringValue(*this, string->string().substr(start_index, i - start_index)));
        }
        summed_values.push_back(value);
    }

    // Return sum value
    return std::shared_ptr<RegValue>(new SumValue(*this, summed_values));
}

ErrorOr<std::shared_ptr<RegValue>> Interpreter::builtin_function_optional(std::vector<std::shared_ptr<RegValue>> arguments) {
    if (!expect_arguments_size(arguments, 1))
        return { };

    if (!verify_matchable(arguments[0]))
        return { };
    auto matchable = arguments[0];

    // Return sum value
    return std::shared_ptr<RegValue>(new OptionalValue(*this, matchable));
}

bool Interpreter::verify_matchable(std::shared_ptr<RegValue> value) {
    if (!value->can_be_matched()) {
        // TODO: Better error
        set_error("not matchable");
        return false;
    }
    return true;
}

bool Interpreter::verify_matchable(std::vector<std::shared_ptr<RegValue>> values) {
    for (auto value : values) {
        if (!verify_matchable(value))
            return false;
    }
    return true;
}

ErrorOr<void> Interpreter::run() {
    for (auto instruction : m_instructions) {
        // Quit if errored
        if (instruction->run(*this).is_error())
            return false;
    }
    return true;
}
