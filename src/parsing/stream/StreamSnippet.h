#pragma once

#include "State.h"

class StreamSnippet {
public:
    StreamSnippet(State start_state, size_t length)
        : m_start_state(start_state), m_length(length) { }
    ~StreamSnippet() { }

    State start_state() { return m_start_state; }
    size_t length() { return m_length; }

private:
    State m_start_state;
    size_t m_length;
};
