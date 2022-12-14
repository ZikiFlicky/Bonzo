#pragma once

#include <utils/TextSnippet.h>
#include <runtime/values/abstract/Value.h>
#include <runtime/Interpreter.h>
#include <utils/ErrorOr.h>
#include <utils/TextSnippet.h>

class Expr {
public:
    Expr(TextSnippet snippet)
        : m_snippet(snippet) { }
    virtual ~Expr() { }

    TextSnippet snippet() { return m_snippet; }
    virtual ErrorOr<std::shared_ptr<Value>> eval(RuntimeManager& rtm) = 0;

private:
    TextSnippet m_snippet;
};
