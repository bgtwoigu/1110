@echo off

rem windowĬ���ַ�����GBK,���ڽ��ĳЩϵͳ�ַ�����ΪUTF-8��������
set OUTPUT_CHARSET=GBK

call flexi021 preobjs
call XGConsole /command="flexi021 genobjs -j40" /openmonitor /profile="XGConsole.xml"
rem ɾ����s�ļ����ɵ�o�ļ�
call \bin\sh RmEmptyFile.sh flexi021
call flexi021







