file(GLOB ss_top_SOURCES src/*.*)

file(GLOB ss_system_SOURCES src/System/*.*)
file(GLOB ss_system_os_SOURCES src/System/OS/*.*)
file(GLOB ss_system_states_SOURCES src/System/StateManagement/*.*)

file(GLOB ss_utils_SOURCES src/Utils/*.*)
file(GLOB ss_utils_math_SOURCES src/Utils/Math/*.*)

if(BUILD_STATIC)
    set(BUILD_MODE STATIC)
else()
    set(BUILD_MODE SHARED)
endif()

add_library(SSEngine ${BUILD_MODE} ${ss_top_SOURCES} ${ss_system_SOURCES} ${ss_system_os_SOURCES} ${ss_utils_SOURCES} ${ss_utils_math_SOURCES} ${ss_system_states_SOURCES})

target_link_libraries(SSEngine assimp Glew Glfw Soil ${ZLIB_LIBRARIES} ${GL_LIBRARIES} ${GLFW_LIBRARIES})

source_group("SSEngine" FILES ${ss_top_SOURCES})

source_group("SSEngine\\System" FILES ${ss_system_SOURCES})
source_group("SSEngine\\System\\OS" FILES ${ss_system_os_SOURCES})
source_group("SSEngine\\System\\StateManagement" FILES ${ss_system_states_SOURCES})

source_group("SSEngine\\Utils" FILES ${ss_utils_SOURCES})
source_group("SSEngine\\Utils\\Math" FILES ${ss_utils_math_SOURCES})

#install(TARGETS SSEngine RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
