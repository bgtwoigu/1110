#ifndef __spl_ime_eng_h_
#define __spl_ime_eng_h_



/** \enum SplKeyType
	\brief key type defination
 */
enum SplKeyType
{
	SPKT_Down,
	SPKT_Up,
	SPKT_LongPress
};


/** \enum SplKey
	\brief key defination
 */
enum SplKey
{
/* char key */
	SPKEY_NULL						= 0,

	SPKEY_0 						= '0',
	SPKEY_1							= '1',
	SPKEY_2							= '2',
	SPKEY_3							= '3',
	SPKEY_4							= '4',
	SPKEY_5							= '5',	
	SPKEY_6							= '6',	
	SPKEY_7							= '7',
	SPKEY_8							= '8',
	SPKEY_9							= '9',

	SPKEY_Star						= '*',			
	SPKEY_Sharp						= '#',

	SPKEY_Dot                       = '.',

	SPKEY_AT						= '@',

	SPKEY_FUNC_NULL					= 0x80,
	SPKEY_0L,
	SPKEY_1L,
	SPKEY_2L,
	SPKEY_3L,
	SPKEY_4L,
	SPKEY_5L,
	SPKEY_6L,
	SPKEY_7L,
	SPKEY_8L,
	SPKEY_9L,

/* function key */
	SPKEY_Left,
	SPKEY_Right,
	SPKEY_Up,
	SPKEY_Down,
	SPKEY_OK,
	SPKEY_Back,
	SPKEY_Return,

    SPKEY_Num
} ;

/** 
    SIME internal return values definition
 */
enum SIMEReturn
{
	SMR_OK                      = 0x00,	///< OK, event handled
	SMR_Ignored					= 0x01,	///< event ignored

	SMR_Failed					= 0x10,	///< return errors of MMI internal begin with this value
	SMR_EngineNotInit,					///< engine not initialized
	SMR_NoEngine,						///< no engine
	SMR_NullEvent,						///< pointer to event is null!
	SMR_InvalidEventType,				///< invalid event type! see \ref EventType
	SMR_NullInitData,					///< pointer to init data is null when event type is SplImeInit
	SMR_InvalidInputMode,				///< invalid input mode, see \ref SplInputMode
	SMR_UnsupportedLanguageType,		///< unsupported language type
	SMR_InvalidKeyType,				    ///< invalid key type, see \ref SplKeyType
	SMR_InvalidKey,						///< invalid key, see \ref SplKey
    SMR_InvalidParam,                   ///< ���������ȷ

	SMR_NUM
} ;



/* ----------------------------------------------------------------------------------- */
/* ��ʼ������ */
/* ----------------------------------------------------------------------------------- */
typedef struct __tag_InitData
{
	void         *  imeData;

	unsigned char   reserved1;
	unsigned char   reserved2;
	unsigned char   reserved3;
	unsigned char   reserved4;
	
} SplInitData;


typedef long (*Spl_GetStringWidthAFunc) (const char *);
typedef long (*Spl_GetStringWidthWFunc) (const unsigned short *);

/* ----------------------------------------------------------------------------------- */
/* UI���� */
/* ----------------------------------------------------------------------------------- */
typedef struct __tag_UIInfo
{
	Spl_GetStringWidthAFunc	    fpGetStrWidthA;     //  ���char�ַ������
	Spl_GetStringWidthWFunc	    fpGetStrWidthW;     //  ���ַ����Ŀ��

	unsigned long               candidateWidth;     // ��ѡ�����
	unsigned short              candMinSpacing;     // ÿ����ѡ֮�����С���
    unsigned short              padding;

}SplUIInfo;


/* ----------------------------------------------------------------------------------- */
/* ������� */
/* ----------------------------------------------------------------------------------- */
typedef struct __tag_OutputInfo
{
	unsigned short      *       inputString;     /**< ���������봮 */

	unsigned short      *       candidates[20]; /**< ��ѡ�ִ� */
	unsigned short              candidatesNum;  /**< ��ѡ�ĸ��� */
	unsigned short              candidateIndex; /**< ��ǰ�����ĺ�ѡ������ */	

	unsigned char               isShowIMWin;    /**< �Ƿ���ʾ���뷨���� */
    unsigned char               isSelectedCand; /**< ��ǰ�Ƿ�����ѡѡ��״̬ */	
	unsigned char               isCanUpScreen;  /**< �ڵ�ǰ״̬���Ƿ��������*/
	unsigned char               padding;

	unsigned short              upscreenLen;    /**< isCanUpScreenΪ��ʱ,�����Ҫ�������ַ����� */	
    unsigned short              padding2;
	unsigned short      *       upscreenStr;    /**< isCanUpScreenΪ��ʱ,�����Ҫ�������ַ��� */

	unsigned char				isAssociateMode;        // �Ƿ�����������ģʽ
	unsigned char				isChnInterpunctionMode; // �Ƿ������ı��ģʽ		
	unsigned char				isEnInterpunctionMode;  // �Ƿ���Ӣ�ı��ģʽ
	unsigned char				isSwitch;               // �Ƿ�����Ӣ���޷��л�״̬
	
	unsigned char				isShowDownArrow;        // �жϺ�ѡҳ�Ƿ����·�
	unsigned char				isShowUpArrow;          // �жϺ�ѡҳ�Ƿ����Ϸ�
	unsigned char				isShowLeftArrow;        // �ж��Ƿ�������ѡ���ѡ
	unsigned char				isShowRightArrow;       // �ж��Ƿ�������ѡ���ѡ

} SplOutputInfo ;



/* ----------------------------------------------------------------------------------- */
/* ȫ������ */
/* ----------------------------------------------------------------------------------- */
typedef struct __tag_ImeGlobals
{
	SplInitData		initData;
	SplUIInfo		uiInfo;
	SplOutputInfo   outputInfo;
	
} SplImeGlobals;


#ifdef __cplusplus
extern "C" {
#endif

extern SplImeGlobals    g_SplImeGlobals;

extern enum SIMEReturn SplImeInit(void);
extern enum SIMEReturn SplImeDeinit(void);
	
extern enum SIMEReturn SplImeProcessKey(enum SplKey uKey, enum SplKeyType type);

extern enum SIMEReturn SplImeHandleInputEvent(int nType, unsigned long param1, unsigned long param2);
	
#ifdef __cplusplus
}
#endif

#endif