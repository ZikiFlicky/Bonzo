#pragma once

#include <runtime/values/abstract/Value.h>

#include <string>
#include <unordered_map>

class Scope {
public:
    Scope(Scope* prev = nullptr)
        : m_prev(prev) { }
    ~Scope();

    Scope* prev() { return m_prev; }

    bool contains_variable(std::string name);
    void set_variable(std::string name, std::shared_ptr<Value> value);
    std::shared_ptr<Value> get_variable(std::string name);

private:
    Scope* m_prev;
    std::unordered_map<std::string, std::shared_ptr<Value>> m_variables;
};
