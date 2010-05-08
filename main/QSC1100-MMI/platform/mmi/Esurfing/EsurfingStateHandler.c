#ifndef _ESURFING_STATE_HANDLER_C_
#define _ESURFING_STATE_HANDLER_C_
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
#include "Esurfing_priv.h"

/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ MORSE_NONE ������
static NextFSMAction Esurfing_StateNoOperationHandler(CEsurfingMenu *pMe);

// ״̬ MORSE_INIT ������
static NextFSMAction Esurfing_StateInitHandler(CEsurfingMenu *pMe);

// ״̬ MORSE_MAIN ������
static NextFSMAction Esurfing_StateMainHandler(CEsurfingMenu *pMe);

// ״̬ Customer Hotline ������
static NextFSMAction Esurfing_StateBestToneHandler(CEsurfingMenu *pMe);


// ״̬Exit Handler ������
static NextFSMAction Esurfing_StateExitHandler(CEsurfingMenu *pMe);

// ״̬ MORSE_FULLNAME ������
static NextFSMAction Esurfing_StateMsgBoxHandler(CEsurfingMenu *pMe);


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
NextFSMAction Esurfing_ProcessState(CEsurfingMenu *pMe)
{
	NextFSMAction retVal = NFSMACTION_WAIT;

	if (NULL == pMe)
	{
		return retVal;
	}

	switch (pMe->m_eCurState)
	{
			case ESURFING_STATE_NONE:
					retVal = Esurfing_StateNoOperationHandler(pMe);
					break;

			case ESURFING_STATE_INIT:
					retVal = Esurfing_StateInitHandler(pMe);
					break;
        
			case ESURFING_STATE_MAIN:
					retVal = Esurfing_StateMainHandler(pMe);
					break;	
            
	    case ESURFING_STATE_BESTTONE:
	        retVal = Esurfing_StateBestToneHandler(pMe);
	        break;
	              
			case ESURFING_STATE_MSGBOX:
					retVal = Esurfing_StateMsgBoxHandler(pMe);
					break;	
            
			case ESURFING_STATE_EXIT:
					retVal = Esurfing_StateExitHandler(pMe);
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
static NextFSMAction Esurfing_StateNoOperationHandler(CEsurfingMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	MOVE_TO_STATE(ESURFING_STATE_EXIT)
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
static NextFSMAction Esurfing_StateInitHandler(CEsurfingMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	MOVE_TO_STATE(ESURFING_STATE_MAIN)
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
static NextFSMAction Esurfing_StateMainHandler(CEsurfingMenu *pMe)
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
			Esurfing_ShowDialog(pMe,IDD_DIALOG_MAIN);
			return NFSMACTION_WAIT;
        
		case DLGRET_ESURFING_BESTTONE:
			MOVE_TO_STATE(ESURFING_STATE_BESTTONE)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(ESURFING_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;	
            
		case DLGRET_CANCELED:
			MOVE_TO_STATE(ESURFING_STATE_EXIT)
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
static NextFSMAction Esurfing_StateBestToneHandler(CEsurfingMenu *pMe)
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
			Esurfing_ShowDialog(pMe,IDD_DIALOG_BESTTONE);
			return NFSMACTION_WAIT;
			
		case DLGRET_ESURFING_MAIN:
			MOVE_TO_STATE(ESURFING_STATE_MAIN)
			return NFSMACTION_CONTINUE;
	
    case DLGRET_MSGBOX:
	    MOVE_TO_STATE(ESURFING_STATE_MSGBOX)
	    return NFSMACTION_CONTINUE; 

		case DLGRET_CANCELED:
			MOVE_TO_STATE(ESURFING_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} // StateMainHandler


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
static NextFSMAction Esurfing_StateExitHandler(CEsurfingMenu *pMe)
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
static NextFSMAction Esurfing_StateMsgBoxHandler(CEsurfingMenu *pMe)
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
			Esurfing_ShowDialog(pMe, IDD_DIALOG_MSGBOX);
			return NFSMACTION_WAIT;
			
		case DLGRET_ESURFING_MAIN:
			MOVE_TO_STATE(ESURFING_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_ESURFING_BESTTONE:
			MOVE_TO_STATE(ESURFING_STATE_BESTTONE)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_CANCELED:
			MOVE_TO_STATE(ESURFING_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} 

#endif
