#include <memory>
#include <sstream>

#include "Program.h"
#include "BytecodeLoader.h"

namespace som {

    CBytecodeLoader::CBytecodeLoader(const std::string& fileName)
    {
        m_fileName = fileName;
        m_file.open(m_fileName, std::ios::binary);
        m_program = std::make_unique<Program>();
    }

    CBytecodeLoader::~CBytecodeLoader()
    {
        if (m_file) {
            m_file.close();
        }
    }

    std::unique_ptr<Program>&& CBytecodeLoader::load()
    {
        if (!m_file) {
            std::stringstream err;
            err << "Error opening bytecode file: " << m_fileName << std::endl;
            m_lastError = err.str();
            return std::move(std::unique_ptr<Program>(nullptr));
        }

        int32_t entryPoint;
        m_file.read(reinterpret_cast<char*>(&entryPoint), sizeof entryPoint);
        m_program->setEntryPoint(entryPoint);

        int8_t tag;
        while(!m_file.eof()) {
            m_file.read(reinterpret_cast<char*>(&tag), sizeof tag);
            switch (ValueTag(tag))
            {
            case ValueTag::IntVal:
                loadIntVal();
                break;
            case ValueTag::DoubleVal:
                loadDoubleVal();
                break;
            case ValueTag::StringVal:
                loadStringVal();
                break;
            case ValueTag::MethodVal:
                loadMethodVal();
                break;
            case ValueTag::ClassVal:
                loadClassVal();
                break;
            
            default:
                m_lastError = "Unknown tag occurred.";
                return std::move(m_program);
            }
        }
        return std::move(m_program);
    }

    void CBytecodeLoader::loadIntVal()
    {
        int32_t value;
        m_file.read(reinterpret_cast<char*>(&value), sizeof value);
        m_program->registerConstant(value);
    }

    void CBytecodeLoader::loadDoubleVal()
    {
        double value;
        m_file.read(reinterpret_cast<char*>(&value), sizeof value);
        m_program->registerConstant(value);
    }

    void CBytecodeLoader::loadStringVal()
    {
        int32_t size;
        m_file.read(reinterpret_cast<char*>(&size), sizeof size);
        char* cStrVal = new char[size + 1];
        m_file.read(cStrVal, size);
        cStrVal[size] = '\0';
        std::string strVal(cStrVal);
        m_program->registerConstant(strVal);
    }

    void CBytecodeLoader::loadMethodVal()
    {
        int32_t args[3]; // pattern index, nargs, nlocals
        m_file.read(reinterpret_cast<char*>(args), (sizeof *args) * 3);
        m_program->registerMethod(args[0], args[1], args[2], loadInstructionBlock());
    }

    void CBytecodeLoader::loadClassVal()
    {
        int32_t args[2]; // identifier, number of slots
        m_file.read(reinterpret_cast<char*>(args), 2 * (sizeof *args));
        std::vector<int32_t> slots;
        for (auto i = 0; i < args[1]; ++i) {
            int32_t tmp;
            m_file.read(reinterpret_cast<char*>(&tmp), sizeof tmp);
            slots.push_back(tmp);
        }
        m_program->registerClass(args[0], slots);
    }

    insVector* CBytecodeLoader::loadInstructionBlock()
    {
        insVector* block = new insVector();
        int32_t size;
        m_file.read(reinterpret_cast<char*>(&size), sizeof size);
        block->reserve(size);
        for (auto i = 0; i < size; ++i) {
            block->emplace_back(loadInstruction());
        }
        return block;
    }

    ByteIns* CBytecodeLoader::loadInstruction()
    {
        int8_t opcode;
        m_file.read(reinterpret_cast<char*>(&opcode), sizeof opcode);
        switch (OpCode(opcode))
        {
        case OpCode::LitOp:
            return loadLitIns();
        case OpCode::SetSlotOp:
            return loadSetSlotIns();
        case OpCode::SlotOp:
            return loadSlotIns();
        case OpCode::GetSlotOp:
            return loadGetSlotIns();
        case OpCode::CallSlotOp:
            return loadCallSlotIns();
        case OpCode::ReturnOp:
            return loadReturnIns();
        
        default:
            return new ByteIns();
        }
    }

    LitIns* CBytecodeLoader::loadLitIns()
    {
        int32_t idx;
        m_file.read(reinterpret_cast<char*>(&idx), sizeof idx);
        return new LitIns(idx);
    }

    SlotIns* CBytecodeLoader::loadSlotIns()
    {
        int32_t idx;
        m_file.read(reinterpret_cast<char*>(&idx), sizeof idx);
        return new SlotIns(idx);
    }

    SetSlotIns* CBytecodeLoader::loadSetSlotIns()
    {
        int32_t idx;
        m_file.read(reinterpret_cast<char*>(&idx), sizeof idx);
        return new SetSlotIns(idx);
    }

    CallSlotIns* CBytecodeLoader::loadCallSlotIns()
    {
        int32_t args[2]; // method idx, arity
        m_file.read(reinterpret_cast<char*>(args), 2 * (sizeof *args));
        return new CallSlotIns(args[0], args[1]);
    }

    GetSlotIns* CBytecodeLoader::loadGetSlotIns()
    {
        int32_t idx;
        m_file.read(reinterpret_cast<char*>(&idx), sizeof idx);
        return new GetSlotIns(idx);
    }

    ReturnIns* CBytecodeLoader::loadReturnIns()
    {
        return new ReturnIns();
    }

}