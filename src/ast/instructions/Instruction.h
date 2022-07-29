#pragma once

#include <utils/ErrorOr.h>

#include <memory>

class Interpreter;

class Instruction {
public:
    Instruction() { }
    virtual ~Instruction() { }

    virtual ErrorOr<void> run(Interpreter& interpreter) = 0;
};
