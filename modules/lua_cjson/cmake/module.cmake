# 包含本项目中的独特配置
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
if(NOT USE_INTERNAL_FPCONV)
    # Use libc number conversion routines (strtod(), sprintf())
    set(FPCONV_SOURCES ${SRC_DIR}/fpconv.c)
else()
    # Use internal number conversion routines
    add_compile_definitions(USE_INTERNAL_FPCONV )
    set(FPCONV_SOURCES ${SRC_DIR}/g_fmt.c ${SRC_DIR}/dtoa.c)

    include(TestBigEndian)
    TEST_BIG_ENDIAN(IEEE_BIG_ENDIAN)
    if(IEEE_BIG_ENDIAN)
        add_compile_definitions(IEEE_BIG_ENDIAN)
    endif()

    if(MULTIPLE_THREADS)
        set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
        find_package(Threads REQUIRED)
        if(NOT CMAKE_USE_PTHREADS_INIT)
            message(FATAL_ERROR
                    "Pthreads not found - required by MULTIPLE_THREADS option")
        endif()
        add_compile_definitions(MULTIPLE_THREADS)
    endif()
endif()

set(MODULE_SRC_FILES ${SRC_DIR}/lua_cjson.c ${SRC_DIR}/strbuf.c ${FPCONV_SOURCES})

# 开启这个就直接让CJSON变成全局的
#add_compile_definitions(ENABLE_CJSON_GLOBAL) 

if(BUILD_PLATFORM STREQUAL "MinGW")
add_compile_definitions(LUA_LIB )
add_compile_options(-pedantic -fpic) #GNU支持的编译器选项

# set(MODULE_LINK_LIBS ${MODULE_LINK_PATH}/libxxx.dll.a )
elseif(BUILD_PLATFORM STREQUAL "MSVC")
add_compile_definitions(LUA_LIB )

elseif(BUILD_PLATFORM STREQUAL "LinuxGNU")
add_compile_options(-pedantic -fpic) #GNU支持的编译器选项

endif()