add_library(ast
    ASTAbstractVisitor.h
    ASTNodes.h ASTNodes.cpp
    CParseTreeConverter.h CParseTreeConverter.cpp
    )

set_property(TARGET ast PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")