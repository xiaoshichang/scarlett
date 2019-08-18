 if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    find_library(XG_LIBRARY xg PATHS ${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Windows/Release_x64/lib)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Windows/Release_x64/include")

else( CMAKE_BUILD_TYPE STREQUAL "Release" )
	find_library(XG_LIBRARY xg PATHS ${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Windows/Debug_x64/lib)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Windows/Debug_x64/include")

endif( CMAKE_BUILD_TYPE STREQUAL "Release")

MESSAGE( STATUS "CrossGuid lib found: " ${XG_LIBRARY})