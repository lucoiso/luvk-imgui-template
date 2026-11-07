# Author: Lucas Vilas-Boas
# Year: 2026
# Repo: https://github.com/lucoiso/luvk-imgui-template

CMAKE_MINIMUM_REQUIRED(VERSION 3.28)

PROJECT(luvk-imgui-template VERSION 0.1.0 LANGUAGES C CXX)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

OPTION(LUVK_TEMPLATE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

# luvk lives in a submodule (luvk/). LUVK_SOURCE_DIR lets you point at a local checkout while
# developing both repositories side by side.
SET(LUVK_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/luvk" CACHE PATH "Path to the luvk checkout")
IF (NOT EXISTS "${LUVK_SOURCE_DIR}/app/CMakeLists.txt")
    MESSAGE(FATAL_ERROR "The expected luvk checkout was not found at '${LUVK_SOURCE_DIR}'. Initialize the submodule or "
                        "set LUVK_SOURCE_DIR to a luvk checkout that contains the app/platform/imgui targets.")
ENDIF ()

# Pure ImGui template: application layer on, tests off. Normal variables (not FORCE'd cache entries) so nothing
# sticks in the cache when an option is turned back off. The luvk example is kept in the submodule and can be
# built with -DLUVK_BUILD_EXAMPLE=ON.
SET(LUVK_BUILD_APP ON)
SET(LUVK_BUILD_TESTS OFF)
SET(LUVK_WARNINGS_AS_ERRORS ${LUVK_TEMPLATE_WARNINGS_AS_ERRORS})
SET(LUVK_OUTPUT_DIR "${CMAKE_BINARY_DIR}")

ADD_SUBDIRECTORY("${LUVK_SOURCE_DIR}" "${CMAKE_BINARY_DIR}/luvk")

FILE(GLOB LUVK_IMGUI_TEMPLATE_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

ADD_EXECUTABLE(luvk-imgui-template ${LUVK_IMGUI_TEMPLATE_SOURCES})
TARGET_INCLUDE_DIRECTORIES(luvk-imgui-template PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
TARGET_LINK_LIBRARIES(luvk-imgui-template PRIVATE luvk::app)
TARGET_COMPILE_FEATURES(luvk-imgui-template PRIVATE cxx_std_23)

LUVK_APPLY_WARNINGS(luvk-imgui-template)
LUVK_SET_OUTPUT_DIRECTORIES(luvk-imgui-template)
