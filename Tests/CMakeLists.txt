IF(${WIN32})
add_subdirectory(Windows)
ENDIF(${WIN32})


add_executable(Test_Math Test_Math.cpp)
add_test(NAME Test_Math COMMAND Test_Math)

add_executable(Test_Guid Test_Guid.cpp)
target_link_libraries(Test_Guid ${XG_LIBRARY})
__add_xg_platform_dependencies(Test_Guid)
add_test(NAME Test_Guid COMMAND Test_Guid)

add_executable(Test_GMemoryMananger Test_GMemoryMananger.cpp)
target_link_libraries(Test_GMemoryMananger Scarlett)
add_test(NAME Test_GMemoryMananger COMMAND Test_GMemoryMananger)

add_executable(Test_Objects Test_Objects.cpp)
target_link_libraries(Test_Objects Scarlett)
add_test(NAME Test_Objects COMMAND Test_Objects)