#ifndef IMIZONEPRIV_H
#define IMIZONEPRIV_H

/*==============================================================================
// �ļ���
//        MIZONE_priv.h
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
#include "mizone.brh"
#if defined(BREW_STATIC_APP)
  #include "OEMClassIds.h"
#else
  #include "MiZone.bid"
#endif

#include "mizone.h"
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
#define  AEE_MIZONE_RES_FILE (AEE_RES_LANGDIR MIZONE_RES_FILE)
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
   DLGRET_MIZONEACTIVATE,
   DLGRET_ENDACTIVATE,
   DLGRET_MIZONEASTROLOGY,
   DLGRET_MIZONESERVICES,
   DLGRET_MIZONEENDPULL,
   DLGRET_MIZONEINPUTSEND,
   DLGRET_MIZONESENDNAME,
} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// QuickTest Applet ״̬��״̬��
typedef enum _MiZoneState
{
   MIZONE_NONE,
   MIZONE_INIT,
   MIZONE_MAIN,
   MIZONE_MIZONEACTIVATE,
   MIZONE_ENDACTIVATE,
   MIZONE_ASTROLOGY,
   MIZONE_SERVICES,
   MIZONE_ENDPULL,
   MIZONE_INPUTSEND,
   MIZONE_SENDNAME,
   MIZONE_EXIT
} MiZoneState;

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
// MiZone Applet����ṹ�壺
typedef struct _CMiZone {
   DECLARE_VTBL(IMiZone)

   uint32         m_nRefs;
   IModule        *m_pModule;
   IDisplay       *m_pDisplay;     // Display interface
   IShell         *m_pShell;       // Shell interface
   IRUIM          *m_pIRUIM;

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
   MiZoneState     m_ePreState;

   // Applet ��ǰ״̬
   MiZoneState     m_eCurState;

   // Applet�Ƿ��ڹ���״̬
   boolean         m_bSuspending;

   // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
   boolean         m_bAppIsReady;

   IConfig        *m_pConfig;        //IConfig ָ��

   IBacklight     *m_pIBacklight;    //�����������걳���������Ч��

   byte            m_nlightlevel;

   qdsp_cmd_isr_func_type m_qdspisr;
   IAnnunciator    *m_pIAnn;
   IStatic  *      m_pStatic;
   uint16          m_pActiveitem;

#if defined( FEATURE_FM_RADIO)
    boolean m_fmClockwise;
    boolean m_fmIsPowerupBeforeFmTest;
    uint16  m_fmChannelBeforeFmTest;
#endif
    int     m_lineheight;
} CMiZone;
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
            MiZoneState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

#define MIZONE_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
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
void MiZone_UpdateActiveDialogInfo 
(
    CMiZone*pMe,
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
void MiZone_ShowDialog(CMiZone *pMe,uint16  dlgResId);

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
boolean MiZone_RouteDialogEvent(CMiZone *pMe,
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
NextFSMAction MiZone_ProcessState(CMiZone *pMe);

#endif//IMIZONEPRIV_H

