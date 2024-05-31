cmake_minimum_required(VERSION 3.9)
project(yuv2rgb_android)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin/)

set(CMAKE_CXX_STANDARD 11)
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})
message("current dir : ", ${CMAKE_SOURCE_DIR})


set(LibPath ${CMAKE_SOURCE_DIR})
# 添加opencv includes 路径和库
message("platfrom : "  ${platfrom})

# add opencv link_libraries()
# set(OpenCV_DIR  ${CMAKE_SOURCE_DIR}/3rdparty/OpenCV/sdk/native/jni/abi-arm64-v8a)
# set(OpenCV_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/3rdparty/OpenCV/sdk/native/jni/include/opencv2)
# set(OpenCV_LIBRARIES ${CMAKE_SOURCE_DIR}/3rdparty/OpenCV/sdk/native/libs/arm64-v8a)
# list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/3rdparty/OpenCV)


set(opencv_path ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/OpenCV)
set( ENV{OpenCV_DIR} ${opencv_path})

find_package(OpenCV REQUIRED)

message("opencv  INCLUDE  : " ${OpenCV_INCLUDE_DIRS})
message("opencv LIBRARIES : " ${OpenCV_LIBRARIES})
# include_directories(${OpenCV_INCLUDE_DIRS})
# link_directories(${OpenCV_LIBRARIES})
# find_package(OpenCV REQUIRED)

include_directories(
        ${CMAKE_SOURCE_DIR}/yuv_converter.h
)

include_directories(
        ${CMAKE_SOURCE_DIR}/3rdparty/libyuv/include
)

link_directories(
        ${CMAKE_SOURCE_DIR}/3rdparty/libyuv/lib
)
##  executable
add_executable(${PROJECT_NAME} 
            main2.cpp
			yuv_converter.cpp
            )
target_link_libraries(${PROJECT_NAME} 
           yuv
           ${OpenCV_LIBS}
           -ljpeg
        )



