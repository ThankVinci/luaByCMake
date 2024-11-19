if(BUILD_PLATFORM STREQUAL "MinGW") #仅MinGW可用的cmake配置
add_compile_definitions(LUA_BUILD_AS_DLL )
add_compile_definitions(USE_MSVCRT_=0 )

set(BUILD_INFO ", Built by MinGW ${CMAKE_C_COMPILER_VERSION}, ${BUILD_PLATFORM_BITWID} bit")
endif()