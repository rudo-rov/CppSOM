#pragma once

#include <cinttypes>
#include <string>
#include <vector>

namespace som {

    enum class ValueTag : uint8_t {
        IntVal,
        DoubleVal,
        NilVal,
        StringVal,
        MethodVal,
        ClassVal
    };
    
    enum class OpCode : uint8_t {
        LabelOp,
        LitOp,
        PrintOp,
        ArrayOp,
        ObjectOp,
        SlotOp,
        SetSlotOp,
        CallSlotOp,
        SetLocalOp,
        GetLocalOp,
        SetGlobalOp,
        GetGlobalOp,
        GotoOp,
        ReturnOp
    };

    // Values
    struct Value {
        Value() : tag(ValueTag::NilVal) {}
        Value(ValueTag t) : tag(t) {}
        virtual void print() {}
        ValueTag tag;
    };

    struct ByteIns {
        OpCode op;
    };

    struct IntValue : Value {
        IntValue(int32_t val) : Value(ValueTag::IntVal), value(val) {}
        virtual void print() override;
        int32_t value;
    };

    struct DoubleValue : Value {
        DoubleValue(double val) : Value(ValueTag::DoubleVal), value(val) {}
        virtual void print() override;
        double value;
    };

    struct StringValue : Value {
        StringValue(const std::string& val) : Value(ValueTag::StringVal), value(val) {}
        virtual void print() override;
        std::string value;
    };

    struct MethodValue : Value {
        MethodValue(int32_t name, int32_t nargs, int32_t nlocals, const std::vector<ByteIns> code) :
            Value(ValueTag::MethodVal), name(name), nargs(nargs), nlocals(nlocals), code(std::move(code)) {}
        virtual void print() override;
        int32_t name;
        int32_t nargs;
        int32_t nlocals;
        std::vector<ByteIns> code;
    };

    struct ClassValue : Value {
        ClassValue(std::vector<int32_t> slots) : slots(std::move(slots)) {}
        virtual void print() override;
        std::vector<int32_t> slots;
    };

    struct NilValue : Value {
        NilValue() : Value(ValueTag::NilVal) {}
    };

    // Opcodes

    struct LabelIns : ByteIns {
        int name;
    };

    struct LitIns : ByteIns {

    };

    struct PrintIns : ByteIns {

    };

    struct ArrayIns : ByteIns {

    };

    struct ObjectIns : ByteIns {

    };

    struct SlotIns : ByteIns {

    };

    struct SetSlotIns : ByteIns {

    };

    struct CallSlotIns : ByteIns {

    };

    struct SetLocalIns : ByteIns {

    };

    struct GetLocalIns : ByteIns {

    };

    struct SetGlobalIns : ByteIns {

    };

    struct GetGlobalIns : ByteIns {

    };

    struct BranchIns : ByteIns {

    };

    struct GotoIns : ByteIns {

    };

    struct ReturnIns : ByteIns {

    };

}
