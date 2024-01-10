# 本文件为通用cmake文件，一般只需要改项目独特的cmake文件

# 设置包含目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include )

# 设置目标生成目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录

# 由于lua对模块的要求是库名必须和入口函数名一样，需要清除debug后缀和lib前缀
set(CMAKE_DEBUG_POSTFIX "")
# 添加生成动态库目标
add_library(${MODULE_NAME_WITH_MODULE} MODULE ${MODULE_SRC_FILES})
set_target_properties(${MODULE_NAME_WITH_MODULE} PROPERTIES OUTPUT_NAME ${MODULE_NAME})
if(NOT(BUILD_PLATFORM STREQUAL "MSVC"))
#清除模块前缀
set_target_properties(${MODULE_NAME_WITH_MODULE} PROPERTIES PREFIX "") 
endif()

# 链接库
target_link_libraries(${MODULE_NAME_WITH_MODULE} ${LIBLUA_SHARED} ${MODULE_LINK_LIBS} )

# 构建后删除模块中所有符号
add_custom_command(TARGET ${MODULE_NAME_WITH_MODULE} #直接把符号都删了
			POST_BUILD #在目标生成后再执行
			COMMAND ${CMAKE_STRIP} "-s" "$<TARGET_FILE:${MODULE_NAME_WITH_MODULE}>")

# lua模块就安装到lib/lua/${LUA_VERSION}目录下，这样编译之后的lua文件直接require模块的话找的就是一起编译出来的模块
install(TARGETS ${MODULE_NAME_WITH_MODULE}
	LIBRARY DESTINATION lib/lua/${LUA_VERSION}
)