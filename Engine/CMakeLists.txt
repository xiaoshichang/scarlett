add_subdirectory(Sources/Programs)

set(SourcesPath ${PROJECT_SOURCE_DIR}/Engine/Sources)

IF(${WIN32})
	set(PlatformOnlySources 
	${SourcesPath}/Runtime/Core/Application/WindowsApplication.cpp

	${SourcesPath}/Runtime/RHI/DeviceContextD11.cpp
	${SourcesPath}/Runtime/RHI/GraphicsMgrD11.cpp
	)
ELSEIF(${UNIX})
	IF(${APPLE})
	ELSE(${APPLE})
    ENDIF(${APPLE})
ENDIF(${WIN32})


add_library(Scarlett 
${SourcesPath}/Runtime/Core/Memory/Allocator.cpp
${SourcesPath}/Runtime/Core/Memory/MemoryManager.cpp
${SourcesPath}/Runtime/Core/Application/Application.cpp

${PlatformOnlySources}

)

IF(${WIN32})
	target_link_libraries(Scarlett user32 d3d11 d3dcompiler dxgi)
ELSEIF(${UNIX})
	IF(${APPLE})
	ELSE(${APPLE})
    ENDIF(${APPLE})
ENDIF(${WIN32})