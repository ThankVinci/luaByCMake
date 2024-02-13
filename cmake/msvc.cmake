if(BUILD_PLATFORM STREQUAL "MSVC") #仅MSVC可用的cmake配置
set(CMAKE_BUILD_TYPE ) #MSVC取消build_type,交由VS的编译配置自己去管理

add_compile_options(/utf-8)
add_compile_definitions(LUA_BUILD_AS_DLL ) 

set(CMAKE_EXE_LINKER_FLAGS "" )

endif()