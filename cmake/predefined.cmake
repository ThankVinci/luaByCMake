# 项目全局的预定义项
# 一些与项目实际内容无关的项和定义平台的项
# 在CMakeLists文件中project语句之后第一个include

set(CMAKE_DEBUG_POSTFIX "d") # 设置debug的后缀
set(CMAKE_BUILD_TYPE Release) # 设置构建类型为release

set(LUA_VERSION 5.4) #lua版本号，是模块的安装版本的目录，根据源码的版本手动进行更改
set(_LUA_VERSION lua5.4.6) #lua实际版本号，并且是安装目录名，根据源码的版本手动进行更改

# 配置构建平台变量
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		set(BUILD_PLATFORM "MinGW")
	elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
		set(BUILD_PLATFORM "MSVC")
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		set(BUILD_PLATFORM "LinuxGNU")
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	if(CMAKE_C_COMPILER_ID STREQUAL "AppleClang")
		set(BUILD_PLATFORM "AppleClang")
	endif()
endif()

# 配置构建平台位宽
if(CMAKE_SIZEOF_VOID_P EQUAL 8 )
	set(BUILD_PLATFORM_BITWID 64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set(BUILD_PLATFORM_BITWID 32)
else()
	set(BUILD_PLATFORM_BITWID ${CMAKE_SIZEOF_VOID_P}/8)
endif()

# 项目默认的交叉编译的目标平台为LinuxGNU
if(CROSS_COMPILE EQUAL 1)
	set(BUILD_PLATFORM "LinuxGNU")
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件和Windows的dll输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录

# 信息打印
message(STATUS ${CMAKE_C_COMPILER_ID} " " ${CMAKE_C_COMPILER_VERSION}) #打印出编译器
message(STATUS ${CMAKE_SYSTEM_NAME} " " ${CMAKE_SYSTEM_VERSION}) #打印系统名
# 查看debug和release模式的编译选项
message(STATUS "Debug flags: " ${CMAKE_C_FLAGS_DEBUG}) 
message(STATUS "Release flags: " ${CMAKE_C_FLAGS_RELEASE}) #GNU、MINGW的RELEASE默认开O3，MSVC的RELEASE默认开O2
message(STATUS ${CMAKE_BUILD_TYPE})
message(STATUS ${CMAKE_EXE_LINKER_FLAGS} )

# 全局函数定义

## 删除某个目标的符号
function(remove_symbols target )
if(BUILD_PLATFORM STREQUAL "AppleClang") 
set(PARAMS "-S")
else()
set(PARAMS "-s")
endif()
add_custom_command(TARGET ${target}
		POST_BUILD #在目标生成后再执行
		COMMAND ${CMAKE_STRIP} ${PARAMS} "$<TARGET_FILE:${target}>")
endfunction()

function(remove_symbols_unneeded target ) #删除不需要的符号
if(BUILD_PLATFORM STREQUAL "AppleClang") 
set(PARAMS "-S")
else()
set(PARAMS "--strip-unneeded")
endif()
add_custom_command(TARGET ${target}
		POST_BUILD #在目标生成后再执行
		COMMAND ${CMAKE_STRIP} ${PARAMS} "$<TARGET_FILE:${target}>")
endfunction()