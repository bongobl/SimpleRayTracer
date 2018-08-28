cmake_minimum_required(VERSION 3.0)
project(SimpleRayTracer)


include("external/stb.cmake")
include("external/tinyobjloader.cmake")
include("external/glm.cmake")

set(INCLUDE_DIRECTORY "${CMAKE_SOURCE_DIR}/include/")
set(INCLUDE_FILES
	"${INCLUDE_DIRECTORY}/Image.h"
	"${INCLUDE_DIRECTORY}/Model.h"
	"${INCLUDE_DIRECTORY}/Ray.h"
	"${INCLUDE_DIRECTORY}/Triangle.h"
	"${INCLUDE_DIRECTORY}/Vertex.h"
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
	"${SOURCE_DIRECTORY}/Image.cpp"
	"${SOURCE_DIRECTORY}/Model.cpp"
	"${SOURCE_DIRECTORY}/Ray.cpp"
	"${SOURCE_DIRECTORY}/Triangle.cpp"
	"${SOURCE_DIRECTORY}/Vertex.cpp"
)

set(RESOURCE_DIRECTORIES
    "ObjModels/"
)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
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