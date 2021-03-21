add_library(bytecode
    Program.h Program.cpp
    Bytecode.h Bytecode.cpp
    BCClass.h BCClass.cpp
    BytecodeCompiler.h BytecodeCompiler.cpp
)

set_property(TARGET bytecode PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")