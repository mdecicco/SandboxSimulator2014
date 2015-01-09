file(GLOB server_top_SOURCES src/*.*)

include_directories(src/)

add_executable(SandboxServer ${server_top_SOURCES})

source_group("SandboxServer\\" FILES ${server_top_SOURCES})

target_link_libraries(SandboxServer SSEngine)
