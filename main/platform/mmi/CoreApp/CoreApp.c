/*==============================================================================
//           U S E R   I N T E R F A C E   M A I N  M O D U L E
// �ļ�:
//        CoreApp.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵��:
//        ��Ӧ���� UI ����ʼ�㣬�ڿ���ʱ�� BREW �Զ�������         
//
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

#include "BREWVersion.h"
#include "AEE.h"
#include "AEEShell.h"
#include "AEEStdlib.h"
#include "AEEModTable.h"
#include "AEEAppGen.h"
#include "AEEAnnunciator.h"

#include "CoreApp_priv.h"        /* CoreApp external data and function prototypes */

#include "AEEClipboard.h"
#include "appscommonimages.brh"

#include "Appscommon.h"//wlh add
#include "ui.h"
#if defined(FEATURE_WMS_APP)
#include "WMSApp.h"
#endif
#include "OEMRTC.h"
#include "AEEDownload.h"
#include "OEMDeviceNotifier.h"

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/


/*==============================================================================
                                 
                                �ڲ��������Ͷ���
                                 
==============================================================================*/
#ifdef AEE_STATIC

CCoreApp *g_pCoreApp = NULL; 

#endif
/*==============================================================================

                                 ��������
                                 
==============================================================================*/

#ifdef FEATURE_SUPPORT_WAP_APP
boolean PushMod_StartPush(IShell *ps);
#endif

static void CoreApp_RunFSM(CCoreApp *pMe);
static boolean CoreApp_HandleNotify(CCoreApp * pMe, AEENotify *pNotify);
static boolean CoreApp_HandleCMNotify(CCoreApp * pMe, AEENotify *pNotify);
static boolean CoreApp_HandleBattNotify(CCoreApp * pMe, AEENotify *pNotify);
static void CoreApp_PoweronStartApps(CCoreApp *pMe);
static int DBToLevel (int nDBVal);
static void CCharger_EnableICONCB(void *pUser);
static void CCharger_BlinkLowBattIcon(void *pUser);
#ifdef FEATRUE_AUTO_POWER
static void CoreApp_Process_AutoPower_Event(void *pUser);
#endif

static void CoreApp_Process_Batty_Msg(CCoreApp   *pMe, uint16  msg_id);
static void CoreApp_Process_Batty_Msg_CB(void *pp);
#ifdef FEATURE_ICM
static void CoreApp_Process_SS_info(CCoreApp * pMe ,AEECMSSEventData *ss);
#else
static void CoreApp_Process_SS_info(CCoreApp * pMe ,AEETSSEventData *ss);
#endif
static void CoreApp_Process_Charger_Msg(CCoreApp   *pMe);
static uint32 CoreApp_ConvertBattLvToAnnunState(int nBattLevel);
//end added
static void CoreNotifyMP3PlayerAlertEvent(CCoreApp *pMe, boolean toStartAlert);
static void CoreNotifyMP3PlayerAlertEventCB(CCoreApp *pMe);

static void CoreAppReadNVKeyBeepValue(CCoreApp *pMe);
#ifdef CUST_EDITION
 void CoreApp_InitBattStatus(CCoreApp * pMe);
#endif
//static void CoreAppLoadTimepImage(CCoreApp *pMe);   //add by ydc
boolean CoreApp_ProcessOffLine(CCoreApp *pMe);
void SendRTREConfig (CCoreApp *pMe);
static boolean CoreApp_ProcessFTMMode(CCoreApp *pMe);
static void CoreApp_Process_Headset_Msg(CCoreApp *pMe, uint16 msgId);
static boolean CoreApp_GetCardStatus(CCoreApp *pMe,uint8 slot);
#ifdef USES_CONSTEFS
static void CoreApp_RebuildEFS(CCoreApp *pMe);
#endif
static void StereoHeadsetOn(CCoreApp * pMe);
static void HeadsetOff(CCoreApp *pMe);
static void CoreApp_SceneMode(CCoreApp *pMe);
//add by yangdecai  2010-09-10
static void CoreApp_ResetProfileTimer(void *pUser);
static boolean CoreApp_TestCard(CCoreApp *pMe);
static void    CoreApp_ResetRing(CCoreApp *pMe);
//add by yangdecai end
/*==============================================================================

                                 ��������
                                 
==============================================================================*/
/*==============================================================================
����: 
    CoreApp_IsIdle
       
˵��: 
    �ж��Ƿ���IDLE  ״̬��
           
����: 
    void    
    
����ֵ:
    TRUE: ����IDLE  ״̬��
    FALSE:
       
��ע:

==============================================================================*/
boolean CoreApp_IsIdle(void)
{
    if (g_pCoreApp)        
    {
    	return (!g_pCoreApp->m_bSuspended && IDD_IDLE == g_pCoreApp->m_wActiveDlgID);
    }

    return FALSE;
}

/*==============================================================================
����:
    CoreApp_FreeAppData
       
˵��: 
    ���������ͷ�Ӧ�õ����ݳ�Ա��
       
����: 
    po [in]: IApplet �ӿ�ָ�롣
    
����ֵ:
    none
       
��ע:

==============================================================================*/
void CoreApp_FreeAppData(IApplet* po)
{
    CCoreApp *pMe = (CCoreApp*)po;

    if (pMe->m_pIAnn) 
    { 
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn= NULL;
    }
	
#ifdef FEATURE_UIALARM    
    // �ͷ� IAlarm �ӿ�
    if (pMe->m_pAlarm != NULL)
    {
        IAlarm_Release(pMe->m_pAlarm);
        pMe->m_pAlarm = NULL;
    }
#endif 

    if (pMe->m_pAlert != NULL)
    {
        IALERT_Release(pMe->m_pAlert);
        pMe->m_pAlert = NULL;
    }
    // �ͷ� IBatt �ӿ�
    if (pMe->m_pBatt != NULL)
    {
        IBATTERY_Release(pMe->m_pBatt);
        pMe->m_pBatt = NULL;
    }
    
    // �ͷ� IRUIM �ӿ�
    if (pMe->m_pIRUIM != NULL)
    {
        IRUIM_Release(pMe->m_pIRUIM);
        pMe->m_pIRUIM = NULL;
    }
    
    /* ICard */
    if (pMe->m_pICard) 
    {
        ICARD_Release(pMe->m_pICard);
        pMe->m_pICard = NULL;
    }
#ifdef FEATURE_ICM
    // �ͷ� ICM �ӿ�
    if (pMe->m_pCM != NULL)
    {
        (void)ISHELL_RegisterNotify(pMe->a.m_pIShell, 
                                    AEECLSID_CORE_APP,
                                    AEECLSID_CM_NOTIFIER, 
                                    0);
        ICM_Release(pMe->m_pCM);
        pMe->m_pCM = NULL;
    }
#else
    if (pMe->m_pITelephone != NULL)
    {
        (void)ISHELL_RegisterNotify(pMe->a.m_pIShell, 
                                    AEECLSID_CORE_APP,
                                    AEECLSID_PHONENOTIFIER, 
                                    0);
        ITELEPHONE_Release(pMe->m_pITelephone);
        pMe->m_pITelephone = NULL;
    }

	 if (pMe->m_pICallMgr != NULL)
    {
        ICALLMGR_Release(pMe->m_pICallMgr);
        pMe->m_pICallMgr = NULL;
    }
		
	if (pMe->m_pIPhoneCtl != NULL)
    {
        IPHONECTL_Release(pMe->m_pIPhoneCtl);
        pMe->m_pIPhoneCtl = NULL;
    }
#endif
    
    if (pMe->m_pConfig) 
    {
        (void)ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }

    if(pMe->m_pBacklight)
    {
        IBACKLIGHT_Release(pMe->m_pBacklight);
        pMe->m_pBacklight=NULL;
    }

    if (pMe->m_pDisplay != NULL)
    {
        IDISPLAY_Release(pMe->m_pDisplay);
        pMe->m_pDisplay = NULL;
    }
	
#ifndef WIN32
    OEMRTC_Free_All_Node();
#endif//WIN32

    ISHELL_RegisterNotify(pMe->a.m_pIShell,AEECLSID_CORE_APP,  AEECLSID_ALERT_NOTIFIER,0);

    FREEIF(pMe->m_cdg_msgptr);
    if(pMe->m_pAddrPhone)
    {
        IADDRBOOK_Release(pMe->m_pAddrPhone);
        pMe->m_pAddrPhone = NULL;
    }
#ifdef FEATURE_RUIM_PHONEBOOK
    if(pMe->m_pAddrRUIM)
    {
        IADDRBOOK_Release(pMe->m_pAddrRUIM);
        pMe->m_pAddrRUIM = NULL;
    }
#endif
    g_pCoreApp = NULL;

#ifdef FEATURE_GREYBIT
	{
	    extern void GreyBitBrewFont_Done(void);
	    GreyBitBrewFont_Done();
	}
#endif

#ifdef FEATURE_RANDOM_MENU_REND
	// Destroy Rend
	{
	    extern void DisplayRend_Done(void);
	    DisplayRend_Done();
	}
#endif
} /* End CoreApp_FreeAppData */



/*==============================================================================
����: 
    CoreApp_InitAppData
       
˵��: 
    �������ڳ�ʼ��Ӧ�õ����ݳ�Ա��
       
����: 
    po [in]: IApplet �ӿ�ָ�롣
    
����ֵ:
    none
       
��ע:

==============================================================================*/
boolean CoreApp_InitAppData(IApplet* po)
{
    CCoreApp *pMe = (CCoreApp*)po;
    boolean b_FMBackground = FALSE;
    
    if (NULL == pMe)
    {
        return FALSE;
    }
	
#ifdef FEATURE_GREYBIT
	{
	    extern void GreyBitBrewFont_Init(void);
	    GreyBitBrewFont_Init();
	}
#endif

#ifdef USES_CONSTEFS
    CoreApp_RebuildEFS(pMe);
#endif

#ifdef FEATURE_MMGSDI
    pMe->m_nCardStatus = AEECARD_NOT_READY;
#else
    pMe->m_nCardStatus = AEECARD_NO_CARD;
#endif

    pMe->m_bSuspended = FALSE;
    pMe->m_bChargFull = FALSE;
    
    if (SUCCESS != ISHELL_CreateInstance(pMe->a.m_pIShell,
                                         AEECLSID_CARD,
                                         (void **) &pMe->m_pICard))
    {
        return FALSE;
    }
    
    
    // �������ICARD��PIN״̬��ʼ����ɣ��Ա�֤ʣ���ʼ�������ܹ���˳�����
    while (1)
    {
        AEECardPinStatus sPinStatus;
        
        CoreApp_GetCardStatus(pMe, AEECARD_SLOT1);
        if (ICARD_GetPinStatus(pMe->m_pICard, AEECARD_PIN1, &sPinStatus) != SUCCESS)
        {
            MSLEEP(100);
        }
        else
        {
            if(sPinStatus.lock_state == AEECARD_PIN_NOT_INIT || sPinStatus.lock_state == AEECARD_PIN_UNKNOWN)
            {
                if(AEECARD_NO_CARD != pMe->m_nCardStatus)
                {
                    MSLEEP(100);
                }
                else
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
    }
    
    if (TRUE != CoreApp_InitExtInterface(pMe))
    {
        return FALSE;
    }
    
    IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,TRUE);
    CoreTask_SetPwrDnComplete(FALSE);
    
    if( ISHELL_CreateInstance( pMe->a.m_pIShell, AEECLSID_BACKLIGHT, (void **)&pMe->m_pBacklight)!=AEE_SUCCESS)
    {
        return FALSE;
    }
    pMe->bunlockuim =FALSE;
    pMe->m_nMsgID = 0;
    
    pMe->m_eCurState = COREST_INIT;
    pMe->m_eDlgRet = DLGRET_CREATE;
    
    pMe->m_eUIMErrCode = UIMERR_UNKNOW;
    pMe->m_bAcquiredTime = FALSE;
    pMe->m_bExit = FALSE;
    pMe->m_isShift  = FALSE;   //add by yangdecai  2010-07-27
    
#if defined(FEATURE_WMS_APP)
    pMe->m_bsmstipscheck = FALSE;
#endif
#ifdef FEATRUE_AUTO_POWER
    pMe->m_b_autopower_off = FALSE;
    pMe->m_b_needclose_core = FALSE;
#endif
    MEMSET(pMe->m_strPhonePWD, 0, PINCODE_LENGTH + 1);
    MEMSET(pMe->m_strLockuimPWD, 0, sizeof(pMe->m_strLockuimPWD));
    MEMSET(pMe->m_strPIN1, 0, PINCODE_LENGTH + 1);
    MEMSET(pMe->m_strPIN2, 0, PINCODE_LENGTH + 1);
    MEMSET(pMe->m_strPUK, 0, PUKCODE_LENGTH + 1);   
    MEMSET(pMe->m_wPIN, 0, sizeof(pMe->m_wPIN));
    MEMSET(pMe->m_wPUK, 0, sizeof(pMe->m_wPUK));
    
    pMe->m_SYS_MODE_NO_SRV = TRUE;
    MEMSET(pMe->svc_p_name, 0, UIM_CDMA_HOME_SERVICE_SIZE + 1); 
#ifndef FEATURE_USES_LOWMEM
    pMe->m_pStartupAniImg = NULL;
#endif
    pMe->m_battery_time = 0;
    pMe->m_battery_state = TRUE;
#ifdef FEATURE_KEYGUARD
    pMe->m_b_set_lock = FALSE ;
#endif
    
    pMe->m_bConfigSent  = FALSE;
    pMe->m_cdg_msgptr = NULL;
    pMe->m_bActive = TRUE;
    pMe->m_bVerifying = FALSE;
    //if phone power down abnormal, we need set CFGI_FM_BACKGROUND  false to avoid show FM in idle
    ICONFIG_SetItem(pMe->m_pConfig, CFGI_FM_BACKGROUND, &b_FMBackground, sizeof(b_FMBackground));
	
//Add By zzg 2010_11_25 			
#ifdef FEATURE_APP_BLUETOOTH 
	{
		boolean bt_status = FALSE;
		ICONFIG_GetItem(pMe->m_pConfig, CFGI_BT_STATUS, &bt_status, sizeof(bt_status));

		if (TRUE == bt_status)
		{
			MSG_FATAL("***zzg ISHELL_StartBackgroundApplet AEECLSID_BLUETOOTH_APP***", 0, 0, 0);
			ISHELL_StartBackgroundApplet(pMe->a.m_pIShell, AEECLSID_BLUETOOTH_APP, "InitBT");					
		}
	}
#endif
//Add End	

#ifdef FEATURE_PLANEMODE
    pMe->bPlaneModeOn = FALSE;
#endif

#ifdef FEATURE_TORCH_SUPPORT
    pMe->TorchOn = FALSE;
#endif

	(void)ICONFIG_GetItem(pMe->m_pConfig, CFGI_PROFILE_CUR_NUMBER,&pMe->m_CurProfile, sizeof(pMe->m_CurProfile));//CFGI_ALERT_TYPE
	(void)ICONFIG_GetItem(pMe->m_pConfig,CFGI_PROFILE_CUR_NUMBER,&pMe->m_active,sizeof(pMe->m_active));
    CoreAppReadNVKeyBeepValue(pMe);
    g_pCoreApp = pMe;
    return TRUE;
} /* End CoreApp_InitAppData */

/*==============================================================================
����:
    CoreApp_HandleEvent

˵��:
    �������� Core Applet ���¼������������ڱ� Applet ��ȫ���¼��ڱ�����������
    �� Applet �������ṩһ���¼���������

����:
    pi [in]: IApplet �ӿ�ָ�룬����ָ�� Core Applet �ṹ����ĵ�һ����Ա�ĵ�ַ��
    eCode [in]: �������¼�
    wParam [in]: �¼�������
    dwParam [in]: �¼����ݡ�


����ֵ:
    TRUE: �¼��õ�����
    FALSE:�¼�û������

��ע:

==============================================================================*/
static boolean CoreApp_HandleEvent(IApplet * pi,
                                   AEEEvent eCode,
                                   uint16 wParam,
                                   uint32 dwParam)
{
    CCoreApp * pMe = (CCoreApp *)pi;
    switch (eCode)
    {
        case EVT_APP_START:
            if(!CoreApp_RegisterNotify(pMe))
            {
                return FALSE;
            }
            else
            {
                AEEAppStart *as;

                as = (AEEAppStart*)dwParam;
                //if (NULL != pMe->m_pDisplay)
                //{
                //    (void) IDISPLAY_Release(pMe->m_pDisplay);
                //    pMe->m_pDisplay = NULL;
                //}
                //pMe->m_pDisplay = as->pDisplay;
                //(void) IDISPLAY_AddRef(pMe->m_pDisplay);
                pMe->m_rc = as->rc;

                // ��ȡ������߶�
                pMe->m_nLargeFontHeight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay,
                                                                  AEE_FONT_LARGE,
                                                                  NULL,
                                                                  NULL);

                // ��ȡAEE_FONT_NORMAL����߶�
                pMe->m_nNormalFontHeight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay,
                                                                  AEE_FONT_NORMAL,
                                                                  NULL,
                                                                  NULL);

                pMe->m_bSuspended = FALSE;
                pMe->m_bActive = TRUE;
            }
        
            if(pMe->m_pIAnn != NULL)
            {
                IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,TRUE);
            }
            
            // ��ʼ Core Applet ״̬��, ��ǰ״̬�ѳ�ʼΪ COREST_INIT
            CoreApp_RunFSM(pMe);

            CoreApp_PoweronStartApps(pMe);
            //CoreApp_InitBattStatus(pMe);

			{
				extern void EnableUIKeys (boolean flag);
            	EnableUIKeys(TRUE);
			}
            
            (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                                  RESETPROFILE_TIME,
                                  CoreApp_ResetProfileTimer, 
                                  pMe);
            return TRUE;

        case EVT_APP_STOP:
			{
            //if (pMe->m_pDisplay != NULL)
            //{
            //    (void) IDISPLAY_Release(pMe->m_pDisplay);
            //    pMe->m_pDisplay = NULL;
            //}
            JulianType  julian;
        	GetJulianDate(GETTIMESECONDS(), &julian);
            pm_rtc_rw_cmd (PM_RTC_SET_CMD, (pm_rtc_julian_type*)(&julian));
            pMe->m_bSuspended = TRUE;
            pMe->m_bActive = TRUE;

            if (pMe->m_bExit == FALSE)
            {
                // תΪ��̨ģʽ, ֻ���ڹػ�ʱ�����˳� Core Applet
                *((boolean *)dwParam) = FALSE;
            }
            return TRUE;
    	  }

        case EVT_APP_SUSPEND:
        {
            AEESuspendInfo * pinfo = (AEESuspendInfo *)dwParam;

            if (NULL != pinfo)
            {
                // Ҫ��رնԻ���, ����ָ�ʱ��״̬�������´����Ի����
                pinfo->bCloseDialogs = TRUE;
            }

            pMe->m_bSuspended = TRUE;
            pMe->m_bActive = FALSE;
            AEE_CancelTimer(CoreApp_Process_Batty_Msg_CB, (void*)pMe);
            return TRUE;
        }

        case EVT_HEADSET:
        {
            if((boolean)wParam)
            {
                CoreApp_Process_Headset_Msg(pMe, IDS_HEADSET_ON);
            }
            else
            {
                CoreApp_Process_Headset_Msg(pMe, IDS_HEADSET_OFF);
            }
            return TRUE;
        }

        case EVT_APP_RESUME:
        {
            AEEAppStart *as;

            as = (AEEAppStart*)dwParam;
            //if (NULL != pMe->m_pDisplay)
            //{
            //    (void) IDISPLAY_Release(pMe->m_pDisplay);
            //    pMe->m_pDisplay = NULL;
            //}
            //pMe->m_pDisplay = as->pDisplay;
            //(void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
#ifdef FEATURE_ICM
            pMe->m_bemergencymode = CoreApp_IsEmergencyMode(pMe->m_pCM);
            // �ر�ȫ��ͨ������ǰ���ֹ���������ҵ���������������
            ICM_EndAllCalls(pMe->m_pCM);
#else
            pMe->m_bemergencymode = CoreApp_IsEmergencyMode(pMe->m_pITelephone);
            // �ر�ȫ��ͨ������ǰ���ֹ���������ҵ���������������
            ICALLMGR_EndAllCalls(pMe->m_pICallMgr);
#endif
            pMe->m_bSuspended = FALSE;
            pMe->m_bActive = TRUE;

            // ��״̬��
            CoreApp_RunFSM(pMe);
            return TRUE;
        }

        case EVT_DIALOG_INIT:
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_wActiveDlgID = wParam;
			#ifdef FEATURE_USE_TIME_VU
			/* The PMIC has been initialized, get time-of-day from the PMIC's RTC */
			time_set_from_pmic();
			#endif /* FEATURE_USE_TIME_VU */
            CoreApp_SetDialogHandler(pMe);
            return CoreApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);

        case EVT_DIALOG_START:
            return CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            return TRUE;
            
        case EVT_CARD_STATUS:
        case EVT_DISPLAYDIALOGTIMEOUT:
            (void) CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            return TRUE;

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            (void) CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;
            pMe->m_wActiveDlgID = 0;
            pMe->m_pDialogHandler = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء�ע��:EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspended == FALSE)
            {
                // ��״̬��
                CoreApp_RunFSM(pMe);
            }
            return TRUE;

        case EVT_AUTO_POWERDOWN:
            //CORE_ERR("EVT_AUTO_POWERDOWN w= %d dw= %d",wParam,dwParam,0);
#ifdef FEATRUE_AUTO_POWER
            if(wParam == 1)
            {
                ISHELL_SetTimer(pMe->a.m_pIShell, 3000, CoreApp_Process_AutoPower_Event, (void *)pMe);
            }
            else
            {
                CoreApp_Process_AutoPower_Event(pMe);
            }
#endif
            return TRUE; 

        case EVT_NEWMSGIN:
            //CORE_ERR("EVT_NEWMSGIN w= %d dw= %d",wParam,dwParam);
            if(dwParam != 0)
            {
                if(pMe->m_cdg_msgptr == NULL)
                {
#ifdef FEATURE_ICM
                    pMe->m_cdg_msgptr = (AECHAR * )MALLOC(AEECM_MAX_ALPHA_TAG_LENGTH*sizeof(AECHAR));
#else
                    pMe->m_cdg_msgptr = (AECHAR * )MALLOC(AEET_MAX_ALPHA_TAG_LENGTH*sizeof(AECHAR));
#endif
                }
                if(pMe->m_cdg_msgptr != NULL)
                {
                    IDialog  * pDialog = NULL;
                    pDialog = ISHELL_GetActiveDialog(pMe->a.m_pIShell);
                    if(pDialog != NULL)
                    {
#ifdef FEATURE_ICM
                        WSTRLCPY(pMe->m_cdg_msgptr,(AECHAR * )dwParam,AEECM_MAX_ALPHA_TAG_LENGTH);
#else
                        WSTRLCPY(pMe->m_cdg_msgptr,(AECHAR * )dwParam,AEET_MAX_ALPHA_TAG_LENGTH);
#endif
                        pMe->m_nMsgID = 0xFFFF;
                        if(IDIALOG_GetID(pDialog) == IDD_IDLE)
                        {
                            CLOSE_DIALOG(DLGRET_MSG);
                        }
                        else if(IDIALOG_GetID(pDialog) == IDD_MSGBOX)
                        {
                            ISHELL_PostEvent(pMe->a.m_pIShell, AEECLSID_CORE_APP,EVT_USER_REDRAW,  0, 0);
                        }
                        else
                        {
                            FREEIF(pMe->m_cdg_msgptr);
                        }
                    }
                    else
                    {
                        FREEIF(pMe->m_cdg_msgptr);
                    }
                }
                return TRUE;
            }
            return FALSE;
        case EVT_KEY_HELD:
            switch (wParam)
            {
                case AVK_POWER:
                case AVK_END:
                
#ifdef FEATURE_KEYGUARD
                    if(OEMKeyguard_IsEnabled())
                    {
                        OEMKeyguard_SetState(TRUE);  //(FALSE);                      
                        return TRUE;
                    }
#endif
                    if ((pMe->m_bSuspended) || (NULL == pMe->m_pActiveDlg))
                    {
                        return FALSE;
                    }
                    
                    if(pMe->m_eCurState == COREST_LPM)
                    {
                        return CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
                    }
                    
                    pMe->m_ePowerDownType = POWERDOWN_NORMAL;
                    if (pMe->m_eCurState != COREST_POWEROFF
                      &&pMe->m_eCurState != COREST_STARTUPANI
                      &&pMe->m_eCurState != COREST_INIT
                      &&pMe->m_eCurState != COREST_POWERONSYSINIT
                      &&pMe->m_eCurState != COREST_POWERONAPPSDATAINIT)
                    {
                        pMe->m_wStartupAniTime = 0;
                        MOVE_TO_STATE(COREST_POWEROFF)
                        CLOSE_DIALOG(DLGRET_CREATE)
                    }

                    return TRUE;

                case AVK_RWD:
                {
                    // �����л��龰ģʽ
                    ICONFIG_GetItem(pMe->m_pConfig, CFGI_PROFILE_CUR_NUMBER,&pMe->m_CurProfile, sizeof(pMe->m_CurProfile));//CFGI_ALERT_TYPE
                    
                    if(pMe->m_CurProfile != OEMNV_PROFILE_QUIETMODE)
                    {
                        pMe->m_CurProfile = OEMNV_PROFILE_QUIETMODE;
                    }
                    else
                    {
                        pMe->m_CurProfile = OEMNV_PROFILE_NORMALMODE;
                    }

                    CoreApp_SceneMode(pMe);
                    //ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_CUR_NUMBER,&alertType, sizeof(alertType));
                    //CoreApp_UpdateAnnunciator(pMe);
                    return TRUE;
                }
                    
       #ifdef FEATURE_TORCH_SUPPORT

            #if defined(FEATURE_TORCH_KEY_SPACE)	
			
                case AVK_SPACE:
                {
                    if ( pMe->TorchOn == FALSE )
                    {
                        pMe->TorchOn = TRUE;
                        if (pMe->m_pBacklight)
                        {
                            IBACKLIGHT_TurnOnTorch(pMe->m_pBacklight);
                        }
                    }
                    else
                    {
                        pMe->TorchOn = FALSE;
                        if (pMe->m_pBacklight)
                        {                        	
                            IBACKLIGHT_TurnOffTorch(pMe->m_pBacklight);
                        }
                    }                                                   
                    }             

                return TRUE;                
            #endif

			#if defined(FEATURE_TORCH_KEY_LCTRL)	
			
                case AVK_LCTRL:
                {
                    if ( pMe->TorchOn == FALSE )
                    {
                        pMe->TorchOn = TRUE;
                        if (pMe->m_pBacklight)
                        {
                            IBACKLIGHT_TurnOnTorch(pMe->m_pBacklight);
                        }
                    }
                    else
                    {
                        pMe->TorchOn = FALSE;
                        if (pMe->m_pBacklight)
                        {                        	
                            IBACKLIGHT_TurnOffTorch(pMe->m_pBacklight);
                        }
                    }                                                   
                }             

                return TRUE;                
            #endif
            
            #if defined(FEATURE_TORCH_KEY_CAMERA)					
				case AVK_CAMERA:
                {
                    if ( pMe->TorchOn == FALSE )
                    {
                        pMe->TorchOn = TRUE;
                        if (pMe->m_pBacklight)
                        {
                            IBACKLIGHT_TurnOnTorch(pMe->m_pBacklight);
                        }
                    }
                    else
                    {
                        pMe->TorchOn = FALSE;
                        if (pMe->m_pBacklight)
                        {                        	
                            IBACKLIGHT_TurnOffTorch(pMe->m_pBacklight);
                        }
                    }
                }

                return TRUE;
            #endif
            
                break;
        #endif
            default:
                break;
            }
            #ifdef FEATURE_VERSION_HITZ181
            return CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            #endif
            break;
            
        case EVT_GSENSOR_SHAKE:
        case EVT_KEY:
#if MIN_BREW_VERSION(3,0)
            // do not want to handle au
            if ((dwParam & KB_AUTOREPEAT) != 0)
            {
                if ((wParam & AVK_CLR)!=0)
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }
            }
#endif
            return CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);

            
        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_COMMAND:
        {
            return CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
        }
            
        case EVT_HEADSET_CONNECT:
            StereoHeadsetOn(pMe);
            return TRUE;
            
        case EVT_HEADSET_DISCONNECT:
            HeadsetOff(pMe);
            return TRUE;
            
        case EVT_NOTIFY:
            if(((AEENotify *)dwParam)->cls == AEECLSID_ALERT_NOTIFIER)
            {
                //CORE_ERR("AEECLSID_ALERT_NOTIFIER %x",wParam,0,0);
                switch(wParam)
                {
                    case NMASK_ALERT_ONOFF:
                        return TRUE;

                    case NMASK_ALERT_MUTED:
                        return TRUE;

                    default:
                        return FALSE;
                }
            }
            return CoreApp_HandleNotify(pMe, (AEENotify *)dwParam);
            
        case EVT_UPDATEIDLE:
           
            if ((ISHELL_ActiveApplet(pMe->a.m_pIShell) != AEECLSID_CORE_APP) ||
                (pMe->m_wActiveDlgID != IDD_IDLE))
            {
                return TRUE; 
            }
            
            {   // �ڴ�������ӵĿ�����ʼ������ʼ������ֻ��ִ��һ��
                static boolean bNotInitedAlarm = TRUE;
                
                if (bNotInitedAlarm)
                {
#ifdef FEATURE_UIALARM
                    // ֱ��ȡ����Чʱ��ſ�ʼ����������
                    CoreApp_EnableShellAlarms(pMe, TRUE);
#endif
                    bNotInitedAlarm = FALSE;

                }
            }      
            return CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            
#if defined(FEATURE_WMS_APP)
        case EVT_WMS_MSG_RECEIVED_MESSAGE:
            pMe->m_bsmstipscheck = TRUE;
            
            if (ISHELL_ActiveApplet(pMe->a.m_pIShell) == AEECLSID_CORE_APP)
            {
                uint16  nNewsVmail=0, nNewsSMS=0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_VOICEMAIL, &nNewsVmail, NULL, NULL);
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, &nNewsSMS, NULL, NULL);
                
                if (pMe->m_wActiveDlgID == IDD_IDLE)
                {
                    if (nNewsVmail>0 || nNewsSMS>0)
                    {
                        CLOSE_DIALOG(DLGRET_SMSTIPS)
                    }
                }
                else if (pMe->m_wActiveDlgID == IDD_WMSTIPS)
                {// ���´����Ի���,������ʾ
                    if (nNewsVmail>0 || nNewsSMS>0)
                    {
                        CLOSE_DIALOG(DLGRET_CREATE)
                    }
                    else
                    {// ����֪ͨ����Ϊ0Ӧ�����ʾ
                        pMe->m_bsmstipscheck = FALSE;
                        CLOSE_DIALOG(DLGRET_SMSTIPS_OK)
                    }
                }
            }
            
            return TRUE;
#endif            
#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
        case EVT_SENDREGINFOR:
            // ����ע�����
            if (CoreApp_SendReginfo(pMe) != SUCCESS)
            {
                // ���÷���ע����ŵĶ�ʱ������
                (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                                      SENDREGINFOR_TIME,
                                      CoreApp_SendReginfoTimer, 
                                      pMe);
            }
            
            //CORE_ERR("EVT_SENDREGINFOR",0,0,0);
            return TRUE;
            
        // ����ע�����״̬�����¼�
        case EVT_SENDSTATUS:
            //CORE_ERR("EVT_SENDSTATUS",0,0,0);
            if (dwParam != SUCCESS)
            {// ����ע�����ʧ��
                // ���÷���ע����ŵĶ�ʱ������
                (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                                      SENDREGINFOR_TIME,
                                      CoreApp_SendReginfoTimer, 
                                      pMe);
            }
            return TRUE;
            
        //�յ�ע��ȷ�϶���
        case EVT_UPDATEREGSTATUS:
            //CORE_ERR("----Reg--%d-",wParam,0,0);
            if (wParam != 0)
            {
                IdleAPP_Config_Type cfg;
                
                (void)ISHELL_GetPrefs(pMe->a.m_pIShell, 
                            AEECLSID_CORE_APP, 
                            IDLE_CFG_VERSION,
                            &cfg, 
                            sizeof(cfg));
                            
                cfg.m_RegItem.m_bRegistered = TRUE;
                (void)ISHELL_SetPrefs(pMe->a.m_pIShell, 
                        AEECLSID_CORE_APP, 
                        IDLE_CFG_VERSION,
                        &cfg, 
                        sizeof(cfg));
            }
            return TRUE;
#endif
#ifdef FEATURE_SEAMLESS_SMS
        case EVT_SEND_SEAMLESSSMS:
            // ����ע�����
            if (CoreApp_SendSeamlessSMS(pMe) != SUCCESS)
            {
                // ���÷���ע����ŵĶ�ʱ������
                (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                                      SENDSEAMLESSSMS_TIME,
                                      CoreApp_SendSeamlessSMSTimer, 
                                      pMe);
            }
            
            //CORE_ERR("EVT_SEND_SEAMLESSSMS",0,0,0);
            return TRUE;
#endif

#ifdef FEATURE_UTK2
        case EVT_UTKREFRESH:           
            pMe->wRefreshMask = wParam;
            // �ȸı䵱ǰ״̬
            MOVE_TO_STATE(COREST_UTKREFRESH)
            
            // ���� DLGRET_CREATE �رյ�ǰ�Ի���
            CLOSE_DIALOG(DLGRET_CREATE)         
            return TRUE;
#endif
            
        case EVT_RTC:
        {
            if( pMe->m_eCurState == COREST_LPM)
            {

#if defined( FEATURE_POWERDOWN_ALARM)
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_ALARM_FLAG,(void*)&pMe->powerupByAlarm, sizeof(boolean));
#endif              
                CLOSE_DIALOG( DLGRET_RTC)
            }
        }
        return TRUE;
        case EVT_BUSY:
            if(pMe->m_wActiveDlgID == IDD_STARTUPANI)
            {
                return TRUE;//CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            }
            return FALSE;

        case EVT_DRAWMUSICNAME:
        {
#ifndef WIN32//wlh ��ʱ����
#ifdef FEATURE_APP_MUSICPLAYER
            pMe->m_pMusicName = (AECHAR *)dwParam;  // yeuzhen.li add @090118
#endif            
#endif//WIN32
            if ((ISHELL_ActiveApplet(pMe->a.m_pIShell) != AEECLSID_CORE_APP) ||
                (pMe->m_wActiveDlgID != IDD_IDLE))
            {
                return TRUE; 
            }

            //change EVT_DRAWMUSICNAME to EVT_UPDATEIDLE
            CoreApp_RouteDialogEvent(pMe,(AEEEvent)EVT_USER_REDRAW,wParam,dwParam);
            return TRUE;
        }

#ifdef FEATURE_PLANEMODE
        case EVT_PLANEMODEON: /* settings �Ǳߵ����øı����idle����������ʾ����ƭ�û� */
            pMe->m_SYS_MODE_NO_SRV = TRUE;
            break;
#endif
        case EVT_USER:
            switch(wParam) {
            case EVT_UI_EXIT:
                /* AEE is about to exit, set the exit flag to TRUE */
                pMe->m_bExit = TRUE;
                return TRUE;
                
            case EVT_MC_PWR_DOWN:
                CoreApp_Poweroff_Phone(pMe);
                break;
            default:
                break;
            }
            break;
        case EVT_SET_OPERATING_MODE:
#ifdef FEATURE_ICM
            ICM_SetOperatingMode(pMe->m_pCM, (AEECMOprtMode)wParam);
#else
            IPHONECTL_SetOperatingMode(pMe->m_pIPhoneCtl, (AEETOprtMode)wParam);
#endif
            break;
            
        default:
            break;
    }
    
    return(FALSE);
}  // End CoreApp_HandleEvent

/*==============================================================================
����: 
    CoreApp_HandleNotify
       
˵��: 
    �������ڽ� Core Apple �յ���֪ͨ�¼���������Դ�ַ�����Ӧ��������
       
����: 
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ��
    pNotify [in]: ָ�� AEENotify �ṹ�������ָ�롣
    
    
����ֵ:
    TRUE: �¼��õ�����
    FALSE:�¼�û������
       
��ע:

==============================================================================*/
static boolean CoreApp_HandleNotify(CCoreApp * pMe, AEENotify *pNotify)
{
    if (NULL == pMe || NULL == pNotify)
    {
        return FALSE;
    }
    
    switch (pNotify->cls)
    {
#ifdef FEATURE_ICM
        case AEECLSID_CM_NOTIFIER:
#else
        case AEECLSID_PHONENOTIFIER:
#endif
            return CoreApp_HandleCMNotify(pMe, pNotify);

        case AEECLSID_BATTERYNOTIFIER:
            return CoreApp_HandleBattNotify(pMe, pNotify);          
     
        default:
            break;
    }
    
    return FALSE;
}
#ifdef FEATURE_ICM
static void CoreApp_Process_SS_info(CCoreApp * pMe ,AEECMSSEventData *ss)
#else
static void CoreApp_Process_SS_info(CCoreApp * pMe ,AEETSSEventData *ss)
#endif
{
    switch (ss->ss_info.srv_status)
    {
#ifdef FEATURE_ICM
        case AEECM_SRV_STATUS_SRV:
        case AEECM_SRV_STATUS_LIMITED:
#else
        case AEET_SRV_STATUS_SRV:
        case AEET_SRV_STATUS_LIMITED:
#endif
        {
            if(pMe->m_pIAnn == NULL)
            {
                return ;
            }

            switch (ss->ss_info.roam_status)
            {
#ifdef FEATURE_ICM
                case AEECM_ROAM_STATUS_OFF:
#else
                case AEET_ROAM_STATUS_OFF:
#endif
                    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_WAP/*ANNUN_FIELD_ROAM*/, ANNUN_STATE_ROAM_OFF/*ANNUN_STATE_OFF*/);
                    break;
#ifdef FEATURE_ICM
                case AEECM_ROAM_STATUS_ON:
#else
                case AEET_ROAM_STATUS_ON:
#endif
                    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_WAP/*ANNUN_FIELD_ROAM*/, ANNUN_STATE_ROAM_ON/*ANNUN_STATE_ON*/);
                    break;
#ifdef FEATURE_ICM
                case AEECM_ROAM_STATUS_BLINK:
#else
                case AEET_ROAM_STATUS_BLINK:
#endif
                    //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_ROAM, ANNUN_STATE_BLINK);
                    break;
                    
                default:
                    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_WAP/*ANNUN_FIELD_ROAM*/, ANNUN_STATE_ROAM_OFF/*ANNUN_STATE_OFF*/);
                    break;
            }
            break;
        }
#ifdef FEATURE_ICM
        case AEECM_SRV_STATUS_NO_SRV:
        case AEECM_SRV_STATUS_PWR_SAVE:
#else
        case AEET_SRV_STATUS_NO_SRV:
        case AEET_SRV_STATUS_PWR_SAVE:
#endif
            if(pMe->m_pIAnn == NULL)
            {
                return ;
            }
            IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RSSI,ANNUN_STATE_RSSI_NO_SERV/*ANNUN_STATE_RSSI_0*/);
            IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_WAP/*ANNUN_FIELD_ROAM*/, ANNUN_STATE_ROAM_OFF/*ANNUN_STATE_OFF*/);
            break;

        default:
            break;
    }
}
/*==============================================================================
����: 
    CoreApp_HandleCMNotify
       
˵��: 
    ������������ AEECLSID_CM_NOTIFIER ��֪ͨ�¼���
       
����: 
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ��
    pNotify [in]: ָ�� AEENotify �ṹ�������ָ�롣
    
    
����ֵ:
    TRUE: �¼��õ�����
    FALSE:�¼�û������
       
��ע:

==============================================================================*/
static boolean CoreApp_HandleCMNotify(CCoreApp * pMe, AEENotify *pNotify)
{
#ifdef FEATURE_ICM
    AEECMSysMode SysMode;
    boolean  bUpdate = FALSE;
    AEECMNotifyInfo *pEvtInfo;
#else
    AEETSysMode SysMode;
    boolean  bUpdate = FALSE;
    AEETNotifyInfo *pEvtInfo;
#endif

    if (NULL == pNotify)
    {
        return FALSE;
    }
    
    pEvtInfo = pNotify->pData;
    
    SysMode = pEvtInfo->event_data.ss.ss_info.sys_mode;
    
    switch (pNotify->dwMask)
    {
#ifdef FEATURE_ICM
    case NMASK_CM_DATA_CALL:
#else
    case AEET_NMASK_DATA_CALL:
#endif
            switch (pEvtInfo->event)
            {
#ifdef FEATURE_ICM
                case AEECM_EVENT_CALL_CONNECT:
#else
                case AEET_EVENT_CALL_CONNECT:
#endif
                    if(pMe->m_pIAnn != NULL)
                    {
                        IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_WAP, ANNUN_STATE_1X_ON);
                    }
                    break;
#if 0
                case AEECM_EVENT_CALL_ENTER_DORMANT:
                    IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_WAP, ANNUN_STATE_1X_OFF);                   
                    break;
                case AEECM_EVENT_CALL_EXIT_DORMANT:
                    IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_WAP, ANNUN_STATE_1X_ON);                   
                    break;
#endif
#ifdef FEATURE_ICM
                case AEECM_EVENT_CALL_END: 
#else
                case AEET_EVENT_CALL_END: 
#endif
                    if(pMe->m_pIAnn != NULL)
                    {
                        IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_WAP, ANNUN_STATE_1X_OFF);   
                    }
                    break;
            }
            break;
#ifdef FEATURE_ICM
       case NMASK_CM_SS:
#else
       case AEET_NMASK_SS:
#endif
            switch (pEvtInfo->event)
            {
#ifdef FEATURE_ICM
                case AEECM_EVENT_SS_SRV_CHANGED:
                case AEECM_EVENT_SS_RSSI:
#else
                case AEET_EVENT_SS_SRV_CHANGED:
                case AEET_EVENT_SS_RSSI:
#endif
#ifdef FEATURE_ICM
                    if (SysMode == AEECM_SYS_MODE_NO_SRV)
#else
                    if (SysMode == AEET_SYS_MODE_NO_SRV)
#endif
                    {
                        pMe->m_SYS_MODE_NO_SRV = TRUE;
                        if(pMe->m_pIAnn != NULL)
                        {
                            IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RSSI, ANNUN_STATE_RSSI_NO_SERV);
                        }
#ifdef FEATURE_LED_CONTROL
                        IBACKLIGHT_SigLedDisable(pMe->m_pBacklight);
#endif
                        bUpdate = TRUE;
                    }
#ifdef FEATURE_ICM
                    else if (AEECM_SYS_MODE_NONE != SysMode)
#else
                    else if (AEET_SYS_MODE_NONE != SysMode)
#endif
                    {
                        if (pMe->m_bAcquiredTime == FALSE)
                        {
                            pMe->m_bAcquiredTime = TRUE;
                        }
                        if(pMe->m_SYS_MODE_NO_SRV == TRUE)
                        {
                            bUpdate = TRUE;                        
                            pMe->m_SYS_MODE_NO_SRV = FALSE;
                        }
                        
                        {
                            if(pMe->m_pIAnn != NULL)
                            {
                                IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RSSI, DBToLevel(pEvtInfo->event_data.ss.ss_info.rssi));
                            }
                        }
                        //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RSSI, DBToLevel(pEvtInfo->event_data.ss.ss_info.rssi));
#ifdef FEATURE_LED_CONTROL
#if defined( FEATURE_HAS_NETWORK_LED_CONTROL)
                        if( DBToLevel(pEvtInfo->event_data.ss.ss_info.rssi) == ANNUN_STATE_RSSI_0)
                        {
                            IBACKLIGHT_SigLedDisable(pMe->m_pBacklight);
                        }
                        else
                        {
                            IBACKLIGHT_SigLedDisable(pMe->m_pBacklight);    //cancel the previous LED control
                            IBACKLIGHT_SigLedEnable(pMe->m_pBacklight,SIG_LED_NETWORK);
                        }
#endif
#endif
                    }

                    CoreApp_Process_SS_info(pMe,&pEvtInfo->event_data.ss);
                    break;
#ifdef FEATURE_ICM
                case AEECM_EVENT_SS_HDR_RSSI:
                    break;
#endif
                default:
                    break;
            }
            break;
#ifdef FEATURE_ICM
        case NMASK_CM_PHONE:
#else
        case AEET_NMASK_PHONE:
#endif
            switch(pEvtInfo->event)
            {
                // System preference changed
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_SYSTEM_PREF:
#else
                case AEET_EVENT_PH_SYSTEM_PREF:
#endif
                    return TRUE;

                // PLMN: Subscription info available
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_SUBSCRIPTION:
#else
                case AEET_EVENT_PH_SUBSCRIPTION:
#endif
                    return TRUE;

                // CDMA lock mode was changed
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_CDMA_LOCK_MODE:
                case AEECM_EVENT_PH_FUNDS_LOW:
#else
                case AEET_EVENT_PH_CDMA_LOCK_MODE:
                case AEET_EVENT_PH_FUNDS_LOW:
#endif
                    return TRUE;

                // NAM selection was changed
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_NAM_SEL:
#else
                case AEET_EVENT_PH_NAM_SEL:
#endif
                    // Reset the date, operator name
                    {
                        int nErr;
                        IDownload *pIDownload=NULL;
                        AEEMobileInfo info;
                        
                        ICONFIG_GetItem(pMe->m_pConfig, CFGI_MOBILEINFO, &info, sizeof(info));
                         
                        nErr = ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_DOWNLOAD, (void **) &pIDownload);
                        if (nErr==SUCCESS && pIDownload)
                        {
                            IDOWNLOAD_SetSubscriberID(pIDownload, info.szMobileID, STRLEN(info.szMobileID));
                            IDOWNLOAD_Release(pIDownload);
                        }
                    }
                    return TRUE;

                // Current NAM was changed
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_CURR_NAM:
#else
                case AEET_EVENT_PH_CURR_NAM:
#endif
                    (void) AEE_IssueSystemCallback(AEE_SCB_DEVICE_INFO_CHANGED);
                    return TRUE;

                // Answer voice as data was changed.
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_ANSWER_VOICE:
#else
                case AEET_EVENT_PH_ANSWER_VOICE:
#endif
                    return TRUE;

                // Entering powerdown sleep mode
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_STANDBY_SLEEP:
#else
                case AEET_EVENT_PH_STANDBY_SLEEP:
#endif
                    return TRUE;

                // Existing powerdown sleep mode
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_STANDBY_WAKE:
#else
                case AEET_EVENT_PH_STANDBY_WAKE:
#endif
                    return TRUE;

                // DMA maintenance required command
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_MAINTREQ:
#else
                case AEET_EVENT_PH_MAINTREQ:
#endif
                    return TRUE;

                // In use state was changed
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_IN_USE_STATE:
#else
                case AEET_EVENT_PH_IN_USE_STATE:
#endif
                    return TRUE;

                // Phone information is now available
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_INFO_AVAIL:
#else
                case AEET_EVENT_PH_INFO_AVAIL:
#endif
                    if (!pMe->m_bProvisioned) {
                       InitAfterPhInfo(pMe, pEvtInfo->event_data.ph.oprt_mode);
                    }
                    return TRUE;

                // Operating mode was changed
#ifdef FEATURE_ICM
                case AEECM_EVENT_PH_OPRT_MODE:
#else
                case AEET_EVENT_PH_OPRT_MODE:
#endif
                    /* continue processing of the event, e.g., update banner etc
                       this is done regardless of whether we are in the Test Mode
                       dialog or not */
                    switch (pEvtInfo->event_data.ph.oprt_mode)
                    {
#ifdef FEATURE_ICM
                    case AEECM_OPRT_MODE_FTM:
#else
                    case AEET_OPRT_MODE_FTM:
#endif
                      return CoreApp_ProcessFTMMode(pMe);
#ifdef FEATURE_ICM
                    case AEECM_OPRT_MODE_OFFLINE:
#else
                    case AEET_OPRT_MODE_OFFLINE:
#endif
                      return CoreApp_ProcessOffLine(pMe);
#ifdef FEATURE_ICM
                    case AEECM_OPRT_MODE_ONLINE:
#else
                    case AEET_OPRT_MODE_ONLINE:
#endif
                      CoreApp_ProcessSubscriptionStatus(pMe);
                      return TRUE;
#ifdef FEATURE_ICM
                    case AEECM_OPRT_MODE_LPM:
#else
                    case AEET_OPRT_MODE_LPM:
#endif 
                      return TRUE;
                      
                    default:
                      /* unhandled OPRT events */
                      return FALSE;
                    }
                    //if(AEECM_OPRT_MODE_ONLINE == pEvtInfo->event_data.ph.oprt_mode)
                    //{
                    //    CoreApp_load_uim_esn(pMe);
                    //}
                    break;

                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    
    if (bUpdate)
    {
        ISHELL_PostEvent(pMe->a.m_pIShell, 
                         AEECLSID_CORE_APP,
                         EVT_UPDATEIDLE,
                         0,0L);
    }
    return TRUE;
}

#ifdef CUST_EDITION
 void CoreApp_InitBattStatus(CCoreApp * pMe)
{
    AEENotify pNotify;

    MEMSET(&pNotify,0x00,sizeof(AEENotify));
    if(pMe->m_pBatt != NULL)
    {        
        pNotify.dwMask = NMASK_BATTERY_CHARGERSTATUS_CHANGE;
        CoreApp_HandleBattNotify(pMe,&pNotify);
        pNotify.dwMask = NMASK_BATTERY_STATUS_CHANGE;
        CoreApp_HandleBattNotify(pMe,&pNotify); 
    }       
}
#endif

/*=============================================================================
FUNCTION: CoreApp_GetCardStatus

DESCRIPTION:
Obtains Card Status for a given slot

SIDE EFFECTS:
  None.

=============================================================================*/
static boolean CoreApp_GetCardStatus(CCoreApp *pMe,uint8 slot)
{
    /* Get the card status from card */
    if (AEECARD_SLOT1 == slot) 
    {
        ICARD_GetCardStatus(pMe->m_pICard, (uint8 *) &(pMe->m_nCardStatus));
    }
    else 
    {
        return FALSE;
    }
    
    return TRUE;
}

/*==============================================================================
����: 
    CoreApp_HandleBattNotify
       
˵��: 
    ������������ AEECLSID_CM_NOTIFIER ��֪ͨ�¼���
       
����: 
    pMe [In]: ָ�� Core Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ��
    pNotify [in]: ָ�� AEENotify �ṹ�������ָ�롣
    
    
����ֵ:
    TRUE: �¼��õ�����
    FALSE:�¼�û������
       
��ע:

==============================================================================*/
static boolean CoreApp_HandleBattNotify(CCoreApp * pMe, AEENotify *pNotify)
{
    //AEEBattLevel *pBattLevel;
    AEEBatteryChargerStatus nChargerStatus;
    AEEBatteryStatus nBattStatus;   
    
    if ((NULL == pMe) || (pNotify == NULL))
    {
        return FALSE;
    }
        
    switch (pNotify->dwMask) 
    {
        // �ⲿ��Դ�����γ�
        case NMASK_BATTERY_EXTPWR_CHANGE:
            pMe->m_bExtPwrState = IBATTERY_GetExternalPower(pMe->m_pBatt);

#ifdef FEATURE_APP_MEDIAGALLERY
            MediaGallery_SetUSBCableConnect(pMe->m_bExtPwrState);
#endif
            if(pMe->m_bExtPwrState == FALSE && pMe->m_wActiveDlgID == IDD_LPM)
            {
                // in this time ,we need power down the phone
                ISHELL_CancelTimer(pMe->a.m_pIShell, CoreApp_Draw_Charger_image, pMe);
                //CLOSE_DIALOG(DLGRET_OK)
                CoreApp_Process_Batty_Msg(pMe, IDS_POWEROFF_1);
                AEE_SetSysTimer(POWER_DOWN_TIME , CoreApp_Poweroff_Phone, pMe);
                return TRUE;
            }
            
            CoreApp_Process_Charger_Msg(pMe);
            pMe->m_battery_time= 0;
            pMe->m_battery_state = TRUE ;
            AEE_CancelTimer( CoreApp_Process_Batty_Msg_CB, (void*)pMe);
            IBACKLIGHT_Enable(pMe->m_pBacklight);
            pMe->m_bChargFull = FALSE;
            break;
        
        // ���״̬�ı�
        case NMASK_BATTERY_CHARGERSTATUS_CHANGE:  
            nChargerStatus = IBATTERY_GetChargerStatus(pMe->m_pBatt);
            
            if(pMe->m_wActiveDlgID == IDD_LPM)
            {
                ISHELL_PostEvent(pMe->a.m_pIShell, 
                                 AEECLSID_CORE_APP,
                                 EVT_USER_REDRAW,
                                 0,0L);
            }
            switch(nChargerStatus)
            {
                case AEEBATTERY_CHARGERSTATUS_FULLY_CHARGE:
                {
                    pMe->m_bChargFull = TRUE;
                    pMe->m_bExtPwrState = TRUE;
                    CoreApp_Process_Batty_Msg(pMe, IDS_FULLY_CHARGED);
                    (void) ISHELL_CancelTimer(pMe->a.m_pIShell, CCharger_EnableICONCB, (void *) pMe);
                    if(pMe->m_pIAnn != NULL)
                    {
                        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BATT, ANNUN_STATE_BATT_FULL);
                    }
#ifdef FEATURE_APP_MEDIAGALLERY
                    MediaGallery_SetUSBCableConnect(pMe->m_bExtPwrState);
#endif
                    break;
                }

                case AEEBATTERY_CHARGERSTATUS_CHARGING:
                {
                    pMe->m_bExtPwrState = TRUE;
#ifdef FEATURE_APP_MEDIAGALLERY
                    MediaGallery_SetUSBCableConnect(pMe->m_bExtPwrState);
#endif              
                    if(pMe->m_bChargFull)
                    {
                        break;
                    }
                    (void) ISHELL_SetTimer(pMe->a.m_pIShell,1000, CCharger_EnableICONCB, (void *) pMe);
                    break;
                }

                default:
                    break;
            }
            break;
            
        case NMASK_BATTERY_STATUS_CHANGE:
            pMe->m_bExtPwrState = IBATTERY_GetExternalPower(pMe->m_pBatt);
            if (FALSE == pMe->m_bExtPwrState)
            {
                nBattStatus = IBATTERY_GetStatus(pMe->m_pBatt);
                switch(nBattStatus)
                {
                    case AEEBATTERY_STATUS_POWERDOWN:  // Phone must be powered down
                    {
                        ISHELL_SendEvent(pMe->a.m_pIShell,AEECLSID_DIALER, EVT_BATT_POWERDOWN, 0, 0);
                        CoreApp_Process_Batty_Msg(pMe, IDS_POWEROFF_1);
                        AEE_SetSysTimer(POWER_DOWN_TIME , CoreApp_Poweroff_Phone, pMe);
                        break;
                    }

                    case AEEBATTERY_STATUS_LOW:        // Battery is low
                    {
                        (void) ISHELL_CancelTimer(pMe->a.m_pIShell,CCharger_BlinkLowBattIcon, (void *) pMe);                        
                        if(pMe->m_pIAnn != NULL)
                        {
                            IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_BATT, ANNUN_STATE_BATT_LOW | ANNUN_STATE_BLINK);
                        }
                        (void) ISHELL_SetTimer(pMe->a.m_pIShell, 10000, CCharger_BlinkLowBattIcon, (void *) pMe);
                        CoreApp_Process_Batty_Msg(pMe, IDS_LOWBATTMSG_TEXT);
                        break;
                    }

                    case AEEBATTERY_STATUS_NORMAL:      // Battery is normal
                    default:
                        break;
                }
            }
            break;

        case NMASK_BATTERY_LEVEL_CHANGE:
        {
            uint32 nBattState = CoreApp_ConvertBattLvToAnnunState(CoreApp_GetBatteryLevel(pMe));
            //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BATT, ANNUN_STATE_OFF);
            IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BATT, nBattState);
            break;
        }
        
        default:
            ERR("Unknown IBatt notification", 0, 0, 0);
            break;
    }
    
    return FALSE;
}
/*==============================================================================
����: 
    CoreApp_CreateInstance
       
˵��: 
    ������������Ӧ�ó���
       
����: 
    ClsId [in]: ������ʵ����Ӧ�ö���� ID
    pIShell [in]: IShell �ӿ�ָ�롣
    pMod [in]: ������ʵ��Ӧ��������ģ��ӿ�ָ�롣
    ppObj [in/out]: ���ڷ��ش�����Ӧ�ö�����ָ����Ч�� AEEApplet �ṹ��
    
    
����ֵ:
    SUCCESS:�ɹ���Ӧ�ûᱻ����
    EFAILED:ʧ�ܣ�Ӧ�ò�������
       
��ע:
    ��Ӧ������ʱ�������������á�����ģ������ṩ��������ȷ?:������Ͳ���һ�¡��?
    �ģ�����У�� ClsId ��Ȼ������ļ� AEEAppGen.c �ṩ�ĺ��� AEEApplet_New()
    ������ AEEApplet_New() �ɹ��󣬺���������Ӧ���ض���ʼ����
==============================================================================*/
int CoreApp_CreateInstance(AEECLSID ClsId,IShell * pIShell,IModule * pMod,void ** ppObj)
{
    *ppObj = NULL;
    
    if (AEEApplet_New(sizeof(CCoreApp),                    /* Size of our private class */
                      ClsId,                               /* Our class ID */
                      pIShell,                             /* Shell interface */
                      pMod,                                /* Module instance */
                      (IApplet**)ppObj,                    /* Return object */
                      (AEEHANDLER)CoreApp_HandleEvent,     /* Our event handler */
                      (PFNFREEAPPDATA)CoreApp_FreeAppData))/* Cleanup function */
    {
        if (CoreApp_InitAppData((IApplet*)*ppObj)) 
        {
            return(AEE_SUCCESS);
        }
        
        // ��ʼ�����ɹ����ͷ�Ϊ�� Applet ������ڴ�
        IAPPLET_Release((IApplet*)*ppObj);
    }
    
    return (EFAILED);
} /* End CoreApp_CreateInstance / AEEClsCreateInstance */

#ifdef AEE_STATIC
/*==============================================================================
����: 
    CoreApp_Load
       
˵��: 
    ������������Ӧ�ó���
       
����: 
    ps [in]: IShell �ӿ�ָ�롣
    pHelpers [in]: 
    pMod [in/out]: ���ڷ��ش�����ģ���ַ��
    
����ֵ:
    ����Ӧ�õĺ���ָ��
       
��ע:

==============================================================================*/
int CoreApp_Load(IShell *ps, void * pHelpers, IModule ** pMod)
{
    int nRet;
    
    nRet = AEEStaticMod_New((int16)(sizeof(AEEMod)),
                            ps, 
                            pHelpers,
                            pMod,
                            CoreApp_CreateInstance,
                            NULL);
    return(nRet);
} /* End CoreApp_Load */

#endif  /* AEE_STATIC */

/*==============================================================================
����: 
    CoreApp_RunFSM
       
˵��: 
    Core Applet����״̬�����档
       
����: 
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    none
       
��ע:
       
==============================================================================*/
static void CoreApp_RunFSM(CCoreApp *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = CoreApp_ProcessState(pMe);

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
} // CoreApp_RunFSM

/*==============================================================================
����:
    CoreApp_RegisterNotify

˵��:
    ���ڽ������������߲���ģʽ��

����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
boolean CoreApp_RegisterNotify(CCoreApp *pMe)
{
    int nRet;
    uint32 dwMask;
#ifdef FEATURE_ICM
    // register with ICM
    dwMask = NMASK_CM_PHONE|NMASK_CM_SS|NMASK_CM_DATA_CALL;
    //dwMask = NMASK_CM_PHONE|NMASK_CM_SS;
    nRet = ISHELL_RegisterNotify(pMe->a.m_pIShell, 
                                 AEECLSID_CORE_APP, 
                                 AEECLSID_CM_NOTIFIER, 
                                 dwMask);
#else
    // register with ICM
    dwMask = AEET_NMASK_PHONE|AEET_NMASK_SS|AEET_NMASK_DATA_CALL;
    //dwMask = NMASK_CM_PHONE|NMASK_CM_SS;
    nRet = ISHELL_RegisterNotify(pMe->a.m_pIShell, 
                                 AEECLSID_CORE_APP, 
                                 AEECLSID_PHONENOTIFIER, 
                                 dwMask);
#endif
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    
    // ע�� IBatt ֪ͨ�¼�����ע���ⲿ��Դ�ͳ��״̬֪ͨ�¼�������
    // ֪ͨ�¼�һ���������������н���ע��
    dwMask = NMASK_BATTERY_STATUS_CHANGE 
            |NMASK_BATTERY_LEVEL_CHANGE
            |NMASK_BATTERY_CHARGERSTATUS_CHANGE
            |NMASK_BATTERY_EXTPWR_CHANGE;
                                           
    nRet = ISHELL_RegisterNotify(pMe->a.m_pIShell,
                        AEECLSID_CORE_APP, 
                        AEECLSID_BATTERYNOTIFIER,  
                        dwMask);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }     
    
    if(ISHELL_RegisterNotify(pMe->a.m_pIShell, AEECLSID_CORE_APP,
        AEECLSID_ALERT_NOTIFIER, NMASK_ALERT_ONOFF | NMASK_ALERT_MUTED) != SUCCESS)
    {
        return FALSE;
    }

    return TRUE;    
   
}

/*==============================================================================
����:
    CoreApp_InitExtInface

˵��:
    ���ڽ������������߲���ģʽ��

����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע     :  WHEN                    WHO                WHAT                     WHY
            2008-02-20              fj                 V1.6                     Instance AEECLSID_ALERT for power on ring       
==============================================================================*/
boolean CoreApp_InitExtInterface(CCoreApp *pMe)
{
    int nRet;
#ifdef FEATURE_ICM
    AEECMPhInfo phInfo;
    
    pMe->m_bProvisioned = FALSE;
    // ���� ICM �ӿ�
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_CALLMANAGER,
                                 (void **) &pMe->m_pCM);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    if (pMe->m_pCM == NULL) 
    {
        return FALSE;
    }
    
    ICM_SetRSSIDeltaThreshold(pMe->m_pCM, 5);
    /* If phone info is available, do not wait for PH_INFO_AVAIL event for
       * starting provisioning */
    if (!pMe->m_bProvisioned && (SUCCESS == ICM_GetPhoneInfo(pMe->m_pCM, &phInfo, sizeof(AEECMPhInfo))))
#else
    AEETPhInfo phInfo;
    
    pMe->m_bProvisioned = FALSE;
    // ���� ICM �ӿ�
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_TELEPHONE,
                                 (void **) &pMe->m_pITelephone);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    if (pMe->m_pITelephone == NULL) 
    {
        return FALSE;
    }	

	nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_CALLMGR,
                                 (void **) &pMe->m_pICallMgr);
	if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    if (pMe->m_pICallMgr == NULL) 
    {
        return FALSE;
    }
	
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_PHONECTL,
                                 (void **) &pMe->m_pIPhoneCtl);
	if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    if (pMe->m_pIPhoneCtl == NULL) 
    {
        return FALSE;
    }
	
    IPHONECTL_SetRSSIDeltaThreshold(pMe->m_pIPhoneCtl, 5);
    /* If phone info is available, do not wait for PH_INFO_AVAIL event for
       * starting provisioning */
    if (!pMe->m_bProvisioned && (SUCCESS == ITELEPHONE_GetPhoneInfo(pMe->m_pITelephone, &phInfo, sizeof(AEETPhInfo))))
#endif
    {
        InitAfterPhInfo(pMe, phInfo.oprt_mode);
    }

    // ���� IBatt �ӿ�
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_BATTERY,
                                 (void **) &pMe->m_pBatt);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_ALERT,
                                 (void **)&pMe->m_pAlert);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    
    if (pMe->m_pAlert == NULL) 
    {
        return FALSE;
    }
    
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_ANNUNCIATOR,
                                 (void **) &pMe->m_pIAnn);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    if (pMe->m_pIAnn == NULL) 
    {
        return FALSE;
    }
    else
    {
        uint32 nBattState = CoreApp_ConvertBattLvToAnnunState(CoreApp_GetBatteryLevel(pMe));
        IANNUNCIATOR_EnableAnnunciatorBar(pMe->m_pIAnn,AEECLSID_DISPLAY1,FALSE);
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BATT, nBattState);
    }
    
#ifdef FEATURE_UIALARM
    // ���� IAlarm �ӿ�
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_UIALARM,
                                 (void **)&pMe->m_pAlarm);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    if (pMe->m_pAlarm == NULL) 
    {
        return FALSE;
    }
#endif 
    
#ifndef WIN32
    // ���� IRUIM �ӿ�
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_RUIM,
                                 (void **) &pMe->m_pIRUIM);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    if (pMe->m_pIRUIM == NULL) 
    {
        return FALSE;
    }
#endif//WIN32
    
    // Create the IConfig Serivce object.
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_CONFIG,
                                (void **) &pMe->m_pConfig);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    if (pMe->m_pConfig == NULL) 
    {
        return FALSE;
    }
    
    if (ISHELL_CreateInstance(pMe->a.m_pIShell,
                                            AEECLSID_DISPLAY,
                                            (void **)&pMe->m_pDisplay)!=AEE_SUCCESS)
    {
        CoreApp_FreeAppData( (IApplet*)pMe );
        return FALSE;
    }
    
    IDisplay_SetPrefs(pMe->m_pDisplay, "a:0", 3);
    IDisplay_SetClipRect(pMe->m_pDisplay, NULL);
    pMe->m_bemergencymode = FALSE;
    return TRUE;

}

/*==============================================================================
����:
    CoreApp_PoweronStartApps

˵��:
    ����ʱ������ Applet ��Ҫ��̨����һ�Σ��Ա�ע�����Ӧ�á�

����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
static void CoreApp_PoweronStartApps(CCoreApp *pMe)
{
    static boolean bRun = FALSE;
    
    if (bRun)
    {
        return;
    }
    
#ifdef FEATURE_SUPPORT_WAP_APP
    PushMod_StartPush(pMe->a.m_pIShell);
#endif

#ifdef FEATURE_KEYGUARD
#ifdef FEATURE_ICM
    OEMKeyguard_Init(pMe->a.m_pIShell,pMe->m_pCM,pMe->m_pAlert,pMe->m_pIAnn);
#else
    OEMKeyguard_Init(pMe->a.m_pIShell,pMe->m_pITelephone,pMe->m_pAlert,pMe->m_pIAnn);
#endif
#endif
    
    bRun = TRUE;
}


/*===========================================================================
FUNCTION DBToLevel

DESCRIPTION
  Converts RSSI absolute DB value to an annunciator level

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
static int DBToLevel (int nDBVal)
{
    if (nDBVal < 92)
        return ANNUN_STATE_RSSI_4;
    else if (nDBVal < 97)
        return ANNUN_STATE_RSSI_3;
    else if (nDBVal < 102)
        return ANNUN_STATE_RSSI_2;
    else if (nDBVal < 108)
        return ANNUN_STATE_RSSI_1;
    else
        return ANNUN_STATE_RSSI_0;
}

/*=============================================================================
FUNCTION: CCharger_EnableICONCB

DESCRIPTION:

PARAMETERS:
   *pUser:

RETURN VALUE:
    None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void CCharger_EnableICONCB(void *pUser)
{
    CCoreApp    *pMe = (CCoreApp *)pUser;
    static int BattLevel = 0;
    
    (void) ISHELL_CancelTimer(pMe->a.m_pIShell, CCharger_EnableICONCB, (void *) pMe);

    if (TRUE == pMe->m_bExtPwrState)
    {
        uint32 nBattState = 0;
        
        if (BattLevel++ >= CHARGE_FULL_STATE)
        {
            BattLevel = 0;
        }
        
        nBattState = CoreApp_ConvertBattLvToAnnunState(BattLevel);
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BATT, nBattState);
        (void) ISHELL_SetTimer(pMe->a.m_pIShell, 1000, CCharger_EnableICONCB, (void *) pMe);
    }
    else
    {   //��ʱ����ֹͣ���ʱ�ĵ��level��ʾ
        uint32 nBattState = CoreApp_ConvertBattLvToAnnunState(CoreApp_GetBatteryLevel(pMe));
        IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BATT, nBattState);
    }
}

/*=============================================================================
FUNCTION: CCharger_BlinkLowBattIcon

DESCRIPTION:

PARAMETERS:
   *pUser:

RETURN VALUE:
    None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void CCharger_BlinkLowBattIcon(void *pUser)
{
    CCoreApp    *pMe = (CCoreApp *)pUser;
    //static int BattLevel = 0;

    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_BATT, ANNUN_STATE_BATT_LOW);
}
#ifdef FEATURE_ICM
/*=============================================================================
FUNCTION: CoreApp_IsEmergencyMode

DESCRIPTION: Check if the  phone  is emergency mode

PARAMETERS:
    pICM            :  [IN] ICM Instance

RETURN VALUE:
    boolean         : return TRUE if phone  is in emergency mode

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
boolean CoreApp_IsEmergencyMode(ICM* pICM)
{
    AEECMPhInfo phoneInfo;
    MSG_FATAL("CoreApp_IsEmergencyMode Start",0,0,0);
    
    //PRINT_FUNCTION_NAME();

    if (!pICM)
    {
        return FALSE;
    }

    ICM_GetPhoneInfo(pICM, &phoneInfo, sizeof(phoneInfo));
    if (phoneInfo.mode_pref == AEECM_MODE_PREF_EMERGENCY)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    MSG_FATAL("CoreApp_IsEmergencyMode End",0,0,0); 
}
#else
/*=============================================================================
FUNCTION: CoreApp_IsEmergencyMode

DESCRIPTION: Check if the  phone  is emergency mode

PARAMETERS:
    m_pITelephone            :  [IN] m_pITelephone Instance

RETURN VALUE:
    boolean         : return TRUE if phone  is in emergency mode

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
boolean CoreApp_IsEmergencyMode(ITelephone* pITelephone)
{
    AEETPhInfo phoneInfo;
    MSG_FATAL("CoreApp_IsEmergencyMode Start",0,0,0);
    
    //PRINT_FUNCTION_NAME();

    if (!pITelephone)
    {
        return FALSE;
    }

    ITELEPHONE_GetPhoneInfo(pITelephone, &phoneInfo, sizeof(phoneInfo));
    if (phoneInfo.mode_pref == AEET_MODE_PREF_EMERGENCY)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    MSG_FATAL("CoreApp_IsEmergencyMode End",0,0,0); 
}
#endif
#ifdef FEATRUE_AUTO_POWER
static void CoreApp_Process_AutoPower_Event(void *pUser)
{
    CCoreApp    *pMe = (CCoreApp *)pUser;
    db_items_value_type db;
    db_get(DB_IN_USE, &db);
    if(db.in_use)
    //if(AEECM_IS_VOICECALL_CONNECTED(pMe->m_pCM))
    {
        ISHELL_PostEvent(pMe->a.m_pIShell,AEECLSID_DIALER,EVT_AUTO_POWERDOWN,0,0);
        return;
    }
    pMe->m_b_autopower_off = TRUE;
    if (ISHELL_ActiveApplet(pMe->a.m_pIShell) != AEECLSID_CORE_APP)
    {
        pMe->m_b_needclose_core = TRUE;
        ISHELL_StartApplet(pMe->a.m_pIShell, AEECLSID_CORE_APP);        
    }
    else
    {
        if(IDIALOG_GetID(ISHELL_GetActiveDialog(pMe->a.m_pIShell)) == IDD_IDLE)
        {
            CLOSE_DIALOG(DLGRET_MSG);
        }
    }
}
#endif

#ifdef FEATURE_UIALARM
/*==============================================================================
������
       CoreApp_EnableShellAlarms

˵����
       �������ڹ����ָ�ͨ�� IAlarm �ӿ����õľ�������

������
       pMe [In]: ָ��CoreApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       bEnabled [In]: ����������
       
����ֵ��
       �ޡ�

��ע��:
       �� bEnabled ΪTRUE ʱ�ָ������������򣬹��𾯱�����
       
==============================================================================*/
void CoreApp_EnableShellAlarms(CCoreApp   *pMe, boolean  bEnabled)
{
    if (NULL == pMe)
    {
        return;
    }
    
    if (bEnabled) 
    {
        IAlarm_ResumeAlarms(pMe->m_pAlarm);
    } 
    else 
    {
        IAlarm_SuspendAlarms(pMe->m_pAlarm);
    }
}
#endif
static void CoreApp_Process_Batty_Msg(CCoreApp   *pMe, uint16  msg_id)
{
    static uint16 prv_msg_id = 0;
    if(prv_msg_id != msg_id)
    {
        AEE_CancelTimer( CoreApp_Process_Batty_Msg_CB, (void*)pMe);
        pMe->m_battery_time = 0;
        pMe->m_battery_state = TRUE;
    }

    if(pMe->m_battery_time < MAX_BATTERY_POP_COUNT && pMe->m_battery_state)
    {
        CoreNotifyMP3PlayerAlertEvent(pMe, TRUE);
        if(msg_id == IDS_LOWBATTMSG_TEXT)
            IALERT_StartAlerting(pMe->m_pAlert, NULL, NULL, AEEALERT_ALERT_LOW_BATTERY);
        else
            IALERT_StartAlerting(pMe->m_pAlert, NULL, NULL, AEEALERT_ALERT_ROAMING);
        ISHELL_SetTimer(pMe->a.m_pIShell, 1500,(PFNNOTIFY)CoreNotifyMP3PlayerAlertEventCB, pMe);
        if((pMe->m_wActiveDlgID == IDD_PWDIMSIMCC ||
            pMe->m_wActiveDlgID == IDD_PWDINPUT ||
            pMe->m_wActiveDlgID == IDD_UIMSECCODE ||
            pMe->m_wActiveDlgID == IDD_EMERGENCYNUMLIST ||
            pMe->m_wActiveDlgID == IDD_IDLE
            ) && (pMe->m_bSuspended == FALSE))
        {
            pMe->m_nMsgID = msg_id;
            MSG_FATAL("CoreApp_Process_Batty_Msg 1",0,0,0); 
            CLOSE_DIALOG(DLGRET_BATT_INFO)
        }
        pMe->m_battery_time ++;
        pMe->m_battery_state = FALSE ;
        AEE_SetSysTimer(600000/*600000 10 minute*/, CoreApp_Process_Batty_Msg_CB, (void*)pMe);
    }
    prv_msg_id = msg_id;
}
static void CoreApp_Process_Batty_Msg_CB(void *pp)
{
    CCoreApp   *pMe = (CCoreApp *)pp;
    pMe->m_battery_state = TRUE;
}

static void CoreApp_Process_Charger_Msg(CCoreApp   *pMe)
{
    //CORE_ERR("Charger_Msg %d %d",pMe->m_wActiveDlgID,pMe->m_bExtPwrState);
    if((   pMe->m_wActiveDlgID == IDD_PWDIMSIMCC ||
            pMe->m_wActiveDlgID == IDD_PWDINPUT ||
            pMe->m_wActiveDlgID == IDD_UIMSECCODE ||
            pMe->m_wActiveDlgID == IDD_EMERGENCYNUMLIST ||
            pMe->m_wActiveDlgID == IDD_IDLE
            ) && (pMe->m_bSuspended == FALSE))
    {
        MSG_FATAL("CoreApp_Process_Charger_Msg Start",0,0,0); 
        if(pMe->m_bExtPwrState)
        {
            pMe->m_nMsgID = IDS_CHARGER_ON;
         CLOSE_DIALOG(DLGRET_BATT_INFO)

        }
       else
       {
          pMe->m_nMsgID = IDS_CHARGER_OFF;
          CLOSE_DIALOG(DLGRET_BATT_INFO)

       }
    }
}

static void CoreApp_Process_Headset_Msg(CCoreApp *pMe, uint16 msgId)
{
    if((   pMe->m_wActiveDlgID == IDD_PWDIMSIMCC ||
            pMe->m_wActiveDlgID == IDD_PWDINPUT ||
            pMe->m_wActiveDlgID == IDD_UIMSECCODE ||
            pMe->m_wActiveDlgID == IDD_EMERGENCYNUMLIST ||
            pMe->m_wActiveDlgID == IDD_IDLE
            ) && (pMe->m_bSuspended == FALSE))
    {
          pMe->m_nMsgID = msgId;
          CLOSE_DIALOG(DLGRET_BATT_INFO)
    }
}
#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
/*==============================================================================
������
    CoreApp_SendReginfoTimer

˵����
    ���ڷ��� EVT_SENDREGINFOR �� AEECLSID_CORE_APP ֪ͨ�������ֻ�ע���¼���

������
    pme [in]��ָ��CCoreApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    �ޡ�

��ע��
    �� Applet �����𣬽������κβ���ֱ�ӷ��ء�
       
==============================================================================*/
void CoreApp_SendReginfoTimer(void *pme)
{
   CCoreApp *pMe = (CCoreApp *)pme;
   
   if (NULL == pMe)
   {
      return;
   }
   
   // ����EVT_DISPLAYDIALOGTIMEOUT�¼�
  (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                          AEECLSID_CORE_APP,
                          EVT_SENDREGINFOR,
                          0,
                          0);
}

/*==============================================================================
������
    CoreApp_SendReginfoCallBack

˵����
    ���� OEMTAPI �Ļص��������� OEMTAPI ��������֪ͨApplet����Ϣ����״̬��

������
    pUser {in]��void����ָ��(ʵ����ָ�� CCoreApp �ṹ��ָ��)
    status [in]����Ϣ���ͷ��صĽ���롣
       
����ֵ��
    �ޡ�

��ע:
       

==============================================================================*/
void CoreApp_SendReginfoCallBack(void * pUser, int status)
{
    CCoreApp *pMe = (CCoreApp *) pUser;
    
    if (pMe == NULL)
    {
        return;
    }
    
    (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                AEECLSID_CORE_APP,
                EVT_SENDSTATUS,
                0,
                (uint32)status);
}

/*==============================================================================
������
    CoreApp_SendReginfo

˵����
    ������������ע����Ϣ��

������
    pMe [in]��ָ��CCoreApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
    int 

��ע:

==============================================================================*/
int CoreApp_SendReginfo(CCoreApp   *pMe)
{
    IdleAPP_Config_Type cfg;
    int  result = SUCCESS;
    AEEMobileInfo     mi;
    IWmsApp *pIWmsApp = NULL;
    AECHAR  wstrType[2] = {(AECHAR)POWERUP_REGISTER_CHINAUNICOM, 0};
    //CORE_ERR("START CoreApp_SendReginfo",0,0,0);
    
    if (pMe == NULL)
    {
        return EFAILED;
    }
    
    // ��ȡ�ֻ����Ӵ���
    OEM_GetConfig (CFGI_MOBILEINFO, &mi,  sizeof(AEEMobileInfo));
    if (mi.dwESN == 0)
    {
        return EFAILED;
    }

    (void)ISHELL_GetPrefs(pMe->a.m_pIShell, 
                AEECLSID_CORE_APP, 
                IDLE_CFG_VERSION,
                &cfg, 
                sizeof(cfg));
                            
    if (cfg.nInitIndictor != IDLE_CFG_VERSION)
    {   // �����״ο���ʹ��
        cfg.nInitIndictor = IDLE_CFG_VERSION;
        (void)STRCPY(cfg.m_RegItem.szMobileIMSI, mi.szMobileID);
        cfg.m_RegItem.m_bRegistered = FALSE;
        
        // ��ʼ������
        (void)ISHELL_SetPrefs(pMe->a.m_pIShell, 
                AEECLSID_CORE_APP, 
                IDLE_CFG_VERSION,
                &cfg, 
                sizeof(cfg));
    }
    else
    {
        if ((STRCMP(cfg.m_RegItem.szMobileIMSI, mi.szMobileID)==0) &&
            cfg.m_RegItem.m_bRegistered)
        {// ��ע��,������ע��
            return SUCCESS;
        }
    }
            
    result = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_WMSAPP,
                                 (void **) &pIWmsApp);
    if ((result == SUCCESS) && (NULL != pIWmsApp))
    {
        result = IWmsApp_SendSpecMessage(pIWmsApp, wstrType);
        IWmsApp_Release(pIWmsApp);
    }
    
    //CORE_ERR("END CoreApp_SendReginfo",0,0,0);
    return result;
}   
#endif

#ifdef FEATURE_SEAMLESS_SMS
void CoreApp_SendSeamlessSMSTimer(void *pme)
{
   CCoreApp *pMe = (CCoreApp *)pme;
   
   if (NULL == pMe)
   {
      return;
   }
   
   // ����EVT_DISPLAYDIALOGTIMEOUT�¼�
  (void) ISHELL_PostEvent(pMe->a.m_pIShell,
                          AEECLSID_CORE_APP,
                          EVT_SEND_SEAMLESSSMS,
                          0,
                          0);
}

int CoreApp_SendSeamlessSMS(CCoreApp   *pMe)
{
    int  result = SUCCESS;
    AEEMobileInfo     mi;
    IWmsApp *pIWmsApp = NULL;
    AECHAR  wstrType[2] = {(AECHAR)POWERUP_REGISTER_SEAMLESSSMS, 0};
    //CORE_ERR("START CoreApp_SendSeamlessSMS",0,0,0);
    
    if (pMe == NULL)
    {
        return EFAILED;
    }
    
    // ��ȡ�ֻ����Ӵ���
    OEM_GetConfig (CFGI_MOBILEINFO, &mi,  sizeof(AEEMobileInfo));
    if (mi.dwESN == 0)
    {
        return EFAILED;
    }
    
    result = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_WMSAPP,
                                 (void **) &pIWmsApp);
    if ((result == SUCCESS) && (NULL != pIWmsApp))
    {
        result = IWmsApp_SendSpecMessage(pIWmsApp, wstrType);
        IWmsApp_Release(pIWmsApp);
    }
    
    //CORE_ERR("END CoreApp_SendSeamlessSMS",0,0,0);
    return result;
}   

#endif

int CoreApp_GetBatteryLevel(CCoreApp *pMe)
{
    uint32  pdwData;
    uint16  uBattLevel ;
    uint16  uBattScale;
    uint8   ATBattLevel;
    uint8   uLevel;
  
    if ( IBATTERY_GetBatteryLevel( pMe->m_pBatt, &(pdwData) ) != SUCCESS )
    {
        return 0;
    }
    
    uBattLevel = GETBATTERYLEVEL(pdwData);
    uBattScale = GETBATTERYSCALE(pdwData);
    
    if ( uBattScale == 0 )
    {
        return 0;
    }
    
    if ( uBattLevel == 0 )
    {
        ATBattLevel = 0;
    }
    else
    {
        ATBattLevel = (uBattLevel * 100) / uBattScale;
    }
    ERR("miaoxiaoming: ATBattLevel =%d",ATBattLevel ,0,0);
    /* convert to 0-4 levels */
    if ( ATBattLevel <= 40 )
    {
        uLevel = 0;
    }
    else if ( ATBattLevel <= 50 )
    {
        uLevel = 1;
    }
    else if ( ATBattLevel <= 65 )
    {
        uLevel = 2;
    }
    else if ( ATBattLevel <= 80 )
    {
        uLevel = 3;
    }
    else
    {
        uLevel = 4;
    }
    
    return uLevel;
}

/*==============================================================================
������
       CoreApp_ConvertBattLvToAnnunState

˵����
       ����ص����ȼ�ת��Ϊ״̬����ʾ������

������
       nBattLevel [In]: 
       
����ֵ��
       uint32

��ע��:
       �� bEnabled ΪTRUE ʱ�ָ������������򣬹��𾯱�����
       
==============================================================================*/
static uint32 CoreApp_ConvertBattLvToAnnunState(int nBattLevel)
{
    uint32 nState = 0;
    
    if(nBattLevel == 0)
    {
        nState = ANNUN_STATE_BATT_LOW;
    }
    else if(nBattLevel == 1)
    {
        nState = ANNUN_STATE_BATT_1;
    }
    else if(nBattLevel == 2)
    {
        nState = ANNUN_STATE_BATT_2;
    }
    else if(nBattLevel == 3)
    {
        nState = ANNUN_STATE_BATT_3;
    }
    else
    {
        nState = ANNUN_STATE_BATT_FULL;
    }

    return nState;
}
//end added
static void CoreNotifyMP3PlayerAlertEvent(CCoreApp *pMe, boolean toStartAlert)
{
 #ifdef FEATURE_APP_MUSICPLAYER
    if(GetMp3PlayerStatus() != MP3STATUS_NONE)
    {
        ISHELL_SendEvent(pMe->a.m_pIShell,
                         AEECLSID_APP_MUSICPLAYER,
                         EVT_ALARM,
                         toStartAlert,
                         0);
    }
 #endif
}
static void CoreNotifyMP3PlayerAlertEventCB(CCoreApp *pMe)
{
    CoreNotifyMP3PlayerAlertEvent(pMe,FALSE);
}

static void CoreAppReadNVKeyBeepValue(CCoreApp *pMe)
{
    byte p_beep_level[PROFILENUMBER]; 
    byte cur_active = 0;
    byte cur_btkeyVol = 0;
    byte btKeyVol = 0;

   (void) ICONFIG_GetItem(pMe->m_pConfig,
                    CFGI_PROFILE_CUR_NUMBER,
                    &cur_active,
                    sizeof(cur_active));
   
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_PROFILE_BEEP_VOL, 
                           &p_beep_level,
                           sizeof(p_beep_level));
    
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_BEEP_VOL, 
                           &btKeyVol,
                           sizeof(byte));
    
    
    cur_btkeyVol = p_beep_level[cur_active];
        
    if(btKeyVol != cur_btkeyVol)
    {
         btKeyVol = cur_btkeyVol; //changed NV data  
         ICONFIG_SetItem(pMe->m_pConfig,CFGI_BEEP_VOL, &btKeyVol,sizeof(byte));
    }

}

#ifdef FEATURE_UIM_RUN_TIME_ENABLE
extern nv_stat_enum_type ui_get_nv(
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to space for item */
);
extern nv_stat_enum_type ui_put_nv(
  nv_items_enum_type item,        /* which item */
  nv_item_type *data_ptr          /* pointer to data for item */
);

/*===========================================================================
FUNCTION GetRTREConfig

DESCRIPTION
  Get RTRE Config.

DEPENDENCIES
  SK UI

SIDE EFFECTS
  None
===========================================================================*/
uint64 GetRTREConfig(CCoreApp *pMe)
{
  nv_item_type nvi;
  nv_stat_enum_type result;

  result = ui_get_nv (NV_RTRE_CONFIG_I, &nvi);

  if(result != NV_DONE_S) {  /* write to NV */
#ifdef FEATURE_RTRE_DEFAULT_IS_NV
    nvi.rtre_config = NV_RTRE_CONFIG_NV_ONLY;
#else
    nvi.rtre_config = NV_RTRE_CONFIG_RUIM_ONLY;
#endif
    (void) ui_put_nv (NV_RTRE_CONFIG_I, &nvi);
  }
  return (uint64)(nvi.rtre_config);
}

/*===========================================================================
FUNCTION SetRTREConfig

DESCRIPTION
  Set RTRE Config.

DEPENDENCIES
  None

SIDE EFFECTS
  None
===========================================================================*/
boolean SetRTREConfig(CCoreApp *pMe, uint64 nNewSetting)
{
  int retVal;
  
  if (pMe == NULL) {
    //CORE_ERR("pMe is Null", 0, 0, 0);
    return FALSE;
  }

  switch (nNewSetting)
  {
#ifdef FEATURE_UIM_RUIM_W_GSM_ACCESS
    case NV_RTRE_CONFIG_SIM_ACCESS:
#ifdef FEATURE_ICM
      retVal = ICM_SetRTREConfig(pMe->m_pCM, AEECM_RTRE_CONFIG_SIM_ACCESS);
#else
      retVal = IPHONECTL_SetRTREConfig(pMe->m_pIPhoneCtl, AEECM_RTRE_CONFIG_SIM_ACCESS);
#endif
      break;
#endif
    case NV_RTRE_CONFIG_RUIM_ONLY:
#ifdef FEATURE_ICM
      retVal = ICM_SetRTREConfig(pMe->m_pCM, AEECM_RTRE_CONFIG_RUIM_ONLY);
#else
      retVal = IPHONECTL_SetRTREConfig(pMe->m_pIPhoneCtl, AEET_RTRE_CONFIG_RUIM_ONLY);
#endif
      break;
      
    case NV_RTRE_CONFIG_NV_ONLY:
#ifdef FEATURE_ICM
      retVal = ICM_SetRTREConfig(pMe->m_pCM, AEECM_RTRE_CONFIG_NV_ONLY);
#else
      retVal = IPHONECTL_SetRTREConfig(pMe->m_pIPhoneCtl, AEET_RTRE_CONFIG_NV_ONLY);
#endif
      break;
      
    case NV_RTRE_CONFIG_RUIM_OR_DROP_BACK:
    default:
#ifdef FEATURE_ICM
      retVal = ICM_SetRTREConfig(pMe->m_pCM, AEECM_RTRE_CONFIG_RUIM_OR_DROP_BACK);
#else
      retVal = IPHONECTL_SetRTREConfig(pMe->m_pIPhoneCtl, AEET_RTRE_CONFIG_RUIM_OR_DROP_BACK);
#endif
      break;
  }
  return (retVal == AEE_SUCCESS) ? TRUE : FALSE;
}

/*=============================================================================
FUNCTION: SendRTREConfig

DESCRIPTION:
  Read config from NV and send it to CM.
  This function is only executed once at powerup.

SIDE EFFECTS:
  None.

=============================================================================*/
void SendRTREConfig (CCoreApp *pMe)
{
  uint32 val;

  if ((pMe->m_bConfigSent == FALSE)
#ifdef FEATURE_MMGSDI_DUAL_SLOT
      &&
      (IsBothSlotReady(pMe))
#endif /*FEATURE_MMGSDI_DUAL_SLOT */
     )
  {
    val = GetRTREConfig(pMe);
    pMe->m_bConfigSent = SetRTREConfig(pMe, val);
  }
}
#endif /* F_UIM_RUN_TIME_ENABLE */

/*===========================================================================
FUNCTION InitAfterPhInfo

DESCRIPTION
  Executes startup sequence after phone information is available from ICM.
  If phone info is available at Core App startup, no need to wait for
  AEECM_EVENT_PH_INFO_AVAIL event.

DEPENDENCIES
  ICM interface should have been created previously.

SIDE EFFECTS
  None
===========================================================================*/
#ifdef FEATURE_ICM
void InitAfterPhInfo(CCoreApp *pMe, AEECMOprtMode mode)
{
  if (pMe == NULL)
  {
    //CORE_ERR("Null pMe",0,0,0);
    return;
  }
#ifdef FEATURE_UIM_RUN_TIME_ENABLE
  /* Send RTRE config changed to CM - response from CM will */
  /* generate subscription changed event */
  SendRTREConfig(pMe);
#endif

  //Change modes if we're provisioned properly.
  if (mode != AEECM_OPRT_MODE_FTM)
  {
    if (ui_check_provisioning() && mode != AEECM_OPRT_MODE_OFFLINE &&
        mode != AEECM_OPRT_MODE_OFFLINE_CDMA &&
        mode != AEECM_OPRT_MODE_OFFLINE_AMPS) {
      //ICM_SetOperatingMode(pMe->m_pCM, AEECM_OPRT_MODE_ONLINE);
      ISHELL_PostEventEx (pMe->a.m_pIShell, EVTFLG_ASYNC, AEECLSID_CORE_APP,
                          EVT_SET_OPERATING_MODE,AEECM_OPRT_MODE_ONLINE,0L);
    }
    else
    {
      ICM_SetOperatingMode(pMe->m_pCM, AEECM_OPRT_MODE_OFFLINE);
    }
  }
  else
  {
    ICM_SetOperatingMode(pMe->m_pCM, AEECM_OPRT_MODE_OFFLINE);
  }
  pMe->m_bProvisioned = TRUE;
}
#else
void InitAfterPhInfo(CCoreApp *pMe, AEETOprtMode mode)
{
  if (pMe == NULL)
  {
    //CORE_ERR("Null pMe",0,0,0);
    return;
  }
#ifdef FEATURE_UIM_RUN_TIME_ENABLE
  /* Send RTRE config changed to CM - response from CM will */
  /* generate subscription changed event */
  SendRTREConfig(pMe);
#endif

  //Change modes if we're provisioned properly.
  if (mode != AEET_OPRT_MODE_FTM)
  {  
    extern boolean ui_check_provisioning(void);
    if (ui_check_provisioning() && mode != AEET_OPRT_MODE_OFFLINE &&
        mode != AEET_OPRT_MODE_OFFLINE_CDMA &&
        mode != AEET_OPRT_MODE_OFFLINE_AMPS) {
      //ICM_SetOperatingMode(pMe->m_pCM, AEECM_OPRT_MODE_ONLINE);
      ISHELL_PostEventEx (pMe->a.m_pIShell, EVTFLG_ASYNC, AEECLSID_CORE_APP,
                          EVT_SET_OPERATING_MODE,AEET_OPRT_MODE_ONLINE,0L);
    }
    else
    {
      IPHONECTL_SetOperatingMode(pMe->m_pIPhoneCtl, AEET_OPRT_MODE_OFFLINE);
    }
  }
  else
  {
    IPHONECTL_SetOperatingMode(pMe->m_pIPhoneCtl, AEET_OPRT_MODE_OFFLINE);
  }
  pMe->m_bProvisioned = TRUE;
}
#endif
/*=============================================================================
FUNCTION: ProcessSubscriptionStatus

DESCRIPTION:
  Look at various parameters to see the subscription status and send it to CM

SIDE EFFECTS:
  None.

=============================================================================*/
void CoreApp_ProcessSubscriptionStatus (CCoreApp *pMe)
{
    boolean bSubAvail = TRUE;
    
    /* the technology is in slot 1 */
    /* is the pin perm disabled? or blocked */
    if (pMe->m_eUIMErrCode == UIMERR_NONE || pMe->m_eUIMErrCode == UIMERR_NOUIM)
    {
        bSubAvail = TRUE;
    }
    else
    {
        bSubAvail = FALSE;
    } /* pin is not permanently disabled */
#ifdef FEATURE_ICM
    //CORE_ERR("CoreApp_ProcessSubscriptionStatus", 0, 0, 0);
    ICM_SetSubscriptionStatus(pMe->m_pCM, AEECM_SYS_MODE_CDMA, bSubAvail);
#else
    IPHONECTL_SetSubscriptionStatus(pMe->m_pIPhoneCtl, AEET_SYS_MODE_CDMA, bSubAvail);
#endif
}

/*=============================================================================
FUNCTION: CoreApp_ProcessOffLine

DESCRIPTION:
  Display the corresponding message for offline mode

SIDE EFFECTS:
  None.

=============================================================================*/
boolean CoreApp_ProcessOffLine(CCoreApp *pMe)
{
  if (pMe == NULL)
  {
     //CORE_ERR("Null pMe Ptr", 0, 0, 0);
     return FALSE;
  }
  //CORE_ERR("CoreApp_ProcessOffLine", 0, 0, 0);
  return TRUE;

}

/*=============================================================================
FUNCTION: CoreApp_ProcessFTMMode

DESCRIPTION:
  Display the corresponding message for FTM mode

SIDE EFFECTS:
  None.

=============================================================================*/
static boolean CoreApp_ProcessFTMMode(CCoreApp *pMe)
{
#if defined (FEATURE_APP_FLDDBG)
    // Launch the debug screen of the field debug app.
    if((ISHELL_StartAppletArgs(pMe->a.m_pIShell, AEECLSID_FIELDDEBUGAPP, "FTM")) != AEE_SUCCESS) {
        //CORE_ERR("Could not start FldDbg App", 0, 0, 0);
    }
#endif
    //CORE_ERR("CoreApp_ProcessFTMMode", 0, 0, 0);
    return TRUE;
}

#ifdef USES_CONSTEFS
#define COREAPP_REBUILDEFS_BUFSIZE  1024
#define COREAPP_REBUILDEFS_VER      USES_CONSTEFS "/" PROJECTNAME ".ver"

static void CoreApp_RebuildDirFiles(IFileMgr *pFileMgr, const char * pszDir, byte *pBuff)
{
    // ö�ٵ�ǰ���ļ�
    if(SUCCESS == IFILEMGR_EnumInit(pFileMgr,pszDir,FALSE))
    {
        AEEFileInfo myInfo;
        char        szName[AEE_MAX_FILE_NAME+1];
        IFile      *pFileSrc;
        IFile      *pFileDst;
        int         nCount;
        
        while(TRUE == IFILEMGR_EnumNext(pFileMgr, &myInfo))
        {
            STRCPY(szName,AEEFS_ROOT_DIR);
            STRCAT(szName,&(myInfo.szName[sizeof(USES_CONSTEFS)]));
            //if(SUCCESS != IFILEMGR_Test(pFileMgr, szName))
            {
                pFileSrc = IFILEMGR_OpenFile(pFileMgr,myInfo.szName,_OFM_READ);
                pFileDst = IFILEMGR_OpenFile(pFileMgr,szName,_OFM_CREATE);
                if(NULL != pFileSrc && NULL != pFileDst)
                {
                    while(1)
                    {
                        nCount = IFILE_Read(pFileSrc,pBuff,COREAPP_REBUILDEFS_BUFSIZE);
                        if(nCount> 0)
                        {
                            IFILE_Write(pFileDst,pBuff,nCount);
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                RELEASEIF(pFileSrc);
                RELEASEIF(pFileDst);
            }
        }
    }
}

static void CoreApp_RebuildDir(CCoreApp *pMe, const char * pszDir, byte *pBuff)
{
    int       nRet;
    IFileMgr *pFileMgr  = NULL;

    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_FILEMGR, (void **) &pFileMgr);
    if (nRet != SUCCESS) 
    {
        goto EXIT_FUN;
    }
    
    CoreApp_RebuildDirFiles(pFileMgr, pszDir, pBuff);
    
    // ö�ٵ�ǰ���ļ���
    if(SUCCESS == IFILEMGR_EnumInit(pFileMgr,pszDir,TRUE))
    {
        AEEFileInfo myInfo;
        
        while(TRUE == IFILEMGR_EnumNext(pFileMgr, &myInfo))
        {
            CoreApp_RebuildDir(pMe, myInfo.szName, pBuff);
        }
    }
    
EXIT_FUN:
    RELEASEIF(pFileMgr);
}

static void CoreApp_RebuildEFS(CCoreApp *pMe)
{
    int       nRet;
    IFileMgr *pFileMgr  = NULL;
    IFile    *pFile     = NULL;
    byte     *pBuff     = NULL;
    
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_FILEMGR, (void **) &pFileMgr);
    if (nRet != SUCCESS) 
    {
        goto EXIT_FUN;
    }
    
    // �Ѿ����ڶ�Ӧ�汾�ŵ��ļ����������ؽ�EFS
    if(SUCCESS == IFILEMGR_Test(pFileMgr,COREAPP_REBUILDEFS_VER))
    {
        goto EXIT_FUN;
    }
    
    pBuff = (byte *)MALLOC(COREAPP_REBUILDEFS_BUFSIZE);
    if(!pBuff)
    {
        goto EXIT_FUN;
    }
    
    // �ݹ�ö�� USES_CONSTEFS �ļ���������ļ�,�����临�Ƶ� USES_CONSTEFS �滻Ϊ AEEFS_ROOT_DIR ���ļ�������
    CoreApp_RebuildDir(pMe, USES_CONSTEFS, pBuff);
    
    // EFS�ؽ���ɣ����� COREAPP_REBUILDEFS_VER �ļ�
    pFile = IFILEMGR_OpenFile(pFileMgr, COREAPP_REBUILDEFS_VER,_OFM_CREATE);
EXIT_FUN:
    FREEIF(pBuff);
    RELEASEIF(pFile);
    RELEASEIF(pFileMgr);
}
#endif

/*===========================================================================
FUNCTION StereoHeadsetOn

DESCRIPTION
  Upon receiving stereo headset on event, this function updates the
  corresponding info.

DEPENDENCIES


SIDE EFFECTS
  None
===========================================================================*/
static void StereoHeadsetOn(CCoreApp * pMe)
{
	boolean stereoHeadSetOn = TRUE;
	int     nRetVal;
	AEEDeviceNotify devnotify;

	byte			return_ringer_level[PROFILENUMBER];
	byte			return_beep_level[PROFILENUMBER];
	byte			set_ringer_level;
	byte			set_beep_level;
	byte			m_CallVolume;
	
	if ((pMe == NULL) || (pMe->m_pConfig == NULL))
	{
		MSG_ERROR("NULL pointer, pMe=0x%x", pMe, 0, 0);
		return;
	}
	if(pMe->m_pIAnn)
	{
		IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_FMRADIO/*ANNUN_FIELD_HEADSET*/, ANNUN_STATE_HEADSET_ON/*ANNUN_STATE_ON*/);
	}

	ICONFIG_SetItem(pMe->m_pConfig, CFGI_HEADSET_PRESENT, &stereoHeadSetOn, 1);

	/*Also set the headset in ICONFIG because OEMCall will use it if WCDMA is defined*/
	nRetVal = ICONFIG_SetItem(pMe->m_pConfig, CFGI_STEREO_HEADSET, &stereoHeadSetOn, 1);

	if (nRetVal != SUCCESS)
	{
		MSG_HIGH("Failed to set config item, %d", nRetVal, 0, 0);
	}

    /* Change the audio path */
    uisnd_set_device_status(SND_DEVICE_STEREO_HEADSET, UISND_DEV_ENABLED);
    uisnd_set_device_auto(NULL,NULL);
    
    //wangliang modify!  2010-09-25
	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_RINGER_VOL,
	                    return_ringer_level,
	                    sizeof(return_ringer_level));


	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_BEEP_VOL,
	                    return_beep_level,
	                    sizeof(return_beep_level));

	(void) ICONFIG_GetItem(pMe->m_pConfig,CFGI_EAR_VOL,&m_CallVolume,sizeof(byte));
	
	set_ringer_level            =   return_ringer_level[pMe->m_CurProfile];
	set_beep_level              =   return_beep_level[pMe->m_CurProfile];

	snd_set_volume( SND_DEVICE_STEREO_HEADSET, SND_METHOD_KEY_BEEP,set_beep_level, NULL, NULL );
	snd_set_volume( SND_DEVICE_STEREO_HEADSET, SND_METHOD_VOICE,m_CallVolume, NULL, NULL );		

	snd_set_volume( SND_DEVICE_STEREO_HEADSET, SND_METHOD_MESSAGE,set_ringer_level, NULL, NULL );	
	snd_set_volume( SND_DEVICE_STEREO_HEADSET, SND_METHOD_RING,set_ringer_level, NULL, NULL );	
	
#ifdef FEATURE_MULTIMEDIA
	if(!IsMp3PlayerStatusNone())  //add by miaoxiaoming
	{
		ISHELL_PostEvent(pMe->a.m_pIShell, AEECLSID_APP_MUSICPLAYER,EVT_HEADSET_CONNECT, 0, 0);
	}
#endif   



	devnotify.wParam = TRUE;
	AEE_SEND_HEADSET_EVT(&devnotify);
} /* End HeadsetOn */


/*===========================================================================
FUNCTION HeadsetOff

DESCRIPTION
  Upon receiving headset off event, this function updates the
  corresponding info.

DEPENDENCIES


SIDE EFFECTS
  None
===========================================================================*/
static void HeadsetOff(CCoreApp *pMe)
{
	boolean headSetOn = FALSE;
	int     nRetVal;
	AEEDeviceNotify devnotify;
	byte		return_ringer_level[PROFILENUMBER];
	byte	    return_beep_level[PROFILENUMBER];
	byte		set_ringer_level;
	byte		set_beep_level;
	byte        m_CallVolume;
	
	if ((pMe == NULL) || (pMe->m_pConfig == NULL))
	{
		MSG_ERROR("NULL pointer, pMe=0x%x", pMe, 0, 0);
		return;
	}

	if(pMe->m_pIAnn)
	{
		IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_FMRADIO/*ANNUN_FIELD_HEADSET*/, ANNUN_STATE_HEADSET_OFF/*ANNUN_STATE_ON*/);
	}

	ICONFIG_SetItem(pMe->m_pConfig, CFGI_HEADSET_PRESENT, &headSetOn, 1);

	devnotify.wParam = FALSE;
	AEE_SEND_HEADSET_EVT(&devnotify);
    
	/* Change the audio path */
	//uisnd_set_device_status(SND_DEVICE_STEREO_HEADSET, UISND_DEV_UNATTACHED);
	//uisnd_set_device_status(SND_DEVICE_HEADSET, UISND_DEV_UNATTACHED);

	/*Also set the headset in ICONFIG because OEMCall will use it if WCDMA is defined*/
	nRetVal = ICONFIG_SetItem(pMe->m_pConfig, CFGI_HEADSET, &headSetOn, 1);
	if (nRetVal != SUCCESS)
	{
		MSG_HIGH("Failed to set config item, %d", nRetVal, 0, 0);
	}

	nRetVal = ICONFIG_SetItem(pMe->m_pConfig, CFGI_STEREO_HEADSET, &headSetOn, 1);
	if (nRetVal != SUCCESS)
	{
		MSG_HIGH("Failed to set config item, %d", nRetVal, 0, 0);
	}
	
    uisnd_set_device_status(SND_DEVICE_STEREO_HEADSET, UISND_DEV_UNATTACHED);
    uisnd_set_device_status(SND_DEVICE_HEADSET, UISND_DEV_UNATTACHED);	
#ifdef FEATURE_FM_OEM
    uisnd_set_device_status(SND_DEVICE_HEADSET_FM, UISND_DEV_UNATTACHED);
#endif
    uisnd_set_device_auto(NULL,NULL);

//wangliang modify!  2010-09-25
	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_RINGER_VOL,
	                    return_ringer_level,
	                    sizeof(return_ringer_level));

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_BEEP_VOL,
	                    return_beep_level,
	                    sizeof(return_beep_level));

	(void) ICONFIG_GetItem(pMe->m_pConfig,CFGI_EAR_VOL,&m_CallVolume,sizeof(byte));
                                                
	set_ringer_level            =   return_ringer_level[pMe->m_CurProfile];
	set_beep_level              =   return_beep_level[pMe->m_CurProfile];

	snd_set_volume( SND_DEVICE_HANDSET, SND_METHOD_KEY_BEEP,set_beep_level, NULL, NULL );
	snd_set_volume( SND_DEVICE_HANDSET, SND_METHOD_VOICE,m_CallVolume, NULL, NULL );		

	snd_set_volume( SND_DEVICE_HANDSET, SND_METHOD_MESSAGE,set_ringer_level, NULL, NULL );	
	snd_set_volume( SND_DEVICE_HANDSET, SND_METHOD_RING,set_ringer_level, NULL, NULL );	
	
#ifdef FEATURE_MULTIMEDIA
	if(!IsMp3PlayerStatusNone())  //add by miaoxiaoming
	{
		ISHELL_PostEvent(pMe->a.m_pIShell, AEECLSID_APP_MUSICPLAYER,EVT_HEADSET_CONNECT, 0, 0);
	}
#endif

	devnotify.wParam = FALSE;
	AEE_SEND_HEADSET_EVT(&devnotify);
} /*End HeadsetOff */


static void CoreApp_SceneMode(CCoreApp *pMe)
{
	byte	return_alert_type[PROFILENUMBER],
		    return_missed_call_alert[PROFILENUMBER],
		    //return_headset_autoanswer[PROFILENUMBER],
		    return_keysnd_type[PROFILENUMBER],
		    /*return_keytone_length[PROFILENUMBER],*/
		    return_ringer_level[PROFILENUMBER],
		    return_handset_ear_level[PROFILENUMBER],
		    return_beep_level[PROFILENUMBER], //;
		    return_power_onoff[PROFILENUMBER];
	uint16 	return_start_music[PROFILENUMBER];
	uint16 	return_shutdown_music[PROFILENUMBER];

	ringID  return_call_ringer[PROFILENUMBER],
	    	return_alarm_ringer[PROFILENUMBER];

	byte	set_alert_type,
			set_missed_call_alert,
			//set_headset_autoanswer,
			set_keysnd_type,
			/*set_keytone_length,*/
			set_ringer_level,
			set_handset_ear_level,
			set_beep_level, //;
			set_power_onoff;

	ringID  set_call_ringer;
	ringID  set_alarm_ringer; 
	uint16  set_start_music,
	    	set_shutdown_music;
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
	byte    set_bt_sms_ringer, return_bt_sms_ringer[PROFILENUMBER];
	ringID  set_sms_ringer_id, return_sms_ringer_id[PROFILENUMBER];
#endif

    (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_PROFILE_ALERT_TYPE,
                           return_alert_type,
                           sizeof(return_alert_type));

#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_PROFILE_SMS_RINGER,
                           return_bt_sms_ringer,
                           sizeof(return_bt_sms_ringer));
#endif

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                   CFGI_PROFILE_CALL_RINGER,
	                   (void*)return_call_ringer,
	                   sizeof(return_call_ringer));

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_ALARM_RINGER,
	                    (void*)return_alarm_ringer,
	                    sizeof(return_alarm_ringer));

#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_SMS_RINGER_ID,
	                    (void*)return_sms_ringer_id,
	                    sizeof(return_sms_ringer_id));
#endif

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_MISSED_CALL_ALERT,
	                    return_missed_call_alert,
	                    sizeof(return_missed_call_alert));

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_STARTUP_MUSIC,
	                    return_start_music,
	                    sizeof(return_start_music));

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_SHUTDOWN_MUSIC,
	                    return_shutdown_music,
	                    sizeof(return_shutdown_music));

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_POWERONOFF_ALERT,
	                    return_power_onoff,
	                    sizeof(return_power_onoff));

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_KEYSND_TYPE,
	                    return_keysnd_type,
	                    sizeof(return_keysnd_type));

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_RINGER_VOL,
	                    return_ringer_level,
	                    sizeof(return_ringer_level));

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_EAR_VOL,
	                    return_handset_ear_level,
	                    sizeof(return_handset_ear_level));

	(void) ICONFIG_GetItem(pMe->m_pConfig,
	                    CFGI_PROFILE_BEEP_VOL,
	                    return_beep_level,
	                    sizeof(return_beep_level));

	set_alert_type              =   return_alert_type[pMe->m_CurProfile];
	set_call_ringer             =   return_call_ringer[pMe->m_CurProfile];
	set_alarm_ringer            =   return_alarm_ringer[pMe->m_CurProfile];
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
	set_bt_sms_ringer           =   return_bt_sms_ringer[pMe->m_CurProfile];
	set_sms_ringer_id           =   return_sms_ringer_id[pMe->m_CurProfile];
#endif          
	set_missed_call_alert       =   return_missed_call_alert[pMe->m_CurProfile];
	set_power_onoff             =   return_power_onoff[pMe->m_CurProfile];
	set_start_music             =   return_start_music[pMe->m_CurProfile];
	set_shutdown_music          =   return_shutdown_music[pMe->m_CurProfile];
	//set_headset_autoanswer      =   return_headset_autoanswer[pMe->m_CurProfile];
	set_keysnd_type             =   return_keysnd_type[pMe->m_CurProfile];
	//set_keytone_length          =   return_keytone_length[pMe->m_CurProfile];
	set_ringer_level            =   return_ringer_level[pMe->m_CurProfile];
	set_handset_ear_level       =   return_handset_ear_level[pMe->m_CurProfile];
	set_beep_level              =   return_beep_level[pMe->m_CurProfile];


	(void) ICONFIG_SetItem(pMe->m_pConfig,
	                  CFGI_PROFILE_CUR_NUMBER,
	                  &pMe->m_CurProfile,
	                  sizeof(byte));

	(void)ICONFIG_SetItem(pMe->m_pConfig,
	                  CFGI_ALERT_TYPE,
	                  &set_alert_type,
	                  sizeof(set_alert_type));
	                  
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
	(void)ICONFIG_SetItem(pMe->m_pConfig,
	                  CFGI_SMS_RINGER,
	                  &set_bt_sms_ringer,
	                  sizeof(set_bt_sms_ringer));
#endif

	{
		byte ring_id = 0;
		ring_id = set_call_ringer.midID;
		ICONFIG_SetItem(pMe->m_pConfig,CFGI_RINGER_TYPE,&ring_id,sizeof(byte));
	}
	{
		uint16 alarm_id = 0;
		alarm_id = set_alarm_ringer.midID;

		(void)ICONFIG_SetItem(pMe->m_pConfig,
		                      CFGI_ALARM_RINGER,
		                      &alarm_id,
		                      sizeof(alarm_id));
	}

#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
	{
		uint16 sms_id = 0;
		sms_id = set_sms_ringer_id.midID;

		(void)ICONFIG_SetItem(pMe->m_pConfig,
		                      CFGI_SMS_RINGER_ID,
		                      &sms_id,
		                      sizeof(sms_id));
	}
#endif

	(void)ICONFIG_SetItem(pMe->m_pConfig,
	                  CFGI_MISSED_CALL_ALERT,
	                  &set_missed_call_alert,
	                  sizeof(set_missed_call_alert));

	(void) ICONFIG_SetItem(pMe->m_pConfig,
	                   CFGI_PROFILE_POWERONOFF_ALERT,
	                   &set_power_onoff,
	                   sizeof(set_power_onoff));

	(void) ICONFIG_SetItem(pMe->m_pConfig,
	                   CFGI_PROFILE_POWERONOFF_ALERT,
	                   &set_start_music,
	                   sizeof(set_start_music));

	(void) ICONFIG_SetItem(pMe->m_pConfig,
	                   CFGI_PROFILE_POWERONOFF_ALERT,
	                   &set_shutdown_music,
	                   sizeof(set_shutdown_music));

	(void)ICONFIG_SetItem(pMe->m_pConfig,
	                  CFGI_KEYBEEP_SOUND,
	                  &set_keysnd_type,
	                  sizeof(set_keysnd_type));


	(void)ICONFIG_SetItem(pMe->m_pConfig,
	                  CFGI_RINGER_VOL,
	                  &set_ringer_level,
	                  sizeof(set_ringer_level));

	(void)ICONFIG_SetItem(pMe->m_pConfig,
	                  CFGI_BEEP_VOL,
	                  &set_beep_level,
	                  sizeof(set_beep_level));

	(void)ICONFIG_GetItem(pMe->m_pConfig,
	                  CFGI_PROFILE_CUR_NUMBER,
	                  &pMe->m_active,
	                  sizeof(pMe->m_active));

		
	switch(pMe->m_active)
	{
		case OEMNV_PROFILE_NORMALMODE:          //����ģʽ
		  IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_ALERT);
		  break;

		case OEMNV_PROFILE_QUIETMODE:           //����ģʽ
		  IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_SILENT);
		  break;

		case OEMNV_PROFILE_MEETING:             //����ģʽ
		  IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_VIBRATOR);
		  break;

		case OEMNV_PROFILE_NOISEMODE:           //����ģʽ
		  IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_VIBRING);
		  break;

		case OEMNV_PROFILE_CARMODE:             //����ģʽ
		  IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_VIBRING);
		  break;

		default:
		  break;
	}
      
#ifdef FEATURE_APP_MUSICPLAYER
	if(GetMp3PlayerStatus() == MP3STATUS_RUNONBACKGROUND)
    {
        ISHELL_SendEvent(pMe->a.m_pIShell,
                         AEECLSID_APP_MUSICPLAYER,
                         EVT_ALARM,
                         FALSE,
                         TRUE);
    }
#endif
}

// �������һ�μ���
static void CoreApp_ResetProfileTimer(void *pUser)
{
    CCoreApp *pMe = (CCoreApp *)pUser;
    if(!CoreApp_TestCard(pMe))
	{
		CoreApp_ResetRing(pMe);
	}
}

//add by yangdecai  2010-09-10
static boolean CoreApp_TestCard(CCoreApp *pMe)
{
	boolean Result = FALSE;
	IFileMgr          *m_pFileMgr;
	if( SUCCESS != ISHELL_CreateInstance(pMe->a.m_pIShell,
            AEECLSID_FILEMGR,
            (void**)&m_pFileMgr))
	{
		return EFAILED;
	}
	
	if(SUCCESS == IFILEMGR_Test(m_pFileMgr, AEEFS_CARD0_DIR))
	{
		Result = TRUE;
	}
	else
	{
		Result = FALSE;
	}
	RELEASEIF(m_pFileMgr);
	return Result;
}
static void    CoreApp_ResetRing(CCoreApp *pMe)
{
	ringID nNewSmsConfigRinger[PROFILENUMBER];
	ringID nNewCallConfigRinger[PROFILENUMBER];
	ringID nNewAlarmConfigRinger[PROFILENUMBER];
	boolean Relsut = FALSE;
	
	ICONFIG_GetItem(pMe->m_pConfig,CFGI_PROFILE_CALL_RINGER,(void*)nNewCallConfigRinger,sizeof(nNewCallConfigRinger));
	ICONFIG_GetItem(pMe->m_pConfig,CFGI_PROFILE_SMS_RINGER_ID,(void*)nNewSmsConfigRinger,sizeof(nNewSmsConfigRinger));
	ICONFIG_GetItem(pMe->m_pConfig,CFGI_PROFILE_ALARM_RINGER,(void*)nNewAlarmConfigRinger,sizeof(nNewAlarmConfigRinger));

	//sms

	if(nNewSmsConfigRinger[OEMNV_PROFILE_NORMALMODE].ringType == OEMNV_MP3_RINGER)
	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewSmsConfigRinger[OEMNV_PROFILE_NORMALMODE].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
    		nNewSmsConfigRinger[OEMNV_PROFILE_NORMALMODE].ringType = OEMNV_MID_RINGER;
    		nNewSmsConfigRinger[OEMNV_PROFILE_NORMALMODE].midID =OEMNV_SMS_RINGER_ID;
    		Relsut = ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_SMS_RINGER_ID,(void*)nNewSmsConfigRinger,sizeof(nNewSmsConfigRinger));
        }
	}
	if(nNewSmsConfigRinger[OEMNV_PROFILE_QUIETMODE].ringType == OEMNV_MP3_RINGER)
   	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewSmsConfigRinger[OEMNV_PROFILE_QUIETMODE].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
       		nNewSmsConfigRinger[OEMNV_PROFILE_QUIETMODE].ringType = OEMNV_MID_RINGER;
    		nNewSmsConfigRinger[OEMNV_PROFILE_QUIETMODE].midID =OEMNV_SMS_RINGER_ID;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_SMS_RINGER_ID,(void*)nNewSmsConfigRinger,sizeof(nNewSmsConfigRinger));
        }
   	}
	   
    if(nNewSmsConfigRinger[OEMNV_PROFILE_MEETING].ringType == OEMNV_MP3_RINGER)
   	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewSmsConfigRinger[OEMNV_PROFILE_MEETING].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
       		nNewSmsConfigRinger[OEMNV_PROFILE_MEETING].ringType = OEMNV_MID_RINGER;
    		nNewSmsConfigRinger[OEMNV_PROFILE_MEETING].midID =OEMNV_SMS_RINGER_ID;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_SMS_RINGER_ID,(void*)nNewSmsConfigRinger,sizeof(nNewSmsConfigRinger));
        }
   	}
	if(  nNewSmsConfigRinger[OEMNV_PROFILE_NOISEMODE].ringType == OEMNV_MP3_RINGER)
	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewSmsConfigRinger[OEMNV_PROFILE_NOISEMODE].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
    		nNewSmsConfigRinger[OEMNV_PROFILE_NOISEMODE].ringType = OEMNV_MID_RINGER;
    		nNewSmsConfigRinger[OEMNV_PROFILE_NOISEMODE].midID =OEMNV_SMS_RINGER_ID;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_SMS_RINGER_ID,(void*)nNewSmsConfigRinger,sizeof(nNewSmsConfigRinger));
        }
	}
	
	//CALL
	if(nNewCallConfigRinger[OEMNV_PROFILE_NORMALMODE].ringType == OEMNV_MP3_RINGER)
	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewCallConfigRinger[OEMNV_PROFILE_NORMALMODE].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
    		nNewCallConfigRinger[OEMNV_PROFILE_NORMALMODE].ringType = OEMNV_MID_RINGER;
    		nNewCallConfigRinger[OEMNV_PROFILE_NORMALMODE].midID =OEMNV_DEFAULTRINGER;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_CALL_RINGER,(void*)nNewCallConfigRinger,sizeof(nNewCallConfigRinger));
        }
	}
	if(nNewCallConfigRinger[OEMNV_PROFILE_QUIETMODE].ringType == OEMNV_MP3_RINGER)
   	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewCallConfigRinger[OEMNV_PROFILE_QUIETMODE].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
       		nNewCallConfigRinger[OEMNV_PROFILE_QUIETMODE].ringType = OEMNV_MID_RINGER;
    		nNewCallConfigRinger[OEMNV_PROFILE_QUIETMODE].midID =OEMNV_DEFAULTRINGER;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_CALL_RINGER,(void*)nNewCallConfigRinger,sizeof(nNewCallConfigRinger));
        }
   	}
	   
    if(nNewCallConfigRinger[OEMNV_PROFILE_MEETING].ringType == OEMNV_MP3_RINGER)
   	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewCallConfigRinger[OEMNV_PROFILE_MEETING].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
       		nNewCallConfigRinger[OEMNV_PROFILE_MEETING].ringType = OEMNV_MID_RINGER;
    		nNewCallConfigRinger[OEMNV_PROFILE_MEETING].midID =OEMNV_DEFAULTRINGER;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_CALL_RINGER,(void*)nNewCallConfigRinger,sizeof(nNewCallConfigRinger));
        }
   	}
	if(  nNewCallConfigRinger[OEMNV_PROFILE_NOISEMODE].ringType == OEMNV_MP3_RINGER)
	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewCallConfigRinger[OEMNV_PROFILE_NOISEMODE].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
    		nNewCallConfigRinger[OEMNV_PROFILE_NOISEMODE].ringType = OEMNV_MID_RINGER;
    		nNewCallConfigRinger[OEMNV_PROFILE_NOISEMODE].midID =OEMNV_DEFAULTRINGER;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_CALL_RINGER,(void*)nNewCallConfigRinger,sizeof(nNewCallConfigRinger));
        }
	}


	//ALASRM
	if(nNewAlarmConfigRinger[OEMNV_PROFILE_NORMALMODE].ringType == OEMNV_MP3_RINGER)
	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewAlarmConfigRinger[OEMNV_PROFILE_NORMALMODE].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
    		nNewAlarmConfigRinger[OEMNV_PROFILE_NORMALMODE].ringType = OEMNV_MID_RINGER;
    		nNewAlarmConfigRinger[OEMNV_PROFILE_NORMALMODE].midID = OEMNV_ALARM_RINGER;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_ALARM_RINGER,(void*)nNewAlarmConfigRinger,sizeof(nNewAlarmConfigRinger));
        }
	}
	if(nNewAlarmConfigRinger[OEMNV_PROFILE_QUIETMODE].ringType == OEMNV_MP3_RINGER)
   	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewAlarmConfigRinger[OEMNV_PROFILE_QUIETMODE].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
       		nNewAlarmConfigRinger[OEMNV_PROFILE_QUIETMODE].ringType = OEMNV_MID_RINGER;
    		nNewAlarmConfigRinger[OEMNV_PROFILE_QUIETMODE].midID = OEMNV_ALARM_RINGER;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_ALARM_RINGER,(void*)nNewAlarmConfigRinger,sizeof(nNewAlarmConfigRinger));
        }
   	}
	   
    if(nNewAlarmConfigRinger[OEMNV_PROFILE_MEETING].ringType == OEMNV_MP3_RINGER)
   	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewAlarmConfigRinger[OEMNV_PROFILE_MEETING].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
       		nNewAlarmConfigRinger[OEMNV_PROFILE_MEETING].ringType = OEMNV_MID_RINGER;
    		nNewAlarmConfigRinger[OEMNV_PROFILE_MEETING].midID = OEMNV_ALARM_RINGER;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_ALARM_RINGER,(void*)nNewAlarmConfigRinger,sizeof(nNewAlarmConfigRinger));
        }
   	}
	if(  nNewAlarmConfigRinger[OEMNV_PROFILE_NOISEMODE].ringType == OEMNV_MP3_RINGER)
	{
        if(STRNICMP(AEEFS_CARD0_DIR,nNewAlarmConfigRinger[OEMNV_PROFILE_NOISEMODE].szMusicname,STRLEN(AEEFS_CARD0_DIR)) == 0)
        {
    		nNewAlarmConfigRinger[OEMNV_PROFILE_NOISEMODE].ringType = OEMNV_MID_RINGER;
    		nNewAlarmConfigRinger[OEMNV_PROFILE_NOISEMODE].midID = OEMNV_ALARM_RINGER;
    		ICONFIG_SetItem(pMe->m_pConfig, CFGI_PROFILE_ALARM_RINGER,(void*)nNewAlarmConfigRinger,sizeof(nNewAlarmConfigRinger));
        }
	}
}
//add by yangdecai end

