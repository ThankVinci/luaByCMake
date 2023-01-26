# luaByCMake
基于lua最新5.4.4版本创建的luaCMake项目，可直接编译出lua、luac、动态库与静态库。

要自己尝试构建过程可以参考lua源码中的Makefile文件，其中除了lua.c和luac.c外的源文件用于产生动态库和静态库，lua.c链接静态库产生lua二进制文件，luac.c链接静态库产生luac二进制文件。

新增我自己json配置，配置了64位的msvc和clang的debug和release版本。

后续考虑添加arm64的编译配置。

注：本项目仅在VS2019上测试通过
