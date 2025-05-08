cmake_minimum_required(VERSION 3.10)
project(FabricSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL Widgets OpenGLWidgets)
find_package(OpenGL REQUIRED)

if(APPLE AND NOT TARGET GLU::GLU)
    add_library(GLU::GLU INTERFACE IMPORTED)
    set_target_properties(GLU::GLU PROPERTIES
        INTERFACE_LINK_LIBRARIES "-framework OpenGL"
    )
endif()

qt_standard_project_setup()

include_directories(
    ${CMAKE_SOURCE_DIR}
)

# add_definitions(-DENABLE_CLOTH_LOG) // Добавление логгирования в проект

# Секции исходников
set(SIM_SRC
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/Fabric/Cloth.cpp
    ${CMAKE_SOURCE_DIR}/Object/Object.cpp
    ${CMAKE_SOURCE_DIR}/Vec/Vec3.cpp
)

set(VIEWER_SRC
    ${CMAKE_SOURCE_DIR}/viewer/ui/camera/CameraSettingsDialog.cpp
    ${CMAKE_SOURCE_DIR}/viewer/ui/window/MainWindow.cpp
    ${CMAKE_SOURCE_DIR}/viewer/ui/simulation/SimulationSettingsDialog.cpp
    ${CMAKE_SOURCE_DIR}/viewer/viewer_main.cpp
    ${CMAKE_SOURCE_DIR}/viewer/ClothViewer.cpp
    ${CMAKE_SOURCE_DIR}/Fabric/Cloth.cpp
    ${CMAKE_SOURCE_DIR}/Object/Object.cpp
    ${CMAKE_SOURCE_DIR}/Vec/Vec3.cpp
)

# 👉 1. Симулятор
add_executable(simulate ${SIM_SRC})

# 👉 2. Визуализация
add_executable(viewer ${VIEWER_SRC})
target_link_libraries(viewer
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    OpenGL::GL
    GLU::GLU
)