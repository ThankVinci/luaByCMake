if(BUILD_PLATFORM STREQUAL "MSVC") #仅MSVC可用的cmake配置
add_compile_options(/utf-8)
add_compile_definitions(LUA_BUILD_AS_DLL ) 

set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME lib${LUA_NAME}) #MSVC编译产生的库名就是原来的库名，所以需要手动加上lib
set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME}54)

endif()