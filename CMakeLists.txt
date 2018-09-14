cmake_minimum_required(VERSION 3.0)
project(SimpleRayTracer)


include("external/stb.cmake")
include("external/tinyobjloader.cmake")
include("external/glm.cmake")

set(INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/include/")
set(INCLUDE_FILES
	"${INCLUDE_DIRECTORY}/Image.h"
	"${INCLUDE_DIRECTORY}/App.h"
	"${INCLUDE_DIRECTORY}/Model.h"
	"${INCLUDE_DIRECTORY}/MeshPool.h"
	"${INCLUDE_DIRECTORY}/Ray.h"
	"${INCLUDE_DIRECTORY}/Triangle.h"
	"${INCLUDE_DIRECTORY}/Vertex.h"
	"${INCLUDE_DIRECTORY}/Material.h"
	"${INCLUDE_DIRECTORY}/CubeMap.h"
	"${INCLUDE_DIRECTORY}/Camera.h"
	"${INCLUDE_DIRECTORY}/RenderUtils.h"
)

set(INCLUDE_DIRECTORIES
    "${INCLUDE_DIRECTORY}"
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
	"${SOURCE_DIRECTORY}/MeshPool.cpp"
	"${SOURCE_DIRECTORY}/Ray.cpp"
	"${SOURCE_DIRECTORY}/Triangle.cpp"
	"${SOURCE_DIRECTORY}/Vertex.cpp"
	"${SOURCE_DIRECTORY}/Material.cpp"
	"${SOURCE_DIRECTORY}/CubeMap.cpp"
	"${SOURCE_DIRECTORY}/Camera.cpp"
	"${SOURCE_DIRECTORY}/RenderUtils.cpp"

)

set(RESOURCE_DIRECTORIES
    "resources/"
)
add_executable(SimpleRayTracer "${INCLUDE_FILES}" "${SOURCE_FILES}")
target_include_directories(SimpleRayTracer PRIVATE "${INCLUDE_DIRECTORIES}")


foreach(RESOURCE_DIRECTORY ${RESOURCE_DIRECTORIES})
    add_custom_command(
        TARGET SimpleRayTracer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/${RESOURCE_DIRECTORY}"
            "${CMAKE_BINARY_DIR}/${RESOURCE_DIRECTORY}"
    )
endforeach()