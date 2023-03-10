#1.设置模块名称##########################
#这里提供了懒人一键式构建，一般来说只需要修改模块名称即可，但还是要发挥一下主观能动性
#需要修改的地方可能也就目标链接和预定义之类的
set(MODULE_NAME_WITH_SHARED "cjson_shared")
set(MODULE_NAME "cjson")
#############################################

#2.设置目标生成目录##########################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
#############################################

#3.设置包含目录、库目录和参加编译源文件######
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include )
#aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MODULE_SRC_FILES)
#由于这个项目比较特殊，所以把参数编译的源文件抽出来写到3.1中了
#############################################

#3.1设置参与编译的源文件#####################
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src )
##插入一个不速之客，由于这个不速之客的出现，使得这一部分定义的东西要往前挪了
###这一段是设置FPCONV_SOURCES源文件，从原版的CMakeLists文件中直接拿出来的
####修改了源文件的目录，使用的add_definitions也改成add_compile_definitions
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
#############################################

#4.添加生成库目标############################
#注：由于lua对模块名称的限制以及一些编译器编译库文件名的会加上lib的缘故，所以要删除前缀
add_library(${MODULE_NAME_WITH_SHARED} SHARED ${MODULE_SRC_FILES}) #添加生成动态库目标
set_target_properties(${MODULE_NAME_WITH_SHARED} PROPERTIES OUTPUT_NAME ${MODULE_NAME})
if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
else()
	set_target_properties(${MODULE_NAME_WITH_SHARED} PROPERTIES PREFIX "") #除了MSVC外的编译器都删除前缀
endif()
#############################################

#5.生成文件后进行安装的安装目录##############
#lua模块就安装到lib/lua/${LUA_VERSION}目录下，这样编译之后的lua文件直接require模块的话找的就是一起编译出来的模块
install(TARGETS ${MODULE_NAME_WITH_SHARED} 
	RUNTIME DESTINATION ${INSTALL_DIRECTORY}/lib/lua/${LUA_VERSION}
	LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib/lua/${LUA_VERSION}
)#这里是因为Windows生成动态库会放到RUNTIME中，所以RUNTIME和LIBRARY都设为该目录
#############################################

#6.目标链接设置##############################
target_link_libraries(${MODULE_NAME_WITH_SHARED} liblua_shared) #必须链接lua的动态库
#############################################

#############################################
#		以下的内容就属于需要经常改的了		#
#		（如果增加编译平台的话）			#
#############################################

#7.编译器、预定义、链接选项、生成事件########
#add_compile_definitions(ENABLE_CJSON_GLOBAL) #开启这个就直接让CJSON变成全局的
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_compile_definitions(LUA_LIB )
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		add_compile_options(-pedantic -fpic) #GNU支持的编译器选项
		add_custom_command(TARGET ${MODULE_NAME_WITH_SHARED} #直接把符号都删了
			POST_BUILD #在目标生成后再执行
			COMMAND strip "-s" "$<TARGET_FILE:${MODULE_NAME_WITH_SHARED}>") 
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	add_compile_options(-pedantic -fpic) #GNU支持的编译器选项
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
endif()
#############################################