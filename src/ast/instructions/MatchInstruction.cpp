#include "MatchInstruction.h"
#include <runtime/Interpreter.h>
#include <runtime/MatchHandler.h>

#include <iostream>


MatchInstruction::MatchInstruction(std::shared_ptr<Expr> expr, TextSnippet snippet)
    : Instruction(snippet), m_expr(expr) {
}

MatchInstruction::~MatchInstruction() {
}

ErrorOr<void> MatchInstruction::run(RuntimeManager& rtm) {
    auto maybe_value = m_expr->eval(rtm);
    if (maybe_value.is_error())
        return false;
    auto value = maybe_value.value();
    if (!value->can_be_matched()) {
        rtm.set_error("input not generable/matchable", snippet().start());
        return false;
    }
    switch (rtm.operation_type()) {
    case OperationType::GenerateRegex:
        std::cout << "Generated: " << value->generate_regex() << std::endl;
        break;
    case OperationType::MatchAgainst: {
        SearchProvider handler(rtm.compare_text());
        auto snippets = handler.find_from_value(value);
        std::cout << "Found " << snippets.size() << " matches" << (snippets.size() > 0 ? " (not including the last position):" : "") << std::endl;
        for (auto& s : snippets)
            std::cout << s.start().to_string() << " -> " << s.end().to_string() << " (length " << s.length() << ")" << std::endl;
        break;
    }
    default:
        assert(0);
    }
    return true; // Ran successfully
}
