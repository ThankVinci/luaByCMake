cmake_minimum_required (VERSION 3.12)
set(CROSS_COMPILE 0) # 当CROSS_COMPILE为1时表示开启交叉编译
if(CROSS_COMPILE EQUAL 1) # 配置仅在交叉编译时可用
	set(CROSS_IN_WINDOW 0) # 如果是在Windows下进行交叉编译需要把这个置为1
	set(EXEC_SUFFIX )
	if(CROSS_IN_WINDOW EQUAL 1)
		set(EXEC_SUFFIX ".exe")
	endif()
	include($ENV{TOOLCHAIN_HOME_AARCH64_LIBC}/cmake/toolchain.cmake ) #这里导入工具链目录下的toolchain.cmake文件
endif()

project ("luaByCMake")
# 预定义中会设置一些与项目无关的变量，判断构建的目标平台、设置安装路径和全局函数
include("./cmake/predefined.cmake")
# 目标平台的配置中只有一些简单的与系统相关宏定义以及链接的库
include("./cmake/mingw.cmake")
include("./cmake/msvc.cmake")
include("./cmake/gnu.cmake")
include("./cmake/darwin.cmake")
# 主模块中包含了lua源代码的内容
include("./cmake/mainmodule.cmake")

# 添加生成子项目目录
set(MODULE_PATH modules)
add_subdirectory(${MODULE_PATH}/lua_demo )
#dd_subdirectory(${MODULE_PATH}/lua_cjson ) 
#add_subdirectory(${MODULE_PATH}/lua_iconv ) 
#add_subdirectory(${MODULE_PATH}/lua_lfs )
if(CMAKE_SYSTEM_NAME STREQUAL "Windows") ##只有Windows需要这个模块
	#add_subdirectory(${MODULE_PATH}/lua_exforwin )
endif()
