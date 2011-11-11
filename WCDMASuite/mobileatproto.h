#ifndef AFX_MOBILEATPROTO_H__5C1DED7A_16CB_4916_8D36_D736B04B32EE__INCLUDED_
#define AFX_MOBILEATPROTO_H__5C1DED7A_16CB_4916_8D36_D736B04B32EE__INCLUDED_

#include <afxmt.h>
//#include "GLobalConst.h"
#include "CMobilePacket.h"
#include "CriticalSecMon.h"

class CCallManager;
class CMobileATProto  
{
public:
	enum
	{ 
		SOUND_TYPE_DIAL = 0, 
		SOUND_TYPE_DATA = 1, 
		SOUND_TYPE_FAX = 3, 
		SOUND_TYPE_SMS = 4 
	};
private:
	static CMobilePacket* GeneratePacket(const char* strCmd, size_t strSize);
public:
	///ģ����̿���
	static CMobilePacket* KeyInput(char cKey);

	///�õ���������
	static CMobilePacket* GetVolumn();

	///��ѯSIM���Ƿ����
	static CMobilePacket* CheckSimExist();
	
	///���û���
	static CMobilePacket* SetEcho(bool bOn);	// ok

	///����SIM������
	static CMobilePacket* TestSimLock();		// ok
	///����SIM������
	static CMobilePacket* SETSimLock(int mode, const CString& strPinCode);		// ok
	//����ME���󱨸�
	static CMobilePacket* SetCmee();

	//��������Ǽ�״̬����
	static CMobilePacket* SetCreg();

	//��ѯ����Ǽ�״̬
	static CMobilePacket* QueryCreg();

	//��ѯGPRS����
	static CMobilePacket*  QueryGprsAttach();

	//��ѯEDGE����
	static CMobilePacket*  QueryEDGEAttach();
	
	///�������� 
	static CMobilePacket* SetVolumn(int nVolumn);

	//
	static CMobilePacket* RegisterNet(const CString& strNetName);
	static CMobilePacket* QueryCurNet();
	static CMobilePacket* QueryAllNet();
	///��ѯ�����绰
//	static CMobilePacket* LocalPhoneNum();

	///���ñ����绰
//	static CMobilePacket* SetLocalPhone(const CString& strLocalPhone);

														//////////SMS//////////
	//���ö��Ÿ�ʽΪtext��ʽ
	static CMobilePacket* CMobileATProto::SetCMGF();			// ok set 1

	//�����ַ���
	static CMobilePacket* CMobileATProto::SetCharset();

	///��ѯ����Ϣ��������
	static CMobilePacket* CscaPhoneNum();

	static CMobilePacket* SMSSave(const CString& strPhoneNum, const CString& strContent);		// modify 

	static CMobilePacket* SMSRead(int nID);					// ok

	static CMobilePacket* SMSDelete(int nID);					// ok
	static CMobilePacket* SMSDeleteAll();						// ok

	static CMobilePacket* SMSBoxSelect(const CString& strType);// ok

	static CMobilePacket* SetSMSPrompt();					// ok

	static CMobilePacket* SetSMSPraCmd(int fmt);					// ok
														//////////SMS//////////

	///��ѯ�汾
	static CMobilePacket* LocalVersion();
	
	static CMobilePacket* CheckCPIN();

	static CMobilePacket* SetMode(int mode);		// add by lab686

	static CMobilePacket* QueryMode();		// add by lab686
	
	static CMobilePacket* CheckSignal();

	static CMobilePacket* CheckHDRSignal();						// ok lab686

	static CMobilePacket* CheckSYSINFO();						// ok lab686
	
	static CMobilePacket* QueryBaud();
	
	static CMobilePacket* QueryIMEI();

	static CMobilePacket* CheckMIC();

	static CMobilePacket* SetMICMute(bool bSet);
	
	static CMobilePacket* SoundPlay(int nType, int nVolumn, int nSound);

	static CMobilePacket* SetBaudrate(int iRate);

	static CMobilePacket* GetBaudrate();

	static CMobilePacket* SetDialShow(bool bShow);
	
	static CMobilePacket* SetPhoneBookEnable();
	
	static CMobilePacket* DialHangUp();

	static CMobilePacket* DialCallOut(const CString& strPhoneNum, CCallManager* pCallMgr);
	
	static CMobilePacket* DialAccept(CCallManager* pCallMgr);

	static CMobilePacket* SendDtmf(const CString& strKey, size_t callnumber);

	static CMobilePacket* SendDDSETEX(size_t comnumber);	// add by lab686 20091030

	static CMobilePacket* SetCVOICE(size_t comnumber);			// add by lab686 20091103 // OK
	static CMobilePacket* PhoneBookType();

	//static CMobilePacket* PhoneBookParam();

	static CMobilePacket* PhoneBookLoad(int nMinID, int nMaxID);

	static CMobilePacket* PhoneBookDelete(int nID);

	static CMobilePacket* PhoneBookSelect(const CString& strType);

	static CMobilePacket* PhoneBookModify(int nID, const CString& strPhoneNum, const CString& strName);

	static CMobilePacket* CheckRUIMSurport();

	static CMobilePacket* ReadAOLock();

	static CMobilePacket* SetAOLock(bool bLock);

	static CMobilePacket* ReadAILock();

	static CMobilePacket* SetAILock(bool bLock);

	static CMobilePacket* SelectRing(BYTE nMusicIndex, BYTE deviceNo);

	static CMobilePacket* InputPIN1(const CString& strPIN);

	static CMobilePacket* InputPUK1(const CString& strPUK, const CString& strNewPIN);

	static CMobilePacket* QueryLockTag();

	static CMobilePacket* SendFlash(const char* strNum = NULL);

	static CMobilePacket* SendPDUSMS(const CString& SMSCenter, const CString& strPhoneNum, const CString& strText, CMobilePacket*& cmd2);

	static CMobilePacket* CallHold(int code);
	static CMobilePacket* CallForwardSet(int reason, int mode, const char* strNum);
	static CMobilePacket* CallForwardCancel(int reason, int mode);
	static CMobilePacket* CallForwardQuery(int reason, int mode);
	static CMobilePacket* QueryCallEnd();

	static CMobilePacket* TestComATOK();

	static CMobilePacket* QuerySWRV();
	static CMobilePacket* QueryIDStatus();
	static CMobilePacket* QueryID();
	static CMobilePacket* QueryMN();
	static CMobilePacket* QueryState();

	static CMobilePacket* SetDevToUDisk();

	static CMobilePacket* QueryPPP();

	static CMobilePacket* SetPCMSWitch(int nMode);
	static CMobilePacket* SetApn(const CString& strApn);
	static CMobilePacket* RSPDevice();
};

/**
*	AT command ��.
*	����AT �����ͬ�����⡣�ڲ�ʹ�����ü���
*	�����ֶ�һ��AT��������׷�١�
*	�����ʵ����һ���������������Կ��Լ�
*	�ذ�ֵ(By value)����
*/
static CCriticalSection g_CSATCmd;
class CATCmd
{
public:
	//���캯��
	explicit CATCmd(CMobilePacket* pPacket = NULL)
	{
		InitializeCriticalSection(&m_CS);
		m_pObj = new TATPacket(pPacket);
	}

	//��������
	CATCmd(const CATCmd& atcmd)
	{
		InitializeCriticalSection(&m_CS);
		atcmd.m_pObj->AddRef();
		m_pObj = atcmd.m_pObj;
	}

	//operator=
	CATCmd& operator=(const CATCmd& atcmd)
	{
		//CCriticalMonitor lock(m_CS);
		g_CSATCmd.Lock();

		if(&atcmd != this)
		{
			m_pObj->Release();
			atcmd.m_pObj->AddRef();
			m_pObj = atcmd.m_pObj;
		}
		g_CSATCmd.Unlock();

		return *this;
	}

	//����
	~CATCmd()
	{
		m_pObj->Release();
		m_pObj = NULL;
		DeleteCriticalSection(&m_CS);
	}

	//�����ڲ���Packet����
	CMobilePacket* GetPacket()
	{
		return m_pObj->m_pPacket;
	}

	CMobilePacket* GetPacket()const//const �汾
	{
		return m_pObj->m_pPacket;
	}

	//�ȴ����������¼�
	DWORD WaitEvent(DWORD dwMillisec)
	{
		return ::WaitForSingleObject(m_pObj->m_Event, dwMillisec);
	}

	//�������������¼�
	void SetEvent()const
	{
		//CCriticalMonitor lock(m_CS);
		g_CSATCmd.Lock();
		m_pObj->m_Event.SetEvent();
		g_CSATCmd.Unlock();
	}

	//������������¼�
	void ResetEvent()
	{
		//CCriticalMonitor lock(m_CS);
		g_CSATCmd.Lock();
		m_pObj->m_Event.ResetEvent();
		g_CSATCmd.Unlock();
	}

	//�������������벢��������¼�
	void SetErrorCode(int code)const
	{
		//CCriticalMonitor lock(m_CS);
		g_CSATCmd.Lock();
		if(m_pObj->m_pPacket)
			m_pObj->m_pPacket->SetReturnCode(code);
		g_CSATCmd.Unlock();
	}
private:
	//�ڲ���Packet��װ��
	struct TATPacket
	{
		CEvent m_Event;	//��������¼�
		long   m_nRef;	//���ü���

		CMobilePacket* m_pPacket;//Packet���󣬱������ù�new������������,����ΪNULL
	public:
		//����
		TATPacket(CMobilePacket* pPacket = NULL)
			:m_Event(FALSE, TRUE),m_nRef(1),m_pPacket(pPacket)
		{
		}

		//����Ӧ�ü���
		void AddRef()
		{
			::InterlockedIncrement(&m_nRef);
		}

		//����Ӧ�ü���
		void Release()
		{
			::InterlockedDecrement(&m_nRef);
			if(! m_nRef)
			{
				delete this;
			}
		}
	private://��ֹ����
		TATPacket(const TATPacket&);
		TATPacket& operator=(const TATPacket&);
	private:
		~TATPacket()
		{
			delete m_pPacket;//�ͷ�packet����
			m_pPacket = NULL;
		}
	};

private:
	TATPacket* m_pObj;//�ڲ���Packet��װ�����ָ�룬��ָ��ʼ����Ч
	mutable CRITICAL_SECTION m_CS;
};

#endif
