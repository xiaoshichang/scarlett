IF(${WIN32})
add_subdirectory(Windows)
ENDIF(${WIN32})


add_executable(Test_Math Test_Math.cpp)
add_test(NAME Test_Math COMMAND Test_Math)

add_executable(Test_Guid Test_Guid.cpp)
target_link_libraries(Test_Guid ${XG_LIBRARY})
add_test(NAME Test_Guid COMMAND Test_Guid)