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

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long")

file(GLOB SOURCES *.c *.S)
add_executable(pong ${SOURCES})

target_link_libraries(pong ${LIBS})

install(TARGETS pong DESTINATION bin)
