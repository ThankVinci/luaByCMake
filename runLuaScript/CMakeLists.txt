#########用于调试的变量###################
set(RLUA_EXEC OFF)
#########用于调试的变量###################

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

if(RLUA_EXEC)
add_compile_definitions(RLUA_EXEC )
endif()
#########设置编译器选项和预定义选项##############

#########设置目标生成目录##############
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
#########设置目标生成目录##############

#######设置头文件包含目录和编译源文件################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src RUNLUA_SRC_FILE)
#######设置头文件包含目录和编译源文件################

###########添加生成目标################
if(RLUA_EXEC)
add_executable(rlua ${RUNLUA_SRC_FILE}) #添加生成可执行文件目标
target_link_libraries(rlua liblua_shared) #必须链接lua的动态库
else()
add_library(rlua_shared SHARED ${RUNLUA_SRC_FILE}) #添加生成动态库目标
set_target_properties(rlua_shared PROPERTIES OUTPUT_NAME rua)
target_link_libraries(rlua_shared liblua_shared) #必须链接lua的动态库
endif()
###########添加生成库目标################

###########生成文件后进行安装##############

if(RLUA_EXEC)
install(TARGETS rlua 
			RUNTIME DESTINATION ${INSTALL_DIRECTORY}/bin
		)
else()
install(TARGETS rlua_shared 
			RUNTIME DESTINATION ${INSTALL_DIRECTORY}/lib
			LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib
			ARCHIVE DESTINATION ${INSTALL_DIRECTORY}/lib
		)
endif()
###########生成文件后进行安装##############