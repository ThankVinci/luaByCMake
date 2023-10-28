2023-10-28更新：

鸽了好久，本来想要大刀阔斧把lfs改成在Windows下支持宽字符串的版本，结果看了一下源码发现C库其实已经提供了文件操作的接口，而lfs的作者对文件IO的操作是基于C库实现的，
因此从理论上来说，可以切换成使用宽字符串的API，进而正常地在Windows的中文环境下使用UTF8编码进行操作。当然最终的结果就是按我的预想一样，为了不大改到lua的源码，
所以在lfs需要使用宽字符串的文件API的时候，把utf-8字符串转为宽字符串，用完再回收这个宽字符串以免发生内存泄漏。

2023-7-13更新：

本模块修改自**hishamhm**的**luafilesystem**：https://github.com/lunarmodules/luafilesystem ，版本1.8.0

我也不知道原先根目录下一些文件有什么用，怕删除了一些开源协议相关的东西，所以只删除了Makefile相关的文件，头文件和源文件都归类放到src和include目录下，然后新增一个CMakeLists文件用于构建本项目(**luafilesystem**)。

进行lua模块编译，需要以下的文件：

1. lua54.dll/liblua.dll/liblua.so，这是需要链接的动态库，放于CMakeLists文件同级的lib目录下
2. lauxlib.h、lua.h、lua.hpp、luaconf.h、lualib.h，这是需要包含的头文件，放于CMakeLists文件同级的include/lua目录下

作为lua项目的子项目模块，以上的问题都很好解决，动态库在lua项目的CMakeLists文件已经写好生成的目标，直接拿过来用就行，然后作为子项目的话，可以访问父项目的包含目录，所以这些文件根本不需要拉进项目，包括之后如果luafilesystem有进行更新的话，就直接把头文件和源文件覆盖到include和src目录下即可。
