file(GLOB ld32_top_SOURCES src/*.*)

#include_directories(${PROJECT_SOURCE_DIR}/engine/src)

include_directories(src/)

add_executable(LudumDare32 ${ld32_top_SOURCES})

source_group("LudumDare32\\" FILES ${ld32_top_SOURCES})

target_link_libraries(LudumDare32 SSEngine)
