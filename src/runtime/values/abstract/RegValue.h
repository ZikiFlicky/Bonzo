#pragma once

#include <memory>
#include <cassert>

#include "utils/ErrorOr.h"

class Interpreter;

class TupleValue;
class StringValue;
class CallableValue;

class RegValue {
public:
    RegValue(Interpreter& interpreter)
        : m_interpreter(interpreter) { }
    virtual ~RegValue() { }

    Interpreter& interpreter() { return m_interpreter; }

    virtual ErrorOr<std::shared_ptr<RegValue>> or_with(std::shared_ptr<RegValue> shared_this, std::shared_ptr<RegValue> rhs);
    virtual ErrorOr<std::shared_ptr<RegValue>> add_with(std::shared_ptr<RegValue> shared_this, std::shared_ptr<RegValue> rhs);
    virtual bool can_be_matched() { return false; }
    virtual std::string generate_regex() { assert(0); }
    virtual bool needs_parens() { return true; }
    std::string generate_regex_as_child();

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

private:
    Interpreter& m_interpreter;
};
