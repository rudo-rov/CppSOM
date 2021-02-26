// SOM.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include <fstream>
#include <memory>

// #define ANTLR4CPP_STATIC // Needed when linking static antlr library
#include "antlr4-runtime.h"
#include "SOMLexer.h"
#include "SOMParser.h"

#include "./ast/CParseTreeConverter.h"
#include "SOMParserBaseVisitor.h"
#include "./ast/ASTNodes.h"

int main(int argc, char** argv)
{
    // Check the input arguments number
    if (argc <= 1) {
        std::cerr << "Invalid input arguments\n";
        return 1;
    }

    // Open the SOM source file
    std::ifstream input;
    input.open(argv[1]);
    if (!input) {
        std::cerr << "Unable to open SOM source file: " << argv[1] << std::endl;
        return -1;
    }

    antlr4::ANTLRInputStream inputStream(input);
    SOMLexer lexer(&inputStream);
    antlr4::CommonTokenStream tokens(&lexer);
    SOMParser parser(&tokens);

    SOMParser::ClassdefContext* tree = parser.classdef();
    som::CParseTreeConverter visitor;
    som::ASTNode* ret = visitor.visitClassdef(tree).as<som::ASTNode*>();
    som::nodePtr ast = std::unique_ptr<som::ASTNode>(ret);

    return 0;
}


