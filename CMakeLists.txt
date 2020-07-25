cmake_minimum_required(VERSION 3.10)

# set the project name
project(editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB source_files src/*.cpp)

find_package(sdl2 CONFIG REQUIRED)


# add the executable
add_executable(editor ${source_files})

target_link_libraries(editor PRIVATE sdl2)