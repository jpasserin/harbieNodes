cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)

project(harbieNodes)
set(MAYA_VERSION "2022" CACHE STRING "The Maya Version")

find_package(Maya REQUIRED)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/../output/${MAYA_VERSION})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set(SOURCE_FILES
    "sources/_Utils.cpp"
    "headers/_Utils.h"
    "sources/pluginMain.cpp" 
    "sources/AbsRotationTrackerNode.cpp"
    "headers/AbsRotationTrackerNode.h"
    "sources/BeanRemoteNode.cpp"
    "headers/BeanRemoteNode.h"
    "sources/CorneBulgeInitNode.cpp"
    "headers/CorneaBulgeInitNode.h"
    "sources/CorneaBulgeNode.cpp"
    "headers/CorneaBulgeNode.h"
    "sources/CurveMultiAttachNode.cpp"
    "headers/CurveMultiAttachNode.h"
    "sources/FkIk2BonesNode.cpp"
    "headers/FkIk2BonesNode.h"
    "sources/InverseRotOrderNode.cpp"
    "headers/InverseRotOrderNode.h"
    "sources/MeshMultiAttachNode.cpp"
    "headers/MeshMultiAttachNode.h"
    "sources/PointAtBlendedAxisNode.cpp"
    "headers/PointAtBlendedAxisNode.h"
    "sources/PointAtDoubleAxisNode.cpp"
    "headers/PointAtDoubleAxisNode.h"
    "sources/PoseConstraintNode.cpp"
    "headers/PoseConstraintNode.h"
    "sources/PreviewPlaneNode.cpp"
    "headers/PreviewPlaneNode.h"
    "sources/RotationTrackerNode.cpp"
    "headers/RotationTrackerNode.h"
    "sources/SpinePointAtNode.cpp"
    "headers/SpinePointAtNode.h"
    "sources/SplitMatrixNode.cpp"
    "headers/SplitMatrixNode.h"
    "sources/SurfaceInfoCmd.cpp"
    "headers/SurfaceInfoCmd.h"
    "sources/SurfaceMultiAttachNode.cpp"
    "headers/SurfaceMultiAttachNode.h"
    "sources/rotationToSliderNode.cpp"
    "headers/rotationToSliderNode.h"
)

include_directories(${MAYA_INCLUDE_DIR} maya lib)
link_directories(${MAYA_LIBRARY_DIR})
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES})

MAYA_PLUGIN(${PROJECT_NAME})
install(TARGETS ${PROJECT_NAME} ${MAYA_TARGET_TYPE} DESTINATION .)

