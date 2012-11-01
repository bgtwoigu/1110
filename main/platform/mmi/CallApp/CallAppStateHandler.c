/*==============================================================================
// �ļ���
//        CallStateHandler.c
//        2007-11-01 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//
// �ļ�˵����
//
// ���ߣ�Gemsea
// �������ڣ�2007-11-01
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��    ���ⵥ��  �޸����ݡ�λ�ü�ԭ��
-----------      ----------  --------  -----------------------------------------------
2007-11-01                  ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "CallApp_priv.h"
#ifdef FEATURE_UTK2
#include "UTK.h"
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
// ״̬ STATE_NULL ������
static NextFSMAction STATE_NULLHandler(CCallApp *pMe);

// ״̬ STATE_INIT ������
static NextFSMAction STATE_INITHandler(CCallApp *pMe);

// ״̬ STATE_CALLING_FROM_ANOTHERAPP ������
static NextFSMAction STATE_CALLING_FROM_ANOTHERAPP_Handler(CCallApp *pMe);

// ״̬ STATE_NUMBER_FROM_CORE ������
static NextFSMAction STATE_NUMBER_FROM_COREHandler(CCallApp *pMe);

// ״̬ STATE_OUT_RESTRICT ������
static NextFSMAction STATE_OUT_RESTRICT_Hanlder(CCallApp *pMe);

// ״̬ STATE_ORIGINATION ������
static NextFSMAction STATE_ORIGINATIONHanlder(CCallApp *pMe);

// ״̬ STATE_CONVERSATION ������
static NextFSMAction STATE_CONVERSATIONHandler(CCallApp *pMe);

// ״̬ STATE_CONV_DIALER ������
static NextFSMAction STATE_CONV_DIALERHandler(CCallApp *pMe);

// ״̬ STATE_ENDCALL ������
static NextFSMAction STATE_ENDCALLHandler(CCallApp *pMe);

// ״̬ STATE_MISSEDCALL ������
static NextFSMAction STATE_MISSEDCALLHandler(CCallApp *pMe);

// ״̬ STATE_INCOMINGCALL ������
static NextFSMAction STATE_INCOMINGCALLHandler(CCallApp *pMe);

// ״̬ STATE_PW ������
static NextFSMAction STATE_PWHandler(CCallApp *pMe);

#ifdef FEATURE_UTK2
// ״̬ STATE_CALLCONFIRM ������
static NextFSMAction STATE_CALLCONFIRM_Handler(CCallApp *pMe);
#endif

// ״̬ STATE_EXIT ������
static NextFSMAction STATE_EXITHandler(CCallApp *pMe);
#ifdef FEATRUE_SET_IP_NUMBER
static NextFSMAction STATE_IP_Number_Set_Mode(CCallApp *pMe);
#endif

#ifdef FEATURE_EDITABLE_RECORD
static NextFSMAction STATE_EDIT_REC_NUMBERHandler(CCallApp *pMe);
#endif
#ifdef FEATURE_OEMOMH
static NextFSMAction STATE_NONOMHHandler(CCallApp *pMe);
#endif

// Autoanswer timer callback
//static void CallApp_HandleAutoAnswerTimer(void *pUser);

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
       CallApp_ProcessState
˵��:
       CallApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��CallApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction CallApp_ProcessState(CCallApp *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }
    //CALL_ERR("curstate: %d   %d",pMe->m_eCurState,pMe->m_eDlgRet,0);
    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case STATE_NULL:
            retVal = STATE_NULLHandler(pMe);
            break;

        case STATE_INIT:
            retVal = STATE_INITHandler(pMe);
            break;

        case STATE_CALLING_FROM_ANOTHERAPP:			
            retVal = STATE_CALLING_FROM_ANOTHERAPP_Handler(pMe);
            break;
#if 0
        case STATE_NUMEDIT_OPTION:
            retVal = STATE_NUMEDIT_OPTIONHandler(pMe);
            break;
#endif
        case STATE_NUMBER_FROM_IDLE:
            retVal = STATE_NUMBER_FROM_COREHandler(pMe);
            break;

        case STATE_OUT_RESTRICT:
            retVal = STATE_OUT_RESTRICT_Hanlder(pMe);
            break;

        case STATE_CALLING:
            retVal = STATE_ORIGINATIONHanlder(pMe);
            break;

        case STATE_CONVERSATION:
            retVal = STATE_CONVERSATIONHandler(pMe);
            break;

        case STATE_CONV_DIALER:
            retVal = STATE_CONV_DIALERHandler(pMe);
            break;

        case STATE_ENDCALL:
            retVal = STATE_ENDCALLHandler(pMe);
            break;

        case STATE_MISSEDCALL:
            retVal = STATE_MISSEDCALLHandler(pMe);
            break;

        case STATE_INCOMINGCALL:
            retVal = STATE_INCOMINGCALLHandler(pMe);
            break;

        case STATE_PW:
            retVal=STATE_PWHandler(pMe);
            break;

#ifdef FEATURE_UTK2
        case STATE_CALLCONFIRM:
            retVal = STATE_CALLCONFIRM_Handler(pMe);
            break;

#endif
#ifdef FEATRUE_SET_IP_NUMBER
        case STATE_SEL_IP_NUMBER:
            retVal = STATE_IP_Number_Set_Mode(pMe);
            break;
#endif/*FEATRUE_SET_IP_NUMBER*/
#ifdef FEATURE_EDITABLE_RECORD
        case STATE_EDIT_REC_NUMBER:
            retVal = STATE_EDIT_REC_NUMBERHandler(pMe);
            break;
#endif
#ifdef FEATURE_OEMOMH
        case STATE_NONOMH:
            retVal = STATE_NONOMHHandler(pMe);
            break;
#endif
        case STATE_EXIT:
            retVal = STATE_EXITHandler(pMe);
            break;

        default:
            break;
    }

    CALL_ERR("curstate %d prestate %d dlgret %d CallApp_ProcessState",pMe->m_eCurState,pMe->m_ePreState,pMe->m_eDlgRet);
    //if(retVal ==NFSMACTION_WAIT )
    //{
    //    CALL_ERR("NFSMACTION_WAIT",0,0,0);
    //}
    //if(retVal ==NFSMACTION_CONTINUE )
    //{
    //    CALL_ERR("NFSMACTION_CONTINUE",0,0,0);
    //}

    return retVal;
}

/*==============================================================================
������
       STATE_NULLHandler
˵����
       STATE_NULL ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_NULLHandler(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(STATE_EXIT)
    return NFSMACTION_CONTINUE;
} // STATE_NULLHandler


/*==============================================================================
������
       STATE_INITHandler
˵����
       STATE_INIT ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_INITHandler(CCallApp *pMe)
{
    call_start_info_type  *pStart;

    //uint32 dwMask = 0;

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    pMe->m_running   = TRUE;
    
    // Setup the current audio path (maybe the headset is already plugged in)
    //CallApp_SetupCallAudio(pMe);

    if (pMe->m_nStartCallType == START_WITH_ARGS)
    {
        pMe->m_bCloseAllApplet = FALSE;
        MOVE_TO_STATE(STATE_CALLING)
        return NFSMACTION_CONTINUE;
    }
    else if (pMe->m_nStartCallType == START_INCOMING_CALL)
    {
        pMe->m_bCloseAllApplet = FALSE;
        MOVE_TO_STATE(STATE_INCOMINGCALL)
        return NFSMACTION_CONTINUE;
    }
    else if (pMe->m_nStartCallType == START_END_CALL)
    {
        pMe->m_bCloseAllApplet = FALSE;
        MOVE_TO_STATE(STATE_ENDCALL)
        return NFSMACTION_CONTINUE;
    }
    else if (pMe->m_nStartCallType == START_INCOMING_ANSWERED)
    {
        pMe->m_bCloseAllApplet = FALSE;
        if(pMe->m_CallsTable)
        {
            pMe->m_CallsTable->call_name[0] = 0;
            (void)CallApp_NumberLookup(pMe,
                                     pMe->m_CallsTable->call_number,
                                     pMe->m_CallsTable->call_name,
                                     sizeof(pMe->m_CallsTable->call_name)/sizeof(AECHAR),
                                     pMe->m_CallsTable->ringer,
                                     NULL);
        }
        MOVE_TO_STATE(STATE_CONVERSATION)
        return NFSMACTION_CONTINUE;
    }
#ifdef FEATURE_OEMOMH
    else if(pMe->m_nStartCallType == START_NONOMH)
    {
        pMe->m_bCloseAllApplet = FALSE;
        MOVE_TO_STATE(STATE_NONOMH)
        return NFSMACTION_CONTINUE;
    }
#endif
    else
    {
        if( (pMe->AppStart->pszArgs[0] == 'S')||(pMe->AppStart->pszArgs[0] == 's') )//This is form Coreapp to start the dialer app in phone start
        {
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;
        }
        pStart = (call_start_info_type *)pMe->AppStart->pszArgs;/*lint !e826*/
        if(pStart == NULL)
        {
            return NFSMACTION_WAIT;
        }

        switch (pStart->b_closeAll)
        {
            case CALL_INFO_YES:
                pMe->m_bCloseAllApplet = TRUE;
                break;

            case CALL_INFO_NO:
                pMe->m_bCloseAllApplet = FALSE;
                break;

            default:
                ASSERT_NOT_REACHABLE
        }

        //CALL_ERR("CallApp_Start: %c dial_str = %C ", pStart->catgory,pStart->dial_str,0);
        switch (pStart->catgory)
        {
            case CALL_INFO_DIAL:
            {
#ifdef KEYSND_ZY
                uint16 m_bkeyinfo= 0;
#endif /* KEYSND_ZY */
                STRTOWSTR(pStart->dial_str, pMe->m_DialString, sizeof(pMe->m_DialString));
#ifdef KEYSND_ZY
                //����ΰ����İ�����
                if(pStart->dial_str[0] == '*')//�Ǽ�
                {
                    m_bkeyinfo = AVK_STAR;
                }
                else if(pStart->dial_str[0] == '#')//����
                {
                    m_bkeyinfo = AVK_POUND;
                }
                else//���ּ�
                {
                    m_bkeyinfo = pStart->dial_str[0]-48+AVK_0;
                }
                CallApp_SpecialKeySnd(pMe,m_bkeyinfo);
#endif /* KEYSND_ZY */
                MOVE_TO_STATE(STATE_NUMBER_FROM_IDLE)
                return NFSMACTION_CONTINUE;
            }
#ifdef FEATURE_SUPPORT_BT_APP
            case CALL_INFO_SEND_TO_BT:
                pMe->m_bt_audio = SEND_BT_CALL;
#endif
            case CALL_INFO_SEND:
                STRTOWSTR(pStart->dial_str, pMe->m_DialString, sizeof(pMe->m_DialString));
                //pMe->m_b_from_another = TRUE;
                MOVE_TO_STATE(STATE_CALLING_FROM_ANOTHERAPP)
                return NFSMACTION_CONTINUE;

#ifdef FEATURE_UTK2
            case CALL_INFO_UTK:
            {
                CallAppState  eTsatate = STATE_CALLING_FROM_ANOTHERAPP;
                Setup_Call  *pstCall;
                extern call_start_info_type *pUTKCALLinfo;

                // ��ʱ�����ֵ�������ͷ�
                pUTKCALLinfo = NULL;
                pstCall = UTK_Get_Setup_Call_Buf();
                (void)WSTRLCPY(pMe->m_DialString, pstCall->wszDialNum ,MAX_SIZE_DIAL_STR);
                if (WSTRLEN(pstCall->wszAlpha)>0)
                {
                    eTsatate = STATE_CALLCONFIRM;
                }
                else
                {
#ifdef FEATURE_ICM
                    //if (pMe->m_lastCallState == AEECM_CALL_STATE_CONV)
                    if(AEECM_IS_VOICECALL_CONNECTED(pMe->m_pICM))
#else
					AEETCalls po;
					
					if(SUCCESS != ITELEPHONE_GetCalls(pMe->m_pITelephone, &po,sizeof(AEETCalls)))
					{
						return NFSMACTION_WAIT;
					}
                    //if (pMe->m_lastCallState == AEECM_CALL_STATE_CONV)
                    //if(AEECM_IS_VOICECALL_CONNECTED(pMe->m_pICM))
                    if(po.dwCount>0)
#endif
                    {
                        switch(pstCall->cmd_describe.command_restricttag)
                        {
                            // ��ǰ����ͨ��״̬�����ܽ��к���
                            case 0x00:
                            case 0x01:
                                eTsatate = STATE_CONVERSATION;
                                break;

                            // ��ǰ����ͨ��״̬���Ҷϵ�ǰͨ���ٺ���
                            case 0x04:
                            case 0x05:
                                pMe->m_bEndcurcallForUTKCall = TRUE;
#ifdef FEATURE_ICM
                                ICM_EndAllCalls(pMe->m_pICM);
#else
                                ICALLMGR_EndAllCalls(pMe->m_pICallMgr);
#endif
                                eTsatate = STATE_ENDCALL;
                                break;

                            // ���Խ���3��ͨ��
                            default:
                                eTsatate = STATE_CALLING_FROM_ANOTHERAPP;
                                break;
                        }
                    }
                }

                MOVE_TO_STATE(eTsatate)
            }
            return NFSMACTION_CONTINUE;
#endif //FEATURE_UTK2

#ifdef FEATRUE_SET_IP_NUMBER
            case CALL_INFO_IP_CALL:
                STRTOWSTR(pStart->dial_str, pMe->m_DialString, sizeof(pMe->m_DialString));
                pMe->m_b_from_numedit = FALSE;
                MOVE_TO_STATE(STATE_SEL_IP_NUMBER)
                return NFSMACTION_CONTINUE;
#endif /*FEATRUE_SET_IP_NUMBER*/
#ifdef FEATURE_EDITABLE_RECORD
            case CALL_INFO_TO:
            case CALL_INFO_FROM:
            case CALL_INFO_MISSED:
                STRTOWSTR(pStart->dial_str, pMe->m_DialString, sizeof(pMe->m_DialString));
                pMe->m_bEditRecNumber = TRUE;
                MOVE_TO_STATE(STATE_EDIT_REC_NUMBER)
                return NFSMACTION_CONTINUE;
                break;
#endif
            default:
                ASSERT_NOT_REACHABLE
        }
    }

    return NFSMACTION_WAIT;
} // STATE_INITHandler


/*==============================================================================
������
       STATE_CALLING_FROM_ANOTHERAPP_Handler
˵����
       STATE_CALLING_FROM_ANOTHERAPP ״̬������
       ���ڴ��������APP���еİκ�״̬��

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_CALLING_FROM_ANOTHERAPP_Handler(CCallApp *pMe)
{

    MAKE_CALL_VALUE result = CALL_SUCESS;
    //byte b_emerg = FALSE;

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    //CALL_ERR("curstate %d prestate %d dlgret %d STATE_CALLING_FROM_ANOTHERAPP_Handler",pMe->m_eCurState,pMe->m_ePreState,pMe->m_eDlgRet);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            result = CallApp_MakeCall(pMe);
			
            if(CALL_SUCESS == result)
            {
                MOVE_TO_STATE(STATE_CALLING)
            }
            else if(CALL_FAIL_ANOTHER == result)
            {
                MOVE_TO_STATE(STATE_EXIT)
            }
            else if(CALL_FAIL_RESTICT == result)
            {
                MOVE_TO_STATE(STATE_OUT_RESTRICT)
            }
            else
            {
                pMe->m_bNotOverwriteDlgRet = TRUE;
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;

        case DLGRET_DIALER_INVALID_EMGNUMBER:
            pMe->m_DialString[0] = 0;
            CallApp_ShowMsgBox(pMe, IDS_INVALIDEMGNUM);
            return NFSMACTION_WAIT;

        case DLGRET_EMGNUMBER_CALL_ONLY:
            CallApp_ShowMsgBox(pMe, IDS_EMGNUMBER_CALL_ONLY);
            return NFSMACTION_WAIT;

        case DLGRET_CALL_ATTEMPT_FAILED:
#ifdef FEATURE_FDN
            CallApp_ShowDialog(pMe, IDD_MO_NOT_ALLOWED);
            return NFSMACTION_WAIT;
#else
            MOVE_TO_STATE(STATE_EXIT)
#endif /* FEATURE_FDN */
            return NFSMACTION_CONTINUE;

        case DLGRET_DIALER_RESTRICT:
            MOVE_TO_STATE(STATE_OUT_RESTRICT)
            return NFSMACTION_CONTINUE;

		//Add By zzg 2012_10_31
		case DLGRET_PROMPT:
			CallApp_ShowDialog(pMe, IDD_PROMPT);
            return NFSMACTION_WAIT;
		//Add End	

        case DLGRET_MSGBOX:
            CallApp_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;

        default:
            break;
    }
    return NFSMACTION_WAIT;
} // STATE_CALLNUMBERHandler
/*==============================================================================
������
       STATE_DIALERHandler
˵����
       STATE_DIALER ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_NUMBER_FROM_COREHandler(CCallApp *pMe)
{
#ifdef FEATURE_ICM
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    pMe->m_b_incall = AEECM_IS_VOICECALL_CONNECTED(pMe->m_pICM);
#else
	AEETCalls po;
	
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
	
	if(SUCCESS != ITELEPHONE_GetCalls(pMe->m_pITelephone, &po,sizeof(AEETCalls)))
	{
		return NFSMACTION_WAIT;
	}

    pMe->m_b_incall = po.dwCount>0?TRUE:FALSE;
#endif
    MSG_FATAL("STATE_NUMBER_FROM_COREHandler %d",pMe->m_b_incall,0,0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CallApp_ShowDialog(pMe, IDD_NUMEDIT);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            if(pMe->m_msg_text_id != IDS_NUM_TOO_LONG)
            {
                MOVE_TO_STATE(STATE_EXIT)
            }
            return NFSMACTION_CONTINUE;
#if 0
        case DLGRET_NUMEDIT_OPTION:

            MOVE_TO_STATE(STATE_NUMEDIT_OPTION)
            return NFSMACTION_CONTINUE;
#endif
        case DLGRET_DIALER_INVALID_EMGNUMBER:
            pMe->m_DialString[0] = 0;

            //clear another dial string in Emer call
            if(pMe->idle_info.uimLocked)
            {
                pMe->m_DialString[0] = 0;
            }
            CallApp_ShowMsgBox(pMe, IDS_INVALIDEMGNUM);
            /*
            CallApp_Init_Msgbox_String(pMe, IDS_INVALIDEMGNUM, NULL);
            CallApp_ShowDialog(pMe, IDD_MSGBOX);*/
            return NFSMACTION_WAIT;

        case DLGRET_EMGNUMBER_CALL_ONLY:
            CallApp_ShowMsgBox(pMe, IDS_EMGNUMBER_CALL_ONLY);
            /*
            CallApp_Init_Msgbox_String(pMe, IDS_EMGNUMBER_CALL_ONLY, NULL);
            CallApp_ShowDialog(pMe, IDD_MSGBOX);*/
            return NFSMACTION_WAIT;

        case DLGRET_CALL_ATTEMPT_FAILED:
#ifdef FEATURE_FDN
            CallApp_ShowDialog(pMe, IDD_MO_NOT_ALLOWED);
            return NFSMACTION_WAIT;
#else
            MOVE_TO_STATE(STATE_EXIT)
#endif /* FEATURE_FDN */
            return NFSMACTION_CONTINUE;

        case DLGRET_DIALER_RESTRICT:
            MOVE_TO_STATE(STATE_OUT_RESTRICT)
            return NFSMACTION_CONTINUE;

		//Add By zzg 2012_10_31
		case DLGRET_PROMPT:
			CallApp_ShowDialog(pMe, IDD_PROMPT);
            return NFSMACTION_WAIT;
		//Add End
		
        case DLGRET_MSGBOX:
            CallApp_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;

#ifdef FEATRUE_SET_IP_NUMBER
        case DLGRET_SEL_IP_NUMBER:
            MOVE_TO_STATE(STATE_SEL_IP_NUMBER)
            return NFSMACTION_CONTINUE;
#endif /*FEATRUE_SET_IP_NUMBER*/

        case DLGRET_NUM_TOO_LONG:
            CallApp_ShowMsgBox(pMe, IDS_NUM_TOO_LONG);
            return NFSMACTION_WAIT;
#ifdef FEATURE_SUPPORT_WAP_APP
#ifdef FEATURE_CARRIER_CHINA_TELCOM
        case DLGRET_INVALID_LINK_NUM:
            CallApp_ShowMsgBox(pMe, IDS_INVALID_LINK_NUM);
            return NFSMACTION_WAIT;
#endif
#endif

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_DIALERHandler

/*==============================================================================
������
       STATE_OUT_RESTRICT_Hanlder
˵����
       STATE_OUT_RESTRICT ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_OUT_RESTRICT_Hanlder(CCallApp *pMe)
{

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    //CALL_ERR("STATE_OUT_RESTRICT_Hanlder %d",pMe->m_eDlgRet,0,0);

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CallApp_ShowMsgBox(pMe, IDS_RESTRICT_OUTGOING);
            /*
            CallApp_Init_Msgbox_String(pMe, IDS_RESTRICT_OUTGOING, NULL);
            CallApp_ShowDialog(pMe, IDD_MSGBOX);*/
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            //if (pMe->m_lastCallState == AEECM_CALL_STATE_CONV)
            if(pMe->m_b_incall)
            {
                MOVE_TO_STATE(STATE_CONVERSATION)
            }
            else
            {
                MOVE_TO_STATE(STATE_EXIT)
            }
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_OUT_RESTRICT_Hanlder


/*==============================================================================
������
       STATE_ORIGINATIONHanlder
˵����
       STATE_ORIGINATION ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_ORIGINATIONHanlder(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            FREEIF(pMe->m_pwstrDialStringkeep);
            CallApp_ShowDialog(pMe, IDD_CALLING);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_ORIGINATIONHanlder


/*==============================================================================
������
       STATE_CONVERSATIONHandler
˵����
       STATE_CONVERSATION ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_CONVERSATIONHandler(CCallApp *pMe)
{
    byte minAlert = OEMNV_ALERT_DISABLE;

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    //CALL_ERR("STATE_CONVERSATIONHandler",0,0,0);

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:

            pMe->m_bNotOverwriteDlgRet = FALSE;

            (void) ICONFIG_GetItem(pMe->m_pConfig,
                                                CFGI_MINUTE_ALERT,
                                                &minAlert,
                                                sizeof(minAlert));
            if (minAlert == OEMNV_ALERT_ENABLE)
            {
                (void) ISHELL_SetTimer(pMe->m_pShell,
                                                TIMEOUT_MS_CONV_TIMER,
                                                CallApp_HandleConversationTimer,
                                                (void*) pMe);
            }
            CallApp_ShowDialog(pMe, IDD_CONVERSATION);
#ifdef  FEATURE_PERU_VERSION
            pMe->in_convert = TRUE; //to save another incoming call in conversation;
#endif
            return NFSMACTION_WAIT;

        case DLGRET_CONV_DIAL:
            MOVE_TO_STATE(STATE_CONV_DIALER)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            MOVE_TO_STATE(STATE_CONVERSATION)
            return NFSMACTION_CONTINUE;

        case DLGRET_BACK_TO_IDLE:
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;

		//Add By zzg 2012_10_31
		case DLGRET_PROMPT:
			CallApp_ShowDialog(pMe, IDD_PROMPT);
            return NFSMACTION_WAIT;
		//Add End
		
        case DLGRET_MSGBOX:
            CallApp_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_CONVERSATIONHandler


/*==============================================================================
������
       STATE_CONV_DIALERHandler
˵����
       STATE_CONV_DIALER ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_CONV_DIALERHandler(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CallApp_ShowDialog(pMe, IDD_NUMEDIT);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            if(pMe->m_msg_text_id != IDS_NUM_TOO_LONG)
            {
                MOVE_TO_STATE(STATE_CONVERSATION)
            }
            return NFSMACTION_CONTINUE;

		//Add By zzg 2012_10_31
		case DLGRET_PROMPT:
			CallApp_ShowDialog(pMe, IDD_PROMPT);
            return NFSMACTION_WAIT;
		//Add End
		
        case DLGRET_MSGBOX:
            CallApp_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;

        case DLGRET_DIALER_RESTRICT:
            MOVE_TO_STATE(STATE_OUT_RESTRICT)
            return NFSMACTION_CONTINUE;

        case DLGRET_NUM_TOO_LONG:
            CallApp_ShowMsgBox(pMe, IDS_NUM_TOO_LONG);
            return NFSMACTION_WAIT;
        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_CONV_DIALERHandler

/*==============================================================================
������
       STATE_ENDCALLHandler
˵����
       STATE_ENDCALL ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_ENDCALLHandler(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
#ifdef FEATURE_IS2000_SCC_CODES
            pMe->m_bAnswerHold = FALSE;
#endif /* FEATURE_IS2000_SCC_CODES */
            // Cancel conversation timer
            (void) ISHELL_CancelTimer(pMe->m_pShell,
                                      CallApp_HandleConversationTimer,
                                     (void*)pMe);

#ifdef FEATURE_UTK2
            if (pMe->m_bEndcurcallForUTKCall)
            {
                Setup_Call  *pstCall;

                pMe->m_bEndcurcallForUTKCall = FALSE;
                pstCall = UTK_Get_Setup_Call_Buf();
                if (NULL != pstCall)
                {
                    (void)WSTRLCPY(pMe->m_DialString, pstCall->wszDialNum ,MAX_SIZE_DIAL_STR);
                    MOVE_TO_STATE(STATE_CALLING_FROM_ANOTHERAPP)
                    return NFSMACTION_CONTINUE;
                }
            }
#endif

            CallApp_ShowDialog(pMe, IDD_ENDCALL);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            //if missing the call waiting, should go to missed call dialog.
            if ( pMe->m_MissedCallCount > 0 )
            {
                MOVE_TO_STATE(STATE_MISSEDCALL)
            }
            else
            {
                if (NULL != pMe->m_pwstrDialStringkeep)
                {
                    pMe->m_DialString[0] = 0;
                    (void)WSTRCPY(pMe->m_DialString, pMe->m_pwstrDialStringkeep);
                    FREEIF(pMe->m_pwstrDialStringkeep);
                    MOVE_TO_STATE(STATE_NUMBER_FROM_IDLE)
                }
                else
                {
                    MOVE_TO_STATE(STATE_EXIT)
                }
            }
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_ENDCALLHandler


/*==============================================================================
������
       STATE_MISSEDCALLHandler
˵����
       STATE_MISSEDCALL ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_MISSEDCALLHandler(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    if(0 == pMe->m_MissedCallCount)
    {
        MOVE_TO_STATE(STATE_EXIT)
        return NFSMACTION_CONTINUE;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        case DLGRET_OK:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CallApp_ShowDialog(pMe, IDD_MISSED);
            return NFSMACTION_WAIT;

        //case DLGRET_OK:
        case DLGRET_BACK_TO_IDLE:
            IALERT_StopAlerting(pMe->m_pAlert);
            pMe->m_MissedCallCount = 0;
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;

		//Add By zzg 2012_10_31
		case DLGRET_PROMPT:
			CallApp_ShowDialog(pMe, IDD_PROMPT);
            return NFSMACTION_WAIT;
		//Add End
		
        case DLGRET_MSGBOX:
            CallApp_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;

        case DLGRET_NUMEDIT:
            IALERT_StopAlerting(pMe->m_pAlert);
            pMe->m_MissedCallCount = 0;
            MOVE_TO_STATE(STATE_NUMBER_FROM_IDLE)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_MISSEDCALLHandler

/*==============================================================================
������
       STATE_INCOMINGCALLHandler
˵����
       STATE_INCOMINGCALL ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_INCOMINGCALLHandler(CCallApp *pMe)
{
#ifndef FEATURE_ICM
	AEETCalls po;
#endif

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
#ifdef FEATURE_ICM
    pMe->m_b_incall = AEECM_IS_VOICECALL_CONNECTED(pMe->m_pICM);
#else
	if(SUCCESS != ITELEPHONE_GetCalls(pMe->m_pITelephone, &po,sizeof(AEETCalls)))
	{
		return NFSMACTION_WAIT;
	}

    pMe->m_b_incall = po.dwCount>0?TRUE:FALSE;
#endif
    MSG_FATAL("STATE_INCOMINGCALLHandler %d %d",pMe->m_b_incall,ICM_GetActiveCallIDs(pMe->m_pICM, 
                                           (AEECM_CALL_TYPE_VOICE
                                           |AEECM_CALL_TYPE_EMERGENCY
                                           |AEECM_CALL_TYPE_STD_OTASP
                                           |AEECM_CALL_TYPE_NON_STD_OTASP), 
                                           AEECM_CALL_STATE_NONE, NULL, 0),0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            {
                boolean bHeadsetPresent = FALSE;
#ifdef FEATURE_ICM
                AEECMCallInfo ci;
                if(pMe->m_CallsTable)
                {
                    if(AEE_SUCCESS != ICM_GetCallInfo(pMe->m_pICM, pMe->m_CallsTable->call_id, &ci, sizeof(AEECMCallInfo)))
                    {
                        return NFSMACTION_WAIT;
                    }
                }

                if(AEECM_CALL_STATE_IDLE == ci.call_state)
#else
                AEETCallInfo ci;
                if(pMe->m_CallsTable)
                {
                    if(AEE_SUCCESS != ITELEPHONE_GetCallInfo(pMe->m_pITelephone, pMe->m_CallsTable->call_id, &ci, sizeof(AEETCallInfo)))
                    {
                        return NFSMACTION_WAIT;
                    }
                }

                if(AEET_CALL_STATE_IDLE == ci.call_state)
#endif
                {
                    MOVE_TO_STATE(STATE_MISSEDCALL)
                    return NFSMACTION_CONTINUE;
                }

                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                       CFGI_HEADSET_PRESENT,
                                       &bHeadsetPresent,
                                       sizeof(bHeadsetPresent));
#ifndef FEATURE_CDG2_TEST
                if (bHeadsetPresent)
#endif //FEATURE_CDG2_TEST
                {
                    byte autoAnswerTime = 0;

                    (void) ICONFIG_GetItem(pMe->m_pConfig,
                                           CFGI_HEADSET_AUTOANSWER,
                                           &autoAnswerTime,
                                           sizeof(autoAnswerTime));

                    if (autoAnswerTime > 0)
                    {
                        (void) ISHELL_SetTimer(pMe->m_pShell,
                                              (int32) autoAnswerTime * 1000,
                                              CallApp_HandleAutoAnswerTimer,
                                              pMe);
                    }/*autoAnswerTime > 0*/
                }/*bHeadsetPresent == TRUE*/
            }

            CallApp_ShowDialog(pMe, IDD_INCOMINGCALL);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            if(pMe->m_b_incall)
            {
                MOVE_TO_STATE(STATE_CONVERSATION)
                return NFSMACTION_CONTINUE;
            }
            else
            {
                MOVE_TO_STATE(STATE_EXIT)
                return NFSMACTION_CONTINUE;
            }

        case DLGRET_PW:
                MOVE_TO_STATE(STATE_PW)
                return NFSMACTION_CONTINUE;
                   

        case DLGRET_BACK_TO_IDLE:
            if (NULL != pMe->m_pwstrDialStringkeep)
            {
                pMe->m_DialString[0] = 0;
                (void)WSTRCPY(pMe->m_DialString, pMe->m_pwstrDialStringkeep);
                FREEIF(pMe->m_pwstrDialStringkeep);
                MOVE_TO_STATE(STATE_NUMBER_FROM_IDLE)
            }
            else
            {
                MOVE_TO_STATE(STATE_EXIT)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_CONNECT:
            MOVE_TO_STATE(STATE_CONVERSATION)
            return NFSMACTION_CONTINUE;
        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_INCOMINGCALLHandler

/*==============================================================================
������
       STATE_CONV_DIALERHandler
˵����
       STATE_CONV_DIALER ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_PWHandler(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CallApp_ShowDialog(pMe, IDD_PWD);
            return NFSMACTION_WAIT;
       case DLGRET_PASS:
            MOVE_TO_STATE(STATE_INCOMINGCALL)
            return NFSMACTION_CONTINUE;    

        case DLGRET_FAILD:
            CallApp_ShowMsgBox(pMe, IDS_INVALID);
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:
             MOVE_TO_STATE(STATE_PW)
             return NFSMACTION_CONTINUE;
        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_CONV_DIALERHandler


#ifdef FEATURE_UTK2
/*==============================================================================
������
    STATE_CALLCONFIRM_Handler

˵����
    STATE_CALLCONFIRM ״̬������

������
    pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��
    ��״̬���ڴ��� UTK �������ʱҪ���û�ȷ�ϵ����
==============================================================================*/
static NextFSMAction STATE_CALLCONFIRM_Handler(CCallApp *pMe)
{
#ifndef FEATURE_ICM
	AEETCalls po;
#endif
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
#ifdef FEATURE_ICM
    pMe->m_b_incall = AEECM_IS_VOICECALL_CONNECTED(pMe->m_pICM);
#else
	if(SUCCESS != ITELEPHONE_GetCalls(pMe->m_pITelephone, &po,sizeof(AEETCalls)))
	{
		return NFSMACTION_WAIT;
	}
	
	pMe->m_b_incall = po.dwCount>0?TRUE:FALSE;
#endif
    MSG_FATAL("STATE_CALLCONFIRM_Handler %d",pMe->m_b_incall,0,0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CallApp_ShowDialog(pMe, IDD_USERCONFIRM);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
        {
            CallAppState  eTsatate = STATE_CALLING_FROM_ANOTHERAPP;
            Setup_Call  *pstCall;

            pstCall = UTK_Get_Setup_Call_Buf();
            if (NULL == pstCall)
            {
                MOVE_TO_STATE(STATE_EXIT)
                return NFSMACTION_CONTINUE;
            }

            (void)WSTRLCPY(pMe->m_DialString, pstCall->wszDialNum ,MAX_SIZE_DIAL_STR);

            //if (pMe->m_lastCallState == AEECM_CALL_STATE_CONV)
            if(pMe->m_b_incall)
            {
                switch(pstCall->cmd_describe.command_restricttag)
                {
                    // ��ǰ����ͨ��״̬�����ܽ��к���
                    case 0x00:
                    case 0x01:
                        eTsatate = STATE_CONVERSATION;
                        break;

                    // ��ǰ����ͨ��״̬���Ҷϵ�ǰͨ���ٺ���
                    case 0x04:
                    case 0x05:
                        pMe->m_bEndcurcallForUTKCall = TRUE;
#ifdef FEATURE_ICM
                        ICM_EndAllCalls(pMe->m_pICM);
#else
                        ICALLMGR_EndAllCalls(pMe->m_pICallMgr);
#endif
                        eTsatate = STATE_ENDCALL;
                        break;

                    // ���Խ���3��ͨ��
                    default:
                        eTsatate = STATE_CALLING_FROM_ANOTHERAPP;
                        break;
                }
            }

            MOVE_TO_STATE(eTsatate)
            // ��ͨҪ�������ص� UTK
            if (eTsatate == STATE_CONVERSATION)
            {
#ifdef EVT_BACKUTK
                (void) ISHELL_PostEvent(pMe->m_pShell,
                                                        AEECLSID_APP_UTK,
                                                        EVT_BACKUTK,
                                                        0,
                                                        0);
#endif/*EVT_BACKUTK*/
                return NFSMACTION_WAIT;
            }
        }
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            //if (pMe->m_lastCallState == AEECM_CALL_STATE_CONV)
            if(pMe->m_b_incall)
            {
                MOVE_TO_STATE(STATE_CONVERSATION)
#ifdef EVT_BACKUTK
                (void) ISHELL_PostEvent(pMe->m_pShell,
                                                        AEECLSID_APP_UTK,
                                                        EVT_BACKUTK,
                                                        0,
                                                        0);
#endif/*EVT_BACKUTK*/
                return NFSMACTION_WAIT;
            }
            else
            {
                MOVE_TO_STATE(STATE_EXIT)
                return NFSMACTION_CONTINUE;
            }

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_CALLCONFIRM_Handler
#endif

/*==============================================================================
������
       STATE_EXITHandler
˵����
       STATE_EXIT ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_EXITHandler(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    //CALL_ERR("STATE_EXITHandler",0,0,0);
    CallApp_Free_All_Call_Table(pMe);

    if (pMe->m_bCloseAllApplet)
    {
        (void)ISHELL_CloseApplet(pMe->m_pShell, TRUE);
    }

    else
    {
        (void)ISHELL_CloseApplet(pMe->m_pShell, FALSE);
    }

    return NFSMACTION_WAIT;
} // STATE_EXITHandler

/*=============================================================================
FUNCTION: CallApp_HandleAutoAnswerTimer

DESCRIPTION: Autoanswer timer callback.  When this callback
             is invoked, it answers the call and exits the applet.

PARAMETERS:
   *pUser: CCallApp object pointer

RETURN VALUE:
    None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void CallApp_HandleAutoAnswerTimer(void *pUser)
{
    CCallApp         *pMe = (CCallApp *)pUser;
#ifdef FEATURE_ICM
    AEECMCallInfo ci;

    // Check if we are still in the incoming call state.
    if(pMe->m_CallsTable)
    {
        if(AEE_SUCCESS != ICM_GetCallInfo(pMe->m_pICM, pMe->m_CallsTable->call_id, &ci, sizeof(AEECMCallInfo)))
        {
            return;
        }
    }

    if (AEECM_CALL_STATE_INCOM == ci.call_state)
#else
    AEETCallInfo ci;

    // Check if we are still in the incoming call state.
    if(pMe->m_CallsTable)
    {
        if(AEE_SUCCESS != ITELEPHONE_GetCallInfo(pMe->m_pITelephone, pMe->m_CallsTable->call_id, &ci, sizeof(AEETCallInfo)))
        {
            return;
        }
    }

    if (AEET_CALL_STATE_INCOM == ci.call_state)
#endif
    {
        // Auto answer the call
        CallApp_AnswerCall(pMe,FALSE,0,0,TRUE);
    }
}

#ifdef FEATRUE_SET_IP_NUMBER
static NextFSMAction STATE_IP_Number_Set_Mode(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            IP_Number_Cfg  ip_cfg;
            MEMSET((void*)&ip_cfg,0 , sizeof(IP_Number_Cfg));
            pMe->m_b_have_set_ip = FALSE;
            ICONFIG_GetItem(pMe->m_pConfig,CFGI_IP_NUMBER,(void *)&ip_cfg,sizeof(IP_Number_Cfg));
            if(ip_cfg.ip_count>0)
            {
                pMe->m_b_have_set_ip = TRUE;
            }
            else
            {
                pMe->m_b_have_set_ip = FALSE;
            }

            if(pMe->m_b_have_set_ip)
            {
                CallApp_ShowDialog(pMe, IDD_IP_NUMBER);
            }
            else
            {
                CallApp_ShowMsgBox(pMe, IDS_INVALID_IP_NUMBER);
                /*
                CallApp_Init_Msgbox_String(pMe, IDS_INVALID_IP_NUMBER, NULL);
                CallApp_ShowDialog(pMe, IDD_MSGBOX);*/
            }
            return NFSMACTION_WAIT;
        }

        case DLGRET_CANCELED:
            if(pMe->m_b_from_numedit)
            {
                MOVE_TO_STATE(STATE_NUMBER_FROM_IDLE)
            }
            else
            {
                MOVE_TO_STATE(STATE_EXIT)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_BACK_TO_IDLE:
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;

        case DLGRET_OK:
            if(pMe->m_b_from_numedit)
            {
                MOVE_TO_STATE(STATE_NUMBER_FROM_IDLE)
            }
            else
            {
                MOVE_TO_STATE(STATE_EXIT)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_CALLING:
            MOVE_TO_STATE(STATE_CALLING)
            return NFSMACTION_CONTINUE;

		//Add By zzg 2012_10_31
		case DLGRET_PROMPT:
			CallApp_ShowDialog(pMe, IDD_PROMPT);
            return NFSMACTION_WAIT;
		//Add End	

        case DLGRET_MSGBOX:
            CallApp_ShowMsgBox(pMe, IDS_RESTRICT_OUTGOING);
            /*
            CallApp_Init_Msgbox_String(pMe, IDS_RESTRICT_OUTGOING, NULL);
            CallApp_ShowDialog(pMe, IDD_MSGBOX);*/
            return NFSMACTION_WAIT;

        default:
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;;
    }

    return NFSMACTION_WAIT;
}
#endif /*FEATRUE_SET_IP_NUMBER*/

#ifdef FEATURE_EDITABLE_RECORD
static NextFSMAction STATE_EDIT_REC_NUMBERHandler(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CallApp_ShowDialog(pMe, IDD_NUMEDIT);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            if(pMe->m_msg_text_id != IDS_NUM_TOO_LONG)
            {
                MOVE_TO_STATE(STATE_EXIT)
            }
            return NFSMACTION_CONTINUE;

		//Add By zzg 2012_10_31
		case DLGRET_PROMPT:
			CallApp_ShowDialog(pMe, IDD_PROMPT);
            return NFSMACTION_WAIT;
		//Add End	

        case DLGRET_MSGBOX:
            CallApp_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;

        case DLGRET_NUM_TOO_LONG:
            CallApp_ShowMsgBox(pMe, IDS_NUM_TOO_LONG);
            return NFSMACTION_WAIT;

#ifdef FEATURE_SUPPORT_WAP_APP
#ifdef FEATURE_CARRIER_CHINA_TELCOM
        case DLGRET_INVALID_LINK_NUM:
            CallApp_ShowMsgBox(pMe, IDS_INVALID_LINK_NUM);
            return NFSMACTION_WAIT;
#endif
#endif

        //case DLGRET_REC_EDIT_CONFIRM:
            //CallApp_Save_Edit_Rec_Number(pMe);
            //MOVE_TO_STATE(STATE_EXIT)
            //return NFSMACTION_CONTINUE;

        case DLGRET_DIALER_RESTRICT:
            MOVE_TO_STATE(STATE_OUT_RESTRICT)
            return NFSMACTION_CONTINUE;

        case DLGRET_EMGNUMBER_CALL_ONLY:
            CallApp_ShowMsgBox(pMe, IDS_EMGNUMBER_CALL_ONLY);
            return NFSMACTION_WAIT;

        case DLGRET_DIALER_INVALID_EMGNUMBER:
            pMe->m_DialString[0] = 0;
            CallApp_ShowMsgBox(pMe, IDS_INVALIDEMGNUM);
            return NFSMACTION_WAIT;

        default:
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;
    }

    return NFSMACTION_WAIT;
}
#endif

#ifdef FEATURE_OEMOMH
/*==============================================================================
������
       STATE_NONOMHHandler
˵����
       STATE_NONOMH ״̬������

������
       pMe [in]��ָ��CallApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction STATE_NONOMHHandler(CCallApp *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            CallApp_ShowMsgBox(pMe, IDS_NOOMH_CARD);
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:
            if(pMe->m_clsOMHApplet)
            {
                ISHELL_StartApplet(pMe->m_pShell, pMe->m_clsOMHApplet);
            }
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // STATE_NONOMHHandler
#endif
