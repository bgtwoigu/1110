#include "stdafx.h"
#include "StringLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringLink::CStringLink()
:m_nSize(0)
{
}

CStringLink::~CStringLink()
{
}

/*
ɾ�����м�¼(�ַ���)
*/
void CStringLink::DeleteAll()
{
	//���ﲢ��ɾ���������ڲ��洢
	//���ǽ��������ݵĸ�����Ϊ0��
	//�ڲ�������Ϊ����ظ���һ��ʹ��,
	//�����Ը�Ч��
	m_nSize = 0;
}

/*
�ڶ���β����Ӽ�¼(�ַ���)������������ӵļ�¼�����
*/
size_t CStringLink:: AddRecord(const std::string& inStrLine)
{
	if(m_nSize==m_Data.size())
	{
		m_Data.push_back(inStrLine);
		++m_nSize;
	}
	else
	{
		m_Data[m_nSize] = inStrLine;
		++m_nSize;
	}
	return m_nSize-1;
}

const std::string& CStringLink::GetRecord(size_t iIndex)const
{
	ASSERT(iIndex<m_nSize);

	return m_Data[iIndex];
}

size_t CStringLink::Size()const
{
	return m_nSize;
}

