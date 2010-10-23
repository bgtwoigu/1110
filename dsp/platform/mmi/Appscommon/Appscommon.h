#ifndef APPSCOMMON_H
#define APPSCOMMON_H
/*==============================================================================
//           A P P S   C O M M O N   F U N C T I O N S
// �ļ���
//        Appscommon.h
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------

==============================================================================*/



/*==============================================================================
                             
                             ���ļ��������ⲿ�ļ�
                             
==============================================================================*/
#include "AEE.h"
#include "AEEShell.h"
#include "AEEMenu.h" 
#include "AEEDisp.h"
#include "AEEStdLib.h"
#include "AEEFile.h"
#include "OEMFeatures.h"
#include "OEMText.h"
/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
// ͼ��ͼƬ��Դ
// must use full file name for BREW 3
#define  AEE_APPSCOMMONRES_IMAGESFILE "fs:/sys/appscommonimages.bar"

// ���� BREW 3 ����Ҫ���ض�����Դ�ļ���
// ��������ص��ַ�����Դ�ļ�
#define  AEE_APPSCOMMONRES_LANGFILE ("fs:/sys/" AEE_RES_LANGDIR APPSCOMMON_RES_FILE)


#define RGB_WHITE_NO_TRANS MAKE_RGB(0xFF, 0xFF, 0xFF)

#if defined(FEATURE_DISP_176X220)
    #define SCREEN_WIDTH            176
    #define SCREEN_HEIGHT           220
    #define STATEBAR_HEIGHT         14
    #define BOTTOMBAR_HEIGHT        22
    #define TITLEBAR_HEIGHT         30
    #define MENUITEM_HEIGHT         22
    #define SCROLLBAR_WIDTH         5
    #define STATUSBAR_HEIGHT        14
#elif defined(FEATURE_DISP_128X128)
    #define SCREEN_WIDTH            128
    #define SCREEN_HEIGHT           128
    #define STATEBAR_HEIGHT            14
	#define BOTTOMBAR_HEIGHT        16
    #define TITLEBAR_HEIGHT         16
    #define MENUITEM_HEIGHT          16
    #define SCROLLBAR_WIDTH          5
    #define STATUSBAR_HEIGHT         16
#elif defined(FEATURE_DISP_160X128)
    #define SCREEN_WIDTH            160
    #define SCREEN_HEIGHT           128
    #define STATEBAR_HEIGHT         14
    #define BOTTOMBAR_HEIGHT        16
    #define TITLEBAR_HEIGHT         16
    #define MENUITEM_HEIGHT         16
    #define SCROLLBAR_WIDTH         5
    #define STATUSBAR_HEIGHT        16
#elif defined(FEATURE_DISP_220X176)		   //176 = statebar(22)+bottombar(22)  +6*menuitem(22)
    #define SCREEN_WIDTH            220
    #define SCREEN_HEIGHT           176
    #define STATEBAR_HEIGHT         22
    #define BOTTOMBAR_HEIGHT        22
    #define TITLEBAR_HEIGHT         22
    #define MENUITEM_HEIGHT         22
    #define SCROLLBAR_WIDTH         5
    #define STATUSBAR_HEIGHT        22	
#elif defined(FEATURE_DISP_128X160)			//160 = statebar(16)+bottombar(16) +8*menuitem(16)
	#define SCREEN_WIDTH            128
    #define SCREEN_HEIGHT           160
    #define STATEBAR_HEIGHT         16
    #define BOTTOMBAR_HEIGHT        16
    #define TITLEBAR_HEIGHT         16
    #define MENUITEM_HEIGHT         16
    #define SCROLLBAR_WIDTH         5
    #define STATUSBAR_HEIGHT        16
#elif defined(FEATURE_DISP_240X320)	
	#define SCREEN_WIDTH            240
    #define SCREEN_HEIGHT           320
    #define STATEBAR_HEIGHT         14
    #define BOTTOMBAR_HEIGHT        16
    #define TITLEBAR_HEIGHT         16
    #define MENUITEM_HEIGHT          16
    #define SCROLLBAR_WIDTH          5
    #define STATUSBAR_HEIGHT         16	
#elif defined(FEATURE_DISP_320X240)			//240 = statebar(32)+bottombar(32)  +5*menuitem(32)
	#define SCREEN_WIDTH            320
    #define SCREEN_HEIGHT           240
    #define STATEBAR_HEIGHT         24
    #define BOTTOMBAR_HEIGHT        24
    #define TITLEBAR_HEIGHT         24
    #define MENUITEM_HEIGHT          32
    #define SCROLLBAR_WIDTH          5
    #define STATUSBAR_HEIGHT         23	
#else
    #define SCREEN_WIDTH            160
    #define SCREEN_HEIGHT           128
    #define STATEBAR_HEIGHT            14
    #define BOTTOMBAR_HEIGHT        16
    #define TITLEBAR_HEIGHT         16
    #define MENUITEM_HEIGHT         16
    #define SCROLLBAR_WIDTH         5
    #define STATUSBAR_HEIGHT        16
#endif

#define APPSCOMMON_BG_COLOR       (RGB_BLACK)

#define APPSCOMMON_DEFAULT_REND 0 //wlh 20090405 add for rend ���Ч��  REND_RANDOM = 0
#define TEXT_GRAPHIC_FONT_COLOR  (RGB_WHITE)
#define TEXT_GRAPHIC_BG_COLOR      (RGB_BLACK)

#define TEXT_FONT_COLOR                 (RGB_BLACK)
#define TEXT_BG_COLOR                     (RGB_WHITE)

#define APPSCOMMON_MENUBG_XPOS  60
#define APPSCOMMON_MENUBG_YPOS  47

#define PROMPTMSG_TIMER        1000

#define MSGBOX_MAXTEXTLEN  256
//������ĸ��������
#define CON_ACCENT_UPPER_START 192
#define CON_ACCENT_UPPER_END   221

#define CON_ACCENT_LOWER_START 224
#define CON_ACCENT_LOWER_END   253

#define OFFSET_ASCII                    (0xFF)
#define MAX_STR              128
#ifndef FEATURE_SMARTSEARCH
   #define FEATURE_SMARTSEARCH
#endif
#if defined( AEE_SIMULATOR)
    #define APPSCOMMON_RES_LANG_FILE "fs:/sys/en/appscommon.bar"
#else
    #define APPSCOMMON_RES_LANG_FILE     ("fs:/sys/" AEE_RES_LANGDIR APPSCOMMON_RES_FILE)
#endif

//����ģ����Զ����APPISREADY_TIMER��EVT_APPISREADY���ж��嵽appscommon.h������TIMERֵͳһΪ100ms
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY��
#define GET_RGB_R(v)     (uint32) ( ((RGBVAL)(v) >> 8) & (0x000000FF) )
#define GET_RGB_G(v)     (uint32) ( ((RGBVAL)(v) >> 16) & (0x000000FF) )
#define GET_RGB_B(v)     (uint32) ( ((RGBVAL)(v) >> 24) & (0x000000FF) )
#define GET_RGBA_A(v)   (uint32) ( (RGBVAL)(v) & (0x000000FF) )

#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
#define TOUCH_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif

/*==============================================================================
                                 
                                  �������Ͷ���
                                 
==============================================================================*/
typedef enum TitleBar_e_Type
{
    // ����Ϣ��������
    TBAR_SMS_MAIN,
    
    // ������������
    TBAR_SETTING_MAIN,
    
    // �ٱ�����������
    TBAR_EXTRA_MAIN,
    
    // ����ҵ����������
    TBAR_UMAX_MAIN,

    // ��Ϸ��������
    TBAR_GAMES_MAIN,

    // ���ö�����ʾͼƬ
    TBAR_COMMON,

    // �ڱ������������Ҽ�ͷ
    TBAR_ARROW
} TitleBar_e_Type;


/*
 * ����з��࣬������˫���������������ڼ����ַ���ʱ���Էֳ����࣬��������
 * switch�
 * */
#define BTBAR_SINGLE_MASK   0x1000
#define BTBAR_DOUBLE_MASK   0x2000
#define BTBAR_TRI_MASK      0x4000

typedef enum BottomBar_e_Type
{
   // �޵ײ���ʾ��
   BTBAR_NONE,

   /*******************start BTBAR_SINGLE_MASK*****************************/
   BTBAR_BACK= 0x1000 + 1,   // -----����
   BTBAR_CANCEL,            // -----ȡ�� 
   BTBAR_CLOSE,              // -----�ر�
   BTBAR_DELETE,             //guoys add @2008.10.20 for no match dont use select
   BTBAR_END,                // -----�һ�
   BTBAR_NEXT,               // -----��һ��
   BTBAR_OK,                 //BY XIAO.CHENG  
   BTBAR_PROMPT_OK,
   BTBAR_SOS,                // -----��������
   BTBAR_STOP,
   BTBAR_SNOOZE,             //add by yangdecai
                             //display stop when alarming
   
   /*******************start BTBAR_DOUBLE_MASK*****************************/
   BTBAR_ACCEPT_REJECT = 0x2000 + 1,
   BTBAR_ADD_BACK,
   BTBAR_ANSWER_IGNORE,
   BTBAR_ANSWER_MUTE,
   BTBAR_ANSWER_REJECT,
   BTBAR_ANSWER_UNMUTE,
   BTBAR_BACK_DELETE,
   BTBAR_BACK_SOS,        // ����-----��������
   BTBAR_STOP_BACK,
   BTBAR_CALL_BACK,
   BTBAR_CONTINU_BACK,    // ����-----����
   BTBAR_CONTINU_DELETE,  // ����-----ɾ��
   BTBAR_CREATE_BACK,     // ����-----����
   BTBAR_EARSE_BACK,      // ɾ��-----����
   BTBAR_EDIT_BACK,       // �༭-----����
   BTBAR_EXCHANGE_BACK,
   BTBAR_EXCHANGE_DELETE,
   BTBAR_FIND_BACK,       // ����-----����
   BTBAR_GALLERY_BACK,
   BTBAR_GALLERY_PLAY_BACK,
   BTBAR_LEFT_START_NULL,
   BTBAR_MENU_BACK,       // �˵�-----����
   BTBAR_MENU_CONTACTS,   // �˵�-----�绰��
   //Add By zzg 2010_07_20
   BTBAR_MENU_FRENDUO,	 // �˵�------FRENDUO
   BTBAR_VIEWMORE_BACK,	 // �������-----����
   BTBAR_FACEBOOK_CHAT,	 // Facebook-----chat
   //Add End
   BTBAR_OK_BACK,         // ȷ��-----����
   BTBAR_OK_CANCEL,       // ȷ��-----ȡ��
   BTBAR_OK_DEL,
   BTBAR_OK_DELETE,       // ȷ��-----ɾ��
   BTBAR_OPTION_BACK,     // ѡ��-----����
   BTBAR_OPTION_STOP,
   BTBAR_OPTION_DEL,
   BTBAR_OPTION_DELETE,   // ѡ��-----ɾ����
   BTBAR_OPTION_ENDCALL,  // ѡ��-----�һ�:
   BTBAR_OPTION_HANDSFREEON,
   BTBAR_OPTION_NORMAL,
   BTBAR_OPTION_UP,       // ѡ��-----����
   BTBAR_PAUSE_BACK, 
   BTBAR_PLAY_BACK, 
   BTBAR_PLAY_STOP,   
#ifdef FEATURE_SPORTS_APP
   BTBAR_PAUSE_STOP,
   BTBAR_RESUME_STOP,
#endif
   BTBAR_READ_BACK,       // �Ķ�-----����
   BTBAR_RECORD_STOP,
   BTBAR_REPLAY_BACK,     // �ز�-----����
   BTBAR_RESET_BACK,      // ����-----����
   BTBAR_RESUME_BACK,
   BTBAR_RETRY_CANCEL,
   BTBAR_SAVE_BACK,       // ����-----����
   BTBAR_SAVE_CANCEL,     // ����-----ȡ��
   BTBAR_SAVE_DELETE,     // ����-----ɾ��
   BTBAR_SELECT_BACK,     // ѡ��-----����
   BTBAR_SELECT_DEL,      // ѡ��-----����
   BTBAR_SELECT_STOP,     // ѡ��-----ֹͣ    
   BTBAR_SEND_BACK,       // ����-----����
   BTBAR_SEND_EARSE,      // ����-----ɾ��
   BTBAR_SNOOZE_DONE,
   BTBAR_SNOOZE_STOP,
   BTBAR_START_BACK,
   BTBAR_UNLOCK_SOS,
   BTBAR_SPORT_STOP,
   BTBAR_VIEW_SNOOZE,
   BTBAR_VIEW_STOP,


   /*******************start BTBAR_TRI_MASK*****************************/
   BTBAR_CANCEL_RESUME_NULL = 0x4000 + 1,
   BTBAR_FULLSCREEN_PAUSE_STOP,
   BTBAR_JEWISH_TODAY_BACK,
   BTBAR_OPTION_CREATE_BACK,
   BTBAR_OPTION_EDIT_BACK,    // ɾ��---�༭---����
   BTBAR_OPTION_OK_BACK,
   BTBAR_OPTION_OK_DEL,
   BTBAR_OPTION_PAUSE_STOP,
   BTBAR_OPTION_PLAY_BACK,
   BTBAR_OPTION_PLAY_STOP,
   BTBAR_OPTION_SAVE_DEL,
   BTBAR_OPTION_TODAY_BACK,
   BTBAR_SAVE_AM_BACK,
   BTBAR_SAVE_IP_DELETE,      // ����--IP---ɾ����
   BTBAR_SAVE_PM_BACK,
   BTBAR_SELECT_DEL_BACK,     // ѡ��---ɾ��---����
   BTBAR_SELECT_OK_BACK,   
   BTBAR_SELECT_OK_DEL,
   BTBAR_SELECT_SET_BACK,
   BTBAR_SNOOZE_CONTINUE_STOP,
   BTBAR_CONTINUE_RESET_BACK,
   BTBAR_CALL_MODIFY_BACK,
   BTBAR_YES_NO_CANCEL,
#ifdef FEATURE_FLEXI_STATIC_BREW_APP
   BTBAR_FNASRANI_FPORTAL,   //fnasrani--fportal
   BTBAR_FMUSLIM_FPORTAL,   //fmuslim  ---fnasrani
   BTBAR_CONTACTS_FPORTAL,   //fmuslim  ---fnasrani   
#endif /*FEATURE_FLEXI_STATIC_BREW_APP*/
   BTBAR_OPTION,

   BTBAR_MAX = 0xFFFF, /*For ADS compiler allocate one byte to enumerate type
                     variable by default if the max value not exceed 256*/

} BottomBar_e_Type;

typedef enum PromptMsg_e_Type
{
    /* �ŷ�ͼ��*/
    //����ʾ��Ϣ����
    MESSAGE_NONE,
    
    /* ���Ǽ�̾�ž���ͼ��*/
    //������Ϣ��ʾ���ײ���ʾ�����ַ�
    MESSAGE_ERR,    
    
    /* ��̾��ͼ��*/
    //�澯��Ϣ��ʾ���ײ���ʾ�����ַ����������а�������1����Զ�����  
    MESSAGE_WARNNING, 
    
    /* ɳ©ͼ��*/
    //�ȴ���Ϣ��ʾ���ײ���ʾ�����ַ����������а�������1����Զ�����     
    MESSAGE_WAITING, 
    
    /* �ʺ�ͼ��*/
    //ȷ����Ϣ��ʾ���ײ���ʾ�����ַ����������а�������1����Զ�����   
    MESSAGE_CONFIRM,
    
    /* �ŷ�ͼ��*/
    //��ʾ��Ϣ��ʾ���ײ���ʾ�����ַ����������а�������1����Զ�����  
    MESSAGE_INFORMATION,
    
    /* �ֻ��ӶԹ�ͼ��*/
    //�¼���ɺ���Ϣ��ʾ���ײ���ʾ����ȷ�ϣ����أ������а�������������������󷵻�  
    MESSAGE_INFORMATIVE           
} PromptMsg_e_Type;

typedef struct _Theme_Param_type
{
    RGBVAL  themeColor;      // ������ɫ
    RGBVAL  textColor;       // �ı���ɫ
    RGBVAL  seltextColor;    // ѡ���ı���ɫ
    RGBVAL  bkcolor;         // ������ɫ
    
    // ���в�����ʱû�ã��������ܻ��õ�
    RGBVAL  selbkcolor;           // ѡ���ı�������ɫ
    uint16  nTBarImgResID;        // ������ͼƬ����Դ�ļ��е� ID
    char    strTBarImgResFile[MAX_FILE_NAME]; // ������ͼƬ���ڵ���Դ�ļ�
    uint16  nBBarImgResID;        // �����ı�����Դ�ļ��е� ID
    char    strBBarImgResFile[MAX_FILE_NAME]; // ����ͼƬ���ڵ���Դ�ļ�
} Theme_Param_type;

typedef struct _TitleBar_Param_type
{
    TitleBar_e_Type eTBarType;          // ����������
    AEERect         *prc;               // ����������
    AECHAR          *pwszTitle;         // �������ı�
    uint16          nTitleResID;        // �������ı�����Դ�ļ��е� ID
    char            strTitleResFile[MAX_FILE_NAME]; // ������ͼƬ���ڵ���Դ�ļ�
    uint16          nImgResID;          // ������ͼƬ����Դ�ļ��е� ID
    char            strImgResFile[MAX_FILE_NAME];  // ������ͼƬ���ڵ���Դ�ļ�
    uint32          dwAlignFlags;       // �������ı����뷽ʽ
} TitleBar_Param_type;

typedef struct _BottomBar_Param_type
{
    BottomBar_e_Type    eBBarType;          // ��������
    AEERect             *prc;               // ��������
    uint16              nImgResID;          // ����ͼƬ����Դ�ļ��е� ID
    char                strImgResFile[MAX_FILE_NAME]; // ����ͼƬ���ڵ���Դ�ļ�
} BottomBar_Param_type;

typedef struct _PromptMsg_Param_type
{
    PromptMsg_e_Type    ePMsgType;          // ��ʾ��Ϣ����
    AEERect             *prc;               // ��ʾ��Ϣ����
    AECHAR              *pwszTitle;         // �������ı�
    BottomBar_e_Type    eBBarType;          // ��������
    AECHAR              *pwszMsg;           // ��ʾ��Ϣ�ı�    
    uint16              nMsgResID;          // ��ʾ��Ϣ�ַ�����Դ�ļ��е� ID
    char                strMsgResFile[MAX_FILE_NAME]; // ��ʾ��Ϣ�ַ����ڵ���Դ�ļ�
} PromptMsg_Param_type;

#define GSM_CHARACTER_SET_SIZE      0x80
extern const char GsmToUcs2[GSM_CHARACTER_SET_SIZE];

#define UCS2_TO_GSM_TABLE_SIZE      0x100
#define NON_GSM                     UCS2_TO_GSM_TABLE_SIZE
#define GSM_CHARACTER_SET_SIZE      0x80
extern  const AECHAR  Ucs2ToGsm[UCS2_TO_GSM_TABLE_SIZE];

#ifdef FEATURE_LANG_HEBREW
#define     HEBREW_CONVERT_TABLE_SIZE   0x100
#define     NON_HEB     HEBREW_CONVERT_TABLE_SIZE 

// Ucs2ToHebrew ��ʹ��˵��: 
// ��Ҫ����UNICODE�ַ� wChar ת��Ϊ Hebrew ��ֵ����ʹ�� Ucs2ToHebrew[wChar & 0x00FF]
// �����ַ� 0x203E ��0x00D7 ����ʹ�ô˱�0x203E-->0xAF ��0x00D7-->0xAA �� ����ֵ NON_HEB
// ��ʾ����Ӧ�ַ���ת��ʧ��
extern const AECHAR  Ucs2ToHebrew[HEBREW_CONVERT_TABLE_SIZE];

// HebrewToUcs2 ��ʹ��˵��: 
// ��Ҫ�����޷��� char �ַ� c ת��Ϊ Hebrew UNICODE ��ֵ����ʹ�� Ucs2ToHebrew[c] . ����ֵΪ 0����ʾת��
// ���ɹ�
extern const AECHAR  HebrewToUcs2[HEBREW_CONVERT_TABLE_SIZE];
#endif

#ifdef FEATURE_SPORTS_APP
typedef struct
{
   char*    argsz;
   boolean              runOrwalk;
   AECHAR               subject[64];
   AECHAR               location[64];
} Sport_Appointment;

#endif

/*==============================================================================

                                 ��������
                                 
==============================================================================*/

                       
/*==============================================================================
����:
    DrawBottomBar

˵��:
    ���������ڻ���������صĵײ���ʾ����

����:
    pIDisplay [in]: ָ�� IDisplay �ӿڵ�ָ�롣
    BParam [in]: �ײ���ʾ����ز���ָ�롣

����ֵ:
    none

��ע:
    ���������ڻ���������صĵײ���ʾ���������޹صĵײ���ʾͼƬ�������л��ƣ���
    Ҫ���ô˺�����ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ����
    ֧�ֵײ���3����ť��ʾ��
==============================================================================*/
void DrawBottomBar(IDisplay  * pIDisplay, BottomBar_Param_type *BParam);
                       
/*==============================================================================
����:
    DrawTitleBar

˵��:
    ���������ڻ�������ʾ����

����:
    pIDisplay [in]: ָ�� IDisplay �ӿڵ�ָ�롣
    TParam [in]: ��������ز���ָ�롣

����ֵ:
    none

��ע:
    ���������ڻ�����������ʾ��ͼƬ���Է���Դ�ļ����������ص��ֻ������Ƶı����ı�
    ���ɲ������롣ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ�
==============================================================================*/
void DrawTitleBar(IDisplay  * pIDisplay, TitleBar_Param_type *TParam);

/*==============================================================================
����:
    DrawTextWithProfile

˵��:
    �������ڻ��ƴ��������ı���

����:
    pShell [in]��IShell �ӿ�ָ�롣
    pDisplay [in]��IDisplay �ӿ�ָ�롣
    ProfileColor [in]���ı�������RGBVAL��ɫֵ��
    ���� Font��pcText��nChars��x��y��prcBackground��dwFlags ����ͬ�ӿں���
    IDISPLAY_DrawText ��

����ֵ:
    none

��ע:
    ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ�
==============================================================================*/
void DrawTextWithProfile(IShell* pShell, 
    IDisplay * pDisplay, 
    RGBVAL ProfileColor, 
    AEEFont Font, 
    const AECHAR * pcText, 
    int nChars,
    int x, 
    int y, 
    const AEERect * prcBackground, 
    uint32 dwFlags
);

/*==============================================================================
����:
    DrawGreyBitTextWithProfile

˵��:
    �������ڻ��ƴ��������ı���

����:
    pShell [in]��IShell �ӿ�ָ�롣
    pDisplay [in]��IDisplay �ӿ�ָ�롣
    ProfileColor [in]���ı�������RGBVAL��ɫֵ��
    ���� Font��pcText��nChars��x��y��prcBackground��dwFlags ����ͬ�ӿں���
    IDISPLAY_DrawText ��

����ֵ:
    none

��ע:
    ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ�
==============================================================================*/
void DrawGreyBitTextWithProfile(IShell* pShell, 
    IDisplay * pDisplay, 
    RGBVAL ProfileColor, 
    int Font, 
    const AECHAR * pcText, 
    int nChars,
    int x, 
    int y, 
    const AEERect * prcBackground, 
    uint32 dwFlags
);


/*==============================================================================
����:
    EncodePWDToUint16

˵��:
    �������ַ���ת��Ϊ�޷�����������

����:
    pszPWD [in]�������ַ�����

����ֵ:
    none

��ע:
    �����ַ����������Ϊ8����ֻ���ַ�0-9��*��#
==============================================================================*/
uint16 EncodePWDToUint16(char *pszPWD);

/*==============================================================================
����:
    Appscom_GetThemeParameters

˵��:
    �������ڻ�ȡ��ǰ���õ�������ز�����

����:
    TParam [in/out]: ���ڷ��ؽ���Ĳ���ָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
void Appscom_GetThemeParameters(Theme_Param_type *TParam);

/*==============================================================================
����:
    GetTitleBarHeight

˵��:
    ���������ڻ�ȡ������ʾ���ĸ߶ȡ�

����:
    pIDisplay [in]: ָ�� IDisplay �ӿڵ�ָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
int GetTitleBarHeight(IDisplay  * pIDisplay);

/*==============================================================================
����:
    GetBottomBarHeight

˵��:
    ���������ڻ�ȡ�ײ���ʾ���ĸ߶ȡ�

����:
    pIDisplay [in]: ָ�� IDisplay �ӿڵ�ָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
int GetBottomBarHeight(IDisplay  * pIDisplay);

/*==============================================================================
����:
    DrawPromptMessage

˵��:
    �������ڻ�ȡ��ǰ���õ�������ز�����

����:
    pIDisplay  [in]��IDisplayָ�롣
    pStatic    [in]: IStatic�ؼ���  
    PParam     [in]: ��ʾ��Ϣ��ز���ָ�롣

����ֵ:
    none

��ע:
    ���������ڻ�������Ϣ��ʾ�򡣵����߱��뽨һ������static�ؼ���dialog������dialog�ľ��ο�ȫ����Ϊ0��Ȼ����ô˺���
    ʵ����Ϣ�ַ������ͼƬ����ʾ����ʾ��ɺ��״̬������ͼ�ʱ���Ĵ�����dialog������ɣ�������������
    ״̬���Ĵ���ͼ�ʱ���Ĵ���
    ÿ����Ϣ�����в�ͬ���ô���������Ҫ��׼��Ϣ���ͣ�ȷ�����һ�£�
    
    1.MESSAGE_NONE��
    �����κ�ͼƬ��
    
    2.MESSAGE_ERR/MESSAGE_WARNNING��
    For tempo screen, there are no soft keys displayed. 
    The prompt screen will display for 3 seconds and then back to the previous screen 
    automatically. User can also press any key to dismiss the pop-up screen. 
    and back to the previous screen.  
      
    3.MESSAGE_WAITING��
    For the waiting screens there are no soft keys displayed. 
    The wait clock is displayed in full screen. 
    E.g. modules: SMS initializing, phonebook loading and other normal waiting modules 
       
    4.MESSAGE_CONFIRM��
    When the user has to make a key press to confirm or not an action, 
    two soft keys are displayed Icon used is not fixed, The sentence should be finished by question mark.
    E.g. modules: Confirm Delete, Save Entry? , Confirm Reset
    
    5.MESSAGE_INFORMATION��
    For tempo screen, there are no soft keys displayed��
    E.g. modules: Information of positive actions. Deleted, Saved, Copied, Done, Moved, 
    Timer Reset, Activated, Deactivated, Pin OK, Pin Changed. The prompt screen will 
    display for 500ms and then back to the previous screen automatically. 
    User can also press any key to dismiss the pop-up screen and back to the previous screen.
    
    6.MESSAGE_INFORMATIVE��
    For tempo screen, there are no soft keys displayed��
    E.g. modules: Call Timer, Call Cost  Any missed screens, 
    The prompt screen will display for 500ms 
    and then back to the previous screen automatically. User can also press any 
    key to dismiss the pop-up screen, and back to the previous screen.  
   
==============================================================================*/
void DrawPromptMessage (IDisplay *pIDisplay,
                        IStatic *pStatic,
                        PromptMsg_Param_type *PParam);

/*==============================================================================
����:
    DrawBackground

˵��:
    ���������ڻ��ؼ�����ͼ��

����:
    pDisplay   [in]��IDisplayָ�롣
    pRect      [in]��Ŀ�������С��

����ֵ:
    none

��ע:
    ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ�
==============================================================================*/
void DrawBackground( IDisplay *pDisplay, AEERect *pRect);
#ifdef FEATURE_KEYGUARD
void Appscomm_Draw_Keyguard_Msg(IDisplay *pIDisplay,IStatic *pStatic,boolean unlockkey);
#endif

#ifdef FEATURE_KEYGUARD
void Appscommon_Draw_Keyguard_Time(IDisplay *pIDisplay);
#endif

void DrawBottomBar_Ex(IShell    *m_pIShell, IDisplay  * pIDisplay, BottomBar_e_Type    eBBarType);

/*==============================================================================
����: 
    DecodeAlphaString
       
˵��: 
    ���������� Alpha ���ַ�����
       
����: 
    pdata [in]��ָ�����������ֽ����С�
    nLen [in]�����������ֽ����г���,��λ�ֽڡ�
    wstrOut [out]�� ���Buffer
    nBufSize [in]:  ���Buffer��С,��λ AECHAR
       
����ֵ:
    none
       
��ע:
       
==============================================================================*/
void DecodeAlphaString(byte *pdata, int nLen, AECHAR *wstrOut, int nBufSize);
int Appscomm_is_incoming_state(int bb);

 void SetArrowFlagonIM(boolean bFlag);
 boolean GetArrowFlagonIM(void);

/*==============================================================================
����: 
    Appscommon_DrawDialogBoxFrame
       
˵��: 
    ���Ƶ����Ի���߿򼰱���ɫ��
       
����: 
    pBoxRect [in]: ���뽫Ҫ���ƶԻ���ľ��η�Χ�� 
    bUpward [in]: �Ի����ͷ�������Ƿ����Ϸ���
    nFrameColor [in]: �Ի���߿���ɫ��
    nBgColor [in]: �Ի��򱳾���ɫ
       
����ֵ:
    TRUE: ���Ƴɹ�
    FALSE: ����ʧ��
       
��ע:
       
==============================================================================*/
boolean Appscommon_DrawDialogBoxFrame(AEERect *pBoxRect, boolean bUpward, RGBVAL nFrameColor, RGBVAL nBgColor);//added by  chengxiao 2009.02.16

/*==============================================================================
����: 
    Appscommon_DrawPopUpDialogBox
       
˵��: 
    ���Ƶ����Ի���(���У����ı�)��
       
����: 
    pIDisplay [in]:
    x [in]: ���뽫Ҫ���ƶԻ������Ͻ�x���ꡣ
    y [in]: ���뽫Ҫ���ƶԻ������Ͻ�y���ꡣ
    strDisplay[in]:���뽫Ҫ���ƶԻ����ı����ݡ�
       
����ֵ:
    TRUE: ���Ƴɹ�
    FALSE: ����ʧ��
       
��ע:
    �����ں�������֮ǰʹ��IDISPLAY_SetColor�����ı���ɫ�������ʾ����Ϊ
    ���֮����Ļ����ʾ���x��С�����֮һ��Ļ����ʾ���y������
    menu���һ������ʾ����ʼ���꣬ �����Ľ��Զ�������
       
==============================================================================*/
boolean Appscommon_DrawPopUpDialogBox(IDisplay *pIDisplay, int x, int y, AECHAR *strDisplay);//added by  chengxiao 2008.10.31

/*==============================================================================
����: 
    Appscommon_DrawDigitalNumber
       
˵��: 
    ���Ʒ��߶����������
       
����: 
    pDisplay [in]: 
    number[in]: ��Ҫ���Ƶ����֡�
    nLineWidth[in]: ��Ҫ���Ƶ�����������ȡ�
    fontRect [in]: �������ֵı߽���ο�
    fontColor[in]: ��Ҫ���Ƶ�������ɫ��
       
����ֵ:
    FALSE: ����ʧ��
    TRUE: ���Ƴɹ�
       
��ע:
       
==============================================================================*/
boolean Appscommon_DrawDigitalNumber (IDisplay *pDisplay, int number, int nLineWidth, AEERect *fontRect, RGBVAL fontColor);//added by chengxiao 2008.12.05


/*=============================================================================

FUNCTION: WStr2Alpha

DESCRIPTION: 
    ���ַ���ת��Ϊ��Ӧ�İ����ַ���
    
PARAMETERS:
    pDest:
    nLen : Ŀ���ַ������� in AECHAR
    pSrc :
    
RETURN VALUE:
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:
=============================================================================*/
void WStr2Alpha(AECHAR *pDest, int nLen , AECHAR *pSrc);


/*=============================================================================

FUNCTION: AlphaNCMP

DESCRIPTION: 
    �Ƚ����� Alpha �ַ����Ƚϳ���, NULL �ַ���β,*��ͨ���
    
PARAMETERS:
    pStr1:
    pStr2:
    nLen : �Ƚϵĳ��ȣ�in AECHAR
        
RETURN VALUE:
    boolean
        
COMMENTS:
    
SIDE EFFECTS:

SEE ALSO:
=============================================================================*/
boolean AlphaNCMP(AECHAR *pStr1, AECHAR *pStr2, int nLen);

/*==============================================================================
����: 
    Appscommon_CompareName

˵��: 
    ���������ڱȽ������ַ����Ա�����

����: 
    wstrNam1 [in]: �ַ���1��
    wstrNam2 [in]: �ַ���2��
����ֵ: 
    1 : wstrNam1>wstrNam2
    0 : wstrNam1==wstrNam2
    -1: wstrNam1<wstrNam2

��ע:
    ����ǵ绰������ OEM ���ڳ�ʼ�� Cache ʱ����ô˺������� Cache ����UI �㲻��������
    ����ģ���������������

==============================================================================*/
int Appscommon_CompareName(AECHAR *wstrNam1,AECHAR *wstrNam2);


/*��ȡ�������ֵĿ��*/
int GetSingleNumberWidth(IDisplay *pDisplay, AEEFont Font);
boolean IsDoubleBytesIME(AEETextInputMode imnputmode);
boolean HaveNoneASCIIChar ( AECHAR * pWstr, int * pPostion);

/*==============================================================================
����: 
    Appscommon_ResetBackground

˵��: 
    ����ĳ������ı���ͼƬ��

����: 
    pDisplay [in]: 
    BgImage [in]: ����ͼƬָ��
    BgColor [in]: ������ɫ
    rect [in]: ��Ҫ���Ƶľ��η�Χ
    x,y[in]: ͼƬ���Ͻ�����Ļ�е�����������������
    
����ֵ: 
    ��

��ע:
    �������ͼƬ��Ϊ�գ������ͼƬ��ָ�����η�Χ��
    ����ָ����Χ���Ϊ����ɫ��

==============================================================================*/
void Appscommon_ResetBackground(IDisplay *pDisplay, IImage *BgImage, RGBVAL BgColor, AEERect * rect, int x, int y);
/*==============================================================================
����: 
    Appscommon_DrawScrollBar

˵��: 
    ���ƹ�������

����: 
    pDisplay [in]: 
    nCurrentIdx [in]: ��ǰҳ��������һ������ֵ(��1Ϊ���)
    nTotalItem [in]: ��Ŀ����
    nPageItem[in]: ����Ļ�����ʾ��Ŀ��
    ScrollRect [in]: ��Ҫ���Ƶľ��η�Χ
    
����ֵ: 
    ��

��ע:

==============================================================================*/
void Appscommon_DrawScrollBar(IDisplay *pDisplay, int nCurrentIdx, int nTotalItem, int nPageItem, AEERect *ScrollRect);

// ��װһ������ת��Ϊ��ĸ
#ifdef FEATURE_LANG_CHINESE
AECHAR CNUnicode2Letter(AECHAR c);
/*=============================================================================

Macro : Letter2Alpha

DESCRIPTION: 
    ��Ӣ����ĸת��Ϊ��Ӧ�İ����ַ�
    
PARAMETERS:
    letter:
    
=============================================================================*/
AECHAR CNLetter2Alpha(AECHAR letter);

#endif
//added by chengxiao 2009.02.19
/*==============================================================================
����: 
    Appscommon_GetRandomColor

˵��: 
    ���ƹ�������

����: 
    nMaxVal [in]: ����ƽ��ֵ���ֵ���������ƻ�ȡɫ�ʵ����ȣ�
    �粻��Ҫ���ƣ�����Ϊ255����
    
����ֵ: 
    ��õ���ɫRGBֵ

��ע:

==============================================================================*/
RGBVAL Appscommon_GetRandomColor(uint32 nMaxVal);

/*==============================================================================
����: 
    Appscommon_ResetBackgroundEx

˵��: 
    ����ĳ������ı���ɫ�Լ�ͼƬ��

����: 
    pDisplay [in]: 
    rect [in]: ��Ҫ���Ƶľ��η�Χ
    bDrawImage[in] : �Ƿ����ͼƬ��FALSE����䱳��ɫ
    
����ֵ: 
    ��

��ע:
    ��ָ����Χ���ΪĬ�ϵı���ɫAPPSCOMMON_BG_COLOR������������ΪTRUEʱ��
    ����Ĭ�ϱ���ͼƬ��

==============================================================================*/
void Appscommon_ResetBackgroundEx(IDisplay *pDisplay, AEERect * rect, boolean bDrawImage);

int Appscommon_bluetooth_used_sd(int aa);
/*==============================================================================
����:
    IPAdd_FromNetValueToString

˵��:
    ������IP��ַ������ֵת��Ϊ�ַ�����ʽ��

����:
    dwAddNet [in]��IP��ַ����ֵ��
    pszbuf [in/out]�����ؽ���� Buffer��
    nSize [in]��Buffer ��С��

����ֵ:
    TRUE: ת���ɹ�
    FALSE:ת��ʧ��

��ע:

==============================================================================*/
boolean IPAdd_FromNetValueToString(uint32 dwAddNet, char *pszbuf, int nSize);

/*==============================================================================
����:
    IPAdd_FromStringToNetValue

˵��:
    ������IP��ַ���ַ�����ʽת��Ϊ����ֵ��

����:
    pdwAddNet [in/out]������IP��ַ����ֵ�� Buffer��
    pszbuf [in]���ַ�����ʽ��IP��ַ��

����ֵ:
    TRUE: ת���ɹ�
    FALSE:ת��ʧ��

��ע:

==============================================================================*/
boolean IPAdd_FromStringToNetValue(uint32 *pdwAddNet, char *pszbuf);

/*==============================================================================
����:
    Appscommon_FormatTimeString

˵��:
    ������ʱ��ת��Ϊ�ַ�����ʾ��ʽ��

����:
    nSeconds [in]����Ҫת����ʱ�䡣
    wstrTime [in]������ַ��������ַ��
    nTimeStringLength[in]������ַ������������ȡ�

����ֵ:
    TRUE: ת���ɹ�
    FALSE:ת��ʧ��

��ע:

==============================================================================*/
boolean Appscommon_FormatTimeString(uint32 nSeconds, AECHAR* wstrTime, int nTimeStringLength);


#define APP_MEDIA_ALLOW         0    
#define APP_MEDIA_IMPACT_BY_FM  1
#define APP_MEDIA_IMPACT_BY_MP3 2
extern int app_media_scheduler(void);

#endif // APPSCOMMON_H
