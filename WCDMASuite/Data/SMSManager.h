#ifndef _SMSMANAGER_H_
#define _SMSMANAGER_H_

#include "SMSPC.h"
#include "SMSEmbed.h"

class CSMSManager
{
public:
	void AddNewEmbedSMS(const TSMSItem& info);
	void SetEmbedSMSBoxLimit(size_t size);
public:
	CSMSPC& GetSMSPCRecv();
	const CSMSPC& GetSMSPCRecv()const;
	
	CSMSPC& GetSMSPCSend();
	const CSMSPC& GetSMSPCSend()const;
	
	CSMSPC& GetSMSPCNotSend();
	const CSMSPC& GetSMSPCNotSend()const;

	CSMSPC& GetSMSPCDraft();
	const CSMSPC& GetSMSPCDraft()const;

	CSMSPC& GetSMSPCRecyle();
	const CSMSPC& GetSMSPCRecyle()const;

	CSMSEmbed& GetSMSEmbedRecv();
	const CSMSEmbed& GetSMSEmbedRecv()const;

	CSMSEmbed& GetSMSEmbedSend();
	const CSMSEmbed& GetSMSEmbedSend()const;

	CSMSEmbed& GetSMSEmbedDraft();
	const CSMSEmbed& GetSMSEmbedDraft()const;

	bool  IsReadID(int ID)const;
private:
	CSMSPC m_PCRecv;			//PC�ռ���
	CSMSPC m_PCSend;			//PC�ѷ���
	CSMSPC m_PCNotSend;			//PCδ����
	CSMSPC m_PCDraft;			//PC�ݸ���
	CSMSPC m_PCRecyle;			//PC����վ

	CSMSEmbed m_EmbedRecv;		//Ƕ����ռ���
	CSMSEmbed m_EmbedSend;		//Ƕ����ѷ���
	CSMSEmbed m_EmbedDraft;		//Ƕ��˲ݸ���
};

#endif
