cmake_minimum_required(VERSION 3.6)
project(SimpleRayTracer)


include("external/stb.cmake")
include("external/tinyobjloader.cmake")
include("external/glm.cmake")

set(PROJECT_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/")
set(INCLUDE_FILES
	"${PROJECT_INCLUDE_DIR}/Image.h"
	"${PROJECT_INCLUDE_DIR}/App.h"
	"${PROJECT_INCLUDE_DIR}/Model.h"
	"${PROJECT_INCLUDE_DIR}/Ray.h"
	"${PROJECT_INCLUDE_DIR}/Triangle.h"
	"${PROJECT_INCLUDE_DIR}/Vertex.h"
	"${PROJECT_INCLUDE_DIR}/Material.h"
	"${PROJECT_INCLUDE_DIR}/CubeMap.h"
	"${PROJECT_INCLUDE_DIR}/Light.h"
	"${PROJECT_INCLUDE_DIR}/Camera.h"
	"${PROJECT_INCLUDE_DIR}/RenderUtils.h"
)

set(ALL_INCLUDE_DIRECTORIES
    "${PROJECT_INCLUDE_DIR}"
    "${STB_INCLUDE_DIRECTORY}"
    "${TINYOBJLOADER_INCLUDE_DIRECTORY}"
    "${GLM_INCLUDE_DIRECTORY}"
)


set(SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}/src/" )
set(SOURCE_FILES 
	"${SOURCE_DIRECTORY}/main.cpp"
	"${SOURCE_DIRECTORY}/App.cpp"
	"${SOURCE_DIRECTORY}/Image.cpp"
	"${SOURCE_DIRECTORY}/Model.cpp"
	"${SOURCE_DIRECTORY}/Ray.cpp"
	"${SOURCE_DIRECTORY}/Triangle.cpp"
	"${SOURCE_DIRECTORY}/Vertex.cpp"
	"${SOURCE_DIRECTORY}/Material.cpp"
	"${SOURCE_DIRECTORY}/CubeMap.cpp"
	"${SOURCE_DIRECTORY}/Light.cpp"
	"${SOURCE_DIRECTORY}/Camera.cpp"
	"${SOURCE_DIRECTORY}/RenderUtils.cpp"

)

set(RESOURCE_DIRECTORIES
    "resources/"
)

add_executable(SimpleRayTracer "${INCLUDE_FILES}" "${SOURCE_FILES}")
target_include_directories(SimpleRayTracer PRIVATE "${ALL_INCLUDE_DIRECTORIES}")


foreach(RESOURCE_DIRECTORY ${RESOURCE_DIRECTORIES})
    add_custom_command(
        TARGET SimpleRayTracer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/${RESOURCE_DIRECTORY}"
            "${CMAKE_BINARY_DIR}/${RESOURCE_DIRECTORY}"
    )
endforeach()