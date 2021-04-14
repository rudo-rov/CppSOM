add_library(vm_objects
    VMClass.h VMClass.cpp
    VMObject.h VMObject.cpp
    VMString.h VMString.cpp
    VMValue.h
)

set_property(TARGET vm_objects PROPERTY 
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")