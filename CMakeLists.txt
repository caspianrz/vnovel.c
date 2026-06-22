cmake_minimum_required(VERSION 3.20)

project(
	vnovel.c
	VERSION 0.1.0
	LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(VNOVEL_C_BUILD_EXAMPLE "Build examples." ON)

file(GLOB vnovel_sources "src/*.c")

add_library(vnovel.c ${vnovel_sources})
add_library(vnovel::lib ALIAS vnovel.c)

target_include_directories(
	vnovel.c
	PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

target_link_libraries(
	vnovel.c
	PUBLIC
	SDL3::SDL3
	SDL3_image::SDL3_image
)

# Asset copying for example project
set(ASSET_SOURCE_DIR ${CMAKE_SOURCE_DIR}/examples/assets)
set(ASSET_OUTPUT_DIR ${CMAKE_BINARY_DIR}/examples/assets)

add_custom_target(copy_assets ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${ASSET_SOURCE_DIR}
          ${ASSET_OUTPUT_DIR}
  COMMENT "Copying example assets to build directory"
)

if(VNOVEL_C_BUILD_EXAMPLE)
    set(examples
        "window:examples/window.c"
		"background:examples/background.c"
		"sample_scene:examples/sample_scene.c"
    )

	set(example_output_dir "${CMAKE_BINARY_DIR}/examples")

    foreach(example ${examples})
        string(REPLACE ":" ";" pair ${example})

        list(GET pair 0 name)
        list(GET pair 1 source)

        add_executable(${name} ${source})
        target_link_libraries(${name} PRIVATE vnovel::lib)

		set_target_properties(${name} PROPERTIES
        	RUNTIME_OUTPUT_DIRECTORY ${example_output_dir}
    	)

        target_include_directories(${name} PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include>
        )
    endforeach()
endif()
