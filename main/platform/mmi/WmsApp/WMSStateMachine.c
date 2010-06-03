/*==============================================================================
//           W M S   A P P L E T  M O D U L E
// �ļ�:
//        WMSStateMachine.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        ����Ϣģ��״̬������������ļ�
//                 
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------

==============================================================================*/


/*==============================================================================
                             
                             ���ļ��������ⲿ�ļ�
                             
==============================================================================*/
#include "WMSApp_priv.h" 
#include "WMSUtil.h"

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/



/*==============================================================================

                                 ��������

==============================================================================*/
// WMSST_VOICEMAIL ״̬������
static NextFSMAction WMSST_VOICEMAIL_Handler(WmsApp *pMe);

// WMSST_VIEWVOICEMAIL ״̬������
static NextFSMAction WMSST_VIEWVOICEMAIL_Handler(WmsApp *pMe);

// WMSST_VMNUMALERT ״̬������
static NextFSMAction WMSST_VMNUMALERT_Handler(WmsApp *pMe);

// WMSST_VMNUM ״̬������
static NextFSMAction WMSST_VMNUM_Handler(WmsApp *pMe);

// WMSST_VOICEMAILOPTS ״̬������
static NextFSMAction WMSST_VOICEMAILOPTS_Handler(WmsApp *pMe);

#ifdef FEATURE_CONTACT_APP
// WMSST_EXTARCTDETAILS ״̬������
static NextFSMAction WMSST_EXTARCTDETAILS_Handler(WmsApp *pMe);

// WMSST_EXTARCTEDITEMLIST ״̬������
static NextFSMAction WMSST_EXTARCTEDITEMLIST_Handler(WmsApp *pMe);

// WMSST_EXTARCTEDITEMOPT ״̬������
static NextFSMAction WMSST_EXTARCTEDITEMOPT_Handler(WmsApp *pMe);
#endif

// WMSST_NONE ״̬������
static NextFSMAction WMSST_NONE_Handler(WmsApp *pMe);

// WMSST_INIT ״̬������
static NextFSMAction WMSST_INIT_Handler(WmsApp *pMe);

// WMSST_MAIN ״̬������
static NextFSMAction WMSST_MAIN_Handler(WmsApp *pMe);

// WMSST_CHKPWD ״̬������
static NextFSMAction WMSST_CHKPWD_Handler(WmsApp *pMe);

// WMSST_INBOXES ״̬������
static NextFSMAction WMSST_INBOXES_Handler(WmsApp *pMe);

// WMSST_VIEWINBOXMSG ״̬������
static NextFSMAction WMSST_VIEWINBOXMSG_Handler(WmsApp *pMe);

// WMSST_TONUMLIST ״̬������
static NextFSMAction WMSST_TONUMLIST_Handler(WmsApp *pMe);

// WMSST_WRITEMSG ״̬������
static NextFSMAction WMSST_WRITEMSG_Handler(WmsApp *pMe);

// WMSST_SENDOPTS ״̬������
static NextFSMAction WMSST_SENDOPTS_Handler(WmsApp *pMe);

// WMSST_SENDING ״̬������
static NextFSMAction WMSST_SENDING_Handler(WmsApp *pMe);

// WMSST_OUTBOX ״̬������
static NextFSMAction WMSST_OUTBOX_Handler(WmsApp *pMe);

// WMSST_VIEWOUTBOXMSG ״̬������
static NextFSMAction WMSST_VIEWOUTBOXMSG_Handler(WmsApp *pMe);

// WMSST_DRAFT ״̬������
static NextFSMAction WMSST_DRAFT_Handler(WmsApp *pMe);

// WMSST_VIEWDRAFTMSG ״̬������
static NextFSMAction WMSST_VIEWDRAFTMSG_Handler(WmsApp *pMe);

// WMSST_DRAFTMSGOPTS ״̬������
static NextFSMAction WMSST_DRAFTMSGOPTS_Handler(WmsApp *pMe);

// WMSST_TEMPLATES ״̬������
static NextFSMAction WMSST_TEMPLATES_Handler(WmsApp *pMe);

// WMSST_TEMPLATESOPTS ״̬������
static NextFSMAction WMSST_TEMPLATESOPTS_Handler(WmsApp *pMe);

// WMSST_VIEWTEMPLATE ״̬������
static NextFSMAction WMSST_VIEWTEMPLATE_Handler(WmsApp *pMe);

// WMSST_EDITTEMPLATE ״̬������
static NextFSMAction WMSST_EDITTEMPLATE_Handler(WmsApp *pMe);

// WMSST_DELETEMSGS ״̬������
static NextFSMAction WMSST_DELETEMSGS_Handler(WmsApp *pMe);

// WMSST_DELMSGCONFIRM ״̬������
static NextFSMAction WMSST_DELMSGCONFIRM_Handler(WmsApp *pMe);

// WMSST_MSGSETTING ״̬������
static NextFSMAction WMSST_MSGSETTING_Handler(WmsApp *pMe);

// WMSST_RESERVEDMSG ״̬������
static NextFSMAction WMSST_RESERVEDMSG_Handler(WmsApp *pMe);

// WMSST_RSVDMSGTIME ״̬������
static NextFSMAction WMSST_RSVDMSGTIME_Handler(WmsApp *pMe);

// WMSST_RESERVEDMSGS ״̬������
static NextFSMAction WMSST_RESERVEDMSGS_Handler(WmsApp *pMe);

// WMSST_VIEWRESERVEDMSG ״̬������
static NextFSMAction WMSST_VIEWRESERVEDMSG_Handler(WmsApp *pMe);

// WMSST_RESERVEDMSGOPT ״̬������
static NextFSMAction WMSST_RESERVEDMSGOPT_Handler(WmsApp *pMe);

// WMSST_RESERVEDMSGALERT ״̬������
static NextFSMAction WMSST_RESERVEDMSGALERT_Handler(WmsApp *pMe);

// WMSST_AUTOSAVE  ״̬������
static NextFSMAction WMSST_AUTOSAVE_Handler(WmsApp *pMe);

// WMSST_PRIORITY  ״̬������
static NextFSMAction WMSST_PRIORITY_Handler(WmsApp *pMe);

// WMSST_REPORTS  ״̬������
static NextFSMAction WMSST_REPORTS_Handler(WmsApp *pMe);

#ifdef FEATURE_AUTOREPLACE
// WMSST_AUTOREPLACE  ״̬������
static NextFSMAction WMSST_AUTOREPLACE_Handler(WmsApp *pMe);
#endif

//WMSST_SENDMODE  ״̬������
static NextFSMAction WMSST_SENDMODE_Handler(WmsApp *pMe);

// WMSST_CALLBACKNUMSWITCH  ״̬������
static NextFSMAction WMSST_CALLBACKNUMSWITCH_Handler(WmsApp *pMe);

// WMSST_CALLBACKNUMSET  ״̬������
static NextFSMAction WMSST_CALLBACKNUMSET_Handler(WmsApp *pMe);

#ifdef FEATURE_CDSMS_RUIM
// WMSST_STORAGE  ״̬������
static NextFSMAction WMSST_STORAGE_Handler(WmsApp *pMe);

// WMSST_MSGCOPYCONFIRM  ״̬������
static NextFSMAction WMSST_MSGCOPYCONFIRM_Handler(WmsApp *pMe);

// WMSST_MSGCOPY  ״̬������
static NextFSMAction WMSST_MSGCOPY_Handler(WmsApp *pMe);
#endif

// WMSST_MSGVALIDITY  ״̬������
static NextFSMAction WMSST_MSGVALIDITY_Handler(WmsApp *pMe);

// WMSST_AUTODELETE  ״̬������
static NextFSMAction WMSST_AUTODELETE_Handler(WmsApp *pMe);

// WMSST_RESERVEDMSGALERTTIMEOUT  ״̬������
static NextFSMAction WMSST_RESERVEDMSGALERTTIMEOUT_Handler(WmsApp *pMe);

// WMSST_EXIT ״̬������
static NextFSMAction WMSST_EXIT_Handler(WmsApp *pMe);

// WMSST_INBOXMSGOPTS ״̬������
static NextFSMAction WMSST_INBOXMSGOPTS_Handler(WmsApp *pMe);

// WMSST_OUTMSGOPTS ״̬������
static NextFSMAction WMSST_OUTMSGOPTS_Handler(WmsApp *pMe);

// WMSST_MEMSTATUS ״̬������
static NextFSMAction WMSST_MEMSTATUS_Handler(WmsApp *pMe);

static NextFSMAction WMSST_MANAGEMENT_Handler(WmsApp *pMe);

static NextFSMAction WMSST_COPYINBOX_Handler(WmsApp *pMe);

static NextFSMAction WMSST_MOVEINBOX_Handler(WmsApp *pMe);

static NextFSMAction WMSST_SELECTFROM_Handler(WmsApp *pMe);

// WMSST_CONTINUESEND_QUERY ״̬������
static NextFSMAction WMSST_CONTINUESEND_QUERY_Handler(WmsApp *pMe);

/*==============================================================================

                                 ��������
                                 
==============================================================================*/
/*==============================================================================
����:
    WmsApp_ProcessState

˵��:
    ���� pMe->m_currState, ������Ӧ��״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
NextFSMAction WmsApp_ProcessState(WmsApp *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;
    
    if (NULL == pMe)
    {
        return retVal;
    }

    if(pMe->m_currState == WMSST_OUTMSGOPTS || 
        pMe->m_currState == WMSST_INBOXMSGOPTS ||
        pMe->m_currState == WMSST_RESERVEDMSGOPT ||
        pMe->m_currState == WMSST_DRAFTMSGOPTS ||
        pMe->m_currState == WMSST_DELMSGCONFIRM ||
        pMe->m_currState == WMSST_MSGCOPYCONFIRM)
    {
        pMe->m_isPopMenu = TRUE;
    }
    else
    {
        pMe->m_isPopMenu = FALSE;
    }
    
    // ����WMS applet״̬��������Ӧ��״̬������
    switch (pMe->m_currState)
    {
        case WMSST_NONE:
            return WMSST_NONE_Handler(pMe);
            
        case WMSST_INIT:
            return WMSST_INIT_Handler(pMe);
            
        case WMSST_MAIN:
            return WMSST_MAIN_Handler(pMe);
            
        case WMSST_CHKPWD:
            return WMSST_CHKPWD_Handler(pMe);

        case WMSST_INBOXES:
            return WMSST_INBOXES_Handler(pMe);
            
        case WMSST_VOICEMAIL:
            return WMSST_VOICEMAIL_Handler(pMe);
            
        case WMSST_VIEWVOICEMAIL:
            return WMSST_VIEWVOICEMAIL_Handler(pMe);
            
        case WMSST_VMNUMALERT:
            return WMSST_VMNUMALERT_Handler(pMe);
            
        case WMSST_VMNUM:
            return WMSST_VMNUM_Handler(pMe);
            
        case WMSST_VOICEMAILOPTS:
            return WMSST_VOICEMAILOPTS_Handler(pMe);
            
        case WMSST_VIEWINBOXMSG:
            return WMSST_VIEWINBOXMSG_Handler(pMe);
    
#ifdef FEATURE_CONTACT_APP
        case WMSST_EXTARCTDETAILS:
            return WMSST_EXTARCTDETAILS_Handler(pMe);
            
        case WMSST_EXTARCTEDITEMLIST:
            return WMSST_EXTARCTEDITEMLIST_Handler(pMe);
            
        case WMSST_EXTARCTEDITEMOPT:
            return WMSST_EXTARCTEDITEMOPT_Handler(pMe);
#endif

        case WMSST_TONUMLIST:
            return WMSST_TONUMLIST_Handler(pMe);
            
        case WMSST_WRITEMSG:
            return WMSST_WRITEMSG_Handler(pMe);
            
        case WMSST_SENDOPTS:
            return WMSST_SENDOPTS_Handler(pMe);
            
        case WMSST_SENDING:
            return WMSST_SENDING_Handler(pMe);
                   
        case WMSST_OUTBOX:
            return WMSST_OUTBOX_Handler(pMe);
            
        case WMSST_VIEWOUTBOXMSG:
            return WMSST_VIEWOUTBOXMSG_Handler(pMe);
            
        case WMSST_DRAFT:
            return WMSST_DRAFT_Handler(pMe);
            
        case WMSST_VIEWDRAFTMSG:
            return WMSST_VIEWDRAFTMSG_Handler(pMe);

        case WMSST_DRAFTMSGOPTS:
            return WMSST_DRAFTMSGOPTS_Handler(pMe);
     
        case WMSST_TEMPLATES:
            return WMSST_TEMPLATES_Handler(pMe);
            
        case WMSST_TEMPLATESOPTS:
            return WMSST_TEMPLATESOPTS_Handler(pMe);
            
        case WMSST_VIEWTEMPLATE:
            return WMSST_VIEWTEMPLATE_Handler(pMe);
            
        case WMSST_EDITTEMPLATE:
            return WMSST_EDITTEMPLATE_Handler(pMe);
            
        case WMSST_DELETEMSGS:
            return WMSST_DELETEMSGS_Handler(pMe);
            
        case WMSST_DELMSGCONFIRM:
            return WMSST_DELMSGCONFIRM_Handler(pMe);
            
        case WMSST_MSGSETTING:
            return WMSST_MSGSETTING_Handler(pMe);
            
        case WMSST_RESERVEDMSG:
            return WMSST_RESERVEDMSG_Handler(pMe);

        case WMSST_RSVDMSGTIME:
            return WMSST_RSVDMSGTIME_Handler(pMe);
            
        case WMSST_RESERVEDMSGS:
            return WMSST_RESERVEDMSGS_Handler(pMe);
            
        case WMSST_VIEWRESERVEDMSG:
            return WMSST_VIEWRESERVEDMSG_Handler(pMe);
            
        case WMSST_RESERVEDMSGOPT:
            return WMSST_RESERVEDMSGOPT_Handler(pMe);
            
        case WMSST_RESERVEDMSGALERT:
            return WMSST_RESERVEDMSGALERT_Handler(pMe);
            
        case WMSST_AUTOSAVE:
            return WMSST_AUTOSAVE_Handler(pMe);
            
        case WMSST_PRIORITY:
            return WMSST_PRIORITY_Handler(pMe);
            
        case WMSST_REPORTS:
            return WMSST_REPORTS_Handler(pMe);
            
#ifdef FEATURE_AUTOREPLACE
        case WMSST_AUTOREPLACE:
            return WMSST_AUTOREPLACE_Handler(pMe);
#endif            
            
        case WMSST_SENDMODE:
            return WMSST_SENDMODE_Handler(pMe);

        case WMSST_CALLBACKNUMSWITCH:
            return WMSST_CALLBACKNUMSWITCH_Handler(pMe);
            
        case WMSST_CALLBACKNUMSET:
            return WMSST_CALLBACKNUMSET_Handler(pMe);
            
#ifdef FEATURE_CDSMS_RUIM
        case WMSST_STORAGE:
            return WMSST_STORAGE_Handler(pMe);
            
        case WMSST_MSGCOPYCONFIRM:
            return WMSST_MSGCOPYCONFIRM_Handler(pMe);

        case WMSST_MSGCOPY:
            return WMSST_MSGCOPY_Handler(pMe);
#endif  

        case WMSST_MSGVALIDITY:
            return WMSST_MSGVALIDITY_Handler(pMe);
            
        case WMSST_AUTODELETE:
            return WMSST_AUTODELETE_Handler(pMe);
            
        case WMSST_RESERVEDMSGALERTTIMEOUT:
            return WMSST_RESERVEDMSGALERTTIMEOUT_Handler(pMe);
            
        case WMSST_INBOXMSGOPTS:
            return WMSST_INBOXMSGOPTS_Handler(pMe);
            
        case WMSST_OUTMSGOPTS:
            return WMSST_OUTMSGOPTS_Handler(pMe);
     
        case WMSST_MEMSTATUS:
            return WMSST_MEMSTATUS_Handler(pMe);
            
        case WMSST_MANAGEMENT:
            return WMSST_MANAGEMENT_Handler(pMe);
            
        case WMSST_COPYINBOX:
            return WMSST_COPYINBOX_Handler(pMe);
            
        case WMSST_MOVEINBOX:
            return WMSST_MOVEINBOX_Handler(pMe);
            
        case WMSST_CONTINUESEND_QUERY:
            return WMSST_CONTINUESEND_QUERY_Handler(pMe);

        case WMSST_SELECTFROM:
            return WMSST_SELECTFROM_Handler(pMe);
            
        case WMSST_EXIT:
            return WMSST_EXIT_Handler(pMe);
            
        default:
            break;
    }

    return retVal;
}

/*==============================================================================
����:
    WMSST_NONE_Handler

˵��:
    WMSST_NONE ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_NONE_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(WMSST_EXIT);
    return NFSMACTION_CONTINUE;
}   // WMSST_NONE_Handler

/*==============================================================================
����:
    WMSST_INIT_Handler

˵��:
    WMSST_INIT ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_INIT_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            pMe->m_ePMsgType = MESSAGE_WARNNING;
            WmsApp_ShowMsgBox(pMe, IDS_NOTREADY);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_INIT

/*==============================================================================
����:
    WMSST_MAIN_Handler

˵��:
    WMSST_MAIN ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_MAIN_Handler(WmsApp *pMe)
{
    boolean  bsmslock = FALSE;
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_SMS_LOCK_CHECK,
                           &bsmslock,
                           sizeof(bsmslock));
    bsmslock = FALSE; //�����Ӳ˵������ܣ���mainmenu����ʱ���ܾͺ���

    pMe->m_wPrevMenuSel = 0 ;

    switch (pMe->m_eDlgReturn)
    {
        // ����������
        case DLGRET_CREATE:
            // ����������Ҫ�ı������ù���
            WmsApp_MemberReset(pMe);
            
            // �ṹ��������
            WMSMessageStruct_Reset(&pMe->m_msSend);
            
#if defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
            pMe->m_eMBoxType = WMS_MB_VOICEMAIL;
            MOVE_TO_STATE(WMSST_VOICEMAIL)
            return NFSMACTION_CONTINUE;
#else            
            WmsApp_ShowDialog(pMe, IDD_MAIN);
            return NFSMACTION_WAIT;
#endif            
            
        // ��Ϣ�Ի��򷵻�
        case DLGRET_MSGBOX_OK:
            if (pMe->m_wMsgResID != IDS_EMPTY)
            {
                bsmslock = FALSE;
                switch (pMe->m_eMBoxType)
                {
                    case WMS_MB_OUTBOX:
                        MOVE_TO_STATE(WMSST_OUTBOX)
                        if (bsmslock)
                        {
                            pMe->m_stchkpwdbk = pMe->m_currState;
                            pMe->m_currState = WMSST_CHKPWD;
                        }
                        return NFSMACTION_CONTINUE;
                    
                    case WMS_MB_INBOX:
                        MOVE_TO_STATE(WMSST_INBOXES)
                        if (bsmslock)
                        {
                            pMe->m_stchkpwdbk = pMe->m_currState;
                            pMe->m_currState = WMSST_CHKPWD;
                        }
                        return NFSMACTION_CONTINUE;
                        
                    case WMS_MB_DRAFT:
                        MOVE_TO_STATE(WMSST_DRAFT)
                        if (bsmslock)
                        {
                            pMe->m_stchkpwdbk = pMe->m_currState;
                            pMe->m_currState = WMSST_CHKPWD;
                        }
                        return NFSMACTION_CONTINUE;
                        
                    default:
                        break;
                }
            }
            WmsApp_ShowDialog(pMe, IDD_MAIN);
            return NFSMACTION_WAIT;

        // �û���������ѡ��-- �ռ���(RUIM+Phone)
        case DLGRET_INBOXES:
            bsmslock = FALSE;
            pMe->m_eMBoxType = WMS_MB_INBOX;
            {
                uint16  nMsgID = 0;
                
                nMsgID = WmsApp_GetmemAlertID(pMe, WMS_MB_INBOX);
                if (nMsgID != 0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, nMsgID);
                    return NFSMACTION_WAIT;
                }
            }
            MOVE_TO_STATE(WMSST_INBOXES)
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE;
        
        
        // �û���������ѡ��-- �½�����Ϣ
        case DLGRET_WRITEMSG:
            pMe->m_bTextFullAlert = FALSE;
            bsmslock = FALSE;
            // ���Ⱥ����ַ����
            WmsApp_FreeMultiSendList(pMe->m_pSendList);
            pMe->m_eCreateWMSType = SEND_MSG_NEW;
            
            pMe->m_STSwitchToEditMsg = pMe->m_currState;
            pMe->m_dwInsertPos = 0;
            MOVE_TO_STATE(WMSST_WRITEMSG) 
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE;
            

        // �û���������ѡ��-- ������
        case DLGRET_OUTBOX:
            bsmslock = FALSE;
            pMe->m_eMBoxType = WMS_MB_OUTBOX;
            {
                uint16  nMsgID=0;
                
                nMsgID = WmsApp_GetmemAlertID(pMe, WMS_MB_OUTBOX);
                if (nMsgID != 0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, nMsgID);
                    return NFSMACTION_WAIT;
                }
            }
            MOVE_TO_STATE(WMSST_OUTBOX)
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE;
            
        // �û���������ѡ��-- ��������
        case DLGRET_VIEWVOICEMSG:
            bsmslock = FALSE;
            pMe->m_eMBoxType = WMS_MB_VOICEMAIL;
            MOVE_TO_STATE(WMSST_VOICEMAIL)
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE;
            
        // �û���������ѡ��-- �ݸ���
        case DLGRET_DRAFT:
            bsmslock = FALSE;
            pMe->m_eMBoxType = WMS_MB_DRAFT;
            {
                uint16  nMsgID=0;
                
                nMsgID = WmsApp_GetmemAlertID(pMe, WMS_MB_DRAFT);
                if (nMsgID != 0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, nMsgID);
                    return NFSMACTION_WAIT;
                }
            }
            
            MOVE_TO_STATE(WMSST_DRAFT)
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE;

        // �û���������ѡ��-- ��Ϣģ��
        case DLGRET_TEMPLATES:
            bsmslock = FALSE;
            pMe->m_eInsertType = INSERT_NONE;
            pMe->m_eMBoxType = WMS_MB_TEMPLATE;
            MOVE_TO_STATE(WMSST_TEMPLATES)
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE;
            
        // �û���������ѡ��-- �洢��״̬
        case DLGRET_MEMSTATUS:
            MOVE_TO_STATE(WMSST_MEMSTATUS)
            return NFSMACTION_CONTINUE;
            
        //�û���������ѡ��--��Ϣ����    
        case DLGRET_MANAGEMENT:
            MOVE_TO_STATE(WMSST_MANAGEMENT)
            return NFSMACTION_CONTINUE;


        // �û���������ѡ��-- ɾ����Ϣ
        case DLGRET_DELETEMSGS:
            bsmslock = FALSE;
            {
                uint16  nInMsgs=0;
                uint16  nOutMsgs=0;
                uint16  nDraftMsgs=0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, NULL, &nInMsgs);
                wms_cacheinfolist_getcounts(WMS_MB_OUTBOX, NULL, NULL, &nOutMsgs);
                wms_cacheinfolist_getcounts(WMS_MB_DRAFT, NULL, NULL, &nDraftMsgs);

                if ((nInMsgs+nOutMsgs+nDraftMsgs)==0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
            }
                
            MOVE_TO_STATE(WMSST_DELETEMSGS)
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE;

        // �û���������ѡ��-- ��Ϣ����
        case DLGRET_SETTINGS:
            bsmslock = FALSE;
            MOVE_TO_STATE(WMSST_MSGSETTING)
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE;

        // �û���������ѡ��-- ԤԼ��Ϣ
        case DLGRET_RESERVEDMSG:
            bsmslock = FALSE;
            MOVE_TO_STATE(WMSST_RESERVEDMSG)
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_MAIN_Handler

/*==============================================================================
����:
    WMSST_CHKPWD_Handler

˵��:
    WMSST_CHKPWD ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_CHKPWD_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    pMe->m_wPrevMenuSel = 0 ;

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_MSGBOX_OK:
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_PWD);
            return NFSMACTION_WAIT;

        // �����������
        case DLGRET_PWDERR:
            pMe->m_ePMsgType = MESSAGE_WARNNING;
            WmsApp_ShowMsgBox(pMe, IDS_PWDERR);
            return NFSMACTION_WAIT;
            
        // ����������ȷ
        case DLGRET_PWDOK:
            if (pMe->m_bNeedContinueSend)
            {
                pMe->m_currState = WMSST_CONTINUESEND_QUERY;
                pMe->m_stcontinuesendbk = pMe->m_stchkpwdbk;
            }
            else
            {
                pMe->m_currState = pMe->m_stchkpwdbk;
            }
                
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            if (pMe->m_stchkpwdbk == WMSST_RESERVEDMSGALERT)
            {
                // ����ԤԼ���Ŷ�ʱ��
                WmsApp_ReservedMsgSetTimer(pMe);
            }
            
            if (pMe->m_bNaturalStart == FALSE)
            {
                MOVE_TO_STATE(WMSST_EXIT)
            }
            else
            {
                MOVE_TO_STATE(WMSST_EXIT)
            }
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_CHKPWD_Handler

/*==============================================================================
����:
    WMSST_INBOXES_Handler

˵��:
    WMSST_INBOXES ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_INBOXES_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
        case DLGRET_LOADCANCELED:
        case DLGRET_LOADFAILED:
            pMe->m_eMBoxType = WMS_MB_INBOX;
            WmsApp_ShowDialog(pMe, IDD_MESSAGELIST);
            return NFSMACTION_WAIT;

        case DLGRET_LOAD:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_WAIT;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_LOADOK:
            if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
            {
                MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
            }
            else
            {
                MOVE_TO_STATE(WMSST_INBOXMSGOPTS)
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OPT:
            pMe->m_eOptType = OPT_VIA_LISTMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_INBOXES_Handler

/*==============================================================================
����:
    WMSST_VIEWINBOXMSG_Handler

˵��:
    WMSST_VIEWINBOXMSG ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VIEWINBOXMSG_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_VIEWMSG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_INBOXES)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            MOVE_TO_STATE(WMSST_INBOXMSGOPTS)
            return NFSMACTION_CONTINUE;

        case DLGRET_INFO:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            pMe->m_eDlgReturn = DLGRET_REPLY;
            pMe->m_bDoNotOverwriteDlgResult = TRUE;
            MOVE_TO_STATE(WMSST_INBOXMSGOPTS)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VIEWINBOXMSG_Handler

/*==============================================================================
����:
    WMSST_INBOXMSGOPTS_Handler

˵��:
    WMSST_INBOXMSGOPTS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_INBOXMSGOPTS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_MSGOPTS);
            return NFSMACTION_WAIT;

        case DLGRET_VIEW:
            // ������Ϣ�Ѷ���������û�޸���Ϣ��ǣ����貹δ������
            {
                wms_cache_info_node  *pnode = NULL;
                uint16 wIndex=pMe->m_wCurindex;
                
                // ȡ��Ϣ cache info �ڵ�
                if (wIndex>=RUIM_MSGINDEX_BASE)
                {
                    wIndex = wIndex - RUIM_MSGINDEX_BASE;
                    pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_RUIM, wIndex);
                }
                else
                {
                    pnode = wms_cacheinfolist_getnode(WMS_MB_INBOX, WMS_MEMORY_STORE_NV_CDMA, wIndex);
                }
                
                if (NULL != pnode)
                {
#ifdef FEATURE_SMS_UDH
                    if (pnode->pItems != NULL)
                    {
                        int i;
                        
                        for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                        {
                            pnode = pMe->m_CurMsgNodes[i];
                            
                            if ((NULL != pnode) &&
                                (pnode->msg_tag == WMS_TAG_MT_NOT_READ))
                            {
                                (void)IWMS_MsgModifyTag(pMe->m_pwms,
                                                        pMe->m_clientId,
                                                        &pMe->m_callback,
                                                        (void *)pMe,
                                                        pnode->mem_store,
                                                        pnode->index,
                                                        WMS_TAG_MT_READ);
                            }
                        }
                    }
                    else
#endif                        
                    {
                        (void)IWMS_MsgModifyTag(pMe->m_pwms,
                                                pMe->m_clientId,
                                                &pMe->m_callback,
                                                (void *)pMe,
                                                pnode->mem_store,
                                                pnode->index,
                                                WMS_TAG_MT_READ);
                    }
                }
            }
                
            MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
            {
                MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
            }
            else
            {
                MOVE_TO_STATE(WMSST_INBOXES)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_REPLY:
            {// ���ڻظ��������ٽ����ַ�б��������
                AECHAR  *pwstrNum = NULL;
                CMultiSendItemInfo *pItem = NULL;
                
                // �����Ⱥ����ַ����
                WmsApp_FreeMultiSendList(pMe->m_pSendList);
                
                // �����ظ���ַ 
#if defined(FEATURE_CARRIER_VENEZUELA_MOVILNET)
                // ����Ӫ��Ҫ��ظ�����ʱ CBN ����
                if (WSTRLEN(pMe->m_msCur.m_szCallBkNum)>0)
                {
                    pwstrNum = pMe->m_msCur.m_szCallBkNum;
                }
                else if (WSTRLEN(pMe->m_msCur.m_szNum)>0)
                {
                    pwstrNum = pMe->m_msCur.m_szNum;
                }
#else
                // ��Ӫ��Ҫ��ظ�����ʱȱʡ��ַȡOriginating Address
                if (WSTRLEN(pMe->m_msCur.m_szNum)>0)
                {
                    pwstrNum = pMe->m_msCur.m_szNum;
                }
                else if (WSTRLEN(pMe->m_msCur.m_szCallBkNum)>0)
                {
                    pwstrNum = pMe->m_msCur.m_szCallBkNum;
                }
#endif                
                
                if (NULL != pwstrNum)
                { 
                    pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
                }
                    
                // ���ظ����뱣��������
                if ((pItem == NULL) || 
                    (SUCCESS != IVector_AddElement(pMe->m_pSendList, pItem)))
                {// �պ����޷��ظ�
                    if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_INBOXES)
                    }
                    if (NULL != pItem)
                    {
                        sys_free(pItem);
                    }
                    
                    return NFSMACTION_CONTINUE;
                }
                (void)WSTRCPY(pItem->m_szTo, pwstrNum);
                pMe->m_CurAddID = MSG_CMD_BASE;
                
                // �ӵ绰����ȡ����, ������ʾ
                WMSUtil_GetContactName(pMe, pItem->m_szTo, pItem->m_szName, MAX_TITLE_LEN);
            }
            
            pMe->m_STSwitchToEditMsg = pMe->m_currState;
            pMe->m_eCreateWMSType = SEND_MSG_REPLY;
            pMe->m_dwInsertPos = 0;
            
            // �ṹ��������
            WMSMessageStruct_Reset(&pMe->m_msSend);
            
            MOVE_TO_STATE(WMSST_WRITEMSG)
            return NFSMACTION_CONTINUE;

        case DLGRET_FORWARD:
            // ���Ⱥ����ַ����
            WmsApp_FreeMultiSendList(pMe->m_pSendList);

            // ������Ϣ
            WmsApp_CopyCurMsgForSend(pMe);
            
            pMe->m_STSwitchToEditMsg = pMe->m_currState;
            pMe->m_eCreateWMSType = SEND_MSG_FORWARD;
            pMe->m_dwInsertPos = 0;
            MOVE_TO_STATE(WMSST_WRITEMSG)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_DELETE:
            pMe->m_eEraseWMSType = ERASE_INBOXES_ONE;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_CLEARALL:
            pMe->m_eEraseWMSType = CLEAR_INBOXES;
            pMe->m_STSwitchToEditMsg = WMSST_MAIN;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;
#ifdef FEATURE_CONTACT_APP
        case DLGRET_SAVENUM:
            pMe->m_ExtractType = EXTRACT_NUM;
            WmsApp_FreeSaveNumList(pMe);
            
            // ������Ϣ������������
            WmsApp_BuildNumList(pMe, &pMe->m_msCur);
            
            if (IVector_Size(pMe->m_pSaveNumList) < 1)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_NOVALIDNUM);
                return NFSMACTION_WAIT;
            }
            MOVE_TO_STATE(WMSST_EXTARCTEDITEMLIST)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_SAVEEMAIL:
            pMe->m_ExtractType = EXTRACT_EMAILADD;
            WmsApp_FreeSaveNumList(pMe);
            
            // ������Ϣ�������ʵ�ַ����
            WmsApp_BuildMailAddList(pMe, pMe->m_msCur.m_szMessage);
            
            if (IVector_Size(pMe->m_pSaveNumList) < 1)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_NOVALIDMAILADD);
                return NFSMACTION_WAIT;
            }
            MOVE_TO_STATE(WMSST_EXTARCTEDITEMLIST)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_SAVEURL:
            pMe->m_ExtractType = EXTRACT_URL;
            WmsApp_FreeSaveNumList(pMe);
            
            // ������Ϣ����URL��
            WmsApp_BuildUrlList(pMe, pMe->m_msCur.m_szMessage);
            
            if (IVector_Size(pMe->m_pSaveNumList) < 1)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_NOVALIDURL);
                return NFSMACTION_WAIT;
            }
            MOVE_TO_STATE(WMSST_EXTARCTEDITEMLIST)
            return NFSMACTION_CONTINUE;
        case DLGRET_EXTACTDETAILS:
            MOVE_TO_STATE(WMSST_EXTARCTDETAILS)
            return NFSMACTION_CONTINUE;
#endif            
         
#ifdef FEATURE_CDSMS_RUIM
        case DLGRET_COPYTORUIM:
        case DLGRET_MOVETORUIM:
            {
                int nCopys = 0;
                int i;
                
                for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                {
                    if ((pMe->m_CurMsgNodes[i] != NULL) &&
                        (pMe->m_CurMsgNodes[i]->mem_store != WMS_MEMORY_STORE_RUIM))
                    {
                        nCopys++;
                    }
                }
                if (pMe->m_memruimStatuse.used_tag_slots+nCopys > pMe->m_memruimStatuse.max_slots)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_INSUFFICIENCY);
                    return NFSMACTION_WAIT;
                }
            }
            
            if (pMe->m_eDlgReturn == DLGRET_COPYTORUIM)
            {
                pMe->m_CopyType = COPYTORUIM;
            }
            else
            {
                pMe->m_CopyType = MOVETORUIM;
            }
            
            MOVE_TO_STATE(WMSST_MSGCOPYCONFIRM)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_COPYTOPNONE:
        case DLGRET_MOVETOPNONE:
            {
                uint16 nCopys = 0;
                uint16 nMsgs = 0;
                uint16  nOnUIMs = 0;
                
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, &nOnUIMs, &nMsgs);
                nMsgs -= nOnUIMs;
                
                {
                    int i;
                    
                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                    {
                        if ((pMe->m_CurMsgNodes[i] != NULL) &&
                            (pMe->m_CurMsgNodes[i]->mem_store != WMS_MEMORY_STORE_NV_CDMA))
                        {
                            nCopys++;
                        }
                    }
                }

                if (nMsgs+nCopys > IN_MAX)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_INSUFFICIENCY);
                    return NFSMACTION_WAIT;
                }
            }
            
            if (pMe->m_eDlgReturn == DLGRET_COPYTOPNONE)
            {
                pMe->m_CopyType = COPYTOPHONEINBOXE;
            }
            else
            {
                pMe->m_CopyType = MOVETOPHONEINBOX;
            }
            
            MOVE_TO_STATE(WMSST_MSGCOPYCONFIRM)
            return NFSMACTION_CONTINUE;
#endif /* FEATURE_CDSMS_RUIM */

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_INBOXMSGOPTS_Handler

#ifdef FEATURE_CDSMS_RUIM
/*==============================================================================
����:
    WMSST_MSGCOPYCONFIRM_Handler

˵��:
    WMSST_MSGCOPYCONFIRM ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_MSGCOPYCONFIRM_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            if ((COPYTORUIM == pMe->m_CopyType) ||
#if defined(FEATURE_CDSMS_RUIM)
                (COPYALLMTTORUIM == pMe->m_CopyType) ||
                (COPYALLRUIMTOPHONE == pMe->m_CopyType) ||
#endif
                (COPYTOPHONEINBOXE == pMe->m_CopyType))
            {
                pMe->m_wMsgResID = IDS_SURECOPY;
            }
            else
            {
                pMe->m_wMsgResID = IDS_SUREMOVE;
            }
            WmsApp_ShowDialog(pMe, IDD_CONFIRM);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            if(pMe->m_prevState == WMSST_INBOXMSGOPTS)
            {
                if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
                {
                    MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
                }
                else
                {
                    MOVE_TO_STATE(WMSST_INBOXES)
                }
            }
            else
            {
                MOVE_TO_STATE(pMe->m_prevState)
            }
            return NFSMACTION_CONTINUE;
    
        case DLGRET_OK:
            MOVE_TO_STATE(WMSST_MSGCOPY)
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
            
    return NFSMACTION_CONTINUE;
} // WMSST_MSGCOPYCONFIRM_Handler

/*==============================================================================
����:
    WMSST_MSGCOPY_Handler

˵��:
    WMSST_MSGCOPY ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_MSGCOPY_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_MSGCOPY);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            // ���Ƿ���Ҫ��ʾ�ռ���,�ڴ�����������ռ���ǰ�����˼��
#if defined(FEATURE_CDSMS_RUIM)
            if (COPYALLMTTORUIM == pMe->m_CopyType ||
                COPYALLRUIMTOPHONE == pMe->m_CopyType)
            {
                pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
                WmsApp_ShowMsgBox(pMe, IDS_COPIED);
                return NFSMACTION_WAIT;
            }
            else if(MOVEALLMTTORUIM == pMe->m_CopyType ||
                    MOVEALLRUIMTOPHONE == pMe->m_CopyType)
            {
                pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
                WmsApp_ShowMsgBox(pMe, IDS_MOVED);
                return NFSMACTION_WAIT;
            }
            else
#endif                
            {
                pMe->m_eDlgReturn = DLGRET_INBOXES;
            }
            MOVE_TO_STATE(WMSST_MAIN)
            return NFSMACTION_CONTINUE;

#if defined(FEATURE_CDSMS_RUIM)
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MANAGEMENT)
            return NFSMACTION_CONTINUE;
#endif        

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
            
    return NFSMACTION_CONTINUE;
} // WMSST_MSGCOPY_Handler
#endif
/*==============================================================================
����:
    WMSST_VOICEMAIL_Handler

˵��:
    WMSST_VOICEMAIL ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VOICEMAIL_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            pMe->m_eMBoxType = WMS_MB_VOICEMAIL;
            MEMSET(pMe->m_msSend.m_szNum, 0, sizeof(pMe->m_msSend.m_szNum));
            {
                wms_cache_info_node  *pnode = NULL;
                
                pnode = wms_cacheinfolist_getnode(pMe->m_eMBoxType, WMS_MEMORY_STORE_NV_CDMA, 0);
                if ((NULL != pnode) && (pnode->msg_tag == WMS_TAG_MT_NOT_READ))
                {
                    (void)IWMS_MsgModifyTag(pMe->m_pwms,
                                            pMe->m_clientId,
                                            &pMe->m_callback,
                                            (void *)pMe,
                                            pnode->mem_store,
                                            pnode->index,
                                            WMS_TAG_MT_READ);
                }
            }
            WmsApp_ShowDialog(pMe, IDD_VOICEMAIL);
            return NFSMACTION_WAIT;

        case DLGRET_VIEWVOICEMSG:
            {
                uint16  nMsgID=0;
                
                nMsgID = WmsApp_GetmemAlertID(pMe, WMS_MB_VOICEMAIL);

                if (nMsgID != 0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, nMsgID);
                    return NFSMACTION_WAIT;
                }
            }
            MOVE_TO_STATE(WMSST_VIEWVOICEMAIL)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX_OK:
            WmsApp_ShowDialog(pMe, IDD_VOICEMAIL);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            pMe->m_bCallVMNum = TRUE;
            pMe->m_wMsgResID = IDS_VMNUM_WARNING;
            MOVE_TO_STATE(WMSST_VMNUMALERT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_VMNUM:
            MOVE_TO_STATE(WMSST_VMNUM)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
#if defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
            MOVE_TO_STATE(WMSST_EXIT)
#else
            MOVE_TO_STATE(WMSST_MAIN)
#endif            
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VOICEMAIL_Handler

/*==============================================================================
����:
    WMSST_VIEWVOICEMAIL_Handler

˵��:
    WMSST_VIEWVOICEMAIL ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VIEWVOICEMAIL_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_VIEWMSG);
            return NFSMACTION_WAIT;

        case DLGRET_DELETE:
            pMe->m_eEraseWMSType = ERASE_VOICEMAIL_ONE;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_CALLVMNUM:
            {// ���������������Ƿ����ã���������ֱ�Ӳ���
                AECHAR    wstrVMNum[OEMNV_VOICEMAIL_MAXLEN]={0};
                
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_VOICEMAIL_NUMBER,
                                       wstrVMNum,
                                       sizeof(wstrVMNum));
                
                if (WSTRLEN(wstrVMNum)>0)
                {
                    WMSExtApp_CallPhoneNumber(pMe, wstrVMNum, FALSE);
                    MOVE_TO_STATE(WMSST_VOICEMAIL)
                    return NFSMACTION_WAIT;
                }
                else
                {
                    pMe->m_bCallVMNum = TRUE;
                    pMe->m_wMsgResID = IDS_VMNUM_WARNING;
                    MOVE_TO_STATE(WMSST_VMNUMALERT)
                }
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_INFO:
        case DLGRET_OK:
        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_VOICEMAIL)
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VIEWVOICEMAIL_Handler

/*==============================================================================
����:
    WMSST_VMNUMALERT_Handler

˵��:
    WMSST_VMNUMALERT ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VMNUMALERT_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_CONFIRM);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            MOVE_TO_STATE(WMSST_VMNUM)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            pMe->m_bCallVMNum = FALSE;
            MOVE_TO_STATE(WMSST_VOICEMAIL)
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VMNUMALERT_Handler

/*==============================================================================
����:
    WMSST_VMNUM_Handler

˵��:
    WMSST_VMNUM ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VMNUM_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_VMNUM);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            if (pMe->m_bCallVMNum)
            {// ���������������Ƿ����ã���������ֱ�Ӳ���
                AECHAR    wstrVMNum[OEMNV_VOICEMAIL_MAXLEN]={0};
                
                pMe->m_bCallVMNum = FALSE;
                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_VOICEMAIL_NUMBER,
                                       wstrVMNum,
                                       sizeof(wstrVMNum));
                
                if (WSTRLEN(wstrVMNum)>0)
                {
                    WMSExtApp_CallPhoneNumber(pMe, wstrVMNum, FALSE);
                    MOVE_TO_STATE(WMSST_VOICEMAIL)
                    return NFSMACTION_WAIT;
                }
            }
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
        case DLGRET_CANCELED:
            pMe->m_bCallVMNum = FALSE;
            MOVE_TO_STATE(WMSST_VOICEMAIL)
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VMNUM_Handler


/*==============================================================================
����:
    WMSST_DRAFT_Handler

˵��:
    WMSST_DRAFT ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_DRAFT_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:  
        case DLGRET_LOADCANCELED:
        case DLGRET_LOADFAILED:
            WmsApp_ShowDialog(pMe, IDD_MESSAGELIST);
            return NFSMACTION_WAIT;
            
        case DLGRET_LOAD:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_LOADOK:
            if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
            {
                MOVE_TO_STATE(WMSST_VIEWDRAFTMSG)
            }
            else
            {
                MOVE_TO_STATE(WMSST_DRAFTMSGOPTS)
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OPT:
            pMe->m_eOptType = OPT_VIA_LISTMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_DRAFT_Handler

/*==============================================================================
����:
    WMSST_VIEWDRAFTMSG_Handler

˵��:
 
    WMSST_VIEWDRAFTMSG ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VIEWDRAFTMSG_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_VIEWMSG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_DRAFT)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            MOVE_TO_STATE(WMSST_DRAFTMSGOPTS)
            return NFSMACTION_CONTINUE;

        case DLGRET_INFO:
#if 0            
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            pMe->m_eDlgReturn = DLGRET_DELETE;
            pMe->m_bDoNotOverwriteDlgResult = TRUE;
            MOVE_TO_STATE(WMSST_DRAFTMSGOPTS)
            return NFSMACTION_CONTINUE;
#endif
            // ���Ⱥ����ַ����
            WmsApp_FreeMultiSendList(pMe->m_pSendList);
            
            // ������Ϣ
            WmsApp_CopyCurMsgForSend(pMe);
            
            pMe->m_STSwitchToEditMsg = pMe->m_currState;
            pMe->m_eCreateWMSType = SEND_MSG_DRAFT;
            MOVE_TO_STATE(WMSST_WRITEMSG)
            pMe->m_dwInsertPos = 0;
                
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VIEWDRAFTMSG_Handler


/*==============================================================================
����:
    WMSST_DRAFTMSGOPTS_Handler

˵��:
    
    WMSST_DRAFTMSGOPTS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_DRAFTMSGOPTS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_MSGOPTS);
            return NFSMACTION_WAIT;
            
        case DLGRET_VIEW:
            MOVE_TO_STATE(WMSST_VIEWDRAFTMSG)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
            {
                MOVE_TO_STATE(WMSST_VIEWDRAFTMSG)
            }
            else
            {
                MOVE_TO_STATE(WMSST_DRAFT)
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_EDIT:          // DLGRET_FORWARD
            // ���Ⱥ����ַ����
            WmsApp_FreeMultiSendList(pMe->m_pSendList);

            // ������Ϣ
            WmsApp_CopyCurMsgForSend(pMe);
            
            pMe->m_STSwitchToEditMsg = pMe->m_currState;
            pMe->m_eCreateWMSType = SEND_MSG_DRAFT;
            MOVE_TO_STATE(WMSST_WRITEMSG)
            pMe->m_dwInsertPos = 0;
                
            return NFSMACTION_CONTINUE;

        case DLGRET_DELETE:
            pMe->m_eEraseWMSType = ERASE_DRAFT_ONE;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_CLEARALL:
            pMe->m_eEraseWMSType = CLEAR_DRAFT;
            pMe->m_STSwitchToEditMsg = WMSST_MAIN;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_DRAFTMSGOPTS_Handler

/*==============================================================================
����:
    WMSST_TEMPLATES_Handler

˵��:
    WMSST_TEMPLATES ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_TEMPLATES_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_LOADCANCELED:
        case DLGRET_CREATE:  
            WmsApp_ShowDialog(pMe, IDD_TEMPLATES);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            if (pMe->m_eInsertType == INSERT_EDITWMS)
            {// ����ģ��
                // �����輰ʱ����˱�ǣ������ģ���з������ٲ���ģ�彫�޷�
                // ͨ��ȡ�������˳����ţ�����������ѭ��
                pMe->m_eInsertType = INSERT_NONE;
                MOVE_TO_STATE(WMSST_WRITEMSG)
            }
            else if (pMe->m_eInsertType == INSERT_EDITTEPLATE)
            {// ����ģ��
                // �����輰ʱ����˱��
                pMe->m_eInsertType = INSERT_NONE;
                MOVE_TO_STATE(WMSST_EDITTEMPLATE)
            }
            else
            {
                MOVE_TO_STATE(WMSST_MAIN)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_OPT:
            pMe->m_eOptType = OPT_VIA_LISTMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_CONTINUE;
            
        case DLGRET_LOAD:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_WAIT;
            
        case DLGRET_LOADOK:
            if (pMe->m_eInsertType == INSERT_EDITWMS)
            {// �༭��Ϣʱ����ģ��
                // �����輰ʱ����˱�ǣ������ģ���з������ٲ���ģ�彫�޷�
                // ͨ��ȡ�������˳����ţ�����������ѭ��
                pMe->m_eInsertType = INSERT_NONE;
                MOVE_TO_STATE(WMSST_WRITEMSG)
                
                if (pMe->m_bTextFullAlert)
                {
                    pMe->m_eDlgReturn = DLGRET_TEXTFULL;
                    pMe->m_bTextFullAlert = FALSE;
                    pMe->m_bDoNotOverwriteDlgResult = TRUE;
                }
            }
            else if (pMe->m_eInsertType == INSERT_EDITTEPLATE)
            {// �༭ģ��ʱ����ģ��
                // �����輰ʱ����˱��
                pMe->m_eInsertType = INSERT_NONE;
                MOVE_TO_STATE(WMSST_EDITTEMPLATE)
                
                if (pMe->m_bTextFullAlert)
                {
                    pMe->m_eDlgReturn = DLGRET_TEXTFULL;
                    pMe->m_bTextFullAlert = FALSE;
                    pMe->m_bDoNotOverwriteDlgResult = TRUE;
                }
            }
            else
            {
                if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
                {
                    MOVE_TO_STATE(WMSST_VIEWTEMPLATE)
                }
                else
                {
                    MOVE_TO_STATE(WMSST_TEMPLATESOPTS)
                }
            }
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_TEMPLATES_Handler

/*==============================================================================
����:
    WMSST_TEMPLATESOPTS_Handler

˵��:
    WMSST_TEMPLATESOPTS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_TEMPLATESOPTS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:  
            WmsApp_ShowDialog(pMe, IDD_MSGOPTS);
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_TEMPLATES)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CANCELED:
            if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
            {
                MOVE_TO_STATE(WMSST_VIEWTEMPLATE)
            }
            else
            {
                MOVE_TO_STATE(WMSST_TEMPLATES)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_VIEW:
            MOVE_TO_STATE(WMSST_VIEWTEMPLATE)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_DELETE:
            pMe->m_eEraseWMSType = ERASE_TEMPLATE_ONE;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_ADDNEW:
            {
                uint16    nNew = 0;
                
                wms_cacheinfolist_getcounts(WMS_MB_TEMPLATE, &nNew, NULL, NULL);
                if (nNew+MAX_OEMTEMPLATES>=PHRASE_MAX)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_FULL);
                    return NFSMACTION_WAIT;
                }
            }
            if (NULL != pMe->m_msSend.m_szMessage)
            {
                FREE(pMe->m_msSend.m_szMessage);
                pMe->m_msSend.m_szMessage = NULL;
            }
            pMe->m_dwInsertPos = 0;
            pMe->m_bAddTemplate = TRUE;
            MOVE_TO_STATE(WMSST_EDITTEMPLATE)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_EDIT:
            if(pMe->m_eOptType == OPT_VIA_LISTMSG)
            {
                pMe->m_bAddTemplate = FALSE;
                pMe->m_dwInsertPos = 0;
                MOVE_TO_STATE(WMSST_EDITTEMPLATE)
            }
            else
            {
                pMe->m_eCreateWMSType = SEND_MSG_TEMPLATE;
                pMe->m_STSwitchToEditMsg = WMSST_TEMPLATES;
                MOVE_TO_STATE(WMSST_WRITEMSG)
                pMe->m_dwInsertPos = 0;
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_SEND:
            pMe->m_eCreateWMSType = SEND_MSG_TEMPLATE;
            pMe->m_STSwitchToEditMsg = WMSST_TEMPLATES;
            MOVE_TO_STATE(WMSST_TONUMLIST)//WMSST_WRITEMSG
            pMe->m_dwInsertPos = 0;
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_TEMPLATESOPTS_Handler

/*==============================================================================
����:
    WMSST_VIEWTEMPLATE_Handler

˵��:
    WMSST_VIEWTEMPLATE ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VIEWTEMPLATE_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_VIEWMSG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_TEMPLATES)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            MOVE_TO_STATE(WMSST_TEMPLATESOPTS)
            return NFSMACTION_CONTINUE;
        
        case DLGRET_INFO:
            pMe->m_eDlgReturn = DLGRET_SEND;
            pMe->m_bDoNotOverwriteDlgResult = TRUE;
            MOVE_TO_STATE(WMSST_TEMPLATESOPTS)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VIEWTEMPLATE_Handler


/*==============================================================================
����:
    WMSST_EDITTEMPLATE_Handler

˵��:
    WMSST_EDITTEMPLATE ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_EDITTEMPLATE_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_EDITTEMPLATE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            // ��Ҫ����ʾ��������,��ʾʱ�Ὣ���뱣�������ýṹ��,ȡ��ʱӦ
            // �ص��༭ǰ��״̬
            MOVE_TO_STATE(WMSST_TEMPLATES)
            return NFSMACTION_CONTINUE;

        case DLGRET_SAVE:
            if (pMe->m_msSend.m_szMessage == NULL)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_NULLTXTALERT);
                return NFSMACTION_WAIT;
            }
            
            // �����Ϣ
            if (NULL != pMe->m_pCurCltPhrase)
            {
                FREE(pMe->m_pCurCltPhrase);
                pMe->m_pCurCltPhrase = NULL;
            }
            
            if (pMe->m_bAddTemplate || (pMe->m_wCurTemplate>=(MSG_CMD_BASE+PHRASE_START)))
            {
                pMe->m_pCurCltPhrase = WmsApp_PackPhrase(pMe, &pMe->m_msSend, FALSE);
            }
            else
            {// ����ģ��ֻ֧�ֳ������Ĳ���
                pMe->m_pCurCltPhrase = WmsApp_PackPhrase(pMe, &pMe->m_msSend, TRUE);
            }
            
            // ������Ϣ
            if (NULL != pMe->m_pCurCltPhrase)
            {
                pMe->m_pCurCltPhrase->msg_hdr.tag = WMS_TAG_PHRASE;
                
                if (pMe->m_bAddTemplate)
                {
                    (void)IWMS_MsgWrite(pMe->m_pwms, 
                                pMe->m_clientId, 
                                &pMe->m_callback,
                                (void*)pMe,
                                WMS_WRITE_MODE_INSERT,
                                pMe->m_pCurCltPhrase);
                }
                else
                {
                    if (pMe->m_wCurTemplate>=MSG_CMD_BASE)
                    {// �û�������
                        pMe->m_pCurCltPhrase->msg_hdr.index = pMe->m_wCurTemplate - MSG_CMD_BASE;

                        if (pMe->m_pCurCltPhrase->msg_hdr.index>=PHRASE_START)
                        {
                            (void)IWMS_MsgWrite(pMe->m_pwms, 
                                        pMe->m_clientId, 
                                        &pMe->m_callback,
                                        (void*)pMe,
                                        WMS_WRITE_MODE_REPLACE,
                                        pMe->m_pCurCltPhrase);
                        }
                        else
                        {// ����ģ����Ϣ
                            pMe->m_pCurCltPhrase->msg_hdr.tag = WMS_TAG_MO_TEMPLATE;
                            pMe->m_pCurCltPhrase->msg_hdr.mem_store = WMS_MEMORY_STORE_RUIM;
                            (void)IWMS_MsgWriteTemplate(pMe->m_pwms, 
                                        pMe->m_clientId, 
                                        &pMe->m_callback,
                                        (void*)pMe,
                                        WMS_WRITE_MODE_REPLACE,
                                        pMe->m_pCurCltPhrase);
                        }
                    }
                    else
                    {// ���ó�����
                        switch (pMe->m_wCurTemplate)
                        {
                            case IDS_TEMPLATE1:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START;
                                break;
                                
                            case IDS_TEMPLATE2:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START+1;
                                break;
                                
                            case IDS_TEMPLATE3:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START+2;
                                break;
                                
                            case IDS_TEMPLATE4:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START+3;
                                break;
                                
                            case IDS_TEMPLATE5:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START+4;
                                break;
                                
                            case IDS_TEMPLATE6:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START+5;
                                break;
                                
                            case IDS_TEMPLATE7:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START+6;
                                break;
                                
                            case IDS_TEMPLATE8:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START+7;
                                break;
                                
                            case IDS_TEMPLATE9:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START+8;
                                break;
                                
                            case IDS_TEMPLATE10:
                                pMe->m_pCurCltPhrase->msg_hdr.index = PHRASE_START+9;
                                break;
                                
                            default:
                                break;
                        }
                        
                        (void)IWMS_MsgWrite(pMe->m_pwms, 
                                    pMe->m_clientId, 
                                    &pMe->m_callback,
                                    (void*)pMe,
                                    WMS_WRITE_MODE_REPLACE,
                                    pMe->m_pCurCltPhrase);
                    }
                }
            }
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
        
        case DLGRET_TEXTFULL:
            pMe->m_ePMsgType = MESSAGE_WARNNING;
            WmsApp_ShowMsgBox(pMe, IDS_TEXTFULL);
            return NFSMACTION_WAIT;
        
        // ����ģ������ʾ��Ϣ�Ի��򷵻�ֵ
        case DLGRET_MSGBOX_OK:
            if ((pMe->m_wMsgResID == IDS_TEXTFULL) ||
                (pMe->m_wMsgResID == IDS_NULLTXTALERT))
            {
                WmsApp_ShowDialog(pMe, IDD_EDITTEMPLATE);
                return NFSMACTION_WAIT;
            }
            MOVE_TO_STATE(WMSST_TEMPLATES)
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_EDITTEMPLATE_Handler

/*==============================================================================
����:
    WMSST_TONUMLIST_Handler

˵��:
    WMSST_TONUMLIST ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_TONUMLIST_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_TONUMLIST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            if(pMe->m_STSwitchToEditMsg == WMSST_TEMPLATES)
            {
                if(pMe->m_eOptType == OPT_VIA_VIEWMSG)
                {
                    MOVE_TO_STATE(WMSST_VIEWTEMPLATE);
                }
                else
                {
                    MOVE_TO_STATE(WMSST_TEMPLATES);
                }
            }
            else
            {
//                if (pMe->m_pSendList)
//                {
//                    WmsApp_FreeMultiSendList(pMe->m_pSendList);
//                }
//                pMe->m_CurAddID = MSG_CMD_BASE;
                MOVE_TO_STATE(WMSST_WRITEMSG)
                pMe->m_dwInsertPos = 0;         
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_SENDOPT:
#if defined(FEATURE_CARRIER_THAILAND_HUTCH) 
            if ((pMe->m_eCreateWMSType == SEND_MSG_RESERVE) ||
                (pMe->m_eCreateWMSType == SEND_MSG_EDITRESERVE))
            {
                MOVE_TO_STATE(WMSST_RSVDMSGTIME)
            }
            else
            {
                if (pMe->m_eCreateWMSType == SEND_MSG_DRAFT) 
                {// ɾ����ǰ�ݸ�
                    int i, nRet;
                    wms_cache_info_node  *pnode = NULL;
                    
                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                    {
                        if (pMe->m_CurMsgNodes[i] != NULL)
                        {
                            pnode = pMe->m_CurMsgNodes[i];
                            
                            // ����ɾ����Ϣ����
                            nRet = ENOMEMORY;
                            do
                            {
                                nRet = IWMS_MsgDelete(pMe->m_pwms,
                                                      pMe->m_clientId,
                                                      &pMe->m_callback,
                                                      (void *)pMe,
                                                      pnode->mem_store,
                                                      pnode->index);
                            } while(nRet != SUCCESS);
                            pMe->m_CurMsgNodes[i] = NULL;
                        }
                    }
                }
                
                MOVE_TO_STATE(WMSST_SENDING)
            }
#else
//����Ϊԭ��sendopt�Ĳ�����ȫ���ƶ����˴�������sendopt������д���ŵ�option�˵��� 
#if !defined(FEATURE_CARRIER_THAILAND_HUTCH) 
            if (IVector_Size(pMe->m_pSendList) > 0)
#endif                
            {
                // �����Ϣ
                WmsApp_PrepareUserDataMOList(pMe);
                pMe->m_idxUserdata = 0;
                
                if ((pMe->m_SendOPT != SENDOPT_SEND) &&
                    (pMe->m_eCreateWMSType != SEND_MSG_RESERVE) &&
                    (pMe->m_eCreateWMSType != SEND_MSG_EDITRESERVE))
                {// ��Ϣ��Ҫ���棬�����ȼ��洢�ռ䣬�洢�ռ䲻������ʾ�û����û�����ѡ����ټ���!
                    int32  nItems = 0;
                    uint16  nMsgs=0, nOnUIMs=0, nMax=OUT_MAX;
                    
                    // ��ȡ��Ϣ��
#if defined(FEATURE_CARRIER_THAILAND_HUTCH) 
                    if (pMe->m_SendOPT == SENDOPT_SAVE)
                    {// HUTCH Ҫ���û�ѡ"������"ʱ����Ϣ����ݸ���
                        wms_cacheinfolist_getcounts(WMS_MB_DRAFT, NULL, &nOnUIMs, &nMsgs);
                        nMax = DRAFT_MAX;
                        
                        if (pMe->m_eCreateWMSType == SEND_MSG_DRAFT)
                        {
                            int i;
                            
                            for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                            {
                                if (pMe->m_CurMsgNodes[i] != NULL)
                                {
                                    nMax++;
                                }
                            }
                        }
                    }
                    else
                    {
                        wms_cacheinfolist_getcounts(WMS_MB_OUTBOX, NULL, &nOnUIMs, &nMsgs);
                    }
#else
                    wms_cacheinfolist_getcounts(WMS_MB_OUTBOX, NULL, &nOnUIMs, &nMsgs);
#endif                   
                    
                    nItems = IVector_Size(pMe->m_pUserDataMOList);
                    
                    if ((nItems+nMsgs-nOnUIMs)>nMax)
                    {
                        pMe->m_ePMsgType = MESSAGE_WARNNING;
                        WmsApp_ShowMsgBox(pMe, IDS_STOREINSUFFICIENCY);
                        return NFSMACTION_WAIT;
                    }
                }
                
#if defined(FEATURE_CARRIER_THAILAND_HUTCH) 
                // ���� Hutch �汾, �緢��ѡ��� "Save Only" ���ڷ���ǰһ��״̬����
                if ((pMe->m_eCreateWMSType == SEND_MSG_DRAFT) &&
                    (pMe->m_SendOPT == SENDOPT_SAVE))
#else
                if (pMe->m_eCreateWMSType == SEND_MSG_DRAFT)
#endif                
                {// ɾ����ǰ�ݸ�
                    int i, nRet;
                    wms_cache_info_node  *pnode = NULL;
                    
                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                    {
                        if (pMe->m_CurMsgNodes[i] != NULL)
                        {
                            pnode = pMe->m_CurMsgNodes[i];
                            
                            // ����ɾ����Ϣ����
                            nRet = ENOMEMORY;
                            do
                            {
                                nRet = IWMS_MsgDelete(pMe->m_pwms,
                                                   pMe->m_clientId,
                                                   &pMe->m_callback,
                                                   (void *)pMe,
                                                   pnode->mem_store,
                                                   pnode->index);
                            } while(nRet != SUCCESS);
                            pMe->m_CurMsgNodes[i] = NULL;
                        }
                    }
                }
                
                if (pMe->m_SendOPT == SENDOPT_SAVE)
                {// ����״̬���ؾ��� WMSST_SENDING ״̬
                    wms_client_message_s_type *pClientMsg = NULL;
                    int nRet;
#if !defined(FEATURE_CARRIER_THAILAND_HUTCH) 
                    CMultiSendItemInfo *pItem = NULL;
                    
                    pItem = (CMultiSendItemInfo *)IVector_ElementAt(pMe->m_pSendList, 0);
                    
                    if (NULL != pItem)
                    {
                        // ������ǰ����
                        pMe->m_msSend.m_szNum[0] = 0;
                        (void)WSTRCPY(pMe->m_msSend.m_szNum, pItem->m_szTo);
                    }

                    pClientMsg = WmsApp_GetClientMsgMO(pMe, TRUE);
#else                    
                    pClientMsg = WmsApp_GetClientMsgMO(pMe, FALSE);
#endif
                    
                    while (NULL != pClientMsg)
                    {
                        nRet = ENOMEMORY;
                        
#if defined(FEATURE_CARRIER_THAILAND_HUTCH) 
                        // HUTCH Ҫ���û�ѡ"������"ʱ����Ϣ����ݸ���
                        // Must modify message tag!
                        pClientMsg->msg_hdr.tag = WMS_TAG_MO_DRAFT;
#endif
                        do
                        {
                            nRet = IWMS_MsgWrite(pMe->m_pwms, 
                                                 pMe->m_clientId, 
                                                 &pMe->m_callback,
                                                 (void*)pMe,
                                                 WMS_WRITE_MODE_INSERT,
                                                 pClientMsg);
                        } while(nRet != SUCCESS);
#ifndef WIN32
                        // ����10������ȷ����ʱ��ִ�б�����Ϣ�Ĳ���
                        MSLEEP(10);
#endif                                        
                                            
                        FREE(pClientMsg);
#if defined(FEATURE_CARRIER_THAILAND_HUTCH) 
                        pClientMsg = WmsApp_GetClientMsgMO(pMe, FALSE);
#else
                        pClientMsg = WmsApp_GetClientMsgMO(pMe, TRUE);
#endif                        
                    }
                    pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
                    WmsApp_ShowMsgBox(pMe, IDS_SAVED);
                    return NFSMACTION_WAIT;
                }
                
                if ((pMe->m_eCreateWMSType == SEND_MSG_RESERVE) ||
                    (pMe->m_eCreateWMSType == SEND_MSG_EDITRESERVE))
                {
                    MOVE_TO_STATE(WMSST_RSVDMSGTIME)
                }
                else
                {
                    MOVE_TO_STATE(WMSST_SENDING)
                }
            }
            else
            {// ��ֻ�Ƿ�����ʩ������Ӧִ�е���
                MOVE_TO_STATE(WMSST_EXIT)
            }

#endif            
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_OK:
            if (pMe->m_wMsgResID == IDS_SAVED)
            {
                MOVE_TO_STATE(WMSST_MAIN)
                return NFSMACTION_CONTINUE;
            }
            else if(pMe->m_wMsgResID == IDS_STOREINSUFFICIENCY)
            {
                MOVE_TO_STATE(WMSST_TONUMLIST)
                return NFSMACTION_CONTINUE;
            }
        case DLGRET_SELECTFROMOPT:
            MOVE_TO_STATE(WMSST_SELECTFROM);
            return NFSMACTION_CONTINUE;
        
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_TONUMLIST_Handler

/*==============================================================================
����:
    WMSST_WRITEMSG_Handler

˵��:
    WMSST_WRITEMSG ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_WRITEMSG_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        // �����༭��Ϣ�Ի������
        case DLGRET_CREATE:
            if (!pMe->m_bNaturalStart)
            {
                pMe->m_eCreateWMSType = SEND_MSG_NEW;
            }
            if ((pMe->m_eCreateWMSType != SEND_MSG_EDITRESERVE)&&(pMe->m_prevState != WMSST_SENDOPTS)
                                                                &&(pMe->m_prevState != WMSST_TONUMLIST))
            {
                byte btSendmode;
                byte btPriority;
                boolean report;
                boolean bVal;

                ICONFIG_GetItem(pMe->m_pConfig, CFGI_WMS_SENDMODE, &btSendmode, sizeof(btSendmode));
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_WMS_PRIORITY, &btPriority, sizeof(btPriority));
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_WMS_DELIVERYREPORTS, &report, sizeof(report));
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_WMS_CALLBACKNUMSWITCH, &bVal, sizeof(bVal));

                pMe->m_SendOPT = btSendmode;
                pMe->m_msSend.m_epriority = btPriority;
                pMe->m_msSend.m_bDeliveryReport = report;
                pMe->m_CbVal = bVal;
                if(bVal)
                {
                    AECHAR    wstrNum[WMS_ADDRESS_MAX]={0};
                    
                    (void) ICONFIG_GetItem(pMe->m_pConfig,
                                           CFGI_CALLBACKNUM,
                                           wstrNum,
                                           sizeof(wstrNum));
                    
                    (void)MEMSET(pMe->m_msSend.m_szCallBkNum, 0, sizeof(pMe->m_msSend.m_szCallBkNum));
                    if (WSTRLEN(wstrNum) == 0)
                    {// ȡĬ��ֵ: ת����� IMSI_S
#ifdef FEATURE_CARRIER_VENEZUELA_MOVILNET
                        AECHAR  wstrPrefix1[]={(AECHAR)'1',(AECHAR)'5', (AECHAR)'8', 0};
                        AECHAR  wstrPrefix2[]={(AECHAR)'1',(AECHAR)'9', (AECHAR)'9', 0};
                        AECHAR  wstrPrefix3[5]={0};
                        int     nLen = 0, i;
                        
                        (void) ICONFIG_GetItem(pMe->m_pConfig,
                                               CFGI_IMSI_S,
                                               pMe->m_msSend.m_szCallBkNum,
                                               sizeof(AECHAR) * OEMNV_IMSI_S_LENGTH);
                        nLen = WSTRLEN(pMe->m_msSend.m_szCallBkNum);                       
                        // ��"158"���"0416"
                        // ��"199"���"0419"
                        if (nLen>3 && nLen<MAX_PH_DIGITS)
                        {
                            if (WSTRNCMP(pMe->m_msSend.m_szCallBkNum, wstrPrefix1, 3) == 0)
                            {
                                (void)STRTOWSTR("0416", wstrPrefix3, sizeof(wstrPrefix3));
                            }
                            else if (WSTRNCMP(pMe->m_msSend.m_szCallBkNum, wstrPrefix2, 3) == 0)
                            {
                                // ��"199"���"0426" guoys modified for movilnet change 2008.10.28
                                //(void)STRTOWSTR("0419", wstrPrefix3, sizeof(wstrPrefix3));
                                (void)STRTOWSTR("0426", wstrPrefix3, sizeof(wstrPrefix3));
                            }
                        }
                        if (WSTRLEN(wstrPrefix3)>0)
                        {
                            for (i=nLen; i>3; i--)
                            {
                                pMe->m_msSend.m_szCallBkNum[i] = pMe->m_msSend.m_szCallBkNum[i-1];
                            }
                            pMe->m_msSend.m_szCallBkNum[0] = wstrPrefix3[0];
                            pMe->m_msSend.m_szCallBkNum[1] = wstrPrefix3[1];
                            pMe->m_msSend.m_szCallBkNum[2] = wstrPrefix3[2];
                            pMe->m_msSend.m_szCallBkNum[3] = wstrPrefix3[3];
                            pMe->m_msSend.m_szCallBkNum[nLen+1] = 0;
                        }
#endif                                                   
                    }
                    else
                    {
                        (void)WSTRCPY(pMe->m_msSend.m_szCallBkNum, wstrNum);
                    }
                }                    
            }
            WmsApp_ShowDialog(pMe, IDD_WRITEMSG);
            return NFSMACTION_WAIT;
        case DLGRET_MSGBOX_OK:
            switch(pMe->m_wMsgResID)
            {
                case IDS_INPUTNULL:
                case IDS_TEXTFULL:
                case IDS_FULL:
                case IDS_NULLTXTALERT:
                case IDS_INSUFFICIENCY:
                    MOVE_TO_STATE(WMSST_WRITEMSG)
                    return NFSMACTION_CONTINUE;

                case IDS_SAVED:
                    MOVE_TO_STATE(WMSST_MAIN)
                    return NFSMACTION_CONTINUE;

                default:
                    MOVE_TO_STATE(WMSST_MAIN)
                    return NFSMACTION_CONTINUE;
            }
            return TRUE;

        // �û���������棬ѡ����/����������
        case DLGRET_SEND:
            pMe->m_ContinueSendType = NONE_CONTINUE;
#ifdef FEATURE_CARRIER_THAILAND_HUTCH           
            // Hutch �������հ׶���
            if (NULL == pMe->m_msSend.m_szMessage)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_INPUTNULL); 
                return NFSMACTION_WAIT;
            }
#endif //FEATURE_CARRIER_THAILAND_HUTCH
            if (pMe->m_eCreateWMSType == SEND_MSG_REPLY)
            {// ���ڻظ�����ţ�������׼���ã���������
                MOVE_TO_STATE(WMSST_TONUMLIST)//WMSST_SENDOPT
            }
            else
            {
                pMe->m_nCurAddPage = 0;
                MOVE_TO_STATE(WMSST_TONUMLIST)
            }
            return NFSMACTION_CONTINUE;
            
        // �û�ѡ����볣����
        case DLGRET_INSERTTEMPLATES:
            pMe->m_eInsertType = INSERT_EDITWMS;
            pMe->m_bTextFullAlert = FALSE;
            pMe->m_wCurTemplate = IDS_TEMPLATE1;
            MOVE_TO_STATE(WMSST_TEMPLATES)
            return NFSMACTION_CONTINUE;
            
        // �û�ѡ�񷵻�
        case DLGRET_CANCELED:
            if (pMe->m_bNaturalStart == FALSE)
            {
                pMe->m_STSwitchToEditMsg = WMSST_EXIT;
            }
            switch(pMe->m_STSwitchToEditMsg)
            {
                // ��״̬�ɲ鿴�ռ�����һ����Ϣ�Ĳ���ѡ�����
                case WMSST_INBOXMSGOPTS:
                    if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_INBOXES)
                    }
                    break;
                    
                // ��״̬�ɲ鿴��������һ����Ϣ�Ĳ���ѡ�����
                case WMSST_OUTMSGOPTS:
                    if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWOUTBOXMSG)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_OUTBOX)
                    }
                    break;
                    
                // ��״̬�ɲ鿴�ղؼ���һ����Ϣ�Ĳ���ѡ�����
                case WMSST_VIEWDRAFTMSG:
                case WMSST_DRAFTMSGOPTS:
                    if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWDRAFTMSG)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_DRAFT)
                    }
                    break;

                // ��״̬��ԤԼ��Ϣ���������
                case WMSST_RESERVEDMSG:
                    
                    MOVE_TO_STATE(WMSST_RESERVEDMSG)
                    break;

                // ��״̬�ɲ鿴ԤԼ��Ϣѡ��������
                case WMSST_RESERVEDMSGOPT:                    
                if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
                {
                    MOVE_TO_STATE(WMSST_VIEWRESERVEDMSG)
                }
                else
                {
                    MOVE_TO_STATE(WMSST_RESERVEDMSGS)
                }
                break;                    
                    
#ifdef FEATURE_CONTACT_APP
                // ��״̬�ɱ������ѡ��״̬����
                case WMSST_EXTARCTEDITEMLIST://WMSST_EXTARCTEDITEMOPT:
#endif                    
                    
                // ��״̬��ģ��ѡ�����
                case WMSST_TEMPLATES:
                
                // ��״̬��������ѡ����д��Ϣ����
                case WMSST_MAIN:
                    // ����ǰһ״̬
                    MOVE_TO_STATE(pMe->m_STSwitchToEditMsg)
                    break;
                    
                default:
                    // ���˳������������
                    MOVE_TO_STATE(WMSST_EXIT)
                    break;
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_TEXTFULL:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_TEXTFULL);
            return NFSMACTION_WAIT;

        case DLGRET_SAVE:
            {
                uint16    nNew = 0;
                
                wms_cacheinfolist_getcounts(WMS_MB_TEMPLATE, &nNew, NULL, NULL);
                if (nNew+MAX_OEMTEMPLATES>=PHRASE_MAX)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_FULL);
                    return NFSMACTION_WAIT;
                }
            }            
            if (pMe->m_msSend.m_szMessage == NULL)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_NULLTXTALERT);
                return NFSMACTION_WAIT;
            }

            if(WSTRLEN(pMe->m_msSend.m_szMessage) > MAX_TEMPLATECHARS)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_INSUFFICIENCY);
                return NFSMACTION_WAIT;
            }
            
            // �����Ϣ
            if (NULL != pMe->m_pCurCltPhrase)
            {
                FREE(pMe->m_pCurCltPhrase);
                pMe->m_pCurCltPhrase = NULL;
            }
            pMe->m_pCurCltPhrase = WmsApp_PackPhrase(pMe, &pMe->m_msSend, FALSE);
            
            // ������Ϣ
            if (NULL != pMe->m_pCurCltPhrase)
            {
                pMe->m_pCurCltPhrase->msg_hdr.tag = WMS_TAG_PHRASE;
                

                (void)IWMS_MsgWrite(pMe->m_pwms, 
                            pMe->m_clientId, 
                            &pMe->m_callback,
                            (void*)pMe,
                            WMS_WRITE_MODE_INSERT,
                            pMe->m_pCurCltPhrase);
            }
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;

        case DLGRET_SENDOPT:

            if ((pMe->m_eCreateWMSType == SEND_MSG_RESERVE) ||
                (pMe->m_eCreateWMSType == SEND_MSG_EDITRESERVE))
            {
                pMe->m_wPrevMenuSel = IDC_MENU_SENDOPT;
            }
            else
            {
                pMe->m_wPrevMenuSel = IDC_MENU_SENDOPT;
            }            
            MOVE_TO_STATE(WMSST_SENDOPTS)
            return NFSMACTION_CONTINUE;
        case DLGRET_EXIT_EDITOR:
            pMe->m_ePMsgType = MESSAGE_CONFIRM;
            WmsApp_ShowMsgBox(pMe, IDS_SAVE);
            return NFSMACTION_WAIT;

        case DLGRET_SAVEDRAFT:
            // �û�ѡ�񱣴棬�������뵽�ݸ���
            {
                int32  nItems = 0;
                uint16 nMsgs = 0;
                
                // �ͷ��û������б�
                WmsApp_FreeUserDataMOList(pMe->m_pUserDataMOList);
                
                // �����Ϣ
                WmsApp_PrepareUserDataMOList(pMe);
                pMe->m_idxUserdata = 0;
                
                nItems = IVector_Size(pMe->m_pUserDataMOList);
                
                // ��ȡ�ݸ�����Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_DRAFT, NULL, NULL, &nMsgs);
                
                if ((nMsgs+nItems) <= DRAFT_MAX)
                {// �洢�ռ��㹻�������жϵ����뵽�ݸ���
                    wms_client_message_s_type *pClientMsg = NULL;
                    int nRet;
                    
                    WmsApp_FreeMultiSendList(pMe->m_pSendList);
                    
                    pClientMsg = WmsApp_GetClientMsgMO(pMe, FALSE);
                    while (pClientMsg != NULL)
                    {
                        // Must modify message tag!
                        pClientMsg->msg_hdr.tag = WMS_TAG_MO_DRAFT;
                        
                        // ������Ϣ
                        nRet = ENOMEMORY;
                        do 
                        {
                            nRet = IWMS_MsgWrite(pMe->m_pwms, 
                                                 pMe->m_clientId, 
                                                 &pMe->m_callback,
                                                 (void*)pMe,
                                                 WMS_WRITE_MODE_INSERT,
                                                 pClientMsg);
                        } while(nRet != SUCCESS);
#ifndef WIN32
                        // ����10������ȷ����ʱ��ִ�б�����Ϣ�Ĳ���
                        MSLEEP(10);
#endif                                        
                                            
                        FREE(pClientMsg);
                        pClientMsg = WmsApp_GetClientMsgMO(pMe, FALSE);
                    }
                    
                    WmsApp_FreeMultiSendList(pMe->m_pSendList);
                    
                    // �ͷ��û������б�
                    WmsApp_FreeUserDataMOList(pMe->m_pUserDataMOList);
                    
                    pMe->m_idxUserdata = 0;
                }
                else
                {
                    // �ͷ��û������б�
                    WmsApp_FreeUserDataMOList(pMe->m_pUserDataMOList);
                }
            } 
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;

        case DLGRET_WRITEMSG:
            MOVE_TO_STATE(WMSST_WRITEMSG)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // WMSST_WRITEMSG_Handler

/*==============================================================================
����:
    WMSST_SENDOPTS_Handler

˵��:
    WMSST_SENDOPTS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_SENDOPTS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_SENDOPTS);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_WRITEMSG)//WMSST_TONUMLIST
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            MOVE_TO_STATE(WMSST_WRITEMSG)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX_OK:
            if (pMe->m_wMsgResID == IDS_SAVED)
            {
                MOVE_TO_STATE(WMSST_MAIN)
                return NFSMACTION_CONTINUE;
            }
            
            WmsApp_ShowDialog(pMe, IDD_SENDOPTS);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_SENDOPTS_Handler

/*==============================================================================
����:
    WMSST_SENDING_Handler

˵��:
    WMSST_SENDING ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:
       
==============================================================================*/
static NextFSMAction WMSST_SENDING_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        // ��ʾ������Ϣ״̬��ʾ����
        case DLGRET_CREATE:
            pMe->m_SucNum = 0;
            pMe->m_FailNum = 0;
            WmsApp_ShowDialog(pMe, IDD_SENDING);
            return NFSMACTION_WAIT;

        case DLGRET_END:
            //��Ϣ�������
            if (pMe->m_bMuitiSend)
            {
                pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
                WmsApp_ShowMsgBox(pMe, IDS_SENDDONE);
                return NFSMACTION_WAIT;
            } // ע�⣺�������ִ�� case DLGRET_MSGBOX_OK
        case DLGRET_MSGBOX_OK:
            if (pMe->m_eCreateWMSType == SEND_MSG_RESERVE)
            {
                uint32  dwSecs;
                wms_cache_info_node  *pnode = NULL;
                
                dwSecs = GETTIMESECONDS();
                wms_cacheinfolist_enumbegin(WMS_MB_RESERVE);
                pnode = wms_cacheinfolist_enumnext(WMS_MB_RESERVE);
                if (NULL != pnode)
                {
                    if (pnode->dwTime <= dwSecs)
                    {
                        MOVE_TO_STATE(WMSST_RESERVEDMSGALERT)
                        return NFSMACTION_CONTINUE;
                    }
                    else
                    {
                        WmsApp_ReservedMsgSetTimer(pMe);
                    }
                }
                
                if (pMe->m_bNaturalStart)
                {
                    MOVE_TO_STATE(WMSST_MAIN)
                }
                else
                {
                    MOVE_TO_STATE(WMSST_EXIT)
                }
            }
            else if (pMe->m_stcontinuesendbk != WMSST_NONE)
            {
                if (pMe->m_stcontinuesendbk == WMSST_INBOXES)
                {
                    pMe->m_currState = WMSST_MAIN;
                    pMe->m_eDlgReturn = DLGRET_INBOXES;
                }
                else
                {
                    pMe->m_currState = pMe->m_stcontinuesendbk;
                }
                
                pMe->m_stcontinuesendbk = WMSST_NONE;
            }
            else if (pMe->m_bNaturalStart)
            {
                MOVE_TO_STATE(WMSST_MAIN)
            }
            else
            {
                MOVE_TO_STATE(WMSST_EXIT)
            }

            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_SENDING_Handler

/*==============================================================================
����:
    WMSST_OUTBOX_Handler

˵��:
    WMSST_OUTBOX ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_OUTBOX_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
        case DLGRET_LOADCANCELED:
        case DLGRET_LOADFAILED:
            WmsApp_ShowDialog(pMe, IDD_MESSAGELIST);
            return NFSMACTION_WAIT;
            
        case DLGRET_LOAD:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_LOADOK:
            if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
            {
                MOVE_TO_STATE(WMSST_VIEWOUTBOXMSG)
            }
            else
            {
                MOVE_TO_STATE(WMSST_OUTMSGOPTS)
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OPT:
            pMe->m_eOptType = OPT_VIA_LISTMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_OUTBOX_Handler

/*==============================================================================
����:
    WMSST_VIEWOUTBOXMSG_Handler

˵��:
    WMSST_VIEWOUTBOXMSG ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VIEWOUTBOXMSG_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_VIEWMSG);
           
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_OUTBOX)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            MOVE_TO_STATE(WMSST_OUTMSGOPTS)
            return NFSMACTION_CONTINUE;

        case DLGRET_INFO:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            pMe->m_eDlgReturn = DLGRET_RESEND;
            pMe->m_bDoNotOverwriteDlgResult = TRUE;
            MOVE_TO_STATE(WMSST_OUTMSGOPTS)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VIEWOUTBOXMSG_Handler

/*==============================================================================
����:
    WMSST_OUTMSGOPTS_Handler

˵��:
    WMSST_OUTMSGOPTS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_OUTMSGOPTS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_MSGOPTS);
            return NFSMACTION_WAIT;

        case DLGRET_VIEW:
            MOVE_TO_STATE(WMSST_VIEWOUTBOXMSG)
            return NFSMACTION_CONTINUE;
        
        case DLGRET_CANCELED:
            if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
            {
                MOVE_TO_STATE(WMSST_VIEWOUTBOXMSG)
            }
            else
            {
                MOVE_TO_STATE(WMSST_OUTBOX)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_FORWARD:
            // ���Ⱥ����ַ����
            WmsApp_FreeMultiSendList(pMe->m_pSendList);

            // ������Ϣ
            WmsApp_CopyCurMsgForSend(pMe);
            
            pMe->m_STSwitchToEditMsg = pMe->m_currState;
            pMe->m_eCreateWMSType = SEND_MSG_FORWARD;
            MOVE_TO_STATE(WMSST_WRITEMSG)
            pMe->m_dwInsertPos = 0;
            return NFSMACTION_CONTINUE;

        case DLGRET_DELETE:
            pMe->m_eEraseWMSType = ERASE_OUTBOX_ONE;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_CLEARALL:
            pMe->m_eEraseWMSType = CLEAR_OUTBOX;
            pMe->m_STSwitchToEditMsg = WMSST_MAIN;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_RESEND:
            pMe->m_idxCur = 0;
            
            {// �����ط�������������Ϣ�༭���棬Ҳ�������ַ�б��������
                AECHAR  *pwstrNum = NULL;
                CMultiSendItemInfo *pItem = NULL;
                
                // �����Ⱥ����ַ����
                WmsApp_FreeMultiSendList(pMe->m_pSendList);
                
                // ������ַ 
                if (WSTRLEN(pMe->m_msCur.m_szNum)>0)
                {
                    pwstrNum = pMe->m_msCur.m_szNum;
                }
                else
                {
                    // ״̬������Ǩ��
                    return NFSMACTION_CONTINUE;
                }
                
                pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
                    
                // ���ظ����뱣��������
                if ((pItem == NULL) || 
                    (SUCCESS != IVector_AddElement(pMe->m_pSendList, pItem)))
                {// �պ����޷��ظ�
                    if (NULL != pItem)
                    {
                        sys_free(pItem);
                    }
                    
                    // ״̬������Ǩ��
                    return NFSMACTION_CONTINUE;
                }
                (void)WSTRCPY(pItem->m_szTo, pwstrNum);
                pMe->m_CurAddID = MSG_CMD_BASE;
                
                // �ӵ绰����ȡ����, ������ʾ
                WMSUtil_GetContactName(pMe, pItem->m_szTo, pItem->m_szName, MAX_TITLE_LEN);
            }
            
            pMe->m_eCreateWMSType = SEND_MSG_RESEND;
            MOVE_TO_STATE(WMSST_SENDING)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_OUTMSGOPTS_Handler

/*==============================================================================
����:
    WMSST_DELETEMSGS_Handler

˵��:
    WMSST_DELETEMSGS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_DELETEMSGS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_MSGBOX_OK:
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_DELETEMSGS);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MANAGEMENT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_CLEARINBOXES:
        case DLGRET_CLEAROUTBOX:
        case DLGRET_CLEARDRAFT:
            {
                uint16 wMsgs = 0;
                
                // ��ȡ��Ϣ��
                if (pMe->m_eDlgReturn == DLGRET_CLEARINBOXES)
                {
                    wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, NULL, &wMsgs);
                }
                else if (pMe->m_eDlgReturn == DLGRET_CLEAROUTBOX)
                {
                    wms_cacheinfolist_getcounts(WMS_MB_OUTBOX, NULL, NULL, &wMsgs);
                }
                else
                {
                    wms_cacheinfolist_getcounts(WMS_MB_DRAFT, NULL, NULL, &wMsgs);
                }

                if (wMsgs==0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
            }
            pMe->m_STSwitchToEditMsg = WMSST_MANAGEMENT;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;
            
#ifdef FEATURE_CDSMS_RUIM
        case DLGRET_CLEARRUIM:
            if (pMe->m_memruimStatuse.used_tag_slots == 0)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                return NFSMACTION_WAIT;
            }
            pMe->m_STSwitchToEditMsg = WMSST_MANAGEMENT;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;
#endif

        case DLGRET_CLEARPHONE:
            {
                uint16 wMsgs = 0;
                
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, NULL, &wMsgs);
#ifdef FEATURE_CDSMS_RUIM
                wMsgs -= pMe->m_memruimStatuse.used_tag_slots;
#endif
                if (wMsgs == 0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
            }
            pMe->m_STSwitchToEditMsg = WMSST_MANAGEMENT;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_DELETEREADMSG:
            {
                uint16 wMsgs = 0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, NULL, &wMsgs);
                
                if (wMsgs==0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
            }
            pMe->m_eEraseWMSType = ERASE_READMSG;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_CLEARRESERVED:
            {
                uint16 wMsgs = 0;

                //��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_RESERVE, NULL, NULL, &wMsgs);
                if(wMsgs == 0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
            }
            pMe->m_eEraseWMSType = CLEAR_ALLRESERVED;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;
            
        //ɾ��ȫ����Ϣ
        case DLGRET_CLEARALL:
            {
                uint16  nInMsgs=0;
                uint16  nOutMsgs=0;
                uint16  nDraftMsgs=0;
                uint16  nVoice=0;
                uint16  nReserveMsgs=0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, NULL, &nInMsgs);
                wms_cacheinfolist_getcounts(WMS_MB_OUTBOX, NULL, NULL, &nOutMsgs);
                wms_cacheinfolist_getcounts(WMS_MB_DRAFT, NULL, NULL, &nDraftMsgs);
                wms_cacheinfolist_getcounts(WMS_MB_VOICEMAIL, NULL, NULL, &nVoice);
                wms_cacheinfolist_getcounts(WMS_MB_RESERVE, NULL, NULL, &nReserveMsgs);
                
                if ((nInMsgs+nOutMsgs+nDraftMsgs+nVoice+nReserveMsgs) == 0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
            }
            pMe->m_STSwitchToEditMsg = WMSST_MANAGEMENT;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // WMSST_DELETEMSGS_Handler

/*==============================================================================
����:
    WMSST_DELMSGCONFIRM_Handler

˵��:
    WMSST_DELMSGCONFIRM ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_DELMSGCONFIRM_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            pMe->m_wMsgResID = IDS_DELETE;
            WmsApp_ShowDialog(pMe, IDD_CONFIRM);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            switch(pMe->m_eMBoxType)
            {
                case WMS_MB_INBOX:
                    if(pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_INBOXES)                        
                    }
                    break;
                    
                case WMS_MB_OUTBOX:
                    if(pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWOUTBOXMSG)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_OUTBOX)
                    }
                    break;
                    
                case WMS_MB_DRAFT:
                    if(pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWDRAFTMSG)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_DRAFT)
                    }
                    break;
                    
                case WMS_MB_TEMPLATE:
                    if(pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWTEMPLATE)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_TEMPLATES)
                    }
                    break;

                case WMS_MB_RESERVE:
                    if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWRESERVEDMSG)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_RESERVEDMSGS)
                    }
                    break;

                case WMS_MB_RSVFAILED:
                    if(pMe->m_eOptType == OPT_VIA_VIEWMSG)
                    {
                        MOVE_TO_STATE(WMSST_VIEWRESERVEDMSG)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_RESERVEDMSGS)
                    }
                    break;
                    
                default:
                    MOVE_TO_STATE(pMe->m_prevState)
            }
            return NFSMACTION_CONTINUE;
    
        case DLGRET_OK:
            WmsApp_ShowDialog(pMe, IDD_DELETING);
            return NFSMACTION_WAIT;

        case DLGRET_DONE:
            switch(pMe->m_eEraseWMSType)
            {
                case ERASE_DRAFT_ONE: 
                    {
                        uint16  nMsgs;
                        
                        // ��ȡ��Ϣ��
                        wms_cacheinfolist_getcounts(WMS_MB_DRAFT, NULL, NULL, &nMsgs);
                
                        if (nMsgs == 0)
                        {
                            MOVE_TO_STATE(WMSST_MAIN)
                        }
                        else
                        {
                            MOVE_TO_STATE(WMSST_DRAFT)
                        }
                    }
                    break;
                
                case ERASE_INBOXES_ONE: 
                    {
                        uint16  nMsgs;
                        
                        // ��ȡ��Ϣ��
                        wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, NULL, &nMsgs);
                
                        if (nMsgs == 0)
                        {
                            MOVE_TO_STATE(WMSST_MAIN)
                        }
                        else
                        {
                            MOVE_TO_STATE(WMSST_INBOXES)
                        }
                    }
                    break;
 
                case ERASE_OUTBOX_ONE:
                    {
                        uint16  nMsgs;
                        
                        // ��ȡ��Ϣ��
                        wms_cacheinfolist_getcounts(WMS_MB_OUTBOX, NULL, NULL, &nMsgs);
                
                        if (nMsgs == 0)
                        {
                            MOVE_TO_STATE(WMSST_MAIN)
                        }
                        else
                        {
                            MOVE_TO_STATE(WMSST_OUTBOX)
                        }
                    }
                    break;
                    
                case ERASE_RESERVED_ONE:
                    {
                        uint16 nNews=0, nMsgs=0, nAlert = 0;
                        
                        // ��ȡ��Ϣ��
                        wms_cacheinfolist_getcounts(WMS_MB_RESERVE, &nNews, NULL, &nMsgs);
                        
                        if (pMe->m_eMBoxType == WMS_MB_RESERVE)
                        {
                            WmsApp_ReservedMsgSetTimer(pMe);
                            nAlert = nNews;
                        }
                        else
                        {
                            nAlert = nMsgs-nNews;
                        }
                
                        if (nAlert == 0)
                        {
                            MOVE_TO_STATE(WMSST_RESERVEDMSG)
                        }
                        else
                        {
                            MOVE_TO_STATE(WMSST_RESERVEDMSGS)
                        }
                    }
                    break;
                    
                case ERASE_VOICEMAIL_ONE:
                    MOVE_TO_STATE(WMSST_VOICEMAIL)
                    break;
                    
#ifdef FEATURE_CDSMS_RUIM                	
                case CLEAR_RUIM:
#endif
                case CLEAR_PHONE:
                case CLEAR_INBOXES:
                case CLEAR_OUTBOX:
                case CLEAR_DRAFT:
                //case ERASE_READMSG:
                case CLEAR_ALL:
                    MOVE_TO_STATE(pMe->m_STSwitchToEditMsg)
                    break;
                    
                case CLEAR_RESERVED:
                case CLEAR_RESERVEFAILED:
                    if (pMe->m_eEraseWMSType == CLEAR_RESERVED)
                    {
                        WmsApp_ReservedMsgSetTimer(pMe);
                    }
                    
                    if(pMe->m_prevState == WMSST_DELETEMSGS)
                    {
                        MOVE_TO_STATE(WMSST_DELETEMSGS)
                    }
                    else
                    {
                        MOVE_TO_STATE(WMSST_RESERVEDMSG)
                    }
                    break;

                case CLEAR_ALLRESERVED:
                    WmsApp_ReservedMsgSetTimer(pMe);
                    MOVE_TO_STATE(WMSST_DELETEMSGS)
                    break;
                    
                case ERASE_TEMPLATE_ONE:
                    pMe->m_wCurTemplate = IDS_TEMPLATE1;
                    MOVE_TO_STATE(WMSST_TEMPLATES)
                    break;
                    
                default:
                    MOVE_TO_STATE(WMSST_EXIT)
                    break;
            }
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
            
    return NFSMACTION_CONTINUE;
} // WMSST_DELMSGCONFIRM_Handler

/*==============================================================================
����:
    WMSST_MSGSETTING_Handler

˵��:
    WMSST_MSGSETTING ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_MSGSETTING_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_SETTING);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MAIN)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_SETTINGAUTOSAVE:
            MOVE_TO_STATE(WMSST_AUTOSAVE)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_PRIORITY:
            MOVE_TO_STATE(WMSST_PRIORITY)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_REPORTS:
            MOVE_TO_STATE(WMSST_REPORTS)
            return NFSMACTION_CONTINUE;
            
#ifdef FEATURE_AUTOREPLACE
        case DLGRET_AUTOREPLACE:
            MOVE_TO_STATE(WMSST_AUTOREPLACE)
            return NFSMACTION_CONTINUE;
#endif            
            
        case DLGRET_SENDMODE:
            MOVE_TO_STATE(WMSST_SENDMODE)
            return NFSMACTION_CONTINUE;

        case DLGRET_CALLBACKNUM:
            MOVE_TO_STATE(WMSST_CALLBACKNUMSWITCH)
            return NFSMACTION_CONTINUE;           

        case DLGRET_MSGVALIDITY:
            MOVE_TO_STATE(WMSST_MSGVALIDITY)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_AUTODELETE:
            MOVE_TO_STATE(WMSST_AUTODELETE)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_RESERVEDMSGALERTTIMEOUT:
            MOVE_TO_STATE(WMSST_RESERVEDMSGALERTTIMEOUT)
            return NFSMACTION_CONTINUE; 
        
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_MSGSETTING_Handler

/*==============================================================================
����:
    WMSST_RESERVEDMSG_Handler

˵��:
    WMSST_RESERVEDMSG ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_RESERVEDMSG_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGBOX_OK:
            WmsApp_ShowDialog(pMe, IDD_RESERVEDMSG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MAIN)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_NEWRESERVEDMSG:
            pMe->m_bTextFullAlert = FALSE;
            
            // ���Ⱥ����ַ����
            WmsApp_FreeMultiSendList(pMe->m_pSendList);
            pMe->m_eCreateWMSType = SEND_MSG_RESERVE;
            
            pMe->m_STSwitchToEditMsg = pMe->m_currState;
            MEMSET(&pMe->m_rsvDateTime, 0, sizeof(pMe->m_rsvDateTime));
            // �ṹ��������
            WMSMessageStruct_Reset(&pMe->m_msSend);
            
            pMe->m_dwInsertPos = 0;
            MOVE_TO_STATE(WMSST_WRITEMSG)  
            return NFSMACTION_CONTINUE;
            
        case DLGRET_RESERVEDMSGLST:
        case DLGRET_FAILEDRSVDMSGS:
            {
                uint16 nNews=0, nMsgs=0, nAlert = 0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_RESERVE, &nNews, NULL, &nMsgs);
                if (pMe->m_eDlgReturn == DLGRET_RESERVEDMSGLST)
                {
                    pMe->m_eMBoxType = WMS_MB_RESERVE;
                    nAlert = nNews;
                }
                else
                {
                    pMe->m_eMBoxType = WMS_MB_RSVFAILED;
                    nAlert = nMsgs-nNews;
                }
                
                if (nAlert == 0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
            }
            
            pMe->m_eMakeListMode = MAKEMSGLIST_INIT;
            MOVE_TO_STATE(WMSST_RESERVEDMSGS)  
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_RESERVEDMSG_Handler

/*==============================================================================
����:
    WMSST_RSVDMSGTIME_Handler

˵��:
    WMSST_RSVDMSGTIME ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_RSVDMSGTIME_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_GETDT);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_TONUMLIST)          
            return NFSMACTION_CONTINUE;
                        
        case DLGRET_YEARERR:
            pMe->m_ePMsgType = MESSAGE_INFORMATION;
            WmsApp_ShowMsgBox(pMe, IDS_YEARERR);
            return NFSMACTION_WAIT;
            
        case DLGRET_DATEERR:
            pMe->m_ePMsgType = MESSAGE_WARNNING;
            WmsApp_ShowMsgBox(pMe, IDS_TIMEERR);
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:
            // ����ԤԼ����
            if (IVector_Size(pMe->m_pSendList) > 0)
            {
                int i, nRet;
                int32  nItems = 0;
                uint16  nMsgs=0, nMax=RESERVE_MAX;
                CMultiSendItemInfo *pItem = NULL;
                wms_client_message_s_type *pClientMsg = NULL;
                wms_cache_info_node  *pnode = NULL;
                
                // �����Ϣ
                WmsApp_PrepareUserDataMOList(pMe);
                pMe->m_idxUserdata = 0;
                
                if (pMe->m_eCreateWMSType == SEND_MSG_EDITRESERVE)
                {// ��ǰ�Ǳ༭�Ѵ洢ԤԼ����
                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                    {
                        if (pMe->m_CurMsgNodes[i] != NULL)
                        {
                            nMax++;
                        }
                    }
                }
                
                // ��Ϣ��Ҫ���棬�����ȼ��洢�ռ䣬�洢�ռ䲻������ʾ�û����û�����ѡ����ټ���!
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_RESERVE, &nMsgs, NULL, NULL);
                nItems = IVector_Size(pMe->m_pUserDataMOList);
                if ((nItems+nMsgs)>nMax - MAX_RSVFAILD)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_STOREINSUFFICIENCY);
                    return NFSMACTION_WAIT;
                }
                
                if (pMe->m_eCreateWMSType == SEND_MSG_EDITRESERVE)
                {// ��ǰ�Ǳ༭�Ѵ洢ԤԼ����,��ɾ��
                    for (i=0; i<LONGSMS_MAX_PACKAGES; i++)
                    {
                        if (pMe->m_CurMsgNodes[i] != NULL)
                        {
                            pnode = pMe->m_CurMsgNodes[i];
                            
                            // ����ɾ����Ϣ����
                            nRet = ENOMEMORY;
                            do
                            {
                                nRet = IWMS_MsgDelete(pMe->m_pwms,
                                                   pMe->m_clientId,
                                                   &pMe->m_callback,
                                                   (void *)pMe,
                                                   pnode->mem_store,
                                                   pnode->index);
                            } while(nRet != SUCCESS);
#ifndef WIN32
                            // ����10������ȷ����ʱ��ִ�б�����Ϣ�Ĳ���
                            MSLEEP(10);
#endif                                        
                            pMe->m_CurMsgNodes[i] = NULL;
                        }
                    }
                }
                
                pItem = (CMultiSendItemInfo *)IVector_ElementAt(pMe->m_pSendList, 0);
                
                if (NULL != pItem)
                {
                    // ������ǰ����
                    pMe->m_msSend.m_szNum[0] = 0;
                    (void)WSTRCPY(pMe->m_msSend.m_szNum, pItem->m_szTo);
                }

                pClientMsg = WmsApp_GetClientMsgMO(pMe, TRUE);
                
                while (NULL != pClientMsg)
                {
                    nRet = ENOMEMORY;
                    
                    do
                    {
                        nRet = IWMS_MsgWrite(pMe->m_pwms, 
                                             pMe->m_clientId, 
                                             &pMe->m_callback,
                                             (void*)pMe,
                                             WMS_WRITE_MODE_INSERT,
                                             pClientMsg);
                    } while(nRet != SUCCESS);
#ifndef WIN32
                    // ����10������ȷ����ʱ��ִ�б�����Ϣ�Ĳ���
                    MSLEEP(10);
#endif                                        
                    FREE(pClientMsg);
                    pClientMsg = WmsApp_GetClientMsgMO(pMe, TRUE);
                }
                pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
                WmsApp_ShowMsgBox(pMe, IDS_SAVED);
                return NFSMACTION_WAIT;
            }
            else
            {// ��ֻ�Ƿ�����ʩ������Ӧִ�е���
                MOVE_TO_STATE(WMSST_EXIT)
            }
            
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX_OK:
            switch (pMe->m_wMsgResID)
            {
                case IDS_SAVED:
                    pMe->m_eMBoxType = WMS_MB_RESERVE;
                    pMe->m_eMakeListMode = MAKEMSGLIST_INIT;
                    MOVE_TO_STATE(WMSST_RESERVEDMSGS)
                    WmsApp_ReservedMsgSetTimer(pMe);
                    break;
                    
                default:   
                    WmsApp_ShowDialog(pMe, IDD_GETDT);
                    return NFSMACTION_WAIT;
            }
            return NFSMACTION_CONTINUE;
        
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_RSVDMSGTIME_Handler

/*==============================================================================
����:
    WMSST_RESERVEDMSGS_Handler

˵��:
    WMSST_RESERVEDMSGS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_RESERVEDMSGS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
        case DLGRET_LOADCANCELED:
        case DLGRET_LOADFAILED:
            WmsApp_ShowDialog(pMe, IDD_MESSAGELIST);
            return NFSMACTION_WAIT;
            
        case DLGRET_LOAD:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_RESERVEDMSG)
            return NFSMACTION_CONTINUE;

        case DLGRET_LOADOK:
            if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
            {
                MOVE_TO_STATE(WMSST_VIEWRESERVEDMSG)
            }
            else
            {
                MOVE_TO_STATE(WMSST_RESERVEDMSGOPT)
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OPT:
            pMe->m_eOptType = OPT_VIA_LISTMSG;
            WmsApp_ShowDialog(pMe, IDD_LOADINGMSG);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_RESERVEDMSGS_Handler

/*==============================================================================
����:
    WMSST_VIEWRESERVEDMSG_Handler

˵��:
    WMSST_VIEWRESERVEDMSG ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VIEWRESERVEDMSG_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_VIEWMSG);
           
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_RESERVEDMSGS)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            MOVE_TO_STATE(WMSST_RESERVEDMSGOPT)
            return NFSMACTION_CONTINUE;

        case DLGRET_INFO:
            pMe->m_eOptType = OPT_VIA_VIEWMSG;
            pMe->m_eDlgReturn = DLGRET_EDIT;
            pMe->m_bDoNotOverwriteDlgResult = TRUE;
            MOVE_TO_STATE(WMSST_RESERVEDMSGOPT)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VIEWRESERVEDMSG_Handler

/*==============================================================================
����:
    WMSST_RESERVEDMSGOPT_Handler

˵��:
    WMSST_RESERVEDMSGOPT ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_RESERVEDMSGOPT_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_MSGOPTS);
            return NFSMACTION_WAIT;

        case DLGRET_VIEW:
            MOVE_TO_STATE(WMSST_VIEWRESERVEDMSG)
            return NFSMACTION_CONTINUE;
        
        case DLGRET_CANCELED:
            if (pMe->m_eOptType == OPT_VIA_VIEWMSG)
            {
                MOVE_TO_STATE(WMSST_VIEWRESERVEDMSG)
            }
            else
            {
                MOVE_TO_STATE(WMSST_RESERVEDMSGS)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_EDIT:
            // ������Ϣ
            WMSMessageStruct_Reset(&pMe->m_msSend);
            MEMCPY(&pMe->m_msSend, &pMe->m_msCur, sizeof(pMe->m_msSend));
            if (NULL != pMe->m_msCur.m_szMessage)
            {
                pMe->m_msSend.m_szMessage = WSTRDUP(pMe->m_msCur.m_szMessage);
            }
            
            // ׼����ǰ����
            {
                CMultiSendItemInfo *pItem = NULL;
                
                // �����Ⱥ����ַ����
                WmsApp_FreeMultiSendList(pMe->m_pSendList);
                
                if (WSTRLEN(pMe->m_msCur.m_szNum)>0)
                {
                    pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
                    
                    if ((NULL != pItem) &&
                        (SUCCESS != IVector_AddElement(pMe->m_pSendList, pItem)))
                    {
                        sys_free(pItem);
                        pItem = NULL;
                    }
                    
                    if (NULL != pItem)
                    {
                        (void)WSTRCPY(pItem->m_szTo, pMe->m_msCur.m_szNum);
                        pMe->m_CurAddID = MSG_CMD_BASE;
                        
                        // �ӵ绰����ȡ����, ������ʾ
                        WMSUtil_GetContactName(pMe, pItem->m_szTo, pItem->m_szName, MAX_TITLE_LEN);
                    }
                }
            }
            
            MEMSET(&pMe->m_rsvDateTime, 0, sizeof(pMe->m_rsvDateTime));
            pMe->m_rsvDateTime.dwSecs = pMe->m_msCur.m_dwTime;
            
            pMe->m_STSwitchToEditMsg = pMe->m_currState;
            pMe->m_eCreateWMSType = SEND_MSG_EDITRESERVE;
            MOVE_TO_STATE(WMSST_WRITEMSG)
            pMe->m_dwInsertPos = 0;
            return NFSMACTION_CONTINUE;

        case DLGRET_SEND:
        case DLGRET_RESEND:
            pMe->m_idxCur = 0;
            
            {// �����ط�������������Ϣ�༭���棬Ҳ�������ַ�б��������
                AECHAR  *pwstrNum = NULL;
                CMultiSendItemInfo *pItem = NULL;
                
                // �����Ⱥ����ַ����
                WmsApp_FreeMultiSendList(pMe->m_pSendList);
                
                // ������ַ 
                if (WSTRLEN(pMe->m_msCur.m_szNum)>0)
                {
                    pwstrNum = pMe->m_msCur.m_szNum;
                }
                else
                {
                    // ״̬������Ǩ��
                    return NFSMACTION_CONTINUE;
                }

                //����ԤԼ�����޸�ʱ���
                if(pMe->m_eDlgReturn == DLGRET_SEND)
                {
                    uint32 dwTime;

                    dwTime = GETTIMESECONDS();
                    pMe->m_msCur.m_dwTime = dwTime;
                }
                
                pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
                    
                // ���ظ����뱣��������
                if ((pItem == NULL) || 
                    (SUCCESS != IVector_AddElement(pMe->m_pSendList, pItem)))
                {// �պ����޷��ظ�
                    if (NULL != pItem)
                    {
                        sys_free(pItem);
                    }
                    
                    // ״̬������Ǩ��
                    return NFSMACTION_CONTINUE;
                }
                (void)WSTRCPY(pItem->m_szTo, pwstrNum);
                pMe->m_CurAddID = MSG_CMD_BASE;
                
                // �ӵ绰����ȡ����, ������ʾ
                WMSUtil_GetContactName(pMe, pItem->m_szTo, pItem->m_szName, MAX_TITLE_LEN);
            }
            
            pMe->m_eCreateWMSType = SEND_MSG_RESERVE;
            MOVE_TO_STATE(WMSST_SENDING)
            return NFSMACTION_CONTINUE;  
            
        case DLGRET_DELETE:
            pMe->m_eEraseWMSType = ERASE_RESERVED_ONE;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_CLEARALL:
            if (pMe->m_eMBoxType == WMS_MB_RESERVE)
            {
                pMe->m_eEraseWMSType = CLEAR_RESERVED;
            }
            else
            {
                pMe->m_eEraseWMSType = CLEAR_RESERVEFAILED;
            }
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_RESERVEDMSGOPT_Handler

/*==============================================================================
����:
    WMSST_RESERVEDMSGALERT_Handler

˵��:
    WMSST_RESERVEDMSGALERT ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_RESERVEDMSGALERT_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_RESERVEDMSGALERT);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            WmsApp_ReservedMsgSetTimer(pMe);
            if (pMe->m_bNaturalStart)
            {
                MOVE_TO_STATE(WMSST_MAIN)
            }
            else
            {
                MOVE_TO_STATE(WMSST_EXIT)
            }
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            // ���õ�ǰ��Ϣ�б�
            MEMSET(pMe->m_CurMsgNodes, 0, sizeof(pMe->m_CurMsgNodes));
            WmsApp_FreeMsgNodeMs(pMe);
            
            {// ����ԤԼ���ŷ��Ͳ����Ȳ�������Ϣ�༭���棬Ҳ�������ַ�б��������
                CMultiSendItemInfo *pItem = NULL;
                wms_cache_info_node  *pnode = NULL;
                
                // �����Ⱥ����ַ����
                WmsApp_FreeMultiSendList(pMe->m_pSendList);
                
                wms_cacheinfolist_enumbegin(WMS_MB_RESERVE);
                pnode = wms_cacheinfolist_enumnext(WMS_MB_RESERVE);
                
                if (NULL == pnode)
                {
                    // �ݴ���
                    MOVE_TO_STATE(WMSST_EXIT)
                    return NFSMACTION_CONTINUE;
                }
                else
                {
                    pMe->m_idxCur = 0;
#ifdef FEATURE_SMS_UDH
                    if (pnode->pItems != NULL)
                    {
                        MEMCPY(pMe->m_CurMsgNodes, pnode->pItems, sizeof(pMe->m_CurMsgNodes));
                    }
                    else
#endif
                    {
                        pMe->m_CurMsgNodes[0] = pnode;
                    }
                }
                
                // ������ַ 
                if ((NULL != pnode->pszNum) && (STRLEN(pnode->pszNum)>0))
                {
                    pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
                    
                    // ���ظ����뱣��������
                    if ((pItem == NULL) || 
                        (SUCCESS != IVector_AddElement(pMe->m_pSendList, pItem)))
                    {
                        // �ݴ���
                        MOVE_TO_STATE(WMSST_EXIT)
                        
                        if (NULL !=pItem)
                        {
                            sys_free(pItem);
                        }
                        
                        WmsApp_ReservedMsgSetTimer(pMe);
                        return NFSMACTION_CONTINUE;
                    }
                    (void)STRTOWSTR(pnode->pszNum, pItem->m_szTo, sizeof(pItem->m_szTo));
                    pMe->m_CurAddID = MSG_CMD_BASE;
                    
                    // �ӵ绰����ȡ����, ������ʾ
                    WMSUtil_GetContactName(pMe, pItem->m_szTo, pItem->m_szName, MAX_TITLE_LEN);
                }
                else
                {
                    // �ݴ���
                    MOVE_TO_STATE(WMSST_EXIT)
                    
                    WmsApp_ReservedMsgSetTimer(pMe);
                    return NFSMACTION_CONTINUE;
                }
            }
            
            pMe->m_eCreateWMSType = SEND_MSG_RESERVE;
            
            // ״̬Ǩ�����ͽ���
            MOVE_TO_STATE(WMSST_SENDING)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_RESERVEDMSGALERT_Handler

/*==============================================================================
����:
    WMSST_AUTOSAVE_Handler

˵��:
    WMSST_AUTOSAVE ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_AUTOSAVE_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_OUTMSGSAVEOPT);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_AUTOSAVE_Handler

/*==============================================================================
����:
    WMSST_PRIORITY_Handler

˵��:
    WMSST_PRIORITY ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_PRIORITY_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_PRIORITY);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_PRIORITY_Handler

/*==============================================================================
����:
    WMSST_SENDMODE_Handler

˵��:
    WMSST_SENDMODE ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_SENDMODE_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_SENDMODE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_SENDMODE_Handler

/*==============================================================================
����:
    WMSST_REPORTS_Handler

˵��:
    WMSST_REPORTS ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_REPORTS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_REPORTS);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_REPORTS_Handler

#ifdef FEATURE_AUTOREPLACE
/*==============================================================================
����:
    WMSST_AUTOREPLACE_Handler

˵��:
    WMSST_AUTOREPLACE ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_AUTOREPLACE_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_AUTOREPLACE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_AUTOREPLACE_Handler
#endif /* FEATURE_AUTOREPLACE */

/*==============================================================================
����:
    WMSST_CALLBACKNUMSWITCH_Handler

˵��:
    WMSST_CALLBACKNUMSWITCH ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_CALLBACKNUMSWITCH_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_CALLBACKNUMSWITCH);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        case DLGRET_CALLBACKNUM:
            MOVE_TO_STATE(WMSST_CALLBACKNUMSET)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_CALLBACKNUMSWITCH_Handler

/*==============================================================================
����:
    WMSST_CALLBACKNUMSET_Handler

˵��:
    WMSST_CALLBACKNUMSET ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_CALLBACKNUMSET_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_CALLBACKNUM);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_CALLBACKNUMSET_Handler

/*==============================================================================
����:
    WMSST_AUTODELETE_Handler

˵��:
    WMSST_AUTODELETE ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_AUTODELETE_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_AUTODELETE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_AUTODELETE_Handler

/*==============================================================================
����:
    WMSST_RESERVEDMSGALERTTIMEOUT_Handler

˵��:
    WMSST_RESERVEDMSGALERTTIMEOUT ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_RESERVEDMSGALERTTIMEOUT_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_RSVDMSGALERTTIMEOUT_SETTING);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_RESERVEDMSGALERTTIMEOUT_Handler

#ifdef FEATURE_CDSMS_RUIM
/*==============================================================================
����:
    WMSST_STORAGE_Handler

˵��:
    WMSST_STORAGE ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_STORAGE_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_STORAGE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MANAGEMENT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_STORAGE_Handler
#endif

/*==============================================================================
����:
    WMSST_MSGVALIDITY_Handler

˵��:
    WMSST_MSGVALIDITY ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_MSGVALIDITY_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_MESSAGEVALIDITY);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MSGSETTING)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            pMe->m_ePMsgType = MESSAGE_INFORMATIVE;
            WmsApp_ShowMsgBox(pMe, IDS_SAVED);
            return NFSMACTION_WAIT;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_MSGVALIDITY_Handler

/*==============================================================================
����:
    WMSST_VOICEMAILOPTS_Handler

˵��:
 
    WMSST_VOICEMAILOPTS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_VOICEMAILOPTS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_MSGOPTS);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_VOICEMAIL)
            return NFSMACTION_CONTINUE;

        case DLGRET_DELETE:
            pMe->m_eEraseWMSType = ERASE_VOICEMAIL_ONE;
            MOVE_TO_STATE(WMSST_DELMSGCONFIRM)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_VOICEMAILOPTS_Handler


#ifdef FEATURE_CONTACT_APP
/*==============================================================================
����:
    WMSST_EXTARCTDETAILS_Handler

˵��:
    WMSST_EXTARCTDETAILS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_EXTARCTDETAILS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_MSGBOX_OK:
        case DLGRET_CREATE:                     
            WmsApp_ShowDialog(pMe, IDD_EXTARCTDETAILS);
            return NFSMACTION_WAIT;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_VIEWINBOXMSG)//WMSST_INBOXMSGOPTS
            return NFSMACTION_CONTINUE;

        case DLGRET_SAVENUM:
            pMe->m_ExtractType = EXTRACT_NUM;
            WmsApp_FreeSaveNumList(pMe);
            
            // ������Ϣ������������
            WmsApp_BuildNumList(pMe, &pMe->m_msCur);
            
            if (IVector_Size(pMe->m_pSaveNumList) < 1)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_NOVALIDNUM);
                return NFSMACTION_WAIT;
            }
            MOVE_TO_STATE(WMSST_EXTARCTEDITEMLIST)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_SAVEEMAIL:
            pMe->m_ExtractType = EXTRACT_EMAILADD;
            WmsApp_FreeSaveNumList(pMe);
            
            // ������Ϣ�������ʵ�ַ����
            WmsApp_BuildMailAddList(pMe, pMe->m_msCur.m_szMessage);
            
            if (IVector_Size(pMe->m_pSaveNumList) < 1)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_NOVALIDMAILADD);
                return NFSMACTION_WAIT;
            }
            MOVE_TO_STATE(WMSST_EXTARCTEDITEMLIST)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_SAVEURL:
            pMe->m_ExtractType = EXTRACT_URL;
            WmsApp_FreeSaveNumList(pMe);
            
            // ������Ϣ����URL��
            WmsApp_BuildUrlList(pMe, pMe->m_msCur.m_szMessage);
            
            if (IVector_Size(pMe->m_pSaveNumList) < 1)
            {
                pMe->m_ePMsgType = MESSAGE_WARNNING;
                WmsApp_ShowMsgBox(pMe, IDS_NOVALIDURL);
                return NFSMACTION_WAIT;
            }
            MOVE_TO_STATE(WMSST_EXTARCTEDITEMLIST)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_EXTARCTDETAILS_Handler

/*==============================================================================
����:
    WMSST_EXTARCTEDITEMLIST_Handler

˵��:
    WMSST_EXTARCTEDITEMLIST ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_EXTARCTEDITEMLIST_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:                     
            WmsApp_ShowDialog(pMe, IDD_EXTARCTEDITEMLIST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            //MOVE_TO_STATE(WMSST_EXTARCTDETAILS)
            MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            MOVE_TO_STATE(WMSST_EXTARCTEDITEMOPT)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_EXTARCTEDITEMLIST_Handler

/*==============================================================================
����:
    WMSST_EXTARCTEDITEMOPT_Handler

˵��:
    WMSST_EXTARCTEDITEMOPT ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_EXTARCTEDITEMOPT_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:                     
            WmsApp_ShowDialog(pMe, IDD_EXTARCTEDITEM_OPT);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_EXTARCTEDITEMLIST)
            return NFSMACTION_CONTINUE;

        case DLGRET_WRITEMSG:
            {
                CMultiSendItemInfo *pItem = NULL;
                AECHAR    *pExtractItem = NULL;

                pExtractItem = (AECHAR *)IVector_ElementAt(pMe->m_pSaveNumList, pMe->m_CurExtractItem);
                
                // �����Ⱥ����ַ����
                WmsApp_FreeMultiSendList(pMe->m_pSendList);
                
                // �����շ����뱣��������
                pItem = (CMultiSendItemInfo *)sys_malloc(sizeof(CMultiSendItemInfo));
                if (pItem == NULL)
                {
                    MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
                    return NFSMACTION_CONTINUE;
                }
                (void)WSTRCPY(pItem->m_szTo, pExtractItem);
                pMe->m_CurAddID = MSG_CMD_BASE;
                if (SUCCESS != IVector_AddElement(pMe->m_pSendList, pItem))
                {
                    sys_free(pItem);
                    MOVE_TO_STATE(WMSST_VIEWINBOXMSG)
                    return NFSMACTION_CONTINUE;
                }
                
                WMSUtil_GetContactName(pMe, pItem->m_szTo, pItem->m_szName, MAX_TITLE_LEN);
            }
            
            pMe->m_STSwitchToEditMsg = WMSST_EXTARCTEDITEMLIST;//WMSST_EXTARCTEDITEMOPT;
            pMe->m_eCreateWMSType = SEND_MSG_REPLY;
            pMe->m_dwInsertPos = 0;
            MOVE_TO_STATE(WMSST_WRITEMSG)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_EXTARCTEDITEMOPT_Handler
#endif

/*==============================================================================
����:
    WMSST_MEMSTATUS_Handler

˵��:
    WMSST_MEMSTATUS ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_MEMSTATUS_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_MEMSTATUS);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:              
            MOVE_TO_STATE(WMSST_MANAGEMENT)   
            return NFSMACTION_CONTINUE;

        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_MEMSTATUS_Handler

/*==============================================================================
����:
    WMSST_CONTINUESEND_QUERY_Handler

˵��:
    WMSST_CONTINUESEND_QUERY ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_CONTINUESEND_QUERY_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            pMe->m_wMsgResID = IDS_CONTINUESENDQUERY;
            WmsApp_ShowDialog(pMe, IDD_CONFIRM);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            if (pMe->m_stcontinuesendbk == WMSST_INBOXES)
            {
                pMe->m_currState = WMSST_MAIN;
                pMe->m_eDlgReturn = DLGRET_INBOXES;
            }
            else
            {
                pMe->m_currState = pMe->m_stcontinuesendbk;
            }
            pMe->m_stcontinuesendbk = WMSST_NONE;
            pMe->m_bNeedContinueSend = FALSE;
            pMe->m_ContinueSendType = NONE_CONTINUE;
                
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            pMe->m_bNeedContinueSend = FALSE;
            MOVE_TO_STATE(WMSST_SENDING)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_CONTINUESEND_QUERY_Handler

/*==============================================================================
����:
    WMSST_MANAGEMENT_Handler

˵��:
    WMSST_MANAGEMENT ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_MANAGEMENT_Handler(WmsApp *pMe)
{
    boolean  bsmslock = FALSE;    
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                           CFGI_SMS_LOCK_CHECK,
                           &bsmslock,
                           sizeof(bsmslock));
    bsmslock = FALSE;
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_MANAGEMENT);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MAIN)    
            return NFSMACTION_CONTINUE;
            
        case DLGRET_DELETEMSGS:
            bsmslock = FALSE;
            {
                uint16  nInMsgs=0;
                uint16  nOutMsgs=0;
                uint16  nDraftMsgs=0;
                uint16  nReserveMsgs=0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, NULL, &nInMsgs);
                wms_cacheinfolist_getcounts(WMS_MB_OUTBOX, NULL, NULL, &nOutMsgs);
                wms_cacheinfolist_getcounts(WMS_MB_DRAFT, NULL, NULL, &nDraftMsgs);
                wms_cacheinfolist_getcounts(WMS_MB_RESERVE, NULL, NULL, &nReserveMsgs);

                if ((nInMsgs+nOutMsgs+nDraftMsgs+nReserveMsgs)==0)
                {
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
            }
                
            MOVE_TO_STATE(WMSST_DELETEMSGS)
            if (bsmslock)
            {
                pMe->m_stchkpwdbk = pMe->m_currState;
                pMe->m_currState = WMSST_CHKPWD;
            }
            return NFSMACTION_CONTINUE; 

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(WMSST_MANAGEMENT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_COPYINBOX:          
            MOVE_TO_STATE(WMSST_COPYINBOX)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_MOVEINBOX:
            MOVE_TO_STATE(WMSST_MOVEINBOX)
            return NFSMACTION_CONTINUE;

#ifdef FEATURE_CDSMS_RUIM
        case DLGRET_STORAGE:
            MOVE_TO_STATE(WMSST_STORAGE)
            return NFSMACTION_CONTINUE;
#endif             

        case DLGRET_MEMSTATUS:
            MOVE_TO_STATE(WMSST_MEMSTATUS)
            return NFSMACTION_CONTINUE;
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_MANAGEMENT_Handler

/*==============================================================================
����:
    WMSST_COPYINBOX_Handler

˵��:
    WMSST_COPYINBOX ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_COPYINBOX_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGBOX_OK:
            WmsApp_ShowDialog(pMe, IDD_COPYINBOX);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MANAGEMENT)    
            return NFSMACTION_CONTINUE;  

        case DLGRET_COPYINBOXMSGTORUIM:
            {
                uint16 wMsgs = 0;
                uint16 wOnUIM = 0;
                uint16 wOnPH = 0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, &wOnUIM, &wMsgs);
                wOnPH = wMsgs-wOnUIM;
                wMsgs = pMe->m_memruimStatuse.max_slots - pMe->m_memruimStatuse.used_tag_slots;

                if(wOnPH > wMsgs)
                {
                    pMe->m_nCopyNum = wMsgs;
                }
                else
                {
                    pMe->m_nCopyNum = wOnPH;
                }

                if (wOnPH == 0)
                {// �ռ����л�������Ϣ��Ϊ 0
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
                else if (pMe->m_nCopyNum == 0)//(wOnPH > wMsgs)
                {// �����ÿռ䲻��
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_INSUFFICIENCY);
                    return NFSMACTION_WAIT;
                }
            }
            MOVE_TO_STATE(WMSST_MSGCOPYCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_COPYRUIMMSGTOINBOX:
            {
                uint16 wMsgs = 0;
                uint16 wOnUIM = 0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, &wOnUIM, &wMsgs);

                if(wMsgs > IN_MAX)
                {
                    pMe->m_nCopyNum = IN_MAX - (wMsgs - wOnUIM);
                }
                else
                {
                    pMe->m_nCopyNum = pMe->m_memruimStatuse.used_tag_slots;
                }

                if (wOnUIM == 0)
                {// ������Ϣ��Ϊ 0
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
                else if (pMe->m_nCopyNum == 0)//(wMsgs> IN_MAX)
                {// �����ռ�����ÿռ䲻��
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_INSUFFICIENCY);
                    return NFSMACTION_WAIT;
                }
            }
            MOVE_TO_STATE(WMSST_MSGCOPYCONFIRM)
            return NFSMACTION_CONTINUE;            
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_COPYINBOX_Handler

/*==============================================================================
����:
    WMSST_MOVEINBOX_Handler

˵��:
    WMSST_MOVEINBOX ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_MOVEINBOX_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
        case DLGRET_MSGBOX_OK:
            WmsApp_ShowDialog(pMe, IDD_MOVEINBOX);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_MANAGEMENT)    
            return NFSMACTION_CONTINUE;

        case DLGRET_MOVEINBOXMSGTORUIM:
            {
                uint16 wMsgs = 0;
                uint16 wOnUIM = 0;
                uint16 wOnPH = 0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, &wOnUIM, &wMsgs);
                wOnPH = wMsgs-wOnUIM;
                wMsgs = pMe->m_memruimStatuse.max_slots - pMe->m_memruimStatuse.used_tag_slots;
                if(wOnPH > wMsgs)
                {
                    pMe->m_nCopyNum = wMsgs;
                }
                else
                {
                    pMe->m_nCopyNum = wOnPH;
                }

                if (wOnPH == 0)
                {// �ռ����л�������Ϣ��Ϊ 0
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
                else if (pMe->m_nCopyNum == 0)//(wOnPH > wMsgs)//Ŀ��ռ�Ϊ0ʱ�Ų������ƶ�
                {// �����ÿռ䲻��
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_INSUFFICIENCY);
                    return NFSMACTION_WAIT;
                }
            }
            MOVE_TO_STATE(WMSST_MSGCOPYCONFIRM)
            return NFSMACTION_CONTINUE;

        case DLGRET_MOVERUIMMSGTOINBOX:
            {
                uint16 wMsgs = 0;
                uint16 wOnUIM = 0;
                
                // ��ȡ��Ϣ��
                wms_cacheinfolist_getcounts(WMS_MB_INBOX, NULL, &wOnUIM, &wMsgs);

                if(wMsgs > IN_MAX)
                {
                    pMe->m_nCopyNum = IN_MAX - (wMsgs - wOnUIM);
                }
                else
                {
                    pMe->m_nCopyNum = pMe->m_memruimStatuse.used_tag_slots;
                }

                if (wOnUIM == 0)
                {// ������Ϣ��Ϊ 0
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_EMPTY);
                    return NFSMACTION_WAIT;
                }
                else if (pMe->m_nCopyNum == 0)//(wMsgs> IN_MAX)
                {// �����ռ�����ÿռ䲻��
                    pMe->m_ePMsgType = MESSAGE_WARNNING;
                    WmsApp_ShowMsgBox(pMe, IDS_INSUFFICIENCY);
                    return NFSMACTION_WAIT;
                }
            }
            MOVE_TO_STATE(WMSST_MSGCOPYCONFIRM)
            return NFSMACTION_CONTINUE;  
            
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_MOVEINBOX_Handler


/*==============================================================================
����:
    WMSST_SELECTFROM_Handler

˵��:
    WMSST_SELECTFROM ״̬��������
    
����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_SELECTFROM_Handler(WmsApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgReturn)
    {
        case DLGRET_CREATE:
            WmsApp_ShowDialog(pMe, IDD_SELECTFROMOPT);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(WMSST_TONUMLIST);
            return NFSMACTION_CONTINUE;
          
        default:
            // ���˳������������
            MOVE_TO_STATE(WMSST_EXIT)
            return NFSMACTION_CONTINUE;
    }
} // WMSST_SELECTFROM_Handler
/*==============================================================================
����:
    WMSST_EXIT_Handler

˵��:
    WMSST_EXIT ״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
static NextFSMAction WMSST_EXIT_Handler(WmsApp *pMe)
{
    // ����������Ҫ�ı������ù���
    WmsApp_MemberReset(pMe);
    
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
    return NFSMACTION_WAIT;
} // WMSST_EXIT_Handler

