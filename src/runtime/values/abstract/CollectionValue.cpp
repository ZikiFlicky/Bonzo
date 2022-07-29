#include "CollectionValue.h"

CollectionValue::CollectionValue(Interpreter& interpreter, std::vector<std::shared_ptr<RegValue>> values)
    : RegValue(interpreter), m_values(values) {
}

CollectionValue::~CollectionValue() {
}

bool CollectionValue::can_values_be_matched() {
    for (auto value : values()) {
        if (!value->can_be_matched())
            return false;
    }
    return true;
}
