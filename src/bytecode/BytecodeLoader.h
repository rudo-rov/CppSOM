#pragma once

#include <fstream>
#include <string>

#include "Program.h"
#include "Bytecode.h"

namespace som {

    class CBytecodeLoader {
    public:
        CBytecodeLoader(const std::string& fileName);
        ~CBytecodeLoader();
        std::ifstream& fileStream() { return m_file; }
        std::string lastError() const { return m_lastError; }

        std::unique_ptr<Program>&& load();

    private:
        std::unique_ptr<Program> m_program;
        std::string m_fileName;
        std::ifstream m_file;
        std::string m_lastError;

        void loadIntVal();
        void loadDoubleVal();
        void loadStringVal();
        void loadMethodVal();
        void loadPrimitiveVal();
        void loadClassVal();
        void loadBlockVal();
        void loadArrayVal();

        insVector* loadInstructionBlock();
        ByteIns* loadInstruction();
        LitIns* loadLitIns();
        SlotIns* loadSlotIns();
        SetSlotIns* loadSetSlotIns();
        SendIns* loadSendIns();
        GetSlotIns* loadGetSlotIns();
        ReturnIns* loadReturnIns();
        GetArgIns* loadGetArgIns();
        GetSelfIns* loadGetSelfIns();
        BlockIns* loadBlockIns();
        ReturnNLIns* loadReturnNLIns();
        GetLocalIns* loadGetLocalIns();
        SetLocalIns* loadSetLocalIns();
    };

}