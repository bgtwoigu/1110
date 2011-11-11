// InfoDialup.h: interface for the CInfoDialup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFODIALUP_H__D3EAC210_5415_45E4_9C2F_C52C5CD80F1B__INCLUDED_)
#define AFX_INFODIALUP_H__D3EAC210_5415_45E4_9C2F_C52C5CD80F1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GlobalDataCenter.h"
#include "InfoConnect.h"


struct TMTUSetting
{
	DWORD m_dwPPPMTU;
	DWORD m_dwVPNMTU;
public:
	TMTUSetting();
	bool Save()const;
	void Load();
	void ResetToDefault();
	bool operator==(const TMTUSetting& an)const;
	bool operator!=(const TMTUSetting& an)const
	{
		return !operator==(an);
	}
};

class CInfoOptions
{
public:
	CInfoOptions();
	
	bool SaveToReg();
	bool LoadFromReg();
	void ResetToDefault();

	CString m_strConnection;///ͨѶ��������(entry Name)
	CString m_strPhoneNum; 	///���ŵ绰����
	CString m_strUserName; 	///�û���
	CString m_strPassword; 	///����
	CString m_strApn;///�����
	CString m_initialAT;///modem��ʼ��AT
	
	CString m_strDevName; 	///�豸����(modem)
	CString m_strDNSMaster;	///��DNS
	CString m_strDNSSlave;	///��DNS
	CString m_strStaticIP;
	bool m_bDNSEnable;
	bool m_bUseStaticIP;
	bool	m_bEnableAutoConnection;//����ʱ�Զ�����
	bool	m_bAutoReconnect;//�Ͽ�ʱ�Զ���������

	CString m_ComPort;		///������


	CString m_strIncomeRing;//��������
	CString m_strSMSRing;	//��������
	CString m_strCallConnectRing;//�绰��ͨ����
	CString m_strCallDisconnectRing;//�绰�Ͽ�����


//	int m_TDelay;//T����ʱjiangguochao 20070119 deleted for P/T delaying
//	int m_PDelay;//T����ʱjiangguochao 20070119 deleted for P/T delaying

//	CString m_strLocalPhone;//��������
//	CString m_strLocalName;	//��������
	//CString m_strCscaPhone;//��������

	TMTUSetting m_MTUSetting; //MTU(Max Transmit Unit)����
};

#endif // !defined(AFX_INFODIALUP_H__D3EAC210_5415_45E4_9C2F_C52C5CD80F1B__INCLUDED_)
