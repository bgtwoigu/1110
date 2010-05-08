#ifndef _SPORTSAPPMENU_STATE_HANDLER_C_
#define _SPORTSAPPMENU_STATE_HANDLER_C_
/*********************************************************************************

ģ����       : <SportApplication>
�ļ���       : <SportSAppMenuStateHandler.c>
����ļ�     : 
�ļ�ʵ�ֹ��� : Sports Application
����         : 
�汾         : 1.0
--------------------------------------------------------------------------------
��ע         : 
--------------------------------------------------------------------------------
�޸ļ�¼ : 

******************************************************************************/



/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "SportSApp_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ SPORTSAPP_NONE ������
static NextFSMAction SportSApp_StateNoOperationHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_INIT ������
static NextFSMAction SportSApp_StateInitHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_MAIN ������
static NextFSMAction SportSApp_StateMainHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_PEDOMETER_MAIN ������
static NextFSMAction SportSApp_StatePedometerHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_PEDOMETER_MAIN ������
static NextFSMAction SportSApp_StatePedometerOptionHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_PEDOMETER_RECORD ������
static NextFSMAction SportSApp_StatePedometerOptRecordHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_PEDOMETER_RECORD_VIEW ������
static NextFSMAction SportSApp_StatePedometerRecViewHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_PERSONAL ������
static NextFSMAction SportSApp_StatePersonalHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_PERSONAL_OPTION ������
static NextFSMAction SportSApp_StatePersonalOptionHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_PERSONAL_EDIT������
static NextFSMAction SportSApp_StatePersonalEditHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_PERSONAL_INPUT������
static NextFSMAction SportSApp_StatePersonalInputHandler(CSportsMenu *pMe);

//״̬SPORT_STATE_MSGBOX ������
static NextFSMAction SportSApp_StateMsgBoxHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_PEDOMETER_BGROUND ������
static NextFSMAction SportSApp_StatePedometerConfirmBgRunHandler(CSportsMenu *pMe);

// ״̬ SPORTSAPP_EXIT ������
static NextFSMAction SportSApp_StateExitHandler(CSportsMenu *pMe);

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
NextFSMAction SportSApp_ProcessState(CSportsMenu *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    switch (pMe->m_eCurState)
    {
        	case SPORT_STATE_NONE:
            		retVal = SportSApp_StateNoOperationHandler(pMe);
            		break;

        	case SPORT_STATE_INIT:
            		retVal = SportSApp_StateInitHandler(pMe);
            		break;

        	case SPORT_STATE_MAIN:
            		retVal = SportSApp_StateMainHandler(pMe);
            		break;
					
		case SPORT_STATE_PETOMETER:
			retVal = SportSApp_StatePedometerHandler(pMe);
			break;
			
		case SPORT_STATE_PETOMETER_OPTION:
			retVal = SportSApp_StatePedometerOptionHandler(pMe);
			break;
			
		case SPORT_STATE_PEDOMETER_RECORD:
			retVal = SportSApp_StatePedometerOptRecordHandler(pMe);
			break;
			
		case SPORT_STATE_PEDOMETER_RECORD_VIEW:
			retVal = SportSApp_StatePedometerRecViewHandler(pMe);
			break;
			
		case SPORT_STATE_PERSONAL:
			retVal = SportSApp_StatePersonalHandler(pMe);
			break;
			
		case SPORT_STATE_PERSONAL_OPTION:
			retVal = SportSApp_StatePersonalOptionHandler(pMe);
			break;
		case SPORT_STATE_PERSONAL_EDIT:
			retVal = SportSApp_StatePersonalEditHandler(pMe);
			break;
			
		case SPORT_STATE_INPUTED:
			retVal = SportSApp_StatePersonalInputHandler(pMe);
			break;
		case SPORT_STATE_MSGBOX:
			retVal = SportSApp_StateMsgBoxHandler(pMe);
			break;
		case SPORT_STATE_APP_BGROUND:
			retVal = SportSApp_StatePedometerConfirmBgRunHandler(pMe);
			break;
        	case SPORT_STATE_EXIT:
            		retVal = SportSApp_StateExitHandler(pMe);
            		break;
        	default:
            		ASSERT_NOT_REACHABLE;
    }
    SPORTS_ERR("curstate %d prestate %d dlgret %d CallApp_ProcessState",pMe->m_eCurState,pMe->m_ePreState,pMe->m_eDlgRet);
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
static NextFSMAction SportSApp_StateNoOperationHandler(CSportsMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(SPORT_STATE_EXIT)
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler


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
static NextFSMAction SportSApp_StateInitHandler(CSportsMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(SPORT_STATE_MAIN)
    return NFSMACTION_CONTINUE;
} // StateInitHandler


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
static NextFSMAction SportSApp_StateMainHandler(CSportsMenu *pMe)
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
            		SportSApp_ShowDialog(pMe, IDD_DIALOG_MAIN);
            		return NFSMACTION_WAIT;
				
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL)
			return NFSMACTION_CONTINUE;
			
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SPORT_STATE_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateMainHandler


/*==============================================================================
������
       SportSApp_StatePedometerHandler
˵����
       SPORT_STATE_PEDOMETER ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SportSApp_StatePedometerHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			SportSApp_ShowDialog(pMe,IDD_DIALOG_PEDOMETER);//IDD_DIALOG_PEDOMETER
			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(SPORT_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SportSApp_StatePedometerOptionHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SportSApp_StatePedometerOptionHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			SportSApp_ShowDialog(pMe,IDD_DIALOG_PEDOMETER_OPTION);//IDD_DIALOG_PEDOMETER
			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD)
			return NFSMACTION_CONTINUE;

		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(SPORT_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SportSApp_StatePedometerRecViewHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SportSApp_StatePedometerRecViewHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			SportSApp_ShowDialog(pMe,IDD_DIALOG_PED_RECORD_VIEW);//IDD_DIALOG_PEDOMETER
			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD)
			return NFSMACTION_CONTINUE;

		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL)
			return NFSMACTION_CONTINUE;

            case DLGRET_MSGBOX:
                MOVE_TO_STATE(SPORT_STATE_MSGBOX)
                return NFSMACTION_CONTINUE;  

		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SportSApp_StatePedometerOptRecordHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SportSApp_StatePedometerOptRecordHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			SportSApp_ShowDialog(pMe,IDD_DIALOG_PED_RECORD);//IDD_DIALOG_PEDOMETER
			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD)			
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
		case DLGRET_PERSONAL:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL)
			return NFSMACTION_CONTINUE;
        case DLGRET_MSGBOX:
            MOVE_TO_STATE(SPORT_STATE_MSGBOX)
            return NFSMACTION_CONTINUE;    
		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SportSApp_StatePersonalHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SportSApp_StatePersonalHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			SportSApp_ShowDialog(pMe,IDD_DIALOG_PERSONAL);//IDD_DIALOG_PERSONAL
			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD)			
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL_EDIT:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_EDIT)
				
			return NFSMACTION_CONTINUE;
		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SportSApp_StatePersonalOptionHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SportSApp_StatePersonalOptionHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			SportSApp_ShowDialog(pMe,IDD_DIALOG_PER_OPTION);//IDD_DIALOG_PERSONAL
			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD)			
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL)
			return NFSMACTION_CONTINUE;
		case DLGRET_PERSONAL_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_OPTION)
			return NFSMACTION_CONTINUE;		
			
		case DLGRET_PERSONAL_EDIT:
			MOVE_TO_STATE(DLGRET_PERSONAL_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SportSApp_StatePersonalEditHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SportSApp_StatePersonalEditHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			SportSApp_ShowDialog(pMe,IDD_DIALOG_PER_EDIT);//IDD_DIALOG_PERSONAL
			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD)			
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL_EDIT:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_EDIT:
			MOVE_TO_STATE(SPORT_STATE_INPUTED)
			return NFSMACTION_CONTINUE;
				
		case DLGRET_INPUT:
			MOVE_TO_STATE(SPORT_STATE_INPUTED)
			return NFSMACTION_CONTINUE;
					
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(SPORT_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;

		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SportSApp_StatePersonalInputHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SportSApp_StatePersonalInputHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			SportSApp_ShowDialog(pMe,IDD_DIALOG_INPUT);//IDD_DIALOG_PERSONAL
			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD)			
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL)
			return NFSMACTION_CONTINUE;
		case DLGRET_PERSONAL_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_OPTION)
			return NFSMACTION_CONTINUE;			
		case DLGRET_PERSONAL_EDIT:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_EDIT)
			return NFSMACTION_CONTINUE;

		case DLGRET_EDIT:
			MOVE_TO_STATE(SPORT_STATE_INPUTED)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_OK:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_INPUT:
			MOVE_TO_STATE(SPORT_STATE_INPUTED)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_MSGBOX:
			MOVE_TO_STATE(SPORT_STATE_MSGBOX)
			return NFSMACTION_CONTINUE;
	
		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SportSApp_StateHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/

static NextFSMAction SportSApp_StateMsgBoxHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
	            // Show dialog
	            if(SUCCESS != SportSApp_ShowMsgBox(pMe, pMe->m_pSportsAppMenu.m_wMsgResID))
	            {
	                MOVE_TO_STATE(SPORT_STATE_EXIT)
	                return NFSMACTION_CONTINUE;
	            }

			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD) 		
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PERSONAL:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL)
			return NFSMACTION_CONTINUE;
		case DLGRET_PERSONAL_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_OPTION)
			return NFSMACTION_CONTINUE; 		
		case DLGRET_PERSONAL_EDIT:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_EDIT)
			return NFSMACTION_CONTINUE;

		case DLGRET_EDIT:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_OK:
			MOVE_TO_STATE(SPORT_STATE_PERSONAL_EDIT)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_INPUT:
			MOVE_TO_STATE(SPORT_STATE_INPUTED)
			return NFSMACTION_CONTINUE;
		
		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
	}

	return NFSMACTION_WAIT;
}

/*==============================================================================
������
       SportSApp_StatePedometerConfirmBgRunHandler
˵����
       SPORT_STATE_EXIT ״̬������

������
       pMe [in]��ָ��SportSApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction SportSApp_StatePedometerConfirmBgRunHandler(CSportsMenu *pMe)
{
	if (NULL == pMe)
	{
		return NFSMACTION_WAIT;
	}

	pMe->m_nSubDlgId = 0 ;	  //���¼����Ӳ˵�ѡ������λ��

	switch(pMe->m_eDlgRet)
	{
		case DLGRET_CREATE:
			pMe->m_bNotOverwriteDlgRet = FALSE;
			SportSApp_ShowDialog(pMe,IDD_APP_BGROUND);
			return NFSMACTION_WAIT;
			
		case DLGRET_SPORTS:
			MOVE_TO_STATE(SPORT_STATE_MAIN)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_OPTION:
			MOVE_TO_STATE(SPORT_STATE_PETOMETER_OPTION)
			return NFSMACTION_CONTINUE;
			
		case DLGRET_PEDOMETER_RECORD_VIEW:
			MOVE_TO_STATE(SPORT_STATE_PEDOMETER_RECORD_VIEW)
			return NFSMACTION_CONTINUE;
		case DLGRET_APP_BGROND:
			MOVE_TO_STATE(SPORT_STATE_APP_BGROUND)
			return NFSMACTION_CONTINUE;
		case DLGRET_CANCELED:
			MOVE_TO_STATE(SPORT_STATE_EXIT)
			return NFSMACTION_CONTINUE;

		default:
			ASSERT_NOT_REACHABLE;
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
static NextFSMAction SportSApp_StateExitHandler(CSportsMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler

#endif
