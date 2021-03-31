add_subdirectory(ast)
add_subdirectory(utils)
add_subdirectory(bytecode)
add_subdirectory(interpretation)

add_executable(som SOM.cpp)
target_link_libraries(som ast utils bytecode interpret antlr4_static antlr_som)
set_property(TARGET som PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
