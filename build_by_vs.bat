goto start
:: �����ļ��ύ��gitʱ���Զ������з�תΪLF��������Ҳ����ı��ҵ����ý�������������Ŀ�Ļ��з����ı䣬
:: ��������������������ļ�����ʧ�ܵĻ������ֶ����ļ��Ļ��з��ĳ�CR LF
:: cmake -G �����ʾҪ��������Ŀ�����������ͣ�����ָ������vs2022���������ʹ��cmake -G �鿴֧�ֵ�����
:: ִ����cmake����󽫻���buildĿ¼������luabycmake��vs��Ŀ
:: ʹ��devenv����Ͱ�װ�����������Ҫ�Լ�����devenv��·����������
:start

@echo off
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
"C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.exe" luaByCMake.sln /Project install.vcxproj /Rebuild Release /out log.txt