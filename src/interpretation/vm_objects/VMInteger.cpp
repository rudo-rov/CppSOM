#include <string>
#include "../Interpret.h"
#include "VMInteger.h"

namespace som {

    void VMInteger::dispatchPrimitive(const std::string& selector, CodeAddress retAddress, int32_t arity, CInterpret* interpret)
    {
        auto fnIt = integerPrimitives.find(selector);
        if (fnIt == integerPrimitives.end()) {
            // VMClass::dispatchPrimitive(selector, stack);
        }
        auto fn = integerPrimitives.at(selector);
        interpret->executionStack().pushFrame(retAddress, arity + 1);
        fn(this, interpret);
    }

    void VMInteger::asString(CInterpret* interpret)
    {
        auto receiver = interpret->executionStack().getSelf();
        std::string value = std::to_string(receiver->getValue().asInt());
        interpret->executionStack().push(std::make_shared<VMObject>(interpret->globalContext().getClass("String"), VMValue(value)));
    }

    void VMInteger::plus(CInterpret* interpret)
    {
        auto argument = interpret->executionStack().getArgument(0)->getValue().asInt();
        auto receiver = interpret->executionStack().getSelf()->getValue().asInt();
        interpret->executionStack().push(std::make_shared<VMObject>(interpret->globalContext().getClass("Integer"), VMValue(receiver + argument)));
    }

    void VMInteger::minus(CInterpret* interpret)
    {
        auto argument = interpret->executionStack().getArgument(0)->getValue().asInt();
        auto receiver = interpret->executionStack().getSelf()->getValue().asInt();
        interpret->executionStack().push(std::make_shared<VMObject>(interpret->globalContext().getClass("Integer"), VMValue(receiver - argument)));
    }

    void VMInteger::mult(CInterpret* interpret)
    {
        auto argument = interpret->executionStack().getArgument(0)->getValue().asInt();
        auto receiver = interpret->executionStack().getSelf()->getValue().asInt();
        interpret->executionStack().push(std::make_shared<VMObject>(interpret->globalContext().getClass("Integer"), VMValue(receiver * argument)));
    }

    void VMInteger::div(CInterpret* interpret)
    {
        auto argument = interpret->executionStack().getArgument(0)->getValue().asInt();
        auto receiver = interpret->executionStack().getSelf()->getValue().asInt();
        interpret->executionStack().push(std::make_shared<VMObject>(interpret->globalContext().getClass("Integer"), VMValue(receiver / argument)));
    }

    void VMInteger::floatDiv(CInterpret* interpret)
    {
        auto argument = interpret->executionStack().getArgument(0)->getValue().asInt();
        auto receiver = interpret->executionStack().getSelf()->getValue().asInt();
        interpret->executionStack().push(std::make_shared<VMObject>(interpret->globalContext().getClass("Double"), VMValue((double)receiver / argument)));
    }

    void VMInteger::mod(CInterpret* interpret)
    {
        auto argument = interpret->executionStack().getArgument(0)->getValue().asInt();
        auto receiver = interpret->executionStack().getSelf()->getValue().asInt();
        interpret->executionStack().push(std::make_shared<VMObject>(interpret->globalContext().getClass("Integer"), VMValue(receiver % argument)));
    }

    void VMInteger::cmp(CInterpret* interpret)
    {
        auto argument = interpret->executionStack().getArgument(0)->getValue().asInt();
        auto receiver = interpret->executionStack().getSelf()->getValue().asInt();
        if (argument == receiver) {
            interpret->executionStack().push(interpret->globalContext().getTrue());
        } else {
            interpret->executionStack().push(interpret->globalContext().getFalse());
        }
    }

}