#pragma once

#include <utils/TextPosition.h>
#include <cassert>

class TextSnippet {
public:
    TextSnippet(TextPosition start, TextPosition end)
        : m_start(start), m_end(end) { }
    TextSnippet() { }

    TextPosition& start() { return m_start; }
    TextPosition& end() { return m_end; }
    size_t length() {
        size_t start_index = m_start.index();
        size_t end_index = m_end.index();
        assert(end_index >= start_index);
        return end_index - start_index;
    }
    bool similar_to(TextSnippet& snippet) {
        return start().similar_to(snippet.start()) && end().similar_to(snippet.end());
    }

private:
    TextPosition m_start, m_end;
};

