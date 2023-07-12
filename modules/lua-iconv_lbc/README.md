本模块修改自**ittner**的**lua-iconv**：https://github.com/ittner/lua-iconv，版本7，未对代码进行任何改动

我也不知道原先根目录下一些文件有什么用，怕删除了一些协议或者什么东东，所以只把头文件和源文件都归类放到src和include目录下，然后新增一个CMakeLists文件用于构建本项目(**lua-iconv-module**)。

进行lua模块编译，需要以下的文件：

1. lua54.dll/liblua.dll/liblua.so，这是需要链接的动态库，放于CMakeLists文件同级的lib目录下
2. lauxlib.h、lua.h、lua.hpp、luaconf.h、lualib.h，这是需要包含的头文件，放于CMakeLists文件同级的include/lua目录下
3. iconv.h头文件和libiconv库文件，为方便起见，这些都已经放置在项目中

作为lua项目的子项目模块，以上的问题都很好解决，动态库在lua项目的CMakeLists文件已经写好生成的目标，直接拿过来用就行，然后作为子项目的话，可以访问父项目的包含目录，所以这些文件根本不需要拉进项目，包括之后如果lua-iconv有进行更新的话，就直接把头文件和源文件覆盖到include和src目录下即可。



libiconv的版本为1.17，Windows编译的话，我是使用msys2编译的，自己配的mingw64+cmd或者git自带的bash在编译时会出现各种错误，用msys2编译比较省事。

当然也因此在模块的lib中的库为mingw64编译出来的库，msvc无法使用。（之后我会想一下怎么把libiconv改成cmake，大概率是没时间搞的）
