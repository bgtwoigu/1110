#ifndef DISPLAYMENUPRIV_H
#define DISPLAYMENUPRIV_H

/*==============================================================================
// �ļ���
//        DisplayMenu_priv.h
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//
// �ļ�˵����
//
// ���ߣ�Gemsea
// �������ڣ�2007-11-01
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��    ���ⵥ��  �޸����ݡ�λ�ü�ԭ��
-----------      ----------  --------  -----------------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#ifndef AEE_SIMULATOR
#include "customer.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif

#if !defined(BREW_STATIC_APP)
// Use the .bid file if compiling as dynamic applet
   #include "DisplayMenu.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_APP_DISPLAYMENU
      #error AEECLSID_APP_DISPLAYMENU must be defined
   #endif
#endif

#include "DisplayMenu.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
//#include "OEMMacros.h"
#include "DisplayMenu.h"

#include "AEEAnnunciator.h"

#include "AEEMenu.h"
#include "AEEConfig.h"
#include "AppComFunc.h"
#include "Appscommon.h"
#include "OEMText.h"
#include "AEEBacklight.h"
#include "AEEFile.h"
#ifndef WIN32
#include "Oemui.h"
#include "db.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32
#include "OEMCFGI.h"

#ifdef FEATURE_RANDOM_MENU_REND//wlh 20090405 add for rend
#include "Rendering.h"
#endif//FEATURE_RANDOM_MENU_REND
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define DISPLAYMENU_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif//FEATURE_LCD_TOUCH_ENABLE

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
#define  AEE_APPSDISPLAYMENU_RES_FILE AEE_RES_LANGDIR DISPLAYMENU_RES_FILE
#define DISP_ERR(format, code1, code2, code3) //ERR(format, code1, code2, code3)
// ������ײ���ʾ���궨��
#define DISPLAYMENU_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}
/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
    DLGRET_CREATE,
    DLGRET_OK,
    DLGRET_CANCELED,
    DLGRET_BACKLIGHTING,  //IDD_BACKLIGHTING_MENU
    DLGRET_CONTRAST,      //IDD_CONTRAST_MENU
    DLGRET_BRIGHTNESS = DLGRET_CONTRAST, //IDD_CONTRAST_MENU
    DLGRET_SCREENTIMEOUT, //IDD_SCREENSAVERTIMEOUT_DIALOG
    DLGRET_PICTURE,       //IDD_PICTURE_DLG
    DLGRET_IMGDELMSG,      //IDD_IMGDELMSG_DLG
    DLGRET_DESKTOP_THEME,//IDD_DESKTOP_THEME
    DLGRET_LED_CONTROL  , 
    DLGRET_KEY_PAD_CTL ,
    DLGRET_MENU_STYLE_CTL,
#ifdef FEATURE_RANDOM_MENU_COLOR
    DLGRET_COLORFUL_MENU_CTL,
    DLGRET_COLOR_SETTING,
#endif
#ifdef FEATURE_RANDOM_MENU_REND//wlh 20090405 add for rend
    DLGRET_REND_MENU_CTL,
#endif
    DLGRET_MSG_POP
} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// DisplayMenu Applet ״̬��״̬��
typedef enum _DisplayMenuState
{
    DISPLAYMENUST_NONE,
    DISPLAYMENUST_INIT,
    DISPLAYMENUST_MAIN,
    DISPLAYMENUST_BACKLIGHTING,
    DISPLAYMENUST_CONTRAST,    
    DISPLAYMENUST_SCREENTIMEOUT,
    DISPLAYMENUST_PICTURE,
    DISPLAYMENUST_IMGDELMSG,    
    DISPLAYMENUST_DESKTOP_THEME,
    DISPLAYMENUST_LED_CONTROL,
    DISPLAYMENUST_KEY_PAD_CONTROL,
    DISPLAYMENUST_MENU_STYLE_CONTROL,
#ifdef FEATURE_RANDOM_MENU_COLOR
    DISPLAYMENUST_COLORFUL_MENU,
    DISPLAYMENUST_COLOR_SETTING,
#endif
#ifdef FEATURE_RANDOM_MENU_REND//wlh 20090405 add for rend
    DISPLAYMENUST_REND_MENU,
#endif
    DISPLAYMENUST_EXIT

} DisplayMenuState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
    NFSMACTION_WAIT,
    NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct DisplayMenuMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} DisplayMenuMod;

// ǽֽ����, ��/�ػ�����
typedef enum {
     WALLPAPER_NONE
    ,WALLPAPER_MAIN      /*//��������ͼ*/
    ,WALLPAPER_SUB        /*//С������ͼ*/
#ifdef FEATURE_ANIMATION_POWERUPDOWN
    ,ANIMATION_POWERUP   /*//��������*/
    ,ANIMATION_POWERDOWN /*//�ػ�����*/
#endif//FEATURE_ANIMATION_POWERUPDOWN

#ifdef FEATURE_SCREEN_SAVE
    ,SCREENSAVE_TYPE    /*//����ǽֽ*/
#endif
} PICTURE_TYPE;

//ͼƬ��ʽ����
typedef enum
{
    IMAGE_NONE,
    IMAGE_BMP,          //bmp
    IMAGE_PNG,          //png
    IMAGE_JPEG,         //jpg
    IMAGE_GIF,          //gif
    IMAGE_BCI,        //bci
    IMAGE_LAST
} ImageType;

typedef enum {
   SET_CONTRAST,
   SET_BACKLIGHT
} SET_CONTRAST_BACKLIGHT_TYPE;

typedef struct  _image_name_table
{
    char imange_name[MAX_FILE_NAME+1];//65
    struct _image_name_table *pref;
    struct _image_name_table *next;
}image_name_table;

typedef struct _double_list
{
    image_name_table *phead;
    image_name_table *ptail;
    image_name_table *pcur;
    int16            count;
}double_list;

// DisplayMenu Applet����ṹ�壺
typedef struct _CDisplayMenu
{
    DECLARE_VTBL(IDisplayMenu)
    uint32          m_nRefs;          // �������ü�����
    IModule        *m_pModule;
    IDisplay       *m_pDisplay;
    IShell         *m_pShell;
    IAnnunciator *m_pIAnn;

    AEERect         m_rc;
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog        *m_pActiveDlg;
    // ��ǰ��Ի���ID
    uint16          m_pActiveDlgID;
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRet_Value_e_Type m_eDlgRet;
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean          m_bNotOverwriteDlgRet;
    // Applet ǰһ״̬
    DisplayMenuState m_ePreState;
    // Applet ��ǰ״̬
    DisplayMenuState m_eCurState;
    // Applet�Ƿ��ڹ���״̬
    boolean         m_bSuspending;
    // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean         m_bAppIsReady;

    uint16          m_currDlgId;       /*���浱ǰ��ѡ�˵�λ��*/
    uint16          m_nSubDlgId;       /*�μ��˵�λ��*/
    uint16          m_nExSubDlgId;    /* �������˵�λ��*/
    IConfig         *m_pConfig;        /*IConfig ָ��*/
    byte            m_lightlevel;      /*����*/
    //byte            m_contrastlevel;     /* //�Աȶ�*/
    IBacklight      *m_pIBacklight;   /* �����������걳���������Ч��*/
#if defined(FEATURE_BACKLIGHT_KEYPAD)
    IBacklight      *m_pKeypadBacklight;   /* �����������갴���ƿ���ʱ���������Ч��*/
#endif
    IFileMgr        *m_pIFileMgr;     /* IFILEMGRָ��*/
    PICTURE_TYPE     m_PICType;        /* ǽֽ����,��/�ػ�����*/
    image_name_table *m_CurPaper;       /* Ŀǰ��ʾ��ǽֽ*/
    boolean          m_DelImageBack;   /* ����ͼƬNV�����Ƿ�λ��nvͼƬ*/
    //uint16          m_WallCount;      /* ��ͨǽֽ/��������*/
    //uint16          m_Downcount;      /* ����ǽֽ����*/
    IImage           *m_pWallPaper;    /* ǽֽIImageָ��*/
    IImage           *m_pAniPaper;     /*���ػ�����IImageָ��*/
    //IImage           *m_pTitleImg;     /* title����IImageָ��*/
    boolean          m_bOkToDo;        /* ���ͼƬʱ���ư����ٶ�*/
    //boolean         m_contrastAdjust;  // Adjusting contrast?
    //boolean         m_storeInit;       // Store this contrast level?
    //byte            m_initLevel;       // Initial contrast level
    //byte            m_newLevel;        // New contrast level
    //IOEM_TSGBridge  *m_pOEM_TSGBridge; // IOEMTSG_Bridge interface
    //int16           m_controlId;
    AEEApplet         a;
    SET_CONTRAST_BACKLIGHT_TYPE  m_contrast_backlight;
    byte m_BiasV;
    uint16            m_count;
    boolean           m_b_local;
#ifdef FEATURE_LED_CONTROL
    byte              m_led_ctr;
#endif
#ifdef FEATRUE_KEY_PAD_CTL
    //ClockCtl        m_ClockCtlStart;
    //ClockCtl        m_ClockCtlEnd;
    ITimeCtl          *m_TimeCtlStart;
    ITimeCtl          *m_TimeCtlEnd;
    byte              m_SetTimeRange;
    Key_pad_Cfg       m_ClockCfg;
    IMenuCtl*         m_pState;
    uint16            m_nCtlID;           //�ؼ�ID
    ClockCtlFocus     m_focus;    
    uint16           m_nTimeCtlCount;    //0��������Сʱ��,1�����ڷ�����
    uint16           m_nNumKeyCount;     //��¼������ʱ��ʱ�İ�������.0:Сʱʮλ    
#endif
    uint16            m_msg_id;

#ifdef FEATURE_RANDOM_MENU_COLOR
    RGBVAL         m_nCFGColor; 
    boolean         m_bCustomColor;
#endif
#ifdef FEATURE_RANDOM_MENU_REND//wlh 20090405 add for REND
    uint32          m_nCFGRend;//��ǰЧ�� 
    boolean         m_bRandomRend;//�Ƿ��������Ч��
#endif
} CDisplayMenu;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// DisplayMenu_APPIsReadyTimer����
#define APPISREADY_TIMER       100                        
#define EVT_APPISREADY        (EVT_USER+1)               
#ifdef FEATRUE_KEY_PAD_CTL
#define LINEHEIGHTEX          (IDISPLAY_GetFontMetrics(pMe->m_pDisplay,AEE_FONT_NORMAL,NULL,NULL) +2)
#define TEXT_X                (LINEHEIGHTEX/2)  
#define TEXT_DX               (40-TEXT_X)
#define CONTROL_X             40
#define CONTROL_DX            (pMe->m_rc.dx-40)
#endif
#define DISPLAY_MENU_SOFK_HIGHT 0
// �û����¸��½����¼�
//#define EVT_USER_REDRAW   (EVT_USER+2)

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            DisplayMenuState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

#define  NUM_OF_STARTUP_ANIMATION_ITEM    5       //��������ѡ��
#define  NUM_OF_POWEROFF_ANIMATION_ITEM   5       //�ػ�����ѡ��
#define  MAX_WALLPAPER_NUMBER             100     //����ǽֽ��������
#define  FRAME_NUM_OF_ANIMATION           8       //������֡��
#define  ANIMATION_TIMER                  300     //������֡��

#define  BANNER_MAXTXT                    8       //�����ʺ�������ַ���

#define SCREENSAVERTIME_30_SECONDS     (30)
#define SCREENSAVERTIME_2_MINUTES  (2 * 60)
#define SCREENSAVERTIME_5_MINUTES  (5 * 60)
/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
       DisplayMenu_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��DisplayMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void DisplayMenu_ShowDialog(CDisplayMenu *pMe,uint16  dlgResId);

/*==============================================================================
����:
       DisplayMenu_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��DisplayMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean DisplayMenu_RouteDialogEvent(CDisplayMenu *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       DisplayMenu_ProcessState
˵��:
       DisplayMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��DisplayMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction DisplayMenu_ProcessState(CDisplayMenu *pMe);

#endif
//DISPLAYMENUPRIV_H

