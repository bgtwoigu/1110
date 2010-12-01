/** ----------------------------------------------------------------------------
 * mainmenu_priv.h
 *
 * Copyright(c) 2007 Anylook
 *
 *
 * History:
 *
 * when            who               what and why
 * -----------  -----------    -----------------------------
 * 2008.12.24                created  for  Q1
 *
 * -----------------------------------------------------------------------------
 */

#ifndef MAINMENU_PRIV_H
#define MAINMENU_PRIV_H
#endif

#include "mainmenu.brh"
#include "AEEConfig.h"
#include "AEEAnnunciator.h"

#if defined (FEATURE_DISP_160X128)
#define  MAX_MATRIX_ITEMS 12
#define  MAX_MATRIX_ROWS 3
#define  MAX_MATRIX_COLS 4
#elif defined (FEATURE_DISP_220X176)
#define  MAX_MATRIX_ITEMS 12
#define  MAX_MATRIX_ROWS 3
#define  MAX_MATRIX_COLS 4
#elif defined (FEATURE_DISP_128X128)
#define  MAX_MATRIX_ITEMS 9
#define  MAX_MATRIX_ROWS 3
#define  MAX_MATRIX_COLS 3
#elif defined (FEATURE_DISP_128X160)
#define  MAX_MATRIX_ITEMS 12
#define  MAX_MATRIX_ROWS 4
#define  MAX_MATRIX_COLS 3
#elif defined (FEATURE_DISP_176X220)
#define  MAX_MATRIX_ITEMS 12
#define  MAX_MATRIX_ROWS 4
#define  MAX_MATRIX_COLS 3
#elif defined (FEATURE_DISP_240X320)
#define  MAX_MATRIX_ITEMS 12
#define  MAX_MATRIX_ROWS 4
#define  MAX_MATRIX_COLS 3
#elif defined (FEATURE_DISP_320X240)
#define  MAX_MATRIX_ITEMS 12
#define  MAX_MATRIX_ROWS 3
#define  MAX_MATRIX_COLS 4
#endif

/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/

typedef struct _MainMenuMod
{
    DECLARE_VTBL( IModule)

    uint32   referenceCounter;
    
} MainMenuMod;

//MAINMENU  �µļ���״̬
typedef enum MainmenuState
{
    MAINST_MAIN,
    MAINST_EXIT
} MainmenuState;

typedef enum
{
    MAINMENU_STOP=0x00,
    MAINMENU_RUNNING,
    MAINMENU_SUSPEND
} MAINMENU_STATUS_e_type;

typedef enum DLGRetValue
{
    DLGRET_CREATE,
    DLGRET_CANCELED
}DLGRetValue;

typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
   
} NextFSMAction;


typedef struct _Point
{
    int x;
    int y;
} Point;

typedef struct _MainMenu
{
    DECLARE_VTBL(IMainMenu)
    uint32          referenceCounter;
    
    IModule        *m_pModule;
    IDisplay       *m_pDisplay;
    IShell         *m_pShell;
    IConfig        *m_pConfig;

    IDialog        *m_pActiveIDlg;
    uint32          m_pActivedlgID;
    DLGRetValue     m_eDlgReturn;
    
    int             m_nRow;
    int             m_nColumn;
    
    MAINMENU_STATUS_e_type  m_eAppStatus;   // Applet ��ǰ����״̬
    
    boolean         m_bDoNotOverwriteDlgResult; //�Ƿ���д�Ի�����    
    
    IImage         *m_pImageBg;
    IImage         *m_pImageIcon[MAX_MATRIX_ITEMS];
    IImage         *m_pAnimate;
    Point           m_Icondefault_Pt[MAX_MATRIX_ITEMS];
    Point           m_IconFocus_Pt[MAX_MATRIX_ITEMS];
    uint16          m_IconTitle[MAX_MATRIX_ITEMS];
    AEERect         m_rc;
    
    MainmenuState   m_prevState;        // Appletǰһ״̬
    MainmenuState   m_currState;        // Applet��ǰ״̬
    
#ifdef FEATURE_RANDOM_MENU_COLOR
    RGBVAL          m_nBgColor;
    boolean         m_nRandomMenu;
#endif
	IAnnunciator   *m_pIAnn;
} MainMenu;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
#if defined (FEATURE_DISP_160X128)
#define ICON_WIDTH              40
#define ICON_HEIGHT             32
#define ICON_ANIMATED_WIDTH     40
#define ICON_ANIMATED_HEIGHT    32
#elif defined (FEATURE_DISP_128X128)
#if defined FEATURE_VERSION_H19C  
#define ICON_WIDTH              26
#define ICON_HEIGHT             26
#define ICON_ANIMATED_WIDTH     26
#define ICON_ANIMATED_HEIGHT    26
#else
#define ICON_WIDTH              42
#define ICON_HEIGHT             32
#define ICON_ANIMATED_WIDTH     42
#define ICON_ANIMATED_HEIGHT    32
#endif
#elif defined (FEATURE_DISP_220X176)
#define ICON_WIDTH              50 //40
#define ICON_HEIGHT             40 //32
#define ICON_ANIMATED_WIDTH     50 //40
#define ICON_ANIMATED_HEIGHT    40 //32
#elif defined (FEATURE_DISP_128X160)
#define ICON_WIDTH              40
#define ICON_HEIGHT             32
#define ICON_ANIMATED_WIDTH     40
#define ICON_ANIMATED_HEIGHT    32
#elif defined (FEATURE_DISP_176X220)
#define ICON_WIDTH              40
#define ICON_HEIGHT             32
#define ICON_ANIMATED_WIDTH     40
#define ICON_ANIMATED_HEIGHT    32
#elif defined (FEATURE_DISP_240X320)
#define ICON_WIDTH              40
#define ICON_HEIGHT             32
#define ICON_ANIMATED_WIDTH     40
#define ICON_ANIMATED_HEIGHT    32
#elif defined (FEATURE_DISP_320X240)
#define ICON_WIDTH              40
#define ICON_HEIGHT             32
#define ICON_ANIMATED_WIDTH     40
#define ICON_ANIMATED_HEIGHT    32
#endif

#define MAINMENU_RES_FILE_LANG        AEE_RES_LANGDIR MAINMENU_RES_FILE
#define MAINMENU_RES_FILE_IMAGE       MAINMENUIMAGES_RES_FILE

#define STARTARGPREFIX_VIEWPLAYER            'P'
#define STARTARGPREFIX_VIEWDATA              'D'

#ifndef FEATURE_DISP_128X128


	#define ICON1_ANI      "fs:/image/mainmenu/qsc1100_01.png"
    #define ICON2_ANI      "fs:/image/mainmenu/qsc1100_02.png"
    #define ICON3_ANI      "fs:/image/mainmenu/qsc1100_03.png"
    #define ICON4_ANI      "fs:/image/mainmenu/qsc1100_04.png"
    #define ICON5_ANI      "fs:/image/mainmenu/qsc1100_05.png"
    #define ICON6_ANI      "fs:/image/mainmenu/qsc1100_06.png"
    #define ICON7_ANI      "fs:/image/mainmenu/qsc1100_07.png"
    #define ICON8_ANI      "fs:/image/mainmenu/qsc1100_08.png"
    #define ICON9_ANI      "fs:/image/mainmenu/qsc1100_09.png"
    #define ICON10_ANI     "fs:/image/mainmenu/qsc1100_10.png"
    #define ICON11_ANI     "fs:/image/mainmenu/qsc1100_11.png"
    #define ICON12_ANI     "fs:/image/mainmenu/qsc1100_12.png"
    
#endif
#if defined (FEATURE_DISP_128X128)
#if defined FEATURE_VERSION_H19C    
    #define ICON_ANI_BG    "fs:/image/mainmenu/Backgroud.png"
#else
    #define ICON1_ANI      "fs:/image/mainmenu/MediaGallery.png"
    #define ICON2_ANI      "fs:/image/mainmenu/Contact.png"
    #define ICON3_ANI      "fs:/image/mainmenu/UTK.png"
    #define ICON4_ANI      "fs:/image/mainmenu/FmRadio.png"
    #define ICON5_ANI      "fs:/image/mainmenu/Message_Mainmenu.png"
    #define ICON6_ANI      "fs:/image/mainmenu/Recentcall.png"
    #define ICON7_ANI      "fs:/image/mainmenu/MusicPlayer.png"
    #define ICON8_ANI      "fs:/image/mainmenu/Setting.png"
    #define ICON9_ANI      "fs:/image/mainmenu/application.png"
#endif
#endif



#ifndef FEATURE_DISP_128X128
	#define ICON1_ANI_1      "fs:/image/mainmenu/qsc1100_01_focus.png"
    #define ICON2_ANI_1      "fs:/image/mainmenu/qsc1100_02_focus.png"
    #define ICON3_ANI_1      "fs:/image/mainmenu/qsc1100_03_focus.png"
    #define ICON4_ANI_1      "fs:/image/mainmenu/qsc1100_04_focus.png"
    #define ICON5_ANI_1      "fs:/image/mainmenu/qsc1100_05_focus.png"
    #define ICON6_ANI_1      "fs:/image/mainmenu/qsc1100_06_focus.png"
    #define ICON7_ANI_1      "fs:/image/mainmenu/qsc1100_07_focus.png"
    #define ICON8_ANI_1      "fs:/image/mainmenu/qsc1100_08_focus.png"
    #define ICON9_ANI_1      "fs:/image/mainmenu/qsc1100_09_focus.png"
    #define ICON10_ANI_1     "fs:/image/mainmenu/qsc1100_10_focus.png"
    #define ICON11_ANI_1     "fs:/image/mainmenu/qsc1100_11_focus.png"
    #define ICON12_ANI_1     "fs:/image/mainmenu/qsc1100_12_focus.png"
    
#endif
#if defined (FEATURE_DISP_128X128)
#if defined FEATURE_VERSION_H19C
    #define ICON1_ANI_1    "fs:/image/mainmenu/Recent_calls.gif"
    #define ICON2_ANI_1    "fs:/image/mainmenu/Multimedia.gif"
    #define ICON3_ANI_1    "fs:/image/mainmenu/contacts.gif"
    #define ICON4_ANI_1    "fs:/image/mainmenu/User_Profile.gif"
    #define ICON5_ANI_1    "fs:/image/mainmenu/Messages.gif"
    #define ICON6_ANI_1    "fs:/image/mainmenu/Tools.gif"
    #define ICON7_ANI_1    "fs:/image/mainmenu/Setting.gif"
    #define ICON8_ANI_1    "fs:/image/mainmenu/Game.gif"
    #define ICON9_ANI_1    "fs:/image/mainmenu/Net_Serve.gif"
#else
    #define ICON1_ANI_1    "fs:/image/mainmenu/MediaGallery_focus.png"
    #define ICON2_ANI_1    "fs:/image/mainmenu/Contact_focus.png"
    #define ICON3_ANI_1    "fs:/image/mainmenu/UTK_focus.png"
    #define ICON4_ANI_1    "fs:/image/mainmenu/FmRadio_focus.png"
    #define ICON5_ANI_1    "fs:/image/mainmenu/Message_focus.png"
    #define ICON6_ANI_1    "fs:/image/mainmenu/Recentcall_focus.png"
    #define ICON7_ANI_1    "fs:/image/mainmenu/MusicPlayer_focus.png"
    #define ICON8_ANI_1    "fs:/image/mainmenu/Setting_focus.png"
    #define ICON9_ANI_1    "fs:/image/mainmenu/application_focus.png"
#endif
#endif


// �˺��õ�ǰ״̬������ǰ״̬������nextState״̬���µ�ǰ״̬
#define MOVE_TO_STATE(nextState)            \
{                                           \
    MainmenuState tpState;                    \
    tpState = nextState;                    \
    pMe->m_prevState = pMe->m_currState;    \
    pMe->m_currState = tpState;             \
}

//�رնԻ���ĺ�
#define CLOSE_DIALOG(dlgResult)             \
{                                           \
    pMe->m_eDlgReturn = dlgResult;          \
    (void) ISHELL_EndDialog(pMe->m_pShell); \
}

/*==============================================================================

                                 ��������

==============================================================================*/

static int MainMenuMod_CreateInstance( IModule *po,
    IShell      *pIShell,
    AEECLSID    ClsId,
    void        **ppObj
);

static uint32 MainMenuMod_AddRef( IModule *po);

static uint32 MainMenuMod_Release( IModule *po);

static void MainMenuMod_FreeResources( IModule *po, IHeap *ph, IFileMgr *pfm);

