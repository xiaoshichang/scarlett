IF(${WIN32})
add_subdirectory(Windows)
ENDIF(${WIN32})


add_executable(Test_Math Test_Math.cpp)
add_test(NAME Test_Math COMMAND Test_Math)