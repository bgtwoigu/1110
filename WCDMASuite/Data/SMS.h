#ifndef _SMS_H_
#define _SMS_H_

//////////////////////////////////
//Ƕ��˶�������
enum ESMSType
{
	SMS_UNREAD = 0,
	SMS_READ,	
	SMS_UNSEND,
	SMS_SEND,
};

//PC�˶�������
enum PSMSType
{
	SMS_PC_RECV = 0,	//�ռ���
	SMS_PC_SEND,		//�ѷ���
	SMS_PC_UNSEND,		//δ����
	SMS_PC_UNREAD		//δ��
};

enum SMSMailBoxID
{
	SMS_MAILBOX_RECV = 0,	//�ռ���
	SMS_MAILBOX_SEND,		//�ѷ���
	SMS_MAILBOX_UNSEND,		//δ����
	SMS_MAILBOX_DRAFT,		//�ݸ���
	SMS_MAILBOX_RECYLE		//����վ
};

struct TSMSItem
{
	CString strPhoneNum;	//�绰����
	CTime tmSMSTime;		//����ʱ��
	CString strText;		//����
	ESMSType iSMSState;		//Ƕ��˶�������
	PSMSType nPSMSState;	//PC�˶�������
	int iSaveDays;//�������� (������ʹ��) -1��Ч����Զ����
	int iID; //ID,uim���е���ţ�����pc���ģ���sqlite3�Զ�����
	SMSMailBoxID nMailBoxID;		//����ID 0�ռ���1�ѷ���2δ����3�ݸ���4����վ 
	SMSMailBoxID nOrigMailBoxID;	//ԭ����ID,���ڻ���վ�ָ�����
	int nIndex;		//�������е�����


	CString TimeString()const;

	bool operator==(const TSMSItem& item)const;

	TSMSItem();
};

struct ISMSObserver
{
	virtual void OnSMSAdd(size_t index, const TSMSItem& sms) = 0;
	virtual void OnSMSSort() = 0;
};

class CSMS
{
public:
	virtual void Clear() = 0;					
	virtual TSMSItem& GetAt(size_t nIndex) = 0;	
	virtual const TSMSItem& GetAt(size_t nIndex)const = 0;	
	virtual void Delete(size_t nID) = 0;	
	virtual bool FindID(int id)const = 0;	
	
	virtual TSMSItem& AddBack(const TSMSItem& item) = 0;							
	virtual TSMSItem& AddFront(const TSMSItem& item) = 0;												

	virtual size_t Size()const =0;						

	virtual void SetLimit(size_t nLimit) = 0;
	virtual size_t GetLimit()const = 0;

	virtual bool Find(const TSMSItem& item)const = 0;

	virtual void AddObserver(ISMSObserver* pObserver) = 0;
	virtual void RemoveObserver(ISMSObserver* pObserver) = 0;

};

#endif