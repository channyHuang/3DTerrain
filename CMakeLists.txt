cmake_minimum_required(VERSION 3.14)

project(3DTerrain LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(osg_path /Users/channyhuang/Documents/thirdlib/OpenSceneGraph/include)
set(osg_lib /Users/channyhuang/Documents/thirdlib/OpenSceneGraph/build/lib)

include_directories(${osg_path})
include_directories("/usr/local/include")

set(IMGUI_PATH ./../commonClass/imgui)
set(COMMON_PATH ./../commonClass)
set(COMMONFUN_PATH ./../commonClass/commonFunc)
set(COMMONMATH_PATH ./../commonClass/commonMath)
set(COMMONGEOM_PATH ./../commonClass/commonGeometry)

set(GENERATOR_PATH ./generator)
set(VOXELS_PATH ./voxels)
set(CONTROLER_PATH ./controler)
include_directories("./../commonClass/imgui")
include_directories(${COMMON_PATH})
include_directories("./")

link_directories("/usr/local/lib")
link_directories("/usr/local/lib")
link_directories("/System/Library/Frameworks")
link_directories(${osg_lib})

add_definitions(-DGLFW_STATIC)

file(GLOB MAIN_FILE ./*.h ./*.cpp)
file(GLOB IMGUI_FILES ${IMGUI_PATH}/*.*)
file(GLOB GENERATOR_FILES ${GENERATOR_PATH}/*.h ${GENERATOR_PATH}/*.cpp)
file(GLOB VOXELS_FILES ${VOXELS_PATH}/*.h ${VOXELS_PATH}/*.cpp)
file(GLOB CONTROLER_FILES ${CONTROLER_PATH}/*.h ${CONTROLER_PATH}/*.cpp)
file(GLOB COMMONFUNC_FILES ${COMMONFUN_PATH}/*.h ${COMMONFUN_PATH}/*.cpp)
file(GLOB COMMONMath_FILES ${COMMONMATH_PATH}/*.h ${COMMONMATH_PATH}/*.cpp)
file(GLOB COMMONGeom_FILES ${COMMONGEOM_PATH}/*.h ${COMMONGEOM_PATH}/*.cpp)
#file(GLOB FILEDIALOG_FILES ${COMMONGEOM_PATH}/nativefiledialog/*.h ${COMMONGEOM_PATH}/nativefiledialog/*.c)

add_executable(3DTerrain
    ${MAIN_FILE}
    ${IMGUI_FILES}
    ${GENERATOR_FILES}
    ${VOXELS_FILES}
    ${CONTROLER_FILES}

    ${COMMONFUNC_FILES}
    ${COMMONMath_FILES}
    ${COMMONGeom_FILES}
    #${FILEDIALOG_FILES}
)

target_link_libraries(3DTerrain #[[Qt${QT_VERSION_MAJOR}::Core]]
    libglfw3.a
    libGLEW.a
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework CoreVideo"
    "-framework IOKit"
    osg
    osgViewer
    osgUtil
    osgGA
    osgDB
    OpenThreads

)

install(TARGETS 3DTerrain
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
