# CMakeList.txt: luaByCMake 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

project ("luaByCMake")

set(CMAKE_DEBUG_POSTFIX "d") #设置生成debug库的后缀

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #设置静态库输出目录

#message(STATUS ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
include_directories(${CMAKE_SOURCE_DIR}/include) #设置头文件所在目录
aux_source_directory(${CMAKE_SOURCE_DIR}/src LIB_SRC_FILES) #设置要生成的静态库的源文件所在目录，从而选中该目录下所有文件

#这两个文件编译静态库的时候要排除
set(LUA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c") #编译可执行文件的时候需要
set(LUAC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c") #编译动态库的时候需要

list(REMOVE_ITEM LIB_SRC_FILES  ${LUA_PATH} ${LUAC_PATH}) #从源文件中剔除lua.c和luac.c,注意这里必须传入的是绝对路径，否则无法删除

add_library(liblua STATIC ${LIB_SRC_FILES}) #添加生成静态库目标liblua
find_library(liblua ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}) #在输出路径下找产生的静态库

add_executable(lua ${LUA_PATH}) #添加生成可执行文件目标lua
TARGET_LINK_LIBRARIES(lua liblua) #链接可执行文件和静态库

add_executable(luac ${LUAC_PATH}) #添加生成可执行文件目标luac
TARGET_LINK_LIBRARIES(luac liblua) #链接可执行文件和静态库

#为了避免干扰最后才生成动态库（傻了，静态库和动态库名字不一样应该不会产生干扰）
add_library(lua54 SHARED ${LIB_SRC_FILES}) #添加生成动态库目标lua54