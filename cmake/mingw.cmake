if(BUILD_PLATFORM STREQUAL "MinGW") #仅MinGW可用的cmake配置
add_compile_definitions(LUA_BUILD_AS_DLL )

set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME lib${LUA_NAME}) #MSVC编译产生的库名就是原来的库名
set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME}54)
set_target_properties(${LIBLUA_STATIC} PROPERTIES PREFIX "") #删除前缀
set_target_properties(${LIBLUA_SHARED} PROPERTIES PREFIX "")

endif()