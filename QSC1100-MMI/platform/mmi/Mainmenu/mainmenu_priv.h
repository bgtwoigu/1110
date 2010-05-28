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
#include "mainmenuimages.brh"
#include "AEEConfig.h"

#ifdef FEATURE_SUPPORT_WAP_APP
#include "WapBrowser.bid"
#endif

#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "G_Sensor.h"
#endif



#define  MAX_MATRIX_ITEMS 9
#define  MAX_MATRIX_ROWS 3
#define  MAX_MATRIX_COLS 3

#define FEATURE_ICON_MOVE_ANIMATION //added by chengxiao 2009.04.10

#if !defined(FEATURE_CARRIER_CHINA_VERTU)
#define FEATURE_FOCUS_ANIMATION
#endif
#define MAX_TURN_NUM   20

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

    MAINST_GAME,
    
    MAINST_PLAYER,

    MAINST_DATA, 

    #ifdef FEATRUE_SUPPORT_G_SENSOR
    MAINST_MSGBOX,
    #endif   

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
        
    DLGRET_GAME,

    DLGRET_MEDIA,

    DLGRET_MAIN,

    DLGRET_DATA,

    #ifdef FEATRUE_SUPPORT_G_SENSOR
    DLGRET_MSGBOX,
    #endif

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
    uint32       referenceCounter;
    
    IModule     *m_pModule;
    IDisplay    *m_pDisplay;
    IShell      *m_pShell;
    IConfig     *m_pConfig;

    IDialog     *m_pActiveIDlg;
    uint32       m_pActivedlgID;
    uint16       m_MainSel;   //һ���˵����
    uint16       m_MenuSel;   //�����˵����
    DLGRetValue  m_eDlgReturn;
    
    int         m_nRow;
    int         m_nColumn;
#ifdef FEATURE_ICON_MOVE_ANIMATION
    int         m_nPrevRow;
    int         m_nPrevColumn;
#endif
#ifdef FEATURE_FOCUS_ANIMATION
    int         m_nIconAniFrameIdx;            // ͼ�궯����ǰ֡����
#endif

    MAINMENU_STATUS_e_type  m_eAppStatus;   // Applet ��ǰ����״̬
    
    boolean     m_bDoNotOverwriteDlgResult; //�Ƿ���д�Ի�����    
    boolean     m_bNormalStart;
    
    IImage      *m_pImageBg;
    IImage      *m_pImageIcon[MAX_MATRIX_ITEMS];
    IImage      *m_pAnimate;
    Point       m_Icondefault_Pt[MAX_MATRIX_ITEMS];
    Point       m_IconFocus_Pt[MAX_MATRIX_ITEMS];
    AEERect     m_rc;
    
    MainmenuState             m_prevState;        // Appletǰһ״̬
    MainmenuState             m_currState;        // Applet��ǰ״̬

    #ifdef FEATRUE_SUPPORT_G_SENSOR
    uint16           m_wMsgResID;
    #endif
    
#ifdef FEATURE_RANDOM_MENU_COLOR
    RGBVAL         m_nBgColor; //added by chengxiao 2009.02.23
    boolean         m_nRandomMenu; //added by chengxiao 2009.02.23
#endif
#ifdef FEATURE_ICON_MOVE_ANIMATION
    IBitmap       *m_pDevImage;
    boolean       m_bMoveing;
#endif

	IImage		*m_pImageTurn[MAX_TURN_NUM];//wlh 20090409 add

} MainMenu;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/

#define ICON_WIDTH              34
#define ICON_HEIGHT             28
#define ICON_ANIMATED_WIDTH     42
#define ICON_ANIMATED_HEIGHT    32
#define ICON_ANIMATED_FRAME     1 //����ͼƬ��֡��
#ifdef FEATURE_ICON_MOVE_ANIMATION
#define ICON_ANIMATED_MOVE_FRAME  6
#define ICON_ANIMATED_MOVE_RATE    15
#endif

#define MAINMENU_RES_FILE_LANG        AEE_RES_LANGDIR MAINMENU_RES_FILE
#define MAINMENU_RES_FILE_IMAGE       MAINMENUIMAGES_RES_FILE

#define STARTARGPREFIX_VIEWPLAYER            'P'
#define STARTARGPREFIX_VIEWDATA              'D'


#define ICON1_ANI      "fs:/image/mainmenu/Game1.png"
#define ICON2_ANI      "fs:/image/mainmenu/Tools3.png"
#define ICON3_ANI      "fs:/image/mainmenu/Contact5.png"
#define ICON4_ANI      "fs:/image/mainmenu/Message6.png"
#define ICON5_ANI      "fs:/image/mainmenu/Recentcall7.png"
#define ICON6_ANI      "fs:/image/mainmenu/Setting8.png"
#define ICON7_ANI      "fs:/image/mainmenu/Sports9.png"
#define ICON8_ANI      "fs:/image/mainmenu/Number10.png"
#define ICON9_ANI      "fs:/image/mainmenu/Wap12.png"


//wlh 20090410 add start
#define ICON1_ANI_1      "fs:/image/mainmenu/Game1_1.png"
#define ICON2_ANI_1      "fs:/image/mainmenu/Tools3_1.png"
#define ICON3_ANI_1      "fs:/image/mainmenu/Contact5_1.png"
#define ICON4_ANI_1      "fs:/image/mainmenu/Message6_1.png"
#define ICON5_ANI_1      "fs:/image/mainmenu/Recentcall7_1.png"
#define ICON6_ANI_1      "fs:/image/mainmenu/Setting8_1.png"
#define ICON7_ANI_1      "fs:/image/mainmenu/Sports9_1.png"
#define ICON8_ANI_1      "fs:/image/mainmenu/Profile10_1.png"
#define ICON9_ANI_1      "fs:/image/mainmenu/Number10_1.png"
#define ICON10_ANI_1     "fs:/image/mainmenu/Wap12_1.png"


#define ICON1_ANI_2      "fs:/image/mainmenu/Game1_2.png"
#define ICON2_ANI_2      "fs:/image/mainmenu/Tools3_2.png"
#define ICON3_ANI_2      "fs:/image/mainmenu/Contact5_2.png"
#define ICON4_ANI_2      "fs:/image/mainmenu/Message6_2.png"
#define ICON5_ANI_2      "fs:/image/mainmenu/Recentcall7_2.png"
#define ICON6_ANI_2      "fs:/image/mainmenu/Setting8_2.png"
#define ICON7_ANI_2      "fs:/image/mainmenu/Sports9_2.png"
#define ICON8_ANI_2      "fs:/image/mainmenu/Profile10_2.png"
#define ICON9_ANI_2      "fs:/image/mainmenu/Number10_2.png"
#define ICON10_ANI_2     "fs:/image/mainmenu/Wap12_2.png"
//wlh 20090410 add end

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

