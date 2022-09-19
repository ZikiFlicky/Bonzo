#pragma once

#include <utils/ErrorOr.h>
#include <utils/TextSnippet.h>

#include <memory>

class Interpreter;

class Instruction {
public:
    Instruction(TextSnippet snippet)
        : m_snippet(snippet) { }
    virtual ~Instruction() { }

    virtual ErrorOr<void> run(Interpreter& interpreter) = 0;

protected:
    TextSnippet snippet() { return m_snippet; }

private:
    TextSnippet m_snippet;
};
