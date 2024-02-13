# lua动态库和静态库的编译配置

# 添加生成目标
add_library(${LIBLUA_STATIC} STATIC ${LIB_SRC_FILES})
add_library(${LIBLUA_SHARED} SHARED ${LIB_SRC_FILES})

if(BUILD_PLATFORM STREQUAL "MinGW") #仅MinGW可用的cmake配置
set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME lib${LUA_NAME}) #MSVC编译产生的库名就是原来的库名
set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME}54)
set_target_properties(${LIBLUA_STATIC} PROPERTIES PREFIX "") #删除前缀
set_target_properties(${LIBLUA_SHARED} PROPERTIES PREFIX "")
elseif(BUILD_PLATFORM STREQUAL "MSVC") #仅MSVC可用的cmake配置
set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME lib${LUA_NAME}) #MSVC编译产生的库名就是原来的库名，所以需要手动加上lib
set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME}54)
elseif(BUILD_PLATFORM STREQUAL "LinuxGNU")
set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME ${LUA_NAME}) #GNU编译产生的库名前面会自己加上lib
set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME})
elseif(BUILD_PLATFORM STREQUAL "AppleClang") 
set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME ${LUA_NAME}) #AppleClang编译产生的库名前面会自己加上lib
set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME})
endif()

# 删除动态库的符号
if(NOT(CMAKE_STRIP STREQUAL ""))
remove_symbols_unneeded(${LIBLUA_SHARED})
endif()

# 编译后安装
install(TARGETS ${LIBLUA_SHARED} ${LIBLUA_STATIC} 
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)