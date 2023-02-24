#########设置编译器选项和预定义选项##############
#设置通用的编译器选项、预定义

#以下分编译器的编译器选项和预定义
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
##喵喵喵
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
##喵喵喵
else()
##其他情况
endif()
#以下分平台的编译器选项和预定义
if(WIN32) 
#MSVC动态库需要暴露接口，所以要添加编译器预定义暴露接口(使用lua源码自带的预定义)
add_compile_definitions(LUA_LIB )
else()
if(APPLE)
#喵喵喵
else()
#喵喵喵
endif()
endif()
#########设置编译器选项和预定义选项##############

#########设置目标生成目录##############
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
#########设置目标生成目录##############

#######设置头文件包含目录和编译源文件################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src DEMOLIB_SRC_FILES)
#######设置头文件包含目录和编译源文件################


###########添加生成库目标################
#注：由于lua库名称的限制以及GNU编译器编译库文件名的特性，所以要删除前缀
add_library(demolib_shared SHARED ${DEMOLIB_SRC_FILES}) #添加生成动态库目标
set_target_properties(demolib_shared PROPERTIES OUTPUT_NAME demo)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
set_target_properties(demolib_shared PROPERTIES PREFIX "") #删除前缀
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
else()
endif()

target_link_libraries(demolib_shared liblua_shared) #必须链接lua的动态库
###########添加生成库目标################