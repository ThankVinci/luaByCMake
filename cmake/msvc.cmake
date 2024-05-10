if(BUILD_PLATFORM STREQUAL "MSVC") #仅MSVC可用的cmake配置
set(CMAKE_BUILD_TYPE ) #MSVC取消build_type,交由VS的编译配置自己去管理()

add_compile_options(/utf-8)
add_compile_definitions(LUA_BUILD_AS_DLL ) 

set(CMAKE_EXE_LINKER_FLAGS "" )

set(BUILD_INFO ", Built by Visual Studio ${CMAKE_C_COMPILER_VERSION}, ${BUILD_PLATFORM_BITWID} bit")

if(GENERATE_VS_SLN EQUAL 1) # 配置仅在生成VS解决方案时可用,若在VS19以上直接打开cmake项目则不需要这一步
	set(CMAKE_BUILD_TYPE Debug) # 指定构建类型，如果要切换构建类型，请在此处修改并重新生成sln解决方案，再在sln项目中切换，不是很方便
	message( STATUS ${VS})
endif()

endif()