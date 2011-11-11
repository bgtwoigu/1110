#ifndef AFX_CALLRECORD_H__078CC5AD_2C0A_48B1_9CBE_D725AF866E63__INCLUDED_
#define AFX_CALLRECORD_H__078CC5AD_2C0A_48B1_9CBE_D725AF866E63__INCLUDED_

#include <vector>
#include "Data.h"

//ͨ������
enum ECallType
{
	CALL_INCOMING,	//���
	CALL_OUTGOING,	//���
};

enum CallRecordType
{
	CALLRECORD_RECV = 0,	//�ѽӵ绰
	CALLRECORD_DAIL,		//�Ѳ��绰
	CALLRECORD_MISS			//δ�ӵ绰
};


struct TCallInfo
{
	int		  nID;		//ͨ����¼ΨһID
	CallRecordType eRecordType;	//ͨ����¼����
	ECallType type;		//����
	CString   strTelephone;	//����
	CString   strName;		//����
	CTime	  begin;	//��ʼʱ��
	DWORD	  period;	//����ʱ��
	
	TCallInfo();
	CString TimeString()const;
	bool IsMissingCall()const;
	bool IsFailingCall()const;
};

class CCallRecord;
struct ICallRecordObserver
{
	virtual void OnAdd(CCallRecord* pSender, int nIndex, const TCallInfo& newItem) = 0;
};

class CCallRecord : public Data 
{
public:
	CCallRecord();
	~CCallRecord();

	size_t Size()const;							//�õ�ͨ����¼����
	TCallInfo& GetAt(size_t index);				//�õ�һ��ͨ����¼
	const TCallInfo& GetAt(size_t index)const;	//�õ�һ��ͨ����¼
	TCallInfo& Add(const TCallInfo& item);		//����һ��ͨ����¼
	void Delete(size_t index);					//ɾ��һ��ͨ����¼
	void Clear();								//�������ͨ����¼

	DWORD GetCallInTime()const;					//�������绰ʱ��
	DWORD GetCallOutTime()const;				//�������绰ʱ��

	void AddObserver(ICallRecordObserver* pObserver);
	void RemoveObserver(ICallRecordObserver* pObserver);

	BOOL GetRecordsByRecordType(size_t nRecordType);		//�������͵õ�ͨ����¼
	void ClearVector();							//����ڴ�Vector
private:
	CCallRecord(const CCallRecord&);
	CCallRecord& operator=(const CCallRecord&);
private:
	DWORD CalculateTime(ECallType type)const;
	void  FireEvent_Add(int nIndex, const TCallInfo& newItem);

	virtual void  modifyFieldByDB(int argc, char** argv, char** columnName);
	BOOL Insert(TCallInfo& item);				//����һ����Ϣ�����ݿ�
	int FindIndexByID(size_t nID)const;
	
private:
	std::vector<TCallInfo*> m_Data;
	std::vector<ICallRecordObserver*>	m_Observers;

	mutable CRITICAL_SECTION		m_CS;//�ٽ�������

	size_t m_nRecordType;				//��ǰͨ����¼����

	sqlite3* m_pDB;
};

/*
CREATE TABLE SC700_CallRecord(CallRecordID integer PRIMARY KEY,
							 RecordType int default(0),
							 Flag int (0),
							 Telephone char(20) default(NULL),
							 Name char(50) default(NULL),
							 BegTime char(30) default(NULL),
							 CallPeriod char(30) default(NULL))
*/

#endif // !defined(AFX_CALLRECORD_H__078CC5AD_2C0A_48B1_9CBE_D725AF866E63__INCLUDED_)
