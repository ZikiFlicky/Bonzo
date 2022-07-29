#pragma once

#include "Scope.h"
#include <ast/instructions/Instruction.h>
#include <utils/ErrorOr.h>

#include <string>
#include <vector>

class Interpreter final {
public:
    Interpreter(std::vector<std::shared_ptr<Instruction>>& instructions, std::string& compare_text);
    ~Interpreter();

    bool has_errored() { return m_has_errored; }

    void set_error(std::string error_message);
    void show_error();
    void enter_new_scope();
    void leave_scope();
    void set_variable(std::string name, std::shared_ptr<Value> value);
    void set_local_variable(std::string name, std::shared_ptr<Value> value);
    std::shared_ptr<Value> get_variable(std::string name);
    bool expect_arguments_size(std::vector<std::shared_ptr<Value>> arguments, size_t size);

    template<typename T>
    T* verify_type(std::shared_ptr<Value> value, std::string error) {
        T* casted = dynamic_cast<T*>(value);
        if (!casted)
            set_error(error);
        return casted;
    }
    bool verify_matchable(std::shared_ptr<Value> value);
    bool verify_matchable(std::vector<std::shared_ptr<Value>> values);

    ErrorOr<void> run();

private:
    bool m_has_errored { false };
    std::string m_error_message { };

    std::vector<std::shared_ptr<Instruction>>& m_instructions;
    std::string& m_compare_text;
    Scope* m_top_scope { new Scope };

    ErrorOr<std::shared_ptr<Value>> builtin_function_arbitrary_length(std::vector<std::shared_ptr<Value>> arguments);
    ErrorOr<std::shared_ptr<Value>> builtin_function_some(std::vector<std::shared_ptr<Value>> arguments);
    ErrorOr<std::shared_ptr<Value>> builtin_function_any(std::vector<std::shared_ptr<Value>> arguments);
    ErrorOr<std::shared_ptr<Value>> builtin_function_separated(std::vector<std::shared_ptr<Value>> arguments);
    ErrorOr<std::shared_ptr<Value>> builtin_function_map(std::vector<std::shared_ptr<Value>> arguments);
    ErrorOr<std::shared_ptr<Value>> builtin_function_nocase(std::vector<std::shared_ptr<Value>> arguments);
    ErrorOr<std::shared_ptr<Value>> builtin_function_optional(std::vector<std::shared_ptr<Value>> arguments);
};
