@echo off

rem windowĬ���ַ�����GBK,���ڽ��ĳЩϵͳ�ַ�����ΪUTF-8��������
set OUTPUT_CHARSET=GBK

call %1 preobjs
call %1 genobjs -j8
call %1







