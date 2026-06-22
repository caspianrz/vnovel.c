cmake_minimum_required(VERSION 3.20)

project(
	vnovel.c
	VERSION 0.1.0
	LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(VNOVEL_C_BUILD_EXAMPLE "Build examples." ON)

set(vnovel_sources "src/engine.c")

add_library(vnovel.c ${vnovel_sources})
add_library(vnovel::lib ALIAS vnovel.c)

target_include_directories(
	vnovel.c
	PUBLIC
	${PROJECT_SOURCE_DIR}/include
)

find_package(SDL3 REQUIRED)

target_link_libraries(
	vnovel.c
	PUBLIC
	SDL3::SDL3
)

if(VNOVEL_C_BUILD_EXAMPLE)
    set(examples
        "window:examples/window.c"
    )

	set(example_output_dir "${CMAKE_BINARY_DIR}/example")

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
