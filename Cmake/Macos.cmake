set (CMAKE_C_STANDARD 11)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_C_COMPILER               "clang")
set (CMAKE_CXX_COMPILER             "clang++")

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
find_library(XG_LIBRARY xg PATHS ${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Darwin/Release_x64/lib)
include_directories("${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Darwin/Release_x64/include")

else( CMAKE_BUILD_TYPE STREQUAL "Release" )
find_library(XG_LIBRARY xg PATHS ${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Darwin/Debug_x64/lib)
include_directories("${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Darwin/Debug_x64/include")

endif( CMAKE_BUILD_TYPE STREQUAL "Release")

MESSAGE( STATUS "CrossGuid lib found: " ${XG_LIBRARY})