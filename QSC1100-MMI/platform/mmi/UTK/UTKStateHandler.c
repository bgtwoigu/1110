/*==============================================================================
// �ļ���
//        UTKStateHandler.c
//        2007-11-13 ��ϲ�����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2007-11-13
// �������ڣ���ϲ��
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-13         ��ϲ��         ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "UTK_priv.h"
#include "UTKUtils.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ UTKST_NONE ������
static NextFSMAction UTKST_NONE_Handler(CUTK *pMe);

// ״̬ UTKST_INIT ������
static NextFSMAction UTKST_INIT_Handler(CUTK *pMe);

#ifdef FEATURE_CARRIER_CHINA_TELCOM
static NextFSMAction UTKST_SPLASH_Handler(CUTK *pMe);
#endif //FEATURE_CARRIER_CHINA_TELCOM

// ״̬ UTKST_MAIN ������
static NextFSMAction UTKST_MAIN_Handler(CUTK *pMe);

// ״̬ UTKST_LIST ������
static NextFSMAction UTKST_LIST_Handler(CUTK *pMe);

// ״̬ UTKST_INPUT ������
static NextFSMAction UTKST_INPUT_Handler(CUTK *pMe);

// ״̬ UTKST_DISPLAY ������
static NextFSMAction UTKST_DISPLAY_Handler(CUTK *pMe);

// ״̬ UTKST_SENDMSG ������
static NextFSMAction UTKST_SENDMSG_Handler(CUTK *pMe);

// ״̬ UTKST_PLAYTONE ������
static NextFSMAction UTKST_PLAYTONE_Handler(CUTK *pMe);

// ״̬ UTKST_MSG ������
static NextFSMAction UTKST_MSG_Handler(CUTK *pMe);

// ״̬ UTKST_MSG ������
static NextFSMAction UTKST_POPMSG_Handler(CUTK *pMe);

// ״̬ UTKST_EXIT ������
static NextFSMAction UTKST_EXIT_Handler(CUTK *pMe);

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
       UTK_ProcessState
˵��:
       UTK Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��UTK Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction UTK_ProcessState(CUTK *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case UTKST_NONE:
            return UTKST_NONE_Handler(pMe);
            
        case UTKST_INIT:
            return UTKST_INIT_Handler(pMe);
            
#ifdef FEATURE_CARRIER_CHINA_TELCOM        
        case UTKST_SPLASH:
            return UTKST_SPLASH_Handler(pMe);
#endif //FEATURE_CARRIER_CHINA_TELCOM            
            
        case UTKST_MAIN:
            return UTKST_MAIN_Handler(pMe);
            
        case UTKST_LIST:
            return UTKST_LIST_Handler(pMe);
            
        case UTKST_INPUT:
            return UTKST_INPUT_Handler(pMe);
            
        case UTKST_DISPLAY:
            return UTKST_DISPLAY_Handler(pMe);
            
        case UTKST_SENDMSG:
            return UTKST_SENDMSG_Handler(pMe);
             
        case UTKST_PLAYTONE:    
            return UTKST_PLAYTONE_Handler(pMe);     
                        
        case UTKST_MSG:    
            return UTKST_MSG_Handler(pMe);   

        case UTKST_POPMSG:    
            return UTKST_POPMSG_Handler(pMe);              
            
        case UTKST_EXIT:
            return UTKST_EXIT_Handler(pMe);

        default:
            break;
    }
    return retVal;
}

/*==============================================================================
������
       UTKST_NONE_Handler
˵����
       UTKST_NONE ״̬������
       
������
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_NONE_Handler(CUTK *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(UTKST_EXIT);
    return NFSMACTION_CONTINUE;
} // UTKST_NONE_Handler


/*==============================================================================
������
       UTKST_INIT_Handler
˵����
       UTKST_INIT ״̬������
       
������
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_INIT_Handler(CUTK *pMe)
{

//#ifdef FEATURE_CARRIER_CHINA_TELCOM    
    //MOVE_TO_STATE(UTKST_SPLASH)
//#endif //FEATURE_CARRIER_CHINA_TELCOM    
    MOVE_TO_STATE(UTKST_MAIN);
    return NFSMACTION_CONTINUE;
} // UTKST_INIT_Handler

#ifdef FEATURE_CARRIER_CHINA_TELCOM
/*==============================================================================
������
    UTKST_SPLASH_Handler
    
˵����
    UTKST_SPLASH ״̬������
       
������
    pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
    NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
    NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
    ����Ӫ����Ҫȷ���Ƿ���ʾ����������������������״ֱ̬������������״̬���ɡ�       
==============================================================================*/
static NextFSMAction UTKST_SPLASH_Handler(CUTK *pMe)
{
    NextFSMAction  eRet = NFSMACTION_WAIT;
    
    if (NULL == pMe)
    {
        return eRet;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            {
                boolean  bNeedSplash = FALSE;
                
#ifdef FEATURE_CARRIER_CHINA_TELCOM
                bNeedSplash = TRUE;
#endif
                if (bNeedSplash)
                {
                    UTK_ShowDialog(pMe, IDD_SPLASH_DIALOG);
                }
                else
                {
                    MOVE_TO_STATE(UTKST_MAIN);
                    eRet = NFSMACTION_CONTINUE;
                }
            }
            return eRet;
        
        case DLGRET_CANCELED:
            MOVE_TO_STATE(UTKST_MAIN);
            return NFSMACTION_CONTINUE;
        
        default:
            break;
    }

    return eRet;
}
#endif //FEATURE_CARRIER_CHINA_TELCOM

/*==============================================================================
������
       UTKST_MAIN_Handler
˵����
       UTKST_MAIN ״̬������
       
������
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_MAIN_Handler(CUTK *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            UTK_ShowDialog(pMe, IDD_MAIN_DIALOG);
            return NFSMACTION_WAIT;
        
        case DLGRET_CANCELED:
            pMe->m_bNormalExit = TRUE;            
            MOVE_TO_STATE(UTKST_EXIT);
            return NFSMACTION_CONTINUE;

        case DLGRET_POPMSG:
            pMe->m_eMsgRetState = UTKST_EXIT;   
            pMe->m_wMsgID = IDS_NONEUTK;     
            pMe->m_MsgTimeout = TIMEOUT_MS_MSGBOX;            
            MOVE_TO_STATE(UTKST_POPMSG);
            return NFSMACTION_CONTINUE;            
      
        default:
            break;
    }

    return NFSMACTION_WAIT;
} // UTKST_MAIN_Handler


/*==============================================================================
������
       UTKST_LIST_Handler
˵����
       UTKST_LIST ״̬������
       
������
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_LIST_Handler(CUTK *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            UTK_ShowDialog(pMe, IDD_LIST_DIALOG);
            return NFSMACTION_WAIT;
        
        default:
            break;
    }

    return NFSMACTION_WAIT;
} // UTKST_LIST_Handler


/*==============================================================================
������
       UTKST_INPUT_Handler
˵����
       UTKST_INPUT ״̬������
       
������
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_INPUT_Handler(CUTK *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            UTK_ShowDialog(pMe, IDD_INPUT_DIALOG);
            return NFSMACTION_WAIT;
                   
        case DLGRET_OK:   
            pMe->m_eMsgRetState = UTKST_INPUT;   
            pMe->m_wMsgID = IDS_GETINPUT_WRONG_LEN;     
            pMe->m_MsgTimeout = TIMEOUT_MS_MSGBOX;            
            MOVE_TO_STATE(UTKST_MSG);
            return NFSMACTION_CONTINUE;  
                                                              
        default:
            break;
    }

    return NFSMACTION_WAIT;
} // UTKST_INPUT_Handler


/*==============================================================================
������
       UTKST_DISPLAY_Handler
˵����
       UTKST_DISPLAY ״̬������
       
������
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_DISPLAY_Handler(CUTK *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            UTK_ShowDialog(pMe, IDD_DISPLAY_DIALOG);
            return NFSMACTION_WAIT;
        
        default:
            break;
    }

    return NFSMACTION_WAIT;
} // UTKST_DISPLAY_Handler


/*==============================================================================
������
       UTKST_EXIT_Handler
˵����
       UTKST_EXIT ״̬������
       
������
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_EXIT_Handler(CUTK *pMe)
{
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // UTKST_EXIT_Handler

/*==============================================================================
������
       UTKST_SENDMSG_Handler

˵����
        UTKST_SENDMSG ״̬��������

������
       pMe [in]��ָ��SMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ��ͣ״̬����
       NFSMACTION_WAIT��ָʾֹͣ״̬����

��ע��
       
==============================================================================*/
static NextFSMAction UTKST_SENDMSG_Handler(CUTK *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        // ��ʾ������Ϣ״̬��ʾ����
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            UTK_ShowDialog(pMe, IDD_SENDMSG_DIALOG);
            return NFSMACTION_WAIT;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // UTKST_SENDMSG_Handler

/*==============================================================================
������
       UTKST_PLAYTONE_Handler
˵����
       UTKST_PLAYTONE ״̬������
       
������
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_PLAYTONE_Handler(CUTK *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            UTK_ShowDialog(pMe, IDD_PLAYTONE_DIALOG);
            return NFSMACTION_WAIT; 
        
        default:
            break;
    }

    return NFSMACTION_WAIT;
} // UTKST_PLAYTONE_Handler

/*==============================================================================
������
       UTKST_MSG_Handler
˵����
       UTKST_MSG ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_MSG_Handler(CUTK *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            pMe->m_bNotOverwriteDlgRet = FALSE;
            UTK_ShowDialog(pMe, IDD_MSGBOX);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(pMe->m_eMsgRetState);
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // UTKST_MSG_Handler

/*==============================================================================
������
       UTKST_MSG_Handler
˵����
       UTKST_MSG ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction UTKST_POPMSG_Handler(CUTK *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            pMe->m_bNotOverwriteDlgRet = FALSE;
            UTK_ShowDialog(pMe, IDD_POPMSG);
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(pMe->m_eMsgRetState);
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
} // UTKST_MSG_Handler

