goto start
:: �����ļ��ύ��gitʱ���Զ������з�תΪLF��������Ҳ����ı��ҵ����ý�������������Ŀ�Ļ��з����ı䣬
:: ��������������������ļ�����ʧ�ܵĻ������ֶ����ļ��Ļ��з��ĳ�CR LF
:: �����ʾ�Ҳ���make�Ļ����뽫����ʹ�õ�mingw�������е�mingw32-make.exe����һ�ݸ���Ϊmake.exe
:: Ĭ�Ϲ���Ϊrelease�汾�����ǵ�һ����������Ϊdebug
:start

@echo off

set build_type="Release" 
if "%1"=="debug" (
	set build_type="Debug"
)
mkdir build
cd build
make clean
cmake .. -G "MinGW Makefiles" -D CMAKE_BUILD_TYPE=%build_type%
make -j8
make install -j8
cd ..