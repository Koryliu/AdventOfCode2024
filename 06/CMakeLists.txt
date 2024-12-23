cmake_minimum_required(VERSION 3.10)

project(AoC_Day06)

set(C gcc)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g")

file(GLOB SOURCE "src/*.c")

# Main Program Executable
add_executable(AoC_Day06 ${SOURCE})
set_target_properties(AoC_Day06 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../bin/")
