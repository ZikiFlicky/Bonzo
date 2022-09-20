#pragma once

#include <memory>
#include <cassert>
#include <vector>

#include <utils/ErrorOr.h>
#include <utils/TextPosition.h>

class Interpreter;
class TextSnippet;
class MatchHandler;
class MatchState;

class TupleValue;
class StringValue;
class CallableValue;

class Value {
public:
    Value() { }
    virtual ~Value() { }

    virtual ErrorOr<std::shared_ptr<Value>> or_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs,
        TextPosition operator_position, Interpreter& interpreter);
    virtual ErrorOr<std::shared_ptr<Value>> add_with(std::shared_ptr<Value> shared_this, std::shared_ptr<Value> rhs,
        TextPosition operator_position, Interpreter& interpreter);
    virtual bool can_be_matched() { return false; }
    virtual std::string generate_regex() { assert(0); }
    virtual bool needs_parens() { return true; }
    std::string generate_regex_as_child();
    virtual bool try_match(MatchState& state) = 0;

    virtual bool is_tuple() { return false; }
    virtual bool is_string() { return false; }
    virtual bool is_callable() { return false; }

    TupleValue* tuple() {
        assert(is_tuple());
        // FIXME: This should be dynamic_cast?
        return (TupleValue*)(this);
    }

    StringValue* string() {
        assert(is_string());
        return (StringValue*)(this);
    }

    CallableValue* callable() {
        assert(is_callable());
        return (CallableValue*)(this);
    }
};
