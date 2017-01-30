cmake_minimum_required(VERSION 3.6)
project(super_res)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


include_directories(${DIR} /usr/local/include)
link_directories (${DIR} /usr/local/lib)


FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)

find_package(OpenCV)

add_subdirectory(ext ext_build)
include_directories(
        # GLFW library for OpenGL context creation
        ${GLFW_INCLUDE_DIR}
        # GLEW library for accessing OpenGL functions
        ${GLEW_INCLUDE_DIR}
        # NanoVG drawing library
        ${NANOVG_INCLUDE_DIR}
        # NanoGUI user interface library
        ${NANOGUI_INCLUDE_DIR}
        ${NANOGUI_EXTRA_INCS}
        ${OpenCV_INCLUDE_DIRS}
)


set(SOURCE_FILES
        main.cpp
        MedianEstimation.cpp
        MedianEstimation.h
        OpticalFlow.cpp
        OpticalFlow.h
        ImageLoader.cpp
        ImageLoader.h
        Parameters.h
        ImageSet.h
        SimpleImageSet.cpp
        SimpleImageSet.h
        SuperResolution.cpp
        SuperResolution.h
        util.h
        Timer.h
        PointSpreadFunction.h
        util.cpp
        SuperResolutionApplication.cpp
        SuperResolutionApplication.h
)




add_executable(super_res ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(super_res
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        opencv_core
        opencv_imgproc
        opencv_video
        opencv_optflow
        ${EXTERNAL_LIBS}
        ${OpenCV_LIBS}
        nanogui
        ${NANOGUI_EXTRA_LIBS}
)

