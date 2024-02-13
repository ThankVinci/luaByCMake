# lua和luac的编译配置

# 添加生成目标
add_executable(${LUAC_EXEC} ${LUAC_PATH})
add_executable(${LUA_EXEC} ${LUA_PATH})

target_link_libraries(${LUAC_EXEC} ${LIBLUA_STATIC} ${CMAKE_EXE_LINKER_FLAGS} )
target_link_libraries(${LUA_EXEC} ${LIBLUA_SHARED} ${CMAKE_EXE_LINKER_FLAGS} ) 

set_target_properties(${LUAC_EXEC} PROPERTIES OUTPUT_NAME ${LUA_NAME}c)
set_target_properties(${LUA_EXEC} PROPERTIES OUTPUT_NAME ${LUA_NAME})

# 删除exec的符号
if(NOT(CMAKE_STRIP STREQUAL ""))
remove_symbols(${LUA_EXEC})
remove_symbols(${LUAC_EXEC})
endif()

# 编译后安装
install(TARGETS ${LUA_EXEC} ${LUAC_EXEC} 
	RUNTIME DESTINATION bin
)