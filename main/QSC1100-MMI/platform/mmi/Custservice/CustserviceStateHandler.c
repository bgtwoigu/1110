#ifndef _CUSTSERVICE_STATE_HANDLER_C_
#define _CUSTSERVICE_STATE_HANDLER_C_
/*******************************************************************
*	Copyright(c)2008-2009 TCL CDMA SW developement Team Reserved
*
********************************************************************/
/********************************************************************

	Team		: 	CDMA JRD SW
	Author		: 	Qiong.chen Create 
	Version 	:	1.0.0
	Function Name 	:
	Change Time 	:
		Qiong.chen Create 2009/02/04
		
*********************************************************************/

/*==============================================================================
								 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "Custservice_priv.h"

/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ MORSE_NONE ������
static NextFSMAction CustService_StateNoOperationHandler(CCustServiceMenu *pMe);

// ״̬ MORSE_INIT ������
static NextFSMAction CustService_StateInitHandler(CCustServiceMenu *pMe);

// ״̬ MORSE_MAIN ������
static NextFSMAction CustService_StateMainHandler(CCustServiceMenu *pMe);

// ״̬ Customer Hotline ������
static NextFSMAction CustService_StateCustHotlineHandler(CCustServiceMenu *pMe);

// ״̬ Mobile E Hall ������
static NextFSMAction CustService_StateMobileEHallHandler(CCustServiceMenu *pMe);

// ״̬ Calling Secretary ������
static NextFSMAction CustService_StateCallSecretaryHandler(CCustServiceMenu *pMe);

// ״̬ My Subscription ������
static NextFSMAction CustService_StateMySubscriptionHandler(CCustServiceMenu *pMe);

// ״̬ Phone Service Guide ������
static NextFSMAction CustService_StatePhoneSerGuideHandler(CCustServiceMenu *pMe);

// ״̬Exit Handler ������
static NextFSMAction CustService_StateExitHandler(CCustServiceMenu *pMe);

// ״̬ MORSE_FULLNAME ������
static NextFSMAction CustService_StateMsgBoxHandler(CCustServiceMenu *pMe);


/*==============================================================================
                                 ��������
==============================================================================*/
/*==============================================================================
����:
       SportSApp_ProcessState
˵��:
       SportSApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��SportSApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction CustService_ProcessState(CCustServiceMenu *pMe)
{
	NextFSMAction retVal = NFSMACTION_WAIT;

	if (NULL == pMe)
	{
		return retVal;
	}

	switch (pMe->m_eCurState)
	{
		case CUSTSERVICE_STATE_NONE:
			retVal = CustService_StateNoOperationHandler(pMe);
			break;

		case CUSTSERVICE_STATE_INIT:
			retVal = CustService_StateInitHandler(pMe);
			break;
        
		case CUSTSERVICE_STATE_MAIN:
			retVal = CustService_StateMainHandler(pMe);
			break;	
            
	    case CUSTSERVICE_STATE_CUSTHOTLINE:
            retVal = CustService_StateCustHotlineHandler(pMe);
            break;
            
        case CUSTSERVICE_STATE_MOBILEHALL:
            retVal = CustService_StateMobileEHallHandler(pMe);
            break;
            
        case CUSTSERVICE_STATE_CALLSECRETARY:
            retVal = CustService_StateCallSecretaryHandler(pMe);
            break;
            
        case CUSTSERVICE_STATE_SUBSCRIPTION:
            retVal = CustService_StateMySubscriptionHandler(pMe);
            break;
            
        case CUSTSERVICE_STATE_PHSERGUIDE:
            retVal = CustService_StatePhoneSerGuideHandler(pMe);
            break;
            
		case CUSTSERVICE_STATE_MSGBOX:
			retVal = CustService_StateMsgBoxHandler(pMe);
			break;	
            
		case CUSTSERVICE_STATE_EXIT:
			retVal = CustService_StateExitHandler(pMe);
			break;
            
		default:
			break;
	}
    return retVal;
}

/*==============================================================================
������
       SportSApp_StateNoOperationHandler
˵����
       SPORT_STATE_NONE ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction CustService_StateNoOperationHandler(CCustServiceMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	MOVE_TO_STATE(CUSTSERVICE_STATE_EXIT)
	return NFSMACTION_CONTINUE;
}  

/*==============================================================================
������
       SportSApp_StateInitHandler
˵����
       SPORT_STATE_INIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction CustService_StateInitHandler(CCustServiceMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	MOVE_TO_STATE(CUSTSERVICE_STATE_MAIN)
	return NFSMACTION_CONTINUE;
} 

/*==============================================================================
������
       SportSApp_StateMainHandler
˵����
       SPORTSAPP_MAIN ״̬������

������
       pMe [in]��ָ��SportSapp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction CustService_StateMainHandler(CCustServiceMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ; 

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			CustService_ShowDialog(pMe,IDD_DIALOG_MAIN);//test by Joe 090204
			return NFSMACTION_WAIT;
        
		case DLGRET_CUSTSERVICE_CUSTHOTLINE:
			MOVE_TO_STATE(CUSTSERVICE_STATE_CUSTHOTLINE)
			return NFSMACTION_CONTINUE;

        case DLGRET_CUSTSERVICE_MOBILEHALL:
            MOVE_TO_STATE(CUSTSERVICE_STATE_MOBILEHALL)
            return NFSMACTION_CONTINUE;

        case DLGRET_CUSTSERVICE_CALLSECRETARY:
            MOVE_TO_STATE(CUSTSERVICE_STATE_CALLSECRETARY)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CUSTSERVICE_SUBSCRIPTION:
            MOVE_TO_STATE(CUSTSERVICE_STATE_SUBSCRIPTION)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CUSTSERVICE_PHSERGUIDE:
            MOVE_TO_STATE(CUSTSERVICE_STATE_PHSERGUIDE)
            return NFSMACTION_CONTINUE;
            
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(CUSTSERVICE_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;	
            
		case DLGRET_CANCELED:
			MOVE_TO_STATE(CUSTSERVICE_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} // StateMainHandler

/*==============================================================================
������
       SportSApp_StateMainHandler
˵����
       SPORTSAPP_MAIN ״̬������

������
       pMe [in]��ָ��SportSapp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction CustService_StateCustHotlineHandler(CCustServiceMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ; 

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			CustService_ShowDialog(pMe,IDD_DIALOG_HOTLINE);
			return NFSMACTION_WAIT;
			
		case DLGRET_CUSTSERVICE_MAIN:
			MOVE_TO_STATE(CUSTSERVICE_STATE_MAIN)
			return NFSMACTION_CONTINUE;
	
        case DLGRET_MSGBOX:
                MOVE_TO_STATE(CUSTSERVICE_STATE_MSGBOX)
                return NFSMACTION_CONTINUE; 

		case DLGRET_CANCELED:
			MOVE_TO_STATE(CUSTSERVICE_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} // StateMainHandler

/*==============================================================================
������
       Morse_StateFullNameHandler
˵����
       SPORTSAPP_MAIN ״̬������

������
       pMe [in]��ָ��SportSapp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction CustService_StateMobileEHallHandler(CCustServiceMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0; 

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			CustService_ShowDialog(pMe,IDD_DIALOG_MOBILEHALL);
			return NFSMACTION_WAIT;
			
		case DLGRET_CUSTSERVICE_MAIN:
			MOVE_TO_STATE(CUSTSERVICE_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_CUSTSERVICE_MOBILEHALL:
			MOVE_TO_STATE(CUSTSERVICE_STATE_MOBILEHALL)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(CUSTSERVICE_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;	
            
		case DLGRET_CANCELED:
			MOVE_TO_STATE(CUSTSERVICE_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} // StateMainHandler

/*==============================================================================
������
       Morse_StateFullNameHandler
˵����
       SPORTSAPP_MAIN ״̬������

������
       pMe [in]��ָ��SportSapp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction CustService_StateCallSecretaryHandler(CCustServiceMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    pMe->m_nSubDlgId = 0; 

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CustService_ShowDialog(pMe,IDD_DIALOG_CALLSECRETARY);
            return NFSMACTION_WAIT;
            
        case DLGRET_CUSTSERVICE_MAIN:
            MOVE_TO_STATE(CUSTSERVICE_STATE_MAIN)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CUSTSERVICE_CALLSECRETARY:
            MOVE_TO_STATE(CUSTSERVICE_STATE_CALLSECRETARY)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX:
            MOVE_TO_STATE(CUSTSERVICE_STATE_MSGBOX)
            return NFSMACTION_CONTINUE; 
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(CUSTSERVICE_STATE_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       Morse_StateFullNameHandler
˵����
       SPORTSAPP_MAIN ״̬������

������
       pMe [in]��ָ��SportSapp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction CustService_StateMySubscriptionHandler(CCustServiceMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    pMe->m_nSubDlgId = 0; 

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CustService_ShowDialog(pMe,IDD_DIALOG_MYSUBSCRIPTION);
            return NFSMACTION_WAIT;
            
        case DLGRET_CUSTSERVICE_MAIN:
            MOVE_TO_STATE(CUSTSERVICE_STATE_MAIN)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CUSTSERVICE_SUBSCRIPTION:
            MOVE_TO_STATE(CUSTSERVICE_STATE_SUBSCRIPTION)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX:
            MOVE_TO_STATE(CUSTSERVICE_STATE_MSGBOX)
            return NFSMACTION_CONTINUE; 
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(CUSTSERVICE_STATE_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}


static NextFSMAction CustService_StatePhoneSerGuideHandler(CCustServiceMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    pMe->m_nSubDlgId = 0; 

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CustService_ShowDialog(pMe,IDD_DIALOG_PHSERGUIDE);
            return NFSMACTION_WAIT;
            
        case DLGRET_CUSTSERVICE_MAIN:
            MOVE_TO_STATE(CUSTSERVICE_STATE_MAIN)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CUSTSERVICE_PHSERGUIDE:
            MOVE_TO_STATE(CUSTSERVICE_STATE_PHSERGUIDE)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX:
            MOVE_TO_STATE(CUSTSERVICE_STATE_MSGBOX)
            return NFSMACTION_CONTINUE; 
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(CUSTSERVICE_STATE_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}



/*==============================================================================
������
       SportSApp_StateExitHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction CustService_StateExitHandler(CCustServiceMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} 


/*==============================================================================
������
       SportSApp_StateMainHandler
˵����
       SPORTSAPP_MAIN ״̬������

������
       pMe [in]��ָ��SportSapp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction CustService_StateMsgBoxHandler(CCustServiceMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ; 

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			CustService_ShowDialog(pMe, IDD_DIALOG_MSGBOX);
			return NFSMACTION_WAIT;
			
		case DLGRET_CUSTSERVICE_MAIN:
			MOVE_TO_STATE(CUSTSERVICE_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_CUSTSERVICE_CUSTHOTLINE:
			MOVE_TO_STATE(CUSTSERVICE_STATE_CUSTHOTLINE)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_CUSTSERVICE_MOBILEHALL:
			MOVE_TO_STATE(CUSTSERVICE_STATE_MOBILEHALL)
			return NFSMACTION_CONTINUE;

 		case DLGRET_CUSTSERVICE_CALLSECRETARY:
			MOVE_TO_STATE(CUSTSERVICE_STATE_CALLSECRETARY)
			return NFSMACTION_CONTINUE;
            
        case DLGRET_CUSTSERVICE_SUBSCRIPTION:
            MOVE_TO_STATE(CUSTSERVICE_STATE_SUBSCRIPTION)
            return NFSMACTION_CONTINUE;

        case DLGRET_CUSTSERVICE_PHSERGUIDE:
            MOVE_TO_STATE(CUSTSERVICE_STATE_PHSERGUIDE)
            return NFSMACTION_CONTINUE;
            
		case DLGRET_CANCELED:
			MOVE_TO_STATE(CUSTSERVICE_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} 

#endif
