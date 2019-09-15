add_subdirectory(Sources/Programs)

set(SourcesPath ${PROJECT_SOURCE_DIR}/Engine/Sources)

IF(${WIN32})
	set(PlatformOnlySources 
	${SourcesPath}/Runtime/Core/Application/WindowsApplication.cpp

	${SourcesPath}/Runtime/RHI/D11/GraphicsMgrD11.cpp
	${SourcesPath}/Runtime/RHI/D11/VertexBufferD11.cpp
	${SourcesPath}/Runtime/RHI/D11/ShaderD11.cpp
	${SourcesPath}/Runtime/RHI/D11/RenderMeshD11.cpp
	${SourcesPath}/Runtime/RHI/D11/IndexBufferD11.cpp
	${SourcesPath}/Runtime/RHI/D11/MaterialD11.cpp
	${SourcesPath}/Runtime/RHI/D11/TextureD11.cpp
	${SourcesPath}/Runtime/RHI/D11/SamplerStateD11.cpp
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
${SourcesPath}/Runtime/Core/Object/Components/CameraComponent.cpp

${SourcesPath}/Runtime/Core/Object/System/MeshRenderSystem.cpp
${SourcesPath}/Runtime/Core/Object/System/CameraSystem.cpp
${SourcesPath}/Runtime/Core/Object/System/RenderDebugSystem.cpp

${SourcesPath}/Runtime/Core/Time/TimeMgr.cpp
${SourcesPath}/Runtime/Core/Input/InputMgr.cpp

${SourcesPath}/Runtime/RHI/RenderObject.cpp
${SourcesPath}/Runtime/RHI/RenderMesh.cpp
${SourcesPath}/Runtime/RHI/VertexBuffer.cpp
${SourcesPath}/Runtime/RHI/IndexBuffer.cpp
${SourcesPath}/Runtime/RHI/Shader.cpp
${SourcesPath}/Runtime/RHI/Material.cpp
${SourcesPath}/Runtime/RHI/Texture.cpp
${SourcesPath}/Runtime/RHI/SamplerState.cpp

${PlatformOnlySources}

)

IF(${WIN32})
	target_link_libraries(Scarlett user32 d3d11 d3dcompiler dxgi )
	target_link_libraries(Scarlett ${ASSIMP_LIBRARY} ${IRRXML_LIBRARY} ${ZLIB_LIBRARY})
ELSEIF(${UNIX})
	IF(${APPLE})
	ELSE(${APPLE})
    ENDIF(${APPLE})
ENDIF(${WIN32})

target_link_libraries(Scarlett ${XG_LIBRARY})
__add_xg_platform_dependencies(Scarlett)