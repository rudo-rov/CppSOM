#include <sstream>
#include <cstring>

#include "antlr4-runtime.h"
#include "SOMLexer.h"
#include "SOMParser.h"

#include "Config.h"
#include "../ast/CParseTreeConverter.h"
#include "../bytecode/BytecodeCompiler.h"


namespace som {

    void compileFile(std::ifstream& file, Program& program)
    {
        antlr4::ANTLRInputStream inputStream(file);
        antlr_som::SOMLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokens(&lexer);
        antlr_som::SOMParser parser(&tokens);

        antlr_som::SOMParser::ClassdefContext* tree = parser.classdef();
        CParseTreeConverter visitor;
        ASTNode* fileAST = visitor.visitClassdef(tree).as<ASTNode*>();
        nodePtr ast(fileAST);

        CBytecodeCompiler bcCompiler(&program);
        ast->accept(bcCompiler);
    }
    
    bool CConfig::loadFromArgs(int argc, char** argv)
    {
        if (argc <= 2) {
            // TODO: Some help message
            m_lastError = "Invalid input arguments";
            return false;
        }
        
        m_input = argv[2];
        if (strcmp("-c", argv[1]) == 0) {
            m_mode = Mode::Compile;
        } else if (strcmp("-r", argv[1]) == 0) {
            m_mode = Mode::Interpret;
        } else {
            std::stringstream errMssg;
            errMssg << "Unknown option: " << argv[1];
            return false;
        }
        return true;
    }

}