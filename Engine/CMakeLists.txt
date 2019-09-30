add_subdirectory(Sources/Programs)

set(SourcesPath ${PROJECT_SOURCE_DIR}/Engine/Sources)
set(ExternalPath ${PROJECT_SOURCE_DIR}/Engine/External)

IF(${WIN32})
	set(PlatformOnlySources 
	${SourcesPath}/Runtime/Core/Application/WindowsApplication.cpp

	${SourcesPath}/Runtime/RHI/D11/GraphicsMgrD11.cpp
	${SourcesPath}/Runtime/RHI/D11/VertexBufferD11.cpp
	${SourcesPath}/Runtime/RHI/D11/ShaderD11.cpp
	${SourcesPath}/Runtime/RHI/D11/MeshD11.cpp
	${SourcesPath}/Runtime/RHI/D11/IndexBufferD11.cpp
	${SourcesPath}/Runtime/RHI/D11/MaterialD11.cpp
	${SourcesPath}/Runtime/RHI/D11/TextureD11.cpp
	${SourcesPath}/Runtime/RHI/D11/SamplerStateD11.cpp
	${SourcesPath}/Runtime/RHI/D11/DepthStencilStateD11.cpp
	${SourcesPath}/Runtime/RHI/D11/DDSTextureLoader.cpp

	${ExternalPath}/Glad/src/glad.c
	${ExternalPath}/Glad/src/glad_wgl.c

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
${SourcesPath}/Runtime/Core/Application/GlobalConfig.cpp
${SourcesPath}/Runtime/Core/Application/AssetFinder.cpp

${SourcesPath}/Runtime/Core/Object/World.cpp
${SourcesPath}/Runtime/Core/Object/Entity.cpp

${SourcesPath}/Runtime/Core/Object/Components/TransformComponent.cpp
${SourcesPath}/Runtime/Core/Object/Components/MeshRenderComponent.cpp
${SourcesPath}/Runtime/Core/Object/Components/CameraComponent.cpp
${SourcesPath}/Runtime/Core/Object/Components/SkeletonComponent.cpp

${SourcesPath}/Runtime/Core/Object/System/MeshRenderSystem.cpp
${SourcesPath}/Runtime/Core/Object/System/CameraSystem.cpp
${SourcesPath}/Runtime/Core/Object/System/RenderDebugSystem.cpp

${SourcesPath}/Runtime/Core/Time/TimeMgr.cpp
${SourcesPath}/Runtime/Core/Input/InputMgr.cpp

${SourcesPath}/Runtime/RHI/opengl/GraphicsMgrGL.cpp
${SourcesPath}/Runtime/RHI/opengl/MeshGL.cpp
${SourcesPath}/Runtime/RHI/opengl/VertexBufferGL.cpp
${SourcesPath}/Runtime/RHI/opengl/IndexBufferGL.cpp
${SourcesPath}/Runtime/RHI/opengl/ShaderGL.cpp
${SourcesPath}/Runtime/RHI/opengl/MaterialGL.cpp
${SourcesPath}/Runtime/RHI/opengl/TextureGL.cpp
${SourcesPath}/Runtime/RHI/opengl/SamplerStateGL.cpp

${SourcesPath}/Runtime/Core/UI/AutoReleasePool.cpp
${SourcesPath}/Runtime/Core/UI/Director.cpp
${SourcesPath}/Runtime/Core/UI/Dispatcher.cpp
${SourcesPath}/Runtime/Core/UI/Event.cpp
${SourcesPath}/Runtime/Core/UI/Listener.cpp
${SourcesPath}/Runtime/Core/UI/Node.cpp
${SourcesPath}/Runtime/Core/UI/Ref.cpp
${SourcesPath}/Runtime/Core/UI/Renderer.cpp
${SourcesPath}/Runtime/Core/UI/Sprite.cpp

${PlatformOnlySources}

)

IF(${WIN32})
	target_link_libraries(Scarlett user32 d3d11 d3dcompiler dxgi opengl32 dxguid.lib)
	target_link_libraries(Scarlett ${ASSIMP_LIBRARY} ${IRRXML_LIBRARY} ${ZLIB_LIBRARY})
ELSEIF(${UNIX})
	IF(${APPLE})
	ELSE(${APPLE})
    ENDIF(${APPLE})
ENDIF(${WIN32})

target_link_libraries(Scarlett ${XG_LIBRARY})
__add_xg_platform_dependencies(Scarlett)