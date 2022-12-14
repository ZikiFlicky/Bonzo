#pragma once

#include "Value.h"
#include <utils/ErrorOr.h>
#include <utils/TextSnippet.h>

#include <vector>

// TODO: Maybe move this?
struct ValueSnippetPair {
    std::shared_ptr<Value> value;
    TextSnippet snippet;
};

struct CallInfo {
    std::vector<ValueSnippetPair> arguments;
    TextSnippet call_snippet;
    RuntimeManager& rtm;
};

class CallableValue : public Value {
public:
    CallableValue(std::string name)
        : Value(), m_name(name) { }

    std::string name() { return m_name.empty() ? "<unnamed>" : m_name; }

    bool is_callable() override { return true; }

    virtual ErrorOr<std::shared_ptr<Value>> call(CallInfo& info) = 0;

private:
    std::string m_name;
};
