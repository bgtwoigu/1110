/*==============================================================================
// �ļ���
//        ClockAppsStateHandler.c
//        2007-11-12  Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//
// �ļ�˵����
//
// ���ߣ�2007-11-12
// �������ڣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
2007-11-12           ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "ClockApps_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ ALARMST_NONE ������
static NextFSMAction StateNoOperationHandler(CClockApps *pMe);

// ״̬ ALARMST_INIT ������
static NextFSMAction StateInitHandler(CClockApps *pMe);

// ״̬ ALARMST_MAIN ������
static NextFSMAction StateAlarmMainHandler(CClockApps *pMe);

// ״̬ ALARMST_OPTION ������
static NextFSMAction StateAlarmOptionHandler(CClockApps *pMe);

// ״̬ ALARMST_SUB ������
static NextFSMAction StateAlarmSubHandler(CClockApps *pMe);

// ״̬ ALARMST_MESSEGE ������
//static NextFSMAction StateAlarmMessegeHandler(CClockApps *pMe);

// ״̬ ALARMST_REACH ������
static NextFSMAction StateAlarmTimeReachHandler(CClockApps *pMe);

// ״̬ ALARMST_EXIT ������
static NextFSMAction StateExitHandler(CClockApps *pMe);



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
       ClockApps_ProcessState
˵��:
       ClockApps Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��ClockApps Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction ClockApps_ProcessState(CClockApps *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case ALARMST_NONE:
            retVal = StateNoOperationHandler(pMe);
            break;

        case ALARMST_INIT:
            retVal = StateInitHandler(pMe);
            break;

        case ALARMST_MAIN:
            retVal = StateAlarmMainHandler(pMe);
            break;

        case ALARMST_OPTION:
            retVal = StateAlarmOptionHandler(pMe);
            break;

        case ALARMST_SUB:
            retVal = StateAlarmSubHandler(pMe);
            break;

        case ALARMST_REACH:
            retVal = StateAlarmTimeReachHandler(pMe);
            break;

        case ALARMST_EXIT:
            retVal = StateExitHandler(pMe);
            break;


        default:
            break;
    }
    return retVal;
}

/*==============================================================================
������
       StateNoOperationHandler
˵����
       ALARMST_NONE ״̬������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateNoOperationHandler(CClockApps *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(ALARMST_EXIT)
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler


/*==============================================================================
������
       StateInitHandler
˵����
       ALARMST_INIT ״̬������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateInitHandler(CClockApps *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(ALARMST_MAIN)
    return NFSMACTION_CONTINUE;

} // StateInitHandler


/*==============================================================================
������
       StateAlarmMainHandler
˵����
       ALARMST_MAIN ״̬������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateAlarmMainHandler(CClockApps *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            (void)ClockApps_ShowDialog(pMe, IDD_ALARMCLOCK_DLG);
            return NFSMACTION_WAIT;

        case DLGRET_ALARMCLOCK_SUB:
            MOVE_TO_STATE(ALARMST_SUB)
            return NFSMACTION_CONTINUE;

        case DLGRET_ALARMCLOCK_OPTION:
            MOVE_TO_STATE(ALARMST_OPTION)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(ALARMST_EXIT)
            return NFSMACTION_CONTINUE;

        default:
          break;
    }

    return NFSMACTION_WAIT;
} // StateAlarmMainHandler

// ״̬ ALARMST_OPTION ������
static NextFSMAction StateAlarmOptionHandler(CClockApps *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            (void)ClockApps_ShowDialog(pMe, IDD_ALARMCLOCK_OPTION);
            return NFSMACTION_WAIT;

        case DLGRET_DELCONFIRM:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            (void)ClockApps_ShowDialog(pMe, IDD_DELCONFIRM);
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:         
            (void)ClockApps_ShowDialog(pMe, IDD_WARNING);
            return NFSMACTION_WAIT;
            
        case DLGRET_ALARMCLOCK_SUB:
            MOVE_TO_STATE(ALARMST_SUB)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(ALARMST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
          break;
    }

    return NFSMACTION_WAIT;
} // StateAlarmOptionHandler
/*==============================================================================
������
       StateAlarmSubHandler
˵����
       ALARMST_SUB ״̬������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateAlarmSubHandler(CClockApps *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            (void)ClockApps_ShowDialog(pMe, IDD_ALARMCLOCK_SUB);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_OK:
            MOVE_TO_STATE(ALARMST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
             break;
    }

    return NFSMACTION_WAIT;
} // StateAlarmSubHandler
///*==============================================================================
//������
//       StateAlarmMessegeHandler
//˵����
//       ALARMST_MESSEGE ״̬������
//
//������
//       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
//
//����ֵ��
//       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
//       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
//
//��ע��
//
//==============================================================================*/
//static NextFSMAction StateAlarmMessegeHandler(CClockApps *pMe)
//{
//    if (NULL == pMe)
//    {
//        return NFSMACTION_WAIT;
//    }
//
//    switch(pMe->m_eDlgRet)
//    {
//        case DLGRET_CREATE:
//            pMe->m_bNotOverwriteDlgRet = FALSE;
//            (void)ClockApps_ShowDialog(pMe, IDD_ALARMCLOCK_MESSEGE);
//            return NFSMACTION_WAIT;
//
//        case DLGRET_OK:
//            MOVE_TO_STATE(ALARMST_MAIN)
//            return NFSMACTION_CONTINUE;
//
//        case DLGRET_CANCELED:
//            MOVE_TO_STATE(ALARMST_SUB)
//            return NFSMACTION_CONTINUE;
//
//        default:
//            break;
//    }
//
//    return NFSMACTION_WAIT;
//} // StateAlarmMessegeHandler
//

/*==============================================================================
������
       StateAlarmTimeReachHandler
˵����
       ALARMST_REACH ״̬������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateAlarmTimeReachHandler(CClockApps *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            (void)ClockApps_ShowDialog(pMe, IDD_ALARMTIME_REACH);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            ERR("%d", pMe->m_return, 0, 0);
            switch (pMe->m_return)
            {
                case 1:
                case 4:
                    MOVE_TO_STATE(ALARMST_MAIN)
                    break;

                case 2:
                    MOVE_TO_STATE(ALARMST_SUB)
                    break;

                default:
                    MOVE_TO_STATE(ALARMST_EXIT)
                    break;
            }
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // StateAlarmTimeReachHandler


/*==============================================================================
������
       StateExitHandler
˵����
       ALARMST_EXIT ״̬������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateExitHandler(CClockApps *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler


