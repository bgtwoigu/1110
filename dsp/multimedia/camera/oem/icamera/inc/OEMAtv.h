#ifndef __OEMATV_H__
#define __OEMATV_H__
#include "AEE.h"
#include "AEEComdef.h"
#include "AEE_OEMComdef.h"
#include "AEEInterface.h"
#include "AEEIBitmap.h"

#include "OEMCamera.h"
#include "AEECamera.h"


#include "tlgPlatform.h"

typedef  unsigned short int ATV_U16;
typedef  unsigned char ATV_U8;

/*include by tlgPlatform.h*/
#if 0
typedef	enum {
	TLG_REGION_USA,				// ����
	TLG_REGION_CANADA,			// ���ô�
	TLG_REGION_KOREA,				// ����
	TLG_REGION_TAIWAN,			// ̨��
	TLG_REGION_MEXICO,			// ī����
	TLG_REGION_CHILE,				// ����
	TLG_REGION_VENEZUELA,			// ί������
	TLG_REGION_PHILIPPINES,		// ���ɱ�
	TLG_REGION_JAMAICA,			// �����
	TLG_REGION_CHINA,				// �й���½
	TLG_REGION_SHENZHEN,			// ����
	TLG_REGION_VIETNAM,			// Խ��
	TLG_REGION_WESTERNEUROPE,	// ��ŷ
	TLG_REGION_TURKEY,			// ������
	TLG_REGION_UAE,				// ������
	TLG_REGION_AFGHANISTAN,		// ������
	TLG_REGION_SINGAPORE,			// �¼���
	TLG_REGION_THAILAND,			// ̩��
	TLG_REGION_CAMBODIA,			// ����կ
	TLG_REGION_INDONESIA,			// ӡ��������
	TLG_REGION_MALAYSIA,			// ��������
	TLG_REGION_LAOS,				// ����
	TLG_REGION_PORTUGAL,			// ������
	TLG_REGION_SWEDEN,			// ���
	TLG_REGION_PAKISTAN,			// �ͻ�˹̹
	TLG_REGION_SPAIN,				// ������
	TLG_REGION_JAPAN,				// �ձ�
	TLG_REGION_UK,					// Ӣ��
	TLG_REGION_HONGKONG,			// ���
	TLG_REGION_SOUTHAFRICA,		// �Ϸ�
	TLG_REGION_BRAZIL,				// ����
	TLG_REGION_ARGENTINA,		// ����͢
	TLG_REGION_INDIA,				// ӡ��
	TLG_REGION_BURMA,				// ���
	TLG_REGION_SUDAN,				// �յ�
	TLG_REGION_YEMEN,				// Ҳ��
	TLG_REGION_PERU,				// ��³
	TLG_REGION_BOLIVIA,			// ����ά��
	TLG_REGION_ECUADOR,			// ��϶��
	TLG_REGION_SURINAME, 			// ������
	TLG_REGION_AUSTRALIA,			// �Ĵ�����
	TLG_REGION_NEWZEALAND,		// ������
#ifdef    TLG_1120
	TLG_REGION_NORTHKOREA, 	/*������*/	
	TLG_REGION_MONGOLIA ,	/*	�ɹ�*/		
	TLG_REGION_AZERBAIJAN ,	/*�����ݽ�*/	
	TLG_REGION_CZETH, 		/*�ݿ�*/		
	TLG_REGION_POLAND, 		/*����*/		
	TLG_REGION_RUSSION,		/*����˹*/	
	TLG_REGION_UKRAINE, 		/*�ڿ���*/	
	TLG_REGIONI_TUNISIA, 	/*	ͻ��˹ */	
	TLG_REGION_EGYPT, 		/*����*/		
	TLG_REGION_MOROCCO, 		/*Ħ���*/	
	TLG_REGION_IRAQ,		/*������*/	
	TLG_REGION_SAUDIARABIA,	/*ɳ��*/		
	TLG_REGION_GREECE,		/*ϣ�� */		
#endif
	TLG_REGION_TOTAL
} TLG_REGION_CODE;


typedef	enum {
	TLG_TV_525_LINES,
	TLG_TV_625_LINES
} TLG_TV_SCANLINES;

typedef	enum {
	TLG_SCRMD_WINDOW,
	TLG_SCRMD_FULLSCREEN
} TLG_SCREEN_MODE;

typedef enum {
	TLG_TUNEMD_AUTO,
	TLG_TUNEMD_MANUAL
} TLG_TUNE_MODE;


typedef struct {
	uint8 mBrightness;				// ����
	uint8 mContrast;					// �Աȶ�
	uint8 mSaturation;				// ɫ���Ͷ�
	uint8 mDefinition;				// ������
	uint8 mVolume;					// ����

	uint8 mHOffset,					// ˮƽƫ�Ƶ���
	 	 mHReduce,					// ˮƽ��С����
		 mVOffset,					// ��ֱƫ�Ƶ���
		 mVReduce;					// ��ֱ��С����
} TLG_TV_OPTIONS;

typedef struct {
	TLG_TUNE_MODE mTuneMode;		// ��ǰ��гģʽ
	TLG_SCREEN_MODE mScrMode;		// ��ǰ��Ļģʽ
	TLG_REGION_CODE mRegion;		// ��ǰ���ӵ�������

	uint8 mIdxListCnt;				// Ƶ���б�����Ч��������
	char mCurListIdx;				// ��ǰƵ���б������
	uint8 mCurChnIdx;				// ��ǰ����Ƶ��������
	struct {
		uint8 ChnIdx;					// Ƶ������
		char ChnName[7*2+1];		// Ƶ������
	} mIdxListBuf[TLG_MAX_PROGRAM_NUM];				// Ƶ�������б�
} TLG_TV_STORAGE;

typedef struct {
	uint8 mVolume;					// ����
} TLG_FM_OPTIONS;

typedef struct {
	TLG_TUNE_MODE mTuneMode;		// ��ǰ��гģʽ

	uint8 mIdxListCnt;				// ��̨�б�����Ч��������
	uint8 mCurListIdx;				// ��ǰ��̨�б������
#if 0 //FM_CUTOFF_AT_76MHZ
	uint16
#else /* 0 //FM_CUTOFF_AT_76MHZ */
	uint8
#endif /* 0 //FM_CUTOFF_AT_76MHZ */
		mCurChnIdx;				// ��ǰ������̨������
	struct {
#if 0 //FM_CUTOFF_AT_76MHZ
	uint16
#else /* 0 //FM_CUTOFF_AT_76MHZ */
	uint8
#endif /* 0 //FM_CUTOFF_AT_76MHZ */
				ChnIdx;				// ��̨����
	} mIdxListBuf[32];				// ��̨�����б�
} TLG_FM_STORAGE;
#endif



/******************************************************************************/



typedef enum
{
    HANDLE_FOR_TV_MAIN,
    HANDLE_FOR_TV_SETCHN
}ATV_NOTIFY_HANDLE_ID_e;


typedef enum
{
    ATV_SET_CHN_SUCCESS,
    ATV_SET_CHN_FAILED
}SET_CHN_STATUS_e;

typedef struct TVSETCHNNOTIFY
{
    SET_CHN_STATUS_e status;
    int chn;          //���ص�Ƶ���ţ������0��ʾʧ��
}ATV_SetChn_Notify_t;

typedef struct ITlgAtv ITlgAtv;

typedef void (*PFNATVNOTIFY)(void * pUser, void * pNotify);

#define tv_ver2
#ifndef tv_ver2 
#define INHERIT_ITlgAtv(iname)   \
        INHERIT_IQueryInterface(iname);   \
        int (*Start)(OEMINSTANCE p, int16 nMode, uint32 dwParam);  \
        int (*Stop)(OEMINSTANCE p);   \
        int (*GetFrame)(OEMINSTANCE p, IBitmap ** ppFrame);   \
        int (*RegisterNotify)(OEMINSTANCE h, ATV_NOTIFY_HANDLE_ID_e Id,PFNATVNOTIFY pfnNotify, void * pUser);  \
        int (*SetChn)(OEMINSTANCE p,ATV_U16 chn, ATV_SET_CHN_TYPE_e type);      \
        int (*SetParam)(OEMINSTANCE h,ATV_SET_PARAM_e type, int hparam, int lparam);  \
        int (*GetParam)(OEMINSTANCE h,ATV_GET_PARAM_e type, void * hparam, void * lparam);  \
        int (*Update)(OEMINSTANCE h, uint32 dwParam);
#else 
#define INHERIT_ITlgAtv(iname)   \
        INHERIT_IQueryInterface(iname);   \
        int (*Create)(OEMINSTANCE p);   \
        int (*Start)(OEMINSTANCE p, int16 nMode, uint32 dwParam);  \
        int (*Stop)(OEMINSTANCE p);   \
        int (*GetFrame)(OEMINSTANCE p, IBitmap ** ppFrame);   \
        int (*RegisterNotify)(OEMINSTANCE h, ATV_NOTIFY_HANDLE_ID_e Id,PFNATVNOTIFY pfnNotify, void * pUser);  \
        int (*SetChn)(OEMINSTANCE p,ATV_U16 chn, ATV_SET_CHN_TYPE_e type);      \
        int (*SetParam)(OEMINSTANCE h,ATV_SET_PARAM_e type, int hparam, int lparam);  \
        int (*GetParam)(OEMINSTANCE h,ATV_GET_PARAM_e type, void * hparam, void * lparam);  \
        int (*Update)(OEMINSTANCE h, uint32 dwParam);
#endif  
// int (*Update)(OEMINSTANCE h, uint32 dwParam);
	// int (*Update)(OEMINSTANCE h, uint32 dwParam);

AEEINTERFACE(ITlgAtv)
{
    INHERIT_ITlgAtv(ITlgAtv)
};


struct ITlgAtv
{
  const AEEVTBL(ITlgAtv) *pvt;
  OEMINSTANCE pCamera;
  OEMINSTANCE pBitmap;
  PFNATVNOTIFY   pfnNotify;
  void *            pUser;
  int nRefs;
 // boolean bTvStart;
};

struct TlgAtv_ScanInfo
{    
    OEMINSTANCE pAtv;
    int chn;  
};

/******************************************************************************/
 int OEMTLGAtv_New(IShell *pIShell,AEECLSID ClsID,void** ppInterface);
#ifdef tv_ver2
static int OEMTLGAtv_Create(OEMINSTANCE h);
#endif
static int OEMTLGAtv_Delete(void *h);
static int OEMTLGAtv_Start(OEMINSTANCE h, int16 nMode, uint32 dwParam);
static int OEMTLGAtv_Stop(OEMINSTANCE h);
static int  OEMTLGAtv_GetFrame(OEMINSTANCE h, IBitmap ** ppFrame);
static int OEMTLGAtv_RegisterNotify(OEMINSTANCE h, ATV_NOTIFY_HANDLE_ID_e Id,PFNATVNOTIFY pfnNotify, void * pUser);
static uint32 OEMTLGAtv_AddRef(ITlgAtv* pIAtv);
static uint32 OEMTLGAtv_Release(ITlgAtv* pIAtv);
static int OEMTLGAtv_QueryInterface(ITlgAtv* pIAtv, AEECLSID ClsID, void** ppInterface);
static int OEMTLGAtv_SetChn(OEMINSTANCE h,ATV_U16 chn,ATV_SET_CHN_TYPE_e type);

static int OEMTLGAtv_SetParam(OEMINSTANCE h,ATV_SET_PARAM_e type, int hparam, int lparam);
static int OEMTLGAtv_GetParam(OEMINSTANCE h,ATV_GET_PARAM_e type, void * hparam, void * lparam);
static int OEMTLGAtv_Update(OEMINSTANCE h, uint32 dwParam);








/******************************************************************************/
typedef struct ITlgFm ITlgFm;

#define INHERIT_ITlgFm(iname)   \
        INHERIT_IQueryInterface(iname);   \
        int (*Start)(OEMINSTANCE p, int16 nMode, uint32 dwParam);  \
        int (*Stop)(OEMINSTANCE p);   \
        int (*AutoScanFm)(TLG_FM_STORAGE *fm_storage);      \
        int (*SetFmChn)(ATV_U16 chn);       


AEEINTERFACE(ITlgFm)
{
    INHERIT_ITlgFm(ITlgFm)
};


struct ITlgFm
{
  const AEEVTBL(ITlgFm) *pvt;
  int nRefs;
};



extern int OEMTLGFm_New(IShell *pIShell,AEECLSID ClsID,void** ppInterface);
extern int OEMTLGFm_Delete(void *h);
extern int OEMTLGFm_Start(OEMINSTANCE h, int16 nMode, uint32 dwParam);
extern int OEMTLGFm_Stop(OEMINSTANCE h);
extern uint32 OEMTLGFm_AddRef(ITlgFm* pfm);
extern uint32 OEMTLGFm_Release(ITlgFm* pfm);
extern int OEMTLGFm_QueryInterface(ITlgFm* pfm, AEECLSID ClsID, void** ppInterface);

extern int OEMTLGFm_AutoScan(TLG_FM_STORAGE *fm_storage);
extern int OEMTLGFm_SetChn(ATV_U16 chn);


#endif


