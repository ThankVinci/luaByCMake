#1.运行库使用变量、库名#################
set(LUARNR_SHARED "luarnr_shared")
set(LUARNR_STATIC "luarnr_static")
set(DEMO_EXEC "demo_exec")
set(LUARNR "luarnr")
set(DEMO "demo")
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(LUARNR_TARGET ${LUARNR_SHARED} )
else()
	set(LUARNR_TARGET ${LUARNR_STATIC} )
endif()
#############################################

#2.设置目标生成目录##########################
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
#############################################

#3.设置包含目录、库目录和参加编译源文件######
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
					${CMAKE_CURRENT_SOURCE_DIR}/include/io   
					${CMAKE_CURRENT_SOURCE_DIR}/include/luarnr ) #设置头文件所在目录

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/io IO_SRC )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/luarnr LUARNR_CLASS_SRC )
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src DEMO_SRC )
#############################################

#4.添加生成目标##############################
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_library(${LUARNR_TARGET} SHARED ${LUARNR_CLASS_SRC})
else()
	add_library(${LUARNR_TARGET} STATIC ${LUARNR_CLASS_SRC})
endif()
set_target_properties(${LUARNR_TARGET} PROPERTIES OUTPUT_NAME ${LUARNR})

add_executable(${DEMO_EXEC} ${DEMO_SRC} ${IO_SRC} )
set_target_properties(${DEMO_EXEC} PROPERTIES OUTPUT_NAME ${DEMO})
#############################################

#5.链接库####################################
target_link_libraries(${LUARNR_TARGET} ${RLUA_STATIC}) #链接rlua的静态库
target_link_libraries(${DEMO_EXEC} ${LUARNR_TARGET}) #链接LUARNR_SHARED
#############################################

#6.编译器、预定义、链接选项、生成事件########
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	add_compile_definitions(LUARNR_API )
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		add_custom_command(TARGET ${LUARNR_TARGET} #直接把符号都删了
			POST_BUILD #在目标生成后再执行
			COMMAND strip "-s" "$<TARGET_FILE:${LUARNR_TARGET}>") 
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	target_link_libraries(${LUARNR_TARGET} ${CMAKE_EXE_LINKER_FLAGS} )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
endif()
#############################################

#7.生成文件后进行安装########################
install(TARGETS ${DEMO_EXEC} ${LUARNR_TARGET}
	RUNTIME DESTINATION ${INSTALL_DIRECTORY}/bin
	LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib
	ARCHIVE DESTINATION ${INSTALL_DIRECTORY}/lib
)
#############################################

#8.安装脚本和json############################
install(
	FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/scripts/main.lua 
	${CMAKE_CURRENT_SOURCE_DIR}/tests/entry.json 
	DESTINATION ${INSTALL_DIRECTORY}/bin )
#############################################
