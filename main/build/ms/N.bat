@echo off

rem windowĬ���ַ�����GBK,���ڽ��ĳЩϵͳ�ַ�����ΪUTF-8��������
set OUTPUT_CHARSET=GBK

call %1 preobjs
call XGConsole /command="%1 genobjs -j40" /openmonitor /profile="XGConsole.xml"
rem ɾ����s�ļ����ɵ�o�ļ�
call sh RmEmptyFile.sh %1
call %1







