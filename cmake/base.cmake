# 设置一些CMake项目变量
set(CMAKE_DEBUG_POSTFIX "d") # 设置debug的后缀
set(CMAKE_BUILD_TYPE Release) # 设置构建类型为release

# 编译器预定义设定
add_compile_definitions(NDEBUG ) # 设置lua中原本的预定义
add_compile_definitions(LUA_COMPAT_5_3 )

set(LUA_VERSION 5.4) #lua版本号，是模块的安装版本的目录，根据源码的版本手动进行更改
set(_LUA_VERSION lua5.4.6) #lua实际版本号，并且是安装目录名，根据源码的版本手动进行更改
set(LBC_VERSION 1.0.2) #luaByCMake版本号

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

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件和Windows的dll输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录

# 设置安装目录的根目录
set(INSTALL_PATH_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/installed/)

# 添加生成目标
add_library(${LIBLUA_STATIC} STATIC ${LIB_SRC_FILES})
add_library(${LIBLUA_SHARED} SHARED ${LIB_SRC_FILES})
add_executable(${LUAC_EXEC} ${LUAC_PATH})
add_executable(${LUA_EXEC} ${LUA_PATH})

target_link_libraries(${LUAC_EXEC} ${LIBLUA_STATIC} ) #luac链接静态库
target_link_libraries(${LUA_EXEC} ${LIBLUA_SHARED} ) 

set_target_properties(${LUAC_EXEC} PROPERTIES OUTPUT_NAME ${LUA_NAME}c)
set_target_properties(${LUA_EXEC} PROPERTIES OUTPUT_NAME ${LUA_NAME})

# 编译后安装
install(TARGETS ${LUA_EXEC} ${LUAC_EXEC} ${LIBLUA_SHARED} ${LIBLUA_STATIC} 
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

# 信息打印
message(STATUS ${CMAKE_C_COMPILER_ID}) #打印出编译器
message(STATUS ${CMAKE_SYSTEM_NAME}) #打印系统名
# 查看debug和release模式的编译选项
message(STATUS ${CMAKE_C_FLAGS_DEBUG}) 
message(STATUS ${CMAKE_C_FLAGS_RELEASE}) #GNU、MINGW的RELEASE默认开O3，MSVC的RELEASE默认开O2
message(STATUS ${CMAKE_BUILD_TYPE})
message(STATUS ${CMAKE_EXE_LINKER_FLAGS} )