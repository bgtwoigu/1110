#ifndef PCMCIA_HELP_HEADER
#define PCMCIA_HELP_HEADER

#include "thread.h"
//����Ӳ����װ�򵼴��ڣ����У���ر���
class CWizardMonitor: public CThread
{
protected:
	virtual void Go();
};

//�޸�OXser.inf����һ����ʱ�ļ�
CString CreateTempInf(const CString& strDstPath, const CString& strHWID);
void DeleteTempInf(const CString& strDstPath);

#endif