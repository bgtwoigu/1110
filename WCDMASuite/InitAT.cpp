// InitAT.cpp: implementation of the CInitAT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sunnycat.h"
#include "InitAT.h"
#include "Porting.h"
#include "SunnyCatDlg.h"
#include "devenum.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInitAT::CInitAT(CMobileManager& mgr)
:m_Mgr(mgr),m_bIsResetCom(false),m_bRunning(false)
{

}

CInitAT::~CInitAT()
{

}

void CInitAT::Go()
{
	TRACE(TEXT(" CInitAT::Go start \n"));
	if(m_bRunning)
		return;
	m_bRunning = true;
	m_Mgr.PostMessageToClient(WM_INIT_START);
	SetWorkMode(m_Mgr,USETYPE_USBMODEM,true);
	bool bOK = m_Mgr.Init();
	if(!bOK)
	{
		m_Mgr.ShutDown();
		CString strModemN;
		strModemN.Format(TEXT(""));

//		CString strModemName;
		
//		strModemName = GetModemName();
		
		if(m_Mgr.IsModemExisting(strModemN)&&m_Mgr.m_bSimExist)					
		{
			m_bIsResetCom = true;
			m_Mgr.m_pOwner->m_bIsResetCom = true;
		}
		else
		{
			m_bIsResetCom = false;
			m_Mgr.m_pOwner->m_bIsResetCom = false;
		}
		
		m_Mgr.m_SerialPort.StopMonitoring();
		if(m_bIsResetCom)
		{
			m_Mgr.m_pOwner->ResetCom(TEXT("4D36E96D-E325-11CE-BFC1-08002BE10318"), m_Mgr.GetPortDevName());
			::Sleep(2000);
		}
		
		if(m_Mgr.IsModemExisting(strModemN))
		{
			m_bIsResetCom = false;
			m_Mgr.m_pOwner->m_bIsResetCom = false;
		}
		if(m_bIsResetCom)
			bOK = m_Mgr.Init();
			
		
		if(!bOK)
		{
			m_Mgr.isAbleCom = false;

			bool bDeviceExit = false;
	
			bDeviceExit = m_Mgr.IsModemExisting(strModemN);
			
			if(!bDeviceExit)
			{
				m_bIsResetCom = false;
			}
			m_Mgr.PostMessageToClient(WM_INIT_ERROR,1,0);
			m_Mgr.ShutDown();	//20070514
		}
		else
		{
			m_bIsResetCom = false;
			m_Mgr.m_pOwner->m_bIsResetCom = false;
		}
	}
	
	
	if(bOK)
	{
		m_Mgr.isAbleCom = true;
		CATCmd cmd;
	
		cmd = m_Mgr.SendATPacket(CMobileATProto::CheckCPIN());
		m_Mgr.WaitForATCompleted(cmd,1000);
		m_Mgr.m_strPINResponse.MakeLower();
		
		cmd = m_Mgr.SendATPacket(CMobileATProto::QueryCurNet());//��ѯ��ǰ����
				m_Mgr.WaitForATCompleted(cmd,1000); 					
		
		if(m_Mgr.m_strPINResponse == "ready")
		{
			m_Mgr.m_simLocked = false;
		}
		else if ((m_Mgr.m_strPINResponse == "sim pin")||
				(m_Mgr.m_strPINResponse == "sim puk"))
		{			
			m_Mgr.m_simLocked = true;
			m_Mgr.PostMessageToClient(WM_SIGNAL_SIMLOCK, 0, 0);
			m_Mgr.PostMessageToClient(WM_INIT_END);
			return;
		}
		else if (m_Mgr.m_strPINResponse == "sim busy");
		else
		{
			m_Mgr.PostMessageToClient(WM_INIT_ERROR,1,0);
			m_Mgr.ShutDown();
			m_Mgr.isAbleCom = false;
			
			m_bRunning = false;
			return;
		}
//		cmd = m_Mgr.SendATPacket(CMobileATProto::TestSimLock());//�ر�Echo // ok
//		m_Mgr.WaitForATCompleted(cmd,1000);

		cmd = m_Mgr.SendATPacket(CMobileATProto::SetEcho(false));//�ر�Echo // ok
		m_Mgr.WaitForATCompleted(cmd,1000);

		//����CMEE
		cmd = m_Mgr.SendATPacket(CMobileATProto::SetCmee());				// OK LAB686
		m_Mgr.WaitForATCompleted(cmd,1000);
		
		cmd = m_Mgr.SendATPacket(CMobileATProto::QueryIMEI());				// ok lab686
		m_Mgr.WaitForATCompleted(cmd,1000);
		
//		cmd = m_Mgr.SendATPacket(CMobileATProto::SetCreg());//ʹ������Ǽ�״̬����
//		m_Mgr.WaitForATCompleted(cmd,1000);		

		/************************SMS*********************************/
		// ������SMS ��ʾ��Ϣ 
 		cmd = m_Mgr.SendATPacket(CMobileATProto::SetSMSPrompt());			// ok
		m_Mgr.WaitForATCompleted(cmd,1000);

		// ����SMS �洢λ��
		cmd = m_Mgr.SendATPacket(CMobileATProto::SMSBoxSelect("SM"));			// OK 
		m_Mgr.WaitForATCompleted(cmd,1000);

		// ����SMS ��ʽ
		cmd = m_Mgr.SendATPacket(CMobileATProto::SetCMGF());//���ö��Ÿ�ʽ  // ok
		m_Mgr.WaitForATCompleted(cmd,1000);	

		// ��������ģʽΪ���ģʽ
//		cmd = m_Mgr.SendATPacket(CMobileATProto::SetMode(8));//���ö��Ÿ�ʽ  // ok
//		m_Mgr.WaitForATCompleted(cmd,1000);	
		/************************************************************/

		/************************VOICE**********************************/
//		int nPCMVoice = 2;
//		cmd = m_Mgr.SendATPacket(CMobileATProto::SendDDSETEX(nPCMVoice));	// OK // CMobileATProto::SetPCMSWitch(nPCMVoice));
//		m_Mgr.WaitForATCompleted(cmd,1000);

//		cmd = m_Mgr.SendATPacket(CMobileATProto::SetCVOICE(0));			// OK  ADD BY LAB686 20091103
//		m_Mgr.WaitForATCompleted(cmd,1000);
		
		//����������ʾ
		cmd = m_Mgr.SendATPacket(CMobileATProto::SetDialShow(true));			// û����������ʽ���ȼ���
		m_Mgr.WaitForATCompleted(cmd,1000);	

		cmd = m_Mgr.SendATPacket(CMobileATProto::PhoneBookSelect("SM"));
		m_Mgr.WaitForATCompleted(cmd,1000);
		/************************************************************/
		
		cmd = m_Mgr.SendATPacket(CMobileATProto::CheckSignal());//��ѯ�ź�ǿ�� // ok
		m_Mgr.WaitForATCompleted(cmd,1000);								

//		cmd = m_Mgr.SendATPacket(CMobileATProto::CheckHDRSignal());//��ѯ�ź�ǿ�� // ok
//		m_Mgr.WaitForATCompleted(cmd,1000);	

		DWORD dwTime = GetTickCount();
		while(m_Mgr.m_nSignal <=5 )
		{
			cmd = m_Mgr.SendATPacket(CMobileATProto::CheckSignal());//��ѯ�ź�ǿ��// ok
			m_Mgr.WaitForATCompleted(cmd,1000);
//			cmd = m_Mgr.SendATPacket(CMobileATProto::CheckHDRSignal());//��ѯ�ź�ǿ��// ok
//			m_Mgr.WaitForATCompleted(cmd,1000);
			if((GetTickCount()-dwTime)>15000)
			{
				break;                   
			}
			::Sleep(1000);
		}		
		cmd = m_Mgr.SendATPacket(CMobileATProto::SetCharset());//�����ַ���
		m_Mgr.WaitForATCompleted(cmd,1000);	
		
		cmd = m_Mgr.SendATPacket(CMobileATProto::CscaPhoneNum());//�������ĺ���
		m_Mgr.WaitForATCompleted(cmd,1000);	
		cmd = m_Mgr.SendATPacket(CMobileATProto::CheckSignal());//��ѯ�ź�ǿ��
		m_Mgr.WaitForATCompleted(cmd,1000);
//		cmd = m_Mgr.SendATPacket(CMobileATProto::CheckHDRSignal());//��ѯ�ź�ǿ��
//		m_Mgr.WaitForATCompleted(cmd,1000);

		cmd = m_Mgr.SendATPacket(CMobileATProto::TestSimLock());//����SIM����// ok
		m_Mgr.WaitForATCompleted(cmd,1000);		
		
	}
	m_Mgr.PostMessageToClient(WM_INIT_END);

	m_bRunning = false;
	TRACE(TEXT(" CInitAT::Go end \n"));
}
