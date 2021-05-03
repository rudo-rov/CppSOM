// SOM.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "utils/Config.h"
#include "SOMParserBaseVisitor.h"
#include "./ast/ASTNodes.h"
#include "utils/SourceFilesDir.h"
#include "bytecode/Program.h"
#include "bytecode/BytecodeLoader.h"
#include "interpretation/Interpret.h"

int main(int argc, char** argv)
{
    som::CConfig appConfig;
    if (!appConfig.loadFromArgs(argc, argv)) {
        std::cerr << appConfig.getLastError() << std::endl;
        return 1;
    }

    if (appConfig.getMode() == som::CConfig::Mode::Compile) {
        som::SourceFilesDir srcFiles(appConfig.getInputFiles());
        if (!srcFiles.loadSourceFiles()) {
            // TODO: error message
            return 1;
        }
        som::Program program;
        for (auto& file : srcFiles.getSrcFiles()) {
            som::compileFile(*file, program);
        }
        program.print();
        // TODO: some sane program name
        if (!program.serialize("bytecode.bc")) {
            // TODO: error messages
            return 1;
        }
    } else { // Interpret mode
        som::CBytecodeLoader bcLoader("bytecode.bc"); // TODO: filename
        std::unique_ptr<som::Program> program = std::move(bcLoader.load());
        if (!program) {
            std::cerr << bcLoader.lastError() << std::endl;
            return 1;
        }
        // program->print();
        som::CInterpret interpret(std::move(program));
        interpret.initialize();
        interpret.interpret();
    }
    
    return 0;
}


