cmake_minimum_required(VERSION 3.10)

project(vec-tests VERSION 1.0.0 LANGUAGES C)


set(SRCS_PATH "src/*.c")
set(INCLUDE_DIR "miunte/")

file(GLOB_RECURSE SRCS ${SRCS_PATH})


set(MAIN "main")

add_executable(${MAIN} ${SRCS})
target_include_directories(${MAIN} PRIVATE ${INCLUDE_DIR})

set(COMPILE_AND_LINK_OPTIONS 
    -Og -g
    -W -Wall -Wextra -Wformat -pedantic
    -fsanitize=address)
target_compile_options(${MAIN} PRIVATE ${COMPILE_AND_LINK_OPTIONS})
target_link_options(${MAIN} PRIVATE ${COMPILE_AND_LINK_OPTIONS})

target_link_libraries(${MAIN} vec)

add_subdirectory(".." "vec/")
