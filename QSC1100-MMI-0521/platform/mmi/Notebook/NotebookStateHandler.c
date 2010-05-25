#ifndef _NOTEBOOK_STATE_HANDLER_C_
#define _NOTEBOOK_STATE_HANDLER_C_
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
#include "Notebook_priv.h"

/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ NoOperation ������
static NextFSMAction Notebook_StateNoOperationHandler(CNotebookMenu *pMe);

// ״̬ Init ������
static NextFSMAction Notebook_StateInitHandler(CNotebookMenu *pMe);

// ״̬ Main ������
static NextFSMAction Notebook_StateMainHandler(CNotebookMenu *pMe);

// ״̬ MainOption ������
static NextFSMAction Notebook_StateMainOptionHandler(CNotebookMenu *pMe);

// ״̬ Edit ������
static NextFSMAction Notebook_StateEditHandler(CNotebookMenu *pMe);

// ״̬ View ������
static NextFSMAction Notebook_StateViewHandler(CNotebookMenu *pMe);

// ״̬ EditOption ������
static NextFSMAction Notebook_StateEditOptionHandler(CNotebookMenu *pMe);

// ״̬Exit  ������
static NextFSMAction Notebook_StateExitHandler(CNotebookMenu *pMe);

// ״̬ Message box ������
static NextFSMAction Notebook_StateMsgBoxHandler(CNotebookMenu *pMe);


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
NextFSMAction Notebook_ProcessState(CNotebookMenu *pMe)
{
	NextFSMAction retVal = NFSMACTION_WAIT;

	if (NULL == pMe)
	{
		return retVal;
	}

	switch (pMe->m_eCurState)
	{
        case NOTEBOOK_STATE_NONE:
        		retVal = Notebook_StateNoOperationHandler(pMe);
        		break;

        case NOTEBOOK_STATE_INIT:
        		retVal = Notebook_StateInitHandler(pMe);
        		break;

        case NOTEBOOK_STATE_MAIN:
        		retVal = Notebook_StateMainHandler(pMe);
        		break;	

        case NOTEBOOK_STATE_MAIN_OPTION:
                retVal = Notebook_StateMainOptionHandler(pMe);
                break;
                
        case NOTEBOOK_STATE_EDIT:
                retVal = Notebook_StateEditHandler(pMe);
                break;
                
        case NOTEBOOK_STATE_VIEW:
                retVal = Notebook_StateViewHandler(pMe);
                break;
                
        case NOTEBOOK_STATE_EDIT_OPTION:
        		retVal = Notebook_StateEditOptionHandler(pMe);
        		break;
                
        case NOTEBOOK_STATE_MSGBOX:
        		retVal = Notebook_StateMsgBoxHandler(pMe);
        		break;	

        case NOTEBOOK_STATE_EXIT:
        		retVal = Notebook_StateExitHandler(pMe);
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
static NextFSMAction Notebook_StateNoOperationHandler(CNotebookMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	MOVE_TO_STATE(NOTEBOOK_STATE_EXIT)
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
static NextFSMAction Notebook_StateInitHandler(CNotebookMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	MOVE_TO_STATE(NOTEBOOK_STATE_MAIN)
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
static NextFSMAction Notebook_StateMainHandler(CNotebookMenu *pMe)
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
			Notebook_ShowDialog(pMe,IDD_DIALOG_MAIN);
			return NFSMACTION_WAIT;
        
		case DLGRET_NOTEBOOK_MAIN_OPTION:
			MOVE_TO_STATE(NOTEBOOK_STATE_MAIN_OPTION)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_NOTEBOOK_EDIT:
			MOVE_TO_STATE(NOTEBOOK_STATE_EDIT)
			return NFSMACTION_CONTINUE;

		case DLGRET_NOTEBOOK_EDIT_OPTION:
			MOVE_TO_STATE(NOTEBOOK_STATE_EDIT_OPTION)
			return NFSMACTION_CONTINUE;

		case DLGRET_NOTEBOOK_VIEW:
			MOVE_TO_STATE(NOTEBOOK_STATE_VIEW)
			return NFSMACTION_CONTINUE;

            
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(NOTEBOOK_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;	
            
		case DLGRET_CANCELED:
			MOVE_TO_STATE(NOTEBOOK_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} // StateMainHandler

static NextFSMAction Notebook_StateEditHandler(CNotebookMenu *pMe)
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
			Notebook_ShowDialog(pMe,IDD_DIALOG_EDIT);
			return NFSMACTION_WAIT;

		case DLGRET_NOTEBOOK_MAIN:
			MOVE_TO_STATE(NOTEBOOK_STATE_MAIN)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_NOTEBOOK_MAIN_OPTION:
			MOVE_TO_STATE(NOTEBOOK_STATE_MAIN_OPTION)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_NOTEBOOK_EDIT:
			MOVE_TO_STATE(NOTEBOOK_STATE_EDIT)
			return NFSMACTION_CONTINUE;

		case DLGRET_NOTEBOOK_EDIT_OPTION:
			MOVE_TO_STATE(NOTEBOOK_STATE_EDIT_OPTION)
			return NFSMACTION_CONTINUE;

		case DLGRET_NOTEBOOK_VIEW:
			MOVE_TO_STATE(NOTEBOOK_STATE_VIEW)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(NOTEBOOK_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;	
            
		case DLGRET_CANCELED:
			MOVE_TO_STATE(NOTEBOOK_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} // StateMainHandler

static NextFSMAction Notebook_StateViewHandler(CNotebookMenu *pMe)
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
			Notebook_ShowDialog(pMe,IDD_DIALOG_VIEW);
			return NFSMACTION_WAIT;

		case DLGRET_NOTEBOOK_MAIN:
			MOVE_TO_STATE(NOTEBOOK_STATE_MAIN)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_NOTEBOOK_MAIN_OPTION:
			MOVE_TO_STATE(NOTEBOOK_STATE_MAIN_OPTION)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_NOTEBOOK_EDIT:
			MOVE_TO_STATE(NOTEBOOK_STATE_EDIT)
			return NFSMACTION_CONTINUE;

		case DLGRET_NOTEBOOK_EDIT_OPTION:
			MOVE_TO_STATE(NOTEBOOK_STATE_EDIT_OPTION)
			return NFSMACTION_CONTINUE;

		case DLGRET_NOTEBOOK_VIEW:
			MOVE_TO_STATE(NOTEBOOK_STATE_VIEW)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(NOTEBOOK_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;	
            
		case DLGRET_CANCELED:
			MOVE_TO_STATE(NOTEBOOK_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} // StateMainHandler

/*==============================================================================
������
       Notebook_StateMainOptionHandler
˵����
       NOTEBOOK_MAIN ״̬������

������
       pMe [in]��ָ��Notebook Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Notebook_StateMainOptionHandler(CNotebookMenu *pMe)
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
			Notebook_ShowDialog(pMe,IDD_MAIN_OPTION);
			return NFSMACTION_WAIT;
			
		case DLGRET_NOTEBOOK_MAIN:
			MOVE_TO_STATE(NOTEBOOK_STATE_MAIN)
			return NFSMACTION_CONTINUE;

        case DLGRET_NOTEBOOK_EDIT:
            MOVE_TO_STATE(NOTEBOOK_STATE_EDIT)
            return NFSMACTION_CONTINUE;
            
		case DLGRET_NOTEBOOK_VIEW:
			MOVE_TO_STATE(NOTEBOOK_STATE_VIEW)
			return NFSMACTION_CONTINUE;	
            
        case DLGRET_MSGBOX:
    	    MOVE_TO_STATE(NOTEBOOK_STATE_MSGBOX)
    	    return NFSMACTION_CONTINUE; 

		case DLGRET_CANCELED:
			MOVE_TO_STATE(NOTEBOOK_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} // StateMainHandler


static NextFSMAction Notebook_StateEditOptionHandler(CNotebookMenu *pMe)
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
			Notebook_ShowDialog(pMe,IDD_EDIT_OPTION);
			return NFSMACTION_WAIT;
			
		case DLGRET_NOTEBOOK_MAIN:
			MOVE_TO_STATE(NOTEBOOK_STATE_MAIN)
			return NFSMACTION_CONTINUE;

        case DLGRET_NOTEBOOK_EDIT:
            MOVE_TO_STATE(NOTEBOOK_STATE_EDIT)
            return NFSMACTION_CONTINUE;

        case DLGRET_NOTEBOOK_EDIT_OPTION:
            MOVE_TO_STATE(NOTEBOOK_STATE_EDIT_OPTION)
            return NFSMACTION_CONTINUE;

		case DLGRET_NOTEBOOK_VIEW:
			MOVE_TO_STATE(NOTEBOOK_STATE_VIEW)
			return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX:
    	    MOVE_TO_STATE(NOTEBOOK_STATE_MSGBOX)
    	    return NFSMACTION_CONTINUE; 

		case DLGRET_CANCELED:
			MOVE_TO_STATE(NOTEBOOK_STATE_EXIT)
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
static NextFSMAction Notebook_StateExitHandler(CNotebookMenu *pMe)
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
static NextFSMAction Notebook_StateMsgBoxHandler(CNotebookMenu *pMe)
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
			Notebook_ShowDialog(pMe, IDD_DIALOG_MSGBOX);
			return NFSMACTION_WAIT;
			
		case DLGRET_NOTEBOOK_MAIN:
			MOVE_TO_STATE(NOTEBOOK_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_NOTEBOOK_MAIN_OPTION:
			MOVE_TO_STATE(NOTEBOOK_STATE_MAIN_OPTION)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_NOTEBOOK_EDIT:
			MOVE_TO_STATE(NOTEBOOK_STATE_EDIT)
			return NFSMACTION_CONTINUE;

		case DLGRET_NOTEBOOK_EDIT_OPTION:
			MOVE_TO_STATE(NOTEBOOK_STATE_EDIT_OPTION)
			return NFSMACTION_CONTINUE;

		case DLGRET_NOTEBOOK_VIEW:
			MOVE_TO_STATE(NOTEBOOK_STATE_VIEW)
			return NFSMACTION_CONTINUE;
            
		case DLGRET_CANCELED:
			MOVE_TO_STATE(NOTEBOOK_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			break;
	}

	return NFSMACTION_WAIT;
} 

#endif
