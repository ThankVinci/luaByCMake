goto start
:: 由于文件提交到git时会自动将换行符转为LF，并且我也不想改变我的配置进而导致整个项目的换行符都改变，
:: 所以如果遇到本批处理文件运行失败的话，请手动将文件的换行符改成CR LF
:: cmake -G 后面表示要构建的项目的生成器类型，这里指定的是vs2022，详情可以使用cmake -G 查看支持的类型
:: 执行完cmake命令后将会在build目录下生成luabycmake的vs项目
:: 使用devenv编译和安装解决方案，需要自己配置devenv的路径！！！！
:start

@echo off
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" luaByCMake.sln /Project install.vcxproj /Rebuild Release /out log.txt