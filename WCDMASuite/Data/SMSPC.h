#ifndef SMS_HEADER_5F6FCE2A453545f1BBCCE862BF35C08D
#define SMS_HEADER_5F6FCE2A453545f1BBCCE862BF35C08D

#include <vector>
#include "Data.h"
#include "SMS.h"


class CSMSPC : public Data,public CSMS
{
public:
	CSMSPC();
	~CSMSPC();

	virtual void Clear();					//��յ�ǰ����,����վΪ����ɾ��,����Ϊ�������վ
	virtual TSMSItem& GetAt(size_t nIndex);	//�õ�һ����Ϣ
	virtual const TSMSItem& GetAt(size_t nIndex)const;	//�õ�һ����Ϣ
	virtual void Delete(size_t nID);	//ɾ��һ����Ϣ
	virtual bool FindID(int id)const;	
	
	virtual TSMSItem& AddBack(const TSMSItem& item);	//����һ����Ϣ�����ͬʱ�������ݿ�
	virtual TSMSItem& AddFront(const TSMSItem& item);	//����һ����Ϣ����ǰͬʱ�������ݿ�
	void ForeverDelete(size_t nID);					

	virtual size_t Size()const;							//������Ϣ����

	void operator+=(const TSMSItem& item);
	void operator+=(const CSMSPC& list);

	virtual void SetLimit(size_t nLimit);
	virtual size_t GetLimit()const;

	virtual bool Find(const TSMSItem& item)const;

	virtual void AddObserver(ISMSObserver* pObserver);
	virtual void RemoveObserver(ISMSObserver* pObserver);
	BOOL UpdateStatus(int index);
	BOOL GetRecordsByMailBoxID(size_t nMailBoxID);	//��������ID�õ���ǰ����ȫ����Ϣ
	BOOL GetRecordsByFilter(CString filter, int maxRowCount = -1);	//����SQL���õ���ǰ����ȫ����Ϣ
	void MoveAllRecordsToRecyle();				//�ƶ���ǰ����������Ϣ������վ
	BOOL MoveOneRecordToRecyle(size_t nID);		//�ƶ���ǰ��Ϣ������վ
	
	BOOL RestoreOneRecord(size_t nID);			//�ӻ���վ�л�ԭһ����Ϣ
	void RestoreAllRecords();					//�ӻ���վ�л�ԭ������Ϣ
	void ClearVector();
	TSMSItem& CSMSPC::FindRecordByIndex(size_t id);	
	int FindIndexByID(size_t nID)const;

private:
	void FireEvent_SMSAdd(size_t index, const TSMSItem& sms);
	void FireEvent_Sort();

	virtual void  modifyFieldByDB(int argc, char** argv, char** columnName);	

	BOOL Insert(TSMSItem& item);				//����һ����Ϣ�����ݿ�
								//����ڴ�Vector
	BOOL getBlobDatasByFilter(CString const filter);
	
private://��ֹ����
	CSMSPC& operator=(const CSMSPC&);
	CSMSPC(const CSMSPC&);
private:
	std::vector<TSMSItem*> m_Data;
	size_t m_nLimit;
	size_t m_nMailBoxID;	//��ǰ����ID

	std::vector<ISMSObserver*> m_Observers;
	mutable CRITICAL_SECTION		m_CS;

	sqlite3* m_pDB;
};

/*
CREATE TABLE SC700_SMS(SMSID integer PRIMARY KEY,
					   MailBoxID int default(0),
					   OrigMailBoxID int default(0),
					   Flag int default(0),
					   Telephone char(20) default(NULL),
					   Text blob,
					   Time char(30) default(NULL));
*/
#endif
