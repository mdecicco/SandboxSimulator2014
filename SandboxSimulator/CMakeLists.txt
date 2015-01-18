file(GLOB game_top_SOURCES src/*.*)
file(GLOB game_client_SOURCES src/Client/*.*)
file(GLOB game_server_SOURCES src/Server/*.*)
file(GLOB game_common_SOURCES src/Common/*.*)

#include_directories(${PROJECT_SOURCE_DIR}/engine/src)

include_directories(src/)

add_executable(SandboxSimulator ${game_top_SOURCES} ${game_common_SOURCES} ${game_client_SOURCES} ${game_server_SOURCES})

source_group("SandboxSimulator\\" FILES ${game_top_SOURCES})

source_group("SandboxSimulator\\Common" FILES ${game_common_SOURCES})

source_group("SandboxSimulator\\Client" FILES ${game_client_SOURCES})

source_group("SandboxSimulator\\Server" FILES ${game_server_SOURCES})

target_link_libraries(SandboxSimulator SSEngine)
