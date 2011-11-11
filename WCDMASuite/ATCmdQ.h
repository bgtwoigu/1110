#ifndef AFX_TXBUFFERLINK_H__C19E50DE_6999_4A8A_8E29_4A678BB63D0C__INCLUDED_
#define AFX_TXBUFFERLINK_H__C19E50DE_6999_4A8A_8E29_4A678BB63D0C__INCLUDED_

#include "GlobalDataCenter.h"
#include "MobileATProto.h"
#include <list>
#include <Afxmt.h>

/**
*	���͵�AT�������.
*	��������������Ҫ���͵�AT����.�������̰߳�ȫ��.
*/
class CATCmdQ  
{
public:
	//���캯��
	CATCmdQ();

	//��������
	~CATCmdQ();

	//ȡ��ͷ��
	bool PopTopRecord(CATCmd & Record);

	//�ӵ�β��
	void AddBack(const CATCmd & Record);

	//�ӵ�ͷ��
	void AddFront(const CATCmd& Record);

	//ɾ������
	void DeleteAll();

	//��ѯ����
	size_t Size()const
	{
		return m_Data.size();
	}

	HANDLE	EventNotEmpty(){return m_EventQ;}
private://��ֹ����copy constructor �� operator=
	CATCmdQ(const CATCmdQ&);
	CATCmdQ& operator=(const CATCmdQ&);
protected:
	CEvent				m_EventQ;	//���зǿ�
	CCriticalSection	m_cs; //�����б��ٽ���
	std::list<CATCmd>	m_Data;//���������

	typedef std::list<CATCmd>::iterator ITERATOR;
	typedef std::list<CATCmd>::const_iterator CONST_ITERATOR;
};

#endif
