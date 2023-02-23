# CMakeList.txt: luaByCMake 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

#######弃用1#############
if(WIN32) #如果是MSVC项目的话，请把set(ENV{PATH}..)那一行注释掉，以免干扰MSVC找编译器
set(MINGW_HOME $ENV{MINGW_HOME}) #获取系统的环境变量MINGW_HOME，前提是系统环境变量中有这个变量，不然还是得手动添加绝对路径
#set(ENV{PATH} $ENV{MINGW_HOME}\\bin) #由于cmake过程中只需要编译链接的工具，所以就在作用域内直接把环境变量PATH设置为MinGW的bin目录
endif()
#######弃用1结束#########

#设置项目名
project ("luaByCMake")

message(STATUS ${CMAKE_C_COMPILER_ID}) #为了方便起见，一进来就打印出编译器

#########设置编译器选项和预定义选项##############
#设置通用的编译器选项、预定义
add_compile_options(-O2 ) #-Wall开启全部警告
#add_compile_definitions(NDEBUG )
add_compile_definitions(LUA_COMPAT_5_3 )

#以下分编译器的编译器选项和预定义
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
if(WIN32)
add_compile_options(-s )
endif()
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
##喵喵喵
else()
##其他情况
endif()
#以下分平台的编译器选项和预定义
if(WIN32) 
#MSVC动态库需要暴露接口，所以要添加编译器预定义暴露接口(使用lua源码自带的预定义)
add_compile_definitions(LUA_BUILD_AS_DLL)
else()
add_compile_options(-Wl,-E )
add_compile_options(-lm -ldl ) #加载数学库、显示加载动态库
if(APPLE)
add_compile_options(-lreadline )
add_compile_definitions(LUA_USE_MACOSX )
add_compile_definitions(LUA_USE_READLINE )
else()
add_compile_definitions(LUA_USE_LINUX )
endif()
endif()
#########设置编译器选项和预定义选项##############

#########设置目标生成目录##############
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
#########设置目标生成目录##############

#######设置头文件包含目录和编译源文件################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include) #设置头文件所在目录
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LIB_SRC_FILES) #设置要生成的静态库的源文件所在目录，从而选中该目录下所有文件
#这两个文件编译静态库的时候要排除
set(LUA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c) #编译可执行文件lua的时候需要
set(LUAC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c) #编译可执行文件luac的时候需要

list(REMOVE_ITEM LIB_SRC_FILES  ${LUA_PATH} ${LUAC_PATH}) #从源文件中剔除lua.c和luac.c,注意这里必须传入的是绝对路径，否则无法删除
#######设置头文件包含目录和编译源文件################

###########添加生成库目标################
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
###########添加生成库目标################

###########添加生成可执行文件目标################
add_executable(luac ${LUAC_PATH}) #添加生成可执行文件目标luac
add_executable(lua ${LUA_PATH}) #添加生成可执行文件目标lua

target_link_libraries(luac liblua_static ) #链接可执行文件和静态库和系统库
if(CMAKE_HOST_UNIX)
target_link_libraries(lua liblua_static ) #链接可执行文件和静态库和系统库
elseif(CMAKE_HOST_WIN32)
#最上面的代码中WIN32加上了LUA_BUILD_AS_DLL编译参数，使得VS项目中具有函数导出接口，所以可以直接进行连接动态库
target_link_libraries(lua liblua_shared )
else()
#其他情况下没试过
endif()
###########添加生成可执行文件目标################



###########添加生成子项目目录################
add_subdirectory(libdemo ) #添加子项目目录
###########添加生成子项目目录################