@echo off

rem windowĬ���ַ�����GBK,���ڽ��ĳЩϵͳ�ַ�����ΪUTF-8��������
set OUTPUT_CHARSET=GBK
rem ���û�������
rem PATH=..\..\Tools\xMake;..\..\..\Tools\xMake;..\..\platform\brew\brewpk\brewtools\ResourceEditor;%PATH%

rem b n������������
IF "%~1"=="n" goto INCREDI_BUILD
rem b c����ļ�
IF "%~1"=="c" goto CLEAN
rem b ��ͨ����
:NORMAL_BUILD
w021 %~1
goto EXIT

:INCREDI_BUILD
rem makeʹ�õ��߳������bat����
call w021 preobjs
call XGConsole /command="w021 genobjs -j20" /openmonitor /profile="XGConsole.xml"
rem ɾ����s�ļ����ɵ�o�ļ�
call \bin\sh RmEmptyFile.sh W001
call w021
goto EXIT



:CLEAN
w021 clean
goto EXIT

:EXIT
rem echo msgbox "�������,�˳���">msg.vbs
rem msg.vbs
rem del msg.vbs
rem IF NOT "%*"=="" goto END
msg %username% /TIME:60000 �������,�˳���
goto END


:END
rem shutdown -s -f
rem rundll32.exe powrprof.dll,SetSuspendState 0,1,0



