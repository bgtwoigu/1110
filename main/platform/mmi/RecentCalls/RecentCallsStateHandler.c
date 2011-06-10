/*==============================================================================
// �ļ���
//        recentcallsStateHandler.c
//        2007-11-18 ��ϲ�����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2007-11-18
// �������ڣ���ϲ��
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-18         ��ϲ��         ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "recentcalls_priv.h"

#ifdef FEATURE_RUIM_PHONEBOOK
//#include "OEMRUIM.h"
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
static NextFSMAction stateverifypasswordhandle( CRecentCalls* pMe);

// ״̬ STATE_RMAINMENU ������
static NextFSMAction statemainmenuhandle(CRecentCalls *pMe);

// ״̬ STATE_RECORD_LIST ������
static NextFSMAction staterecordhandle(CRecentCalls *pMe);

// ״̬ STATE_RTIME_MENU ������
static NextFSMAction statetimemenuhandle(CRecentCalls *pMe);

// ״̬ STATE_RDEL_MENU ������
static NextFSMAction statedelmenuhandle(CRecentCalls *pMe);

// ״̬ STATE_RECORDDEAL ������
static NextFSMAction staterecorddealhandle(CRecentCalls *pMe);

// ״̬ STATE_RWARN ������
static NextFSMAction statewarnhandle(CRecentCalls *pMe);

// ״̬ STATE_RTIME ������
static NextFSMAction statetimehandle(CRecentCalls *pMe);

// ״̬ STATE_DETAIL ������
static NextFSMAction statedetailhandle(CRecentCalls *pMe);

// ״̬ STATE_REXIT ������
static NextFSMAction stateexithandle(CRecentCalls *pMe);

// ״̬ STATE_SELECT_RETURN ������
static NextFSMAction Handler_STATE_SELECT_RETURN(CRecentCalls *pMe);
//״̬ STATE_ASKPASSWORD ������
static NextFSMAction State_AskPasswordHandler(CRecentCalls *pMe);

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
       recentcalls_ProcessState
˵��:
       recentcalls Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��recentcalls Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction recentcalls_ProcessState(CRecentCalls *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }
    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        
        case STATE_VERIFY_PASSWORD:
        {
            retVal = stateverifypasswordhandle( pMe);
        }
        break;
        
        case STATE_RMAINMENU:
            retVal = statemainmenuhandle(pMe);
            break;
            
        case STATE_RECORD_LIST:
            retVal = staterecordhandle(pMe);
            break;
            
        case STATE_RTIME_MENU:
            retVal = statetimemenuhandle(pMe);
            break;
            
        case STATE_RDEL_MENU:
            retVal = statedelmenuhandle(pMe);
            break;
            
        case STATE_RECORDDEAL:
            retVal = staterecorddealhandle(pMe);
            break;
            
        case STATE_RWARN:
            retVal = statewarnhandle(pMe);
            break;
            
        case STATE_RTIME:
            retVal = statetimehandle(pMe);
            break;
        
        case STATE_DETAIL:
            retVal = statedetailhandle(pMe);
            break;

        //wuuquan.tang add for wms 20081127
        case STATE_SELECT_RETURN:
            retVal = Handler_STATE_SELECT_RETURN(pMe);
		case STATE_ASKPASSWORD:
			retVal = State_AskPasswordHandler(pMe);
            break;
        
        case STATE_REXIT:
        default:
            retVal = stateexithandle(pMe);
            break;            

    }
    return retVal;
}

/*==============================================================================
������
       stateverifypasswordhandle
˵����
       STATE_VERIFY_PASSWORD ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT    ��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction stateverifypasswordhandle( CRecentCalls* pMe)
{
    
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch( pMe->m_eDlgRet)
    {
        
        case DLGRET_CREATE:
        {
            boolean needPassword = FALSE;
            extern int OEM_GetConfig(AEEConfigItem i, void * pBuff, int nSize);
            OEM_GetConfig( CFGI_RECENTCALL_LOCK_CHECK,
                         &needPassword,
                         sizeof( boolean)
                     );
            if( !needPassword)
            {
                goto __stateverifypasswordhandle_passed__;
            }
            
            pMe->m_bNotOverwriteDlgRet = FALSE;
            recentcalls_ShowDialog( pMe, IDD_VERIFY_PASSWORD);
            return NFSMACTION_WAIT;
        }
       case  DLGRET_INPUT_INVALID:
       {
            recentcalls_ShowMsgBox(pMe, IDS_MSG_INPUTINVALID);
            return NFSMACTION_WAIT;
       }

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_VERIFY_PASSWORD);
            return NFSMACTION_CONTINUE;
        
        case DLGRET_VERIFY_PASSWORD_PASSED:
        {
__stateverifypasswordhandle_passed__:           
            switch(pMe->m_charAppStartArgs)
            {
                case STARTARG_OUTGCALL:
               pMe->m_callsCategory = AEECALLHISTORY_CALL_TYPE_TO;//CALLHISTORY_OUTGOING_CATEGORY;
                    MOVE_TO_STATE( STATE_RECORD_LIST);
                    break;

                case STARTARG_MISSEDCALL:
                    pMe->m_callsCategory = AEECALLHISTORY_CALL_TYPE_MISSED;//CALLHISTORY_MISSED_CATEGORY;
                    MOVE_TO_STATE( STATE_RECORD_LIST);
                    break;

                case STARTARG_ALLCALL:
                    //pMe->m_eStartMethod = STARTMETHOD_FINDNUMBER;
                    //pMe->m_nRemainWMSNum = RecentCalls_GetRemainWMSNum(pMe);
                    pMe->m_callsCategory = AEECALLHISTORY_CALL_TYPE_ALL;//CALLHISTORY_ALL_CATEGORY;
                    MOVE_TO_STATE( STATE_RECORD_LIST);
                    break;
                case STARTARG_ONECALL:
                    pMe->m_eStartMethod = STARTMETHOD_SELECTFIELD;
                    pMe->m_nRemainWMSNum = 1;
                    pMe->m_callsCategory = AEECALLHISTORY_CALL_TYPE_ALL;//CALLHISTORY_ALL_CATEGORY;
                    MOVE_TO_STATE( STATE_RECORD_LIST);
                    break;
                default:
                    pMe->m_ePreState = STATE_REXIT;
                    pMe->m_eCurState = STATE_RMAINMENU;                
                    break;
            }
            return NFSMACTION_CONTINUE;
        }

        case DLGRET_CANCELED:
        {
            MOVE_TO_STATE( STATE_REXIT)
            return NFSMACTION_CONTINUE;
        }
    }

    return NFSMACTION_WAIT;    
}

/*==============================================================================
������
       statemainmenuhandle
˵����
       STATE_RMAINMENU ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction statemainmenuhandle(CRecentCalls *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            recentcalls_ShowDialog(pMe, IDD_RMAINMENU);
            return NFSMACTION_WAIT;

        case DLGRET_MISSEDCALL:
        case DLGRET_INCOMCALL:
        case DLGRET_OUTGCALL:
            MOVE_TO_STATE(STATE_RECORD_LIST)
            return NFSMACTION_CONTINUE;

        case DLGRET_TIMEMENU:
            MOVE_TO_STATE(STATE_RTIME_MENU)
            return NFSMACTION_CONTINUE;

        case DLGRET_DELMENU:
            MOVE_TO_STATE(STATE_RDEL_MENU)
            return NFSMACTION_CONTINUE;


        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_REXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // statemainmenuhandle


/*==============================================================================
������
       staterecordhandle
˵����
       STATE_RECORD_LIST ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction staterecordhandle(CRecentCalls *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            recentcalls_ShowDialog(pMe, IDD_RLISTRECORD);
            return NFSMACTION_WAIT;

        case DLGRET_RECORDDEAL:
            MOVE_TO_STATE(STATE_RECORDDEAL)
            return NFSMACTION_CONTINUE;
            //add by yangdecai 2010-11-23
        case DLGRET_DELONE:
            MOVE_TO_STATE(STATE_RWARN)
            return NFSMACTION_CONTINUE;     
        case DLGRET_WARN:
            MOVE_TO_STATE(STATE_RWARN)
            return NFSMACTION_CONTINUE;   
         case  DLGRET_DETAIL:
            MOVE_TO_STATE(STATE_DETAIL)
            return NFSMACTION_CONTINUE;
        case DLGRET_CANCELED:
            if(pMe->m_eStartMethod == STARTMETHOD_NORMAL 
                || pMe->m_eStartMethod == STARTMETHOD_FINDNUMBER
                || pMe->m_eStartMethod == STARTMETHOD_SELECTFIELD)
            {
                MOVE_TO_STATE(STATE_REXIT)
            }
            else    /*STARTMETHOD_MAINMENU*/
            {
                MOVE_TO_STATE(STATE_RMAINMENU)
            }

            return NFSMACTION_CONTINUE;

        case DLGRET_REFRESH:
            MOVE_TO_STATE(STATE_RECORD_LIST)
            return NFSMACTION_CONTINUE;
        case DLGRET_OK:
            if(STARTMETHOD_FINDNUMBER == pMe->m_eStartMethod ||
                STARTMETHOD_SELECTFIELD == pMe->m_eStartMethod)
            {
                MOVE_TO_STATE(STATE_SELECT_RETURN)
            }
            return NFSMACTION_CONTINUE;
        case DLGRET_MAX_WMS_CONTACTS:
            recentcalls_ShowMsgBox(pMe, IDS_MAX_SELECT);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_RECORD_LIST)
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    return NFSMACTION_WAIT;
} // staterecordhandle


/*==============================================================================
������
       statetimemenuhandle
˵����
       STATE_RTIME_MENU ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction statetimemenuhandle(CRecentCalls *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            recentcalls_ShowDialog(pMe, IDD_RTIME_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_TIME:
            MOVE_TO_STATE(STATE_RTIME)
            return NFSMACTION_CONTINUE;

        /*case DLGRET_TIMECLEAR:
            MOVE_TO_STATE(STATE_RWARN)
            return NFSMACTION_CONTINUE;*/

        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_RMAINMENU)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }
    
    return NFSMACTION_WAIT;
} // statetimemenuhandle


/*==============================================================================
������
       statedelmenuhandle
˵����
       STATE_RDEL_MENU ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction statedelmenuhandle(CRecentCalls *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            recentcalls_ShowDialog(pMe, IDD_RDEL_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_WARN:
            MOVE_TO_STATE(STATE_RWARN)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_RMAINMENU)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }
    
    return NFSMACTION_WAIT;
} // statedelmenuhandle


/*==============================================================================
������
       staterecorddealhandle
˵����
       STATE_RECORDDEAL ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction staterecorddealhandle(CRecentCalls *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            recentcalls_ShowDialog(pMe, IDD_RDEAL_RECORD);
            return NFSMACTION_WAIT;
        
        case DLGRET_DETAIL:
           MOVE_TO_STATE(STATE_DETAIL)
           return NFSMACTION_CONTINUE;         
                    
        case DLGRET_DELONE:
            MOVE_TO_STATE(STATE_RWARN)
            return NFSMACTION_CONTINUE;         

        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_RECORD_LIST)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            MOVE_TO_STATE(STATE_SELECT_RETURN)
            return NFSMACTION_CONTINUE;
        case DLGRET_MAX_WMS_CONTACTS:
            recentcalls_ShowMsgBox(pMe, IDS_MAX_SELECT);
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_RECORD_LIST);
            return NFSMACTION_CONTINUE;
            
        default:
            break;
    }
    return NFSMACTION_WAIT;
} // staterecorddealhandle


/*==============================================================================
������
       statewarnhandle
˵����
       STATE_RWARN ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction statewarnhandle(CRecentCalls *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            recentcalls_ShowDialog(pMe, IDD_RWARN);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            switch(pMe->selectState)
            {
               case IDS_MISSED_CALLS:
               case IDS_RECEIVED_CALLS:
               case IDS_OUTGOING_CALLS:
                  if (pMe->m_ePreState == STATE_RDEL_MENU)//delete all entries
                  {
                     MOVE_TO_STATE(STATE_RDEL_MENU);
                  }
                  else  //STATE_RECORDDEAL, delete one entry
                  {
                     if (pMe->m_eStartMethod == STARTMETHOD_NORMAL)
                     {
#ifdef FEATURE_THAILAND                           
                         MOVE_TO_STATE(STATE_RECORD_LIST);
                         Recntcall_is_view_state(1);
#else              
                         MOVE_TO_STATE(STATE_REXIT);
#endif
                     }
                     else  /*STARTMETHOD_MAINMENU*/
                     {
                         MOVE_TO_STATE(STATE_RMAINMENU);
                     }
                  }
                  break;                  

               case IDS_ALL_CALLS:
                  MOVE_TO_STATE(STATE_RDEL_MENU);
                  break;
                  
               case IDS_DELETE:
                  MOVE_TO_STATE(STATE_RECORD_LIST)
                  break;
               
               case IDS_RESET_TIME:
                  MOVE_TO_STATE(STATE_RTIME_MENU)
                  break;
                  
               default:
                  break;
            }
            return NFSMACTION_CONTINUE;

        default:
            break;
    }
    return NFSMACTION_WAIT;
} // statewarnhandle


/*==============================================================================
������
       statetimehandle
˵����
       STATE_RTIME ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction statetimehandle(CRecentCalls *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            recentcalls_ShowDialog(pMe, IDD_RTIME);
            return NFSMACTION_WAIT;
        case DLGRET_TIMECLEAR:
			MSG_FATAL("---------ok2",0,0,0);
#if defined (FEATURE_VERSION_W515)||defined (FEATURE_VERSION_S1000T)       
                    MOVE_TO_STATE(STATE_ASKPASSWORD)
#else 
                    MOVE_TO_STATE(STATE_RWARN)
#endif
            
            return NFSMACTION_CONTINUE;
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_RTIME_MENU)
            return NFSMACTION_CONTINUE;
                    
        default:
            break;
    }
    return NFSMACTION_WAIT;
} // statetimehandle

/*==============================================================================
������
       statedetailhandle
˵����
       STATE_DETAIL ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction statedetailhandle(CRecentCalls *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            recentcalls_ShowDialog(pMe, IDD_DETAIL);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_RECORD_LIST)
            return NFSMACTION_CONTINUE;
                    
        default:
            break;
    }
    return NFSMACTION_WAIT;
} // statedetailhandle


/*==============================================================================
������
       stateexithandle
˵����
       STATE_REXIT ״̬������
       
������
       pMe [in]��ָ��recentcalls Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction stateexithandle(CRecentCalls *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

   (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);
    return NFSMACTION_WAIT;
} // stateexithandle

/*==============================================================================
������
       Handler_STATE_SELECT_RETURN
˵����
       STATE_SELECT_RETURN ״̬������
              
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SELECT_RETURN(CRecentCalls *pMe)
{
    extern sSelectRecordListNode* pSelectRecordListRoot;
    //extern Record pRecord;
    boolean       bRet = TRUE;
    
    if(pMe->m_pSelFldCB)
    {
        bRet = pMe->m_pSelFldCB(pSelectRecordListRoot);
    }

    if(bRet)
    {
        MOVE_TO_STATE(STATE_REXIT);
    }
    return NFSMACTION_CONTINUE;
} // Handler_STATE_SELECT_RETURN
/*==============================================================================
������
       State_AskPasswordHandler
˵����
       STATE_ASKPASSWORD ״̬������
              
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/

static NextFSMAction State_AskPasswordHandler(CRecentCalls *pMe)
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
				recentcalls_ShowDialog(pMe, IDD_ASK_PASSWORD_DIALOG);
				MSG_FATAL("---------->ok1",0,0,0);
				return NFSMACTION_WAIT;
	          
			case DLGRET_VALIDPINPASS:
				MSG_FATAL("---------->ok2",0,0,0);
				MOVE_TO_STATE(STATE_RWARN);
				return NFSMACTION_CONTINUE;
	
			case DLGRET_VALIDPINFAILED:
				MSG_FATAL("---------->ok3",0,0,0);
				//pMe->m_wMsgID = IDS_MSG_INPUTINVALID;
				recentcalls_ShowMsgBox(pMe, IDS_MSG_INPUTINVALID);
			//	MOVE_TO_STATE(STATE_RTIME_MENU);
				return NFSMACTION_WAIT; 
				
			case DLGRET_MSGBOX_OK:
				MSG_FATAL("---------->ok4",0,0,0);
				MOVE_TO_STATE(STATE_RTIME_MENU);		  
				return NFSMACTION_CONTINUE;
	
			case DLGRET_CANCELED:
			MOVE_TO_STATE(STATE_RTIME_MENU)
			return NFSMACTION_CONTINUE;
	
			default:
			break;
		}
	
		return NFSMACTION_WAIT;

	
}

