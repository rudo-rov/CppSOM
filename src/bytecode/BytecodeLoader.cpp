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
        while(m_file.read(reinterpret_cast<char*>(&tag), sizeof tag)) {
            // m_file.read(reinterpret_cast<char*>(&tag), sizeof tag);
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
            case ValueTag::PrimitiveVal:
                loadPrimitiveVal();
                break;
            case ValueTag::ClassVal:
                loadClassVal();
                break;
            case ValueTag::BlockVal:
                loadBlockVal();
                break;
            case ValueTag::ArrayVal:
                loadArrayVal();
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

    void CBytecodeLoader::loadPrimitiveVal()
    {
        int32_t args[2];
        m_file.read(reinterpret_cast<char*>(args), (sizeof args[0]) * 2);
        m_program->registerMethod(args[0], args[1]);
    }

    void CBytecodeLoader::loadClassVal()
    {
        int32_t args[3]; // identifier, number of slots
        m_file.read(reinterpret_cast<char*>(args), 3 * (sizeof *args));
        std::vector<int32_t> slots;
        for (auto i = 0; i < args[2]; ++i) {
            int32_t tmp;
            m_file.read(reinterpret_cast<char*>(&tmp), sizeof tmp);
            slots.push_back(tmp);
        }
        m_program->registerClass(args[0], args[1], slots);
    }

    void CBytecodeLoader::loadBlockVal()
    {
        int32_t nargs;
        m_file.read(reinterpret_cast<char*>(&nargs), sizeof nargs);
        m_program->registerBlock(nargs, loadInstructionBlock());
    }

    void CBytecodeLoader::loadArrayVal()
    {
        int32_t elemsNr;
        insVector* values = new insVector();
        m_file.read(reinterpret_cast<char*>(&elemsNr), sizeof elemsNr);
        for (auto i = 0; i < elemsNr; ++i) {
            values->emplace_back(loadInstruction());
        }
        m_program->registerArray(values);
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
        case OpCode::SendOp:
            return loadSendIns();
        case OpCode::ReturnOp:
            return loadReturnIns();
        case OpCode::GetArgOp:
            return loadGetArgIns();
        case OpCode::GetSelfOp:
            return loadGetSelfIns();
        case OpCode::BlockOp:
            return loadBlockIns();
        case OpCode::ReturnNLOp:
            return loadReturnNLIns();
        case OpCode::GetLocalOp:
            return loadGetLocalIns();
        case OpCode::SetLocalOp:
            return loadSetLocalIns();
        
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

    GetArgIns* CBytecodeLoader::loadGetArgIns()
    {
        int32_t idx;
        m_file.read(reinterpret_cast<char*>(&idx), sizeof idx);
        return new GetArgIns(idx);
    }

    SendIns* CBytecodeLoader::loadSendIns()
    {
        int32_t args[2]; // method idx, arity
        m_file.read(reinterpret_cast<char*>(args), 2 * (sizeof *args));
        return new SendIns(args[0], args[1]);
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

    GetSelfIns* CBytecodeLoader::loadGetSelfIns()
    {
        return new GetSelfIns();
    }

    BlockIns* CBytecodeLoader::loadBlockIns()
    {
        int32_t idx;
        m_file.read(reinterpret_cast<char*>(&idx), sizeof idx);
        return new BlockIns(idx);
    }

    ReturnNLIns* CBytecodeLoader::loadReturnNLIns()
    {
        int32_t lvl;
        m_file.read(reinterpret_cast<char*>(&lvl), sizeof lvl);
        return new ReturnNLIns(lvl);
    }

    GetLocalIns* CBytecodeLoader::loadGetLocalIns()
    {
        int32_t idx;
        m_file.read(reinterpret_cast<char*>(&idx), sizeof idx);
        return new GetLocalIns(idx);
    }

    SetLocalIns* CBytecodeLoader::loadSetLocalIns()
    {
        int32_t idx;
        m_file.read(reinterpret_cast<char*>(&idx), sizeof idx);
        return new SetLocalIns(idx);
    }

}
