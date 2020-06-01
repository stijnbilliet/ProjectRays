cmake_minimum_required(VERSION 3.6)

#set module path for custom cmake
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CMake")

#project setup
project(SwiftByte3D)

#find assimp
find_package(assimp REQUIRED)
message(STATUS "Found assimp in ${ASSIMP_INCLUDE_DIRS}")
include_directories(${ASSIMP_INCLUDE_DIRS})

#find OpenCL
find_package(OpenCL REQUIRED) #install from CUDA pkg
message(STATUS "Found OpenCL in ${OpenCL_INCLUDE_DIRS}")
include_directories(${OpenCL_INCLUDE_DIRS})

#find SDL2
find_package(SDL2 REQUIRED)
message(STATUS "Found SDL2 in ${SDL2_INCLUDE_DIR}")
include_directories(${SDL2_INCLUDE_DIR})

#find GLM
find_package(glm REQUIRED)
message(STATUS "Found GLM in ${GLM_INCLUDE_DIRS}")
include_directories(${GLM_INCLUDE_DIRS})

#find stb
set(STB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb_image/include")
include_directories(${STB_INCLUDE_DIR})
message(STATUS "stb included from ${STB_INCLUDE_DIR}.")

#find glad
#added source directly into project saves us from glads source regeneration
set(GLAD_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glad/include")
include_directories(${GLAD_INCLUDE_DIR})

#find radeonrays
message(STATUS "Adding external project link to RadeonRays")
include(ExternalProject)
ExternalProject_Add(
    RadeonRaysSDK
    GIT_REPOSITORY "https://github.com/GPUOpen-LibrariesAndSDKs/RadeonRays_SDK.git"
    GIT_TAG "legacy-2.0"
    SOURCE_DIR ${CMAKE_BINARY_DIR}/3rdParty/RadeonRays
    BINARY_DIR ${CMAKE_BINARY_DIR}/3rdParty/RadeonRays-build
)
#find CLW

#run project setup
add_subdirectory(Projects)