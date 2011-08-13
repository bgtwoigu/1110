/*=================================================================================================================
// �ļ���VideoPlayerStateHandler.c
//
//	  ��Ȩ����(c) 2008 Anylook
//	
//
// �ļ�˵����
//	  ���ļ�����VideoPlayer Applet״̬����������������ͼ��ɹ��ⲿ�ļ����õ�
//	  ����������
//
// ���ߣ�������
// �������ڣ�2008-10-09

=================================================================================================================*/

/*=================================================================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��       �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------
  2008-10-09     ������          ��ʼ�汾

=================================================================================================================*/

/*=================================================================================================================
                             ���ļ��������ⲿ�ļ�
=================================================================================================================*/

#include "VideoPlayer_priv.h"
#include "Appscommon.brh"
/*=================================================================================================================
                         ���壨�궨�弰����������ǰ�������ͳ���
=================================================================================================================*/

/*=================================================================================================================
                                  ���Ͷ���
=================================================================================================================*/


/*=================================================================================================================
                             ��������(�ڲ������嵥��                                                
=================================================================================================================*/
// ״̬STATE_NONE������
static NextFSMAction StateNoOperationHandler(CVideoPlayer *pMe);

// ״̬STATE_INIT������
static NextFSMAction StateInitHandler(CVideoPlayer *pMe);

//״̬STATE_PLAYER������
static NextFSMAction StatePlayerHandler(CVideoPlayer *pMe);

//״̬STATE_OPTION������
//static NextFSMAction StateOptionHandler(CVideoPlayer *pMe);

//״̬STATE_MSGBOX������
static NextFSMAction StateMSGBOXHandler(CVideoPlayer *pMe);

// ״̬STATE_EXIT������
static NextFSMAction StateExitHandler(CVideoPlayer *pMe);
/*=================================================================================================================
                              ȫ������
=================================================================================================================*/

/*=================================================================================================================
                              ���أ���̬������
=================================================================================================================*/


/*=================================================================================================================
                              ��������
=================================================================================================================*/
/*=================================================================================================================
������VideoPlayer_ProcessState

˵����
       VideoPlayer Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��:

=================================================================================================================*/
NextFSMAction VideoPlayer_ProcessState(CVideoPlayer *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;
    
    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case STATE_NONE:
            retVal = StateNoOperationHandler(pMe);
            break;
            
        case STATE_INIT:
            retVal = StateInitHandler(pMe);
            break;
        
        case STATE_PLAYER:
            retVal = StatePlayerHandler(pMe);
            break;
#if 0
        case STATE_OPTION:
            retVal = StateOptionHandler(pMe);
            break;
#endif
        case STATE_MSGBOX:
            retVal = StateMSGBOXHandler(pMe);
            break;
        
        case STATE_EXIT:
            retVal = StateExitHandler(pMe);
            break;
        
        default:
            break;
    }
    
    return retVal;
}

/*=================================================================================================================
������StateNoOperationHandler

˵����
 
       ����״̬STATE_NONEʱ���������á�����״̬��������������һ״̬ΪSTATE_EXIT
       ��

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
         NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��:

=================================================================================================================*/
static NextFSMAction StateNoOperationHandler(CVideoPlayer *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(STATE_EXIT);
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler

/*=================================================================================================================
������StateInitHandler

˵����
 
����״̬STATE_INITʱ���������á�����״̬��������������һ״̬ΪSTATE_NOPLAY��

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
         NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��:

=================================================================================================================*/
static NextFSMAction StateInitHandler(CVideoPlayer *pMe)
{
    DBGPRINTF("LINA INIT");
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
      
    else
    {
        MOVE_TO_STATE(STATE_PLAYER);
        return NFSMACTION_CONTINUE;
    }
}  // StateInitHandler


/*=================================================================================================================
������StatePlayerHandler

˵����
       ����״̬STATE_PLAYERʱ����������

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
         NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��:

=================================================================================================================*/
static NextFSMAction StatePlayerHandler(CVideoPlayer *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE: 
            pMe->m_bNotOverwriteDlgRet = FALSE;
            VideoPlayer_ShowDialog(pMe, IDD_VIDEOPLAYER);
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX:
            if(pMe->Is848Busy)
            {
                if(app_media_scheduler() == APP_MEDIA_IMPACT_BY_FM)
                {
                    VideoPlayer_ShowMsgBox(pMe,IDS_CONFIRM_OFF_FM);
                }
                if(app_media_scheduler() == APP_MEDIA_IMPACT_BY_MP3)
                {
                    VideoPlayer_ShowMsgBox(pMe,IDS_CONFIRM_OFF_MP);
                }                
            }
            else
            {
                VideoPlayer_ShowMsgBox(pMe,IDS_PLAYFAILED);
            }
            MOVE_TO_STATE(STATE_MSGBOX)
            return NFSMACTION_CONTINUE;
#if 0
        case DLGRET_OPTION:           
            MOVE_TO_STATE(STATE_OPTION)
            return NFSMACTION_CONTINUE;
#endif

        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;
		case DLGRET_FAILD:
			VideoPlayer_ShowMsgBox(pMe,IDS_PLAYFORMATFAILD);
			MOVE_TO_STATE(STATE_MSGBOX)
            return NFSMACTION_CONTINUE;
        
        default:
            break;
    }

    return NFSMACTION_WAIT;
}// StatePlayerHandler


/*=================================================================================================================
������StateOptionHandler

˵����
       ����״̬STATE_OPTIONʱ���������á�

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��:

=================================================================================================================*/
#if 0
static NextFSMAction StateOptionHandler(CVideoPlayer *pMe)
{   
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;           
            VideoPlayer_ShowDialog(pMe, IDD_OPTION);            
            return NFSMACTION_WAIT;
             
        case DLGRET_MSGBOX:
            if(pMe->m_nItemID==IDS_PLAY) 
            {
                VideoPlayer_ShowMsgBox(pMe,IDS_PLAYFAILED);
            }
            if(pMe->m_nItemID==IDS_DELETE)
            {              
                VideoPlayer_ShowMsgBox(pMe,IDS_DEL);
            }
            MOVE_TO_STATE(STATE_MSGBOX)
            return NFSMACTION_CONTINUE;
        
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_PLAYER)
            return NFSMACTION_CONTINUE;
        
        default:
            break;
    }

    return NFSMACTION_WAIT;
}// StateOptionHandler
#endif
/*=================================================================================================================
 ������StateMSGBOXHandler
            
 ˵����
       ����״̬STATE_MSGBOXʱ���������á�
            
 ������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
            
 ����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
            
 ��ע��:
            
=================================================================================================================*/
static NextFSMAction StateMSGBOXHandler(CVideoPlayer *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            VideoPlayer_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;

        case DLGRET_VIDEOPLAYER:
            MOVE_TO_STATE(STATE_PLAYER)
            return NFSMACTION_CONTINUE;
#if 0
        case DLGRET_MSGBOX:
            VideoPlayer_ShowMsgBox(pMe,IDS_DONE);
            break;
#endif      
        case DLGRET_CANCELED:           
            MOVE_TO_STATE(STATE_EXIT)
            return NFSMACTION_CONTINUE;            
            
        default:
            break;
    }

    return NFSMACTION_WAIT;
}//StateMSGBOXHandler

/*=================================================================================================================
������StateExitHandler

˵����
       ����״̬STATE_EXITʱ���������á����ڹر�Applet��

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
         NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��:

=================================================================================================================*/
static NextFSMAction StateExitHandler(CVideoPlayer *pMe)
{
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler
