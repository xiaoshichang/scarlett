cmake_minimum_required(VERSION 3.1)

Project(Scarlett)

include_directories("${PROJECT_SOURCE_DIR}/Cmake")
include_directories("${PROJECT_SOURCE_DIR}/Engine/Sources")

include(${PROJECT_SOURCE_DIR}/Cmake/TargetArchDetect.cmake)
target_architecture(TARGET_ARCH)

include(${PROJECT_SOURCE_DIR}/Cmake/CrossGuidPlatformDependencies.cmake)

include(${PROJECT_SOURCE_DIR}/Cmake/Toolchain.cmake)

include(${PROJECT_SOURCE_DIR}/Cmake/Common.cmake)

IF(${WIN32})
	include(${PROJECT_SOURCE_DIR}/Cmake/Windows.cmake)
ELSEIF(${UNIX})
	IF(${APPLE})
		include(${PROJECT_SOURCE_DIR}/Cmake/Macos.cmake)
	ELSE(${APPLE})
    ENDIF(${APPLE})

ENDIF(${WIN32})

add_subdirectory(Engine)
add_subdirectory(Tests)
