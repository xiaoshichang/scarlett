 if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    find_library(XG_LIBRARY xg PATHS ${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Windows/Release_x64/lib)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Windows/Release_x64/include")
	find_library(ASSIMP_LIBRARY assimp-vc141-mt PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Release_x64/lib)
	find_library(IRRXML_LIBRARY IrrXML PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Release_x64/lib)
	find_library(ZLIB_LIBRARY   zlibstatic PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Release_x64/lib)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Release_x64/include")

else( CMAKE_BUILD_TYPE STREQUAL "Release" )
	find_library(XG_LIBRARY xg PATHS ${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Windows/Debug_x64/lib)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/CrossGuid/Windows/Debug_x64/include")

	find_library(ASSIMP_LIBRARY assimp-vc141-mtd PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Debug_x64/lib)
	find_library(IRRXML_LIBRARY IrrXMLd PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Debug_x64/lib)
	find_library(ZLIB_LIBRARY   zlibstaticd PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Debug_x64/lib)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Debug_x64/include")

endif( CMAKE_BUILD_TYPE STREQUAL "Release")

MESSAGE( STATUS "XG_LIBRARY found: " ${XG_LIBRARY})
MESSAGE( STATUS "ASSIMP_LIBRARY found: " ${ASSIMP_LIBRARY})
MESSAGE( STATUS "IRRXML_LIBRARY found: " ${IRRXML_LIBRARY})
MESSAGE( STATUS "ZLIB_LIBRARY found: " ${ZLIB_LIBRARY})