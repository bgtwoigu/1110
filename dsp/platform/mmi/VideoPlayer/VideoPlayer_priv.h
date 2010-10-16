#ifndef VIDEOPLAYERPRIV_H
#define VIDEOPLAYERPRIV_H
/*=================================================================================================================
// �ļ���
//        VideoPlayer_priv.h
//        2008-10-06 �����Ȳ���汾(Draft Version)
//        ��Ȩ����(c) 2008 Anylook
//        
// �ļ�˵����
//        
// ���ߣ� 2008-10-06
// �������ڣ�������
// ��ǰ�汾��Draft
=================================================================================================================*/

/*================================================================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
2008-10-06        ������         ��ʼ�汾
=================================================================================================================*/



/*=================================================================================================================
                                 ���ļ��������ⲿ�ļ�
=================================================================================================================*/
#ifndef AEE_SIMULATOR
#include "customer.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif

#if !defined(BREW_STATIC_APP)
// Use the .bid file if compiling as dynamic applet
   #include "VideoPlayer.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_VIDEOPLAYER
      #error AEECLSID_VIDEOPLAYER must be defined
   #endif
#endif
#include "videoplayer.brh"
#include "videoplayer_images.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
#include "VideoPlayer.h"
#include "MediaGallery.h"
#include "AEEMenu.h"
#include "AEEBacklight.h"
#include "OEMText.h"
#include "OEMCFGI.h"
#include "OEMSVC.h"
#ifndef WIN32
#include "ui.h"
#include "err.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32
#include "AEEMedia.h" 
#include "AEEMediaFormats.h"
#include "AEEImageCtl.h" 

#include "AEEMediaUtil.h"
#include "OEMFS.h"
#include "OEMNVINT.h"
#include "AEEConfig.h"    
#include "AEEBacklight.h" 
#include "Appscommon.h"

/*=================================================================================================================
                               �궨��ͳ���
=================================================================================================================*/
#if defined( AEE_SIMULATOR)
    #define  AEE_APPSVIDEOPLAYER_RES_FILE "fs:/mod/videoplayer/en/videoplayer.bar"
#else
#define  AEE_APPSVIDEOPLAYER_RES_FILE (AEE_RES_LANGDIR VIDEOPLAYER_RES_FILE)
#endif 

#define VIDEOPLAYER_IMAGES_RES_FILE "videoplayer_images.bar"

// �ļ����ڿɷ��õ�����ļ���
#define MAX_NUMBER               1000

//���ٰ�����ʱ����
#define APPISREADY_TIMER          100
//���ٰ����¼�
#define EVT_APPISREADY   (EVT_USER+30) 

// �Ի����Զ��رյ�ʱ��
#define DIALOGTIMEOUT            3000

// ���ûص���ʱ��
#define SETTIME_LONG             3000
#define SETTIME_MIDDLE          1000
#define SETTIME_SHORT            300

// ������ַ���������󳤶�
#define  MAX_STR_LEN             64

//ͼƬ��������ʾ������������ֵ
#define  MAX_NAME_LEN            167

// ��GALLERY����VIDEOPLAYER�ı�־
#define STARTARGS_GALLERY       ((char)'G')

// ����ÿ����Ĳ�
#define VP_VOLUME_STEP           20

// һ����������
#define NUM_OF_VOLBAR_ITEM       6

//���յ�����Ϣ����֮ǰ֪ͨvideo app ,��AEEEvent.h�ﶨ��
// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {                  \
            VideoPlayerState tpState;               \
            tpState = nextState;                    \
            pMe->m_ePreState = pMe->m_eCurState;    \
            pMe->m_eCurState = tpState;             \
                                 }

//������ײ���ʾ���궨��
#define DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}
// �رնԻ���
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                              }


//wlh 20090420 add icon x/y
//��ʼ��ͣ
#define VIDEOPLAYER_PLAY_X 34
#define VIDEOPLAYER_PLAY_Y 182
#define VIDEOPLAYER_PLAY_W 16
#define VIDEOPLAYER_PLAY_H 16

//��ʼ PART
#define VIDEOPLAYER_PLAY_PART_X 16
#define VIDEOPLAYER_PLAY_PART_Y 182
#define VIDEOPLAYER_PLAY_PART_W 52
#define VIDEOPLAYER_PLAY_PART_H 16
//��ͣ PART
#define VIDEOPLAYER_PAUSE_PART_X 16
#define VIDEOPLAYER_PAUSE_PART_Y 182
#define VIDEOPLAYER_PAUSE_PART_W 52
#define VIDEOPLAYER_PAUSE_PART_H 16
//FR PART
#define VIDEOPLAYER_FR_PART_X 0
#define VIDEOPLAYER_FR_PART_Y 168
#define VIDEOPLAYER_FR_PART_W 176
#define VIDEOPLAYER_FR_PART_H 13

//���
#define VIDEOPLAYER_FORWARD_X 159
#define VIDEOPLAYER_FORWARD_Y 168
#define VIDEOPLAYER_FORWARD_W 17
#define VIDEOPLAYER_FORWARD_H 13

//����
#define VIDEOPLAYER_REWIND_X 0
#define VIDEOPLAYER_REWIND_Y 168
#define VIDEOPLAYER_REWIND_W 17
#define VIDEOPLAYER_REWIND_H 13

//ȫ��
#define VIDEOPLAYER_FULLSCREEN_X 2
#define VIDEOPLAYER_FULLSCREEN_Y 184
#define VIDEOPLAYER_FULLSCREEN_W 11
#define VIDEOPLAYER_FULLSCREEN_H 10

//���ȵ�
#define VIDEOPLAYER_GLIDER_X 19
#define VIDEOPLAYER_GLIDER_Y 169
#define VIDEOPLAYER_GLIDER_W 9
#define VIDEOPLAYER_GLIDER_H 9

//��һ��
#define VIDEOPLAYER_PREVIOUS_X 16
#define VIDEOPLAYER_PREVIOUS_Y 184
#define VIDEOPLAYER_PREVIOUS_W 16
#define VIDEOPLAYER_PREVIOUS_H 13

//��һ��
#define VIDEOPLAYER_NEXT_X 52
#define VIDEOPLAYER_NEXT_Y 184
#define VIDEOPLAYER_NEXT_W 16
#define VIDEOPLAYER_NEXT_H 13

//������
#define VIDEOPLAYER_SCHEDULE_X 19
#define VIDEOPLAYER_SCHEDULE_Y 169
#define VIDEOPLAYER_SCHEDULE_W 138
#define VIDEOPLAYER_SCHEDULE_H 10

//ʱ��
#define VIDEOPLAYER_TIME_X 69
#define VIDEOPLAYER_TIME_Y 181
#define VIDEOPLAYER_TIME_W 47
#define VIDEOPLAYER_TIME_H 16

//����
#define VIDEOPLAYER_VOLUME_X 129
#define VIDEOPLAYER_VOLUME_Y 182
#define VIDEOPLAYER_VOLUME_W 33
#define VIDEOPLAYER_VOLUME_H 14

//��������
#define VIDEOPLAYER_VOLUME_ADD_X 162
#define VIDEOPLAYER_VOLUME_ADD_Y 182
#define VIDEOPLAYER_VOLUME_ADD_W 12
#define VIDEOPLAYER_VOLUME_ADD_H 16

//��������
#define VIDEOPLAYER_VOLUME_LOW_X 117
#define VIDEOPLAYER_VOLUME_LOW_Y 182
#define VIDEOPLAYER_VOLUME_LOW_W 12
#define VIDEOPLAYER_VOLUME_LOW_H 16

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define VIDEOPLAYER_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif//FEATURE_LCD_TOUCH_ENABLE

typedef enum VPlayerRecttype
{
	TYPE_PLAYER,
	TYPE_DECVOLUME,
	TYPE_ADDVOLUME,
	TYPE_PREVIOUS,
	TYPE_NEXT,
	TYPE_KUAIJIN,
	TYPE_KUAITUI,
}VPlayerRecttype;
/*=================================================================================================================
                             ���Ͷ���
=================================================================================================================*/

/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   
    DLGRET_CREATE,              //��ʼֵ��������Ҫ�����Ի��� 
    DLGRET_VIDEOPLAYER,         //IDD_VIDEOPLAYER
    //DLGRET_OPTION,              //����ѡ��,IDD_OPTION
    //DLGRET_DELETE,              //�Ƿ�ɾ������,IDD_DELETE   
    DLGRET_MSGBOX,              //��Ϣ������IDD_MSGBOX
    DLGRET_OK,                  //�Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
    DLGRET_CANCELED
}DLGRetValue;

/*----------------------״̬�����������������---------------------*/
//VideoPlayerState Applet ״̬��״̬��
typedef enum _VideoPlayerState
{
    STATE_NONE = 0,             //No state
    STATE_INIT,                 //Initial state. 
    STATE_PLAYER,               //IDD_VIDEOPLAYER
    //STATE_OPTION,               //IDD_OPTION
    //STATE_DELETE,
    STATE_MSGBOX,
    STATE_EXIT                  //State where we close the applet.
}VideoPlayerState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
    NFSMACTION_WAIT,            //ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����  
    NFSMACTION_CONTINUE         //ָʾ������״̬��״̬������ֹͣ��       
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct VideoPlayerMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} VideoPlayerMod;

//�����ȼ�
typedef enum CVolumeLevel 
{  
    VOLUME_OFF,
    VOLUME_ONE,
    VOLUME_TWO,
    VOLUME_THREE,
    VOLUME_FOUR,
    VOLUME_FIVE
} CVolumeLevel;
// VideoPlayer Applet����ṹ�壺
typedef struct _CVideoPlayer
{
    DECLARE_VTBL(IVideoPlayer)
    uint32           m_nRefs;     // �������ü�����
    IModule          *m_pModule;
    IDisplay         *m_pDisplay;
    IShell           *m_pShell;
    IConfig          *m_pConfig;    
    IMenuCtl         *m_pMenuCtl; //IMenuCtlָ��
    IMedia           *m_pMedia;//IMediaָ��
    IImage           *m_pImage;
    IFileMgr         *m_pIFileMgr;    
    IDatabase        *m_pFileDB;
    
    AEERect          m_rc;    
 
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog          *m_pActiveDlg;

    // ��ǰ��Ի���ID
    uint16           m_pActiveDlgID;

    // ��ǰѡ��˵�ID
    uint16           m_nItemID;

    //�ַ�����ԴID
    uint16           m_nMsgResID;

    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRetValue      m_eDlgRet;

    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean          m_bNotOverwriteDlgRet;

    // Applet ǰһ״̬
    VideoPlayerState m_ePreState;

    // Applet ��ǰ״̬
    VideoPlayerState m_eCurState;

    // Applet�Ƿ��ڹ���״̬
    boolean          m_bSuspending;
    boolean          m_bEarUp;
    
    // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean          m_bAppIsReady;   

    //media data
    AEEMediaData     m_md;

    //��ʾ����Ļ�ϵ��ļ���
    AECHAR           m_playname[MAX_STR_LEN];
    //ʹ����m_playname[m_np]��
    int              m_np;
    //video�б��еļ�¼�� 
    uint32           m_RecordCount;

    //��ǰ���ŵ��ļ���,����·��
    char             *m_FileToPlay;
       
    //�û�ѡ�����ĸ��˵�
    uint16           m_MenuSelected;

    //�Ƿ��GALLERY����벥����
    boolean          IsGallery;
    
    //�Ƿ���ͣ
    boolean          IsPause;

    //�Ƿ��ڲ���״̬
    boolean          IsPlay;
    
    //�Ƿ�ȫ��
    boolean          IsFullScreen;

    //��ʼ�������ļ��ķ�����Ϣ
    int16           m_InitFailed;
    int16           m_PlayFailed;
    /*�Ƿ���Բ�����һ��(ֻ�д�videoplayer�����ɲ�����һ�ף�
     �˱�������Ϊ��gallery�������videoplayer����������־*/
    boolean         m_IsPlaynext;
              
    //�ص��������ж��Ƿ����û�ֹͣ���ţ���ֹ�û�����ֹͣ���ص�����
    //����������һ���ļ�
    boolean          UserStop;
    // 848�Ƿ��ڷ�æ״̬
    boolean          Is848Busy;
      
    //������ʱ��
    uint32           bTotalTime;
    
    //������ʱ  ��ʱ��
    uint32           bCurrentTime;
   
    //��ͣʱ��ʱ��
    uint32           m_bPauseTime;

    //�ܵ�����
    uint16           totalvolume;
    
    //�����ȼ�
    CVolumeLevel     m_bVolumeLevel;
    
    //��ĳЩ��������²�������ͣ
    boolean          PauseLock;
           
    //����
    IBacklight       *Ibacklight;
    //����timer
    byte             bktimer;
    //��������
    byte             m_KeySound;
    byte             m_CKSound;
    //������ʾ��ʽ
    byte             m_SMSRinger;
    
    //APP ������ʽ
    //AppletStartMethod  m_eStartMethod;
    //Applet�Ƿ��ڻ״̬
    boolean          m_bActive;    
	VPlayerRecttype m_rtype;////wlh 20090420 add Ϊ�����𲥷����򣬼���������������ˢ�£����˸�����
}CVideoPlayer;

/*=================================================================================================================
                                 ȫ������
=================================================================================================================*/

/*=================================================================================================================
                                 ���أ���̬������
=================================================================================================================*/

/*=================================================================================================================
                                 ��������
=================================================================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
/*=================================================================================================================
����:
       VideoPlayer_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��VideoPlayer Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


=================================================================================================================*/

int VideoPlayer_ShowDialog(CVideoPlayer *pMe,uint16  dlgResId);

/*=================================================================================================================
����:
    VideoPlayer_ShowMsgBox

˵��:
    ������״̬����������������Ϣ�Ի�����ʾ��Ϣ��ʾ�û���

����:
    pMe [in]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wTextResId [in]: ��Ϣ�ı���Դ ID��

����ֵ:
    none

��ע:
=================================================================================================================*/

int VideoPlayer_ShowMsgBox(CVideoPlayer *pMe, uint16 wTextResId);
                    
/*=================================================================================================================
����:
       VideoPlayer_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��VideoPlayer Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

=================================================================================================================*/
boolean VideoPlayer_RouteDialogEvent(CVideoPlayer *pMe,
                                     AEEEvent    eCode,
                                     uint16  wParam,
                                     uint32 dwParam);


/*----------------------״̬����غ�������---------------------*/
/*=================================================================================================================
����:
       VideoPlayer_ProcessState
˵��:
       VideoPlayer Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��VideoPlayer Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

=================================================================================================================*/
NextFSMAction VideoPlayer_ProcessState(CVideoPlayer *pMe);
/*=================================================================================================================
������VideoPlayer_ReleaseVideo

˵����
       ֹͣ���ͷ�Video

������
       pMe [in]��ָ��VideoPlayer Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
        ��

��ע��

=================================================================================================================*/
void VideoPlayer_ReleaseVideo(CVideoPlayer *pMe);

/*===================================================================================================================
    ��Ƶ���ŵ�ģʽ
 ===================================================================================================================*/
boolean VideoPlayer_PlayMod(CVideoPlayer *pMe, uint16 wParam);

/*=================================================================================================================
��ʼ��video
=================================================================================================================*/
void VideoPlayer_InitVideo(CVideoPlayer * pMe);

//��ֹ���ٰ����¼�
void VideoPlayer_APPIsReadyTimer(void *pMe);
extern boolean videoplayer_play_flag;
#endif
//VIDEOPLAYER_H
