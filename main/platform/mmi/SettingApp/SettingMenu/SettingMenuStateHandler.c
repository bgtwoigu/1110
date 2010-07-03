/*==============================================================================
// �ļ���
//        SettingMenuStateHandler.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//
// �ļ�˵����
//
// ���ߣ�Gemsea
// �������ڣ�2007-11-01
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��    ���ⵥ��  �޸����ݡ�λ�ü�ԭ��
-----------      ----------  --------  -----------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "SettingMenu_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ SETTINGMENUST_NONE ������
static NextFSMAction SettingMenu_StateNoOperationHandler(CSettingMenu *pMe);

// ״̬ SETTINGMENUST_INIT ������
static NextFSMAction SettingMenu_StateInitHandler(CSettingMenu *pMe);

// ״̬ SETTINGMENUST_MAIN ������
static NextFSMAction SettingMenu_StateMainHandler(CSettingMenu *pMe);

// ״̬ SETTINGMENUST_CALLSETTING ������
static NextFSMAction SettingMenu_StateCallSettingHandler(CSettingMenu *pMe);

// ״̬ SETTINGMENUST_PHONESETTING ������
static NextFSMAction SettingMenu_StatePhoneSettingHandler(CSettingMenu *pMe);

// ״̬ SETTINGMENUST_CALLSETTINGSEL ������
static NextFSMAction SettingMenu_StateCallSettingSelHandler(CSettingMenu *pMe);

// ״̬ SETTINGMENUST_NETWORKSETTING ������
static NextFSMAction SettingMenu_StateNetworkSettingHandler(CSettingMenu *pMe);

// ״̬ SETTINGMENUST_DIVERT ������
static NextFSMAction SettingMenu_StateDivertHandler(CSettingMenu *pMe);

// ״̬ SETTINGMENUST_CALLFORWARDSEL ������
static NextFSMAction SettingMenu_StateCallForwardSelHandler(CSettingMenu *pMe);

// ״̬ SETTINGMENUST_CALLFORWARDINPUT ������
static NextFSMAction SettingMenu_StateCallForwardInputHandler(CSettingMenu *pMe);
#ifdef FEATURE_KEYGUARD
// ״̬ SETTINGMENUST_AKG ������
static NextFSMAction SettingMenu_StateAKGHandler(CSettingMenu *pMe);
#endif

#ifdef FEATURE_SET_BANNER
// ״̬ SETTINGMENUST_BANNER ������
static NextFSMAction SettingMenu_StateBannerHandler(CSettingMenu *pMe);
#endif

#ifdef FEATURE_TIME_DATA_SETTING
// ״̬ TIMESETTING ������
static NextFSMAction SettingMenu_StateTimeHandler(CSettingMenu *pMe);
#ifdef FEATURE_DOUBLE_SIM_CARD
static NextFSMAction   SettingMenu_StateSimHandler(CSettingMenu *pMe);
static NextFSMAction   SettingMenu_StateSimChoiceHandler(CSettingMenu *pMe);
#endif
// ״̬ dateSETTING ������
static NextFSMAction SettingMenu_StateDateHandler(CSettingMenu *pMe);
#endif
// ״̬ SETTINGMENUST_LANGUAGE ������
#ifdef FEATURE_LANGUAGE
static NextFSMAction SettingMenu_StateLanguageHandler(CSettingMenu *pMe);
#endif
// ״̬ SETTINGMENUST_EXIT ������
static NextFSMAction SettingMenu_StateExitHandler(CSettingMenu *pMe);

#ifdef FEATURE_PERU_VERSION
static NextFSMAction SettingMenu_StatePhoneNumerHandler(CSettingMenu *pMe);
#endif

#ifdef FEATRUE_AUTO_POWER
static NextFSMAction SettingMenu_StateAutoPowerHandler(CSettingMenu *pMe);
#endif
#ifdef FEATURE_SUPPORT_G_SENSOR
static NextFSMAction SettingMenu_StateShake(CSettingMenu *pMe);

static NextFSMAction SettingMenu_StateShakeSub(CSettingMenu *pMe);
#endif
static NextFSMAction SettingMenu_StatePasswordHandler(CSettingMenu *pMe); 

static NextFSMAction SettingMenu_StateCallRestrictHandler(CSettingMenu *pMe); 

static NextFSMAction SettingMenu_StateOutGoingHandler(CSettingMenu *pMe); 

static NextFSMAction SettingMenu_StateIncomingHandler(CSettingMenu *pMe);

static NextFSMAction SettingMenu_StateAutoAnswer_Mode(CSettingMenu *pMe);

#ifdef FEATRUE_SET_IP_NUMBER
static NextFSMAction SettingMenu_StateIP_Number_Set_Mode(CSettingMenu *pMe);
#endif
// ״̬ DLGRET_KEYLENGTH ������
static NextFSMAction Sound_StateKeylengthHandler(CSettingMenu *pMe);

static NextFSMAction Sound_StateAutoAnswerSubHandler(CSettingMenu *pMe);
#ifdef FEATURE_PLANEMODE
static NextFSMAction SettingMenu_StatePlaneModeHandler(CSettingMenu *pMe);
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
       SettingMenu_ProcessState
˵��:
       SettingMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��SettingMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction SettingMenu_ProcessState(CSettingMenu *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    MSG_FATAL("SettingMenu_ProcessState Start",0,0,0);
    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case SETTINGMENUST_NONE:
            retVal = SettingMenu_StateNoOperationHandler(pMe);
            break;

        case SETTINGMENUST_INIT:
            retVal = SettingMenu_StateInitHandler(pMe);
            break;

        case SETTINGMENUST_MAIN:
            ERR("SettingMenu_ProcessState1111111111111111111111111",0,0,0);
            retVal = SettingMenu_StateMainHandler(pMe);
            break;

        case SETTINGMENUST_CALLSETTING:
            retVal = SettingMenu_StateCallSettingHandler(pMe);
            break;

        case SETTINGMENUST_PHONESETTING:
            retVal = SettingMenu_StatePhoneSettingHandler(pMe);
            break;

        case SETTINGMENUST_CALLSETTINGSEL:
            retVal = SettingMenu_StateCallSettingSelHandler(pMe);
            break;

        case SETTINGMENUST_NETWORKSETTING:
            retVal = SettingMenu_StateNetworkSettingHandler(pMe);
            break;

        case SETTINGMENUST_DIVERT:
            retVal = SettingMenu_StateDivertHandler(pMe);
            break;

        case SETTINGMENUST_CALLFORWARDSEL:
            retVal = SettingMenu_StateCallForwardSelHandler(pMe);
            break;

        case SETTINGMENUST_CALLFORWARDINPUT:
            retVal = SettingMenu_StateCallForwardInputHandler(pMe);
            break;
#ifdef FEATURE_KEYGUARD
        case SETTINGMENUST_AKG:
            retVal = SettingMenu_StateAKGHandler(pMe);
            break;
#endif

#ifdef FEATURE_SET_BANNER
        case SETTINGMENUST_BANNER:
            retVal = SettingMenu_StateBannerHandler(pMe);
            break;
#endif

#ifdef FEATURE_TIME_DATA_SETTING
        case SETTINGMENUST_TIMESETTING:
            retVal = SettingMenu_StateTimeHandler(pMe);
            break;

        case SETTINGMENUST_DATESETTING:
            retVal = SettingMenu_StateDateHandler(pMe);
            break;
#endif
#ifdef FEATURE_LANGUAGE
        case SETTINGMENUST_LANGUAGE:
            retVal = SettingMenu_StateLanguageHandler(pMe);
            break;
#endif
        case SETTINGMENUST_EXIT:
            retVal = SettingMenu_StateExitHandler(pMe);
            break;

#ifdef FEATURE_PERU_VERSION
        case SETTINGMENUST_PHONENUMBER:
            retVal = SettingMenu_StatePhoneNumerHandler(pMe);
            break;
#endif
#ifdef FEATRUE_AUTO_POWER
        case SETTINGMENUST_AUTO_POWER:
            retVal = SettingMenu_StateAutoPowerHandler(pMe);
            break;
#endif
#ifdef FEATURE_SUPPORT_G_SENSOR
        case SETTINGMENUST_SHAKE:
            retVal = SettingMenu_StateShake(pMe);
            break;

        case SETTINGMENUST_SHAKESUB:
            retVal = SettingMenu_StateShakeSub(pMe);
            break;
#endif
        case SETTINGMENUST_PASSWORD:
            retVal = SettingMenu_StatePasswordHandler(pMe);
            break;
            
        case SETTINGMENUST_CALLRESTRICT:
            retVal = SettingMenu_StateCallRestrictHandler(pMe);
            break;
            
        case SETTINGMENUST_RESTRICTOUTGOING:
            retVal = SettingMenu_StateOutGoingHandler(pMe);
            break;
        
        case SETTINGMENUST_RESTRICTINCOMING:
            retVal = SettingMenu_StateIncomingHandler(pMe);
            break;
        case SETTINGMENUST_AUTO_ANSWER:
            retVal = SettingMenu_StateAutoAnswer_Mode(pMe);
            break;

#ifdef FEATRUE_SET_IP_NUMBER
        case SETTINGMENUST_IP_NUMBER_SET:
            retVal = SettingMenu_StateIP_Number_Set_Mode(pMe);
            break;
#endif
        case SETTINGMENUST_KEYLENGTH:
            retVal = Sound_StateKeylengthHandler(pMe);
            break;

        case SETTINGMENUST_AUTOANSWERSUB:
            retVal = Sound_StateAutoAnswerSubHandler(pMe);
            break;

#ifdef FEATURE_PLANEMODE
        case SETTINGMENUST_PLANEMODE:
            retVal = SettingMenu_StatePlaneModeHandler(pMe);
            break; 
#endif
#ifdef FEATURE_DOUBLE_SIM_CARD
        case SETTINGMENUST_SIMSETTING:
            retVal = SettingMenu_StateSimHandler(pMe);
            break;
        case SETTINGMENUST_SIMSETTING_CHOICE:
            MSG_FATAL("SettingMenu_ProcessState SETTINGMENUST_SIMSETTING_CHOICE",0,0,0);
            retVal = SettingMenu_StateSimChoiceHandler(pMe);
            break;            
#endif
        default:
            ASSERT_NOT_REACHABLE;
    }
    MSG_FATAL("curstate %d prestate %d dlgret %d CallApp_ProcessState",pMe->m_eCurState,pMe->m_ePreState,pMe->m_eDlgRet);
    return retVal;
}


/*==============================================================================
������
       StateNoOperationHandler
˵����
       SETTINGMENUST_NONE ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateNoOperationHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(SETTINGMENUST_EXIT)
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler


/*==============================================================================
������
       StateInitHandler
˵����
       SETTINGMENUST_INIT ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateInitHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(SETTINGMENUST_MAIN)
    return NFSMACTION_CONTINUE;
} // StateInitHandler


/*==============================================================================
������
       StateMainHandler
˵����
       SETTINGMENUST_MAIN ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateMainHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    pMe->m_nSubDlgId = 0 ;    //���¼����Ӳ˵�ѡ������λ��

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_MAIN_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_PHONESETTING:
            MOVE_TO_STATE(SETTINGMENUST_PHONESETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_CALLSETTING:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            return NFSMACTION_CONTINUE;

#ifdef FEATURE_PERU_VERSION
        case DLGRET_PHONE_NUMBER:
            MOVE_TO_STATE(SETTINGMENUST_PHONENUMBER)
            return NFSMACTION_CONTINUE;
#endif
#ifdef FEATRUE_AUTO_POWER
         case DLGRET_AUTO_POWER:
            MOVE_TO_STATE(SETTINGMENUST_AUTO_POWER)
            return NFSMACTION_CONTINUE;
#endif
#ifdef FEATURE_SUPPORT_G_SENSOR
        case DLGRET_SHAKE:
            MOVE_TO_STATE(SETTINGMENUST_SHAKE)
            return NFSMACTION_CONTINUE;
#endif
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateMainHandler


/*==============================================================================
������
       StateCallSettingHandler
˵����
       SETTINGMENUST_CALLSETTING ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateCallSettingHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_CALLSETTING_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CALLSETTINGSEL:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTINGSEL)
            return NFSMACTION_CONTINUE;

        case DLGRET_NETWORKSETTING:
            MOVE_TO_STATE(SETTINGMENUST_NETWORKSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_DIVERT:
            MOVE_TO_STATE(SETTINGMENUST_DIVERT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CALLFORWARDSEL:
            MOVE_TO_STATE(SETTINGMENUST_CALLFORWARDSEL)
            return NFSMACTION_CONTINUE;

        case DLGRET_AUTO_ANSWER_MODE:
            MOVE_TO_STATE(SETTINGMENUST_AUTO_ANSWER)
            return NFSMACTION_CONTINUE;
            
#ifdef FEATURE_CARRIER_THAILAND_HUTCH              
        case DLGRET_AUTO_ANSWER_MODE_SUB:
            MOVE_TO_STATE(SETTINGMENUST_AUTOANSWERSUB)
            return NFSMACTION_CONTINUE;
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
        
        case DLGRET_PASSWORD:
        	MOVE_TO_STATE(SETTINGMENUST_PASSWORD)
        	return NFSMACTION_CONTINUE;           
            
        case DLGRET_CALLFORWARDINPUT:
            MOVE_TO_STATE(SETTINGMENUST_CALLFORWARDINPUT)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_MAIN)
            return NFSMACTION_CONTINUE;
#ifdef FEATRUE_SET_IP_NUMBER
        case DLGRET_IP_NUMBER_SET:
            MOVE_TO_STATE(SETTINGMENUST_IP_NUMBER_SET)
            return NFSMACTION_CONTINUE;
#endif//#ifdef FEATRUE_SET_IP_NUMBER

        case DLGRET_KEYLENGTH:
            MOVE_TO_STATE(SETTINGMENUST_KEYLENGTH)
            return NFSMACTION_CONTINUE;
        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateCallSettingHandler


/*==============================================================================
������
       StatePhoneSettingHandler
˵����
       SETTINGMENUST_PHONETTING ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StatePhoneSettingHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_PHONESETTING_MENU);
            return NFSMACTION_WAIT;

#ifdef FEATURE_PLANEMODE
        case DLGRET_PLANEMODE:
            MOVE_TO_STATE(SETTINGMENUST_PLANEMODE)
            return NFSMACTION_CONTINUE;
#endif
#ifdef FEATURE_LANGUAGE
        case DLGRET_LANGUAGE:
            MOVE_TO_STATE(SETTINGMENUST_LANGUAGE)
            return NFSMACTION_CONTINUE;
#endif
#ifdef FEATURE_KEYGUARD
        case DLGRET_AKG:
            MOVE_TO_STATE(SETTINGMENUST_AKG)
            return NFSMACTION_CONTINUE;
#endif

#ifdef FEATURE_SET_BANNER
        case DLGRET_BANNER:
            MOVE_TO_STATE(SETTINGMENUST_BANNER)
            return NFSMACTION_CONTINUE;
#endif

        #ifdef FEATURE_TIME_DATA_SETTING
        case DLGRET_TIMESETTING:
            MOVE_TO_STATE(SETTINGMENUST_TIMESETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_DATESETTING:
            MOVE_TO_STATE(SETTINGMENUST_DATESETTING)
            return NFSMACTION_CONTINUE;
         #endif
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_MAIN)
            return NFSMACTION_CONTINUE;
#ifdef  FEATURE_DOUBLE_SIM_CARD
        case DLGRET_SIMSETTING:
            MOVE_TO_STATE(SETTINGMENUST_SIMSETTING)
            return NFSMACTION_CONTINUE;
        
#endif

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StatePhoneSettingHandler


/*==============================================================================
������
       StateCallSettingSelHandler
˵����
       SETTINGMENUST_CALLSETTINGSEL ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateCallSettingSelHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_CALLSETTINGSEL);
            return NFSMACTION_WAIT;

#ifdef FEATRUE_SET_IP_NUMBER
        case DLGRET_CALLFORWARDINPUT:
            MOVE_TO_STATE(SETTINGMENUST_CALLFORWARDINPUT)
            return NFSMACTION_CONTINUE;
#endif//#ifdef FEATRUE_SET_IP_NUMBER
#ifdef FEATRUE_AUTO_POWER
        case DLGRET_CALLSETTING:
            MOVE_TO_STATE(SETTINGMENUST_MAIN)
            return NFSMACTION_CONTINUE;
#endif //#ifdef FEATRUE_AUTO_POWER
        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:    
            if(pMe->m_CallSettingSel == IDS_ANYKEY_ANSWER_TITLE
                ||pMe->m_CallSettingSel == IDS_COVER_ANSWER
                ||pMe->m_CallSettingSel == IDS_SEND_ANSWER
                ||pMe->m_CallSettingSel == IDS_HEADSET_ANSWER
                )
            {
                MOVE_TO_STATE(SETTINGMENUST_AUTO_ANSWER)
            }
#ifdef FEATRUE_SET_IP_NUMBER
            else if(pMe->m_CallSettingSel == IDS_SET_IP_CALL_OPTION)
            {
                MOVE_TO_STATE(SETTINGMENUST_IP_NUMBER_SET)
            }
#endif//#ifdef FEATRUE_SET_IP_NUMBER
#ifdef FEATRUE_AUTO_POWER
            else if(pMe->m_CallSettingSel == IDS_AM_PM_SET)
            {
                MOVE_TO_STATE(SETTINGMENUST_AUTO_POWER)
            }
#endif //#ifdef FEATRUE_AUTO_POWER
            else
            {
                MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
#ifdef FEATRUE_AUTO_POWER
            if(pMe->m_msg_id == IDS_AUTOPOWER_WARNING)
            {
                MOVE_TO_STATE(SETTINGMENUST_AUTO_POWER)
            }          
#endif
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateCallSettingSelHandler


/*==============================================================================
������
       StateNetworkSettingHandler
˵����
       SETTINGMENUST_NETWORKSETTING ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateNetworkSettingHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_NETWORKSETTING_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_DIVERT:
            MOVE_TO_STATE(SETTINGMENUST_DIVERT)
            return NFSMACTION_CONTINUE;

        case DLGRET_CALLFORWARDSEL:
            MOVE_TO_STATE(SETTINGMENUST_CALLFORWARDSEL)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateNetworkSettingHandler


/*==============================================================================
������
       StateDivertHandler
˵����
       SETTINGMENUST_DIVERT ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateDivertHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGBOX_OK:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_DIVERT_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CALLFORWARDSEL:
            MOVE_TO_STATE(SETTINGMENUST_CALLFORWARDSEL)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_msg_id = IDS_NOT_GET_FEATURE_CODE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateDivertHandler


/*==============================================================================
������
       StateCallForwardSelHandler
˵����
       SETTINGMENUST_CALLFORWARDSEL ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateCallForwardSelHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGBOX_OK:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_CALLFORWARD_SEL);
            return NFSMACTION_WAIT;

        case DLGRET_CALLFORWARDINPUT:
            MOVE_TO_STATE(SETTINGMENUST_CALLFORWARDINPUT)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            if(pMe->m_CFType == CALLFORWARD_WAIT 
#ifdef FEATURE_CARRIER_SUDAN_SUDATEL
                || pMe->m_CFType == CALLFORWARD_DND
#endif
                )
            {
                MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            }
            else
            {
                MOVE_TO_STATE(SETTINGMENUST_DIVERT)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_msg_id = IDS_NOT_GET_FEATURE_CODE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateCallForwardSelHandler


/*==============================================================================
������
       StateCallForwardInputHandler
˵����
       SETTINGMENUST_CALLFORWARDINPUT ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateCallForwardInputHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_MSGBOX_OK:
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_CALLFORWARD_INPUT);
            return NFSMACTION_WAIT;

        case DLGRET_WARNING:
            pMe->m_msg_id = IDS_WARNING_MESSEGE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
#ifdef FEATRUE_SET_IP_NUMBER
            if(pMe->m_input_mode == IDS_IP_NUMBER_SET)
            {
                MOVE_TO_STATE(SETTINGMENUST_IP_NUMBER_SET)
            }
            else if(pMe->m_input_mode == IDS_SET_IP_CALL_OPTION)
            {
                MOVE_TO_STATE(SETTINGMENUST_CALLSETTINGSEL)
            }
            else
#endif
            {
                MOVE_TO_STATE(SETTINGMENUST_DIVERT)
            }
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateCallForwardInputHandler

#ifdef FEATURE_KEYGUARD
/*==============================================================================
������
       StateAKGHandler
˵����
       SETTINGMENUST_AKG ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateAKGHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_AKG_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:            
            MOVE_TO_STATE(SETTINGMENUST_PHONESETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateAKGHandler
#endif
#ifdef FEATURE_SET_BANNER
/*==============================================================================
������
       StateBannerHandler
˵����
       SETTINGMENUST_BANNER ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateBannerHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_BANNER_TXT);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_PHONESETTING)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateBannerHandler
#endif
/*==============================================================================
������
       StateTimeHandler
˵����
       SETTINGMENUST_timesetting ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
#ifdef FEATURE_TIME_DATA_SETTING
static NextFSMAction SettingMenu_StateTimeHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_TIMESETTING);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:            
            MOVE_TO_STATE(SETTINGMENUST_PHONESETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateTimeHandler


/*==============================================================================
������
       StatedateHandler
˵����
       SETTINGMENUST_datesetting ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateDateHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_DATESETTING);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:            
            MOVE_TO_STATE(SETTINGMENUST_PHONESETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StatedateHandler

#endif
/*==============================================================================
������
       StateTimeHandler
˵����
       SETTINGMENUST_timesetting ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
#ifdef FEATURE_DOUBLE_SIM_CARD
static NextFSMAction   SettingMenu_StateSimChoiceHandler(CSettingMenu *pMe)
{
    MSG_FATAL("SettingMenu_StateSimChoiceHandler Start",0,0,0);
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            MSG_FATAL("SettingMenu_StateSimChoiceHandler DLGRET_CREATE",0,0,0);
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_SIMSETTING_CHOICE_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:            
            MOVE_TO_STATE(SETTINGMENUST_SIMSETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_SIMSETTING:
            MSG_FATAL("SettingMenu_StateSimChoiceHandler DLGRET_SIMSETTING",0,0,0);
            MOVE_TO_STATE(DLGRET_SIMSETTING)
            return NFSMACTION_CONTINUE;            

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}

static NextFSMAction   SettingMenu_StateSimHandler(CSettingMenu *pMe)
{
    MSG_FATAL("SettingMenu_StateSimHandler Start",0,0,0);
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_SIMSETTING);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:            
            MOVE_TO_STATE(SETTINGMENUST_PHONESETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;
            
        case DLGRET_SIMSETTING_CHOICE:
            MSG_FATAL("SettingMenu_StateSimHandler DLGRET_SIMSETTING_CHOICE",0,0,0);
            MOVE_TO_STATE(SETTINGMENUST_SIMSETTING_CHOICE)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}
#endif

/*==============================================================================
������
       StateLanguageHandler
˵����
       SETTINGMENUST_LANGUAGE ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
#ifdef FEATURE_LANGUAGE
static NextFSMAction SettingMenu_StateLanguageHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_LANGUAGE_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:            
            MOVE_TO_STATE(SETTINGMENUST_PHONESETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        //case DLGRET_OK:
        //    ISHELL_CloseApplet(pMe->m_pShell, TRUE);
        //    return NFSMACTION_WAIT;
            
        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateLanguageHandler
#endif

/*==============================================================================
������
       StateExitHandler
˵����
       SETTINGMENUST_EXIT ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateExitHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler
#ifdef FEATURE_PERU_VERSION
/*==============================================================================
������
       StatePhoneNumerHandler
˵����
       SETTINGMENUST_PHONENUMBER ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StatePhoneNumerHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            SettingMenu_ShowDialog(pMe, IDD_PHONE_NUMBER);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}
#endif
#ifdef FEATRUE_AUTO_POWER
static NextFSMAction SettingMenu_StateAutoPowerHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            SettingMenu_ShowDialog(pMe, IDD_AUTO_POWER);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_CALLSETTINGSEL:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTINGSEL)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_OK:
            if(pMe->m_msg_id == IDS_DONE)
            {
                MOVE_TO_STATE(SETTINGMENUST_MAIN)
                return NFSMACTION_CONTINUE;
            }
            else
            {
                SettingMenu_ShowDialog(pMe, IDD_AUTO_POWER);
                return NFSMACTION_WAIT;
            }

        default:
            ASSERT_NOT_REACHABLE;
    }
    return NFSMACTION_WAIT;
}
#endif
static NextFSMAction SettingMenu_StateAutoAnswer_Mode(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            SettingMenu_ShowDialog(pMe, IDD_ANSWER_MODE);
            return NFSMACTION_WAIT;

        case DLGRET_CALLSETTINGSEL:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTINGSEL)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_AUTO_ANSWER_MODE_SUB:
            MOVE_TO_STATE(SETTINGMENUST_AUTOANSWERSUB)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}
#ifdef FEATRUE_SET_IP_NUMBER
static NextFSMAction SettingMenu_StateIP_Number_Set_Mode(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            SettingMenu_ShowDialog(pMe, IDD_IP_NUMBER_SET);
            return NFSMACTION_WAIT;

        case DLGRET_CALLFORWARDINPUT:
            MOVE_TO_STATE(SETTINGMENUST_CALLFORWARDINPUT)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_CALLSETTINGSEL:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTINGSEL)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}
#endif /*FEATRUE_SET_IP_NUMBER*/

/*==============================================================================
������
       StateKeylengthHandler
˵����
       SOUNDMENUST_KEYLENGTH ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateKeylengthHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_KEYTONE_LENGTH);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateKeylengthHandler

/*==============================================================================
������
       StateAutoAnswerSubHandler
˵����
       SOUNDMENUST_AUTOANSWERSUB ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateAutoAnswerSubHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_AUTOANSWER_SUB);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:         
#ifdef FEATURE_CARRIER_THAILAND_HUTCH               
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
#else            
            MOVE_TO_STATE(SETTINGMENUST_AUTO_ANSWER)   
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
            return NFSMACTION_CONTINUE;

        default:
            return NFSMACTION_WAIT;
    }

    return NFSMACTION_WAIT;
} // StateAutoAnswerSubHandler
#ifdef FEATURE_SUPPORT_G_SENSOR
/*==============================================================================
������
       StateShake
˵����
       SETTINGMENUST_SHAKE ״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
#ifdef FEATURE_SPORTS_APP
extern boolean is_g_sportsapp_pedometer_bground_flag();
#endif
static NextFSMAction SettingMenu_StateShake(CSettingMenu *pMe)
{
#ifdef FEATURE_SPORTS_APP

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:    
        {
            if(is_g_sportsapp_pedometer_bground_flag())
            {
                pMe->m_msg_id = IDS_PROCCESS;
                SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            }
            else
            {
                SettingMenu_ShowDialog(pMe, IDD_SHAKE);
            }
            return NFSMACTION_WAIT;
        }
        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(SETTINGMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_SHAKESUB:
            MOVE_TO_STATE(SETTINGMENUST_SHAKESUB)
            return NFSMACTION_CONTINUE;
        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_PROCCESS;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;    
        default:
            ASSERT_NOT_REACHABLE;
    }
#endif    
    return NFSMACTION_WAIT;	     

}

/*==============================================================================
������
       StateShakeSub
˵����
       SETTINGMENUST_SHAKESUB״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SettingMenu_StateShakeSub(CSettingMenu *pMe)
{

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:    
            SettingMenu_ShowDialog(pMe, IDD_SHAKESUB);
            return NFSMACTION_WAIT;
            
        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(SETTINGMENUST_SHAKE)
            return NFSMACTION_CONTINUE;
        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;
            
        default:
            ASSERT_NOT_REACHABLE;
    }
    return NFSMACTION_WAIT;	     

}
#endif //#ifdef FEATURE_SUPPORT_G_SENSOR
/*==============================================================================
������
       SettingMenu_StateCallRestrictHandler
˵����
       SETTINGMENUST_CallRestrict״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/ 
static NextFSMAction SettingMenu_StateCallRestrictHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_RESTRICT);
            return NFSMACTION_WAIT;

        case DLGRET_RESTRICTOUTGOING:
            MOVE_TO_STATE(SETTINGMENUST_RESTRICTOUTGOING)
            return NFSMACTION_CONTINUE;

        case DLGRET_RESTRICTINCOMING:
            MOVE_TO_STATE(SETTINGMENUST_RESTRICTINCOMING)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateRestrictHandler

/*==============================================================================
������
       SettingMenu_StateOutGoingHandler
˵����
       SETTINGMENUST_OutGoing״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/ 
static NextFSMAction SettingMenu_StateOutGoingHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_RESTRICT_OUTGOING);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:            
            MOVE_TO_STATE(SETTINGMENUST_CALLRESTRICT)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SettingMenu_StateIncomingHandler
˵����
       SETTINGMENUST_Incoming״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StateIncomingHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_RESTRICT_INCOMING);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:            
            MOVE_TO_STATE(SETTINGMENUST_CALLRESTRICT)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}
/*==============================================================================
������
       SettingMenu_StatePasswordHandler
˵����
       SETTINGMENUST_PASSWORD״̬������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StatePasswordHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_PASSWORD);
            return NFSMACTION_WAIT;
            
        case DLGRET_INVALID:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe,IDD_MSGBOX);
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:
            MOVE_TO_STATE(SETTINGMENUST_CALLRESTRICT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(SETTINGMENUST_PASSWORD)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_CANCEL:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            return NFSMACTION_CONTINUE;
        
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SETTINGMENUST_CALLSETTING)
            return NFSMACTION_CONTINUE;

        default:
        break;
    }

    return NFSMACTION_WAIT;
} // StatePasswordHandler
        	
#ifdef FEATURE_PLANEMODE
/*==============================================================================
������
       SettingMenu_StatePlaneModeHandler
˵����
       SETTINGMENUST_PLANEMODE ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SettingMenu_StatePlaneModeHandler(CSettingMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SettingMenu_ShowDialog(pMe, IDD_PLANEMODE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:            
            MOVE_TO_STATE(SETTINGMENUST_PHONESETTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_WARNING:
            
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DONE;
            SettingMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // SettingMenu_StatePlaneModeHandler
#endif 

