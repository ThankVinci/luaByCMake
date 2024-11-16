cmake_minimum_required (VERSION 3.12)
set(CROSS_COMPILE 0) # 当CROSS_COMPILE为1时表示开启交叉编译
if(CROSS_COMPILE EQUAL 1) # 配置仅在交叉编译时可用
	set(CROSS_IN_WINDOW 0) # 如果是在Windows下进行交叉编译需要把这个置为1
	set(EXEC_SUFFIX )
	if(CROSS_IN_WINDOW EQUAL 1)
		set(EXEC_SUFFIX ".exe")
	endif()
	include($ENV{TOOLCHAIN_HOME_AARCH64_LIBC}/cmake/toolchain.cmake ) #这里导入工具链目录下的toolchain.cmake文件
	#$ENV{TOOLCHAIN_HOME_AARCH64_LIBC}是指交叉编译链工具的根目录
	#本项目提供了参考模板，在"本项目根目录/toolchains/toolchain.cmake"，是aarch64-none-linux-gnu的配置，可自行修改成别的编译链的配置，将其复制到"$ENV{TOOLCHAIN_HOME_AARCH64_LIBC}/cmake"中使用
endif()

set(GENERATE_VS_SLN 0) # 若通过CMAKE生成VS解决方案的话，请将此变量设为1，并在msvc.cmake设置构建类型，因为生成sln之后无法预知BUILD_TYPE，因此Release和Debug的安装目录就会混淆

project ("luaByCMake" C)
# 预定义中会设置一些与项目无关的变量，判断构建的目标平台、设置安装路径和全局函数
set(BUILD_INFO "")
include("./cmake/predefined.cmake")
# 目标平台的配置中只有一些简单的与系统相关宏定义以及链接的库
include("./cmake/mingw.cmake")
include("./cmake/msvc.cmake")
include("./cmake/gnu.cmake")
include("./cmake/darwin.cmake")
# 设置安装目录
set(INSTALL_PATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/installed)
set(INSTALL_PATH ${INSTALL_PATH_ROOT}/${BUILD_PLATFORM})
set(CMAKE_INSTALL_PREFIX ${INSTALL_PATH}_${CMAKE_BUILD_TYPE}/${_LUA_VERSION})
# 主模块中包含了lua源代码的内容
include("./cmake/mainmodule.cmake")

add_compile_definitions(BUILD_INFO="${BUILD_INFO}")

# 添加生成子项目目录
set(MODULE_PATH modules)
add_subdirectory(${MODULE_PATH}/lua_demo )
add_subdirectory(${MODULE_PATH}/lua_cjson ) 
#add_subdirectory(${MODULE_PATH}/lua_iconv ) 
add_subdirectory(${MODULE_PATH}/lua_lfs )
if(CMAKE_SYSTEM_NAME STREQUAL "Windows") ##只有Windows需要这个模块
	add_subdirectory(${MODULE_PATH}/lua_efw )
endif()
