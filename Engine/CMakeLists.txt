add_subdirectory(Sources/Programs)

set(SourcesPath ${PROJECT_SOURCE_DIR}/Engine/Sources)

add_library(Scarlett 
${SourcesPath}/Runtime/Core/Memory/Allocator.cpp
${SourcesPath}/Runtime/Core/Memory/MemoryManager.cpp
)