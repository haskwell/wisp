cmake_minimum_required(VERSION 3.15)
project(wisp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default to Debug if you don't pick a build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(XKBCOMMON REQUIRED IMPORTED_TARGET xkbcommon)

add_library(wisp
        # Core
        src/platform/wayland/core/context.cpp
        src/platform/wayland/core/display.cpp
        src/platform/wayland/core/event_manager.cpp
        src/platform/wayland/core/globals.cpp
        src/platform/wayland/core/object_manager.cpp
        src/platform/wayland/core/proxy.cpp
        src/platform/wayland/core/shmBuffer.cpp
        src/platform/wayland/core/window.cpp
        src/platform/wayland/core/xkb.cpp

        # Protocol
        src/platform/wayland/protocol/wl_buffer.cpp
        src/platform/wayland/protocol/wl_callback.cpp
        src/platform/wayland/protocol/wl_compositor.cpp
        src/platform/wayland/protocol/wl_display.cpp
        src/platform/wayland/protocol/wl_keyboard.cpp
        src/platform/wayland/protocol/wl_pointer.cpp
        src/platform/wayland/protocol/wl_registry.cpp
        src/platform/wayland/protocol/wl_seat.cpp
        src/platform/wayland/protocol/wl_shm.cpp
        src/platform/wayland/protocol/wl_shm_pool.cpp
        src/platform/wayland/protocol/wl_surface.cpp
        src/platform/wayland/protocol/xdg_surface.cpp
        src/platform/wayland/protocol/xdg_toplevel.cpp
        src/platform/wayland/protocol/xdg_wm_base.cpp
        src/platform/wayland/protocol/zxdg_decoration_manager.cpp
        src/platform/wayland/protocol/zxdg_toplevel.cpp

        # Wire
        src/platform/wayland/wire/connection.cpp
        src/platform/wayland/wire/deserializer.cpp
        src/platform/wayland/wire/serializer.cpp
        src/platform/wayland/wire/wayland_utils.cpp
)

target_include_directories(wisp
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_compile_options(wisp PRIVATE -Wall -Wextra -fsanitize=address,undefined)

target_compile_options(wisp PRIVATE
        -Wall -Wextra
        -fsanitize=address,undefined
)

target_link_options(wisp PUBLIC
        -fsanitize=address,undefined
)

target_link_libraries(wisp PUBLIC PkgConfig::XKBCOMMON)

add_executable(wisp_test src/main.cpp)
target_link_libraries(wisp_test PRIVATE wisp)