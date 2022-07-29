#pragma once

#include "RegValue.h"

#include <vector>

class CollectionValue : public RegValue {
public:
    CollectionValue(Interpreter& interpreter, std::vector<std::shared_ptr<RegValue>> values);
    ~CollectionValue() override;

    std::vector<std::shared_ptr<RegValue>> values() { return m_values; }

protected:
    bool can_values_be_matched();

private:
    std::vector<std::shared_ptr<RegValue>> m_values;
};
