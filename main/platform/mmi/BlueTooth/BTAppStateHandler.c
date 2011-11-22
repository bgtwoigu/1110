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

// ״̬ BTAPPST_DEVICE ������
static NextFSMAction BTApp_StateDeviceHandler(CBTApp *pMe);
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
// ״̬ BTAPPST_MYINFO_EDIT ������
static NextFSMAction BTApp_StateMyInfoEditHandler(CBTApp *pMe);
// ״̬ BTAPPST_SET_SECURITY ������
static NextFSMAction BTApp_StateSetSecurityHandler(CBTApp *pMe);
// ״̬ BTAPPST_SET_DISCOVERABLE ������
static NextFSMAction BTApp_StateSetDiscoverableHandler(CBTApp *pMe);
// ״̬ BTAPPST_SET_IOCAPABILITY ������
static NextFSMAction BTApp_StateSetIOCapabilityHandler(CBTApp *pMe);
// ״̬ BTAPPST_SET_DEBUGKEY ������
static NextFSMAction BTApp_StateSetDebugKeyHandler(CBTApp *pMe);



// ״̬ BTAPPST_FTP ������
static NextFSMAction BTApp_StateFtpHandler(CBTApp *pMe);
// ״̬ BTAPPST_FTP_SERVER ������
static NextFSMAction BTApp_StateFtpServerHandler(CBTApp *pMe);
// ״̬ BTAPPST_FTP_CLIENT ������
static NextFSMAction BTApp_StateFtpClientHandler(CBTApp *pMe);
// ״̬ BTAPPST_FTP_SETTING ������
static NextFSMAction BTApp_StateFtpSettingHandler(CBTApp *pMe);
// ״̬ BTAPPST_FTP_SERVER_REGISTER ������
static NextFSMAction BTApp_StateFtpServerRegisterHandler(CBTApp *pMe);
// ״̬ BTAPPST_FTP_BROWSE ������
static NextFSMAction BTApp_StateFtpBrowseHandler(CBTApp *pMe);
// ״̬ BTAPPST_FTP_BROWSE_OPITION ������
static NextFSMAction BTApp_StateFtpBrowseOpitionHandler(CBTApp *pMe);





// ״̬ BTAPPST_BT_MSGBOX ������
//static NextFSMAction BTApp_StateMsgBoxHandler(CBTApp *pMe);
// ״̬ BTAPPST_PROMPT ������
//static NextFSMAction BTApp_StatePromptHandler(CBTApp *pMe);

// ״̬ BTAPPST_BT_EDIT ������
static NextFSMAction BTApp_StateEditHandler(CBTApp *pMe);
// ״̬ BTAPPST_BT_SEND_FILE ������
static NextFSMAction BTApp_StateSendFileHandler(CBTApp *pMe);
// ״̬ BTAPPST_BT_OBEX_LIST_SERVERS ������
static NextFSMAction BTApp_StateObexListServersHandler(CBTApp *pMe);
// ״̬ BTAPPST_BT_FILE_PROGRESS ������
//static NextFSMAction BTApp_StateFileProgressHandler(CBTApp *pMe);



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

#define ASSERT_BT_NOT_REACHABLE 


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
        {
			retVal = BTApp_StateNoOperationHandler(pMe);
            break;
        }

        case BTAPPST_INIT:			
        {
			retVal = BTApp_StateInitHandler(pMe);
            break;
		}
	
        case BTAPPST_MAIN:           
        {
			retVal = BTApp_StateMainHandler(pMe);
            break;
		}	
			
        case BTAPPST_DEVICESRH:
        {
			retVal = BTApp_StateDeviceSearchHandler(pMe);
            break;
		}	

        case BTAPPST_SRHRESULT:			
        {
			retVal = BTApp_StateSearchResultHandler(pMe);
            break;
		}	

		case BTAPPST_DEVICEINFO:			
        {
			retVal = BTApp_StateDeviceInfoHandler(pMe);
            break;
		}	
			
		case BTAPPST_DEVICEINFO_OPITION:			
        {
			retVal = BTApp_StateDeviceInfoOpitionHandler(pMe);
            break;	
		}	

		case BTAPPST_BOND_OPITION:			
        {
			retVal = BTApp_StateBondOpitionHandler(pMe);
            break;	
		}	

		case BTAPPST_DEVICE:
        {
			retVal = BTApp_StateDeviceHandler(pMe);
            break;
		}	
		case BTAPPST_DEVICE_LIST:
        {
			retVal = BTApp_StateDeviceListHandler(pMe);
            break;
		}
			
        case BTAPPST_MYINFO:			
        {
			retVal = BTApp_StateMyInfoHandler(pMe);
            break;
		}	
			
		case BTAPPST_MYINFO_OPITION:			
        {
			retVal = BTApp_StateMyInfoOpitionHandler(pMe);
            break;
		}	

		//BTAPPST_EDIT_NAME/BTAPPST_EDIT_SHORTNAME/BTAPPST_EDIT_MANUDATA
		case BTAPPST_MYINFO_EDIT:
        {
			retVal = BTApp_StateMyInfoEditHandler(pMe);
            break;
		}      

		case BTAPPST_SET_SECURITY:
        {   
			retVal = BTApp_StateSetSecurityHandler(pMe);
            break;
		}	
			
        case BTAPPST_SET_DISCOVERABLE:
        {    
			retVal = BTApp_StateSetDiscoverableHandler(pMe);
            break;
		}	

        case BTAPPST_SET_IOCAPABILITY:
        {   
			retVal = BTApp_StateSetIOCapabilityHandler(pMe);
            break;
		}	

        case BTAPPST_SET_DEBUGKEY:
        {    
			retVal = BTApp_StateSetDebugKeyHandler(pMe);
            break;
		}	

		case BTAPPST_FTP:
        {    
			retVal = BTApp_StateFtpHandler(pMe);
            break;	
		}	

		case BTAPPST_FTP_SERVER:
        {    
			retVal = BTApp_StateFtpServerHandler(pMe);
            break;
		}	

		case BTAPPST_FTP_CLIENT:
        {    
			retVal = BTApp_StateFtpClientHandler(pMe);
            break;
		}	

		case BTAPPST_FTP_SETTING:
        {    
			retVal = BTApp_StateFtpSettingHandler(pMe);
            break;	
		}	

		case BTAPPST_FTP_SERVER_REGISTER:
        {    
			retVal = BTApp_StateFtpServerRegisterHandler(pMe);
            break;
		}	

		case BTAPPST_FTP_BROWSE:
        {   
			retVal = BTApp_StateFtpBrowseHandler(pMe);
            break;
		}	

		case BTAPPST_FTP_BROWSE_OPITION:
        {    
			retVal = BTApp_StateFtpBrowseOpitionHandler(pMe);
            break;	
		}	

		/*	
		case BTAPPST_BT_MSGBOX:
        {    
			retVal = BTApp_StateMsgBoxHandler(pMe);
            break;	
		}	

		
        case BTAPPST_PROMPT:			
        {    
			retVal = BTApp_StatePromptHandler(pMe);
            break;
		}
		*/
			
		case BTAPPST_BT_EDIT:
        {    
			retVal = BTApp_StateEditHandler(pMe);
            break;
		}	

		case BTAPPST_BT_SEND_FILE:
        {    
			retVal = BTApp_StateSendFileHandler(pMe);
            break;	
		}	

		case BTAPPST_BT_OBEX_LIST_SERVERS:			
        {    
			retVal = BTApp_StateObexListServersHandler(pMe);
            break;	
		}	

		/*	
		case BTAPPST_BT_FILE_PROGRESS:			
        {    
			retVal = BTApp_StateFileProgressHandler(pMe);
            break;
		}	
		*/
			
		case BTAPPST_EXIT:
		{	
			retVal = BTApp_StateExitHandler(pMe);
            break;
		}	

        default:
        {   
			ASSERT_BT_NOT_REACHABLE;
		}	
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

	//���¼����Ӳ˵�ѡ������λ��
    pMe->m_nSubDlgId = 0 ;    

	MSG_FATAL("***zzg BTApp_StateMainHandler m_eDlgRet=%d***", pMe->m_eDlgRet, 0, 0);

    switch (pMe->m_eDlgRet)
    {
    	case DLGRET_MSGBOX_CANCELED:
    	case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
        case DLGRET_CREATE:
        {
			pMe->m_bNotOverwriteDlgRet = FALSE;
            BTApp_ShowDialog(pMe, IDD_BT_MAINMENU);
            return NFSMACTION_WAIT;
        }

		case DLGRET_DEVICESRH:			
		{
			MOVE_TO_STATE(BTAPPST_DEVICESRH)
            return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_DEVICE:
		{
			MOVE_TO_STATE(BTAPPST_DEVICE)
            return NFSMACTION_CONTINUE;	
		}

		case DLGRET_MYINFO:			
		{
			MOVE_TO_STATE(BTAPPST_MYINFO)
            return NFSMACTION_CONTINUE;
		}

		case DLGRET_FTP:
		{
			MOVE_TO_STATE(BTAPPST_FTP)
            return NFSMACTION_CONTINUE;		
		}
		

		case DLGRET_BT_MSGBOX:
		{
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}

		case DLGRET_PROMPT:
		{
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:			
		{
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;			
		}	
		
        case DLGRET_CANCELED:
        {
			MOVE_TO_STATE(BTAPPST_EXIT)
            return NFSMACTION_CONTINUE;
        }

		case DLGRET_SRHRESULT:
		{	
			MOVE_TO_STATE(BTAPPST_SRHRESULT)
            return NFSMACTION_CONTINUE;
		}	

        default:
        {
			ASSERT_BT_NOT_REACHABLE;
        }
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
    	//case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:
        case DLGRET_CREATE:
		{
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_DEVICE_SEARCH);            
			return NFSMACTION_WAIT;
        }
		
		case DLGRET_SRHRESULT:
		{	
			MOVE_TO_STATE(BTAPPST_SRHRESULT)
            return NFSMACTION_CONTINUE;
		}	

		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{				
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}	
			
		case DLGRET_MSGBOX_CANCELED:	//Add By zzg 2011_09_24	
        case DLGRET_CANCELED:
		{	
			if (pMe->mSD.bDiscovering)
			{
				IBTEXTSD_StopDeviceDiscovery(pMe->mSD.po);
			}
			
			BTApp_CancelDevNameRequest(pMe);

            MOVE_TO_STATE(BTAPPST_MAIN)
            return NFSMACTION_CONTINUE;
		}
		
        default:
        {    
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:
		case DLGRET_CREATE:
		{
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_SEARCH_RESULT);
			return NFSMACTION_WAIT;
		}	
			
		case DLGRET_DEVICEINFO:
		{	
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;
		}	
		
		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}	
						
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_MAIN)
			return NFSMACTION_CONTINUE;
		}	

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}	
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_DEVICE_INFO);
			return NFSMACTION_WAIT;
		}

		case DLGRET_DEVICEINFO_OPITION:
		{
			MOVE_TO_STATE(BTAPPST_DEVICEINFO_OPITION)
			return NFSMACTION_CONTINUE;
		}
		

		case DLGRET_BT_MSGBOX:
		{			
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}
			
		case DLGRET_PROMPT:
		{			
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
						
		case DLGRET_CANCELED:
		{
			if (pMe->m_dialog_id == IDD_DEVICE_LIST)				
			{
				MOVE_TO_STATE(BTAPPST_DEVICE_LIST)
			}
			else if (pMe->m_dialog_id == IDD_SEARCH_RESULT)
			{
				MOVE_TO_STATE(BTAPPST_SRHRESULT)
			}
			else
			{
				MOVE_TO_STATE(BTAPPST_MAIN)
			}
			
			return NFSMACTION_CONTINUE;
		}
		default:
		{
			ASSERT_BT_NOT_REACHABLE;
		}
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

	MSG_FATAL("***zzg BTApp_StateDeviceInfoOpitionHandler m_eDlgRet=%x***", pMe->m_eDlgRet, 0, 0);

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_DEVICE_INFO_OPITION);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BOND_OPITION:
		{
			MOVE_TO_STATE(BTAPPST_BOND_OPITION)
			return NFSMACTION_CONTINUE;		
		}

		case DLGRET_BT_MSGBOX:
		{
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}
			
		case DLGRET_PROMPT:
		{
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_CANCELED:
		{
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;
		}
			
		default:
		{
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:			
		{
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_BOND_OPITION);
			return NFSMACTION_WAIT;
		}
			
		case DLGRET_BT_MSGBOX:
		{
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}		
			
		case DLGRET_PROMPT:
		{			
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{			
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
						
		case DLGRET_CANCELED:
		{
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;
		}

		default:
		{
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_DEVICE ������
static NextFSMAction BTApp_StateDeviceHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{		
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_DEVICE);
			return NFSMACTION_WAIT;
		}

		case DLGRET_DEVICE_LIST:
		{
			MOVE_TO_STATE(BTAPPST_DEVICE_LIST)
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_BT_MSGBOX:
		{			
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}
			
		case DLGRET_PROMPT:
		{			
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{			
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_CANCELED:
		{
			MOVE_TO_STATE(BTAPPST_MAIN)
			return NFSMACTION_CONTINUE;
		}

		default:
		{
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_DEVICE_LIST);
			return NFSMACTION_WAIT;	
		}

		case DLGRET_DEVICEINFO:
		{
			MOVE_TO_STATE(BTAPPST_DEVICEINFO)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_BT_MSGBOX:
		{			
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}
			
		case DLGRET_PROMPT:
		{
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;		
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_CANCELED:
		{
			MOVE_TO_STATE(BTAPPST_DEVICE)
			return NFSMACTION_CONTINUE;
		}	
	
		default:
		{
			ASSERT_BT_NOT_REACHABLE;
		}	
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_MYINFO);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_MYINFO_OPITION:			
		{	
			MOVE_TO_STATE(BTAPPST_MYINFO_OPITION)
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	
			
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;		

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
		
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_MAIN)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_MYINFO_OPITION);
			return NFSMACTION_WAIT;
		}						

		//Add By zzg 2011_05_28
		case DLGRET_MYINFO_EDIT:
		{
			MOVE_TO_STATE(BTAPPST_MYINFO_EDIT)
			return NFSMACTION_CONTINUE;
		}
		//Add End
		
		case DLGRET_SET_SECURITY:
		{	
			MOVE_TO_STATE(BTAPPST_SET_SECURITY)
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_SET_DISCOVERABLE:
		{	
			MOVE_TO_STATE(BTAPPST_SET_DISCOVERABLE)
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_SET_IOCAPABILITY:
		{	
			MOVE_TO_STATE(BTAPPST_SET_IOCAPABILITY)
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_SET_DEBUGKEY:
		{	
			MOVE_TO_STATE(BTAPPST_SET_DEBUGKEY)
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	
		
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
					
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_MYINFO_EDIT ������
static NextFSMAction BTApp_StateMyInfoEditHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}
	
	switch(pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_MYINFO_EDIT);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_MSGBOX:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	
			
		case DLGRET_PROMPT:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;
		}
			
		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_SECURITY);
			return NFSMACTION_WAIT;
		}
	
		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}	
			
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	
			
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_DISCOVERABLE);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}			
			
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_IO_CAPABILITY);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}	
			
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	
			
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_DEBUG_KEY);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}
			
		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}	
			
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	
			
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_MYINFO)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_FTP ������
static NextFSMAction BTApp_StateFtpHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FTP);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_FTP_SERVER:
		{	
			MOVE_TO_STATE(BTAPPST_FTP_SERVER)
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_FTP_CLIENT:
		{	
			MOVE_TO_STATE(BTAPPST_FTP_CLIENT)
			return NFSMACTION_CONTINUE; 
		}

		case DLGRET_FTP_SETTING:
		{	
			MOVE_TO_STATE(BTAPPST_FTP_SETTING)
			return NFSMACTION_CONTINUE; 
		}
		
		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	
			
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	
						
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_MAIN)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_FTP_SERVER ������
static NextFSMAction BTApp_StateFtpServerHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:		
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FTP_SERVER);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_FTP_SERVER_REGISTER:
		{	
			MOVE_TO_STATE(BTAPPST_FTP_SERVER_REGISTER)
			return NFSMACTION_CONTINUE; 
		}
			
		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	
		
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_FTP)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}

// ״̬ BTAPPST_FTP_CLIENT������
static NextFSMAction BTApp_StateFtpClientHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:		
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FTP_CLIENT);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_OBEX_LIST_SERVERS:			
		{							
			MOVE_TO_STATE(BTAPPST_BT_OBEX_LIST_SERVERS)
			return NFSMACTION_CONTINUE;
		}	

		case DLGRET_FTP_BROWSE:			
		{							
			MOVE_TO_STATE(BTAPPST_FTP_BROWSE)
			return NFSMACTION_CONTINUE;	
		}		
		
		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	
		
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}	
			
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_FTP)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}

// ״̬ BTAPPST_FTP_SETTING ������
static NextFSMAction BTApp_StateFtpSettingHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch (pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:		
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FTP_SETTINGS);
			return NFSMACTION_WAIT;
		}	
		
		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}				
			
		case DLGRET_PROMPT:
		{				
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_FTP)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_FTP_SERVER_REGISTER ������
static NextFSMAction BTApp_StateFtpServerRegisterHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:		
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FTP_SERVER_REGISTER);
			return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	
		
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}	

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_FTP_SERVER)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_FTP_BROWSE������
static NextFSMAction BTApp_StateFtpBrowseHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_FTP_BROWSE:		
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FTP_BROWSE);
			return NFSMACTION_WAIT;
		}	
		case DLGRET_FTP_BROWSE_OPITION:			
		{					
			MOVE_TO_STATE(BTAPPST_FTP_BROWSE_OPITION)
			return NFSMACTION_CONTINUE;		
		}	

		case DLGRET_FTP_CLIENT:
		{	
			MOVE_TO_STATE(BTAPPST_FTP_CLIENT)
			return NFSMACTION_CONTINUE;
		}	
		
		case DLGRET_BT_MSGBOX:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}	
		
		case DLGRET_PROMPT:
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_CANCELED:		
		{	
			if (!pMe->mFTP.bObjectTransfer)
			{
				MOVE_TO_STATE(BTAPPST_FTP_CLIENT)			
			}
			return NFSMACTION_CONTINUE;
		}	

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_FTP_BROWSE_OPITION������
static NextFSMAction BTApp_StateFtpBrowseOpitionHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FTP_BROWSE_OPITION);
			return NFSMACTION_WAIT;
		}	
		
		case DLGRET_FTP_CLIENT:
		{	
			MOVE_TO_STATE(BTAPPST_FTP_CLIENT)
			return NFSMACTION_CONTINUE;
		}	

		case DLGRET_FTP_BROWSE:
		{	
			MOVE_TO_STATE(BTAPPST_FTP_BROWSE)
			return NFSMACTION_CONTINUE;
		}
		
		case DLGRET_BT_MSGBOX:			
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}
			
		case DLGRET_PROMPT:			
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:			
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;
		}
			
		case DLGRET_CANCELED:
		{	
			MOVE_TO_STATE(BTAPPST_FTP_BROWSE)
			return NFSMACTION_CONTINUE;
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_EDIT);
			return NFSMACTION_WAIT;		
		}	
			
		case DLGRET_CANCELED:				
		{	
			MOVE_TO_STATE(pMe->m_edit_state_id)					
			return NFSMACTION_CONTINUE;
		}

		case DLGRET_PROMPT:			
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;	
		}
			
		case DLGRET_BT_MSGBOX:			
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;	
		}

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;	
			
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;		
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_SEND_FILE);
			return NFSMACTION_WAIT;		
		}	

		case DLGRET_BT_OBEX_LIST_SERVERS:			
		{								
			MOVE_TO_STATE(BTAPPST_BT_OBEX_LIST_SERVERS)
			return NFSMACTION_CONTINUE;
		}	
			
		case DLGRET_BT_MSGBOX:			
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;		
		}

		case DLGRET_PROMPT:			
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;		
		}
			
		case DLGRET_CANCELED:		
		{	
			(void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
			return NFSMACTION_WAIT;
		}	

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
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
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_PROGRESS_CANCELED:	
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;		
			BTApp_ShowDialog(pMe, IDD_BT_OBEX_LIST_SERVERS);
			return NFSMACTION_WAIT;		
		}	

		//Add By zzg 2011_06_08
		//have connected...
		case DLGRET_FTP_CLIENT:
		{
			MOVE_TO_STATE(BTAPPST_FTP_CLIENT)
			return NFSMACTION_CONTINUE;
		}
		//Add End

		case DLGRET_BT_SEND_FILE:
		{	
			MOVE_TO_STATE(BTAPPST_BT_SEND_FILE)
			return NFSMACTION_CONTINUE;		
		}		
			
		case DLGRET_BT_MSGBOX:			
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;
		}

		case DLGRET_PROMPT:			
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;
		}

		case DLGRET_BT_FILE_PROGRESS:
		{	
			//MOVE_TO_STATE(BTAPPST_BT_FILE_PROGRESS)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;
		}

		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;	
		}
			
		case DLGRET_CANCELED:		
		{	
			if (pMe->m_obex_list_id == IDD_BT_FTP_CLIENT)	
			{
				MOVE_TO_STATE(BTAPPST_FTP_CLIENT)
			}
			else 
			{
				MOVE_TO_STATE(BTAPPST_BT_SEND_FILE)
			}
			return NFSMACTION_CONTINUE;	
		}

		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}


// ״̬ BTAPPST_BT_FILE_PROGRESS ������
/*
static NextFSMAction BTApp_StateFileProgressHandler(CBTApp *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_MSGBOX_CANCELED:
		case DLGRET_PROMPT_CANCELED:
		case DLGRET_CREATE:
		{	
			pMe->m_bNotOverwriteDlgRet = FALSE;			
			BTApp_ShowDialog(pMe, IDD_BT_FILE_PROGRESS);
			return NFSMACTION_WAIT;		
		}	

		case DLGRET_BT_SEND_FILE:
		{	
			MOVE_TO_STATE(BTAPPST_BT_SEND_FILE)
			return NFSMACTION_CONTINUE;		
		}

		case DLGRET_PROMPT:			
		{	
			//MOVE_TO_STATE(BTAPPST_PROMPT)
			//return NFSMACTION_CONTINUE;	

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_PROMPT);	
            return NFSMACTION_WAIT;	
		}
			
		case DLGRET_BT_EDIT:
		{	
			MOVE_TO_STATE(BTAPPST_BT_EDIT)
			return NFSMACTION_CONTINUE;		
		}
			
		case DLGRET_BT_MSGBOX:			
		{	
			//MOVE_TO_STATE(BTAPPST_BT_MSGBOX)
			//return NFSMACTION_CONTINUE;

			pMe->m_bNotOverwriteDlgRet = FALSE;
			BTApp_ShowDialog(pMe, IDD_BT_MSGBOX);
			return NFSMACTION_WAIT;				
		}
			
		case DLGRET_CANCELED:		
		{	
			if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_BLUETOOTH_APP)
			{
				(void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
			}
			else
			{
				MOVE_TO_STATE(BTAPPST_MAIN)
			}
			
			return NFSMACTION_CONTINUE;	
		}
		
		default:
		{	
			ASSERT_BT_NOT_REACHABLE;
		}
	}

	return NFSMACTION_WAIT;
}
*/



static NextFSMAction BTApp_StateExitHandler(CBTApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler
