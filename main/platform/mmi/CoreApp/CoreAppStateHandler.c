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

#include "OEMRTC.h"
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
//״̬COREST_IMSIMCCPWD������
#ifdef FATRUE_LOCK_IMSI_MCCMNC
static NextFSMAction COREST_IMSIMCCPWD_Handler(CCoreApp *pMe);
#endif
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

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case COREST_NONE:
            retVal = COREST_NONE_Handler(pMe);
            break;
            
        case COREST_INIT:
            retVal = COREST_INIT_Handler(pMe);
            break;
        
        case COREST_ALARM:
            retVal = COREST_ALARM_Handler(pMe);
            break;    
                    
        case COREST_LPM:
            retVal = COREST_LPM_Handler(pMe);
            break;
        //case COREST_SERVICEREQ:
        //    retVal = COREST_SERVICEREQ_Handler(pMe);
        //    break;
        case COREST_VERIFYPHONEPWD:
            retVal = COREST_VERIFYPHONEPWD_Handler(pMe);
            break;
            
        case COREST_VERIFYUIM:
            retVal = COREST_VERIFYUIM_Handler(pMe);
            break;
            
        case COREST_UIMERR:
            retVal = COREST_UIMERR_Handler(pMe);
            break;

        case COREST_EMERGENCYCALL:
            retVal = COREST_EMERGENCYCALL_Handler(pMe);
            break;
            
        case COREST_POWERONSYSINIT:
            retVal = COREST_POWERONSYSINIT_Handler(pMe);
            break;
            
        case COREST_STARTUPANI:
            retVal = COREST_STARTUPANI_Handler(pMe);
            break;
            
        case COREST_POWERONAPPSDATAINIT:
            retVal = COREST_POWERONAPPSDATAINIT_Handler(pMe);
            break;
            
        case COREST_STANDBY:
            retVal = COREST_STANDBY_Handler(pMe);
            break;
            
        case COREST_NOTICE:
            retVal = COREST_NOTICE_Handler(pMe);
            break;
            
#if defined(FEATURE_WMS_APP)
        case COREST_SMSTIP:
            retVal = COREST_SMSTIP_Handler(pMe);
            break;
#endif            
        
        case COREST_DEEPSLEEP:
            retVal = COREST_DEEPSLEEP_Handler(pMe);
            break;
            
        case COREST_POWEROFF:
            retVal = COREST_POWEROFF_Handler(pMe);
            break;
            
#ifdef FEATURE_UTK2
        case COREST_UTKREFRESH:
            retVal = COREST_UTKREFRESH_Handler(pMe);
            break;            
#endif //FEATURE_UTK2
#ifdef FATRUE_LOCK_IMSI_MCCMNC
	    case COREST_IMSIMCCPWD:
            retVal = COREST_IMSIMCCPWD_Handler(pMe);
	        break;
#endif
        default:
            break;
    }
    CORE_ERR("curstate %d prestate %d dlgret %d CoreApp_ProcessState",pMe->m_eCurState,pMe->m_ePreState,pMe->m_eDlgRet);
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
    
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if (!IsRunAsUIMVersion())
            {// �������Ϊ�޿��汾,�����ز����Ƿ������׵�
              /*  // TODO
                if ()
                {
                    // ������� COREST_SERVICEREQ, �����޿��汾,�����ز���
                    // ���ò������������ò���������
                    
                    tepState = COREST_SERVICEREQ;
                }*/
                
            }
            MOVE_TO_STATE(tepState)
            
            return NFSMACTION_CONTINUE;
            
        default:
            break;            
        
    }
    
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
    uint32 esn=0;
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //����У׼�۲��������ܽ���������	
#ifndef FEATURE_CDG2_TEST
    ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_ESN,
                          &esn,
                          sizeof(esn));
#endif
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            if(esn)
            {
                tepState = COREST_NONE;
                if(CoreApp_Start_Alarm(pMe))
                {
                    tepState = COREST_ALARM;
                }
                else if (IBATT_GetExternalPower(pMe->m_pBatt))
                {
                    static boolean lpm   = 0;
                    // ���������ⲿ��Դ������ػ����״̬
                    CORE_ERR("LPM test: have extern power!",0);

                    //DBGPRINTF( ";-----------------------------------------");
                    //DBGPRINTF( ";now is charging");

                    if( !lpm)
                    {
                        lpm = 1;
                    }
                    else
                    {
                        OEMRTC_Process_Auto_Power_On();

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
#ifdef FATRUE_LOCK_IMSI_MCCMNC
                    tepState = COREST_IMSIMCCPWD;
#else 
                    tepState = COREST_VERIFYPHONEPWD;
#endif
                }
                CORE_ERR("LPM test:new state is %d",tepState);
                if (tepState != COREST_NONE)
                {
                    MOVE_TO_STATE(tepState)
                    return NFSMACTION_CONTINUE;
                }
                else
                {
                    CoreApp_ShowDialog(pMe, IDD_LPM);
                    return NFSMACTION_WAIT;
                }
            }
            else /*esn ==0*/
            {
#ifdef FATRUE_LOCK_IMSI_MCCMNC
                tepState = COREST_IMSIMCCPWD;
#else 
                tepState = COREST_VERIFYPHONEPWD;
#endif
                MOVE_TO_STATE(tepState)
                return NFSMACTION_CONTINUE;            
            }
            //����У׼�۲��������ܽ���������          
            
        case DLGRET_OK:
#ifdef FATRUE_LOCK_IMSI_MCCMNC
            MOVE_TO_STATE(COREST_IMSIMCCPWD)
#else
            MOVE_TO_STATE(COREST_VERIFYPHONEPWD)
#endif
            return NFSMACTION_CONTINUE;
        case DLGRET_RTC:
        {

            DBGPRINTF( ";----------------------------------------");
            DBGPRINTF( ";response to EVT_RTC");

#if defined( FEATURE_POWERDOWN_ALARM)
            if( pMe->powerupByAlarm)
            {
                DBGPRINTF( ";start by alarm, to show alarm");
                tepState = COREST_ALARM;
            }
            else
#endif				
            {
                DBGPRINTF( ";start by auto poweron, to verify password");
                tepState = COREST_VERIFYPHONEPWD;
            }
            MOVE_TO_STATE( tepState)
            return NFSMACTION_CONTINUE;
        }

        default:
            break;            
    }
    
    return NFSMACTION_WAIT;
} // COREST_LPM_Handler
#if 0 
/*==============================================================================
����:
    COREST_SERVICEREQ_Handler
       
˵��:
    COREST_SERVICEREQ ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_SERVICEREQ_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK: // ����Ϣ�Ի��򷵻�
            CoreApp_ShowMsgDialog(pMe, IDS_SERVICEREQ);
            return NFSMACTION_WAIT;
            
        default:
            break;
    }
    
    return NFSMACTION_WAIT;
} // COREST_SERVICEREQ_Handler
#endif
 #ifdef FATRUE_LOCK_IMSI_MCCMNC
/*==============================================================================
����:
    COREST_IMSIMCCPWD_Handler
       
˵��:
    COREST_IMSIMCCPWD_Handler ״̬������
       
����:
    pMe [in]:ָ�� Core Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    NFSMACTION_CONTINUE:ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT:ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע:
       
==============================================================================*/
static NextFSMAction COREST_IMSIMCCPWD_Handler(CCoreApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK: // ����Ϣ�Ի��򷵻�
        {
            uint16 MccValue;
            uint16   MncValue;	
            uint16 mcc;
            uint16 mnc;	

            ServiceProvider List_SP[OEMNV_MAX_SERVICEPROVIDER_NUMBER]={0};
            SetImsi List[OEMNV_MAX_SERVICEPROVIDER_NUMBER] = {0};
            // ����Ƿ�Ϊ�п�״̬
            if (IsRunAsUIMVersion())
            {
                //ȡ����MCC��MNC
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                CFGI_IMSI_MCC,
                                &MccValue,
                                sizeof(MccValue));

                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                CFGI_IMSI_11_12,
                                &MncValue,
                                sizeof(MncValue));;
                //ȡ������MCC��MNC������ֵ
                (void) ICONFIG_GetItem(pMe->m_pConfig, 
                                CFGI_IMSI_SETMCC,
                                (void*)List,
                                sizeof(SetImsi) * OEMNV_MAX_SERVICEPROVIDER_NUMBER);	
                //ȡMCC��MNC��INI�ļ�
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                CFGI_SERVICE_PROVIDER,
                                (void*) List_SP,
                                sizeof(ServiceProvider) * OEMNV_MAX_SERVICEPROVIDER_NUMBER);

                mcc = (uint16)ATOI(List_SP[0].mcc);
                mnc = (uint16)ATOI(List_SP[0].mnc);

                //MSG_ERROR("99999999999999999%d,%d,%d", List[0].mcc, List[0].bsetimsi,List[0].mnc);
                //MSG_ERROR("2222222222222222%d,%d", MccValue,MncValue,0);	 
                if(List[0].bsetimsi)
                {
                    if (( MccValue !=  List[0].mcc)||(MncValue !=  List[0].mnc))
                    {
                        CoreApp_ShowDialog(pMe, IDD_PWDIMSIMCC);
                        return NFSMACTION_WAIT;                      
                    }
                    else
                    {
                        MOVE_TO_STATE(COREST_VERIFYPHONEPWD);
                    }
                }
                else
                {
                MOVE_TO_STATE(COREST_VERIFYPHONEPWD);
                }
            }
            else
            {
                MOVE_TO_STATE(COREST_VERIFYPHONEPWD);
            }
                return NFSMACTION_CONTINUE;
        }
        case DLGRET_ENTEROK:
        {
            uint16 wPWD=0;

            (void) ICONFIG_GetItem(pMe->m_pConfig, 
                            CFGI_PHONE_PASSWORD,
                            &wPWD,
                            sizeof(uint16));

            if (wPWD == EncodePWDToUint16(pMe->m_strPhonePWD))
            {// �������
                MOVE_TO_STATE(COREST_VERIFYPHONEPWD);
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
            MOVE_TO_STATE(COREST_EMERGENCYCALL)
            return NFSMACTION_CONTINUE;

        // add for Process batty notify,
        case DLGRET_BATT_INFO:
            CoreApp_ShowDialog(pMe,IDD_MSGBOX);
            return NFSMACTION_WAIT;
            // add for Process batty notify

        default:
            break;
    }

    return NFSMACTION_CONTINUE;
} // COREST_VERIFYPHONEPWD_Handler
#endif
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
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK: // ����Ϣ�Ի��򷵻�
            {
                boolean bValue = FALSE;
                
                // ����Ƿ������ֻ��������⹦��
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                            CFGI_PHONE_PASSWORD_CHECK,
                            &bValue,
                            sizeof(bValue));
                if (bValue)
                {
                    CoreApp_ShowDialog(pMe, IDD_PWDINPUT);
                    return NFSMACTION_WAIT;
                }
                else if (IsRunAsUIMVersion())
                {// ��������Ϊ�п��汾, ���п�����ؼ��
                    MOVE_TO_STATE(COREST_VERIFYUIM)
                }
                else
                {
                    //MOVE_TO_STATE(COREST_POWERONSYSINIT)
                    MOVE_TO_STATE(COREST_STARTUPANI);
                }
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_ENTEROK:
            {
                uint16 wPWD=0;
                
                (void) ICONFIG_GetItem(pMe->m_pConfig, 
                                       CFGI_PHONE_PASSWORD,
                                       &wPWD,
                                       sizeof(uint16));
                
                if (wPWD == EncodePWDToUint16(pMe->m_strPhonePWD))
                {// �������
                    if (IsRunAsUIMVersion())
                    {// ��������Ϊ�п��汾, ���п�����ؼ��
                        MOVE_TO_STATE(COREST_VERIFYUIM)
                    }
                    else
                    {
                        //MOVE_TO_STATE(COREST_POWERONSYSINIT)
                        MOVE_TO_STATE(COREST_STARTUPANI);
                    }
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
            MOVE_TO_STATE(COREST_EMERGENCYCALL)
            return NFSMACTION_CONTINUE;

        case DLGRET_BATT_INFO:
            CoreApp_ShowDialog(pMe,IDD_MSGBOX);
            return NFSMACTION_WAIT;
            
        default:
            break;
    }

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
    
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK:
            // ��鿨�Ƿ����
            if (IRUIM_IsCardConnected(pMe->m_pIRUIM)) 
            {// �����˿�
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
                // ����Ӫ�̶Կ�����Ҫ������������ü�麯��: TODO
                /*
                else if ()
                {
                    // ����������Ӫ��Ҫ�󣬿���Ч
                    pMe->m_eUIMErrCode = UIMERR_LOCKED;
                    MOVE_TO_STATE(COREST_UIMERR)
                }
                */
                else
                {
                    // UIM OK
                    //MOVE_TO_STATE(COREST_POWERONSYSINIT)
                    MOVE_TO_STATE(COREST_STARTUPANI);

                }
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
                    MOVE_TO_STATE(COREST_UIMERR)
                }
            }
            
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
                    if (IRUIM_PINCheck(pMe->m_pIRUIM, IRUIM_CHV1, pMe->m_strPIN1))
                    {
                        // ͨ����֤
                        //MOVE_TO_STATE(COREST_POWERONSYSINIT)
                        MOVE_TO_STATE(COREST_STARTUPANI);
                    }
                    else
                    {
                        AEECHVStatus  chvst;
                        
                        (void)IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
                        if (chvst.chv1CountRemain > 0)
                        {
                            // �������,��֤ʧ��
                            CoreApp_ShowMsgDialog(pMe, IDS_INVALIDPIN);
                        }
                        else
                        {
                            // �������,������
                            CoreApp_ShowMsgDialog(pMe, IDS_UIMBLOCKED);
                        }
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
                    if (IRUIM_UnblockCHV(pMe->m_pIRUIM,
                                        IRUIM_CHV1,
                                        pMe->m_strPUK,
                                        pMe->m_strPIN1) == AEE_SUCCESS)
                    {
                        // �ɹ�����
                        //MOVE_TO_STATE(COREST_POWERONSYSINIT)
                        MOVE_TO_STATE(COREST_STARTUPANI);
                    }
                    else
                    {
                        // �������,��֤ʧ��
                        CoreApp_ShowMsgDialog(pMe, IDS_INVALIDPUK);
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
                pMe->bunlockuim = TRUE;
                MOVE_TO_STATE( COREST_STARTUPANI)
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
    if(pMe->m_b_PH_INFO_AVAIL == TRUE)
    {
        pMe->m_b_PH_INFO_AVAIL = FALSE;
        InitProvisioning();
    }
    // Ϊ���н������У���������������״̬
    //CoreApp_SetOperatingModeOnline(pMe);
    pMe->m_b_online_from = ON_LINE_FROM_EMERGENCY;
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK:
            CoreApp_ShowDialog(pMe, IDD_EMERGENCYNUMLIST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(pMe->m_ePreState)
            return NFSMACTION_CONTINUE;

        case DLGRET_BATT_INFO:
            CoreApp_ShowDialog(pMe,IDD_MSGBOX);
            return NFSMACTION_WAIT;
        
        default:
            break;
    }
        
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

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
#ifdef  FEATURE_2008_POWERON_LOGIC
            if (IsRunAsUIMVersion() && (pMe->m_eUIMErrCode == UIMERR_NONE))
            { // ϵͳ�������п��汾�ҿ���ȷ����
#ifdef FEATURE_UTK2
                UTK_SendTerminalProfile();
#endif //FEATURE_UTK2

                // ���� UIM �����ã�ͨ���ı� NAM �� MC ����װ�� ESN
                (void) ICM_SetNAMSel(pMe->m_pCM, AEECM_NAM_1);
            }
#else
#ifdef FEATURE_UTK2
            if (IsRunAsUIMVersion() && (pMe->m_eUIMErrCode == UIMERR_NONE))
            { // ϵͳ�������п��汾�ҿ���ȷ����
                // UTK Profile download
                UTK_SendTerminalProfile();
            }
#endif //FEATURE_UTK2
#endif
#ifdef FEATURE_POWERUP_REGISTER_CHINAUNICOM
            // ���÷���ע����ŵĶ�ʱ������
            (void)ISHELL_SetTimer(pMe->a.m_pIShell, 
                                  SENDREGINFOR_TIME,
                                  CoreApp_SendReginfoTimer, 
                                  pMe);

            CORE_ERR("ISHELL_SetTimer CoreApp_SendReginfoTimer",0,0,0);

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
                cm_ph_cmd_oprt_mode(NULL, NULL, CM_CLIENT_ID_ANONYMOUS, SYS_OPRT_MODE_LPM);
                MOVE_TO_STATE(COREST_STANDBY);
                return NFSMACTION_CONTINUE;

#endif //FEATURE_PLANEMODE

        default:
            break;
    }
    
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

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
 #if defined( FEATURE_IDLE_LOCK_RUIM)&&defined(FEATURE_UIM)
                if (IsRunAsUIMVersion() && IRUIM_IsCardConnected(pMe->m_pIRUIM))
                {   
                     boolean lockFlg = TRUE;     //not find right MCC and MNC value lable should lock current RUIM 
                     isAllowIMSI(pMe,&lockFlg);
                     if (lockFlg)
                     {
                         // ����������Ӫ��Ҫ�󣬿���Ч
                         pMe->m_eUIMErrCode = UIMERR_LOCKED;
                         MOVE_TO_STATE(COREST_UIMERR)
                         return NFSMACTION_CONTINUE;
                     }
                     if(pMe->bunlockuim)                   //�����������������ȷ���˲���Ϊ��                     
                     {                     
                          pMe->m_eUIMErrCode = UIMERR_NONE;                     
                     }                     
                  }
#endif //defined( FEATURE_IDLE_LOCK_RUIM)&&defined(FEATURE_UIM)
#ifdef  FEATURE_2008_POWERON_LOGIC
            if(pMe->m_b_PH_INFO_AVAIL == TRUE)
            {
                pMe->m_b_PH_INFO_AVAIL = FALSE;
                InitProvisioning();
            }
            // ��������������״̬
            //CoreApp_SetOperatingModeOnline(pMe);
#else
            if(ON_LINE_FROM_EMERGENCY == pMe->m_b_online_from)
            {
                pMe->m_b_online_from = ON_LINE_FROM_NORMAL;
            }
            InitProvisioning();
#endif

            CoreApp_ShowDialog(pMe, IDD_STARTUPANI);
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:
            MOVE_TO_STATE(COREST_POWERONAPPSDATAINIT)
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    
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
            
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    
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
    //DBGPRINTF("EVT_AUTO_POWERDOWN %d %d",pMe->m_b_autopower_off,pMe->m_eDlgRet);
    //#endif
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGOK:
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
            MOVE_TO_STATE(COREST_SMSTIP)
            return NFSMACTION_CONTINUE;
#endif            

        case DLGRET_BATT_INFO:
            CoreApp_ShowDialog(pMe,IDD_MSGBOX);
            return NFSMACTION_WAIT;
#ifdef FEATURE_KEYGUARD
        case DLGRET_EMGCALL:
            MOVE_TO_STATE(COREST_EMERGENCYCALL)
            return NFSMACTION_CONTINUE;
#endif
        case DLGRET_GS_SHAKE:
            MOVE_TO_STATE(COREST_STANDBY)
            return NFSMACTION_CONTINUE; 
        default:
            break;
    }
    
    return NFSMACTION_WAIT;
} // COREST_STANDBY_Handler

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
                    uint16  nNewsVmail=0;
                    
                    wms_cacheinfolist_getcounts(WMS_MB_VOICEMAIL, &nNewsVmail, NULL, NULL);
                    if (nNewsVmail)
                    {
                        IWmsApp_ViewVMail(pWmsApp);
                    }
                    else
                    {
                        IWmsApp_ShowMessageList(pWmsApp);
                    }
                    
                    IWmsApp_Release(pWmsApp);
                    return NFSMACTION_WAIT;
                }
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_SMSTIPS_OK:
            MOVE_TO_STATE(COREST_STANDBY)
            return NFSMACTION_CONTINUE;
        
        default:
            break;
    }
    
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

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
#ifdef FEATRUE_AUTO_POWER
        case DLGRET_MSGOK:
#endif
#ifndef FEATURE_USES_LOWMEM
            if(pMe->m_ePowerDownType == POWERDOWN_NORMAL)
            {
                pMe->m_pStartupAniImg = ISHELL_LoadImage( pMe->a.m_pIShell, PWROFF_ANI_FILE);
            }
#endif
            CoreApp_ShowDialog(pMe, IDD_POWERDOWN);
            return NFSMACTION_WAIT;
            
        default:
            break;
    }	
    
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
    CORE_ERR("db_poweruptype= %d: flag= %d",need_capture.db_poweruptype,flag,0);
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
            else
            {
                db_items_value_type  db_value;
                
                db_value.bRefreshing = TRUE;
                db_put(DB_REFRESHING, &db_value);
            }
            pMe->m_nMsgID = IDS_UTKREFRESH;
                
            CoreApp_ShowDialog(pMe, IDD_UTKREFRESH);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGOK:
        default:
            {
                db_items_value_type  db_value;
                
                db_value.bRefreshing = FALSE;
                db_put(DB_REFRESHING, &db_value);
            }
            MOVE_TO_STATE(COREST_STANDBY)
    }
    
    return NFSMACTION_CONTINUE;
} // IDLEST_UTKREFRESH_Handler
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
    static SProvider List_SP[] = 
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
	
    DBGPRINTF("szMCC %c",szMCC);
    DBGPRINTF("szMNC %c",szMNC);	 
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
#ifndef  FEATURE_2008_POWERON_LOGIC 
void CoreApp_load_uim_esn(CCoreApp *pMe)
{
    CORE_ERR("CoreApp_load_uim_esn %x %x %x",IsRunAsUIMVersion(),pMe->m_b_online_from,pMe->m_eUIMErrCode);
    if (IsRunAsUIMVersion() && (pMe->m_eUIMErrCode == UIMERR_NONE) && (ON_LINE_FROM_NORMAL== pMe->m_b_online_from))
    { // ϵͳ�������п��汾�ҿ���ȷ����
        pMe->m_b_online_from = ON_LINE_FROM_NONE;// set to ture!
        // ���� UIM �����ã�ͨ���ı� NAM �� MC ����װ�� ESN
        (void) ICM_SetNAMSel(pMe->m_pCM, AEECM_NAM_1);
        //CORE_ERR("CoreApp_load_uim_esn2 %x %x %x",IsRunAsUIMVersion(),pMe->m_b_online_from,pMe->m_eUIMErrCode);
    }
}
#endif


