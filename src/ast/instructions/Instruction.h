#pragma once

#include <utils/ErrorOr.h>
#include <utils/TextSnippet.h>

#include <memory>

class RuntimeManager;

class Instruction {
public:
    Instruction(TextSnippet snippet)
        : m_snippet(snippet) { }
    virtual ~Instruction() { }

    virtual ErrorOr<void> run(RuntimeManager& rtm) = 0;

protected:
    TextSnippet snippet() { return m_snippet; }

private:
    TextSnippet m_snippet;
};
