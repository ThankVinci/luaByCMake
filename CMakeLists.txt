cmake_minimum_required (VERSION 3.12)

#######弃用1#############
#if(WIN32) #如果是MSVC项目的话，请把set(ENV{PATH}..)那一行注释掉，以免干扰MSVC找编译器
#set(MINGW_HOME $ENV{MINGW_HOME}) #获取系统的环境变量MINGW_HOME，前提是系统环境变量中有这个变量，不然还是得手动添加绝对路径
#set(ENV{PATH} $ENV{MINGW_HOME}\\bin) #由于cmake过程中只需要编译链接的工具，所以就在作用域内直接把环境变量PATH设置为MinGW的bin目录
#endif()
#######弃用1结束#########

#1.设置项目名、版本信息、项目相关变量########
project ("luaByCMake")
set(LUA_VERSION 5.4) #lua版本号，根据源码的版本进行更改
set(LBC_VERSION 1.0) #luaByCMake版本号

#项目生成目标的变量
set(LIBLUA_STATIC liblua_static)
set(LIBLUA_SHARED liblua_shared)
set(LUAC_EXEC luac_exec)
set(LUA_EXEC lua_exec)
set(LUA_NAME lua)
#############################################

#2.信息打印处################################
message(STATUS ${CMAKE_C_COMPILER_ID}) #打印出编译器
message(STATUS ${CMAKE_SYSTEM_NAME}) #打印系统名
#查看debug和release模式的编译选项
message(STATUS ${CMAKE_C_FLAGS_DEBUG}) 
message(STATUS ${CMAKE_C_FLAGS_RELEASE}) #GNU、MINGW的RELEASE默认开O3，MSVC的RELEASE默认开O2
#############################################

#3.设置目标生成目录##########################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
#############################################

#4.设置安装目录的根目录######################
set(INSTALL_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/installed)
#############################################

#5.设置包含目录、库目录和参加编译源文件######
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include) #设置头文件所在目录
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LIB_SRC_FILES) #设置要生成的静态库的源文件所在目录，从而选中该目录下所有文件
#这两个文件编译成库的时候要排除
set(LUA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c) #编译可执行文件lua的时候需要
set(LUAC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c) #编译可执行文件luac的时候需要
list(REMOVE_ITEM LIB_SRC_FILES  ${LUA_PATH} ${LUAC_PATH}) #从源文件中剔除lua.c和luac.c,注意这里必须传入的是绝对路径
#############################################

#6.添加生成库目标############################
add_library(${LIBLUA_STATIC} STATIC ${LIB_SRC_FILES}) #添加生成静态库目标liblua_static
add_library(${LIBLUA_SHARED} SHARED ${LIB_SRC_FILES}) #添加生成动态库目标liblua_shared
#############################################

#7.添加生成可执行文件目标####################
add_executable(${LUAC_EXEC} ${LUAC_PATH}) #添加生成可执行文件目标luac_exec
add_executable(${LUA_EXEC} ${LUA_PATH}) #添加生成可执行文件目标lua_exec
###########################################

#8.生成可执行文件链接设置###################
target_link_libraries(${LUAC_EXEC} ${LIBLUA_STATIC} ) #luac链接静态库
target_link_libraries(${LUA_EXEC} ${LIBLUA_SHARED} ) #lua链接动态库
#############################################

#9.生成文件后进行安装的安装目录##############
install(TARGETS ${LUA_EXEC} ${LUAC_EXEC} ${LIBLUA_SHARED} ${LIBLUA_STATIC} 
	RUNTIME DESTINATION ${INSTALL_DIRECTORY}/bin/
	LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib/
	ARCHIVE DESTINATION ${INSTALL_DIRECTORY}/lib/
)
#############################################

#############################################
#		以下的内容就属于需要经常改的了		#
#		（如果增加编译平台的话）			#
#############################################

#10.生成目标属性设置###########################
set_target_properties(${LUAC_EXEC} PROPERTIES OUTPUT_NAME ${LUA_NAME}c)
set_target_properties(${LUA_EXEC} PROPERTIES OUTPUT_NAME ${LUA_NAME})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME lib${LUA_NAME}) #MSVC编译产生的库名就是原来的库名
	set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME}54)
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU") #makefile中mingw生成的库名就直接叫原本的名字，所以这里进行删除
		set_target_properties(${LIBLUA_STATIC} PROPERTIES PREFIX "") #删除前缀
		set_target_properties(${LIBLUA_SHARED} PROPERTIES PREFIX "")
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME ${LUA_NAME}) #GNU编译产生的库名前面会自己加上lib
	set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME ${LUA_NAME}) #AppleClang编译产生的库名前面会自己加上lib
	set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME})
endif()
#############################################

#11.编译器、预定义、链接选项、生成事件#######

#设置lua项目中原本设置的通用的预定义
add_compile_definitions(NDEBUG )
add_compile_definitions(LUA_COMPAT_5_3 )

#以下分平台设置预定义、链接选项、自定义事件
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_compile_definitions(LUA_BUILD_AS_DLL ) #MSVC动态库需要暴露接口，所以要添加编译器预定义暴露接口(使用lua源码自带的预定义)
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")#这个分支指MinGW
		#传递-s选项给ld，所有生成的exe都会删除符号表
		#makefile中只给lua删除了符号表，所以这个我就注释掉了改成在运行自定义命令
		#为什么只删除mingw生成的lua的符号表，不删除别的平台的符号表（Darwin我试了删除会报错，但是Linux可以删除）
		#set(CMAKE_EXE_LINKER_FLAGS "-Wl,-s") 
		#set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--strip-unneeded") #设置无效，编译报错
		add_custom_command(TARGET ${LIBLUA_SHARED} #makefile中mingw生成的共享库会删除不需要的符号
			POST_BUILD #在目标生成后再执行
			COMMAND strip "--strip-unneeded" "$<TARGET_FILE:${LIBLUA_SHARED}>") 
		add_custom_command(TARGET ${LUA_EXEC} #makefile中mingw生成的lua会删除所有符号
			POST_BUILD #在目标生成后再执行
			COMMAND strip "-s" "$<TARGET_FILE:${LUA_EXEC}>") 
	elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
		#喵喵喵
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux") #linux还有个readline的构建，但是不知道咋整
	add_compile_definitions(LUA_USE_LINUX ) #LUA_USE_READLINE	
	set(CMAKE_EXE_LINKER_FLAGS "-lm -Wl,-E -ldl" ${CMAKE_EXE_LINKER_FLAGS} ) #-lreadline 
	message(STATUS ${CMAKE_EXE_LINKER_FLAGS} )
	target_link_libraries(${LUAC_EXEC} ${CMAKE_EXE_LINKER_FLAGS} )
	target_link_libraries(${LUA_EXEC} ${CMAKE_EXE_LINKER_FLAGS} )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	add_compile_definitions(LUA_USE_MACOSX LUA_USE_READLINE )
	set(CMAKE_EXE_LINKER_FLAGS "-lreadline")
endif()
#############################################

#12.设置RunPath##############################
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU") #makefile中mingw生成的库名就直接叫原本的名字，所以这里进行删除
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set_property(
		TARGET ${LUA_EXEC}
		PROPERTY INSTALL_RPATH
		"$ORIGIN/"
		"$ORIGIN/../lib/"
	)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(CMAKE_MACOSX_RPATH ON)
	add_custom_command(
		TARGET ${LUA_EXEC} 
		POST_BUILD 
		COMMAND ${CMAKE_INSTALL_NAME_TOOL} -add_rpath "@rpath/;@rpath/../lib/" "$<TARGET_FILE:${LUA_EXEC}>" )
endif()
#############################################

#13.添加生成子项目目录#######################
add_subdirectory(runLuaScript )
add_subdirectory(libdemo )
add_subdirectory(lua-cjson-general ) 
#############################################
