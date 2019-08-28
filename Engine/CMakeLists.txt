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

${SourcesPath}/Runtime/Core/Object/World.cpp
${SourcesPath}/Runtime/Core/Object/Entity.cpp

${SourcesPath}/Runtime/Core/Object/Components/TransformComponent.cpp
${SourcesPath}/Runtime/Core/Object/Components/MeshRenderComponent.cpp


${SourcesPath}/Runtime/Core/Object/System/MeshRenderSystem.cpp

${SourcesPath}/Runtime/RHI/RenderObject.cpp

${PlatformOnlySources}

)

IF(${WIN32})
	target_link_libraries(Scarlett user32 d3d11 d3dcompiler dxgi )
ELSEIF(${UNIX})
	IF(${APPLE})
	ELSE(${APPLE})
    ENDIF(${APPLE})
ENDIF(${WIN32})

target_link_libraries(Scarlett ${XG_LIBRARY})
__add_xg_platform_dependencies(Scarlett)