file(GLOB ss_top_SOURCES src/*.*)

file(GLOB ss_system_SOURCES src/System/*.*)

file(GLOB ss_core_SOURCES src/Core/*.*)

file(GLOB ss_rendering_SOURCES src/Rendering/*.*)

file(GLOB ss_utils_SOURCES src/Utils/*.*)
file(GLOB ss_utils_math_SOURCES src/Utils/Math/*.*)

if(BUILD_STATIC)
    set(BUILD_MODE STATIC)
else()
    set(BUILD_MODE SHARED)
endif()

add_library(SSEngine ${BUILD_MODE} ${ss_top_SOURCES} ${ss_system_SOURCES} ${ss_core_SOURCES} ${ss_rendering_SOURCES} ${ss_utils_SOURCES} ${ss_utils_math_SOURCES})

target_link_libraries(SSEngine Glew Glfw Soil ${ZLIB_LIBRARIES} ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

source_group("SSEngine" FILES ${ss_top_SOURCES})

source_group("SSEngine\\System" FILES ${ss_system_SOURCES})

source_group("SSEngine\\Core" FILES ${ss_core_SOURCES})

source_group("SSEngine\\Rendering" FILES ${ss_rendering_SOURCES})

source_group("SSEngine\\Utils" FILES ${ss_utils_SOURCES})
source_group("SSEngine\\Utils\\Math" FILES ${ss_utils_math_SOURCES})

#install(TARGETS SSEngine RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
