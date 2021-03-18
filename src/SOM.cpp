// SOM.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "antlr4-runtime.h"
#include "SOMLexer.h"
#include "SOMParser.h"

#include "./ast/CParseTreeConverter.h"
#include "SOMParserBaseVisitor.h"
#include "./ast/ASTNodes.h"
#include "utils/SourceFilesDir.h"

int main(int argc, char** argv)
{
    if (argc <= 1) {
        std::cerr << "Invalid input arguments\n";
        return 1;
    }

    som::SourceFilesDir srcFiles(argv[1]);
    if (!srcFiles.loadSourceFiles()) {
        return 1;
    }

    
    std::vector<som::nodePtr> asts;
    for (auto& file : srcFiles.getSrcFiles()) {
        antlr4::ANTLRInputStream inputStream(*file);
        SOMLexer lexer(&inputStream);
        antlr4::CommonTokenStream tokens(&lexer);
        SOMParser parser(&tokens);

        SOMParser::ClassdefContext* tree = parser.classdef();
        som::CParseTreeConverter visitor;
        som::ASTNode* fileAST = visitor.visitClassdef(tree).as<som::ASTNode*>();
        asts.emplace_back(fileAST);
    }

    return 0;
}


