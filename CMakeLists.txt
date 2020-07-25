cmake_minimum_required(VERSION 3.10)

project(editor CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB source_files src/*.cpp src/*/*.cpp)

set(SDL_SHARED_ENABLED_BY_DEFAULT OFF)

add_subdirectory(deps/sdl2)
add_subdirectory(deps/entt)


add_executable(editor ${source_files})

target_include_directories(editor PRIVATE src)
target_link_libraries(editor PRIVATE EnTT SDL2-static ${EXTRA_LIBS})

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(editor PRIVATE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:/W4;/permissive->>"
)
