/*==============================================================================
// �ļ���
//        fmRadioStateHandler.c
//        2007-11-11 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook.,LTD.
//        
// �ļ�˵����
//        
// ���ߣ�2007-11-11
// �������ڣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
2007-11-11              ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "FmRadio_priv.h"
#include "Appscommon.h"
#include "appscommon.brh"
#include "appscommonimages.brh"
#include "fmradiols.brh"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ FMRADIOST_NONE ������
static NextFSMAction FmRadioStateNoOperationHandler(CFmRadio *pMe);

// ״̬ FMRADIOST_INIT ������
static NextFSMAction FmRadioStateInitHandler(CFmRadio *pMe);

// ״̬ FMRADIOST_USER_MSG ������
static NextFSMAction FmRadioStateUserMsgHandler(CFmRadio *pMe);

// ״̬ FMRADIOST_MAIN ������
static NextFSMAction FmRadioStateMainHandler(CFmRadio *pMe);

// ״̬ FMRADIOST_OPTION ������
static NextFSMAction FmRadioStateOptionHandler(CFmRadio *pMe);

// ״̬ FMRADIOST_LIST ������
static NextFSMAction FmRadioStateListHandler(CFmRadio *pMe);

// ״̬ FMRADIOST_EXIT ������
static NextFSMAction FmRadioStateExitHandler(CFmRadio *pMe);

/*==============================================================================
                                 ȫ������
==============================================================================*/


/*==============================================================================
                                 ���أ���̬������
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
int  FmRadio_ShowMsgBoxDialog( CFmRadio *pMe, 
                               char*       pszResFile,
                               uint16      msgResId,
                               uint16      dlgResId,                               
                               AECHAR     *msg, 
                               int         msglen)
{
     IStatic* pIStatic = NULL;
     AECHAR szText[MSGBOX_MAXTEXTLEN];
     PromptMsg_Param_type MsgParam={0};

     if(SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_STATIC,
                                         (void **)&pIStatic))
     {        
        return FALSE;
     }

     /*Set the static control text based on the user settings of the
      * m_nMsgBoxTextId parameter.*/
     ISHELL_LoadResString(pMe->m_pShell,
           pszResFile,  //AEE_APPSCOMMONRES_LANGFILE,
           msgResId,
           szText,
           sizeof(szText));
     MsgParam.ePMsgType = MESSAGE_CONFIRM;
     MsgParam.pwszMsg = szText;
     MsgParam.eBBarType = BTBAR_OK_CANCEL;
     /*we do not special ISTATIC control, because of DIALOG will redraw
      * ISTATIC control, and the text color is the default, it may be the
      * same as background, but DrawPromptMessage function will change the
      * text color.*/
     DrawPromptMessage(pMe->m_pDisplay, pIStatic, &MsgParam);
     if(pIStatic)
     ISTATIC_Release(pIStatic);
     IDISPLAY_Update(pMe->m_pDisplay);

     return TRUE;
}

/*==============================================================================
����:
       FmRadio_ProcessState
˵��:
       FmRadio Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��FmRadio Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction FmRadio_ProcessState(CFmRadio *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case FMRADIOST_NONE:
            retVal = FmRadioStateNoOperationHandler(pMe);
            break;

        case FMRADIOST_INIT:
            retVal = FmRadioStateInitHandler(pMe);
            break;
            
        case FMRADIOST_USER_MSG:
            retVal = FmRadioStateUserMsgHandler(pMe);
            break;
            
        case FMRADIOST_MAIN:
            retVal = FmRadioStateMainHandler(pMe);
            break;

        case FMRADIOST_OPTION:
            retVal = FmRadioStateOptionHandler(pMe);
            break;         
            
        case FMRADIOST_LIST:
            retVal = FmRadioStateListHandler(pMe);   
            break;    
                                            
        case FMRADIOST_EXIT:
            retVal = FmRadioStateExitHandler(pMe);
            break;
            
        default:
            break;
    }
    return retVal;
}


/*==============================================================================
������
       FmRadioStateNoOperationHandler
˵����
       FMRADIOST_NONE ״̬������

������
       pMe [in]��ָ��FmRadio Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction FmRadioStateNoOperationHandler(CFmRadio *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(FMRADIOST_EXIT)

    return NFSMACTION_CONTINUE;
} // FmRadioStateNoOperationHandler

/*==============================================================================
������
       FmRadioStateInitHandler
˵����
       FMRADIOST_INIT ״̬������

������
       pMe [in]��ָ��FmRadio Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction FmRadioStateInitHandler(CFmRadio *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(FMRADIOST_MAIN)
    return NFSMACTION_CONTINUE;
} // FmRadioStateInitHandler
static NextFSMAction FmRadioStateUserMsgHandler(CFmRadio *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
          //  FmRadio_ShowDialog(pMe, IDD_FM_RADIO_OPTION);
          
            FmRadio_ShowMsgBoxDialog( pMe, 
                                    AEE_APPSCOMMONRES_LANGFILE,
                                    IDS_CONFIRM_OFF_MP,
                                    0,//IDD_FM_RADIO_USER,                                      
                                    NULL,
                                    -1);
            return NFSMACTION_WAIT;

        case DLGRET_OPTION_SELECT:
            MOVE_TO_STATE(FMRADIOST_EXIT)            
            return NFSMACTION_CONTINUE;  

        case DLGRET_CANCELED:
            MOVE_TO_STATE(FMRADIOST_INIT)
            return NFSMACTION_WAIT;        

        default:
            break;
    }

    return NFSMACTION_WAIT;
}//FmRadioStateUserMsgHandler


/*==============================================================================
������
       FmRadioStateMainHandler
˵����
       FMRADIOST_MAIN ״̬������

������
       pMe [in]��ָ��FmRadio Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction FmRadioStateMainHandler(CFmRadio *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            FmRadio_ShowDialog(pMe, IDD_FM_RADIO_MAIN);
            return NFSMACTION_WAIT;

        case DLGRET_OPTION_SELECT:
            MOVE_TO_STATE(FMRADIOST_OPTION)
            return NFSMACTION_CONTINUE;  

        case DLGRET_CANCELED:
            MOVE_TO_STATE(FMRADIOST_EXIT)
            return NFSMACTION_CONTINUE;        

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // FmRadioStateMainHandler

/*==============================================================================
������
       FmRadioStateOptionHandler
˵����
       FMRADIOST_Option ״̬������

������
       pMe [in]��ָ��FmRadio Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction FmRadioStateOptionHandler(CFmRadio *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            FmRadio_ShowDialog(pMe, IDD_FM_RADIO_OPTION);
            return NFSMACTION_WAIT;

        case DLGRET_OPTION_LIST:
            MOVE_TO_STATE(FMRADIOST_LIST)
            return NFSMACTION_CONTINUE;  
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(FMRADIOST_MAIN)
            return NFSMACTION_CONTINUE;              

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // FmRadioStateOptionHandler

/*==============================================================================
������
       FmRadioStateListHandler
˵����
       FMRADIOST_Option ״̬������

������
       pMe [in]��ָ��FmRadio Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction FmRadioStateListHandler(CFmRadio *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            FmRadio_ShowDialog(pMe, IDD_FM_RADIO_LIST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(FMRADIOST_MAIN)
            return NFSMACTION_CONTINUE;              

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // FmRadioStateListHandler

/*==============================================================================
������
       FmRadioStateExitHandler
˵����
       FMRADIOST_EXIT ״̬������

������
       pMe [in]��ָ��FmRadio Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction FmRadioStateExitHandler(CFmRadio *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // FmRadioStateExitHandler