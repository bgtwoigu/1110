@echo off

rem windowĬ���ַ�����GBK,���ڽ��ĳЩϵͳ�ַ�����ΪUTF-8��������
set OUTPUT_CHARSET=GBK

call w021 preobjs
call XGConsole /command="w021 genobjs -j20" /openmonitor /profile="XGConsole.xml"
rem ɾ����s�ļ����ɵ�o�ļ�
call \bin\sh RmEmptyFile.sh W001
call w021
goto EXIT







