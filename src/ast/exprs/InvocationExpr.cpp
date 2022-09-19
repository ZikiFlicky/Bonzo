#include "InvocationExpr.h"
#include <runtime/values/FuncValue.h>

InvocationExpr::InvocationExpr(TextSnippet snippet, std::shared_ptr<Expr> invoked, std::vector<std::shared_ptr<Expr>> arguments)
    : Expr(snippet), m_invoked(invoked), m_arguments(arguments) {
}

InvocationExpr::~InvocationExpr() {
}

ErrorOr<std::shared_ptr<Value>> InvocationExpr::eval(Interpreter& interpreter) {
    (void)interpreter;
    auto maybe_value = m_invoked->eval(interpreter);
    // If errored
    if (maybe_value.is_error())
        return { };
    auto value = maybe_value.value();
    // Verify value is callable
    if (!value->is_callable()) {
        // TODO: Maybe it's better to do this kind of stuff with snippets
        interpreter.set_error("tried to invoke non-callable", snippet().start());
        return { };
    }

    auto callable = value->callable();
    // Loop tuple arguments
    std::vector<ValueSnippetPair> arguments;
    for (auto argument : m_arguments) {
        auto maybe_evaluated = argument->eval(interpreter);
        if (maybe_evaluated.is_error())
            return { };
        arguments.push_back({ maybe_evaluated.value(), argument->snippet() });
    }
    CallInfo info(arguments, snippet());

    interpreter.add_call_trace({ snippet().start(), callable->name() });
    // Finally call the callable
    auto return_value = callable->call(info);
    if (return_value.is_error())
        return {};
    // If succeeded, it means we aren't backtracking an error so we can remove the call position
    interpreter.remove_call_trace();
    return return_value.value();
}
