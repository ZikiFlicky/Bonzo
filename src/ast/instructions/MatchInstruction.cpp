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
        rtm.print("Generated: " + value->generate_regex() + "\n");
        break;
    case OperationType::MatchAgainst: {
        SearchProvider handler(rtm.compare_text());
        auto snippets = handler.find_from_value(value);
        rtm.print("Found " + std::to_string(snippets.size()) + " matches" + (snippets.size() > 0 ? ":" : std::string()) + "\n");
        for (auto& s : snippets)
            rtm.print(s.start().to_string() + " -> " + s.end().previous_position().to_string() + " (length " + std::to_string(s.length()) + ")" + "\n");
        break;
    }
    default:
        assert(0);
    }
    return true; // Ran successfully
}
