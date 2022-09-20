#pragma once

#include "Value.h"

#include <vector>

class CollectionValue : public Value {
public:
    CollectionValue(std::vector<std::shared_ptr<Value>> values);
    ~CollectionValue() override;

    std::vector<std::shared_ptr<Value>> values() { return m_values; }

protected:
    bool can_values_be_matched();

private:
    std::vector<std::shared_ptr<Value>> m_values;
};
