#pragma once

#include <parsing/stream/StreamSnippet.h>
#include <runtime/values/abstract/RegValue.h>
#include <runtime/Interpreter.h>
#include <utils/ErrorOr.h>

class Expr {
public:
    Expr(StreamSnippet snippet)
        : m_snippet(snippet) { }
    virtual ~Expr() { }

    StreamSnippet snippet() { return m_snippet; }
    virtual ErrorOr<std::shared_ptr<RegValue>> eval(Interpreter& interpreter) = 0;

private:
    StreamSnippet m_snippet;
};
