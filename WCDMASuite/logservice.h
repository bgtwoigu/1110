#if !defined(AFX_LOGSERVICE_H__10B64A0B_2E01_4441_B6C7_58B5BC29AFE1__INCLUDED_)
#define AFX_LOGSERVICE_H__10B64A0B_2E01_4441_B6C7_58B5BC29AFE1__INCLUDED_

#include <afxmt.h>
#include <fstream>

/** Log �����ࡣ
*	�����ṩ������log�������
*	Ӧ��Singletonģʽ��ֻ��ȫ��Ψһʵ��
*/
class CLogService  
{
	CLogService();
	CLogService(const CLogService&);
	CLogService operator=(const CLogService&);
public:
	static CLogService& Instance();
	~CLogService();
	void Enable(bool bEnabled);
	bool Enabled()const{return m_bEnable;}
	void LogString(const char* str, size_t size = 0);
	void SetFileName(const char*);
private:
	CCriticalSection m_CS;
	std::ofstream	m_LogStream;
	bool			m_bEnable;
};

#endif // !defined(AFX_LOGSERVICE_H__10B64A0B_2E01_4441_B6C7_58B5BC29AFE1__INCLUDED_)
