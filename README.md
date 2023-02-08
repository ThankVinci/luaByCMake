# luaByCMake
基于lua最新5.4.4版本创建的luaCMake项目，可直接编译出lua、luac、动态库与静态库。

要自己尝试构建过程可以参考lua源码中的Makefile文件，其中除了lua.c和luac.c外的源文件用于产生动态库和静态库，lua.c链接静态库产生lua二进制文件，luac.c链接静态库产生luac二进制文件。

新增我自己json配置，配置了64位的msvc和clang的debug和release版本。

后续考虑添加arm64的编译配置。

注：本项目仅在VS2019上测试通过



2023.2.8更新

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
