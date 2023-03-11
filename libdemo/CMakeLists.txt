#1.设置模块名称##########################
#这里提供了懒人一键式构建，一般来说只需要修改模块名称即可，但还是要发挥一下主观能动性
#需要修改的地方可能也就目标链接和预定义之类的
set(MODULE_NAME_WITH_SHARED "demolib_shared")
set(MODULE_NAME "demo")
#############################################

#2.设置目标生成目录##########################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
#############################################

#3.设置包含目录、库目录和参加编译源文件######
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MODULE_SRC_FILES)
#############################################

#4.添加生成库目标############################
#注：由于lua对模块名称的限制以及一些编译器编译库文件名的会加上lib的缘故，所以要删除前缀
add_library(${MODULE_NAME_WITH_SHARED} MODULE ${MODULE_SRC_FILES}) #添加生成动态库目标
set_target_properties(${MODULE_NAME_WITH_SHARED} PROPERTIES OUTPUT_NAME ${MODULE_NAME})
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
else()
	set_target_properties(${MODULE_NAME_WITH_SHARED} PROPERTIES PREFIX "") #除了MSVC外的编译器都删除前缀
endif()
#############################################

#5.生成文件后进行安装的安装目录##############
#lua模块就安装到lib/lua/${LUA_VERSION}目录下，这样编译之后的lua文件直接require模块的话找的就是一起编译出来的模块
install(TARGETS ${MODULE_NAME_WITH_SHARED} 
	#RUNTIME DESTINATION ${INSTALL_DIRECTORY}/lib/lua/${LUA_VERSION}
	LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib/lua/${LUA_VERSION}
)#这里是因为Windows生成动态库会放到RUNTIME中，所以RUNTIME和LIBRARY都设为该目录
#############################################

#6.目标链接设置##############################
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(${MODULE_NAME_WITH_SHARED} liblua_shared)
else()
	target_link_libraries(${MODULE_NAME_WITH_SHARED} liblua_static)
endif()
#############################################

#############################################
#		以下的内容就属于需要经常改的了		#
#		（如果增加编译平台的话）			#
#############################################

#7.编译器、预定义、链接选项、生成事件########
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_compile_definitions(LUA_LIB )
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		add_custom_command(TARGET ${MODULE_NAME_WITH_SHARED} #直接把符号都删了
			POST_BUILD #在目标生成后再执行
			COMMAND strip "-s" "$<TARGET_FILE:${MODULE_NAME_WITH_SHARED}>") 
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
endif()
#############################################