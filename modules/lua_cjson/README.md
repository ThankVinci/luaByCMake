本模块修改自**openresty**维护的**lua-cjson**：https://github.com/openresty/lua-cjson ，版本2.1.0.9

我也不知道原先根目录下一些文件有什么用，怕删除了一些协议，所以只删除了CMakeLists和Makefile，头文件和源文件都归类放到src和include目录下，然后新增一个CMakeLists文件用于构建本项目(**lua-cjson-general**)。

如同名称含义一样，它是一个通用的项目（当然原先的项目就支持5.1+），原项目的makefile文件不适用于mingw，而cmake文件会链接静态库导致编译出来的库文件包含了liblua的全部符号。所以就编写了这个cmake文件，可以只进行动态库链接。

进行lua模块编译，需要以下的文件：

1. lua54.dll/liblua.dll/liblua.so，这是需要链接的动态库，放于CMakeLists文件同级的lib目录下
2. lauxlib.h、lua.h、lua.hpp、luaconf.h、lualib.h，这是需要包含的头文件，放于CMakeLists文件同级的include/lua目录下

作为lua项目的子项目模块，以上的问题都很好解决，动态库在lua项目的CMakeLists文件已经写好生成的目标，直接拿过来用就行，然后作为子项目的话，可以访问父项目的包含目录，所以这些文件根本不需要拉进项目，包括之后如果lua-cjson有进行更新的话，就直接把头文件和源文件覆盖到include和src目录下即可。
