cmake_minimum_required(VERSION 3.8)
project(BTP)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES xy_cut.cpp )
add_executable(BTP ${SOURCE_FILES})
cmake_minimum_required(VERSION 2.8.4)
#project(OpenCVTest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package( OpenCV REQUIRED )
set(SOURCE_FILES xy_cut.cpp)
add_executable(OpenCVTest ${SOURCE_FILES})
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries( OpenCVTest ${OpenCV_LIBS} )
include_directories("/usr/local/include")
TARGET_LINK_LIBRARIES( BTP opencv_core opencv_imgproc opencv_video opencv_imgcodecs opencv_core
        opencv_highgui ${EXTERNAL_LIBS})
