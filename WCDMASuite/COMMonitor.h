#ifndef AFX_COMMONITOR_H__8BD28A9B_01E8_4A44_9E02_550987167D91__INCLUDED_
#define AFX_COMMONITOR_H__8BD28A9B_01E8_4A44_9E02_550987167D91__INCLUDED_

#include "MobileATProto.h"
#include "SerialPort.h"

//#define USE_SIMULATOR


#define SERIAL_PORT CSerialPort


class CMobileManager;
class CCOMRecvThread;
class CCOMSendThread;
class CCOMMonitor  
{
public:
	CCOMMonitor(CMobileManager* pPortOwner);
	~CCOMMonitor();

	//����COM�ڷ���/�����߳�
	bool StartMonitoring(const CString& COMPort, UINT baud);

	//ֹͣCOM�ڷ���/�����߳�
	void StopMonitoring();

	//�ж�COM�ڷ���/�����߳��Ƿ�����
	bool IsThreadActive();

	//���õ�ǰ�����͵�AT����
	void PushCmd(CATCmd& at);

	//���õ�ǰAT����ķ�����
	void SetReturnCode(int errorCode);

	//��ѯ�ź�ǿ���������⴦��
	void QuerySignal();

	//����ʱҪ��ն���
	void ClearCmdQ();

	bool m_commonitorstate;			// add by lab686	2009/10/28  17:39:00
private:
	bool InitSerial(const CString& COMPort, UINT baud);
	void StartThreads();
private:
	
	CEvent m_EventExitThead;//�߳��˳��¼�
	SERIAL_PORT		m_COM;	//���ڶ���

private:
	CCOMSendThread* m_pSendThread;
	CCOMRecvThread* m_pRecvThread;
};

#endif
