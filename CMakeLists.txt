cmake_minimum_required(VERSION 3.16)

# Set the project name
project(bonzo VERSION 1.0.0 LANGUAGES CXX)

set(BASE_SOURCES
    src/ast/exprs/VariableExpr.cpp
    src/ast/exprs/InvocationExpr.cpp
    src/ast/exprs/abstract/BinExpr.cpp
    src/ast/exprs/OrExpr.cpp
    src/ast/exprs/AddExpr.cpp
    src/ast/exprs/TupleExpr.cpp
    src/ast/exprs/StringExpr.cpp
    src/ast/instructions/FuncInstruction.cpp
    src/ast/instructions/MatchInstruction.cpp
    src/ast/instructions/RuleInstruction.cpp
    src/parsing/Parser.cpp
    src/parsing/Token.cpp
    src/parsing/Lexer.cpp
    src/runtime/values/abstract/CollectionValue.cpp
    src/runtime/values/abstract/Value.cpp
    src/runtime/values/ArbitraryLengthValue.cpp
    src/runtime/values/CharChoiceValue.cpp
    src/runtime/values/ExtFuncValue.cpp
    src/runtime/values/FuncValue.cpp
    src/runtime/values/OptionalValue.cpp
    src/runtime/values/OrValue.cpp
    src/runtime/values/SeparatedValue.cpp
    src/runtime/values/SomeValue.cpp
    src/runtime/values/SumValue.cpp
    src/runtime/values/SpecialValue.cpp
    src/runtime/values/StringValue.cpp
    src/runtime/Interpreter.cpp
    src/runtime/MatchHandler.cpp
    src/runtime/Scope.cpp
)

set(EXEC_SOURCES
    ${BASE_SOURCES}
    src/main.cpp
)

set(TEST_SOURCES
    ${BASE_SOURCES}
    tests/Interface.cpp
    tests/RunTests.cpp
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src/)
add_executable(bonzo ${EXEC_SOURCES})
add_executable(runtests ${TEST_SOURCES})
