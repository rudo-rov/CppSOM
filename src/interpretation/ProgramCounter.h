#pragma once
#include <cinttypes>
#include <vector>
#include "../bytecode/Program.h"

namespace som {

    typedef std::vector<std::unique_ptr<ByteIns>>::iterator CodeAddress;
    
    class CProgramCounter {
    public:
        CProgramCounter(CodeAddress&& entryPoint) : m_pc(entryPoint) {}

        void setAddress(const CodeAddress& address) { m_pc = address; }
        CodeAddress nextInstruction() { return ++m_pc; }
        CodeAddress currentInstruction() const { return m_pc; }


    private:
        CodeAddress m_pc;
    };

}