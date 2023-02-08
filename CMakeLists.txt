# CMakeList.txt: luaByCMake 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

if(WIN32) #如果是MSVC项目的话，请把set(ENV{PATH}..)那一行注释掉，以免干扰MSVC找编译器
set(MINGW_HOME $ENV{MINGW_HOME}) #获取系统的环境变量MINGW_HOME，前提是系统环境变量中有这个变量，不然还是得手动添加绝对路径
#set(ENV{PATH} $ENV{MINGW_HOME}\\bin) #由于cmake过程中只需要编译链接的工具，所以就在作用域内直接把环境变量PATH设置为MinGW的bin目录
endif()

project ("luaByCMake")

message(STATUS ${CMAKE_C_COMPILER_ID}) #w为了方便起见，一进来就打印出编译器

set(CMAKE_C_FLAGS "-O2 ") # 去掉-Wall可以不显示所有警告
if(CMAKE_HOST_UNIX)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -DLUA_COMPAT_5_3 -DLUA_USE_LINUX")
elseif(CMAKE_HOST_WIN32)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLUA_COMPAT_5_3 -DLUA_BUILD_AS_DLL") #Wextra在VS下会报错
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) #设置静态库输出目录

#message(STATUS ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
include_directories(${CMAKE_SOURCE_DIR}/include) #设置头文件所在目录
aux_source_directory(${CMAKE_SOURCE_DIR}/src LIB_SRC_FILES) #设置要生成的静态库的源文件所在目录，从而选中该目录下所有文件

#这两个文件编译静态库的时候要排除
set(LUA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c) #编译可执行文件lua的时候需要
set(LUAC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c) #编译可执行文件luac的时候需要

list(REMOVE_ITEM LIB_SRC_FILES  ${LUA_PATH} ${LUAC_PATH}) #从源文件中剔除lua.c和luac.c,注意这里必须传入的是绝对路径，否则无法删除

add_library(liblua_static STATIC ${LIB_SRC_FILES}) #添加生成静态库目标liblua_static
add_library(liblua_shared SHARED ${LIB_SRC_FILES}) #添加生成动态库目标liblua_shared

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
set_target_properties(liblua_static PROPERTIES OUTPUT_NAME lua) #GNU编译产生的库名前面会自己加上lib
set_target_properties(liblua_shared PROPERTIES OUTPUT_NAME lua)
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
set_target_properties(liblua_static PROPERTIES OUTPUT_NAME liblua) #MSVC编译产生的库名就是原来的库名
set_target_properties(liblua_shared PROPERTIES OUTPUT_NAME lua54)
else()
#其他情况下没试过，手头没有MacOS设备，半夜三更的也懒得装个黑苹果试试
endif()

find_library(liblua_static ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}) #在输出路径下找产生的静态库

add_executable(luac ${LUAC_PATH}) #添加生成可执行文件目标luac
add_executable(lua ${LUA_PATH}) #添加生成可执行文件目标lua

if(CMAKE_HOST_UNIX)
#类UNIX系统下就需要这样的链接
target_link_libraries(luac liblua_static m -Wl,-E dl) #链接可执行文件和静态库和系统库
target_link_libraries(lua liblua_static m -Wl,-E dl) #链接可执行文件和静态库和系统库
elseif(CMAKE_HOST_WIN32)
target_link_libraries(luac liblua_static) #链接可执行文件和静态库
find_library(liblua_shared ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}) #在输出路径下找产生的动态库
target_link_libraries(lua liblua_shared)
else()
#其他情况下没试过
endif()

#add_subdirectory(testlib ) #添加子项目目录