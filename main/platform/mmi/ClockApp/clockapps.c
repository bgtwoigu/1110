/*==============================================================================
// �ļ���
//        ClockApps.c
//        2007-11-12 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook CO.,LTD.
//
// �ļ�˵����
//
// ���ߣ�2007-11-12
// �������ڣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
when         who            what, where, why
----------   ---            ---------------------------------------------------
=============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "ClockApps_priv.h"
#include "oemnvint.h"
#include "OEMRTC.h"
#include "AEECM.h"

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/

#if defined(AEE_STATIC)
int ClockAppsMod_Load(IShell *ps, void * pHelpers, IModule ** pMod);
static int  ClockApps_CreateInstance(AEECLSID    ClsId,
                                    IShell     *pIShell,
                                    IModule    *po,
                                    void      **ppObj);
#endif // AEE_STATIC

static int ClockApps_New(IShell * pIShell,IModule * po,void ** ppObj);

static uint32  ClockApps_AddRef(IClockApps *pi);

static uint32  ClockApps_Release (IClockApps *pi);

static boolean ClockApps_HandleEvent(IClockApps *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static int ClockApps_InitAppData(CClockApps *pMe);

static void ClockApps_FreeAppData(CClockApps *pMe);

static void ClockApps_RunFSM(CClockApps *pMe);

static void ClockApps_Delay(CClockApps *pMe);
//��ʱ��ʱ�䵽��������/��
static boolean ClockApps_Expired(CClockApps *pMe, AlarmType eAlarmType, boolean justRegister);
static void ClockApps_APPIsReadyTimer(void *pme);
/*==============================================================================
                                 ȫ������
==============================================================================*/


/*==============================================================================
                                 ���أ���̬������
==============================================================================*/


// ֻ����һ��ClockAppsʵ����ÿ�δ���ClockApps Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CClockApps gClockApps;

static const VTBL(IClockApps) gClockAppsMethods =
{
    ClockApps_AddRef,
    ClockApps_Release,
    ClockApps_HandleEvent
};

/*==============================================================================
����:
       ClockAppsMod_Load

˵��:
       ģ��װ�غ�����

����:
       pIShell [in]��IShell�ӿ�ָ�롣
       ph ��û��ʹ��
       ppMod [out]��

����ֵ:
       int

��ע:

==============================================================================*/
#if defined(AEE_STATIC)
int ClockAppsMod_Load(IShell *ps, void * pHelpers, IModule ** pMod)
{

return(AEEStaticMod_New((int16)(sizeof(AEEMod)),ps,pHelpers,pMod,(PFNMODCREATEINST)ClockApps_CreateInstance,(PFNFREEMODDATA)NULL));
}
#endif

/*==============================================================================
����:
       ClockAppsMod_CreateInstance

˵��:
        ͨ������(��ʼ��)ClockApps����������ģ��ʵ����

����:
       po [in]��IModule�ӿڶ���ָ�롣
       pIShell [in]��IShell�ӿڶ���ָ�롣
       ClsId [in]��Ҫ������ʵ������ID��
       ppObj [out]:�����ڱ������ʵ����ָ���ָ�������

����ֵ:
       EFAILED;����ʧ�ܡ�
       SUCCESS;�����ɹ���

��ע:
       �����û�ȥ���á�

==============================================================================*/
#if defined(AEE_STATIC)
static int  ClockApps_CreateInstance(AEECLSID    ClsId,
    IShell *pIShell,
                                    IModule    *po,
                                    void      **ppObj)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_ALARMCLOCK)
    {
        return EFAILED;
    }

    if (ClockApps_New(pIShell, po, ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}
#endif // AEE_STATIC
/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����:
       ClockApps_New

˵��:
        ��ʼ��ClockApps Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IClockApps����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int ClockApps_New(IShell * pIShell,IModule * po,void ** ppObj)
{
    CClockApps *pMe;
    int retVal = SUCCESS;

    pMe = &gClockApps;

    if (0 == pMe->m_nRefs)
    {
        // Must initialize the object
        MEMSET(pMe,  0, sizeof(CClockApps));
        INIT_VTBL(pMe, IClockApps, gClockAppsMethods);

        pMe->m_nRefs     = 0;
        pMe->m_pShell    = pIShell;
        pMe->m_pModule   = po;

        (void) ISHELL_AddRef(pMe->m_pShell);
        (void) IMODULE_AddRef(pMe->m_pModule);

        retVal = ClockApps_InitAppData(pMe);
    }

    ++pMe->m_nRefs;
    *ppObj = (IClockApps*)pMe;
    return retVal;
}

/*==============================================================================
����:
       ClockApps_AddRef

˵��:
       ClockApps Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��IClockApps�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��ClockApps Applet��������

��ע:

==============================================================================*/
static uint32  ClockApps_AddRef(IClockApps *pi)
{
    register CClockApps *pMe = (CClockApps*)pi;

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       ClockApps_Release

˵��:
       �ͷŶ�ClockApps Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��IClockApps�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��ClockApps Applet��������

��ע:

==============================================================================*/
static uint32  ClockApps_Release (IClockApps *pi)
{
    register CClockApps *pMe = (CClockApps*)pi;

//    ASSERT(pMe != NULL);

    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    // �ͷ�Applet�����Դ
    ClockApps_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       ClockApps_InitAppData

˵��:
       ��ʼ��ClockApps Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

==============================================================================*/
static int ClockApps_InitAppData(CClockApps *pMe)
{
    AEEDeviceInfo  di;

    if (NULL  == pMe)
    {
        return EFAILED;
    }
    ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
    pMe->m_rc.x       = 0;
    pMe->m_rc.y       = 0;
    pMe->m_rc.dx      = (int16) di.cxScreen;
    pMe->m_rc.dy      = (int16) di.cyScreen;

    pMe->m_bAppIsReady = FALSE;
    pMe->m_ePreState = ALARMST_NONE;
    pMe->m_eCurState = ALARMST_INIT;
    pMe->m_pActiveDlg = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    pMe->m_nCtlID = IDC_CLOCK_ST_LIST;   //�ؼ�ID
    pMe->m_nTimeCtlCount = 0;    //0��������Сʱ��,1�����ڷ�����
#ifdef FEATURE_SUB_LCD
    pMe->m_pWallSubImage = NULL;
#endif //FEATURE_SUB_LCD

    pMe->m_currDlgId = 0;

    pMe->m_nNumKeyCount = 0;    //ʱ��ؼ���������
    pMe->m_dwDispTime = 0;
    pMe->m_dwHour = 0;
    pMe->m_dwMin = 0;

	pMe->m_bAlarmOff = TRUE;
    pMe->m_bAlarmSuspend = FALSE;

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_CONFIG,
                                            (void **)&pMe->m_pConfig))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_BACKLIGHT,
                                            (void **)&pMe->m_pBacklight))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }

#if !defined( AEE_SIMULATOR)
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_ANNUNCIATOR,
                                            (void **)&pMe->m_pIAnn))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }
	IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
#endif

#if defined( FEATURE_CARRIER_SUDAN_SUDATEL) && !defined( AEE_SIMULATOR)
    ICONFIG_GetItem(pMe->m_pConfig, CFGI_LANGUAGE_SELECTION, &pMe->language, sizeof(pMe->language));
#endif

#if !defined( AEE_SIMULATOR)
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_ALERT,
                                            (void **)&pMe->m_pAlert))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }
#endif

#ifdef FEATURE_SUB_LCD
    // ����IDisplaySub�ӿ�
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_DISPLAYSUB,
                                            (void **)&pMe->m_pDisplaySub))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }
#endif
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_SOUND,
                                            (void **)&pMe->m_pSound))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }
    //���ӿ����LIST �ؼ�
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                         AEECLSID_LISTCTL,
                         (void **)&pMe->m_pState))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }
    
    //ʱ��ؼ�
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                         AEECLSID_COUNTDOWNCTL,
                         (void **)&pMe->m_pTime))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }
    
    //ѭ����ʽLIST �ؼ�
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                         AEECLSID_LISTCTL,
                         (void **)&pMe->m_pRepMode))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }
    
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                         AEECLSID_LISTCTL,
                         (void **)&pMe->m_pSnooze))
    {
        ClockApps_FreeAppData(pMe);
        return EFAILED;
    }
//end added

    return SUCCESS;
}

/*==============================================================================
����:
       ClockApps_FreeAppData

˵��:
       �ͷ�ClockApps Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void ClockApps_FreeAppData(CClockApps *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }

    ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);

    if (pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }

    if (pMe->m_pBacklight)
    {
        IBACKLIGHT_Release(pMe->m_pBacklight);
        pMe->m_pBacklight = NULL;
    }

	if (pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
		pMe->m_pIAnn = NULL;
    }

#if !defined( AEE_SIMULATOR)
    if (pMe->m_pAlert)
    {
        IALERT_Release(pMe->m_pAlert);
        pMe->m_pAlert = NULL;
    }
#endif

#ifdef FEATURE_SUB_LCD
    if (pMe->m_pDisplaySub != NULL)
    {
        IDISPLAY_Release(pMe->m_pDisplaySub);
        pMe->m_pDisplaySub = NULL;
    }
#endif

    if (pMe->m_pSound != NULL)
    {
        ISound_Release(pMe->m_pSound);
        pMe->m_pSound = NULL;
    }
    if(pMe->m_pState != NULL)
    {
        (void)IMENUCTL_Release(pMe->m_pState);
        pMe->m_pState = NULL;
    }
    if(pMe->m_pTime != NULL)
    {
        (void)ITIMECTL_Release(pMe->m_pTime);
        pMe->m_pTime = NULL;
    }
    if(pMe->m_pRepMode != NULL)
    {
        (void)IMENUCTL_Release(pMe->m_pRepMode);
        pMe->m_pRepMode = NULL;
    }
    if(pMe->m_pSnooze != NULL)
    {
        (void)IMENUCTL_Release(pMe->m_pSnooze);
        pMe->m_pSnooze = NULL;
    }
//end added
}

/*==============================================================================
����:
       ClockApps_RunFSM

˵��:
       ClockApps Applet����״̬�����档

����:
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void ClockApps_RunFSM(CClockApps *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = ClockApps_ProcessState(pMe);

        if (pMe->m_bNotOverwriteDlgRet)
        {
            pMe->m_bNotOverwriteDlgRet = FALSE;
        }
        else
        {
            pMe->m_eDlgRet = DLGRET_CREATE;
        }

        if (nextFSMAction == NFSMACTION_WAIT)
        {
            break;
        }
    }
}

/*==============================================================================
����:
       ClockApps_HandleEvent

˵��:
       ClockApps  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��IClockApps�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
#define EVT_REREGISTER_ALARMS   EVT_USER + 20
static void reregisterAlarms( CClockApps *pMe)
{

	int i = 0;

	for(i = 0; i < NUM_OF_ALARMCLOCK; i++)
	{
		// ִ������
		ClockApps_Expired(pMe, i, TRUE);
	}
}
static boolean ClockApps_HandleEvent(IClockApps *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CClockApps *pMe = (CClockApps*)pi;
    AEEAppStart *as;

    DBGPRINTF("eCode = %x,wParam = %d",eCode, wParam);

    switch (eCode)
    {
        case EVT_APP_START:
            pMe->m_bAppIsReady = FALSE;
//            ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
//            if (NULL != pMe->m_pDisplay)
//            {
//                (void) IDISPLAY_Release(pMe->m_pDisplay);
//                pMe->m_pDisplay = NULL;
//            }
            pMe->m_pDisplay = as->pDisplay;
//            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;

            {
                AEEDeviceInfo di; 
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dy = di.cyScreen;
            }

            pMe->m_bSuspending = FALSE;
            // ��ʼClockApps״̬��
            ClockApps_RunFSM(pMe);
            return TRUE;
        // ��ʱ��ʱ�䵽

        case EVT_ALARM:
            {
#if 0
            uint16 i;
#endif

            DBGPRINTF( ";------evt_alarm, %d", pMe->m_bAlarmOff);

#if 0
            if (!ClockApps_CanAlert(pMe))
            {
                // ��ͨ��������ԭ���ݲ����������������Ƴ�2����
                pMe->m_eCurAlarmType = (AlarmType)wParam;
                //ClockApps_Delay(pMe);
            }
            else if( pMe->m_bAlarmOff)
#endif//���������ͨ����ҲҪִ�����ӣ�ֻ�����ֵķ�ʽ��ͬ�����������ﲻ������
            if(pMe->m_bAlarmOff)
            {
                pMe->m_bAlarmOff = FALSE;

#if 1
                ClockApps_Expired(pMe, wParam, FALSE);
#else
                    // Ϊ�˱������������ظ������ӺͶ�������������Щ���Ӳ�
                    // �����ֵ������,ÿ�ζ�ȫ���ж�һ��,�����ظ���������
                    // ִ��,�������������ӱ�Ŵ���Ǹ�.
                    for(i = 0; i < NUM_OF_ALARMCLOCK; i++)
                    {
                        // ִ������
                        ClockApps_Expired(pMe, i);
                    }
#endif

                ISHELL_PostEvent( pMe->m_pShell, AEECLSID_ALARMCLOCK, EVT_REREGISTER_ALARMS, 0, 0);

            }
        }
        return TRUE;
        case EVT_ALARMOFF:
            if(pMe->m_eCurState == ALARMST_REACH)
            {
                pMe->m_bAlarmSuspend = TRUE;
            }
            return TRUE;
        case EVT_REREGISTER_ALARMS:
        {
#if defined( FEATURE_POWERDOWN_ALARM)
            registerPowerdownAlarmclock();
#endif
            reregisterAlarms( pMe);
        }
            return TRUE;

        case EVT_APP_STOP:
//            if (pMe->m_pDisplay != NULL)
//            {
//                (void) IDISPLAY_Release(pMe->m_pDisplay);
//                pMe->m_pDisplay = NULL;
//            }
            pMe->m_bSuspending = TRUE;

            return TRUE;

        case EVT_APP_SUSPEND:
            pMe->m_bSuspending = TRUE;

            return TRUE;

        case EVT_APP_RESUME:
//            ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
            pMe->m_bSuspending = FALSE;

//            if (pMe->m_pDisplay != NULL)
//            {
//                (void) IDISPLAY_Release(pMe->m_pDisplay);
//                pMe->m_pDisplay = NULL;
//            }
            pMe->m_pDisplay = as->pDisplay;
//            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            {
                AEEDeviceInfo di; 
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dy = di.cyScreen;
            }


            if( pMe->m_eCurState == ALARMST_OPTION)
            {
                pMe->m_eCurState = ALARMST_MAIN;
            }
            if(pMe->m_eCurState == ALARMST_REACH)
            {
                if(pMe->m_bAlarmSuspend)
                {
                    CClockApps_Snooze(pMe);
                    pMe->m_eCurState = ALARMST_EXIT;   
                    pMe->m_bAlarmSuspend = FALSE;
                }
            }

            ClockApps_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            if (wParam == OEM_IME_DIALOG)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            if (wParam == OEM_IME_DIALOG)
            {
                return TRUE;
            }
            return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            ClockApps_APPIsReadyTimer,
                            pMe);
            return TRUE;

        case EVT_APPISREADY:
            (void) ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_bAppIsReady = TRUE;
            return TRUE;

        case EVT_KEY_PRESS:
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN: 
#endif

            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY_RELEASE:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY_HELD:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_GSENSOR_SHAKE:
        case EVT_KEY:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);

/*        case EVT_PEN_DOWN_1:
        case EVT_PEN_MOVE:
        case EVT_PEN_DOWN:
//            ERR("HANDLE PEN EVENTS", 0, 0, 0);
            if (!pMe->m_bAppIsReady)
            {
//                ERR("APP IS NOT READY", 0, 0, 0);
                return TRUE;
            }
            return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);
*/
        case EVT_COMMAND:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == OEM_IME_DIALOG)
            {
               (void)ISHELL_PostEvent(pMe->m_pShell,
                                      AEECLSID_ALARMCLOCK,
                                      EVT_USER_REDRAW,
                                      0,
                                      0);
                return TRUE;
            }

            //pMe->m_AlarmReach = FALSE;

            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void) ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼClockApps״̬��
                ClockApps_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_USER:
		{
			if (wParam == AVK_CLR)
			{
				eCode = EVT_KEY;
			}
			else if (wParam == AVK_SELECT)
			{
				if (dwParam == 0)
				{
					eCode = EVT_KEY_PRESS;
				}
				else
				{
					eCode = EVT_COMMAND;
				}
			}
			return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);
		}
		return TRUE;
			
#endif

        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return ClockApps_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
       ClockApps_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCClockApps�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void ClockApps_APPIsReadyTimer(void *pme)
{
    CClockApps *pMe = (CClockApps *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_ALARMCLOCK,
                            EVT_APPISREADY,
                            0,
                            0);
}



/*==============================================================================
������
       AlarmClock_Expired
˵����
       ���������������������棬���趨�´�����ʱ�䡣��������Ե�ǰʱ����趨
       ʱ�����Ƚϣ���ȷ���Ƿ��ӳ����֣�����������30���ڡ�

������
       pMe [in]��ָ�� AlarmClock Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eAlarmType [in]���������͡�

����ֵ��
       �ޡ�

��ע��

==============================================================================*/
static boolean ClockApps_Expired(CClockApps *pMe, AlarmType eAlarmType, boolean justRegister)
{
    uint32 dwNow;
    uint32 dwAlarmTime;
    uint32 dwTepTime;
    uint32 dwSnooze;
    boolean  wWeekAlarmMask = FALSE;
    uint16 wValue;
    boolean bSetTimer = FALSE;

    // EVT_ALARM�¼���ʱ����ǰ�趨ʱ�������ӵ������������������bDelay����
    // ӦΪTRUE����ʾ���Ƴ����֡�
    boolean bDelay = FALSE;
    JulianType  jDate;
#ifdef FEATURE_UIALARM
    IAlarm *pAlarm = NULL;
#endif
#if defined( FEATURE_ONCE_ALARM)
    boolean continueAlarm    = 0;
#endif

    if (pMe == NULL)
    {
        return FALSE;
    }

#ifdef FEATURE_UIALARM
    if (SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                        AEECLSID_UIALARM,
                                        (void**)&pAlarm))
    {
        return FALSE;
    }
#endif
    //��ȡ������ѡ������ݽṹ
    (void) ISHELL_GetPrefs(pMe->m_pShell,
                           AEECLSID_ALARMCLOCK,
                           CLOCK_CFG_VERSION,
                           &pMe->m_ClockCfg,
                           sizeof(ClockAppCfg));

    //�Ƿ���������
    if (pMe->m_ClockCfg.bStateOn[eAlarmType])
    {
        bSetTimer = TRUE;       //�����Ѿ������˿���
    }

    if (!bSetTimer) // ȡ����ʱ��
    {
#ifdef FEATURE_UIALARM
        (void)IAlarm_CancelAlarm(pAlarm,
                AEECLSID_ALARMCLOCK,
                (uint16)eAlarmType);
        IAlarm_Release(pAlarm);
#else
        (void) ISHELL_CancelAlarm(pMe->m_pShell,
                AEECLSID_ALARMCLOCK,
                (uint16)eAlarmType);
#endif
		return FALSE;
    }

    //����ʵ�ʶ�ʱʱ��,������AEEAlarm���ж�ʱ������ʵ�ʹ���
    // ȡ��ǰʱ��
    dwNow = GETTIMESECONDS();
    GETJULIANDATE(dwNow, &jDate);

    wValue = pMe->m_ClockCfg.RepMode[eAlarmType];  //�ظ���ʽ

    switch (eAlarmType)
    {
        case WEEK_ALARM_1:
        case WEEK_ALARM_2:
        case WEEK_ALARM_3:
        case WEEK_ALARM_4:
        case WEEK_ALARM_5:
        case WEEK_ALARM_6:
        //case WEEK_ALARM_7:
        //case WEEK_ALARM_8:
            // �жϵ������������Ƿ�����
            switch (wValue)
            {
                case WEEK_ALARM_REP1:   // ÿ��
                    wWeekAlarmMask = TRUE;
                    break;

#if defined( FEATURE_CARRIER_ISRAEL_PELEPHONE)
                case WEEK_ALARM_REP2: // ����~~����
                    if( jDate.wWeekDay < 4 || jDate.wWeekDay == 6)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP3: // ����~~����
                    if( jDate.wWeekDay != 5)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP4: // ����
                    if(jDate.wWeekDay == 6)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP5: // ��1
                    if(jDate.wWeekDay == 0)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP6: // ��2
                    if(jDate.wWeekDay == 1)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP7: // ��3
                    if(jDate.wWeekDay == 2)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP8: // ��4
                    if(jDate.wWeekDay == 3)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP9: // ��5
                    if(jDate.wWeekDay == 4)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP10: // ��6
                    if(jDate.wWeekDay == 5)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;
#else
                case WEEK_ALARM_REP2: // ��һ~~����
                    if(jDate.wWeekDay > 4)//����������
                    {
                        wWeekAlarmMask = FALSE;
                    }
                    else
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP3: // ��һ~~����
                    if(jDate.wWeekDay > 5)//����
                    {
                        wWeekAlarmMask = FALSE;
                    }
                    else
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP4: // ��һ
                    if(jDate.wWeekDay == 0)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP5: // �ܶ�
                    if(jDate.wWeekDay == 1)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP6: // ����
                    if(jDate.wWeekDay == 2)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP7: // ����
                    if(jDate.wWeekDay == 3)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP8: // ����
                    if(jDate.wWeekDay == 4)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP9: // ����
                    if(jDate.wWeekDay == 5)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;

                case WEEK_ALARM_REP10: // ����
                    if(jDate.wWeekDay == 6)
                    {
                        wWeekAlarmMask = TRUE;
                    }
                    break;
#endif
                    
#if defined( FEATURE_ONCE_ALARM)
                case WEEK_ALARM_REP11:   // һ��
                    wWeekAlarmMask = TRUE;
                    break;
#endif

                default:
#ifdef FEATURE_UIALARM
                    IAlarm_Release(pAlarm);
#endif
                    return FALSE;
            }

            //����״̬������,�ҵ�������������,��������Ч
            if (pMe->m_ClockCfg.bStateOn[eAlarmType] && wWeekAlarmMask)
            {
                pMe->m_bWeekAlarmEnabled = TRUE;
            }
            else
            {
                pMe->m_bWeekAlarmEnabled = FALSE;
            }
            //����ʱ��
            dwAlarmTime = pMe->m_ClockCfg.dwWATime[eAlarmType];
            break;

        default:
#ifdef FEATURE_UIALARM
            IAlarm_Release(pAlarm);
#endif
			return FALSE;
    }

    // ��ʱ��ת��Ϊ�룬�����趨����ʱ��ֵdwAlarmTime
#if defined( FEATURE_ONCE_ALARM)
	dwAlarmTime = dwAlarmTime;
#else
	dwAlarmTime = dwAlarmTime / 1000;
#endif
    jDate.wHour = 0;
    jDate.wMinute = 0;
    jDate.wSecond = 0;
#if defined( FEATURE_ONCE_ALARM)
	if( pMe->m_ClockCfg.RepMode[eAlarmType] != WEEK_ALARM_REP11)
	{
		dwAlarmTime += JULIANTOSECONDS(&jDate);
	}
#else
	dwAlarmTime += JULIANTOSECONDS(&jDate);
#endif
    dwSnooze = (pMe->m_ClockCfg.Snooze[eAlarmType] + MAX_ALARM_MSECS/1000) * 10;
    debug("dwSnooze = %d",dwSnooze);
    debug("dwNow = %d;dwAlarmTime = %d",dwNow,dwAlarmTime);
    if (dwAlarmTime > dwNow)
    {
        // ��������ʱ��δ��
        dwTepTime = dwAlarmTime - dwNow;

        // Ϊ����������������10
        // �����ڣ���dwTepTimeС��10�����������֣����������ӳ�1���ӡ�
        // ����������趨ʱ��ǰ��10��������Ϊ������
        if (dwTepTime >= 10)
        {
            bDelay = TRUE;

#if defined( FEATURE_ONCE_ALARM)
			continueAlarm = TRUE;
#endif
        }
        else
        {
            // ʱ�����趨ʱ��������1���ӣ��������֣�������ʱ��������1��,
            // �������ʾ������������EVT_ALARM�¼�
            dwTepTime = 24 * 60 * 60;
        }
    }
    else if((dwNow - dwAlarmTime) <= dwSnooze)   //������10������
    {
        // ��������ʱ���ѹ����趨Ϊ����
        dwTepTime = dwAlarmTime + 24 * 60 * 60 - dwNow;
    }
    else   //С��
    {
        // ��������ʱ���ѹ����趨Ϊ����
        dwTepTime = dwAlarmTime + 24 * 60 * 60 - dwNow;
        bDelay = TRUE;
    }

#if defined( FEATURE_ONCE_ALARM)
    if( pMe->m_ClockCfg.RepMode[eAlarmType] == WEEK_ALARM_REP11 && !continueAlarm)
    {
        pMe->m_ClockCfg.bStateOn[eAlarmType] = 0;
        (void) ISHELL_SetPrefs(pMe->m_pShell,
                           AEECLSID_ALARMCLOCK,
                           CLOCK_CFG_VERSION,
                           &pMe->m_ClockCfg,
                           sizeof(ClockAppCfg));
    }
#endif

    // ���뻯Ϊ����
    //    dwTepTime /= 60;
    // Now the parameter have been changed to seconds instead of min

    if (dwTepTime == 0)
    {
        dwTepTime = 5;
    }

#if defined( FEATURE_ONCE_ALARM)
    if( pMe->m_ClockCfg.RepMode[eAlarmType] != WEEK_ALARM_REP11 ||
        ( pMe->m_ClockCfg.RepMode[eAlarmType] == WEEK_ALARM_REP11 && continueAlarm)
    )
    {
#ifdef FEATURE_UIALARM
        (void) IAlarm_SetAlarm(pAlarm,
                                AEECLSID_ALARMCLOCK,
                                (uint16)eAlarmType,
                                dwTepTime);
        IAlarm_Release(pAlarm);
#else
        (void) ISHELL_SetAlarm(pMe->m_pShell,
                               AEECLSID_ALARMCLOCK,
                                (uint16)eAlarmType,
                                dwTepTime);
#endif
    }
#else
#ifdef FEATURE_UIALARM
    (void) IAlarm_SetAlarm(pAlarm,
                            AEECLSID_ALARMCLOCK,
                            (uint16)eAlarmType,
                            dwTepTime);
    IAlarm_Release(pAlarm);
#else
    (void) ISHELL_SetAlarm(pMe->m_pShell,
                           AEECLSID_ALARMCLOCK,
                            (uint16)eAlarmType,
                            dwTepTime);
#endif
#endif

    if (bDelay)
    {
        return FALSE;
    }

    // ��������
    if (!pMe->m_bWeekAlarmEnabled || justRegister)
    {
        return FALSE;
    }
    
    if (!ClockApps_CanAlert(pMe))// ���������ͨ���о�ֻ��5�룬����������ʾ����
    {
        ISOUND_Vibrate(pMe->m_pSound, 2000);
        if(AEECLSID_DIALER == ISHELL_ActiveApplet(pMe->m_pShell))
        {
            ISHELL_PostEvent(pMe->m_pShell, AEECLSID_DIALER, EVT_ALARM, 0, 0);
        }
        return TRUE;
    }
    else
    {
        // REACH�Ի���Ҫ��ʾ������
        pMe->m_eCurAlarmType = (AlarmType)eAlarmType;

        MOVE_TO_STATE(ALARMST_REACH)    //ת������ʱ�䵽״̬
        // ��ǰ Applet ��δ���ڻ״̬������ Applet
        if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_ALARMCLOCK)
        {
            DBGPRINTF( ";-----to start alarm clock applet");
            (void) ISHELL_StartApplet(pMe->m_pShell, AEECLSID_ALARMCLOCK);
        }
        else
        {
            if(ISHELL_GetActiveDialog(pMe->m_pShell)
            != pMe->m_pActiveDlg)
            {
                (void)ISHELL_EndDialog(pMe->m_pShell);
            }

            CLOSE_DIALOG(DLGRET_CREATE);

        }
    }
    return TRUE;
}


/*==============================================================================
������
       ClockApps_CanAlert
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
boolean ClockApps_CanAlert(CClockApps *pMe)
{
#if 0    
    db_items_value_type dbItemValue = {0};
    db_get( DB_IN_USE, &dbItemValue);
    if( dbItemValue.in_use)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
#endif   // #if 0   db.in_useҲ������wap����ʱ��״̬����������жϷ���ʵ��
    
    ICM *pICM = NULL;
    uint16 num = 0;
 
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                            AEECLSID_CM, 
                                            (void **)&pICM))
    {
        return FALSE;
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

    if(num > 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}



/*==============================================================================
������
       ClockApps_Delay
˵����
       ���������������Ƴ�2����

������
       pMe [in]��ָ�� AlarmClock Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       ��

��ע��

==============================================================================*/
static void ClockApps_Delay(CClockApps *pMe)
{
#ifdef FEATURE_UIALARM
    IAlarm *pAlarm = NULL;
#endif

    if (NULL == pMe)
    {
        return;
    }

#ifdef FEATURE_UIALARM
    if (SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
        AEECLSID_UIALARM,
        (void**)&pAlarm))
    {
        return;
    }
#endif

    //������ʾ������ʱ���������2����
    //...

#ifdef FEATURE_UIALARM
    (void) IAlarm_SetAlarm(pAlarm,
                            AEECLSID_ALARMCLOCK,
                            (uint16)pMe->m_eCurAlarmType,
                            (120 - 1));//����delayʱ���õ�1�����
    //��ȡ������ѡ������ݽṹ
    (void) ISHELL_GetPrefs(pMe->m_pShell,
                           AEECLSID_ALARMCLOCK,
                           CLOCK_CFG_VERSION,
                           &pMe->m_ClockCfg,
                           sizeof(ClockAppCfg));
    //�����������õ�ʱ��
    pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] += 2*MSSEC_PER_MIN;

    //�洢������ѡ������ݽṹ
    (void) ISHELL_SetPrefs(pMe->m_pShell,
                           AEECLSID_ALARMCLOCK,
                           CLOCK_CFG_VERSION,
                           &pMe->m_ClockCfg,
                           sizeof(ClockAppCfg));
    IAlarm_Release(pAlarm);
#else
    (void) ISHELL_SetAlarm(pMe->m_pShell,
                            AEECLSID_ALARMCLOCK,
                            (uint16)pMe->m_eCurAlarmType,
                            120);
#endif
}

// when restore factory settings, the ClockAppCfg SHALL be zero-cleared
void ClockApps_ClearAll(void)
{
    ClockAppCfg cfg;

    MEMSET( &cfg, 0, sizeof( cfg));
    ISHELL_SetPrefs( AEE_GetShell(),
               AEECLSID_ALARMCLOCK,
               CLOCK_CFG_VERSION,
               &cfg,
               sizeof(ClockAppCfg)
           );
}


#if defined( FEATURE_POWERDOWN_ALARM)
static int registerPowerdownAlarmclockIf( uint32 now)
{
    ClockAppCfg clockCfg        = {0};
    uint32      today           = 0;
    uint32      last            = 0xffffffff;
    uint32      time            = 0;
    boolean     wWeekAlarmMask  = 0;
    boolean     registered      = 0;
    JulianType  jdate           = {0};
    int         result          = 0;
    int         i               = 0;
#ifdef FEATURE_APP_WORLDTIME
    extern boolean Calendar_FormatDateTime( uint32 seconds, AECHAR* resultString, int resultStringLength);
#endif /*FEATURE_APP_WORLDTIME*/
    DBGPRINTF( ";---------------------");
    DBGPRINTF( ";try to register powerdown alarmclock");

    GETJULIANDATE( now, &jdate);
    DBGPRINTF( ";now is %d%02d%02d %02d:%02d, weekday %d", jdate.wYear, jdate.wMonth, jdate.wDay, jdate.wHour, jdate.wMinute, jdate.wWeekDay + 1);
    jdate.wHour = jdate.wMinute = jdate.wSecond = 0;
    today = JULIANTOSECONDS( &jdate);
    if( now == 0 || today == 0)
    {
        DBGPRINTF( ";now = %d, today = %d, do NOT register", now, today);
        return -1;
    }
    result = ISHELL_GetPrefs( AEE_GetShell(),
                    AEECLSID_ALARMCLOCK,
                    CLOCK_CFG_VERSION,
                    &clockCfg,
                    sizeof(ClockAppCfg)
               );

    if( result !=SUCCESS)
    {
        DBGPRINTF( ";ISHELL_GetPrefs failed!");
        i = NUM_OF_ALARMCLOCK;
    }

    {
        PowerDown_Alarm_Cfg alarmCfg = {0};
        OEM_SetConfig(CFGI_POWERDOWN_ALARM, (void*)&alarmCfg, sizeof(alarmCfg));
    }

    for( i = 0; i < NUM_OF_ALARMCLOCK ; i++)
    {

        if( !clockCfg.bStateOn[i])
        {
            DBGPRINTF( ";clock[%d] is off", i);
            continue;
        }

        switch ( clockCfg.RepMode[i])
        {
            case WEEK_ALARM_REP1:   // ÿ��
                DBGPRINTF( ";clock[%d] repeat everyday", i);
                wWeekAlarmMask = TRUE;
                break;

#if defined( FEATURE_CARRIER_ISRAEL_PELEPHONE)
				case WEEK_ALARM_REP2: // ����~~����
					if( jdate.wWeekDay < 4 || jdate.wWeekDay == 6)
					{
						wWeekAlarmMask = TRUE;
					}
					break;

				case WEEK_ALARM_REP3: // ����~~����
					if( jdate.wWeekDay != 5)
					{
						wWeekAlarmMask = TRUE;
					}
					break;

				case WEEK_ALARM_REP4: // ����
					if(jdate.wWeekDay == 6)
					{
						wWeekAlarmMask = TRUE;
					}
					break;

				case WEEK_ALARM_REP5: // ��1
					if(jdate.wWeekDay == 0)
					{
						wWeekAlarmMask = TRUE;
					}
					break;

				case WEEK_ALARM_REP6: // ��2
					if(jdate.wWeekDay == 1)
					{
						wWeekAlarmMask = TRUE;
					}
					break;

				case WEEK_ALARM_REP7: // ��3
					if(jdate.wWeekDay == 2)
					{
						wWeekAlarmMask = TRUE;
					}
					break;

				case WEEK_ALARM_REP8: // ��4
					if(jdate.wWeekDay == 3)
					{
						wWeekAlarmMask = TRUE;
					}
					break;

				case WEEK_ALARM_REP9: // ��5
					if(jdate.wWeekDay == 4)
					{
						wWeekAlarmMask = TRUE;
					}
					break;

				case WEEK_ALARM_REP10: // ��6
					if(jdate.wWeekDay == 5)
					{
						wWeekAlarmMask = TRUE;
					}
					break;
#else
				case WEEK_ALARM_REP2: // ��һ~~����
                if(jdate.wWeekDay > 4)//����������
                {
                    wWeekAlarmMask = FALSE;
                }
                else
                {
                    wWeekAlarmMask = TRUE;
                }
                break;

            case WEEK_ALARM_REP3: // ��һ~~����
                if(jdate.wWeekDay > 5)//����
                {
                    wWeekAlarmMask = FALSE;
                }
                else
                {
                    wWeekAlarmMask = TRUE;
                }
                break;

            case WEEK_ALARM_REP4: // ��һ
                if(jdate.wWeekDay == 0)
                {
                    wWeekAlarmMask = TRUE;
                }
                break;

            case WEEK_ALARM_REP5: // �ܶ�
                if(jdate.wWeekDay == 1)
                {
                    wWeekAlarmMask = TRUE;
                }
                break;

            case WEEK_ALARM_REP6: // ����
                if(jdate.wWeekDay == 2)
                {
                    wWeekAlarmMask = TRUE;
                }
                break;

            case WEEK_ALARM_REP7: // ����
                if(jdate.wWeekDay == 3)
                {
                    wWeekAlarmMask = TRUE;
                }
                break;

            case WEEK_ALARM_REP8: // ����
                if(jdate.wWeekDay == 4)
                {
                    wWeekAlarmMask = TRUE;
                }
                break;

            case WEEK_ALARM_REP9: // ����
                if(jdate.wWeekDay == 5)
                {
                    wWeekAlarmMask = TRUE;
                }
                break;

            case WEEK_ALARM_REP10: // ����
                if(jdate.wWeekDay == 6)
                {
                    wWeekAlarmMask = TRUE;
                }
                break;
#endif
                
            case WEEK_ALARM_REP11: // һ��
                DBGPRINTF( ";clock[%d] repeat once", i);
                wWeekAlarmMask = TRUE;
                break;  

            default:
                DBGPRINTF( ";clock[%d] repeat is invalid", i);
                wWeekAlarmMask = FALSE;
                break;
        }

        if( !wWeekAlarmMask)
        {
            DBGPRINTF( ";clock[%d] today is off, %d", i, jdate.wWeekDay + 1);
            continue;
        }

#if defined( FEATURE_ONCE_ALARM)
		if( clockCfg.RepMode[i] == WEEK_ALARM_REP11)
		{
			time = clockCfg.dwWATime[i];
		}
		else
		{
			time = clockCfg.dwWATime[i] + today;
		}
#else
		time = clockCfg.dwWATime[i] / 1000 + today;
#endif
        DBGPRINTF( ";clock[%d].time = %d, now = %d, last>time:%d", i, time, now, last > time);
        if( time > now && last > time)
        {
            PowerDown_Alarm_Cfg alarmCfg    = {0};
            AECHAR              text[64]    = {0};

            last                = time;
            alarmCfg.bStateOn   = TRUE;
            alarmCfg.alarm_id   = i;
            alarmCfg.dwWATime   = last;
            OEM_SetConfig( CFGI_POWERDOWN_ALARM, (void*)&alarmCfg, sizeof(alarmCfg));
#ifdef FEATURE_APP_WORLDTIME
            Calendar_FormatDateTime( time, text, sizeof( text));
#endif /*FEATURE_APP_WORLDTIME*/

            DBGPRINTF( ";set CFGI_POWERDOWN_ALARM, id=%d, time=%S", alarmCfg.alarm_id, text);

            registered ++;
        }
    }
#ifndef WIN32
#if !defined( AEE_SIMULATOR)
    if( registered)
    {
        OEMRTC_Process_PowerDown_Alarm();
    }
#endif
#endif//WIN32
    return registered;
}

void registerPowerdownAlarmclock( void)
{
    uint32      now             = 0;
    uint32      tommorrow       = 0;
    JulianType  jdate           = {0};
    int         result          = 0;

    DBGPRINTF( ";-------------------------------------------");
    DBGPRINTF( ";to register powerdown alarmclock");
#ifndef WIN32
    if( !OEMRTC_Get_Current_Time( &now))
    {
        DBGPRINTF( ";now is invalid: %d", now);
        return;
    }
#endif//WIN32
	result = registerPowerdownAlarmclockIf( now);
    DBGPRINTF( ";today register result is %d", result);
    DBGPRINTF( ";---------------------");
    if( result == 0)
    {
        GETJULIANDATE( now, &jdate);
        jdate.wHour = jdate.wMinute = jdate.wSecond = 0;
        tommorrow = JULIANTOSECONDS( &jdate) + 24*60*60;

        result = registerPowerdownAlarmclockIf( tommorrow);
        DBGPRINTF( ";tommorrow register result is %d", result);
        DBGPRINTF( ";---------------------");
    }
    DBGPRINTF( ";-------------------------------------------");
}

void snoozePowerdownAlarmclock( void)
{
    ClockAppCfg         clockCfg    = {0};
    PowerDown_Alarm_Cfg alarmCfg    = {0};
    int                 result      = 0;

    AECHAR text[64] = {0};
#ifdef FEATURE_APP_WORLDTIME    
    extern boolean Calendar_FormatDateTime( uint32 seconds, AECHAR* resultString, int resultStringLength);
#endif /*FEATURE_APP_WORLDTIME*/

    DBGPRINTF( ";-----------------------------------------------------------");
    DBGPRINTF( ";snoozePowerdownAlarmClock");

    result = ISHELL_GetPrefs( AEE_GetShell(),
                    AEECLSID_ALARMCLOCK,
                    CLOCK_CFG_VERSION,
                    &clockCfg,
                    sizeof(ClockAppCfg)
               );

    if( result == SUCCESS)
    {
        uint32      oldtime = 0;
        uint32      newtime = 0;
        uint32      now     = 0;
        uint32      today   = 0;
        uint32      snooze  = 0;
        JulianType  jdate   = {0};
#ifndef WIN32
        if( !OEMRTC_Get_Current_Time( &now))
        {
            return;
        }
#endif//WIN32        
        snooze = clockCfg.Snooze[alarmCfg.alarm_id];
        if(0 == snooze)
        {
            return;
        }
        
        GETJULIANDATE( now, &jdate);
        jdate.wHour = jdate.wMinute = jdate.wSecond = 0;
        today = JULIANTOSECONDS( &jdate);

        OEM_GetConfig( CFGI_POWERDOWN_ALARM, (void*)&alarmCfg, sizeof(alarmCfg));

        oldtime = clockCfg.dwWATime[alarmCfg.alarm_id];
#if defined( FEATURE_ONCE_ALARM)
		if( clockCfg.RepMode[alarmCfg.alarm_id] == WEEK_ALARM_REP11)
		{
			newtime = oldtime;
		}
		else
		{
			newtime = oldtime + today;
		}
#else
		newtime = oldtime / 1000 + today;
#endif
#ifdef FEATURE_APP_WORLDTIME
        Calendar_FormatDateTime( newtime, text, sizeof( text));
#endif /*FEATURE_APP_WORLDTIME*/

        DBGPRINTF( ";alarm time: %S", text);
        while( newtime < now)
        {
            newtime += snooze;
        }
#ifdef FEATURE_APP_WORLDTIME        
        Calendar_FormatDateTime( newtime, text, sizeof( text));
#endif /*FEATURE_APP_WORLDTIME*/

        DBGPRINTF( ";snooze time: %S", text);

#if defined( FEATURE_ONCE_ALARM)
		if( clockCfg.RepMode[alarmCfg.alarm_id] == WEEK_ALARM_REP11)
		{
			clockCfg.dwWATime[alarmCfg.alarm_id] = newtime;
		}
		else
		{
			clockCfg.dwWATime[alarmCfg.alarm_id] = newtime - today;
		}
#else
		clockCfg.dwWATime[alarmCfg.alarm_id] = (newtime - today) * 1000;
#endif
        ISHELL_SetPrefs( AEE_GetShell(), AEECLSID_ALARMCLOCK, CLOCK_CFG_VERSION, &clockCfg, sizeof(ClockAppCfg));

        registerPowerdownAlarmclock();

        clockCfg.dwWATime[alarmCfg.alarm_id] = oldtime;
        ISHELL_SetPrefs( AEE_GetShell(), AEECLSID_ALARMCLOCK, CLOCK_CFG_VERSION, &clockCfg, sizeof(ClockAppCfg));
    }
    else
    {
        DBGPRINTF( ";get alarm data failed");
    }
    DBGPRINTF( ";-----------------------------------------------------------");
}

void registerAgainPowerdownAlarmclock( void)
{
    PowerDown_Alarm_Cfg     alarmCfg    = {0};
    uint32                  now         = 0;
#ifndef WIN32
    if( !OEMRTC_Get_Current_Time( &now))
    {
        return;
    }
#endif//WIN32
    OEM_GetConfig( CFGI_POWERDOWN_ALARM, (void*)&alarmCfg, sizeof(alarmCfg));
    if( alarmCfg.dwWATime >= now)
    {
#ifndef WIN32
        OEMRTC_Process_PowerDown_Alarm();
#endif//WIN32
    }
    else
    {
        registerPowerdownAlarmclock();
    }
}
#endif
