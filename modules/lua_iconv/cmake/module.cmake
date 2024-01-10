# 包含本项目中的独特配置
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/iconv )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MODULE_SRC_FILES)

if(BUILD_PLATFORM STREQUAL "MinGW")
add_compile_definitions(LUA_LIB )
add_compile_options(-pedantic -fpic) #GNU支持的编译器选项

set(MODULE_LINK_LIBS ${MODULE_LINK_PATH}/libiconv.dll.a )

elseif(BUILD_PLATFORM STREQUAL "MSVC")
add_compile_definitions(LUA_LIB )

elseif(BUILD_PLATFORM STREQUAL "LinuxGNU")
add_compile_options(-pedantic -fpic) #GNU支持的编译器选项

endif()