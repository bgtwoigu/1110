@echo off
@set PROJECTNAME=Flexi203guru
@set BASEPROJECTNAME=W203
@set QUALCOMMNAME=SBNAALZ
@set QUALCOMMNAME_BUILD=SXNAALZ
@set RESOURCEDIR=Flexi203
@set MODELVERSION=%PROJECTNAME%_CAM_FM_V1.0
@set HWVERSION=W203_MB_V0.6

rem delete some files for compile muslim or nasrani
del %PROJECTNAME%\CoreAppDialogHandler.o
del %PROJECTNAME%\OEMConstFiles_Dynamic.c
del %PROJECTNAME%\OEMConstFiles_Dynamic.o
del %PROJECTNAME%\OEMModTableExt.o
del %PROJECTNAME%\Staticapp.o
del %PROJECTNAME%\CoreApp.o

IF "%*"=="" goto DO_COPY
goto DO_WORK
:DO_COPY
xcopy %QUALCOMMNAME_BUILD% %PROJECTNAME% /S /Q /I /Y /D 2>&1 > temp.tmp
xcopy ..\..\core\boot\romboot\osbl\osbl_lib_%QUALCOMMNAME% ..\..\core\boot\romboot\osbl\osbl_lib_%PROJECTNAME% /S /Q /I /Y /D 2>&1 > temp.tmp
xcopy ..\..\core\boot\romboot\dbl\dbl_lib_%QUALCOMMNAME% ..\..\core\boot\romboot\dbl\dbl_lib_%PROJECTNAME% /S /Q /I /Y /D 2>&1 > temp.tmp
xcopy ..\..\core\boot\romboot\fsbl\fsbl_lib_%QUALCOMMNAME% ..\..\core\boot\romboot\fsbl\fsbl_lib_%PROJECTNAME% /S /Q /I /Y /D 2>&1 > temp.tmp
cd %PROJECTNAME%\libs\hsusb
IF NOT EXIST %PROJECTNAME% ren %QUALCOMMNAME% %PROJECTNAME%
cd ..\..\..

:DO_WORK
del /f /a build%PROJECTNAME%.log.old

if exist build%PROJECTNAME%.log move build%PROJECTNAME%.log build%PROJECTNAME%.log.old > nul

make -r -f dmss1100.mak %*  USES_RELEASE_VERSION=yes USES_NO_ASSERT=yes USES_DS_1536=no USES_BREW_APPMGR=no USES_FLEXI_STATICAPP=yes USES_NASRANI=no USES_GURU=yes USES_FMN2010=yes USES_FPT005=yes USES_CAH006=yes USES_CONSTEFS=yes USES_MMI=yes EXTLANG1=id EXTLANG2=NO USES_REND=yes USES_LCD_SIZE=160x128 USES_RESDIR=%RESOURCEDIR% USES_CAMERA=yes USES_NO_DEBUG=yes USES_HSU_MS_FD=yes USES_HFAT=yes USES_SDCC=yes USES_SECBOOT_2_0=yes USES_SPANSION_S29WS256N0SB=yes USES_HS_USB_ON_MODEM_PROC=yes USES_BREW_3.1=yes USES_LINKER_XREFS=yes USES_HS_USB_PMIC_PHY=yes USES_XO=yes USES_NOR_PSRAM=yes USES_OSBL_SBI_PMIC_LIB=yes USES_AUDFMT_AMR=yes USES_IALARM=yes USES_CDMA=yes USES_NO_STRIP_NO_ODM=yes USES_RFMSM=yes USES_OEMSBL_SPLASH_SCREEN=yes USES_ROMBOOT=yes USES_CHARGER=yes USES_BREW_SMS=yes USES_GENERATE_MDSP=yes USES_LOWTIER_LOWMEM=yes USES_FSBL_SBI_PMIC_LIB=yes USES_PROCESS_ERROR_CAL=yes USES_MEM_USAGE_HTML=yes USES_OSBL_CHG_SCREEN=no USES_CLEAN_REMOVE=yes USES_AUTH=yes USES_BREW_BUIW=no USES_QHSUSB_DEAD_BATTERY_CHARGING=yes USES_SURF=yes USES_ERR_EXCLUDE_RESET_DETECT=yes USES_SPLIT_CODE_DATA=yes USES_REMOVE_WMS_RESOURCES=yes USES_HSU=yes USES_SNDCTL=yes USES_SBI=yes USES_AUDIOEQ=yes USES_UIM=yes  USES_NORPRG_HDR=yes USES_EFS2=yes USES_ZBLD=yes USES_HS_USB_CHG_REMOTE_API=yes USES_BOOT_SPLASH_SCREEN=yes USES_BUILD_FLASHTOOLS=yes USES_STRIP=yes USES_QSC1100_TARGET=yes USES_RF_QSC1100=yes USES_ARM_SBC_CODEC=yes USES_LOW=yes USES_QHSUSB=yes USES_SERVICES_ERR=yes  ASIC=q1100 BUILD=%PROJECTNAME% VERSION=3375 BUILDDIR=%PROJECTNAME% 2>&1 | ..\..\tools\build\qtee build%PROJECTNAME%.log

IF %ERRORLEVEL% == 0 goto SUCCESSMSG
IF NOT %ERRORLEVEL% == 0 goto FAILEDMSG
goto END

:FAILEDMSG
rem echo msgbox "���������������">msg.vbs
rem msg.vbs
rem del msg.vbs
msg %username% /TIME:60000 ���������������
goto END

:SUCCESSMSG
rem echo msgbox "����ɹ���">msg.vbs
rem msg.vbs
rem del msg.vbs
IF NOT "%*"=="" goto END
msg %username% /TIME:60000 ������ɣ�
goto END

:END

