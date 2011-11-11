#ifndef CONNECTRECORD_7965C2BBCF8F4f47B74F12B535D3673A
#define CONNECTRECORD_7965C2BBCF8F4f47B74F12B535D3673A

#include <vector>
#include "Data.h"
#include <winbase.h>

struct TConnectInfo
{
	int		nID;		//������¼ΨһID
	CTime tmBeginTime;  //��ʼʱ��
	unsigned int iTotalSecond; //�ܹ����ӵ�����
	__int64 iSend; //������
	__int64 iReceive; //������

	__int64 Flux()const
	{
		return iSend + iReceive;
	}
	unsigned int BelongYearMonth()const;

	TConnectInfo();
	CString TimeString()const;
};

struct IConnectionRecordObserver
{
	virtual void OnAdd(size_t index, const TConnectInfo& info) = 0;
};

class CConnectRecord : public Data
{
public:
	CConnectRecord();
	~CConnectRecord();

	size_t Size()const;				//�õ�������¼��
	const TConnectInfo& GetAt(size_t index)const;	//�õ�һ��������¼
	void AddFront(const TConnectInfo& info);		//����һ��������¼��ǰ��
	void AddTail(const TConnectInfo& info);			//����һ��������¼�ں���
	void Clear();									//�������������¼
	void Delete(size_t nID);						//ɾ��һ��������¼
	
	void GetStatTime(CTime tmStartTime, CTime tmEndTime,
		CString &strTime, CString &strFlux)const;
	void CConnectRecord::GetStatTime(CTime tmStartTime, 
								 CTime tmEndTime, 
								 __int64& i64FluxUpLoad,
								 __int64& i64FluxDownLoad) const;

	size_t GetMonthStatCount()const;
	bool GetMonthStat(size_t iIndex, 
			CString &strDate, 
			CString &strLastTime, 
			CString &strFlux)const;
	void ClearVector();
	void AddObserver(IConnectionRecordObserver* pObserver);
	void RemoveObserver(IConnectionRecordObserver* pObserver);

	BOOL GetAllRecords();			//�õ�����������¼

	int FindIndexByID(size_t id)const;
	
private:
	virtual void  modifyFieldByDB(int argc, char** argv, char** columnName);
	
	BOOL Insert(const TConnectInfo& item);	//����һ��������¼��Ϣ
	void FireEvent_Add(size_t index, const TConnectInfo& info);
private:
	CConnectRecord(const CConnectRecord&);
	CConnectRecord& operator=(const CConnectRecord&);
private:
	std::vector<TConnectInfo*> m_Connections;
	std::vector<IConnectionRecordObserver*> m_Observers;
	mutable CRITICAL_SECTION m_CS;

	sqlite3* m_pDB;
};

/*
CREATE TABLE SC700_ConnectRecord(ConnectRecordID integer PRIMARY KEY,
							 BegTime char(30) default(NULL),
							 ConnectPeriod char(30) default(NULL),
							 SendNum char(30) default(NULL),
							 RecvNum char(30) default(NULL))
*/

#endif
