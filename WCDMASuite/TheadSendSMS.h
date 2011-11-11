#pragma once

#include "Util.h"
#include "DlgProgressEx.h"
#include "GlobalDataCenter.h"

#include <vector>

using std::vector;
class CMobileManager;
//�����߳���
/***************************************
 *^HCMGS=<da>[,<toda>](CR)text is entered<ctrl-Z/ESC>
 * ^HCMGW �����ź��룬0��  �洢״̬�����ȼ�����������/0�������ʽ/1������/1��
/***************************************/
class CThreadSendSMS:public CPrgressThread
{
	CMobileManager&			m_Mgr;
	const vector<CString>&	m_Numbers;
	const vector<CString>&	m_Contents;
	int						m_nPriority;
	int						m_nSaveDays;
	EStringType				m_StringType;
//	bool					m_bSendState;
	int sendFail;
//	int sendOK;
	int m_iSendmaxrec;
public:
	CThreadSendSMS(	CMobileManager& mgr, 
					const vector<CString>& numbers, 
					const vector<CString>& sendStrings,
					int   priority,
					int   saveDays,
					EStringType type);
	
protected:
	virtual void DoWork();
private:
	bool SendTo(size_t nNumberIndex);
	bool SendOneSMS(const CString& number, const CString& content);
	void SaveSentSMS(const CString& number, const CString& content);
};
