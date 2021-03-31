add_library(interpret
    Interpret.h Interpret.cpp
    ProgramCounter.h ProgramCounter.cpp
    Frame.h Frame.cpp
    ExecutionStack.h ExecutionStack.cpp
    ObjectReference.h ObjectReference.cpp
)

set_property(TARGET interpret PROPERTY 
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")