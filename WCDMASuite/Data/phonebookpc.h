#ifndef AFX_PHONEBOOKPC_H__9F171FCF_2CA3_46D4_80BE_CDC8F2D8FFB5__INCLUDED_
#define AFX_PHONEBOOKPC_H__9F171FCF_2CA3_46D4_80BE_CDC8F2D8FFB5__INCLUDED_

#include "PhoneBook.h"
#include "Data.h"
#include <vector>

struct TPBClass
{
	int nID;	//��Ƭ��ID,��sqlite3�Զ�����
	int nParentID;	//��Ƭ�и�ID��������������չ
	CString strClassName;	//��Ƭ������
	CString strVoicePath;	//����·��
};

class CPhonebookPCClass : public Data
{
public:
	CPhonebookPCClass();
	~CPhonebookPCClass();
public:
	bool	Add(const TPBClass& item);	//����һ������Ƭ��
	bool	Delete(size_t nID);				//����IDɾ��һ����Ƭ��	
	bool	Modify(const TPBClass& item);	//�޸�һ����Ƭ��

	virtual const	TPBClass& GetAt(size_t index)const; //�õ�һ����Ϣ
	virtual size_t	Size()const;
	bool execCommand_(CString cmd);
	void ClearVector(); 
	BOOL GetAllRecords();	//�õ�ȫ����Ƭ����Ϣ
	int FindIndexByID(size_t id)const;
	int FindIDByName(CString strName);
	CString FindNameByID(int id);
private:
	virtual void  modifyFieldByDB(int argc, char** argv, char** columnName);
	void ChangeItem(TPBClass& item);
	
private:
	TPBClass m_PBClass;
	sqlite3* m_pDB;
	std::vector<TPBClass*> m_Data;
};	
class CPhonebookPC : public Data,public CPhonebook   
{
public:
	CPhonebookPC();
	virtual ~CPhonebookPC();
	
public:
	virtual bool	Add(const TPhoneBookItemEx& item);	//����һ����ϵ����Ϣ
	virtual bool	Delete(size_t nID);				//����IDɾ��һ����ϵ����Ϣ	
	virtual bool	Modify(const TPhoneBookItemEx& item);	//�޸�һ����Ϣ
	virtual const	TPhoneBookItemEx& GetAt(size_t index)const; //�õ�һ����Ϣ
	virtual size_t	Size()const;						//������ϵ�˸���

	virtual bool	Find(const TPhoneBookItemEx& item, size_t* pIndex);
	virtual void	SortByName(bool bUpOrder);
	virtual size_t  MaxStorage()const;
	BOOL GetRecordsByTypeName(CString strTypeName);	//������Ƭ�����õ���ǰ��Ƭ����ȫ����Ƭ��Ϣ
	BOOL GetRecordsByNumber(CString number);
	BOOL GetRecordsByTypeName(int nParentID);	//�õ�������Ƭ����ȫ����Ƭ��Ϣ
	BOOL GetRecordsByName(CString name);
	BOOL GetAllRecords();		//�õ����е绰����Ϣ
	void Clear();	//��յ�ǰ��Ƭ����������ϵ����Ϣ
	BOOL ClearCurrClassCard(int nParentID);
	void ClearVector();
	int FindIndexByID(size_t id)const;
	BOOL IsExitRecord(const TPhoneBookItemEx& item,BOOL bIsOutLook);	//�ж��Ƿ����ظ���¼
private:
	virtual void  modifyFieldByDB(int argc, char** argv, char** columnName);
	BOOL Insert(const TPhoneBookItemEx& item);	//����һ����ϵ����Ϣ
	void ChangeItem(TPhoneBookItemEx& item);
	
private:
	CPhonebookPC(const CPhonebookPC&);
	CPhonebookPC& operator=(const CPhonebookPC&);
private:
	std::vector<TPhoneBookItemEx*> m_Data;
	CString m_strTypeName;

	sqlite3* m_pDB;
};

/*
CREATE TABLE SC700_PBClass(PBClassID integer PRIMARY KEY,
					PBParentID int default(0),
					PBClassName char(50) default(NULL),
					PBClassVoice char(256) default(NULL))
*/

/*
CREATE TABLE SC700_PhoneBook(PhoneID integer PRIMARY KEY,
							 typename char(30) default(NULL),
							 ParentID int default(1),
							 Name char(50) default(NULL),
							 Business char(30) default(NULL),
							 Email char(50) default(NULL),
							 Country char(50) default(NULL),
							 HomePhone char(20) default(NULL),
							 MobilePhone char(20) default(NULL),
							 Fax char(20) default(NULL),
							 Office char(50) default(NULL),
							 OfficePhone char(20) default(NULL))
*/
#endif
