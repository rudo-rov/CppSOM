#pragma once

#include <cinttypes>
#include <string>
#include <vector>
#include <fstream>

namespace som {

    enum class ValueTag : uint8_t {
        IntVal,
        DoubleVal,
        NilVal,
        StringVal,
        MethodVal,
        PrimitiveVal,
        ClassVal
    };
    
    enum class OpCode : uint8_t {
        NoOp,
        LabelOp,
        LitOp,
        ArrayOp,
        ObjectOp,
        SlotOp,
        SetSlotOp,
        GetSlotOp,
        SendOp,
        SetLocalOp,
        GetLocalOp,
        SetGlobalOp,
        GetGlobalOp,
        GetArgOp,
        GetSelfOp,
        ReturnOp
    };

    // Values
    struct Value {
        Value() : tag(ValueTag::NilVal) {}
        Value(ValueTag t) : tag(t) {}
        virtual void print() {}
        virtual void serialize(std::ofstream& file);
        ValueTag tag;
    };

    struct ByteIns {
        ByteIns() : op(OpCode::NoOp) {}
        ByteIns(OpCode opCode) : op(opCode) {}
        virtual void print();
        virtual void serialize(std::ofstream& file);
        OpCode op;
    };
    typedef std::vector<std::unique_ptr<ByteIns>> insVector;

    struct IntValue : Value {
        IntValue(int32_t val) : Value(ValueTag::IntVal), value(val) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t value;
    };

    struct DoubleValue : Value {
        DoubleValue(double val) : Value(ValueTag::DoubleVal), value(val) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        double value;
    };

    struct StringValue : Value {
        StringValue(const std::string& val) : Value(ValueTag::StringVal), value(val) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        std::string value;
    };

    struct MethodValue : Value {
        MethodValue(int32_t name, int32_t nargs, int32_t nlocals, insVector* code) :
            Value(ValueTag::MethodVal), name(name), nargs(nargs), nlocals(nlocals), code(code) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t name;
        int32_t nargs;
        int32_t nlocals;
        std::unique_ptr<std::vector<std::unique_ptr<ByteIns>>> code;
    };

    struct PrimitiveValue : Value {
        PrimitiveValue(int32_t name, int32_t nargs) : Value(ValueTag::PrimitiveVal), name(name), nargs(nargs) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t name;
        int32_t nargs;
    };

    struct ClassValue : Value {
        ClassValue(int32_t identifier, std::vector<int32_t> slots) : Value(ValueTag::ClassVal), identifier(identifier), slots(std::move(slots)) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t identifier;
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
        LitIns(int32_t idx) : ByteIns(OpCode::LitOp), idx(idx) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t idx;
    };

    struct ArrayIns : ByteIns {

    };

    struct ObjectIns : ByteIns {

    };

    struct SlotIns : ByteIns {
        SlotIns(int32_t slotIdx) : ByteIns(OpCode::SlotOp), slotIdx(slotIdx) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t slotIdx;
    };

    struct SetSlotIns : ByteIns {
        SetSlotIns(int32_t slotIdx) : ByteIns(OpCode::SetSlotOp), slotIdx(slotIdx) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t slotIdx;
    };

    struct GetSlotIns : ByteIns {
        GetSlotIns(int32_t slotIdx) : ByteIns(OpCode::GetSlotOp), slotIdx(slotIdx) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t slotIdx;
    };

    struct SendIns : ByteIns {
        SendIns(int32_t methodIdx, int32_t arity) : ByteIns(OpCode::SendOp), methodIdx(methodIdx), arity(arity) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t methodIdx;
        int32_t arity;
    };

    struct SetLocalIns : ByteIns {
        SetLocalIns(int32_t idx) : ByteIns(OpCode::SetLocalOp), idx(idx) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t idx;
    };

    struct GetLocalIns : ByteIns {
        GetLocalIns(int32_t idx) : ByteIns(OpCode::GetLocalOp), idx(idx) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t idx;
    };

    struct GetArgIns : ByteIns {
        GetArgIns(int32_t idx) : ByteIns(OpCode::GetArgOp), idx(idx) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
        int32_t idx;
    };

    struct GetSelfIns : ByteIns {
        GetSelfIns() : ByteIns(OpCode::GetSelfOp) {}
        virtual void print() override;
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
        ReturnIns() : ByteIns(OpCode::ReturnOp) {}
        void print() override;
        void serialize(std::ofstream& file) override;
    };

}
