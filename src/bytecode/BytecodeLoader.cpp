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
            // return std::make_unique<Program>(nullptr);
            return std::move(std::unique_ptr<Program>(nullptr));
        }

        int8_t tag;
        while(!m_file.eof()) {
            m_file.get(reinterpret_cast<char*>(&tag), sizeof tag);
            switch (ValueTag(tag))
            {
            case ValueTag::IntVal:
                loadIntVal();
                break;
            
            default:
                break;
            }
        }
        return std::move(m_program);
    }

    void CBytecodeLoader::loadIntVal()
    {
        int32_t value;
        m_file.get(reinterpret_cast<char*>(&value), sizeof value);
        m_program->registerConstant(value);
    }

    void CBytecodeLoader::loadDoubleVal()
    {
        double value;
        m_file.get(reinterpret_cast<char*>(&value), sizeof value);
        m_program->registerConstant(value);
    }

    void CBytecodeLoader::loadStringVal()
    {
        int32_t size;
        m_file.get(reinterpret_cast<char*>(&size), sizeof size);
        char* cStrVal = new char[size + 1];
        m_file.get(cStrVal, size);
        cStrVal[size] = '\0';
        std::string strVal(cStrVal);
        m_program->registerConstant(strVal);
    }

    void CBytecodeLoader::loadMethodVal()
    {
        int32_t args[3]; // pattern index, nargs, nlocals
        m_file.get(reinterpret_cast<char*>(args), (sizeof *args) * 3);
        std::vector<ByteIns> code; // TODO
        m_program->registerMethod(args[0], args[1], args[3], code);
    }

}