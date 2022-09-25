#include "TupleExpr.h"

#include <runtime/values/TupleValue.h>

#include <memory>

TupleExpr::TupleExpr(TextSnippet snippet, std::vector<std::shared_ptr<Expr>> arguments)
    : Expr(snippet), m_arguments(arguments) {
}

ErrorOr<std::shared_ptr<Value>> TupleExpr::eval(RuntimeManager& rtm) {
    // Loop tuple arguments
    std::vector<std::shared_ptr<Value>> values;
    for (auto argument : m_arguments) {
        auto maybe_evaluated = argument->eval(rtm);
        if (maybe_evaluated.is_error())
            return { };
        values.push_back(maybe_evaluated.value());
    }
    return std::shared_ptr<Value>(new TupleValue(values));
}
