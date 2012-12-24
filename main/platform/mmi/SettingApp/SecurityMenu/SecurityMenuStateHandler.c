/*==============================================================================
// �ļ���
//        SecurityMenuStateHandler.c
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
-----------      ----------  --------  -----------------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "SecurityMenu_priv.h"
#ifdef FEATURE_ICM
#include "AEECM.h"
#else
#include "AEETelephone.h"
#include "AEETelDef.h"
#endif
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ SECURITYMENU_NONE ������
static NextFSMAction Security_StateNoOperationHandler(CSecurityMenu *pMe);

// ״̬ SECURITYMENU_INIT ������
static NextFSMAction Security_StateInitHandler(CSecurityMenu *pMe);

// ״̬ SECURITYMENU_MAIN ������
static NextFSMAction Security_StateMainHandler(CSecurityMenu *pMe);
//״̬  SECURITYMENU_APPLICATIONLOCK������
static NextFSMAction Security_StateApplicationLockHandler(CSecurityMenu *pMe);

//״̬SECURITYMENU_PHONEPASSWORD ������
static NextFSMAction Security_StatePhonePassWordHandler(CSecurityMenu *pMe);

//״̬SECURITYMENU_KEYLOCK������
static NextFSMAction Security_StatePhoneKeylockHandler(CSecurityMenu *pMe);

//״̬SECURITYMENU_PHONEPASSWORDINPUT ������
static NextFSMAction Security_StateCallPassWordInputHandler(CSecurityMenu *pMe);
// ״̬ SECURITYMENU_PINCHECK ������
static NextFSMAction Security_StatePinCheckHandler(CSecurityMenu *pMe);

//״̬ SECURITYMENU_PINCHANGE ������
static NextFSMAction Security_StatePinChangeHandler(CSecurityMenu *pMe);

// ״̬ SECURITYMENU_ASKPASSWORD ������
static NextFSMAction Security_StateAskPasswordHandler(CSecurityMenu *pMe);

#ifdef FEATURE_LCD_TOUCH_ENABLE
// ״̬ SECURITYMENU_TSIMPASSWORDINPUT ������
static NextFSMAction Security_StateTsimPasswordInputHandler(CSecurityMenu *pMe);    
#endif

// ״̬ SECURITYMENU_ASKPIN ������
static NextFSMAction Security_StateAskPinHandler(CSecurityMenu *pMe);

//״̬  SECURITYMENU_ASKCALLPASSWORD ������
static NextFSMAction Security_StateAskCallPasswordHandler(CSecurityMenu *pMe);

// ״̬ SECURITYMENU_AFFIRMPASSWORD ������
static NextFSMAction Security_StateAffirmPassWordHandler(CSecurityMenu *pMe);

//״̬  SECURITYMENU_ASKPUKPASSWORD������
static NextFSMAction Security_StateAskPUKPassWordHandler(CSecurityMenu * pMe);

//״̬  SECURITYMENU_UIMERR������
static NextFSMAction Security_StateUIMErrHandler(CSecurityMenu * pMe);

//״̬  SECURITYMENU_EMERGENCYCALL������
static NextFSMAction Security_StateEmergencyCallHandler(CSecurityMenu * pMe);

// ״̬ SETTINGMENUST_RESTORE ������
static NextFSMAction Security_StateRestoreHandler(CSecurityMenu *pMe);

// ״̬ SECURITYMENU_EXIT ������
static NextFSMAction Security_StateExitHandler(CSecurityMenu *pMe);

//״̬  SECURITYMENU_CHANGECODE ������
static NextFSMAction Security_StateChangeCodeHandler(CSecurityMenu *pMe);
#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)
//״̬  MOBILE_TRACKER ������
static NextFSMAction Security_StateMobileTracker(CSecurityMenu *pMe);
#endif

#ifdef FEATURE_VERSION_C316	
static NextFSMAction Security_StateOneKeyLockKeypadHandler(CSecurityMenu *pMe);
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
       SecurityMenu_ProcessState
˵��:
       SecurityMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��SecurityMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction SecurityMenu_ProcessState(CSecurityMenu *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case SECURITYMENU_NONE:
            retVal = Security_StateNoOperationHandler(pMe);
            break;

        case SECURITYMENU_INIT:
            retVal = Security_StateInitHandler(pMe);
            break;

        case SECURITYMENU_MAIN:
            retVal = Security_StateMainHandler(pMe);
            break;
        case SECURITYMENU_APPLICATIONLOCK:
            retVal = Security_StateApplicationLockHandler(pMe);
            break;
            
        case SECURITYMENU_CHANGECODE:
            retVal = Security_StateChangeCodeHandler(pMe);
            break;

        case SECURITYMENU_PHONEPASSWORD:
            retVal = Security_StatePhonePassWordHandler(pMe);
            break;
            
        #if defined(FEATURE_VERSION_HITZ181)||defined(FEATURE_VERSION_MTM)||defined(FEATURE_VERSION_W317A)
        case SECURITYMENU_KEYLOCK:
        	retVal = Security_StatePhoneKeylockHandler(pMe);
            break;
        #endif
            
        case SECURITYMENU_PHONEPASSWORDINPUT:
            retVal = Security_StateCallPassWordInputHandler(pMe);
            break;
        case SECURITYMENU_PINCHECK:
            retVal = Security_StatePinCheckHandler(pMe);
            break;

        case SECURITYMENU_PINCHANGE:
            retVal = Security_StatePinChangeHandler(pMe);
            break;

        case SECURITYMENU_ASKPIN:
            retVal = Security_StateAskPinHandler(pMe);
            break;

        case SECURITYMENU_ASKPASSWORD:
            retVal = Security_StateAskPasswordHandler(pMe);
            break;
#ifdef FEATURE_LCD_TOUCH_ENABLE
        case SECURITYMENU_TSIMPASSWORDINPUT:
            retVal = Security_StateTsimPasswordInputHandler(pMe);
            break;
#endif
        case SECURITYMENU_ASKCALLPASSWORD:
            retVal = Security_StateAskCallPasswordHandler(pMe);
            break;

        case SECURITYMENU_AFFIRMPASSWORD:
            retVal = Security_StateAffirmPassWordHandler(pMe);
            break;

        case SECURITYMENU_ASKPUKPASSWORD:
            retVal = Security_StateAskPUKPassWordHandler(pMe);
            break;

        case SECURITYMENU_UIMERR:
            retVal = Security_StateUIMErrHandler(pMe);
            break;
            
        case SECURITYMENU_EMERGENCYCALL:
            retVal = Security_StateEmergencyCallHandler(pMe);
            break;
		#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)
		case MOBILE_TRACKER:
			retVal = Security_StateMobileTracker(pMe);
			break;
		#endif
        case SECURITYMENU_RESTORE:
            retVal = Security_StateRestoreHandler(pMe);
            break;

        case SECURITYMENU_EXIT:
            retVal = Security_StateExitHandler(pMe);
            break;

#ifdef FEATURE_VERSION_C316	
        case SECURITYMENU_ONEKEY_LOCK_KEYPAD:
            retVal = Security_StateOneKeyLockKeypadHandler(pMe);
            break;
#endif

        default:
            break;
    }
    SEC_ERR("curstate %d prestate %d dlgret %d CallApp_ProcessState",pMe->m_eCurState,pMe->m_ePreState,pMe->m_eDlgRet);
    return retVal;
}

/*==============================================================================
������
       StateNoOperationHandler
˵����
       SECURITYMENU_NONE ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateNoOperationHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(SECURITYMENU_EXIT)
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler


/*==============================================================================
������
       StateInitHandler
˵����
       SECURITYMENU_INIT ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateInitHandler(CSecurityMenu *pMe)
{
    boolean IsFactoryTestMode = FALSE;
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_FACTORY_TEST_MODE,
                          &IsFactoryTestMode,
                          sizeof(IsFactoryTestMode));
    if(IsFactoryTestMode)
    {
        MOVE_TO_STATE(SECURITYMENU_MAIN)
    }
    else
    {
        MOVE_TO_STATE(SECURITYMENU_ASKPASSWORD)
    }
    return NFSMACTION_CONTINUE;
} // StateInitHandler


/*==============================================================================
������
       StateMainHandler
˵����
       SECURITYMENU_MAIN ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateMainHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StateMainHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_MAIN_DIALOG);
            return NFSMACTION_WAIT;
        case DLG_APPLICATIONLOCK:
            MOVE_TO_STATE(SECURITYMENU_APPLICATIONLOCK)
            return NFSMACTION_CONTINUE;
        case DLG_PINCHECK:
            MOVE_TO_STATE(SECURITYMENU_PINCHECK)
            return NFSMACTION_CONTINUE;
    
        case DLGRET_RESTORE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            Security_ShowMsgBox(pMe, IDS_RESTORE_CONFIRM);
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_CONFIRM:
            if(pMe->m_wMsgID == IDS_RESTORE_CONFIRM)
            {
                pMe->m_wMsgID = IDS_RESTORING;
            }
            else if(pMe->m_wMsgID == IDS_DELETE_CONFIRM)
            {
                pMe->m_wMsgID = IDS_DELETING;
            }
            SecurityMenu_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_CANCEL:
            MOVE_TO_STATE(SECURITYMENU_MAIN)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_DELETE:
            pMe->m_bNotOverwriteDlgRet = FALSE;            
            Security_ShowMsgBox(pMe, IDS_DELETE_CONFIRM);
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:
            MOVE_TO_STATE(SECURITYMENU_MAIN);
            return NFSMACTION_CONTINUE;
        case DLGRET_MSGBOX_OK:
            if(pMe->m_wMsgID != IDS_SAVED)
            {
                Security_ShowMsgBox(pMe, IDS_SAVED);
                return NFSMACTION_WAIT;
            }
            else
            {
                MOVE_TO_STATE(SECURITYMENU_MAIN);
                return NFSMACTION_CONTINUE;
            }
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SECURITYMENU_EXIT)
            return NFSMACTION_CONTINUE;

        case DLG_PHONEPASSWORD:
            MOVE_TO_STATE(SECURITYMENU_PHONEPASSWORD)
            return NFSMACTION_CONTINUE;

#ifdef FEATURE_VERSION_C316	
        case DLGRET_ONEKEY_LOCK_KEYPAD:
            MOVE_TO_STATE(SECURITYMENU_ONEKEY_LOCK_KEYPAD)
            return NFSMACTION_CONTINUE;
#endif
            
        #if defined(FEATURE_VERSION_HITZ181)||defined(FEATURE_VERSION_MTM)||defined(FEATURE_VERSION_W317A)
        case DLGRET_KEYLOCK:
        	MOVE_TO_STATE(SECURITYMENU_KEYLOCK)
            return NFSMACTION_CONTINUE;
        #endif

        case DLGRET_CHANGECODE:
            MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
            return NFSMACTION_CONTINUE;
		case DLGRET_ARKPASSWORD:
			MOVE_TO_STATE(SECURITYMENU_ASKPASSWORD)
			return NFSMACTION_CONTINUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE
		case DLGRET_TSIMPASSWORDINPUT:
            MOVE_TO_STATE(SECURITYMENU_TSIMPASSWORDINPUT)
            return NFSMACTION_CONTINUE;
#endif
#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)
	   case DLGRET_MOBILE_TRACKER:
	   		MOVE_TO_STATE(MOBILE_TRACKER)
	   		return NFSMACTION_CONTINUE;
#endif 
        default:
            break;
    }

    return NFSMACTION_WAIT;
} // StateMainHandler

/*==============================================================================
������
       StateApplicationLockHandler
˵����
       SECURITYMENU_APPLICATIONLOCK ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateApplicationLockHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_APPLICATIONLOCK_DIALOG);
            return NFSMACTION_WAIT;            
        
        case DLGRET_TOSHOWMSG:
            //pMe->m_wMsgID = IDS_SAVED;
            Security_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;             

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(SECURITYMENU_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // StateApplicationLockHandler


#ifdef FEATURE_VERSION_C316
/*==============================================================================
������
       Security_StateOneKeyLockKeypadHandler
˵����
       SECURITYMENU_ONEKEY_LOCK_KEYPAD ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateOneKeyLockKeypadHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_ONEKEY_LOCK_KEYPAD_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            if( pMe->m_lock_sel == SEC_SEL_PHONE_LOCK)
            {
                MOVE_TO_STATE(SECURITYMENU_MAIN)
            }
            else
            {
                MOVE_TO_STATE(SECURITYMENU_MAIN)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_TOSHOWMSG:
            //pMe->m_wMsgID = IDS_SAVED;
            Security_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT; 

        default:
            break;
    }

    return NFSMACTION_WAIT;

}

#endif

/*==============================================================================
������
       StatePhonePassWordHandler
˵����
       SECURITYMENU_PHONEPASSWORD ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StatePhonePassWordHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StatePhonePassWordHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_PHONE_PASSWORD_CHECK_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            if( pMe->m_lock_sel == SEC_SEL_PHONE_LOCK)
            {
                MOVE_TO_STATE(SECURITYMENU_MAIN)
            }
            else
            {
                MOVE_TO_STATE(SECURITYMENU_MAIN)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_TOSHOWMSG:
            //pMe->m_wMsgID = IDS_SAVED;
            Security_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT; 

        default:
            break;
    }

    return NFSMACTION_WAIT;

}
/*==============================================================================
������
       Security_StatePhoneKeylockHandler
˵����
       SECURITYMENU_KEYLOCK ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StatePhoneKeylockHandler(CSecurityMenu *pMe)
{
	if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StatePhonePassWordHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_KEY_LOCK_CHECK_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            if( pMe->m_lock_sel == SEC_SEL_PHONE_LOCK)
            {
                MOVE_TO_STATE(SECURITYMENU_MAIN)
            }
            else
            {
                MOVE_TO_STATE(SECURITYMENU_MAIN)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_TOSHOWMSG:
            //pMe->m_wMsgID = IDS_SAVED;
            Security_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT; 

        default:
            break;
    }

    return NFSMACTION_WAIT;
}


/*==============================================================================
������
       StateCallPassWordInputHandler
˵����
       SECURITYMENU_PHONEPASSWORDINPUT ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateCallPassWordInputHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StateCallPassWordInputHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_PHONE_PASSWORD_INPUT_DIALOG);
            return NFSMACTION_WAIT;
        case DLGRET_VALIDPINFAILED:
            //pMe->m_wMsgID = IDS_MSG_INPUTINVALID;
            Security_ShowMsgBox(pMe, IDS_MSG_INPUTINVALID);
            return NFSMACTION_WAIT; 
            
        case DLGRET_OK:
            MOVE_TO_STATE(SECURITYMENU_PHONEPASSWORDINPUT);          
            return NFSMACTION_CONTINUE;
        
        case DLGRET_AFFIRMPASSWORD://DLGRET_INPUTSECOND:
            if(IsRunAsUIMVersion())
            {
                pMe->m_IsPin = FALSE;
            }

            MOVE_TO_STATE(SECURITYMENU_AFFIRMPASSWORD)
            return NFSMACTION_CONTINUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE            
        case DLGRET_TSIMPASSWORDINPUT:
            MOVE_TO_STATE(SECURITYMENU_TSIMPASSWORDINPUT); 
            return NFSMACTION_CONTINUE;  
#endif
        //case DLGRET_OK:
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
            return NFSMACTION_CONTINUE;
            

        default:
            break;
    }

    return NFSMACTION_WAIT;

}

/*==============================================================================
������
       StatePinCheckHandler
˵����
       SECURITYMENU_PINCHECK ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StatePinCheckHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StatePinCheckHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_PIN_CHECK_DIALOG);
            return NFSMACTION_WAIT;
        case DLGRET_ISCOMFIRMPASSWORD:
            pMe->m_bIsConfirmPassword = TRUE;
            MOVE_TO_STATE(SECURITYMENU_ASKPIN)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_RESTRICT:
            Security_ShowMsgBox(pMe, IDS_NOTALLOW);
            return NFSMACTION_WAIT; 
            
        case DLGRET_OK:
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SECURITYMENU_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       StatePinChangeHandler
˵����
       SECURITYMENU_PINCHANGE ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StatePinChangeHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StatePinChangeHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_PIN_CHANGE_DIALOG);
            return NFSMACTION_WAIT;
            
        case DLGRET_VALIDPINFAILED:
            //pMe->m_wMsgID = IDS_MSG_INPUTINVALID;
            Security_ShowMsgBox(pMe, IDS_MSG_INPUTINVALID);
            return NFSMACTION_WAIT; 
#ifdef FEATURE_LCD_TOUCH_ENABLE            
        case DLGRET_TSIMPASSWORDINPUT:
            MOVE_TO_STATE(SECURITYMENU_TSIMPASSWORDINPUT);          
            return NFSMACTION_CONTINUE;
#endif            
        case DLGRET_OK:
            MOVE_TO_STATE(SECURITYMENU_PINCHANGE);          
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
            return NFSMACTION_CONTINUE;

        case DLGRET_AFFIRMPASSWORD://DLGRET_INPUTSECOND:
            pMe->m_IsPin = TRUE;
            MOVE_TO_STATE(SECURITYMENU_AFFIRMPASSWORD)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       StateAskPasswordHandler
˵����
       SECURITYMENU_ASKPASSWORD ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateAskPasswordHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StateAskPinHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            #if 0
            if(IsRunAsUIMVersion())
            {
                AEECHVStatus  chvst;
                chvst.chv1CountRemain = 0;
                IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
                pMe->m_nCanAttemptTimes = chvst.chv1CountRemain/*MAX_PIN_ATTEMPTS*/;
            }
            #endif
            SecurityMenu_ShowDialog(pMe, IDD_ASK_PASSWORD_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_VALIDPINPASS:

			switch(pMe->m_currDlgId)
			{
				case IDS_RESTORE: 
					{
						MOVE_TO_STATE(SECURITYMENU_RESTORE);
					}
				break;
				case 0:
//				default:
					{
						MOVE_TO_STATE(SECURITYMENU_MAIN);
					}
				break;
			}
            

			return NFSMACTION_CONTINUE;

        case DLGRET_VALIDPINFAILED:
            //pMe->m_wMsgID = IDS_MSG_INPUTINVALID;
            Security_ShowMsgBox(pMe, IDS_MSG_INPUTINVALID);
            return NFSMACTION_WAIT; 
            
        case DLGRET_OK:
            MOVE_TO_STATE(SECURITYMENU_ASKPASSWORD);          
            return NFSMACTION_CONTINUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE            
        case DLGRET_TSIMPASSWORDINPUT:
            MOVE_TO_STATE(SECURITYMENU_TSIMPASSWORDINPUT);          
            return NFSMACTION_CONTINUE;    
#endif
        case DLGRET_CANCELED:
        MOVE_TO_STATE(SECURITYMENU_EXIT)
        return NFSMACTION_CONTINUE;

        default:
        break;
    }

    return NFSMACTION_WAIT;
} // StateAskPinHandler


#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316)
//״̬  MOBILE_TRACKER ������
static NextFSMAction Security_StateMobileTracker(CSecurityMenu *pMe)
{
	 if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
	switch(pMe->m_eDlgRet)
    {
    	case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
			SecurityMenu_ShowDialog(pMe, IDD_MOBILE_TRACKER_DIALOG);
			return NFSMACTION_WAIT;
		case DLGRET_OK:
            //MOVE_TO_STATE(SECURITYMENU_ASKPASSWORD);     
            MOVE_TO_STATE(SECURITYMENU_MAIN);
            return NFSMACTION_CONTINUE;
		case DLGRET_CANCELED:
        	MOVE_TO_STATE(SECURITYMENU_EXIT)
        	return NFSMACTION_CONTINUE;
	}
	return NFSMACTION_WAIT;
}
#endif



/*==============================================================================
������
       TsimPasswordInput
˵����
       SECURITYMENU_TSIMPASSWORDINPUT ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��
==============================================================================*/
#ifdef FEATURE_LCD_TOUCH_ENABLE
static NextFSMAction Security_StateTsimPasswordInputHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    SEC_ERR("Security_StateTsimPasswordInputHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_TSIMINPUT_PASSWORD_DIALOG);
            return NFSMACTION_WAIT;
         //add by pyuangui
         case DLGRET_OK:  
            if(pMe->m_pActiveTSIMInputID==IDD_ASK_CALL_PASSWORD_DIALOG)
            MOVE_TO_STATE(SECURITYMENU_ASKCALLPASSWORD)  
            else if(pMe->m_pActiveTSIMInputID==IDD_PHONE_PASSWORD_INPUT_DIALOG)
            MOVE_TO_STATE(SECURITYMENU_PHONEPASSWORDINPUT)
            else if(pMe->m_pActiveTSIMInputID==IDD_AFFIRM_PASSWORD)
            MOVE_TO_STATE(SECURITYMENU_AFFIRMPASSWORD)
            else if(pMe->m_pActiveTSIMInputID==IDD_ASK_PIN_DIALOG)
            MOVE_TO_STATE(SECURITYMENU_ASKPIN)
            else if(pMe->m_pActiveTSIMInputID==IDD_PIN_CHANGE_DIALOG)
            MOVE_TO_STATE(SECURITYMENU_PINCHANGE)
            else if(pMe->m_pActiveTSIMInputID==IDD_ASK_PASSWORD_DIALOG) 
            MOVE_TO_STATE(SECURITYMENU_ASKPASSWORD) 
            else
            (void)ISHELL_CloseApplet(pMe->m_pShell, FALSE);   
            return NFSMACTION_CONTINUE;

         case DLGRET_CANCELED:
            if(pMe->m_pActiveTSIMInputID==IDD_ASK_CALL_PASSWORD_DIALOG)
            MOVE_TO_STATE(SECURITYMENU_ASKCALLPASSWORD)  
            else if(pMe->m_pActiveTSIMInputID==IDD_PHONE_PASSWORD_INPUT_DIALOG)
            MOVE_TO_STATE(SECURITYMENU_PHONEPASSWORDINPUT)
            else if(pMe->m_pActiveTSIMInputID==IDD_AFFIRM_PASSWORD)
            MOVE_TO_STATE(SECURITYMENU_AFFIRMPASSWORD)
            else if(pMe->m_pActiveTSIMInputID==IDD_ASK_PIN_DIALOG)
            MOVE_TO_STATE(SECURITYMENU_ASKPIN)
            else if(pMe->m_pActiveTSIMInputID==IDD_PIN_CHANGE_DIALOG)
            MOVE_TO_STATE(SECURITYMENU_PINCHANGE)
            else if(pMe->m_pActiveTSIMInputID==IDD_ASK_PASSWORD_DIALOG) 
            MOVE_TO_STATE(SECURITYMENU_ASKPASSWORD) 
            else
            (void)ISHELL_CloseApplet(pMe->m_pShell, FALSE);   
            return NFSMACTION_CONTINUE;
         // add end   
        default:
        break;
    }

    return NFSMACTION_WAIT;
} // TsimPasswordInput

#endif
/*==============================================================================
������
       StateAskPinHandler
˵����
       SECURITYMENU_ASKPIN ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateAskPinHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StateAskPinHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            if(IsRunAsUIMVersion())
            {
                AEECHVStatus  chvst;
                chvst.chv1CountRemain = 0;
                IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
                pMe->m_nCanAttemptTimes = chvst.chv1CountRemain/*MAX_PIN_ATTEMPTS*/;
            }

            SecurityMenu_ShowDialog(pMe, IDD_ASK_PIN_DIALOG);
            return NFSMACTION_WAIT;

            
        case DLGRET_VALIDPINPASS:
//            MOVE_TO_STATE(SECURITYMENU_PINSETTING);
        MOVE_TO_STATE(SECURITYMENU_PINCHANGE);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            if (pMe->m_nCanAttemptTimes > 0)
            {
                MOVE_TO_STATE(SECURITYMENU_ASKPIN)
            }
            else
            {
                pMe->m_eRUIMSCode = ENTERPUK_STEP0;
                MOVE_TO_STATE(SECURITYMENU_ASKPUKPASSWORD)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_VALIDPINFAILED://DLGRET_OK:
            if(IsRunAsUIMVersion())
            {
                if (pMe->m_nCanAttemptTimes > 0)
                {
                    //pMe->m_wMsgID = IDS_INVALIDPIN;
                    Security_ShowMsgBox(pMe, IDS_INVALIDPIN);
                    return NFSMACTION_WAIT; 
                }
                else
                {
                    // PIN �뱻��������PUK��
                    //pMe->m_wMsgID = IDS_PIN_FAILED;
                    Security_ShowMsgBox(pMe, IDS_PIN_FAILED);
                }
                return NFSMACTION_CONTINUE;
            }
/*            else
            {
                Security_ShowMsgBox(pMe, IDS_MSG_INPUTINVALID);
                return NFSMACTION_WAIT;
            }*/

        case DLGRET_ISCOMFIRMPASSWORD:
            //pMe->m_wMsgID = IDS_SAVED;
            Security_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT; 
#ifdef FEATURE_LCD_TOUCH_ENABLE
        case DLGRET_TSIMPASSWORDINPUT:
            MOVE_TO_STATE(SECURITYMENU_TSIMPASSWORDINPUT);          
            return NFSMACTION_CONTINUE;  
#endif            
        case DLGRET_MSGBOX_OK:
            if (pMe->m_bIsConfirmPassword)
            {
                pMe->m_bIsConfirmPassword = FALSE;
                MOVE_TO_STATE(SECURITYMENU_MAIN);
            }
            return NFSMACTION_CONTINUE;
            

        //case DLGRET_OK:
        case DLGRET_CANCELED:
            pMe->m_bIsConfirmPassword = FALSE;
            MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
            return NFSMACTION_CONTINUE;

        default:
        break;
    }

    return NFSMACTION_WAIT;
} // StateAskPinHandler
 // StateAskPinHandler

/*==============================================================================
������
       StateAskCallPasswordHandler
˵����
       SECURITYMENU_ASKCALLPASSWORD ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateAskCallPasswordHandler(CSecurityMenu *pMe)
 {
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_ASK_CALL_PASSWORD_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_VALIDPINPASS:
            MOVE_TO_STATE(SECURITYMENU_PHONEPASSWORDINPUT);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            MOVE_TO_STATE(SECURITYMENU_ASKCALLPASSWORD);          
            return NFSMACTION_CONTINUE;

        case DLGRET_VALIDPINFAILED:
            //pMe->m_wMsgID = IDS_MSG_INPUTINVALID;
            Security_ShowMsgBox(pMe, IDS_MSG_INPUTINVALID);
            return NFSMACTION_WAIT; 
#ifdef FEATURE_LCD_TOUCH_ENABLE
        case DLGRET_TSIMPASSWORDINPUT:
            MOVE_TO_STATE(SECURITYMENU_TSIMPASSWORDINPUT);          
            return NFSMACTION_CONTINUE;   
#endif
        //case DLGRET_OK: 
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}
 // StateAskCallPasswordHandler
 
/*==============================================================================
������
       StateExitHandler
˵����
       SECURITYMENU_EXIT ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateExitHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler


/*==============================================================================
������
       StateAffirmPassWordHandler
˵����
       SECURITYMENU_AFFIRMPASSWORD ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateAffirmPassWordHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StateAffirmPassWordHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_AFFIRM_PASSWORD);
            return NFSMACTION_WAIT;
        case DLGRET_OK://DLGRET_MSGBOX_OK:
            {
            if(IsRunAsUIMVersion())
                {
                    if(pMe->m_IsPin)
                    {
                        MOVE_TO_STATE(SECURITYMENU_PINCHANGE); 
                        return NFSMACTION_CONTINUE;
                    }
                    else
                    {
                        MOVE_TO_STATE(SECURITYMENU_PHONEPASSWORDINPUT); 
                        return NFSMACTION_CONTINUE;
                    }
                }
            else
                {
                    MOVE_TO_STATE(SECURITYMENU_PHONEPASSWORDINPUT); 
                    return NFSMACTION_CONTINUE;
                }
            }

        case DLGRET_MSGBOX_OK://DLGRET_MSGBOX_CANCEL:
            MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
            return NFSMACTION_CONTINUE;
             
        case DLGRET_VALIDPINFAILED:
            //pMe->m_wMsgID = IDS_DIF_PASSWORD;
            Security_ShowMsgBox(pMe, IDS_DIF_PASSWORD);
            return NFSMACTION_WAIT;
        case DLGRET_SETPINPASS:
            // ��ʾ��ʾ��Ϣ
            //pMe->m_wMsgID = IDS_PIN_CHANGED;
            Security_ShowMsgBox(pMe, IDS_PIN_CHANGED);
            return NFSMACTION_WAIT;

        case DLGRET_SETPINFAILED:
            // ��ʾ��ʾ��Ϣ
            //pMe->m_wMsgID = IDS_MODIFYPIN_FAILED;
            Security_ShowMsgBox(pMe, IDS_MODIFYPIN_FAILED);
            return NFSMACTION_WAIT;
#ifdef FEATURE_LCD_TOUCH_ENABLE
        case DLGRET_TSIMPASSWORDINPUT:
            MOVE_TO_STATE(SECURITYMENU_TSIMPASSWORDINPUT);          
            return NFSMACTION_CONTINUE; 
#endif            
        case DLGRET_TOSHOWMSG:
            //pMe->m_wMsgID = IDS_PHONE_PASSWORD_CHANGED;
            Security_ShowMsgBox(pMe, IDS_PHONE_PASSWORD_CHANGED);
            return NFSMACTION_WAIT;

        //case DLGRET_OK: 
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
            return NFSMACTION_CONTINUE;
            /*if(IsRunAsUIMVersion())
            {
                    MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
                if(pMe->m_IsPin)
                {
//                    MOVE_TO_STATE(SECURITYMENU_PINSETTING)
                    MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
                }
                else
                {
                    MOVE_TO_STATE(SECURITYMENU_CHANGECODE)
                }
            }
            else
            {
                MOVE_TO_STATE(SECURITYMENU_PHONELOCK)
            }
            }*/

        default:
        break;
    }

    return NFSMACTION_WAIT;
} // StateGpsHandler

/*==============================================================================
������
       Security_StateAskPUKPassWordHandler
˵����
       SECURITYMENU_ASKPUKPASSWORD ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateAskPUKPassWordHandler(CSecurityMenu * pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_OK:
            {
                AEECHVStatus  chvst;
                
                if (IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst) != SUCCESS)
                {// ��ȡ״̬ʧ��
                    MOVE_TO_STATE(SECURITYMENU_UIMERR)
                }
                else if (chvst.chv1UnblockRemain == 0)
                {
                    // PUK��ʣ���������Ϊ 0������Ч
                    MOVE_TO_STATE(SECURITYMENU_UIMERR)
                }
                else
                {
                    pMe->m_bNotOverwriteDlgRet = FALSE;
                    SecurityMenu_ShowDialog(pMe, IDD_PUK_DIALOG);
                    return NFSMACTION_WAIT;
                }
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_VALIDPINPASS:
            //pMe->m_wMsgID = IDS_PUK_CORRECT;
            Security_ShowMsgBox(pMe, IDS_PUK_CORRECT);
            return NFSMACTION_WAIT; 

        case DLGRET_MSGBOX_OK:
            if (pMe->m_bIsConfirmPassword)
            {
                pMe->m_bIsConfirmPassword = FALSE;
                MOVE_TO_STATE(SECURITYMENU_MAIN);
            }
            else
            {
                MOVE_TO_STATE(SECURITYMENU_CHANGECODE);
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_EMGCALL:
            MOVE_TO_STATE(SECURITYMENU_EMERGENCYCALL)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_VALIDPINFAILED:
            SecurityMenu_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;
            
        default:
            break;
    }
                
    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       Security_StateUIMErrHandler
˵����
       SECURITYMENU_UIMERR ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateUIMErrHandler(CSecurityMenu * pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_UIMERR_DIALOG);
            return NFSMACTION_WAIT;
            
        case DLGRET_EMGCALL:
            MOVE_TO_STATE(SECURITYMENU_EMERGENCYCALL)
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       Security_StateEmergencyCallHandler
˵����
       SECURITYMENU_EMERGENCYCALL ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateEmergencyCallHandler(CSecurityMenu * pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
        
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_EMGCALL_DIALOG);
            return NFSMACTION_WAIT;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(pMe->m_ePreState)
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       StateChangeCode
˵����
       SECURITYMENU_CHANGECODE ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateChangeCodeHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_CHANGE_CODE);
            return NFSMACTION_WAIT;

        case DLG_ASKCALLPASSWORD://DLG_PHONEPASSWORDINPUT:
            MOVE_TO_STATE(SECURITYMENU_ASKCALLPASSWORD)
            return NFSMACTION_CONTINUE;

        case DLG_ASKPIN://DLG_PINCHANGE:
            MOVE_TO_STATE(SECURITYMENU_ASKPIN)//(SECURITYMENU_PINCHANGE)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SECURITYMENU_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}
/*==============================================================================
������
       StateRestoreHandler
˵����
       SECURITYMENU_RESTORE ״̬������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Security_StateRestoreHandler(CSecurityMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //SEC_ERR("StateRestoreHandler", 0, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SecurityMenu_ShowDialog(pMe, IDD_RESTORE_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_RESTORE_MESSAGE:
            //pMe->m_wMsgID = IDS_RESTORING;
            Security_ShowMsgBox(pMe, IDS_RESTORING);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SECURITYMENU_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            MOVE_TO_STATE(SECURITYMENU_MAIN);
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_OK:
            Security_ShowMsgBox(pMe, IDS_RESET_DONE);
            return NFSMACTION_WAIT;
        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}
