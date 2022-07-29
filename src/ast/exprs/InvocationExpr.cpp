#include "InvocationExpr.h"
#include <runtime/values/FuncValue.h>

InvocationExpr::InvocationExpr(StreamSnippet snippet, std::shared_ptr<Expr> invoked, std::vector<std::shared_ptr<Expr>> arguments)
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
        interpreter.set_error("tried to invoke non-callable");
        return { };
    }
    auto callable = value->callable();
    // Loop tuple arguments
    std::vector<std::shared_ptr<Value>> arguments;
    for (auto argument : m_arguments) {
        auto maybe_evaluated = argument->eval(interpreter);
        if (maybe_evaluated.is_error())
            return { };
        arguments.push_back(maybe_evaluated.value());
    }
    // FIXME: This should be cast to a Callable type
    // Finally call the callable
    return callable->call(arguments);
}
