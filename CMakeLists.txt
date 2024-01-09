cmake_minimum_required (VERSION 3.12)
set(CROSS_COMPILE 0) # 当CROSS_COMPILE为1时表示开启交叉编译
if(CROSS_COMPILE EQUAL 1) # 配置仅在交叉编译时可用
	set(CROSS_IN_WINDOW 1) # linux下进行编译需要把
	set(EXEC_SUFFIX )
	if(CROSS_IN_WINDOW EQUAL 1)
		set(EXEC_SUFFIX ".exe")
	endif()
	include($ENV{TOOLCHAIN_HOME_AARCH64_LIBC}/cmake/toolchain.cmake ) #这里导入工具链目录下的toolchain.cmake文件
endif()

project ("luaByCMake")
# 请注意，base和common之间夹的是不同编译器的配置，
# 这里打算设计成使用不同系统不同编译链工具就能使得唯一一种配置生效
# base中会定义一些变量给不同的配置使用，而common会拿出配置中通用的变量出来用
include("./cmake/base.cmake")
include("./cmake/mingw.cmake")
include("./cmake/msvc.cmake")
include("./cmake/gnu.cmake")
include("./cmake/darwin.cmake")
include("./cmake/common.cmake")

# 添加生成子项目目录
set(MODULE_PATH modules)
add_subdirectory(${MODULE_PATH}/lua_demo )
add_subdirectory(${MODULE_PATH}/lua_cjson ) 
#add_subdirectory(${MODULE_PATH}/lua_iconv ) 
add_subdirectory(${MODULE_PATH}/lua_lfs ) 
if(CMAKE_SYSTEM_NAME STREQUAL "Windows") ##只有Windows需要这个模块
	add_subdirectory(${MODULE_PATH}/lua_exforwin )
endif()
