// ComParser.cpp: implementation of the CComParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComParser::CComParser()
{
	m_ParserBuffer = new char[512];
	m_BufferSize = 512;
}

CComParser::~CComParser()
{
	delete[] m_ParserBuffer;
	m_BufferSize = NULL;
	m_BufferSize = 0;
}

void CComParser::ResizeBuffer(size_t nSize)
{
	if(nSize>m_BufferSize)
	{
		delete[] m_ParserBuffer; 
		m_ParserBuffer = new char[nSize];
		m_BufferSize = nSize;
	}
}


//ȥ����ߵ�ָ���ַ�,���ص�һ�����Ǹ��ַ���λ��
static const char* EraseLeft(const char* str, const char* end, char trim)
{
	for(;str<end && *str == trim; ++str);
	return str;
}

//�ҵ���һ��ָ���ַ���λ��
static const char* FindFirst(const char* str, const char* end, char c)
{
	for(;str<end && *str != c; ++str);
	return str;
}
//Ĩȥ����ָ���ַ�
static void EraseRight(char* str, size_t size, char c)
{
	for(char* ss = str+size-1; ss>=str && *ss==c; --ss);
	if(ss<str)
		*str = 0;
	else
		*(ss+1) = 0;
}
/*
���ַ����ĸ����������������ŵ�m_parsedParamLink��
������ݱ����Ǳ�׼�� :,,"",(), �����ĸ�ʽ
*/
void CComParser::ParseString(const std::string& strParent)
{
	const char* cBegin = strParent.c_str();
	const char* cEnd = cBegin+strParent.size();
	const char* cCur = cBegin;
	m_parsedParamLink.DeleteAll();
	ResizeBuffer(strParent.size());

	char* buf = m_ParserBuffer;

	cBegin = EraseLeft(cBegin, cEnd, ' ');
	if(cBegin==cEnd)return;
	cCur = FindFirst(cBegin, cEnd, ':');
	if(cCur == cEnd)return;

	size_t strSize = cCur-cBegin;
	memcpy(buf, cBegin, strSize);
	EraseRight(buf,strSize,' ');
	if(buf[0])m_parsedParamLink.AddRecord(buf);

	cBegin = cCur+1;
	cCur = cBegin;
	while(cCur<cEnd)
	{
		cBegin = EraseLeft(cBegin, cEnd, ' ');
		if(cBegin==cEnd)break;

		if(*cBegin == '\"')//ƥ������
		{
			cCur = FindFirst(cBegin+1, cEnd, '\"');
			cCur = EraseLeft(cCur, cEnd,' ');
			if(cCur>=cEnd)break;

			strSize = cCur-cBegin-1;
			memcpy(buf, cBegin+1, strSize);
			EraseRight(buf,strSize,' ');
			if(buf[0])
				m_parsedParamLink.AddRecord(buf);
			else
				m_parsedParamLink.AddRecord("");
		}
		else if(*cBegin == '(')//ƥ������
		{
			cCur = FindFirst(cBegin+1, cEnd, ')');
			cCur = EraseLeft(cCur, cEnd,' ');
			if(cCur>=cEnd)break;

			strSize = cCur-cBegin-1;
			memcpy(buf, cBegin+1, strSize);
			EraseRight(buf,strSize,' ');
			if(buf[0])
				m_parsedParamLink.AddRecord(buf);
			else
				m_parsedParamLink.AddRecord("");
		}
		else
		{
			cCur = FindFirst(cBegin+1, cEnd, ',');
			strSize = cCur-cBegin;
			memcpy(buf, cBegin, strSize);
			EraseRight(buf,strSize,' ');
			if(buf[0])m_parsedParamLink.AddRecord(buf);
		}

		cBegin = FindFirst(cCur, cEnd, ',');
		if(cBegin >= cEnd)break;
		++cBegin;//����','
		cCur = cBegin;
	}
}

/*
�õ�ParseString�������������
*/
const std::string& CComParser::GetParsedParam(int iIndex)const
{
	static const std::string NULLString;

	ASSERT(iIndex<m_parsedParamLink.Size());
	if(iIndex>=m_parsedParamLink.Size())
		return NULLString;

	return m_parsedParamLink.GetRecord(iIndex);
}

size_t CComParser::GetParsedParamCount()const
{
	return m_parsedParamLink.Size();
}

