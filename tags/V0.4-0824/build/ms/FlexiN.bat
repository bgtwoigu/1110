@echo off

rem windowĬ���ַ�����GBK,���ڽ��ĳЩϵͳ�ַ�����ΪUTF-8��������
set OUTPUT_CHARSET=GBK

call flexi preobjs
call XGConsole /command="flexi genobjs -j20" /openmonitor /profile="XGConsole.xml"
rem ɾ����s�ļ����ɵ�o�ļ�
call \bin\sh RmEmptyFile.sh Flexi
call flexi







