#pragma once
#include <fstream>
#include <string>
#include "bytecode/Program.h"

namespace som {

    const std::string coreLibPath = "core_lib";

    void compileFile(std::ifstream& file, Program& program);
    
    class CConfig {
    public:
        enum class Mode {
            Compile,
            Interpret
        };

        CConfig() {}
        ~CConfig() {}

        bool loadFromArgs(int argc, char** argv);
        Mode getMode() const { return m_mode; }
        std::string getLastError() const { return m_lastError; }
        std::string getInputFiles() const { return m_input; }

    private:
        Mode m_mode;
        std::string m_input;
        std::string m_lastError;
    };

}