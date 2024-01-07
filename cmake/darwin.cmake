if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND CMAKE_C_COMPILER_ID STREQUAL "AppleClang") 
add_compile_definitions(LUA_USE_MACOSX LUA_USE_READLINE )

set(_COMPILER AppleClang) #设置编译器名称，供common.cmake使用

set(CMAKE_EXE_LINKER_FLAGS "-lreadline")
target_link_libraries(${LUAC_EXEC} ${CMAKE_EXE_LINKER_FLAGS} )
target_link_libraries(${LUA_EXEC} ${CMAKE_EXE_LINKER_FLAGS} )

set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME ${LUA_NAME}) #AppleClang编译产生的库名前面会自己加上lib
set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME})

#设置RunPath
#set(CMAKE_MACOSX_RPATH ON) #测试之后发现这个不开也是可以的
set_property(
	TARGET ${LUA_EXEC}
	PROPERTY INSTALL_RPATH
	"@executable_path/"
	"@executable_path/../lib/"
)

endif()