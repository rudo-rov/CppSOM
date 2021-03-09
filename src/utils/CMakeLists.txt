add_library(utils
    SourceFilesDir.h SourceFilesDir.cpp
    )

set_property(TARGET utils PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")