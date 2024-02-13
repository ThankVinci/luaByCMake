if(BUILD_PLATFORM STREQUAL "MinGW") #仅MinGW可用的cmake配置
add_compile_definitions(LUA_BUILD_AS_DLL )

endif()