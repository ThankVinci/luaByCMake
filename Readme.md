# luaByCMake
基于lua最新5.4.6版本创建的luaCMake项目，可直接编译出lua、luac、动态库与静态库。

要自己尝试构建过程可以参考lua源码中的Makefile文件，其中除了lua.c和luac.c外的源文件用于产生动态库和静态库，lua.c链接静态库产生lua二进制文件，luac.c链接静态库产生luac二进制文件。

# 最近的更新

## 2024.11.20

1. 为了方便调试，`efw`模块的子模块目录`lua_exforwin`正式改名为`lua_efw`；
2.  模仿`lua_writestring`实现宽字符串版本：`lua_writewstring`；
3. 修复在`lua`命令行中导入`efw`模块会引起错误的问题，原因是宽字符串输出后翻译模式没有改回`_O_TEXT`导致命令行`readline`时`fputs`时以`_O_U16TEXT`模式输出了窄字符串触发了错误；
4. 偶然发现一个`bug`，在导入`efw`模块之后`print`会失效，调试后发现是`msvcrt`的问题，即，最新一次更新的修正，使用`fwrite`写宽字符串，使用`msvcrt`把输出模式改成`U16`之后输出就无效了，不清楚原因，用了好几个大版本的`mingw_msvcrt`都一样，所以只能多加点兼容，如果使用`mingw_msvcrt`进行编译的话，就手动把`mingw.cmake`中的宏开关启动。`mingw_ucrt`不会有这个问题；

## 2024.6.14

无意中看到有小伙伴不知道怎么编译这个项目，也不知道他是不是已经放弃了这个项目，我回忆了一下，已经废弃的autogen.lua文件中还有已经废弃的Readme_old.md中虽然有讲到如何编译，不过已经和我现在的操作不太一样了。详情请看`项目编译说明.md`，不过现在项目已经提供了构建脚本: [`build_by_mingw.bat`、`build_by_vs.bat`、`build_in_linux_macos.sh`]，放置在本项目的根目录下。

## 2024.6.9

时隔4个月的更新，本来是不想修改到ua的源码的，本次更新还是修改了lua程序的源码：

1. 在Windows下通过命令行调用lua，参数指定为lua文件，若lua文件路径编码为非本地代码页编码时会找不到文件，研究了一下，发现是因为传入的参数就是本地代码页编码的char*，这个问题很好解决，用Windows的API重新获取了一遍UTF16参数，然后转为UTF8即可。
2. 拿到UTF8文件路径后，通过os.execute调用lua程序还是会找不到文件，因为通过os.execute调用lua还是通过命令行调用的，因此得从函数入口开始到打开文件前进行处理，那么efw这个模块就不起作用了，因为efw加载原理是得先能够加载lua脚本，再在lua脚本中加载efw模块达到替换lua的API的作用。

综合以上原因，修改了lua的main入口函数，使其先获取了一次UTF16参数，再转为UTF8编码，走原来的处理流程；修改了lauxlib.c，在Windows下重新实现了luaL_loadfilex函数（当然是直接从efw中拷贝过来的）；至于efw就直接移除了baselib_ex.c中的dofile函数和auxlib_ex.c源文件。

## 2024.2.14

春节回家之后好懒好懒，一直在看僵约，直到前两天晚上才想起来我春节回来是要适配MacOS的编译的，但是cmake配置看起来还是不太顺眼，就比如说所有的子模块的common其实都是同一个common，其中会指定子模块链接lua动态库和安装的路径，所以，这部分内容应该所有子模块都一样的，所以就抽出来改了个名字而已，然后原来子模块中重复的common就删除了；

主模块的改动比较大，原先我是把不同平台的配置几乎全部分离开，但是后面我发现，不同配置实际上区别是一些link的flags和宏定义，像编译生成的目标文件名称和前缀之类的属性其实不应该在单独的配置中设置，把这些配置写到同一个地方，根据不同的平台去实现，这样看起来清晰一点；

目前改完之后MinGW、GNU和AppleClang是正常编译和安装了，但是MSVC又出问题了（哭笑不得）（原因排查了一下是VS的CMAKE_EXE_LINKER_FLAGS有问题）

## 2024.1.8

原先的CMakeLists实在过于臃肿，经过了几个月的工作吧，也悟到了一些经验，遂重新把根项目的CMakeLists重写了，将不同编译器的配置写成.cmake文件，在CMakeLists中导入，一些通用的配置就写到base.cmake和common.cmake中，另外也提供了交叉编译的支持。

目前在Linux、Windows上编译成功，交叉编译也成功，MacOS还是只能等回家过年装虚拟机试试。

## 2023.10~11

实现了Windows下的efw模块，为Windows下的lua提供了utf-8支持。