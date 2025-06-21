cmake_minimum_required(VERSION 3.22)

# project name
set(FLX_PROJECT_NAME OpenGLEngine)

project(${FLX_PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

# options
option(FLX_WITH_CONSOLE "With Console" OFF)

if(CMAKE_CONFIGURATION_TYPES)
    set(IS_MULTI_CONFIG TRUE)
else()
    set(IS_MULTI_CONFIG FALSE)
endif()

# global settings
if (MSVC)
    add_compile_definitions(UNICODE _UNICODE)
    add_compile_options(/utf-8)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # MSVC use static CRT library (/MT)
endif ()

# set output dir
set(FLX_EXE_DIR "${CMAKE_BINARY_DIR}/bin")
set(FLX_STATIC_LIB_DIR "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FLX_EXE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FLX_STATIC_LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FLX_STATIC_LIB_DIR})

# configure files
configure_file(Config/Config.h.in ${CMAKE_BINARY_DIR}/Config/Config.h)

# build shared libraries
set(BUILD_SHARED_LIBS ON)

# ======================================= build and add source code =======================================

# main
if (FLX_WITH_CONSOLE)
    add_executable(${FLX_PROJECT_NAME})
    target_sources(${FLX_PROJECT_NAME} PRIVATE Main/main.cpp)
else ()
    add_executable(${FLX_PROJECT_NAME} WIN32)
    target_sources(${FLX_PROJECT_NAME} PRIVATE Main/WinMain.cpp)
endif ()
target_sources(${FLX_PROJECT_NAME} PRIVATE Main/AppMain.h Main/AppMain.cpp)

# Core
file(GLOB_RECURSE FLX_CORE_SOURCE CONFIGURE_DEPENDS
        Core/*.h
        Core/*.cpp
)
target_sources(${FLX_PROJECT_NAME} PRIVATE ${FLX_CORE_SOURCE})

# Graphic
file(GLOB_RECURSE FLX_GRAPHIC_SOURCE CONFIGURE_DEPENDS
        Graphic/*.h
        Graphic/*.cpp
)
target_sources(${FLX_PROJECT_NAME} PRIVATE ${FLX_GRAPHIC_SOURCE})

# Component
file(GLOB_RECURSE FLX_COMPONENT_SOURCE CONFIGURE_DEPENDS
        Component/*.h
        Component/*.cpp
)
target_sources(${FLX_PROJECT_NAME} PRIVATE ${FLX_COMPONENT_SOURCE})

# AssetClass
file(GLOB_RECURSE FLX_ASSET_CLASS_SOURCE CONFIGURE_DEPENDS
        AssetClass/*.h
        AssetClass/*.cpp
)
target_sources(${FLX_PROJECT_NAME} PRIVATE ${FLX_ASSET_CLASS_SOURCE})

# Render
file(GLOB_RECURSE FLX_RENDER_SOURCE CONFIGURE_DEPENDS
        Render/*.h
        Render/*.cpp
)
target_sources(${FLX_PROJECT_NAME} PRIVATE ${FLX_RENDER_SOURCE})

# Managers
file(GLOB_RECURSE FLX_MANAGERS_SOURCE CONFIGURE_DEPENDS
        Managers/*.h
        Managers/*.cpp
)
target_sources(${FLX_PROJECT_NAME} PRIVATE ${FLX_MANAGERS_SOURCE})

# Misc
file(GLOB_RECURSE FLX_MISC_SOURCE CONFIGURE_DEPENDS
        Misc/*.h
        Misc/*.cpp
)
target_sources(${FLX_PROJECT_NAME} PRIVATE ${FLX_MISC_SOURCE})

# Platform
file(GLOB_RECURSE FLX_PLATFORM_SOURCE CONFIGURE_DEPENDS
        Platform/*.h
        Platform/*.cpp
)
target_sources(${FLX_PROJECT_NAME} PRIVATE ${FLX_PLATFORM_SOURCE})

# Game
file(GLOB_RECURSE FLX_GAME_SOURCE CONFIGURE_DEPENDS
        Game/*.h
        Game/*.cpp
)
target_sources(${FLX_PROJECT_NAME} PRIVATE ${FLX_GAME_SOURCE})

# my target properties
# SIMD
target_compile_definitions(${FLX_PROJECT_NAME} PRIVATE
        GLM_FORCE_INTRINSICS
        GLM_FORCE_SIMD_AVX2
)
if (MSVC)
    target_compile_options(${FLX_PROJECT_NAME} PRIVATE /arch:AVX2)
else ()
    target_compile_options(${FLX_PROJECT_NAME} PRIVATE -mavx2)
endif ()
# my macros
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${FLX_PROJECT_NAME} PRIVATE FLX_DEBUG_BUILD)
endif ()

# include directories
target_include_directories(${FLX_PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

# ============================================ 3rd party libraries ============================================

# Windows library
target_link_libraries(${FLX_PROJECT_NAME} PRIVATE winmm.lib)

# GLM library
target_include_directories(${FLX_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/3rdParty/glm/include")

# OpenGL library
target_link_libraries(${FLX_PROJECT_NAME} PRIVATE opengl32.lib)

# stb library
target_include_directories(${FLX_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/3rdParty/stb/include")
target_sources(${FLX_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/3rdParty/stb/StbImage.cpp")

# GLFW library (shared)
add_subdirectory(3rdParty/glfw)
target_link_libraries(${FLX_PROJECT_NAME} PRIVATE glfw)
set_target_properties(glfw PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${FLX_EXE_DIR})

# GLAD library
target_include_directories(${FLX_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/3rdParty/glad/include")
target_sources(${FLX_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/3rdParty/glad/src/glad.c")

# Assimp library (shared)
set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory("${CMAKE_SOURCE_DIR}/3rdParty/assimp")
target_link_libraries(${FLX_PROJECT_NAME} PRIVATE assimp::assimp)
set_target_properties(assimp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${FLX_EXE_DIR})

# ImGui library
target_include_directories(${FLX_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/3rdParty/imgui")
target_include_directories(${FLX_PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/3rdParty/imgui/imgui")
target_sources(${FLX_PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/3rdParty/imgui/imgui/imgui.cpp"
        "${CMAKE_SOURCE_DIR}/3rdParty/imgui/imgui/imgui_draw.cpp"
        "${CMAKE_SOURCE_DIR}/3rdParty/imgui/imgui/imgui_tables.cpp"
        "${CMAKE_SOURCE_DIR}/3rdParty/imgui/imgui/imgui_widgets.cpp"
        "${CMAKE_SOURCE_DIR}/3rdParty/imgui/imgui/backends/imgui_impl_glfw.cpp"
        "${CMAKE_SOURCE_DIR}/3rdParty/imgui/imgui/backends/imgui_impl_opengl3.cpp"
)

# ============================================ commands after build ============================================

# Copy resources files
if(IS_MULTI_CONFIG)
    set(FLX_EXE_DIR_REAL "${FLX_EXE_DIR}/$<CONFIG>")
else()
    set(FLX_EXE_DIR_REAL "${FLX_EXE_DIR}")
endif()
add_custom_command(
        TARGET ${FLX_PROJECT_NAME} POST_BUILD

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Shaders"
        "${FLX_EXE_DIR_REAL}/Shaders"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Resources"
        "${FLX_EXE_DIR_REAL}/Resources"
)

# ============================================ install ============================================

# install settings
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/Install)
install(
        TARGETS ${FLX_PROJECT_NAME}
)