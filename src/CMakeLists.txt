enable_language(ASM)
include_directories(../include)

# Add libraries for linux
if(UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL_PKG sdl2)
    if(NOT SDL_PKG_FOUND)
        message(FATAL_ERROR "No sdl found")
    endif()
    include_directories(${SDL_PKG_INCLUDE_DIRS})
    LIST(APPEND LIBS ${SDL_PKG_LIBRARIES})
endif()

# Add libraries for windows
if(WIN32)
    include_directories(../sdl-mingw/include/SDL2)
    find_library(SDL_PKG_LIBRARIES SDL2 ${CMAKE_SOURCE_DIR}/sdl-mingw/lib)
    add_definitions( -D__WIN32__ )
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long")

set(SOURCES main.c paddles.c pong.c assets.S)
add_executable(pong ${SOURCES})
add_custom_command(TARGET pong POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/sdl-mingw/bin/SDL2.dll"
    $<TARGET_FILE_DIR:pong>)

target_link_libraries(pong ${SDL_PKG_LIBRARIES})

install(TARGETS pong DESTINATION bin)
