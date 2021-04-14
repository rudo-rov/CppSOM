add_library(vm_objects
    VMClass.h VMClass.cpp
    VMObject.h
    VMString.h VMString.cpp
)

set_property(TARGET vm_objects PROPERTY 
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")