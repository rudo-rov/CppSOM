#include <string>
#include "VMInteger.h"

namespace som {

    void VMInteger::dispatchPrimitive(const std::string& selector, CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto fnIt = integerPrimitives.find(selector);
        if (fnIt == integerPrimitives.end()) {
            // VMClass::dispatchPrimitive(selector, stack);
        }
        auto fn = integerPrimitives.at(selector);
        fn(this, stack, globalCtx);
    }

    void VMInteger::asString(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto receiver = stack.pop();
        std::string value = std::to_string(receiver->getValue().asInt());
        stack.push(std::make_shared<VMObject>(globalCtx.getClass("String"), VMValue(value)));
    }

    void VMInteger::plus(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto argument = stack.pop()->getValue().asInt();
        auto receiver = stack.pop()->getValue().asInt();
        stack.push(std::make_shared<VMObject>(globalCtx.getClass("Integer"), VMValue(receiver + argument)));
    }

    void VMInteger::minus(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto argument = stack.pop()->getValue().asInt();
        auto receiver = stack.pop()->getValue().asInt();
        stack.push(std::make_shared<VMObject>(globalCtx.getClass("Integer"), VMValue(receiver - argument)));
    }

    void VMInteger::mult(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto argument = stack.pop()->getValue().asInt();
        auto receiver = stack.pop()->getValue().asInt();
        stack.push(std::make_shared<VMObject>(globalCtx.getClass("Integer"), VMValue(receiver * argument)));
    }

    void VMInteger::div(CExecutionStack& stack, CGlobalContext& globalCtx)
    {
        auto argument = stack.pop()->getValue().asInt();
        auto receiver = stack.pop()->getValue().asInt();
        stack.push(std::make_shared<VMObject>(globalCtx.getClass("Integer"), VMValue(receiver / argument)));
    }

}