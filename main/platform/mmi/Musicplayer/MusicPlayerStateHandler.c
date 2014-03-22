/*==============================================================================
// �ļ���
//        MusicPlayerStateHandler.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "MusicPlayer_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
typedef NextFSMAction (*FSMSTATE_HANDLER)(CMusicPlayer *pMe);


/*==============================================================================
                                 ��������
==============================================================================*/
// ״̬ STATE_NONE ������
static NextFSMAction Handler_STATE_NONE(CMusicPlayer *pMe);

// ״̬ STATE_INIT ������
static NextFSMAction Handler_STATE_INIT(CMusicPlayer *pMe);

//״̬ STATE_PLAYMUSIC_WINDOWS ������
static NextFSMAction Handler_STATE_PLAYMUSIC_WINDOWS(CMusicPlayer *pMe);

/*״̬STATE_MAINMENU������*/
static NextFSMAction Handler_STATE_MAINOPTSMENU(CMusicPlayer *pMe);

/*״̬STATE_PLAYLIST������*/
static NextFSMAction Handler_STATE_PLAYLIST(CMusicPlayer *pMe);

/*״̬STATE_SET_AS_RINGTONE������*/
static NextFSMAction Handler_STATE_SET_AS_RINGTONE(CMusicPlayer *pMe);

/*״̬STATE_SETTINGA�Ĵ�����*/
static NextFSMAction Handler_STATE_SETTING(CMusicPlayer *pMe);

/*״̬STATE_PLAYLIST _OPTS�Ĵ�����*/
static NextFSMAction Handler_STATE_PLAYLIST_OPTS(CMusicPlayer *pMe);

/*���������б�STATE_CREATELIST�Ĵ�����*/
static NextFSMAction Handler_STATE_CREATEORRENAMELIST(CMusicPlayer *pMe);

/*�鿴�����б�����������STATE_PLAYLIST_VIEW״̬�Ĵ�����*/
static NextFSMAction Handler_STATE_PLAYLIST_VIEW(CMusicPlayer *pMe);

/*�Ծ���playlist��musiclist���в���*/
static NextFSMAction Handler_STATE_VIEW_OPTS(CMusicPlayer *pMe);

/*ɾ����ǰ�����б��µ����и���*/
static NextFSMAction Handler_STATE_VIEW_DELETEALL(CMusicPlayer *pMe);

/*�򵥲��Ŵ���״̬����GALLERY���õ�״̬������*/
static NextFSMAction Handler_STATE_SIMPLEPLAYER(CMusicPlayer *pMe);

/*���Ʋ�������ʱ�Ĵ���״̬*/
static NextFSMAction Handler_STATE_PEDOMETERCALLPLAYER(CMusicPlayer *pMe);
// ״̬ STATE_MSG ������
static NextFSMAction Handler_STATE_MSG(CMusicPlayer *pMe);

//yes or no close FM if FM run on background
static NextFSMAction Handler_STATE_MSGFMBGPLAYPROMPT(CMusicPlayer *pMe);
// ״̬ STATE_EXIT ������
static NextFSMAction Handler_STATE_EXIT(CMusicPlayer *pMe);
/*״̬STATE_MSGLISTFULL������*/
static NextFSMAction Handler_STATE_MSGLISTFULL(CMusicPlayer *pMe);
#if defined(FEATURE_FLASHLIGHT_SUPPORT)
static NextFSMAction Handler_STATE_FLASHLIGHT(CMusicPlayer *pMe);
#endif
// lizy add for data protect @20140321
#ifdef FEATURE_VERSION_C316
static NextFSMAction Handler_STATE_PWD(CMusicPlayer *pMe);
#endif

/*==============================================================================
                                 ȫ������
==============================================================================*/


/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
//ע�⣺������������б���_priv.h�е�FSMState���塰���롱Ҫ���ֶ�Ӧ��
//�򵥵������ǽ�״̬�б��Ƶ�����Ȼ����ǰ�����Handler_��
static const FSMSTATE_HANDLER gFSMStateHandler[] =
{
    Handler_STATE_NONE,
    Handler_STATE_INIT,
#ifdef FEATURE_VERSION_C316
	Handler_STATE_PWD,
#endif
    Handler_STATE_PLAYMUSIC_WINDOWS,
    Handler_STATE_MAINOPTSMENU,
    Handler_STATE_PLAYLIST,
    Handler_STATE_SET_AS_RINGTONE,
    Handler_STATE_SETTING,
    #if defined(FEATURE_FLASHLIGHT_SUPPORT)
	Handler_STATE_FLASHLIGHT,
	#endif
    Handler_STATE_MSG,
    Handler_STATE_PLAYLIST_OPTS,
    Handler_STATE_CREATEORRENAMELIST,
    Handler_STATE_PLAYLIST_VIEW,
    Handler_STATE_VIEW_OPTS,
    Handler_STATE_VIEW_DELETEALL,
    Handler_STATE_SIMPLEPLAYER,
    Handler_STATE_PEDOMETERCALLPLAYER,
    Handler_STATE_MSGFMBGPLAYPROMPT,
    Handler_STATE_MSGLISTFULL,
    Handler_STATE_EXIT
};


/*==============================================================================
                                 ��������
==============================================================================*/
/*==============================================================================
����:
       CMusicPlayer_ProcessState
˵��:
       MusicPlayer Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��MusicPlayer Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction CMusicPlayer_ProcessState(CMusicPlayer *pMe)
{
    int nSize;

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    nSize = ARRAY_SIZE(gFSMStateHandler);
    if ((int)pMe->m_eCurState < nSize )
    {
        // ���ݵ�ǰ״̬������Ӧ��״̬������
        return  gFSMStateHandler[pMe->m_eCurState](pMe);
    }
    else
    {
#if defined(AEE_STATIC)
 //      ASSERT_NOT_REACHABLE
#endif
        return NFSMACTION_WAIT;
    }
}
/*==============================================================================
������
       Handler_STATE_NONE
˵����
       STATE_NONE ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_NONE(CMusicPlayer *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif

    MOVE_TO_STATE(STATE_EXIT);
    return NFSMACTION_CONTINUE;
} // Handler_STATE_NONE

/*==============================================================================
������
       Handler_STATE_INIT
˵����
       STATE_INIT ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_INIT(CMusicPlayer *pMe)
{
#ifdef FEATURE_VERSION_C316
	boolean locksel;
#endif

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
	DBGPRINTF("Handler_STATE_INIT enter here locksel %d,pwdwright %d",locksel,pMe->b_pwdWright);
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    if(pMe->m_eStartMethod==STARTMETHOD_NORMAL)
    {
    // liyz add for test @20140321
#ifdef FEATURE_VERSION_C316
		OEM_GetConfig(CFGI_MULTIMEDIA_LOCK_CHECK, &locksel, sizeof( locksel));
	    
		DBGPRINTF("Handler_STATE_INIT enter here locksel %d,pwdwright %d",locksel,pMe->b_pwdWright);
		if((locksel) && (!pMe->b_pwdWright))
		{
			MOVE_TO_STATE(STATE_PWD);
		}
		else
#endif
	    {
	      DBGPRINTF("Handler_STATE_INIT enter here");
		#if defined (FEATURE_VERSION_C337)|| defined (FEATURE_VERSION_W317A) || defined(FEATURE_VERSION_IC241A_MMX)|| defined (FEATURE_VERSION_KK5)//|| defined (FEATURE_VERSION_K212)
			MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
		#else
			//MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
	       pMe->m_nMsgResID = IDS_MSG_WAITING;
	       pMe->m_eMsgRetState = STATE_PLAYMUSIC_WINDOWS;
	       pMe->m_eMsgType = MESSAGE_WAITING;
	       MOVE_TO_STATE(STATE_MSG);
		#endif
	    }
	   
    }
    else if(pMe->m_eStartMethod==STARTMETHOD_SIMPLEPLAYER)
    {
        MOVE_TO_STATE(STATE_SIMPLEPLAYER);
    }
    else if(pMe->m_eStartMethod == STARTMETHOD_PEDOMETERCALLPLAYER)
    {
        MOVE_TO_STATE(STATE_PEDOMETERCALLPLAYER);
    }
    return NFSMACTION_CONTINUE;
}
#ifdef FEATURE_VERSION_C316
static NextFSMAction Handler_STATE_PWD(CMusicPlayer *pMe)
{
	MSG_FATAL("Handler_STATE_PWD....",0,0,0);
	switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
			if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_PWD))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;

        case MGDLGRET_PASS:    
			DBGPRINTF("Handler_STATE_PWD MGDLGRET_PASS");
			MOVE_TO_STATE(STATE_INIT);
            return NFSMACTION_CONTINUE;

        case MGDLGRET_FAILD:    
			MSG_FATAL("MGDLGRET_FAILD.............",0,0,0);
			CMusicPlayer_ShowDialog(pMe, IDD_PWD_INVAD);
            return NFSMACTION_WAIT;
		
        default:
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;
    }
}

#endif

/*==============================================================================
������
       Handler_STATE_PLAYMUSIC_WINDOWS
˵����
       STATE_PLAYMUSIC_WINDOWS ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_PLAYMUSIC_WINDOWS(CMusicPlayer *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
  
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {           
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_WINDOWS))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        }
        case DLGRET_OK:
            MOVE_TO_STATE(STATE_MAINOPTSMENU);
            break;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            break;
            
       default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
   }
 return NFSMACTION_CONTINUE;
    
}
/*==============================================================================
������
       Handler_STATE_MAINOPTSMENU
˵����
       STATE_MAINOPTSMENU ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_MAINOPTSMENU(CMusicPlayer *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_MAINMENU))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
        }

        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;
        
        case DLGRET_PLAY:          
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;

        case DLGRET_PAUSE:
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;
            
        case DLGRET_PLAYLIST:         
            pMe->m_nCurPlaylistID = 0;
            MOVE_TO_STATE(STATE_PLAYLIST);
            break;
            
        case DLGRET_ADDMUSIC_FAILED:
			//pMe->m_eCurState = STATE_MSGLISTFULL;
            MOVE_TO_STATE(STATE_MSGLISTFULL);
            //if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_ADD_FAILED))
            //{
             //   MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
             //   return NFSMACTION_CONTINUE;
            //}
            //MSG_FATAL("Handler_STATE_MAINOPTSMENU.........................",0,0,0);
            //MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            //return NFSMACTION_CONTINUE;
            //return NFSMACTION_WAIT;
            break;
            
        case DLGRET_SETASRINGTONE:            
            MOVE_TO_STATE(STATE_SET_AS_RINGTONE);         
            break;

        case DLGRET_SETTINGS:          
            pMe->m_nCtlID = IDC_MUSICPLAY_MODE;   //�ؼ�ID
            pMe->m_nPlayMode = pMe->m_MusicPlayerCfg.eMusicPlayMode;
            pMe->m_nVolume = pMe->m_MusicPlayerCfg.eMusicVolume;
            pMe->m_nMusicSort = pMe->m_MusicPlayerCfg.eMusicSortBy;
            MOVE_TO_STATE(STATE_SETTING);
            break;

        case DLGRET_MINIMIZE:
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;
//#ifdef FEATURE_SUPPORT_BT_APP
#if defined(FEATURE_FLASHLIGHT_SUPPORT)
		case DLGRET_FLASHLIGHT:
			MOVE_TO_STATE(STATE_FLASHLIGHT);
			break;
#endif
#ifdef FEATURE_SUPPORT_BT_AUDIO		//Modify by zzg 2011_10_19
        case DLGRET_UNCONNECT_BT:
             pMe->m_eMsgType = MESSAGE_WARNNING;
             if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_UNCONNECT_BT))
             {
                 MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
                 return NFSMACTION_CONTINUE;
             }
             return NFSMACTION_WAIT;

        case DLGRET_OPENNING_SCO:
             pMe->m_eMsgType = MESSAGE_WAITING;
              if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_OPENNING_BT_SCO))
              {
                  MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
                  return NFSMACTION_CONTINUE;
              }
              return NFSMACTION_WAIT;

        case DLGRET_CLOSED_BT:
            pMe->m_eMsgType = MESSAGE_INFORMATION;
           if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_DONE))
           {
              MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
              return NFSMACTION_CONTINUE;
           }
           return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;
#endif
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    return NFSMACTION_CONTINUE;
}
static NextFSMAction Handler_STATE_MSGLISTFULL(CMusicPlayer *pMe)
{
	#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
	MSG_FATAL("Handler_STATE_MSG.................",0,0,0);
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            pMe->m_nMsgResID = IDS_MSG_ADD_FAILED;
            if(SUCCESS != CMusicPlayer_ShowDialog(pMe, IDD_MSGFULL))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_MSGBOX_OK:
            //MOVE_TO_STATE(pMe->m_eMsgRetState);
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            return NFSMACTION_CONTINUE;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}
/*==============================================================================
������
       Handler_STATE_PLAYLIST
˵����
       STATE_PLAYLIST ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_PLAYLIST(CMusicPlayer *pMe)
{
   #if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog message
            if(SUCCESS != CMusicPlayer_ShowDialog(pMe, IDD_PLAYLIST_MAINMENU))
            {
                //MOVE_TO_STATE(STATE_EXIT);
                MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
         case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;
            
        case DLGRET_OK:
            MOVE_TO_STATE(STATE_PLAYLIST_OPTS);
            break;
            
       case DLGRET_CREATEORRENAMELIST:
            MOVE_TO_STATE(STATE_CREATEORRENAMELIST);
            break;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_PLAYLIST);
            break;
            
        //case DLGRET_CREATELIST_FAILED:
          //  pMe->m_eMsgType = MESSAGE_WARNNING;
          //  if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_ADD_FAILED))
          //  {
           //     MOVE_TO_STATE(STATE_PLAYLIST);
           //     return NFSMACTION_CONTINUE;
           // }
           // return NFSMACTION_WAIT;
        case DLGRET_PLAYLISTPLAY:
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;

        case DLGRET_NO_RECORD:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_NORECORD))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_ERR_FORMAT:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_ERR_FORMAT))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;

}
/*==============================================================================
������
       Handler_STATE_SET_AS_RINGTONE
˵����
       STATE_SET_AS_RINGTONE״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SET_AS_RINGTONE(CMusicPlayer *pMe)
{
  #if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_SETASRINGTONE))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
           
            return NFSMACTION_WAIT;
        }
        case DLGRET_CANCELED:   
              MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
              break;

        case DLGRET_SELECT_NONE:
             pMe->m_eMsgType = MESSAGE_WARNNING;
             if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_SELECT_NONE))
            {
                MOVE_TO_STATE(STATE_SET_AS_RINGTONE);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_OK:
            pMe->m_eMsgType = MESSAGE_INFORMATIVE;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_DONE))
            {
                MOVE_TO_STATE(STATE_SET_AS_RINGTONE);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
            if(pMe->m_eMsgType == MESSAGE_WARNNING)
            {
                MOVE_TO_STATE(STATE_SET_AS_RINGTONE);
            }
            else
            {
               MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            }
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
   }
   return NFSMACTION_CONTINUE;	

}
#if defined(FEATURE_FLASHLIGHT_SUPPORT)
static NextFSMAction Handler_STATE_FLASHLIGHT(CMusicPlayer *pMe)
{
#if defined(AEE_STATIC)
		ASSERT(pMe != NULL);
#endif

    switch (pMe->m_eDlgRet)
    {
        // ����������
        case DLGRET_CREATE:
            {
            	  MSG_FATAL("Handler_STATE_FLASHLIGHT",0,0,0);
				  if(SUCCESS != CMusicPlayer_ShowDialog(pMe, IDD_FLASHLIGHT_SETTING))
				  {
					  MOVE_TO_STATE(STATE_EXIT);
					  return NFSMACTION_CONTINUE;
				  }
				  
				  return NFSMACTION_WAIT;
            }
         case DLGRET_CANCELED:         
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
			break;
        default:
		#if defined(AEE_STATIC)
			ASSERT_NOT_REACHABLE
		#endif
			break;

    }
}
#endif
/*==============================================================================
������
       Handler_STATE_SETTING
˵����
       STATE_SETTING ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_SETTING(CMusicPlayer *pMe)
{
 #if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {          
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_SET_MAINMENU))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
           
            return NFSMACTION_WAIT;
        }

        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
             MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
             break;

        case DLGRET_OK:
            pMe->m_eMsgType = MESSAGE_INFORMATIVE;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_DONE))
            {
                MOVE_TO_STATE(STATE_SETTING);
                return NFSMACTION_CONTINUE;
            }
                        
            return NFSMACTION_WAIT; 
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
             break;
   }
   return NFSMACTION_CONTINUE;

}
/*==============================================================================
������
       Handler_STATE_PLAYLIST_OPTS
˵����
       STATE_PLAYLIST_OPTS ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_PLAYLIST_OPTS(CMusicPlayer *pMe)
{
    char fileName[MP3_MAX_FILE_NAME];
 #if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
     
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_PLAYLIST_OPTS))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
          
            return NFSMACTION_WAIT;
        }

        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_PLAYLIST);
            break;
        
        case DLGRET_CREATEORRENAMELIST:
            MOVE_TO_STATE(STATE_CREATEORRENAMELIST);
            break;

        case DLGRET_ADDMUSIC_FAILED:
        case DLGRET_CREATELIST_FAILED:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_ADD_FAILED))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_RENAMELIST_FAILED:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_RENAME_FAILED))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_PLAYLISTPLAY:
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;

        case DLGRET_PLAYLISTVIEW:
            pMe->m_nCurMusiclistID = 0;
            MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
            break;

        case DLGRET_DELPLAYLISTWARN:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_DELPLAYLISTWARN))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_PLAYLISTDELETE:
             if(SUCCESS != CMusicPlayer_ShowYesNoDialog( pMe, 
                                                    IDS_YESNO_DEL,
                                                    NULL,
                                                    -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
       case DLGRET_YES:
        { 
            (void)STRCPY(fileName,MUSICPLAYLIST_DIR);
            (void)STRCAT(fileName,"/");
            (void)STRCAT(fileName,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName);
            (void)STRCAT(fileName,".txt");
            IFILEMGR_Remove(pMe->m_pFileMgr,fileName);
            if(STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)==0)
            {
                CMusicPlayer_ReleaseMedia(pMe);
                pMe->m_bPlaying = FALSE;
                pMe->m_bPaused = FALSE;
                pMe->m_MusicPlayerCfg.lastPlayPlaylist[0]='\0';
#ifdef FEATURE_VERSION_EC99
                (void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/jnstyle.mp3");
#elif defined(FEATURE_VERSION_K212_ND)
		(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Wuxinghongqi.mp3");
#elif defined(FEATURE_VERSION_W021_WSF_CN_FM)||defined(FEATURE_VERSION_W021_WSF_CN_P3)||defined(FEATURE_VERSION_W021_WSF_P7)
#if defined(FEATURE_VERSION_W021_WSF_CN_P6)
		(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Yiwangeshebude.mp3");
#elif defined(FEATURE_VERSION_W021_WSF_P7)
		(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Jiefangjunqichuanghao.mp3");
#else
		(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Away.mp3");
#endif
		(void)STRCPY(pMe->m_PlayingMusiclist[1].pMusicName,"1fs:/hsmm/music/Zadangbingderen.mp3");
#else
                (void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Away.mp3");
#endif
                pMe->m_MusicPlayerCfg.lastPlayMusicID=0;
#if defined(FEATURE_VERSION_W021_WSF_CN_FM)||defined(FEATURE_VERSION_W021_WSF_CN_P3)||defined(FEATURE_VERSION_W021_WSF_CN_P6)||defined(FEATURE_VERSION_W021_WSF_P7)
				pMe->m_nPlayinglistMusicNum = 2;
#else
                pMe->m_nPlayinglistMusicNum = 1;
#endif
                (void) ISHELL_SetTimer(pMe->m_pShell,300,(PFNNOTIFY)CMusicPlayer_InitMusic,pMe);
            }
            if(pMe->m_nCurPlaylistID == pMe->m_nPlaylistNum - 1)
            {
                pMe->m_nCurPlaylistID--;
            }
            MOVE_TO_STATE(STATE_PLAYLIST);
            break;

        }
        case DLGRET_NO:
             MOVE_TO_STATE(STATE_PLAYLIST);
            break;

        case DLGRET_NO_RECORD:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_NORECORD))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_ERR_FORMAT:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_ERR_FORMAT))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_PLAYLIST);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}
/*==============================================================================
������
       Handler_STATE_CREATEORRENAMELIST
˵����
       STATE_CREATEORRENAMELIST ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_CREATEORRENAMELIST(CMusicPlayer *pMe)
{
 #if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    switch(pMe->m_eDlgRet)
    {
        //case DLGRET_MSGBOX_OK:
        case DLGRET_CREATE:
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_CREATEORRENAMELIST))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        
        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_PLAYLIST);
            break;

        case DLGRET_OK:
            //pMe->m_bDifferentMsg = TRUE;
            pMe->m_eMsgType = MESSAGE_INFORMATIVE;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_DONE))
            {
                //pMe->m_bDifferentMsg = FALSE;
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }   
            return NFSMACTION_WAIT;
         
        case DLGRET_REPEAT_PLAYLISTNAME:
             FREEIF(pMe->m_pBuffer);
             pMe->m_eMsgType = MESSAGE_WARNNING;
             if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_REPEAT_PLAYLISTNAME))
            {
                MOVE_TO_STATE(STATE_CREATEORRENAMELIST);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        case DLGRET_INVALID_PLAYLISTNAME:
            FREEIF(pMe->m_pBuffer);
            pMe->m_eMsgType = MESSAGE_WARNNING;
             if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_INVALIDNAME))
            {
                MOVE_TO_STATE(STATE_CREATEORRENAMELIST);
                return NFSMACTION_CONTINUE;
            }
                
            return NFSMACTION_WAIT;
        case  DLGRET_INPUT_NULL:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_INPUTNULL))
            {
                MOVE_TO_STATE(STATE_CREATEORRENAMELIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_MSGBOX_OK:
            if(pMe->m_eMsgType == MESSAGE_WARNNING)
           {  
               // show options dialog
               if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_CREATEORRENAMELIST))
               {
                   MOVE_TO_STATE(STATE_EXIT);
                   return NFSMACTION_CONTINUE;
               }
              
               return NFSMACTION_WAIT;
           }
           else
           {
               //pMe->m_bDifferentMsg = FALSE;
               MOVE_TO_STATE(STATE_PLAYLIST);
               break;
           }
           
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
           break;

    }
   return NFSMACTION_CONTINUE;
}
/*==============================================================================
������
       Handler_STATE_PLAYLIST_VIEW
˵����
       STATE_PLAYLIST_VIEW ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_PLAYLIST_VIEW(CMusicPlayer *pMe)
{
  #if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_PLAYLIST_VIEW))
        {
            //MOVE_TO_STATE(STATE_EXIT);
            MOVE_TO_STATE(STATE_PLAYLIST);
            return NFSMACTION_CONTINUE;
        }

            return NFSMACTION_WAIT;
        }

        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:
           MOVE_TO_STATE(STATE_PLAYLIST);
           break;

        case DLGRET_VIEWPLAY:         
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;
                
        case DLGRET_NO_RECORD:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_NORECORD))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_ERR_FORMAT:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_ERR_FORMAT))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
            break;

        case DLGRET_OK:
            MOVE_TO_STATE(STATE_VIEW_OPTS);
            break;
                
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}
/*==============================================================================
������
       Handler_STATE_VIEW_OPTS
˵����
       STATE_VIEW_OPTS ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_VIEW_OPTS(CMusicPlayer *pMe)
{
   int i;
 #if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_VIEW_OPTS))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
        }

        //Proccess yourself dialog retrn value here
        case DLGRET_CANCELED:     
            MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
            break;
        
        case DLGRET_VIEWPLAY:         
            MOVE_TO_STATE(STATE_PLAYMUSIC_WINDOWS);
            break;

        case DLGRET_DELPLAYLISTWARN:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_DELPLAYLISTWARN))
            {
                MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_VIEWDELETE:
            if(SUCCESS != CMusicPlayer_ShowYesNoDialog( pMe, 
                                                IDS_YESNO_DEL,
                                                NULL,
                                                -1))
            {
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_YES:
             for(i=0;i<MUSIC_MAX_FILESNUM;i++)
             {
                if(pMe->m_Musiclist[i].pMusicName[0]>pMe->m_Musiclist[pMe->m_nCurMusiclistID].pMusicName[0])
                {
                    pMe->m_Musiclist[i].pMusicName[0]--;
                }
             }
             for(i=pMe->m_nCurMusiclistID;i<MUSIC_MAX_FILESNUM-1;i++)
             {
              (void)STRCPY(pMe->m_Musiclist[i].pMusicName,pMe->m_Musiclist[i+1].pMusicName);
             }
             MEMSET(pMe->m_Musiclist[MUSIC_MAX_FILESNUM-1].pMusicName,
                    0,
                    MP3NAME_MAX_LEN*sizeof(char));
             (void)CMusicPlayer_WriteMusiclist(pMe ,
                                               pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName);
             if(STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,
                pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)==0)
             {
                CMusicPlayer_ReadMusiclist(pMe,
                                           pMe->m_MusicPlayerCfg.lastPlayPlaylist,
                                           TRUE);
                if(pMe->m_MusicPlayerCfg.lastPlayMusicID == pMe->m_nCurMusiclistID )
                {
                    CMusicPlayer_ReleaseMedia(pMe);
                    if(0 == pMe->m_nPlayinglistMusicNum)
                    {
                        pMe->m_MusicPlayerCfg.lastPlayPlaylist[0]='\0';
#ifdef FEATURE_VERSION_EC99
                        (void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/jnstyle.mp3");
#elif defined(FEATURE_VERSION_K212_ND)
		(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Wuxinghongqi.mp3");
#elif defined(FEATURE_VERSION_W021_WSF_CN_FM)||defined(FEATURE_VERSION_W021_WSF_CN_P3)||defined(FEATURE_VERSION_W021_WSF_P7)
#if defined(FEATURE_VERSION_W021_WSF_CN_P6)
		(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Yiwangeshebude.mp3");
#elif defined(FEATURE_VERSION_W021_WSF_P7)
	(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Jiefangjunqichuanghao.mp3");
#else
		(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Away.mp3");
#endif
		(void)STRCPY(pMe->m_PlayingMusiclist[1].pMusicName,"1fs:/hsmm/music/Zadangbingderen.mp3");
#else
                        (void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Away.mp3");
#endif
                        pMe->m_MusicPlayerCfg.lastPlayMusicID=0;
#if defined(FEATURE_VERSION_W021_WSF_CN_FM)||defined(FEATURE_VERSION_W021_WSF_CN_P3)||defined(FEATURE_VERSION_W021_WSF_CN_P6)||defined(FEATURE_VERSION_W021_WSF_P7)
						pMe->m_nPlayinglistMusicNum = 2;
#else
                        pMe->m_nPlayinglistMusicNum = 1;
#endif
                    }
                    (void) ISHELL_SetTimer(pMe->m_pShell,300,(PFNNOTIFY)CMusicPlayer_InitMusic,pMe);
                }
                else if(pMe->m_MusicPlayerCfg.lastPlayMusicID > pMe->m_nCurMusiclistID)
                {
                    pMe->m_MusicPlayerCfg.lastPlayMusicID --;
                }
            }
             if(pMe->m_nCurMusiclistID == pMe->m_nPlaylistMusicNum - 1 && pMe->m_nCurMusiclistID != 0)
             {
                pMe->m_nCurMusiclistID -- ;
             }
            pMe->m_eMsgType = MESSAGE_INFORMATIVE;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_DONE))
            {
                MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
                return NFSMACTION_CONTINUE;
            }   
            return NFSMACTION_WAIT;
             
       case DLGRET_NO:
             MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
             break;

       case DLGRET_NO_RECORD:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_NORECORD))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;

        case DLGRET_ERR_FORMAT:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_ERR_FORMAT))
            {
                MOVE_TO_STATE(STATE_PLAYLIST);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
        case DLGRET_MSGBOX_OK:
            MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
            break;
       case DLGRET_ADDMUSIC_FAILED:
            pMe->m_eMsgType = MESSAGE_WARNNING;
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_ADD_FAILED))
            {
                MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
            
       case DLGRET_VIEWDELETEALL:
            MOVE_TO_STATE(STATE_VIEW_DELETEALL);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}
/*==============================================================================
������
       Handler_STATE_VIEW_DELETEALL
˵����
       STATE_VIEW_DELETEALL ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_VIEW_DELETEALL(CMusicPlayer *pMe)
{
   int i;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    switch(pMe->m_eDlgRet)
    {
       case DLGRET_CREATE:
        {
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowYesNoDialog( pMe, 
                                                        IDS_YESNO_DELALL,
                                                        NULL,
                                                        -1))
            {
            MOVE_TO_STATE(STATE_EXIT);
            return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        }
        case DLGRET_YES:
        {
          for(i=0;i<MUSIC_MAX_FILESNUM;i++)
          {
             MEMSET(pMe->m_Musiclist[i].pMusicName,
                    0,
                    MP3NAME_MAX_LEN*sizeof(char));
          }
          (void)CMusicPlayer_WriteMusiclist(pMe ,
                                            pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName);
          if(STRCMP(pMe->m_MusicPlayerCfg.lastPlayPlaylist,pMe->m_Playlist[pMe->m_nCurPlaylistID].pPlaylistName)==0)
          {
            CMusicPlayer_ReleaseMedia(pMe);
            pMe->m_MusicPlayerCfg.lastPlayPlaylist[0]='\0';
#ifdef FEATURE_VERSION_EC99
            (void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/jnstyle.mp3");
#elif defined(FEATURE_VERSION_K212_ND)
		(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Wuxinghongqi.mp3");
#elif defined(FEATURE_VERSION_W021_WSF_CN_FM)||defined(FEATURE_VERSION_W021_WSF_CN_P3)||defined(FEATURE_VERSION_W021_WSF_P7)
#if defined(FEATURE_VERSION_W021_WSF_CN_P6)
		(void)STRCPY(pMe->m_PlayingMusiclist[2].pMusicName,"1fs:/hsmm/music/Yiwangeshebude.mp3");
#elif defined(FEATURE_VERSION_W021_WSF_P7)
	(void)STRCPY(pMe->m_PlayingMusiclist[2].pMusicName,"1fs:/hsmm/music/Jiefangjunqichuanghao.mp3");
#else
		(void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Away.mp3");
#endif
		(void)STRCPY(pMe->m_PlayingMusiclist[1].pMusicName,"1fs:/hsmm/music/Zadangbingderen.mp3");
#else
            (void)STRCPY(pMe->m_PlayingMusiclist[0].pMusicName,"1fs:/hsmm/music/Away.mp3");
#endif
            pMe->m_MusicPlayerCfg.lastPlayMusicID=0;
#if defined(FEATURE_VERSION_W021_WSF_CN_FM)||defined(FEATURE_VERSION_W021_WSF_CN_P3)||defined(FEATURE_VERSION_W021_WSF_CN_P6)||defined(FEATURE_VERSION_W021_WSF_P7)
			pMe->m_nPlayinglistMusicNum = 2;
#else
            pMe->m_nPlayinglistMusicNum = 1;
#endif
            (void) ISHELL_SetTimer(pMe->m_pShell,300,(PFNNOTIFY)CMusicPlayer_InitMusic,pMe);
          }
          pMe->m_eMsgType = MESSAGE_INFORMATIVE;
          if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, IDS_MSG_DONE))
         {
            MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
            return NFSMACTION_CONTINUE;
         }   
         return NFSMACTION_WAIT;
       }
        case DLGRET_MSGBOX_OK:
        case DLGRET_NO:
            MOVE_TO_STATE(STATE_PLAYLIST_VIEW);
            break;
            
           default:
#if defined(AEE_STATIC)
        ASSERT_NOT_REACHABLE
#endif
        break;
   }
    
    return NFSMACTION_CONTINUE;
   
}
/*==============================================================================
������
       Handler_STATE_SIMPLEPLAYER
˵����
       STATE_SIMPLEPLAYER ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/

static NextFSMAction Handler_STATE_SIMPLEPLAYER(CMusicPlayer *pMe)
{
 #if defined(AEE_STATIC)
     ASSERT(pMe != NULL);
#endif
  
    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
        {           
            // show options dialog
            if(SUCCESS != CMusicPlayer_ShowDialog( pMe, IDD_SIMPLEPLAYER))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            return NFSMACTION_WAIT;
        }
        case DLGRET_CANCELED:
            MOVE_TO_STATE(STATE_EXIT);
            break;
            
       default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
   }
 return NFSMACTION_CONTINUE;
    
}
/*==============================================================================
������
       Handler_STATE_PEDOMETERCALLPLAYER
˵����
       STATE_PEDOMETERCALLPLAYER ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
��ע��
       
==============================================================================*/

static NextFSMAction Handler_STATE_PEDOMETERCALLPLAYER(CMusicPlayer *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif

    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }
    pMe->m_MusicPlayerCfg.eMusicPlayMode = PLAYMODE_CYCLE;
    pMe->m_MusicPlayerCfg.eMusicVolume = VOLUME_THREE;
    pMe->m_bPlaying = TRUE;
    //��Ϊ�ײ�ʹ�õ����¼����ݻ��ƣ���Ҫ���ײ��ʼ��������notify��ʱ�䣬���������˴���0.5���Ӻ󲥷���һ��      
    (void) ISHELL_SetTimer(pMe->m_pShell,500,(PFNNOTIFY) CMusicPlayer_PlayMusic,pMe);
    return NFSMACTION_WAIT;
}

/*==============================================================================
������
       Handler_STATE_MSG
˵����
       STATE_MSG ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_MSG(CMusicPlayer *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
            
            if(SUCCESS != CMusicPlayer_ShowMsgBox(pMe, pMe->m_nMsgResID))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
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
} // Handler_STATE_MSG
/*==============================================================================
������
       Handler_STATE_MSGFMBGPLAYPROMPT
˵����
       STATE_MSGFMBGPLAYPROMPT ״̬������
       
������
       pMe [in]��ָ��CMusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/

static NextFSMAction Handler_STATE_MSGFMBGPLAYPROMPT(CMusicPlayer *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            // Show dialog
           if(SUCCESS != CMusicPlayer_ShowYesNoDialog( pMe, 
                                                        IDS_YESNOMSG_FMBGPLAY,
                                                        NULL,
                                                        -1))
            {
                MOVE_TO_STATE(STATE_EXIT);
                return NFSMACTION_CONTINUE;
            }
            
            return NFSMACTION_WAIT;
            
        //Proccess yourself dialog retrn value here
        case DLGRET_YES:
            ISHELL_SendEvent(pMe->m_pShell,
                             AEECLSID_APP_FMRADIO,
                             EVT_CLOSEAPP,
                             0,
                             0);  
            pMe->m_bStartApp = TRUE;
           (void) ISHELL_SetTimer(pMe->m_pShell,200,(PFNNOTIFY)MP3_InitMusicCB,pMe);
            MOVE_TO_STATE(STATE_INIT);
            break;

        case DLGRET_NO:
            //(void)ISHELL_CancelTimer(pMe->m_pShell,(PFNNOTIFY)MP3_InitMusicCB, pMe);
            MOVE_TO_STATE(STATE_EXIT);
            break;
            
        default:
#if defined(AEE_STATIC)
            ASSERT_NOT_REACHABLE
#endif
            break;
    }
    
    return NFSMACTION_CONTINUE;
}

/*==============================================================================
������
       Handler_STATE_EXIT
˵����
       STATE_EXIT ״̬������
       
������
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����
       
��ע��
       
==============================================================================*/
static NextFSMAction Handler_STATE_EXIT(CMusicPlayer *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    (void) ISHELL_CloseApplet(pMe->m_pShell,FALSE);
    return NFSMACTION_WAIT;
    
} // Handler_STATE_EXIT
