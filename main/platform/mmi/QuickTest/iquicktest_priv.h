#ifndef IQUICKTESTPRIV_H
#define IQUICKTESTPRIV_H

/*==============================================================================
// �ļ���
//        iquciktest_priv.h
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
07-11-13               ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "OEMFeatures.h"
#include "AEE.h"              // Standard AEE Declarations
#include "AEEModGen.h"
#include "AEEDate.h"
#include "AEEDB.h"            // Standard AEE Declarations
#include "AEEMenu.h"

#include "AEENet.h"           // AEENet services
#include "AEEAlert.h"
#include "AEEStdLib.h"        // AEE StdLib Services
#include "AEEText.h"          // AEE StdLib Services
#include "AEEFile.h"          // AEEFile Services
#include "AEEConfig.h"
#include "AEEBacklight.h"
#include "OEMCFGI.h"
#include "AEERUIM.h"
#include "quicktest.brh"
#if defined(BREW_STATIC_APP)
  #include "OEMClassIds.h"
#else
  #include "quicktest.bid"
#endif

#include "IQuickTest.h"
#ifndef WIN32
#include "Hs.h"
#include "voc.h"
#include "uim.h"
#include "Qdspext.h"
#endif
#include "AEEAnnunciator.h"
#include "AppComFunc.h"
#include "Appscommon.h"
//#include "AEECallList.h"
#include "AEECallHistory.h"
#include "appscommonimages.brh"
#include "OEMNV.h"

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
#define  AEE_APPSCOMMONRES_IMAGESFILE "fs:/sys/appscommonimages.bar"
#if defined(FEATURE_DISP_160X128)
#define SECONDE_LINE  50
#define THRID_LINE    66
#define FOUR_LINE     82
#define FIVE_LINE     98
#define LEFT_START    44
#define RITHT_START   94
#define RTIHT_END     116
#define DOWN_Y        34
#define ENDCALL_START 136
#define ENDCALL_END   140

#elif defined(FEATURE_DISP_220X176)
#define SECONDE_LINE  68
#define THRID_LINE    90
#define FOUR_LINE     112
#define FIVE_LINE     134
#define LEFT_START    60
#define RITHT_START   128
#define RTIHT_END     164
#define DOWN_Y        44
#define ENDCALL_START 192
#define ENDCALL_END   198
#else
#define SECONDE_LINE  68
#define THRID_LINE    90
#define FOUR_LINE     112
#define FIVE_LINE     134
#define LEFT_START    60
#define RITHT_START   128
#define RTIHT_END     164
#define DOWN_Y        44
#define ENDCALL_START 192
#define ENDCALL_END   198
#endif
#define  AEE_QUICKTEST_RES_FILE (AEE_RES_LANGDIR QUICKTEST_RES_FILE)
typedef void (* qdsp_cmd_isr_func_type) (void);

/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   // ��ʼֵ��������Ҫ�����Ի���
   DLGRET_CREATE,

   // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_OK,
   DLGRET_CANCELED,

   // ���˵��Ի���
   DLGRET_KEYTEST,            //��������
   DLGRET_TOUCHSCREENTEST,    //����������
   DLGRET_YAMAHATEST,         //YAMAHA����
   DLGRET_VIBRATETEST,        //�񶯲���
   DLGRET_BACKLIGHTTEST,      //�������
   DLGRET_FLIPTEST,           //���ǲ���
   DLGRET_FMTEST,             //FM����
   DLGRET_CAMERATEST,               //camera ����
   DLGRET_SDTEST,             //SD����
   DLGRET_VERTEST,            //VER����
   DLGRET_LCDTEST,            //LCD����
   DLGRET_CALLTEST,           //mic����
   DLGRET_RESTOREFACTORY      //�ָ���������
} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// QuickTest Applet ״̬��״̬��
typedef enum _QuickTestState
{
   QUICKTESTST_NONE,
   QUICKTESTST_INIT,
   QUICKTESTST_MAIN,
   QUICKTESTST_KEYTEST,
   QUICKTESTST_TOUCHSCREENTEST,
   QUICKTESTST_YAMAHATEST,
   QUICKTESTST_VIBRATETEST,
   QUICKTESTST_BACKLIGHTTEST,
   QUICKTESTST_FLIPTEST,
   QUICKTESTST_FMTEST,
   QUICKTESTST_CAMERATEST,
   QUICKTESTST_SDTEST,
   QUICKTESTST_VERTEST,
   QUICKTESTST_LCDTEST,
   QUICKTESTST_CALLTEST,
   QUICKTESTST_RESTOREFACTORY,
   QUICKTESTST_EXIT
} QuickTestState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;
#ifdef FEATURE_SUPPORT_BT_APP
typedef enum _dilag_type
{
   LCD_TEST,
   BT_TEST
} dilag_type;
#endif
// QuickTest Applet����ṹ�壺
typedef struct _CQuickTest {
   DECLARE_VTBL(IQuickTest)

   uint32         m_nRefs;
   IModule        *m_pModule;
   IDisplay       *m_pDisplay;     // Display interface
   IShell         *m_pShell;       // Shell interface
   

   ISound         *m_pISound;
   IRUIM          *m_pIRUIM;
   IALERT         *m_pIAlert;
   uint16         colorMask;
   IRingerMgr     *m_pRingerMgr;

   AEERect        m_rc;

   // ��ǰ��Ի���IDialog�ӿ�ָ��
   IDialog        *m_pActiveDlg;

   // ���˵�ѡ����
   uint16         m_mainMenuSel;

   // ��ǰ��Ի���ID
   uint16         m_pActiveDlgID;

   // ��Ի���ر�ʱ�ķ��ؽ��
   DLGRet_Value_e_Type    m_eDlgRet;

   // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
   boolean        m_bNotOverwriteDlgRet;

   // Applet ǰһ״̬
   QuickTestState m_ePreState;

   // Applet ��ǰ״̬
   QuickTestState m_eCurState;

   // Applet�Ƿ��ڹ���״̬
   boolean        m_bSuspending;

   // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
   boolean        m_bAppIsReady;

   IConfig        *m_pConfig;        //IConfig ָ��

   IBacklight     *m_pIBacklight;    //�����������걳���������Ч��

   byte            m_nlightlevel;

   qdsp_cmd_isr_func_type m_qdspisr;
   IAnnunciator    *m_pIAnn;

#if defined( FEATURE_FM_RADIO)
    boolean m_fmClockwise;
    boolean m_fmIsPowerupBeforeFmTest;
    uint16  m_fmChannelBeforeFmTest;
#endif
    int     m_lineheight;
#ifdef FEATURE_SUPPORT_BT_APP
    dilag_type m_dilag_type;
#endif
#ifdef FEATURE_BREW_CAMERA 
    boolean m_isFormCamera;
#endif
    uint32  m_testkeycount;
} CQuickTest;
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// SysMenu_APPIsReadyTimer����
#define APPISREADY_TIMER       400                       
#define EVT_APPISREADY         (EVT_USER+10)
#define MAX_STRING_LENGTH      64

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            QuickTestState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
������QuickTest_UpdateActiveDialogInfo

˵����
       ����QuickTest Applet�����ڹؼ��ĶԻ��������

������
       pMe [in]��ָ��QuickTest Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam [in]����Ի�����ԴID��
       dwParam [in]:��ָ���Ի����IDialog�ӿ�ָ�롣

����ֵ��
        ��

��ע��:
       ���º��QuickTest Apple�Ի������ԡ�QuickTest_RouteDialogEvent������
       ������Щ���԰ѶԻ����¼�·������Ӧ�Ĵ�������
       �˺�������QuickTest_HandleEvent���á�

==============================================================================*/
void QuickTest_UpdateActiveDialogInfo
(
    CQuickTest *pMe,
    uint16  wParam,
    uint32 dwParam
);
/*==============================================================================
����:
       QuickTest_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��QuickTest Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void QuickTest_ShowDialog(CQuickTest *pMe,uint16  dlgResId);

/*==============================================================================
����:
       QuickTest_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��QuickTest Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean QuickTest_RouteDialogEvent(CQuickTest *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
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
NextFSMAction QuickTest_ProcessState(CQuickTest *pMe);

#endif//IQUICKTESTPRIV_H

