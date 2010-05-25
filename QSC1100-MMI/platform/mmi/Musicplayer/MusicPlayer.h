#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

/*==============================================================================
// �ļ���
//        MusicPlayer.h
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


/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��ӿڶ���
==============================================================================*/
typedef struct _IMusicPlayer  IMusicPlayer;
 QINTERFACE(IMusicPlayer)
{
    INHERIT_IApplet(IMusicPlayer);
    int (*SimplePlayer)(IMusicPlayer *p,char * musicName);
    int (*PedometerCallPlayer)(IMusicPlayer *p);
};

typedef enum Mp3Player_Status 
{
    MP3STATUS_NONE,     // =0,mp3Player is not run
    MP3STATUS_RUNONFOREGROUND,  // =1, mp3Player run on foreround
    MP3STATUS_RUNONBACKGROUND   // =2,mp3Player run on background
}Mp3Player_Status;
#define MUSICNAME_AUTO_SCROLL_TIME  500
/*==============================================================================
                                 ��ӿں�������
==============================================================================*/
#define IMUSICPLAYER_AddRef(p)           GET_PVTBL(p,IMusicPlayer)->AddRef(p)

#define IMUSICPLAYER_Release(p)          GET_PVTBL(p,IMusicPlayer)->Release(p)

#define IMUSICPLAYER_HandleEvent(p,ec,wp,dw) GET_PVTBL(p,IMusicPlayer)->HandleEvent(p, ec, wp, dw)
#define IMUSICPLAYER_SimplePlayer(p,musicName)  GET_PVTBL(p,IMusicPlayer)->SimplePlayer(p,musicName)
#define IMUSICPLAYER_PedometerCallPlayer(p)         GET_PVTBL(p, IMusicPlayer)->PedometerCallPlayer(p)
Mp3Player_Status GetMp3PlayerStatus(void);
#ifdef FEATURE_SUPPORT_BT_APP
//#ifdef PLAY_MUSIC_USING_BT_AG
boolean GetIsPlayMP3ByBt(void);
//#endif
#endif

#endif
//MUSICPLAYER_H

