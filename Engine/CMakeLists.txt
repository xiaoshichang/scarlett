add_subdirectory(Sources/Programs)

set(SourcesPath ${PROJECT_SOURCE_DIR}/Engine/Sources)

IF(${WIN32})
	set(PlatformOnlySources 
	${SourcesPath}/Runtime/Core/Application/WindowsApplication.cpp
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