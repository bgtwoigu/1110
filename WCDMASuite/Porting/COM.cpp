#include "StdAfx.h"
#include "../Porting.h"
#include "../RegistryEx.h"
#inlcude "../devenum.h"

#ifdef COM_MODEM

//��ע����е�����·��

PORTING_API
const CString& GetConfigString()
{
	static CString strConfig = "Software\\WCDMA Modem\\WCDMA";

	return strConfig;
}

//Modem����
PORTING_API
const CString& GetModemName()
{
	CModemList modemList;

	static CString strModem;
	std::vector<size_t> vMatched;
	for(size_t i=0; i<modemList.Count(); ++i)
	{
		const TModemInfo& info = modemList.GetAt(i);
		CString strID = info.strHardwareID;
		strID.MakeLower();
		if( strID.Find(TEXT("vid_05c6&pid_9003&rev_0000&mi_03")) >=0 )
		{
			 strModem = info.strFriendlyName;
			 TRACE(TEXT("GetModemName strModem = %s\n"), strModem);		 
			 return strModem;
		}
	}	

	static CString strModem;
	strModem.Format(TEXT("SinCom Mobile"));
	return strModem;
}

PORTING_API
const CString& GetModemComName()
{
	CModemList modemList;
		
	std::vector<size_t> vMatched;
	for(size_t i=0; i<modemList.Count(); ++i)
	{
		const TModemInfo& info = modemList.GetAt(i);
		CString strID = info.strHardwareID;
		strID.MakeLower();
		if( strID.Find(TEXT("vid_05c6&pid_9003&rev_0000&mi_03")) >=0 )
		{
			 TRACE(TEXT("GetModemComName strPortAttached = %s\n"), info.strPortAttached);
			 return info.strPortAttached;
		}
	}
	// ������!
	static CString strModem = "COM13";
	return strModem;
}

//������
PORTING_API
const CString& GetConnectionName()
{
	static CString strConnection = "WCDMA Wireless Connection"; //��������(entry Name)
	return strConnection;
}

//Ӧ��ID��������֤ͬһ��Ӧ�ò�����������
PORTING_API
const CString& GetAppID()
{
	static CString strID = "{51EA1C23-BE10-406A-9CF2-B7CDEAB1FB39}";

	return strID;
}


//Ӧ��������ʾ�ڱ�����
PORTING_API
const CString& GetAppName()
{
	static CString str = "WCDMA Modem";

	return str;
}


//modem��ʼ��AT����·��
PORTING_API
const CString& GetModemConfigString()
{
	static CString strConfig = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96D-E325-11CE-BFC1-08002BE10318}";
	return strConfig;
}

PORTING_API 
const CString& GetHardwareID(int i)
{
	static CString strHWID = "Rev_0000";
	static CString strHWIDNULL="";

	if(i)return strHWIDNULL;

	return strHWID;
}

//�����ļ�����ʹ����LockCode
PORTING_API
const CString& GetEncryptFile()
{
	static CString str = "";

	return str;
}

#endif//COM_MODEM
