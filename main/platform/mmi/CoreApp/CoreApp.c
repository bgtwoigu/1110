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

static void CoreApp_Process_SS_info(CCoreApp * pMe ,AEECMSSEventData *ss);

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
    if(g_pCoreApp)        
        return (!g_pCoreApp->m_bSuspended && IDD_IDLE==g_pCoreApp->m_wActiveDlgID);

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

    if (pMe->m_pAlert)
    {
        IALERT_Release(pMe->m_pAlert);
        pMe->m_pAlert = NULL;
    }


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
    g_pCoreApp = NULL;

#ifdef FEATURE_GREYBIT
{
    extern void GreyBitBrewFont_Done(void);
    GreyBitBrewFont_Done();
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
#ifdef FEATURE_MMGSDI
    pMe->m_nCardStatus = AEECARD_NOT_READY;
#else
    pMe->m_nCardStatus = AEECARD_NO_CARD;
#endif
    pMe->m_bSuspended = FALSE;
    
	if(SUCCESS != ISHELL_CreateInstance(pMe->a.m_pIShell,
                                        AEECLSID_CARD,
                                        (void **) &pMe->m_pICard))
	{
        return FALSE;
	}
    
    if (TRUE != CoreApp_RegisterNotify(pMe))
    {
        return FALSE;
    }
    
    if (TRUE != CoreApp_InitExtInterface(pMe))
    {
        return FALSE;
    }
	MSG_FATAL("CoreApp_InitAppData            ::::croeapp111:::",0,0,0);
    MSG_FATAL("IANNUNCIATOR_SetFieldIsActiveEx::::croeapp111:::",0,0,0);
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
    pMe->m_bProvisioned = FALSE;
    pMe->m_bConfigSent  = FALSE;
    pMe->m_cdg_msgptr = NULL;
    pMe->m_bActive = TRUE;
    
    //if phone power down abnormal, we need set CFGI_FM_BACKGROUND  false to avoid show FM in idle
    ICONFIG_SetItem(pMe->m_pConfig, CFGI_FM_BACKGROUND, &b_FMBackground, sizeof(b_FMBackground));

#ifdef FEATURE_PLANEMODE
    pMe->bPlaneModeOn = FALSE;
#endif

#ifdef FEATURE_TORCH_SUPPORT
	pMe->TorchOn = FALSE;
#endif

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
    //DBGPRINTF("%x %x %x CoreApp_HandleEvent",eCode,wParam,dwParam);
    switch (eCode)
    {
        case EVT_APP_START:
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

#ifndef WIN32
            EnableUIKeys(TRUE);
#endif//WIN32
            return TRUE;

        case EVT_APP_STOP:
            //if (pMe->m_pDisplay != NULL)
            //{
            //    (void) IDISPLAY_Release(pMe->m_pDisplay);
            //    pMe->m_pDisplay = NULL;
            //}
            pMe->m_bSuspended = TRUE;
            pMe->m_bActive = TRUE;

            if (pMe->m_bExit == FALSE)
            {
                // תΪ��̨ģʽ, ֻ���ڹػ�ʱ�����˳� Core Applet
                *((boolean *)dwParam) = FALSE;
            }
            return TRUE;

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
            if(pMe->m_pIAnn != NULL)
            {
                if((boolean)wParam)
                {
                    CoreApp_Process_Headset_Msg(pMe, IDS_HEADSET_ON);
                    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_FMRADIO/*ANNUN_FIELD_HEADSET*/, ANNUN_STATE_HEADSET_ON/*ANNUN_STATE_ON*/);
					snd_set_device(SND_DEVICE_HANDSET, SND_MUTE_MUTED, SND_MUTE_MUTED, NULL, NULL);	//Add By zzg 2010_07_18
					snd_set_device(SND_DEVICE_STEREO_HEADSET, SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, NULL, NULL);	//Add By zzg 2010_07_18
				}
                else
                {
                    CoreApp_Process_Headset_Msg(pMe, IDS_HEADSET_OFF);
                    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_FMRADIO/*ANNUN_FIELD_HEADSET*/, ANNUN_STATE_HEADSET_OFF/*ANNUN_STATE_OFF*/);
					snd_set_device(SND_DEVICE_STEREO_HEADSET, SND_MUTE_MUTED, SND_MUTE_MUTED, NULL, NULL);	//Add By zzg 2010_07_18
					snd_set_device(SND_DEVICE_HANDSET, SND_MUTE_UNMUTED, SND_MUTE_UNMUTED, NULL, NULL);	//Add By zzg 2010_07_18
				}				
            }
								
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
            ISHELL_PostEventEx(pMe->a.m_pIShell, EVTFLG_ASYNC, AEECLSID_CORE_APP, EVT_USER_REDRAW,0,0L);//need to redraw IDLE
#endif
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
            pMe->m_bemergencymode = CoreApp_IsEmergencyMode(pMe->m_pCM);
            // �ر�ȫ��ͨ������ǰ���ֹ���������ҵ���������������
            ICM_EndAllCalls(pMe->m_pCM);

            pMe->m_bSuspended = FALSE;
            pMe->m_bActive = TRUE;

            // ��״̬��
            CoreApp_RunFSM(pMe);
            return TRUE;
        }

        case EVT_DIALOG_INIT:
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_wActiveDlgID = wParam;
            CoreApp_SetDialogHandler(pMe);
            return CoreApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);

        case EVT_DIALOG_START:
            return CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) CoreApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            return TRUE;

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
                    pMe->m_cdg_msgptr = (AECHAR * )MALLOC(AEECM_MAX_ALPHA_TAG_LENGTH*sizeof(AECHAR));
                }
                if(pMe->m_cdg_msgptr != NULL)
                {
                    IDialog  * pDialog = NULL;
                    pDialog = ISHELL_GetActiveDialog(pMe->a.m_pIShell);
                    if(pDialog != NULL)
                    {
                        WSTRLCPY(pMe->m_cdg_msgptr,(AECHAR * )dwParam,AEECM_MAX_ALPHA_TAG_LENGTH);
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
           #ifdef FEATURE_TORCH_SUPPORT
				#if defined(FEATURE_PROJECT_W203) || defined(FEATURE_PROJECT_W204) 
					case AVK_SPACE:
					{
						
						if ( pMe->TorchOn == FALSE )
						{
							pMe->TorchOn = TRUE;
							if (pMe->m_pBacklight)
							{
								IBACKLIGHT_TurnOnTorch(pMe->m_pBacklight);
								//IBACKLIGHT_Disable(pMe->m_pBacklight);
							}
						}
						else
						{
							pMe->TorchOn = FALSE;
							if (pMe->m_pBacklight)
							{
								IBACKLIGHT_TurnOffTorch(pMe->m_pBacklight);
								//IBACKLIGHT_Disable(pMe->m_pBacklight);
							}
						}
											
					}

					return TRUE;
				#endif

				#if defined(FEATURE_PROJECT_W021)
					case AVK_CAMERA:
					{
						if ( pMe->TorchOn == FALSE )
						{
							pMe->TorchOn = TRUE;
							if (pMe->m_pBacklight)
							{
				        		IBACKLIGHT_TurnOnTorch(pMe->m_pBacklight);
				        		IBACKLIGHT_Disable(pMe->m_pBacklight);
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
            DBGPRINTF( ";-------------------------------");
            DBGPRINTF( ";EVT_RTC");
            if( pMe->m_eCurState == COREST_LPM)
            {
                DBGPRINTF( ";LPM, to restart coreapp");

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
            ICM_SetOperatingMode(pMe->m_pCM, (AEECMOprtMode)wParam);
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
        case AEECLSID_CM_NOTIFIER:
            return CoreApp_HandleCMNotify(pMe, pNotify);

        case AEECLSID_BATTERYNOTIFIER:
            return CoreApp_HandleBattNotify(pMe, pNotify);			
     
        default:
            break;
    }
    
    return FALSE;
}
static void CoreApp_Process_SS_info(CCoreApp * pMe ,AEECMSSEventData *ss)
{
    switch (ss->ss_info.srv_status)
    {
        case AEECM_SRV_STATUS_SRV:
        case AEECM_SRV_STATUS_LIMITED:
        {
            if(pMe->m_pIAnn == NULL)
            {
                return ;
            }

            switch (ss->ss_info.roam_status)
            {
                case AEECM_ROAM_STATUS_OFF:
                    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_WAP/*ANNUN_FIELD_ROAM*/, ANNUN_STATE_ROAM_OFF/*ANNUN_STATE_OFF*/);
                    break;

                case AEECM_ROAM_STATUS_ON:
                    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_WAP/*ANNUN_FIELD_ROAM*/, ANNUN_STATE_ROAM_ON/*ANNUN_STATE_ON*/);
                    break;

                case AEECM_ROAM_STATUS_BLINK:
                    //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_ROAM, ANNUN_STATE_BLINK);
                    break;
                    
                default:
                    IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_WAP/*ANNUN_FIELD_ROAM*/, ANNUN_STATE_ROAM_OFF/*ANNUN_STATE_OFF*/);
                    break;
            }
            break;
        }
        
        case AEECM_SRV_STATUS_NO_SRV:
        case AEECM_SRV_STATUS_PWR_SAVE:
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
    AEECMSysMode SysMode;
    boolean  bUpdate = FALSE;
    AEECMNotifyInfo *pEvtInfo;
   
    if (NULL == pNotify)
    {
        return FALSE;
    }
    
    pEvtInfo = pNotify->pData;
    
    SysMode = pEvtInfo->event_data.ss.ss_info.sys_mode;
    
    switch (pNotify->dwMask)
    {
	case NMASK_CM_DATA_CALL:
            DBGPRINTF("NMASK_CM_DATA_CALL %d",pEvtInfo->event);
            switch (pEvtInfo->event)
            {
                case AEECM_EVENT_CALL_CONNECT:
                    DBGPRINTF("AEECM_EVENT_CALL_CONNECT");
                    if(pMe->m_pIAnn != NULL)
                    {
                        IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_WAP, ANNUN_STATE_1X_ON);
                    }
                    break;
#if 0
                case AEECM_EVENT_CALL_ENTER_DORMANT:
                    DBGPRINTF("AEECM_EVENT_CALL_ENTER_DORMANT");
                    IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_WAP, ANNUN_STATE_1X_OFF);                   
                    break;
                case AEECM_EVENT_CALL_EXIT_DORMANT:
                    DBGPRINTF("AEECM_EVENT_CALL_EXIT_DORMANT");
                    IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_WAP, ANNUN_STATE_1X_ON);                   
                    break;
#endif
                case AEECM_EVENT_CALL_END: 
                    DBGPRINTF("AEECM_EVENT_CALL_END");
                    if(pMe->m_pIAnn != NULL)
                    {
                        IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_WAP, ANNUN_STATE_1X_OFF);   
                    }
                    break;
            }
            break;
       case NMASK_CM_SS:
            DBGPRINTF("NMASK_CM_SS %d %d",pEvtInfo->event,SysMode);
            switch (pEvtInfo->event)
            {
                case AEECM_EVENT_SS_SRV_CHANGED:
                case AEECM_EVENT_SS_RSSI:
                    if (SysMode == AEECM_SYS_MODE_NO_SRV)
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
                    else if (AEECM_SYS_MODE_NONE != SysMode)
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
                    
                case AEECM_EVENT_SS_HDR_RSSI:
                    break;
                    
                default:
                    break;
            }
            break;
            
        case NMASK_CM_PHONE:
            DBGPRINTF("NMASK_CM_PHONE %d",pEvtInfo->event);
            switch(pEvtInfo->event)
            {
                // System preference changed
                case AEECM_EVENT_PH_SYSTEM_PREF:
                    return TRUE;

                // PLMN: Subscription info available
                case AEECM_EVENT_PH_SUBSCRIPTION:
                    return TRUE;

                // CDMA lock mode was changed
                case AEECM_EVENT_PH_CDMA_LOCK_MODE:
                case AEECM_EVENT_PH_FUNDS_LOW:
                    return TRUE;

                // NAM selection was changed
                case AEECM_EVENT_PH_NAM_SEL:
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
                case AEECM_EVENT_PH_CURR_NAM:
                    (void) AEE_IssueSystemCallback(AEE_SCB_DEVICE_INFO_CHANGED);
                    return TRUE;

                // Answer voice as data was changed.
                case AEECM_EVENT_PH_ANSWER_VOICE:
                    return TRUE;

                // Entering powerdown sleep mode
                case AEECM_EVENT_PH_STANDBY_SLEEP:
                    return TRUE;

                // Existing powerdown sleep mode
                case AEECM_EVENT_PH_STANDBY_WAKE:
                    return TRUE;

                // DMA maintenance required command
                case AEECM_EVENT_PH_MAINTREQ:
                    return TRUE;

                // In use state was changed
                case AEECM_EVENT_PH_IN_USE_STATE:
                    return TRUE;

                // Phone information is now available
                case AEECM_EVENT_PH_INFO_AVAIL:
                    if (!pMe->m_bProvisioned) {
                       InitAfterPhInfo(pMe, pEvtInfo->event_data.ph.oprt_mode);
                    }
                    return TRUE;

                // Operating mode was changed
                case AEECM_EVENT_PH_OPRT_MODE:
                    /* continue processing of the event, e.g., update banner etc
                       this is done regardless of whether we are in the Test Mode
                       dialog or not */
                    switch (pEvtInfo->event_data.ph.oprt_mode)
                    {
                    case AEECM_OPRT_MODE_FTM:
                      return CoreApp_ProcessFTMMode(pMe);
                      
                    case AEECM_OPRT_MODE_OFFLINE:
                      return CoreApp_ProcessOffLine(pMe);
                      
                    case AEECM_OPRT_MODE_ONLINE:
                      CoreApp_ProcessSubscriptionStatus(pMe);
                      return TRUE;
                      
                    case AEECM_OPRT_MODE_LPM:
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
        ISHELL_PostEventEx(pMe->a.m_pIShell, 
                           EVTFLG_ASYNC, 
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
    
    MSG_FATAL("***zzg CoreApp_HandleBattNotify %x***",pNotify->dwMask, 0, 0);
    
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
            break;
		
        // ���״̬�ı�
        case NMASK_BATTERY_CHARGERSTATUS_CHANGE:
            
            MSG_FATAL("***zzg NMASK_BATTERY_CHARGERSTATUS_CHANGE***", 0, 0, 0);
            
            nChargerStatus = IBATTERY_GetChargerStatus(pMe->m_pBatt);

            MSG_FATAL("***zzg nChargerStatus=%d***", nChargerStatus, 0, 0);
            
            if(pMe->m_wActiveDlgID == IDD_LPM)
            {
                ISHELL_PostEventEx(pMe->a.m_pIShell, 
                                   EVTFLG_ASYNC, 
                                   AEECLSID_CORE_APP,
                                   EVT_USER_REDRAW,
                                   0,0L);
            }
            switch(nChargerStatus)
            {
                case AEEBATTERY_CHARGERSTATUS_FULLY_CHARGE:
                {
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
#ifdef FEATURE_CARRIER_THAILAND_HUTCH                        
                    (void) ISHELL_SetTimer(pMe->a.m_pIShell,500, CCharger_EnableICONCB, (void *) pMe);
#else
                    (void) ISHELL_SetTimer(pMe->a.m_pIShell,1000, CCharger_EnableICONCB, (void *) pMe);
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
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
#if defined(AEE_STATIC)
int CoreApp_CreateInstance(AEECLSID ClsId,IShell * pIShell,IModule * pMod,void ** ppObj)
#else
int AEEClsCreateInstance(AEECLSID ClsId,IShell * pIShell,IModule * pMod,void ** ppObj)
#endif
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
    AEECMPhInfo phInfo;
    
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
    
    /* If phone info is available, do not wait for PH_INFO_AVAIL event for
       * starting provisioning */
    if (!pMe->m_bProvisioned && (SUCCESS == ICM_GetPhoneInfo(pMe->m_pCM, &phInfo, sizeof(AEECMPhInfo))))
    {
        InitAfterPhInfo(pMe, phInfo.oprt_mode);
    }
    
    // register with ICM
    dwMask = NMASK_CM_PHONE|NMASK_CM_SS|NMASK_CM_DATA_CALL;
    //dwMask = NMASK_CM_PHONE|NMASK_CM_SS;
    nRet = ISHELL_RegisterNotify(pMe->a.m_pIShell, 
                                 AEECLSID_CORE_APP, 
                                 AEECLSID_CM_NOTIFIER, 
                                 dwMask);
    if (nRet != SUCCESS) 
    {
        return FALSE;
    }
    
    // ���� IBatt �ӿ�
    nRet = ISHELL_CreateInstance(pMe->a.m_pIShell,
                                 AEECLSID_BATTERY,
                                 (void **) &pMe->m_pBatt);
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
    OEMKeyguard_Init(pMe->a.m_pIShell,pMe->m_pCM,pMe->m_pAlert,pMe->m_pIAnn);
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
#ifdef FEATURE_CARRIER_THAILAND_HUTCH           
        (void) ISHELL_SetTimer(pMe->a.m_pIShell, 500, CCharger_EnableICONCB, (void *) pMe);
#else
        (void) ISHELL_SetTimer(pMe->a.m_pIShell, 1000, CCharger_EnableICONCB, (void *) pMe);
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
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
#ifdef FEATRUE_AUTO_POWER
static void CoreApp_Process_AutoPower_Event(void *pUser)
{
    CCoreApp    *pMe = (CCoreApp *)pUser;
    db_items_value_type db;
    db_get(DB_IN_USE, &db);
    if(db.in_use)
    //DBGPRINTF("EVT_AUTO_POWERDOWN");
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
    
    DBGPRINTF("coreapp btKeyVol = %d,cur_btkeyVol = %d",btKeyVol,cur_btkeyVol);
    
    if(btKeyVol != cur_btkeyVol)
    {
         btKeyVol = cur_btkeyVol; //changed NV data  
         ICONFIG_SetItem(pMe->m_pConfig,CFGI_BEEP_VOL, &btKeyVol,sizeof(byte));
    }

}

#ifdef FEATURE_UIM_RUN_TIME_ENABLE
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
    nvi.rtre_config = NV_RTRE_CONFIG_RUIM_OR_DROP_BACK;//NV_RTRE_CONFIG_RUIM_ONLY;
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
      retVal = ICM_SetRTREConfig(pMe->m_pCM, AEECM_RTRE_CONFIG_SIM_ACCESS);
      break;
#endif
    case NV_RTRE_CONFIG_RUIM_ONLY:
      retVal = ICM_SetRTREConfig(pMe->m_pCM, AEECM_RTRE_CONFIG_RUIM_ONLY);
      break;
    case NV_RTRE_CONFIG_NV_ONLY:
      retVal = ICM_SetRTREConfig(pMe->m_pCM, AEECM_RTRE_CONFIG_NV_ONLY);
      break;
    case NV_RTRE_CONFIG_RUIM_OR_DROP_BACK:
    default:
      retVal = ICM_SetRTREConfig(pMe->m_pCM, AEECM_RTRE_CONFIG_RUIM_OR_DROP_BACK);
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
void InitAfterPhInfo(CCoreApp *pMe, AEECMOprtMode mode)
{
  if (pMe == NULL)
  {
    //CORE_ERR("Null pMe",0,0,0);
    return;
  }
  DBGPRINTF("InitAfterPhInfo %d",mode,0,0);
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
    
    //CORE_ERR("CoreApp_ProcessSubscriptionStatus", 0, 0, 0);
    ICM_SetSubscriptionStatus(pMe->m_pCM, AEECM_SYS_MODE_CDMA, bSubAvail);
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

