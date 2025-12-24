# Author: Lucas Vilas-Boas
# Year: 2025
# Repo: https://github.com/lucoiso/luvk-imgui-template

FETCHCONTENT_DECLARE(imgui
                     GIT_REPOSITORY https://github.com/ocornut/imgui.git
                     GIT_TAG v1.92.5-docking
)
FETCHCONTENT_MAKEAVAILABLE(imgui)

SET(IMGUI_PRIVATE_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

IF (BUILD_SHARED_LIBS)
    ADD_LIBRARY(imgui SHARED ${IMGUI_PRIVATE_SOURCES})
    CORE_DEFINE_MODULE_API(imgui)
ELSE ()
    ADD_LIBRARY(imgui STATIC ${IMGUI_PRIVATE_SOURCES})
ENDIF ()

TARGET_INCLUDE_DIRECTORIES(imgui PUBLIC ${imgui_SOURCE_DIR})