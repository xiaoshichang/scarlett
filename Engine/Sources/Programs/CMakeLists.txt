
add_executable(Game Game.cpp)
target_link_libraries(Game Scarlett)

install( TARGETS Game DESTINATION "${PROJECT_SOURCE_DIR}/Binary/${TARGET_ARCH}/${CMAKE_BUILD_TYPE}")
install( DIRECTORY "${PROJECT_SOURCE_DIR}/Asset"
    DESTINATION "${PROJECT_SOURCE_DIR}/Binary/${TARGET_ARCH}/${CMAKE_BUILD_TYPE}"
)