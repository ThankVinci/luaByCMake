# luaByCMake
基于lua最新5.4.4版本创建的luaCMake项目，可直接编译出lua、luac、动态库与静态库。

要自己尝试构建过程可以参考lua源码中的Makefile文件，其中除了lua.c和luac.c外的源文件用于产生动态库和静态库，lua.c链接静态库产生lua二进制文件，luac.c链接静态库产生luac二进制文件。

新增我自己的json配置，配置了64位的msvc和clang的debug和release版本。

后续考虑添加arm64的编译配置。

注：本项目仅在VS2019、MinGW、GNU上测试通过

## 编译注意事项

为了防止污染源代码，项目中应创建build目录（当然是随意的），其中有三个目录，GNU、MinGW、MSVC

1. 在Linux下，进入build/GNU，输入：

   ```shell
   cmake ../../
   ```

   在GNU目录下创建出makefile项目

2. 在Windows下，进入build/MinGW，输入：

   ```cmd
   cmake -G "MinGW Makefiles" ../../
   ```

   在MinGW目录下创建出makefile项目

3. 在Windows下，进入build/MSVC，输入：

   ```cmd
   cmake ../../
   ```

   在MSVC目录下创建出VS项目

4. 其他平台尚未测试





[TOC]

# 折腾之路

## 2023.2.27更新

1. 本次更新内容是在生成目标文件之后安装到指定目录，主要是每次编译之后要把lua模块复制到我安装lua的路径中很麻烦，并且之前也闹了点乌龙，拿MinGW编译出来的lua去调用MSVC编译出来的库，所以就按照lua规定的那样，模块可以放到安装目录下的lib/lua/${LUA_VERSION}，于是就虚构出一个安装目录来代指实际的安装目录，到时候只要把相关文件复制到实际安装目录中就OK了。

   在luaByCMake这个项目的根目录下的CMakeLists文件在生成目标之后添加如下代码进行安装：
   
   ```cmake
   #########设置安装目录##################
   set(INSTALL_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/installed)
   #########设置安装目录##################
   ###########生成文件后进行安装##############
   install(TARGETS lua luac liblua_shared 
   		RUNTIME DESTINATION ${INSTALL_DIRECTORY}/bin/
   		LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib/
   	)
   ###########生成文件后进行安装##############
   ```
   
   为方便起见也在本CMakeLists文件下，新增LUA_VERSION变量，方便子模块安装时使用
   
   ```cmake
   set(LUA_VERSION 5.4) #版本号，根据源码的版本进行更改
   ```

   在子模块中，生成库之后添加如下代码进行安装：
   
   ```cmake
   ###########生成文件后进行安装##############
	#lua模块就安装到lib/lua/${LUA_VERSION}目录下，这样编译之后的lua文件直接require模块的话找的就是一起编译出来的模块
	install(TARGETS demolib_shared 
			RUNTIME DESTINATION ${INSTALL_DIRECTORY}/lib/lua/${LUA_VERSION}
			LIBRARY DESTINATION ${INSTALL_DIRECTORY}/lib/lua/${LUA_VERSION}
   	)
   	#这里是因为Windows生成动态库会放到RUNTIME中，所以RUNTIME和LIBRARY都设为该目录
   ###########生成文件后进行安装##############
   ```
   
   **MSVC不走make install，所以上述修改在VS2019中无论是直接打开cmake项目还是使用cmake生成VS项目中都不会起作用**
   
   
   
1. 之前把lua源码改成cmake项目时忘了把lua源码中的README文件搬过来，这里面指示了lua的相关协议出处

1. 从lua官网中把license复制过来加入到项目中

   

## 2023.2.24更新

1. 2.8的cmake代码已经弃用，但是我不会删除，可以回过头来看一下当初踩的坑；

2. 本次更新主要是进行了一个规范化的处理，提高一些可读性吧，使用cmake的新api去添加不同的编译器、平台的编译器选项或者预处理，所以cmake版本至少得是3.12；

3. 然后又去看了lua源码的makefile文件、cmake的一些api，然后又对比了makefile中mingw、linux的区别。

4. 本次更新搭建了一个cmake项目的框架，修改后的CMakeLists文件如下：

   ```cmake
   # CMakeList.txt: luaByCMake 的 CMake 项目，在此处包括源代码并定义
   # 项目特定的逻辑。
   #
   cmake_minimum_required (VERSION 3.10)
   
   #######弃用1#############
   if(WIN32) #如果是MSVC项目的话，请把set(ENV{PATH}..)那一行注释掉，以免干扰MSVC找编译器
   set(MINGW_HOME $ENV{MINGW_HOME}) #获取系统的环境变量MINGW_HOME，前提是系统环境变量中有这个变量，不然还是得手动添加绝对路径
   #set(ENV{PATH} $ENV{MINGW_HOME}\\bin) #由于cmake过程中只需要编译链接的工具，所以就在作用域内直接把环境变量PATH设置为MinGW的bin目录
   endif()
   #######弃用1结束#########
   
   #设置项目名
   project ("luaByCMake")
   
   message(STATUS ${CMAKE_C_COMPILER_ID}) #为了方便起见，一进来就打印出编译器
   
   
   #########设置编译器选项和预定义选项##############
   #设置通用的编译器选项、预定义
   #add_compile_options(-O2 ) #-Wall开启全部警告，这里不手动开O2，否则VS的debug的/RTC选项会冲突
   add_compile_definitions(NDEBUG )
   add_compile_definitions(LUA_COMPAT_5_3 )
   
   #查看debug和release模式的选项
   message(STATUS ${CMAKE_C_FLAGS_DEBUG}) 
   message(STATUS ${CMAKE_C_FLAGS_RELEASE}) #GNU、MINGW的RELEASE默认开O3，MSVC的RELEASE默认开O2
   
   #以下分编译器的编译器选项和预定义
   if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
   if(WIN32)
   add_compile_options(-s )
   endif()
   elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
   ##喵喵喵
   else()
   ##其他情况
   endif()
   #以下分平台的编译器选项和预定义
   if(WIN32) 
   #MSVC动态库需要暴露接口，所以要添加编译器预定义暴露接口(使用lua源码自带的预定义)
   add_compile_definitions(LUA_BUILD_AS_DLL)
   else()
   add_compile_definitions(LUA_USE_LINUX )
   set(CMAKE_EXE_LINKER_FLAGS "-lm -Wl,-E -ldl") #如果是linux的话，编译链接时要链接一些库
   endif()
   #########设置编译器选项和预定义选项##############
   
   #########设置目标生成目录##############
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin) #设置可执行文件输出目录（Windows下dll会产生在这个目录下）
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置动态库输出目录
   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib) #设置静态库输出目录
   #########设置目标生成目录##############
   
   #######设置头文件包含目录和编译源文件################
   include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include) #设置头文件所在目录
   aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src LIB_SRC_FILES) #设置要生成的静态库的源文件所在目录，从而选中该目录下所有文件
   #这两个文件编译静态库的时候要排除
   set(LUA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c) #编译可执行文件lua的时候需要
   set(LUAC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c) #编译可执行文件luac的时候需要
   
   list(REMOVE_ITEM LIB_SRC_FILES  ${LUA_PATH} ${LUAC_PATH}) #从源文件中剔除lua.c和luac.c,注意这里必须传入的是绝对路径，否则无法删除
   #######设置头文件包含目录和编译源文件################
   
   ###########添加生成库目标################
   add_library(liblua_static STATIC ${LIB_SRC_FILES}) #添加生成静态库目标liblua_static
   add_library(liblua_shared SHARED ${LIB_SRC_FILES}) #添加生成动态库目标liblua_shared
   
   if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
   set_target_properties(liblua_static PROPERTIES OUTPUT_NAME lua) #GNU编译产生的库名前面会自己加上lib
   set_target_properties(liblua_shared PROPERTIES OUTPUT_NAME lua)
   elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
   set_target_properties(liblua_static PROPERTIES OUTPUT_NAME liblua) #MSVC编译产生的库名就是原来的库名
   set_target_properties(liblua_shared PROPERTIES OUTPUT_NAME lua54)
   else()
   #其他情况下没试过，手头没有MacOS设备，半夜三更的也懒得装个黑苹果试试
   endif()
   ###########添加生成库目标################
   
   ###########添加生成可执行文件目标################
   add_executable(luac ${LUAC_PATH}) #添加生成可执行文件目标luac
   add_executable(lua ${LUA_PATH}) #添加生成可执行文件目标lua
   
   
   if(CMAKE_HOST_UNIX)
   target_link_libraries(lua liblua_static ${CMAKE_EXE_LINKER_FLAGS})
   target_link_libraries(luac liblua_static ${CMAKE_EXE_LINKER_FLAGS}) #链接可执行文件和静态库和系统库
   elseif(CMAKE_HOST_WIN32)
   #最上面的代码中WIN32加上了LUA_BUILD_AS_DLL编译参数，使得VS项目中具有函数导出接口，所以可以直接进行连接动态库
   target_link_libraries(lua liblua_shared )
   target_link_libraries(luac liblua_static ) #链接可执行文件和静态库和系统库
   else()
   #其他情况下没试过
   endif()
   ###########添加生成可执行文件目标################
   
   
   
   ###########添加生成子项目目录################
   add_subdirectory(libdemo ) #添加子项目目录
   ###########添加生成子项目目录################
   ```



## 2023.2.8更新

发现MinGW编译lua是链接了动态库，但是使用MSVC编译发现：

1. ~~lua源代码中没有提供dll的导出接口，所以MSVC编译的话只能链接静态库~~，所以就直接搞个按编译器类型生成目标名称以及链接库，GNU编译器如果是类UNIX平台就链接静态库，如果是WIN32平台就链接动态库，MSVC编译器就链接静态库；
2. MSVC编译时，即使使用set_target_properties也不支持静态库和动态库同名，所以MSVC编译时，静态库就叫liblua，而动态库就叫lua54；
3. 其实第一点不对，lua源代码中是提供了dll导出接口了，这也是我很纳闷的问题，为什么MinGW编译能导出一个引导库，VS这么强大不行？怪我连luaconf文件都没打开过看一下，我是在配置链接参数的时候发现的，lua编译链接的时候提供了一个宏，当这个宏是MinGW的意思的时候，就会开启一个dll导出接口，所以MSVC也是可以用这个导出接口的。

在Linux（Ubuntu18）上测试编译项目结果失败，主要是lua和luac失败了，里面报的是找不到一些系统链接库的问题，本来以为是编译环境是坏的，于是试了一下用Linux编译了lua的make项目，结果通过，说明不是环境坏了，而是我编译用的参数不对，于是仔细看了一下makefile文件，发现我确实没有仔细看makefile文件，当时是说大致知道makefile文件的作用就行，但是现在看来都是坑：

1. makefile文件中Linux的编译链接是有参数的：

   ```cmake
   #SYSCFLAGS="-DLUA_USE_LINUX" SYSLIBS="-Wl,-E -ldl"
   #SYSCFLAGS是参数，SYSLIBS是系统库
   #不止这些，makefile文件最上面有这些：
   CC= gcc -std=gnu99
   CFLAGS= -O2 -Wall -Wextra -DLUA_COMPAT_5_3 $(SYSCFLAGS) $(MYCFLAGS)
   LDFLAGS= $(SYSLDFLAGS) $(MYLDFLAGS)
   LIBS= -lm $(SYSLIBS) $(MYLIBS)
   
   #这说明了我们实际上需要添加的编译参数为：
   -std=gnu99 -O2 -Wall -Wextra -DLUA_COMPAT_5_3 -DLUA_USE_LINUX
   #我们需要的链接库：
   -lm -Wl,-E -ldl
   ```

   于是最终相关的Cmake代码：

   ```cmake
   #编译参数
   if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" AND CMAKE_HOST_UNIX)
   set(CMAKE_C_FLAGS "-std=gnu99 -O2 -Wall -Wextra -DLUA_COMPAT_5_3 -DLUA_USE_LINUX")
   endif()
   
   #luac链接库
   if(${CMAKE_C_COMPILER_ID} STREQUAL "GNU" AND CMAKE_HOST_UNIX)
   target_link_libraries(luac liblua_static m -Wl,-E dl) #链接可执行文件和静态库和系统库
   else()
   target_link_libraries(luac liblua_static ) #链接可执行文件和静态库
   endif()
   
   #lua链接库
   if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
   if(CMAKE_HOST_UNIX)
   target_link_libraries(lua liblua_static m -Wl,-E dl) #链接可执行文件和静态库和系统库
   elseif(CMAKE_HOST_WIN32)
   find_library(liblua_shared ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) #在输出路径下找产生的动态库
   target_link_libraries(lua liblua_shared)
   endif()
   elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
   find_library(liblua_shared ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}) #在输出路径下找产生的动态库
   target_link_libraries(lua liblua_static)
   else()
   #其他情况下没试过
   endif()
   ```

2. MinGW相关的参数也应该如法炮制，由于我发现了lua使用了一个宏来使得MinGW编译时可以导出接口（不过似乎没有dll导出接口时MinGW编译照样可以导出一个引导库），所以最终的改动还是挺大的，直接见CMakeLists文件就行了。

经过今天这一折腾，倒是了解了一点make的知识，编译的相关参数。



# 总结一下踩的坑

## 1.导出函数的问题

在MSVC下必须使用导出接口进行函数接口导出，否则要链接动态库的时候找不到函数；

MinGW和GNU的话不需要进行函数接口导出，函数接口本身就是暴露的。

使用vs工具dumpbin查看函数接口可以发现，MSVC编译的lua54.dll只有153个函数接口，而MinGW编译的liblua.dll有344个函数接口：

```cmd
dumpbin /exports dllPath
```

## 2.不同编译器的库不能混用的问题

不同操作系统的库不能混用，这是常识，但是不同编译器的库不能混用我倒是不知道，就描述一下遇到的问题：

我电脑中Path中的lua是使用MinGW编译的，但是我为了测试cmake配置跨平台项目，于是用vs打开cmake项目进行编译，然后把生成的lua模块放到我电脑Path的lua的路径中，结果一直找不到模块，但是相同的项目使用MinGW编译出的模块却可以，一开始我是一直在想是不是导出接口的问题，找了好几个小时，最后我才反应过来可能是编译器不同的原因。

也许应该有一个通用的什么标识可以让MSVC和MinGW编译的库可以互认，但是个人感觉没这个必要。
