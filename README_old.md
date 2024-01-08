# luaByCMake
基于lua最新5.4.6版本创建的luaCMake项目，可直接编译出lua、luac、动态库与静态库。

要自己尝试构建过程可以参考lua源码中的Makefile文件，其中除了lua.c和luac.c外的源文件用于产生动态库和静态库，lua.c链接静态库产生lua二进制文件，luac.c链接静态库产生luac二进制文件。

新增我自己的json配置，配置了64位的msvc和clang的debug和release版本。

后续考虑添加arm64的编译配置。（在做了在做了）之前用openwrt交叉编译链成功过。

注：之前很长一段时间为了适配Windows上的UTF-8，可能不经意间改动了什么，或者说新加载的模块有问题，反正Linux上无法编译通过，所以接下来这段时间主要要进行的是Linux上的适配，确保项目能够在Windows和Linux上编译通过，MacOS的话，由于没有机器，安装镜像目前不在旁边只能等到过年回家才能装个虚拟机试试了。

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

## 2023.10.28更新（好家伙鸽了4个月）

觉得不能这么鸽下去了，详情见UTF-8支持的可行性分析.md

## 2023.6.28更新

在luaByCMake项目中新增了lua-iconv子模块，该模块依赖libiconv库，因此编译了一份libiconv，并把库文件和头文件防止在lua-iconv项目下，具体看lua-iconv的README。

## 2023.3.17不是更新

最近赶毕设，这个项目就暂时不管了，不过LuaRunner其实已经可以正常工作了，只不过存在一个问题，那就是如果脚本执行出错的话，程序可能会一直拿不到数据，比如说require一个模块，但是这个模块是不存在的，由于脚本是使用pcall调用，所以即使错误也只是打印堆栈而已，脚本会直接结束不会影响到程序本身，但是如果一个模块不存在的话，我是希望能够知道的，所以我就写了一个模版，不过模版感觉很乱就是了，处理所谓的错误，我就是把容易出错的地方包装成一个函数，然后使用pcall调用，使其错误了也能返回true或false，然后在false的时候包装返回错误信息，我学lua时间也不长，也不知道能不能优化一下（见17号更新的main.lua)。

## 2023.3.13更新

又改了一个大夜。

1. 本次更新将MinGW配置到了VS的CMakeSettings.json中，在其他的机器上需要配置自己的MinGW目录，需要直接编辑json文件，设置`MINGW64_ROOT`为自己机器上的MinGW；
2. 为了使得脚本能够返回字节流数据，所以rlua库中的runLua函数新增一个参数用于接收返回值数组中每个返回值的大小；
3. 本次更新把LuaRnr的接口进行了规范化，LuaRnr_imp类中封装了大部分的功能，LuaRnr类作为接口类，提供部分简单的接口；
4. 本次更新也对lua脚本运行器能够正常运行的脚本进行了一个规范化设计，因为这里也踩了坑。

## 2023.3.12更新

原本是写了一个luaRunnerByCMake项目，但是没有放进来，本来是想着区分不同操作系统，在调试的时候也发现太乱了，在Windows下我可能一会搞MinGW一会搞MSVC，然后就混乱了。最后还是决定把luaRunnerByCMake整合到luaByCMake中，整合进来的好处就是一个平台编译就一定链接了同一个平台的相关库，各种链接库问题会少很多。

1. 本次更新把runLuaScript子项目中的生成目标可执行文件和动态库永久取消掉了，改成生成静态库，同时也链接了liblua静态库，做这一步修改的原因就是lua其实很轻量级啊，如果拆成多个动态库的话，链接起来就麻烦多了；
2. 取消了runLuaScript项目的可执行文件之后，调试的工作就交给它的子项目luaRunnerByCMake，这个项目中配置了两个生成目标，一个是luarnr，是最终的生成库，另一个则是demo，用于对这个库的功能进行调试；
3. 修改了lua源代码中的cpath，使得lua加载模块的时候能够找到./luamodules/lib这个目录下的C包，后面看还有没有需要改的地方；
4. linux下编译还是发现问题了，写到坑里面。

## 2023.3.11更新

我宣布这是我优化这个项目以来最爽的一次，学到很多东西。

兴奋到语无伦次了，抱歉。

今天把电脑升级到32G内存，于是装了个MacOS的虚拟机，想到luaByCMake项目只配置了Linux和Windows，所以又增加了Darwin的编译设置，不更新不打紧，一更新我的强迫症就犯了，所以此次更新除了增加Darwin以外，整个CMakeLists文件大换血：

1. 本次更新规范化了luaByCmake项目的构建，按照每个步骤去分块，给每个步骤标序号，同时尽可能把每个步骤之间的耦合降到最低，使得整个CMakeLists文件看上去一目了然，改动的时候不会牵一发而动全身；
2. 之前写的时候是按编译器类型去分，但是现在发现一个问题，我只有在Windows下才会同时用到两种编译器（MinGW和MSVC），而用Linux时只会用到GNU，至于MacOS用的频次更少，所以我应该以操作系统来进行区分，这样哪怕该操作系统下有什么其他的编译器，再做区分就好了；
3. 编译mingw的lua.exe和lua54.dll的时候，按照原先项目中的设定进行符号删除，给二进制文件做裁剪，lua54删除不需要的符号，lua删除全部符号；
4. 编译linux下的lua的时候，CMAKE_EXE_LINKER_FLAGS这个cmake变量不会起作用，所以linux下需要手动设置target_link_libraries，把变量添加到链接中，其他平台则不需要，只要把传给ld的选项需要添加到变量即可完成链接；
5. linux和darwin下，链接动态库会存在一个路径问题，查了一下发现可以通过指定runpath来解决，于是就这么搞了，详情见坑的解析；
6. 本次更新把lua模块的模版也进行一个大改，现在如果要新建一个模版的话，只需要把模版复制一份，改目录名和CMakeLists中的模块名称，新建模块更方便了；

## 2023.3.8~3.9更新

1. ~~本次更新添加了一个测试api的test子模块，但是我没有搞清楚这个模块的定位，所以没有添加进来。~~

2. 本次更新添加了一个名为runLuaScript的子项目，该项目将会被编译成库（库名称为rua，别问我为什么叫rua），然后结合引导库文件和头文件实现链接。

   rua库中提供了一个对外开放的run函数，它的声明在rlua.h头文件中，直接贴出来：

   ```c
   RLUA_DLL_API int run(int argc, char** argv, int* retc, char*** retv);
   ```

   函数接收四个参数，前两个为输入，后两个为输出，输入的两个参数我是参考了lua的main函数传参给pmain函数，前两个参数为必须的，如果只是执行脚本而不需要得到返回数据的话，后两个参数可以传入NULL。

3. 本库调用的脚本代码如果要返回数据的话，必须遵守以下规则：

   ```c
   res = {
   	retc = 2,
   	retv = {"返回的第一个结果","返回的第二个结果"}
   }
   return res
   ```

   也就是说，在lua脚本中必须用一个表包含真实返回值的数量以及真实值，并且这个表中的key不能更改，只能是retc和retv。

### 说明

写这个模块的主要原因是希望能够在其他的项目中调用lua脚本，但是不想在程序中嵌入一个lua的程序后调用命令行，这样看上去太不优雅了，一开始是想写简单点，反正用require也可以进行模块调用，只要lua脚本文件和动态库放一起就行，但是后面发现不可以，因为如果lua文件是加载到像Qt这种具有资源系统的框架中的话，要运行lua脚本可能还要从PE中解压出来，放到一个地方拉屎，我的想法就是搞一个和lua的main函数功能差不多，但是只能加载字符串为lua脚本后进行调用的函数，有源代码作参考，写起来其实并不困难，只是看懂API的用法多花了点时间，事后发现看的一些API其实都用不到。

如何加载一个文件为C的字符串，这个我不管，接口已经实现，只要想办法把入参搞到手就行。

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

## 3.链接器选项相关

1. 首先是CMAKE_EXE_LINKER_FLAGS这个变量吧，添加到这个变量中的编译选项会传递给链接器，在Windows和MacOS下，这个变量就有作用，但是在linux下这个变量是无效的，就跟一个普通变量一样，最终我还是手动设置target_link_libraries，把linux需要的链接加上；

2. 后知后觉，lua原项目的makefile中的`SYSCFLAGS`之类的参数是预定义宏，`SYSLIBS`之类的参数可以直接传递给链接器，而`SYSLDFLAGS`之类的参数也是要传递给链接器的，但是传递的方式比较特别

   ```cmake
   add_compile_definitions(NDEBUG ) #添加一个宏
   set(CMAKE_EXE_LINKER_FLAGS "-lm -ldl") #传递`SYSLIBS`
   set(CMAKE_EXE_LINKER_FLAGS "-Wl,-E") #传递`SYSLDFLAGS`：E
   ```

3. 关于strip，因为看了MinGW的配置中使用了`strip --strip-unneeded`，但是下面却使用SYSLDFLAGS传递了`-s`，这让我不解，最后才知道，虽然strip也有-s，但是和链接器的-s却不是同一个-s，只是含义一样，所以`--strip-unneeded`链接器是不认的，只能手动设置，幸好是cmake中可以执行自定义的命令

   ```makefile
   #makefile中对mingw的编译配置
   mingw:
   $(MAKE) "LUA_A=lua54.dll" "LUA_T=lua.exe" \
   "AR=$(CC) -shared -o" "RANLIB=strip --strip-unneeded" \
   "SYSCFLAGS=-DLUA_BUILD_AS_DLL" "SYSLIBS=" "SYSLDFLAGS=-s" lua.exe
   $(MAKE) "LUAC_T=luac.exe" luac.exe
   ```

   `strip --strip-unneeded`，就是删除符号表中不需要的符号，`SYSLDFLAGS=-s`则是传递参数给链接器ld，让其删除所有符号，一开始我是不知道这两个东西的区别的，就在想为什么一个是执行strip另一个是传递给链接器，后来我试了好几次都报错，问了chatgpt才知道不能直接把--strip-unneeded传给链接器，因为这个是strip的运行参数。所以最后的解决方案是在CMakeLists中调用自定义命令去执行strip，把生成的lua54.dll中不需要的符号进行删除；同理也删除lua.exe的所有符号。

   调用自定义命令在cmake中对目标进行符号删除

   ```cmake
   add_custom_command(TARGET ${MODULE_NAME_WITH_MODULE} #直接把符号都删了
       POST_BUILD #在目标生成后再执行
       COMMAND strip "-s" "$<TARGET_FILE:${MODULE_NAME_WITH_MODULE}>") 
   ```

   最让我觉得惊奇的是，我故意把生成的模块的符号表全删除了，但是仍然能够完成require操作，同时模块的体积确实变小了。

## 4.关于符号

1. MinGW编译的lua模块不管有没有用暴露接口的宏，模块中的接口都是能够找到的，让我比较懵逼的是，如果使用strip删除了模块了所有符号，使用nm显示没有符号的情况下，仍然可以require该模块成功。这属实是震惊到我了。

## 5.关于动态库

1. 有一个小问题就是动态库，在Windows下为dll、在linux下为so、但是在MacOS下编译出来却是dylib，属实给我整不会了，打开搜索引擎找，你看苹果这么大一公司，总不能lua都跑不了吧，可能是我搜的姿势不对，反正看半天找不出个所以然，最后是什么呢，我灵机一动，想到不久之前（其实很久了），我还在想add_library里面`SHARED`和`MODULE`的区别，但是我没有动手去查啊，就这么放着不管（敲头）

   ```txt
   STATIC, SHARED, or MODULE may be given to specify the type of library to be created. STATIC libraries are archives of object files for use when linking other targets. SHARED libraries are linked dynamically and loaded at runtime. MODULE libraries are plugins that are not linked into other targets but may be loaded dynamically at runtime using dlopen-like functionality. If no type is given explicitly the type is STATIC or SHARED based on whether the current value of the variable BUILD_SHARED_LIBS is ON. For SHARED and MODULE libraries the POSITION_INDEPENDENT_CODE target property is set to ON automatically. A SHARED library may be marked with the FRAMEWORK target property to create an macOS Framework.
   ```

   cmake官方文档是这么写的，`SHARED`库动态链接并在运行时加载。 `MODULE`库是没有链接到其他目标但可以在运行时使用类似 dlopen 的功能动态加载的插件。也就是说，编译这些模块，其实都不是运行时加载的，都是动态添加的插件，因此，所有的lua模块生成库都应该改成`MODULE`。

   然后我就设置模块为`MODULE`，结果MacOS编译出来so文件了。

2. 一个小细节，之前一直认为Windows下编译dll都会放到bin目录，因为会识别成RUNTIME，但其实不是这样，当编译动态库指定为`MODULE`时，编译出来的dll就是纯纯的库，所以会被放到LIBRARY中，因此从中能够得到一个结论，如果编译的动态库不是运行时链接而是在程序中作为模块打开（比如说lua的C包），那么编译时就应该编译成`MODULE`而不是`SHARED`。

3. 其实这么来看，lua原项目的配置有点不太合理，只有在Windows下编译会产生动态库和链接动态库，linux和MacOS下，只会产生静态库和链接静态库，这意味着每次链接一次lua静态库就会多一份资源的浪费，其实很没有必要，但是如果在linux下进行编译的话，存在一个问题，首先是运行库liblua.so，lua需要链接liblua.so才可以运行，但是编译的时候这个链接的路径就写死了，可以用：

   ```shell
   ldd xxx
   ```

   来查看链接的库的路径，然后使用：

   ```shell
   readelf -d xxx
   ```

   来查看elf表中有没有RUNPATH或者RPATH属性。

   RUNPATH其实也就是告诉二进制文件，让它运行的时候去哪里找so文件，一般来说，so文件都是存在于系统目录中，它找就去系统目录找，哪怕so文件和exec文件放在一起，程序都找不到库，所以为了解决这个实际问题，我就设置了RPATH属性，这样只要RPATH的路径下有库那就可以连接。

## 6.关于RPATH

1. 关于RPATH的问题，说简单也简单，说复杂也复杂，首先就是lua中涉及到的两种库，运行时链接库和模块库，我们在编译的时候，lua链接了这个动态库，模块也链接了这个动态库，使用ldd指令就可以看到程序或者模块库链接的动态库以及地址，这里直接给出一个暴论：不用管模块的链接，只要lua主程序能加载到动态库就行，那就得讲讲加载的规则，lua主程序或者具有lua功能的程序/库要可以加载到这个库，那么就可以这么设置， lua主程序还是按照它原来的加载，也就是加载lua本目录或者父目录下的lib目录中的liblua，而我们想实现具有lua脚本执行功能的库，就可以加载本目录或者本目录下的lib/lua目录下的liblua。

2. 看网络上天花乱坠的介绍方法，结果我是试了一天才试出来CMAKE中可用的方法，使用方法很简单，分linux和macos：

   ```cmake
   if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
   	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
   	endif()
   elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
   	set_property(
   		TARGET ${LUA_EXEC}
   		PROPERTY INSTALL_RPATH
   		"$ORIGIN/"
   		"$ORIGIN/../lib/"
   	)
   elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
   	#set(CMAKE_MACOSX_RPATH ON) #测试之后发现这个不开也是可以的
   	set_property(
   		TARGET ${LUA_EXEC}
   		PROPERTY INSTALL_RPATH
   		"@executable_path/"
   		"@executable_path/../lib/"
   	)
   endif()
   ```

   设置什么CMAKE中的RPATH之类的变量为TRUE或者FALSE我试过很多次并没有决定性作用，而生成目标的INSTALL_RPATH属性才有决定性作用。

3. 关于封装了lua脚本执行功能的库和liblua的链接问题，可能需要再商榷一下，因为如果考虑搞跨平台的话，Windows只能设置该运行库与liblua在同一目录下。

4. 最后一个也是最重要的，和CMAKE无关，是lua源码中加载模块库的路径cpath，在非Windows的情况下，我在源码中新增了父目录的lib目录；在使用运行库的情况下，应该新增一个寻找同级目录下的lib/lua/5.4。

## 7.还是与库相关

1. 现在才发现一件事情，在linux下，一个生成动态库的目标不能链接静态库，反之同理，可能macos也是一样的；
2. 然后是遇到了一个坑，就是相同的代码，linux下运行demo程序直接段错误了，但是Windows下和MacOS下都不会。只有demo程序有问题。

## 8.与脚本相关

本LuaRunner中需要规范lua执行脚本，否则脚本会执行失败或者得不到返回的数据，因此，对脚本进行如下规范要求：

1. 返回值如果有就必须是个table并且包含`retc`、`rets`、`retv`三个key：

   ```lua
   local retv1 = "返回第一个接口"
   local retv2 = "返回第二个接口"
   
   res = {
   	retc = 2,
   	rets = {#retv1,#retv2},
   	retv = {retv1,retv2}
   }
   
   return res
   ```

   如果要有返回值的话，返回值必须是一个table，否则返回值是无效的，作为返回值的table，必须有3个key的值：`retc`的值必须是一个int值，告知从脚本返回的返回值的数量，`rets`的值必须是一个table，table中包含了返回值列表中每个返回值的长度，`retv`的值必须是一个包含所有返回值的table。

2. 关于字符串类型需知

   在lua中的string字符串可以包含0x00的字节，也就是说lua的string字符串可以作为字节流使用，但C字符串会在0x00处截断，lua中lua_tostring函数可以创建一个C字符串，本来如此便好，但lua_tostring函数在某些编译器下会回收掉C字符串的空间导致内存访问的内存错误，所以在获取到脚本中的lua_tostring变量之后还要拷贝一份数据出来，既然要拷贝，就免不了获取数据的长度，在上述例子中，rets表中就传入了待拷贝的数据长度，但是很明显就是（3*7=21字节），如果这个字符串经由lua_tostring函数就会得到一个长度为22字节的C字符串（带末尾空字符），但是进行拷贝的时候就会拷贝错成21字节，而如果是普通字节流数据的话，这个数据长度却是正确的，所以废话说了这么多还是讲一下脚本对字符串的规范：

   在本LuaRunner可执行的脚本中定义的所有字符串如果涉及到要返回的，在字符串的末尾一定都要加上\0，这样计算出来的要拷贝的数据长度才是正确的，例：

   ```lua
   local retv1 = "返回第一个接口\0"
   local retv2 = "返回第二个接口\0"
   ```

3. 当一个脚本执行无效时，如果程序是命令行窗口的话，程序会异常退出，但是如果是使用QT去调用的话，当脚本运行到发生错误的地方之后，脚本的执行就结束了，而我设计的luarnr的返回值是建立在脚本运行成功的条件之上的，所以如果想要知道脚本究竟在哪里运行错误的话，就应该在脚本中容易发生错误的地方先进行预判，这样来说一个脚本只执行一个功能是最好不过的。

   要进行预判的话，其实就是想让程序遇到错误也不要退出，即错误处理，lua中的pcall函数是保护调用的功能，传入一个函数，如果在调用中如果发生错误的话，pcall会返回false和错误堆栈，如果调用成功的话，就可以返回true和被调用函数的返回值。

   比如说我要加载一个cjson模块，那么可能模块不存在，所以：

   ```lua
   local load_module = function(module_name)
      return require(module_name) 
   end
   
   local load_status,cjson_module = pcall(load_module,"cjson")
   if(load_status == true)
   then
       print("加载成功")
   else
       print("加载失败")
   end
   ```

   

4. 

5. 
