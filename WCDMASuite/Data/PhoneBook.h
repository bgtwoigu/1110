#ifndef PHONEBOOK_H
#define PHONEBOOK_H


struct TPhoneBookItem
{
	CString strName;	//����
	CString strMobilePhone;//�绰����
};

struct TPhoneBookItemEx: public TPhoneBookItem
{
	int		nID;			//��ϵ��ΨһID
	int		nParentID;		//��ID
	CString	strTypeName;	//������
	CString	strBusiness;	//ְ��
	CString strEmail;		//�ʼ�
	CString strCountry;		//����/����
	CString strHomePhone;	//��ͥ�绰
	CString strFax;			//����
	CString strOffice;		//��λ	
	CString strOfficePhone;	//�칫�绰
public:
	bool operator==(const TPhoneBookItemEx& item) const;
	BOOL operator=(const TPhoneBookItemEx& item);
	TPhoneBookItemEx();
};

class CPhonebook
{
protected:
	virtual ~CPhonebook(){}
public:
	virtual size_t	Size()const = 0;

	virtual void	SortByName(bool bUpOrder) = 0;
	virtual size_t  MaxStorage()const = 0;
};

#endif
