/*==============================================================================
// �ļ���
//        BTAppStateHandler.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//
// �ļ�˵����
//
// ���ߣ�zzg
// �������ڣ�2011-01-04
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
#include "BTApp_priv.h"		//"BTApp.h"


/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ BTAPPST_NONE ������
static NextFSMAction BTApp_StateNoOperationHandler(CBTApp *pMe);

// ״̬ BTAPPST_INIT ������
static NextFSMAction BTApp_StateInitHandler(CBTApp *pMe);

// ״̬ BTAPPST_MAIN ������
static NextFSMAction BTApp_StateMainHandler(CBTApp *pMe);

// ״̬ BTAPPST_DEVICESRH ������
static NextFSMAction BTApp_StateDeviceSearchHandler(CBTApp *pMe);

// ״̬ BTAPPST_SRHRESULT ������
static NextFSMAction BTApp_StateSearchResultHandler(CBTApp *pMe);
// ״̬ BTAPPST_DEVICEINFO ������
static NextFSMAction BTApp_StateDeviceInfoHandler(CBTApp *pMe);
// ״̬ BTAPPST_DEVICEINFO_OPITION ������
static NextFSMAction BTApp_StateDeviceInfoOpitionHandler(CBTApp *pMe);
// ״̬ BTAPPST_BOND_OPITION ������
static NextFSMAction BTApp_StateBondOpitionHandler(CBTApp *pMe);




// ״̬ BTAPPST_DEVICE_LIST ������
static NextFSMAction BTApp_StateDeviceListHandler(CBTApp *pMe);

// ״̬ BTAPPST_BOND_LIST ������
static NextFSMAction BTApp_StateBondListHandler(CBTApp *pMe);

// ״̬ BTAPPST_ALL_LIST ������
static NextFSMAction BTApp_StateAllListHandler(CBTApp *pMe);

// ״̬ BTAPPST_CONNECT_LIST ������
static NextFSMAction BTApp_StateConnectListHandler(CBTApp *pMe);



// ״̬ BTAPPST_MYINFO ������
static NextFSMAction BTApp_StateMyInfoHandler(CBTApp *pMe);
// ״̬ BTAPPST_MYINFO_OPITION ������
static NextFSMAction BTApp_StateMyInfoOpitionHandler(CBTApp *pMe);
// ״̬ BTAPPST_EDIT_NAME ������
static NextFSMAction BTApp_StateEditNameHandler(CBTApp *pMe);
// ״̬ BTAPPST_EDIT_SHORTNAME ������
static NextFSMAction BTApp_StateEditShortNameHandler(CBTApp *pMe);
// ״̬ BTAPPST_EDIT_MANUDATA ������
static NextFSMAction BTApp_StateEditManuDataHandler(CBTApp *pMe);
// ״̬ BTAPPST_SET_SECURITY ������
static NextFSMAction BTApp_StateSetSecurityHandler(CBTApp *pMe);
// ״̬ BTAPPST_SET_DISCOVERABLE ������
static NextFSMAction BTApp_StateSetDiscoverableHandler(CBTApp *pMe);
// ״̬ BTAPPST_SET_IOCAPABILITY ������
static NextFSMAction BTApp_StateSetIOCapabilityHandler(CBTApp *pMe);
// ״̬ BTAPPST_SET_DEBUGKEY ������
static NextFSMAction BTApp_StateSetDebugKeyHandler(CBTApp *pMe);
// ״̬ BTAPPST_BT_MSGBOX ������
static NextFSMAction BTApp_StateMsgBoxHandler(CBTApp *pMe);
// ״̬ BTAPPST_PROMPT ������
static NextFSMAction BTApp_StatePromptHandler(CBTApp *pMe);
// ״̬ BTAPPST_BT_EDIT ������
static NextFSMAction BTApp_StateEditHandler(CBTApp *pMe);
// ״̬ BTAPPST_BT_SEND_FILE ������
static NextFSMAction BTApp_StateSendFileHandler(CBTApp *pMe);
// ״̬ BTAPPST_BT_OBEX_LIST_SERVERS ������
static NextFSMAction BTApp_StateObexListServersHandler(CBTApp *pMe);
// ״̬ BTAPPST_BT_FILE_PROGRESS ������
static NextFSMAction BTApp_StateFileProgressHandler(CBTApp *pMe);



// ״̬ BTAPPST_EXIT ������
static NextFSMAction BTApp_StateExitHandler(CBTApp *pMe);


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
       BTApp_ProcessState
˵��:
       BTApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��BTApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction BTApp_ProcessState(CBTApp *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    MSG_FATAL("***zzg BTApp_ProcessState Start pMe->m_eCurState=%d***", pMe->m_eCurState,0,0);
	
    if (NULL == pMe)
    {
    	MSG_FATAL("***zzg BTApp_ProcessState NULL == pMe***", 0,0,0);
        return retVal;
    }
	
    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case BTAPPST_NONE:
			MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_NONE***", 0,0,0);
            retVal = BTApp_StateNoOperationHandler(pMe);
            break;

        case BTAPPST_INIT:
			MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_INIT***", 0,0,0);
            retVal = BTApp_StateInitHandler(pMe);
            break;

        case BTAPPST_MAIN:
            MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_MAIN***", 0,0,0);
            retVal = BTApp_StateMainHandler(pMe);
            break;
			
        case BTAPPST_DEVICESRH:
            retVal = BTApp_StateDeviceSearchHandler(pMe);
            break;

        case BTAPPST_SRHRESULT:
			MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_SRHRESULT***", 0,0,0);
            retVal = BTApp_StateSearchResultHandler(pMe);
            break;

		case BTAPPST_DEVICEINFO:
			MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_DEVICEINFO***", 0,0,0);
            retVal = BTApp_StateDeviceInfoHandler(pMe);
            break;
			
		case BTAPPST_DEVICEINFO_OPITION:
			MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_DEVICEINFO_OPITION***", 0,0,0);
            retVal = BTApp_StateDeviceInfoOpitionHandler(pMe);
            break;	

		case BTAPPST_BOND_OPITION:
			MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_BOND_OPITION***", 0,0,0);
            retVal = BTApp_StateBondOpitionHandler(pMe);
            break;	
			
		case BTAPPST_DEVICE_LIST:
            retVal = BTApp_StateDeviceListHandler(pMe);
            break;
			
        case BTAPPST_BOND_LIST:
            retVal = BTApp_StateBondListHandler(pMe);
            break;

        case BTAPPST_ALL_LIST:
            retVal = BTApp_StateAllListHandler(pMe);
            break;

        case BTAPPST_CONNECT_LIST:
            retVal = BTApp_StateConnectListHandler(pMe);
            break;
			
        case BTAPPST_MYINFO:
			MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_MYINFO***", 0, 0, 0);
            retVal = BTApp_StateMyInfoHandler(pMe);
            break;
			
		case BTAPPST_MYINFO_OPITION:
			MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_MYINFO_OPITION***", 0, 0, 0);
            retVal = BTApp_StateMyInfoOpitionHandler(pMe);
            break;

        case BTAPPST_EDIT_NAME:
            retVal = BTApp_StateEditNameHandler(pMe);
            break;

        case BTAPPST_EDIT_SHORTNAME:
            retVal = BTApp_StateEditShortNameHandler(pMe);
            break;

        case BTAPPST_EDIT_MANUDATA:
            retVal = BTApp_StateEditManuDataHandler(pMe);
            break;

		case BTAPPST_SET_SECURITY:
            retVal = BTApp_StateSetSecurityHandler(pMe);
            break;
			
        case BTAPPST_SET_DISCOVERABLE:
            retVal = BTApp_StateSetDiscoverableHandler(pMe);
            break;

        case BTAPPST_SET_IOCAPABILITY:
            retVal = BTApp_StateSetIOCapabilityHandler(pMe);
            break;

        case BTAPPST_SET_DEBUGKEY:
            retVal = BTApp_StateSetDebugKeyHandler(pMe);
            break;

		case BTAPPST_BT_MSGBOX:
            retVal = BTApp_StateMsgBoxHandler(pMe);
            break;	

        case BTAPPST_PROMPT:
			MSG_FATAL("***zzg BTApp_ProcessState BTAPPST_PROMPT***", 0, 0, 0);
            retVal = BTApp_StatePromptHandler(pMe);
            break;
			
		case BTAPPST_BT_EDIT:
            retVal = BTApp_StateEditHandler(pMe);
            break;

		case BTAPPST_BT_SEND_FILE:
            retVal = BTApp_StateSendFileHandler(pMe);
            break;	

		case BTAPPST_BT_OBEX_LIST_SERVERS:
			MSG_FATAL("***zzg ProcessState BTAPPST_BT_OBEX_LIST_SERVERS***", 0, 0, 0);	
            retVal = BTApp_StateObexListServersHandler(pMe);
            break;	
			
		case BTAPPST_BT_FILE_PROGRESS:
			MSG_FATAL("***zzg ProcessState BTAPPST_BT_FILE_PROGRESS***", 0, 0, 0);	
            retVal = BTApp_StateFileProgressHandler(pMe);
            break;
			
		case BTAPPST_EXIT:
			retVal = BTApp_StateExitHandler(pMe);
            break;

        default:
            ASSERT_NOT_REACHABLE;
    }
    MSG_FATAL("curstate %d prestate %d dlgret %d BTApp_ProcessState",pMe->m_eCurState,pMe->m_ePreState,pMe->m_eDlgRet);
    return retVal;
}


/*==============================================================================
������
       StateNoOperationHandler
˵����
       BTAPPST_NONE ״̬������

������
       pMe [in]��ָ��BTApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction BTApp_StateNoOperationHandler(CBTApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

	MSG_FATAL("***zzg BTApp_StateNoOperationHandler***", 0, 0, 0);

    MOVE_TO_STATE(BTAPPST_EXIT)
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler


/*==============================================================================
������
       StateInitHandler
˵����
       BTAPPST_INIT ״̬������

������
       pMe [in]��ָ��BTApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction BTApp_StateInitHandler(CBTApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
	
    MOVE_TO_STATE(BTAPPST_MAIN)
    return NFSMACTION_CONTINUE;
} // StateInitHandler


/*==============================================================================
������
       StateMainHandler
˵����
       BTAPPST_MAIN ״̬������

������
       pMe [in]��ָ��BTApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction BTApp_StateMainHandler(CBTApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    pMe->m_nSubDlgId = 0 ;    //���¼����Ӳ˵�ѡ������λ��

	MSG_FATAL("***zzg BTApp_StateMainHandler m_eDlgRet=%d***", pMe->m_eDlgRet, 0, 0);

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            BTApp_ShowDialog(pMe, IDD_BT_MAINMENU);
            return NFSMACTION_WAIT;

		case DLGRET_DEVICESRH:
			MSG_FATAL("***zzg BTApp_StateMainHandler DLGRET_DEVICESRH***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_DEVICESRH)
            return NFSMACTION_CONTINUE;
			
		case DLGRET_DEVICE_LIST:
			MOVE_TO_STATE(BTAPPST_DEVICE_LIST)
            return NFSMACTION_CONTINUE;	

		case DLGRET_MYINFO:
			MSG_FATAL("***zzg BTApp_StateMainHandler DLGRET_MYINFO***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_MYINFO)
            return NFSMACTION_CONTINUE;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateMainHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;			
			
		case DLGRET_PROMPT:
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
			
        case DLGRET_CANCELED:
            MOVE_TO_STATE(BTAPPST_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateMainHandler


// ״̬ BTAPPST_DEVICESRH ������
static NextFSMAction BTApp_StateDeviceSearchHandler(CBTApp *pMe)
{
	if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
		{
			MSG_FATAL("***zzg IDD_DEVICE_SEARCH DLGRET_CREATE***", 0, 0, 0);
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_DEVICE_SEARCH);            
			 return NFSMACTION_WAIT;
        }
		
		case DLGRET_SRHRESULT:
			MOVE_TO_STATE(BTAPPST_SRHRESULT)
            return NFSMACTION_CONTINUE;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateDeviceSearchHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateDeviceSearchHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateDeviceSearchHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;
			
        case DLGRET_CANCELED:
			
			if ( pMe->mSD.bDiscovering )
			{
				IBTEXTSD_StopDeviceDiscovery( pMe->mSD.po );
			}
			
			BTApp_CancelDevNameRequest( pMe );

            MOVE_TO_STATE(BTAPPST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_SRHRESULT ������
static NextFSMAction BTApp_StateSearchResultHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			MSG_FATAL("***zzg IDD_SEARCH_RESULT DLGRET_CREATE***", 0, 0, 0);
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_SEARCH_RESULT);
			return NFSMACTION_WAIT;
			
		case DLGRET_DEVICEINFO:
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateSearchResultHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateSearchResultHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateSearchResultHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MAIN)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

// ״̬ BTAPPST_DEVICEINFO ������
static NextFSMAction BTApp_StateDeviceInfoHandler(CBTApp * pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			MSG_FATAL("***zzg IDD_BT_DEVICE_INFO DLGRET_CREATE***", 0, 0, 0);
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_DEVICE_INFO);
			return NFSMACTION_WAIT;

		case DLGRET_DEVICEINFO_OPITION:
			MOVE_TO_STATE(BTAPPST_DEVICEINFO_OPITION)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateDeviceInfoHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateDeviceInfoHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateDeviceInfoHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;
						
		case DLGRET_CANCELED:
			if ((pMe->m_dialog_id == IDD_ALL_LIST)
				|| (pMe->m_dialog_id == IDD_BOND_LIST)
				|| (pMe->m_dialog_id == IDD_CONNECT_LIST))
			{
				MOVE_TO_STATE(BTAPPST_DEVICE_LIST)
			}
			else
			{
				MOVE_TO_STATE(BTAPPST_SRHRESULT)
			}
			
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_DEVICEINFO_OPITION ������
static NextFSMAction BTApp_StateDeviceInfoOpitionHandler(CBTApp * pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			MSG_FATAL("***zzg IDD_BT_DEVICE_INFO_OPITION DLGRET_CREATE***", 0, 0, 0);
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_DEVICE_INFO_OPITION);
			return NFSMACTION_WAIT;

		case DLGRET_BOND_OPITION:
			MOVE_TO_STATE(BTAPPST_BOND_OPITION)
			return NFSMACTION_CONTINUE;		

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateDeviceInfoOpitionHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateDeviceInfoOpitionHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateDeviceInfoOpitionHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;
			
		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

// ״̬ BTAPPST_BOND_OPITION ������
static NextFSMAction BTApp_StateBondOpitionHandler(CBTApp * pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			MSG_FATAL("***zzg IDD_BT_BOND_OPITION DLGRET_CREATE***", 0, 0, 0);
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_BOND_OPITION);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateBondOpitionHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateBondOpitionHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateBondOpitionHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;
						
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}






// ״̬ BTAPPST_DEVICE_LIST ������
static NextFSMAction BTApp_StateDeviceListHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_DEVICE_LIST);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BOND_LIST:
			MOVE_TO_STATE(BTAPPST_BOND_LIST)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_ALL_LIST:
			MOVE_TO_STATE(BTAPPST_ALL_LIST)
			return NFSMACTION_CONTINUE; 

		case DLGRET_CONNECT_LIST:
			MOVE_TO_STATE(BTAPPST_CONNECT_LIST)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateDeviceListHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateDeviceListHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateDeviceListHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MAIN)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_BOND_LIST ������
static NextFSMAction BTApp_StateBondListHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BOND_LIST);
			return NFSMACTION_WAIT;	

		case DLGRET_DEVICEINFO:
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateBondListHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateBondListHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateBondListHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_DEVICE_LIST)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_ALL_LIST ������
static NextFSMAction BTApp_StateAllListHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_ALL_LIST);
			return NFSMACTION_WAIT;

		case DLGRET_DEVICEINFO:
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateAllListHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateAllListHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateAllListHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;	
		
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_DEVICE_LIST)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_CONNECT_LIST ������
static NextFSMAction BTApp_StateConnectListHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_CONNECT_LIST);
			return NFSMACTION_WAIT;	

		case DLGRET_DEVICEINFO:
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateConnectListHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateConnectListHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateConnectListHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_DEVICE_LIST)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}




// ״̬ BTAPPST_MYINFO ������
static NextFSMAction BTApp_StateMyInfoHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}
	
	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			MSG_FATAL("***zzg BTApp_StateMyInfoHandler DLGRET_CREATE***", 0, 0, 0);
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_MYINFO);
			return NFSMACTION_WAIT;

		case DLGRET_MYINFO_OPITION:			
			MOVE_TO_STATE(BTAPPST_MYINFO_OPITION)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateMyInfoHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateMyInfoHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateMyInfoHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
		
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MAIN)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_MYINFO_OPITION ������
static NextFSMAction BTApp_StateMyInfoOpitionHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}
	
	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			MSG_FATAL("***zzg BTApp_StateMyInfoOpitionHandler DLGRET_CREATE***", 0, 0, 0);
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_MYINFO_OPITION);
			return NFSMACTION_WAIT;
			
		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_EDIT_NAME:
			MOVE_TO_STATE(BTAPPST_EDIT_NAME)
			return NFSMACTION_CONTINUE;
		
		case DLGRET_EDIT_SHORTNAME:
			MOVE_TO_STATE(BTAPPST_EDIT_SHORTNAME)
			return NFSMACTION_CONTINUE;	

		case DLGRET_EDIT_MANUDATA:
			MOVE_TO_STATE(BTAPPST_EDIT_MANUDATA)
			return NFSMACTION_CONTINUE;

		case DLGRET_SET_SECURITY:
			MOVE_TO_STATE(BTAPPST_SET_SECURITY)
			return NFSMACTION_CONTINUE;	

		case DLGRET_SET_DISCOVERABLE:
			MOVE_TO_STATE(BTAPPST_SET_DISCOVERABLE)
			return NFSMACTION_CONTINUE;

		case DLGRET_SET_IOCAPABILITY:
			MOVE_TO_STATE(BTAPPST_SET_IOCAPABILITY)
			return NFSMACTION_CONTINUE;

		case DLGRET_SET_DEBUGKEY:
			MOVE_TO_STATE(BTAPPST_SET_DEBUGKEY)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateMyInfoOpitionHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateMyInfoOpitionHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateMyInfoOpitionHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
					
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_EDIT_NAME ������
static NextFSMAction BTApp_StateEditNameHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}
	
	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_EDITNAME);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateEditNameHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateEditNameHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateEditNameHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		

		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;
			
		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_EDIT_SHORTNAME ������
static NextFSMAction BTApp_StateEditShortNameHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}
	
	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_EDIT_SHORTNAME);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateEditShortNameHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateEditShortNameHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateEditShortNameHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
		
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_EDIT_MANUDATA ������
static NextFSMAction BTApp_StateEditManuDataHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}
	
	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_EDIT_MANU_DATA);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateEditManuDataHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateEditManuDataHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateEditManuDataHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

// ״̬ BTAPPST_SET_SECURITY ������
static NextFSMAction BTApp_StateSetSecurityHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}
	
	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_SECURITY);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateSetSecurityHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateSetSecurityHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateSetSecurityHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}



// ״̬ BTAPPST_SET_DISCOVERABLE ������
static NextFSMAction BTApp_StateSetDiscoverableHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_DISCOVERABLE);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateSetDiscoverableHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateSetDiscoverableHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateSetDiscoverableHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		

		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

// ״̬ BTAPPST_SET_IOCAPABILITY ������
static NextFSMAction BTApp_StateSetIOCapabilityHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_IO_CAPABILITY);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	

		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateSetIOCapabilityHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateSetIOCapabilityHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateSetIOCapabilityHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;			
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_SET_DEBUGKEY ������
static NextFSMAction BTApp_StateSetDebugKeyHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_DEBUG_KEY);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_BT_MSGBOX:
			MSG_FATAL("***zzg BTApp_StateSetDebugKeyHandler DLGRET_BT_MSGBOX***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_EDIT:
			MSG_FATAL("***zzg BTApp_StateSetDebugKeyHandler DLGRET_BT_EDIT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:
			MSG_FATAL("***zzg BTApp_StateSetDebugKeyHandler DLGRET_PROMPT***", 0, 0, 0);
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

// ״̬ BTAPPST_BT_MSGBOX ������
static NextFSMAction BTApp_StateMsgBoxHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	MSG_FATAL("***zzg BTApp_StateMsgBoxHandler m_eDlgRet=%d***", pMe->m_eDlgRet, 0, 0);

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;
			
			
		case DLGRET_CANCELED:		
			MSG_FATAL("***zzg BTApp_StateMsgBoxHandler DLGRET_CANCELED m_msg_id=%d***", pMe->m_msg_id, 0, 0);	
	
			if ((pMe->m_prompt_state_id == BTAPPST_DEVICEINFO)
				|| (pMe->m_prompt_state_id == BTAPPST_DEVICE_LIST))
			{
				pMe->m_msg_state_id = pMe->m_prompt_state_id;
			}				

			MOVE_TO_STATE(pMe->m_msg_state_id)	
			return NFSMACTION_CONTINUE;

			
			/*	
			switch(pMe->m_msg_id)
			{				
				case IDS_MSG_OOB_DATA_CREATED:
				{
					MOVE_TO_STATE(BTAPPST_MYINFO)	
					return NFSMACTION_CONTINUE;;
				}		
				
				case IDS_MSG_DEV_ADDED:
				case IDS_MSG_DEV_ADD_FAILED:
									
				case IDS_MSG_BONDED:
				case IDS_MSG_BOND_FAILED:	
					
				//case IDS_MSG_DEV_REM:
				//case IDS_MSG_DEV_REM_FAILED:
				{
					MOVE_TO_STATE(BTAPPST_DEVICEINFO)
					return NFSMACTION_CONTINUE;
				}
				
				case IDS_MSG_NO_DEV:
				case IDS_MSG_NO_BONDED_DEV:
				case IDS_MSG_NO_CONNECTED_DEV:	
				case IDS_MSG_DEV_UNBONDED:	
				//case IDS_MSG_DEV_REM:
				{	
					MOVE_TO_STATE(BTAPPST_DEVICE_LIST)
					return NFSMACTION_CONTINUE;
				}

				case IDS_MSG_DEV_REM:
				case IDS_MSG_DEV_REM_FAILED:				
				{
					MOVE_TO_STATE(BTAPPST_DEVICE_LIST)	
					return NFSMACTION_CONTINUE;;
				}				

				case IDS_BT_CLOSED:
				{
					MOVE_TO_STATE(BTAPPST_MAIN)	
					return NFSMACTION_CONTINUE;;
				}
					
				default:
				{
					MOVE_TO_STATE(BTAPPST_MAIN)	
					return NFSMACTION_CONTINUE;;
				}
			}			
			return NFSMACTION_CONTINUE;
			*/
		case DLGRET_PROMPT:			
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
			
		case DLGRET_BT_EDIT:
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;			
			
		case DLGRET_BT_MSGBOX:			
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;	
			
		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}



// ״̬ BTAPPST_PROMPT ������
static NextFSMAction BTApp_StatePromptHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	MSG_FATAL("***zzg StatePromptHandler m_eDlgRet=%d***", pMe->m_eDlgRet, 0, 0);

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			MSG_FATAL("***zzg StatePromptHandler DLGRET_CREATE***", 0, 0, 0);
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);
			return NFSMACTION_WAIT;		

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_CANCELED:
			MSG_FATAL("***zzg BTApp_StatePromptHandler DLGRET_CANCELED m_porompt_id=%d, m_prompt_state_id=%d***", pMe->m_prompt_id, pMe->m_prompt_state_id, 0);	
			
			MOVE_TO_STATE(pMe->m_prompt_state_id)	
			return NFSMACTION_CONTINUE;

			/*
			switch(pMe->m_prompt_id)
			{
				case IDS_PROMPT_REMOVE_ONE:
				{
					MOVE_TO_STATE(BTAPPST_DEVICEINFO)	
					return NFSMACTION_CONTINUE;;
				}			

				case IDS_PROMPT_PROCEED_BONDING:
				{
					MSG_FATAL("***zzg StatePromptHandler IDS_PROMPT_PROCEED_BONDING***", 0, 0, 0);
					MOVE_TO_STATE(BTAPPST_DEVICEINFO)	//BTAPPST_MAIN ??
					return NFSMACTION_CONTINUE;;
				}	

				case IDS_PROMPT_UNBOND_ONE:
				{
					MOVE_TO_STATE(BTAPPST_DEVICEINFO)	
					return NFSMACTION_CONTINUE;;
				}
				
				case IDS_PROMPT_UNBOND_ALL:
				case IDS_PROMPT_REMOVE_ALL:
				{
					MOVE_TO_STATE(BTAPPST_DEVICE_LIST)	
					return NFSMACTION_CONTINUE;;
				}				
				default:
				{
					MOVE_TO_STATE(BTAPPST_MAIN)	
					return NFSMACTION_CONTINUE;;
				}
			}		
			
			return NFSMACTION_CONTINUE;
			*/

		case DLGRET_PROMPT:			
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
			
		case DLGRET_BT_EDIT:
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;			
			
		case DLGRET_BT_MSGBOX:			
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;	
	
		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

// ״̬ BTAPPST_BT_EDIT ������
static NextFSMAction BTApp_StateEditHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_EDIT);
			return NFSMACTION_WAIT;		

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;	
			
		case DLGRET_CANCELED:				

			MOVE_TO_STATE(pMe->m_edit_state_id)	
				
			/*
			switch(pMe->m_edit_id)
			{
				case IDS_PASS_KEY:
				{
					MSG_FATAL("***zzg StateEditHand IDS_PASS_KEY***", 0, 0, 0);					
					MOVE_TO_STATE(BTAPPST_DEVICEINFO)					
					return NFSMACTION_CONTINUE;;
				}			
				
				default:
				{
					MOVE_TO_STATE(BTAPPST_MAIN)	
					return NFSMACTION_CONTINUE;;
				}
			}			
			*/
				
			return NFSMACTION_CONTINUE;

		case DLGRET_PROMPT:			
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
			
		case DLGRET_BT_EDIT:
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;			
			
		case DLGRET_BT_MSGBOX:			
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;				

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}



// ״̬ BTAPPST_BT_SEND_FILE ������
static NextFSMAction BTApp_StateSendFileHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_SEND_FILE);
			return NFSMACTION_WAIT;		

		case DLGRET_BT_OBEX_LIST_SERVERS:			
			MSG_FATAL("***zzg SendFileHandle DLGRET_BT_OBEX_LIST_SERVERS***", 0, 0, 0);						
			MOVE_TO_STATE(BTAPPST_BT_OBEX_LIST_SERVERS)
			return NFSMACTION_CONTINUE;

		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;

		case DLGRET_PROMPT:			
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
			
		case DLGRET_BT_EDIT:
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;			
			
		case DLGRET_BT_MSGBOX:			
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;			
			
		case DLGRET_CANCELED:		
			(void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
			return NFSMACTION_WAIT;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

// ״̬ BTAPPST_BT_OBEX_LIST_SERVERS ������
static NextFSMAction BTApp_StateObexListServersHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			MSG_FATAL("***zzg BTApp_StateObexListServersHandler DLGRET_CREATE***", 0, 0, 0);	
			BTApp_ShowDialog(pMe, IDD_BT_OBEX_LIST_SERVERS);
			return NFSMACTION_WAIT;		

		case DLGRET_BT_SEND_FILE:
			MOVE_TO_STATE(BTAPPST_BT_SEND_FILE)
			return NFSMACTION_CONTINUE;		
			
		case DLGRET_BT_FILE_PROGRESS:
			MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PROMPT:			
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
			
		case DLGRET_BT_EDIT:
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;			
			
		case DLGRET_BT_MSGBOX:			
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;				
			
		case DLGRET_CANCELED:		
			MOVE_TO_STATE(BTAPPST_BT_SEND_FILE)
			return NFSMACTION_CONTINUE;	

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_BT_FILE_PROGRESS ������
static NextFSMAction BTApp_StateFileProgressHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			MSG_FATAL("***zzg BTApp_StateFileProgressHandler DLGRET_CREATE***", 0, 0, 0);	
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;		

		case DLGRET_BT_SEND_FILE:
			MOVE_TO_STATE(BTAPPST_BT_SEND_FILE)
			return NFSMACTION_CONTINUE;		

		case DLGRET_PROMPT:			
			MOVE_TO_STATE(BTAPPST_PROMPT)
			return NFSMACTION_CONTINUE;		
			
		case DLGRET_BT_EDIT:
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;			
			
		case DLGRET_BT_MSGBOX:			
			MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			return NFSMACTION_CONTINUE;				
			
		case DLGRET_CANCELED:		
			if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_BLUETOOTH_APP)
			{
				(void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
			}
			else
			{
				MOVE_TO_STATE(BTAPPST_MAIN)
			}
			
			return NFSMACTION_CONTINUE;	

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}




static NextFSMAction BTApp_StateExitHandler(CBTApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler
