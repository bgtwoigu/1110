#include "stdafx.h"
#include "COMMonitor.h"
#include "MobileManager.h"
#include "Thread.h"
#include "ATCmdQ.h"
#include "CHangupCmdPacket.h"
#include "CallCmdPacket.h"
#include "AcceptCmdPacket.h"
#include "SetBaudCmdPacket.h"
#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void LogString(const char* str, size_t size);

//*******************************************
//	���ڽ����߳�
//*******************************************
class CCOMRecvThread: public CThread
{
	CEvent&			m_EventExit;
	SERIAL_PORT&	m_SerialPort;
	CMobileManager& m_Mgr;
	CEvent			m_eventRead;
	OVERLAPPED		m_overlapped;
	char			m_buffer[102400];

public:
	CCOMRecvThread(CEvent& eventExit, SERIAL_PORT& port, CMobileManager& mgr)
		:m_EventExit(eventExit), m_SerialPort(port), m_Mgr(mgr),
		m_eventRead(false, true)
	{
		ZeroMemory(&m_overlapped, sizeof(OVERLAPPED));
		m_overlapped.hEvent = m_eventRead;
	}

protected:
	virtual void Go()
	{
		TRACE(TEXT("COM RECV THREAD IS RUNNING.\n"));

		try
		{
			while(ReadFromCOM());
		}
		catch(CSerialException& e)
		{
			LogString(e.what(), 0);
			TRACE(TEXT("%s\n"),e.what());
			AfxGetApp()->m_pMainWnd->PostMessage(WM_SERIALPORT_ERROR, 0, 0);
		}
		TRACE(TEXT("COM RECV THREAD STOPPED.\n"));
	}
private:
	bool ReadFromCOM()
	{
		m_eventRead.ResetEvent();
		DWORD dwErrors;
		m_SerialPort.ClearError(dwErrors);
		if( m_SerialPort.Read(m_buffer, sizeof(m_buffer), m_overlapped) )
		{
			return false;
		}
		HANDLE hEvent[2] = 
		{
			m_EventExit,
			m_eventRead
		};

		while(true)
		{
			DWORD dwWait = WaitForMultipleObjects(2,hEvent, false, 3000);
			switch(dwWait)
			{
			case WAIT_OBJECT_0: //�˳��߳�
				return false;
			case WAIT_OBJECT_0+1://IO���
				{
				DWORD dwBytesRead = 0;
				m_SerialPort.GetOverlappedResult(m_overlapped, dwBytesRead, FALSE);
				m_buffer[dwBytesRead] = 0;
				OnRecvData(m_buffer, dwBytesRead);
				//m_SerialPort.Purge(PURGE_RXABORT|PURGE_RXCLEAR);
				return true;
				}
			case WAIT_TIMEOUT:
				{
					TRACE(TEXT("TimeOut..............\n"));
					break;
				}
			case WAIT_FAILED:
				{
					TRACE(TEXT("COM Error...............\n"));
					break;
				}
			} 
		}

		return false;
	}



	void OnRecvData(const char* buf, size_t size)
	{
		if(size)
		{
			
			TRACE(TEXT("Read:%s\r\n"), ToUnicode(buf).c_str());//20070605 ���رն��绰��ʱ�����
			LogString(buf, size);

			for(DWORD i=0;i<size; ++i)
				m_Mgr.OnCOMByteRecv(buf[i]);
		}
	}
};

//*******************************************
//	���ڷ����߳�
//*******************************************
class CCOMSendThread: public CThread
{
	CEvent&			m_EventExit;
	SERIAL_PORT&	m_SerialPort;
	CATCmdQ			m_CmdQ;
	CATCmd			m_CurCmd;		//��ǰAT����
	bool			m_bQuerySignal;
	CEvent			m_eventWrite;
	OVERLAPPED		m_overlapped;
public:
	CCOMSendThread(CEvent& eventExit, SERIAL_PORT& port)
		:m_EventExit(eventExit), m_SerialPort(port), 
		m_bQuerySignal(false),
		m_eventWrite(false, true)
	{
		ZeroMemory(&m_overlapped, sizeof(OVERLAPPED));
		m_overlapped.hEvent = m_eventWrite;
	}

	void PustATCmd(const CATCmd& cmd)
	{
		if(m_CurCmd.WaitEvent(0) == WAIT_TIMEOUT)
		{
			CHangupCmdPacket* hangup = dynamic_cast<CHangupCmdPacket*>(cmd.GetPacket());
			CAcceptCmdPacket* accept = dynamic_cast<CAcceptCmdPacket*>(cmd.GetPacket());
			CSetBaudCmdpacket *SetBaud = dynamic_cast<CSetBaudCmdpacket*>(cmd.GetPacket());
			if(!hangup && !accept &&!SetBaud)
			{
				cmd.SetErrorCode(RETURN_BUSY);
				
				cmd.SetEvent();

			}
			else if(hangup)
			{
				HandleHangupCmd(cmd);
			}
			else if(accept)
			{
				m_CmdQ.AddBack(cmd);
			}
			else if(SetBaud)
			{
				m_CmdQ.AddBack(cmd);
			}
		}	
		else
		{
			m_CmdQ.AddBack(cmd);
		}
	}

	void HandleHangupCmd(const CATCmd& cmd)
	{
		CCallCmdPacket* pCallCmd = dynamic_cast<CCallCmdPacket*>(m_CurCmd.GetPacket());
		if(pCallCmd)//����Ǻ��������ֱ��ȡ������
		{
			SetReturnCode(RETURN_ERROR);

			m_CmdQ.AddBack(cmd);
		}
		else
		{
		//	m_CmdQ.AddBack(cmd);
			cmd.SetErrorCode(RETURN_BUSY);
			cmd.SetEvent();
		}
	}

	void SetReturnCode(int errorCode)
	{
		m_CurCmd.SetErrorCode(errorCode);
		m_CurCmd.SetEvent();
	}

	void QuerySignal()
	{
		if(!m_CmdQ.Size())//��ѯ�ź��������ȼ����
		{
			m_bQuerySignal = false;
			PustATCmd(CATCmd(CMobileATProto::CheckSignal()));
//			PustATCmd(CATCmd(CMobileATProto::CheckHDRSignal()));
//			PustATCmd(CATCmd(CMobileATProto::QueryCreg()));
//			PustATCmd(CATCmd(CMobileATProto::QueryGprsAttach()));
//			PustATCmd(CATCmd(CMobileATProto::QueryEDGEAttach()));
		}
		else
			m_bQuerySignal = true;
	}
protected:
	virtual void Go()
	{
		TRACE(TEXT("COM SEND THREAD IS RUNNING.\n"));
		try
		{
			m_CurCmd.SetEvent();
			while(SendToCom());
		}
		catch(CSerialException& e)
		{
			LogString(e.what(), 0);
			TRACE(TEXT("%s\n"),e.what());
			AfxGetApp()->m_pMainWnd->PostMessage(WM_SERIALPORT_ERROR, 0, 0);
		}
		TRACE(TEXT("COM SEND THREAD STOPPED.\n"));
		m_CmdQ.DeleteAll();
	}
private:
	bool SendToCom()
	{
		HANDLE hEvent[2] = 
		{
			m_EventExit,
			m_CmdQ.EventNotEmpty()
		};	

		DWORD dwWait = WaitForMultipleObjects(2,hEvent, false, 30);
		switch(dwWait)
		{
		case WAIT_OBJECT_0:	//�˳��߳�
			return false;
		case WAIT_OBJECT_0+1://������зǿ�
			return SendCmd();
			break;
		}

		return true;
	}

	bool SendCmd()
	{
		//�ȴ���ǰ�������
		DWORD dwTick = GetTickCount();
		while(GetTickCount()-dwTick <20000)//20�볬ʱ
		{
			if(WaitForSingleObject(m_EventExit, 0) == WAIT_OBJECT_0)//�˳�
				return false;
			if(m_CurCmd.WaitEvent(30) == WAIT_OBJECT_0)
			{
				break;
			}
		}

		if(m_CmdQ.PopTopRecord(m_CurCmd ))
		{
			SendData();
			if(!m_CmdQ.Size() && m_bQuerySignal)//��ѯ�ź��������ȼ����
			{
				m_bQuerySignal = false;
				PustATCmd(CATCmd(CMobileATProto::CheckSignal()));
//				PustATCmd(CATCmd(CMobileATProto::CheckHDRSignal()));
//				PustATCmd(CATCmd(CMobileATProto::QueryCreg()));
//				PustATCmd(CATCmd(CMobileATProto::QueryGprsAttach()));
//				PustATCmd(CATCmd(CMobileATProto::QueryEDGEAttach()));
			}
		}
		return true;
	}

	void SendData()
	{
		const CMobilePacket* packet = m_CurCmd.GetPacket();

		LogString(packet->Message(), packet->MessageSize());

		TRACE(TEXT("\nSEND:%s   :%d bytes\n"),ToUnicode(packet->Message()).c_str(), packet->MessageSize());

		m_eventWrite.ResetEvent();

		TRACE(TEXT("unsigned char5"));
		m_SerialPort.Write(packet->Message(), packet->MessageSize(), m_overlapped);

		
	}
};

//*******************************************
//	���ڼ�����
//*******************************************
CCOMMonitor::CCOMMonitor(CMobileManager* pPortOwner)
:m_EventExitThead(FALSE, TRUE),
 m_commonitorstate(FALSE)
{
	m_pSendThread = new CCOMSendThread(m_EventExitThead, m_COM);
	m_pRecvThread = new CCOMRecvThread(m_EventExitThead, m_COM, *pPortOwner);
}

CCOMMonitor::~CCOMMonitor()
{
	StopMonitoring();
	delete m_pSendThread;
	delete m_pRecvThread;
}

bool CCOMMonitor::StartMonitoring(const CString& COMPort, UINT baud)
{
	if(IsThreadActive())return false;
	if(!InitSerial(COMPort, baud)) return false;

	m_commonitorstate = TRUE;
	StartThreads();
	return true;
}


bool CCOMMonitor::InitSerial(const CString& COMPort, UINT baud)
{
	if(m_COM.IsOpen())
	{
		m_COM.Close();
		Sleep(2000);
	}
	try
	{
		CString sCOMName;
		if(COMPort.Find(TEXT("\\\\")) == -1)
			sCOMName.Format(TEXT("\\\\.\\%s"), COMPort);
		else
			sCOMName = COMPort;
		std::string str = ToMultiBytes(LPCTSTR(sCOMName));
		bool b_comOK = m_COM.Open(
			str.c_str(),				//COM
			baud,					//������
			CSerialPort::NoParity,	//У��λ
			8,						//����λ
			CSerialPort::OneStopBit,//ֹͣλ 
			CSerialPort::NoFlowControl,//������ 
			TRUE);					//�ص�IO
		if(!b_comOK)
		{
			return false;
		}
		COMMTIMEOUTS cto = { 1, 30, 1, 0, 0 };//����С��20ms,�������ղ�����
		m_COM.SetTimeouts(cto);

		m_COM.Setup(8192,8192);
		m_COM.SetDTR();
	//	m_COM.SetRTS();
	//	m_COM.SetMask(EV_RLSD | EV_ERR);
		
		return true;
	}
	catch(CSerialException& exception)
	{
		LogString(exception.what(), 0 );
		TRACE1("%s\n",exception.what());
		return false;
	}
}

void CCOMMonitor::StartThreads()
{
	m_EventExitThead.ResetEvent();
	m_pSendThread->Start();
	m_pRecvThread->Start();

	m_pRecvThread->WaitForComplete(1000);
	m_pSendThread->WaitForComplete(100);
	
}


void CCOMMonitor::StopMonitoring()
{
	m_EventExitThead.SetEvent();
	m_pSendThread->WaitForComplete();//20070517
	m_pRecvThread->WaitForComplete();//20070517
	m_COM.Close();
	m_commonitorstate = false;
}

bool CCOMMonitor::IsThreadActive()
{
	return m_pSendThread->IsRunning() && m_pRecvThread->IsRunning();
}

void CCOMMonitor::PushCmd(CATCmd& at)
{
	m_pSendThread->PustATCmd(at);
}

void CCOMMonitor::SetReturnCode(int errorCode)
{
	m_pSendThread->SetReturnCode(errorCode);
}

void CCOMMonitor::QuerySignal()
{
	m_pSendThread->QuerySignal();
}


