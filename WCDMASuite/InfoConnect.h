#pragma once 

class CInfoConnect
{
public:
	CInfoConnect();

	CTime m_tmConnectBeginTime;
	unsigned int m_iConnectTotalSecond;
	///�ܽ���
	DWORD m_dwRecvTotal;
	///�ܷ���
	DWORD m_dwSendTotal;
	///ÿ�����
	DWORD m_dwRecvPS;
	///ÿ�뷢��
	DWORD m_dwSendPS;
};