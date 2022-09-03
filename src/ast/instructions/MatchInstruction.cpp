#include "MatchInstruction.h"
#include <runtime/Interpreter.h>
#include <runtime/MatchHandler.h>

#include <iostream>


MatchInstruction::MatchInstruction(std::shared_ptr<Expr> expr)
    : Instruction(), m_expr(expr) {
}

MatchInstruction::~MatchInstruction() {
}

ErrorOr<void> MatchInstruction::run(Interpreter& interpreter) {
    auto maybe_value = m_expr->eval(interpreter);
    if (maybe_value.is_error())
        return false;
    auto value = maybe_value.value();
    if (!value->can_be_matched()) {
        interpreter.set_error("input not generable/matchable");
        return false;
    }
    switch (interpreter.operation_type()) {
    case Interpreter::OperationType::GenerateRegex:
        std::cout << "Generated: " << value->generate_regex() << std::endl;
        break;
    case Interpreter::OperationType::MatchAgainst: {
        SearchProvider handler(interpreter.compare_text());
        auto snippets = handler.find_from_value(value);
        std::cout << "Found " << snippets.size() << " matches (not including the last position)" << (snippets.size() > 0 ? ":" : "") << std::endl;
        for (auto& s : snippets)
            std::cout << s.start().to_string() << " -> " << s.end().to_string() << " (length " << s.length() << ")" << std::endl;
        break;
    }
    default:
        assert(0);
    }
    return true; // Ran successfully
}
