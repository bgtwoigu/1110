#ifndef CRITICAL_SECTION_MONITOR_
#define CRITICAL_SECTION_MONITOR_


#include <afxmt.h>

/**
*	�򵥵��ٽ���������.
*	���ö������������ʵ���Զ��Ľ����ٽ���
*/
class CCriticalMonitor
{
public:
	CCriticalMonitor(CRITICAL_SECTION& obj)
		:m_obj(obj)
	{
		EnterCriticalSection(&m_obj);
	}

	CCriticalMonitor(CCriticalSection& obj)
		:m_obj(obj.m_sect)
	{
		EnterCriticalSection(&m_obj);
	}

	~CCriticalMonitor()
	{
		LeaveCriticalSection(&m_obj);
	}
private://��ֹ���������operator=
	CCriticalMonitor(const CCriticalMonitor&);
	CCriticalMonitor& operator=(const CCriticalMonitor&);
private:
	CRITICAL_SECTION& m_obj;
};

#endif