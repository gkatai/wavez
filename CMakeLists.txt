cmake_minimum_required(VERSION 3.10)

project(wavez C)

# Export compile commands for language servers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C standard to C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Display build type
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Add raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/raylib)

# Add raygui (header-only library)
set(RAYGUI_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/raygui/src)

# Add source files
file(GLOB SOURCES "src/*.c")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link raylib
target_link_libraries(${PROJECT_NAME} raylib)

# Include raygui
target_include_directories(${PROJECT_NAME} PRIVATE ${RAYGUI_INCLUDE_DIR})

# Optional: Add compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

# WebAssembly specific settings
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(${PROJECT_NAME} PRIVATE
        -sWASM=1
        -sALLOW_MEMORY_GROWTH=1
        -sASYNCIFY=1
        --preload-file=${CMAKE_SOURCE_DIR}/assets@assets
    )

    # Enable AddressSanitizer in Debug builds
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE
            -fsanitize=address
        )
        target_link_options(${PROJECT_NAME} PRIVATE
            -fsanitize=address
            -sASSERTIONS=1
        )
        message(STATUS "AddressSanitizer enabled for Debug build")
    endif()

    # Add run target
    add_custom_target(run
        COMMAND emrun --no_browser ${PROJECT_NAME}.html
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running ${PROJECT_NAME} with emrun"
    )

    # Add debug and release targets
    add_custom_target(debug
        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Reconfiguring and building in Debug mode"
    )

    add_custom_target(release
        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Reconfiguring and building in Release mode"
    )
endif()

