/*==============================================================================
// �ļ���
//        SoundMenuStateHandler.c
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
-----------      ----------  --------  -----------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "SoundMenu_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ SOUNDMENUST_NONE ������
static NextFSMAction Sound_StateNoOperationHandler(CSoundMenu *pMe);

// ״̬ SOUNDMENUST_INIT ������
static NextFSMAction Sound_StateInitHandler(CSoundMenu *pMe);

// ״̬ SOUNDMENUST_MAIN ������
static NextFSMAction Sound_StateMainHandler(CSoundMenu *pMe);

// ״̬ SOUNDMENUST_HINT ������
static NextFSMAction Sound_StateHintHandler(CSoundMenu *pMe);

// ״̬ SOUNDMENUST_RINGER ������
static NextFSMAction Sound_StateRingerHandler(CSoundMenu *pMe);

#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
// ״̬ SOUNDMENUST_SMSRING ������
static NextFSMAction Sound_StateSmsRingHandler(CSoundMenu *pMe);
#endif

// ״̬ SOUNDMENUST_VOLUME ������
static NextFSMAction Sound_StateVolumeHandler(CSoundMenu *pMe);

//static NextFSMAction StateOtherHandler(CSoundMenu *pMe);

static NextFSMAction Sound_StateOtherSelHandler(CSoundMenu *pMe);

// ״̬ SOUNDMENUST_VOLUMESUB ������
static NextFSMAction Sound_StateVolumeSubHandler(CSoundMenu *pMe);

// ״̬ SOUNDMENUST_EXIT ������
static NextFSMAction Sound_StateExitHandler(CSoundMenu *pMe);

//#ifdef KEYSND_ZY
// ״̬ SOUNDMENUST_KEYSND ������
static NextFSMAction Sound_StateKeySndHandler(CSoundMenu *pMe);
//#endif /* KEYSND_ZY */

// ״̬ DLGRET_KEYLENGTH ������
//static NextFSMAction Sound_StateKeylengthHandler(CSoundMenu *pMe);

#if 0
//״̬ SOUNDMENUST_WARNING ������
static NextFSMAction Sound_StateWarningHandler(CSoundMenu *pMe);
#endif
#if 0
// ״̬ SOUNDMENUST_AUTOANSWERSUB ������
static NextFSMAction Sound_StateAutoAnswerSubHandler(CSoundMenu *pMe);
#endif
//״̬ SOUNDMENUST_SOUNDMENU ������
static NextFSMAction Sound_StateSoundMenuHandler(CSoundMenu *pMe);

// ״̬ SOUNDMENUST_SCENEMODESUB ������
static NextFSMAction Sound_StateSceneModeSubHandler(CSoundMenu *pMe);

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
       SoundMenu_ProcessState
˵��:
       SoundMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��SoundMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction SoundMenu_ProcessState(CSoundMenu *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case SOUNDMENUST_NONE:
            retVal = Sound_StateNoOperationHandler(pMe);
            break;

        case SOUNDMENUST_INIT:
            retVal = Sound_StateInitHandler(pMe);
            break;

        case SOUNDMENUST_MAIN:
            retVal = Sound_StateMainHandler(pMe);
            break;

        case SOUNDMENUST_SCENEMODESUB:
            retVal = Sound_StateSceneModeSubHandler(pMe);
            break;

        case SOUNDMENUST_SOUNDMENU:
            retVal = Sound_StateSoundMenuHandler(pMe);
            break;

        case SOUNDMENUST_HINT:
            retVal = Sound_StateHintHandler(pMe);
            break;

        case SOUNDMENUST_RINGER:
            retVal = Sound_StateRingerHandler(pMe);
            break;

#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
        case SOUNDMENUST_SMSRING:
            retVal = Sound_StateSmsRingHandler(pMe);
            break;
#endif

        case SOUNDMENUST_OTHERSEL:
            retVal = Sound_StateOtherSelHandler(pMe);
            break;
#if 0
        case SOUNDMENUST_AUTOANSWERSUB:
            retVal = Sound_StateAutoAnswerSubHandler(pMe);
            break;
#endif
        case SOUNDMENUST_KEYSND:
            retVal = Sound_StateKeySndHandler(pMe);
            break;

        //case SOUNDMENUST_KEYLENGTH:
        //    retVal = Sound_StateKeylengthHandler(pMe);
        //    break;

        case SOUNDMENUST_VOLUME:
            retVal = Sound_StateVolumeHandler(pMe);
            break;

        case SOUNDMENUST_VOLUMESUB:
            retVal = Sound_StateVolumeSubHandler(pMe);
            break;

#if 0
        case SOUNDMENUST_WARNING:
            retVal = Sound_StateWarningHandler(pMe);
            break;
#endif

        case SOUNDMENUST_EXIT:
            retVal = Sound_StateExitHandler(pMe);
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
       SOUNDMENUST_NONE ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateNoOperationHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(SOUNDMENUST_EXIT)
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler


/*==============================================================================
������
       StateInitHandler
˵����
       SOUNDMENUST_INIT ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateInitHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    if(ISHELL_RegisterNotify(pMe->m_pShell, AEECLSID_APP_SOUNDMENU,
        AEECLSID_ALERT_NOTIFIER, NMASK_ALERT_ONOFF | NMASK_ALERT_MUTED) != SUCCESS)
    {
        return NFSMACTION_CONTINUE;
    }
    MOVE_TO_STATE(SOUNDMENUST_MAIN)
    return NFSMACTION_CONTINUE;
} // StateInitHandler


/*==============================================================================
������
       StateMainHandler
˵����
       SOUNDMENUST_MAIN ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateMainHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_SCENEMODE_MENU);
            return NFSMACTION_WAIT;
        case DLGRET_MESSAGE:
            SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;
            
        case DLGRET_SCENEMODESUB:
            MOVE_TO_STATE(SOUNDMENUST_SCENEMODESUB)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_OK:
            pMe->m_sSubDlgId = IDS_WARNING_MESSEGE;
            SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SOUNDMENUST_EXIT)
            return NFSMACTION_CONTINUE;
        case DLGRET_OK:
            MOVE_TO_STATE(SOUNDMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateSceneModeHandler


/*==============================================================================
������
       StateSceneModeSubHandler
˵����
       SOUNDMENUST_SCENEMODESUB ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateSceneModeSubHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_SCENEMODE_SUB);
            return NFSMACTION_WAIT;

        case DLGRET_SOUNDMENU:
            switch(app_media_scheduler())
            {
                case APP_MEDIA_IMPACT_BY_FM:
                    pMe->m_sSubDlgId = IDS_CONFIRM_OFF_FM;
                    SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
                    return NFSMACTION_WAIT;

                case APP_MEDIA_IMPACT_BY_MP3:
                	/*
                    pMe->m_sSubDlgId = IDS_CONFIRM_OFF_MP;
                    SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
                    return NFSMACTION_WAIT;
                    */

                    //Add By zzg 2010_08_30                   
                    MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
                    return NFSMACTION_CONTINUE;
                    //Add End

                default:
                    MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
                    return NFSMACTION_CONTINUE;
            }

        case DLGRET_MSGBOX_OK:

            if(pMe->m_sSubDlgId == IDS_ACTIVATING || pMe->m_sSubDlgId == IDS_RESTORED)
            {
                pMe->m_sSubDlgId = IDS_WARNING_MESSEGE;
                SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
                return NFSMACTION_WAIT;
            }
            
            if(pMe->m_sSubDlgId == IDS_CONFIRM_OFF_FM)
            {
                ISHELL_SendEvent( pMe->m_pShell, AEECLSID_APP_FMRADIO, EVT_CLOSEAPP, 0, 0); 
            }
            else if(pMe->m_sSubDlgId == IDS_CONFIRM_OFF_MP)
            {
                ISHELL_SendEvent( pMe->m_pShell, AEECLSID_APP_MUSICPLAYER, EVT_CLOSEAPP, 0, 0);
            }
            MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            return NFSMACTION_CONTINUE;

        case DLGRET_MSGBOX_CANCEL:
            MOVE_TO_STATE(SOUNDMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        case DLGRET_MESSAGE:
            SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;
            //return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
        case DLGRET_OK:
            MOVE_TO_STATE(SOUNDMENUST_MAIN)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }
    return NFSMACTION_WAIT;
} // StateSceneModeSubHandler


/*==============================================================================
������
       StateSoundMenuHandler
˵����
       SOUNDMENUST_SOUNDMENU ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateSoundMenuHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    pMe->m_fSubDlgId = 0;    //���¼����Ӳ˵�ѡ������λ��

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            //if (pMe->m_bNaturalStart == NORMAL_ENTRY)
            //{  // ������������ Applet
            SoundMenu_ShowDialog(pMe, IDD_SOUNDMENU_PROFILES);
            //}
            //else if(pMe->m_bNaturalStart == SOUNDLIST_ENTRY)
            //{
            //  SoundMenu_ShowDialog(pMe, IDD_SOUNDMENU_MENU);
            //}
            return NFSMACTION_WAIT;

        case DLGRET_HINT:
            MOVE_TO_STATE(SOUNDMENUST_HINT)
            return NFSMACTION_CONTINUE;

        case DLGRET_RINGER:
            MOVE_TO_STATE(SOUNDMENUST_RINGER)
            return NFSMACTION_CONTINUE;

#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
        case DLGRET_SMSRING:
            MOVE_TO_STATE(SOUNDMENUST_SMSRING)
            return NFSMACTION_CONTINUE;
#endif

        case DLGRET_OTHERSEL:
            MOVE_TO_STATE(SOUNDMENUST_OTHERSEL)
            return NFSMACTION_CONTINUE;

        //case DLGRET_AUTOANSWERSUB:
        //    MOVE_TO_STATE(SOUNDMENUST_AUTOANSWERSUB)
        //    return NFSMACTION_CONTINUE;

        case DLGRET_KEYSND:
            MOVE_TO_STATE(SOUNDMENUST_KEYSND)
            return NFSMACTION_CONTINUE;

        //case DLGRET_KEYLENGTH:
        //    MOVE_TO_STATE(SOUNDMENUST_KEYLENGTH)
        //    return NFSMACTION_CONTINUE;

        case DLGRET_VOLUME:
            MOVE_TO_STATE(SOUNDMENUST_VOLUME)
            return NFSMACTION_CONTINUE;

#if 0
        case DLGRET_WARNING:
            MOVE_TO_STATE(SOUNDMENUST_WARNING)
            return NFSMACTION_CONTINUE;
#endif

        case DLGRET_CANCELED:
            if (pMe->m_bNaturalStart == NORMAL_ENTRY)
            {   // ������������ Applet
                MOVE_TO_STATE(SOUNDMENUST_MAIN)
            }
            else if(pMe->m_bNaturalStart == SOUNDLIST_ENTRY || (pMe->m_RingerType == SET_ANOTHER))
            {
                MOVE_TO_STATE(SOUNDMENUST_EXIT)
            }
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateSoundMenuHandler


/*==============================================================================
������
       StateHintHandler
˵����
       SOUNDMENUST_HINT ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateHintHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_HINT_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_MESSAGE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateHintHandler


/*==============================================================================
������
       StateRingerHandler
˵����
       SOUNDMENUST_RINGER ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateRingerHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_RINGER_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_MESSAGE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
        case DLGRET_CANCELED:
            if(pMe->m_RingerType == SET_ANOTHER)
            {
                MOVE_TO_STATE(SOUNDMENUST_EXIT)
            }
            else
            {
                MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            }
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateRingerHandler

#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
/*==============================================================================
������
       StateSmsRingHandler
˵����
       SOUNDMENUST_SMSRING ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateSmsRingHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_SMSRING_MENU);
            return NFSMACTION_WAIT;
            
        case DLGRET_MESSAGE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateSmsRingHandler
#endif

/*==============================================================================
������
       StateVolumeHandler
˵����
       SOUNDMENUST_VOLUME ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateVolumeHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_VOLUME_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_VOLUMESUB:
            MOVE_TO_STATE(SOUNDMENUST_VOLUMESUB)
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateVolumeHandler


/*==============================================================================
������
       StateOtherSelHandler
˵����
       SOUNDMENUST_OTHERSEL ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateOtherSelHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_OTHERSEL_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_MESSAGE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateOtherSelHandler


/*==============================================================================
������
       StateVolumeSubHandler
˵����
       SOUNDMENUST_VOLUMESUB ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateVolumeSubHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_VOLUME_SUB);
            return NFSMACTION_WAIT;
        case DLGRET_MESSAGE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
        case DLGRET_CANCELED:
            MOVE_TO_STATE(SOUNDMENUST_VOLUME)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateVolumeSubHandler

/*==============================================================================
������
       StateKeySndHandler
˵����
       SOUNDMENUST_KEYSND ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateKeySndHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_KEYSND_MENU);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateFlipSndHandler
#if 0
/*==============================================================================
������
       StateKeylengthHandler
˵����
       SOUNDMENUST_KEYLENGTH ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateKeylengthHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_KEYTONE_LENGTH);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateKeylengthHandler
#endif
#if 0
/*==============================================================================
������
       StateAutoAnswerSubHandler
˵����
       SOUNDMENUST_AUTOANSWERSUB ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateAutoAnswerSubHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_AUTOANSWER_SUB);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            if (pMe->m_bNaturalStart == NORMAL_ENTRY)
            {   // ������������ Applet
                MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            }
            else if(pMe->m_bNaturalStart == AUTOANSWER_ENTRY)
            {
                MOVE_TO_STATE(SOUNDMENUST_EXIT)
            }
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateAutoAnswerSubHandler
#endif

#if 0
/*==============================================================================
������
       StateWarningHandler
˵����
       SOUNDMENUST_WARNING ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateWarningHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            SoundMenu_ShowDialog(pMe, IDD_WARNING_MESSEGE);
            return NFSMACTION_WAIT;

        case DLGRET_HINT:
            MOVE_TO_STATE(SOUNDMENUST_HINT)
            return NFSMACTION_CONTINUE;

        case DLGRET_RINGER:
            MOVE_TO_STATE(SOUNDMENUST_RINGER)
            return NFSMACTION_CONTINUE;

#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
        case DLGRET_SMSRING:
            MOVE_TO_STATE(SOUNDMENUST_SMSRING)
            return NFSMACTION_CONTINUE;
#endif

        case DLGRET_CANCELED:
            MOVE_TO_STATE(SOUNDMENUST_SOUNDMENU)
            return NFSMACTION_CONTINUE;

        default:
            ASSERT_NOT_REACHABLE;
    }

    return NFSMACTION_WAIT;
} // StateVolumeSubHandler
#endif


/*==============================================================================
������
       StateExitHandler
˵����
       SOUNDMENUST_EXIT ״̬������

������
       pMe [in]��ָ��SoundMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction Sound_StateExitHandler(CSoundMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler







