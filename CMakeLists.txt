cmake_minimum_required(VERSION 3.6)
project(super_res)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)
find_package(OpenCV)


set(SOURCE_FILES
        main.cpp
        MedianEstimation.cpp
        MedianEstimation.h
        OpticalFlow.cpp
        OpticalFlow.h
        ImageLoader.cpp
        ImageLoader.h
        Parameters.h ImageSet.h SimpleImageSet.cpp SimpleImageSet.h SuperResolution.cpp SuperResolution.h util.h Timer.h PointSpreadFunction.h util.cpp)

add_executable(super_res ${SOURCE_FILES})

find_package( OpenCV )
include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories(${DIR} /usr/local/include)
link_directories (${DIR} /usr/local/lib)

TARGET_LINK_LIBRARIES(super_res
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        opencv_core
        opencv_imgproc
        opencv_video
        opencv_optflow
        ${EXTERNAL_LIBS})

target_link_libraries(super_res ${OpenCV_LIBS})