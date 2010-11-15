#ifndef AEETV_H
#define AEETV_H
/*===============================================================================
    INCLUDES AND VARIABLE DEFINITIONS
===============================================================================*/
/*-------------------------------------------------------------------
            Defines
-------------------------------------------------------------------*/

/*-------------------------------------------------------------------
      Include Files
-------------------------------------------------------------------*/
#include "AEE.h"
#include "AEEMedia.h"
#include "AEEBitmap.h"
#include "AEE_OEMComdef.h"
#include "AEEIBitmap.h"
#define INHERIT_FROM_CAMERA
#ifdef INHERIT_FROM_CAMERA
#include "AEECameraEx.h"
#endif

typedef  unsigned short int ATV_U16;
typedef  unsigned char ATV_U8;

//#define TLG_TEST_SENSITIVE               //���Ե��ӵ������ȣ��ύ����ʱʹ��
#define TLG_MAX_PROGRAM_NUM 32

//#define TLG_CMD_SET_CHN  (CAM_CMD_BASE-1)             //��UI�Ļص����cmd
//#define TLG_STATUS_SET_CHN_DONE  (CAM_STATUS_BASE-1)
//#define TLG_STATUS_SET_CHN_FAIL  (CAM_STATUS_BASE-2)

typedef	enum {
    TLG_REGION_START = -1,
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
	TLG_TV_625_LINES,
	TLG_TV_NONE
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

	ATV_U16 mHOffset,					// ˮƽƫ�Ƶ���
	 	 mHReduce,					// ˮƽ��С����
		 mVOffset,					// ��ֱƫ�Ƶ���
		 mVReduce;					// ��ֱ��С����
} TLG_TV_OPTIONS;


typedef struct {
	uint8 mVolume;					// ����
} TLG_FM_OPTIONS;

typedef struct {
	TLG_TUNE_MODE mTuneMode;		// ��ǰ��гģʽ

	uint8 mIdxListCnt;				// ��̨�б�����Ч��������
	uint8 mCurListIdx;				// ��ǰ��̨�б������
#if 0//FM_CUTOFF_AT_76MHZ
	uint16
#else /* FM_CUTOFF_AT_76MHZ */
	uint8
#endif /* FM_CUTOFF_AT_76MHZ */
		mCurChnIdx;				// ��ǰ������̨������
	struct {
#if 0//FM_CUTOFF_AT_76MHZ
	uint16
#else /* FM_CUTOFF_AT_76MHZ */
	uint8
#endif /* FM_CUTOFF_AT_76MHZ */
				ChnIdx;				// ��̨����
	} mIdxListBuf[32];				// ��̨�����б�
} TLG_FM_STORAGE;


typedef enum
{
    TLG_SET_REGION,
    TLG_SET_VOLUME,
    TLG_SET_DEFINITION,      /*����������0~63*/    
    TLG_SET_BRIGHT,         /*��������0~15*/
    TLG_SET_CONTRAST,       /*���öԱȶ�0~15*/
    TLG_SET_SATURATION,     /*����ɫ�ʱ��Ͷ�0~255*/
    TLG_SET_DISPLAY_SIZE,   /*���û���ƫ����Ԥ��*/     
    TLG_SET_DISPLAY_OFFSET,    /*���û���ƫ����Ԥ��*/
    CAM_SET_DISPLAY_SIZE,
    CAM_SET_SIZE,
}ATV_SET_PARAM_e;



typedef enum
{    
    TLG_GET_CHN_COUNT,
#ifdef TLG_TEST_SENSITIVE
    TLG_GET_TFCODE,
    TLG_GET_TTLGAIN,
    TLG_GET_CHROMLOCK
#endif
}ATV_GET_PARAM_e;


typedef enum
{
    ATV_AUTO_SCAN,               /*�Զ�����ģʽ*/
    ATV_FAST_SET,                /*������̨*/
    ATV_SET_AV_STOP              /*��̨ʱ����Ƶ��ֹ*/   
}ATV_SET_CHN_TYPE_e;

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
    int chn;          //���ص�����Ƶ���ţ������0��ʾʧ��
}ATV_SetChn_Notify_t;


typedef struct ITlgAtv ITlgAtv;

typedef void (*PFNATVNOTIFY)(void * pUser, void * pNotify);

#define tv_ver2
#ifndef tv_ver2 
#define INHERIT_ITlgAtv(iname)   \
        INHERIT_IQueryInterface(iname);   \
        int (*Start)(OEMINSTANCE p, int16 nMode, uint32 dwParam);  \
        int (*Stop)(OEMINSTANCE p);   \
        int (*GetFrame)(OEMINSTANCE h, IBitmap ** ppFrame);   \
        int (*RegisterNotify)(OEMINSTANCE h, ATV_NOTIFY_HANDLE_ID_e Id,PFNATVNOTIFY pfnNotify, void * pUser);  \
        int (*SetChn)(OEMINSTANCE p,ATV_U16 chn, ATV_SET_CHN_TYPE_e type);       \
        int (*SetParam)(OEMINSTANCE h,ATV_SET_PARAM_e type, int hparam, int lparam);  \
        int (*GetParam)(OEMINSTANCE h,ATV_GET_PARAM_e type, void * hparam, void * lparam); \
        int (*Update)(OEMINSTANCE h, uint32 dwParam)
#else
#define INHERIT_ITlgAtv(iname)   \
        INHERIT_IQueryInterface(iname);   \
        int (*Create)(OEMINSTANCE p);   \
        int (*Start)(OEMINSTANCE p, int16 nMode, uint32 dwParam);  \
        int (*Stop)(OEMINSTANCE p);   \
        int (*GetFrame)(OEMINSTANCE h, IBitmap ** ppFrame);   \
        int (*RegisterNotify)(OEMINSTANCE h, ATV_NOTIFY_HANDLE_ID_e Id,PFNATVNOTIFY pfnNotify, void * pUser);  \
        int (*SetChn)(OEMINSTANCE p,ATV_U16 chn, ATV_SET_CHN_TYPE_e type);       \
        int (*SetParam)(OEMINSTANCE h,ATV_SET_PARAM_e type, int hparam, int lparam);  \
        int (*GetParam)(OEMINSTANCE h,ATV_GET_PARAM_e type, void * hparam, void * lparam); \
        int (*Update)(OEMINSTANCE h, uint32 dwParam)
#endif 
AEEINTERFACE(ITlgAtv)
{
    INHERIT_ITlgAtv(ITlgAtv);
};


struct ITlgAtv
{
  const AEEVTBL(ITlgAtv) *pvt;
  OEMINSTANCE *pCamera;
  OEMINSTANCE pBitmap;
  PFNATVNOTIFY   pfnNotify;
  void *            pUser;
  int nRefs;
};


#ifdef tv_ver2
#define  ITV_Create(p)                p->pvt->Create(p)
#endif
/*����������ͬʱʹ��camera��tv�Ľӿ�*/
#define ITV_start(p,m,dw)                   p->pvt->Start(p,m,dw)
#define ITV_StartPreview(p)                 ITV_start(p,CAM_MODE_PREVIEW,0)
#define ITV_StopPreview(p)                 ITV_Stop(p)

#define ITV_updateimg(p,dw)                 p->pvt->Update(p,dw)


#define ITV_Stop(p)                         p->pvt->Stop(p)
/*���￴�������ʹ����tv�Ľӿ�*/
#define ITV_AddRef(p)                       p->pvt->AddRef(p)
#define ITV_Release(p)                      p->pvt->Release(p)
#define ITV_QueryInterface(p, i, p2)        p->pvt->QueryInterface(p, i, p2)

#define ITV_RegisterNotify(p, pfn, pu)              p->pvt->RegisterNotify(p,HANDLE_FOR_TV_MAIN, (PFNATVNOTIFY)pfn, pu)
#define ITV_RegisterSetChnNotify(p, pfn, pu)        p->pvt->RegisterNotify(p,HANDLE_FOR_TV_SETCHN,(PFNATVNOTIFY)pfn, pu)

//#define ITV_RegisterNotify(p, pfn, pu)      AEE_GET_ATV_PVTBL(p)->RegisterNotify(p, pfn, pu)

/*�����ǵ����ض��Ľӿ�*/
#define ITV_SetParam(p, c, p1, p2)           p->pvt->SetParam(p,c, p1, p2)
#define ITV_GetParam(p, c, p1, p2)           p->pvt->GetParam(p,c, p1, p2)

#define ITV_SetRegion(p, v)                 ITV_SetParam(p,TLG_SET_REGION,(TLG_REGION_CODE)v,0)               /*�ӿ����������飬������Է���0*/
#define ITV_SetVolume(p, v)                 ITV_SetParam(p,TLG_SET_VOLUME,(TLG_REGION_CODE)v,0)
#define ITV_SetContrast(p, v)               ITV_SetParam(p, TLG_SET_CONTRAST,(int32)(v),0)       /*���öԱȶ�0~15*/
#define ITV_SetBrightness(p, v)             ITV_SetParam(p, TLG_SET_BRIGHT,(int32)(v),0)           /*��������0~15*/
#define ITV_SetDefinition(p, v)             ITV_SetParam(p, TLG_SET_DEFINITION,(int32)(v),0)       /*����������0~63*/
#define ITV_SetSaturation(p, v)             ITV_SetParam(p, TLG_SET_SATURATION,(int32)(v),0)       /*����ɫ�ʱ��Ͷ�0~255*/
#define ITV_SetDisplay_Size(p, v,u)         ITV_SetParam(p, TLG_SET_DISPLAY_SIZE, (int32)(v),(u))  /*���û���ƫ����Ԥ��*/ 
#define ITV_SetDisplay_Offset(p, v,u)       ITV_SetParam(p, TLG_SET_DISPLAY_OFFSET, (int32)(v),(u))  /*���û���ƫ����Ԥ��*/

#define ITV_SetDisplaySize(p,v)             ITV_SetParam(p, CAM_SET_DISPLAY_SIZE, (int32)(v),0)
#define ITV_SetSize(p,v)                    ITV_SetParam(p, CAM_SET_SIZE, (int32)(v),0)
#define ITV_GetChnCount(p,v)                ITV_GetParam(p,TLG_GET_CHN_COUNT,(void *)(v),(void *)NULL)        
#define ITV_GetFrame(p,v)                   p->pvt->GetFrame(p,v)
#define ITV_SetChn(p,v,y)                   p->pvt->SetChn(p,v,y)

#ifdef TLG_TEST_SENSITIVE
#define ITV_GetTF(p,v)                       ITV_GetParam(p,TLG_GET_TFCODE,(void *)(v),(void *)NULL)
#define ITV_GetTG(p,v)                       ITV_GetParam(p,TLG_GET_TTLGAIN,(void *)(v),(void *)NULL)  
#define ITV_GetChrLock(p,v)                  ITV_GetParam(p,TLG_GET_CHROMLOCK,(void *)(v),(void *)NULL)  
#endif

typedef struct ITlgFm ITlgFm;

#define INHERIT_ITlgFm(iname)   \
        INHERIT_IQueryInterface(iname);   \
        int (*Start)(OEMINSTANCE p, int16 nMode, uint32 dwParam);  \
        int (*Stop)(OEMINSTANCE p);   \
        int (*AutoScanFm)(TLG_FM_STORAGE *fm_storage);      \
        int (*SetFmChn)(ATV_U16 chn)


AEEINTERFACE(ITlgFm)
{
    INHERIT_ITlgFm(ITlgFm);
};


struct ITlgFm
{
  const AEEVTBL(ITlgFm) *pvt;
  int nRefs;
};

#define IFM_Start(p,m,dw)                AEEGETPVTBL(p)->pvt->Start(p,m,dw)
#define IFM_Stop(p)                      AEEGETPVTBL(p)->pvt->Stop(p)
#define IFM_SetChn(p,v)                  AEEGETPVTBL(p)->pvt->SetFmChn(v)
#define IFM_AutoScan(p,v)                AEEGETPVTBL(p)->pvt->AutoScanFm(v)
#endif

