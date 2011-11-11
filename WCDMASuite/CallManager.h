// CallManager.h: interface for the CCallManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALLMANAGER_H__7E9C04C0_69FD_47D7_8CC1_70903529DA20__INCLUDED_)
#define AFX_CALLMANAGER_H__7E9C04C0_69FD_47D7_8CC1_70903529DA20__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordManager.h"
#include "mobileatproto.h"
#include "mobilemanager.h"
#include <afxmt.h>
#include "data\CallRecord.h"


//����״̬
enum CallStatus
{
	CALLSTATE_IDLE,			//����״̬
	CALLSTATE_INCOMING,		//��������״̬
	CALLSTATE_IN_TALKING,	//�������״̬
	CALLSTATE_OUT_TALKING,	//��������״̬
	CALLSTATE_OUTGONING,	//���ں���״̬
};

//����״̬�����ӿ�
struct ICallStatusListener
{
	virtual void OnCallEnd(CCallManager* pSender) = 0;
	virtual void OnCallComing(CCallManager* pSender) = 0;
	virtual void OnCallGoing(CCallManager* pSender) = 0;
	virtual void OnTalking(CCallManager* pSender) = 0;
	virtual void OnCallForward(const CString& strForwardNum) = 0;
	virtual void OnFlashCallConnected() = 0;	
	virtual void OnCallWaiting(const CString& strNum) = 0;	
};

class CMobileManager;
class CCallManager  
{
public:	
	CMobileManager* m_pMgr;
	ICallStatusListener* m_pListener;
	//����
	void Call(const CString& strNumber);
	//�Ҷ�
	void EndCall(bool selfHangup = false);
	//�����绰
	void AcceptIncomingCall();
	void OnCallWating(const CString& strNum);
	void SetListener(ICallStatusListener* pListener);
	//���β���
	void SendDTMF(const CString& str);
	const CString& GetDTMFString()const;
	//���ú��е�״̬
	void SetCallStatus(CallStatus status);
	//��ȡ���е�״̬
	CallStatus GetCallStatus(void);
	//���м�ʱ
	void CallTime(void);
	void SetMobileManager(CMobileManager* pMgr);
	void OnCallEnd(int nReason, bool selfHangup = false);		//��Ӧ+CCEND
	void OnCallContinue();
	void OnOutgoingCall();	//��Ӧ+ZCORG
	void OnIncomingCall(const CString& strPhone);	//��Ӧ+CLIP
	//���ú��е�״̬
	CallStatus  m_setCallStatus;
	CString		m_strDTMF;		//���κ����绰��DTMF��
//	CMobileATProto* m_mobileAT;
	CCallManager();
	virtual ~CCallManager();

	const TCallInfo& GetCallInfo()const;
	void RecordCallInfo(TCallInfo* callInfo);

	DWORD GetCurCallTime()const;

	int m_callID;				// add by lab686 2009/10/29  used annal the  uniquely number of the calling value between 1 to 6

	void SetVoiceCom();	// add by lab686 20091030 	set AT^DDSETEX=2

private:
	TCallInfo	m_CallInfo;		//��ǰ������Ϣ

	BOOL	m_bRing;		//��������һ��
	BOOL	m_bIncoming;	//����������ʾһ��
	bool    flagSelfHangup;

};

#endif // !defined(AFX_CALLMANAGER_H__7E9C04C0_69FD_47D7_8CC1_70903529DA20__INCLUDED_)
