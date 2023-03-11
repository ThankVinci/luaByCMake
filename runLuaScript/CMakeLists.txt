#1.调试模块使用变量、模块名##########################
set(RLUA_EXEC_MAIN ON) #调试用

set(RLUA_SHARED "rua_shared")
set(RLUA_EXEC "rua_exec")
set(RLUA "rua")

if(RLUA_EXEC_MAIN)
	add_compile_definitions(RLUA_EXEC_MAIN )
	set(RLUA_TARGET ${RLUA_EXEC})
else()
	set(RLUA_TARGET ${RLUA_SHARED})
endif()
############################################

#2.设置目标生成目录##########################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
#############################################

#3.设置包含目录、库目录和参加编译源文件######
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src RUNLUA_SRC_FILE)
#############################################

#4.添加生成目标和链接库######################
if(RLUA_EXEC_MAIN)
	add_executable(${RLUA_EXEC} ${RUNLUA_SRC_FILE}) #添加生成可执行文件目标
else()
	add_library(${RLUA_SHARED} SHARED ${RUNLUA_SRC_FILE}) #添加生成动态库目标
endif()
set_target_properties(${RLUA_TARGET} PROPERTIES OUTPUT_NAME ${RLUA})
target_link_libraries(${RLUA_TARGET} ${LIBLUA_SHARED}) #必须链接lua的动态库

#############################################
message(STATUS ${LIBLUA_SHARED})

#5.生成文件后进行安装########################
install(TARGETS ${RLUA_TARGET} 
	RUNTIME DESTINATION ${INSTALL_DIRECTORY}/bin
	LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib
	ARCHIVE DESTINATION ${INSTALL_DIRECTORY}/lib
)
#############################################

#############################################
#		以下的内容就属于需要经常改的了		#
#		（如果增加编译平台的话）			#
#############################################

#6.编译器、预定义、链接选项、生成事件########
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_compile_definitions(LUA_LIB )
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		#add_custom_command(TARGET ${MODULE_NAME_WITH_MODULE} #直接把符号都删了
			#POST_BUILD #在目标生成后再执行
			#COMMAND strip "-s" "$<TARGET_FILE:${MODULE_NAME_WITH_MODULE}>") 
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	if(RLUA_EXEC_MAIN)
		target_link_libraries(${RLUA_EXEC} ${CMAKE_EXE_LINKER_FLAGS} )
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
endif()
#############################################