#ifndef	_GBINPUT_H_
#define _GBINPUT_H_

#include "GBLt.h"
#include "GBLt_def.h"
#include "EB.h"

//#ifndef WIN32
//#include "wchar.h"
//#endif

#define __GBSHDATA		/** @����ʹ��ܿ���*/

#define GB_DATA_UNICODE16		1 /**< ��������ʹ��Unicode�������� */
#define GB_DATA_GB			2 /**< ��������ʹ��GB�������� */

#ifndef GB_DATA_CODE
#define GB_DATA_CODE	GB_DATA_UNICODE16
#else
#define GB_DATA_CODE	GB_DATA_GB
#endif

#ifndef __GB_TYPE__
#define __GB_TYPE__
typedef const void *                GBCPVOID;
typedef void *                      GBPVOID;
typedef int                         GBINT;
typedef unsigned int                GBUINT;
typedef unsigned short              GBUINT16;
typedef long                        GBINT32;
typedef long                        GBLONG;
typedef unsigned long               GBULONG;
typedef unsigned long               GBUINT32;
typedef unsigned char               GBBYTE;
#ifndef DEFINED_GBCHAR
typedef char                        GBCHAR;
#endif
typedef unsigned char               GBUINT8;
typedef char                        GBINT8;
typedef char *                      GBPSTR;
typedef const char *                GBPCSTR;
typedef unsigned short *            GBPWSTR;
typedef const unsigned short *      GBCPWSTR;
typedef unsigned char 				GBBOOL8;
typedef unsigned int				GBBOOL;
#endif


//#define VIETNAM
//#define DIFF_CAPITAL

/**@brief �м�㶨������뷨ģʽ */
typedef enum  {
    GBIMNull = 0,/**< �� */
    GBPinYin,/**< ƴ�����뷨 */
    GBStroke,/**< �ʻ����뷨 */
    GBYinStroke,/**< ������뷨 */
    GBSmartLatin,/**< ����Ӣ�����뷨 */
    GBCharacter,/**< ��ĸ���뷨 */
	GBNumber,/**< �������뷨 */
#ifdef DIFF_CAPITAL
	GBCAPITALCharacter,
	GBLOWERCASECharacter
#endif
} GBInputMode;

/**@brief ���������ʼ������ */
typedef struct __tag_gb_chn_engine_init_data {
    GBCPVOID                        pEngineData; /**< ������������ݵ�ַ */
    GBPVOID                          pSelfMadeData; /** ����������Լ���Ƶ���ݵ�ַ����byteΪ��λ */
    GBUINT32                        nSelfMadeDataLength; /**< ����������Լ���Ƶ���ݵĳ��� */
    GBInputMode                     nInputMethod; /**< ��ʼ����������ģʽ */
    GBINT                           nCandDesireLenInWord; /**< ���ĺ�ѡ�ִʵ������� */
    GBINT                           nInterpunctionDesireLen; /**< ���ĺ�ѡ������ */
    fpEBShDataUpdate                fpOnSelfMadeDataChanged; /**< ����������Լ���Ƶ���ݸ���֪ͨ���� */
    GBINT							reserved[9];/** Ԥ�� */
} GBChnEngineInitData;  

/**@brief ���������ʼ������ */
typedef struct __tag_gb_lt_engine_init_data {
    GBCPVOID                        pEngineData; /**< Ӣ����������� */
    GBPVOID                         pSelfMadeData; /**< Ӣ��������Լ���Ƶ���ݵ�ַ */
    GBUINT32                        nSelfMadeDataLength; /**< ������Լ���Ƶ���ݳ��ȣ���byteΪ��λ */
    GBINT                           nCandDesireLenInWord;/**< Ӣ�ĺ�ѡ����ĳ���֮�͵����ֵ */
    GBINT                           nCandDesireCountInWord;/**< Ӣ�ĺ�ѡ����ĸ��������ֵ */
    fpUpdateSelfHelpData            fpOnSelfMadeDataChanged;/**< Ӣ��������Լ���Ƶ���ݸ���֪ͨ���� */
    GBINT							reserved[10];/** Ԥ�� */
} GBLtEngineInitData;

/**@brief �������뷨�������� */
typedef struct __tag_gb_setting {
    GBBOOL8                         bUseLongPhress;/**< �Ƿ�ʹ�ó��� */
    GBBOOL8                         bUpNumber;/**< �������л����� */
    GBBOOL8                         bChnEnConvert;/**< ��Ӣ���л����� */
    GBBOOL8                         reserved[13];/** Ԥ�� */
} GBSetting;


typedef struct __tag_GBIME_INIT {
    GBChnEngineInitData				gbChnEngineInit;
    GBLtEngineInitData				gbLtEngineInit;
    GBSetting                       gbSetting;
    GBINT32                         reserved[4];
} GBIME_INIT;

//#define _SHOW_DEBUG
#define CANDNUM_APAGE_CHARIM	8
#define MESSAGE_MAX_LENGTH 32
#define MAX_UPSCREEN  32   /**@brief ��������ֵĳ��� */



typedef enum 
{
   NO_GBIME_MODE,
   PT_PINYIN_MODE,
   KEY_PINYIN_MODE,
   KEY_STROKE_MODE,
   HW_ASSOC_MODE
}GBIME_MODE;

/**@brief ����1 */
#define GB_VK1 '1'
/**@brief ����2 */
#define	GB_VK2 '2' 
/**@brief ����3 */
#define	GB_VK3 '3'
/**@brief ����4 */
#define	GB_VK4 '4'
/**@brief ����5 */
#define	GB_VK5 '5'
/**@brief ����6 */
#define	GB_VK6 '6'
/**@brief ����7 */
#define	GB_VK7 '7'
/**@brief ����8 */ 
#define	GB_VK8 '8' 
/**@brief ����9 */
#define	GB_VK9 '9'
/**@brief ����0 */ 
#define	GB_VK0 '0' 

/**@brief ����1 */
#define GB_VK1L (1024+6)
/**@brief ����2 */ 
#define	GB_VK2L (1024+7)
/**@brief ����3 */ 
#define	GB_VK3L (1024+8) 
/**@brief ����4 */
#define	GB_VK4L (1024+9) 
/**@brief ����5 */
#define	GB_VK5L (1024+10)
/**@brief ����6 */ 
#define	GB_VK6L (1024+11) 
/**@brief ����7 */
#define	GB_VK7L (1024+12)
/**@brief ����8 */ 
#define	GB_VK8L (1024+13) 
/**@brief ����9 */
#define	GB_VK9L (1024+14) 
/**@brief ����0 */
#define	GB_VK0L (1024+15) 

#ifndef _SHOW_DEBUG
/**@brief ����OK */
#define GB_VK_OK		(1024+16) 

#define GB_VK_RETURN	(1024+17) 
/**@brief ���ؼ� */
#define GB_VK_BACK		(1024+18) 
/**@brief ���ϼ� */
#define GB_VK_UP		(1024+19) 
/**@brief ���¼� */
#define GB_VK_DOWN		(1024+20)
/**@brief ��� */
#define GB_VK_LEFT		(1024+21) 
/**@brief �Ҽ� */
#define GB_VK_RIGHT	(1024+22) 
/**@brief #�� */
#define GB_VK_SHARP	'#' 
/**@brief *�� */
#define GB_VK_STAR		'*' 
#endif

/**@brief ����ƴ�����ں�����������Ļ��� */
#define CFG_SYLLABLE_WTH		(2048+1)
/**@brief ����ϵͳ��ص��ַ�����ȵĻص����� */ 
#define CFG_GETSTREXTENT_FUNC	(2048+2) 
/**@brief ���Ի�������� */
#define CFG_INITENGINE			(2048+3) 
/**@brief ���û�����뷨 */
#define CFG_SETACTIVEIM			(2048+4) 
/**@brief ���û�����뷨 */
#define CFG_RESETACTIVEIM		(2048+5) 
/**@brief �����Ƿ�ɸı����뷨 */
#define CFG_ISCANCHANGEIM		(2048+6) 
/**@brief ������ʾ��ƴ�����ڼ�ļ����� */
#define CFG_SYLLABLE_SPLIT		(2048+7) 
/**@brief ����ֻ���������� */
#define CFG_SETCHINESE_LIMIT	(2048+8) 
/**@brief ����ֻ������Ӣ�� */
#define CFG_SETENGLISH_LIMIT	(2048+9)
/**@brief ����ϵͳ��ص�UNICODE�ַ�����ȵĻص����� */
#define CFG_GETWSTREXTENT_FUNC	(2048+10) 

/**@brief ѡ��ѡ��ƴ������*/
#define CFG_SELECT_SYLLABLE		(2048+11) 

/**@brief ���ݴ���ĺ��ֵõ������*/
#define  CFG_GET_ASSOCIATE    (2048+12) 

struct GBIM
{
	unsigned char indentifyName;	
	void *pIS;
	struct GBIM *pSwitchItem;
	unsigned char status;
	void (*GBIMOnUp)(struct GBIM *p,void *param);
	void (*GBIMOnDown)(struct GBIM *p,void *param);
	void (*GBIMOnLeft)(struct GBIM *p,void *param);
	void (*GBIMOnRight)(struct GBIM *p,void *param);
	void (*GBIMOnNumS)(struct GBIM *p,void *param,int realkey);
	void (*GBIMOnNumL)(struct GBIM *p,void *param,int realkey);
	void (*GBIMOnBlank)(struct GBIM *p,void *param);
	void (*GBIMOnBack)(struct GBIM *p,void *param);
	void (*GBIMSwitchIM)(struct GBIM *p,void *param);
	void (*GBIMReset)(struct GBIM *p,void *param);
	void (*GBIMDelete)(struct GBIM *p,void *param);
	void (*GBIMInit)(struct GBIM *p,void *param);
	void (*GBIMPrepareShowMessage)(struct GBIM *p,void *param);
	int (*GBIMCanPageUp)(struct GBIM *p,void *param);
	int (*GBIMCanPageDown)(struct GBIM *p,void *param);
	int (*GBIMCanPrevHeader)(struct GBIM *p,void *param);
	int (*GBIMCanNextHeader)(struct GBIM *p,void *param);	
	void (*GBIMOnStar)(struct GBIM *p,void *param);
#ifdef _SHOW_DEBUG
	void (*GBIMDisplay)(struct GBIM *p,void *param);
#endif
	void (*GBIMOnSharp)(struct GBIM *p,void *param);
	void (*GBIMOnReturn)(struct GBIM *p,void *param);
};

typedef struct GBIM GBIM;

#define MAX_INPUTLTH			32
#define MAX_COMPLTH				5
#define MAX_SYLLABLELTH			20
#define MAX_CANDS_BUF           64

typedef int (*pfGetStrLth_T)(const char *pstr);

/**@brief ��MMI�㽻���ṹ�� */
typedef struct
{
	GBIM*			GBIMS;/**< ָ�����뷨 */
	int 			activeIM;/**< ��ǰ���뷨����������������� */
	int				prevIM;/**< ǰһ���뷨����������������� */
	GBInputMode		activeMode;/**< ������뷨 */
	GBSetting		gbSet;/**< MMI��Ҫ���������ָ������õĽṹ */
	int				isShowCandWin;/**< ��ǰ��ѡ���ڵ���ʾ״̬ */
	int				isSelectedCand;/**< ��ǰ�Ƿ�ѡ�к�ѡ�� */
	int				isCanChangeIM;/**< ��ǰ�Ƿ�����޸����뷨 */
	int				isChineseOnly;/**< ��ǰ�Ƿ�ֻ�����뺺�� */
	int				isEnglishOnly;/**< ��ǰ�Ƿ�ֻ������Ӣ�� */
	int				isCandHelpTips;/**< ��ǰ�Ƿ���ʾ�����ı� */
	int				noHandleKeyEvent;/**< ���뷨�����Ƿ���(�ڲ�ʹ��) */
	int				shouldUpScreen;/**< ��ǰ�Ƿ�Ӧ������*/
	int				option;/**< ����Ӣ�ĺ�ѡ�����ʽ */
	int				syllableAreaWth;/**< ƴ�����ں�����������Ļ��� */
	int				syllableSplit;/**< ��ʾ��ƴ�����ڼ�ļ����� */
	pfGetStrLth_T 	pfGetStrLth;/**< ϵͳ��ص��ַ�����ȵĻص����� */
	pfGetStrLth_T 	pfGetWStrLth;/**< ϵͳ��ص��ַ�����ȵĻص����� */
	unsigned short 	upscreenStr[MAX_UPSCREEN];/**< shouldUpScreenΪ��ʱ,�����Ҫ�������ַ��� */
	int	  			upscreenLen ;/**< shouldUpScreenΪ��ʱ,�����Ҫ�������ֵĸ��� */
	char 			fullOutput[MAX_INPUTLTH*2];/**< ���������봮 */
	char 			stroke[MAX_INPUTLTH*2];/**< �ʻ������봮 */
	unsigned short 	components[MAX_COMPLTH];/**< ���������봮 */
	int 			componentsLth;/**< ���׸���	 */
	int 			syllableSelIndex;/**< ƴ�����ڵ�������Ϊ0ʱ��ʾ��ǰѡ�������� */
	char*			syllable[MAX_SYLLABLELTH];/**< ƴ������ */
	int 			syllableNum;/**< ƴ�����ڵĸ��� */
	int 			selectIndex;/**< ��ǰѡ��ĺ�ѡ�ִʵ����� */
	unsigned short  cands[MAX_CANDS_BUF+1][MESSAGE_MAX_LENGTH];/**< ��ѡ�ִ� */
	int 			candsNum;/**< ��ѡ�ִʵĸ��� */
	GBIME_MODE gbImeMode;
}GBIMWT;

typedef struct 
{
    unsigned short key;
    unsigned short candidates[32];
}KeyCandidates;

#ifdef __cplusplus 
extern "C"{
#endif
extern GBIMWT GBIMW;
#ifdef __cplusplus 
}
#endif

/**  @brief MMI���õĽӿں���
*  @param[int] realkey \n
				-CFG_SYLLABLE_WTH		����ƴ�����ں�����������Ļ��� \n
				-CFG_GETSTREXTENT_FUNC	����ϵͳ��ص��ַ�����ȵĻص�����,paramΪ����ϵͳ��صĺ���ָ�� \n
				-CFG_INITENGINE			��ʼ���������,param��ֵΪGBIME_INIT�Ľṹָ�� \n
				-CFG_SETACTIVEIM		���û�����뷨,param��ֵΪenum GBInputMode�е�ֵ \n
				-CFG_RESETACTIVEIM		���û�����뷨,param������ʱ������ \n
				-CFG_ISCANCHANGEIM		�����Ƿ�ɸı����뷨,paramΪ1��ʾ��ǰ���뻷���ɸı����뷨,0Ϊ���ɸı� \n
				-CFG_SYLLABLE_SPLIT		������ʾ��ƴ�����ڼ�ļ����� \n
				-CFG_SETCHINESE_LIMIT	����ֻ����������,paramΪ1��ʾֻ����������,0Ϊȡ�������� \n
				-CFG_SETENGLISH_LIMIT	����ֻ������Ӣ��,paramΪ1��ʾֻ������Ӣ��,0Ϊȡ�������� \n
				-CFG_SELECT_SYLLABLE	ƴ������ģʽ��ֱ��ѡ��ǰҳ�ĵ�param��ƴ�����ڣ�param����Ϊ[0,syllableLen),�Ƿ�param��Ӱ�� \n

*  @param[unsigned long] param ���Ӧ����realkey�����õĲ���
*  @return 1����,0������
*  @note �����м����MMI��ΨһͨѶ�ĺ���
*  @author hxc
*/
int HandleKeyEvent(int realkey,unsigned long param);

#endif
