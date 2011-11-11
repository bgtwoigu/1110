#include "stdafx.h"
#include "Execute.h"
#include "Thread.h"
#include "PCMCIA-Helper.h"
#include "../../DevEnum.h"
#include "SelComDlg.h"
#include "ExecuteDlg.h"

HANDLE RunProgram(const CString& strCmdLine, bool bShowWindow);
CString GetAppPath();
int ShowMessageBox(UINT strid, UINT nType = MB_OK);


/** 2000/XP�µİ�װ
*/

bool InstallCOM_2KXP(const CString& strDestDir)
{
	//UninstallUSB_2KXP(strDestDir);//����ɾ�����е�Driver
		
	CString strCmd;
	strCmd = strDestDir + "MdmInst.exe " + g_strCom;//dlg.m_strCom;
	HANDLE hProcess = RunProgram(strCmd, false);
	if(!hProcess)return false;

	WaitForSingleObject(hProcess, INFINITE);

	

	return true;
}

/** 2000/XP�µ�ж��
*/
bool UninstallCOM_2KXP(const CString& strDestDir)
{
	HANDLE hProcess = RunProgram(strDestDir+"FastRemove2KXP.exe -c", false);
	if(!hProcess)return false;

	WaitForSingleObject(hProcess, INFINITE);

	return true;
}