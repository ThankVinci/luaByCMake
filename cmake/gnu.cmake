if(BUILD_PLATFORM STREQUAL "LinuxGNU") #仅LinuxGNU可用的cmake配置
#linux还有个readline的构建，但是不知道咋整
add_compile_definitions(LUA_USE_LINUX ) #LUA_USE_READLINE

set(CMAKE_EXE_LINKER_FLAGS "-lm -Wl,-E -ldl" ${CMAKE_EXE_LINKER_FLAGS} ) #-lreadline 

set(BUILD_INFO ", Built by GNU ${CMAKE_C_COMPILER_VERSION}, ${BUILD_PLATFORM_BITWID} bit")
endif()