#include "Scope.h"
#include "values/abstract/Value.h"

Scope::~Scope() {
}

bool Scope::contains_variable(std::string name) {
    return m_variables.contains(name);
}

void Scope::set_variable(std::string name, std::shared_ptr<Value> value) {
    m_variables[name] = value;
}

std::shared_ptr<Value> Scope::get_variable(std::string name) {
    if (!contains_variable(name))
        return nullptr;
    return m_variables[name];
}
