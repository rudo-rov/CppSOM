add_subdirectory(ast)

add_executable(som SOM.cpp)
target_link_libraries(som ast antlr4_static antlr_som)
set_property(TARGET som PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
