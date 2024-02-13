if(BUILD_PLATFORM STREQUAL "AppleClang") 
add_compile_definitions(LUA_USE_MACOSX LUA_USE_READLINE )

set(CMAKE_EXE_LINKER_FLAGS "-lreadline" ${CMAKE_EXE_LINKER_FLAGS})

#设置RunPath
#set(CMAKE_MACOSX_RPATH ON) #测试之后发现这个不开也是可以的
set_property(
	TARGET ${LUA_EXEC}
	PROPERTY INSTALL_RPATH
	"@executable_path/"
	"@executable_path/../lib/"
)

endif()