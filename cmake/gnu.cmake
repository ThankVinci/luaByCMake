if(CMAKE_SYSTEM_NAME STREQUAL "Linux" AND CMAKE_C_COMPILER_ID STREQUAL "GNU") #仅LinuxGNU可用的cmake配置
#linux还有个readline的构建，但是不知道咋整
add_compile_definitions(LUA_USE_LINUX ) #LUA_USE_READLINE

set(_COMPILER LinuxGNU) #设置编译器名称，供common.cmake使用

set(CMAKE_EXE_LINKER_FLAGS "-lm -Wl,-E -ldl" ${CMAKE_EXE_LINKER_FLAGS} ) #-lreadline 
target_link_libraries(${LUAC_EXEC} ${CMAKE_EXE_LINKER_FLAGS} )
target_link_libraries(${LUA_EXEC} ${CMAKE_EXE_LINKER_FLAGS} )

set_target_properties(${LIBLUA_STATIC} PROPERTIES OUTPUT_NAME ${LUA_NAME}) #GNU编译产生的库名前面会自己加上lib
set_target_properties(${LIBLUA_SHARED} PROPERTIES OUTPUT_NAME ${LUA_NAME})

#设置RunPath
set_property(
	TARGET ${LUA_EXEC}
	PROPERTY INSTALL_RPATH
	"$ORIGIN/"
	"$ORIGIN/../lib/"
)

endif()