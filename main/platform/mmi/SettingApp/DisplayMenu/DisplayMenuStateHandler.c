/*==============================================================================
// �ļ���
//        DisplayMenuStateHandler.c
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
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "DisplayMenu_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ DISPLAYMENUST_NONE ������
static NextFSMAction Display_StateNoOperationHandler(CDisplayMenu *pMe);

// ״̬ DISPLAYMENUST_INIT ������
static NextFSMAction Display_StateInitHandler(CDisplayMenu *pMe);

// ״̬ DISPLAYMENUST_MAIN ������
static NextFSMAction Display_StateMainHandler(CDisplayMenu *pMe);

// ״̬ DISPLAYMENUST_PICTURE ������
static NextFSMAction Display_StatePictureHandler(CDisplayMenu *pMe);

// ״̬ DISPLAYMENUST_IMGDELMSG ������
static NextFSMAction Display_StateImgDelMsgHandler(CDisplayMenu *pMe);

// ״̬ DISPLAYMENUST_BACKLIGHTING ������
static NextFSMAction Display_StatebacklightingHandler(CDisplayMenu *pMe);

// ״̬ DISPLAYMENUST_CONTRAST ������
static NextFSMAction Display_StateContrastHandler(CDisplayMenu *pMe);
#ifdef FEATURE_SCREEN_SAVE
// ״̬ DISPLAYMENUST_SCREENTIMEOUT ������
static NextFSMAction Display_StateScreenTimeoutHandler(CDisplayMenu *pMe);
#endif
// ״̬ DISPLAYMENUST_EXIT ������
static NextFSMAction Display_StateExitHandler(CDisplayMenu *pMe);

static NextFSMAction Display_StateDeskTopthemeHandler(CDisplayMenu *pMe);
#ifdef FEATURE_LED_CONTROL
static NextFSMAction Display_StateLED_ControlHandler(CDisplayMenu *pMe);
#endif
#ifdef FEATRUE_KEY_PAD_CTL
static NextFSMAction Display_StateKeyPad_Control_Handler(CDisplayMenu *pMe);
#endif
#ifdef FEATURE_MENU_STYLE
static NextFSMAction  Display_StateMenuStyle_Control_Handler(CDisplayMenu *pMe);
#endif
#ifdef FEATURE_RANDOM_MENU_COLOR
static NextFSMAction  Display_StateColorfulMenu_Control_Handler(CDisplayMenu *pMe); //added by chengxiao 2009.02.20

static NextFSMAction  Display_StateColorSetting_Handler(CDisplayMenu *pMe);
#endif
#ifdef FEATURE_RANDOM_MENU_REND//wlh 20090405 add for rend
static NextFSMAction  Display_StateRendMenu_Control_Handler(CDisplayMenu *pMe); 
#endif
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
       DisplayMenu_ProcessState
˵��:
       DisplayMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��DisplayMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction DisplayMenu_ProcessState(CDisplayMenu *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case DISPLAYMENUST_NONE:
            retVal = Display_StateNoOperationHandler(pMe);
            break;

        case DISPLAYMENUST_INIT:
            retVal = Display_StateInitHandler(pMe);
            break;

        case DISPLAYMENUST_MAIN:
            retVal = Display_StateMainHandler(pMe);
            break;

        case DISPLAYMENUST_BACKLIGHTING:
            retVal = Display_StatebacklightingHandler(pMe);
            break;

        case DISPLAYMENUST_CONTRAST:
            retVal = Display_StateContrastHandler(pMe);
            break;
#ifdef FEATURE_SCREEN_SAVE
        case DISPLAYMENUST_SCREENTIMEOUT:
            retVal = Display_StateScreenTimeoutHandler(pMe);
            break;
#endif
        case DISPLAYMENUST_PICTURE:
            retVal = Display_StatePictureHandler(pMe);
            break;

        case DISPLAYMENUST_IMGDELMSG:
            retVal = Display_StateImgDelMsgHandler(pMe);
            break;

        case DISPLAYMENUST_DESKTOP_THEME:
            retVal = Display_StateDeskTopthemeHandler(pMe);
            break;

#ifdef FEATURE_LED_CONTROL
        case DISPLAYMENUST_LED_CONTROL:
            retVal = Display_StateLED_ControlHandler(pMe);
            break;
#endif

#ifdef FEATRUE_KEY_PAD_CTL
        case DISPLAYMENUST_KEY_PAD_CONTROL:
            retVal = Display_StateKeyPad_Control_Handler(pMe);
            break;
#endif

#ifdef  FEATURE_MENU_STYLE
       case DISPLAYMENUST_MENU_STYLE_CONTROL:
            retVal = Display_StateMenuStyle_Control_Handler(pMe);
            break;
#endif
#ifdef FEATURE_RANDOM_MENU_COLOR
        case DISPLAYMENUST_COLORFUL_MENU:
            retVal = Display_StateColorfulMenu_Control_Handler(pMe);
            break;

        case DISPLAYMENUST_COLOR_SETTING:
            retVal = Display_StateColorSetting_Handler(pMe);
            break;
#endif
#ifdef FEATURE_RANDOM_MENU_REND//wlh 20090405 add for rend
        case DISPLAYMENUST_REND_MENU:
            retVal = Display_StateRendMenu_Control_Handler(pMe);
            break;
#endif        
        case DISPLAYMENUST_EXIT:
            retVal = Display_StateExitHandler(pMe);
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
       DISPLAYMENUST_NONE ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StateNoOperationHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(DISPLAYMENUST_EXIT)
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler


/*==============================================================================
������
       StateInitHandler
˵����
       DISPLAYMENUST_INIT ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StateInitHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(DISPLAYMENUST_MAIN)
    return NFSMACTION_CONTINUE;
} // StateInitHandler


/*==============================================================================
������
       StateMainHandler
˵����
       DISPLAYMENUST_MAIN ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StateMainHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //DISP_ERR("%d StateMainHandler", pMe->m_eDlgRet, 0, 0);
    pMe->m_nSubDlgId = 0;    //���¼����Ӳ˵�ѡ������λ��

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_DISPLAYMENU_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_BACKLIGHTING:
            MOVE_TO_STATE(DISPLAYMENUST_BACKLIGHTING)
            return NFSMACTION_CONTINUE;

        case DLGRET_CONTRAST:
            MOVE_TO_STATE(DISPLAYMENUST_CONTRAST)
            return NFSMACTION_CONTINUE;
#ifdef FEATURE_SCREEN_SAVE
        case DLGRET_SCREENTIMEOUT:
            MOVE_TO_STATE(DISPLAYMENUST_SCREENTIMEOUT)
            return NFSMACTION_CONTINUE;
#endif
        case DLGRET_PICTURE:
            MOVE_TO_STATE(DISPLAYMENUST_PICTURE)
            return NFSMACTION_CONTINUE;

        case DLGRET_IMGDELMSG:
            MOVE_TO_STATE(DISPLAYMENUST_IMGDELMSG)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_EXIT)
            return NFSMACTION_CONTINUE;

        case DLGRET_DESKTOP_THEME:
            MOVE_TO_STATE(DISPLAYMENUST_DESKTOP_THEME)
            return NFSMACTION_CONTINUE;
#ifdef FEATURE_LED_CONTROL
        case DLGRET_LED_CONTROL:
            MOVE_TO_STATE(DISPLAYMENUST_LED_CONTROL)
            return NFSMACTION_CONTINUE;
#endif

#ifdef FEATRUE_KEY_PAD_CTL
        case DLGRET_KEY_PAD_CTL:
            MOVE_TO_STATE(DISPLAYMENUST_KEY_PAD_CONTROL)
            return NFSMACTION_CONTINUE;
#endif
#ifdef FEATURE_MENU_STYLE
        case DLGRET_MENU_STYLE_CTL:
            MOVE_TO_STATE(DISPLAYMENUST_MENU_STYLE_CONTROL)
            return NFSMACTION_CONTINUE;           
#endif
#ifdef FEATURE_RANDOM_MENU_COLOR
        case DLGRET_COLORFUL_MENU_CTL:
            MOVE_TO_STATE(DISPLAYMENUST_COLORFUL_MENU)
            return NFSMACTION_CONTINUE;
#endif
#ifdef FEATURE_RANDOM_MENU_REND//wlh 20090405 add for rend
        case DLGRET_REND_MENU_CTL:
            MOVE_TO_STATE(DISPLAYMENUST_REND_MENU)
            return NFSMACTION_CONTINUE;
#endif
        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateMainHandler


/*==============================================================================
������
       StatePictureHandler
˵����
       DISPLAYMENUST_PICTURE ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StatePictureHandler(CDisplayMenu *pMe)
{
    MSG_FATAL("Display_StatePictureHandler Start",0,0,0);
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //DISP_ERR("%d StatePictureHandler", pMe->m_eDlgRet, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_PICTURE_DLG);
            return NFSMACTION_WAIT;

        case DLGRET_IMGDELMSG:
            MOVE_TO_STATE(DISPLAYMENUST_IMGDELMSG)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }
    MSG_FATAL("Display_StatePictureHandler End",0,0,0);
    return NFSMACTION_WAIT;
} // StatePictureHandler


/*==============================================================================
������
       StateImgDelMsgHandler
˵����
       DISPLAYMENUST_IMGDELMSG ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StateImgDelMsgHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //DISP_ERR("%d StateImgDelMsgHandler", pMe->m_eDlgRet, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_DEL_ITEM_YESNO;
            DisplayMenu_ShowDialog(pMe, IDD_IMGDELMSG_DLG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_PICTURE)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateImgDelMsgHandler

/*==============================================================================
������
       StatebacklightingHandler
˵����
       DISPLAYMENUST_BACKLIGHTING ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StatebacklightingHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //DISP_ERR("%d StatebacklightingHandler", pMe->m_eDlgRet, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_BACKLIGHTING_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSG_POP:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_IMGDELMSG_DLG);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StatebacklightingHandler


/*==============================================================================
������
       StateContrastHandler
˵����
       DISPLAYMENUST_CONTRAST ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StateContrastHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //DISP_ERR("%d StateContrastHandler", pMe->m_eDlgRet, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_CONTRAST_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSG_POP:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_IMGDELMSG_DLG);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateContrastHandler

#ifdef FEATURE_SCREEN_SAVE
/*==============================================================================
������
       StateScreenTimeoutHandler
˵����
       DISPLAYMENUST_SCREENTIMEOUT ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StateScreenTimeoutHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //DISP_ERR("%d StateScreenTimeoutHandler", pMe->m_eDlgRet, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_SCREENSAVERTIMEOUT_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateScreenTimeoutHandler

#endif
/*==============================================================================
������
       StateExitHandler
˵����
       DISPLAYMENUST_EXIT ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StateExitHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler



/*==============================================================================
������
       StateDeskTopthemeHandler
˵����
       DISPLAYMENUST_DESKTOP_THEME ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Display_StateDeskTopthemeHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //DISP_ERR("%d StateDeskTopthemeHandler", pMe->m_eDlgRet, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_DESKTOP_THEME);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateImgDelMsgHandler
#ifdef FEATURE_LED_CONTROL
static NextFSMAction Display_StateLED_ControlHandler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //DISP_ERR("%d StateLED_ControlHandler", pMe->m_eDlgRet, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_LED_CONTROL);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;

}
#endif

#ifdef FEATRUE_KEY_PAD_CTL
static NextFSMAction Display_StateKeyPad_Control_Handler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    //DISP_ERR("%d Display_StateKeyPad_Control_Handler", pMe->m_eDlgRet, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_KEY_PAD_CTL);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSG_POP:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            pMe->m_msg_id = IDS_KEYPAD_TIME_ERR ;
            DisplayMenu_ShowDialog(pMe, IDD_IMGDELMSG_DLG);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;

}
#endif

#ifdef FEATURE_MENU_STYLE
/*==============================================================================
������
       StateMenustyletHandler
˵����
       DISPLAYMENUST_CONTRAST ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction  Display_StateMenuStyle_Control_Handler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    DISP_ERR("%d State StateMenuStyle change", pMe->m_eDlgRet, 0, 0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_MENU_STYLE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSG_POP:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_IMGDELMSG_DLG);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} 
#endif

#ifdef FEATURE_RANDOM_MENU_COLOR
/*==============================================================================
������
       Display_StateColorfulMenu_Control_Handler
˵����
       DISPLAYMENUST_COLORFUL_MENU ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction  Display_StateColorfulMenu_Control_Handler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_COLORFUL_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_COLOR_SETTING:
            MOVE_TO_STATE(DISPLAYMENUST_COLOR_SETTING)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} 


/*==============================================================================
������
       Display_StateColorSetting_Handler
˵����
       DISPLAYMENUST_COLOR_SETTING ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction  Display_StateColorSetting_Handler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_COLOR_SETTING);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_COLORFUL_MENU)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSG_POP:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_IMGDELMSG_DLG);
            return NFSMACTION_WAIT;
            
        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
}
#endif
#ifdef FEATURE_RANDOM_MENU_REND//wlh 20090405 add for rend
/*==============================================================================
������
       Display_StateRendMenu_Control_Handler
˵����
       DISPLAYMENUST_REND_MENU ״̬������

������
       pMe [in]��ָ��DisplayMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction  Display_StateRendMenu_Control_Handler(CDisplayMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_REND_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(DISPLAYMENUST_MAIN)
            return NFSMACTION_CONTINUE;

		case DLGRET_MSG_POP:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            DisplayMenu_ShowDialog(pMe, IDD_IMGDELMSG_DLG);
            return NFSMACTION_WAIT;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} 
#endif//FEATURE_RANDOM_MENU_REND
