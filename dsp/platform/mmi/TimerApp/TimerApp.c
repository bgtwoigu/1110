/*==============================================================================
// �ļ���
//        AppTimer.c
//        2007-10-31 ����汾(Draft Version)
//        ��Ȩ����(c) 2008 Anylook
//
// �ļ�˵����
//
// �������ڣ�2008-07-17
// ��ǰ�汾��Draft
==============================================================================*/


/*==============================================================================
�޸���ʷ��¼��
when         who            what, where, why
----------   ---            ---------------------------------------------------
2008-07-15	 	modify draft code.
2008-07-8   raojin.wu   
=============================================================================*/

/*===========================================================================
                     INCLUDE FILES FOR MODULE

===========================================================================*/
#ifndef AEE_SIMULATOR
#include "customer.h"         //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"  // Simulator Feature Definitions
#endif

#include "AEEAlert.h"
#include "AEEShell.h"
#include "AEEAppGen.h"
#include "AEEStdLib.h"
#include "AEEHeap.h"
#include "AEEMenu.h"
#include "AEETime.h"
#include "AEEModTable.h"
#include "AEEdisp.h"
#include "AppComFunc.h"
#include "OEMClassIDs.h"
#include "Appscommon.h"
#include "OEMCFGI.h"
#include "Oemnvint.h"
#ifdef FEATURE_ICM
#include "AEECM.h"
#else
#include "AEETelephone.h"
#include "AEETelDef.h"
#endif
#ifdef FEATURE_APP_MUSICPLAYER
#include "MusicPlayer.h"
#endif 
#include "AEEAnnunciator.h"

#include "apptimerres.brh"
#include "appscommonimages.brh"

#if defined( AEE_SIMULATOR)
    #define  AEE_APPTIMER_RES_FILE "fs:/mod/apptimer/en/apptimerres.bar"
#else
    #define  AEE_APPTIMER_RES_FILE (AEE_RES_LANGDIR APPTIMERRES_RES_FILE)
#endif

#define MSSEC_PER_HOUR         (60*60*1000)  //һСʱ�ڵĺ�����
#define MSSEC_PER_MIN           (60*1000)     //һ�����ڵĺ�����

/*===========================================================================

                    MACRO DEFINITIONS

===========================================================================*/
//#define  AEE_APPTIMER_RES_FILE (AEE_RES_LANGDIR APPTIMER_RES_FILE)
#define RUNNING_UPDATE_MSECS  100
#define TIMER_SCREEN_XOFFSET 20
#define TIMER_SCREEN_HEIGHT   38
#define TIMER_SCREEN_YOFFSET 33

#if defined(FEATURE_DISP_220X176)
#define TIMER_SCREEN_WIDTH    80
#define TIMER_IMAGE_WIDTH      120
#elif defined(FEATURE_DISP_320X240)
#define TIMER_SCREEN_WIDTH    100
#define TIMER_IMAGE_WIDTH      140
#else
#define TIMER_SCREEN_WIDTH    60
#define TIMER_IMAGE_WIDTH      100
#endif
/*===========================================================================

                    TYPE DECLARATIONs

===========================================================================*/

typedef enum {
   W_IDLE = 0,     //��ʼ״̬
   W_STOPPED,      //ֹͣ
   W_RUNNING,      //��ʱ��
   W_CONFIRM,      //��ʱ״̬
} AppTimerState;


typedef struct _CAppTimer CAppTimer;
struct _CAppTimer {
    AEEApplet          a;
    //IMenuCtl           *m_pmenu;
    AppTimerState   timer_state; //�жϵ�ǰ��״̬
    boolean              m_suspending;//�ж��Ƿ��Ǻ�̨���л��Ƿ����
    ITimeCtl           *m_pTime;  //�������ؼ�
    
    //IStatic             * pStatic;
    //IStatic             * pStaticText;

    IConfig            *m_pConfig;         //IConfig�ӿ�ָ��
    
    IAnnunciator        *m_pIAnn;
    IALERT            * m_pAlert;           //IALERTָ��
    boolean           m_bAlert;         /*��ʶ����״̬*/
    ISound            * m_pSound;//�񶯽ӿ�ָ��
    boolean           m_bSound;  /*��ʶ������״̬*/
    
    uint16              m_nTimeCtlCount;    //0��������Сʱ��,1�����ڷ�����,2����������
    uint16              m_nNumKeyCount;     /*��¼������ʱ��ʱ�İ�������.0:Сʱʮλ; 1:Сʱ��λ;   
                                                                2:����ʮλ  3:���Ӹ�λ;   4:����ʮλ  5:���Ӹ�λ*/

    uint32             m_nTicks;//����ʱ�̵��ֻ�ʱ��
    uint32             AppTimerTime;//��¼itime�ؼ���ʼ����ֵ
    int32               m_dwDispTime;       //��Ļ����ʾ��ʱ�䣬�����С
    
    int32               m_dwHour;           //��¼�����ּ����õ�Сʱ
    int32               m_dwMin;            //��¼�����ּ����õķ���
    int32               m_dwSec;            //��¼�����ּ����õ�����

    int32               cxScreen;           //��Ļ���
    int32               cyScreen;           //��Ļ�߶�
    int32               titleBarHeight;   //�������߶�
    boolean             m_Normal;
};

/*===========================================================================

                       FUNCTION DECLARATIONS

===========================================================================*/
int  AppTimerMod_CreateInstance(AEECLSID ClsId,
                                    IShell *pIShell,
                                    IModule *po,
                                    void **ppObj);
int  AppTimerMod_Load(IShell *ps,
                          void *pHelpers,
                          IModule **pMod);

static boolean AppTimer_HandleEvent(CAppTimer * pme, AEEEvent eCode, uint16 wParam, uint32 dwParam);
static void      AppTimer_Free(CAppTimer *pme);

static void      AppTimer_Stop(CAppTimer *pme);
static void      AppTimer_Running(CAppTimer *pme);

static int         AppTimer_Gettime(CAppTimer *pme,uint32 nTicks);
static void      AppTimer_Redraw(CAppTimer *pme);
static boolean CAppTimer_HandleKeyEvent(CAppTimer *pMe, uint16 wParam);
static boolean InitAppTimer(CAppTimer *pme);
static int32 AppTimerPromptMessage(CAppTimer *pme);
static void AppTimer_RingStop(CAppTimer *pme);
static void AppTimer_Vibrate(CAppTimer *pme);
static void AppTimer_StopVibrate(CAppTimer *pme);
static void AppTimer_Confirm(CAppTimer *pme);
//static int AppTimer_StaticTextInit(CAppTimer *pme);
static void TimerNotifyMP3PlayerAlertEvent(CAppTimer *pMe, boolean toStartAlert);
static boolean AppTimer_CanAlert(CAppTimer *pMe);
static void AppTimer_ClearAnnField(CAppTimer *pme);

/*=============================================================================
FUNCTION: AppTimerMod_Load

DESCRIPTION:oem����ں���

PARAMETERS:
   *ps:
   *pHelpers:
   **pMod:

RETURN VALUE:
   int:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int AppTimerMod_Load(IShell *ps,
                         void *pHelpers,
                         IModule **pMod)
{
   return AEEStaticMod_New(sizeof(AEEMod),
                           ps,
                           pHelpers,
                           pMod,
                           (PFNMODCREATEINST) AppTimerMod_CreateInstance,
                           (PFNFREEMODDATA) NULL);
}

/*=============================================================================
FUNCTION: AppTimerMod_CreateInstance

DESCRIPTION:

PARAMETERS:
   ClsId:
   *pIShell:
   *po:
   **ppObj:

RETURN VALUE:
   int:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
#if defined(BREW_STATIC_APP)
int AppTimerMod_CreateInstance(AEECLSID ClsId,
                                   IShell *pIShell,
                                   IModule *po,
                                   void **ppObj)
#else
int AEEClsCreateInstance(AEECLSID ClsId,
                         IShell *pIShell,
                         IModule *po,
                         void **ppObj)
#endif
{
    CAppTimer     *pme;

    // Allocate the applet class.

    if (!AEEApplet_New(sizeof(CAppTimer),
                       ClsId,
                       pIShell,
                       po,
                       (IApplet **) ppObj,
                       (AEEHANDLER) AppTimer_HandleEvent,
                       (PFNFREEAPPDATA) AppTimer_Free)) {
       return ENOMEMORY;
    }

    pme = (CAppTimer *) *ppObj;
    
    InitAppTimer(pme);

    return AEE_SUCCESS;
}


/*=============================================================================
FUNCTION:  InitAppTimer

DESCRIPTION: Returns module information

PARAMETERS:
   *pme:

RETURN VALUE:
   PFNMODENTRY:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean InitAppTimer(CAppTimer *pme)
{
       AEERect rect;
       AEEDeviceInfo deviceInfo;
	
    //pme->m_pmenu = NULL;
    pme->m_pTime = NULL;
    //pme->pStatic = NULL;
    //pme->pStaticText = NULL;
    pme->m_pSound = NULL;
	
#ifndef AEE_SIMULATOR
	pme->m_pAlert = NULL;
#endif

    if (AEE_SUCCESS != ISHELL_CreateInstance(pme->a.m_pIShell, AEECLSID_CONFIG, (void **)&pme->m_pConfig))
    {
	 goto FAILED;
    }
    
    /*if (AEE_SUCCESS != ISHELL_CreateInstance(pme->a.m_pIShell, AEECLSID_MENUCTL, (void **)&pme->m_pmenu))
    {
        goto FAILED;
    }*/

    if (AEE_SUCCESS != ISHELL_CreateInstance(pme->a.m_pIShell, AEECLSID_COUNTDOWNCTL, (void **)&pme->m_pTime))
    {
        goto FAILED;
    }

    /*if( AEE_SUCCESS != ISHELL_CreateInstance( pme->a.m_pIShell, AEECLSID_STATIC, (void **)&pme->pStatic))
    {
        goto FAILED;
    }*/
    
    /*if( AEE_SUCCESS != ISHELL_CreateInstance( pme->a.m_pIShell, AEECLSID_STATIC, (void **)&pme->pStaticText))
    {
        goto FAILED;
    }*/
    
    if (AEE_SUCCESS != ISHELL_CreateInstance(pme->a.m_pIShell, AEECLSID_SOUND,(void **)&pme->m_pSound)) 
    {
        goto FAILED;
    }

#if !defined( AEE_SIMULATOR)
    if (AEE_SUCCESS != ISHELL_CreateInstance(pme->a.m_pIShell, AEECLSID_ALERT,(void **)&pme->m_pAlert)) 
    {
        goto FAILED;
    }
#endif //end of  AEE_SIMULATOR

    if (AEE_SUCCESS != ISHELL_CreateInstance(pme->a.m_pIShell,
                                            AEECLSID_ANNUNCIATOR,
                                            (void **)&pme->m_pIAnn))
    {
        goto FAILED;
    }
	MSG_FATAL("IANNUNCIATOR_SetFieldIsActiveEx::::timeapp111:::",0,0,0);
    //IANNUNCIATOR_SetFieldIsActiveEx(pme->m_pIAnn,FALSE);
    pme->timer_state = W_IDLE;
    pme->m_nTicks = 0;
    pme->m_suspending = FALSE;
    pme->m_dwDispTime = 0;
    pme->m_nNumKeyCount = 0;
    
    ISHELL_GetDeviceInfo(pme->a.m_pIShell, &deviceInfo);
    pme->cxScreen = deviceInfo.cxScreen;
    pme->cyScreen = deviceInfo.cyScreen;
    pme->titleBarHeight = GetTitleBarHeight(pme->a.m_pIDisplay);

    //if(pme->m_pmenu)
    //{
        /*���������*/
        /*SETAEERECT( &rect, 0, pme->cyScreen - BOTTOMBAR_HEIGHT,  pme->cxScreen, BOTTOMBAR_HEIGHT);
        IMENUCTL_SetRect(pme->m_pmenu, &rect);
        
        IMENUCTL_SetProperties( pme->m_pmenu, MP_BIND_ITEM_TO_NUMBER_KEY);
        IMENUCTL_SetActive( pme->m_pmenu, TRUE);
    }*/
        /*TIME�ؼ���*/
    if(NULL != pme->m_pTime)
    {
        //uint32 dwProps;
#if 0        
        SETAEERECT( &rect, 
                            (pme->cxScreen - TIMER_IMAGE_WIDTH)/2 + TIMER_SCREEN_XOFFSET+2,
                            pme->titleBarHeight + MENUITEM_HEIGHT*2 +  TIMER_SCREEN_YOFFSET - 16,
                            TIMER_SCREEN_WIDTH,
                            0);
#else
        SETAEERECT( &rect, 
                            (pme->cxScreen - TIMER_IMAGE_WIDTH)/2 + TIMER_SCREEN_XOFFSET+2,
                            MENUITEM_HEIGHT*2 +  TIMER_SCREEN_YOFFSET - 16,
                            TIMER_SCREEN_WIDTH,
                            0);
#endif

        //SETAEERECT( &rect, 0, (pme->cyScreen - pme->titleBarHeight) / 2 , pme->cxScreen, pme->titleBarHeight*2);
        ITIMECTL_SetRect(pme->m_pTime, &rect);
        ITIMECTL_SetFont(pme->m_pTime, AEE_FONT_BOLD, AEE_FONT_BOLD);
        //ITIMECTL_SetFont(pme->m_pTime, AEE_FONT_LARGE, AEE_FONT_LARGE);
        //dwProps = ITIMECTL_GetProperties(pme->m_pTime);
        ITIMECTL_SetProperties(pme->m_pTime, TP_AUTOREDRAW);
        (void) ITIMECTL_SetTimeEx(pme->m_pTime, pme->m_dwDispTime, TRUE);
        
        ITIMECTL_SetActive( pme->m_pTime, TRUE);
    }

    return TRUE;
    
FAILED:
    AppTimer_Free(pme);
    return FALSE;
}
#if 0
/*=============================================================================
FUNCTION: AppTimer_StaticTextInit

DESCRIPTION:������̬ʱ���ʽ��ʾ�ı�
   Called when applet init or resumed.

PARAMETERS:
   *pme:

RETURN VALUE:
0:OK;-1:FAIL

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int AppTimer_StaticTextInit(CAppTimer *pme)
{
        //draw static text information.
        AEERect  rect  = {0};
        AECHAR wszTitle[16];

        if(NULL ==  pme->pStaticText)
        {
            if( AEE_SUCCESS != ISHELL_CreateInstance( pme->a.m_pIShell, AEECLSID_STATIC, (void **)&pme->pStaticText))
            {
                  return -1;
            }
        }
        /*�߶�Ϊtitlebarhight����time�ؼ�֮���һ��2�߶ȵķָ���*/
        SETAEERECT( &rect, 0, 2*pme->titleBarHeight, pme->cxScreen, MENUITEM_HEIGHT );
        ISTATIC_SetRect(pme->pStaticText, &rect);
        ISTATIC_SetProperties(pme->pStaticText, ST_CENTERTEXT|ST_MIDDLETEXT|ST_TRANSPARENT | ST_TRANSPARENTBACK);
        ISHELL_LoadResString(pme->a.m_pIShell, AEE_APPTIMER_RES_FILE, IDS_APPTIMER_INFO, wszTitle, sizeof(wszTitle));
        ISTATIC_SetText(pme->pStaticText, NULL, wszTitle, AEE_FONT_NORMAL, AEE_FONT_NORMAL);
        ISTATIC_SizeToFit(pme->pStaticText, &rect);
        
        ISTATIC_SetActive(pme->pStaticText, TRUE);
        return 0;
}
#endif
/*=============================================================================
FUNCTION: AppTimer_Free

DESCRIPTION:
   Called by AEEAppGen when the applet's memory is being released.

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void AppTimer_Free(CAppTimer *pme)
{
    if (pme->m_pConfig)
    {
        ICONFIG_Release(pme->m_pConfig);
        pme->m_pConfig = NULL;
    }

    /*if (pme->m_pmenu != NULL) {
       IMENUCTL_Release(pme->m_pmenu);
       pme->m_pmenu = NULL;
    }*/

    if (pme->m_pTime != NULL) {
       ITIMECTL_Release(pme->m_pTime);
       pme->m_pTime = NULL;
    }

    /*if( pme->pStatic != NULL)
    {
        ISTATIC_Release( pme->pStatic);
        pme->pStatic = NULL;
    }*/
    
    /*if( pme->pStaticText != NULL)
    {
        ISTATIC_Release( pme->pStaticText);
        pme->pStaticText = NULL;
    }*/
    
    if(pme->m_pSound != NULL)
    {
        ISOUND_Release(pme->m_pSound);
        pme->m_pSound = NULL;
    }

    if(pme->m_pIAnn != NULL)
    {
        IANNUNCIATOR_Release(pme->m_pIAnn);
        pme->m_pIAnn = NULL;
    } 
    
#ifndef AEE_SIMULATOR
    if(pme->m_pAlert!= NULL)
    {
        IALERT_Release(pme->m_pAlert);
        pme->m_pAlert = NULL;
    }
#endif
    
    ISHELL_CancelTimer(pme->a.m_pIShell,NULL,pme);
}
/*=============================================================================
FUNCTION: AppTimerPromptMessage

DESCRIPTION:
   Called by AEEAppGen when the applet's itime expire

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int32 AppTimerPromptMessage(CAppTimer *pme)
{
        //PromptMsg_Param_type    parm;
        AECHAR                 wszTitle[16];
        byte     curProfile = 0;
        byte     byte_return[PROFILENUMBER];
        ringID   ring_id[PROFILENUMBER];//��������ID
        IImage* prompt;
        AEERect rc;

#if 0
        if(NULL == pme->pStatic)
        {
            if( AEE_SUCCESS != ISHELL_CreateInstance( pme->a.m_pIShell, AEECLSID_STATIC, (void **)&pme->pStatic))
            {
                AppTimer_Free(pme);
                return FALSE;
            }
        }  

        MEMSET( &parm, 0, sizeof( parm));
        parm.ePMsgType  = MESSAGE_INFORMATIVE;
        parm.eBBarType  = BTBAR_PROMPT_OK;
#endif
        ISHELL_LoadResString(pme->a.m_pIShell, AEE_APPTIMER_RES_FILE, IDS_APPTIMER_PROMPT, wszTitle, sizeof(wszTitle));
        //parm.pwszTitle = wszTitle;
        prompt = ISHELL_LoadResImage(pme->a.m_pIShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_TIMER);

//        parm.pwszMsg = wszTitle;
        
        //ISHELL_CreateDialog( pme->a.m_pIShell,AEE_APPTIMER_RES_FILE,IDD_DIALOG_6001,NULL);
        
        //parm.nMsgResID = IDS_APPTIMER_PROMPT;
        //MEMCPY(parm.strMsgResFile,AEE_APPTIMER_RES_FILE,sizeof(parm.strMsgResFile));

        pme->m_dwHour = 0;
        pme->m_dwMin = 0;
        pme->m_dwSec = 0;

        ICONFIG_GetItem(pme->m_pConfig, CFGI_PROFILE_CUR_NUMBER, &curProfile, sizeof(curProfile));

        ICONFIG_GetItem(pme->m_pConfig, CFGI_PROFILE_SMS_RINGER, byte_return, sizeof(byte_return));
        if((OEMNV_ALERTTYPE_VIB == byte_return[curProfile]) || (OEMNV_ALERTTYPE_VIBRINGER == byte_return[curProfile]))
        {
             AppTimer_Vibrate(pme);
             pme->m_bSound = TRUE;
             ISHELL_SetTimer(pme->a.m_pIShell, 60000, (PFNNOTIFY)AppTimer_StopVibrate, pme);
        }

#ifndef AEE_SIMULATOR
        pme->m_bAlert = TRUE;
        //ȡ�����趨������
        ICONFIG_GetItem(pme->m_pConfig,CFGI_PROFILE_ALARM_RINGER, (void*)ring_id,sizeof(ring_id));
        /*�����IALERT_StartRingerAlert()�ᵼ��normal�����ģʽ��һ��,
        ����û����ʱ�������񶯺����������*/
        TimerNotifyMP3PlayerAlertEvent(pme, TRUE);
        if(ring_id[curProfile].ringType == OEMNV_MID_RINGER)
        {
            IALERT_StartRingerAlert(pme->m_pAlert, ring_id[curProfile].midID);
        }
        else
        {
            if( IALERT_StartMp3Alert(pme->m_pAlert, ring_id[curProfile].szMusicname, ALERT_NORMAL_SND) != SUCCESS )
            {
                IALERT_StartRingerAlert(pme->m_pAlert, ring_id[curProfile].midID);
            }
        }
        ISHELL_SetTimer(pme->a.m_pIShell, 60000, (PFNNOTIFY)(AppTimer_RingStop), pme);
#endif

//        DrawPromptMessage( pme->a.m_pIDisplay, pme->pStatic, &parm);
#if 0
        SETAEERECT(&rc, 0, TITLEBAR_HEIGHT, pme->cxScreen, pme->cyScreen - TITLEBAR_HEIGHT - BOTTOMBAR_HEIGHT);
#else
        SETAEERECT(&rc, 0, 0, pme->cxScreen, pme->cyScreen - TITLEBAR_HEIGHT - BOTTOMBAR_HEIGHT);
#endif
        Appscommon_ResetBackgroundEx(pme->a.m_pIDisplay, &rc, TRUE);
        if( prompt != NULL)
        {
            AEEImageInfo ImageInfo;
            
            IIMAGE_GetInfo(prompt, &ImageInfo);
            //IIMAGE_Draw(prompt, 0, TITLEBAR_HEIGHT);
            IIMAGE_Draw(prompt, (pme->cxScreen - ImageInfo.cx)/2, TITLEBAR_HEIGHT + (pme->cyScreen - BOTTOMBAR_HEIGHT - ImageInfo.cy)/2);
            IIMAGE_Release(prompt);
            prompt = NULL;
        }
        //draw string
        if( wszTitle != NULL)
        {
            AEEDeviceInfo di;
            RGBVAL nOldFontColor;
            ISHELL_GetDeviceInfo(pme->a.m_pIShell, &di);
#if 0            
            SETAEERECT(&rc, 0, TITLEBAR_HEIGHT, di.cxScreen, TITLEBAR_HEIGHT);
#else
            SETAEERECT(&rc, 0, 0, di.cxScreen, TITLEBAR_HEIGHT);
#endif
            nOldFontColor = IDISPLAY_SetColor(pme->a.m_pIDisplay, CLR_USER_TEXT, RGB_WHITE);
            IDISPLAY_DrawText(pme->a.m_pIDisplay, 
                                AEE_FONT_NORMAL, 
                                wszTitle, 
                                -1, 
                                rc.x, 
                                rc.y, 
                                &rc, 
                                IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE |IDF_TEXT_TRANSPARENT);
            (void)IDISPLAY_SetColor(pme->a.m_pIDisplay, CLR_USER_TEXT, nOldFontColor);
        }
        pme->timer_state = W_CONFIRM;
        {
            BottomBar_Param_type  BBarParam ={0};
            
            BBarParam.eBBarType = BTBAR_BACK;
            DrawBottomBar(pme->a.m_pIDisplay, &BBarParam);
        }

        return SUCCESS;
}
/*==============================================================================
������
       AppTimer_Vibrate

˵����
       �˺���ʹ�ֻ�������״̬��

������
       pMe [in]��ָ��CAppTimer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       �ޡ�

��ע��:
==============================================================================*/
static void AppTimer_Vibrate(CAppTimer *pme)
{
    static boolean vib = FALSE;

    // Alternate between vibrating and not
    vib = !vib;
    if( vib )
    {
        ISOUND_Vibrate(pme->m_pSound, 1500);
        (void)ISHELL_SetTimer(pme->a.m_pIShell, 1500, (PFNNOTIFY)(AppTimer_Vibrate), pme);
    }
    else
    {
        ISOUND_StopVibrate(pme->m_pSound);
        (void)ISHELL_SetTimer(pme->a.m_pIShell, 1000, (PFNNOTIFY)(AppTimer_Vibrate), pme);
    }
}
/*==============================================================================
������
       AppTimer_StopVibrate

˵����
       �˺���ʹ�ֻ�ȡ����״̬��

������
       pme ��ָ��CAppTimer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       �ޡ�

��ע��:
==============================================================================*/
static void AppTimer_StopVibrate(CAppTimer *pme)
{
    ISHELL_CancelTimer(pme->a.m_pIShell, (PFNNOTIFY)AppTimer_Vibrate, pme);
}

/*==============================================================================
������
       AppTimer_ClearAnnField

˵����
       �˺���ʹ�ֻ�ȡ����״̬��

������
       pme ��ָ��CAppTimer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       �ޡ�

��ע��:
==============================================================================*/
static void AppTimer_ClearAnnField(CAppTimer *pme)
{
    IANNUNCIATOR_SetField(pme->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_TIMER_OFF);
}

/*==============================================================================
������
       AppTimer_RingStop

˵����
       �˺���ʹ�ֻ�ֹͣ����״̬��

������
       pme ��ָ��CAppTimer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       �ޡ�

��ע��:
==============================================================================*/
static void AppTimer_RingStop(CAppTimer *pme)
{
#ifndef AEE_SIMULATOR
    if(pme->m_bAlert == TRUE)
    {
        pme->m_bAlert = FALSE;
        IALERT_StopRingerAlert(pme->m_pAlert);
        IALERT_StopMp3Alert(pme->m_pAlert);
        TimerNotifyMP3PlayerAlertEvent(pme, FALSE);
    }
    pme->timer_state = W_IDLE;
    if(pme->m_Normal)
    {
        ITIMECTL_SetActive(pme->m_pTime, TRUE);
        AppTimer_Redraw(pme);
    }
    else
    {
        ISHELL_CloseApplet(pme->a.m_pIShell, FALSE);
    }
#endif
}
/*=============================================================================
FUNCTION:  AppTimer_HandleEvent

DESCRIPTION:apptimer event handle

PARAMETERS:
   *pme:

RETURN VALUE:
   boolean:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean AppTimer_HandleEvent(CAppTimer *pme, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
	if (eCode == EVT_USER)
	{
		if (wParam == AVK_CLR)
		{
			eCode = EVT_KEY;
		}
		else if (wParam == AVK_SELECT)
		{
			eCode = EVT_KEY;
		}
		else if (wParam == AVK_INFO)
		{
			eCode = EVT_KEY;
		}
	}
#endif
    switch (eCode)
    {
        case EVT_APP_START:
        if((pme->m_suspending == TRUE) && (pme->timer_state == W_RUNNING) && (0 == ITIMECTL_GetTime(pme->m_pTime)))
        {
            //pme->timer_state = W_STOPPED;
            pme->m_suspending = FALSE;
            
            AppTimer_Redraw( pme);
            if(SUCCESS == AppTimerPromptMessage(pme))
            {
                return TRUE;
            }
            return FALSE;
        }
        if(pme->m_pIAnn != NULL)
         {
            IANNUNCIATOR_SetFieldIsActiveEx(pme->m_pIAnn,FALSE);
        }
        pme->m_suspending = FALSE;
        AppTimer_Redraw(pme);
        return TRUE;

        case EVT_APP_SUSPEND:
            pme->m_suspending = TRUE;
            if (pme->timer_state != W_RUNNING)
            {
                ISHELL_CancelTimer(pme->a.m_pIShell, NULL, pme);
            }

            if(pme->timer_state == W_CONFIRM)
            {
                IALERT_StopMp3Alert(pme->m_pAlert);
            }
            /*if(NULL != pme->pStatic)
            {
                 ISTATIC_Release(pme->pStatic);
                 pme->pStatic = NULL;
            }*/
            /*if(NULL != pme->pStaticText)
            {
                 ISTATIC_Release(pme->pStaticText);
                 pme->pStaticText = NULL;
            }*/
            
            return TRUE;

        case EVT_APP_RESUME:
            pme->m_suspending = FALSE;

            /*if( AEE_SUCCESS != ISHELL_CreateInstance( pme->a.m_pIShell, AEECLSID_STATIC, (void **)&pme->pStatic))
            {
                return FALSE;
            }*/
            /*if(0 != AppTimer_StaticTextInit(pme))
            {
                return FALSE;
            }*/
            
            switch (pme->timer_state)
            {
                case W_RUNNING:
                    AppTimer_Running( pme);
                    break;

                case W_STOPPED:
                    AppTimer_Stop( pme);
                    break;

                case W_CONFIRM:
                    AppTimer_Confirm(pme);
                    break;
            }
            AppTimer_Redraw( pme);
            return TRUE;


        case EVT_APP_STOP:
            if(0 != ITIMECTL_GetTime(pme->m_pTime) && pme->timer_state != W_CONFIRM)
            {
                *((boolean *)dwParam) = FALSE;
                pme->m_suspending = TRUE;
                return TRUE;
            }

            pme->m_dwHour = 0;
            pme->m_dwMin = 0;
            pme->m_dwSec = 0;

            pme->m_bSound = FALSE;
            //pme->m_bAlert = FALSE;
            ISHELL_CancelTimer(pme->a.m_pIShell, (PFNNOTIFY)AppTimer_Vibrate, pme);
            ISHELL_CancelTimer(pme->a.m_pIShell, (PFNNOTIFY)AppTimer_StopVibrate, pme);
            if(pme->m_bAlert)
            {
                pme->m_bAlert =FALSE;
                IALERT_StopRingerAlert(pme->m_pAlert);
                IALERT_StopMp3Alert(pme->m_pAlert);
                TimerNotifyMP3PlayerAlertEvent(pme, FALSE);
            }
            pme->timer_state = W_IDLE;
            IANNUNCIATOR_SetField(pme->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_TIMER_OFF);
            return TRUE;

        case EVT_APP_NO_SLEEP:
        case EVT_DIALOG_INIT:
        case EVT_DIALOG_START:
        case EVT_DIALOG_END:
        case EVT_USER_REDRAW:
            return TRUE;

        case EVT_KEY:
            if(pme->m_bSound == TRUE)
            {
                pme->m_bSound = FALSE;
                ISHELL_CancelTimer(pme->a.m_pIShell, (PFNNOTIFY)AppTimer_Vibrate, pme);
                ISHELL_CancelTimer(pme->a.m_pIShell, (PFNNOTIFY)AppTimer_StopVibrate, pme);
            }
#ifndef AEE_SIMULATOR
            if(pme->m_bAlert == TRUE)
            {
                pme->m_bAlert = FALSE;
                IALERT_StopRingerAlert(pme->m_pAlert);
                IALERT_StopMp3Alert(pme->m_pAlert);
                TimerNotifyMP3PlayerAlertEvent(pme, FALSE);
            }
#endif
            switch(wParam)
            {

                case AVK_CLR: 
                {
                    if (pme->timer_state == W_RUNNING || pme->timer_state == W_STOPPED)
                    {
                        ISHELL_CloseApplet(pme->a.m_pIShell, FALSE);
                    }
                    else
                    {
                        //pme->m_dwDispTime = 0;
                        //(void) ITIMECTL_SetTimeEx(pme->m_pTime, pme->m_dwDispTime, TRUE);
                        pme->timer_state = W_IDLE;
                        ISHELL_CancelTimer(pme->a.m_pIShell, NULL, pme);
                        ISHELL_CloseApplet(pme->a.m_pIShell, FALSE);
                        IANNUNCIATOR_SetField(pme->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_TIMER_OFF);
                    }

                    return TRUE;
                }
                case AVK_SELECT:
                    {
                        switch( pme->timer_state)
                        {
                            case W_IDLE:
                                {
                                      if(100 < ITIMECTL_GetTime(pme->m_pTime))
                                      {
                                          pme->timer_state    = W_RUNNING;
                                          pme->AppTimerTime = ITIMECTL_GetTime(pme->m_pTime);
                                          pme->m_nTicks       = ISHELL_GetTimeMS( pme->a.m_pIShell);
                                          ITIMECTL_SetActive(pme->m_pTime, FALSE);
                                          AppTimer_Running( pme);
                                          IANNUNCIATOR_SetField(pme->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_TIMER_ON);
                                      }
                                      else
                                      {
                                        ;//prompt a dialog to set the time
                                      }
                                }
                                break;
                            
                            case W_RUNNING:
                                pme->m_dwDispTime = 0;
                                pme->m_dwHour = 0;
                                pme->m_dwMin = 0;
                                pme->m_dwSec = 0;
                                (void) ITIMECTL_SetTimeEx(pme->m_pTime, pme->m_dwDispTime, TRUE);
                                pme->timer_state = W_IDLE;
                                ISHELL_CancelTimer(pme->a.m_pIShell, NULL, pme);
                                pme->m_nTimeCtlCount = 0;
                                pme->m_nNumKeyCount = 0;
                                ITIMECTL_SetActive(pme->m_pTime, TRUE);
                                IANNUNCIATOR_SetField(pme->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_TIMER_OFF); 
                                break;

                            case W_CONFIRM:
                                return TRUE;

                            default:
                                break;
                        }
                        AppTimer_Redraw( pme);
                        return TRUE;
                    }

                case AVK_UP:
                case AVK_DOWN:
                    //ITIMECTL_HandleEvent(pme->m_pTime,EVT_KEY,wParam,dwParam);
                    //break;
                    return TRUE;

                default:
                    if(pme->timer_state == W_IDLE)
                    {
                        CAppTimer_HandleKeyEvent(pme,wParam);
                    }
                    break;
             }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
			{
				int i=0;
				int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
				int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
				int xy[][4] = {
					{42,107,64,130},      // hour
					{77,107,99,130},      // minute
					{111,107,133,130},    // second
					{60,184,118,202},
					{119,184,176,202},
					{0,184,59,202}
				};
				for( i = 0; i < 6; i ++)
				{
					if( wXPos >= xy[i][0] &&
						wXPos <= xy[i][2] &&
						wYPos >= xy[i][1] &&
						wYPos <= xy[i][3]
					)
					{
						if (i == 0)
						{
							if (pme->timer_state == W_IDLE)
							{
								pme->m_nTimeCtlCount = 0;
								pme->m_nNumKeyCount = 0;
								ITIMECTL_SetEditField(pme->m_pTime,ITF_HOUR); 
								(void)ITIMECTL_Redraw(pme->m_pTime);
							}

						}
						else if (i == 1)
						{
							if (pme->timer_state == W_IDLE)
							{
								pme->m_nTimeCtlCount = 1;
								pme->m_nNumKeyCount = 2;
								ITIMECTL_SetEditField(pme->m_pTime,ITF_MIN); 
								(void)ITIMECTL_Redraw(pme->m_pTime);
							}
						}
						else if (i == 2)
						{
							if (pme->timer_state == W_IDLE)
							{
								pme->m_nTimeCtlCount = 2;
								pme->m_nNumKeyCount = 4;
								ITIMECTL_SetEditField(pme->m_pTime,ITF_SEC); 
								(void)ITIMECTL_Redraw(pme->m_pTime);
							}
						}
						else if (i == 3)
						{
							boolean rt =  ISHELL_PostEvent(pme->a.m_pIShell,AEECLSID_APPTIMER,EVT_USER,AVK_INFO,0);
							return rt;
						}
						else if (i == 4)
						{
							boolean rt =  ISHELL_PostEvent(pme->a.m_pIShell,AEECLSID_APPTIMER,EVT_USER,AVK_CLR,0);
							return rt;
						}
						else if (i == 5)
						{
							boolean rt =  ISHELL_PostEvent(pme->a.m_pIShell,AEECLSID_APPTIMER,EVT_USER,AVK_SELECT,0);
							return rt;
						}
					}
				}
			}
			return TRUE;
#endif              
        default:
            break;
        }

    return (FALSE);
}

/*=============================================================================
FUNCTION: AppTimer_ReDraw

DESCRIPTION:������ʱ������

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void AppTimer_Redraw(CAppTimer *pme)
{
    TitleBar_Param_type  TBarParam = {0};
    BottomBar_Param_type  BBarParam ={0};
    AECHAR                   wszTitle[16] = {0};
    AEERect                  rect            = {0};
    RGBVAL                   nOldFontColor;
    IImage                    *pBgImage = NULL;
    BottomBar_e_Type    btbType[] = {BTBAR_START_BACK, 
                                     BTBAR_CANCEL_RESUME_NULL, 
                                     BTBAR_STOP_BACK, 
                                     BTBAR_STOP_BACK};
    
    //draw background
#if 0    
    SETAEERECT(&rect, 0, pme->titleBarHeight, pme->cxScreen, pme->cyScreen - BOTTOMBAR_HEIGHT);
#else
    SETAEERECT(&rect, 0, 0, pme->cxScreen, pme->cyScreen - BOTTOMBAR_HEIGHT);
#endif
    Appscommon_ResetBackgroundEx(pme->a.m_pIDisplay, &rect, TRUE);

    //Draw title bar
    ISHELL_LoadResString(pme->a.m_pIShell, AEE_APPTIMER_RES_FILE, IDS_APPTIMER_TITLE, wszTitle, sizeof(wszTitle));
    TBarParam.pwszTitle = wszTitle;
	#if 0
    TBarParam.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER;
    DrawTitleBar(pme->a.m_pIDisplay, &TBarParam);
    #else
	IANNUNCIATOR_SetFieldText(pme->m_pIAnn,wszTitle);
	#endif
    //draw static text info
    MEMSET(wszTitle, 0, sizeof(wszTitle));
	#if 0
    SETAEERECT( &rect, 0, pme->titleBarHeight + MENUITEM_HEIGHT/2, pme->cxScreen, MENUITEM_HEIGHT);
	#else
	SETAEERECT( &rect, 0, MENUITEM_HEIGHT/2, pme->cxScreen, MENUITEM_HEIGHT);
	#endif
    ISHELL_LoadResString(pme->a.m_pIShell, AEE_APPTIMER_RES_FILE, IDS_APPTIMER_INFO, wszTitle, sizeof(wszTitle));
    nOldFontColor = IDISPLAY_SetColor(pme->a.m_pIDisplay, CLR_USER_TEXT, RGB_WHITE);
    IDISPLAY_DrawText(pme->a.m_pIDisplay, 
                        AEE_FONT_NORMAL, 
                        wszTitle, 
                        -1, 
                        0, 
                        0, 
                        &rect, 
                        IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE |IDF_TEXT_TRANSPARENT);
    (void)IDISPLAY_SetColor(pme->a.m_pIDisplay, CLR_USER_TEXT, nOldFontColor);

    //draw clock image
    pBgImage = ISHELL_LoadResImage(pme->a.m_pIShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_TIMER_CLOCK);
    if(pBgImage != NULL)
    {
    	#if 0
        IImage_Draw(pBgImage, (rect.dx - TIMER_IMAGE_WIDTH)/2, pme->titleBarHeight + MENUITEM_HEIGHT*2);
		#else
		IImage_Draw(pBgImage, (rect.dx - TIMER_IMAGE_WIDTH)/2,  MENUITEM_HEIGHT*2);
		#endif
        IImage_Release(pBgImage);
        pBgImage = NULL;
    }

    //draw line
    BBarParam.eBBarType = btbType[pme->timer_state];
    if(pme->timer_state == W_IDLE || pme->timer_state == W_CONFIRM)
    {
        uint32 dwseconds = ITIMECTL_GetTime(pme->m_pTime);
        if(dwseconds == 0)
        {
            BBarParam.eBBarType = BTBAR_BACK;
        }
    }
    DrawBottomBar(pme->a.m_pIDisplay, &BBarParam);

    ITIMECTL_Redraw( pme->m_pTime);
    //IMENUCTL_Redraw( pme->m_pmenu);
    //ISTATIC_Redraw(pme->pStaticText);
    IDISPLAY_Update(pme->a.m_pIDisplay);
}

/*=============================================================================
FUNCTION: AppTimer_Gettime

DESCRIPTION:������ǰҪˢ�µ�ʱ��

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static int AppTimer_Gettime(CAppTimer *pme,uint32 nTicks)
{
    pme->m_dwDispTime = pme->AppTimerTime - (nTicks - pme->m_nTicks);

    ITIMECTL_SetProperties(pme->m_pTime,0);

    if(pme->m_suspending != TRUE)
    {
        (void) ITIMECTL_SetTimeEx(pme->m_pTime, pme->m_dwDispTime,TRUE);
    }

    return ((100 >= pme->m_dwDispTime)?-1:0);
}
/*=============================================================================
FUNCTION: AppTimer_Running

DESCRIPTION: ��ʱ����ʼ������ʱ

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void AppTimer_Running(CAppTimer *pme)
{
        uint32   nMS;
        int ret = 0;

        //���ص�ǰʱ�䣨���룩��
        //���ص�ֵȡ���� BREW �ƶ��豸�ӻ�վ��ȡ���豸��ǰʱ��ֵ
        nMS = ISHELL_GetTimeMS(pme->a.m_pIShell);

        ret = AppTimer_Gettime(pme,nMS); //������ǰ��ʱ��
        if(-1 != ret)
        {
                // �㷨����������100ms��ʱ��
                nMS = (RUNNING_UPDATE_MSECS - (ISHELL_GetTimeMS(pme->a.m_pIShell) - nMS));

                (void) ISHELL_SetTimer(pme->a.m_pIShell,(int32) nMS,(PFNNOTIFY)AppTimer_Running, pme);
        }
        else if(TRUE == pme->m_suspending) //��̨������ʱ
        {
                pme->m_dwDispTime = 0;
                (void) ITIMECTL_SetTimeEx(pme->m_pTime, pme->m_dwDispTime, TRUE);
                (void) ISHELL_CancelTimer(pme->a.m_pIShell,(PFNNOTIFY)AppTimer_Running, pme);
                
                if(AppTimer_CanAlert(pme))
                {
                    ret = ISHELL_StartApplet(pme->a.m_pIShell, AEECLSID_APPTIMER);
                    pme->m_Normal = FALSE;
                }
                else
                {
                    ISOUND_Vibrate(pme->m_pSound, 2000);
                    pme->timer_state = W_IDLE;
                    pme->m_nTimeCtlCount = 0;
                    pme->m_nNumKeyCount = 0;
                    ITIMECTL_SetActive(pme->m_pTime, TRUE);

                    IANNUNCIATOR_SetField(pme->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_TIMER_ON|ANNUN_STATE_BLINK);
                    IANNUNCIATOR_SetUnblinkTimer(pme->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_TIMER_OFF, 10000);
                    ISHELL_SetTimer(pme->a.m_pIShell, 10000, (PFNNOTIFY)AppTimer_ClearAnnField, pme);// �����TIMER Ҫ��״̬���Ǳߵ���˸ʱ��һ�� 
                    
                }
        }
        else //����������ʱ
        {
            pme->m_dwDispTime = 0;
            (void) ITIMECTL_SetTimeEx(pme->m_pTime, pme->m_dwDispTime, TRUE);        
            (void) ISHELL_CancelTimer(pme->a.m_pIShell,(PFNNOTIFY)AppTimer_Running, pme);
            pme->timer_state = W_CONFIRM;
            pme->m_Normal = TRUE;
            AppTimerPromptMessage(pme);
        }
}

/*=============================================================================
FUNCTION: AppTimer_Stop

DESCRIPTION: ��ʱ��ֹͣ

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void AppTimer_Stop(CAppTimer *pme)
{
    (void) ISHELL_CancelTimer(pme->a.m_pIShell, (PFNNOTIFY)AppTimer_Running, pme);
    pme->m_dwDispTime = pme->AppTimerTime - (ISHELL_GetTimeMS(pme->a.m_pIShell) - pme->m_nTicks);
    ITIMECTL_SetProperties(pme->m_pTime,0);
    (void) ITIMECTL_SetTimeEx(pme->m_pTime, pme->m_dwDispTime, TRUE);

    pme->timer_state = W_STOPPED;
    AppTimer_Redraw( pme);
}
/*=============================================================================
FUNCTION: AppTimer_Stop

DESCRIPTION: ��ʱ����ʱ������

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void AppTimer_Confirm(CAppTimer *pme)
{
    pme->timer_state = W_IDLE;
    pme->m_nTimeCtlCount = 0;
    pme->m_nNumKeyCount = 0;
    ITIMECTL_SetActive(pme->m_pTime, TRUE);
    AppTimer_Redraw( pme);
    return;
}
/*==============================================================================
������
       CAppTimer_HandleKeyEvent

˵����
       ʱ��ؼ��°����ּ�����ʱ��

������
       pMe [in]��ָ��Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam [in]: �����¼�.


����ֵ��
       boolean��

��ע��:

==============================================================================*/
static boolean CAppTimer_HandleKeyEvent(CAppTimer *pMe, uint16 wParam)
{
    uint16 nKeyMask = 0;

    if (pMe == NULL)
    {
        return FALSE;
    }
    
    switch(wParam)
    {
       case AVK_LEFT:
            if(pMe->m_nTimeCtlCount == 0)
            {
                pMe->m_nTimeCtlCount = 2;
                pMe->m_nNumKeyCount = 4;
                ITIMECTL_SetEditField(pMe->m_pTime,ITF_SEC); 
                (void)ITIMECTL_Redraw(pMe->m_pTime);
            }
            else if(pMe->m_nTimeCtlCount == 1)
            {
                pMe->m_nTimeCtlCount = 0;
                pMe->m_nNumKeyCount = 0;
                ITIMECTL_SetEditField(pMe->m_pTime,ITF_HOUR); 
                (void)ITIMECTL_Redraw(pMe->m_pTime);
            }
            else if(pMe->m_nTimeCtlCount == 2)
            {
                pMe->m_nTimeCtlCount = 1;
                pMe->m_nNumKeyCount = 2;
                ITIMECTL_SetEditField(pMe->m_pTime,ITF_MIN); 
                (void)ITIMECTL_Redraw(pMe->m_pTime);
            }
            return TRUE;
            
        case AVK_RIGHT:
            //��ʱ��ؼ�����������,���԰����¼������õ�ʱ��
            if(pMe->m_nTimeCtlCount == 0)
            {
                pMe->m_nTimeCtlCount = 1;
                pMe->m_nNumKeyCount = 2;
                ITIMECTL_SetEditField(pMe->m_pTime,ITF_MIN); 
                (void)ITIMECTL_Redraw(pMe->m_pTime);
            }
            else if(pMe->m_nTimeCtlCount == 1)
            {
                pMe->m_nTimeCtlCount = 2;
                pMe->m_nNumKeyCount = 4;
                ITIMECTL_SetEditField(pMe->m_pTime,ITF_SEC); 
                (void)ITIMECTL_Redraw(pMe->m_pTime);
            }
            else if(pMe->m_nTimeCtlCount == 2)
            {
                pMe->m_nTimeCtlCount = 0;
                pMe->m_nNumKeyCount = 0;
                ITIMECTL_SetEditField(pMe->m_pTime,ITF_HOUR); 
                (void)ITIMECTL_Redraw(pMe->m_pTime);
            }
            return TRUE;

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
            nKeyMask = wParam - AVK_0;
            break;

        default:
            break;
    }
    
    if(pMe->m_nNumKeyCount == 0)    //Сʱ��ʮλ
    {
        //Сʱ��ֻ������23Сʱ
        if(nKeyMask == 0 || nKeyMask == 1 || nKeyMask == 2)
        {
            pMe->m_dwHour = 10* nKeyMask * MSSEC_PER_HOUR;
            pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin + pMe->m_dwSec;

            (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);
             pMe->m_nTimeCtlCount = 0;
        }
        else
        {
            return TRUE;
        }
    }
    else if(pMe->m_nNumKeyCount == 1)   //Сʱ�ĸ�λ
    {
        if(nKeyMask == 0 || nKeyMask == 1 || nKeyMask == 2 || nKeyMask == 3)
        {
            pMe->m_dwHour += nKeyMask * MSSEC_PER_HOUR;
            pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin  + pMe->m_dwSec;

            (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);

            //�༭�����Զ�����Ϊ����
            ITIMECTL_SetEditField(pMe->m_pTime, ITF_MIN);
            (void)ITIMECTL_Redraw(pMe->m_pTime);
            pMe->m_nTimeCtlCount = 1;
        }
        else 
        {
            return TRUE;
        }
    }
    else if(pMe->m_nNumKeyCount == 2)   //���ӵ�ʮλ
    {
        //���ӵ�ʮλ����������6789,�������õ�ʱ�佫����
        if(nKeyMask != 6 && nKeyMask != 7
           && nKeyMask != 8 && nKeyMask != 9)
        {
            pMe->m_dwMin = 10 * nKeyMask * MSSEC_PER_MIN;
            pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin + pMe->m_dwSec ;

            (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);
             pMe->m_nTimeCtlCount = 1;
        }
        else
        {
            return TRUE;
        }
    }
    else if(pMe->m_nNumKeyCount == 3)  //���ӵĸ�λ
    {
        pMe->m_dwMin += nKeyMask * MSSEC_PER_MIN;
        pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin + pMe->m_dwSec;

        (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);

        //�༭�����Զ�����Ϊ����
        ITIMECTL_SetEditField(pMe->m_pTime, ITF_SEC);
        (void)ITIMECTL_Redraw(pMe->m_pTime);
        pMe->m_nTimeCtlCount = 2;
    }
    else if(pMe->m_nNumKeyCount == 4) //����ʮλ
    {
        if(nKeyMask != 6 && nKeyMask != 7
        && nKeyMask != 8 && nKeyMask != 9)
        {
            pMe->m_dwSec = 10 * nKeyMask * 1000;
            pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin + pMe->m_dwSec ;

            (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);
            pMe->m_nTimeCtlCount = 2;
        }
        else
        {
            return TRUE;
        }
    }
    else if(pMe->m_nNumKeyCount == 5) //���Ӹ�λ
    {
        pMe->m_dwSec += nKeyMask * 1000;
        pMe->m_dwDispTime = pMe->m_dwHour + pMe->m_dwMin + pMe->m_dwSec;

        (void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);
        
        //�Զ����ÿؼ�����
        pMe->m_nNumKeyCount = 0;    //����m_nNumKeyCount
        pMe->m_nTimeCtlCount = 0;  
        //�༭�����Զ�����Ϊʱ��
        ITIMECTL_SetEditField(pMe->m_pTime, ITF_HOUR);
        (void)ITIMECTL_Redraw(pMe->m_pTime);
        (void)AppTimer_Redraw(pMe);
        return TRUE;    //������m_nNumKeyCount,ֱ�ӷ���
    }

    //ʱ��ؼ�������������
    pMe->m_nNumKeyCount++;
    //pMe->m_nTimeCtlCount++;
    (void)ITIMECTL_Redraw(pMe->m_pTime);
    (void)AppTimer_Redraw(pMe);
    return TRUE;
}
static void TimerNotifyMP3PlayerAlertEvent(CAppTimer *pMe, boolean toStartAlert)
{
 #ifdef FEATURE_APP_MUSICPLAYER
    if(GetMp3PlayerStatus() == MP3STATUS_RUNONBACKGROUND)
    {
        ISHELL_SendEvent(pMe->a.m_pIShell,
                         AEECLSID_APP_MUSICPLAYER,
                         EVT_ALARM,
                         toStartAlert,
                         0);
    }
 #endif
}

/*==============================================================================
������
       AppTimer_CanAlert
˵����
       ��������ȷ���Ƿ���Ի������������棬Ŀǰ����ͨ����������жϣ��Ժ�������
       Ҫ����ڴ���ӡ�

������
       pMe [in]��ָ�� AlarmClock Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       TRUE: ������ʾ
       FALSE: ��������ʾ

��ע��

==============================================================================*/
#ifdef FEATURE_APP_RECORDER
extern boolean recorder_GetRunningFlag(void);
#endif
boolean AppTimer_CanAlert(CAppTimer *pMe)
{
#ifdef FEATURE_ICM
    ICM *pICM = NULL;
    uint16 num = 0;
    
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->a.m_pIShell, 
                                            AEECLSID_CM, 
                                            (void **)&pICM))
    {
        return EFAILED;
    }
    if(pICM)
    {
        num = ICM_GetActiveCallIDs(pICM, 
                               (AEECM_CALL_TYPE_VOICE | AEECM_CALL_TYPE_EMERGENCY), 
                               (AEECM_CALL_STATE_ORIG | AEECM_CALL_STATE_INCOM | 
                                AEECM_CALL_STATE_CONV | AEECM_CALL_STATE_ONHOLD | 
                                AEECM_CALL_STATE_DORMANT),
                               NULL, 
                               0);
        
        ICM_Release(pICM);
        pICM = NULL;
    }
#else
   ITelephone *pITelephone = NULL;
   uint16 num = 0;

   if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
										   AEECLSID_TELEPHONE, 
										   (void **)&pITelephone))
   {
	   return FALSE;
   }
   if(pITelephone)
   {
	   
	   AEETCalls po;
	   
	   if(SUCCESS != ITELEPHONE_GetCalls(pITelephone, &po,sizeof(AEETCalls)))
	   {
		   return FALSE;
	   }
	   num = po.dwCount;
	   
	   ITELEPHONE_Release(pITelephone);
	   pITelephone = NULL;
   }
#endif
#ifdef FEATURE_APP_RECORDER
if(TRUE == recorder_GetRunningFlag())
{
        return FALSE;
}

#endif
    
    if(num > 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


