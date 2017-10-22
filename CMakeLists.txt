project(QRcode)

#版本检查
cmake_minimum_required(VERSION 2.8)

#头文件目录
include_directories(include)

#源文件目录
aux_source_directory(src DIR_SRCS)

set(CMAKE_C_FLAGS -g)
set(LIBRARY_OUTPUT_PATH lib)

#add_library(rs SHARED src/reed-solomon.c)
#add_library(rs-static SHARED src/reed-solomon.c)


set (QR_SOURCE src)
add_library(qr-shared SHARED src/qr.c src/reed-solomon.c)
add_library(qr-static STATIC src/qr.c src/reed-solomon.c)

#添加可执行程序
add_executable(qr  main.c)
target_link_libraries(qr  qr-static)

