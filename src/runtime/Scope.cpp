#include "Scope.h"
#include "values/abstract/RegValue.h"

Scope::~Scope() {
}

bool Scope::contains_variable(std::string name) {
    return m_variables.contains(name);
}

void Scope::set_variable(std::string name, std::shared_ptr<RegValue> value) {
    m_variables[name] = value;
}

std::shared_ptr<RegValue> Scope::get_variable(std::string name) {
    if (!contains_variable(name))
        return nullptr;
    return m_variables[name];
}
