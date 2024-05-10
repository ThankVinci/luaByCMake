# 编译器预定义设定
add_compile_definitions(NDEBUG LUA_COMPAT_5_3 ) # 设置lua中原本的预定义
add_definitions(-w ) # 隐藏warning

# 设置要生成的目标的名称以及变量
set(LIBLUA_STATIC liblua_static)
set(LIBLUA_SHARED liblua_shared)
set(LUAC_EXEC luac_exec)
set(LUA_EXEC lua_exec)
set(LUA_NAME lua)

# 设置包含目录
set(LUA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${LUA_INCLUDE_DIR}) #设置头文件所在目录
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LIB_SRC_FILES)
set(LUA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c) 
set(LUAC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c) 
list(REMOVE_ITEM LIB_SRC_FILES  ${LUA_PATH} ${LUAC_PATH}) #从源文件中剔除lua.c和luac.c,注意这里必须传入的是绝对路径

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/lualib.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/luaexec.cmake")

# 设置安装头文件
set(INSTALL_H_FILES ${LUA_INCLUDE_DIR}/lauxlib.h ${LUA_INCLUDE_DIR}/lua.h ${LUA_INCLUDE_DIR}/lua.hpp ${LUA_INCLUDE_DIR}/luaconf.h ${LUA_INCLUDE_DIR}/lualib.h )
install(FILES ${INSTALL_H_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/include)