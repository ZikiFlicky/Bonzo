#pragma once

#include "Value.h"
#include <utils/ErrorOr.h>
#include <utils/TextSnippet.h>

#include <vector>

// TODO: Maybe move this?
struct ValueSnippetPair {
    ValueSnippetPair(std::shared_ptr<Value> v, TextSnippet s)
        : value(v), snippet(s) { }
    std::shared_ptr<Value> value;
    TextSnippet snippet;
};

struct CallInfo {
    CallInfo(std::vector<ValueSnippetPair> args, TextSnippet snippet)
        : arguments(args), call_snippet(snippet) { }
    std::vector<ValueSnippetPair> arguments;
    TextSnippet call_snippet;
};

class CallableValue : public Value {
public:
    CallableValue(Interpreter& interpreter, std::string name)
        : Value(interpreter), m_name(name) { }

    std::string name() { return m_name.empty() ? "<unnamed>" : m_name; }

    bool is_callable() override { return true; }

    virtual ErrorOr<std::shared_ptr<Value>> call(CallInfo info) = 0;

private:
    std::string m_name;
};
