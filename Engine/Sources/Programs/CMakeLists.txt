


add_executable(Game Game.cpp)
# use mainCRTStartup as entry point and it will call main function in our source.
# use WinMainCRTStartup as entry point and it will call WinMain corresponding.
set_target_properties(Game PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

target_link_libraries(Game Scarlett)

install( TARGETS Game DESTINATION "${PROJECT_SOURCE_DIR}/Binary/${TARGET_ARCH}/${CMAKE_BUILD_TYPE}")
install( DIRECTORY "${PROJECT_SOURCE_DIR}/Asset"
    DESTINATION "${PROJECT_SOURCE_DIR}/Binary/${TARGET_ARCH}/${CMAKE_BUILD_TYPE}"
)