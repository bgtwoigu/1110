/*==============================================================================
// 文件：
//        SettingMenuDialogHandler.c
//        版权所有(c) 2009 Anylook Tech. CO.,LTD.
//
// 文件说明：
//
// 作者：Gemsea
// 创建日期：2007-11-01
// 当前版本：Draft
==============================================================================*/

/*==============================================================================
修改历史记录：
时       间      修 改 人    问题单号  修改内容、位置及原因
-----------      ----------  --------  -----------------------------------------
==============================================================================*/


/*==============================================================================
                                 本文件包含的外部文件
==============================================================================*/
#include "SettingMenu_priv.h"
#include "AEECM.h"
#include "CallApp.h"
#ifdef FEATURE_SID_LOCK
#include "OEMNV.h"
//#include "OEMFeatures.h"
#endif
#include "OEMRTC.h"
#if defined(FEATURE_DISP_160X128)
#include "Appscommon_160x128.brh"
#elif defined(FEATURE_DISP_128X128)
#include "Appscommon_color.brh"
#else
#include "Appscommon_color.brh"
#endif

#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "g_sensor.h"
#endif

#ifdef FEATURE_PLANEMODE
#include "aeecm.h"
#endif

#ifdef FEATURE_APP_MUSICPLAYER
#include "MusicPlayer.h"
#endif 
#include "nv.h"
boolean start_security_setting_by_user;

#define FEATURE_SET_SCENEMODE
#define FEATURE_SET_SOUND_TITLE
/*==============================================================================
                                 宏定义和常数
==============================================================================*/
// 关闭对话框宏
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                                 }

/*==============================================================================
                                 类型定义
==============================================================================*/


/*==============================================================================
                                 函数声明
==============================================================================*/
// 对话框 IDD_MAIN_MENU 事件处理函数
static boolean  HandleMainDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// 对话框 IDD_PHONESETTING_MENU 事件处理函数
static boolean  HandlePhoneSettingDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// 对话框 IDD_CALLSETTING_MENU 事件处理函数
static boolean  HandleCallSettingDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

//对话框 IDD_CALLSETTINGSEL 事件处理函数
static boolean  HandleCallSettingSelDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// 对话框 IDD_DIVERT_MENU 事件处理函数
static boolean  HandleDivertDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// 对话框 IDD_NETWORKSETTING_MENU 事件处理函数
static boolean  HandleNetworkDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// 对话框 IDD_CALLFORWARD_SEL 事件处理函数
static boolean  HandleCallForwardSelDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// 对话框 IDD_CALLFORWARD_INPUT 事件处理函数
static boolean  HandleCallForwardInputDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

#ifdef FEATURE_SET_BANNER
//对话框 IDD_BANNER_TXT 事件处理函数
static boolean  HandleBannerDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
#endif

#ifdef FEATURE_TIME_DATA_SETTING
//对话框 IDD_PHONESETTING_MENU 事件处理函数
static boolean  HandleTimeDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
static boolean  HandleDateDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
#endif
#ifdef  FEATURE_DOUBLE_SIM_CARD
static boolean HandleSimDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
    );
#endif
#ifdef FEATURE_KEYGUARD
//对话框 IDD_AKG_MENU 事件处理函数
static boolean  HandleAKGDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
#endif

#ifdef FEATURE_LANGUAGE
// 对话框 IDD_LANGUAGE_MENU 事件处理函数
static boolean  HandleLanguageDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
#endif

#ifdef FEATURE_PLANEMODE
// 对话框 IDD_LANGUAGE_MENU 事件处理函数
static boolean  HandlePlaneModeDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
#endif

// 对话框 IDD_WARNING_MESSEGE 事件处理函数
static boolean  HandleWarningMessegeDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

#ifdef FEATURE_PERU_VERSION
// 对话框 IDD_PHONE_NUMBER 事件处理函数
static boolean  HandlePhoneNumberDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
#endif
#ifdef FEATURE_NETWORK_SET
// 对话框 IDD_NET_SELECT 事件处理函数
static boolean  HandleNetSelectDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
#endif //FEATURE_PERU_VERSION
#ifdef FEATURE_SID_LOCK
static boolean  HandleNet_Sid_Lock_SettingDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
#endif
// 呼叫一个号码，处理呼叫转移和等待
static void SettingMenu_MakeForwardCall(CSettingMenu *pMe,char *Number);

static void SettingMenu_DialogTimeout(void *pme);

//static void SettingMenu_AutoScroll(IMenuCtl  *pMenu, AEEEvent eCode,   uint16 wParam);

static void SettingMenu_SetItemNumIcon(IMenuCtl   *pMenu);
static void SetMenu_SetCheckBoxAsOff(IMenuCtl *pMenu);
#ifdef FEATRUE_AUTO_POWER
static void SettingMenu_Set_CTL(CSettingMenu *pMe);
static boolean  Setting_Process_Auto_Power(Auto_Power_Cfg *other_set, Auto_Power_Cfg *cur);
static boolean  Setting_HandleAuto_Power_DialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
#ifdef FEATURE_SUPPORT_G_SENSOR
static boolean Setting_Handle_Shake(CSettingMenu *pMe,
	AEEEvent eCode,
	uint16 wParam,
	uint32 dwParam
);
static boolean Setting_Handle_ShakeSub(CSettingMenu *pMe,
	AEEEvent eCode,
	uint16 wParam,
	uint32 dwParam
);
#endif
static boolean Setting_Handle_CallRestrict(CSettingMenu *pMe,
	AEEEvent eCode,
	uint16 wParam,
	uint32 dwParam
);
static boolean Setting_Handle_OutGoing(CSettingMenu *pMe,
	AEEEvent eCode,
	uint16 wParam,
	uint32 dwParam
);
static boolean Setting_Handle_Incoming(CSettingMenu *pMe,
	AEEEvent eCode,
	uint16 wParam,
	uint32 dwParam
);
static boolean Setting_Handle_Password(CSettingMenu *pMe,
	AEEEvent eCode,
	uint16 wParam,
	uint32 dwParam
);
static boolean Setting_Handle_Msgbox(CSettingMenu *pMe,
	AEEEvent eCode,
	uint16 wParam,
	uint32 dwParam
);

static boolean Setting_CClockApps_HandleKeyEvent(CSettingMenu *pMe, uint16 wParam);

static boolean Setting_CClockApps_HandleCmdEvent(CSettingMenu *pMe);

//static boolean Setting_CClockApps_HandleNumKeyEvent(CSettingMenu *pMe, uint16 wParam);
#endif
static boolean Handle_ANSWER_MODE_DialogEveng(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

static void SettingMenu_Process_Feature_Code(CSettingMenu *pMe,uint16 feature_code);

#ifdef FEATRUE_SET_IP_NUMBER
static boolean Handle_IP_Number_Set_DialogEveng(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);
static number_table * SettingMenu_Search_Node(Ip_number_table *table,uint16 id);
#endif /*FEATRUE_SET_IP_NUMBER*/
static boolean  SettingMenu_HandleKeyToneLengthDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

static boolean  HandleAutoAnswerSubDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

#ifdef FEATURE_SID_LOCK
static boolean Setting_inquire_Sid_lock_Cmd(byte cur_set);
#endif
/*==============================================================================
                                 全局数据
==============================================================================*/


/*==============================================================================
                                 本地（静态）数据
==============================================================================*/


/*==============================================================================
                                 函数定义
==============================================================================*/


/*==============================================================================
函数:
       SettingMenu_ShowDialog
说明:
       函数由状态处理函数用来创建相应对话框资源ID的对话框。

参数:
       pMe [In]: 指向SettingMenu Applet对象结构的指针,该结构包含小程序的特定信息.
       dlgResId [in]：对话框资源ID，函数根据该ID创建对话框。

返回值:
       无

备注:


==============================================================================*/
void SettingMenu_ShowDialog(CSettingMenu *pMe,uint16  dlgResId)
{
    int nRet;
    //SETTING_ERR("next show %d dialog", dlgResId, 0, 0);
    // At most one dialog open at once
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        ERR("Trying to create a dialog without closing the previous one",0,0,0);
        return;
    }

    nRet = ISHELL_CreateDialog(pMe->m_pShell,AEE_APPSSETTINGMENU_RES_FILE,dlgResId,NULL);

    if (nRet != SUCCESS)
    {
        ERR("Failed to create the dialog in the SettingMenu applet",0,0,0);
    }
}

/*==============================================================================
函数:
       SettingMenu_RouteDialogEvent
说明:
       函数将BREW事件路由给当前活动对话框的事件处理函数。

参数:
       pMe [In]: 指向SettingMenu Applet对象结构的指针,该结构包含小程序的特定信息.
       eCode [in]：事件代码。
       wParam [in]：与事件eCode关联的数据。
       dwParam [in]：与事件eCode关联的数据。

返回值:
       TRUE：传入事件被处理。
       FALSE：传入事件没被处理。

备注:

==============================================================================*/
boolean SettingMenu_RouteDialogEvent(CSettingMenu *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    if (NULL == pMe)
    {
        return FALSE;
    }

    if (NULL == pMe->m_pActiveDlg)
    {
        return FALSE;
    }
    //SETTING_ERR("%d SettingMenu_RouteDialogEvent", pMe->m_pActiveDlgID, 0, 0);
    switch (pMe->m_pActiveDlgID)
    {
        case IDD_MAIN_MENU:
            return HandleMainDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_CALLSETTING_MENU:
            return HandleCallSettingDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_PHONESETTING_MENU:
            return HandlePhoneSettingDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_CALLSETTINGSEL:
            return HandleCallSettingSelDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_NETWORKSETTING_MENU:
            return HandleNetworkDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_DIVERT_MENU:
            return HandleDivertDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_CALLFORWARD_SEL:
            return HandleCallForwardSelDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_CALLFORWARD_INPUT:
            return HandleCallForwardInputDialogEvent(pMe,eCode,wParam,dwParam);

#ifdef FEATURE_KEYGUARD
        case IDD_AKG_MENU:
            return HandleAKGDialogEvent(pMe,eCode,wParam,dwParam);
#endif

#ifdef FEATURE_SET_BANNER
        case IDD_BANNER_TXT:
            return HandleBannerDialogEvent(pMe,eCode,wParam,dwParam);
#endif

#ifdef FEATURE_TIME_DATA_SETTING
        case IDD_TIMESETTING:
            return HandleTimeDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_DATESETTING:
            return HandleDateDialogEvent(pMe,eCode,wParam,dwParam);
#endif
#ifdef FEATURE_LANGUAGE
        case IDD_LANGUAGE_MENU:
            return HandleLanguageDialogEvent(pMe,eCode,wParam,dwParam);
#endif
#ifdef FEATURE_PLANEMODE
        case IDD_PLANEMODE:
            return HandlePlaneModeDialogEvent(pMe,eCode,wParam,dwParam);
#endif
        case IDD_WARNING_MESSEGE:
            return HandleWarningMessegeDialogEvent(pMe,eCode,wParam,dwParam);

#ifdef FEATURE_PERU_VERSION
        case IDD_PHONE_NUMBER:
            return HandlePhoneNumberDialogEvent(pMe,eCode,wParam,dwParam);
#endif
#ifdef FEATURE_SID_LOCK
        case IDD_NET_SELECT:
            return HandleNet_Sid_Lock_SettingDialogEvent(pMe,eCode,wParam,dwParam);
#endif //FEATURE_PERU_VERSION

#ifdef FEATRUE_AUTO_POWER
        case IDD_AUTO_POWER:
            return Setting_HandleAuto_Power_DialogEvent(pMe,eCode,wParam,dwParam);
#endif
#ifdef FEATURE_SUPPORT_G_SENSOR
        case IDD_SHAKE:
            return Setting_Handle_Shake(pMe,eCode,wParam,dwParam);

        case IDD_SHAKESUB:
            return Setting_Handle_ShakeSub(pMe,eCode,wParam,dwParam);
#endif
        case IDD_PASSWORD:
            return Setting_Handle_Password(pMe,eCode,wParam,dwParam);
                     
        case IDD_RESTRICT:
            return Setting_Handle_CallRestrict(pMe,eCode,wParam,dwParam);
            
        case IDD_RESTRICT_OUTGOING:
            return Setting_Handle_OutGoing(pMe,eCode,wParam,dwParam);
            
        case IDD_RESTRICT_INCOMING:
            return Setting_Handle_Incoming(pMe,eCode,wParam,dwParam);
            
        case IDD_MSGBOX:
            return Setting_Handle_Msgbox(pMe,eCode,wParam,dwParam);

        case IDD_ANSWER_MODE:
            return Handle_ANSWER_MODE_DialogEveng(pMe,eCode,wParam,dwParam);

#ifdef FEATRUE_SET_IP_NUMBER
        case IDD_IP_NUMBER_SET:
            return Handle_IP_Number_Set_DialogEveng(pMe,eCode,wParam,dwParam);
#endif/*FEATRUE_SET_IP_NUMBER*/
        case IDD_KEYTONE_LENGTH:
            return SettingMenu_HandleKeyToneLengthDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_AUTOANSWER_SUB:
            return HandleAutoAnswerSubDialogEvent(pMe,eCode,wParam,dwParam);
#ifdef  FEATURE_DOUBLE_SIM_CARD
        case IDD_SIMSETTING:
            return HandleSimDialogEvent(pMe,eCode,wParam,dwParam);
#endif
        default:
            return FALSE;
    }
}

/*==============================================================================
函数：
       HandleMainDialogEvent
说明：
       IDD_MAIN_MENU对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
      FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleMainDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)
#ifdef FEATURE_SET_SOUND_TITLE
    ISoundMenu  *pSoundList = NULL;
#endif /*FEATURE_SET_SOUND_TITLE*/
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_MAIN);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleMainDialogEvent",eCode,wParam,dwParam);
    //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
             //modi by yangdecai
            //IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_DISPLAY_TITLE, IDS_DISPLAY_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLSETTING_TITLE, IDS_CALLSETTING_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_PHONESETTING_TITLE, IDS_PHONESETTING_TITLE, NULL, 0);
#ifdef FEATRUE_AUTO_POWER
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTO_POWER_TITLE, IDS_AUTO_POWER_TITLE, NULL, 0);
 //           IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTO_POWER_OFF_TIME, IDS_AUTO_POWER_OFF_TIME, NULL, 0);
#endif
#ifdef FEATURE_SUPPORT_G_SENSOR
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SHAKE_TITLE, IDS_SHAKE_TITLE, NULL, 0);
#endif
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SECURITY_TITLE, IDS_SECURITY_TITLE, NULL, 0);
#ifdef FEATURE_SET_SCENEMODE
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SCENEMODE_TITLE, IDS_SCENEMODE_TITLE, NULL, 0);
#endif
#ifdef FEATURE_SET_SOUND_TITLE
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SOUND_TITLE, IDS_SOUND_TITLE, NULL, 0);
#endif
#ifdef FEATURE_PERU_VERSION
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_PHONE_NUMBER, IDS_PHONE_NUMBER, NULL, 0);
#endif
#ifdef FEATURE_SID_LOCK
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_NET_SEL, IDS_NET_SEL, NULL, 0);
#endif
            return TRUE;

        case EVT_DIALOG_START:
            // 给菜单各菜单项加数字编号图标
            SettingMenu_SetItemNumIcon(pMenu);
#ifdef FEATRUE_AUTO_POWER
            pMe->m_auto_power = 0;
#endif
            IMENUCTL_SetSel(pMenu, pMe->m_currDlgId);

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            //pMe->m_currDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_currDlgId = wParam;
            switch (wParam)
            {
#ifdef FEATURE_SET_SOUND_TITLE
                case IDS_SOUND_TITLE:       //声音设置
                     if (AEE_SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
                                                              AEECLSID_APP_SOUNDMENU,
                                                              (void **)&pSoundList))
                     {
                         ISoundMenu_SoundList(pSoundList);
                         ISoundMenu_Release(pSoundList);
                     }
                     return TRUE;
#endif

#ifdef FEATURE_SET_SCENEMODE
                case IDS_SCENEMODE_TITLE:    //情景模式
                    (void) ISHELL_StartApplet(pMe->m_pShell,
                                              AEECLSID_APP_SOUNDMENU);
                    break;
#endif
/*//modi by yangdecai
                case IDS_DISPLAY_TITLE:      //显示设置
                    (void) ISHELL_StartApplet(pMe->m_pShell,
                                              AEECLSID_APP_DISPLAYMENU);
                    break;
*/
               case IDS_PHONESETTING_TITLE:  //话机设置
                    CLOSE_DIALOG(DLGRET_PHONESETTING)
                    break;

                case IDS_SECURITY_TITLE:      //安全设置
#if 1
                    start_security_setting_by_user = TRUE;
#endif
                    (void) ISHELL_StartApplet(pMe->m_pShell,
                                              AEECLSID_APP_SECURITYMENU);
                    break;

                case IDS_CALLSETTING_TITLE:   //通话设置
                    CLOSE_DIALOG(DLGRET_CALLSETTING)
                    break;

#ifdef FEATRUE_AUTO_POWER
                case IDS_AUTO_POWER_TITLE:/*自动开关机设置*/
                    CLOSE_DIALOG(DLGRET_AUTO_POWER)
                    break;
#endif
#ifdef FEATURE_SUPPORT_G_SENSOR
                case IDS_SHAKE_TITLE:
                    CLOSE_DIALOG(DLGRET_SHAKE)
                    break;
#endif
#ifdef FEATURE_PERU_VERSION
                case IDS_PHONE_NUMBER:
                    CLOSE_DIALOG(DLGRET_PHONE_NUMBER)
                    break;
#endif
#ifdef FEATURE_SID_LOCK
                case IDS_NET_SEL:
                    CLOSE_DIALOG(DLGRET_NET_SELECT)
                    break;
#endif  //FEATURE_PERU_VERSION

                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleMainDialogEvent


/*==============================================================================
函数：
       HandleCallSettingDialogEvent
说明：
       IDD_CALLSETTING_MENU对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleCallSettingDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_CALLSETTING);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleCallSettingDialogEvent",eCode,wParam,dwParam);
    //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ANSWER_MODE, IDS_ANSWER_MODE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_MINUTE_ALERT, IDS_MINUTE_ALERT, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTO_REDIAL, IDS_AUTO_REDIAL, NULL, 0);
#ifndef FEATURE_CARRIER_TAIWAN_APBW
#ifdef FEATRUE_SET_IP_NUMBER
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_IP_NUMBER_SET, IDS_IP_NUMBER_SET, NULL, 0);
#endif
#endif
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_DIVERT, IDS_CALLFORWARD_DIVERT, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_WAIT, IDS_CALLFORWARD_WAIT, NULL, 0);
#ifdef FEATURE_CARRIER_THAILAND_HUTCH 
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTOANSWER_TITLE, IDS_AUTOANSWER_TITLE, NULL, 0);
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALL_RESTRICT, IDS_CALL_RESTRICT, NULL, 0);            
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_VOICE_PRIVACY, IDS_VOICE_PRIVACY, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            // 给菜单各菜单项加数字编号图标
            pMe->m_input_mode = 0;
            SettingMenu_SetItemNumIcon(pMenu);

            IMENUCTL_SetSel(pMenu, pMe->m_nSubDlgId);

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            //pMe->m_nSubDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_nSubDlgId = wParam;
            //初始化音量
            (void) ICONFIG_GetItem(pMe->m_pConfig,
                                     CFGI_RINGER_VOL,
                                     &pMe->m_RingVol,
                                     sizeof(byte));

            switch (wParam)
            {
#ifndef FEATURE_CARRIER_TAIWAN_APBW
#ifdef FEATRUE_SET_IP_NUMBER
                case IDS_IP_NUMBER_SET:
                    pMe->m_CallSettingSel = IDS_IP_NUMBER_SET;
                    CLOSE_DIALOG(DLGRET_IP_NUMBER_SET)
                    break;
#endif
#endif
                case IDS_ANSWER_MODE:
                    CLOSE_DIALOG(DLGRET_AUTO_ANSWER_MODE)
                    break;
#ifdef FEATURE_CARRIER_THAILAND_HUTCH                          
                case IDS_AUTOANSWER_TITLE:
                    CLOSE_DIALOG(DLGRET_AUTO_ANSWER_MODE_SUB)
                    break;
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  

                case IDS_MINUTE_ALERT:     //50秒提醒
                    pMe->m_CallSettingSel = IDS_MINUTE_ALERT;
                    CLOSE_DIALOG(DLGRET_CALLSETTINGSEL)
                    break;

                case IDS_NETWORKSETTING_TITLE:  //网络设置
                    CLOSE_DIALOG(DLGRET_NETWORKSETTING)
                    break;
                    
                case IDS_CALLFORWARD_DIVERT:
                    CLOSE_DIALOG(DLGRET_DIVERT)
                    break;
                    
                case IDS_CALLFORWARD_WAIT:
                    pMe->m_CFType = CALLFORWARD_WAIT;
                    CLOSE_DIALOG(DLGRET_CALLFORWARDSEL)
                    break;

                case IDS_VOICE_PRIVACY:     //Voice privacy
                    pMe->m_CallSettingSel = IDS_VOICE_PRIVACY;
                    CLOSE_DIALOG(DLGRET_CALLSETTINGSEL)
                    break;

                case IDS_AUTO_REDIAL:
                    pMe->m_CallSettingSel = IDS_AUTO_REDIAL;
                    CLOSE_DIALOG(DLGRET_CALLSETTINGSEL)
                    break;

                case IDS_KEYTONE_LENGTH:
                    CLOSE_DIALOG(DLGRET_KEYLENGTH)
                    break;
                    
                case IDS_CALL_RESTRICT:
                    CLOSE_DIALOG(DLGRET_PASSWORD)
                    break;

                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleCallSettingDialogEvent


/*==============================================================================
函数：
       HandlePhoneSettingDialogEvent
说明：
       IDD_PHONESETTING_MENU对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandlePhoneSettingDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
	int rt;
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_PHONESETTING);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandlePhoneSettingDialogEvent",eCode,wParam,dwParam);
    //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
#ifdef FEATURE_PLANEMODE
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_PLANE_MODE, IDS_PLANE_MODE, NULL, 0);
#endif
#ifdef FEATURE_LANGUAGE
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_LANGUAGE, IDS_LANGUAGE, NULL, 0);
#endif
#ifdef FEATURE_SET_BANNER
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_BANNER_TITLE, IDS_BANNER_TITLE, NULL, 0);
#endif
#ifdef FEATURE_DOUBLE_SIM_CARD
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE,IDS_SIM_SET,IDS_SIM_SET,NULL,0);
#endif
#ifdef FEATURE_TIME_DATA_SETTING
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_TIMESETTING, IDS_TIMESETTING, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_DATESETTING, IDS_DATESETTING, NULL, 0);
#endif
#ifdef FEATURE_KEYGUARD
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTOKEYGUARD_TITLE, IDS_AUTOKEYGUARD_TITLE, NULL, 0);
#endif
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh 200904007 add 触摸校准
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ADJUSTSETTING, IDS_ADJUSTSETTING, NULL, 0);
#endif //FEATURE_LCD_TOUCH_ENABLE
            return TRUE;

        case EVT_DIALOG_START:
             // 给菜单各菜单项加数字编号图标
            SettingMenu_SetItemNumIcon(pMenu);

            IMENUCTL_SetSel(pMenu, pMe->m_nSubDlgId);

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;


        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
			ISHELL_CancelTimer( pMe->m_pShell, NULL, pMe);//wlh 20090407 add 消除电池充电显示
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            //pMe->m_nSubDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_nSubDlgId = wParam;
            switch (wParam)
            {
                
#ifdef FEATURE_PLANEMODE
                case IDS_PLANE_MODE:
                        /*(void) cm_ph_cmd_oprt_mode(NULL,
                             NULL,
                             CM_CLIENT_ID_ANONYMOUS,
                             SYS_OPRT_MODE_LPM);
                             */
                        CLOSE_DIALOG(DLGRET_PLANEMODE)
                    break;
#endif //FEATURE_PLANEMODE
#ifdef FEATURE_LANGUAGE
                case IDS_LANGUAGE:    //语言选择
                    CLOSE_DIALOG(DLGRET_LANGUAGE)
                    break;
#endif
#ifdef FEATURE_KEYGUARD
                case IDS_AUTOKEYGUARD_TITLE:         //自动锁键盘
                    CLOSE_DIALOG(DLGRET_AKG)
                    break;
#endif
#ifdef FEATURE_SET_BANNER
                case IDS_BANNER_TITLE:      //待机问候语
                    CLOSE_DIALOG(DLGRET_BANNER)
                    break;
#endif
#ifdef FEATURE_DOUBLE_SIM_CARD
                case IDS_SIM_SET:          //开机选卡
                    CLOSE_DIALOG(DLGRET_SIMSETTING)
                    break;
#endif
#ifdef FEATURE_TIME_DATA_SETTING
                case IDS_TIMESETTING:      //时间设置
                    CLOSE_DIALOG(DLGRET_TIMESETTING)
                    break;

                case IDS_DATESETTING:      //日期设置
                    CLOSE_DIALOG(DLGRET_DATESETTING)
                    break;
#endif
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh 20090407 add
				case IDS_ADJUSTSETTING:      //触摸校准设置
					rt = ISHELL_StartApplet(pMe->m_pShell, AEECLSID_ADJUSTPENAPP); 
					break;
#endif
                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandlePhoneSettingDialogEvent


/*==============================================================================
函数：
       HandleCallSettingSelDialogEvent
说明：
       IDD_CALLSETTINGSEL 对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleCallSettingSelDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_CALLSETTINGSEL);
#ifdef FEATURE_SID_LOCK  
    nv_item_type nvi;
    nv_stat_enum_type ret;
#endif
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleCallSettingSelDialogEvent",eCode,wParam,dwParam);
    //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
#ifdef FEATRUE_SET_IP_NUMBER
            if(pMe->m_CallSettingSel ==  IDS_SET_IP_CALL_OPTION)
            {
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SET_DEFAULT, IDS_SET_DEFAULT, NULL, 0);
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SET_EDIT, IDS_SET_EDIT, NULL, 0);
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SET_DELETE, IDS_SET_DELETE, NULL, 0);
            }
            else
#endif /*FEATRUE_SET_IP_NUMBER*/
#ifdef FEATRUE_AUTO_POWER
            if(pMe->m_CallSettingSel == IDS_AM_PM_SET)
            {
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SETMENU_AM, IDS_SETMENU_AM, NULL, 0);
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SETMENU_PM, IDS_SETMENU_PM, NULL, 0);
            }
            else
#endif //#ifdef FEATRUE_AUTO_POWER
#ifdef FEATURE_SID_LOCK
            if(pMe->m_CallSettingSel == IDS_LOCALNET)
            {
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_OPEN_ROAM, IDS_OPEN_ROAM, NULL, 0);
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CLOSE_ROAM, IDS_CLOSE_ROAM, NULL, 0);
            }
            else
#endif //#ifdef FEATRUE_AUTO_POWER

            {
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ENABLE, IDS_ENABLE, NULL, 0);
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_DISABLE, IDS_DISABLE, NULL, 0);
            }
            return TRUE;

        case EVT_DIALOG_START:
            {
                byte   byte_return = 0;        //默认为打开。

                switch(pMe->m_CallSettingSel)
                {
                    case IDS_ANYKEY_ANSWER_TITLE://0x1
                    case IDS_COVER_ANSWER://0x2
                    case IDS_SEND_ANSWER://0x4
                    case IDS_HEADSET_ANSWER://0x8
                        ICONFIG_GetItem(pMe->m_pConfig,CFGI_ANYKEY_ANSWER,&byte_return,sizeof(byte_return));
                        break;


                    case IDS_MINUTE_ALERT:      //50秒提醒
                        ICONFIG_GetItem(pMe->m_pConfig,CFGI_MINUTE_ALERT,&byte_return,sizeof(byte_return));
                        break;

                   case IDS_VOICE_PRIVACY:     //voice privacy
                        ICONFIG_GetItem(pMe->m_pConfig,CFGI_VOICEPRIVACY, &byte_return,sizeof(byte_return));
                        break;
#ifdef FEATRUE_SET_IP_NUMBER
                    case IDS_SET_IP_CALL_OPTION:
                    {
                        //IP_Number_Cfg   num_cfg;
                        //ICONFIG_GetItem(pMe->m_pConfig, CFGI_IP_NUMBER, (void*)&num_cfg, sizeof(IP_Number_Cfg));
                        //byte_return = (byte)pMe->ip_number_tab.ip_state;
                        break;
                    }
#ifdef FEATURE_CARRIER_TAIWAN_APBW
                    case IDS_SET_POUND:
                    {
                        boolean val = FALSE;
                        ICONFIG_GetItem(pMe->m_pConfig,CFGI_IP_POUND, &val,sizeof(val));
                        byte_return = (byte)val;
                        break;
                    }
#endif
#endif
#ifdef FEATURE_SID_LOCK  
                    case IDS_LOCALNET:
                    {
                        nvi.enabled_sid_lock.nam = cm_get_curr_nam();
                        ret = OEMNV_Get(NV_SID_LOCK_I,&nvi);
                        if(NV_DONE_S == ret )
                        {
                            byte_return = nvi.enabled_sid_lock.b_sid_lock;
                        }
                        break;
                    }
#endif
                    case IDS_AUTO_REDIAL:
                    {
                        nv_auto_redial_type m_redial;
                        ICONFIG_GetItem(pMe->m_pConfig, CFGI_AUTO_REDIAL, (void*)&m_redial, sizeof(nv_auto_redial_type));
                        byte_return = (byte)m_redial.enable;
                        break;
                    }
#ifdef FEATRUE_AUTO_POWER
                    case IDS_AM_PM_SET:
                        byte_return = pMe->m_b_selete_Pm;
                        break;
#endif //#ifdef FEATRUE_AUTO_POWER
                    default:
                        return FALSE;
                }
#ifdef FEATRUE_SET_IP_NUMBER
                if(pMe->m_CallSettingSel !=  IDS_SET_IP_CALL_OPTION)
#endif /*FEATRUE_SET_IP_NUMBER*/
                {
                    InitMenuIcons(pMenu);

                    if(pMe->m_CallSettingSel == IDS_ANYKEY_ANSWER_TITLE )
                    {
                        if(byte_return & 0x1)
                        {
                            IMENUCTL_SetSel(pMenu, IDS_ENABLE);
                            SetMenuIcon(pMenu, IDS_ENABLE, TRUE);
                        }
                        else
                        {
                            IMENUCTL_SetSel(pMenu, IDS_DISABLE);
                            SetMenuIcon(pMenu, IDS_DISABLE, TRUE);
                        }
                    }
                    else if(pMe->m_CallSettingSel == IDS_COVER_ANSWER )
                    {
                        if(byte_return & 0x2)
                        {
                            IMENUCTL_SetSel(pMenu, IDS_ENABLE);
                            SetMenuIcon(pMenu, IDS_ENABLE, TRUE);
                        }
                        else
                        {
                            IMENUCTL_SetSel(pMenu, IDS_DISABLE);
                            SetMenuIcon(pMenu, IDS_DISABLE, TRUE);
                        }
                    }
                    else if(pMe->m_CallSettingSel == IDS_SEND_ANSWER )
                    {
                        if(byte_return & 0x4)
                        {
                            IMENUCTL_SetSel(pMenu, IDS_ENABLE);
                            SetMenuIcon(pMenu, IDS_ENABLE, TRUE);
                        }
                        else
                        {
                            IMENUCTL_SetSel(pMenu, IDS_DISABLE);
                            SetMenuIcon(pMenu, IDS_DISABLE, TRUE);
                        }
                    }
                    else if(pMe->m_CallSettingSel == IDS_HEADSET_ANSWER )
                    {
                        if(byte_return & 0x8)
                        {
                            IMENUCTL_SetSel(pMenu, IDS_ENABLE);
                            SetMenuIcon(pMenu, IDS_ENABLE, TRUE);
                        }
                        else
                        {
                            IMENUCTL_SetSel(pMenu, IDS_DISABLE);
                            SetMenuIcon(pMenu, IDS_DISABLE, TRUE);
                        }
                    }
#ifdef FEATRUE_AUTO_POWER
                    else if(pMe->m_CallSettingSel == IDS_AM_PM_SET)
                    {
                        if(byte_return == 0)    //am
                        {
                            IMENUCTL_SetSel(pMenu, IDS_SETMENU_AM);
                            SetMenuIcon(pMenu, IDS_SETMENU_AM, TRUE);
                        }
                        else                    //pm
                        {
                            IMENUCTL_SetSel(pMenu, IDS_SETMENU_PM);
                            SetMenuIcon(pMenu, IDS_SETMENU_PM, TRUE);
                        }
                    }
#endif //#ifdef FEATRUE_AUTO_POWER
                    else if(pMe->m_CallSettingSel == IDS_VOICE_PRIVACY)
                    {
                        if(byte_return == AEECM_PRIVACY_PREF_ENHANCED)    //on
                        {
                            IMENUCTL_SetSel(pMenu, IDS_ENABLE);
                            SetMenuIcon(pMenu, IDS_ENABLE, TRUE);
                        }
                        else if(byte_return == AEECM_PRIVACY_PREF_STANDARD || byte_return == AEECM_PRIVACY_PREF_NONE)                   //off
                        {
                            IMENUCTL_SetSel(pMenu, IDS_DISABLE);
                            SetMenuIcon(pMenu, IDS_DISABLE, TRUE);
                        }
                    }
#ifdef FEATURE_SID_LOCK
                    else if(pMe->m_CallSettingSel == IDS_LOCALNET)
                    {
                        if(byte_return == 0) 
                        {
                            IMENUCTL_SetSel(pMenu, IDS_CLOSE_ROAM);
                            SetMenuIcon(pMenu, IDS_CLOSE_ROAM, TRUE);
                        }
                        else
                        {
                            IMENUCTL_SetSel(pMenu, IDS_OPEN_ROAM);
                            SetMenuIcon(pMenu, IDS_OPEN_ROAM, TRUE);
                        }
                    }
#endif //#ifdef FEATURE_SID_LOCK

                    else
                    {
                        if(byte_return != 0)    //开
                        {
                            IMENUCTL_SetSel(pMenu, IDS_ENABLE);
                            SetMenuIcon(pMenu, IDS_ENABLE, TRUE);
                        }
                        else                    //关
                        {
                            IMENUCTL_SetSel(pMenu, IDS_DISABLE);
                            SetMenuIcon(pMenu, IDS_DISABLE, TRUE);
                        }
                    }
                }
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                IMENUCTL_SetTitle(pMenu,AEE_APPSSETTINGMENU_RES_FILE,pMe->m_CallSettingSel,NULL);
                ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU,EVT_USER_REDRAW,0, 0);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            // 统一更新界面
            //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);

            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
        {
            byte   callset;
            InitMenuIcons(pMenu);
            SetMenuIcon(pMenu, wParam, TRUE);
            switch(wParam)
            {
#ifdef FEATURE_SID_LOCK
                case IDS_CLOSE_ROAM://net reset the phone
                {
                    if(Setting_inquire_Sid_lock_Cmd(0))
                    {
                        db_items_value_type sid_lock;
                        sid_lock.b_sid_lock = 0;
                        db_put(DB_SID_LOCK, &sid_lock);
                        nvi.enabled_sid_lock.nam = cm_get_curr_nam();
                        nvi.enabled_sid_lock.b_sid_lock = 0;
                        ret = OEMNV_Put(NV_SID_LOCK_I,&nvi);
                        pMe->m_msg_id = IDS_RESET_PHONE;
                        CLOSE_DIALOG(DLGRET_WARNING)
                    }
                    else
                    {
                        CLOSE_DIALOG(DLGRET_CANCELED)
                    }
                    return TRUE;
                }

                case IDS_OPEN_ROAM:
                {
                    if(Setting_inquire_Sid_lock_Cmd(1))
                    {
                        db_items_value_type sid_lock;
                        sid_lock.b_sid_lock = 1;
                        db_put(DB_SID_LOCK, &sid_lock);
                        nvi.enabled_sid_lock.nam = cm_get_curr_nam();
                        nvi.enabled_sid_lock.b_sid_lock = 1;
                        ret = OEMNV_Put(NV_SID_LOCK_I,&nvi);
                        pMe->m_msg_id = IDS_SET_SUCESS;
                        CLOSE_DIALOG(DLGRET_WARNING)
                    }
                    else
                    {
                        CLOSE_DIALOG(DLGRET_CANCELED)
                    }
                    return TRUE;
                }
#endif
                case IDS_DISABLE:      //关
                    switch(pMe->m_CallSettingSel)
                    {
                        case IDS_ANYKEY_ANSWER_TITLE:
                        case IDS_COVER_ANSWER:
                        case IDS_SEND_ANSWER:
                        case IDS_HEADSET_ANSWER:
                            ICONFIG_GetItem(pMe->m_pConfig,CFGI_ANYKEY_ANSWER,  &callset, sizeof(callset));
                            if(pMe->m_CallSettingSel == IDS_ANYKEY_ANSWER_TITLE)
                            {
                                callset= callset & 0xE;
                            }
                            else  if(pMe->m_CallSettingSel == IDS_COVER_ANSWER)
                            {
                                callset= callset & 0xD;
                            }
                            else  if(pMe->m_CallSettingSel == IDS_SEND_ANSWER)
                            {
                                callset= callset & 0xB;
                            }
                            else  if(pMe->m_CallSettingSel == IDS_HEADSET_ANSWER)
                            {
                                callset= callset & 0x7;
                            }
                            ICONFIG_SetItem(pMe->m_pConfig, CFGI_ANYKEY_ANSWER,&callset, sizeof(callset));
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;

                        case IDS_MINUTE_ALERT:      //50秒提醒
                            callset = OEMNV_ALERT_DISABLE;
                            ICONFIG_SetItem(pMe->m_pConfig,CFGI_MINUTE_ALERT,&callset,sizeof(callset));
                            CLOSE_DIALOG(DLGRET_WARNING)
                            return TRUE;

                       case IDS_VOICE_PRIVACY:     //voice privacy
                            callset = AEECM_PRIVACY_PREF_STANDARD;
                            ICONFIG_SetItem(pMe->m_pConfig,CFGI_VOICEPRIVACY,&callset,sizeof(callset));
                            CLOSE_DIALOG(DLGRET_WARNING)
                            return TRUE;
#ifdef FEATRUE_SET_IP_NUMBER
                        case IDS_IP_NUMBER_SET:
                        {
                            pMe->ip_number_tab.ip_state = 0;
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#ifdef FEATURE_CARRIER_TAIWAN_APBW
                        case IDS_SET_POUND:
                        {
                            boolean set = FALSE;
                            ICONFIG_SetItem(pMe->m_pConfig,CFGI_IP_POUND,(void*)&set,sizeof(set));
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#endif
#endif

                        case IDS_AUTO_REDIAL:
                        {
                            nv_auto_redial_type m_auto_redial;
                            m_auto_redial.enable = FALSE;
                            m_auto_redial.rings = 10;
                            ICONFIG_SetItem(pMe->m_pConfig,CFGI_AUTO_REDIAL,(void*)&m_auto_redial,sizeof(nv_auto_redial_type));
                            CLOSE_DIALOG(DLGRET_WARNING)
                            return TRUE;
                        }
                        default:
                            return FALSE;
                    }
                    break;

                case IDS_ENABLE:      //开
                    switch(pMe->m_CallSettingSel)
                    {
                        case IDS_ANYKEY_ANSWER_TITLE:     //任意键应答
                        case IDS_COVER_ANSWER:
                        case IDS_SEND_ANSWER:
                        case IDS_HEADSET_ANSWER:
                            ICONFIG_GetItem(pMe->m_pConfig,CFGI_ANYKEY_ANSWER,  &callset, sizeof(callset));
                            if(pMe->m_CallSettingSel == IDS_ANYKEY_ANSWER_TITLE)
                            {
                                callset= callset |0x1;
                            }
                            else  if(pMe->m_CallSettingSel == IDS_COVER_ANSWER)
                            {
                                callset= callset |0x2;
                            }
                            else  if(pMe->m_CallSettingSel == IDS_SEND_ANSWER)
                            {
                                callset= callset |0x4;
                            }
                            else  if(pMe->m_CallSettingSel == IDS_HEADSET_ANSWER)
                            {
                                callset= callset |0x8;
                            }
                            ICONFIG_SetItem(pMe->m_pConfig, CFGI_ANYKEY_ANSWER,&callset, sizeof(callset));
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;

                        case IDS_MINUTE_ALERT:      //50秒提醒
                            callset = OEMNV_ALERT_ENABLE;
                            ICONFIG_SetItem(pMe->m_pConfig,CFGI_MINUTE_ALERT,&callset,sizeof(callset));
                            CLOSE_DIALOG(DLGRET_WARNING)
                            return TRUE;

                        case IDS_VOICE_PRIVACY:     //voice privacy
                            callset = AEECM_PRIVACY_PREF_ENHANCED;
                            ICONFIG_SetItem(pMe->m_pConfig,CFGI_VOICEPRIVACY,&callset,sizeof(callset));
                            CLOSE_DIALOG(DLGRET_WARNING)
                            return TRUE;
#ifdef FEATURE_CARRIER_TAIWAN_APBW
#ifdef FEATRUE_SET_IP_NUMBER
                        case IDS_SET_POUND:
                        {
                            boolean set = TRUE;
                            ICONFIG_SetItem(pMe->m_pConfig,CFGI_IP_POUND,(void*)&set,sizeof(set));
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#endif
#endif
                        case IDS_AUTO_REDIAL:
                        {
                            nv_auto_redial_type m_auto_redial;
                            m_auto_redial.enable = TRUE;
                            m_auto_redial.rings = 10;
                            ICONFIG_SetItem(pMe->m_pConfig,CFGI_AUTO_REDIAL,(void*)&m_auto_redial,sizeof(nv_auto_redial_type));
                            CLOSE_DIALOG(DLGRET_WARNING)
                            return TRUE;
                        }
                        default:
                            return FALSE;
                    }
                    break;
#ifdef FEATRUE_SET_IP_NUMBER
                case IDS_SET_DELETE:
                {
                    uint16 i = 0;
                    if(pMe->m_ip_sel_num == 0) //del the first node
                    {
                        if(pMe->ip_number_tab.head)
                        {
                            number_table * phead = NULL;
                            phead = pMe->ip_number_tab.head;
                            if(WSTRNCMP(pMe->ip_number_tab.default_ip,phead->number,OEM_IP_NUMBER_MAX+1) == 0)
                            {
                                MEMSET(pMe->ip_number_tab.default_ip, 0 ,sizeof(pMe->ip_number_tab.default_ip));
                            }
                            pMe->ip_number_tab.head = pMe->ip_number_tab.head->next;
                            FREE(phead);
                        }
                    }
                    else
                    {
                        number_table * phead = pMe->ip_number_tab.head;
                        while(phead)
                        {
                            if( i == (pMe->m_ip_sel_num -1))//find the node,del the next node
                            {
                                number_table * temp = NULL;
                                temp = phead->next;
                                if(temp)
                                {
                                    phead->next = temp->next;
                                    if(phead->next == NULL) //del the last node,need change tial point
                                    {
                                        pMe->ip_number_tab.tial = phead;
                                    }
                                    if(WSTRNCMP(pMe->ip_number_tab.default_ip,temp->number,OEM_IP_NUMBER_MAX+1) == 0)
                                    {
                                        MEMSET(pMe->ip_number_tab.default_ip, 0 ,sizeof(pMe->ip_number_tab.default_ip));
                                    }
                                    FREE(temp);
                                    break;
                                }
                            }
                            phead = phead->next;
                            i++;
                        }
                    }
                    pMe->ip_number_tab.count --;
                    if(pMe->ip_number_tab.count == 0)
                    {
                        pMe->ip_number_tab.ip_state =0;
                        pMe->ip_number_tab.head = NULL;
                        pMe->ip_number_tab.tial = NULL;
                    }
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    //ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU,EVT_USER_REDRAW, 0,0);
                    return TRUE;
                }

                case IDS_SET_EDIT:
                {
                    pMe->m_input_mode = IDS_SET_IP_CALL_OPTION;
                    CLOSE_DIALOG(DLGRET_CALLFORWARDINPUT)
                    return TRUE;
                }

                case IDS_SET_DEFAULT:
                {
                    number_table * temp = NULL;
                    temp = SettingMenu_Search_Node(&pMe->ip_number_tab,pMe->m_ip_sel_num);
                    MEMSET(pMe->ip_number_tab.default_ip, 0 ,sizeof(pMe->ip_number_tab.default_ip));
                    WSTRLCPY(pMe->ip_number_tab.default_ip,temp->number,OEM_IP_NUMBER_MAX+1);
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                }
#endif //#ifdef FEATRUE_SET_IP_NUMBER
#ifdef FEATRUE_AUTO_POWER
                case IDS_SETMENU_AM:
                case IDS_SETMENU_PM:
                    pMe->m_b_selete_Pm = wParam - IDS_SETMENU_AM;
                    if(Setting_CClockApps_HandleCmdEvent(pMe))
                    {
                        CLOSE_DIALOG(DLGRET_CALLSETTING)
                    }
                    return TRUE;
#endif//#ifdef FEATRUE_AUTO_POWER
                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;
        }

        default:
            break;
    }
    return FALSE;
} // HandleOtherSelDialogEvent


/*==============================================================================
函数：
       HandleNetworkDialogEvent
说明：
       IDD_NETWORKSETTING_MENU对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleNetworkDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_NETWORKSETTING);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleNetworkDialogEvent",eCode,wParam,dwParam);
    //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_DIVERT, IDS_CALLFORWARD_DIVERT, NULL, 0);
#ifdef FEATURE_CARRIER_ISRAEL_PELEPHONE
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CUSTOMER_SERVER, IDS_CUSTOMER_SERVER, NULL, 0);
#else
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_WAIT, IDS_CALLFORWARD_WAIT, NULL, 0);
#endif
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_STRING_DND, IDS_STRING_DND, NULL, 0);
#endif
            return TRUE;

        case EVT_DIALOG_START:
            // 给菜单各菜单项加数字编号图标
            SettingMenu_SetItemNumIcon(pMenu);

            IMENUCTL_SetSel(pMenu, pMe->m_sSubDlgId);

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            pMe->m_sSubDlgId = IMENUCTL_GetSel(pMenu);
            switch (wParam)
            {
                case IDS_CALLFORWARD_DIVERT:     //呼叫转移
                    CLOSE_DIALOG(DLGRET_DIVERT)
                    break;

                case IDS_CALLFORWARD_WAIT:       //呼叫等待
                    pMe->m_CFType = CALLFORWARD_WAIT;
                    CLOSE_DIALOG(DLGRET_CALLFORWARDSEL)
                    break;
#ifdef FEATURE_CARRIER_ISRAEL_PELEPHONE
                case IDS_CUSTOMER_SERVER:
                    STRLCPY(pMe->m_callnumber,"*166",sizeof(pMe->m_callnumber));
                    SettingMenu_MakeForwardCall(pMe,pMe->m_callnumber);
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    break;
#endif
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
                 case IDS_STRING_DND:
                    pMe->m_CFType = CALLFORWARD_DND;
                    CLOSE_DIALOG(DLGRET_CALLFORWARDSEL)
                    return TRUE;
#endif
                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;

        default:
            break;
    }

    return FALSE;
} // HandleNetworkDialogEvent


/*==============================================================================
函数：
       HandleDivertDialogEvent
说明：
       IDD_DIVERT_MENU对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleDivertDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_DIVERT);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleDivertDialogEvent",eCode,wParam,dwParam);
    //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_BUSY, IDS_CALLFORWARD_BUSY, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_NOANSWER, IDS_CALLFORWARD_NOANSWER, NULL, 0);
#ifndef FEATURE_CARRIER_VENEZUELA_MOVILNET            
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_NOCONNECT, IDS_CALLFORWARD_NOCONNECT, NULL, 0);
#endif
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_ANYWAY, IDS_CALLFORWARD_ANYWAY, NULL, 0);
#if (!defined FEATURE_CARRIER_VENEZUELA_MOVILNET) && (!defined FEATURE_CARRIER_THAILAND_HUTCH)    
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_CANCELALL, IDS_CALLFORWARD_CANCELALL, NULL, 0);
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
            return TRUE;

        case EVT_DIALOG_START:
            // 给菜单各菜单项加数字编号图标
            SettingMenu_SetItemNumIcon(pMenu);

            IMENUCTL_SetSel(pMenu, pMe->m_sSubDlgId);

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            //pMe->m_sSubDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_sSubDlgId = wParam;
            switch (wParam)
            {
                case IDS_CALLFORWARD_BUSY:      //遇忙转移
                    pMe->m_CFType = CALLFORWARD_BUSY;
                    CLOSE_DIALOG(DLGRET_CALLFORWARDSEL)
                    return TRUE;

                case IDS_CALLFORWARD_NOANSWER:  //无应答转移
                    pMe->m_CFType = CALLFORWARD_NOANSWER;
                    CLOSE_DIALOG(DLGRET_CALLFORWARDSEL)
                    return TRUE;

                case IDS_CALLFORWARD_NOCONNECT: //未接通转移
                    pMe->m_CFType = CALLFORWARD_NOCONNECT;
                    CLOSE_DIALOG(DLGRET_CALLFORWARDSEL)
                    return TRUE;

                case IDS_CALLFORWARD_ANYWAY:     //无条件转移
                    pMe->m_CFType = CALLFORWARD_ANYWAY;
                    CLOSE_DIALOG(DLGRET_CALLFORWARDSEL)
                    return TRUE;

                case IDS_CALLFORWARD_CANCELALL: //取消全部转移
                {
                    SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_DISABLE_ALL);
                    if(STRLEN(pMe->m_callnumber) == 0)
                    {
                        CLOSE_DIALOG(DLGRET_WARNING)
                    }
                    else
                    {
                        SettingMenu_MakeForwardCall(pMe,pMe->m_callnumber);
#ifndef FEATURE_CARRIER_THAILAND_HUTCH                           
                        CLOSE_DIALOG(DLGRET_CANCELED)
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
                    }
                    return TRUE;
                }

                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleDivertDialogEvent


/*==============================================================================
函数：
       HandleCallForwardSelDialogEvent
说明：
       IDD_CALLFORWARD_SEL对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleCallForwardSelDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_CALLFORWARD_SEL);
    uint16 selete_item = 0;
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleCallForwardSelDialogEvent",eCode,wParam,dwParam);
    //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            pMe->m_input_mode = 0;
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_START, IDS_CALLFORWARD_START, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CALLFORWARD_CANCEL, IDS_CALLFORWARD_CANCEL, NULL, 0);
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
            if(CALLFORWARD_CNIR != pMe->m_CFType && CALLFORWARD_DND != pMe->m_CFType)
            {
                if(CALLFORWARD_WAIT == pMe->m_CFType)
                {
                    //IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_STRING_TEMP_CANCAL, IDS_STRING_TEMP_CANCAL, NULL, 0);
                }
                else
                {
                    IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_TO_VOICEMAIL, IDS_TO_VOICEMAIL, NULL, 0);
                }
            }
#endif
            return TRUE;

        case EVT_DIALOG_START:
            switch (pMe->m_CFType)
            {
                case CALLFORWARD_BUSY:      //遇忙转移
                    pMe->m_nResID = IDS_CALLFORWARD_BUSY;
                    selete_item = CFGI_CALLFORWARD_BUSY_ENABLE;
                    //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_BUSY_ENABLE);
                    break;

                case CALLFORWARD_NOANSWER:  //无应答转移
                    pMe->m_nResID = IDS_CALLFORWARD_NOANSWER;
                    selete_item = CFGI_CALLFORWARD_NOANSWER_ENABLE;
                    //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_NOANSWER_ENABLE);
                    break;

                case CALLFORWARD_NOCONNECT: //未接通转移
                    pMe->m_nResID = IDS_CALLFORWARD_NOCONNECT;
                    selete_item = CFGI_CALLFORWARD_NOCONNECT_ENABLE;
                    //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_NOCONNECT_ENABLE);
                    break;

                case CALLFORWARD_ANYWAY:    //无条件转移
                    pMe->m_nResID = IDS_CALLFORWARD_ANYWAY;
                    selete_item = CFGI_CALLFORWARD_ANYWAY_ENABLE;
                    //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_ANYWAY_ENABLE);
                    break;

                case CALLFORWARD_WAIT:      //呼叫等待
                    pMe->m_nResID = IDS_CALLFORWARD_WAIT;
                    selete_item = CFGI_CALLFORWARD_WAIT_ENABLE;
                    //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_WAIT_ENABLE);
                    break;
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
                //case CALLFORWARD_CNIR:
                //    pMe->m_nResID = IDS_STRING_CNIR;
                //    selete_item = CFGI_CALLFORWARD_CNIR_ENABLE;
                //    break;

                case CALLFORWARD_DND:
                    pMe->m_nResID = IDS_STRING_DND;
                    selete_item = CFGI_CALLFORWARD_DND_ENABLE;
                    break;
#endif
                default:
                    return FALSE;
            }
            SettingMenu_Process_Feature_Code(pMe,selete_item);
            // 给菜单各菜单项加数字编号图标
            SettingMenu_SetItemNumIcon(pMenu);

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            (void)IMENUCTL_SetTitle(pMenu,
                                    AEE_APPSSETTINGMENU_RES_FILE,
                                    pMe->m_nResID,
                                    NULL);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_APP_SETTINGMENU,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
            return TRUE;

        case EVT_USER_REDRAW:
            // 统一更新界面
            //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            pMe->m_input_mode = wParam;
            switch (wParam)
            {
                case IDS_CALLFORWARD_START:     //启动
#ifdef FEATURE_CARRIER_ISRAEL_PELEPHONE
                    if(pMe->m_CFType == CALLFORWARD_NOCONNECT)
                    {
                        SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_NOCONNECT_ENABLE);
                    }
#endif
                    if(STRLEN(pMe->m_callnumber) == 0)
                    {
                        CLOSE_DIALOG(DLGRET_WARNING)
                        return TRUE;
                    }

                    if(pMe->m_CFType == CALLFORWARD_WAIT
#ifdef FEATURE_CARRIER_ISRAEL_PELEPHONE
                        ||pMe->m_CFType == CALLFORWARD_NOCONNECT
#endif
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
                        ||pMe->m_CFType == CALLFORWARD_CNIR
                        ||pMe->m_CFType == CALLFORWARD_DND
#endif
                        )
                    {
                        SettingMenu_MakeForwardCall(pMe,pMe->m_callnumber);
#ifndef FEATURE_CARRIER_THAILAND_HUTCH                           
                        CLOSE_DIALOG(DLGRET_CANCELED)
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
                    }
                    else
                    {
                        CLOSE_DIALOG(DLGRET_CALLFORWARDINPUT)
                    }
                    break;

                case IDS_CALLFORWARD_CANCEL:  //取消
                    {

                        switch (pMe->m_CFType)
                        {
                            case CALLFORWARD_BUSY:      //遇忙转移
                                selete_item = CFGI_CALLFORWARD_BUSY_DISABLE;
                                //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_BUSY_DISABLE);
                                break;

                            case CALLFORWARD_NOANSWER:  //无应答转移
                                selete_item = CFGI_CALLFORWARD_NOANSWER_DISABLE;
                                //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_NOANSWER_DISABLE);
                                break;

                            case CALLFORWARD_NOCONNECT: //未接通转移
                                selete_item = CFGI_CALLFORWARD_NOCONNECT_DISABLE;
                                //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_NOCONNECT_DISABLE);
                                break;

                            case CALLFORWARD_ANYWAY:    //无条件转移
                                selete_item = CFGI_CALLFORWARD_ANYWAY_DISABLE;
                                //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_ANYWAY_DISABLE);
                                break;

                            case CALLFORWARD_WAIT:      //呼叫等待
                                selete_item = CFGI_CALLFORWARD_WAIT_DISABLE;
                                //SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_WAIT_DISABLE);
                                break;
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
                            case CALLFORWARD_DND:
                                selete_item = CFGI_CALLFORWARD_DND_DISABLE;
                                break;

                            case CALLFORWARD_CNIR:
                                selete_item = CFGI_CALLFORWARD_CNIR_DISABLE;
                                break;
#endif
                            default:
                                return FALSE;
                        }
                        SettingMenu_Process_Feature_Code(pMe,selete_item);
                        if(STRLEN(pMe->m_callnumber) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_WARNING)
                        }
                        else
                        {
                            SettingMenu_MakeForwardCall(pMe,pMe->m_callnumber);
#ifndef FEATURE_CARRIER_THAILAND_HUTCH                                
                            CLOSE_DIALOG(DLGRET_CANCELED)
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
                        }
                        break;
                    }
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
                //case IDS_STRING_TEMP_CANCAL:
                //    SettingMenu_Process_Feature_Code(pMe,CFGI_CALLFORWARD_WAIT_TEMP_DISABLE);
                //    if(STRLEN(pMe->m_callnumber) == 0)
                //    {
                //        CLOSE_DIALOG(DLGRET_WARNING)
                //    }
                //    else
                //    {
                //        SettingMenu_MakeForwardCall(pMe,pMe->m_callnumber);
                //        CLOSE_DIALOG(DLGRET_CANCELED)
                //    }
                //    break;

                case IDS_TO_VOICEMAIL:
                    switch (pMe->m_CFType)
                    {
                        case CALLFORWARD_BUSY:
                            selete_item = CFGI_CALLFORWARD_BUSY_VOICEMAIL;
                            break;

                        case CALLFORWARD_NOANSWER:
                            selete_item = CFGI_CALLFORWARD_NOANSWER_VOICEMAIL;
                            break;

                        case CALLFORWARD_NOCONNECT:
                            selete_item = CFGI_CALLFORWARD_NOCONNECT_VOICEMAIL;
                            break;

                        case CALLFORWARD_ANYWAY:
                            selete_item = CFGI_CALLFORWARD_ANYWAY_VOICEMAIL;
                            break;
                    }
                    SettingMenu_Process_Feature_Code(pMe,selete_item);
                    if(STRLEN(pMe->m_callnumber) == 0)
                    {
                        CLOSE_DIALOG(DLGRET_WARNING)
                    }
                    else
                    {
                        SettingMenu_MakeForwardCall(pMe,pMe->m_callnumber);
                        CLOSE_DIALOG(DLGRET_CANCELED)
                    }
                    break;
#endif
                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleCallForwardSelDialogEvent


/*==============================================================================
函数：
       HandleCallForwardInputDialogEvent
说明：
       IDD_CALLFORWARD_INPUT对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleCallForwardInputDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    ITextCtl *pTextCtl= (ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                      IDC_CALLFORWARD_INPUT);
    if (pTextCtl == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleCallForwardInputDialogEvent",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
#ifdef FEATRUE_SET_IP_NUMBER
            pMe->need_save_default = FALSE;
#endif
            return TRUE;

        case EVT_DIALOG_START:
        {
            AEERect rc;

            SETAEERECT(&rc, 0, 0, pMe->m_rc.dx, 32);//pMe->m_rc.dy - 16);
            ITEXTCTL_SetRect(pTextCtl,  &rc);
            //ICONTROL_SetRect((IControl*)pTextCtl, &pMe->m_rc);
#ifdef FEATRUE_SET_IP_NUMBER
            if(pMe->m_input_mode == IDS_IP_NUMBER_SET)
            {
                //IP_Number_Cfg   num_cfg;
                //ICONFIG_GetItem(pMe->m_pConfig, CFGI_IP_NUMBER, (void*)&num_cfg, sizeof(IP_Number_Cfg));
                ITEXTCTL_SetMaxSize (pTextCtl, OEM_IP_NUMBER_MAX);
                ITEXTCTL_SetTitle(pTextCtl,AEE_APPSSETTINGMENU_RES_FILE,IDS_IP_NUMBER_SET,NULL);
                //ITEXTCTL_SetText(pTextCtl, (AECHAR*)num_cfg.Number, -1);
                //ITEXTCTL_SetCursorPos (pTextCtl, TC_CURSOREND);
            }
            else if (pMe->m_input_mode == IDS_SET_IP_CALL_OPTION)
            {
                number_table * temp = NULL;
                temp = SettingMenu_Search_Node(&pMe->ip_number_tab,pMe->m_ip_sel_num);
                ITEXTCTL_SetMaxSize (pTextCtl, OEM_IP_NUMBER_MAX);
                ITEXTCTL_SetTitle(pTextCtl,AEE_APPSSETTINGMENU_RES_FILE,IDS_IP_NUMBER_SET,NULL);
                if(temp)
                {
                    if(WSTRNCMP(pMe->ip_number_tab.default_ip,temp->number,OEM_IP_NUMBER_MAX+1) == 0)
                    {
                        pMe->need_save_default = TRUE;
                    }
                    ITEXTCTL_SetText(pTextCtl, temp->number, -1);
                }
                ITEXTCTL_SetCursorPos (pTextCtl, TC_CURSOREND);
            }
            else
#endif
            {
                ITEXTCTL_SetMaxSize (pTextCtl, 32);
            }
            ITEXTCTL_SetProperties( pTextCtl, TP_FRAME | TP_NOUPDATE|TP_FOCUS_NOSEL);
            //ITEXTCTL_SetProperties( pTextCtl, TP_FRAME | TP_FIXSETRECT |TP_EDITNUMBER_PTSTRING );
            (void) ITEXTCTL_SetInputMode(pTextCtl, AEE_TM_NUMBERS);
            (void) ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU,EVT_USER_REDRAW,0,0);
            return TRUE;
        }

        case EVT_KEY_RELEASE:
        case EVT_USER_REDRAW:
            if ( WSTRLEN(ITEXTCTL_GetTextPtr(pTextCtl)) > 0 &&
                 ITEXTCTL_GetCursorPos(pTextCtl) != TC_CURSORSTART )
            {
                //drawBottomBar(pMe->m_pDisplay, AEE_FONT_NORMAL,BTBAR_OK_DELETE);
                SETTING_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
            }
            else
            {
                //drawBottomBar(pMe->m_pDisplay, AEE_FONT_NORMAL,BTBAR_OK_BACK);
                SETTING_MENU_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
            }
            // 统一更新界面
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);

            (void)ITEXTCTL_Redraw(pTextCtl);

            return TRUE;

        case EVT_DIALOG_END:
#ifdef FEATRUE_SET_IP_NUMBER
            pMe->need_save_default = FALSE;
#endif
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                case AVK_SELECT:
                case AVK_INFO:
                    {
                        AECHAR   CharNum[32] = {(AECHAR)'\0'};    //存储转移号码

                        ITEXTCTL_GetText(pTextCtl, CharNum, sizeof(CharNum));

                        if(CharNum[0] == (AECHAR)'\0')
                        {
                            CLOSE_DIALOG(DLGRET_WARNING)
                        }
                        else
                        {
#ifdef FEATRUE_SET_IP_NUMBER
                            if(pMe->m_input_mode == IDS_IP_NUMBER_SET)
                            {
                                number_table *num_tab = NULL;
                                num_tab = (number_table *)MALLOC(sizeof(number_table));
                                if(num_tab)
                                {
                                    //ITEXTCTL_GetText(pTextCtl,  num_tab->number, OEM_IP_NUMBER_MAX);
                                    WSTRLCPY(num_tab->number,CharNum,OEM_IP_NUMBER_MAX+1);
                                    num_tab->next = NULL;
                                    if(pMe->ip_number_tab.head == NULL)
                                    {
                                        pMe->ip_number_tab.head = num_tab;
                                    }

                                    if(pMe->ip_number_tab.tial)
                                    {
                                        pMe->ip_number_tab.tial->next = num_tab;//add to last node
                                    }
                                    pMe->ip_number_tab.tial = num_tab;
                                    pMe->ip_number_tab.ip_state = 1;
                                    pMe->ip_number_tab.count ++;
                                    SettingMenu_Print_IP_Number(pMe);
                                }
                            }
                            else if(pMe->m_input_mode == IDS_SET_IP_CALL_OPTION)
                            {
                                number_table * temp = NULL;
                                temp = SettingMenu_Search_Node(&pMe->ip_number_tab,pMe->m_ip_sel_num);
                                if(temp)
                                {
                                    //ITEXTCTL_GetText(pTextCtl,  temp->number, OEM_IP_NUMBER_MAX);
                                    WSTRLCPY(temp->number,CharNum,OEM_IP_NUMBER_MAX+1);
                                    if(pMe->need_save_default)
                                    {
                                        MEMSET(pMe->ip_number_tab.default_ip, 0 ,sizeof(pMe->ip_number_tab.default_ip));
                                        WSTRLCPY(pMe->ip_number_tab.default_ip,temp->number,OEM_IP_NUMBER_MAX+1);
                                    }
                                }
                            }
                            else
#endif
                            {
                                char   Number[65];
                                char   temp[32];    //存储转移号码

                                MEMSET(Number, 0, sizeof(Number));
                                STRCPY(Number,pMe->m_callnumber);

                                WSTRTOSTR(ITEXTCTL_GetTextPtr(pTextCtl), temp, sizeof(temp));
                                STRCAT(Number, temp);
                                SettingMenu_MakeForwardCall(pMe,Number);
                            }
#ifndef FEATURE_CARRIER_THAILAND_HUTCH   
                            CLOSE_DIALOG(DLGRET_CANCELED)
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
                        }
                    }
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleCallForwardInputDialogEvent


//*==============================================================================
//函数：
//       HandleWarningMessegeDialogEvent
//说明：
//       IDD_WARNING_MESSEGE对话框事件处理函数
//
//参数：
//       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
//       eCode [in]：事件代码。
//       wParam：事件相关数据。
//       dwParam：事件相关数据。
//
//返回值：
//       TRUE：传入事件被处理。
//       FALSE：传入事件被忽略。
//
//备注：
//
//==============================================================================*/
static boolean  HandleWarningMessegeDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    //PARAM_NOT_REF(dwParam)
    static IStatic * pStatic = NULL;
    SETTING_ERR("%x, %x ,%x,HandleWarningMessegeDialogEvent",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if (NULL == pStatic)
            {
                AEERect rect = {0};
                if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_STATIC,(void **)&pStatic))

                {
                    SETTING_ERR("ISHELL_CreateInstance,AEECLSID_STATIC 2",0,0,0);
                    return FALSE;
                }
                ISTATIC_SetRect(pStatic, &rect);
            }
            return TRUE;

        case EVT_DIALOG_START:
            ISHELL_SetTimer(pMe->m_pShell,1000,SettingMenu_DialogTimeout,pMe);
            ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU,EVT_USER_REDRAW,0,0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
            PromptMsg_Param_type m_PromptMsg;
            MEMSET(&m_PromptMsg,0,sizeof(PromptMsg_Param_type));
            m_PromptMsg.nMsgResID= pMe->m_msg_id ;
            m_PromptMsg.ePMsgType = MESSAGE_INFORMATIVE;
            STRLCPY(m_PromptMsg.strMsgResFile, AEE_APPSSETTINGMENU_RES_FILE,MAX_FILE_NAME);
            m_PromptMsg.eBBarType = BTBAR_BACK;
            DrawPromptMessage(pMe->m_pDisplay,pStatic,&m_PromptMsg);
            IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
            return TRUE;
        }

        case EVT_DIALOG_END:
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            ISHELL_CancelTimer(pMe->m_pShell, NULL,  pMe);
            return TRUE;

        case EVT_DISPLAYDIALOGTIMEOUT:
#ifdef FEATURE_SID_LOCK
            if(pMe->m_msg_id == IDS_RESET_PHONE)
            {
#if 0
                ICM *pIcm = NULL;
                if(ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_CM,
                                                        (void **) &pIcm) != SUCCESS)
                {
                    SETTING_ERR("Unable to create ICM interface", 0, 0, 0);
                    return FALSE;
                }
                //ICM_SetOperatingMode(pIcm, AEECM_OPRT_MODE_OFFLINE);
                ICM_SetOperatingMode(pIcm, AEECM_OPRT_MODE_PWROFF);
                ICM_Release(pIcm);
#else
                OEMRTC_Phone_Reset(pMe->m_pShell);
#endif
            }
#endif
            CLOSE_DIALOG(DLGRET_MSGBOX_OK)
            return TRUE;

        case EVT_KEY:
            ISHELL_CancelTimer(pMe->m_pShell, NULL,  pMe);
            CLOSE_DIALOG(DLGRET_MSGBOX_OK)
            return TRUE;

        default:
            break;
    }

    return FALSE;
}//HandleWarningMessegeDialogEvent

#ifdef FEATURE_KEYGUARD
/*==============================================================================
函数：
       HandleAKGDialogEvent
说明：
       IDD_AKG_MENU 对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleAKGDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl  *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                     IDC_AKG);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleAKGDialogEvent",eCode,wParam,dwParam);
    //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ON, IDS_ON, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_OFF, IDS_OFF, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16    ui16_return = IDS_OFF;
                boolean   AKG = 0;

                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                      CFGI_KEY_LOCK_CHECK,
                                      &AKG,
                                      sizeof(AKG));

                switch (AKG)
                {
                    case 1:   //开
                      ui16_return = IDS_ON;
                      break;
                      
                   default:
                    case 0: //关
                      ui16_return = IDS_OFF;
                      break;
                }

                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, ui16_return, TRUE);
                IMENUCTL_SetSel(pMenu, ui16_return);

                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
                IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);

                (void) ISHELL_PostEvent( pMe->m_pShell,
                                         AEECLSID_APP_SETTINGMENU,
                                         EVT_USER_REDRAW,
                                         0,
                                         0);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                boolean AKG = 0;

                switch (wParam)
                {
                    case IDS_ON:     //开
                        AKG = 1;
                        break;
                        
                    case IDS_OFF:     //关
                        AKG = 0;
                        break;

                    default:
                        ASSERT_NOT_REACHABLE;
                }

                (void) ICONFIG_SetItem(pMe->m_pConfig,
                                       CFGI_KEY_LOCK_CHECK,
                                       &AKG,
                                       sizeof(AKG));
                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, wParam, TRUE);
                CLOSE_DIALOG(DLGRET_WARNING)
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleAKGDialogEvent
#endif
#ifdef FEATURE_SET_BANNER
/*==============================================================================
函数：
       HandleBannerDialogEvent
说明：
       IDD_BANNER_TXT 对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleBannerDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    ITextCtl *pTextCtl= (ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                      IDC_BANNER);
    if (pTextCtl == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleBannerDialogEvent",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
            {
                AECHAR szBuf[OEMNV_BANNER_MAXLEN];
                AEERect rc;
                MEMSET(szBuf, '\0', sizeof(szBuf));

                SETAEERECT(&rc, 0, 0, pMe->m_rc.dx, 32);//pMe->m_rc.dy - 16);
                ITEXTCTL_SetRect(pTextCtl,  &rc);
                //ICONTROL_SetRect((IControl*)pTextCtl, &pMe->m_rc);
                // 长按*键切换输入法
                //ITEXTCTL_SetProperties( pTextCtl, TP_FRAME );
                //ITEXTCTL_SetProperties( pTextCtl, TP_FRAME | TP_STARKEY_SWITCH);
#ifdef Temp_remove
                // 长按*键切换输入法
                ITEXTCTL_SetProperties( pTextCtl, TP_FRAME | TP_STARKEY_SWITCH|TP_FOCUS_NOSEL);
#endif/*Temp_remove*/
                //设置最多字符数
                ITEXTCTL_SetMaxSize(pTextCtl, BANNER_MAXTXT);
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_BANNER/*CFGI_PHONE_BANNER*/,
                                       szBuf,
                                       sizeof(szBuf)) ;
                (void) ITEXTCTL_SetText(pTextCtl, szBuf, -1);

                ITEXTCTL_SetActive(pTextCtl, TRUE);
                ITEXTCTL_Redraw(pTextCtl);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            }
            return TRUE;

        case EVT_KEY_RELEASE:
        case EVT_USER_REDRAW:
            {
                AEETextInputMode nInputMode;
                nInputMode = ITEXTCTL_GetInputMode(pTextCtl,NULL);
                if (nInputMode == AEE_TM_SYMBOLS)
                {
                    return TRUE;
                }

                // 绘制底部操作提示条
                if ( WSTRLEN(ITEXTCTL_GetTextPtr(pTextCtl)) > 0 &&
                     ITEXTCTL_GetCursorPos(pTextCtl) != TC_CURSORSTART )
                {
                    //drawBottomBar(pMe->m_pDisplay, AEE_FONT_NORMAL,BTBAR_OK_DELETE);
                    SETTING_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
                }
                else
                {
                    //drawBottomBar(pMe->m_pDisplay, AEE_FONT_NORMAL,BTBAR_OK_BACK);
                    SETTING_MENU_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
                }

                // 统一更新界面
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);

            }
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                case AVK_SELECT:
                    {
                        AECHAR banner[OEMNV_BANNER_MAXLEN];

                        (void) ITEXTCTL_GetText(pTextCtl,
                                        banner,
                                        OEMNV_BANNER_MAXLEN);
                        (void) ICONFIG_SetItem(pMe->m_pConfig,
                                               CFGI_BANNER/*CFGI_PHONE_BANNER*/,
                                               banner,
                                               sizeof(banner));
                        CLOSE_DIALOG(DLGRET_CANCELED)
                    }
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleBannerDialogEvent

#endif
/*==============================================================================
函数：
       HandleTimeDialogEvent
说明：
       IDD_PHONESETTING_MENU 对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
#ifdef FEATURE_TIME_DATA_SETTING
static boolean  HandleTimeDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)
    static byte bytData = 0;
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_TIMESET);
    SETTING_ERR("%x, %x ,%x,HandleTimeDialogEvent",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }
     //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AMPM, IDS_AMPM, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_DAY, IDS_DAY, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16 wItemID;

                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
                IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_TIME_FORMAT,
                                       &bytData,
                                       sizeof(bytData));

                switch (bytData)
                {
                    case  OEMNV_TIMEFORM_AMPM:
                        wItemID = IDS_AMPM;
                        break;

                    case OEMNV_TIMEFORM_24HR :
                        wItemID = IDS_DAY;
                        break;

                    default:
                        wItemID = IDS_AMPM;
                        break;
                }

                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, wItemID, TRUE);
                IMENUCTL_SetSel(pMenu, wItemID);
                (void) ISHELL_PostEvent( pMe->m_pShell,
                                         AEECLSID_APP_SETTINGMENU,
                                         EVT_USER_REDRAW,
                                         0,
                                         0);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                  default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                byte bytNewData = 0;

                switch (wParam)
                {
                    case  IDS_AMPM:
                       bytNewData = OEMNV_TIMEFORM_AMPM;
                       break;

                    case IDS_DAY:
                       bytNewData = OEMNV_TIMEFORM_24HR;
                       break;

                    default:
                       ASSERT_NOT_REACHABLE;

                }

                if (bytNewData != bytData)
                {
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_TIME_FORMAT,
                                          &bytNewData, sizeof(bytNewData));
                    //将选中的选项标出
                    bytData = bytNewData;
                    InitMenuIcons(pMenu);
                    SetMenuIcon(pMenu, wParam, TRUE);
                    (void)IMENUCTL_Redraw(pMenu);
                }
                CLOSE_DIALOG(DLGRET_WARNING)
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleTimeDialogEvent


/*==============================================================================
函数：
       HandledateDialogEvent
说明：
       IDD_PHONESETTING_MENU 对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/

static boolean  HandleDateDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)
    static byte bytData = 0;
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_DATESET);
    SETTING_ERR("%x, %x ,%x,HandleDateDialogEvent",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }
     //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_DMY, IDS_DMY, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_MDY, IDS_MDY, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_YMD, IDS_YMD, NULL, 0);
            //IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_DMY, IDS_DMY, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16 wItemID;

                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
                IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_DATE_FORMAT,
                                       &bytData,
                                       sizeof(bytData));

                switch (bytData)
                {
                    case  OEMNV_DATEFORM_DMY:
                        wItemID = IDS_DMY;
                        break;

                    case OEMNV_DATEFORM_MDY :
                        wItemID = IDS_MDY;
                        break;

                    default:
                    case OEMNV_DATEFORM_YMD :
                        wItemID = IDS_YMD;
                        break;
                }

                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, wItemID, TRUE);
                IMENUCTL_SetSel(pMenu, wItemID);
                (void) ISHELL_PostEvent( pMe->m_pShell,
                                         AEECLSID_APP_SETTINGMENU,
                                         EVT_USER_REDRAW,
                                         0,
                                         0);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                  default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                byte bytNewData = 0;

                switch (wParam)
                {
                    case  IDS_DMY:
                       bytNewData = OEMNV_DATEFORM_DMY ;
                       break;

                    case IDS_MDY:
                       bytNewData = OEMNV_DATEFORM_MDY ;
                       break;

                    case IDS_YMD:
                       bytNewData = OEMNV_DATEFORM_YMD ;
                       break;

                    default:
                       ASSERT_NOT_REACHABLE;

                }

                if (bytNewData != bytData)
                {
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_DATE_FORMAT,
                                          &bytNewData, sizeof(bytNewData));
                    //将选中的选项标出
                    bytData = bytNewData;
                    InitMenuIcons(pMenu);
                    SetMenuIcon(pMenu, wParam, TRUE);
                    (void)IMENUCTL_Redraw(pMenu);
                }
                CLOSE_DIALOG(DLGRET_WARNING)
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleDATEDialogEvent

#endif
/*==============================================================================
函数：
       HandleSimDialogEvent
说明：
       IDD_PHONESETTING_MENU 对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
#ifdef  FEATURE_DOUBLE_SIM_CARD
static boolean HandleSimDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
    )
{
    PARAM_NOT_REF(dwParam)
    //static byte bytData = 0;
    //static boolean isSwitch = FALSE;
    nv_item_type nvi;
    int ret = 0;

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_SIMSET);
    SETTING_ERR("%x, %x ,%x,HandleDateDialogEvent",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SIM_ONE, IDS_SIM_ONE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SIM_TWO, IDS_SIM_TWO, NULL, 0);
            //IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SIM_SWITCH, IDS_SIM_SWITCH, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16 wItemID;

                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
                IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
               
                ret =OEMNV_Get(NV_ESN_OVER_WRITE_I,&nvi);
                if( NV_NOTACTIVE_S != OEMNV_Get(NV_ESN_OVER_WRITE_I,&nvi))
                {
                    switch (nvi.over_write_esn.esn)
                    {
                        case OEMNV_SIMFORM_ONE:
                            wItemID = IDS_SIM_ONE;
                            break;

                        case OEMNV_SIMFORM_TWO:
                            wItemID = IDS_SIM_TWO;
                            break;
                         default:
                            break;
                    }
                }
                ERR("HandleSimDialogEvent:::::%d::::ret:::%d",nvi.over_write_esn.esn,ret,0);
                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, wItemID, TRUE);
                IMENUCTL_SetSel(pMenu, wItemID);
                (void) ISHELL_PostEvent( pMe->m_pShell,
                                         AEECLSID_APP_SETTINGMENU,
                                         EVT_USER_REDRAW,
                                         0,
                                         0);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                  default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                 nv_item_type nviNew;
                switch (wParam)
                {
                    case  IDS_SIM_ONE:
                       nviNew.over_write_esn.esn = OEMNV_SIMFORM_ONE ;
                       break;

                    case IDS_SIM_TWO:
                       nviNew.over_write_esn.esn = OEMNV_SIMFORM_TWO ;
                       break;
                    default:
                       ASSERT_NOT_REACHABLE;

                }

                if (nviNew.over_write_esn.esn != nvi.over_write_esn.esn)
                {
                    ERR("HandleSimDialogEvent:::::22222%d",nviNew.over_write_esn.esn,0,0);
                    (void)OEMNV_Put(NV_ESN_OVER_WRITE_I,&nviNew);
                    //将选中的选项标出
                    nvi.over_write_esn.esn = nviNew.over_write_esn.esn;
                    InitMenuIcons(pMenu);
                    SetMenuIcon(pMenu, wParam, TRUE);
                    (void)IMENUCTL_Redraw(pMenu);
                    (void)ISHELL_SendEvent( pMe->m_pShell,  AEECLSID_CORE_APP, 
                                EVT_DISPLAYDIALOGTIMEOUT,  0, 0);
                }
                CLOSE_DIALOG(DLGRET_WARNING)
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
    
}
#endif
/*==============================================================================
函数：
       HandleLanguageDialogEvent
说明：
       IDD_LANGUAGE_MENU对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
#ifdef FEATURE_LANGUAGE
static boolean  HandleLanguageDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)
    nv_language_enum_type language;
    byte inputmode;

    IMenuCtl  *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                     IDC_LANGUAGE);
    SETTING_ERR("%x, %x ,%x,HandleLanguageDialogEvent",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:            
#ifdef FEATURE_LANG_CHINESE            
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CHINESE, IDS_CHINESE, NULL, 0);
#endif //FEATURE_LANG_TCHINESE

#ifdef FEATURE_LANG_TCHINESE            
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_TCHINESE, IDS_TCHINESE, NULL, 0);
#endif //FEATURE_LANG_TCHINESE

#if defined(FEATURE_LANG_ITALIAN)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ITALIAN, IDS_ITALIAN, NULL, 0);
#endif
#if defined(FEATURE_LANG_SPANISH)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SPANISH, IDS_SPANISH, NULL, 0);
#endif
#if defined(FEATURE_LANG_PORTUGUESE)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_PORTUGUESE, IDS_PORTUGUESE, NULL, 0);
#endif
#if defined(FEATURE_LANG_INDONESIAN)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_INDONESIAN, IDS_INDONESIAN, NULL, 0);
#if defined(FEATURE_CARRIER_INDONESIA)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_CHINESE, IDS_CHINESE, NULL, 0);
#endif
#endif
#if defined(FEATURE_LANG_HINDI)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_HINDI, IDS_HINDI, NULL, 0);
#endif
#if defined(FEATURE_LANG_VIETNAMESE)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_VIETNAMESE, IDS_VIETNAMESE, NULL, 0);
#endif
#if defined(FEATURE_LANG_ARABIC)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ARABIC, IDS_ARABIC, NULL, 0);
#endif
#if defined(FEATURE_LANG_HEBREW)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_HEBREW, IDS_HEBREW, NULL, 0);
#endif

#if defined(FEATURE_LANG_THAI)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_THAI, IDS_THAI, NULL, 0);
#endif

#if defined(FEATURE_LANG_RUSSIAN)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RUSSIAN, IDS_RUSSIAN, NULL, 0);
#endif
#if defined(FEATURE_LANG_FRENCH)
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_FRENCH,IDS_FRENCH,NULL, 0);
#endif

#ifdef FEATURE_LANG_ENGLISH
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ENGLISH, IDS_ENGLISH, NULL, 0);
#endif //FEATURE_LANG_ENGLISH
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16 nSelItem = 0;
                
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_LANGUAGE_SELECTION,
                                       &language,
                                       sizeof(language));

                switch (language)
                {
#ifdef FEATURE_LANG_CHINESE
                    case NV_LANGUAGE_CHINESE:       //中文
                        nSelItem = IDS_CHINESE;
                        break;
#endif /* FEATURE_LANG_CHINESE */

#ifdef FEATURE_LANG_TCHINESE
                    case NV_LANGUAGE_TCHINESE:       //繁体中文
                        nSelItem = IDS_TCHINESE;
                        break;
#endif /* FEATURE_LANG_TCHINESE */

#ifdef FEATURE_LANG_ITALIAN
                    case NV_LANGUAGE_ITALIAN:       //意大利文
                        nSelItem = IDS_ITALIAN;
                        break;
#endif /* FEATURE_LANG_ITALIAN */

#ifdef FEATURE_LANG_SPANISH
                    case NV_LANGUAGE_SPANISH:       //西班牙文
                        nSelItem = IDS_SPANISH;
                        break;
#endif /* FEATURE_LANG_SPANISH */

#ifdef FEATURE_LANG_PORTUGUESE
                    case NV_LANGUAGE_PORTUGUESE:    //葡萄牙文
                        nSelItem = IDS_PORTUGUESE;
                        break;
#endif /* FEATURE_LANG_PORTUGUESE */

#ifdef FEATURE_LANG_INDONESIAN
                    case NV_LANGUAGE_INDONESIAN:    //印度尼西亚
                        nSelItem = IDS_INDONESIAN;
                        break;
                        
#if defined(FEATURE_CARRIER_INDONESIA)
                    case NV_LANGUAGE_CHINESE:       //中文
                        nSelItem = IDS_CHINESE;
                        break;
#endif  
                        
#endif //FEATURE_LANG_INDONESIAN

#ifdef FEATURE_LANG_HINDI
                    case NV_LANGUAGE_HINDI:    //印度语
                        nSelItem = IDS_HINDI;
                        break;
#endif /*FEATURE_LANG_HINDI*/

#ifdef FEATURE_LANG_ARABIC
                    case NV_LANGUAGE_ARABIC:    //阿拉伯语
                        nSelItem = IDS_ARABIC;
                        break;
#endif /*FEATURE_LANG_ARABIC*/

#ifdef FEATURE_LANG_HEBREW
                    case NV_LANGUAGE_HEBREW:    //希伯来语
                        nSelItem = IDS_HEBREW;
                        break;
#endif /*FEATURE_LANG_HEBREW*/

#ifdef FEATURE_LANG_THAI
                    case NV_LANGUAGE_THAI:    //泰国语
                        nSelItem = IDS_THAI;
                        break;
#endif /*FEATURE_LANG_THAI*/

#ifdef FEATURE_LANG_VIETNAMESE
                    case NV_LANGUAGE_VIETNAMESE:    //越南语
                        nSelItem = IDS_VIETNAMESE;
                        break;
#endif /*FEATURE_LANG_VIETNAMESE*/

#ifdef FEATURE_LANG_RUSSIAN
                    case NV_LANGUAGE_RUSSIAN:    //俄文
                        nSelItem = IDS_RUSSIAN;
                        break;
#endif /*FEATURE_LANG_RUSSIAN*/

#if defined(FEATURE_LANG_FRENCH)
                    case NV_LANGUAGE_FRENCH:
                        nSelItem = IDS_FRENCH;
                        break;
#endif //#if defined(FEATURE_LANG_FRENCH)

                    case NV_LANGUAGE_ENGLISH:       //英文
                    default:
                        nSelItem = IDS_ENGLISH;
                        break;
                }

                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, nSelItem, TRUE);
                IMENUCTL_SetSel(pMenu, nSelItem);
            }

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            switch (wParam)
            {
#if 0   //add by yangdecai for inputmode
#ifdef FEATURE_LANG_CHINESE
                case IDS_CHINESE:       //中文
                    language = NV_LANGUAGE_CHINESE;
                    inputmode = OEM_MODE_T9_PINYIN;    
                    break;
#endif /* FEATURE_LANG_CHINESE */
#endif

#ifdef FEATURE_LANG_TCHINESE
                case IDS_TCHINESE:       //繁体中文
                    language = NV_LANGUAGE_TCHINESE;
                    inputmode = OEM_MODE_T9_ZHUYIN;    
                    break;
#endif /* FEATURE_LANG_TCHINESE */

#ifdef FEATURE_LANG_ITALIAN
                case IDS_ITALIAN:       //意大利文
                    language = NV_LANGUAGE_ITALIAN;
                    inputmode = OEM_MODE_T9_ITALIAN;     
                    break;
#endif /* FEATURE_LANG_ITALIAN */

#ifdef FEATURE_LANG_SPANISH
                case IDS_SPANISH:       //西班牙文
                    language = NV_LANGUAGE_SPANISH;
                    inputmode = OEM_MODE_T9_RAPID_SPANISH;  //OEM_MODE_T9_MT_SPANISH;
                    break;
#endif /* FEATURE_LANG_SPANISH */

#ifdef FEATURE_LANG_PORTUGUESE
                case IDS_PORTUGUESE:    //葡萄牙文
                    language = NV_LANGUAGE_PORTUGUESE;
                    inputmode = OEM_MODE_T9_MT_ENGLISH;//OEM_MODE_T9_RAPID_PORTUGUESE;//OEM_MODE_T9_MT_PORTUGUESE;
                    break;
#endif /* FEATURE_LANG_PORTUGUESE */

#ifdef FEATURE_LANG_INDONESIAN
                case IDS_INDONESIAN:    //印度尼西亚
                    language = NV_LANGUAGE_INDONESIAN;
                    inputmode = OEM_MODE_T9_MT_INDONESIAN;                           
                    break;
                    
#if defined(FEATURE_CARRIER_INDONESIA)
                case IDS_CHINESE:       //中文
                    language = NV_LANGUAGE_CHINESE;
                    (void) ICONFIG_GetItem(pMe->m_pConfig,
                                           CFGI_INPUTMODE,
                                           &inputmode,
                                           sizeof(inputmode));            
                    break;
#endif
                    
#endif /* FEATURE_LANG_INDONESIAN */

#ifdef FEATURE_LANG_HINDI
                case IDS_HINDI:    //印度语
                    language = NV_LANGUAGE_HINDI;
                    inputmode = OEM_MODE_T9_MT_HINDI;                          
                    break;
#endif /*FEATURE_LANG_HINDI*/

#ifdef FEATURE_LANG_ARABIC
                case IDS_ARABIC:    //阿拉伯语
                    language = NV_LANGUAGE_ARABIC;
                    inputmode = OEM_MODE_T9_MT_ARABIC;                             
                    break;
#endif /*FEATURE_LANG_ARABIC*/

#ifdef FEATURE_LANG_HEBREW
                case IDS_HEBREW:    //希伯来语
                    language = NV_LANGUAGE_HEBREW;
                    inputmode = OEM_MODE_T9_MT_HEBREW;                          
                    break;
#endif /*FEATURE_LANG_HEBREW*/

#ifdef FEATURE_LANG_THAI
                case IDS_THAI:    //泰国语
                    language = NV_LANGUAGE_THAI;
                    inputmode = OEM_MODE_T9_MT_THAI;                           
                    break;
#endif /*FEATURE_LANG_THAI*/

#ifdef FEATURE_LANG_VIETNAMESE
                case IDS_VIETNAMESE:    //越南语
                    language = NV_LANGUAGE_VIETNAMESE;
                    inputmode = OEM_MODE_T9_MT_VIETNAMESE;                         
                    break;
#endif /*FEATURE_LANG_VIETNAMESE*/

#ifdef FEATURE_LANG_RUSSIAN
                case IDS_RUSSIAN:    //俄文
                    language = NV_LANGUAGE_RUSSIAN;               
                    break;
#endif /*FEATURE_LANG_RUSSIAN*/

#if defined(FEATURE_LANG_FRENCH)
                case IDS_FRENCH:
                    language = NV_LANGUAGE_FRENCH;
                    inputmode = OEM_MODE_T9_MT_FRENCH;
                    break;
#endif //#if defined(FEATURE_LANG_FRENCH)

                case IDS_ENGLISH:       //英文
                default:
                    language = NV_LANGUAGE_ENGLISH;
              // inputmode = OEM_MODE_T9_MT_ENGLISH;
#if defined FEATURE_LANG_SPANISH
                    inputmode = OEM_MODE_T9_RAPID_ENGLISH;
#else
                    inputmode = OEM_MODE_T9_MT_ENGLISH;
#endif
                    break;
            }
            (void) ICONFIG_SetItem(pMe->m_pConfig,
                                   CFGI_LANGUAGE_SELECTION,
                                   &language,
                                   sizeof(language));
            (void) ICONFIG_SetItem(pMe->m_pConfig,
                                   CFGI_INPUTMODE,
                                   &inputmode,
                                   sizeof(inputmode));  
            InitMenuIcons(pMenu);
            SetMenuIcon(pMenu, wParam, TRUE);
            CLOSE_DIALOG(DLGRET_WARNING)
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleLanguageDialogEvent
#endif

/*==============================================================================
函数：
       SettingMenu_MakeForwardCall
说明：
       呼叫一个号码，处理呼叫转移和等待

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       pNum: 呼叫号码指针
       nNum: 呼叫号码长度

返回值：
       void
备注：

==============================================================================*/
static void SettingMenu_MakeForwardCall(CSettingMenu *pMe,char *Number)
{
    if((pMe->m_pShell) && (Number))
    {
        AECHAR w_buf[AEECM_MAX_DIGITS_LENGTH] = {0};
        ICallApp         *pCallApp = NULL;
        if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
                                                        AEECLSID_DIALER,
                                                        (void **)&pCallApp))
        {
            return ;
        }
        STRTOWSTR(Number, w_buf, sizeof(w_buf));
        DBGPRINTF("%S MakeVoiceCall",Number,0,0);
        //(void)MakeVoiceCall(pMe->m_pShell, FALSE, w_buf);
        ICallApp_CallNumber(pCallApp,w_buf);
        if (pCallApp)
        {
            ICallApp_Release(pCallApp);
            pCallApp = NULL;
        }
    }
}
/*==============================================================================
函数:
       SettingMenu_DialogTimeout

说明:
       定时器回调函数。主要用于自动关闭消息对话框。

参数:
       pme [in]：void *类型指针，暗指CSettingMenu结构指针。

返回值:
       无。

备注:

==============================================================================*/
static void SettingMenu_DialogTimeout(void *pme)
{
    CSettingMenu *pMe = (CSettingMenu *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent(pMe->m_pShell,
                            AEECLSID_APP_SETTINGMENU,
                            EVT_DISPLAYDIALOGTIMEOUT,
                            0,
                            0);
}

#ifdef FEATURE_PERU_VERSION
/*==============================================================================
函数：
       HandlePhoneNumberDialogEvent
说明：
       IDD_PHONE_NUMBER对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandlePhoneNumberDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    SETTING_ERR("%x, %x ,%x,HandlePhoneNumberDialogEvent",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                                    AEECLSID_APP_SETTINGMENU,
                                                    EVT_USER_REDRAW,
                                                    0,
                                                    0);
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT:
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    break;
            }
            return TRUE;

        case EVT_USER_REDRAW:
            {
                AECHAR        pText[20];
                AEERect       rc;
                RGBVAL        old_color;
                uint32 dwFlags = IDF_ALIGN_CENTER |
                                                        IDF_ALIGN_MIDDLE |
                                                        IDF_TEXT_TRANSPARENT;
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                                        CFGI_PHONE_NUMBER,
                                                        pText,
                                                        sizeof(pText));
                SETAEERECT(&rc,
                                                        0,
                                                        0,
                                                        102,
                                                        64);
                (void) IDISPLAY_DrawText(pMe->m_pDisplay,
                                                        AEE_FONT_LARGE,
                                                        pText,
                                                        -1,
                                                        0,
                                                        0,
                                                        &rc,
                                                        dwFlags);
                SETAEERECT(&rc,
                                                        0,
                                                        64,
                                                        102,
                                                        16);
                IDISPLAY_DrawRect(pMe->m_pDisplay, &rc,
                                                        MAKE_RGB(192, 192, 192),
                                                        MAKE_RGB(0, 0, 255),
                                                        IDF_RECT_FILL );

                ISHELL_LoadResString(pMe->m_pShell,
                                                        AEE_APPSSETTINGMENU_RES_FILE,
                                                        IDS_PHONE_NUMBER,
                                                        pText,
                                                        sizeof(pText));

                old_color = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, MAKE_RGB(192, 192, 192));

                (void) IDISPLAY_DrawText(pMe->m_pDisplay,
                                                        AEE_FONT_LARGE,
                                                        pText,
                                                        -1,
                                                        0,
                                                        0,
                                                        &rc,
                                                        dwFlags);
                IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, old_color);
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            }
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        default:
            break;
    }
    return FALSE;
}
#endif
#ifdef FEATURE_NETWORK_SET
/*==============================================================================
函数：
       HandleNetSelectDialogEvent
说明：
       IDD_NET_SELECT对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleNetSelectDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_NET_SELECT);
    nv_stat_enum_type ret;
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleNetSelectDialogEvent",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_NET_A, IDS_NET_A, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_NET_B, IDS_NET_B, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_NET_HOME, IDS_NET_HOME, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_NET_STANDARD, IDS_NET_STANDARD, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
        {
            nv_item_type nvi;
            nvi.system_pref.nam = cm_get_curr_nam();
            ret = OEMNV_Get(NV_SYSTEM_PREF_I,&nvi);
            SETTING_ERR("NV_SYSTEM_PREF_I %d %d %d",ret,nvi.system_pref.sys,nvi.system_pref.nam);
            InitMenuIcons(pMenu);
            switch(nvi.system_pref.sys)
            {
                case NV_SP_STANDARD:
                default:
                    IMENUCTL_SetSel(pMenu, IDS_NET_STANDARD);
                    SetMenuIcon(pMenu, IDS_NET_STANDARD, TRUE);
                    break;

                case NV_SP_HOME_ONLY:
                    IMENUCTL_SetSel(pMenu, IDS_NET_HOME);
                    SetMenuIcon(pMenu, IDS_NET_HOME, TRUE);
                    break;

                case NV_SP_B_ONLY:
                    IMENUCTL_SetSel(pMenu, IDS_NET_B);
                    SetMenuIcon(pMenu, IDS_NET_B, TRUE);
                    break;

                case NV_SP_A_ONLY:
                    IMENUCTL_SetSel(pMenu, IDS_NET_A);
                    SetMenuIcon(pMenu, IDS_NET_A, TRUE);
                    break;

            }
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_APP_SETTINGMENU,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
            return TRUE;
        }


        case EVT_COMMAND:
        {
           nv_item_type nvi;
           nvi.system_pref.nam = cm_get_curr_nam();
           OEMNV_Get(NV_SYSTEM_PREF_I,&nvi);
           switch(wParam)
           {
             case IDS_NET_STANDARD:
                nvi.system_pref.sys = NV_SP_STANDARD;
                break;

             case IDS_NET_HOME:
                nvi.system_pref.sys = NV_SP_HOME_ONLY;
                break;

             case IDS_NET_B:
                nvi.system_pref.sys = NV_SP_B_ONLY;
                break;

             case IDS_NET_A:
                nvi.system_pref.sys = NV_SP_A_ONLY;
                break;
             }
             ret = OEMNV_Put(NV_SYSTEM_PREF_I,&nvi);
             SETTING_ERR("NV_SYSTEM_PREF_I1 %d %d %d",ret,nvi.system_pref.sys,nvi.system_pref.nam);
             CLOSE_DIALOG(DLGRET_CANCELED)
             return TRUE;
        }

        case EVT_KEY:
            if(wParam == AVK_CLR)
            {
                CLOSE_DIALOG(DLGRET_CANCELED)
            }
            return TRUE;

        case EVT_USER_REDRAW:
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        default:
            break;
     }
     return FALSE;
}
#endif
#ifdef FEATURE_SID_LOCK
/*==============================================================================
函数：
       HandleNetSelectDialogEvent
说明：
       IDD_NET_SELECT对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleNet_Sid_Lock_SettingDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_NET_SELECT);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,HandleNet_Sid_Lock_SettingDialogEvent",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_LOCALNET, IDS_LOCALNET, NULL, 0);
         //IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTONET, IDS_AUTONET, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
        {            
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU,EVT_USER_REDRAW, 0,0);
            return TRUE;
        }


        case EVT_COMMAND:
        {
            switch(wParam)
            {
                case IDS_LOCALNET:
                    pMe->m_CallSettingSel =  IDS_LOCALNET;
                    CLOSE_DIALOG(DLGRET_CALLSETTINGSEL)
                    break;
                #if 0
                case IDS_AUTONET:
                {
                    if(Setting_inquire_Sid_lock_Cmd(0))
                    {
                        nv_item_type nvi;
                        nv_stat_enum_type ret;
                        db_items_value_type sid_lock;
                        sid_lock.b_sid_lock = 0;
                        db_put(DB_SID_LOCK, &sid_lock);
                        nvi.enabled_sid_lock.nam = cm_get_curr_nam();
                        nvi.enabled_sid_lock.b_sid_lock = 0;
                        ret = OEMNV_Put(NV_SID_LOCK_I,&nvi);
                        pMe->m_msg_id = IDS_RESET_PHONE;
                    }
                    else
                    {
                        pMe->m_msg_id = IDS_SET_SUCESS;                        
                    }
                    CLOSE_DIALOG(DLGRET_WARNING)
                    break;
                }
               #endif
                default:
                    break;
            }
            return TRUE;
        }

        case EVT_KEY:
            if(wParam == AVK_CLR)
            {
                CLOSE_DIALOG(DLGRET_CANCELED)
            }
            return TRUE;

        case EVT_USER_REDRAW:
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        default:
            break;
     }
     return FALSE;
}
#endif

static void SettingMenu_SetItemNumIcon(IMenuCtl   *pMenu)
{
}
/*==============================================================================
函数：
       SetMenu_SetCheckBoxAsOff
说明：
       用于设定给定菜单控件的全部菜单项单选按钮图标为非选中图标。

参数：
       pMenu {in]：IMenuCtl控件指针。

返回值：
       无。

备注：

==============================================================================*/
static void SetMenu_SetCheckBoxAsOff(IMenuCtl *pMenu)
{
    int nItemCount, index;
    uint16 wItemID;

    if (NULL == pMenu)
    {
        return;
    }

    nItemCount = IMENUCTL_GetItemCount(pMenu);
    for (index = 0; index < nItemCount; index++)
    {
        wItemID = IMENUCTL_GetItemID(pMenu, index);
        SetMenuIcon(pMenu, wItemID, FALSE);
    }
} // SetMenu_SetCheckBoxAsOff
#ifdef FEATRUE_AUTO_POWER
static void SettingMenu_Set_CTL(CSettingMenu *pMe)
{
    //设置控件焦点
    if (pMe->m_nCtlID == IDC_AUTO_POWER_STATUS)
    {
        IMENUCTL_SetActive(pMe->m_pStatus,TRUE);
        IMENUCTL_SetActive(pMe->m_pState, FALSE);
        ITIMECTL_SetActive(pMe->m_pTime, FALSE);
    }       
    if (pMe->m_nCtlID == IDC_AUTO_POWER_STATE)
    {
        IMENUCTL_SetActive(pMe->m_pStatus,FALSE);
        IMENUCTL_SetActive(pMe->m_pState, TRUE);
        ITIMECTL_SetActive(pMe->m_pTime, FALSE);
    }
    else if (pMe->m_nCtlID == IDC_AUTO_POWER_TIME)
    {
        IMENUCTL_SetActive(pMe->m_pStatus,FALSE);       
        IMENUCTL_SetActive(pMe->m_pState, FALSE);
        ITIMECTL_SetActive(pMe->m_pTime, TRUE);
    }
}

/*==============================================================================
函数：
       HandleAlarmSubDialogEvent
说明：
       IDD_ALARMCLOCK_SUB对话框事件处理函数

参数：
       pMe [in]：指向ClockApps Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  Setting_HandleAuto_Power_DialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{

    static byte timeFormatType = 0;

    SETTING_ERR("%x, %x ,%x,Setting_HandleAuto_Power_DialogEvent",eCode,wParam,dwParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            
            //功能选择控件
            if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                             AEECLSID_LISTCTL,
                             (void **)&pMe->m_pStatus))
            {
                return FALSE;
            }
            
            //状态控件
            if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                            AEECLSID_LISTCTL,
                            (void **)&pMe->m_pState))
            {
                (void)IMENUCTL_Release(pMe->m_pStatus);
                return FALSE;
            }
                

            //时间控件
            if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_COUNTDOWNCTL,
                                 (void **)&pMe->m_pTime))
            {
                (void)IMENUCTL_Release(pMe->m_pState);
                (void)IMENUCTL_Release(pMe->m_pStatus);
                return FALSE;
            }

            if(pMe->m_pTime == NULL ||pMe->m_pState == NULL)
            {
                SETTING_ERR("EVT_DIALOG_INIT FAIL", 0, 0, 0);
                return FALSE;
            }
#ifdef SET_REP_MOD_LIST
            //循环方式LIST 控件
            pMe->m_pRepMode = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_AUTO_POWER_REP_MODE);
            if (pMe->m_pRepMode == NULL )
            {
                return FALSE;
            }
#endif
            IMENUCTL_AddItem(pMe->m_pStatus, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTO_POWER_ON_TIME, IDS_AUTO_POWER_ON_TIME, NULL, 0);
            IMENUCTL_AddItem(pMe->m_pStatus, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTO_POWER_OFF_TIME, IDS_AUTO_POWER_OFF_TIME, NULL, 0);
            IMENUCTL_AddItem(pMe->m_pState, AEE_APPSSETTINGMENU_RES_FILE, IDS_ENABLE, IDS_ENABLE, NULL, 0);
            IMENUCTL_AddItem(pMe->m_pState, AEE_APPSSETTINGMENU_RES_FILE, IDS_DISABLE, IDS_DISABLE, NULL, 0);
            pMe->m_nCtlID = IDC_AUTO_POWER_STATUS;
            pMe->m_b_selete_Pm = FALSE;
            pMe->m_b_ampm_mode = OEMNV_TIMEFORM_24HR;
#if 0
            ICONFIG_GetItem(pMe->m_pConfig, CFGI_TIME_FORMAT, (void*)&pMe->m_b_ampm_mode, sizeof(byte));
#endif

        ITIMECTL_SetFont( pMe->m_pTime, AEE_FONT_BOLD, AEE_FONT_BOLD);
#if defined( FEATURE_CARRIER_THAILAND_HUTCH)
        ITIMECTL_SetFont( pMe->m_pTime, AEE_FONT_BOLD, AEE_FONT_BOLD);
#endif
            return TRUE;

        case EVT_DIALOG_START:
        {
            extern int OEM_GetConfig( AEEConfigItem i, void * pBuff, int nSize);
            OEM_GetConfig( CFGI_TIME_FORMAT, &timeFormatType, sizeof( byte));
            ITIMECTL_SetOemProperties( pMe->m_pTime, TP_OEM_COUNTDOWNCTL_EDITABLE|((timeFormatType==OEMNV_TIMEFORM_AMPM)?TP_OEM_COUNTDOWNCTL_12_FORMAT:0));

            (void) ISHELL_PostEvent( pMe->m_pShell,  AEECLSID_APP_SETTINGMENU, EVT_USER_REDRAW, 0,0);
            return TRUE;
        }

        case EVT_USER_REDRAW:
        {
            AEEMenuColors color;        //List Menu 的文本及背景颜色
            TitleBar_Param_type title;
            AECHAR      wszTitle[32];    //闹钟标题
            AECHAR      wszState[16];    //闹钟开关状态
            AECHAR      wszTime[16];     //时间
            AECHAR      wszStatus[16];
            AEERect     rc;
            int16       titleheight = GetTitleBarHeight(pMe->m_pDisplay);
            int16       bottomheight = GetBottomBarHeight(pMe->m_pDisplay);
            int16       itemheight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_BOLD, NULL, NULL);
            int         lineSpace = ( pMe->m_rc.dy - titleheight  - bottomheight - itemheight*3) / 4;
            nv_language_enum_type language = NV_LANGUAGE_ENGLISH;
            ICONFIG_GetItem(pMe->m_pConfig, CFGI_LANGUAGE_SELECTION, &language, sizeof(language));
            //SETTING_ERR("EVT_USER_REDRAW", 0, 0, 0);
            MEMSET(wszTitle,0,sizeof(wszTitle));
            MEMSET(wszState,0,sizeof(wszState));
            MEMSET(wszTime,0,sizeof(wszTime));
            MEMSET(wszStatus,0,sizeof(wszStatus));

            ITIMECTL_SetProperties(pMe->m_pTime,  TP_NO_SECONDS | TP_AUTOREDRAW);
            IMENUCTL_SetOemProperties(pMe->m_pState, OEMMP_SWITCHNAVIGATIONKEY|OEMMP_IDF_ALIGN_CENTER);
            IMENUCTL_SetOemProperties(pMe->m_pStatus, OEMMP_SWITCHNAVIGATIONKEY|OEMMP_IDF_ALIGN_CENTER);

            if(IMENUCTL_GetSel(pMe->m_pStatus) == IDS_AUTO_POWER_ON_TIME)
            {
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_AUTO_POWER_ON, (void*)&pMe->m_ClockCfg, sizeof(Auto_Power_Cfg));
            }
            else if(IMENUCTL_GetSel(pMe->m_pStatus) == IDS_AUTO_POWER_OFF_TIME)
            {
                IMENUCTL_SetSel(pMe->m_pStatus, IDS_AUTO_POWER_OFF_TIME);
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_AUTO_POWER_OFF, (void*)&pMe->m_ClockCfg, sizeof(Auto_Power_Cfg));
            }
            //STATE为真时,状态选中"开"                        

            if(pMe->m_ClockCfg.bStateOn == TRUE)
            {
                IMENUCTL_SetSel(pMe->m_pState, IDS_ENABLE);
            }
            else
            {
                IMENUCTL_SetSel(pMe->m_pState, IDS_DISABLE);
            }
            //记录响闹时间
            pMe->m_dwDispTime =   pMe->m_ClockCfg.dwWATime;

#if 0
            if(pMe->m_dwDispTime >= (12*MSSEC_PER_HOUR) && pMe->m_b_ampm_mode == OEMNV_TIMEFORM_AMPM)
            {
                pMe->m_dwDispTime = pMe->m_dwDispTime - 12*MSSEC_PER_HOUR;
                pMe->m_ClockCfg.dwWATime = pMe->m_dwDispTime;
                pMe->m_b_selete_Pm = TRUE;
            }
#endif
            pMe->m_dwHour = pMe->m_dwDispTime/MSSEC_PER_HOUR;
            pMe->m_dwMin   = pMe->m_dwDispTime%MSSEC_PER_HOUR;

            SETTING_ERR("m_dwHour = %d;m_dwMin = %d cfg= %d",pMe->m_dwHour,pMe->m_dwMin,pMe->m_dwDispTime);

            //设置给定的时间,并刷新屏幕
            (void)ITIMECTL_SetTimeEx(pMe->m_pTime,  pMe->m_dwDispTime, TRUE);
#ifdef SET_REP_MOD_LIST
            //根据重复方式选择方式List选项
            switch(pMe->m_ClockCfg.RepMode)
            {
                default:
                case WEEK_ALARM_REP1:   //每天
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_0);
                    break;

                case WEEK_ALARM_REP2:   //周一~周五
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_1);
                    break;

                case WEEK_ALARM_REP3:   //周一~周六
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_2);
                    break;

                case WEEK_ALARM_REP4:   //周一
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_3);
                    break;

                case WEEK_ALARM_REP5:   //周二
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_4);
                    break;

                case WEEK_ALARM_REP6:   //周三
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_5);
                    break;

                case WEEK_ALARM_REP7:   //周四
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_6);
                    break;

                case WEEK_ALARM_REP8:   //周五
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_7);
                    break;

                case WEEK_ALARM_REP9:   //周六
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_8);
                    break;

                case WEEK_ALARM_REP10:  //周日
                    IMENUCTL_SetSel(pMe->m_pRepMode, IDL_REP_MODE_9);
                    break;

            }
#endif

#ifdef FEATURE_COLOR_DISPLAY
            //初始化List Menu的颜色
            color.wMask = MC_SEL_TEXT | MC_SEL_BACK | MC_TEXT | MC_BACK;
            color.cBack = RGB_WHITE;
            color.cText =  RGB_GRAY;
            color.cSelBack = RGB_WHITE;
            color.cSelText = RGB_BLACK;
#else
            //初始化List Menu的颜色//the same
            color.wMask = MC_SEL_TEXT | MC_SEL_BACK | MC_TEXT | MC_BACK;
            color.cBack = RGB_WHITE;
            color.cText =  RGB_BLACK;
            color.cSelBack = RGB_BLACK;
            color.cSelText = RGB_WHITE;
#endif//FEATURE_COLOR_DISPLAY

            //填充背景颜色
#ifdef FEATURE_CARRIER_CHINA_VERTU
            {
                IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND);
                
                Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                if(pImageBg != NULL)
                {
                    IImage_Release(pImageBg);
                }
            }
#else
            Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif
            //IDISPLAY_FillRect(pMe->m_pDisplay,  &pMe->m_rc,  RGB_BLACK);
            
            //开机，关机选择控件矩形
            {
                SETAEERECT(&rc,  CONTROL_RECT_START_X+10, titleheight + lineSpace,
                                            pMe->m_rc.dx - CONTROL_RECT_START_X-20,
                                            itemheight + 8);
                IDISPLAY_EraseRect(pMe->m_pDisplay,&rc);
                IMENUCTL_SetRect(pMe->m_pStatus, &rc);          
            }
            
            //状态list控件矩形
            {             
                SETAEERECT(&rc,  CONTROL_RECT_START_X+10, titleheight + lineSpace*2 + itemheight,
                                            pMe->m_rc.dx - CONTROL_RECT_START_X-20,
                                            itemheight + 8);
                IDISPLAY_EraseRect(pMe->m_pDisplay,&rc);
                IMENUCTL_SetRect(pMe->m_pState, &rc);
            }
            

            // 时间控件的矩形
            {
                SETAEERECT(&rc,  CONTROL_RECT_START_X+10, titleheight + lineSpace*3 +itemheight*2,
                                                pMe->m_rc.dx - CONTROL_RECT_START_X - 20,
                                                itemheight + 8);
                ITIMECTL_SetRect(pMe->m_pTime, &rc);
                IDISPLAY_FillRect(pMe->m_pDisplay, &rc,  RGB_WHITE);
            }

            //闹钟标题
            (void) ISHELL_LoadResString(pMe->m_pShell,
                                            AEE_APPSSETTINGMENU_RES_FILE,
                                            IDS_AUTO_POWER_TITLE,
                                            wszTitle,
                                            sizeof(wszTitle));
                                            
            //功能选择标题
            (void) ISHELL_LoadResString(pMe->m_pShell,
                                            AEE_APPSSETTINGMENU_RES_FILE,
                                            IDS_AUTOPOWER_STATUS,
                                            wszStatus,
                                            sizeof(wszStatus));

            //状态标题
            (void) ISHELL_LoadResString(pMe->m_pShell,
                                            AEE_APPSSETTINGMENU_RES_FILE,
                                            IDS_AUTO_POWER_STATE,
                                            wszState,
                                            sizeof(wszState));

            //时间标题
            (void) ISHELL_LoadResString(pMe->m_pShell,
                                            AEE_APPSSETTINGMENU_RES_FILE,
                                            IDS_AUTO_POWER_TIME,
                                            wszTime,
                                            sizeof(wszTime));

            rc.x = 0;
            rc.dx = pMe->m_rc.dx;
            rc.y = 0;
            rc.dy = titleheight;
            MEMSET(&title,0,sizeof(TitleBar_Param_type));
            title.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER;
            title.pwszTitle = wszTitle;
            DrawTitleBar(pMe->m_pDisplay,&title);

            {
                RGBVAL nOldFontColor; //added by chengxiao 2009.03.05
                
                nOldFontColor = IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                 //功能
                (void)IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL,
                                      wszStatus,-1,1,2+ titleheight + lineSpace,NULL,
                                      IDF_ALIGN_LEFT | IDF_TEXT_TRANSPARENT);
                //状态
                (void)IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL,
                                      wszState,-1,1,2+ titleheight + lineSpace*2 + itemheight,NULL,
                                      IDF_ALIGN_LEFT | IDF_TEXT_TRANSPARENT);
                //时间
                (void)IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL,
                                      wszTime,-1,1,2+ titleheight + lineSpace*3 + itemheight*2,NULL,
                                      IDF_ALIGN_LEFT | IDF_TEXT_TRANSPARENT);
                IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
            }
            //设置LIST menu控件的颜色
            IMENUCTL_SetColors(pMe->m_pStatus, &color);
            IMENUCTL_SetColors(pMe->m_pState, &color);

            if( pMe->m_nCtlID == IDC_AUTO_POWER_TIME && timeFormatType == OEMNV_TIMEFORM_AMPM)
            {
                if( ITIMECTL_GetTime( pMe->m_pTime) >= 12*3600000)
                {
                    SETTING_MENU_DRAW_BOTTOMBAR( BTBAR_SAVE_AM_BACK);
                }
                else
                {
                    SETTING_MENU_DRAW_BOTTOMBAR( BTBAR_SAVE_PM_BACK);
                }
            }
            else
            {
                SETTING_MENU_DRAW_BOTTOMBAR( BTBAR_SAVE_BACK);
            }

            //Draw ARROW
            {
                IImage      *pR_ResImg = NULL;
                IImage      *pL_ResImg = NULL;
                pL_ResImg  = ISHELL_LoadResImage(pMe->m_pShell,AEE_APPSCOMMONRES_IMAGESFILE,IDB_LEFTARROW);
                pR_ResImg  = ISHELL_LoadResImage(pMe->m_pShell,AEE_APPSCOMMONRES_IMAGESFILE,IDB_RIGHTARROW);
                
                if(pR_ResImg != NULL)
                {
                    //在功能和状态模式后面画该ICON,表示上下键改变值
                    IIMAGE_Draw(pR_ResImg, pMe->m_rc.dx - 10, titleheight+ lineSpace + 4);
                    IIMAGE_Draw(pR_ResImg, pMe->m_rc.dx - 10, titleheight + lineSpace*2 + itemheight + 4);
                }

                if(pL_ResImg != NULL)
                {
                    //在功能和状态模式后面画该ICON,表示上下键改变值
                    IIMAGE_Draw(pL_ResImg, CONTROL_RECT_START_X,titleheight + lineSpace + 4);
                    IIMAGE_Draw(pL_ResImg, CONTROL_RECT_START_X,titleheight + lineSpace*2 + itemheight + 4);
                }
                IIMAGE_Release(pR_ResImg);
                pR_ResImg = NULL;
                IIMAGE_Release(pL_ResImg);
                pL_ResImg = NULL;
            }
            SettingMenu_Set_CTL(pMe);
            (void)IMENUCTL_Redraw(pMe->m_pStatus);
            (void)IMENUCTL_Redraw(pMe->m_pState);
            (void)ITIMECTL_Redraw(pMe->m_pTime);

            //drawBottomBar(pMe->m_pDisplay, AEE_FONT_NORMAL, BTBAR_SELECT_BACK);
            
            // 统一更新界面
            //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            return TRUE;
        }


        case EVT_DIALOG_END:
            //重置控件切换指示值
            pMe->m_nCtlID = IDC_AUTO_POWER_STATUS;   //控件ID
            pMe->m_nTimeCtlCount = 0;       //0代表焦点在小时上,1代表在分钟上
            pMe->m_nNumKeyCount = 0;        //重置编辑为小时的十位

            if(ITIMECTL_IsActive(pMe->m_pTime))
            {
                ITIMECTL_SetActive(pMe->m_pTime, FALSE);
            }
            IMENUCTL_Release(pMe->m_pState);
            ITIMECTL_Release(pMe->m_pTime);
            IMENUCTL_Release(pMe->m_pStatus);
            return TRUE;

        case EVT_KEY:
            //SETTING_ERR("EVT_KEY_RELEASE", 0, 0, 0);
            //时间控件下处理数字键设置时间
            if (ITIMECTL_IsActive(pMe->m_pTime))
            {
#if MIN_BREW_VERSION(3,0)
                // do not want to handle au
                if (((dwParam & KB_AUTOREPEAT) != 0) &&
                    ((wParam == AVK_UP) ||
                    (wParam == AVK_DOWN) ||
                    (wParam == AVK_LEFT) ||
                    (wParam == AVK_RIGHT)))
                {
                    return TRUE;
                }
#endif
                if(ITIMECTL_HandleEvent(pMe->m_pTime, eCode, wParam, dwParam))
                {
                    return TRUE;
                }
            }
            
            if (IMENUCTL_IsActive(pMe->m_pState))
            {
                if(IMENUCTL_HandleEvent(pMe->m_pState, eCode, wParam, dwParam))
                {
                    return TRUE;
                }
            }
            
            if (IMENUCTL_IsActive(pMe->m_pStatus))
            {
                if(IMENUCTL_HandleEvent(pMe->m_pStatus, eCode, wParam, dwParam))
                {
                    return TRUE;
                }
            }            

#if 0
            if((pMe->m_nCtlID == IDC_AUTO_POWER_TIME) &&
                (wParam == AVK_0 ||
                wParam == AVK_1 ||
                wParam == AVK_2 ||
                wParam == AVK_3 ||
                wParam == AVK_4 ||
                wParam == AVK_5 ||
                wParam == AVK_6 ||
                wParam == AVK_7 ||
                wParam == AVK_8 ||
                wParam == AVK_9 ))//为了避免焦点是时间控件时按其他键的多余处理
            {
                Setting_CClockApps_HandleNumKeyEvent(pMe, wParam);
            }
#endif

            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                case AVK_SELECT:
                case AVK_INFO:
                    //SETTING_ERR("EVT_KEY_RELEASE->AVK_SELECT", 0, 0, 0);
                    //当处于clockctl控件,不接收command事件
                    //处理Clock编辑界面应用COMMAND键保存设置内容.
#if 0
                    if(pMe->m_b_ampm_mode == OEMNV_TIMEFORM_AMPM)
                    {
                        if(IMENUCTL_GetSel(pMe->m_pState) == IDS_ENABLE)
                        {
                            pMe->m_ClockCfg.bStateOn = TRUE;
                        }
                        else
                        {
                            pMe->m_ClockCfg.bStateOn = FALSE;
                        }
                        pMe->m_ClockCfg.dwWATime = ITIMECTL_GetTime(pMe->m_pTime);
                        if(pMe->m_ClockCfg.bStateOn ==  FALSE)
                        {
                            if(Setting_CClockApps_HandleCmdEvent(pMe))
                            {
                                //SETTING_ERR("EVT_KEY->AVK_SELECT", 0, 0, 0);
                                CLOSE_DIALOG(DLGRET_CANCELED)
                                return TRUE;
                            }
                            return FALSE;
                        }
                        pMe->m_CallSettingSel = IDS_AM_PM_SET;
                        CLOSE_DIALOG(DLGRET_CALLSETTINGSEL)
                    }
                    else
#endif
                    {
                        if(Setting_CClockApps_HandleCmdEvent(pMe))
                        {
                            //SETTING_ERR("EVT_KEY->AVK_SELECT", 0, 0, 0);
                            pMe->m_msg_id = IDS_DONE;
                            CLOSE_DIALOG(DLGRET_WARNING)
                        }
                    }
                    break;

                default:
                    break;
            }
            return TRUE;

        case EVT_KEY_PRESS:
            //传送给自定义事件EVT_FOCUS_SWITCH处理,是为了避免切换控件的同时
            //控件提前处理方向键事件
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                            AEECLSID_APP_SETTINGMENU,
                                            EVT_FOCUS_SWITCH,
                                            wParam,
                                            0);
            return TRUE;
        case EVT_KEY_RELEASE:
            switch(wParam)
            {
                case AVK_SELECT:
                case AVK_LEFT:
                case AVK_RIGHT: 
                    return TRUE;
                default:
                    break;
            }
            break;
        case EVT_FOCUS_SWITCH:
        {
            //处理Clock编辑界面应用左右键切换控件
            Setting_CClockApps_HandleKeyEvent(pMe, wParam);

            if( wParam == AVK_UP || wParam == AVK_DOWN || wParam == AVK_INFO || wParam == AVK_STAR) //modified by chengxiao 2009.04.10
            {
                if( pMe->m_nCtlID == IDC_AUTO_POWER_TIME && timeFormatType == OEMNV_TIMEFORM_AMPM)
                {
                    if( ITIMECTL_GetTime( pMe->m_pTime) >= 12*3600000)
                    {
                        SETTING_MENU_DRAW_BOTTOMBAR( BTBAR_SAVE_AM_BACK);
                    }
                    else
                    {
                        SETTING_MENU_DRAW_BOTTOMBAR( BTBAR_SAVE_PM_BACK);
                    }
                }
                else
                {
                    SETTING_MENU_DRAW_BOTTOMBAR( BTBAR_SAVE_BACK);
                }
            }

            return TRUE;
        }
#if 1
        case EVT_COMMAND:
            if(IMENUCTL_GetSel(pMe->m_pState) == IDS_ENABLE)
            {
                pMe->m_ClockCfg.bStateOn = TRUE;
            }
            else
            {
                pMe->m_ClockCfg.bStateOn = FALSE;
            }
            //处理Clock编辑界面应用COMMAND键保存设置内容.
            if(Setting_CClockApps_HandleCmdEvent(pMe))
            {
                pMe->m_msg_id = IDS_DONE;
                CLOSE_DIALOG(DLGRET_WARNING)
            }
            return TRUE;
#endif
        default:
            break;
    }

    return FALSE;
} // HandleAlarmSubDialogEvent


/*==============================================================================
函数：
       Setting_CClockApps_HandleKeyEvent

说明：
       处理Clock编辑界面应用左右键切换控件.

参数：
       pMe [in]：指向Applet对象结构的指针。该结构包含小程序的特定信息。
       wParam[in]: 按键代码

返回值：
       boolean。

备注：:

==============================================================================*/
static boolean Setting_CClockApps_HandleKeyEvent(CSettingMenu *pMe, uint16 wParam)
{
    if (pMe == NULL)
    {
        return FALSE;
    }

    SETTING_ERR("%x %d Setting_CClockApps_HandleKeyEvent", wParam, pMe->m_nCtlID, 0);
    SETTING_ERR("CtlCount= %d KeyCount= %d ",pMe->m_nTimeCtlCount ,pMe->m_nNumKeyCount,0);
    switch(wParam)
    {
        case AVK_DOWN:
            if(pMe->m_nCtlID == IDC_AUTO_POWER_STATUS)     //控件为状态,切换到重复方式
            {
                pMe->m_nCtlID = IDC_AUTO_POWER_STATE;
            }
            else if(pMe->m_nCtlID == IDC_AUTO_POWER_STATE) //重复方式
            {
                ITIMECTL_SetProperties(pMe->m_pTime,
                                       TP_NO_SECONDS | TP_AUTOREDRAW);

                ITIMECTL_SetEditField(pMe->m_pTime, ITF_HOUR);
                pMe->m_nCtlID = IDC_AUTO_POWER_TIME;

            }
            else if(pMe->m_nCtlID == IDC_AUTO_POWER_TIME)
            {
                pMe->m_nCtlID = IDC_AUTO_POWER_STATUS;
                pMe->m_nNumKeyCount = 0; 
                pMe->m_nTimeCtlCount = 0;
            }
            else
            {
                pMe->m_nCtlID--;
            }
            SettingMenu_Set_CTL(pMe);
            return TRUE;
                        
        case AVK_UP:
            if(pMe->m_nCtlID == IDC_AUTO_POWER_TIME) 
            {
                
                ITIMECTL_SetProperties(pMe->m_pTime, TP_NO_SECONDS | TP_AUTOREDRAW);
                //编辑区域设置为分钟
                ITIMECTL_SetEditField(pMe->m_pTime, ITF_HOUR);

                pMe->m_nTimeCtlCount = 0;  //0代表焦点在小时上,1代表在分钟上
                pMe->m_nCtlID = IDC_AUTO_POWER_STATE;
            }
            else if(pMe->m_nCtlID == IDC_AUTO_POWER_STATE)   //时间设置
            {
                pMe->m_nCtlID = IDC_AUTO_POWER_STATUS;
            }
            else if(pMe->m_nCtlID == IDC_AUTO_POWER_STATUS)
            {
                ITIMECTL_SetProperties(pMe->m_pTime,
                                       TP_NO_SECONDS | TP_AUTOREDRAW);

                ITIMECTL_SetEditField(pMe->m_pTime, ITF_HOUR); 
                pMe->m_nCtlID = IDC_AUTO_POWER_TIME;
            }
            SettingMenu_Set_CTL(pMe);
            return TRUE;

        case AVK_RIGHT:
        case AVK_LEFT:
            if(pMe->m_nCtlID == IDC_AUTO_POWER_TIME)   //时间控件
            {
                if(pMe->m_nTimeCtlCount == 0)
                {
                    pMe->m_nTimeCtlCount = 1;
                    pMe->m_nNumKeyCount = 2;
                }
                else if(pMe->m_nTimeCtlCount == 1)
                {
                    pMe->m_nTimeCtlCount = 0;
                    pMe->m_nNumKeyCount = 0;
                }
            }
            if(pMe->m_nCtlID == IDC_AUTO_POWER_STATUS) //功能选择控件
            {
                if(IMENUCTL_GetSel(pMe->m_pStatus) == IDS_AUTO_POWER_ON_TIME)
                {
                    ICONFIG_GetItem(pMe->m_pConfig, CFGI_AUTO_POWER_ON, (void*)&pMe->m_ClockCfg, sizeof(Auto_Power_Cfg));
                }
                else if(IMENUCTL_GetSel(pMe->m_pStatus) == IDS_AUTO_POWER_OFF_TIME)
                {
                    IMENUCTL_SetSel(pMe->m_pStatus, IDS_AUTO_POWER_OFF_TIME);
                    ICONFIG_GetItem(pMe->m_pConfig, CFGI_AUTO_POWER_OFF, (void*)&pMe->m_ClockCfg, sizeof(Auto_Power_Cfg));
                }
                //STATE为真时,状态选中"开"                        
    
                if(pMe->m_ClockCfg.bStateOn == TRUE)
                {
                    IMENUCTL_SetSel(pMe->m_pState, IDS_ENABLE);
                }
                else
                {
                    IMENUCTL_SetSel(pMe->m_pState, IDS_DISABLE);
                }
                //记录响闹时间
                pMe->m_dwDispTime =   pMe->m_ClockCfg.dwWATime;
                pMe->m_dwHour = pMe->m_dwDispTime/MSSEC_PER_HOUR;
                pMe->m_dwMin   = pMe->m_dwDispTime%MSSEC_PER_HOUR;
                //设置给定的时间,并刷新屏幕
                (void)ITIMECTL_SetTimeEx(pMe->m_pTime,  pMe->m_dwDispTime, TRUE);                           
            }

            return TRUE;

#if 0
        case AVK_UP:
        case AVK_DOWN:
            //在时间控件激活的情况下,忽略按上下键所设置的时间
            if(pMe->m_nCtlID == IDC_AUTO_POWER_TIME)   //时间控件
            {
                (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);
            }
            return TRUE;
#endif
        default:
            break;
    }
    return TRUE;

}//end CClockApps_HandleKeyEvent
/*==============================================================================
函数：
       Setting_Process_Auto_Power

说明：

参数：

返回值：
       boolean。
       TRUE : cur is ok to set CFGI
       FALSE :cann't  set to CFGI

备注：:

==============================================================================*/
static boolean Setting_Process_Auto_Power(Auto_Power_Cfg *other_set, Auto_Power_Cfg *cur)
{
    if(other_set == NULL || cur == NULL)
    {
        return FALSE;
    }

    if((boolean)other_set->bStateOn == FALSE || (boolean)cur->bStateOn == FALSE)
    {
        return TRUE;
    }
    else
    {
        uint32 time = 0;
        if(other_set->dwWATime > cur->dwWATime)
        {
            time = other_set->dwWATime - cur->dwWATime;
        }
        else
        {
            time = cur->dwWATime - other_set->dwWATime;
        }
        SETTING_ERR("%d %d %d ",time ,other_set->dwWATime, cur->dwWATime);
        if(time >= 180000/*3 min*/)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    
}
/*==============================================================================
函数：
       Setting_CClockApps_HandleCmdEvent

说明：
       处理Clock编辑界面应用COMMAND键及SELECT键保存设置内容.

参数：
       pMe [in]：指向Applet对象结构的指针。该结构包含小程序的特定信息。


返回值：
       boolean。

备注：:

==============================================================================*/
    #include "err.h"

static boolean Setting_CClockApps_HandleCmdEvent(CSettingMenu *pMe)
{
    Auto_Power_Cfg   another_set;
    if (pMe == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("Setting_CClockApps_HandleCmdEvent %d %d %d", pMe->m_ClockCfg.dwWATime, pMe->m_b_selete_Pm, pMe->m_ClockCfg.bStateOn);

    //保存现在设置的时间
#if 0
    if(pMe->m_b_ampm_mode == OEMNV_TIMEFORM_AMPM)
    {
        pMe->m_ClockCfg.dwWATime = pMe->m_ClockCfg.dwWATime + pMe->m_b_selete_Pm *12 *  MSSEC_PER_HOUR;
    }
    else
#endif
    {
    ERR("pMe->m_pStatus= %d pMe->m_ClockCfg.dwWATime=%d",IMENUCTL_GetSel(pMe->m_pState),ITIMECTL_GetTime(pMe->m_pTime),0);
        //保存STATE的值
        if(IMENUCTL_GetSel(pMe->m_pState) == IDS_ENABLE)
        {
            pMe->m_ClockCfg.bStateOn = TRUE;
        }
        else
        {
            pMe->m_ClockCfg.bStateOn = FALSE;
        }
        pMe->m_ClockCfg.dwWATime = ITIMECTL_GetTime(pMe->m_pTime);
    }

    ERR("pMe->m_pStatus= %d",IMENUCTL_GetSel(pMe->m_pStatus),0,0);
    if(IMENUCTL_GetSel(pMe->m_pStatus) == IDS_AUTO_POWER_ON_TIME)
    {
        ICONFIG_GetItem(pMe->m_pConfig, CFGI_AUTO_POWER_OFF, (void*)&another_set, sizeof(Auto_Power_Cfg));
        if(Setting_Process_Auto_Power(&another_set,&pMe->m_ClockCfg) == FALSE)
        {
            pMe->m_msg_id = IDS_AUTOPOWER_WARNING;
            CLOSE_DIALOG(DLGRET_WARNING)
            return FALSE;
        }
        else
        {
            ICONFIG_SetItem(pMe->m_pConfig, CFGI_AUTO_POWER_ON, (void*)&pMe->m_ClockCfg, sizeof(Auto_Power_Cfg));
            OEMRTC_Process_Auto_Power_On();
        }
    }
    else if(IMENUCTL_GetSel(pMe->m_pStatus) == IDS_AUTO_POWER_OFF_TIME)
    {
        ICONFIG_GetItem(pMe->m_pConfig, CFGI_AUTO_POWER_ON, (void*)&another_set, sizeof(Auto_Power_Cfg));
        if(Setting_Process_Auto_Power(&another_set,&pMe->m_ClockCfg) == FALSE)
        {
            pMe->m_msg_id = IDS_AUTOPOWER_WARNING;
            CLOSE_DIALOG(DLGRET_WARNING)
            return FALSE;
        }
        else
        {
            ICONFIG_SetItem(pMe->m_pConfig, CFGI_AUTO_POWER_OFF, (void*)&pMe->m_ClockCfg, sizeof(Auto_Power_Cfg));
        }
    }
#ifdef SET_REP_MOD_LIST
    //保存重复方式的值
    switch(IMENUCTL_GetSel(pMe->m_pRepMode))
    {
        default:
        case IDL_REP_MODE_0:   //每天
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP1;
            break;
        case IDL_REP_MODE_1:   //周一~周五
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP2;
            break;
        case IDL_REP_MODE_2:   //周一~周六
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP3;
            break;
        case IDL_REP_MODE_3:   //周一
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP4;
            break;
        case IDL_REP_MODE_4:   //周二
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP5;
            break;
        case IDL_REP_MODE_5:   //周三
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP6;
            break;
        case IDL_REP_MODE_6:   //周四
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP7;
            break;
        case IDL_REP_MODE_7:   //周五
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP8;
            break;
        case IDL_REP_MODE_8:   //周六
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP9;
            break;
        case IDL_REP_MODE_9:   //周日
            pMe->m_ClockCfg.RepMode = WEEK_ALARM_REP10;
            break;
    }
#endif
    return TRUE;
}//end CClockApps_HandleCmdEvent
#if 0
/*==============================================================================
函数：
       Setting_CClockApps_HandleNumKeyEvent

说明：
       时间控件下按数字键设置时间

参数：
       pMe [in]：指向Applet对象结构的指针。该结构包含小程序的特定信息。
       wParam [in]: 按键事件.


返回值：
       boolean。

备注：:

==============================================================================*/
static boolean Setting_CClockApps_HandleNumKeyEvent(CSettingMenu *pMe, uint16 wParam)
{
    uint16 nKeyMask = 0;

    if (pMe == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x %d Setting_CClockApps_HandleNumKeyEvent", wParam, pMe->m_nNumKeyCount, 0);
    switch(wParam)
    {
        case AVK_0:
            nKeyMask = 0;
            break;

        case AVK_1:
            nKeyMask = 1;
            break;

        case AVK_2:
            nKeyMask = 2;
            break;

        case AVK_3:
            nKeyMask = 3;
            break;

        case AVK_4:
            nKeyMask = 4;
            break;

        case AVK_5:
            nKeyMask = 5;
            break;

        case AVK_6:
            nKeyMask = 6;
            break;

        case AVK_7:
            nKeyMask = 7;
            break;

        case AVK_8:
            nKeyMask = 8;
            break;

        case AVK_9:
            nKeyMask = 9;
            break;

        default:
            break;
    }

    if(pMe->m_nNumKeyCount == 0)    //小时的十位
    {
        if(pMe->m_b_ampm_mode == OEMNV_TIMEFORM_AMPM)
        {
            //小时的十位只有012可以设置,否则设置的时间将超界
            if(nKeyMask == 0 || nKeyMask == 1)
            {
                pMe->m_dwHour = 10* nKeyMask * MSSEC_PER_HOUR;
                pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin;

                (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);
            }
            else
            {
                return TRUE;
            }
        }
        else
        {
            //小时的十位只有012可以设置,否则设置的时间将超界
            if(nKeyMask == 0 || nKeyMask == 1 || nKeyMask == 2)
            {
                pMe->m_dwHour = 10* nKeyMask * MSSEC_PER_HOUR;
                pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin;

                (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);
            }
            else
            {
                return TRUE;
            }
        }
    }
    else if(pMe->m_nNumKeyCount == 1)   //小时的个位
    {
        if(pMe->m_b_ampm_mode == OEMNV_TIMEFORM_AMPM)
        {
            if(pMe->m_dwHour >= 10 *  MSSEC_PER_HOUR)//十位数是2时
            {
                if(nKeyMask == 0 || nKeyMask == 1)   //只处理01按键
                {
                    pMe->m_dwHour += nKeyMask * MSSEC_PER_HOUR;
                    pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin;
                }
                else    //其他按键直接返回
                {
                    return TRUE;
                }
            }
            else    //十位数小于2时
            {
                pMe->m_dwHour += nKeyMask * MSSEC_PER_HOUR;
                pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin;
            }
        }
        else
        {
            if(pMe->m_dwHour >= 10 * 2 * MSSEC_PER_HOUR)//十位数是2时
            {
                if(nKeyMask == 0 || nKeyMask == 1
                   || nKeyMask == 2 || nKeyMask == 3)   //只处理0123按键
                {
                    pMe->m_dwHour += nKeyMask * MSSEC_PER_HOUR;
                    pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin;
                }
                else    //其他按键直接返回
                {
                    return TRUE;
                }
            }
            else    //十位数小于2时
            {
                pMe->m_dwHour += nKeyMask * MSSEC_PER_HOUR;
                pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin;
            }
        }
        (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);

        //编辑区域自动设置为分钟
        ITIMECTL_SetEditField(pMe->m_pTime, ITF_MIN);
        pMe->m_nTimeCtlCount = 1;  //0代表焦点在小时上,1代表在分钟上

        (void)ITIMECTL_Redraw(pMe->m_pTime);
    }
    else if(pMe->m_nNumKeyCount == 2)   //分钟的十位
    {
        //分钟的十位不可以设置6789,否则设置的时间将超界
        if(nKeyMask != 6 && nKeyMask != 7
           && nKeyMask != 8 && nKeyMask != 9)
        {
            pMe->m_dwMin = 10 * nKeyMask * MSSEC_PER_MIN;
            pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin;
            if(pMe->m_dwDispTime >= (12*MSSEC_PER_HOUR) && pMe->m_b_ampm_mode == OEMNV_TIMEFORM_AMPM)
            {
                pMe->m_dwDispTime = pMe->m_dwDispTime - 12*MSSEC_PER_HOUR;
                pMe->m_b_selete_Pm = TRUE;
            }
            (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);
        }
        else
        {
            return TRUE;
        }
    }
    else if(pMe->m_nNumKeyCount == 3)  //分钟的个位
    {
        pMe->m_dwMin += nKeyMask * MSSEC_PER_MIN;
        pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin;
        if(pMe->m_dwDispTime >= (12*MSSEC_PER_HOUR) && pMe->m_b_ampm_mode == OEMNV_TIMEFORM_AMPM)
        {
            pMe->m_dwDispTime = pMe->m_dwDispTime - 12*MSSEC_PER_HOUR;
            pMe->m_b_selete_Pm = TRUE;
        }
        (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);

        ITIMECTL_SetActive(pMe->m_pTime, FALSE);
#ifdef SET_REP_MOD_LIST
        //自动设置控件焦点
        IDIALOG_SetFocus(pMe->m_pActiveDlg, IDC_REP_MOD_LIST);
        pMe->m_nCtlID = IDC_REP_MOD_LIST;
#endif
        pMe->m_nNumKeyCount = 0;    //重置m_nNumKeyCount

        return TRUE;    //重置了m_nNumKeyCount,直接返回
    }

    //时间控件按键次数增加
    pMe->m_nNumKeyCount++;

    return TRUE;
}
#endif
#endif

#ifdef FEATURE_CARRIER_THAILAND_HUTCH
static boolean Handle_ANSWER_MODE_DialogEveng(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_ANSWER_MODE);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,Handle_ANSWER_MODE_DialogEveng",eCode,wParam,dwParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ANYKEY_ANSWER_TITLE, IDS_ANYKEY_ANSWER_TITLE, NULL, 0);
#ifdef FEATRUE_DEVICETYPE_FLIP
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_COVER_ANSWER, IDS_COVER_ANSWER, NULL, 0);
#endif
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SEND_ANSWER, IDS_SEND_ANSWER, NULL, 0);
            //IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_HEADSET_ANSWER, IDS_HEADSET_ANSWER, NULL, 0);
//#ifdef FETRUE_SET_AUTOANSWER
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTOANSWER_TITLE, IDS_AUTOANSWER_TITLE, NULL, 0);
//#endif
            return TRUE;

        case EVT_DIALOG_START:
            SettingMenu_SetItemNumIcon(pMenu);
            IMENUCTL_SetSel(pMenu, pMe->m_currDlgId);

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            pMe->m_currDlgId = wParam;
            pMe->m_CallSettingSel = wParam;
            switch (wParam)
            {
                case IDS_ANYKEY_ANSWER_TITLE:
#ifdef FEATRUE_DEVICETYPE_FLIP
                case IDS_COVER_ANSWER:
#endif
                case IDS_SEND_ANSWER:
                case IDS_HEADSET_ANSWER:
                    CLOSE_DIALOG(DLGRET_CALLSETTINGSEL)
                    break;
//#ifdef FETRUE_SET_AUTOANSWER
                case IDS_AUTOANSWER_TITLE:
                    CLOSE_DIALOG(DLGRET_AUTO_ANSWER_MODE_SUB)
                    break;
//#endif
                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
}
#else //FEATURE_CARRIER_THAILAND_HUTCH
static boolean Handle_ANSWER_MODE_DialogEveng(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_ANSWER_MODE);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x, %x ,%x,Handle_ANSWER_MODE_DialogEveng",eCode,wParam,dwParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SEND_ANSWER, IDS_SEND_ANSWER, NULL, 0);            
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ANYKEY_ANSWER_TITLE, IDS_ANYKEY_ANSWER_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTOANSWER_TITLE, IDS_AUTOANSWER_TITLE, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16    ui16_return = IDS_SEND_ANSWER;
                byte      byte_return, autoanswer;
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_HEADSET_AUTOANSWER,&autoanswer,sizeof(autoanswer));
                if(autoanswer == OEMNV_HEADSET_AUTOANSWER_OFF)
                {
                    ICONFIG_GetItem(pMe->m_pConfig,CFGI_ANYKEY_ANSWER,&byte_return,sizeof(byte_return));
                    switch (byte_return)
                    {
                        case OEM_ANYKEY_ANSWER_MODE:  
                          ui16_return = IDS_ANYKEY_ANSWER_TITLE;
                          break;
                          
                        default:
                        case OEM_AUTO_ANSWER_MODE: 
                          ui16_return = IDS_SEND_ANSWER;
                          break;
                    }
                }
                else
                {
                    ui16_return = IDS_AUTOANSWER_TITLE;
                }

                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, ui16_return, TRUE);
                IMENUCTL_SetSel(pMenu, ui16_return);
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
                IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU, EVT_USER_REDRAW,0, 0);
            }            
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {             
                byte   callset;
                byte   autoanswertype;
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_ANYKEY_ANSWER, &callset,sizeof(callset));
                callset = callset & 0x0;

                switch (wParam)
                {
                    case IDS_ANYKEY_ANSWER_TITLE:                            
                        callset = callset | 0x1;
                        autoanswertype = OEMNV_HEADSET_AUTOANSWER_OFF;
                        break;
                        
                    default:
                    case IDS_SEND_ANSWER:  
                        callset = callset | 0x4;
                        autoanswertype = OEMNV_HEADSET_AUTOANSWER_OFF;
                        break;

                    case IDS_AUTOANSWER_TITLE:
                        callset = callset | 0x8;
                        autoanswertype = OEMNV_HEADSET_AUTOANSWER_5SEC;
                        break;
                        
                }
                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, wParam, TRUE);
                ICONFIG_SetItem(pMe->m_pConfig,CFGI_ANYKEY_ANSWER, &callset,sizeof(callset));
                ICONFIG_SetItem(pMe->m_pConfig,CFGI_HEADSET_AUTOANSWER, &autoanswertype,sizeof(autoanswertype));
                CLOSE_DIALOG(DLGRET_WARNING)
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
}
#endif //FEATURE_CARRIER_THAILAND_HUTCH

static void SettingMenu_Process_Feature_Code(CSettingMenu *pMe,uint16 feature_code)
{
    //uint16 Ruim_Active_code    = 0;
    //uint16 Ruim_Register_code = 0;
    //char  Assnum[18];
    
    if(NULL== pMe)
    {
        return;
    }

    if(IsRunAsUIMVersion())
    {
#ifdef FEATURE_CARRIER_INDONESIA
        uint16 Ruim_Active_code    = 0;
        uint16 Ruim_Register_code = 0;
        char  Assnum[18];
        switch(feature_code)
        {
            case CFGI_CALLFORWARD_DISABLE_ALL:
                Ruim_Active_code    = De_Active_Callforward_Unconditional;
                Ruim_Register_code = DE_REGISTER_CFU;
                break;

            case CFGI_CALLFORWARD_BUSY_ENABLE:
                Ruim_Active_code    = Active_Callforward_Busy;
                Ruim_Register_code = REGISTER_CFB;
                break;

            case CFGI_CALLFORWARD_BUSY_DISABLE:
                Ruim_Active_code    = De_Active_Callforward_Busy;
                Ruim_Register_code = DE_REGISTER_CFB;
                break;

            case CFGI_CALLFORWARD_NOANSWER_ENABLE:
                Ruim_Active_code    = Active_Callforward_No_Answer;
                Ruim_Register_code = REGISTER_CFNA;
                break;

            case CFGI_CALLFORWARD_NOANSWER_DISABLE:
                Ruim_Active_code    = De_Active_Callforward_No_Answer;
                Ruim_Register_code = DE_REGISTER_CFNA;
                break;

            case CFGI_CALLFORWARD_NOCONNECT_ENABLE:
                Ruim_Active_code     = Active_Callforward_Default;
                Ruim_Register_code  = REGISTER_CFD;
                break;

            case CFGI_CALLFORWARD_NOCONNECT_DISABLE:
                Ruim_Active_code    = De_Active_Callforward_Default;
                Ruim_Register_code = DE_REGISTER_CFD;
                break;

            case CFGI_CALLFORWARD_ANYWAY_ENABLE:
                Ruim_Active_code    = Active_Callforward_Unconditional;
                Ruim_Register_code = REGISTER_CFU;
                break;

            case CFGI_CALLFORWARD_ANYWAY_DISABLE:
                Ruim_Active_code    = De_Active_Callforward_Unconditional;
                Ruim_Register_code = DE_REGISTER_CFU;
                break;

            case CFGI_CALLFORWARD_WAIT_ENABLE:
                Ruim_Active_code     = Active_CallWaiting;
                Ruim_Register_code  = Active_CallWaiting;
                break;

            case CFGI_CALLFORWARD_WAIT_DISABLE:
                Ruim_Active_code    = De_Active_CallWaiting;
                Ruim_Register_code = De_Active_CallWaiting;              
                break;

            default:
                Ruim_Active_code    = De_Active_Callforward_Unconditional;
                Ruim_Register_code = De_Active_Callforward_Unconditional;
                break;
        }

        //Read supplement service number from RUIM Active section, if read fail or the number is wrong, then from RUIM Register section or config file
        MEMSET(Assnum, 0x00, sizeof(Assnum));
        if( SUCCESS == IRUIM_Get_Feature_Code(pMe->m_pIRUIM,(byte*)Assnum, Ruim_Active_code) )     
        {
            //if the supplement service number is "**"(Wrong number), then read supplement service number from RUIM Register section or config file
            if ( STRNCMP(Assnum,"**",2) )
            { 
                MEMCPY(pMe->m_callnumber, Assnum, sizeof(Assnum));
                return;
            }
        }

        //Read supplement service number from RUIM Register section,if read fail or the number is wrong, then renad from config file
        MEMSET(Assnum, 0x00, sizeof(Assnum));
        if(SUCCESS == IRUIM_Get_Feature_Code(pMe->m_pIRUIM,(byte*)Assnum,  Ruim_Register_code))
        {
            //if the supplement service number is "**"(Wrong number), then read from config file
            if ( STRNCMP(Assnum,"**",2) )
            { 
                MEMCPY(pMe->m_callnumber, Assnum, sizeof(Assnum));
                return;
            }
        }   
#endif

    }

    //Read supplement service number from Config
    ICONFIG_GetItem(pMe->m_pConfig, feature_code, pMe->m_callnumber, FEATURE_CODE_MAX_LENTH);   
    
    return;
}



#ifdef FEATRUE_SET_IP_NUMBER
static boolean Handle_IP_Number_Set_DialogEveng(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_IP_NUMBER_SET);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    SETTING_ERR("%x, %x ,%x,Handle_IP_Number_Set_DialogEveng",eCode,wParam,dwParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
        {
            IP_Number_Cfg  ip_cfg;
            MEMSET((void*)&ip_cfg,0 , sizeof(IP_Number_Cfg));

            ICONFIG_GetItem(pMe->m_pConfig,CFGI_IP_NUMBER,(void *)&ip_cfg,sizeof(IP_Number_Cfg));
            return TRUE;
        }

        case EVT_DIALOG_START:
            IMENUCTL_SetTitle(pMenu,AEE_APPSSETTINGMENU_RES_FILE,IDS_IP_NUMBER_SET, NULL);
            ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU,EVT_USER_REDRAW, 0,0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
            number_table * p_head = NULL;
            uint16 i = 0;
            boolean b_default_ip = TRUE;
            p_head = pMe->ip_number_tab.head;
            IMENUCTL_DeleteAll(pMenu);
#ifdef FEATURE_CARRIER_TAIWAN_APBW
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SET_POUND, IDS_SET_POUND, NULL, 0);
#endif
            if(pMe->ip_number_tab.count < OEM_IP_NUMBER_COUNT)
            {
                IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ADD_NEW_ITEM, IDS_ADD_NEW_ITEM, NULL, 0);
                IMENUCTL_SetSel(pMenu,IDS_ADD_NEW_ITEM);
            }
            else
            {
                IMENUCTL_SetSel(pMenu,0);
            }

            while(p_head)
            {
                CtlAddItem ai;
                MEMSET(&ai, 0, sizeof(ai));
                ai.pText = p_head->number;
                ai.wItemID = i;
                ai.pszResImage = AEE_APPSCOMMONRES_IMAGESFILE;
                if((b_default_ip == TRUE) &&(WSTRNCMP(pMe->ip_number_tab.default_ip,p_head->number,OEM_IP_NUMBER_MAX+1) == 0)) //only set first to default IP 
                {
                    b_default_ip = FALSE;
                    ai.wImage = IDB_CHECK_ON;
                }
                else
                {
                    ai.wImage = IDB_CHECK_OFF;
                }
                
                IMENUCTL_AddItemEx(pMenu, &ai);
                i ++;
                p_head = p_head->next;
            }

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL);
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            // 统一更新界面
            //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            IMENUCTL_Redraw(pMenu);
            return TRUE;
        }

        case EVT_DIALOG_END:
            if((pMe->ip_number_tab.head != NULL) && (WSTRLEN(pMe->ip_number_tab.default_ip) == 0))//not set default ip,set the first to 
            {
                WSTRLCPY(pMe->ip_number_tab.default_ip,pMe->ip_number_tab.head->number,OEM_IP_NUMBER_MAX+1);
            }
            return TRUE;

        case EVT_CTL_SEL_CHANGED:
            if(wParam == IDS_ADD_NEW_ITEM)
#ifdef FEATURE_CARRIER_TAIWAN_APBW
                || wParam == IDS_SET_POUND
#endif
                )
            {
                SETTING_MENU_DRAW_BOTTOMBAR(BTBAR_SELECT_BACK);
            }
            else
            {
                SETTING_MENU_DRAW_BOTTOMBAR(BTBAR_OPTION_BACK);
            }
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            switch (wParam)
            {
#ifdef FEATURE_CARRIER_TAIWAN_APBW
                case IDS_SET_POUND:
                    pMe->m_CallSettingSel =  IDS_SET_POUND;
                    CLOSE_DIALOG(DLGRET_CALLSETTINGSEL)
                    return TRUE;
#endif
                case IDS_ADD_NEW_ITEM:
                    pMe->m_input_mode = IDS_IP_NUMBER_SET;
                    CLOSE_DIALOG(DLGRET_CALLFORWARDINPUT)
                    return TRUE;

                default://del the wParam item
                {
#if 1
                    pMe->m_CallSettingSel =  IDS_SET_IP_CALL_OPTION;
                    pMe->m_ip_sel_num = wParam;
                    CLOSE_DIALOG(DLGRET_CALLSETTINGSEL)
#else
                    uint16 i = 0;
                    if(wParam == 0) //del the first node
                    {
                        if(pMe->ip_number_tab.head)
                        {
                            number_table * phead = NULL;
                            phead = pMe->ip_number_tab.head;
                            pMe->ip_number_tab.head = pMe->ip_number_tab.head->next;
                            FREE(phead);
                        }
                    }
                    else
                    {
                        number_table * phead = pMe->ip_number_tab.head;
                        while(phead)
                        {
                            if( i == (wParam -1))//find the node,del the next node
                            {
                                number_table * temp = NULL;
                                temp = phead->next;
                                if(temp)
                                {
                                    phead->next = temp->next;
                                    FREE(temp);
                                    break;
                                }
                            }
                            phead = phead->next;
                            i++;
                        }
                    }
                    pMe->ip_number_tab.count --;
                    if(pMe->ip_number_tab.count == 0)
                    {
                        pMe->ip_number_tab.ip_state =0;
                    }
                    ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU,EVT_USER_REDRAW, 0,0);
                    break;
#endif
                }
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
}
static number_table * SettingMenu_Search_Node(Ip_number_table *table,uint16 id)
{
    if(table)
    {
        uint16 i = 0;
        if(id == 0) // the first node
        {
            return table->head;
        }
        else
        {
            number_table * phead = table->head;
            while(phead)
            {
                if( i == (id -1))//find the node,del the next node
                {
                    number_table * temp = NULL;
                    temp = phead->next;
                    return temp;
                }
                phead = phead->next;
                i++;
            }
        }
    }
    return NULL;
}
#endif /* FEATRUE_SET_IP_NUMBER*/

/*void SettingMenu_show_Mem(CSettingMenu *pMe)
{
    IHeap    *heap = NULL;
    uint32     memsize= 0;
    AEEDeviceInfo  di;
    ERR("SettingMenu_show_Mem",0,0,0);
    ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
    (void)ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_HEAP, (void **)&heap);

    if (NULL == heap)
    {
        ERR("MP: Error: Failed to create IHeap instance",0,0,0);
        return;
    }
    memsize = IHEAP_GetMemStats(heap);
    ERR("IHEAP_GetMemStats %d total = %d",memsize,di.dwRAM,0);
    IHEAP_Release(heap);
    heap = NULL;
}*/

/*==============================================================================
函数：
       SettingMenu_HandleKeyToneLengthDialogEvent
说明：
       IDD_KEYTONE_LENGTH对话框事件处理函数

参数：
       pMe [in]：指向SoundMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  SettingMenu_HandleKeyToneLengthDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_KEYTONE_LENGTH);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x,%x,%x,HandleKeyToneLengthDialogEvent",eCode,wParam,dwParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_KEYTONE_NORMAL, IDS_KEYTONE_NORMAL, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_KEYTONE_LONG, IDS_KEYTONE_LONG, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16     ui16_return = IDS_KEYTONE_NORMAL;
                byte       Keylength;
                ICONFIG_GetItem(pMe->m_pConfig,CFGI_KEYTONE_LENGTH,&Keylength,sizeof(Keylength));
                switch(Keylength)
                {
                    case OEMNV_KEYTONE_NORMAL:            //NORMAL
                    default:
                        ui16_return = IDS_KEYTONE_NORMAL;
                        break;

                    case OEMNV_KEYTONE_LONG:             //LONG
                        ui16_return = IDS_KEYTONE_LONG;
                        break;

                }
                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, ui16_return, TRUE);
                IMENUCTL_SetSel(pMenu, ui16_return);
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU,EVT_USER_REDRAW,0,0);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            // 统一更新界面
            //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
        {
            byte byte_set;
            switch (wParam)
            {
                case IDS_KEYTONE_NORMAL:      //NORMAL
                    byte_set = OEMNV_KEYTONE_NORMAL;
                    break;

                case IDS_KEYTONE_LONG:        //LONG
                    byte_set = OEMNV_KEYTONE_LONG;
                    break;

                default:
                    ASSERT_NOT_REACHABLE;
            }

            ICONFIG_SetItem(pMe->m_pConfig,CFGI_KEYTONE_LENGTH,&byte_set,sizeof(byte_set));
            CLOSE_DIALOG(DLGRET_CANCELED)
            return TRUE;
        }
        default:
            break;
    }
    return FALSE;
} // HandleKeyToneLengthDialogEvent



/*==============================================================================
函数：
       HandleAutoAnswerSubDialogEvent
说明：
       IDD_AUTOANSWER_SUB对话框事件处理函数

参数：
       pMe [in]：指向SoundMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  HandleAutoAnswerSubDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    IMenuCtl  *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                     IDC_AUTOANSWER_SUB);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    SETTING_ERR("%x,%x,%x,HandleAutoAnswerSubDialogEvent",eCode,wParam,dwParam);
    //实现菜单循环滚动功能
    //SoundMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_OFF, IDS_OFF, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTOANSWER_5SEC, IDS_AUTOANSWER_5SEC, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_AUTOANSWER_10SEC, IDS_AUTOANSWER_10SEC, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16    ui16_return = IDS_OFF;
                byte      autoanswertype;
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_HEADSET_AUTOANSWER,&autoanswertype,sizeof(autoanswertype));
                switch (autoanswertype)
                {
                    default:
                    case OEMNV_HEADSET_AUTOANSWER_OFF:  //关
                      ui16_return = IDS_OFF;
                      break;

                    case OEMNV_HEADSET_AUTOANSWER_5SEC: //5秒
                      ui16_return = IDS_AUTOANSWER_5SEC;
                      break;

                    case OEMNV_HEADSET_AUTOANSWER_10SEC://10秒
                      ui16_return = IDS_AUTOANSWER_10SEC;
                      break;
                }

                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, ui16_return, TRUE);
                IMENUCTL_SetSel(pMenu, ui16_return);
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SETTINGMENU, EVT_USER_REDRAW,0, 0);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                byte   autoanswertype;
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_HEADSET_AUTOANSWER, &autoanswertype,sizeof(autoanswertype));

                switch (wParam)
                {
                    case IDS_OFF:    //关
                        autoanswertype = OEMNV_HEADSET_AUTOANSWER_OFF;
                        break;

                    case IDS_AUTOANSWER_5SEC:   //5秒
                        autoanswertype = OEMNV_HEADSET_AUTOANSWER_5SEC;
                        break;

                    case IDS_AUTOANSWER_10SEC:  //10秒
                        autoanswertype = OEMNV_HEADSET_AUTOANSWER_10SEC;
                        break;

                    default:
                        autoanswertype = OEMNV_HEADSET_AUTOANSWER_OFF;
                }

                ICONFIG_SetItem(pMe->m_pConfig,CFGI_HEADSET_AUTOANSWER, &autoanswertype,sizeof(autoanswertype));
                CLOSE_DIALOG(DLGRET_CANCELED)

            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleAutoAnswerSubDialogEvent
#ifdef FEATURE_SID_LOCK
static boolean Setting_inquire_Sid_lock_Cmd(byte cur_set)
{
    db_items_value_type sid_lock;
    db_get(DB_SID_LOCK, &sid_lock);
    if(sid_lock.b_sid_lock == cur_set)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
#endif
#ifdef FEATURE_SUPPORT_G_SENSOR
/*==============================================================================
函数：
       Setting_Handle_Shake
说明：
       IDD_SHAKE对话框事件处理函数

参数：
       pMe [in]：指向ClockApps Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean Setting_Handle_Shake(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                    IDC_SHAKE);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    switch (eCode)  
    {
        case EVT_DIALOG_INIT:
            
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_MUSIC, IDS_MUSIC, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_FM_RADIO, IDS_FM_RADIO, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_WALLPAPER, IDS_WALLPAPER, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_SNOOZE_ALARM, IDS_SNOOZE_ALARM, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_VIDEO, IDS_VIDEO, NULL, 0);
            return TRUE;
            
        case EVT_DIALOG_START:
            pMe->m_shaketype_sel = SET_SHAKE_NONE;
            IMENUCTL_SetSel(pMenu, pMe->m_nSubDlgId);
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
             return TRUE;
             
        case EVT_USER_REDRAW:
            
            (void)IMENUCTL_Redraw(pMenu);
                    return TRUE;

        case EVT_DIALOG_END:    
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            //pMe->m_nSubDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_nSubDlgId = wParam;
            switch (wParam)
            {
                case IDS_MUSIC:
                    pMe->m_shaketype_sel = SET_MUSIC_CHECK;
                    CLOSE_DIALOG(DLGRET_SHAKESUB)
                    break;
                case IDS_FM_RADIO:
                    pMe->m_shaketype_sel = SET_FM_RADIO_CHECK;
                    CLOSE_DIALOG(DLGRET_SHAKESUB)
                    break;
                    
                case IDS_WALLPAPER:
                    pMe->m_shaketype_sel = SET_WALLPAPER_CHECK;
                    CLOSE_DIALOG(DLGRET_SHAKESUB)
                    break;
                    
                case IDS_SNOOZE_ALARM:
                    pMe->m_shaketype_sel = SET_SNOOZE_ALARM_CHECK;
                    CLOSE_DIALOG(DLGRET_SHAKESUB)
                    break;
                case IDS_VIDEO:
                    pMe->m_shaketype_sel = SET_VIDEO_CHECK;
                    CLOSE_DIALOG(DLGRET_SHAKESUB)
                    break;
                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;
        default:
            break;              
        }
    return FALSE;    
}

/*==============================================================================
函数：
       Setting_Handle_ShakeSub
说明：
       IDD_SHAKESUB对话框事件处理函数

参数：
       pMe [in]：指向ClockApps Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean Setting_Handle_ShakeSub(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                    IDC_SHAKESUB);
    static boolean bData = FALSE; //用于保存NV项中的 CFGI_SHAKE_MUSIC_CHECK,           
    uint16   cfgi_value = CFGI_SHAKE_MUSIC_CHECK;
    if (pMenu == NULL)
    {
        return FALSE;
    }
    switch (eCode)  
    {
        case EVT_DIALOG_INIT:
            
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ON, IDS_ON, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_OFF, IDS_OFF, NULL, 0);

            return TRUE;
            
        case EVT_DIALOG_START:
        {
            uint16 wItemID;
            uint16 string_id = IDS_MUSIC;
            IMENUCTL_SetSel(pMenu, pMe->m_sSubDlgId);
            InitMenuIcons(pMenu);
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            if(pMe->m_shaketype_sel == SET_MUSIC_CHECK)
            {
                cfgi_value = CFGI_SHAKE_MUSIC_CHECK;
            }
            else if(pMe->m_shaketype_sel == SET_FM_RADIO_CHECK)
            {
                cfgi_value = CFGI_SHAKE_FM_RADIO_CHECK;
                string_id = IDS_FM_RADIO;
            }
            else if(pMe->m_shaketype_sel == SET_WALLPAPER_CHECK)
            {
                cfgi_value = CFGI_SHAKE_WALLPAPER_CHECK;
                string_id = IDS_WALLPAPER;
            }
            else if(pMe->m_shaketype_sel == SET_SNOOZE_ALARM_CHECK)
            {
                cfgi_value = CFGI_SHAKE_SNOOZE_ALARM_CHECK;
                string_id = IDS_SNOOZE_ALARM;
            }
            else if(pMe->m_shaketype_sel == SET_VIDEO_CHECK)
            {
                cfgi_value = CFGI_SHAKE_VIDEO_CHECK;
                string_id = IDS_VIDEO;
            }
            
            IMENUCTL_SetTitle(pMenu,
                               AEE_APPSSETTINGMENU_RES_FILE,
                               string_id,
                               NULL);
            (void) ICONFIG_GetItem(pMe->m_pConfig,
                                    cfgi_value,
                                    &bData,
                                    sizeof(bData));
            SetMenu_SetCheckBoxAsOff(pMenu);
            
            if(bData == TRUE)
            {
                wItemID = IDS_ON;
            }
            else
            {
                wItemID = IDS_OFF;
            }
            SetMenuIcon(pMenu, wItemID, TRUE);
            IMENUCTL_SetSel(pMenu, wItemID);
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_APP_SETTINGMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
             return TRUE;
        }
             
        case EVT_USER_REDRAW:
            
            (void)IMENUCTL_Redraw(pMenu);
                    return TRUE;

        case EVT_DIALOG_END:    
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
        {     
            boolean  bNewData;
            pMe->m_sSubDlgId = IMENUCTL_GetSel(pMenu);
            switch(wParam)
            {
                case IDS_ON:
                    bNewData = TRUE;
                    break;

                case IDS_OFF:
                    bNewData = FALSE;
                    break;

                default:
                    return FALSE;

            }
            if (bNewData != bData)
            {
                if(pMe->m_shaketype_sel == SET_MUSIC_CHECK)
                {
                    cfgi_value = CFGI_SHAKE_MUSIC_CHECK;
                }
                else if(pMe->m_shaketype_sel == SET_FM_RADIO_CHECK)
                {
                    cfgi_value = CFGI_SHAKE_FM_RADIO_CHECK;
                }
                else if(pMe->m_shaketype_sel == SET_WALLPAPER_CHECK)
                {
                    cfgi_value = CFGI_SHAKE_WALLPAPER_CHECK;
                }
                else if(pMe->m_shaketype_sel == SET_SNOOZE_ALARM_CHECK)
                {
                    cfgi_value = CFGI_SHAKE_SNOOZE_ALARM_CHECK;
                }
                else if(pMe->m_shaketype_sel == SET_VIDEO_CHECK)
                {
                    cfgi_value = CFGI_SHAKE_VIDEO_CHECK;
                }           

                (void)ICONFIG_SetItem(pMe->m_pConfig,
                                       cfgi_value,
                                       &bNewData,
                                       sizeof(bNewData));
#ifdef FEATURE_APP_MUSICPLAYER
                if(GetMp3PlayerStatus() == MP3STATUS_RUNONBACKGROUND && bNewData && cfgi_value == CFGI_SHAKE_MUSIC_CHECK)
                {
                    (void)ISHELL_PostEvent(pMe->m_pShell,
                                           AEECLSID_APP_MUSICPLAYER,
                                           EVT_OPENSHAKE,
                                           0,
                                           0);
                }
#endif
                {
                    boolean b_FMBackground = FALSE;
                    OEM_GetConfig(CFGI_FM_BACKGROUND,&b_FMBackground, sizeof(b_FMBackground));
                    if((TRUE == b_FMBackground)&& bNewData && (cfgi_value == CFGI_SHAKE_FM_RADIO_CHECK))
                    {
                        (void)ISHELL_PostEvent(pMe->m_pShell,
                                               AEECLSID_APP_FMRADIO,
                                               EVT_OPENSHAKE,
                                               0,
                                               0);                        
                    }
                }
                //将选中的选项标出
                bData = bNewData;
                SetMenu_SetCheckBoxAsOff(pMenu);
                SetMenuIcon(pMenu, wParam, TRUE);
                (void)IMENUCTL_Redraw(pMenu);
            }

            pMe->m_msg_id = IDS_DONE;
            CLOSE_DIALOG(DLGRET_WARNING);   
            return TRUE;
        }

       //自动返回上级对话框
       case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_CANCELED)
        default:
            break;              
    }
    return FALSE;    
}
#endif //#ifdef FEATURE_SUPPORT_G_SENSOR
/*==============================================================================
函数：
       Setting_Handle_CallRestrict
说明：
       IDC_RESTRICT

参数：
       pMe [in]：指向ClockApps Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/

static boolean  Setting_Handle_CallRestrict(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                         IDC_RESTRICT);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RESTRICT_OUTGOING, IDS_RESTRICT_OUTGOING, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RESTRICT_INCOMING, IDS_RESTRICT_INCOMING, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            //设定标题格式
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE | MP_WRAPSCROLL | MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);

            IMENUCTL_SetSel(pMenu, pMe->m_sSubDlgId);

            (void) ISHELL_PostEvent(pMe->m_pShell,
                                   AEECLSID_APP_SETTINGMENU,
                                   EVT_USER_REDRAW,
                                   0,
                                   0);
            return TRUE;

        case EVT_USER_REDRAW:

            (void)IMENUCTL_Redraw(pMenu);

            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                   CLOSE_DIALOG(DLGRET_CANCELED)
                   return TRUE;

                default:
                   break;
            }
            return TRUE;

        case EVT_COMMAND:
            //pMe->m_sSubDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_sSubDlgId = wParam;
            switch(wParam)
            {
                case IDS_RESTRICT_OUTGOING:   //呼出限制
                    CLOSE_DIALOG(DLGRET_RESTRICTOUTGOING)
                    break;

                case IDS_RESTRICT_INCOMING:   //呼入限制
                    CLOSE_DIALOG(DLGRET_RESTRICTINCOMING)
                    break;

                default:
                    ASSERT_NOT_REACHABLE;
            }
            return TRUE;

        default:
             break;
    }
    return FALSE;
} // HandleRestrictDialogEvent

/*==============================================================================
函数：
       Setting_Handle_OutGoing
说明：
       IDD_RESTRICT_OUTGOING

参数：
       pMe [in]：指向ClockApps Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/

static boolean  Setting_Handle_OutGoing(CSettingMenu *pMe,
                                           AEEEvent       eCode,
                                           uint16         wParam,
                                           uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    static byte bytData = 0;
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_RESTRICT_OUTGOING);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RESTRICT_OUTGOING_ALL, IDS_RESTRICT_OUTGOING_ALL, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RESTRICT_OUTGOING_INTERNATIONAL, IDS_RESTRICT_OUTGOING_INTERNATIONAL, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RESTRICT_OUTGOING_OUTCONTACT, IDS_RESTRICT_OUTGOING_OUTCONTACT, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RESTRICT_OUTGOING_OFF, IDS_RESTRICT_OUTGOING_OFF, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16 wItemID;
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
                IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_RESTRICT_OUTGOING,
                                       &bytData,
                                       sizeof(bytData));

                switch (bytData)
                {
                    case OEMNV_RESTRICT_OUTGOING_ALL:           //全部
                        wItemID = IDS_RESTRICT_OUTGOING_ALL;
                        break;

                    case OEMNV_RESTRICT_OUTGOING_INTERNATIONAL: //国际长途
                        wItemID = IDS_RESTRICT_OUTGOING_INTERNATIONAL;
                        break;

                    case OEMNV_RESTRICT_OUTGOING_OUTCONTACT:    //通讯簿以外
                        wItemID = IDS_RESTRICT_OUTGOING_OUTCONTACT;
                        break;

                    case OEMNV_RESTRICT_OUTGOING_OFF:           //关闭
                        wItemID = IDS_RESTRICT_OUTGOING_OFF;
                        break;

                    default:
                        wItemID = IDS_RESTRICT_OUTGOING_OFF;
                        break;
                }
                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, wItemID, TRUE);
                IMENUCTL_SetSel(pMenu, wItemID);
                (void) ISHELL_PostEvent( pMe->m_pShell,
                                         AEECLSID_APP_SETTINGMENU,
                                         EVT_USER_REDRAW,
                                         0,
                                         0);
            }
            return TRUE;

        case EVT_USER_REDRAW:

            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                  default:
                    break;
            }
            return TRUE;
        case EVT_COMMAND:
            {
                byte bytNewData = 0;

                switch (wParam)
                {
                    case IDS_RESTRICT_OUTGOING_ALL:            //全部
                       bytNewData = OEMNV_RESTRICT_OUTGOING_ALL;
                       break;

                    case IDS_RESTRICT_OUTGOING_INTERNATIONAL:   //国际长途
                       bytNewData = OEMNV_RESTRICT_OUTGOING_INTERNATIONAL;
                       break;

                    case IDS_RESTRICT_OUTGOING_OUTCONTACT:      //通讯簿以外
                       bytNewData = OEMNV_RESTRICT_OUTGOING_OUTCONTACT;
                       break;

                    case IDS_RESTRICT_OUTGOING_OFF:             //关闭
                       bytNewData = OEMNV_RESTRICT_OUTGOING_OFF;
                       break;

                    default:
                       ASSERT_NOT_REACHABLE;

                }

                if (bytNewData != bytData)
                {
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_RESTRICT_OUTGOING,
                                          &bytNewData, sizeof(bytNewData));
                    //将选中的选项标出
                    bytData = bytNewData;
                    InitMenuIcons(pMenu);
                    SetMenuIcon(pMenu, wParam, TRUE);
                    (void)IMENUCTL_Redraw(pMenu);
                }
                CLOSE_DIALOG(DLGRET_WARNING)
            }
            return TRUE;
        default:
            break;
    }
    return FALSE;
} // HandleRestrictOutgoingDialogEvent

/*==============================================================================
函数：
       Setting_Handle_Incoming
说明：
       IDD_RESTRICT_Incoming

参数：
       pMe [in]：指向ClockApps Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/

static boolean  Setting_Handle_Incoming(CSettingMenu *pMe,
                                           AEEEvent       eCode,
                                           uint16         wParam,
                                           uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)

    static byte bytData = 0;

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_RESTRICT_INCOMING);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RESTRICT_INCOMING_ALL, IDS_RESTRICT_INCOMING_ALL, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RESTRICT_INCOMING_OUTCONTACT, IDS_RESTRICT_INCOMING_OUTCONTACT, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_RESTRICT_INCOMING_OFF, IDS_RESTRICT_INCOMING_OFF, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
           {
                uint16 wItemID;
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
                IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                    CFGI_RESTRICT_INCOMING,
                                    &bytData,
                                    sizeof(bytData));

                switch (bytData)
                {
                    case OEMNV_RESTRICT_INCOMING_ALL:           //全部
                        wItemID = IDS_RESTRICT_INCOMING_ALL;
                        break;

                    case OEMNV_RESTRICT_INCOMING_OUTCONTACT:    //通讯簿以外
                        wItemID = IDS_RESTRICT_INCOMING_OUTCONTACT;
                        break;

                    case OEMNV_RESTRICT_INCOMING_OFF:           //关闭
                        wItemID = IDS_RESTRICT_INCOMING_OFF;
                        break;

                    default:
                        wItemID = IDS_RESTRICT_INCOMING_OFF;
                        break;
                }
                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, wItemID, TRUE);
                IMENUCTL_SetSel(pMenu, wItemID);

                (void) ISHELL_PostEvent( pMe->m_pShell,
                                         AEECLSID_APP_SETTINGMENU,
                                         EVT_USER_REDRAW,
                                         0,
                                         0);
            }

            return TRUE;

        case EVT_USER_REDRAW:

           (void)IMENUCTL_Redraw(pMenu);
           return TRUE;

        case EVT_DIALOG_END:
           return TRUE;

        case EVT_KEY:
           switch(wParam)
           {
               case AVK_CLR:
                   CLOSE_DIALOG(DLGRET_CANCELED)
                   return TRUE;

              default:
                   break;
           }

           return TRUE;
        case EVT_COMMAND:
           {
               byte bytNewData = OEMNV_RESTRICT_INCOMING_OFF;

                switch (wParam)
                {
                    case IDS_RESTRICT_INCOMING_ALL:         //全部
                        bytNewData = OEMNV_RESTRICT_INCOMING_ALL;
                        break;

                    case IDS_RESTRICT_INCOMING_OUTCONTACT:  //通讯簿以外
                        bytNewData = OEMNV_RESTRICT_INCOMING_OUTCONTACT;
                        break;

                    case IDS_RESTRICT_INCOMING_OFF:         //关闭
                        bytNewData = OEMNV_RESTRICT_INCOMING_OFF;
                        break;

                    default:
                        ASSERT_NOT_REACHABLE;
                }

                if (bytNewData != bytData)
                {
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_RESTRICT_INCOMING,
                                          &bytNewData,
                                          sizeof(bytNewData));
                    bytData = bytNewData;

                    InitMenuIcons(pMenu);
                    SetMenuIcon(pMenu, wParam, TRUE);
                    (void)IMENUCTL_Redraw(pMenu);
                }
                CLOSE_DIALOG(DLGRET_WARNING)
            }
            return TRUE;
        default:
            break;
    }
    return FALSE;
} // HandleRestrictIncomingDialogEvent

/*==============================================================================
函数：
       Setting_Handle_Password
说明：


参数：
       pMe [in]：指向ClockApps Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean  Setting_Handle_Password(CSettingMenu *pMe,
                                         AEEEvent       eCode,
                                         uint16         wParam,
                                         uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    //static char   *m_strPhonePWD = NULL;
    AECHAR         wstrDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};
    int            nLen = 0;
    char           strDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(NULL == pMe->m_strPhonePWD)
            {
                pMe->m_strPhonePWD = (char *)MALLOC((OEMNV_LOCKCODE_MAXLEN + 1)* sizeof(char)); 
            }
            return TRUE;
            
        case EVT_DIALOG_START:  
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SETTINGMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);

            return TRUE;
            
        case EVT_USER_REDRAW:
            // 绘制相关信息
            {
                AECHAR  text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};
                
                // 先清屏
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif
                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc,RGB_BLACK);
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                    AEE_APPSSETTINGMENU_RES_FILE,
                                                IDS_CALL_RESTRICT, 
                                                text,
                                                sizeof(text));                  
                // 画标题条
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);

                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                    AEE_APPSSETTINGMENU_RES_FILE,
                                                IDS_PASSWORD, 
                                                text,
                                                sizeof(text));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                    AEE_FONT_BOLD, 
                                    text,
                                    -1, 
                                    5, 
                                    (pMe->m_rc.dy)*2/11, 
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
                
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                AEE_FONT_BOLD, 
                                wstrDisplay,
                                -1, 
                                10, 
                                TITLEBAR_HEIGHT + MENUITEM_HEIGHT*3/2,
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
        
                // 绘制底条提示
                if (nLen > 3)
                {// 确定-----删除
                    SETTING_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
                }
                else if(nLen > 0)
                {
                    SETTING_MENU_DRAW_BOTTOMBAR(BTBAR_DELETE)
                }
                else
                {// 确定-----取消
                    SETTING_MENU_DRAW_BOTTOMBAR(BTBAR_CANCEL)
                }
                // 更新显示
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);  
                return TRUE;
                }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
                FREEIF(pMe->m_strPhonePWD);
            }
            return TRUE;

        case EVT_KEY:
            {
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;
                
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                        
                    case AVK_CLR:
                        chEnter = 0;    
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        break;
                        
                    case AVK_SELECT:
                    case AVK_INFO:
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) < 4) //modified by chengxiao 2009.03.06
                        {
                            return TRUE;
                        }
                        else
                        {
                            uint16 wPWD=0;
                            (void) ICONFIG_GetItem(pMe->m_pConfig, 
                                                   CFGI_PHONE_PASSWORD,
                                                   &wPWD,
                                                   sizeof(uint16));
                            
                            if (wPWD == EncodePWDToUint16(pMe->m_strPhonePWD))
                            {// 密码符合
                                CLOSE_DIALOG(DLGRET_OK)
                            }
                            else
                            {// 密码错误
                                CLOSE_DIALOG(DLGRET_INVALID)
                            }
                            return TRUE;
                        }
                        
                    default:
                        return TRUE;
                }
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                if (chEnter == 0)
                {// 删除字符
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < OEMNV_LOCKCODE_MAXLEN)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SETTINGMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;
            
        default:
            break;
    }
    
    return FALSE;
} 

/*==============================================================================
函数：
       Setting_Handle_Msgbox
说明：


参数：
       pMe [in]：指向ClockApps Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/
static boolean Setting_Handle_Msgbox(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    static IStatic * pStatic = NULL;
    if (NULL == pMe)
    {
        return FALSE;
    }

   if (NULL == pStatic)
   {
        AEERect rect = {0};
        if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                                 AEECLSID_STATIC,
                                                 (void **)&pStatic))
        
        {
            return FALSE;
            ERR("ISHELL_CreateInstance,AEECLSID_STATIC 2",0,0,0);
        }        
        ISTATIC_SetRect(pStatic, &rect);  
   }
   
    switch(eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
            (void) ISHELL_PostEventEx(pMe->m_pShell, 
                                    EVTFLG_ASYNC,
                                    AEECLSID_APP_SETTINGMENU,
                                    EVT_USER_REDRAW,
                                    0, 
                                    0);

            return TRUE;

        case EVT_USER_REDRAW:
            {
                PromptMsg_Param_type  Msg_Param={0};
                AECHAR  wstrText[MSGBOX_MAXTEXTLEN];
                
                // 从资源文件取消息内容
                (void)ISHELL_LoadResString(pMe->m_pShell,
                                AEE_APPSSETTINGMENU_RES_FILE,                                
                                IDS_INVALID,
                                wstrText,
                                sizeof(wstrText));
                                
                Msg_Param.ePMsgType = MESSAGE_INFORMATIVE;
                Msg_Param.pwszMsg = wstrText;
                Msg_Param.eBBarType = BTBAR_RETRY_CANCEL;
                DrawPromptMessage(pMe->m_pDisplay, pStatic, &Msg_Param);
            }
            // 更新界面
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);           

            
            return TRUE;

        case EVT_DIALOG_END:

            ISTATIC_Release(pStatic);
            pStatic = NULL;
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT:
                    CLOSE_DIALOG(DLGRET_MSGBOX_OK)
                    break;
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_MSGBOX_CANCEL)
                    break;
                default:
                    break;
            }
            return TRUE;

        default:
            break;
    }

    return FALSE;
}

#ifdef FEATURE_PLANEMODE
/*==============================================================================
函数：
       HandlePlaneModeDialogEvent
说明：
       IDD_PLANEMODE 对话框事件处理函数

参数：
       pMe [in]：指向SettingMenu Applet对象结构的指针。该结构包含小程序的特定信息。
       eCode [in]：事件代码。
       wParam：事件相关数据。
       dwParam：事件相关数据。

返回值：
       TRUE：传入事件被处理。
       FALSE：传入事件被忽略。

备注：

==============================================================================*/

static boolean  HandlePlaneModeDialogEvent(CSettingMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)
    static byte planeMode_cfg = 0;
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_PLANEMODE);
    SETTING_ERR("%x, %x ,%x,HandlePlaneModeDialogEvent",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }
     //实现菜单循环滚动功能
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_ON, IDS_ON, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_OFF, IDS_OFF, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSETTINGMENU_RES_FILE, IDS_QUERY_PLANEMODE, IDS_QUERY_PLANEMODE, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16 wItemID;

                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
                IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SETTING_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_PLANEMODE,
                                       &planeMode_cfg,
                                       sizeof(planeMode_cfg));

                switch (planeMode_cfg)
                {
                    case  OEMNV_PLANEMODE_ON:
                        wItemID = IDS_ON;
                        break;

                    case OEMNV_PLANEMODE_QUERY:
                        wItemID = IDS_QUERY_PLANEMODE;
                        break;

                    case OEMNV_PLANEMODE_OFF :
                        wItemID = IDS_OFF;
                        break;

                    default:
                        break;
                }

                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, wItemID, TRUE);
                IMENUCTL_SetSel(pMenu, wItemID);
                (void) ISHELL_PostEvent( pMe->m_pShell,
                                         AEECLSID_APP_SETTINGMENU,
                                         EVT_USER_REDRAW,
                                         0,
                                         0);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                  default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                byte newCfg = 0;
                AEECMPhInfo phInfo;
                ICM  *pCM = NULL;
                int nRet;
                
                nRet = ISHELL_CreateInstance(pMe->m_pShell,
                                             AEECLSID_CALLMANAGER,
                                             (void **) &pCM);
                if ((nRet != SUCCESS) || (pCM == NULL))
                {
                    return FALSE;
                }
                
                ICM_GetPhoneInfo(pCM, &phInfo, sizeof(phInfo));
                
                switch (wParam)
                {
                    case IDS_ON:
                       if(SYS_OPRT_MODE_LPM != phInfo.oprt_mode)
                       {
                           IANNUNCIATOR_SetField(pMe->m_pAnn, ANNUN_FIELD_RSSI, ANNUN_STATE_AIR_MODE_ON );
                           cm_ph_cmd_oprt_mode(NULL, NULL, CM_CLIENT_ID_ANONYMOUS, SYS_OPRT_MODE_LPM);
                       }
                       newCfg = OEMNV_DATEFORM_DMY ;
                       break;

                    case IDS_OFF:
                       if(SYS_OPRT_MODE_ONLINE != phInfo.oprt_mode)
                       {
                           IANNUNCIATOR_SetField(pMe->m_pAnn, ANNUN_FIELD_RSSI, ANNUN_STATE_AIR_MODE_OFF );
                           cm_ph_cmd_oprt_mode(NULL, NULL, CM_CLIENT_ID_ANONYMOUS, SYS_OPRT_MODE_ONLINE);
                       }
                       newCfg = OEMNV_DATEFORM_MDY ;
                       break;

                    case IDS_QUERY_PLANEMODE:
                       //cm_ph_cmd_oprt_mode(NULL, NULL, CM_CLIENT_ID_ANONYMOUS, SYS_OPRT_MODE_LPM);
                       newCfg = OEMNV_DATEFORM_YMD ;
                       break;

                    default:
                       ASSERT_NOT_REACHABLE;

                }

                if (newCfg != planeMode_cfg)
                {
                    (void)ICONFIG_SetItem(pMe->m_pConfig,
                                          CFGI_PLANEMODE,
                                          &newCfg, sizeof(newCfg));
                    //将选中的选项标出
                    planeMode_cfg = newCfg;
                    InitMenuIcons(pMenu);
                    SetMenuIcon(pMenu, wParam, TRUE);
                    (void)IMENUCTL_Redraw(pMenu);
                }
                CLOSE_DIALOG(DLGRET_WARNING)
                ICM_Release(pCM);
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandlePlaneModeDialogEvent
#endif

