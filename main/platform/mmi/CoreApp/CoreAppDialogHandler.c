/*==============================================================================
//           U S E R   I N T E R F A C E   M A I N  M O D U L E
// �ļ�:
//        CoreAppDialogHandler.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵��:
//        ��Ӧ���� UI ����ʼ�㣬�ڿ���ʱ�� BREW �Զ�������         
//
// ����:2007-09-18
// ��ǰ�汾:Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼:
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------

==============================================================================*/



/*==============================================================================
                             
                             ���ļ��������ⲿ�ļ�
                             
==============================================================================*/
#include "OEMFeatures.h"
#include "CoreApp_priv.h"
#include "AEEMenu.h"
#ifndef WIN32
#include "uim.h"
#include "mobile.h"             // for ver_modelname
#include "mccdma.h"  //for use of cdma_bs_type
#endif
#include "Appscommon.h"
#include "OEMRTC.h"
//#include "AEECallList.h"
#include "clockapps_images.brh"
#include "appscommonimages.brh"
#include "AEEGraphics.h "

#include "clockapps.brh"
#define  CLOCK_RES_PATH ("fs:/mod/clockapps/" AEE_RES_LANGDIR CLOCKAPPS_RES_FILE)
#if defined( FEATURE_POWERDOWN_ALARM)
extern void registerPowerdownAlarmclock( void);
#endif
extern void OEMPriv_DrawMessageCB(void *pUnused);
#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "g_sensor.h"
#include "DisplayMenu.h"
#endif

#ifdef USES_MMI
extern boolean   IsRunAsUIMVersion(void);
extern boolean   IsRunAsFactoryTestMode(void);
#endif
/*==============================================================================

                               �궨��ͳ���
                                 
==============================================================================*/
#define CoreDrawBottomBar(x)                        \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}

// ���ػ���������ʱ��
#define PWRON_ANI_TIME    ((PWRON_ANI_RATE)*(PWRON_ANI_FRAME_COUNT))
#define PWROFF_ANI_TIME  ((PWROFF_ANI_RATE)*(PWROFF_ANI_FRAME_COUNT))


#if defined(FEATURE_DISP_128X128)
#define IDLE_D_CLOCK_X 		15
#define IDLE_D_CLOCK_Y 		25

#define RPLMN_X				IDLE_D_CLOCK_X
#define RPLMN_Y				(IDLE_D_CLOCK_Y+25)

#define DATA_X				IDLE_D_CLOCK_X
#define DATA_Y				(RPLMN_Y + 30) 

#elif defined(FEATURE_DISP_160X128)

#define IDLE_D_CLOCK_X 		5
#define IDLE_D_CLOCK_Y 		25

#define RPLMN_X				5
#define RPLMN_Y				20

#define DATA_X				5
#define DATA_Y				36

#define WEEK_X                        5
#define WEEK_Y                        52

#elif defined(FEATURE_DISP_176X220)

#define IDLE_D_CLOCK_X 		15
#define IDLE_D_CLOCK_Y 		25

#define RPLMN_X				IDLE_D_CLOCK_X
#define RPLMN_Y				(IDLE_D_CLOCK_Y+25)

#define DATA_X				IDLE_D_CLOCK_X
#define DATA_Y				(RPLMN_Y + 30) 

#else

#define IDLE_D_CLOCK_X 		15
#define IDLE_D_CLOCK_Y 		25

#define RPLMN_X				IDLE_D_CLOCK_X
#define RPLMN_Y				(IDLE_D_CLOCK_Y+25)

#define DATA_X				IDLE_D_CLOCK_X
#define DATA_Y				(RPLMN_Y + 30) 

#endif

/*==============================================================================

                                 ���Ͷ���

==============================================================================*/
//typedef struct {
//   char *num_buf;
//   byte  num_len;
//} EmergencyNumber;
static IImage * pWallPaper = NULL;
//int charging_mark = 0;
//int charging_mark2 = 0;

static boolean bHideText = FALSE;
static boolean bImageDecoded = FALSE;
/*==============================================================================

                                 ��������
                                 
==============================================================================*/
// �Ի��� IDD_ALARM �¼�������
#if defined( FEATURE_POWERDOWN_ALARM)
static boolean  IDD_ALARM_Handler(void *pUser,
                                AEEEvent   eCode,
                                uint16     wParam,
                                uint32     dwParam);
#endif//#if FEATURE_POWERDOWN_ALARM
// �Ի��� IDD_LPM �¼�������
static boolean  IDD_LPM_Handler(void *pUser,
                                AEEEvent   eCode,
                                uint16     wParam,
                                uint32     dwParam);
                                
// �Ի��� IDD_MSGBOX �¼�������
static boolean  IDD_MSGBOX_Handler(void *pUser,
                                   AEEEvent   eCode,
                                   uint16     wParam,
                                   uint32     dwParam);
                                
// �Ի��� IDD_PWDINPUT �¼�������
static boolean  IDD_PWDINPUT_Handler(void *pUser,
                                     AEEEvent   eCode,
                                     uint16     wParam,
                                     uint32     dwParam);
                                     
// �Ի��� IDD_UIMERR �¼�������
static boolean  IDD_UIMERR_Handler(void *pUser,
                                   AEEEvent   eCode,
                                   uint16     wParam,
                                   uint32     dwParam);

// �Ի��� IDD_UIMSECCODE �¼�������
static boolean  IDD_UIMSECCODE_Handler(void *pUser,
                                       AEEEvent   eCode,
                                       uint16     wParam,
                                       uint32     dwParam);
                                       
// �Ի��� IDD_EMERGENCYNUMLIST �¼�������
static boolean  IDD_EMERGENCYNUMLIST_Handler(void *pUser,
                                 AEEEvent   eCode,
                                 uint16     wParam,
                                 uint32     dwParam);
                                 
// �Ի��� IDD_STARTUPANI �¼�������
static boolean  IDD_STARTUPANI_Handler(void *pUser,
                                       AEEEvent   eCode,
                                       uint16     wParam,
                                       uint32     dwParam);
                                       
// �Ի��� IDD_LOADING �¼�������
static boolean  IDD_LOADING_Handler(void *pUser,
                                 AEEEvent   eCode,
                                 uint16     wParam,
                                 uint32     dwParam);
                                       
// �Ի��� IDD_IDLE �¼�������
static boolean  IDD_IDLE_Handler(void *pUser,
                                 AEEEvent   eCode,
                                 uint16     wParam,
                                 uint32     dwParam);
                                 
#if defined(FEATURE_WMS_APP)
// �Ի��� IDD_WMSTIPS �¼�������
static boolean  IDD_WMSTIPS_Handler(void *pUser,
                                 AEEEvent   eCode,
                                 uint16     wParam,
                                 uint32     dwParam);
#endif

// �Ի��� IDD_POWERDOWN �¼�������
static boolean  IDD_POWERDOWN_Handler(void  *pMe,
                                      AEEEvent    eCode,
                                      uint16      wParam,
                                      uint32      dwParam);

#ifdef FEATURE_UTK2
static boolean  IDD_UTKREFRESH_Handler(void *pUser,
                                   AEEEvent   eCode,
                                   uint16     wParam,
                                   uint32     dwParam);
#endif //FEATURE_UTK2
// �Ի��� IDD_PWDIMSIMCC �¼�������
#ifdef FATRUE_LOCK_IMSI_MCCMNC
static boolean  IDD_PWDIMSIMCC_Handler(void       *pUser,
                                     AEEEvent   eCode,
                                     uint16     wParam,
                                     uint32     dwParam);
#endif
                                       
// ���´�������Ķ�ʱ�����������������ȶ���ÿ����ִ��һ��
static void CoreApp_UpdateIdleTimer(void *pUser);                                       

static void CoreApp_DrawBannerMessage(CCoreApp    *pMe);

#ifdef FEATURE_APP_MUSICPLAYER
static void CoreApp_DrawMusicName(CCoreApp    *pMe,uint16 nIdx);
static void CoreApp_MusicNameAutoScroll(CCoreApp *pMe);
static boolean Core_bMusicNameRequiredScroll(CCoreApp *pMe, int nIdx);
static void Core_DrawNameResetScroll(CCoreApp *pMe);
#endif

static void CoreApp_SearchingTimer(void *pUser);

static void CoreApp_UpdateDateTime(CCoreApp    *pMe);

// �����������ź���
static void CoreApp_PlayPwrOnAni(CCoreApp *pMe);

//�ػ��������ź���
static void CoreApp_PlayPwrOffAni(CCoreApp *pMe);

//����ǽֽ
static void CoreApp_DrawWallPaper(CCoreApp *pMe);

static boolean CoreApp_LaunchApplet(CCoreApp *pMe,  AEECLSID   classID);

//static void CoreApp_UpdateAnnunciator(CCoreApp *pMe);
static void CoreApp_Process_Rtc_Event(CCoreApp *pMe);

#ifdef FEATURE_KEYGUARD
static void CoreApp_TimeKeyguard(void *pUser);
#endif

static void CoreApp_UpdateBottomBar(CCoreApp    *pMe); 

//static void CoreApp_GetRecordCount(CCoreApp *pMe);

//static boolean CoreApp_PowerAlarm_Event(CCoreApp *pMe);
//static void CoreApp_DrawStartAlarm(CCoreApp *pMe);
//boolean CoreApp_Start_Alarm(CCoreApp *pMe);
static void CoreApp_GetSPN(CCoreApp *pMe);

static void CoreApp_ImageNotify(void *po, IImage *pIImage, AEEImageInfo *pii, int nErr);
/*==============================================================================

                                 ��������
                                 
==============================================================================*/

/*==============================================================================
����:
    CoreApp_ShowDialog
       
˵��:
    ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
    dlgResId [in]:�Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
    ��

��ע:

==============================================================================*/
void CoreApp_ShowDialog(CCoreApp *pMe,uint16  dlgResId)
{
    int nRet;
    MSG_FATAL("CoreApp_ShowDialog Start",0,0,0);
    // һ��ֻ�����һ���Ի���
    if (ISHELL_GetActiveDialog(pMe->a.m_pIShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        MSG_FATAL("Trying to create a dialog without closing the previous one",0,0,0);
        return;
    }

    nRet = ISHELL_CreateDialog(pMe->a.m_pIShell,
                               AEE_COREAPPRES_LANGFILE,
                               dlgResId,NULL);

    if (nRet != SUCCESS)
    {
        MSG_FATAL("Failed to create the dialog in the Core applet",0,0,0);
    }
    MSG_FATAL("CoreApp_ShowDialog End",0,0,0);
}

/*==============================================================================
����:
    CoreApp_ShowMsgDialog
       
˵��:
    ����������ʾָ����Ϣ�ĶԻ���

����:
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
    nResId [in]:��Ϣ�ַ�������Դ�ļ��е� ID �š�

����ֵ:
    none

��ע:

==============================================================================*/
void CoreApp_ShowMsgDialog(CCoreApp *pMe,uint16  nResId)
{
    MSG_FATAL("CoreApp_ShowMsgDialog Start",0,0,0);
    pMe->m_nMsgID = nResId;

    (void)CoreApp_ShowDialog(pMe,IDD_MSGBOX);
    MSG_FATAL("CoreApp_ShowMsgDialog End",0,0,0);
}

/*==============================================================================
����:
    CoreApp_SetDialogHandler
       
˵��:
    �������� pMe->m_wActiveDlgID �趨�Ի��¼�������ָ�� m_pDialogHandler ��Ӧֵ��

����:
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
    none

��ע:
    ���Ҳ�����Ӧ�ĶԻ�������, m_pDialogHandler ����Ϊ NULL
==============================================================================*/
void CoreApp_SetDialogHandler(CCoreApp *pMe)
{
    MSG_FATAL("CoreApp_SetDialogHandler Start",0,0,0);
    switch (pMe->m_wActiveDlgID)
    {
#if defined( FEATURE_POWERDOWN_ALARM)
        case IDD_ALARM:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_ALARM",0,0,0);
            pMe->m_pDialogHandler = IDD_ALARM_Handler;
            break;
#endif //#if FEATURE_POWERDOWN_ALARM
        case IDD_LPM:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_LPM",0,0,0);
            pMe->m_pDialogHandler = IDD_LPM_Handler;
            break;
            
        case IDD_PWDINPUT:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_PWDINPUT",0,0,0);
            pMe->m_pDialogHandler = IDD_PWDINPUT_Handler;
            break;
        #ifdef FATRUE_LOCK_IMSI_MCCMNC
	  case IDD_PWDIMSIMCC:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_PWDIMSIMCC",0,0,0);
            pMe->m_pDialogHandler = IDD_PWDIMSIMCC_Handler;
            break;
        #endif     	
        case IDD_UIMSECCODE:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_UIMSECCODE",0,0,0);
            pMe->m_pDialogHandler = IDD_UIMSECCODE_Handler;
            break;
            
        case IDD_UIMERR:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_UIMERR",0,0,0);
            pMe->m_pDialogHandler = IDD_UIMERR_Handler;
            break;
            
        case IDD_MSGBOX:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_MSGBOX",0,0,0);
            pMe->m_pDialogHandler = IDD_MSGBOX_Handler;
            break;
            
        case IDD_EMERGENCYNUMLIST:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_EMERGENCYNUMLIST",0,0,0);
            pMe->m_pDialogHandler = IDD_EMERGENCYNUMLIST_Handler;
            break;
            
        case IDD_STARTUPANI:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_STARTUPANI",0,0,0);
            pMe->m_pDialogHandler = IDD_STARTUPANI_Handler;
            break;
            
        case IDD_LOADING:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_LOADING",0,0,0);
            pMe->m_pDialogHandler = IDD_LOADING_Handler;
            break;
            
        case IDD_IDLE:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_IDLE",0,0,0);
            pMe->m_pDialogHandler = IDD_IDLE_Handler;
            break;

#if defined(FEATURE_WMS_APP)
        case IDD_WMSTIPS:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_WMSTIPS",0,0,0);
            pMe->m_pDialogHandler = IDD_WMSTIPS_Handler;
            break;
#endif            
        
        case IDD_POWERDOWN:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_POWERDOWN",0,0,0);
            pMe->m_pDialogHandler = IDD_POWERDOWN_Handler;
            break;
            
#ifdef FEATURE_UTK2
        case IDD_UTKREFRESH:
            MSG_FATAL("CoreApp_SetDialogHandler IDD_UTKREFRESH",0,0,0);
            pMe->m_pDialogHandler = IDD_UTKREFRESH_Handler;
            break;            
#endif //FEATURE_UTK2

        default:
            MSG_FATAL("CoreApp_SetDialogHandler default",0,0,0);
            pMe->m_pDialogHandler = NULL;
            break;
    }
    MSG_FATAL("CoreApp_SetDialogHandler End",0,0,0);
}

/*==============================================================================
����:
    CoreApp_RouteDialogEvent
       
˵��:
    ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
    eCode [in]:�¼����롣
    wParam [in]:���¼�eCode���������ݡ�
    dwParam [in]:���¼�eCode���������ݡ�

����ֵ:
    TRUE:�����¼�������
    FALSE:�����¼�û������

��ע:

==============================================================================*/
boolean CoreApp_RouteDialogEvent(CCoreApp *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam)
{
    if (NULL == pMe->m_pDialogHandler)
    {
        return FALSE;
    }
    
    return pMe->m_pDialogHandler((void *)pMe, eCode, wParam, dwParam);
}

/*==============================================================================
����:
    DialogTimeoutCallback
    
˵��:
    ��ʱ�ص�����
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣

       
����ֵ:
    none

       
��ע:
    ���������� Core Applet ���͵�ǰ�Ի�����ʾ��ʱ���¼���
       
==============================================================================*/
static void DialogTimeoutCallback(void *pUser)
{
    CCoreApp *pMe = (CCoreApp*)pUser;

    if (NULL == pMe)
    {
        return;
    }
    
    if (ISHELL_ActiveApplet(pMe->a.m_pIShell) != AEECLSID_CORE_APP)
    {
        return;
    }

    // Post a message to the dialog telling it to close
    (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                            AEECLSID_CORE_APP,
                            EVT_DISPLAYDIALOGTIMEOUT,
                            0,
                            0);
}


/*==============================================================================
����:
    IDD_MSGBOX_Handler
    
˵��:
    IDD_MSGBOX �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
       
��ע:
    ������ʾ��ʾ��Ϣ���û������Զ��رշ��ء�
       
==============================================================================*/
static boolean  IDD_MSGBOX_Handler(void       *pUser,
                                   AEEEvent   eCode,
                                   uint16     wParam,
                                   uint32     dwParam)
{
    CCoreApp *pMe = (CCoreApp *)pUser;
    static IStatic * pStatic = NULL;
    MSG_FATAL("%x %x %x IDD_MSGBOX_Handler",eCode,wParam,dwParam);
    if (NULL == pMe)
    {
        return FALSE;
    }

    switch(eCode)
    {
        case EVT_DIALOG_INIT:
        {
            if (NULL == pStatic)
            {
                AEERect rect = {0};
                if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->a.m_pIShell,AEECLSID_STATIC,(void **)&pStatic))
                {
                    return FALSE;
                }
                ISTATIC_SetRect(pStatic, &rect);
            }
            return TRUE;
        }

        case EVT_DIALOG_START:
        {
            uint32 set_time= 1500;
            switch (pMe->m_nMsgID)
            {
                case IDS_SERVICEREQ:
                case IDS_EXIT_EMERGENCY_MODE:
                    set_time = 0;
                    break;

#ifdef FEATRUE_AUTO_POWER
                case IDS_AUTO_POWER_OFF:
                    //in this state,we need to disable the key guard. 
#ifdef FEATURE_KEYGUARD
                    if(OEMKeyguard_IsEnabled())
                    {
                        OEMKeyguard_SetState(FALSE);
                    }
#endif
                    IBACKLIGHT_Enable(pMe->m_pBacklight);
                    set_time = 20000;
                    break;
#endif /* FEATRUE_AUTO_POWER */

#ifdef FEATURE_PLANEMODE
                case IDS_PLANEMODE_QUERY:
                    set_time = 5000;
                    break;
#endif
                case 0xFFFF:
                    set_time = 3000;
                    break;
                    
                default:
                    break;
            }

            if (set_time != 0)
            {
                // �������͹رնԻ����¼��Ķ�ʱ��
                ISHELL_SetTimer(pMe->a.m_pIShell, set_time,DialogTimeoutCallback,pMe);
            }
            ISHELL_PostEvent(pMe->a.m_pIShell, AEECLSID_CORE_APP,EVT_USER_REDRAW,  0, 0);
            return TRUE;
        }

        case EVT_USER_REDRAW:
        {
            if(pMe->m_nMsgID !=IDS_EXIT_EMERGENCY_MODE)
            {
                PromptMsg_Param_type m_PromptMsg;
                MEMSET(&m_PromptMsg,0,sizeof(PromptMsg_Param_type));
                m_PromptMsg.nMsgResID= pMe->m_nMsgID;
                if(pMe->m_nMsgID == IDS_AUTO_POWER_OFF)
                {
                    m_PromptMsg.ePMsgType = MESSAGE_CONFIRM;
                }
                else if(pMe->m_nMsgID == 0xFFFF)
                {
                    m_PromptMsg.nMsgResID = 0;
                    m_PromptMsg.ePMsgType = MESSAGE_WARNNING;
                    m_PromptMsg.pwszMsg = pMe->m_cdg_msgptr;
                }
                else
                {
                    m_PromptMsg.ePMsgType = MESSAGE_WARNNING;
                }

                if(pMe->m_nMsgID != 0xFFFF)
                {
                    STRLCPY(m_PromptMsg.strMsgResFile, AEE_COREAPPRES_LANGFILE,MAX_FILE_NAME);
                    if((pMe->m_nMsgID == IDS_FULLY_CHARGED) || (pMe->m_nMsgID == IDS_CHARGER_ON))
                    {
                        m_PromptMsg.eBBarType = BTBAR_NONE;
                    }
                    else
                    {
                        m_PromptMsg.eBBarType = BTBAR_OK_BACK;
                    }
                }
                DrawPromptMessage(pMe->m_pDisplay,pStatic,&m_PromptMsg);
            }
            else
            {
                AECHAR  *wstrText = NULL;
                int32  nSize=0;
                nSize = 128 * sizeof(AECHAR);
                wstrText = (AECHAR *) MALLOC(nSize);
                if (NULL != wstrText)
                {
                    ISHELL_LoadResString(pMe->a.m_pIShell, AEE_COREAPPRES_LANGFILE,  pMe->m_nMsgID,wstrText,nSize);
                    ISTATIC_SetProperties(pStatic, ST_CENTERTEXT | ST_MIDDLETEXT);
                    ISTATIC_SetText(pStatic,NULL,wstrText,AEE_FONT_NORMAL,AEE_FONT_NORMAL);
                    FREE(wstrText);
                }                
                ISTATIC_Redraw(pStatic);
                COREAPP_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
            }
            IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
            return TRUE;            
        }

        case EVT_DIALOG_END:
        {
            ISHELL_CancelTimer(pMe->a.m_pIShell, DialogTimeoutCallback,pMe);
            ISTATIC_Release(pStatic);
            pStatic = NULL;
#ifdef FEATURE_PLANEMODE
            if(IDS_PLANEMODE_QUERY == pMe->m_nMsgID)
            {
                FREEIF(pMe->m_cdg_msgptr);
                return TRUE;
            }
#endif
            // �˶Ի��򷵻�ֵ��Ϊ DLGRET_MSGOK ��Ϊ��ֹ���� Applet 
            // �رնԻ���ص�����״̬����ʾ���Ի��򷵻�ֵ��ֵ
#ifdef FEATRUE_AUTO_POWER
            pMe->m_b_autopower_off = FALSE;
            /*if(pMe->m_nMsgID == IDS_AUTO_POWER_OFF)
            {
                pMe->m_eDlgRet = DLGRET_CREATE;
            }
            else
            {
                pMe->m_eDlgRet = DLGRET_MSGOK;
            }*/
            pMe->m_eDlgRet = DLGRET_MSGOK;

            if(pMe->m_b_needclose_core)
            {
                ISHELL_CloseApplet(pMe->a.m_pIShell, FALSE);
            }
#else
            pMe->m_eDlgRet = DLGRET_MSGOK;
#endif //#ifdef FEATRUE_AUTO_POWER
            FREEIF(pMe->m_cdg_msgptr);
            return TRUE;
        }

        case EVT_KEY:
        {
            switch (wParam)
            {
                case AVK_SELECT:
                {
                    if(pMe->m_nMsgID ==IDS_EXIT_EMERGENCY_MODE)
                    {
                        //exit the emergency mode
                        ICM_SetSystemPreference(pMe->m_pCM,
                                                        AEECM_MODE_PREF_PERSISTENT, AEECM_PREF_TERM_PERMANENT, 0,
                                                        AEECM_GW_ACQ_ORDER_PREF_NO_CHANGE, AEECM_BAND_PREF_NO_CHANGE,
                                                        AEECM_ROAM_PREF_NO_CHANGE, AEECM_HYBR_PREF_NO_CHANGE,
                                                        AEECM_SRV_DOMAIN_PREF_NO_CHANGE, AEECM_NETWORK_SEL_MODE_PREF_NO_CHANGE,
                                                        NULL, NULL, NULL);
                        pMe->m_bemergencymode = FALSE;
                    }
#ifdef FEATRUE_AUTO_POWER
                    else if(pMe->m_nMsgID == IDS_AUTO_POWER_OFF)
                    {
                        ISHELL_CancelTimer(pMe->a.m_pIShell,DialogTimeoutCallback,pMe);                        
                        pMe->m_ePowerDownType = POWERDOWN_NORMAL;
                        if (pMe->m_eCurState != COREST_POWEROFF)
                        {
                            //ISHELL_CloseApplet(pMe->a.m_pIShell, TRUE);
                            pMe->m_b_needclose_core = FALSE;
                            pMe->m_wStartupAniTime = 0;
                            MOVE_TO_STATE(COREST_POWEROFF)
                            CLOSE_DIALOG(DLGRET_CREATE)
                            return TRUE;
                        }
                    }
#endif//#ifdef FEATRUE_AUTO_POWER
#ifdef FEATURE_PLANEMODE
                    else if(pMe->m_nMsgID == IDS_PLANEMODE_QUERY)
                    {
                        ISHELL_CancelTimer(pMe->a.m_pIShell,DialogTimeoutCallback,pMe);
                        pMe->m_SYS_MODE_NO_SRV = TRUE;  // ���︶ֵ������idle����ƭ���û���planemode����������ʾ����
                        CLOSE_DIALOG(DLGRET_YES)
                        return TRUE;
                    }
#endif

                }
                case AVK_CLR:    
                    (void) ISHELL_CancelTimer(pMe->a.m_pIShell,DialogTimeoutCallback,pMe);
#ifdef FEATURE_PLANEMODE
                    if(pMe->m_nMsgID == IDS_PLANEMODE_QUERY)
                    {
                        CLOSE_DIALOG(DLGRET_NO);
                    }
                    else
#endif
                    {
                        CLOSE_DIALOG(DLGRET_MSGOK);
                    }
                    break;

                default:
                    break;
            }
            return TRUE;
        }

        case EVT_DISPLAYDIALOGTIMEOUT:
        {
#ifdef FEATRUE_AUTO_POWER
            if(pMe->m_nMsgID == IDS_AUTO_POWER_OFF)
            {
                pMe->m_ePowerDownType = POWERDOWN_NORMAL;
                if (pMe->m_eCurState != COREST_POWEROFF)
                {
                    //ISHELL_CloseApplet(pMe->a.m_pIShell, TRUE);
                    pMe->m_b_needclose_core = FALSE;
                    pMe->m_wStartupAniTime = 0;
                    MOVE_TO_STATE(COREST_POWEROFF)
                    CLOSE_DIALOG(DLGRET_CREATE)
                    return TRUE;
                }
            }
#endif//#ifdef FEATRUE_AUTO_POWER
#ifdef FEATURE_PLANEMODE
            if(pMe->m_nMsgID == IDS_PLANEMODE_QUERY)
            {
                CLOSE_DIALOG(DLGRET_NO);
                return TRUE;
            }
#endif
            CLOSE_DIALOG(DLGRET_MSGOK);
            return TRUE;
        }

        default:
            break;
    }
    return FALSE;
}
/*==============================================================================
����:
    IDD_ALARM_Handler
    
˵��:
    IDD_ALARM �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
==============================================================================*/
#if defined( FEATURE_POWERDOWN_ALARM)
static void powerdown( CCoreApp* pMe)
{
    pMe->m_ePowerDownType = POWERDOWN_NORMAL;
    if (pMe->m_eCurState != COREST_POWEROFF)
    {
        pMe->m_wStartupAniTime = 0;
        MOVE_TO_STATE(COREST_POWEROFF)
        CLOSE_DIALOG(DLGRET_CREATE)
    }
}
static void defaultProcessAlarm( void* pUser)
{
#ifndef WIN32
    extern void snoozePowerdownAlarmclock( void);
    snoozePowerdownAlarmclock();
#endif
    powerdown( (CCoreApp *)pUser);
}

static boolean  IDD_ALARM_Handler(void       *pUser,
                                AEEEvent   eCode,
                                uint16     wParam,
                                uint32     dwParam)
{
    //static IImage * pIImage = NULL;
    static byte keyBeepVolumeSetting = 0;
    static PowerDown_Alarm_Cfg time = {0};

    CCoreApp *pMe = (CCoreApp *)pUser;
    MSG_FATAL("%x %x %x IDD_ALARM_Handler",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            OEM_GetConfig( CFGI_POWERDOWN_ALARM, (void *)&time, sizeof(time));

            ISHELL_SetTimer( pMe->a.m_pIShell, 60000, defaultProcessAlarm, pMe);
            return TRUE;

        case EVT_DIALOG_START:
        {
            byte mute = OEMSOUND_MUTE_VOL;
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
            }
            ISHELL_PostEvent(pMe->a.m_pIShell,AEECLSID_CORE_APP,EVT_USER_REDRAW,0,0);
            ICONFIG_GetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &keyBeepVolumeSetting, sizeof(byte));
            ICONFIG_SetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &mute, sizeof(byte));
            return TRUE;
        }

        case EVT_USER_REDRAW:
        {
            uint32  dwAlarmTime;
            AECHAR  wszTime[16];
            IImage* pResImg = NULL;
            uint16  ring_id = 1;
            byte    profilenum;
            ringID  ringid[PROFILENUMBER];
            AEERect rc = {0};
            RGBVAL nOldFontColor;
            
            MEMSET(wszTime,0,sizeof(wszTime));
            dwAlarmTime = time.dwWATime;
            Appscommon_FormatTimeString(dwAlarmTime, wszTime, sizeof(wszTime));
			pResImg = ISHELL_LoadResImage( pMe->a.m_pIShell, "fs:/mod/clockapps/clockapps_images.bar", IDI_ALARMCLOCK);
            MSG_FATAL("ALARM %x %S",pResImg,wszTime,0);
            Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
            if( pResImg != NULL)
            {
                AEEImageInfo ImageInfo;
                
                IIMAGE_GetInfo(pResImg, &ImageInfo);
                IIMAGE_Draw(pResImg, (pMe->m_rc.dx - ImageInfo.cx)/2, 2*TITLEBAR_HEIGHT + (pMe->m_rc.dy - BOTTOMBAR_HEIGHT - TITLEBAR_HEIGHT- ImageInfo.cy)/2);
                IIMAGE_Release( pResImg);
                pResImg = NULL;
            }

            SETAEERECT(&rc, 0, TITLEBAR_HEIGHT, pMe->m_rc.dx, TITLEBAR_HEIGHT);
            nOldFontColor = IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
            (void)IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_BOLD, wszTime, -1, 0, 0, &rc,
                                  IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_TEXT_TRANSPARENT);
            (void)IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
#if 1
            OEM_GetConfig( CFGI_ALARM_RINGER, &ring_id, sizeof(ring_id));

            ICONFIG_GetItem(pMe->m_pConfig, CFGI_PROFILE_CUR_NUMBER, &profilenum, sizeof(profilenum));
            ICONFIG_GetItem(pMe->m_pConfig, CFGI_PROFILE_ALARM_RINGER, (void*)ringid, sizeof(ringid));
            if(ringid[profilenum].ringType == OEMNV_MID_RINGER)
            {
                IALERT_StartRingerAlert(pMe->m_pAlert, ring_id);
            }
            else
            {
                if ((IALERT_StartMp3Alert(pMe->m_pAlert, ringid[profilenum].szMusicname, ALERT_NORMAL_SND) != SUCCESS))
                {
                    IALERT_StartRingerAlert(pMe->m_pAlert, (uint32)ring_id);
                }
            }
#else
            ICONFIG_GetItem( pMe->m_pConfig,CFGI_ALARM_RINGER,&ring_id,sizeof(ring_id));
            IALERT_StartRingerAlert(pMe->m_pAlert, ring_id);
#endif
            {
                TitleBar_Param_type title = {0};
                AECHAR wszTitle[16] = {0};
                int len = 0;

                ISHELL_LoadResString( pMe->a.m_pIShell,
                                      CLOCK_RES_PATH,
                                      IDS_ALARMCLOCK_TITLE,
                                      wszTitle,
                                      sizeof(wszTitle)
                                  );
                len = WSTRLEN( wszTitle);
                WSPRINTF( wszTitle + len, sizeof( wszTitle) - len*2, L"%d", time.alarm_id + 1);
                MEMSET(&title,0,sizeof(TitleBar_Param_type));
                //title.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER;
                title.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                title.pwszTitle = wszTitle;
				#if 0
                DrawTitleBar(pMe->a.m_pIDisplay,&title);
				#else
                if(pMe->m_pIAnn != NULL)
                {
				    IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,wszTitle);
                }
				#endif
                CoreDrawBottomBar( BTBAR_SNOOZE_CONTINUE_STOP);
            }
            IDISPLAY_Update( pMe->a.m_pIDisplay);
            return TRUE;
        }

        case EVT_DIALOG_END:
            IBACKLIGHT_Enable(pMe->m_pBacklight);
#ifdef FEATURE_LED_CONTROL
            IBACKLIGHT_SigLedDisable( pMe->m_pBacklight);
#endif
#if !defined( AEE_SIMULATOR)
            IALERT_StopRingerAlert(pMe->m_pAlert);
            IALERT_StopMp3Alert(pMe->m_pAlert);
#endif
            ICONFIG_SetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &keyBeepVolumeSetting, sizeof(byte));
			ISHELL_CancelTimer( pMe->a.m_pIShell, defaultProcessAlarm, pMe);
            return TRUE;

        case EVT_KEY:
        {
            if(wParam == AVK_INFO)
            {
#if !defined( AEE_SIMULATOR)
                IALERT_StopRingerAlert(pMe->m_pAlert);
#endif

                CLOSE_DIALOG(DLGRET_OK)
            }
            else if( wParam == AVK_SELECT)
            {
                defaultProcessAlarm( pMe);
            }
            else if( wParam == AVK_CLR)
            {
                powerdown( pMe);
            }
        }
        return TRUE;


        case EVT_COMMAND:
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // IDD_ALARM_Handler

#endif

#ifdef FEATURE_USES_LOWMEM
#include "AEERGBVAL.h"
#define BATT_HEIGHT     80
#define BATT_WIDTH      60

static void CoreApp_DrawChargeing(CCoreApp *pMe, int iLevel)
{
    AEERect bgRect;
    AEERect fgRect;
    AEERect hdRect;
    uint16  chHeight;
    byte    R,G;
    
    SETAEERECT(&bgRect, pMe->m_rc.x+((pMe->m_rc.dx-BATT_WIDTH)>>1), pMe->m_rc.y+((pMe->m_rc.dy-BATT_HEIGHT)>>1), BATT_WIDTH, BATT_HEIGHT);
    fgRect  = bgRect;
    hdRect.dx = BATT_WIDTH/5;
    hdRect.dy = BATT_HEIGHT/15;
    hdRect.x  = bgRect.x+(bgRect.dx-hdRect.dx)/2;
    hdRect.y  = bgRect.y-hdRect.dy;
    chHeight = BATT_HEIGHT/CHARGING_FRAME_COUNT;
    fgRect.dy = chHeight*(iLevel+1);
    fgRect.y  = bgRect.y + chHeight*(CHARGING_FRAME_COUNT-(iLevel+1));

    IDISPLAY_DrawRect(pMe->m_pDisplay, &bgRect, RGB_NONE, RGB_WHITE, IDF_RECT_FILL);
    G = 0xFF*iLevel/(CHARGING_FRAME_COUNT-1);
    R = 0xFF - G;
    IDISPLAY_DrawRect(pMe->m_pDisplay, &fgRect, RGB_NONE,  MAKE_RGB(R, G, 0), IDF_RECT_FILL);
    IDISPLAY_DrawRect(pMe->m_pDisplay, &bgRect, RGB_BLACK, RGB_NONE, IDF_RECT_FRAME);
    IDISPLAY_DrawRect(pMe->m_pDisplay, &hdRect, RGB_NONE, RGB_BLACK, IDF_RECT_FILL);
    IDISPLAY_Update(pMe->m_pDisplay);
}
#endif
/*==============================================================================
����:
    IDD_LPM_Handler
    
˵��:
    IDD_LPM �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
       
��ע:
    ���������� PIN �롢 PUK ���������洦��
       
==============================================================================*/
static boolean  IDD_LPM_Handler(void       *pUser,
                                AEEEvent   eCode,
                                uint16     wParam,
                                uint32     dwParam)
{
    CCoreApp *pMe = (CCoreApp *)pUser;
    MSG_FATAL("%x %x %x IDD_LPM_Handler",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            pMe->m_battery_count = 0;
#ifndef FEATURE_USES_LOWMEM
            pMe->m_battery_Image = ISHELL_LoadImage(pMe->a.m_pIShell, CHARGING_ANIFILE "charge.png");
            IIMAGE_SetParm(pMe->m_battery_Image, IPARM_NFRAMES, CHARGING_FRAME_COUNT, 0);
#endif
            return TRUE;
            
        case EVT_DIALOG_START:
            //IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,FALSE);
            ISHELL_PostEvent(pMe->a.m_pIShell,AEECLSID_CORE_APP,EVT_USER_REDRAW,0,0);
            return TRUE;
            
        case EVT_USER_REDRAW:
        case EVT_UPDATEIDLE:
        {
            AEEBatteryChargerStatus status;
            AECHAR  *wszText=NULL;
            int32   nSize;

            nSize = 32 * sizeof(AECHAR);
            wszText =  (AECHAR *) MALLOC(nSize);
            wszText[0] = 0;
            status = IBATTERY_GetChargerStatus(pMe->m_pBatt);
            MSG_FATAL("ChargerStatus %d ",status,0,0);
            IDISPLAY_ClearScreen(pMe->m_pDisplay);

            if (AEEBATTERY_CHARGERSTATUS_FULLY_CHARGE == status)
            {
                ISHELL_CancelTimer(pMe->a.m_pIShell, CoreApp_Draw_Charger_image, pMe);
#ifndef FEATURE_USES_LOWMEM
                if (NULL != pMe->m_battery_Image)
                {
                    IIMAGE_DrawFrame(pMe->m_battery_Image, CHARGING_FRAME_COUNT - 1, 0, 0);
                }
#else
                CoreApp_DrawChargeing(pMe, CHARGING_FRAME_COUNT - 1);
#endif
            } 
            else if (AEEBATTERY_CHARGERSTATUS_OVERVOLTAGE == status) 
            {
                ISHELL_CancelTimer(pMe->a.m_pIShell, CoreApp_Draw_Charger_image, pMe);
                if (NULL != wszText)
                {
                    (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                                AEE_COREAPPRES_LANGFILE,
                                                IDS_OVERVOLTAGE,
                                                wszText,
                                                nSize);
                }
            }
            else // AEEBATTERY_CHARGERSTATUS_CHARGING
            {
                CoreApp_Draw_Charger_image(pMe);
            }

            // ������ʾ�ı�
            if (NULL != wszText && wszText[0] != 0)
            {
                (void)IDISPLAY_DrawText(pMe->m_pDisplay,
                    AEE_FONT_BOLD,
                    wszText, -1,
                    0, 0, &pMe->m_rc, 
                    IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_TOP);
                
                //FREE ((void*) wszText);
            }

            FREEIF ((void*) wszText);
            IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
            return TRUE;
        }
            
            
        case EVT_DIALOG_END:
        {
            //int i = 0;
            ISHELL_CancelTimer(pMe->a.m_pIShell, CoreApp_Draw_Charger_image, pMe);
#ifndef FEATURE_USES_LOWMEM
            if (NULL != pMe->m_battery_Image)
            {
                IIMAGE_Release(pMe->m_battery_Image);
                pMe->m_battery_Image = NULL;
            }
#endif
            return TRUE;
        }
        case EVT_KEY:
            switch (wParam)
            {
                case AVK_END:
                    CLOSE_DIALOG(DLGRET_OK)                  
                    return TRUE;
                    
                default:
                    break;
            }
            
            return TRUE;
            
        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
            return TRUE;
            
        default:
            break;
    }
    
    return FALSE;
} // IDD_LPM_Handler

/*==============================================================================
����:
    IDD_EMERGENCYNUMLIST_Handler
    
˵��:
    IDD_EMERGENCYNUMLIST �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]���¼����롣
    wParam���¼�������ݡ�
    dwParam���¼�������ݡ�
       
����ֵ:
    TRUE: �����¼�������
    FALSE: �����¼������ԡ�
       
��ע:
       
==============================================================================*/
static boolean  IDD_EMERGENCYNUMLIST_Handler(void  *pUser,
                                 AEEEvent   eCode,
                                 uint16     wParam,
                                 uint32     dwParam)
{
    CCoreApp *pMe = (CCoreApp *)pUser;
    IMenuCtl *pMenu = NULL;
    
    MSG_FATAL("%x %x %x IDD_EMERGENCYNUMLIST_Handler",eCode,wParam,dwParam);
    if (NULL == pMe)
    {
        return FALSE;
    }

    pMenu = (IMenuCtl* )IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_EMERGENCYNUM);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:            
            {
                //AEERect rc;
                AECHAR  wstrNum[MAX_EMERGENCY_NUM_LEN+1];
                EmergencyNum_Table emerg_tab;
                //EmergencyNumber pList[3];
                //EmergencyNumber pTepItem ;
                //EmergencyNumber pItem;
                int   i = 0;
                MSG_FATAL("IDD_EMERGENCYNUMLIST_Handler EVT_DIALOG_START)",0,0,0);
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_EMERGENCYNUM_TABLE, (void*)&emerg_tab, sizeof(EmergencyNum_Table));
                
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL);
                IMENUCTL_SetOemProperties( pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
                IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND); //added by chengxiao 2009.03.20
#endif
                for(i=0; i<emerg_tab.emert_size; i++)
                {
                    wstrNum[0] = 0;
                    STRTOWSTR(emerg_tab.emerg_num[i].num_buf, wstrNum, sizeof(wstrNum));
                    IMENUCTL_AddItem(pMenu, NULL, NULL, 500+i, wstrNum, NULL);
                }
                //SETAEERECT(&rc,  0, 0, pMe->m_rc.dx, pMe->m_rc.dy - (BOTTOMBAR_HEIGHT + IDLE_ICON_HEIGHT));
                //IMENUCTL_SetRect(pMenu, &rc);

                //pList = AEEOEM_GetEmergencyNumList(pMe->m_pOEM_TSGBridge);
                
                //pList[0].num_buf = "10010";
                //pList[0].num_len = 5;

                //pList[1].num_buf = "10011";
                //pList[1].num_len = 5;

                //pList[2].num_buf = "10012";
                //pList[2].num_len = 5;

                //for (j=0; j<3; j++)
                //{
                //    pTepItem = pList[j];
                //    pItem = pTepItem;

                 //   if ((pItem.num_len == 0) || (pItem.num_buf == NULL))
                 //   {
                 //       break;
                 //   }
                           
                 //   wstrNum[0] = 0;
                 //   (void)STRTOWSTR(pItem.num_buf, wstrNum, sizeof(wstrNum));
                 //   (void)IMENUCTL_AddItem(pMenu, NULL, NULL, 500+i, wstrNum, NULL);
                 //   i++;                   
                //}                 
            }
            ISHELL_PostEvent(pMe->a.m_pIShell,AEECLSID_CORE_APP,EVT_USER_REDRAW, 0,0);
            return TRUE;

        case EVT_USER_REDRAW:
            // ���Ƶ�����ʾ
            // Select       Back
            //CoreDrawBottomBar(BTBAR_SELECT_BACK)
            MSG_FATAL("IDD_EMERGENCYNUMLIST_Handler EVT_USER_REDRAW)",0,0,0);
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            MSG_FATAL("IDD_EMERGENCYNUMLIST_Handler EVT_DIALOG_END)",0,0,0);
            if (pMe->m_eDlgRet != DLGRET_CANCELED)
            {
                pMe->m_eDlgRet = DLGRET_CANCELED;
            }
            return TRUE;

        case EVT_KEY:
            switch (wParam)
            {
                case AVK_END:
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
  
                case AVK_SEND:
                    {
                        CtlAddItem ai;
                        uint16   wID;
                        
                        wID = IMENUCTL_GetSel(pMenu);
                        if (IMENUCTL_GetItem(pMenu, wID, &ai))
                        {
                           (void)MakeVoiceCall(pMe->a.m_pIShell, FALSE, (AECHAR *)ai.pText);
                        }
                    }
                    return TRUE;
                    
                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                CtlAddItem ai;
                MSG_FATAL("IDD_EMERGENCYNUMLIST_Handler EVT_COMMAND)",0,0,0);
                if (IMENUCTL_GetItem(pMenu, wParam, &ai))
                {
                     (void)MakeVoiceCall(pMe->a.m_pIShell, FALSE, (AECHAR *)ai.pText);
                }
            }
            return TRUE;

        default:
            break;
    }

    return FALSE;
} // IDD_EMERGENCYNUMLIST_Handler

   #ifdef FATRUE_LOCK_IMSI_MCCMNC
/*==============================================================================
����:
    IDD_PWDINPUT_Handler
    
˵��:
    IDD_PWDINPUT �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
       
��ע:
    ���������� PIN �롢 PUK ���������洦��
       
==============================================================================*/
static boolean  IDD_PWDIMSIMCC_Handler(void       *pUser,
                                     AEEEvent   eCode,
                                     uint16     wParam,
                                     uint32     dwParam)
{
    //IMenuCtl *pMenu = NULL;
    CCoreApp *pMe = (CCoreApp *)pUser;
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    MSG_FATAL("%x %x %x IDD_PWDINPUT_Handler",eCode,wParam,dwParam);
    //pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_PHONEPWD_MENU);
    //if (NULL == pMenu)
    //{
    //    return FALSE;
    //}
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            MEMSET(pMe->m_strPhonePWD, 0, sizeof(pMe->m_strPhonePWD));
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                                    AEECLSID_CORE_APP,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
            //if (NULL != pMenu)
            //{
            //     (void)IMENUCTL_SetTitle(pMenu, AEE_COREAPPRES_LANGFILE, IDS_ENTER_PHONELOCK, NULL);
            //}

            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR  wstrDisplay[PHONEPASSWORDLENTH+1] = {0};
                char    strDisplay[PHONEPASSWORDLENTH+1] = {0};
                int xOffset = 5, nLen = 0;
                AECHAR  text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};
                
                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->a.m_pIShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif
                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc, RGB_BLACK);
                
                // ��������
                (void)ISHELL_LoadResString(pMe->a.m_pIShell, 
                                                            AEE_COREAPPRES_LANGFILE,
                                                            IDS_MCC_LOCK, 
                                                            text,
                                                            sizeof(text));
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
                
                (void)ISHELL_LoadResString(pMe->a.m_pIShell, 
                                                AEE_COREAPPRES_LANGFILE,
                                                IDS_ENTER_PASSWORD, 
                                                text,
                                                sizeof(text));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                    AEE_FONT_BOLD, 
                                    text,
                                    -1, 
                                    xOffset, 
                                    TITLEBAR_HEIGHT + MENUITEM_HEIGHT*1/2,
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
                
                // ��������
                nLen = STRLEN(pMe->m_strPhonePWD);
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                AEE_FONT_BOLD, 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
                                TITLEBAR_HEIGHT + MENUITEM_HEIGHT*3/2,
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
                
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    CoreDrawBottomBar(BTBAR_OK_DELETE)
                }
                else if(nLen > 0)
                {// ɾ��
                    CoreDrawBottomBar(BTBAR_DELETE)
                }
                else
                {// SOS
                    CoreDrawBottomBar(BTBAR_SOS)
                }
            }
            // ������ʾ
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);  
            return TRUE;
            
        case EVT_DIALOG_END:
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
                        
                    case AVK_SEND:
                        CLOSE_DIALOG(DLGRET_EMGCALL)
                        return TRUE;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        if (STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_EMGCALL)
                            return TRUE;
                        }
                        break;
                        
                    case AVK_SELECT:
                        if (STRLEN(pMe->m_strPhonePWD) < 4)
                        {
                            return TRUE;
                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_ENTEROK)
                        }
                        return TRUE;
                        
                    default:
                        return TRUE;
                }
                
                nLen = STRLEN(pMe->m_strPhonePWD);
                if (chEnter == 0)
                {// ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < PHONEPASSWORDLENTH)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                                            AEECLSID_CORE_APP,
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
} // IDD_PWDINPUT_Handler

#endif

/*==============================================================================
����:
    IDD_PWDINPUT_Handler
    
˵��:
    IDD_PWDINPUT �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
       
��ע:
    ���������� PIN �롢 PUK ���������洦��
       
==============================================================================*/
static boolean  IDD_PWDINPUT_Handler(void       *pUser,
                                     AEEEvent   eCode,
                                     uint16     wParam,
                                     uint32     dwParam)
{
    //IMenuCtl *pMenu = NULL;
    CCoreApp *pMe = (CCoreApp *)pUser;
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    MSG_FATAL("%x %x %x IDD_PWDINPUT_Handler",eCode,wParam,dwParam);
    //pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_PHONEPWD_MENU);
    //if (NULL == pMenu)
    //{
    //    return FALSE;
    //}
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            MEMSET(pMe->m_strPhonePWD, 0, sizeof(pMe->m_strPhonePWD));
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                                    AEECLSID_CORE_APP,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
            //if (NULL != pMenu)
            //{
            //     (void)IMENUCTL_SetTitle(pMenu, AEE_COREAPPRES_LANGFILE, IDS_ENTER_PHONELOCK, NULL);
            //}

            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR  wstrDisplay[PHONEPASSWORDLENTH+1] = {0};
                char    strDisplay[PHONEPASSWORDLENTH+1] = {0};
                int xOffset = 5, nLen = 0;
                AECHAR  text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};
                
                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->a.m_pIShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif
                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc, RGB_BLACK);
                
                // ��������
                (void)ISHELL_LoadResString(pMe->a.m_pIShell, 
                                                            AEE_COREAPPRES_LANGFILE,
                                                            IDS_ENTER_PHONELOCK, 
                                                            text,
                                                            sizeof(text));
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
                
                (void)ISHELL_LoadResString(pMe->a.m_pIShell, 
                                                AEE_COREAPPRES_LANGFILE,
                                                IDS_ENTER_PASSWORD, 
                                                text,
                                                sizeof(text));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                    AEE_FONT_BOLD, 
                                    text,
                                    -1, 
                                    xOffset, 
                                    TITLEBAR_HEIGHT + MENUITEM_HEIGHT*1/2,
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
                
                // ��������
                nLen = STRLEN(pMe->m_strPhonePWD);
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                AEE_FONT_BOLD, 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
                                TITLEBAR_HEIGHT + MENUITEM_HEIGHT*3/2,
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
                
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    CoreDrawBottomBar(BTBAR_OK_DELETE)
                }
                else if(nLen > 0)
                {// ɾ��
                    CoreDrawBottomBar(BTBAR_DELETE)
                }
                else
                {// SOS
                    CoreDrawBottomBar(BTBAR_SOS)
                }
            }
            // ������ʾ
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);  
            return TRUE;
            
        case EVT_DIALOG_END:
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
                        
                    case AVK_SEND:
                        CLOSE_DIALOG(DLGRET_EMGCALL)
                        return TRUE;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        if (STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_EMGCALL)
                            return TRUE;
                        }
                        break;
                        
                    case AVK_SELECT:
                        if (STRLEN(pMe->m_strPhonePWD) < 4)
                        {
                            return TRUE;
                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_ENTEROK)
                        }
                        return TRUE;
                        
                    default:
                        return TRUE;
                }
                
                nLen = STRLEN(pMe->m_strPhonePWD);
                if (chEnter == 0)
                {// ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < PHONEPASSWORDLENTH)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                                            AEECLSID_CORE_APP,
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
} // IDD_PWDINPUT_Handler

/*==============================================================================
����:
    IDD_UIMSECCODE_Handler
    
˵��:
    IDD_UIMSECCODE �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
       
��ע:
    ���������� PIN �롢 PUK ���������洦��
       
==============================================================================*/
static boolean  IDD_UIMSECCODE_Handler(void       *pUser,
                                       AEEEvent   eCode,
                                       uint16     wParam,
                                       uint32     dwParam)
{
    static char *szCode = NULL;
    static int nMaxLen = 0;
    static uint16 nResID = 0;
    //IMenuCtl *pMenu = NULL;
    CCoreApp *pMe = (CCoreApp *)pUser;
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    MSG_FATAL("%x %x %x IDD_UIMSECCODE_Handler",eCode,wParam,dwParam);
    //pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_UIMSECCODE_MENU);
    //if (NULL == pMenu)
    //{
    //    return FALSE;
    //}
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            szCode = NULL;
            nMaxLen = 0;
            return TRUE;
            
        case EVT_DIALOG_START:
        {
            //uint16 nResID = 0;            
            // ��ʼ����ʼ����
            switch (pMe->m_eRUIMSCode)
            {
                case ENTERPIN_ONLY:
                    MEMSET(pMe->m_strPIN1, 0, sizeof(pMe->m_strPIN1));
                    szCode = pMe->m_strPIN1;
                    nMaxLen = PINCODE_LENGTH;
                    nResID = IDS_ENTER_PIN;
                    break;
                    
                case ENTERPUK_STEP1:
                    MEMSET(pMe->m_strPIN1, 0, sizeof(pMe->m_strPIN1));
                    szCode = pMe->m_strPIN1;
                    nMaxLen = PINCODE_LENGTH;
                    nResID = IDS_NEWPIN;
                    break;
                    
                case ENTERPUK_STEP2:
                    MEMSET(pMe->m_strPIN2, 0, sizeof(pMe->m_strPIN2));
                    szCode = pMe->m_strPIN2;
                    nMaxLen = PINCODE_LENGTH;
                    nResID = IDS_NEWPIN_AGAIN;
                    break;
                    
                case ENTERPUK_STEP0:
                    MEMSET(pMe->m_strPUK, 0, sizeof(pMe->m_strPUK));
                    szCode = pMe->m_strPUK;
                    nMaxLen = PUKCODE_LENGTH;
                    nResID = IDS_ENTER_PUK;
                    break;
                
                default:
                    szCode = NULL;
                    break;
            }           
            
            ISHELL_PostEvent(pMe->a.m_pIShell,AEECLSID_CORE_APP,EVT_USER_REDRAW, 0,0);            
            return TRUE;
        }
            
            
        case EVT_USER_REDRAW:
            if (NULL == szCode)
            {
                return TRUE;
            }
    
            // ���������Ϣ
            {
                AECHAR  wstrDisplay[12] = {0};
                AEECHVStatus  chvst;
                int xOffset = 5, nLen = 0;
                AECHAR  wstrCnt[32] = {0};
                char    strDisplay[12] = {0};
                AECHAR  text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};
                
                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->a.m_pIShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
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
                // ��������
                if (nResID != 0)
                {
                    ISHELL_LoadResString(pMe->a.m_pIShell, 
                                                        AEE_COREAPPRES_LANGFILE,
                                                        nResID,
                                                        text, 
                                                        sizeof(text));
                    TitleBar_Param.pwszTitle = text;
                    TitleBar_Param.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                    DrawTitleBar(pMe->m_pDisplay,&TitleBar_Param);
                }
                
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                if ((pMe->m_eRUIMSCode == ENTERPIN_ONLY) ||
                    (pMe->m_eRUIMSCode == ENTERPUK_STEP0))
                {// ���ƴ�����ʾ�ַ���
                    // ��ȡUIM����״̬
                    (void)IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
                    
                    (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                                AEE_COREAPPRES_LANGFILE,
                                                IDS_AVAILCNT,
                                                wstrCnt,
                                                sizeof(wstrCnt));
                                                
                    if (pMe->m_eRUIMSCode == ENTERPIN_ONLY)
                    {
                        WSPRINTF(text, sizeof(text),
                                 wstrCnt,  chvst.chv1CountRemain);
                    }
                    else
                    {
                        WSPRINTF(text, sizeof(text),
                                 wstrCnt,  chvst.chv1UnblockRemain);
                    }
                }
                else
                {
                    (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                                AEE_COREAPPRES_LANGFILE,
                                                IDS_ENTER_PASSWORD,
                                                text,
                                                sizeof(text));
                }
                    
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                                    AEE_FONT_BOLD, 
                                                    text,
                                                    -1, 
                                                    xOffset, 
                                                    TITLEBAR_HEIGHT + MENUITEM_HEIGHT*1/2, 
                                                    NULL, 
                                                    IDF_TEXT_TRANSPARENT);
                
                // ��������
                nLen = STRLEN(szCode);
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                                AEE_FONT_BOLD, 
                                                wstrDisplay,
                                                -1, 
                                                2*xOffset,
                                                TITLEBAR_HEIGHT + MENUITEM_HEIGHT*3/2,
                                                NULL, 
                                                IDF_TEXT_TRANSPARENT);
                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);

                
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    CoreDrawBottomBar(BTBAR_OK_DELETE)
                }
                else if(nLen > 0)
                {// ɾ��
                    CoreDrawBottomBar(BTBAR_DELETE)
                }
                else
                {//SOS
                    CoreDrawBottomBar(BTBAR_SOS)
                }
            }
            
            // ������ʾ
            //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);  
            return TRUE;
            
        case EVT_DIALOG_END:
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
                        
                    case AVK_SEND:
                        CLOSE_DIALOG(DLGRET_EMGCALL)
                        return TRUE;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        if (szCode == NULL)
                        {
                            return TRUE;
                        }
                        if (STRLEN(szCode) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_EMGCALL)
                            return TRUE;
                        }
                        break;
                        
                    case AVK_SELECT:
                        if (szCode == NULL)
                        {
                            return TRUE;
                        }
                        if (STRLEN(szCode) < 4)
                        {
                            return TRUE;
                        }
//                        else if ((nMaxLen == PINCODE_LENGTH) && (STRLEN(szCode) < nMaxLen))
//                        {
//                            return TRUE;
//                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_OK)
                        }
                        return TRUE;
                        
                    default:
                        return TRUE;
                }
                
                if (szCode == NULL)
                {
                    return TRUE;
                }
                
                nLen = STRLEN(szCode);
                if (chEnter == 0)
                {// ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        szCode[nLen-1] = chEnter;
                    }
                }
                else if (nLen < nMaxLen)
                {
                    szCode[nLen] = chEnter;
                    nLen++;
                    szCode[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                                            AEECLSID_CORE_APP,
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
} // IDD_UIMSECCODE_Handler

/*==============================================================================
����:
    IDD_UIMERR_Handler
       
˵��:
    IDD_UIMERR �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
       
��ע:
    �������� UIM ���������ʾ������¼���
       
==============================================================================*/
static boolean  IDD_UIMERR_Handler(void       *pUser,
                                   AEEEvent   eCode,
                                   uint16     wParam,
                                   uint32     dwParam)
{
    IStatic * pStatic = NULL;
    CCoreApp *pMe = (CCoreApp *)pUser;
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    MSG_FATAL("%x %x %x IDD_UIMERR_Handler",eCode,wParam,dwParam);
    pStatic = (IStatic*)IDIALOG_GetControl(pMe->m_pActiveDlg, 
                            IDC_UIMERR_STATIC);

    if (NULL == pStatic)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
	        MEMSET(pMe->m_strLockuimPWD, 0, sizeof(pMe->m_strLockuimPWD));
            return TRUE;
            
        case EVT_DIALOG_START:
            //���ÿؼ�����
            {
                AEERect rc;
                SETAEERECT(&rc, 0, 0,pMe->m_rc.dx, pMe->m_rc.dy - BOTTOMBAR_HEIGHT);
                
                // ������
                IDISPLAY_EraseRect(pMe->m_pDisplay,&rc);
                rc.x = 1;
                rc.dx -= 2;
                ISTATIC_SetRect(pStatic, &rc);
                
                // ���þ�̬�ı��ؼ�����
                ISTATIC_SetProperties(pStatic, ST_UNDERLINE | 
                        ST_CENTERTEXT  | ST_NOSCROLL|ST_MIDDLETEXT);
            }

            (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                      AEECLSID_CORE_APP,
                      EVT_USER_REDRAW,
                      0,
                      0);
            return TRUE;
            
        case EVT_USER_REDRAW:
            {
                uint16    nResID = 0;
                AECHAR    *wTipsMsg = NULL;
                
                switch (pMe->m_eUIMErrCode)
                {
                    // û���뿨
                    case UIMERR_NOUIM:
                        nResID = IDS_NORUIM;
                        break;
                        
                    // UIM ����Ч(ԭ����PUK�볬���涨�������¿�ʧЧ)
                    case UIMERR_BLOCKED:
                        nResID = IDS_UIMBLOCKED;
                        break;
                        
                    // UIM ����Ч(ԭ���ǿ���������Ӫ��Ҫ���Ҫ������)
                    case UIMERR_LOCKED:
                        nResID = IDS_LOCKEDRUIM;
                        break;
                        
                    // UIM ����������, ����Ӧִ�е���
                    default:
                        break;
                }
                
                if (nResID != 0)
                {
                    int32  nSize = 128 * sizeof(AECHAR);
                    
                    wTipsMsg = (AECHAR *) MALLOC(nSize);
                    
                    if (NULL != wTipsMsg)
                    {
                        // ȡ��ʾ�ı�
                        (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                                    AEE_COREAPPRES_LANGFILE,
                                                    nResID,
                                                    wTipsMsg,
                                                    nSize);
                                                    
                        // ���þ�̬�ؼ��ı�
                        (void)ISTATIC_SetText(pStatic,
                                NULL,
                                wTipsMsg,
                                AEE_FONT_NORMAL,
                                AEE_FONT_NORMAL);
                                
                        // �ͷŲ���ʹ�õ���ʱ buffer
                        FREE(wTipsMsg);
                        wTipsMsg = NULL;
                    }
                }
                
                ISTATIC_SetActive(pStatic, TRUE);
                (void) ISTATIC_Redraw(pStatic);
                
                // ���Ƶײ���ʾ�˵�
                // -----SOS
                CoreDrawBottomBar(BTBAR_SOS)

                // ����������Ļ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            }          
            
            return TRUE;
            
        case EVT_DIALOG_END:
            return TRUE;
        case EVT_KEY:
		{
		char  chEnter = 0; 
		static int   nLen = 0; 
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
                        
                    case AVK_SEND:
                        CLOSE_DIALOG(DLGRET_EMGCALL)
                        return TRUE;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        CLOSE_DIALOG(DLGRET_EMGCALL)
                        return TRUE;
                        
                    case AVK_SELECT:
                        CLOSE_DIALOG(DLGRET_ENTER)  
                        return TRUE;
                        
                    default:
                        return TRUE;
                }

                nLen = STRLEN(pMe->m_strLockuimPWD);
                if (chEnter == 0)
                {// ɾ���ַ�
                    if (nLen > 0)
                    {
                        pMe->m_strLockuimPWD[nLen-1] = chEnter; 
                    }
                }
                else if (nLen < LOCKUIMPASSWORDLENTH)
                {
                    pMe->m_strLockuimPWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strLockuimPWD[nLen] = 0;
                }
            break;
        }
        default:
            break;
    }
    
    return FALSE;
} // IDD_UIMERR_Handler
/*==============================================================================
����:
    IDD_STARTUPANI_Handler
    
˵��:
    IDD_STARTUPANI �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
       
��ע��WHEN                WHO      WHAT          WHY
      2008-2-16           fj       V1.6          New develop, Add power down animation  
==============================================================================*/
static boolean  IDD_STARTUPANI_Handler(void       *pUser,
                                       AEEEvent   eCode,
                                       uint16     wParam,
                                       uint32     dwParam)
{
    CCoreApp *pMe = (CCoreApp *)pUser;
    
    MSG_FATAL("%x %x %x IDD_STARTUPANI_Handler",eCode,wParam,dwParam);
    switch (eCode) 
    {
        case EVT_DIALOG_INIT:
            if(pMe->m_bemergencymode)
            {
                //exit the emergency mode
                ICM_SetSystemPreference(pMe->m_pCM,
                                                AEECM_MODE_PREF_PERSISTENT, AEECM_PREF_TERM_PERMANENT, 0,
                                                AEECM_GW_ACQ_ORDER_PREF_NO_CHANGE, AEECM_BAND_PREF_NO_CHANGE,
                                                AEECM_ROAM_PREF_NO_CHANGE, AEECM_HYBR_PREF_NO_CHANGE,
                                                AEECM_SRV_DOMAIN_PREF_NO_CHANGE, AEECM_NETWORK_SEL_MODE_PREF_NO_CHANGE,
                                                NULL, NULL, NULL);
                pMe->m_bemergencymode = FALSE;
            }
            IDIALOG_SetProperties((IDialog *)dwParam, DLG_NOT_REDRAW_AFTER_START);
            return TRUE;

        case EVT_DIALOG_START: 
            if(pMe->m_wStartupAniTime == 0)
            {
#ifndef FEATURE_USES_LOWMEM
                if ( NULL == pMe->m_pStartupAniImg )
                {
                    pMe->m_pStartupAniImg = ISHELL_LoadImage( pMe->a.m_pIShell, PWRON_ANI_FILE);
                }
#endif
                (void) ISHELL_PostEvent(pMe->a.m_pIShell, AEECLSID_CORE_APP, EVT_USER_REDRAW, 0, 0);
            }
            else
            {
                //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            }
            return TRUE;

        case EVT_USER_REDRAW:
            if(pMe->m_wStartupAniTime == 0)
            {
                // �ڴ˲��ſ�������
                uint16    aRing_type[PROFILENUMBER]  = {0} ;                                          // CFGI_PROFILE_STARTUP_MUSIC
                byte      Ring_Cur_Music                      =  OEMNV_PROFILE_NORMALMODE;
                byte      poweronoff_alert[PROFILENUMBER] =  {0};

                ICONFIG_GetItem( pMe->m_pConfig, CFGI_PROFILE_CUR_NUMBER, &Ring_Cur_Music, sizeof(Ring_Cur_Music) ); 
                ICONFIG_GetItem(pMe->m_pConfig,CFGI_PROFILE_POWERONOFF_ALERT,poweronoff_alert,sizeof(poweronoff_alert));  
                if(poweronoff_alert[Ring_Cur_Music]  == OEMNV_POWERONOFF_ENABLE)
                {
                    ICONFIG_GetItem( pMe->m_pConfig, CFGI_PROFILE_STARTUP_MUSIC, aRing_type, sizeof(aRing_type) );
                    //DBGPRINTF("fj music_num = %d , Ring_Cur_Music = %d, IDD_STARTUPANI_Handler",(uint32)aRing_type[Ring_Cur_Music], Ring_Cur_Music);
                    IALERT_StartRingerAlert_Ex( pMe->m_pAlert, (uint32)aRing_type[Ring_Cur_Music] );
                }

                // ���ſ�������
                //pMe->m_wStartupAniTime = 0;
#ifndef FEATURE_USES_LOWMEM
                if ( NULL != pMe->m_pStartupAniImg )
#endif
                {     
                    CoreApp_PlayPwrOnAni(pMe);
                }
#ifndef FEATURE_USES_LOWMEM
                else
                {
                    CLOSE_DIALOG( DLGRET_OK ); 
                }
#endif
                //ISHELL_StartAppletArgs(pMe->a.m_pIShell, AEECLSID_DIALER, "S");
            }
            return TRUE; 

        case EVT_DIALOG_END:
            //IALERT_StopRingerAlert(pMe->m_pAlert);
            if (pMe->m_eDlgRet != DLGRET_OK)
            {// �����������Ź����б�����Ӧ������ʱ�ж�
#ifndef FEATURE_USES_LOWMEM
                if (NULL != pMe->m_pStartupAniImg)
                {     
                    (void)ISHELL_CancelTimer(pMe->a.m_pIShell, (PFNNOTIFY)CoreApp_PlayPwrOnAni, pMe);
                    IIMAGE_Stop(pMe->m_pStartupAniImg);
                    IIMAGE_Release(pMe->m_pStartupAniImg);
                    pMe->m_pStartupAniImg = NULL;
                    
                    pMe->m_eDlgRet = DLGRET_OK;
                }
#else
                (void)ISHELL_CancelTimer(pMe->a.m_pIShell, (PFNNOTIFY)CoreApp_PlayPwrOnAni, pMe);
                pMe->m_eDlgRet = DLGRET_OK;
#endif
                IALERT_StopRingerAlert(pMe->m_pAlert);
            }
            return TRUE;

        case EVT_DISPLAYDIALOGTIMEOUT:
            return TRUE;

        case EVT_KEY:
            return TRUE;

        default:
            break;
    }

    return FALSE;
}

/*==============================================================================
����:
    IDD_LOADING_Handler
    
˵��:
    IDD_LOADING �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
       
��ע:
    ������ʾ��ʾ��Ϣ���û������Զ��رշ��ء�
       
==============================================================================*/
static boolean  IDD_LOADING_Handler(void       *pUser,
                                   AEEEvent   eCode,
                                   uint16     wParam,
                                   uint32     dwParam)
{
    CCoreApp *pMe = (CCoreApp *)pUser;
    IStatic * pStatic = NULL;
    
    MSG_FATAL("%x %x %x IDD_LOADING_Handler",eCode,wParam,dwParam);
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    pStatic = (IStatic*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_LOADING);

    if (NULL == pStatic)
    {
        return FALSE;
    }
    
    switch(eCode)
    {
        case EVT_DIALOG_INIT:
            ISTATIC_SetProperties(pStatic, (ST_CENTERTEXT | ST_MIDDLETEXT));
            return TRUE;

        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->a.m_pIShell, 
                                    AEECLSID_CORE_APP,
                                    EVT_USER_REDRAW, 
                                    0, 0);

            return TRUE;

        case EVT_USER_REDRAW:
            {
                AECHAR  *wstrText=NULL;
                int32  nSize=0;

                nSize = 32 * sizeof(AECHAR);
                wstrText = (AECHAR *) MALLOC(nSize);
                
                if (NULL != wstrText)
                {
                    // ����Դ�ļ�ȡ��Ϣ����
                    (void)ISHELL_LoadResString(pMe->a.m_pIShell,
                                    AEE_COREAPPRES_LANGFILE,                                
                                    IDS_LOADING,
                                    wstrText,
                                    nSize);
                                    
                    (void) ISTATIC_SetText(pStatic,
                                    NULL,
                                    wstrText,
                                    AEE_FONT_NORMAL,
                                    AEE_FONT_NORMAL);
                                    
                    FREE(wstrText);
                }
            }
            
            (void) ISTATIC_Redraw(pStatic);

            // �����Զ��رնԻ���Ķ�ʱ��
            (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                                  1000,
                                  DialogTimeoutCallback, 
                                  pMe);
            if(IsRunAsUIMVersion())
            {
                if(IRUIM_IsCardConnected(pMe->m_pIRUIM))
                {
                    if (NULL == pMe->m_pAddrRUIM)
                    {
                        (void) ISHELL_CreateInstance( pMe->a.m_pIShell,
                                                  AEECLSID_ADDRBOOK_RUIM,
                                                  (void **)&pMe->m_pAddrRUIM);
                    }
                }
            }
            
            if (NULL == pMe->m_pAddrPhone)
            {            
                (void) ISHELL_CreateInstance( pMe->a.m_pIShell,
                                      AEECLSID_ADDRBOOK,
                                      (void **)&pMe->m_pAddrPhone);
            }	    
            //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            return TRUE;
        
        case EVT_DISPLAYDIALOGTIMEOUT:            
            CLOSE_DIALOG(DLGRET_INITED);
            return TRUE;

        default:
            break;
    }

    return FALSE;
}

static void CoreApp_ImageNotify(void *po, IImage *pIImage, AEEImageInfo *pii, int nErr)
{   
    CCoreApp *pMe = (CCoreApp *)po;
    
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
    db_items_value_type  need_capture;
#endif

    MSG_ERROR("CoreApp_ImageNotify, nErr:%d",nErr, 0, 0);
    if(SUCCESS == nErr) 
    {
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
        if (pMe->m_capture == DB_CAPTURE_NEED)
        {
            extern IDIB ** OEMANNUNCIATOR_GetWallpaperBarDDB(void);
            IDIB **ppDIB=NULL;
            AEERect rc;
            IBitmap *pDevBmp=NULL;
            IBitmap *pBmp =NULL;
            MSG_FATAL("CoreApp_ImageNotify DB_CAPTURE_NEED",0,0,0);
            pMe->m_capture = DB_CAPTURE_NONE;
            ppDIB = OEMANNUNCIATOR_GetWallpaperBarDDB();
            if ((NULL != ppDIB) &&
                (SUCCESS == IANNUNCIATOR_GetAnnunciatorBarSize(pMe->m_pIAnn,AEECLSID_DISPLAY1, &rc)))
            {
                if (IDISPLAY_GetDeviceBitmap(pMe->m_pDisplay, &pDevBmp) == SUCCESS) 
                {
                    MSG_FATAL("CoreApp_ImageNotify IDISPLAY_GetDeviceBitmap SUCCESS",0,0,0);
                    IBITMAP_CreateCompatibleBitmap(pDevBmp, &pBmp, rc.dx, rc.dy);
                    IBITMAP_Release(pDevBmp);
                    
                    IDISPLAY_SetDestination(pMe->m_pDisplay, pBmp);
                    if ( NULL != pWallPaper )
                    {
                        MSG_FATAL("CoreApp_ImageNotify NULL != pWallPaper",0,0,0);
                        IIMAGE_SetDrawSize(pWallPaper, rc.dx, rc.dy);
                        IIMAGE_Draw(pWallPaper, 0, 0);
                    }
                    else
                    {
                        MSG_FATAL("CoreApp_ImageNotify NULL != IDISPLAY_EraseRect",0,0,0);
                        IDISPLAY_EraseRect(pMe->m_pDisplay, &rc);
                    }
                   // IDISPLAY_UpdateEx(pMe->m_pDisplay, TRUE); // debug for update issue
                    
                    IDISPLAY_SetDestination(pMe->m_pDisplay, NULL);
                    (void)IBITMAP_QueryInterface(pBmp, AEECLSID_DIB, (void **) ppDIB);
                    IBITMAP_Release (pBmp);
                }
            }
        }
#endif                
        if ( NULL != pWallPaper )
        {
            AEEImageInfo info;
            MSG_FATAL("pMe->m_rc.dx:%d, pMe->m_rc.dy:%d", pMe->m_rc.dx, pMe->m_rc.dy, 0);
            IImage_GetInfo(pWallPaper, &info);
            IDisplay_ClearScreen(pMe->m_pDisplay);
            IIMAGE_Draw(pWallPaper, (pMe->m_rc.dx - info.cx)/2, (pMe->m_rc.dy - info.cy)/2);
        }
        else
        {
            MSG_FATAL("CoreApp_ImageNotify NULL != pWallPaper",0,0,0);
            IDISPLAY_EraseRect(pMe->m_pDisplay, &pMe->m_rc);
        }

#ifdef FEATRUE_SET_ANN_FULL_SCREEN
        need_capture.b_capture = DB_CAPTURE_INIDLE;
        db_put(DB_CAPTURE_WALLPER,&need_capture);
        if(pMe->m_pIAnn != NULL)
        {
            IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,TRUE);//added by chengxiao 2008.11.26
        }
#endif

        if (bHideText)
        {
            MSG_FATAL("CoreApp_ImageNotify bHideText",0,0,0);
            CoreApp_UpdateAnnunciator(pMe);//����������Ż����AVK_END��״̬��ͼ����˸����
                                         // �ʰѸþ��ᵽIDISPLAY_UpdateEx֮ǰ
#ifdef FEATURE_APP_MUSICPLAYER                                         
            ISHELL_CancelTimer(pMe->a.m_pIShell,(PFNNOTIFY)CoreApp_MusicNameAutoScroll,pMe);
#endif
           // IDISPLAY_UpdateEx(pMe->m_pDisplay, TRUE); // debug for update issue
            bImageDecoded = TRUE;
            return;
        }  
        MSG_FATAL("CoreApp_ImageNotify 2222",0,0,0);
#ifdef FEATURE_APP_MUSICPLAYER
        //CoreApp_DrawMusicName(pMe);
        Core_DrawNameResetScroll(pMe);
#endif

        // ���Ʒ����ṩ�����ʹ����ʺ���
        CoreApp_DrawBannerMessage(pMe);
    
        // ���Ƶ�ǰ���ڡ�ʱ����Ϣ
        CoreApp_UpdateDateTime(pMe);
        CoreApp_UpdateBottomBar(pMe);//didn't display the sos and key lock icon at the same time
        CoreApp_UpdateAnnunciator(pMe);  //����������Ż����AVK_END��״̬��ͼ����˸����
                                         // �ʰѸþ��ᵽIDISPLAY_UpdateEx֮ǰ
    }   
    bImageDecoded = TRUE;
    IDISPLAY_UpdateEx(pMe->m_pDisplay,TRUE);
    MSG_FATAL("CoreApp_ImageNotify End",0,0,0);
}


/*==============================================================================
����:
    IDD_IDLE_Handler
    
˵��:
    IDD_IDLE �Ի����¼�������
       
����:
    pUser [in]: ���������ָ�� Core Applet ����ṹ��ָ�롣
    eCode [in]: �¼����롣
    wParam: �¼�������ݡ�
    dwParam: �¼�������ݡ�
       
����ֵ:
    TRUE:  �����¼�������
    FALSE: �����¼������ԡ�
       

��ע     :  WHEN                    WHO                WHAT                     WHY
           2008-02-20              fj                 V1.6                      New Dev, for Wallpaper       
==============================================================================*/
#ifdef FEATURE_SPORTS_APP
extern boolean is_g_sportsapp_pedometer_bground_flag();
#endif

static boolean  IDD_IDLE_Handler(void       *pUser,
                                 AEEEvent   eCode,
                                 uint16     wParam,
                                 uint32     dwParam)
{
	//int i;
    CCoreApp *pMe = (CCoreApp *)pUser;
#ifdef FEATURE_KEYGUARD	 
    boolean  bData;
#endif
    MSG_FATAL("%x %x %x IDD_IDLE_Handler",eCode,wParam,dwParam);
    if (NULL == pMe)
    {
        return FALSE;
    }
    if(pMe->m_pIAnn != NULL)
    {
	    IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,TRUE);
    }
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            MSG_FATAL("IDD_IDLE_Handler EVT_DIALOG_INIT",0,0,0);
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetHasTitleText(pMe->m_pIAnn, TRUE);//���ش�������ʱ��Ҫ����ʾtitlebar��־��ԭ��TRUE
            }
            MEMSET(pMe->m_wstrEnterNum, 0, sizeof(pMe->m_wstrEnterNum));
            //CoreApp_GetRecordCount(pMe);
            CoreApp_DrawWallPaper(pMe);
#ifdef FEATURE_KEYGUARD
            if(pMe->m_b_set_lock)
            {
                OEMKeyguard_SetState(TRUE);
                pMe->m_b_set_lock = FALSE;
            }
#endif

            return TRUE;
            
        case EVT_DIALOG_START:
        {
#ifdef FEATRUE_SUPPORT_G_SENSOR
            dword shake;
#endif
            MSG_FATAL("IDD_IDLE_Handler EVT_DIALOG_START",0,0,0);
#ifndef CUST_EDITION
            if(!((MMI_GSENSOR_SHAKE_OPEN == mmi_g_sensor_state) 
                ||(MMI_GSENSOR_SHAKE_OPEN_IN_IDLE == mmi_g_sensor_state)))   //ignore if sensor has been open.
            {
                ICONFIG_GetItem(pMe->m_pConfig,CFGI_GSENSOR,&shake,sizeof(shake));
                if(shake & OEMNV_SHAKE_WALLPAPER_MASK)
                {
#ifdef FEATRUE_SUPPORT_G_SENSOR
#ifndef WIN32//wlh ��ʱ�޸�
                    if(is_g_sportsapp_pedometer_bground_flag() == FALSE)
                    {
                        mmi_g_sensor_process(G_SENSOR_ENABLE | G_SENSOR_SHAKE);
                        mmi_g_sensor_state = MMI_GSENSOR_SHAKE_OPEN;
                    }
#endif//WIN32
#endif
                } 
            }
#endif            
            // ����ǽֽ�������ʾ���Ӫ�����ơ������ʺ���
            // �Լ�ʱ�䡢���ڡ����ڵȡ������ж�ʱ����������
#if(! defined FEATURE_CARRIER_ISRAEL_PELEPHONE) && (! defined FEATURE_LANG_HEBREW)
            if(pMe->svc_p_name[0] == 0)
#endif
            {
                //DBGPRINTF("IRUIM_Read_Svc_P_Name");
                if(IsRunAsUIMVersion())
                {
                    if(EFAILED == IRUIM_Read_Svc_P_Name(pMe->m_pIRUIM,pMe->svc_p_name))
                    {
                        MSG_FATAL("EFAILED == IRUIM_Read_Svc_P_Name",0,0,0);
                        MEMSET(pMe->svc_p_name,0,(UIM_CDMA_HOME_SERVICE_SIZE+1)*sizeof(AECHAR));
                        ICONFIG_GetItem(pMe->m_pConfig, CFGI_BANNER, pMe->svc_p_name, (NV_MAX_LTRS+1)*sizeof(AECHAR));
                    }
                }
                else
                {
                    MEMSET(pMe->svc_p_name,0,(UIM_CDMA_HOME_SERVICE_SIZE+1)*sizeof(AECHAR));
                    {
#if(defined FEATURE_CARRIER_ISRAEL_PELEPHONE) && (defined FEATURE_LANG_HEBREW)
                        nv_language_enum_type sel = 0;
                        ICONFIG_GetItem(pMe->m_pConfig, CFGI_LANGUAGE_SELECTION, &sel, sizeof(sel));
                        if(sel ==  NV_LANGUAGE_HEBREW)
                        {
                            ISHELL_LoadResString(pMe->a.m_pIShell,AEE_COREAPPRES_LANGFILE, IDS_ISRAEL_BANNER, pMe->svc_p_name, sizeof(pMe->svc_p_name));
                        }
                        else
#endif
                        {
                            ICONFIG_GetItem(pMe->m_pConfig, CFGI_BANNER, pMe->svc_p_name, (NV_MAX_LTRS+1)*sizeof(AECHAR));
                        }
                    }
                }
            }
            CoreApp_UpdateIdleTimer(pMe);
			
#ifdef FEATURE_KEYGUARD
            if(!OEMKeyguard_IsEnabled())
            {
                (void)ISHELL_CancelTimer(pMe->a.m_pIShell,
                                CoreApp_TimeKeyguard,
                                pMe);
            (void) ICONFIG_GetItem(pMe->m_pConfig,
                                    CFGI_KEY_LOCK_CHECK/*CFGI_PHONE_KEY_LOCK_CHECK*/,
                                    &bData,
                                    sizeof(bData));
            if(bData)
            {
                if ((ISHELL_ActiveApplet(pMe->a.m_pIShell) == AEECLSID_CORE_APP)&&
                                    (pMe->m_wActiveDlgID == IDD_IDLE))
                {
                    (void)ISHELL_SetTimer(pMe->a.m_pIShell,
                                    30*1000,
                                    CoreApp_TimeKeyguard,
                                    pMe);
                    }
                }
            }
#endif		
#ifdef CUST_EDITION    
            {
				extern void CoreApp_InitBattStatus(CCoreApp * pMe);

            	ISHELL_SetTimer(pMe->a.m_pIShell, 3*1000,(PFNNOTIFY)CoreApp_InitBattStatus,  pMe);                                                                                                         
            }
#endif			
        return TRUE;
        }
            
        case EVT_UPDATEIDLE:
        {
            MSG_FATAL("IDD_IDLE_Handler EVT_UPDATEIDLE",0,0,0);
            CoreApp_DrawWallPaper(pMe); // debug for wallpaper update issue
            if(pWallPaper)
            {
                MSG_FATAL("IDD_IDLE_Handler pWallPaper != NULL",0,0,0);
                bImageDecoded = FALSE;
                IImage_Notify(pWallPaper, (PFNIMAGEINFO)CoreApp_ImageNotify, pMe);
            }

#if defined(FEATURE_WMS_APP)
            if (pMe->m_bsmstipscheck)
            {
                uint16  nNewsVmail=0, nNewsSMS=0;
                MSG_FATAL("IDD_IDLE_Handler pMe->m_bsmstipscheck == TRUE",0,0,0);
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_VOICEMAIL, &nNewsVmail, NULL, NULL);
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, &nNewsSMS, NULL, NULL);
                
                if (nNewsVmail>0 || nNewsSMS>0)
                {
                    CLOSE_DIALOG(DLGRET_SMSTIPS)
                }
            }
#endif            
            
            return TRUE;            
        }

        case EVT_DIALOG_END:
            // ȡ����ض�ʱ��
            MSG_FATAL("IDD_IDLE_Handler EVT_DIALOG_END",0,0,0);
            (void) ISHELL_CancelTimer(pMe->a.m_pIShell,
                                      CoreApp_UpdateIdleTimer,
                                      pMe);
            (void) ISHELL_CancelTimer(pMe->a.m_pIShell,
                                      CoreApp_SearchingTimer,
                                      pMe);
#ifdef FEATURE_KEYGUARD	    
	     (void)ISHELL_CancelTimer(pMe->a.m_pIShell, 
                                    CoreApp_TimeKeyguard,
                                    pMe);
#endif
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
            {
                db_items_value_type  need_capture;
                need_capture.b_capture = DB_CAPTURE_NONE;
                db_put(DB_CAPTURE_WALLPER,&need_capture);
                if(pMe->m_pIAnn != NULL)
                {
                    IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,FALSE);
                }
            }
#endif
            return TRUE;
       
        case EVT_KEY_PRESS: 
            MSG_FATAL("IDD_IDLE_Handler EVT_KEY_PRESS",0,0,0);
            if(pMe->m_bemergencymode)
            {
                return TRUE;
            }
            //if set jpeg as wallpaper, it needs more time to finish decode. in this case
            // press other key in idle quickly while exitting from setting wallpaper, topline
            // will be overlapped with signal bar and other app's title.
            if(bImageDecoded == FALSE && pWallPaper)
            {
                MSG_FATAL("IDD_IDLE_Handler bImageDecoded == FALSE && pWallPaper",0,0,0);
                return TRUE;
            }
            switch (wParam)
            {
				
                case AVK_UP:

#if defined(FEATURE_WMS_APP)
                    MSG_FATAL("IDD_IDLE_Handler AVK_UP",0,0,0);
                    return CoreApp_LaunchApplet(pMe, AEECLSID_WMSAPP);
#endif
                case AVK_DOWN:
                    MSG_FATAL("IDD_IDLE_Handler AVK_DOWN",0,0,0);
                    return CoreApp_LaunchApplet(pMe, AEECLSID_ALARMCLOCK); 
                case AVK_LEFT:
                {
                    MSG_FATAL("IDD_IDLE_Handler AVK_LEFT",0,0,0);
                    return CoreApp_LaunchApplet(pMe, AEECLSID_APP_SETTINGMENU);
                }
				
                case AVK_RIGHT:
					{
                        MSG_FATAL("IDD_IDLE_Handler AVK_RIGHT",0,0,0);
						return CoreApp_LaunchApplet(pMe, AEECLSID_APP_FMRADIO);//
					}
                case AVK_SELECT:
		 	{
				int ret = 0;
#ifdef FEATURE_FLEXI_STATIC_BREW_APP				
#ifdef STATIC_BREW_APP_FOR_NASRANI_NOR_MUSLIM
			   OEM_SetBAM_ADSAccount(STATIC_BREW_APP_FLEXI_NASRANI);
                        ret=  CoreApp_LaunchApplet(pMe, AEECLSID_NASRANI);
#else

			   OEM_SetBAM_ADSAccount(STATIC_BREW_APP_FLEXI_MUSLIM);
                        ret=  CoreApp_LaunchApplet(pMe, AEECLSID_MUSLIM);
#endif
#else
               return CoreApp_LaunchApplet(pMe, AEECLSID_MAIN_MENU);
#endif

				  return ret;
                	}

                case AVK_INFO:
					{
                        MSG_FATAL("AEECLSID_MAIN_MENU",0,0,0);
						return CoreApp_LaunchApplet(pMe, AEECLSID_MAIN_MENU);
					}
                case AVK_CLR:
                    MSG_FATAL("IDD_IDLE_Handler AVK_CLR",0,0,0);
#ifdef FEATURE_VERSION_SMART			//Add By zzg 2010_07_20
					if(!OEMKeyguard_IsEnabled())
                    {
                        return CoreApp_LaunchApplet(pMe, AEECLSID_FRENDUO);
                    }
#else
					if(!OEMKeyguard_IsEnabled())
                    {
                        return CoreApp_LaunchApplet(pMe, AEECLSID_APP_CONTACT);
                    	#ifdef FEATURE_FLEXI_STATIC_BREW_APP
							 OEM_SetBAM_ADSAccount(STATIC_BREW_APP_FLEXI_NASRANI);
							// ISHELL_StartApplet(pMe->m_pShell, AEECLSID_NASRANI);
							 return CoreApp_LaunchApplet(pMe, AEECLSID_NASRANI);
						#else
                        	return CoreApp_LaunchApplet(pMe, AEECLSID_APP_CONTACT);
						#endif
                    }
#endif
                    

                default:
                    break;
            }
            return FALSE;

        case EVT_GSENSOR_SHAKE:
        case EVT_KEY:
            {
#ifdef FEATRUE_SUPPORT_G_SENSOR
                boolean b_FMBackground = FALSE;
                dword shake;
#endif
#ifndef CUST_EDITION
                MSG_FATAL("IDD_IDLE_Handler EVT_GSENSOR_SHAKE",0,0,0);
                if((wParam != AVK_END)&&(wParam != AVK_GSENSOR_FORWARD)&&(wParam != AVK_GSENSOR_BACKWARD))
                {
                   if(!((MMI_GSENSOR_SHAKE_CLOSE == mmi_g_sensor_state) 
                    ||(MMI_GSENSOR_SHAKE_CLOSE_IN_IDLE == mmi_g_sensor_state)))  //ignore if sensor has been closed.
                    {
                        MSG_FATAL("IDD_IDLE_Handler 1",0,0,0);
                        OEM_GetConfig(CFGI_GSENSOR,&shake,sizeof(shake));
                        OEM_GetConfig(CFGI_FM_BACKGROUND,&b_FMBackground, sizeof(b_FMBackground));
                		//do close shake if FM or MP3 is playing.
                        if(!(((shake & OEMNV_SHAKE_FM_RADIO_MASK) && (TRUE == b_FMBackground))
                            ||((shake & OEMNV_SHAKE_MUSIC_MASK) && (MP3STATUS_NONE != GetMp3PlayerStatus()))))
						{
                    		if(is_g_sportsapp_pedometer_bground_flag() == FALSE)
                            {
#ifdef FEATRUE_SUPPORT_G_SENSOR	
#ifndef WIN32//wlh ��ʱ�޸�
                                MSG_FATAL("IDD_IDLE_Handler 2",0,0,0);
                                mmi_g_sensor_process(G_SENSOR_SHAKE_DISABLE);
#endif//WIN32
#endif             
                                mmi_g_sensor_state = MMI_GSENSOR_SHAKE_CLOSE;                    
                            }      
                		}
                    }
                }
#endif                
            }
            
            if(pMe->m_bemergencymode)
            {
                switch (wParam)
                {
                    case AVK_CLR:
                    case AVK_END:
                        MSG_FATAL("IDD_IDLE_Handler 3",0,0,0);
                        CLOSE_DIALOG(DLGRET_MSG)
                        //CoreApp_ShowMsgDialog(pMe,IDS_EXIT_EMERGENCY_MODE);
                        return TRUE;

                    case AVK_SEND:
                    case AVK_LEFT:   
                        MSG_FATAL("IDD_IDLE_Handler 4",0,0,0);
                        return TRUE;
                        
                    default:
                        break;
                }
            }

            //if set jpeg as wallpaper, it needs more time to finish decode. in this case
            // press select key in idle quickly while exitting from setting wallpaper, topline
            // will be overlapped with signal bar and other app's title.
            if(bImageDecoded == FALSE && pWallPaper)
            {
                return TRUE;
            }
            
            switch (wParam)
            {
                case AVK_CLR:
                    MSG_FATAL("IDD_IDLE_Handler AVK_CLR",0,0,0);
#ifdef FEATURE_KEYGUARD
                    MSG_FATAL("IDD_IDLE_Handler sbKeyguardEnabled=%d",OEMKeyguard_IsEnabled(),0,0);
                    if(OEMKeyguard_IsEnabled())
                    {
                        MSG_FATAL("IDD_IDLE_Handler 5",0,0,0);
                        OEMKeyguard_SetState(FALSE);
                        pMe->m_b_set_lock = TRUE;
                        CLOSE_DIALOG(DLGRET_EMGCALL)
                    }
#endif
                    return TRUE;

                case AVK_SELECT:
                    //return   CoreApp_LaunchApplet(pMe, AEECLSID_DIALER);
                    return TRUE;
                 // �ز�
                case AVK_SEND:
                    if (pMe->m_bAcquiredTime && !pMe->m_bemergencymode)
                    {
                        IRecentCalls  *pRecentCall = NULL;
                        if (AEE_SUCCESS == ISHELL_CreateInstance(pMe->a.m_pIShell,
                                                                 AEECLSID_APP_RECENTCALL,
                                                                 (void **)&pRecentCall))
                        {
                            MSG_FATAL("IDD_IDLE_Handler 6",0,0,0);
                            IRecentCalls_SendList(pRecentCall);
                            IRecentCalls_Release(pRecentCall);
                        }
                        return TRUE;                        
                    }
                    break;

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
                case AVK_STAR:
                case AVK_POUND:
                    {
                        ICallApp         *pCallApp = NULL;
                        MSG_FATAL("IDD_IDLE_Handler 7",0,0,0);
                        if ( SUCCESS != ISHELL_CreateInstance( pMe->a.m_pIShell,
                                                        AEECLSID_DIALER,
                                                        (void **)&pCallApp))
                        {
                            return FALSE;
                        }
                        MEMSET(pMe->m_wstrEnterNum, 0, sizeof(pMe->m_wstrEnterNum));
                        if ((AVKType)wParam == AVK_STAR)
                        {
                            pMe->m_wstrEnterNum[0] = (AECHAR)'*';
                        }
                        else if ((AVKType)wParam == AVK_POUND)
                        {
                            pMe->m_wstrEnterNum[0] = (AECHAR)'#';
                        }
                        else
                        {
                            // 48 Ϊ'0'�� ASCII ��ֵ
                            pMe->m_wstrEnterNum[0] = (AECHAR)
                                (48 + ((AVKType)wParam - AVK_0));/*lint !e656*/
                        }
                        ICallApp_VoiceCallDial_F(pCallApp,pMe->m_wstrEnterNum);
                        if (pCallApp) 
                        {
                            MSG_FATAL("IDD_IDLE_Handler 8",0,0,0);
                            ICallApp_Release(pCallApp);
                            pCallApp = NULL;
                        }
                        //MakeVoiceCallDial(pMe->a.m_pIShell , pMe->m_wstrEnterNum);
                        return TRUE;
                    }
                    
// ���, 2008.03.19, �����Ѿ��������������Ĵ�����,����������,�������������
#if 0
                case AVK_LEFT:
                {
                    CoreApp_LaunchApplet(pMe, AEECLSID_APP_FMRADIO);
                }
#endif
#ifndef CUST_EDITION
                case AVK_GSENSOR_FORWARD: 
                 {
                   boolean isShake0n = FALSE; 
                   MSG_FATAL("IDD_IDLE_Handler AVK_GSENSOR_FORWARD",0,0,0);
                   (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_SHAKE_WALLPAPER_CHECK,
                           &isShake0n, 
                           sizeof(boolean));
                    if(isShake0n)
                    {
                         IDisplayMenu * pDisplayMenu = NULL;
                         if (SUCCESS == ISHELL_CreateInstance(pMe->a.m_pIShell,
                             AEECLSID_APP_DISPLAYMENU,
                             (void**)&pDisplayMenu))
                         {
                             if (pDisplayMenu == NULL)
                             {
                                 return FALSE;
                             }
                             MSG_FATAL("IDD_IDLE_Handler 9",0,0,0);
                             IDisplayMenu_ChangePicture(pDisplayMenu, TRUE);
                         }
                         IDisplayMenu_Release(pDisplayMenu);
                         CLOSE_DIALOG(DLGRET_GS_SHAKE);
                         return TRUE;
                    }
                    else
                    {
                         return TRUE;
                    }
                     break;
                 }
                
                 case AVK_GSENSOR_BACKWARD:
                 {
                    boolean isShake0n = FALSE; 
                     MSG_FATAL("IDD_IDLE_Handler AVK_GSENSOR_BACKWARD",0,0,0);
                     (void) ICONFIG_GetItem(pMe->m_pConfig,
                            CFGI_SHAKE_WALLPAPER_CHECK,
                            &isShake0n, 
                            sizeof(boolean));
                     if(isShake0n)
                     {
                         IDisplayMenu * pDisplayMenu = NULL;
                         if (SUCCESS == ISHELL_CreateInstance(pMe->a.m_pIShell,
                             AEECLSID_APP_DISPLAYMENU,
                             (void**)&pDisplayMenu))
                         {
                             if (pDisplayMenu == NULL)
                             {
                                 return FALSE;
                             }
                             MSG_FATAL("IDD_IDLE_Handler 10",0,0,0);
                             IDisplayMenu_ChangePicture(pDisplayMenu, FALSE);
                         }
                         IDisplayMenu_Release(pDisplayMenu);
                         CLOSE_DIALOG(DLGRET_GS_SHAKE);
                         return TRUE;
                     }
                     else
                     {
                         return TRUE;
                     }
                     break;
                 }
#endif                 
          
                default:
                    break;
            }
            break;
        default:
            break;
    }
    MSG_FATAL("IDD_IDLE_Handler return FALSE",0,0,0);
    return FALSE;
} // IDD_IDLE_Handler

#if defined(FEATURE_WMS_APP)
/*==============================================================================
����:
    IDD_WMSTIPS_Handler

˵��:
    CoreApp �Ի��� IDD_WMSTIPS �¼���������

����:
    pMe [in]:       ָ��Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    eCode [in]:     �¼����롣
    wParam:         �¼�����
    dwParam [in]:   ��wParam���������ݡ�

����ֵ:
    TRUE:  �����¼��õ�����
    FALSE: �����¼�û������

��ע:

==============================================================================*/
static boolean IDD_WMSTIPS_Handler(void        *pUser, 
                                   AEEEvent    eCode, 
                                   uint16      wParam, 
                                   uint32      dwParam)
{
    static IStatic * pStatic = NULL;
    static IBitmap * pDevBmp = NULL;
    CCoreApp *pMe = (CCoreApp *)pUser;
    MSG_FATAL("%x %x %x IDD_WMSTIPS_Handler",eCode,wParam,dwParam);
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    if (NULL == pStatic)
    {
        AEERect rc = {0};

        if (ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_STATIC,
                (void**)&pStatic) == SUCCESS)
        {
            ISTATIC_SetRect(pStatic, &rc);
        }
        else
        {
            return FALSE;
        }
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,TRUE);
            }
            return TRUE;

        case EVT_DIALOG_START:
            (void) ISHELL_PostEventEx(pMe->a.m_pIShell, 
                                    EVTFLG_ASYNC,
                                    AEECLSID_CORE_APP,
                                    EVT_USER_REDRAW,
                                    0, 
                                    0);
            return TRUE;

        case EVT_USER_REDRAW:
            {
                AECHAR  wstrText[64]={0};
                AECHAR  wstrFMT[64]={0};
                uint16  nNewsVmail=0, nNewsSMS=0;
                PromptMsg_Param_type  Msg_Param={0};
                db_items_value_type  need_capture;
				int i = 0;
				int temp = 0;
                
                need_capture.b_capture = DB_CAPTURE_NONE;
                db_put(DB_CAPTURE_WALLPER,&need_capture);
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_VOICEMAIL, &nNewsVmail, NULL, NULL);
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, &nNewsSMS, NULL, NULL);
				#ifdef FEATURE_SUPPORT_ID
                //add by yangdecai
                temp = nNewsSMS;
                nNewsSMS = nNewsSMS/LONGSMS_MAX_PACKAGES;
				i = temp%LONGSMS_MAX_PACKAGES;
				if(i>0)
				{
					nNewsSMS ++;
				}
				#endif
				
                //add end
                
                if (nNewsVmail > 0)
                {
                    // ����Դ�ļ�ȡ��Ϣ����
                    (void)ISHELL_LoadResString(pMe->a.m_pIShell,
                                    AEE_COREAPPRES_LANGFILE,                                
                                    IDS_VMAILTIPS_FMT,
                                    wstrFMT,
                                    sizeof(wstrFMT));
                                    
                    WSPRINTF(wstrText, sizeof(wstrText), wstrFMT, nNewsVmail);
                }
                else
                {
                    // ����Դ�ļ�ȡ��Ϣ����
                    (void)ISHELL_LoadResString(pMe->a.m_pIShell,
                                    AEE_COREAPPRES_LANGFILE,                                
                                    IDS_NEWWMSTIPS_FMT,
                                    wstrFMT,
                                    sizeof(wstrFMT));
                                    
                    WSPRINTF(wstrText, sizeof(wstrText), wstrFMT, nNewsSMS);
                }
  
                Msg_Param.ePMsgType = MESSAGE_INFORMATION;
                Msg_Param.pwszMsg = wstrText;
                Msg_Param.eBBarType = BTBAR_READ_BACK;

                if(pDevBmp != NULL)
                {
                    IDISPLAY_BitBlt(pMe->m_pDisplay, 0, 0, pMe->m_rc.dx, pMe->m_rc.dy, 
                            pDevBmp, 0, 0, AEE_RO_COPY);
                }
                DrawPromptMessage(pMe->m_pDisplay, pStatic, &Msg_Param);
            }
            //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            
            return TRUE;

        case EVT_DIALOG_END:
            if(pMe->m_bActive)
            {
                if(pDevBmp != NULL)
                {
                    IBITMAP_Release(pDevBmp);
                    pDevBmp = NULL;
                }
            }
            else if(NULL == pDevBmp)
            {
                IBitmap *pTempBmp = NULL;

                IDISPLAY_GetDeviceBitmap(pMe->m_pDisplay, &pTempBmp);
                IBITMAP_CreateCompatibleBitmap(pTempBmp, &pDevBmp, pMe->m_rc.dx, pMe->m_rc.dy);
                if(NULL != pTempBmp && NULL != pDevBmp)
                {
                    IBITMAP_BltIn(pDevBmp, 0, 0, pMe->m_rc.dx, pMe->m_rc.dy, pTempBmp, 0, 0, AEE_RO_COPY);
                }
            }
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,FALSE);
            }
            return TRUE;
  
        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                case AVK_END:
#ifdef FEATURE_SMSTONETYPE_MID 
                // ֪ͨ�������ر�����
                    IALERT_StopSMSAlert(pMe->m_pAlert);
#endif //#if defined FEATURE_SMSTONETYPE_MID		    
                    IALERT_StopMp3Alert(pMe->m_pAlert);
                    pMe->m_bsmstipscheck = FALSE;
                    CLOSE_DIALOG(DLGRET_SMSTIPS_OK)
                    return TRUE;

                case AVK_SELECT:
#ifdef FEATURE_SMSTONETYPE_MID                        
                    IALERT_StopSMSAlert(pMe->m_pAlert);
#endif //#if defined FEATURE_SMSTONETYPE_MID		    
                    IALERT_StopMp3Alert(pMe->m_pAlert);
                    pMe->m_bsmstipscheck = FALSE;
                    CLOSE_DIALOG(DLGRET_VIEWSMS)
                    return TRUE;
  
                default:
                    break;
            }
            break;

        default:
            break;
    }

    return FALSE;
} // IDD_WMSTIPS_Handler
#endif

/*==============================================================================
������
       IDD_POWERDOWN_Handler
˵����
       IDD_POWERDOWN�Ի����¼�������
       
������
       pMe [in]��ָ��Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��WHEN                WHO      WHAT          WHY
     2008-2-16           fj       V1.6          New develop, Add power down animation      
==============================================================================*/
static boolean  IDD_POWERDOWN_Handler(void *pUser,
                                      AEEEvent   eCode,
                                      uint16     wParam,
                                      uint32     dwParam)
{ 
    CCoreApp *pMe = (CCoreApp *)pUser;    
    MSG_FATAL("%x %x %x IDD_POWERDOWN_Handler",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            EnableUIKeys(FALSE);
            return TRUE;
            
        case EVT_DIALOG_START:
            
#ifdef FEATURE_UIALARM
            // ���𾯱���
            CoreApp_EnableShellAlarms(pMe, FALSE);
#endif
            
#ifdef FEATRUE_AUTO_POWER
        OEMRTC_Process_Auto_Power_On();
#endif

#if defined( FEATURE_POWERDOWN_ALARM)
            {
                extern void registerAgainPowerdownAlarmclock( void);
                registerAgainPowerdownAlarmclock();
            }
#endif
#ifdef FEATURE_APP_MUSICPLAYER
          ISHELL_PostEvent(pMe->a.m_pIShell,
                            AEECLSID_APP_MUSICPLAYER,
                            EVT_CLOSEAPP,
                            0,
                            0);
#endif
            // ���ٹ��� IBatt ֪ͨ��Ϣ
            //(void) ISHELL_RegisterNotify(pMe->a.m_pIShell,
            //            AEECLSID_CORE_APP,
            //            0);

            // Turn off RSSI indicator
            //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RSSI, ANNUN_STATE_OFF);
           
            // �رյ��ͼ��
            //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BATT, ANNUN_STATE_OFF);
            if(pWallPaper)
            {
                IIMAGE_Release(pWallPaper);
                pWallPaper = NULL;
            }
            (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                              AEECLSID_CORE_APP,
                              EVT_USER_REDRAW,
                              0,
                              0);
            
            return TRUE;
            
        case EVT_USER_REDRAW:
 
            // ��ʾ������ش���
            {
                AECHAR    wszMsgText[128];
                AECHAR    wszNewLine[ ] = {(AECHAR)'\n', (AECHAR)'\0'};
                int nLen;
                wszMsgText[0] = 0;
                switch (pMe->m_ePowerDownType)
                {
#ifdef FEATURE_UIM
                    case POWERDOWN_RUIM_DOOR_RESET:
                       {    
                            // �ڴ˲�����ʾ��
                            
                            //IALERT_StartRingerAlert( pMe->m_pAlert, (uint32)aRing_type[Ring_Cur_Music] );
                
                            // ȡ"R-UIM door removed" �ַ���
                            (void) ISHELL_LoadResString( pMe->a.m_pIShell,
                                                         AEE_COREAPPRES_LANGFILE,
                                                         IDS_RUIM_DOOR_REMOVED,
                                                         wszMsgText,
                                                         sizeof(wszMsgText));
                                
                            // ׷���������з�ʵ��һ������
                            (void)WSTRCAT(wszMsgText, wszNewLine);
                            (void)WSTRCAT(wszMsgText, wszNewLine);
                
                            nLen = WSTRLEN(wszMsgText);
                
                            // ȡ"�绰����" �ַ���
                            (void) ISHELL_LoadResString( pMe->a.m_pIShell,
                                                         AEE_COREAPPRES_LANGFILE,
                                                         IDS_RESET_1,
                                                         &wszMsgText[nLen],
                                                         (128 - nLen) * sizeof(AECHAR));
                                
                            // �ӻ��з�
                            (void)WSTRCAT(wszMsgText, wszNewLine);
                        
                            nLen = WSTRLEN(wszMsgText);
                        
                            // ȡ"��������..." �ַ���
                            (void) ISHELL_LoadResString( pMe->a.m_pIShell,
                                                         AEE_COREAPPRES_LANGFILE,
                                                         IDS_RESET_2,
                                                         &wszMsgText[nLen],
                                                         (128 - nLen) * sizeof(AECHAR));
                        }    
                        break;
                       
#endif  /* FEATURE_UIM */                              
                    case POWERDOWN_NORMAL:
                        if(pMe->m_wStartupAniTime == 0)
                        {  
                            // �ڴ˲��Źػ�����
                            uint16    aRing_type[PROFILENUMBER]  = {0} ;                                          // CFGI_PROFILE_STARTUP_MUSIC
                            byte      Ring_Cur_Music                      =  OEMNV_PROFILE_NORMALMODE;
                            byte      poweronoff_alert[PROFILENUMBER] =  {0};			

                            ICONFIG_GetItem( pMe->m_pConfig, CFGI_PROFILE_CUR_NUMBER, &Ring_Cur_Music, sizeof(Ring_Cur_Music) ); 
                            ICONFIG_GetItem(pMe->m_pConfig,CFGI_PROFILE_POWERONOFF_ALERT,poweronoff_alert,sizeof(poweronoff_alert));
                            if(poweronoff_alert[Ring_Cur_Music]  == OEMNV_POWERONOFF_ENABLE)
                            {
                                ICONFIG_GetItem( pMe->m_pConfig, CFGI_PROFILE_SHUTDOWN_MUSIC, aRing_type, sizeof(aRing_type) );
                                //DBGPRINTF("fj music_num = %d , Ring_Cur_Music = %d, IDD_POWERDOWN_Handler",(uint32)aRing_type[Ring_Cur_Music], Ring_Cur_Music);
                                IALERT_StartRingerAlert_Ex( pMe->m_pAlert, (uint32)aRing_type[Ring_Cur_Music] );
                            }
                            
                            //pMe->m_wStartupAniTime = 0; //  ���Ʋ��Ŵ���
#ifndef FEATURE_USES_LOWMEM
                            if ( NULL != pMe->m_pStartupAniImg )
#endif
                            {     
                                CoreApp_PlayPwrOffAni(pMe);
                            }
                            //else
                            //{ 
                            //    (void)ISHELL_PostEvent( pMe->a.m_pIShell, 
                            //                            AEECLSID_CORE_APP, 
                            //                            EVT_DISPLAYDIALOGTIMEOUT, 
                            //                            0, 0);
                            //}
                        }
                        else
                        {
#ifndef FEATURE_USES_LOWMEM
                            if ( NULL != pMe->m_pStartupAniImg )
                            {     
                                IIMAGE_Stop(pMe->m_pStartupAniImg);
                                IIMAGE_Release(pMe->m_pStartupAniImg);
                                pMe->m_pStartupAniImg = NULL;
                            }
#endif
                            (void)ISHELL_PostEvent( pMe->a.m_pIShell,  AEECLSID_CORE_APP, EVT_DISPLAYDIALOGTIMEOUT,  0, 0);
                        }
                    
                        return TRUE;
                        
                    case POWERDOWN_RESET:
                        // ȡ"�绰����" �ַ���
                        (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                AEE_COREAPPRES_LANGFILE,
                                IDS_RESET_1,
                                wszMsgText,
                                sizeof(wszMsgText));
                                
                        // �ӻ��з�
                        (void)WSTRCAT(wszMsgText, wszNewLine);
                        
                        nLen = WSTRLEN(wszMsgText);
                        
                        // ȡ"��������..." �ַ���
                        (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                AEE_COREAPPRES_LANGFILE,
                                IDS_RESET_2,
                                &wszMsgText[nLen],
                                (128 - nLen) * sizeof(AECHAR));
                        break;
                        
                    default:
                        break;
                }
                if(POWERDOWN_NORMAL != pMe->m_ePowerDownType)
                {
                    (void) IDISPLAY_DrawText(pMe->m_pDisplay,
                                    AEE_FONT_NORMAL,
                                    wszMsgText,  -1,  0,  0,  &pMe->m_rc,
                                    IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE);
                }
                // ����������Ļ                        
                //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            }            

            // �����Զ��رնԻ���Ķ�ʱ��
//            (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
//                                  RUIM_DOOR_OPEN_TIMER,
//                                  pMe);
            return TRUE;
                    
        case EVT_DIALOG_END:
//            (void) ISHELL_CancelTimer(pMe->a.m_pIShell,
//                                      pMe);
            //IALERT_StopRingerAlert(pMe->m_pAlert);
            return TRUE;

        case EVT_DISPLAYDIALOGTIMEOUT:
        {
            IALERT_StopRingerAlert(pMe->m_pAlert);
            CoreApp_Poweroff_Phone(pMe);
            CLOSE_DIALOG(DLGRET_OK);
            return TRUE;
        }
          
        default:
            break;
    }
    
    return FALSE;
} // IDD_POWERDOWN_Handler

/*==============================================================================
����:
    CoreApp_UpdateIdleTimer

˵��:
    ����ÿ���Ӹ�������ʱ����ʾ�Ķ�ʱ��������

����:
    pUser [in]:���������ָ�� Core Applet ����ṹ��ָ�롣

����ֵ:
    none

��ע:
       
==============================================================================*/
static void CoreApp_UpdateIdleTimer(void *pUser)
{
    CCoreApp    *pMe = (CCoreApp *)pUser;
    uint32         dwSeconds;

    if (pMe == NULL) 
    {
        return;
    }
    
    // �������ں�ʱ����ʾ
    ISHELL_PostEventEx(pMe->a.m_pIShell, 
                       EVTFLG_ASYNC, 
                       AEECLSID_CORE_APP,
                       EVT_UPDATEIDLE,
                       0,0L);
    
    // �����´ζ�ʱ����ʱʱ�䣬ʱ�価���ܿ�����һ���ӣ�������ʾʱ�侫ȷ
    dwSeconds = GETTIMESECONDS();
    
    if (FALSE == pMe->m_bAcquiredTime) 
    {
        // 20x365x24x60x60 = 630720000
        if (dwSeconds > 630720000) 
        {
            pMe->m_bAcquiredTime = TRUE;
        }
        else
        {
            // ������Ӷ�ʱ��, ϵͳû����Чʱ��ʱ����� 5 ��
            (void) ISHELL_SetTimer(pMe->a.m_pIShell,
                    (int32)(5 * 1000),
                    CoreApp_UpdateIdleTimer,
                    pMe);
                    
            return;
        }
    }
    
    if (dwSeconds > 0) 
    {
        dwSeconds %= 60;
        dwSeconds = (60 - dwSeconds);
    }
     
    if (dwSeconds == 0) 
    {
        dwSeconds = 60;
    }
    
    // ������Ӷ�ʱ��
    (void) ISHELL_SetTimer(pMe->a.m_pIShell,
                           (int32)(dwSeconds * 1000),
                           CoreApp_UpdateIdleTimer,
                           pMe);
}
 /*==============================================================================
����:
    CoreApp_TimeKeyguard

˵��:
    ���� ��ʱ�����̵ĺ���

����:
    pUser [in]:���������ָ�� Core Applet ����ṹ��ָ�롣

����ֵ:
    none

��ע:
       
==============================================================================*/
#ifdef FEATURE_KEYGUARD
static void CoreApp_TimeKeyguard(void *pUser)
{
    CCoreApp    *pMe = (CCoreApp *)pUser;
    MSG_FATAL("CoreApp_TimeKeyguard Start",0,0,0);
    pMe->m_b_set_lock = TRUE;
    if(pMe->m_b_set_lock)
    {
        OEMKeyguard_SetState(TRUE);
        CoreApp_UpdateBottomBar(pMe);
        //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
        pMe->m_b_set_lock = FALSE;
    }
    MSG_FATAL("CoreApp_TimeKeyguard End",0,0,0);
}
#endif
/*==============================================================================
����:
    CoreApp_SearchingTimer

˵��:
    ����ÿ���Ӹ�������ʱ����ʾ�Ķ�ʱ��������

����:
    pUser [in]:���������ָ�� Core Applet ����ṹ��ָ�롣

����ֵ:
    none

��ע:
       
==============================================================================*/
static void CoreApp_SearchingTimer(void *pUser)
{
    CCoreApp  *pMe = (CCoreApp *)pUser;

    if (( pMe == NULL ) || (IDD_IDLE != pMe->m_wActiveDlgID))
    {
        return;
    }

    ISHELL_PostEventEx(pMe->a.m_pIShell, 
                       EVTFLG_ASYNC, 
                       AEECLSID_CORE_APP,
                       EVT_UPDATEIDLE,
                       0,0L);

    (void)ISHELL_SetTimer(pMe->a.m_pIShell,
                          IDLESEARCHINTIME,
                          CoreApp_SearchingTimer,
                          pMe);
}

/*==============================================================================
����:
    CoreApp_DrawBannerMessage

˵��:
    ��������SPN�������ʺ��������ı����ƴ���

����:
    pMe [In]: ָ�� Core Applet ����ṹ��ָ�롣
       
����ֵ:
    �ޡ�

��ע:
    ˢ����ʾ�����ɵ����߸�����Ҫִ����ز�����
    
==============================================================================*/
static void CoreApp_DrawBannerMessage(CCoreApp    *pMe)
{
    AEERect   rc;
    AECHAR    *wszBuf = NULL;
    int32     nSize;
    //AEECMSSInfo  *pssinfo = NULL;
    boolean   bSetsearchingTimer = FALSE;
    
    nSize = 32 * sizeof(AECHAR);
    wszBuf = (AECHAR *)MALLOC(nSize);
    if (NULL == wszBuf)
    {
        return;
    }
    
    // ��ȡ����ض�ʱ��
    (void) ISHELL_CancelTimer(pMe->a.m_pIShell,
                              CoreApp_SearchingTimer,
                              pMe);

    // Determine displaying rectangle     
	SETAEERECT(&rc, RPLMN_X, RPLMN_Y, pMe->m_rc.dx-2*RPLMN_X, pMe->m_nLargeFontHeight);
    wszBuf[0] = 0;
    
#ifdef FEATURE_PLANEMODE    
    if(pMe->m_SYS_MODE_NO_SRV || pMe->bPlaneModeOn == TRUE)
#else
	if(pMe->m_SYS_MODE_NO_SRV)
#endif
    {// ������ʾ����
#ifdef FEATURE_PLANEMODE 
        byte planeMode_cfg;
        (void) ICONFIG_GetItem(pMe->m_pConfig,
                               CFGI_PLANEMODE,
                               &planeMode_cfg,
                               sizeof(planeMode_cfg));
        //OEMNV_PLANEMODE_QUERY == bytData ά�ֵ�ǰbPlaneModeOn��ֵ
        if(OEMNV_PLANEMODE_OFF== planeMode_cfg)
        {
            pMe->bPlaneModeOn = FALSE;
        }
        else if(OEMNV_PLANEMODE_ON == planeMode_cfg)
        {
            pMe->bPlaneModeOn = TRUE;
        }
        
        if(TRUE == pMe->bPlaneModeOn)
        {
            (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                        AEE_COREAPPRES_LANGFILE,
                                        IDS_PLANEMODE,
                                        wszBuf,
                                        nSize);
        }
        else
#endif //FEATURE_PLANEMODE            
        {
            // ������ʾ����
            bSetsearchingTimer = TRUE;
            (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                        AEE_COREAPPRES_LANGFILE,
                                        IDS_SEARCHING,
                                        wszBuf,
                                        nSize);
        }
    }
    else if(pMe->m_bemergencymode)
    {
        ISHELL_LoadResString(pMe->a.m_pIShell,
                                    AEE_COREAPPRES_LANGFILE,
                                    IDS_EMERGENCY_MODE,
                                    wszBuf,
                                    nSize);
    }
    else if (IsRunAsFactoryTestMode())
    {// ����ǳ���ģʽ��ʾ
        (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                    AEE_COREAPPRES_LANGFILE,
                                    IDS_FACTORYTESTMODE,
                                    wszBuf,
                                    nSize);
    }
    else
    {// �������������µ���ʾ
        // ��ȡ�����ʺ��� 
        CoreApp_GetSPN(pMe);
        
        if(pMe->svc_p_name[0] != 0)
        {
            WSTRLCPY(wszBuf,pMe->svc_p_name,32);
        }
        else
        {
#ifdef WIN32
            STRTOWSTR("WIN32BUILD", wszBuf, nSize);
#else
            STRTOWSTR(ver_modelname, wszBuf, nSize);
#endif
        }
    }
    // Display the string
    (void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_NORMAL,
                              wszBuf, -1,
                              0, 0, &rc, 
                              IDF_ALIGN_RIGHT 
                              | IDF_ALIGN_MIDDLE 
                              | IDF_TEXT_TRANSPARENT);
    
    if (bSetsearchingTimer)
    {
        (void)ISHELL_SetTimer(pMe->a.m_pIShell,
                              IDLESEARCHINTIME,
                              CoreApp_SearchingTimer,
                              pMe);
    }
    
    // �ͷŶ�̬������ڴ�
    FREE(wszBuf);
    //FREE(pssinfo);
}

/*==============================================================================
����:
    CoreApp_UpdateDateTime

˵��:
    ���ڸ�������ʱ������ڼ�����ʾ��

����:
    pMe [In]: ָ�� Core Applet ����ṹ��ָ�롣

����ֵ:
    �ޡ�

��ע:
    ˢ����ʾ�����ɵ����߸�����Ҫִ����ز�����
==============================================================================*/
static void CoreApp_UpdateDateTime(CCoreApp    *pMe)
{
    AECHAR      wszDate[64]= {0};
    AECHAR      wFormat[64]= {0};
    JulianType  jDate;
    AEERect     rc;
    AEERect     rc_date;
    AEERect     rc_week;
    //AECHAR      wszBuf[16]= {(AECHAR)'\0'}; //fj remark ,Define but no used
#ifndef FEATURE_CARRIER_SUDAN_SUDATEL
    uint16   nWeekResID;
    //AEEDeviceInfo di;
#endif /*FEATURE_CARRIER_SUDAN_SUDATEL*/
    byte      bTFmt;
    
    if ( pMe == NULL ) 
    {
        return;
    }
    
    // ������������֮һ��������������
    if(pMe->m_bSuspended)
    {
        return;
    }

    CoreApp_Process_Rtc_Event(pMe);
    (void) ICONFIG_GetItem( pMe->m_pConfig,
                           CFGI_TIME_FORMAT,
                           &bTFmt,
                           sizeof(bTFmt));

    // ȡ��ǰ����
    if(pMe->m_bAcquiredTime)
    {
        GetJulianDate(GETTIMESECONDS(), &jDate);
    }
    else
    {

#if (defined(FEATURE_PM1000_RTC) || defined(FEATURE_PMIC_RTC))
        pm_rtc_julian_type time = {0};
        pm_rtc_rw_cmd(PM_RTC_GET_CMD, &time);
        jDate.wSecond = (uint16)time.second&0xFFFF;
        jDate.wMinute = (uint16)time.minute&0xFFFF;
        jDate.wHour = (uint16)time.hour&0xFFFF;
        jDate.wDay = (uint16)time.day&0xFFF;
        jDate.wMonth = (uint16)time.month&0xFFFF;
        jDate.wYear = (uint16)time.year&0xFFFF;
        jDate.wWeekDay = (uint16)time.day_of_week&0xFFFF;
#else
        jDate.wYear = 1980;
        jDate.wMonth = 1;
        jDate.wDay =6;
        jDate.wHour = 0;
        jDate.wMinute =0;
        jDate.wSecond = 0;
        jDate.wWeekDay = 6;
#endif
    }
#ifndef FEATURE_CARRIER_SUDAN_SUDATEL
    // ȷ������������Դ�ַ���
    switch (jDate.wWeekDay)
    {
        case 0: // ����һ
            nWeekResID = AEE_IDS_MONDAY;
            break;
            
        case 1: // ���ڶ�
            nWeekResID = AEE_IDS_TUESDAY;
            break;

        case 2: // ������
            nWeekResID = AEE_IDS_WEDNESDAY;
            break;

        case 3: // ������
            nWeekResID = AEE_IDS_THURSDAY;
            break;

        case 4: // ������
            nWeekResID = AEE_IDS_FRIDAY;
            break;

        case 5: // ������
            nWeekResID = AEE_IDS_SATURDAY;
            break;

        default:  // 6:������
            nWeekResID = AEE_IDS_SUNDAY;
            break;
    }
#endif /*FEATURE_CARRIER_SUDAN_SUDATEL*/

#ifdef FEATRUE_SET_ANN_FULL_SCREEN
	SETAEERECT(&rc, 
			   IDLE_D_CLOCK_X,
			   IDLE_D_CLOCK_Y,
			   pMe->m_rc.dx-2*IDLE_D_CLOCK_X, 
			   26);
	SETAEERECT(&rc_date, 
			   DATA_X,
			   DATA_Y,
			   (pMe->m_rc.dx-2*DATA_X), 
			   pMe->m_nNormalFontHeight);
#if defined(FEATURE_DISP_160X128)
	SETAEERECT(&rc_week, 
		   WEEK_X,
		   WEEK_Y,
		   (pMe->m_rc.dx-2*WEEK_X), 
		   pMe->m_nNormalFontHeight);
#endif
#else
    SETAEERECT(&rc, 
               1,
               1, 
               pMe->m_rc.dx-2, 
               pMe->m_nNormalFontHeight);
    SETAEERECT(&rc_date, 
               (pMe->m_rc.dx-10)/2 ,
               1, 
               pMe->m_rc.dx-(pMe->m_rc.dx-10)/2, 
               pMe->m_nNormalFontHeight);

	SETAEERECT(&rc_week, 
		   1,
		   1,
		   (pMe->m_rc.dx-2*DATA_X), 
               pMe->m_nNormalFontHeight);
#endif
    // ��ʽ��ʱ���ַ���������
    wFormat[0] = 0;
    wszDate[0] = 0;
    (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                AEE_COREAPPRES_LANGFILE,
                                IDS_TIME_TILE,
                                wFormat,
                                sizeof(wFormat));
    if (bTFmt == OEMNV_TIMEFORM_AMPM)
    {        
        uint16    wHour,len;
                                     
        wHour = jDate.wHour > 12 ? (jDate.wHour - 12) : jDate.wHour;
        if(jDate.wHour == 0)
        {
            wHour = 12;
        }
        WSPRINTF(wszDate, sizeof(wszDate), wFormat, wHour, jDate.wMinute);
        len = WSTRLEN(wszDate);
        if(jDate.wHour >= 12)
        {
            wszDate[len] = (AECHAR)'P';
            wszDate[len+1] = (AECHAR)'M';
            wszDate[len+2] = 0;
        }
        else
        {
            wszDate[len] = (AECHAR)'A';
            wszDate[len+1] = (AECHAR)'M';
            wszDate[len+2] = 0;
        }
    }
    else
    {
        WSPRINTF(wszDate, sizeof(wszDate), wFormat, jDate.wHour, jDate.wMinute);
    }
	//IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test               
    // Display the string of date or time or weekday
	#if 1
	{
		DrawGreyBitTextWithProfile(pMe->a.m_pIShell,
	                              pMe->m_pDisplay,
	                              RGB_WHITE_NO_TRANS,
	                              16,
	                              wszDate, -1,
	                              0, 0, &rc, 
	                              IDF_ALIGN_MIDDLE
	                              | IDF_ALIGN_LEFT
	                              | IDF_TEXT_TRANSPARENT);
        //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
	}

	#else
    (void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_BOLD,
                              wszDate, -1,
                              0, 0, &rc, 
                              IDF_ALIGN_MIDDLE
                              | IDF_ALIGN_CENTER
                              | IDF_TEXT_TRANSPARENT);
	#endif
        
 	//IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);//wlh test  
    // ��ʽ�������ַ���������
    wszDate[0] = (AECHAR)'\0';
    wFormat[0] = (AECHAR)'\0';
#ifdef FEATURE_TIME_DATA_SETTING
    {
        //uint16 m_year = 0;
        ICONFIG_GetItem( pMe->m_pConfig, CFGI_DATE_FORMAT,&bTFmt, sizeof(bTFmt));

        //m_year = jDate.wYear -2000;
        switch(bTFmt)
        {
            case OEMNV_DATEFORM_DMY:
#if defined(FEATURE_DISP_176X220)
				WSTRLCPY(wFormat,L"%02d/%02d/%04d",63);
#elif defined(FEATURE_DISP_128X128)
                WSTRLCPY(wFormat,L"%02d/%02d/%04d",63);
#elif defined(FEATURE_DISP_160X128)
				WSTRLCPY(wFormat,L"%02d/%02d/%04d",63);
#else
                WSTRLCPY(wFormat,L"%02d/%02d",63);
#endif //FEATURE_DISP_176X220
                WSPRINTF(wszDate, sizeof(wszDate), wFormat, jDate.wDay, jDate.wMonth, jDate.wYear);
                break;
                
            case OEMNV_DATEFORM_MDY:
#if defined(FEATURE_DISP_176X220)
				WSTRLCPY(wFormat,L"%02d/%02d/%04d",63);
#elif defined(FEATURE_DISP_128X128)
                WSTRLCPY(wFormat,L"%02d/%02d/%04d",63);
#elif defined(FEATURE_DISP_160X128)
				WSTRLCPY(wFormat,L"%02d/%02d/%04d",63);
#else
                WSTRLCPY(wFormat,L"%02d/%02d",63);
#endif //FEATURE_DISP_176X220
                WSPRINTF(wszDate, sizeof(wszDate), wFormat, jDate.wMonth, jDate.wDay, jDate.wYear);
                break;
                
            default:
            case OEMNV_DATEFORM_YMD:
#if defined(FEATURE_DISP_176X220)
				WSTRLCPY(wFormat,L"%04d/%02d/%02d",63);
#elif defined(FEATURE_DISP_128X128)
                WSTRLCPY(wFormat,L"%04d/%02d/%02d",63);
#elif defined(FEATURE_DISP_160X128)
				WSTRLCPY(wFormat,L"%04d/%02d/%02d",63);
#else
                WSTRLCPY(wFormat,L"%02d/%02d",63);
#endif //FEATURE_DISP_176X220
                WSPRINTF(wszDate, sizeof(wszDate), wFormat,  jDate.wYear, jDate.wMonth,jDate.wDay);
                break;
        }
#if defined(FEATURE_DISP_176X220)
		DrawTextWithProfile(pMe->a.m_pIShell,
                                  pMe->m_pDisplay,
                                  RGB_WHITE_NO_TRANS,
                                  AEE_FONT_NORMAL,
                                  &wszDate[0], -1,
                                  0, 0, &rc_date, 
                                  IDF_ALIGN_MIDDLE
                                  | IDF_ALIGN_LEFT
                                  | IDF_TEXT_TRANSPARENT);
#elif defined(FEATURE_DISP_128X128)
        DrawTextWithProfile(pMe->a.m_pIShell,
                                  pMe->m_pDisplay,
                                  RGB_WHITE_NO_TRANS,
                                  AEE_FONT_NORMAL,
                                  &wszDate[0], -1,
                                  0, 0, &rc_date, 
                                  IDF_ALIGN_MIDDLE
                                  | IDF_ALIGN_LEFT
                                  | IDF_TEXT_TRANSPARENT); 		
#elif defined(FEATURE_DISP_160X128)
        DrawGreyBitTextWithProfile(pMe->a.m_pIShell,
                                  pMe->m_pDisplay,
                                  RGB_WHITE_NO_TRANS,
                                  10,
                                  &wszDate[0], -1,
                                  0, 0, &rc_date, 
                                  IDF_ALIGN_MIDDLE
                                  | IDF_ALIGN_RIGHT
                                  | IDF_TEXT_TRANSPARENT); 

#else
        DrawTextWithProfile(pMe->a.m_pIShell,
                                  pMe->m_pDisplay,
                                  RGB_WHITE_NO_TRANS,
                                  AEE_FONT_NORMAL,
                                  &wszDate[0], -1,
                                  0, 0, &rc, 
                                  IDF_ALIGN_MIDDLE
                                  | IDF_ALIGN_RIGHT
                                  | IDF_TEXT_TRANSPARENT); 
#endif //FEATURE_DISP_176X220
    }
#else /*FEATURE_TIME_DATA_SETTING*/
    //Get date format     
    (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                AEE_COREAPPRES_LANGFILE,
                                IDS_MONTH_DATE,
                                wFormat,
                                sizeof(wFormat));

    WSPRINTF(wszDate, sizeof(wszDate), wFormat,jDate.wYear, jDate.wDay, jDate.wMonth);
#if defined(FEATURE_DISP_176X220)
    (void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_NORMAL,
                              &wszDate[5], -1,
                              0, 0, &rc_date, 
                              IDF_ALIGN_MIDDLE
                              | IDF_ALIGN_LEFT
                              | IDF_TEXT_TRANSPARENT);  
#elif defined(FEATURE_DISP_128X128)
	(void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_NORMAL,
                              &wszDate[5], -1,
                              0, 0, &rc_date, 
                              IDF_ALIGN_MIDDLE
                              | IDF_ALIGN_LEFT
                              | IDF_TEXT_TRANSPARENT);
#elif defined(FEATURE_DISP_160X128)
	(void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_NORMAL,
                              &wszDate[5], -1,
                              0, 0, &rc_date, 
                              IDF_ALIGN_MIDDLE
                              | IDF_ALIGN_LEFT
                              | IDF_TEXT_TRANSPARENT);

#else
    (void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_NORMAL,
                              &wszDate[5], -1,
                              0, 0, &rc, 
                              IDF_ALIGN_MIDDLE
                              | IDF_ALIGN_RIGHT
                              | IDF_TEXT_TRANSPARENT);  
#endif //FEATURE_DISP_176X220

#endif /*FEATURE_TIME_DATA_SETTING*/

#ifndef FEATURE_CARRIER_SUDAN_SUDATEL
    // ���������ַ���
    wszDate[0] = 0;
    wFormat[0] = 0;
    
    (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                OEMAEECONTROLS_LNGRES_FILE,
                                nWeekResID,
                                wszDate,
                                sizeof(wszDate));
    
    wszDate[3] = (AECHAR)'\0';
    (void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_NORMAL,
                              wszDate, -1,
                              0, 0, &rc_week, 
                              IDF_ALIGN_MIDDLE
#if defined(FEATURE_DISP_176X220)
							  | IDF_ALIGN_RIGHT
#elif defined(FEATURE_DISP_128X128)
                              | IDF_ALIGN_RIGHT
#elif defined(FEATURE_DISP_160X128)
							  | IDF_ALIGN_RIGHT
#else
                              | IDF_ALIGN_LEFT
#endif //FEATURE_DISP_176X220
                              | IDF_TEXT_TRANSPARENT);
#endif /*FEATURE_CARRIER_SUDAN_SUDATEL*/
            
} // CoreApp_UpdateDateTime

// ���ƴ�������� "Menu         Contacts"
static void CoreApp_UpdateBottomBar(CCoreApp    *pMe)
{
    BottomBar_e_Type    eBBarType = BTBAR_NONE;

    
    if(pMe->m_bemergencymode)
    {
        eBBarType = BTBAR_BACK;
    }
#ifdef FEATURE_KEYGUARD
    else if(OEMKeyguard_IsEnabled())
    {
        eBBarType = BTBAR_UNLOCK_SOS;
    }
#endif
    else
    {
#ifdef FEATURE_VERSION_SMART			//Add By zzg 2010_07_20
		eBBarType = BTBAR_MENU_FRENDUO;
#else	
	#ifndef FEATURE_FLEXI_STATIC_BREW_APP
			eBBarType = BTBAR_MENU_CONTACTS;
	#elif defined STATIC_BREW_APP_FOR_NASRANI_NOR_MUSLIM
		eBBarType = BTBAR_FNASRANI_FPORTAL;
	#else
		eBBarType = BTBAR_FMUSLIM_FPORTAL;
	#endif

#endif
	
    }

    DrawBottomBar_Ex(pMe->a.m_pIShell, pMe->m_pDisplay,eBBarType);
}

/*==============================================================================
����     : CoreApp_PlayPwrOnAni

˵��     :  ���ػ������п��ܲ�һ�������ǵĺ�������ϣ�
                   ���ػ��������ŷֱ��ò�ͬ�ĺ���

����     :  pMe [in]: ָ�� Core Applet ����ṹ��ָ�롣

����     :  �� 

��ע     :  WHEN                    WHO                WHAT                     WHY
           2008-02-17              fj                   V1.6                       New Dev, for add play poweroff animation
==============================================================================*/
static void CoreApp_PlayPwrOnAni(CCoreApp *pMe) 
{
    AEEImageInfo  ImgInfo;  //Gets the information about an image

    ASSERT(pMe != NULL);
    MSG_FATAL("%x %x  CoreApp_PlayPwrOnAni",pMe->m_pStartupAniImg,pMe->m_wStartupAniTime,0);
#ifndef FEATURE_USES_LOWMEM
    if ( (NULL != pMe->m_pStartupAniImg) && (pMe->m_wStartupAniTime < 1)  )
#else
    if (pMe->m_wStartupAniTime < PWRON_ANI_FRAME_COUNT )
#endif
    {
#ifndef FEATURE_USES_LOWMEM
        IIMAGE_GetInfo( pMe->m_pStartupAniImg, &ImgInfo );

        // ���ö����ٶ�(����)
        IIMAGE_SetAnimationRate(pMe->m_pStartupAniImg, PWRON_ANI_RATE);

        // ����ͼ���е�֡��
        IIMAGE_SetFrameCount(pMe->m_pStartupAniImg, PWRON_ANI_FRAME_COUNT);

        // ����Ҫ��ʾ��ͼ���ʵ�ʴ�С
        IIMAGE_SetDrawSize( pMe->m_pStartupAniImg, ImgInfo.cx/PWRON_ANI_FRAME_COUNT, ImgInfo.cy );

        // ��ʼ���ſ�������
        IIMAGE_Start( pMe->m_pStartupAniImg,
                            (pMe->m_rc.dx - ImgInfo.cx/PWRON_ANI_FRAME_COUNT)/2,
                            (pMe->m_rc.dy - ImgInfo.cy)/2 );

        pMe->m_wStartupAniTime++; // �������Ŵ���
        //AEE_SetSysTimer( PWRON_ANI_TIME,  (PFNNOTIFY)CoreApp_PlayPwrOnAni,  (void*)pMe);
       (void) ISHELL_SetTimer(pMe->a.m_pIShell,
                             PWRON_ANI_TIME,
                             (PFNNOTIFY)CoreApp_PlayPwrOnAni,
                             (void*)pMe);
#else
        {
            #define PWRON_STR L"Welcome"
            extern int GreyBitBrewFont_DrawText(IDisplay *p, int nSize, const AECHAR *psz, int nl, int x, int y, const AEERect *prcb, uint32 flags);
            IDISPLAY_ClearScreen(pMe->m_pDisplay);
            GreyBitBrewFont_DrawText(pMe->m_pDisplay, 
                                     8+2*pMe->m_wStartupAniTime,
                                     PWRON_STR,
                                     -1,
                                     0,0,
                                     NULL,
                                     IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE);
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
        }
        pMe->m_wStartupAniTime++; // �������Ŵ���
        //AEE_SetSysTimer( PWRON_ANI_TIME,  (PFNNOTIFY)CoreApp_PlayPwrOnAni,  (void*)pMe);
       (void) ISHELL_SetTimer(pMe->a.m_pIShell,
                             ANI_RATE,
                             (PFNNOTIFY)CoreApp_PlayPwrOnAni,
                             (void*)pMe);
#endif
    }
    else
    {
        IBACKLIGHT_Enable(pMe->m_pBacklight);
        IALERT_StopRingerAlert(pMe->m_pAlert);
#ifndef FEATURE_USES_LOWMEM
        if ( NULL != pMe->m_pStartupAniImg )
        {     
            IIMAGE_Stop(pMe->m_pStartupAniImg);
            IIMAGE_Release(pMe->m_pStartupAniImg);
            pMe->m_pStartupAniImg = NULL;
        }
#endif
        CLOSE_DIALOG(DLGRET_OK)
    }
    
}

/*==============================================================================
����     : CoreApp_PlayPwrOffAni

˵��     :  ���ڲ��Źػ����������ػ������п��ܲ�һ�������ǵĺ�������ϣ�
                   ���ػ��������ŷֱ��ò�ͬ�ĺ���

����     :  pMe [in]: ָ�� Core Applet ����ṹ��ָ�롣

����     :  �� 

��ע     :  WHEN                    WHO                WHAT                     WHY
           2008-02-17              fj                  V1.6                     New Dev, for add play poweroff animation
==============================================================================*/
static void CoreApp_PlayPwrOffAni(CCoreApp *pMe) 
{
    AEEImageInfo  ImgInfo;  //Gets the information about an image

    ASSERT(pMe != NULL);
#ifndef FEATURE_USES_LOWMEM
    if ( (NULL != pMe->m_pStartupAniImg) && (pMe->m_wStartupAniTime < 1)  ) 
#else
    if (pMe->m_wStartupAniTime < PWRON_ANI_FRAME_COUNT) 
#endif
    {
#ifndef FEATURE_USES_LOWMEM
        IIMAGE_GetInfo( pMe->m_pStartupAniImg, &ImgInfo );

        // ���ö����ٶ�(����)
        IIMAGE_SetAnimationRate(pMe->m_pStartupAniImg, PWROFF_ANI_RATE);

        // ����ͼ���е�֡��
        IIMAGE_SetFrameCount(pMe->m_pStartupAniImg, PWROFF_ANI_FRAME_COUNT);

        // ����Ҫ��ʾ��ͼ���ʵ�ʴ�С
        IIMAGE_SetDrawSize( pMe->m_pStartupAniImg, 
                                ImgInfo.cx/PWROFF_ANI_FRAME_COUNT, ImgInfo.cy );

        // ��ʼ���ſ�������
        IIMAGE_Start( pMe->m_pStartupAniImg, 
                                (pMe->m_rc.dx - ImgInfo.cx/PWROFF_ANI_FRAME_COUNT)/2, 
                                (pMe->m_rc.dy - ImgInfo.cy)/2 );
        
        pMe->m_wStartupAniTime++; // �������Ŵ���
        AEE_SetSysTimer( PWRON_ANI_TIME,  (PFNNOTIFY)CoreApp_PlayPwrOffAni,  (void*)pMe);
        //(void) ISHELL_SetTimer(pMe->a.m_pIShell,
        //                      PWROFF_ANI_TIME,
        //                      (PFNNOTIFY)CoreApp_PlayPwrOffAni,
        //                      (void*)pMe);
#else
        {
            #define PWROFF_STR L"Bye-Bye"
            extern int GreyBitBrewFont_DrawText(IDisplay *p, int nSize, const AECHAR *psz, int nl, int x, int y, const AEERect *prcb, uint32 flags);
            IDISPLAY_ClearScreen(pMe->m_pDisplay);
            GreyBitBrewFont_DrawText(pMe->m_pDisplay, 
                                     8+2*(PWRON_ANI_FRAME_COUNT-pMe->m_wStartupAniTime),
                                     PWROFF_STR,
                                     -1,
                                     0,0,
                                     NULL,
                                     IDF_ALIGN_CENTER|IDF_ALIGN_MIDDLE);
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
        }
        pMe->m_wStartupAniTime++; // �������Ŵ���
        AEE_SetSysTimer( ANI_RATE,  (PFNNOTIFY)CoreApp_PlayPwrOffAni,  (void*)pMe);
#endif
    }
    else
    {
#ifndef FEATURE_USES_LOWMEM
        if ( NULL != pMe->m_pStartupAniImg )
        {     
            IIMAGE_Stop(pMe->m_pStartupAniImg);
            IIMAGE_Release(pMe->m_pStartupAniImg);
            pMe->m_pStartupAniImg = NULL;
        }
#endif
        // �����¼��رտ����������ŶԻ�
        (void)ISHELL_SendEvent( pMe->a.m_pIShell,  AEECLSID_CORE_APP, 
                                EVT_DISPLAYDIALOGTIMEOUT,  0, 0);
    }

}

/*==============================================================================
����      :  CoreApp_DrawWallPaper

˵��      :  �������ڻ���ǽֽ, ����ǽֽ�󲻻����LCD��ˢ��,
                  LCD��ˢ���ɵ����߹���

����      :  pMe [In]: ָ��Idle Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       
����ֵ : �ޡ�

��ע     :  WHEN                    WHO                WHAT                     WHY
           2008-02-20              fj                  V1.6                    New Dev, for Wallpaper
==============================================================================*/
static void CoreApp_DrawWallPaper(CCoreApp *pMe) 
{
    AEEImageInfo   ImgInfo;
    static char        szWallPaperName[AEE_MAX_FILE_NAME/*FILESPECLEN*/];
    char                 szNewWallPaperName[AEE_MAX_FILE_NAME/*FILESPECLEN*/];
    //int                    nX = 0,  nY = 0;
    MSG_FATAL("CoreApp_DrawWallPaper Start",0,0,0);
    if ( (NULL == pMe) || (IDD_IDLE != pMe->m_wActiveDlgID) )
    {
        return;
    }

    MEMSET( &ImgInfo, 0x00, sizeof(ImgInfo) );
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
    pMe->m_capture = DB_CAPTURE_NONE;
#endif
    if ( NULL == pWallPaper )
    {  
        MSG_FATAL("CoreApp_DrawWallPaper NULL == pWallPaper",0,0,0);
        // ��ʼ��ǽֽ���ϴ�ȡǽֽ���ɹ�
        MEMSET(szWallPaperName, 0x00, sizeof(szWallPaperName));
        
        // ȡ�趨��ǽֽ�ļ���
        (void) ICONFIG_GetItem(pMe->m_pConfig,
                                CFGI_WALLPAPER,
                                szWallPaperName,
                                sizeof(szWallPaperName));

        // test the file existed or not, if not existed, load the default wallpaper.
        {
            IFileMgr *pFileMgr = NULL;
            ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_FILEMGR, (void **)&pFileMgr);
            if(pFileMgr)
            {
                if(IFILEMGR_Test(pFileMgr, szWallPaperName) == SUCCESS)
                {
                    pWallPaper = ISHELL_LoadImage(pMe->a.m_pIShell, szWallPaperName);
                    MSG_FATAL("CoreApp_DrawWallPaper szWallPaperName=%s",szWallPaperName,0,0);
                }
                else // if specified wallpaper not existed, load the default wallpaper
                {
                    pWallPaper = ISHELL_LoadImage(pMe->a.m_pIShell, OEMNV_WALLPAPER);
                }
                IFILEMGR_Release(pFileMgr);
                pFileMgr = NULL;
            }
        }
        
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
        pMe->m_capture = DB_CAPTURE_NEED;
#endif
    }
    else
    {   
        MSG_FATAL("CoreApp_DrawWallPaper NULL != pWallPaper",0,0,0);
        // �Ѿ��ɹ�����ǽֽͼƬ��������ǽֽͼƬ�����Ƿ񱻱��
        MEMSET(szNewWallPaperName, 0x00, sizeof(szNewWallPaperName));
        
        // ȡ�趨��ǽֽ�ļ���
        (void) ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_WALLPAPER,
                          szNewWallPaperName,
                          sizeof(szNewWallPaperName));
                          
        if ( 0 != STRCMP(szNewWallPaperName, szWallPaperName) )
        {   
            MSG_FATAL("CoreApp_DrawWallPaper pWallPaper was changed",0,0,0);
            // ǽֽ�����ѱ�
            MEMSET( szWallPaperName, 0x00, sizeof(szWallPaperName) );
            (void)STRCPY( szWallPaperName, szNewWallPaperName );
            
            // �ͷ���ǰͼƬռ�õ���Դ
            IIMAGE_Release(pWallPaper);
            
            // test the file existed or not, if not existed, load the default wallpaper.
            {
                IFileMgr *pFileMgr = NULL;
                ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_FILEMGR, (void **)&pFileMgr);
                if(pFileMgr)
                {
                    if(IFILEMGR_Test(pFileMgr, szWallPaperName) == SUCCESS)
                    {
                        MSG_FATAL("szWallPaperName=%s",szWallPaperName,0,0);
                        pWallPaper = ISHELL_LoadImage(pMe->a.m_pIShell, szWallPaperName);
                    }
                    else // if specified wallpaper not existed, load the default wallpaper
                    {
                        MSG_FATAL("szWallPaperName=%s",OEMNV_WALLPAPER,0,0);
                        pWallPaper = ISHELL_LoadImage(pMe->a.m_pIShell, OEMNV_WALLPAPER);
                    }
                    IFILEMGR_Release(pFileMgr);
                    pFileMgr = NULL;
                }
            }
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
            pMe->m_capture = DB_CAPTURE_NEED;
#endif
        }
    }
#if 0
    if ( NULL != pWallPaper )
    {
        IIMAGE_GetInfo(pWallPaper, &ImgInfo);
        
        nX = (ImgInfo.cx >= pMe->m_rc.dx)?  0 : ( (pMe->m_rc.dx - ImgInfo.cx)/2 );
        //#if !defined(LCD_OPIMIZE_ICON)
        //nY = (ImgInfo.cy >= pMe->m_rc.dy)?  12 : ( (pMe->m_rc.dy - ImgInfo.cy - 12)/2 + 12 );
        //#else   
        nY = (ImgInfo.cy >= pMe->m_rc.dy)?  0 : ( (pMe->m_rc.dy - ImgInfo.cy )/2 );
        //#endif 
        
        IIMAGE_Draw(pWallPaper, nX, nY);
    }
    else
    {
        IDISPLAY_EraseRect(pMe->m_pDisplay, &pMe->m_rc);
    }
#endif
    MSG_FATAL("CoreApp_DrawWallPaper End",0,0,0);
}



/*==============================================================================
������
      CoreApp_LaunchApplet

˵����
       ��������ָ�� Applet��

������
       pMe [in]��ָ��Core Applet����ṹ��ָ�롣�ýṹ����С�����
                          �ض���Ϣ��
       classID [in]��ָ�� Applet �� �� ID �š�
����ֵ��
        TRUE�������ɹ�
        FALSE����ǰ��������������ʧ��

��ע��
       
==============================================================================*/
static boolean CoreApp_LaunchApplet(CCoreApp *pMe,  AEECLSID classID)
{
/*
    int i = ISHELL_CanStartApplet(pMe->a.m_pIShell, classID);
    if (!ISHELL_CanStartApplet(pMe->a.m_pIShell, classID)) 
    {
        // ��ǰ�������� ָ�� Applet
        ERR("Can't start Applet 0x%X    %d",classID,i,0);
        return FALSE;
    }
*/
    if (SUCCESS != ISHELL_StartApplet(pMe->a.m_pIShell, classID)) 
    {
        ERR("Failed to start Applet 0x%X",classID,0,0);
        return FALSE;
    } 

    return TRUE;
}

/*==============================================================================
����:
    CoreApp_UpdateAnnunciator

˵��:
    ���ڸ���Annunciator����ʾ��

����:
    pMe [In]: ָ�� Core Applet ����ṹ��ָ�롣

����ֵ:
    �ޡ�

��ע:
    ˢ����ʾ�����ɵ����߸�����Ҫִ����ز�����
==============================================================================*/
void CoreApp_UpdateAnnunciator(CCoreApp *pMe)
{
    /*Set head set icon*/
    uint32 iann    = 0;
    byte alertType;
    boolean b_FMBackground = FALSE;
    boolean b_headset = FALSE;
    boolean missed_call_icon;
    //static boolean b_flag = TRUE;

    ICONFIG_GetItem(pMe->m_pConfig, CFGI_HEADSET_PRESENT, &b_headset, sizeof(b_headset));
    ICONFIG_GetItem(pMe->m_pConfig, CFGI_FM_BACKGROUND, &b_FMBackground, sizeof(b_FMBackground));
    ICONFIG_GetItem(pMe->m_pConfig, CFGI_PROFILE_CUR_NUMBER,&alertType, sizeof(alertType));//CFGI_ALERT_TYPE
    ICONFIG_GetItem(pMe->m_pConfig, CFGI_MISSED_CALL_ICON,&missed_call_icon, sizeof(missed_call_icon));

    if(pMe->m_pIAnn != NULL)
    {
    if (b_headset)
    {
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_FMRADIO/*ANNUN_FIELD_HEADSET*/, ANNUN_STATE_HEADSET_ON/*ANNUN_STATE_ON*/);
    if (b_FMBackground)
    {
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_FMRADIO, ANNUN_STATE_FMRADIO_ON/*ANNUN_STATE_ON*/);
    }
    }
    else
    {
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_FMRADIO, ANNUN_STATE_FMRADIO_OFF/*ANNUN_STATE_OFF*/);
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_FMRADIO/*ANNUN_FIELD_HEADSET*/, ANNUN_STATE_HEADSET_OFF/*ANNUN_STATE_OFF*/);
    }
    }
    switch(alertType)
    {
        case OEMNV_PROFILE_QUIETMODE:           //OEMNV_ALERTTYPE_OFF :
            iann = ANNUN_STATE_RINGTONE_SILENT;//ANNUN_FIELD_SILENT;
            break;

        case OEMNV_PROFILE_NORMALMODE:         //OEMNV_ALERTTYPE_RINGER :
            iann = ANNUN_STATE_RINGTONE_ALERT;//ANNUN_FIELD_RING;
            break;

        case OEMNV_PROFILE_MEETING:              //OEMNV_ALERTTYPE_VIB :
            iann = ANNUN_STATE_RINGTONE_VIBRATOR;//ANNUN_FIELD_VIBRATE;
            break;

        case OEMNV_PROFILE_NOISEMODE:           //OEMNV_ALERTTYPE_VIBRINGER :
            iann = ANNUN_STATE_RINGTONE_VIBRING;//ANNUN_FIELD_ALERT;
            break;

        case OEMNV_PROFILE_CARMODE:             //OEMNV_ALERTTYPE_VIBANDRINGER :
            iann = ANNUN_STATE_RINGTONE_VIBRING;//ANNUN_FIELD_ALERT;
            break;

        default :
            break;            
    }
    if(pMe->m_pIAnn != NULL)
    {
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, iann);
    }
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
    {
        uint32 state = ANNUN_STATE_OFF;
        uint16 i = 0;
        
        /*
        IANNUNCIATOR_GetField (pMe->m_pIAnn,ANNUN_FIELD_RSSI,&state);
        if(state != ANNUN_STATE_OFF)
        {
            IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RSSI, state);
        }
        */
        // �˴��Ѿ�������ȫ��field��״̬������Ҫ�ٵ���Ϊĳ��field��������
        for (i = ANNUN_FIELD_RSSI; i <=ANNUN_FIELD_BATT; i++) 
        {
            state = ANNUN_STATE_OFF;
            if(pMe->m_pIAnn != NULL)
            {
            IANNUNCIATOR_GetField (pMe->m_pIAnn,i,&state);
            //CORE_ERR("GetField i= %d state= %d",i,state);
            if(state != ANNUN_STATE_OFF)
            {
                IANNUNCIATOR_SetField (pMe->m_pIAnn, i, state);
                }
            }
        }
        /*
        IANNUNCIATOR_GetField (pMe->m_pIAnn,ANNUN_FIELD_BLUETOOTH,&state);
        if(state != ANNUN_STATE_OFF)
        {
            IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BLUETOOTH, state);
        }
        */
    }
#endif
    if((missed_call_icon) && (pMe->m_pIAnn != NULL))
    {
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_CALL, ANNUN_STATE_CALL_MISSEDCALL_ON);
    }
}

static void CoreApp_Process_Rtc_Event(CCoreApp *pMe)
{
    static boolean have_run = FALSE;
    if(!have_run)
    {
        uint32 current_time = 0;
        JulianType current_time_ex;
        current_time = GETTIMESECONDS();
        GETJULIANDATE(current_time, &current_time_ex);
        //DBGPRINTF("%d %d:%d current_time_ex",current_time_ex.wYear,current_time_ex.wMonth,current_time_ex.wDay);
        //DBGPRINTF("%d %d:%d current_time_ex",current_time_ex.wHour,current_time_ex.wMinute,current_time_ex.wSecond);
        if(current_time_ex.wYear < 2008)
        {
            JulianType *get_rtc_time = NULL;
#ifndef WIN32
            get_rtc_time = (JulianType*)OEMRTC_hs_bsp_test_rtc_get();
#endif//WIN32
            if(get_rtc_time->wYear < 2008)
            {
                return ;
            }
        }
        
#ifdef FEATRUE_AUTO_POWER
#ifndef WIN32
        OEMRTC_Process_Auto_Power_Down(pMe->a.m_pIShell);
        OEMRTC_Process_Auto_Power_On();
#endif//WIN32
#endif //#ifdef FEATRUE_AUTO_POWER        
        //jignag 20080213 add
        //OEMRTC_Process_PowerDown_Alarm();

#if defined( FEATURE_POWERDOWN_ALARM)
        registerPowerdownAlarmclock();
#endif
        have_run = TRUE;
    }
    return ;
}

//static  boolean CoreApp_PowerAlarm_Event(CCoreApp *pMe)
//{
//
//        uint32 current_time = 0;
//        JulianType current_time_ex;
//        current_time = GETTIMESECONDS();
//        GETJULIANDATE(current_time, &current_time_ex);
//        if(current_time_ex.wYear < 2008)
//        {
//            JulianType *get_rtc_time = NULL;
//            get_rtc_time = (JulianType*)OEMRTC_hs_bsp_test_rtc_get();
//            if(get_rtc_time->wYear < 2008)
//            {
//                return FALSE;
//            }
//        }
//        OEMRTC_hs_bsp_test_rtc_set((void *)&current_time_ex);
//
//        OEMRTC_Process_PowerDown_Alarm();
//}
#if 0
/*static void CoreApp_GetRecordCount(CCoreApp *pMe)
{
    OEMCallHistoryEntry  ent;
    ICallList      *m_pCallList = NULL;
    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_MISSEDCALL,ANNUN_STATE_OFF);
    //CORE_ERR("Count1");
    if (AEE_SUCCESS == ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_CALLLIST, (void **)&m_pCallList))
    {
        //CORE_ERR("Count2");
        if( ICALLLIST_GetEntry(m_pCallList, CALLHISTORY_MISSED_CATEGORY,0,&ent))
        {
            //CORE_ERR("Count3");
            IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_MISSEDCALL,ANNUN_STATE_ON);
        }
        ICALLLIST_Release(m_pCallList);
        m_pCallList = NULL;
    }
}*/
#endif
void CoreApp_Poweroff_Phone(void *pp)
{
    CCoreApp *pMe = (CCoreApp *)pp;
    IBacklight   *pBacklight = NULL;
    boolean b_FMBackground = FALSE;
    
    EnableUIKeys(FALSE);
    uisnd_snd_stop();
    uisnd_multi_stop();
    uisnd_tone_stop();
    
    ISHELL_CreateInstance(pMe->a.m_pIShell,
                         AEECLSID_BACKLIGHT,
                         (void**)&pBacklight);
    if(pBacklight != NULL)
    {
        IBACKLIGHT_TurnOff(pBacklight);
        IBACKLIGHT_Release(pBacklight);
    }
    // ���ٹ��� IBatt ֪ͨ��Ϣ
    (void) ISHELL_RegisterNotify(pMe->a.m_pIShell,
                AEECLSID_CORE_APP,
                AEECLSID_BATTERYNOTIFIER,
                0);
    
    ICONFIG_SetItem(pMe->m_pConfig, CFGI_FM_BACKGROUND, &b_FMBackground, sizeof(b_FMBackground));

    // Turn off RSSI indicator
    if(pMe->m_pIAnn != NULL)
    {
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RSSI, ANNUN_STATE_OFF);
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BATT, ANNUN_STATE_OFF);
    }
    //IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,FALSE);
    //IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
    ICM_SetOperatingMode(pMe->m_pCM, AEECM_OPRT_MODE_PWROFF);
    CoreTask_SetPwrDnComplete(TRUE);
}

void CoreApp_Draw_Charger_image(void *pp)
{
    CCoreApp *pMe = (CCoreApp *)pp;
    //AEEBatteryChargerStatus status;
    
    //CORE_ERR("%d  %d Charger_image",pMe->m_battery_count,pMe->m_bExtPwrState);
    IDISPLAY_ClearScreen(pMe->m_pDisplay);
    if(pMe->m_battery_count >= CHARGING_FRAME_COUNT)
    {
        pMe->m_battery_count = 0;
    }
#ifndef FEATURE_USES_LOWMEM
    if(NULL != pMe->m_battery_Image)
    {
        IIMAGE_DrawFrame(pMe->m_battery_Image, pMe->m_battery_count, 0, 0);
    }
#else
    CoreApp_DrawChargeing(pMe, pMe->m_battery_count);
#endif
    pMe->m_battery_count ++; 
    ISHELL_SetTimer(pMe->a.m_pIShell, CHARGING_ANIMATE_RATE,CoreApp_Draw_Charger_image, pMe);
}
#ifdef FEATURE_UTK2
/*==============================================================================
������
    CoreApp_CloseRefreshDlgTimer

˵����
    ���ڷ��� EVT_DISPLAYDIALOGTIMEOUT �� AEECLSID_CORE_APP ֪ͨ����ر�
    ��ǰ�Ի���

������
    pme [in]��ָ��CoreApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    �ޡ�

��ע��
    �� Applet �����𣬽������κβ���ֱ�ӷ��ء�
       
==============================================================================*/
static void CoreApp_CloseRefreshDlgTimer(void *pme)
{
    CCoreApp *pMe = (CCoreApp *)pme;
#ifdef FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK
    db_items_value_type  db_item;
    boolean  bIninted = FALSE;
#endif
    if (NULL == pMe)
    {
        return;
    }
    
    if (pMe->m_bSuspended)
    {
        return;
    }
#ifdef FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK
    db_get(DB_UIMSMSINIT, &db_item);
    bIninted = db_item.db_uimsmsinited;
    db_get(DB_UIMADDINIT, &db_item);
    bIninted = (bIninted && db_item.db_uimaddinited);
    if(!bIninted)
    {
        (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                          500,
                          CoreApp_CloseRefreshDlgTimer, 
                          pMe);
        return;
    }
#endif
	// ����EVT_DISPLAYDIALOGTIMEOUT�¼�
    (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                      AEECLSID_CORE_APP,
                      EVT_DISPLAYDIALOGTIMEOUT,
                      0,
                      0);
}

/*==============================================================================
������
    IDD_UTKREFRESH_Handler
       
˵����
    IDD_UTKREFRESH �Ի����¼�������
       
������
    pMe [in]��ָ��CoreApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    eCode [in]���¼����롣
    wParam���¼�������ݡ�
    dwParam���¼�������ݡ�
       
����ֵ��
    TRUE�������¼�������
    FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean  IDD_UTKREFRESH_Handler(void *pUser,
                                   AEEEvent   eCode,
                                   uint16     wParam,
                                   uint32     dwParam)
{
    IStatic * pStatic;    
    CCoreApp *pMe = (CCoreApp *)pUser;    
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    pStatic = (IStatic*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_UTKREFRESH_STATIC);

    if (NULL == pStatic)
    {
        return FALSE;
    }
    
    if (NULL == pStatic)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            // ���þ�̬�ı��ؼ����γߴ�
            ISTATIC_SetRect(pStatic, &pMe->m_rc);
            
            // ���þ�̬�ı��ؼ�����
            ISTATIC_SetProperties(pStatic, ST_CENTERTEXT | ST_MIDDLETEXT);
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                                    AEECLSID_CORE_APP,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
            return TRUE;
            
        case EVT_USER_REDRAW:
            {
                AECHAR    wszMsgText[32];
#ifdef FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK
                db_items_value_type db_item;
#endif
                (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                                            AEE_COREAPPRES_LANGFILE,
                                            IDS_UTKREFRESH,
                                            wszMsgText,
                                            sizeof(wszMsgText));
                
                // ���þ�̬�ı��ؼ��ı�
                (void)ISTATIC_SetText(pStatic, 
                                      NULL, 
                                      wszMsgText, 
                                      AEE_FONT_NORMAL, 
                                      AEE_FONT_NORMAL);

                (void)ISTATIC_Redraw(pStatic);
                
#ifdef FEATURE_CDSMS_RUIM
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
#ifndef WIN32
                // ֪ͨ����ˢ�¿��϶���
                if ((pMe->wRefreshMask == REFRESHMASK_UIMALL) ||
                    (pMe->wRefreshMask & REFRESHMASK_UIMSMS))
                {
                    IWmsApp *pWmsApp=NULL;
#ifdef FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK
                    db_item.db_uimsmsinited = FALSE;
                    db_put(DB_UIMSMSINIT, &db_item);
#endif
                    if (ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_WMSAPP,
                            (void**)&pWmsApp) == SUCCESS)
                    {
                        IWmsApp_RefreshRUIMSMS(pWmsApp);
                        
                        IWmsApp_Release(pWmsApp);
                    }
                }
#endif
#endif
#endif
#ifdef FEATURE_RUIM_PHONEBOOK
                // ��ͨ���ͷŽӿ����ͷų�ʼ���� Buffer
                if ((pMe->wRefreshMask == REFRESHMASK_UIMALL) ||
                    (pMe->wRefreshMask & REFRESHMASK_UIMADD))
                {
                    extern void OEMRUIMAddr_Refresh(void);
#ifdef FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK
                    db_item.db_uimaddinited = FALSE;
                    db_put(DB_UIMADDINIT, &db_item);
#endif
                    OEMRUIMAddr_Refresh();
                }
#endif

                // �����Զ��رնԻ���Ķ�ʱ��
                (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                                      500,
                                      CoreApp_CloseRefreshDlgTimer, 
                                      pMe);
            }
            
            return TRUE;
            
        case EVT_DIALOG_END:
            return TRUE;
            
        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_OK)
            return TRUE;
        
        case EVT_KEY:
            return TRUE;
        
        default:
            break;
    }
    
    return FALSE;
} // IDD_UTKREFRESH_Handler
#endif /* FEATURE_UTK2 */

#ifdef FEATURE_APP_MUSICPLAYER
/*==============================================================================
����:
    CoreApp_DrawMusicName

˵��:
    ���Ʊ������ŵ���������

����:
    pMe [In]: ָ�� Core Applet ����ṹ��ָ�롣
       
����ֵ:
    �ޡ�

��ע:
    ˢ����ʾ�����ɵ����߸�����Ҫִ����ز�����
    
==============================================================================*/
#if 0
static void CoreApp_DrawMusicName(CCoreApp    *pMe)
{
    AEERect rect;
    AECHAR musicName[DISP_MUSICNAME_MAX + 5] = {0};
    int pos = 0;
    int rnt = 0;

    if(NULL == pMe || MP3STATUS_RUNONBACKGROUND != GetMp3PlayerStatus() || NULL ==pMe->m_pMusicName)
    {
        return ;
    }
    musicName[pos++] = (AECHAR)'[';
    musicName[pos++] = (AECHAR)' ';
    //only display first 20 AECHAR
    if(DISP_MUSICNAME_MAX < WSTRLEN(g_pMp3FileToPlay))
    {
        //'[' take place musicName[0] and "..."take three
        rnt = WSTRLCPY(&musicName[pos], g_pMp3FileToPlay, DISP_MUSICNAME_MAX - 3 + 1);
        pos += (DISP_MUSICNAME_MAX - 3);
        musicName[pos++] = (AECHAR)'.';
        musicName[pos++] = (AECHAR)'.';
        musicName[pos++] = (AECHAR)'.';
    }
    else
    {
        rnt = WSTRLCPY(&musicName[pos], g_pMp3FileToPlay, WSTRLEN(g_pMp3FileToPlay) + 1);
        pos += WSTRLEN(g_pMp3FileToPlay);
    }
    musicName[pos++] = (AECHAR)' ';
    musicName[pos++] = (AECHAR)']';
    musicName[pos] = (AECHAR)'\0';
    
    SETAEERECT(&rect, 0, pMe->m_rc.dy *2/5, pMe->m_rc.dx, pMe->m_nLargeFontHeight); 

    // Display the string
    (void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_NORMAL,
                              musicName, -1,
                              0, 0, &rect, 
                              IDF_ALIGN_CENTER 
                              | IDF_ALIGN_MIDDLE 
                              | IDF_TEXT_TRANSPARENT);
}
#else
static void CoreApp_DrawMusicName(CCoreApp    *pMe,uint16 nIdx)
{
    AEERect rect;
    AEERect oldClip;
    AEERect clip;
    AECHAR bracket[2] = {(AECHAR)'[',(AECHAR)'\0'};

    if(NULL == pMe || MP3STATUS_RUNONBACKGROUND != GetMp3PlayerStatus() || NULL == pMe->m_pMusicName)
    {
        return ;
    }
    if(pWallPaper != NULL)
    {
        SETAEERECT(&clip, 0, 58, pMe->m_rc.dx, pMe->m_nLargeFontHeight + 4); 
        IDISPLAY_GetClipRect( pMe->m_pDisplay, &oldClip);
        IDISPLAY_SetClipRect( pMe->m_pDisplay, &clip);

        IIMAGE_SetOffset( pWallPaper, clip.x,clip.y);
        IIMAGE_SetDrawSize( pWallPaper, clip.dx,clip.dy);
        IIMAGE_Draw( pWallPaper, clip.x,clip.y);
        IDISPLAY_SetClipRect( pMe->m_pDisplay,&oldClip);
        IIMAGE_SetOffset( pWallPaper, 0,0);
        IIMAGE_SetDrawSize( pWallPaper, pMe->m_rc.dx,pMe->m_rc.dy);
    }
    SETAEERECT(&rect, pMe->m_rc.dx/8, 60, pMe->m_nLargeFontHeight, pMe->m_nLargeFontHeight);
     IDISPLAY_SetColor(pMe->m_pDisplay,CLR_USER_TEXT,MAKE_RGB(60, 128, 196));
     // Display the string
    (void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_BOLD,
                              bracket, -1,
                              0, 0, &rect, 
                              IDF_ALIGN_LEFT
                              | IDF_ALIGN_MIDDLE 
                              | IDF_TEXT_TRANSPARENT);
   bracket[0]=(AECHAR)']';
   SETAEERECT(&rect, pMe->m_rc.dx*7/8, 60, pMe->m_nLargeFontHeight, pMe->m_nLargeFontHeight);
    // Display the string
   (void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_BOLD,
                              bracket, -1,
                              0, 0, &rect, 
                              IDF_ALIGN_LEFT
                              | IDF_ALIGN_MIDDLE 
                              | IDF_TEXT_TRANSPARENT);
  SETAEERECT(&rect, (pMe->m_rc.dx/8 + pMe->m_nLargeFontHeight), 60,(pMe->m_rc.dx*3/4 - 2*DISP_BLANK_WIDTH), pMe->m_nLargeFontHeight);
  (void)DrawTextWithProfile(pMe->a.m_pIShell,
                              pMe->m_pDisplay,
                              RGB_WHITE_NO_TRANS,
                              AEE_FONT_BOLD,
                              pMe->m_pMusicName+nIdx, -1,
                              0, 0, &rect, 
                              IDF_ALIGN_CENTER
                              | IDF_ALIGN_MIDDLE 
                              | IDF_TEXT_TRANSPARENT);
  IDISPLAY_SetColor(pMe->m_pDisplay,CLR_USER_TEXT,CLR_SYS_TITLE_TEXT);
}
#endif
/*������ʾ��ǰ���ڲ��ŵĸ�������*/
static void CoreApp_MusicNameAutoScroll(CCoreApp *pMe)
{
    int nIdx,nIdxNew,n;
    nIdx  = nIdxNew = pMe->m_nAutoScrollIdx;
    n = WSTRLEN(pMe->m_pMusicName);

    if ((ISHELL_ActiveApplet(pMe->a.m_pIShell) != AEECLSID_CORE_APP) ||
                        (pMe->m_wActiveDlgID != IDD_IDLE))
    {
        ISHELL_CancelTimer(pMe->a.m_pIShell,(PFNNOTIFY)CoreApp_MusicNameAutoScroll,pMe);
        return; 
    }
    if(nIdx < n)
    {
        // Measure the text starting from the auto-scroll offset
        if(Core_bMusicNameRequiredScroll(pMe, nIdx))
        {
            nIdxNew++;
        }
        else
        {
            nIdxNew = 0;
        }
        CoreApp_DrawMusicName(pMe,nIdx);
        IDISPLAY_Update(pMe->m_pDisplay);
        //(void)ISHELL_PostEvent(pMe->a.m_pIShell,
        //                       AEECLSID_CORE_APP,
        //                       EVT_UPDATEIDLE,
        //                       0,
        //                       0);
    }
    else
    {
        nIdxNew = 0;
    }

    pMe->m_nAutoScrollIdx = nIdxNew;

    ISHELL_SetTimer( pMe->a.m_pIShell,
                     MUSICNAME_AUTO_SCROLL_TIME,
                    (PFNNOTIFY)CoreApp_MusicNameAutoScroll, 
                     pMe);
}
/*�ж��Ƿ���Ҫ������ʾ*/
static boolean Core_bMusicNameRequiredScroll(CCoreApp *pMe, int nIdx)
{
    int   cx,cxMax;

    cx = IDISPLAY_MeasureText(pMe->m_pDisplay, AEE_FONT_BOLD, pMe->m_pMusicName + nIdx);
   
    cxMax = pMe->m_rc.dx*3/4 - 2*DISP_BLANK_WIDTH; //��ʾ���־��ο���   
    
    return(cx > cxMax);
}
/*�����Ƿ���Ҫ����*/
static void Core_DrawNameResetScroll(CCoreApp *pMe)
{
    if(NULL == pMe || MP3STATUS_RUNONBACKGROUND != GetMp3PlayerStatus() || NULL == pMe->m_pMusicName)
    {
        return ;
    }
    if(pMe==NULL)
    {
        return;
    }
    pMe->m_nAutoScrollIdx = 0;
    if(Core_bMusicNameRequiredScroll(pMe,0))
    {
       CoreApp_MusicNameAutoScroll(pMe); 
    }
    else
    {
        ISHELL_CancelTimer(pMe->a.m_pIShell,(PFNNOTIFY)CoreApp_MusicNameAutoScroll,pMe);
        CoreApp_DrawMusicName(pMe,0);  
    }
}
#endif
static void CoreApp_GetSPN(CCoreApp *pMe)
{

#ifndef WIN32
#if defined FEATURE_CARRIER_THAILAND_CAT
    MEMSET(pMe->svc_p_name,0,sizeof(pMe->svc_p_name));
    return;
#elif ((defined(FEATURE_CARRIER_THAILAND_HUTCH))&&(!(defined(FEATURE_CARRIER_THAILAND_CAT_FACE))))
    MEMSET(pMe->svc_p_name,0,(UIM_CDMA_HOME_SERVICE_SIZE+1)*sizeof(AECHAR));
    ICONFIG_GetItem(pMe->m_pConfig, CFGI_BANNER, pMe->svc_p_name, (NV_MAX_LTRS+1)*sizeof(AECHAR));
    return;
#endif

#ifdef FEATURE_SPN_FROM_BSMCCMNC
    /*1. Represent the 3-digit Mobile Country Code as D1 D2 D3 with the digit equal to zero
        being given the value of ten.
     2. Compute 100 �� D1 + 10 �� D2 + D3 - 111.
    */
    //Get MCC&MNC codeS
    extern cdma_bs_type *cur_bs_ptr;
    boolean   ServNameinNv = FALSE;
    word curr_mcc;
    byte curr_mnc;

    if(NULL == cur_bs_ptr)
    {
        (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                        AEE_COREAPPRES_LANGFILE,
                        IDS_UNKNOWN_NETWORK,
                        pMe->svc_p_name,
                        sizeof(pMe->svc_p_name));   
        return;
    }
    curr_mcc = cur_bs_ptr->csp.esp.mcc + 111;
    curr_mnc = cur_bs_ptr->csp.esp.imsi_11_12 + 11; 

    //ERR("BS mcc(%d), BS mnc(%d)",cur_bs_ptr->esp.mcc,cur_bs_ptr->esp.imsi_11_12,0);

    if ( curr_mcc % 10 == 0 )
    {
       curr_mcc -= 10;
    }
    if ( (curr_mcc/10) % 10 == 0 )
    {
       curr_mcc -= 100;
    }
    if ( curr_mcc >= 1000 )
    {
       curr_mcc -= 1000; 
    } 

    if ( curr_mnc % 10 == 0 )
    {
       curr_mnc -= 10;
    }
    if ( curr_mnc >= 100 )
    {
       curr_mnc -= 100;
    } 

    //Load name string of service provider
    if ( 460 == curr_mcc && 3 == curr_mnc ) 
    {
       (void) ISHELL_LoadResString(pMe->a.m_pIShell,
                       AEE_COREAPPRES_LANGFILE,
                       IDS_CHINA_TELECOM,
                       pMe->svc_p_name,
                       sizeof(pMe->svc_p_name));   
    }
    else
    {
       // RUIM's mcc,mnc are different from the ones from the BS
       // look for the SP list
       int i;
       int len = sizeof(List_SP)/sizeof(ServiceProviderList);
       for (i = 0; i < len; i++)
       {
          if ((List_SP[i].mcc == curr_mcc) && (List_SP[i].mnc == curr_mnc))
          {
             STRTOWSTR ( List_SP[i].ids_name, pMe->svc_p_name, sizeof(pMe->svc_p_name) );
             ServNameinNv = TRUE;
             //ERR("RUIM mcc == BS mcc ",0,0,0);
             break;
          }
       }
       if(!ServNameinNv)
        {
            AECHAR szFormat[16]; 
            STR_TO_WSTR("%d/%d  ", szFormat, sizeof(szFormat));
            WSPRINTF(pMe->svc_p_name, sizeof(pMe->svc_p_name), szFormat, curr_mcc, curr_mnc);
        }
    }
#else //FEATURE_SPN_FROM_BSMCCMNC    

#if(! defined FEATURE_CARRIER_ISRAEL_PELEPHONE) && (! defined FEATURE_LANG_HEBREW)
    if(pMe->svc_p_name[0] == 0)
#endif
    {
        //DBGPRINTF("IRUIM_Read_Svc_P_Name");
        if(IsRunAsUIMVersion())
        {
       
            if(EFAILED == IRUIM_Read_Svc_P_Name(pMe->m_pIRUIM,pMe->svc_p_name))
            {
                MEMSET(pMe->svc_p_name,0,(UIM_CDMA_HOME_SERVICE_SIZE+1)*sizeof(AECHAR));
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_BANNER, pMe->svc_p_name, (NV_MAX_LTRS+1)*sizeof(AECHAR));
            }        
        }
        else
        {
            MEMSET(pMe->svc_p_name,0,(UIM_CDMA_HOME_SERVICE_SIZE+1)*sizeof(AECHAR));
            {
#if(defined FEATURE_CARRIER_ISRAEL_PELEPHONE) && (defined FEATURE_LANG_HEBREW)
                nv_language_enum_type sel = 0;
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_LANGUAGE_SELECTION, &sel, sizeof(sel));
                if(sel ==  NV_LANGUAGE_HEBREW)
                {
                    ISHELL_LoadResString(pMe->a.m_pIShell,AEE_COREAPPRES_LANGFILE, IDS_ISRAEL_BANNER, pMe->svc_p_name, sizeof(pMe->svc_p_name));
                }
                else
#endif
                {
                    ICONFIG_GetItem(pMe->m_pConfig, CFGI_BANNER, pMe->svc_p_name, (NV_MAX_LTRS+1)*sizeof(AECHAR));
                }
            }
        }
    }
#endif //FEATURE_SPN_FROM_BSMCCMNC   
#endif//WIN32
}
