if(BUILD_PLATFORM STREQUAL "AppleClang") 
add_compile_definitions(LUA_USE_MACOSX LUA_USE_READLINE )

set(CMAKE_EXE_LINKER_FLAGS "-lreadline" ${CMAKE_EXE_LINKER_FLAGS})

endif()