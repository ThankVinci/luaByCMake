# luaByCMake
基于lua最新5.4.6版本创建的luaCMake项目，可直接编译出lua、luac、动态库与静态库。

要自己尝试构建过程可以参考lua源码中的Makefile文件，其中除了lua.c和luac.c外的源文件用于产生动态库和静态库，lua.c链接静态库产生lua二进制文件，luac.c链接静态库产生luac二进制文件。

# 最近的更新

## 2024.1.8

原先的CMakeLists实在过于臃肿，经过了几个月的工作吧，也悟到了一些经验，遂重新把根项目的CMakeLists重写了，将不同编译器的配置写成.cmake文件，在CMakeLists中导入，一些通用的配置就写到base.cmake和common.cmake中，另外也提供了交叉编译的支持。

目前在Linux、Windows上编译成功，交叉编译也成功，MacOS还是只能等回家过年装虚拟机试试。

## 2023.10~11

实现了Windows下的efw模块，为Windows下的lua提供了utf-8支持。