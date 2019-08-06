set(Engine_Version_Major 0)
set(Engine_Version_Minor 1)

if(${TARGET_ARCH} STREQUAL "ppc")
	add_compile_definitions(arch_ppc)
elseif(${TARGET_ARCH} STREQUAL "i386")
	add_compile_definitions(arch_i386)
elseif(${TARGET_ARCH} STREQUAL "x86_64")
	add_compile_definitions(arch_x86_64)
elseif(${TARGET_ARCH} STREQUAL "ppc64")
	add_compile_definitions(arch_ppc64)
else()
	add_compile_definitions(arch_unknown)
endif()
MESSAGE( STATUS "TARGET_ARCH: " ${TARGET_ARCH} )



IF (CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "None")
	set(BuildType Debug)
	add_compile_definitions(SDEBUG)
ELSEIF (CMAKE_BUILD_TYPE MATCHES "Release")
	set(BuildType Release)
	add_compile_definitions(SRELEASE)
ELSE ()
ENDIF ()
MESSAGE( STATUS "BuildType: " ${BuildType} )

configure_file("${PROJECT_SOURCE_DIR}/Cmake/EngineConfig.h.in" "${PROJECT_SOURCE_DIR}/Cmake/EngineConfig.h")
