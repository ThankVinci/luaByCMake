if(BUILD_PLATFORM STREQUAL "MSVC") #仅MSVC可用的cmake配置
add_compile_options(/utf-8)
add_compile_definitions(LUA_BUILD_AS_DLL ) 

endif()