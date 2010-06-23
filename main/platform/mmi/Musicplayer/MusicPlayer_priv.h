#ifndef MUSICPLAYER_PRIV_H
#define MUSICPLAYER_PRIV_H

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
#ifndef AEE_SIMULATOR
#include "OEMFeatures.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif

#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"        
#include "AEEMenu.h"          
#include "AEEFile.h" 
#include "AEEText.h"
#include "AEE.h"
#include "AEEMedia.h"         
#include "AEEMediaFormats.h"
#include "AEEImageCtl.h"      
#include "AEEMIMETypes.h"  
#include "AEEDisp.h"
//#include "err.h" 
#include "AEEMediaUtil.h"     
#include "OEMFS.h"
//#include "OEMAppFuncs.h"  
#include "AEEConfig.h"    
#include "AEEBacklight.h" 
#include "Appscommon.h"
#include "AppComFunc.h"
#if defined(FEATURE_DISP_160X128)
#include "Appscommon_160x128.brh"
#elif defined(FEATURE_DISP_128X128)
#include "Appscommon_color.brh"
#else
#include "Appscommon_color.brh"
#endif
#include "AEESound.h"
#include "OEMCFGI.h"
#include "AEECM.h"
#if defined(AEE_STATIC)
#include "AEE_static.h"
#endif
#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "g_sensor.h"
#endif
#ifndef WIN32
#include "ui.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32
#include "MusicPlayer.h"
#include "musicplayer.brh"
#include "musicplayerimage.brh"

//#ifdef FEATURE_SUPPORT_BT_APP
//#undef FEATURE_SUPPORT_BT_APP
//#endif

#ifndef WIN32
#ifdef FEATURE_SUPPORT_BT_APP
#include "bcmapp_ag.h"
#endif
#endif//WIN32

#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH
//����keypress��Ϣ�õĺ�
#define KEY_PRESS_DWPARM   10000
#endif//FEATURE_LCD_TOUCH_ENABLE
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
//For BREW3.0 support the muti-language re-define the RES maco
#define MUSICPLAYER_RES_FILE_LANG  AEE_RES_LANGDIR MUSICPLAYER_RES_FILE
//�����ļ���
#define MUSICPLAYLIST_DIR      "fs:/shared/playlist"
#define MP3_SD_MUSIC_DIR       "fs:/card0/music"
#define MP3_PHONE_MUSIC_DIR    "fs:/hsmm/music"
#define DEFAULT_PLAYLIST       "Default playlist"
#define  MUSICPLAYERCFG_VERSION     (0x0010)
//��֧�ֵ������ļ�����
#define MP3_MUSIC_TYPE     "midi,mp3,wav"

#define MIDI_TYPE    "mid"
#define QCP_TYPE     "qcp"
#define MP3_TYPE     "mp3"
#define AMR_TYPE     "amr"
#define AAC_TYPE     "aac"
#define WAV_TYPE     "wav"
//������ַ���������󳤶�
#define MAX_STR_LEN            64


//ÿ�������б���������ӵ��ļ���Ŀ
#define MUSIC_MAX_FILESNUM     50

//�����Դ����Ĳ����б����
#define PLAYLIST_MAX_NUM  10

//�б�������󳤶�
#define PLAYLISTNAME_MAX_LEN   MAX_STR_LEN  //��Ϊ�����������16�������֣�����ת��ΪUTF8������Ҫ48���ַ�

#define MP3_MAX_FILE_NAME      128
//����ȫ��(����·��)����󳤶�
#define MP3NAME_MAX_LEN   MP3_MAX_FILE_NAME+1



#define CONTROL_RECT_START_X    55  //������Ŀؼ�λ��

#define MAX_INPUT_NAME   16
#define STARTARGS_SIMPLEPLAYER ((char)'S')
#define STARTARGS_PEDOMETERCALLPLAYER ((char)'P')
//LIST�ؼ�
#define IDC_MUSICPLAY_MODE       1
#define IDC_MUSIC_VOLUME         2
#define IDC_MUSIC_SORT           3

#define MAX_LEN_MSGBOX                 (256)

#define TIMEOUT_MS_QUICK               (500)
#define MP3APPISREADY_TIME             (200)
#define MS_FASTFORWARDREWIND_TIME      (5000)  // ����

#define MP3NAME_COLOR                  MAKE_RGB(66,156,255)


#ifndef ERR
#define ERR DBGPRINTF
#endif

#ifndef ASSERT
#define ASSERT(x)\
if(!(x))         \
{                \
   return FALSE;    \
}
#endif

#define FARF_STATE   TRUE
#define FARF_EVENT   TRUE
#define FARF_ADDR    TRUE

#define MP3_FARF(x, p) if(TRUE == FARF_##x)  DBGPRINTF p

#if defined (FEATURE_DISP_160X128)
//wlh 20090415 add ����ICON ���� 
//��������
#define ANI_X 0
#define ANI_Y 0//27
#define ANI_W 176
#define ANI_H 82
//����֡��
#define ANI_NUM 5

//������
#define SCHEDULEBAR_X 25
#define SCHEDULEBAR_Y 40
#define SCHEDULEBAR_W 94
#define SCHEDULEBAR_H 7

//������(�򵥲���)
#define SIMSCHEDULEBAR_X 17
#define SIMSCHEDULEBAR_Y 147
#define SIMSCHEDULEBAR_W 94
#define SIMSCHEDULEBAR_H 7

//���Ű�ť
#define PLAY_X 60
#define PLAY_Y 54
#define PLAY_W 45
#define PLAY_H 44

//ǰһ����Ŀ
#define PREVIOUSPRESS_X 37
#define PREVIOUSPRESS_Y 56
#define PREVIOUSPRESS_W 31
#define PREVIOUSPRESS_H 32

//��һ����Ŀ
#define NEXTPRESS_X 107
#define NEXTPRESS_Y 56
#define NEXTPRESS_W 31
#define NEXTPRESS_H 32

//��������
#define DECREASEVOLUMEPRESS_X 2
#define DECREASEVOLUMEPRESS_Y 79
#define DECREASEVOLUMEPRESS_W 21
#define DECREASEVOLUMEPRESS_H 17

//��������
#define ADDVOLUMEPRESS_X 2
#define ADDVOLUMEPRESS_Y 43
#define ADDVOLUMEPRESS_W 18
#define ADDVOLUMEPRESS_H 18

//����
#define VOLUME_X 5
#define VOLUME_Y 60
#define VOLUME_W 12
#define VOLUME_H 18

//��ǰ���/������
#define LISTINDEX_X 5
#define LISTINDEX_Y 95

//��ǰ����ʱ��
#define PLAYINGTICK_X 105
#define PLAYINGTICK_Y 47
//��ǰ����ʱ�䣨�򵥲��ţ�
#define SIMPLAYINGTICK_X 60
#define SIMPLAYINGTICK_Y 155

//�������򣬰���ǰһ�ף����ţ���һ��
#define OLDPLAY_X 30
#define OLDPLAY_Y 68
#define OLDPLAY_W 114
#define OLDPLAY_H 45

#elif defined (FEATURE_DISP_128X128)
//��������
#define ANI_X 0
#define ANI_Y 0//27
#define ANI_W 176
#define ANI_H 82
//����֡��
#define ANI_NUM 5

//������
#define SCHEDULEBAR_X 25
#define SCHEDULEBAR_Y 40
#define SCHEDULEBAR_W 94
#define SCHEDULEBAR_H 7

//������(�򵥲���)
#define SIMSCHEDULEBAR_X 17
#define SIMSCHEDULEBAR_Y 147
#define SIMSCHEDULEBAR_W 94
#define SIMSCHEDULEBAR_H 7

//���Ű�ť
#define PLAY_X 51
#define PLAY_Y 58
#define PLAY_W 45
#define PLAY_H 44

//ǰһ����Ŀ
#define PREVIOUSPRESS_X 23
#define PREVIOUSPRESS_Y 62
#define PREVIOUSPRESS_W 31
#define PREVIOUSPRESS_H 32

//��һ����Ŀ
#define NEXTPRESS_X 93
#define NEXTPRESS_Y 62
#define NEXTPRESS_W 31
#define NEXTPRESS_H 32

//��������
#define DECREASEVOLUMEPRESS_X 2
#define DECREASEVOLUMEPRESS_Y 79
#define DECREASEVOLUMEPRESS_W 21
#define DECREASEVOLUMEPRESS_H 17

//��������
#define ADDVOLUMEPRESS_X 2
#define ADDVOLUMEPRESS_Y 43
#define ADDVOLUMEPRESS_W 18
#define ADDVOLUMEPRESS_H 18

//����
#define VOLUME_X 5
#define VOLUME_Y 60
#define VOLUME_W 12
#define VOLUME_H 18

//��ǰ���/������
#define LISTINDEX_X 5
#define LISTINDEX_Y 95

//��ǰ����ʱ��
#define PLAYINGTICK_X 75
#define PLAYINGTICK_Y 47
//��ǰ����ʱ�䣨�򵥲��ţ�
#define SIMPLAYINGTICK_X 60
#define SIMPLAYINGTICK_Y 155

//�������򣬰���ǰһ�ף����ţ���һ��
#define OLDPLAY_X 30
#define OLDPLAY_Y 68
#define OLDPLAY_W 96
#define OLDPLAY_H 45

#endif
typedef enum MP3Recttype
{
	TYPE_PLAYER,//pauseһ��
	TYPE_DECVOLUME,
	TYPE_ADDVOLUME,
	TYPE_PREVIOUS,
	TYPE_NEXT
}MP3Recttype;

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define MUSICPLAYER_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif//FEATURE_LCD_TOUCH_ENABLE
/*==============================================================================
                                 ȫ������
==============================================================================*/
/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   DLGRET_CREATE,
   
   DLGRET_PLAY,
   DLGRET_PAUSE,
   DLGRET_PLAYLIST,
   DLGRET_SETASRINGTONE,
   DLGRET_SETTINGS,
   DLGRET_MINIMIZE,
   
   DLGRET_CREATEORRENAMELIST,
   DLGRET_PLAYLISTPLAY,
   DLGRET_PLAYLISTVIEW,
   DLGRET_ADDMUSIC,
   DLGRET_PLAYLISTDELETE,
   
   DLGRET_VIEWPLAY,
   DLGRET_VIEWDELETE,
   DLGRET_VIEWDELETEALL,
   
   DLGRET_INPUT_NULL,
   DLGRET_CREATELIST_FAILED,
   DLGRET_RENAMELIST_FAILED,
   DLGRET_REPEAT_PLAYLISTNAME,
   DLGRET_DELPLAYLISTWARN,
   //DLGRET_INITMUSICFAILED,
   DLGRET_NO_RECORD,
   DLGRET_ERR_FORMAT,
   DLGRET_INVALID_PLAYLISTNAME,
   DLGRET_ADDMUSIC_FAILED,
   DLGRET_SELECT_NONE,
   DLGRET_UNCONNECT_BT,
   DLGRET_OPENNING_SCO,
   DLGRET_CLOSED_BT,
   // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_OK,
   DLGRET_NO,
   DLGRET_YES,
   DLGRET_MSGBOX_OK,
   DLGRET_CANCELED
} DLGRetValue;
//�����ȼ�
typedef enum CVolumeLevel {
    
    VOLUME_OFF,
    VOLUME_ONE,
    VOLUME_TWO,
    VOLUME_THREE,
    VOLUME_FOUR,
    VOLUME_FIVE

} CVolumeLevel;
//����ģʽ
typedef enum CPlayMode{
     PLAYMODE_SINGLE,
     PLAYMODE_REPEAT_ONE,
     PLAYMODE_ORDER,
     PLAYMODE_RANDOM,
     PLAYMODE_CYCLE
     
}CPlayMode;
//����ʽ
typedef enum CSort{
     SORT_NONE,
     //SORT_DATE,
     SORT_NAME
     //SORT_SIZE
}CSort;

typedef enum CPlayerSkin{
            PLAYERSKIN1,
            PLAYERSKIN2
            /*and more*/
}CPlayerSkin;
typedef struct _CMusiclist
{
  char pMusicName[MP3NAME_MAX_LEN];//�����洢����·��������չ���������ļ���
}CMusiclist;
typedef struct _CPlaylist
{
  char  pPlaylistName[PLAYLISTNAME_MAX_LEN];//�����洢������·��������չ���Ĳ����б��ļ���
}CPlaylist;
typedef struct _CMusicPlayerCfg
{
   CPlayMode eMusicPlayMode;
   CVolumeLevel eMusicVolume; 
   CSort eMusicSortBy;
#ifdef FEATURE_MP3PLAYER_SWITCHSKIN
   CPlayerSkin eMp3Skin;
#endif
   int lastPlayMusicID;//�˳�ǰ���һ�β��ŵ�����IDҲ������ʾ��ǰ���ڲ��ŵ�����ID
   char lastPlayPlaylist[PLAYLISTNAME_MAX_LEN];//�˳�ǰ���һ�β��ŵ��б���Ҳ������ʱ�洢���ڲ��ŵ��б���
}CMusicPlayerCfg;
typedef enum _AppletStartMethod{
    STARTMETHOD_NORMAL,
    STARTMETHOD_SIMPLEPLAYER,
    STARTMETHOD_PEDOMETERCALLPLAYER
}AppletStartMethod;

typedef struct _FileName{
   char m_szFileName[MP3NAME_MAX_LEN];
}FileName;
/*----------------------״̬�����������������---------------------*/
// MusicPlayer Applet ״̬��״̬��
typedef enum MP3State
{
   STATE_NONE = 0,               // No state
   STATE_INIT,                   // Initial state.
   STATE_PLAYMUSIC_WINDOWS,
   STATE_MAINOPTSMENU,
   STATE_PLAYLIST,
   STATE_SET_AS_RINGTONE,
   STATE_SETTING,
   STATE_MSG,
   STATE_PLAYLIST_OPTS,
   STATE_CREATEORRENAMELIST,
   STATE_PLAYLIST_VIEW,
   STATE_VIEW_OPTS,
   STATE_VIEW_DELETEALL,
   STATE_SIMPLEPLAYER,
   STATE_PEDOMETERCALLPLAYER,
   STATE_MSGFMBGPLAYPROMPT,
   STATE_EXIT                    // State where we close the applet.
} MP3State;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct CMusicPlayerMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} CMusicPlayerMod;
// MusicPlayer Applet����ṹ�壺
typedef struct _CMusicPlayer
{
    DECLARE_VTBL(IMusicPlayer)
    uint32          m_nRefs;          // �������ü�����
    IModule        *m_pModule;
    IDisplay       *m_pDisplay;
    IShell         *m_pShell;
    IConfig        *m_pConfig;

    AEERect       m_rc;

    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog             *m_pActiveDlg;

    // ��ǰ��Ի���ID
    uint16        m_pActiveDlgID;

    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRetValue   m_eDlgRet;

    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean m_bNotOverwriteDlgRet;

    // Applet ǰһ״̬
    MP3State m_ePreState;

    // Applet ��ǰ״̬
    MP3State m_eCurState;

    // Applet�Ƿ��ڹ���״̬
    boolean m_bSuspending;
    // Applet�Ƿ��ڻ״̬
    boolean             m_bActive;
    // Applet start type
    AppletStartMethod   m_eStartMethod;
    IImage     *m_pImage;
    IImage     *m_paniImage;
    //IMediaָ��
    IMedia     *m_pMedia;
    //IMediaָ��򵥲���ʱ��
    //IMedia     *m_psimplePlayMedia;
    CMusicPlayerCfg m_MusicPlayerCfg;
    //�������浱ǰ����Ĳ����б��������������
    CMusiclist  m_Musiclist[MUSIC_MAX_FILESNUM];
    //�������浱ǰ�����ŵĲ����б��������������
    CMusiclist  m_PlayingMusiclist[MUSIC_MAX_FILESNUM];
    //���ڱ��沥���б�
    CPlaylist        m_Playlist[PLAYLIST_MAX_NUM];
    //��������GALLERY���������ļ���
    char        *m_pSimplePlayMusicName;
    //�����б�����
    uint16  m_nPlaylistNum;
    //��ǰ����de�б��music����
    uint16  m_nPlayinglistMusicNum;
    //��ǰ����ؼ������б��music����
    uint16  m_nPlaylistMusicNum;
    AEEMediaData   m_md;
    //��ǰ����Ĳ����б�ID
    uint16    m_nCurPlaylistID;
    //��ǰ����������б�ID
    uint16    m_nCurMusiclistID;
    //settings
    IMenuCtl      *m_pMode; // IMenuCtlָ�벥��ģʽ
    IMenuCtl      *m_pVolume;//IMenuCtlָ������
    IMenuCtl      *m_pSort;//IMenuCtlָ�� ����ʽ
    //��ǰ���LIST�ؼ�ID
    uint16          m_nCtlID;
    AECHAR   *m_pInputListName;
    AECHAR   *m_pBuffer;
    AECHAR   *m_pMp3FileToPlay; // �������浱ǰ���ڲ��ŵĸ�����
    // message resource ID
    uint16              m_nMsgResID;
    // ָ��MessageBox��Ϣ��ָ�룬��̬�������ͷ�
    AECHAR             *m_pMsgBox_Msg;
    // Applet ����״̬
    MP3State        m_eMsgRetState;
    IFileMgr           *m_pFileMgr;       // IFileMgr interface  
    //��ǰ�����ŵ���ʱ��
    uint32    m_nCurrentTime;
    //�����ŵ����ֵ��ܵ�ʱ��
    uint32    m_nTotalTime;
    uint32    m_nSimPlayCurTime;
    //��ǰ������С
    uint16    m_nCurrentVolume;
    //�Ƿ����ڲ���
    boolean   m_bPlaying;
    //�Ƿ���ͣ
    boolean   m_bPaused;
    //�Ƿ����û�ֹͣ����
    boolean   m_bUserStopped;
    //�Ƿ����û�����������һ��
    boolean   m_bUserPressNext;
    //�Ƿ��ڲ�WAVE����
    boolean   m_bStartAni;
    //App�Ƿ�׼����
    boolean     m_bAppReady;
    //�Ƿ�����������������Ҫ��Ϊ�˴��븴�ðѴ��������б���������϶�Ϊһ
    boolean     m_bRenameOpt;
    //������ʾ����ʼIDX
    int        m_nAutoScrollIdx;
    //�Ƿ��Ǵ��������б���������������б�������ʾ��
    //boolean    m_bDifferentMsg;
    //�ڲ��ŷ���״̬ΪMM_STATUS_ABORTʱ�Ƿ񲻼�����
    boolean    m_bNotContinue;
    boolean    m_bStartApp;
    boolean    m_bSimPlayFailed;
    byte       m_keyBeepVol;
    boolean    m_bInterrupt;
    Mp3Player_Status m_ePreMp3Status;
    PromptMsg_e_Type m_eMsgType; 
    IBacklight *m_pBackLight;
    /*���ڵ�APP SUSPENDʱ�������� ������*/
    uint16     m_nPlayMode;
    uint16     m_nVolume;
    uint16     m_nMusicSort;
    boolean    m_aIdentifier[MUSIC_MAX_FILESNUM];
    //�������ʱ������¼����Ĵ���
    uint8      m_nRandNum;
    AEERect    *m_pMP3FaceRect;
    uint16     m_nSkinImageId;
#ifdef FEATURE_SUPPORT_BT_APP
    //play music by BT or not
    boolean   m_bUseBT;
    // play music by 848 chip or qsc
    boolean   m_bUse848;
    boolean   m_bInterruptByBT;
    // bt ID
    uint8     m_nBTID;
#endif //FEATURE_SUPPORT_BT_APP
    MP3Recttype m_rtype;////wlh 20090415 mod Ϊ�����𲥷����򣬼���������������ˢ�£����˸�����
} CMusicPlayer;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            MP3State tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }
// �رնԻ����
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                               }


/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
       CMusicPlayer_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��MusicPlayer Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       SUCCESS: �����Ի���ɹ�
       ��SUCCESS: �����Ի���ʧ��

��ע:
       

==============================================================================*/
int CMusicPlayer_ShowDialog(CMusicPlayer *pMe, uint16  dlgResId);

int CMusicPlayer_ShowMsgBox(CMusicPlayer *pMe, uint16 wTextResId);

int  CMusicPlayer_ShowYesNoDialog( CMusicPlayer *pMe, 
                               uint16      msgResId ,
                               AECHAR     *msg, 
                               int         msglen );

/*==============================================================================
����:
       MusicPlayer_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��MusicPlayer Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean CMusicPlayer_RouteDialogEvent(CMusicPlayer *pMe,
                                      AEEEvent      eCode,
                                      uint16        wParam,
                                      uint32        dwParam
                                     );

/*----------------------״̬����غ�������---------------------*/
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
NextFSMAction CMusicPlayer_ProcessState(CMusicPlayer *pMe);

/*==============================================================================
������ CMusicPlayer_ReadMusiclist

˵����
      ������ȡָ�������б������������������

������
      pMe:CMusicPlayer *����ָ��
      curfileName:������·���ͺ�׺���ļ���
      isPlaying:TRUE:�Ѵ��ļ��ж�ȡ�����ݷŵ�pMe->m_PlayingMusiclist��
                FALSE:�Ѵ��ļ��ж�ȡ�����ݴ�ŵ�pMe->m_Musiclist��

����ֵ��
    SUCCESS:��ȡ�ɹ�
    EFAILED:��ȡʧ��

��ע��:
        ��
        
==============================================================================*/
int CMusicPlayer_ReadMusiclist(CMusicPlayer *pMe , char *curFileName,boolean isPlaying);

/*==============================================================================
������ CMusicPlayer_WriteMusiclist

˵����
       ����д��ָ�������б������������������

������
      pMe:CMusicPlayer *����ָ��
      curfileName:������·���ͺ�׺���ļ���

����ֵ��
    SUCCESS:��ȡ�ɹ�
    EFAILED:��ȡʧ��

��ע��:
        ��
        
==============================================================================*/
int CMusicPlayer_WriteMusiclist(CMusicPlayer *pMe , char *curFileName);
/*==============================================================================
������MP_ReleaseMedia

˵����
       ֹͣ���ͷ�Mediaָ��

������
       pMe [in]��ָ��MusicPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
        ��

��ע��:

==============================================================================*/
void CMusicPlayer_ReleaseMedia(CMusicPlayer *pMe);

//���Ի�����
boolean CMusicPlayer_InitMusic(CMusicPlayer *pMe);

/*===========================================================================
  ����ĳ������
===========================================================================*/      
void CMusicPlayer_PlayMusic(CMusicPlayer *pMe);

/*===========================================================================
  �ָ�����ĳ������
===========================================================================*/      
void CMusicPlayer_ResumeMusic(CMusicPlayer *pMe);

void CMusicPlayer_SeekMusic(CMusicPlayer *pMe);
void CMusicPlayer_PauseMusic(CMusicPlayer *pMe);

/*����ʱ�䣬���ƣ���С�����ڲ��ŵ��б���������ֽ�������*/
void CMusicPlayer_PlayingMusiclistSortBy(CMusicPlayer * pMe);

/*����ʱ�䣬���ƣ���С�Ե�ǰVIEW���б���������ֽ�������*/
void CMusicPlayer_MusiclistSortBy(CMusicPlayer * pMe);

/*�������ڲ��ŵĸ��������ҵ����ڲ����б���������ID��*/
void MP3_SearchIdByName(CMusicPlayer *pMe,char *musicName);

/*������ʾ��ǰ���ڲ��ŵĸ�������*/
void MP3_MusicNameAutoScroll(CMusicPlayer *pMe);

void MP3_DrawWaveAni(CMusicPlayer *pMe);

void MP3_StopAni(CMusicPlayer *pMe);

/*===========================================================================
   ����CMX�ײ�ص�
===========================================================================*/                       
void CMusicPlayer_MediaNotify(void * pUser, AEEMediaCmdNotify * pCmdNotify);

/*===========================================================================
   ������һ���ļ�, 
   bDirectionΪTRUE, ��һ���ļ�
   bDirectionΪFALSE, ǰһ���ļ�
===========================================================================*/  
void CMusicPlayer_PlayNext(CMusicPlayer *pMe, boolean bDirection);

void SetMp3PlayerStatus(CMusicPlayer *pMe,Mp3Player_Status eSt);
void MP3_InitMusicCB(CMusicPlayer *pMe);
AEERect *GetPlayerSkinRect(CPlayerSkin ePlayerSkin);
//��������������ŵ���ز���
void MP3_ResetRandonIdentifier(CMusicPlayer *pMe);
#ifdef FEATURE_SUPPORT_BT_APP
void MP3_PlayMusicByBTCallBack(void *pCxt, int nErr);
void SetIsPlayMP3ByBt(boolean bPlayMp3ByBt);
#endif

#endif             
//MUSICPLAYER_PRIV_H

