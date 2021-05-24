add_library(utils
    SourceFilesDir.h SourceFilesDir.cpp
    Config.h Config.cpp
    )

target_link_libraries(utils ast)

set_property(TARGET utils PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")