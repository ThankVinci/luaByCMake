@echo off

mkdir build
cd build
make clean
cmake .. -G "MinGW Makefiles"
::如果提示找不到make的话，请将编译使用的mingw工具链中的mingw32-make.exe复制一份改名为make.exe
make -j8
::可以使用make install将编译后的文件安装到项目根目录/installed目录下
::make install