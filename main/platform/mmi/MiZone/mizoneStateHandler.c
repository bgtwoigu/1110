/*==============================================================================
// �ļ���
//        iMIZONEateHandler.c
//        ��Ȩ����(c) 2007 Anylook
//
// �ļ�˵����
//
// ���ߣ�Gemsea
// �������ڣ�2007-11-13
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-13                ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "Mizone_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ MIZONE_NONE ������
static NextFSMAction MIZONENoOperationHandler(CMiZone *pMe);

// ״̬ MIZONE_INIT ������
static NextFSMAction MIZONEInitHandler(CMiZone *pMe);

// ״̬ MIZONE_MAIN ������
static NextFSMAction MIZONEMainHandler(CMiZone *pMe);

// ״̬ MIZONEACTIVATE ������
static NextFSMAction MIZONEACTIVATEHandler(CMiZone *pMe);

// ״̬ MIZONEENDACTIVATE ������
static NextFSMAction MIZONEENDACTIVATEHandler(CMiZone *pMe);

// ״̬ MIZONEASTROLOGY ������
static NextFSMAction MIZONEASTROLOGYHandler(CMiZone *pMe);

// ״̬ MIZONESERVICES ������
static NextFSMAction MIZONESERVICESHandler(CMiZone *pMe);

// ״̬ MIZONEENDPULL ������
static NextFSMAction MIZONEENDPULLHandler(CMiZone *pMe);

// ״̬ MIZONEINPUTSEND ������
static NextFSMAction MIZONEINPUTSENDHandler(CMiZone *pMe);

// ״̬ MIZONESENDNAME ������
static NextFSMAction MIZONESENDNAMEHandler(CMiZone *pMe);

// ״̬ ZIONE_EXIT ������
static NextFSMAction MIZONEExitHandler(CMiZone *pMe);


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
       MiZone_ProcessState
˵��:
       MiZone Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��MiZone Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction MiZone_ProcessState(CMiZone *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }
    MSG_FATAL("***MIZONEACTIVATEHandler****pMe->m_eCurState=%d",pMe->m_eCurState,0,0);
    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case MIZONE_NONE:
            retVal = MIZONENoOperationHandler(pMe);
            break;

        case MIZONE_INIT:
            retVal = MIZONEInitHandler(pMe);
            break;

        case MIZONE_MAIN:
            retVal = MIZONEMainHandler(pMe);
            break;

        case MIZONE_MIZONEACTIVATE:
            retVal = MIZONEACTIVATEHandler(pMe);
            break;
            
        case MIZONE_ENDACTIVATE:
            retVal = MIZONEENDACTIVATEHandler(pMe);
            break;

        case MIZONE_ASTROLOGY:
            retVal = MIZONEASTROLOGYHandler(pMe);
            break;

        case MIZONE_SERVICES:
            retVal = MIZONESERVICESHandler(pMe);
            break;

        case MIZONE_ENDPULL:
            retVal = MIZONEENDPULLHandler(pMe);
            break;    

        case MIZONE_INPUTSEND:
            retVal = MIZONEINPUTSENDHandler(pMe);
            break;    
        
        case MIZONE_SENDNAME:
            retVal = MIZONESENDNAMEHandler(pMe);
            break;  

        case MIZONE_EXIT:
            retVal = MIZONEExitHandler(pMe);
            break;

        default:
            break;
    }
    return retVal;
}

/*==============================================================================
������
       MIZONENoOperationHandler
˵����
       MIZONE_NONE ״̬������

������
       pMe [in]��ָ��MIZONE Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction MIZONENoOperationHandler(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(MIZONE_EXIT);
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler

/*==============================================================================
������MIZONEInitHandler

˵����

����״̬MIZONE_INITʱ���������á�����״̬��������������һ״̬ΪSTATE_MAIN��

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONEInitHandler(CMiZone *pMe)
{
    MOVE_TO_STATE(MIZONE_MAIN);
    return NFSMACTION_CONTINUE;
} //StateInitHandler

/*==============================================================================
������MIZONEMainHandler

˵����
       ����״̬MIZONE_MAINʱ���������á���������״̬MIZONE_MAIN����ز�����

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONEMainHandler(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            MiZone_ShowDialog(pMe, IDD_MAINMENU);
            return NFSMACTION_WAIT;
            
        case DLGRET_MIZONEACTIVATE:
            MOVE_TO_STATE(MIZONE_MIZONEACTIVATE)
            return NFSMACTION_CONTINUE;

        case DLGRET_MIZONESERVICES:
            MOVE_TO_STATE(MIZONE_SERVICES)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(MIZONE_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// MIZONEMainHandler

/*==============================================================================
������MIZONEACTIVATEHandler

˵����
       ����״̬MIZONE_MAINʱ���������á���������״̬MIZONE_MAIN����ز�����

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONEACTIVATEHandler(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("***MIZONEACTIVATEHandler****",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            MiZone_ShowDialog(pMe, IDD_MIZONEACTIVATE);
            return NFSMACTION_WAIT;

        case DLGRET_ENDACTIVATE:
            MOVE_TO_STATE(MIZONE_ENDACTIVATE)
            return NFSMACTION_CONTINUE;

        case DLGRET_MIZONEASTROLOGY:
            MOVE_TO_STATE(MIZONE_ASTROLOGY)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(MIZONE_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// MIZONEACTIVATEHandler


/*==============================================================================
������MIZONEACTIVATEHandler

˵����
       ����״̬MIZONE_MAINʱ���������á���������״̬MIZONE_MAIN����ز�����

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONEENDACTIVATEHandler(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("***MIZONEENDACTIVATEHandler****",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            MiZone_ShowDialog(pMe, IDD_ENDACTIVATE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(MIZONE_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// MIZONEENDACTIVATEHandler

/*==============================================================================
������MIZONEASTROLOGYHandler

˵����
       ����״̬MIZONE_MAINʱ���������á���������״̬MIZONE_MAIN����ز�����

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONEASTROLOGYHandler(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("***MIZONEASTROLOGYHandler****",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            MiZone_ShowDialog(pMe, IDD_ASTROLOGY);
            return NFSMACTION_WAIT;

        case DLGRET_ENDACTIVATE:
            MOVE_TO_STATE(MIZONE_ENDACTIVATE)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(MIZONE_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// MIZONEENDACTIVATEHandler


/*==============================================================================
������MIZONESERVICESHandler

˵����
       ����״̬MIZONE_MAINʱ���������á���������״̬MIZONE_MAIN����ز�����

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONESERVICESHandler(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("***MIZONEASTROLOGYHandler****",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            MiZone_ShowDialog(pMe, IDD_SERVICESMENU);
            return NFSMACTION_WAIT;

        case DLGRET_MIZONEENDPULL:
            MOVE_TO_STATE(MIZONE_ENDPULL)
            return NFSMACTION_CONTINUE;
             
        case DLGRET_CANCELED:
            MOVE_TO_STATE(MIZONE_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// MIZONESERVICESHandler



/*==============================================================================
������MIZONEENDPULLHandler

˵����
       ����״̬MIZONE_MAINʱ���������á���������״̬MIZONE_MAIN����ز�����

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONEENDPULLHandler(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("***MIZONEENDACTIVATEHandler****",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            MiZone_ShowDialog(pMe, IDD_ENDPULL);
            return NFSMACTION_WAIT;

        case DLGRET_MIZONEINPUTSEND:
            MOVE_TO_STATE(MIZONE_INPUTSEND)
            return NFSMACTION_CONTINUE;

        case DLGRET_MIZONESENDNAME:
            MOVE_TO_STATE(MIZONE_SENDNAME)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(MIZONE_SERVICES)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// MIZONEENDPULLHandler

/*==============================================================================
������MIZONEINPUTSENDHandler

˵����
       ����״̬MIZONE_MAINʱ���������á���������״̬MIZONE_MAIN����ز�����

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONEINPUTSENDHandler(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("***MIZONEINPUTSENDHandler****",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            MiZone_ShowDialog(pMe, IDD_INPUTSEND);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(MIZONE_SERVICES)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// MIZONEINPUTSENDHandler


/*==============================================================================
������MIZONESENDNAMEHandler

˵����
       ����״̬MIZONE_MAINʱ���������á���������״̬MIZONE_MAIN����ز�����

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONESENDNAMEHandler(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    MSG_FATAL("***MIZONESENDNAMEHandler****",0,0,0);
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            MiZone_ShowDialog(pMe, IDD_SENDNAME);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(MIZONE_SERVICES)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// MIZONESENDNAMEHandler

/*==============================================================================
������MIZONEExitHandler

˵����
       ����״̬MIZONE_EXITʱ���������á����ڹر�Applet��

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction MIZONEExitHandler(CMiZone *pMe)
{
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // MIZONEExitHandler
