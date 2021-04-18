add_subdirectory(vm_objects)

add_library(interpret
    Interpret.h Interpret.cpp
    ProgramCounter.h ProgramCounter.cpp
    Frame.h Frame.cpp
    ExecutionStack.h ExecutionStack.cpp
    ScopeStack.h ScopeStack.cpp
    GlobalContext.h GlobalContext.cpp
    Heap.h Heap.cpp
)

target_link_libraries(interpret vm_objects)

set_property(TARGET interpret PROPERTY 
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")