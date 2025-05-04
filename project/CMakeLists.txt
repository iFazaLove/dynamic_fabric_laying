cmake_minimum_required(VERSION 3.10)
project(FabricSim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Подключаем все .cpp-файлы рекурсивно
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/Fabric/*.cpp"
    "${CMAKE_SOURCE_DIR}/Object/*.cpp"
    "${CMAKE_SOURCE_DIR}/Vec/*.cpp"
)

include_directories("${CMAKE_SOURCE_DIR}/project")

add_executable(FabricSim ${SOURCES})