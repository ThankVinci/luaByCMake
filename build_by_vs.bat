@echo off

mkdir build
cd build
::-G 后面表示要构建的项目的生成器类型，这里指定的是vs2022，详情可以使用cmake -G 查看支持的类型
cmake .. -G "Visual Studio 17 2022"
::执行完cmake命令后将会在build目录下生成luabycmake的vs项目
::使用devenv编译和安装解决方案，需要自己配置devenv的路径哦
::"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" luaByCMake.sln /Project install.vcxproj /Rebuild Release /out log.txt