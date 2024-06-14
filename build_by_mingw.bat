goto start
= 由于文件提交到git时会自动将换行符转为LF，并且我也不想改变我的配置进而导致整个项目的换行符都改变，
= 所以如果遇到本批处理文件运行失败的话，请手动将文件的换行符改成CR LF
= 如果提示找不到make的话，请将编译使用的mingw工具链中的mingw32-make.exe复制一份改名为make.exe
:start

@echo off
mkdir build
cd build
make clean
cmake .. -G "MinGW Makefiles"
::make -j8
::make install