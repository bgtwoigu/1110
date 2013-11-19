/*==============================================================================
//           U S E R   I N T E R F A C E   M A I N  M O D U L E
// �ļ�:
//        CoreAppStateHandler.c
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
#include "CoreApp_priv.h"
#include "Appscommon.h"
#ifdef FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK
#include "db.h"
#endif
#include "gsdi.h"
#include "OEMRTC.h"
#ifdef FEATURE_OEMOMH
#include "mobile.h"             // for ver_modelname
#endif
//extern int charging_mark2;
/*==============================================================================

                               �궨��ͳ���
                                 
==============================================================================*/

/*==============================================================================

                                 ���Ͷ���
                                 
==============================================================================*/


/*==============================================================================

                                 ��������
                                 
==============================================================================*/
// ״̬ COREST_NONE ������
static NextFSMAction COREST_NONE_Handler(CCoreApp *pMe);

// ״̬ COREST_INIT ������
static NextFSMAction COREST_INIT_Handler(CCoreApp *pMe);

//״̬ COREST_ALARM ������
static NextFSMAction COREST_ALARM_Handler(CCoreApp *pMe);

// ״̬ COREST_LPM ������
static NextFSMAction COREST_LPM_Handler(CCoreApp *pMe);
// ״̬ COREST_SERVICEREQ ������
//static NextFSMAction COREST_SERVICEREQ_Handler(CCoreApp *pMe);

// ״̬ COREST_VERIFYPHONEPWD ������
static NextFSMAction COREST_VERIFYPHONEPWD_Handler(CCoreApp *pMe);

// ״̬ COREST_VERIFYUIM ������
static NextFSMAction COREST_VERIFYUIM_Handler(CCoreApp *pMe);

// ״̬ COREST_UIMERR ������
static NextFSMAction COREST_UIMERR_Handler(CCoreApp *pMe);

// ״̬ COREST_EMERGENCYCALL ������
static NextFSMAction COREST_EMERGENCYCALL_Handler(CCoreApp *pMe);

// ״̬ COREST_POWERONSYSINIT ������
static NextFSMAction COREST_POWERONSYSINIT_Handler(CCoreApp *pMe);

// ״̬ COREST_STARTUPANI ������
static NextFSMAction COREST_STARTUPANI_Handler(CCoreApp *pMe);

// ״̬ COREST_POWERONAPPSDATAINIT ������
static NextFSMAction COREST_POWERONAPPSDATAINIT_Handler(CCoreApp *pMe);

// ״̬ COREST_STANDBY ������
static NextFSMAction COREST_STANDBY_Handler(CCoreApp *pMe);

#if defined(FEATURE_WMS_APP)
// ״̬ COREST_SMSTIP ������
static NextFSMAction COREST_SMSTIP_Handler(CCoreApp *pMe);
#endif

#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)||defined(FEATURE_SALESTRACKER) || defined(FEATURE_VERSION_IC241A_MMX)
#ifndef FEATURE_VERSION_C316
// ״̬ COREST_SALES_TRAKER ������
static NextFSMAction COREST_SALES_TRAKER_Handler(CCoreApp *pMe);
static NextFSMAction COREST_SALES_SUCCESS_Handler(CCoreApp *pMe);

#endif
// ״̬ COREST_SALES_EDIT ������
static NextFSMAction COREST_SALES_EDIT_Handler(CCoreApp *pMe);
#endif

// ״̬ COREST_ESN_EDIT ������
static NextFSMAction COREST_ESN_EDIT_Handler(CCoreApp *pMe);


#if defined(FEATURE_SHOW_RSSI_INFO)
// ״̬ COREST_RSSI_INFO ������
static NextFSMAction COREST_RSSI_INFO_Handler(CCoreApp *pMe);
#endif

//#if defined(FEATURE_VERSION_W317A)
// ״̬ COREST_SALES_SUCCESS ������


//#endif

// ״̬ COREST_NOTICE ������
static NextFSMAction COREST_NOTICE_Handler(CCoreApp *pMe);

// ״̬ COREST_DEEPSLEEP ������
static NextFSMAction COREST_DEEPSLEEP_Handler(CCoreApp *pMe);

// ״̬ COREST_POWEROFF ������
static NextFSMAction COREST_POWEROFF_Handler(CCoreApp *pMe);

#ifdef FEATURE_UTK2
// ״̬ COREST_UTKREFRESH ������
static NextFSMAction COREST_UTKREFRESH_Handler(CCoreApp *pMe);
#endif //FEATURE_UTK2

#ifdef FEATURE_UTK2
extern void UTK_SendTerminalProfile (void);
#endif //FEATURE_UTK2
static boolean CoreApp_Start_Alarm(CCoreApp *pMe);
#if defined( FEATURE_IDLE_LOCK_RUIM)&&defined(FEATURE_UIM)
void static isAllowIMSI(CCoreApp *pMe,boolean *lockFlg);
#endif

#ifdef USES_MMI
extern boolean   IsRunAsUIMVersion(void);
extern boolean   IsRunAsFactoryTestMode(void);
#endif
static int CoreSecurity_VerifyPIN(CCoreApp * pMe, uint8 byPinID);
static int CoreSecurity_VerifyPUK(CCoreApp * pMe, uint8 byPinID);

/*==============================================================================

                                 ȫ������
                                 
==============================================================================*/


/*==============================================================================

                                 ���أ���̬������
                                 
==============================================================================*/

/*==============================================================================

                                 ��������
                                 
==============================================================================*/
/*==============================================================================
����:
    CoreApp_ProcessState
       
˵��:
    Core Applet״̬����������?:������? pMe->m_eCurState ������·����
    ��Ӧ��״̬��������

����:
    pMe [In]: ָ�� Core Applet ����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
    NFSMACTION_CONTINUE: ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT: ָʾ��Ҫ��ʾ�Ի��������û���Ӧ�˳�״̬����

��ע:

==============================================================================*/
NextFSMAction CoreApp_ProcessState(CCoreApp *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;
    MSG_FATAL("CoreApp_ProcessState Start",0,0,0);
    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case COREST_NONE:
            MSG_FATAL("CoreApp_ProcessState Start COREST_NONE",0,0,0);
            retVal = COREST_NONE_Handler(pMe);
            break;
            
        case COREST_INIT:
            MSG_FATAL("CoreApp_ProcessState Start COREST_INIT",0,0,0);
            retVal = COREST_INIT_Handler(pMe);
            break;
        
        case COREST_ALARM:
            MSG_FATAL("CoreApp_ProcessState Start COREST_ALARM",0,0,0);
            retVal = COREST_ALARM_Handler(pMe);
            break;    
                    
        case COREST_LPM:
            MSG_FATAL("CoreApp_ProcessState Start COREST_LPM",0,0,0);
            retVal = COREST_LPM_Handler(pMe);
            break;
        //case COREST_SERVICEREQ:
        //    retVal = COREST_SERVICEREQ_Handler(pMe);
        //    break;
        case COREST_VERIFYPHONEPWD:
            MSG_FATAL("CoreApp_ProcessState Start COREST_VERIFYPHONEPWD",0,0,0);
            retVal = COREST_VERIFYPHONEPWD_Handler(pMe);
            break;
            
        case COREST_VERIFYUIM:
            MSG_FATAL("CoreApp_ProcessState Start COREST_VERIFYUIM",0,0,0);
            retVal = COREST_VERIFYUIM_Handler(pMe);
            break;
            
        case COREST_UIMERR:
            MSG_FATAL("CoreApp_ProcessState Start COREST_UIMERR",0,0,0);
            retVal = COREST_UIMERR_Handler(pMe);
            break;

        case COREST_EMERGENCYCALL:
            MSG_FATAL("CoreApp_ProcessState Start COREST_EMERGENCYCALL",0,0,0);
            retVal = COREST_EMERGENCYCALL_Handler(pMe);
            break;
            
        case COREST_POWERONSYSINIT:
            MSG_FATAL("CoreApp_ProcessState Start COREST_POWERONSYSINIT",0,0,0);
            retVal = COREST_POWERONSYSINIT_Handler(pMe);
            break;
            
        case COREST_STARTUPANI:
            MSG_FATAL("CoreApp_ProcessState Start COREST_STARTUPANI",0,0,0);
            retVal = COREST_STARTUPANI_Handler(pMe);
            break;
            
        case COREST_POWERONAPPSDATAINIT:
            MSG_FATAL("CoreApp_ProcessState Start COREST_POWERONAPPSDATAINIT",0,0,0);
            retVal = COREST_POWERONAPPSDATAINIT_Handler(pMe);
            break;
            
        case COREST_STANDBY:
            MSG_FATAL("CoreApp_ProcessState Start COREST_STANDBY",0,0,0);
            retVal = COREST_STANDBY_Handler(pMe);
            break;
            
        case COREST_NOTICE:
            MSG_FATAL("CoreApp_ProcessState Start COREST_NOTICE",0,0,0);
            retVal = COREST_NOTICE_Handler(pMe);
            break;
            
#if defined(FEATURE_WMS_APP)
        case COREST_SMSTIP:
            MSG_FATAL("CoreApp_ProcessState Start COREST_SMSTIP",0,0,0);
            retVal = COREST_SMSTIP_Handler(pMe);
            break;
#endif     

#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)||defined(FEATURE_SALESTRACKER) || defined(FEATURE_VERSION_IC241A_MMX)
#ifndef FEATURE_VERSION_C316
		case COREST_SALES_TRAKER:
			MSG_FATAL("CoreApp_ProcessState Start COREST_SALES_TRAKER",0,0,0);
            retVal = COREST_SALES_TRAKER_Handler(pMe);
			break;
		case COREST_SALES_SUCCESS:
			MSG_FATAL("CoreApp_ProcessState Start COREST_SALES_SUCCESS_Handler",0,0,0);
			retVal = COREST_SALES_SUCCESS_Handler(pMe);
			break;
#endif
		case COREST_SALES_EDIT:
			retVal = COREST_SALES_EDIT_Handler(pMe);
			break;
        case COREST_ESN_EDIT:
			retVal = COREST_ESN_EDIT_Handler(pMe);
			break;    
#endif
#ifdef FEATURE_SHOW_RSSI_INFO
        case COREST_RSSI_INFO:
            retVal = COREST_RSSI_INFO_Handler(pMe);
            break;
#endif
        
        case COREST_DEEPSLEEP:
            MSG_FATAL("CoreApp_ProcessState Start COREST_DEEPSLEEP",0,0,0);
            retVal = COREST_DEEPSLEEP_Handler(pMe);
            break;
            
        case COREST_POWEROFF:
            MSG_FATAL("CoreApp_ProcessState Start COREST_POWEROFF",0,0,0);
            retVal = COREST_POWEROFF_Handler(pMe);
            break;
            
#ifdef FEATURE_UTK2
        case COREST_UTKREFRESH:
            MSG_FATAL("CoreApp_ProcessState Start COREST_UTKREFRESH",0,0,0);
            retVal = COREST_UTKREFRESH_Handler(pMe);
            break;            
#endif //FEATURE_UTK2
//#if defined(FEATURE_VERSION_W317A)

//#endif

        default:
            break;
    }
    MSG_FATAL("curstate %d prestate %d dlgret %d CoreApp_ProcessState",pMe->m_eCurState,pMe->m_ePreState,pMe->m_eDlgRet);
    return retVal;
} // CoreApp_ProcessState

/*==============================================================================
����:
    COREST_NONE_Handler
       
˵��:
    COREST_NONE ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_NONE_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    return NFSMACTION_WAIT;
} // COREST_NONE_Handler

/*==============================================================================
����:
    COREST_INIT_Handler
       
˵��:
    COREST_INIT ״̬������
       
����:
    pMe [in]: ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE: ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT: ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_INIT_Handler(CCoreApp *pMe)
{
    CoreAppState    tepState = COREST_LPM;
    MSG_FATAL("COREST_INIT_Handler Start",0,0,0);
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            MOVE_TO_STATE(tepState)
            return NFSMACTION_CONTINUE;
            
        default:
            break;            
        
    }
    MSG_FATAL("COREST_INIT_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_INIT_Handler

/*==============================================================================
����:
    COREST_ALARM_Handler
       
˵��:
    COREST_ALARM ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_ALARM_Handler(CCoreApp *pMe)
{
    //CoreAppState    tepState;
    MSG_FATAL("COREST_ALARM_Handler Start",0,0,0);
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            CoreApp_ShowDialog(pMe, IDD_ALARM);
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:
            //MOVE_TO_STATE(COREST_STANDBY)
            //return NFSMACTION_CONTINUE;
            //CoreApp_ShowDialog(pMe, IDD_IDLE);
            MOVE_TO_STATE(COREST_VERIFYPHONEPWD)
            return NFSMACTION_CONTINUE;            

        default:
            break;            
    }
    MSG_FATAL("COREST_ALARM_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_LPM_Handler

/*==============================================================================
����:
    COREST_LPM_Handler
       
˵��:
    COREST_LPM ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_LPM_Handler(CCoreApp *pMe)
{
    CoreAppState    tepState;
    MSG_FATAL("COREST_LPM_Handler Start",0,0,0);
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            tepState = COREST_NONE;
            if(CoreApp_Start_Alarm(pMe))
            {
                tepState = COREST_ALARM;
            }
            else if (IBATTERY_GetExternalPower(pMe->m_pBatt))
            {
                static boolean lpm   = 0;
                db_items_value_type db;
                db_get(DB_POWERUPTYPE,&db);//add by xuhui
                //������Զ�������������LPM����
                if(db.db_poweruptype == DB_POWERUP_BYRESET)
                {
                    tepState = COREST_VERIFYPHONEPWD;
                }
                
                // ���������ⲿ��Դ������ػ����״̬
                MSG_FATAL("LPM test: have extern power!",0,0,0);

                if( !lpm)
                {
                    lpm = 1;
                }
                else
                {
                
#ifdef FEATRUE_AUTO_POWER		
                    OEMRTC_Process_Auto_Power_On();
#endif
#if defined( FEATURE_POWERDOWN_ALARM)
                    {
                        extern void registerAgainPowerdownAlarmclock( void);
                        registerAgainPowerdownAlarmclock();
                    }
#endif
                }
            }
            else
            {
                // ���ֻ�������֤״̬
                tepState = COREST_VERIFYPHONEPWD;
            }
            MSG_FATAL("LPM test:new state is %d",tepState,0,0);
            if (tepState != COREST_NONE)
            {
                MOVE_TO_STATE(tepState)
                return NFSMACTION_CONTINUE;
            }
            else
            {
            #ifdef FEATRUE_AUTO_POWER	
				OEMRTC_Process_Auto_Power_On();
			#endif
				CoreApp_ShowDialog(pMe, IDD_LPM);
                return NFSMACTION_WAIT;
            }
            break;
            
        case DLGRET_OK:
            MOVE_TO_STATE(COREST_VERIFYPHONEPWD)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_RTC:
        {
#if defined( FEATURE_POWERDOWN_ALARM)
            if( pMe->powerupByAlarm)
            {
                tepState = COREST_ALARM;
            }
            else
#endif				
            {
                tepState = COREST_VERIFYPHONEPWD;
            }
            MOVE_TO_STATE( tepState)
            return NFSMACTION_CONTINUE;
        }

        default:
            break;            
    }
    MSG_FATAL("COREST_LPM_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_LPM_Handler

/*==============================================================================
����:
    COREST_VERIFYPHONEPWD_Handler
       
˵��:
    COREST_VERIFYPHONEPWD ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_VERIFYPHONEPWD_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_VERIFYPHONEPWD_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK: // ����Ϣ�Ի��򷵻�
            {
                boolean bValue = FALSE;
                db_items_value_type db;
                db_get(DB_POWERUPTYPE,&db);
                
                MSG_FATAL("COREST_VERIFYPHONEPWD_Handler DLGRET_MSGOK",0,0,0);
                // ����Ƿ������ֻ��������⹦��
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                            CFGI_PHONE_PASSWORD_CHECK,
                            &bValue,
                            sizeof(bValue));
                if (bValue && db.db_poweruptype != DB_POWERUP_BYRESET)
                {
                    CoreApp_ShowDialog(pMe, IDD_PWDINPUT);
                    return NFSMACTION_WAIT;
                }
                else
                {
                    MOVE_TO_STATE(COREST_VERIFYUIM)
                }
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_ENTEROK:
            {
                uint16 wPWD=0;
				//#ifdef FEATURE_VERSION_K212_ND
               // char superpass[6] = {"0000"};
				//#else
				char superpass[6] = {"*#09#"};
				//#endif
                MSG_FATAL("COREST_VERIFYPHONEPWD_Handler DLGRET_ENTEROK",0,0,0);
                (void) ICONFIG_GetItem(pMe->m_pConfig, 
                                       CFGI_PHONE_PASSWORD,
                                       &wPWD,
                                       sizeof(uint16));
                //add by  yangdecai 2011-04-11 super password
                if ((wPWD == EncodePWDToUint16(pMe->m_strPhonePWD))||(0==strcmp(superpass,pMe->m_strPhonePWD)))
                {// �������
                    MOVE_TO_STATE(COREST_VERIFYUIM)
                    return NFSMACTION_CONTINUE;
                }
                else
                {// �������
                    // �������
                    CoreApp_ShowMsgDialog(pMe, IDS_INVALID_PASSWORD);
                    return NFSMACTION_WAIT;
                }
            }
        
        case DLGRET_EMGCALL:
            MSG_FATAL("COREST_VERIFYPHONEPWD_Handler DLGRET_EMGCALL",0,0,0);
            MOVE_TO_STATE(COREST_EMERGENCYCALL)
            return NFSMACTION_CONTINUE;

        case DLGRET_BATT_INFO:
            MSG_FATAL("COREST_VERIFYPHONEPWD_Handler DLGRET_BATT_INFO",0,0,0);
            CoreApp_ShowDialog(pMe,IDD_MSGBOX);
            return NFSMACTION_WAIT;
            
        default:
            break;
    }
    MSG_FATAL("COREST_VERIFYPHONEPWD_Handler End",0,0,0);
    return NFSMACTION_CONTINUE;
} // COREST_VERIFYPHONEPWD_Handler

/*==============================================================================
����:
    COREST_VERIFYUIM_Handler
       
˵��:
    COREST_VERIFYUIM ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_VERIFYUIM_Handler(CCoreApp *pMe)
{
    NextFSMAction  eRet = NFSMACTION_CONTINUE;
    
    MSG_FATAL("COREST_VERIFYUIM_Handler Start",0,0,0);
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK:
#ifdef FEATURE_UIM_RUN_TIME_ENABLE
            /* Send RTRE config changed to CM - response from CM will */
            /* generate subscription changed event */
            SendRTREConfig(pMe);
#endif

#if defined(FEATURE_VERSION_W208S)||defined(FEATURE_VERSION_S600S)
#ifdef FEATURE_UIM
			if (FALSE)
#else
			if (TRUE)
#endif				
#else
			if (!IsRunAsUIMVersion())
#endif 
            
            {
                pMe->m_eUIMErrCode = UIMERR_NOUIM;
                MSG_FATAL("***zzg CoreApp COREST_STARTUPANI 111***",0,0,0);
                MOVE_TO_STATE(COREST_STARTUPANI);
                return eRet;
            }  
            // ��鿨�Ƿ����
            if (IRUIM_IsCardConnected(pMe->m_pIRUIM)) 
            {// �����˿�
#if 1
                AEECardPinStatus sPinStatus;
                MSLEEP(500);// �ȴ�GSDI����PIN Status��������֤PIN֮���״̬��ȡ������
                if (ICARD_GetPinStatus(pMe->m_pICard, AEECARD_PIN1, &sPinStatus) != SUCCESS)
                {// ��ȡ״̬ʧ��
                    pMe->m_eUIMErrCode = UIMERR_BLOCKED;
                    MSG_FATAL("COREST_UIMERR..........................1111",0,0,0);
                    MOVE_TO_STATE(COREST_UIMERR)
                }
                else
                {
                    switch(sPinStatus.lock_state){
                    case AEECARD_PIN_BLOCKED:
                        // PUK��ʣ�������������0��PIN��ʣ���������Ϊ0��
                        // Ҫ���û�����PUK��
                        pMe->m_eRUIMSCode = ENTERPUK_STEP0;
                        CoreApp_ShowDialog(pMe, IDD_UIMSECCODE);
                        eRet = NFSMACTION_WAIT;
                        break;
                        
                    case AEECARD_PIN_ENABLED:
                        // PIN ���⹦�ܴ򿪣�Ҫ���û�����PIN��
                        pMe->m_eRUIMSCode = ENTERPIN_ONLY;
                        CoreApp_ShowDialog(pMe, IDD_UIMSECCODE);
                        eRet = NFSMACTION_WAIT;
                        break;
                        
                    case AEECARD_PIN_VERIFIED:
                    case AEECARD_PIN_DISABLED:
                        pMe->m_eUIMErrCode = UIMERR_NONE;
                        //MOVE_TO_STATE(COREST_POWERONSYSINIT)

                        MSG_FATAL("***zzg CoreApp COREST_STARTUPANI 222***",0,0,0);
                        
                        MOVE_TO_STATE(COREST_STARTUPANI);
                        {
                            //extern void OEMRUIMAddr_Refresh(void);
/*                            
#ifdef FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK
                            db_item.db_uimaddinited = FALSE;
                            db_put(DB_UIMADDINIT, &db_item);
#endif
*/
                            //OEMRUIMAddr_Refresh();
                        }
                        break;

                    case AEECARD_PIN_PERM_DISABLED:
                    case AEECARD_PIN_NOT_VALID:
                    case AEECARD_PIN_UNKNOWN:
                    case AEECARD_PIN_NOT_INIT:
                    default:
                        pMe->m_eUIMErrCode = UIMERR_BLOCKED;
                        MSG_FATAL("COREST_UIMERR..........................2222",0,0,0);
                        MOVE_TO_STATE(COREST_UIMERR)
                        break;
                    }
                }
#else
                AEECHVStatus  chvst;
                
                if (IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst) != SUCCESS)
                {// ��ȡ״̬ʧ��
                    pMe->m_eUIMErrCode = UIMERR_BLOCKED;
                    MOVE_TO_STATE(COREST_UIMERR)
                }
                else if (chvst.chv1UnblockRemain == 0)
                {
                    // PUK��ʣ���������Ϊ 0������Ч
                    pMe->m_eUIMErrCode = UIMERR_BLOCKED;
                    MOVE_TO_STATE(COREST_UIMERR)
                }
                else if (chvst.chv1CountRemain == 0)
                {
                    // PUK��ʣ�������������0��PIN��ʣ���������Ϊ0��
                    // Ҫ���û�����PUK��
                    pMe->m_eRUIMSCode = ENTERPUK_STEP0;
                    CoreApp_ShowDialog(pMe, IDD_UIMSECCODE);
                    eRet = NFSMACTION_WAIT;
                }
                else if (chvst.chv1_enabled)
                {
                    // PIN ���⹦�ܴ򿪣�Ҫ���û�����PIN��
                    pMe->m_eRUIMSCode = ENTERPIN_ONLY;
                    CoreApp_ShowDialog(pMe, IDD_UIMSECCODE);
                    eRet = NFSMACTION_WAIT;
                }
                else
                {
                    // UIM OK
                    pMe->m_eUIMErrCode = UIMERR_NONE;
                    //MOVE_TO_STATE(COREST_POWERONSYSINIT)
                    MOVE_TO_STATE(COREST_STARTUPANI);
                }
#endif
            }
            else
            {// û���뿨
                pMe->m_eUIMErrCode = UIMERR_NOUIM;
                if (IsRunAsFactoryTestMode())
                {
                    // ϵͳ�����ڹ�������ģʽ, �������ļ��
                    //MOVE_TO_STATE(COREST_POWERONSYSINIT)
                    MOVE_TO_STATE(COREST_STARTUPANI);
                }
                else
                {
#if 1//defined(FEATURE_VERSION_N021) || defined(FEATURE_VERSION_W208S)|| defined(FEATURE_VERSION_S600S)|| defined(FEATURE_VERSION_EC99)|| defined(FEATURE_VERSION_K212_HUALU)
                	MOVE_TO_STATE(COREST_STARTUPANI);
#else 
                    MOVE_TO_STATE(COREST_UIMERR)
#endif
                }
            }
            CoreApp_ProcessSubscriptionStatus(pMe);
            return eRet;
            
        case DLGRET_EMGCALL:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            MOVE_TO_STATE(COREST_EMERGENCYCALL)
            return NFSMACTION_CONTINUE;
            //break;
            
        case DLGRET_OK:
            switch (pMe->m_eRUIMSCode)
            {
                case ENTERPIN_ONLY:
                    // ��֤���� PIN ��
                    pMe->m_sPIN.code = pMe->m_wPIN;
                    pMe->m_sPIN.code_len = WSTRLEN(pMe->m_wPIN);
                    if(SUCCESS != CoreSecurity_VerifyPIN(pMe, AEECARD_PIN1))
                    {
                        CoreApp_ShowMsgDialog(pMe, IDS_INVALIDPIN);
                        return NFSMACTION_WAIT;
                    }
                    else
                    {
                        pMe->m_bVerifying = TRUE;
                        CoreApp_ShowMsgDialog(pMe, IDS_WAITING);
                        return NFSMACTION_WAIT;
                    }
                    break;
                    
                case ENTERPUK_STEP0:
                    pMe->m_eRUIMSCode = ENTERPUK_STEP1;
                    pMe->m_bNotOverwriteDlgRet = TRUE;
                    pMe->m_eDlgRet = DLGRET_CONTINPUT;
                    break;
                    
                case ENTERPUK_STEP1:
                    pMe->m_eRUIMSCode = ENTERPUK_STEP2;
                    pMe->m_bNotOverwriteDlgRet = TRUE;
                    pMe->m_eDlgRet = DLGRET_CONTINPUT;
                    break;
                    
                case ENTERPUK_STEP2:
                    if (STRCMP(pMe->m_strPIN2, pMe->m_strPIN1) != 0)
                    {
                        // ǰ�����벻һ��
                        CoreApp_ShowMsgDialog(pMe, IDS_PINDIFFERENT);
                        return NFSMACTION_WAIT;
                    }
                    
                    // �����û��������� PUK ����µ� PIN ��, ��ʼ���� UIM ������
                    pMe->m_sPIN.code = pMe->m_wPIN;
                    pMe->m_sPIN.code_len = WSTRLEN(pMe->m_wPIN);
                    pMe->m_sPUK.code = pMe->m_wPUK;
                    pMe->m_sPUK.code_len = WSTRLEN(pMe->m_wPUK);
                    if(SUCCESS != CoreSecurity_VerifyPUK(pMe, AEECARD_PIN1))
                    {
                        // �������,��֤ʧ��
                        CoreApp_ShowMsgDialog(pMe, IDS_INVALIDPUK);
                        return NFSMACTION_WAIT;
                    }
                    else
                    {
                        pMe->m_bVerifying = TRUE;
                        CoreApp_ShowMsgDialog(pMe, IDS_WAITING);
                        return NFSMACTION_WAIT;
                    }
                    break;
                
                default:
                    break;
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CONTINPUT:
            CoreApp_ShowDialog(pMe, IDD_UIMSECCODE);
            return NFSMACTION_WAIT;

        case DLGRET_BATT_INFO:
            CoreApp_ShowDialog(pMe,IDD_MSGBOX);
            return NFSMACTION_WAIT;
            
        default:
            break;
    }
    MSG_FATAL("COREST_VERIFYUIM_Handler End",0,0,0);            
    return NFSMACTION_WAIT;
} // COREST_VERIFYUIM_Handler

/*==============================================================================
����:
    COREST_UIMERR_Handler
       
˵��:
    COREST_UIMERR ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_UIMERR_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_UIMERR_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // ������� m_eUIMErrCode ֵ����Ӧ����ʾ
            CoreApp_ShowDialog(pMe, IDD_UIMERR);
            return NFSMACTION_WAIT;
        case DLGRET_ENTER:
        {
            char wPWD[10]= "*1796*08#";
            if (STRCMP(wPWD,pMe->m_strLockuimPWD) == 0)
            {// �������
            #ifdef FEATURE_LONG_NETLOCK
            	boolean Is_NetLockclose = TRUE;
			#endif
                pMe->bunlockuim = TRUE;
                MOVE_TO_STATE( COREST_STARTUPANI)
				#ifdef FEATURE_LONG_NETLOCK
				(void)OEM_SetConfig( CFGI_NET_LOCK_FLAGS,
									 &Is_NetLockclose,
									 sizeof(boolean));
				#endif
                return NFSMACTION_CONTINUE;
            }
            //     else
            //       {// �������
            // �������
            //         CoreApp_ShowMsgDialog(pMe, IDS_INVALID_PASSWORD);
            return NFSMACTION_WAIT;
            //      }
        }

        case DLGRET_EMGCALL:        	
            MOVE_TO_STATE(COREST_EMERGENCYCALL)
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    MSG_FATAL("COREST_UIMERR_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_UIMERR_Handler

/*==============================================================================
����:
    COREST_EMERGENCYCALL_Handler
       
˵��:
    COREST_EMERGENCYCALL ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_EMERGENCYCALL_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_EMERGENCYCALL_Handler Start",0,0,0);
    // Ϊ���н������У���������������״̬
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK:
            CoreApp_ShowDialog(pMe, IDD_EMERGENCYNUMLIST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MSG_FATAL("COREST_EMERGENCYCALL_Handler DLGRET_CANCELED",0,0,0);
            MOVE_TO_STATE(pMe->m_ePreState)
            return NFSMACTION_CONTINUE;

        case DLGRET_BATT_INFO:
            MSG_FATAL("COREST_EMERGENCYCALL_Handler DLGRET_BATT_INFO",0,0,0);
            CoreApp_ShowDialog(pMe,IDD_MSGBOX);
            return NFSMACTION_WAIT;
        
        default:
            break;
    }
    MSG_FATAL("COREST_EMERGENCYCALL_Handler End",0,0,0);    
    return NFSMACTION_WAIT;
} // COREST_EMERGENCYCALL_Handler

/*==============================================================================
����:
    COREST_POWERONSYSINIT_Handler
       
˵��:
    COREST_POWERONSYSINIT ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_POWERONSYSINIT_Handler(CCoreApp *pMe)
{

	boolean m_bsendsalessms = FALSE;
	boolean m_bsendReginfosms =FALSE;
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_POWERONSYSINIT_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
#ifdef FEATURE_OEMOMH
          
            if(!gsdi_uim_omh_cap.omh_enabled && IRUIM_IsCardConnected(pMe->m_pIRUIM))
            {
                ISHELL_SendEvent(pMe->a.m_pIShell,AEECLSID_DIALER,EVT_OMH_PROMPT,0,0);
            }
            else
            {
            	
                if((pMe->m_pIRUIM != NULL) && (pMe->m_pConfig != NULL))
                {	
                	
                    int i = 0;
                    int j = 0;
                    AECHAR swBufTemp[61]; 
                    char swBuf[61];
                    char EFmodelBuf[127];
                    #if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)
                    #ifdef FEATURE_VERSION_C260_IC18
                    #ifdef FEATURE_VERSION_C260_IC19
                    static const char mnBuf[] = "MMX C200"; //MOBLOW0283
                    #else
                    static const char mnBuf[] = "MMX C210";
                    #endif
                    #elif defined (FEATURE_VERSION_IC241A_MMX)
                    static const char mnBuf[] = "MMX IC241A";
                    #else
                    static const char mnBuf[] = "MMX C260";
                    #endif
					#elif defined (FEATURE_VERSION_V3CM301)					
					static const char mnBuf[] = "V3MOBILE";
					#else				
					static const char mnBuf[] = "WaterWorld";
					#endif
                    int len = 0;
                    static const char modename[] = CUST_MODEL;
                    EFmodelBuf[0] = 0x01; //01
                    EFmodelBuf[1] = 0x00; //00   
                    len = STRLEN(modename);
                    //     DBGPRINTF("modelBuf len =%d", len);
                    //     DBGPRINTF("EFmodelBuf =%s", EFmodelBuf);
                    //     DBGPRINTF("EFmodelBuf len =%s", STRLEN(EFmodelBuf));
                    //Write Model Information
                    for(i = 2; i < 35; ++i)
                    {
                        if(i < len + 2)
                        {
                            EFmodelBuf[i] = modename[j++];
                        }
                        else
                        {
                            j = 0;
                            EFmodelBuf[i] = 255;
                        }
                    }
                    //     DBGPRINTF("EFmodelBuf =%s", EFmodelBuf);
                    len = STRLEN(mnBuf);
                    //     DBGPRINTF("mnBuf len =%d", len);
                    //     DBGPRINTF("EFmodelBuf len =%s", STRLEN(EFmodelBuf));
                    //Write Manufacturer Name
                    for(i = 34; i < 66; ++i)
                    {
                        if(i < len + 34)
                        {
                            EFmodelBuf[i] = mnBuf[j++];
                        }
                        else
                        {
                            j = 0;
                            EFmodelBuf[i] = 255;
                        }
                    }      
                    //     DBGPRINTF("EFmodelBuf =%s", EFmodelBuf);
                    len = STRLEN(EFmodelBuf);
                    //     DBGPRINTF("EFmodelBuf len =%d", len);
                    (void) ICONFIG_GetItem(pMe->m_pConfig, CFGI_BUILD_VERSION, swBufTemp, sizeof(swBufTemp));
                    //     DBGPRINTF("swBufTemp =%S", swBufTemp);
                    //     DBGPRINTF("swBufTemp len =%d", WSTRLEN(swBufTemp));
                    WSTRTOSTR(swBufTemp, swBuf, sizeof(swBuf) + 1);
                    //     DBGPRINTF("swBuf =%s", swBuf);
                    len = STRLEN(swBuf);
                    //     DBGPRINTF("swBuf len =%d", len);
                    for(i = 66; i < 126; ++i)
                    {
                        if(i < len + 66)
                        {
                            EFmodelBuf[i] = swBuf[j++];
                        }
                        else
                        {
                            j = 0;
                            EFmodelBuf[i] = 255;
                        }                            
                    }
                    len = STRLEN(EFmodelBuf);
                    //     DBGPRINTF("EFmodelBuf len =%d", len);                        
                    EFmodelBuf[126] = '\0';
                    //     DBGPRINTF("swBuf len =%d",STRLEN(swBuf));
                    //     DBGPRINTF("EFmodelBuf =%s", EFmodelBuf);
                    IRUIM_WriteModel(pMe->m_pIRUIM, (byte*)EFmodelBuf);  
                }
            }

            // Fall Through
        case DLGRET_MSGOK:
#endif
#ifdef FEATURE_UTK2

#if defined(FEATURE_VERSION_W208S)||defined(FEATURE_VERSION_S600S)
#ifdef FEATURE_UIM
			if (pMe->m_eUIMErrCode == UIMERR_NONE)
#else
			if (FALSE)
#endif				
#else
			if (IsRunAsUIMVersion() && (pMe->m_eUIMErrCode == UIMERR_NONE))
#endif             
            { // ϵͳ�������п��汾�ҿ���ȷ����
                ui_enable_proactive(TRUE);
                // UTK Profile download
                UTK_SendTerminalProfile();
            }
#endif //FEATURE_UTK2
			MSG_FATAL("pMe->m_eUIMErrCode======%d",pMe->m_eUIMErrCode,0,0);

#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
            // ���÷���ע����ŵĶ�ʱ������
            (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                                  SENDREGINFOR_TIME,
                                  CoreApp_SendReginfoTimer, 
                                  pMe);

            MSG_FATAL("ISHELL_SetTimer CoreApp_SendReginfoTimer",0,0,0);

#endif

#if defined(FEATURE_VERSION_K212_20D)
			if (IRUIM_IsCardConnected(pMe->m_pIRUIM)) 
			{
			 MSG_FATAL("ISHELL_SetTimer m_bsendReginfosms1=%d",m_bsendReginfosms,0,0);
			 (void) ICONFIG_GetItem(pMe->m_pConfig, CFGI_REGINFOSMS_SEND,&m_bsendReginfosms, sizeof(m_bsendReginfosms));
             MSG_FATAL("ISHELL_SetTimer m_bsendReginfosms2=%d",m_bsendReginfosms,0,0);   
			  if(!m_bsendReginfosms)
			  	{
                   (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
		                              SMS_TIME,
		                              CoreApp_ReginfosmsTimer, 
		                              pMe);
			    }
            }

#endif

#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)||defined(FEATURE_SALESTRACKER) || defined(FEATURE_VERSION_IC241A_MMX)
			// 
			if (IRUIM_IsCardConnected(pMe->m_pIRUIM)) 
			{

#if !defined(FEATURE_VERSION_W021_CT100_SALES_TRACK)			
		        (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
		                              MOBILETRACKERREGINFOR_TIME,
		                              CoreApp_MobileTrackerTimer, 
		                              pMe);
#endif                

#if defined (FEATURE_IC19_ESN_TRACKER)
                (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
		                              ESN_TRACKER_TIMER,
		                              CoreApp_EsnTrackerTimer, 
		                              pMe);
#endif

				(void) ICONFIG_GetItem(pMe->m_pConfig,	
										   CFGI_SMS_TRACKER_SEND_B,
										   &m_bsendsalessms, 
										   sizeof(m_bsendsalessms));

				MSG_FATAL("m_bsendsalessms======%d",m_bsendsalessms,0,0);
#if defined(FEATURE_VERSION_W021_CT100_ESN_TRACK)             
                if(TRUE)	
#else
                if(!m_bsendsalessms)
#endif
			    {
			    	#ifndef  FEATURE_VERSION_C316
			    	(void)ISHELL_SetTimer(pMe->a.m_pIShell, 
		                              SMS_TIME,
		                              CoreApp_SalesTrackerTimer, 
		                              pMe);
					#endif
					
			    }
		        //MSG_FATAL("IAlarm_SetAlarm CoreApp_MobileTrackerTimer",0,0,0);
			}
			else
			{
				MSG_FATAL("NO UIM CARD...............",0,0,0);
			}
#endif

#ifdef FEATURE_SEAMLESS_SMS
            (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                                  SENDSEAMLESSSMS_TIME,
                                  CoreApp_SendSeamlessSMSTimer, 
                                  pMe);
#endif
            //MOVE_TO_STATE(COREST_POWERONAPPSDATAINIT)//FJ REMARK
            pMe->m_wStartupAniTime = 0;
            //MOVE_TO_STATE(COREST_STARTUPANI);
            {
#ifdef FEATURE_PLANEMODE
                byte planeModeCfg;
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_PLANEMODE,
                                       &planeModeCfg,
                                       sizeof(planeModeCfg));              
                if(OEMNV_PLANEMODE_QUERY == planeModeCfg)
                {
                    pMe->m_nMsgID = IDS_PLANEMODE_QUERY;
                    CoreApp_ShowDialog(pMe,IDD_MSGBOX);
                    return NFSMACTION_WAIT;
                }
                else if(OEMNV_PLANEMODE_ON== planeModeCfg)
                {
                    pMe->bPlaneModeOn = TRUE;
                    IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_RSSI, ANNUN_STATE_AIR_MODE_ON );
#ifdef FEATURE_VERSION_EC99
                    IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_3G_RSSI, ANNUN_STATE_AIR_MODE_ON );
#endif                     
                    cm_ph_cmd_oprt_mode(NULL, NULL, CM_CLIENT_ID_ANONYMOUS, SYS_OPRT_MODE_LPM);
                    MOVE_TO_STATE(COREST_STANDBY);
                    return NFSMACTION_CONTINUE;
                }
                else
#endif //FEATURE_PLANEMODE           
                {
                    MOVE_TO_STATE(COREST_STANDBY);
                }
            }

            return NFSMACTION_CONTINUE;                               

#ifdef FEATURE_PLANEMODE
        case DLGRET_NO:
            MOVE_TO_STATE(COREST_STANDBY);
            return NFSMACTION_CONTINUE;

        case DLGRET_YES:
            pMe->bPlaneModeOn = TRUE;
            IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_RSSI, ANNUN_STATE_AIR_MODE_ON );
#ifdef FEATURE_VERSION_EC99
            IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_3G_RSSI, ANNUN_STATE_AIR_MODE_ON );
#endif            
            cm_ph_cmd_oprt_mode(NULL, NULL, CM_CLIENT_ID_ANONYMOUS, SYS_OPRT_MODE_LPM);
            MOVE_TO_STATE(COREST_STANDBY);
            return NFSMACTION_CONTINUE;

#endif //FEATURE_PLANEMODE

        default:
            break;
    }
    MSG_FATAL("COREST_POWERONSYSINIT_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_POWERONSYSINIT_Handler

/*==============================================================================
����:
    COREST_STARTUPANI_Handler
       
˵��:
    COREST_STARTUPANI ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_STARTUPANI_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_STARTUPANI_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:			
            if(pMe->bunlockuim)                   //�����������������ȷ���˲���Ϊ��                     
            {                     
                 pMe->m_eUIMErrCode = UIMERR_NONE;
            }
            
#if defined( FEATURE_IDLE_LOCK_RUIM)&&defined(FEATURE_UIM)

#if defined(FEATURE_VERSION_W208S)||defined(FEATURE_VERSION_S600S)
#ifdef FEATURE_UIM
			if (!pMe->bunlockuim && IRUIM_IsCardConnected(pMe->m_pIRUIM))
#else
			if (FALSE)
#endif				
#else
			if (!pMe->bunlockuim && IsRunAsUIMVersion() && IRUIM_IsCardConnected(pMe->m_pIRUIM))
#endif            
            {
                boolean lockFlg = TRUE;     //not find right MCC and MNC value lable should lock current RUIM 
                isAllowIMSI(pMe,&lockFlg);
                if (lockFlg)
                {
                     // ����������Ӫ��Ҫ�󣬿���Ч
                     pMe->m_eUIMErrCode = UIMERR_LOCKED;
                     MSG_FATAL("COREST_UIMERR..........................4444",0,0,0);
                     MOVE_TO_STATE(COREST_UIMERR)
                     return NFSMACTION_CONTINUE;
                }
            }
#endif //defined( FEATURE_IDLE_LOCK_RUIM)&&defined(FEATURE_UIM)
#ifdef FEATURE_NET_LOCK
{
			nv_item_type    nviOldSimChoice;
			int ret = 0;
            extern boolean OEM_IsNetLock(void);
			#ifdef FEATURE_LONG_NETLOCK
			boolean Is_NetLockclose = FALSE;
			(void)OEM_GetConfig( CFGI_NET_LOCK_FLAGS,
									 &Is_NetLockclose,
									 sizeof(boolean));
			//add by wenyu,��ȡV3C137��SPN��
			#ifdef FEATURE_VERSION_V3C137
			CoreApp_GetSPN(pMe);
			if(pMe->svc_p_name[0] != 0)
        { 
        #ifdef FEATURE_OEMOMH
            MSG_FATAL("CoreApp_DrawBannerMessage hasGetSPN = TRUE",0,0,0);
		#endif

        }
			#endif
			MSG_FATAL("Is_NetLockclose:::::::::::::::::::::%d",Is_NetLockclose,0,0);
			if(!Is_NetLockclose)
			{
				MSG_FATAL("pMe->bunlockuim=%d,IRUIM_IsCardConnected(pMe->m_pIRUIM)=%d,OEM_IsNetLock=%d",pMe->bunlockuim,IRUIM_IsCardConnected(pMe->m_pIRUIM),OEM_IsNetLock());
				if(!pMe->bunlockuim && IRUIM_IsCardConnected(pMe->m_pIRUIM) && OEM_IsNetLock())
	            {
	            	
	                pMe->m_eUIMErrCode = UIMERR_LOCKED;
	                MSG_FATAL("COREST_UIMERR..........................5555",0,0,0);
	                MOVE_TO_STATE(COREST_UIMERR)
	                return NFSMACTION_CONTINUE;
	            }
			}
			else
			{
					/*
						if(!pMe->bunlockuim && IRUIM_IsCardConnected(pMe->m_pIRUIM))
			            {
			            	
			                pMe->m_eUIMErrCode = UIMERR_LOCKED;
			                MOVE_TO_STATE(COREST_UIMERR)
			                return NFSMACTION_CONTINUE;
			            }*/
			}
			#else
		//#ifdef FEATURE_PROJECT_M8
		#ifdef FTATURE_LOCK_SIM1_ONLY	
			ret =OEMNV_Get(NV_SIM_SELECT_I,&nviOldSimChoice);
            if( NV_NOTACTIVE_S != ret)
            {
            	if ( nviOldSimChoice.sim_select != OEMNV_SIMFORM_TWO )
                {
                	int i = (int)OEM_IsNetLock();
                	MSG_FATAL("pMe->bunlockuim--=============%d,i=%d",pMe->bunlockuim,i,0);
                	
					if(!pMe->bunlockuim && IRUIM_IsCardConnected(pMe->m_pIRUIM) && OEM_IsNetLock())
		            {
		                pMe->m_eUIMErrCode = UIMERR_LOCKED;
		                MSG_FATAL("COREST_UIMERR..........................66666",0,0,0);
		                MOVE_TO_STATE(COREST_UIMERR)
		                return NFSMACTION_CONTINUE;
		            }
                }
            }			
		#else
            if(!pMe->bunlockuim && IRUIM_IsCardConnected(pMe->m_pIRUIM) && OEM_IsNetLock())
            {
                pMe->m_eUIMErrCode = UIMERR_LOCKED;
                MSG_FATAL("COREST_UIMERR..........................777777",0,0,0);
                MOVE_TO_STATE(COREST_UIMERR)
                return NFSMACTION_CONTINUE;
            }
        #endif
			#endif
}
#endif
            CoreApp_ProcessSubscriptionStatus(pMe);
            if(pMe->m_bemergencymode)
            {
#ifdef FEATURE_ICM
                //exit the emergency mode
                ICM_SetSystemPreference(pMe->m_pCM,
                                        AEECM_MODE_PREF_PERSISTENT, AEECM_PREF_TERM_PERMANENT, 0,
                                        AEECM_GW_ACQ_ORDER_PREF_NO_CHANGE, AEECM_BAND_PREF_NO_CHANGE,
                                        AEECM_ROAM_PREF_NO_CHANGE, AEECM_HYBR_PREF_NO_CHANGE,
                                        AEECM_SRV_DOMAIN_PREF_NO_CHANGE, AEECM_NETWORK_SEL_MODE_PREF_NO_CHANGE,
                                        NULL, NULL, NULL);
#else
            	AEETSystemPreference tSysPref={
												AEET_MODE_PREF_PERSISTENT, 
												AEET_PREF_TERM_PERMANENT, 
												0,
		                                        AEET_GW_ACQ_ORDER_PREF_NO_CHANGE,
		                                        AEET_BAND_PREF_NO_CHANGE,
		                                        AEET_ROAM_PREF_NO_CHANGE,
		                                        AEET_HYBR_PREF_NO_CHANGE,
		                                        AEET_SRV_DOMAIN_PREF_NO_CHANGE, 
		                                        AEET_NETWORK_SEL_MODE_PREF_NO_CHANGE,
		                                        {0xFF,0xFF,0xFF}};  /*if we don,t use plmn set it to 0xff,0xff,0xff*/
                //exit the emergency mode
                IPHONECTL_SetSystemPreference(pMe->m_pIPhoneCtl,NULL,&tSysPref,sizeof(AEETSystemPreference),NULL);
#endif
                pMe->m_bemergencymode = FALSE;
            }
            CoreApp_ShowDialog(pMe, IDD_STARTUPANI);
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:
            MOVE_TO_STATE(COREST_POWERONAPPSDATAINIT)
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    MSG_FATAL("COREST_STARTUPANI_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_STARTUPANI_Handler

/*==============================================================================
����:
    COREST_POWERONAPPSDATAINIT_Handler
       
˵��:
    COREST_POWERONAPPSDATAINIT ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_POWERONAPPSDATAINIT_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MSG_FATAL("COREST_POWERONAPPSDATAINIT_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
#ifndef FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK
            CoreApp_ShowDialog(pMe, IDD_LOADING);
            return NFSMACTION_WAIT;
            
        case DLGRET_INITED:
#else
            db_setuiminitmask(INITUIMSMSMASK);
#endif
            //if(CoreApp_Start_Alarm(pMe)) //is need show rtc power on alarm?
            //{
            //    CORE_ERR("COREST_ALARM:",0,0,0);
            //    //CoreApp_ShowDialog(pMe, IDD_ALARM);
            //    MOVE_TO_STATE(COREST_ALARM)
            //}
            //else
            {
                //CORE_ERR("COREST_STANDBY:");  
                //MOVE_TO_STATE(COREST_STANDBY)
                MOVE_TO_STATE(COREST_POWERONSYSINIT)
            }
#ifdef FEATURE_RUIM_PHONEBOOK

#if defined(FEATURE_VERSION_W208S)||defined(FEATURE_VERSION_S600S)
#ifdef FEATURE_UIM
			if (TRUE)
#else
			if (FALSE)
#endif			 	
#else
			if (IsRunAsUIMVersion())
#endif            
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
#endif
            if (NULL == pMe->m_pAddrPhone)
            {            
                (void) ISHELL_CreateInstance( pMe->a.m_pIShell,
                                      AEECLSID_ADDRBOOK,
                                      (void **)&pMe->m_pAddrPhone);
            }
            
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    MSG_FATAL("COREST_POWERONAPPSDATAINIT_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_POWERONAPPSDATAINIT_Handler

/*==============================================================================
����:
    COREST_STANDBY_Handler
       
˵��:
    COREST_STANDBY ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_STANDBY_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //#ifdef FEATRUE_AUTO_POWER
    //#endif
    MSG_FATAL("COREST_STANDBY_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK:
            CoreApp_ProcessSubscriptionStatus(pMe);
            
            MSG_FATAL("COREST_STANDBY_Handler DLGRET_CREATE",0,0,0);			
#ifdef FEATRUE_AUTO_POWER
            if(pMe->m_b_autopower_off)
            {
                CoreApp_ShowMsgDialog(pMe,IDS_AUTO_POWER_OFF);
            }
            else
#endif /* FEATRUE_AUTO_POWER */                
            {
#ifdef FEATRUE_SET_ANN_FULL_SCREEN
#endif
                CoreApp_ShowDialog(pMe, IDD_IDLE);
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_MSG:
            MSG_FATAL("COREST_STANDBY_Handler DLGRET_MSG",0,0,0);			
#ifdef FEATRUE_AUTO_POWER
            if(pMe->m_b_autopower_off)
            {
                CoreApp_ShowMsgDialog(pMe,IDS_AUTO_POWER_OFF);
            }
            else
#endif /* FEATRUE_AUTO_POWER */
            if(pMe->m_nMsgID ==  0xFFFF && pMe->m_cdg_msgptr != NULL)
            {
                CoreApp_ShowMsgDialog(pMe,pMe->m_nMsgID);
            }
            else
            {
                CoreApp_ShowMsgDialog(pMe,IDS_EXIT_EMERGENCY_MODE);
            }

            return NFSMACTION_WAIT;
            
#if defined(FEATURE_WMS_APP)
        case DLGRET_SMSTIPS:
            MSG_FATAL("COREST_STANDBY_Handler DLGRET_SMSTIPS",0,0,0);
            MOVE_TO_STATE(COREST_SMSTIP)
            return NFSMACTION_CONTINUE;
#endif      
#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_SALESTRACKER) || defined(FEATURE_VERSION_IC241A_MMX)
		case DLGRET_SALES_TRACKER:
			MSG_FATAL("COREST_STANDBY_Handler DLGRET_SALES_TRACKER",0,0,0);
            MOVE_TO_STATE(COREST_SALES_TRAKER)
            return NFSMACTION_CONTINUE;
//#if defined(FEATURE_VERSION_W317A)

		case DLGRET_SALES_SUCESS:
			MSG_FATAL("DLGRET_SALES_SUCESS_Handler DLGRET_SALES_TRACKER",0,0,0);
            MOVE_TO_STATE(COREST_SALES_SUCCESS)
//#endif
            return NFSMACTION_CONTINUE;
		case DLGRET_WMS_DRAFT_SAVED:
			MSG_FATAL("DLGRET_SALES_SUCESS_Handler DLGRET_WMS_DRAFT_SAVED",0,0,0);
            CoreApp_ShowMsgDialog(pMe, IDS_WMS_DRAFT_SAVED);
            return NFSMACTION_WAIT;           	
#endif


        case DLGRET_BATT_INFO:
            MSG_FATAL("COREST_STANDBY_Handler DLGRET_BATT_INFO",0,0,0);
            CoreApp_ShowDialog(pMe,IDD_MSGBOX);
            return NFSMACTION_WAIT;
#ifdef FEATURE_KEYGUARD
        case DLGRET_EMGCALL:
            MSG_FATAL("COREST_STANDBY_Handler DLGRET_EMGCALL",0,0,0);
            MOVE_TO_STATE(COREST_EMERGENCYCALL)
            return NFSMACTION_CONTINUE;
#endif
        case DLGRET_GS_SHAKE:
            MSG_FATAL("COREST_STANDBY_Handler DLGRET_GS_SHAKE",0,0,0);
            MOVE_TO_STATE(COREST_STANDBY)
            return NFSMACTION_CONTINUE; 
        default:
            break;
    }
    MSG_FATAL("COREST_STANDBY_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_STANDBY_Handler
#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_SALESTRACKER) || defined(FEATURE_VERSION_IC241A_MMX)
/*==============================================================================
����:
    COREST_SALES_TRAKER_Handler
       
˵��:
    COREST_SALES_TRAKER ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/

static NextFSMAction COREST_SALES_TRAKER_Handler(CCoreApp *pMe)
{
	if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_SALES_TRAKER_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            CoreApp_ShowDialog(pMe, IDD_SALESTRACKER);
            return NFSMACTION_WAIT;
		case DLGRET_MSGOK:
			MOVE_TO_STATE(COREST_STANDBY)
            return NFSMACTION_CONTINUE;
        default:
            break;
    }
	MSG_FATAL("COREST_SALES_TRAKER_Handler End",0,0,0);
    return NFSMACTION_WAIT;
}
//#if defined(FEATURE_VERSION_W317A)

/*==============================================================================
����:
    COREST_SALES_SUCCESS_Handler
       
˵��:
    COREST_SALES_SUCCESS ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_SALES_SUCCESS_Handler(CCoreApp *pMe)
{
	if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_SALES_SUCCESS_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            CoreApp_ShowDialog(pMe, IDD_SALESSUCCESS);
            return NFSMACTION_WAIT;
		case DLGRET_MSGOK:
			MOVE_TO_STATE(COREST_STANDBY)
            return NFSMACTION_CONTINUE;
        default:
            break;
    }
	MSG_FATAL("COREST_SALES_SUCCESS_Handler End",0,0,0);
    return NFSMACTION_WAIT;
}
#endif
//#endif
#if defined(FEATURE_WMS_APP)
/*==============================================================================
����:
    COREST_SMSTIP_Handler
       
˵��:
    COREST_SMSTIP ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_SMSTIP_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_SMSTIP_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            CoreApp_ShowDialog(pMe, IDD_WMSTIPS);
            return NFSMACTION_WAIT;

        case DLGRET_VIEWSMS:
            MOVE_TO_STATE(COREST_STANDBY)
            {// ���ö��Žӿڽ��в鿴����
                IWmsApp *pWmsApp=NULL;
                
                if (ISHELL_CreateInstance(pMe->a.m_pIShell, AEECLSID_WMSAPP,
                        (void**)&pWmsApp) == SUCCESS)
                {
                    MSG_FATAL("COREST_SMSTIP_Handler %d %d %d",gbWmsVMailNtf,gbWmsSMSNtf,gbWmsLastNtfIsSMS);
                    if (gbWmsVMailNtf && !(gbWmsLastNtfIsSMS && gbWmsSMSNtf))
                    {
                        IWmsApp_ViewVMail(pWmsApp);
                        gbWmsVMailNtf = FALSE;
                    }
                    else
                    {
                        IWmsApp_ShowMessageList(pWmsApp);
                        gbWmsSMSNtf = FALSE;
                    }
                    
                    IWmsApp_Release(pWmsApp);
                    return NFSMACTION_WAIT;
                }
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_SMSTIPS_OK:
            MSG_FATAL("COREST_SMSTIP_Handler %d %d %d",gbWmsVMailNtf,gbWmsSMSNtf,gbWmsLastNtfIsSMS);
            if (gbWmsVMailNtf && !(gbWmsLastNtfIsSMS && gbWmsSMSNtf))
            {
                gbWmsVMailNtf = FALSE;
            }
            else
            {
                gbWmsSMSNtf = FALSE;
            }
            MOVE_TO_STATE(COREST_STANDBY)
            return NFSMACTION_CONTINUE;
        
        default:
            break;
    }
    MSG_FATAL("COREST_SMSTIP_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_SMSTIP_Handler
#endif

/*==============================================================================
����:
    COREST_NOTICE_Handler
       
˵��:
    COREST_NOTICE ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_NOTICE_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet){
    case DLGRET_CREATE:
        CoreApp_ShowDialog(pMe, IDD_MSGBOX);
        return NFSMACTION_WAIT;
        
    case DLGRET_MSGOK:
        MOVE_TO_STATE(COREST_STANDBY)
        return NFSMACTION_CONTINUE;
    }
    return NFSMACTION_WAIT;
} // COREST_NOTICE_Handler

/*==============================================================================
����:
    COREST_DEEPSLEEP_Handler
       
˵��:
    COREST_DEEPSLEEP ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_DEEPSLEEP_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    return NFSMACTION_WAIT;
} // COREST_DEEPSLEEP_Handler

/*==============================================================================
����:
    COREST_POWEROFF_Handler
       
˵��:
    COREST_POWEROFF ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_POWEROFF_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_POWEROFF_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
#ifdef FEATRUE_AUTO_POWER
        case DLGRET_MSGOK:
#endif
            CoreApp_ShowDialog(pMe, IDD_POWERDOWN);
            return NFSMACTION_WAIT;
            
        default:
            break;
    }	
    MSG_FATAL("COREST_POWEROFF_Handler End",0,0,0);
    return NFSMACTION_WAIT;
} // COREST_POWEROFF_Handler
static boolean CoreApp_Start_Alarm(CCoreApp *pMe)
{
#if 1
#if defined( WIN32)
    return TRUE;
#else
    db_items_value_type  need_capture;
    boolean flag = FALSE;
    db_get(DB_POWERUPTYPE, &need_capture);
    ICONFIG_GetItem(pMe->m_pConfig, CFGI_ALARM_FLAG,(void*)&flag, sizeof(flag));
    MSG_FATAL("db_poweruptype= %d: flag= %d",need_capture.db_poweruptype,flag,0);
    if(need_capture.db_poweruptype == DB_POWERUP_BYRTCTIMER && flag == TRUE)
    {
        flag = FALSE;
        ICONFIG_SetItem(pMe->m_pConfig, CFGI_ALARM_FLAG,(void*)&flag, sizeof(flag));
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
#else
    uint32 dwNow;
    uint32 testtime;
    uint32 dwAlarmTime;
    uint32 dwTepTime;
    JulianType  jDate;

    ICONFIG_GetItem(pMe->m_pConfig, CFGI_POWERDOWN_ALARM,
                    (void*)&pMe->m_PowerDownAlarmCfg, sizeof(PowerDown_Alarm_Cfg));

    dwNow = GETTIMESECONDS();
    //CORE_ERR("dwNow%d:",dwNow);
    GETJULIANDATE(dwNow, &jDate);

    dwAlarmTime = pMe->m_PowerDownAlarmCfg.dwWATime;
    CORE_ERR("dwNow%d: dwAlarmTime%d:dwWATime[eAlarmType]: %d",dwNow,dwAlarmTime,pMe->m_PowerDownAlarmCfg.dwWATime);
    //CORE_ERR("pMe->m_ClockCfg.dwWATime[eAlarmType]: %d",pMe->m_PowerDownAlarmCfg.dwWATime);

    dwAlarmTime = dwAlarmTime / 1000;
    CORE_ERR("dwAlarmTime%d:",dwAlarmTime);

    jDate.wHour = 0;
    jDate.wMinute = 0;
    jDate.wSecond = 0;

    testtime = JULIANTOSECONDS(&jDate);

    dwTepTime = dwAlarmTime + JULIANTOSECONDS(&jDate);

    CORE_ERR("dwAlarm%d dwTep%d dwNow%d",dwAlarmTime,dwTepTime,dwNow);
    //CORE_ERR("dwAlarm%d ",dwAlarmTime);
    //CORE_ERR("dwTep%d ",dwTepTime);
    //CORE_ERR("dwNow%d ",dwNow);
    ERR("testtime %d  pMe->m_PowerDownAlarmCfg.bStateOn = %d",testtime,pMe->m_PowerDownAlarmCfg.bStateOn,0);

    if(((dwTepTime+20) >= dwNow)
        &&(dwTepTime < (dwNow + 20))
        &&(pMe->m_PowerDownAlarmCfg.bStateOn == TRUE))
    {
        //CORE_ERR("TRUE %d ",0);
        return TRUE;

    }
    else
    {
        //CORE_ERR("FALSE %d ",0);
        return FALSE;
    }
#endif
}

#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)||defined(FEATURE_SALESTRACKER) || defined(FEATURE_VERSION_IC241A_MMX)
/*==============================================================================
������
    COREST_SALES_EDIT_Handler
˵����
    COREST_SALES_EDIT ״̬������
       
������
    pMe [in]��ָ��Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
    NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction COREST_SALES_EDIT_Handler(CCoreApp *pMe)
{
	if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_SALES_EDIT_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            CoreApp_ShowDialog(pMe, IDD_SALES_EDIT);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGOK:
        default:
            MOVE_TO_STATE(COREST_STANDBY)
    }
    MSG_FATAL("COREST_SALES_EDIT_Handler End",0,0,0);
    return NFSMACTION_CONTINUE;
}


#endif

//#if defined (FEATURE_IC19_ESN_TRACKER)
static NextFSMAction COREST_ESN_EDIT_Handler(CCoreApp *pMe)
{
	if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_ESN_EDIT_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            CoreApp_ShowDialog(pMe, IDD_ESN_TRACKER_EDIT);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGOK:
        default:
            MOVE_TO_STATE(COREST_STANDBY)
    }
    MSG_FATAL("COREST_ESN_EDIT_Handler End",0,0,0);
    return NFSMACTION_CONTINUE;
}
//#endif

#ifdef FEATURE_UTK2
/*==============================================================================
������
    COREST_UTKREFRESH_Handler
˵����
    COREST_UTKREFRESH ״̬������
       
������
    pMe [in]��ָ��Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
    NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction COREST_UTKREFRESH_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_UTKREFRESH_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if (pMe->wRefreshMask & REFRESHMASK_UIMRESET)
            {
                pMe->m_ePowerDownType = POWERDOWN_RUIM_DOOR_RESET;
                pMe->m_wStartupAniTime = 0;
                MOVE_TO_STATE(COREST_POWEROFF)
                return NFSMACTION_CONTINUE;
            }
            
            pMe->m_nMsgID = IDS_UTKREFRESH;
                
          
            CoreApp_ShowDialog(pMe, IDD_UTKREFRESH);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGOK:
        default:
            MOVE_TO_STATE(COREST_STANDBY)
    }
    MSG_FATAL("COREST_UTKREFRESH_Handler End",0,0,0);
    return NFSMACTION_CONTINUE;
} // IDLEST_UTKREFRESH_Handler
#endif

#ifdef FEATURE_SHOW_RSSI_INFO
static NextFSMAction COREST_RSSI_INFO_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("COREST_SALES_EDIT_Handler Start",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            CoreApp_ShowDialog(pMe, IDD_RSSI_INFO);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGOK:
        default:
            MOVE_TO_STATE(COREST_STANDBY)
    }
    MSG_FATAL("COREST_SALES_EDIT_Handler End",0,0,0);
    return NFSMACTION_CONTINUE;
}
#endif

#if defined( FEATURE_IDLE_LOCK_RUIM)&&defined(FEATURE_UIM)
//The function is for locking the ruim according to the mobilization request
void static isAllowIMSI(CCoreApp *pMe,boolean *lockFlg)
{
    typedef struct _SProvider
    {
        char mcc[4];
        char mnc[3];
    }SProvider;
    AEEMobileInfo mi;
    //Build the list of service provider
    static const SProvider List_SP[] = 
    {
        {"000","00"},//fault value
#ifdef FEATURE_CARRIER_THAILAND_HUTCH
        {"520","00"}, //hutch 
#endif //FEATURE_CARRIER_THAILAND_HUTCH
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
        {"634","07"}, //sudan 
#endif // FEATURE_CARRIER_SUDAN_SUDATEL
    };   
    int i,size = sizeof(List_SP)/sizeof(SProvider); 
    char szMCC[4] = {'\0'};
    char szMNC[3] = {'\0'};

    if(pMe->bunlockuim)
    {
         *lockFlg = FALSE; 
           return;
    }
	
    //OEM_GetConfig(CFGI_MOBILEINFO, &mi,	sizeof(AEEMobileInfo));
    GetMobileInfo(&mi);
    STRNCPY(szMCC,mi.szMobileID,3);
    STRNCPY(szMNC,&mi.szMobileID[3],2);
    for(i=0; i<size; i++)
    {
        if ( !STRCMP(szMCC,List_SP[i].mcc) && !STRCMP(szMNC,List_SP[i].mnc))
        {
            *lockFlg = FALSE; 
            break; 
        }
        else
        {
            *lockFlg = TRUE; 
        }
    }
    return;
}
#endif //defined( FEATURE_IDLE_LOCK_RUIM)&&defined(FEATURE_UIM)


/*===========================================================================
FUNCTION PINVerify_cb

DESCRIPTION

  PINVerify_cb is called in response to ICARD_VerifyPin completing, and
  will check the status of pin status(either success or fail).

  Fail:    post EVT_PIN_VERIFY_FAIL event to coreapp
  Success: post EVT_PIN_VERIFY_SUCCESS event to coreapp

  return TRUE;


DEPENDENCIES
  None.


RETURN VALUE
  TRUE/FALSE

SIDE EFFECTS
  None.

===========================================================================*/
static void  PINVerify_cb(void *pNotifyData)
{  
  CCoreApp * pMe = (CCoreApp *) pNotifyData;
  AEECardPinStatus sPinStatus;
  
  if (pMe == NULL) {
    CORE_ERR ("Null pointer", 0,0,0);
    return;
  }
  ICARD_GetPinStatus(pMe->m_pICard, pMe->m_sPinActionStatus.nPinID, &sPinStatus);
  
  ISHELL_PostEventEx(pMe->a.m_pIShell, EVTFLG_ASYNC, AEECLSID_CORE_APP, EVT_CARD_STATUS, sPinStatus.lock_state, sPinStatus.tries_remaining);
  pMe->m_bVerifying = FALSE;
}

/*===========================================================================
FUNCTION CoreSecurity_VerifyPIN

DESCRIPTION

  This function sets the callback function pointer for verify pin and
  call the corresponding ICARD_VerifyPin function

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/EFAILED/EUNSUPPORT

SIDE EFFECTS
  None.
===========================================================================*/
static int CoreSecurity_VerifyPIN(CCoreApp * pMe, uint8 byPinID)
{
  int nReturnStatus = SUCCESS;
  
  /* Populate the m_sCallback structure */
  pMe->m_sCallback.pfnCancel = NULL;
  pMe->m_sCallback.pfnNotify = PINVerify_cb;
  pMe->m_sCallback.pNotifyData = pMe;

  if(pMe->m_sPIN.code_len>0)
  {
    AECHAR ch = pMe->m_sPIN.code[pMe->m_sPIN.code_len-1];
    if(ch<'0' || ch>'9')
    {
        pMe->m_sPIN.code[pMe->m_sPIN.code_len-1] = '\0';
    }
  }
  
  nReturnStatus = ICARD_VerifyPin(pMe->m_pICard, byPinID, &pMe->m_sPIN,
                                  &pMe->m_sPinActionStatus, &pMe->m_sCallback);
  return nReturnStatus;
}


/*===========================================================================
FUNCTION CoreSecurity_VerifyPUK

DESCRIPTION

  This function sets the callback function pointer for verify puk and
  call the corresponding ICARD_UnblockPin function

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/EFAILED/EUNSUPPORT

SIDE EFFECTS
  None.
===========================================================================*/
static int CoreSecurity_VerifyPUK(CCoreApp * pMe, uint8 byPinID)
{
  int nReturnStatus = SUCCESS;

  /* Populate the m_sCallback structure */
  pMe->m_sCallback.pfnCancel = NULL;
  pMe->m_sCallback.pfnNotify = PINVerify_cb;
  pMe->m_sCallback.pNotifyData = pMe;

  if(pMe->m_sPIN.code_len>0)
  {
    AECHAR ch = pMe->m_sPIN.code[pMe->m_sPIN.code_len-1];
    if(ch<'0' || ch>'9')
    {
        pMe->m_sPIN.code[pMe->m_sPIN.code_len-1] = '\0';
    }
  }
  
  if(pMe->m_sPUK.code_len>0)
  {
    AECHAR ch = pMe->m_sPUK.code[pMe->m_sPUK.code_len-1];
    if(ch<'0' || ch>'9')
    {
        pMe->m_sPUK.code[pMe->m_sPUK.code_len-1] = '\0';
    }
  }
  
  nReturnStatus = ICARD_UnblockPin(pMe->m_pICard, byPinID, &pMe->m_sPUK, &pMe->m_sPIN,
                &pMe->m_sPinActionStatus, &pMe->m_sCallback);

  return nReturnStatus;
}

/*===========================================================================
FUNCTION CoreSecurity_ChangePIN

DESCRIPTION

  This function sets the callback function pointer for verify pin and
  call the corresponding ICARD_ChangePin function

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/EFAILED/EUNSUPPORT

SIDE EFFECTS
  None.
===========================================================================*/
int CCoreApp_ChangePIN(CCoreApp *pMe, uint8 byPinID, AECHAR *pOldPIN, AECHAR *pNewPIN)
{
    int nReturnStatus = SUCCESS;
  
    if(!pNewPIN || !pOldPIN)
    {
        return EBADPARM;
    }

    //     DBGPRINTF("CoreApp_ChangePIN %S %S",pOldPIN,pNewPIN);
  
    WSTRCPY(pMe->m_wPIN,pNewPIN);
    pMe->m_sPIN.code = pMe->m_wPIN;
    pMe->m_sPIN.code_len = WSTRLEN(pMe->m_wPIN);
  
    WSTRCPY(pMe->m_wPIN2,pOldPIN);
    pMe->m_sPIN2.code = pMe->m_wPIN2;
    pMe->m_sPIN2.code_len = WSTRLEN(pMe->m_wPIN2);
  
    /* Populate the m_sCallback structure */
    pMe->m_sCallback.pfnCancel = NULL;
    pMe->m_sCallback.pfnNotify = PINVerify_cb;
    pMe->m_sCallback.pNotifyData = pMe;
    
    if(pMe->m_sPIN.code_len>0)
    {
        AECHAR ch = pMe->m_sPIN.code[pMe->m_sPIN.code_len-1];
        if(ch<'0' || ch>'9')
        {
            pMe->m_sPIN.code[pMe->m_sPIN.code_len-1] = '\0';
        }
    }

    if(pMe->m_sPIN2.code_len>0)
    {
        AECHAR ch = pMe->m_sPIN2.code[pMe->m_sPIN2.code_len-1];
        if(ch<'0' || ch>'9')
        {
            pMe->m_sPIN2.code[pMe->m_sPIN2.code_len-1] = '\0';
        }
    }
  
    nReturnStatus = ICARD_ChangePin(pMe->m_pICard, byPinID, &pMe->m_sPIN2, &pMe->m_sPIN,
                                  &pMe->m_sPinActionStatus, &pMe->m_sCallback);
    if(SUCCESS != nReturnStatus)
    {
        // �������,��֤ʧ��
        pMe->m_nMsgID = IDS_INVALIDPIN;
        
        // �ȸı䵱ǰ״̬
        MOVE_TO_STATE(COREST_NOTICE)
        
        // ���� DLGRET_CREATE �رյ�ǰ�Ի���
        CLOSE_DIALOG(DLGRET_CREATE) 
    }
    else
    {
        pMe->m_bVerifying = TRUE;
        pMe->m_nMsgID = IDS_WAITING;
        
        // �ȸı䵱ǰ״̬
        MOVE_TO_STATE(COREST_NOTICE)
        
        // ���� DLGRET_CREATE �رյ�ǰ�Ի���
        CLOSE_DIALOG(DLGRET_CREATE) 
    }
    return nReturnStatus;
}

/*===========================================================================
FUNCTION CoreSecurity_UnblockPIN

DESCRIPTION

  This function sets the callback function pointer for verify puk and
  call the corresponding ICARD_UnblockPin function

DEPENDENCIES
  None.

RETURN VALUE
  SUCCESS/EFAILED/EUNSUPPORT

SIDE EFFECTS
  None.
===========================================================================*/
int CCoreApp_UnblockPIN(CCoreApp *pMe, uint8 byPinID, AECHAR *pPUK, AECHAR *pPIN)
{
    int nReturnStatus = SUCCESS;
    if(!pPUK || !pPIN)
    {
        return EBADPARM;
    }
  
    //     DBGPRINTF("CoreApp_UnblockPIN %S %S",pPUK,pPIN);
  
    WSTRCPY(pMe->m_wPIN,pPIN);
    pMe->m_sPIN.code = pMe->m_wPIN;
    pMe->m_sPIN.code_len = WSTRLEN(pMe->m_wPIN);
    WSTRCPY(pMe->m_wPUK,pPUK);
    pMe->m_sPUK.code = pMe->m_wPUK;
    pMe->m_sPUK.code_len = WSTRLEN(pMe->m_wPUK);
  
    nReturnStatus = CoreSecurity_VerifyPUK(pMe, byPinID);
    if(SUCCESS != nReturnStatus)
    {
        // �������,��֤ʧ��
        pMe->m_nMsgID = IDS_INVALIDPUK;
        
        // �ȸı䵱ǰ״̬
        MOVE_TO_STATE(COREST_NOTICE)
        
        // ���� DLGRET_CREATE �رյ�ǰ�Ի���
        CLOSE_DIALOG(DLGRET_CREATE) 
    }
    else
    {
        pMe->m_bVerifying = TRUE;
        pMe->m_nMsgID = IDS_WAITING;
        
        // �ȸı䵱ǰ״̬
        MOVE_TO_STATE(COREST_NOTICE)
        
        // ���� DLGRET_CREATE �رյ�ǰ�Ի���
        CLOSE_DIALOG(DLGRET_CREATE) 
    }
    return nReturnStatus;
}

extern int OEMRUIMAddr_ReadADNByID(uint16 wRecID, AECHAR **ppName, AECHAR **ppNumber);
int CCoreApp_DisplayADN(CCoreApp *pMe, uint16 wRecID)
{
    int nReturnStatus = OEMRUIMAddr_ReadADNByID(wRecID, &pMe->m_pADNName, &pMe->m_pADNNumber);
    if(nReturnStatus == SUCCESS)
    {
        ISHELL_CloseApplet(pMe->a.m_pIShell, TRUE);
        
        // �������,��֤ʧ��
        pMe->m_nMsgID = IDS_CORE_DISPADN;
        
        // �ȸı䵱ǰ״̬
        MOVE_TO_STATE(COREST_NOTICE)
        
        // ���� DLGRET_CREATE �رյ�ǰ�Ի���
        CLOSE_DIALOG(DLGRET_CREATE) 
    }
    return nReturnStatus;
}

