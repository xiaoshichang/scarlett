add_executable(Test_D2D Test_D2D.cpp)
set_target_properties (Test_D2D PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
set_target_properties (Test_D2D PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
set_target_properties (Test_D2D PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
set_target_properties (Test_D2D PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows")
target_link_libraries(Test_D2D user32 ole32 d2d1)


add_executable(Test_D3D Test_D3D.cpp)
set_target_properties (Test_D3D PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
set_target_properties (Test_D3D PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
set_target_properties (Test_D3D PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
set_target_properties (Test_D3D PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows")
target_link_libraries(Test_D3D user32 d3d11 d3dcompiler)
add_custom_command(TARGET Test_D3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/Tests/Windows/copy.vso
        ${CMAKE_CURRENT_BINARY_DIR}/copy.vso)
add_custom_command(TARGET Test_D3D POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/Tests/Windows/copy.pso
        ${CMAKE_CURRENT_BINARY_DIR}/copy.pso)


add_executable(Test_GL Test_GL.cpp)
set_target_properties (Test_GL PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:windows")
set_target_properties (Test_GL PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:windows")
set_target_properties (Test_GL PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:windows")
set_target_properties (Test_GL PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:windows")
add_custom_command(TARGET Test_GL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/Tests/Windows/color.vs
        ${CMAKE_CURRENT_BINARY_DIR}/color.vs)
add_custom_command(TARGET Test_GL POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/Tests/Windows/color.ps
        ${CMAKE_CURRENT_BINARY_DIR}/color.ps)
set_target_properties(Test_GL PROPERTIES COMPILE_FLAGS "/EHsc")
get_target_property(Test_GL_Complie_Flag Test_GL COMPILE_FLAGS)
message(STATUS "Test_GL_Complie_Flag: ${Test_GL_Complie_Flag}")
target_link_libraries(Test_GL user32 gdi32 opengl32)
