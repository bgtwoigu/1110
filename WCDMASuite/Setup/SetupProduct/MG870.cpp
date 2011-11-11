#include "stdafx.h"
#include "Execute.h"

HANDLE RunProgram(const CString& strCmdLine, bool bShowWindow);
CString GetAppPath();
bool InstallUSB_98(const CString& strDestDir);
bool UninstallUSB_98(const CString& strDestDir);
bool InstallUSB_2KXP(const CString& strDestDir);
bool UninstallUSB_2KXP(const CString& strDestDir);

bool CopyResourceDir(const CString& strLanguage, const CString& strDestDir);
bool RemoveResourceDir(const CString& strDestDir);

int ShowMessageBox(UINT strid, UINT nType = MB_OK);

//Win98 �İ�װ
/*
������谲װ�ű��Ѿ���destDirĿ¼��������ȷ��Ŀ¼�ṹ
*/

bool InstallMG870_98(const CString& strLanguage, const CString& strDestDir)
{
	CopyResourceDir(strLanguage, strDestDir);

	return InstallUSB_98(strDestDir);
}

//Win98 ��ж��
bool UninstallMG870_98(const CString& strDestDir)
{
	RemoveResourceDir(strDestDir);

	return UninstallUSB_98(strDestDir);
}


//Win2000/XP �İ�װ
bool InstallMG870_2KXP(const CString& strLanguage, const CString& strDestDir)
{
	CopyResourceDir(strLanguage, strDestDir);

	InstallUSB_2KXP(strDestDir);

	return true;
}


//Win2000/XP ��ж��
bool UninstallMG870_2KXP(const CString& strDestDir)
{
	RemoveResourceDir(strDestDir);

	return UninstallUSB_2KXP(strDestDir);
}
