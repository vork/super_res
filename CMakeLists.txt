cmake_minimum_required(VERSION 3.6)
project(super_res)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp MedianEstimation.cpp MedianEstimation.h OpticalFlow.cpp OpticalFlow.h ImageLoader.cpp ImageLoader.h)

add_executable(super_res ${SOURCE_FILES})

include_directories(${DIR}/headers /usr/local/include)
link_directories (${DIR}/src/utils /usr/local/lib)

TARGET_LINK_LIBRARIES(super_res opencv_core opencv_imgproc opencv_video ${EXTERNAL_LIBS})