# luaByCMake
基于lua最新5.4.4版本创建的luaCMake项目，可直接编译出lua、luac、动态库与静态库。

要自己尝试构建过程可以参考lua源码中的Makefile文件，其中除了lua.c和luac.c外的源文件用于产生动态库和静态库，lua.c链接静态库产生lua二进制文件，luac.c链接静态库产生luac二进制文件。

新增我自己json配置，配置了64位的msvc和clang的debug和release版本。

后续考虑添加arm64的编译配置。

注：本项目仅在VS2019上测试通过



2023.2.8更新

发现MinGW编译lua是链接了动态库，但是使用MSVC编译发现：

1. lua源代码中没有提供dll的导出接口，所以MSVC编译的话只能链接静态库，所以就直接搞个按编译器类型生成目标名称以及链接库，GNU编译器如果是类UNIX平台就链接静态库，如果是WIN32平台就链接动态库，MSVC编译器就链接静态库；
2. MSVC编译时，即使使用set_target_properties也不支持静态库和动态库同名，所以MSVC编译时，静态库就叫liblua，而动态库就叫lua54；
