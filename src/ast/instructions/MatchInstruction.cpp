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
    switch (rtm.operation_type()) {
    case OperationType::GenerateRegex:
        if (!value->can_be_matched()) {
            rtm.set_error("input not generable", snippet().start());
            return false;
        }
        rtm.print("Generated: " + value->generate_regex() + "\n");
        break;
    case OperationType::MatchAgainst: {
        if (rtm.match_against({ value, m_expr->snippet() }).is_error())
            return false;
        auto& snippets = rtm.matched_snippets();
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
