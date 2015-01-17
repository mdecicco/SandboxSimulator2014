file(GLOB game_top_SOURCES src/*.*)

#include_directories(${PROJECT_SOURCE_DIR}/engine/src)

include_directories(src/)

add_executable(TestGame ${game_top_SOURCES})

source_group("TestGame\\" FILES ${game_top_SOURCES})

target_link_libraries(TestGame SSEngine)
