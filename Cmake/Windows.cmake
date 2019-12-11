 if( CMAKE_BUILD_TYPE STREQUAL "Release" )

	find_library(ASSIMP_LIBRARY assimp-vc141-mt PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Release_x64/lib)
	find_library(IRRXML_LIBRARY IrrXML PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Release_x64/lib)
	find_library(ZLIB_LIBRARY   zlibstatic PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Release_x64/lib)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Release_x64/include")

	include_directories("${PROJECT_SOURCE_DIR}/Engine/External/Boost/include")
	link_directories("${PROJECT_SOURCE_DIR}/Engine/External/Boost/libs")

	find_library(FREETYPE2_LIBRARY freetype PATHS ${PROJECT_SOURCE_DIR}/Engine/External/FreeType2/windows/lib/x64/Release)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/FreeType2/windows")
	include_directories("${PROJECT_SOURCE_DIR}/Engine/External/FreeType2/windows/freetype2")

else( CMAKE_BUILD_TYPE STREQUAL "Release" )

	find_library(ASSIMP_LIBRARY assimp-vc141-mtd PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Debug_x64/lib)
	find_library(IRRXML_LIBRARY IrrXMLd PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Debug_x64/lib)
	find_library(ZLIB_LIBRARY   zlibstaticd PATHS ${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Debug_x64/lib)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/Assimp/Windows/Debug_x64/include")

	include_directories("${PROJECT_SOURCE_DIR}/Engine/External/Boost/include")
	link_directories("${PROJECT_SOURCE_DIR}/Engine/External/Boost/libs")

	find_library(FREETYPE2_LIBRARY freetype PATHS ${PROJECT_SOURCE_DIR}/Engine/External/FreeType2/windows/lib/x64/Debug)
    include_directories("${PROJECT_SOURCE_DIR}/Engine/External/FreeType2/windows")
	include_directories("${PROJECT_SOURCE_DIR}/Engine/External/FreeType2/windows/freetype2")


endif( CMAKE_BUILD_TYPE STREQUAL "Release")


MESSAGE( STATUS "ASSIMP_LIBRARY found: " ${ASSIMP_LIBRARY})
MESSAGE( STATUS "IRRXML_LIBRARY found: " ${IRRXML_LIBRARY})
MESSAGE( STATUS "ZLIB_LIBRARY found: " ${ZLIB_LIBRARY})
MESSAGE( STATUS "freetype2 library found: " ${FREETYPE2_LIBRARY} )


