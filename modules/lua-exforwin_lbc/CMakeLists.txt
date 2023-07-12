#1.设置模块名称##########################
#这里提供了懒人一键式构建，一般来说只需要修改模块名称即可，但还是要发挥一下主观能动性
#需要修改的地方可能也就目标链接和预定义之类的
set(MODULE_NAME_WITH_MODULE "exforwin_module")
set(MODULE_NAME "efw")
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
set(CMAKE_DEBUG_POSTFIX "") #DEBUG清除后缀
#注：由于lua对模块名称的限制以及一些编译器编译库文件名的会加上lib的缘故，所以要删除前缀
add_library(${MODULE_NAME_WITH_MODULE} MODULE ${MODULE_SRC_FILES}) #添加生成动态库目标
set_target_properties(${MODULE_NAME_WITH_MODULE} PROPERTIES OUTPUT_NAME ${MODULE_NAME})
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
else()
	set_target_properties(${MODULE_NAME_WITH_MODULE} PROPERTIES PREFIX "") #除了MSVC外的编译器都删除前缀
endif()
#############################################

#5.生成文件后进行安装########################
#lua模块就安装到lib/lua/${LUA_VERSION}目录下，这样编译之后的lua文件直接require模块的话找的就是一起编译出来的模块
install(TARGETS ${MODULE_NAME_WITH_MODULE} 
	#RUNTIME DESTINATION ${INSTALL_DIRECTORY}/lib/lua/${LUA_VERSION}
	LIBRARY DESTINATION lib/lua/${LUA_VERSION}
)#这里是因为Windows生成动态库会放到RUNTIME中，所以RUNTIME和LIBRARY都设为该目录
#补充：当生成的动态库为MODULE时，会放入到LIBRARY中，所以注释了RUNTIME一行


if(CMAKE_BUILD_TYPE STREQUAL "Release") #只有RELEASE模式下会安装到codelib目录下
	if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		#把动态库安装到我的codelib目录下
		if(DEFINED CODELIB_INSTALL_PATH)
			if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
				install(TARGETS ${MODULE_NAME_WITH_MODULE}
					LIBRARY DESTINATION ${CODELIB_INSTALL_PATH}/luamodules/lib/${LUA_VERSION} )
			endif()
		endif()
	endif()
endif()
#############################################

#6.目标链接设置##############################
set(MODULE_LINK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/ )

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		#set(MODULE_LINK_LIBS ${MODULE_LINK_PATH}/libxxx.dll.a ) 可能某个lua模块需要依赖某个C库
	elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
		#set(MODULE_LINK_LIBS )
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
endif()

target_link_libraries(${MODULE_NAME_WITH_MODULE} ${LIBLUA_SHARED} ${MODULE_LINK_LIBS} )
#############################################

#############################################
#		以下的内容就属于需要经常改的了		#
#		（如果增加编译平台的话）			#
#############################################

#7.编译器、预定义、链接选项、生成事件########
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_compile_definitions(LUA_LIB )
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		add_custom_command(TARGET ${MODULE_NAME_WITH_MODULE} #直接把符号都删了
			POST_BUILD #在目标生成后再执行
			COMMAND strip "-s" "$<TARGET_FILE:${MODULE_NAME_WITH_MODULE}>") 
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
endif()
#############################################