#########设置编译器选项和预定义选项##############
#设置通用的编译器选项、预定义

#以下分编译器的编译器选项和预定义
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
add_compile_options(-pedantic -fpic) #GNU支持的编译器选项
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
##喵喵喵
else()
##其他情况
endif()
#以下分平台的编译器选项和预定义
if(WIN32) 
#MSVC动态库需要暴露接口，所以要添加编译器预定义暴露接口(使用lua源码自带的预定义)
add_compile_definitions(LUA_LIB)
else()
if(APPLE)
#喵喵喵
else()
#喵喵喵
endif()
endif()

#add_compile_definitions(ENABLE_CJSON_GLOBAL) #开启这个就直接让CJSON变成全局的
#########设置编译器选项和预定义选项##############

#########设置目标生成目录##############
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
#########设置目标生成目录##############


#######设置头文件包含目录和编译源文件################
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include )
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
include_directories(${INCLUDE_DIR} )

###这一段是设置FPCONV_SOURCES源文件，从原版的CMakeLists文件中直接拿出来的，只修改了源文件的目录
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
###
set(CJSONLIB_SRC_FILES ${SRC_DIR}/lua_cjson.c ${SRC_DIR}/strbuf.c ${FPCONV_SOURCES})
#######设置头文件包含目录和编译源文件################


###########添加生成库目标################
#注：由于lua库名称的限制以及GNU编译器编译库文件名的特性，所以要删除前缀
add_library(cjson_shared SHARED ${CJSONLIB_SRC_FILES}) #添加生成动态库目标
set_target_properties(cjson_shared PROPERTIES OUTPUT_NAME cjson)
#分平台修改
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
set_target_properties(cjson_shared PROPERTIES PREFIX "") #删除前缀
elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
#喵喵喵
else()
#其他情况
endif()

target_link_libraries(cjson_shared liblua_shared) #必须链接lua的动态库
###########添加生成库目标################