/*==============================================================================
// �ļ���
//        iquicktestStateHandler.c
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
#include "iquicktest_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ QUICKTESTST_NONE ������
static NextFSMAction QUICKTESTSTNoOperationHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_INIT ������
static NextFSMAction QUICKTESTSTInitHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_MAIN ������
static NextFSMAction QUICKTESTSTMainHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_KEYTEST ������
static NextFSMAction QUICKTESTSTKeyTestHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_YAMAHATEST ������
static NextFSMAction QUICKTESTSTYAMAHATestHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_VIBRATETEST ������
static NextFSMAction QUICKTESTSTVibrateTestHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_BACKLIGHTTEST ������
static NextFSMAction QUICKTESTSTBackLightTestHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_FLIPTEST ������
static NextFSMAction QUICKTESTSTFlipTestHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_FMTEST ������
static NextFSMAction QUICKTESTSTFMTestHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_SDTEST ������
static NextFSMAction QUICKTESTSTSDTestHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_VERTEST ������
static NextFSMAction QUICKTESTSTVERTestHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_LCDTEST ������
static NextFSMAction QUICKTESTSTLCDTestHandler(CQuickTest *pMe);

#ifdef FEATURE_TOUCHPANEL
// ״̬ QUICKTESTST_TOUCHSCREENTEST ������
static NextFSMAction QUICKTESTSTTouchScreenTestHandler(CQuickTest *pMe);
#endif

// ״̬ QUICKTESTST_CALLTEST ������
static NextFSMAction QUICKTESTSTCallTestHandler(CQuickTest *pMe);

// ״̬ QUICKTESTST_REGULATEST ������
static NextFSMAction QUICKTESTSTRegulateHandler(CQuickTest *pMe);


// ״̬ QUICKTESTST_RESTOREFACTORY ������
static NextFSMAction QUICKTESTSTRestoreFactoryHandler(CQuickTest *pMe);


// ״̬ QUICKTESTST_EXIT ������
static NextFSMAction QUICKTESTSTExitHandler(CQuickTest *pMe);

#ifdef FEATURE_BREW_CAMERA
// ״̬ QUICKTESTST_CAMERATEST ������
static NextFSMAction QUICKTESTSTCAMERATestHandler(CQuickTest *pMe);
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
       QuickTest_ProcessState
˵��:
       QuickTest Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��QuickTest Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction QuickTest_ProcessState(CQuickTest *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case QUICKTESTST_NONE:
            retVal = QUICKTESTSTNoOperationHandler(pMe);
            break;

        case QUICKTESTST_INIT:
            retVal = QUICKTESTSTInitHandler(pMe);
            break;

        case QUICKTESTST_MAIN:
            retVal = QUICKTESTSTMainHandler(pMe);
            break;

        case QUICKTESTST_KEYTEST:
            retVal = QUICKTESTSTKeyTestHandler(pMe);
            break;

#ifdef FEATURE_TOUCHPANEL
        case QUICKTESTST_TOUCHSCREENTEST:
            retVal = QUICKTESTSTTouchScreenTestHandler(pMe);
            break;
#endif

        case QUICKTESTST_YAMAHATEST:
            retVal = QUICKTESTSTYAMAHATestHandler(pMe);
            break;

        case QUICKTESTST_VIBRATETEST:
            retVal = QUICKTESTSTVibrateTestHandler(pMe);
            break;

        case QUICKTESTST_BACKLIGHTTEST:
            retVal = QUICKTESTSTBackLightTestHandler(pMe);
            break;
            
        case QUICKTESTST_FLIPTEST:
            retVal = QUICKTESTSTFlipTestHandler(pMe);
            break;            

        case QUICKTESTST_FMTEST:
            retVal = QUICKTESTSTFMTestHandler(pMe);
            break;
#ifdef FEATURE_BREW_CAMERA
        case QUICKTESTST_CAMERATEST:
            retVal = QUICKTESTSTCAMERATestHandler(pMe);
            break;
#endif
        case QUICKTESTST_SDTEST:
            retVal = QUICKTESTSTSDTestHandler(pMe);
            break;

        case QUICKTESTST_VERTEST:
            retVal = QUICKTESTSTVERTestHandler(pMe);
            break;

        case QUICKTESTST_LCDTEST:
            retVal = QUICKTESTSTLCDTestHandler(pMe);
            break;

        case QUICKTESTST_CALLTEST:
            retVal = QUICKTESTSTCallTestHandler(pMe);
            break;

        case QUICKTESTST_REGULATE:
            retVal = QUICKTESTSTRegulateHandler(pMe);
            break;      

        case QUICKTESTST_RESTOREFACTORY:
            retVal = QUICKTESTSTRestoreFactoryHandler(pMe);
            break;          

        case QUICKTESTST_EXIT:
            retVal = QUICKTESTSTExitHandler(pMe);
            break;

        default:
            break;
    }
    return retVal;
}

/*==============================================================================
������
       QUICKTESTSTNoOperationHandler
˵����
       QUICKTESTST_NONE ״̬������

������
       pMe [in]��ָ��QucikTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction QUICKTESTSTNoOperationHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(QUICKTESTST_EXIT);
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler

/*==============================================================================
������QUICKTESTSTInitHandler

˵����

����״̬QUICKTESTST_INITʱ���������á�����״̬��������������һ״̬ΪSTATE_MAIN��

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTInitHandler(CQuickTest *pMe)
{
    MOVE_TO_STATE(QUICKTESTST_MAIN);
    return NFSMACTION_CONTINUE;
} //StateInitHandler

/*==============================================================================
������QUICKTESTSTMainHandler

˵����
       ����״̬QUICKTESTST_MAINʱ���������á���������״̬QUICKTESTST_MAIN����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTMainHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_MAINMENU);
            return NFSMACTION_WAIT;

        case DLGRET_KEYTEST:
            MOVE_TO_STATE(QUICKTESTST_KEYTEST)
            return NFSMACTION_CONTINUE;

        case DLGRET_TOUCHSCREENTEST:
            MOVE_TO_STATE(QUICKTESTST_TOUCHSCREENTEST)
            return NFSMACTION_CONTINUE;

        case DLGRET_YAMAHATEST:
            MOVE_TO_STATE(QUICKTESTST_YAMAHATEST)
            return NFSMACTION_CONTINUE;

        case DLGRET_VIBRATETEST:
            MOVE_TO_STATE(QUICKTESTST_VIBRATETEST)
            return NFSMACTION_CONTINUE;

        case DLGRET_BACKLIGHTTEST:
            MOVE_TO_STATE(QUICKTESTST_BACKLIGHTTEST)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_FLIPTEST:
            MOVE_TO_STATE(QUICKTESTST_FLIPTEST)
            return NFSMACTION_CONTINUE;            

        case DLGRET_FMTEST:
            MOVE_TO_STATE(QUICKTESTST_FMTEST)
            return NFSMACTION_CONTINUE;
#ifdef FEATURE_BREW_CAMERA
        case DLGRET_CAMERATEST:
            MOVE_TO_STATE(QUICKTESTST_CAMERATEST)
            return NFSMACTION_CONTINUE;            
#endif
        case DLGRET_SDTEST:
            MOVE_TO_STATE(QUICKTESTST_SDTEST)
            return NFSMACTION_CONTINUE;

        case DLGRET_VERTEST:
            MOVE_TO_STATE(QUICKTESTST_VERTEST)
            return NFSMACTION_CONTINUE;

        case DLGRET_LCDTEST:
            MOVE_TO_STATE(QUICKTESTST_LCDTEST)
            return NFSMACTION_CONTINUE;

        case DLGRET_CALLTEST:
            MOVE_TO_STATE(QUICKTESTST_CALLTEST)
            return NFSMACTION_CONTINUE;

        case DLGRET_REGULATE:
            MOVE_TO_STATE(QUICKTESTST_REGULATE)
            return NFSMACTION_CONTINUE;     

        case DLGRET_RESTOREFACTORY:
            MOVE_TO_STATE(QUICKTESTST_RESTOREFACTORY)
            return NFSMACTION_CONTINUE;
           

        case DLGRET_CANCELED:
            MOVE_TO_STATE(QUICKTESTST_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTMainHandler

/*==============================================================================
QUICKTESTSTKeyTestHandler

˵����
       ����״̬QUICKTESTST_KEYTESTʱ���������á���������״̬QUICKTESTST_KEYTEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTKeyTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_KEYTEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
#ifdef FEATURE_SUPPORT_BT_APP
            pMe->m_dilag_type = BT_TEST;
            MOVE_TO_STATE(QUICKTESTST_LCDTEST) //BT +LCD
#else
            MOVE_TO_STATE(QUICKTESTST_CALLTEST)
#endif
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTKeyTestHandler

/*==============================================================================
QUICKTESTSTYAMAHATestHandler

˵����
       ����״̬QUICKTESTST_YAMAHATESTʱ���������á���������״̬QUICKTESTST_YAMAHATEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTYAMAHATestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_YAMAHATEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
#ifdef FEATURE_VERSION_C337
            MOVE_TO_STATE(QUICKTESTST_VIBRATETEST)
#else
            MOVE_TO_STATE(QUICKTESTST_KEYTEST)
#endif            
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTYAMAHATestHandler

/*==============================================================================
QUICKTESTSTVibrateTestHandler

˵����
       ����״̬QUICKTESTST_VIBRATETESTʱ���������á���������״̬QUICKTESTST_VIBRATETEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTVibrateTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_VIBRATETEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
#ifdef FEATURE_VERSION_C337
            MOVE_TO_STATE(QUICKTESTST_KEYTEST)
#else
            MOVE_TO_STATE(QUICKTESTST_YAMAHATEST)
#endif            
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTVibrateTestHandler

/*==============================================================================
QUICKTESTSTBackLightTestHandler

˵����
       ����״̬QUICKTESTST_BACKLIGHTTESTʱ���������á���������״̬QUICKTESTST_BACKLIGHTTEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTBackLightTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_BACKLIGHTTEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
#ifdef FEATRUE_DEVICETYPE_FLIP               
            MOVE_TO_STATE(QUICKTESTST_FLIPTEST)
#else //FEATRUE_DEVICETYPE_FLIP  
#if 1 //ndef FEATURE_VERSION_C180 //xxzhen
#if defined (FEATURE_VERSION_C337)||defined(FEATURE_NO_VIBRATE)
            MOVE_TO_STATE(QUICKTESTST_YAMAHATEST)
#else
            MOVE_TO_STATE(QUICKTESTST_VIBRATETEST)
#endif            
#else
            MOVE_TO_STATE(QUICKTESTST_YAMAHATEST)
#endif
#endif //FEATRUE_DEVICETYPE_FLIP  
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTBackLightTestHandler

/*==============================================================================
QUICKTESTSTFlipTestHandler

˵����
       ����״̬QUICKTESTST_BACKLIGHTTESTʱ���������á���������״̬QUICKTESTST_BACKLIGHTTEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTFlipTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_FLIPTEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(QUICKTESTST_VIBRATETEST)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTFlipTestHandler

/*==============================================================================
QUICKTESTSTFMTestHandler

˵����
       ����״̬QUICKTESTST_FMTESTʱ���������á���������״̬QUICKTESTST_FMTEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTFMTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_FMTEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            //MOVE_TO_STATE(QUICKTESTST_SDTEST)
#ifdef FEATURE_BREW_CAMERA            
            MOVE_TO_STATE(QUICKTESTST_CAMERATEST)
#else       
            MOVE_TO_STATE(QUICKTESTST_SDTEST)
#endif            
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}//

#ifdef FEATURE_BREW_CAMERA 
static NextFSMAction QUICKTESTSTCAMERATestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_APP_CAMERA, "Formquicktest");
            pMe->m_isFormCamera = TRUE;
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(QUICKTESTST_SDTEST)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }
    return NFSMACTION_WAIT;
}//
#endif
/*==============================================================================
QUICKTESTSTSDTestHandler

˵����
       ����״̬QUICKTESTST_SDTESTʱ���������á���������״̬QUICKTESTST_SDTEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTSDTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_SDTEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            //MOVE_TO_STATE(QUICKTESTST_RESTOREFACTORY)
            MOVE_TO_STATE(QUICKTESTST_REGULATE)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}//
/*==============================================================================
QUICKTESTSTVERTestHandler

˵����
       ����״̬QUICKTESTST_VERTESTʱ���������á���������״̬QUICKTESTST_VERTEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTVERTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_VERTEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
#ifdef FEATURE_SUPPORT_BT_APP
            pMe->m_dilag_type = LCD_TEST;
#endif
            MOVE_TO_STATE(QUICKTESTST_LCDTEST)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}//
/*==============================================================================
QUICKTESTSTLCDTestHandler

˵����
       ����״̬QUICKTESTST_LCDTESTʱ���������á���������״̬QUICKTESTST_LCDTEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTLCDTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_LCDTEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
#ifdef FEATURE_SUPPORT_BT_APP
            if(pMe->m_dilag_type == BT_TEST)
            {
                MOVE_TO_STATE(QUICKTESTST_CALLTEST)
            }
            else
#endif
            {
                MOVE_TO_STATE(QUICKTESTST_BACKLIGHTTEST)
            }
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTLCDTestHandler

#ifdef FEATURE_TOUCHPANEL
/*==============================================================================
QUICKTESTSTTouchScreenTestHandler

˵����
       ����״̬QUICKTESTST_TOUCHSCREENTESTʱ���������á���������״̬
       QUICKTESTST_TOUCHSCREENTEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTTouchScreenTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_TOUCHSCREENTEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(QUICKTESTST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTTouchScreenTestHandler
#endif

/*==============================================================================
QUICKTESTSTCallTestHandler

˵����
       ����״̬QUICKTESTST_CALLTESTʱ���������á���������״̬
       QUICKTESTST_CALLTEST����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTCallTestHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_CALLTEST);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
			if(pMe->m_quicktestmic )
			{
				 MOVE_TO_STATE(QUICKTESTST_EXIT)
			}
			else
			{
#if defined( FEATURE_VERSION_K212)||defined(FEATURE_VERSION_K212_12832)
			MOVE_TO_STATE(QUICKTESTST_CAMERATEST)
#else
#if defined( FEATURE_FM_RADIO)
            MOVE_TO_STATE(QUICKTESTST_FMTEST)
#else
            MOVE_TO_STATE(QUICKTESTST_SDTEST)
#endif
#endif
			}
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTCallTestHandler

/*==============================================================================
QUICKTESTSTRestoreFactoryHandler

˵����
       ����״̬QUICKTESTST_RESTOREFACTORYʱ���������á���������״̬
       QUICKTESTST_RESTOREFACTORY����ز�����

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTRestoreFactoryHandler(CQuickTest *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_RESTORE_FACTORY);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(QUICKTESTST_EXIT)            
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTRestoreFactoryHandler


static NextFSMAction QUICKTESTSTRegulateHandler(CQuickTest * pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch (pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            QuickTest_ShowDialog(pMe, IDD_REGULATE);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(QUICKTESTST_RESTOREFACTORY)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
}// QUICKTESTSTRestoreFactoryHandler


/*==============================================================================
������QUICKTESTSTExitHandler

˵����
       ����״̬QUICKTESTST_EXITʱ���������á����ڹر�Applet��

������
       pMe [in]��ָ��CQuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
        ��

��ע��:

==============================================================================*/
static NextFSMAction QUICKTESTSTExitHandler(CQuickTest *pMe)
{
    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // QUICKTESTSTExitHandler
