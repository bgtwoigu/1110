#pragma once

enum EATReturnCode
{
	RETURN_OK = 1,		//��������Ӧ��OK
	RETURN_TIMEOUT,		//��ָ��ʱ��������Ӧ��û�з���
	RETURN_BUSY,		//ǰһ����������ִ�У���δ���
	RETURN_ERROR = 256,	//��������Ӧ��Error
	RETURN_ERROR_REPORT,//��������Ӧ��Error,�Ҵ��д�����,������ΪErrorCode,�������ֵΪRETURN_ERROR_REPORT+ErrorCode
};


class CMobilePacket
{
	int m_nReturn;	//��������������
	int m_nLen;		//��Ϣ����(�ֽ���)
	char m_szMessage[1024];//��Ϣ����
public: 
	CMobilePacket(const char* strCmd, size_t strSize);
	virtual ~CMobilePacket();

	virtual void SetReturnCode(int code);

	int ReturnCode()const{return m_nReturn;}

	int MessageSize()const{return m_nLen;}
	
	const char* Message()const{return m_szMessage;}
protected:
	CMobilePacket();
	void SetMessage(const char* strCmd, size_t strSize);
};

