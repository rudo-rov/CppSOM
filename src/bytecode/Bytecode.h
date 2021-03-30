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
        ClassVal
    };
    
    enum class OpCode : uint8_t {
        NoOp,
        LabelOp,
        LitOp,
        PrintOp,
        ArrayOp,
        ObjectOp,
        SlotOp,
        SetSlotOp,
        GetSlotOp,
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

    struct ClassValue : Value {
        ClassValue(std::vector<int32_t> slots) : slots(std::move(slots)) {}
        virtual void print() override;
        virtual void serialize(std::ofstream& file) override;
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

    struct PrintIns : ByteIns {

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

    struct CallSlotIns : ByteIns {
        CallSlotIns(int32_t methodIdx, int32_t arity) : ByteIns(OpCode::CallSlotOp), methodIdx(methodIdx), arity(arity) {}
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
