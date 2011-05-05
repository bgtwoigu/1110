#ifndef UTKPRIV_H
#define UTKPRIV_H

/*==============================================================================
// �ļ���
//        UTK_priv.h
//        2007-11-13 ��ϲ�����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2007-11-13
// �������ڣ���ϲ��
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-13         ��ϲ��         ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#ifndef AEE_SIMULATOR
#include "OEMFeatures.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif
#include "OEMClassIDs.h"
#ifndef AEECLSID_APP_UTK
      #error AEECLSID_APP_UTK must be defined
#endif
#include "OEMFeatures.h"
#include "AEE.h"
#include "utk.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
//#include "OEMMacros.h"
#include "AEEText.h"
#include "AEEMenu.h"
#include "AppComFunc.h"
#include "UTK.h"
#include "uimtk.h"
//#include "OEMEvents.h"
#include "UTKUtils.h"
#ifdef CUST_EDITION
#include "custuim.h"
#else
#include "custruim.h"
#endif
//#include "uiutils.h"
#include "AEE_OEM.h"
#include "AEEConfig.h"
//#include "AEEAlert.h"         
#include "AEEPhone.h"
#include "Cai.h"
#include "AEEAlert.h"
#include "Wmsi.h"
#include "Wms.h"
#include "Wmstscdma.h"
#include "AEEWMS.h"
#include "mccdma.h"  
#include "ui.h"

#include "Appscommon.h"
#include "appscommonimages.brh"
#include "OEMCFGI.h"
#include "CallApp.h"
#ifdef FEATURE_ICM
#include "AEECM.h"
#else
#include "AEETelephone.h"
#include "AEETelDef.h"
#endif
#include "OEMSound.h"
#include "OEMConfig.h"
#include "AEEAnnunciator.h"


/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
#define MAX_STR_ACHARS      256 
#define OEM_IME_DIALOG   (0x7FFF)
#define TP_STARKEYSWITCH TP_STARKEY_SWITCH
#define TP_HIDEMODETITLE 0x00000000 //nothing match,0x00008000
#define SetUniMenuConfig(p)         //do nothing SetUniMenuConfig(p)
#define SetUniTextConfig(pT)        //do nothing SetUniTextConfig(pT)
#define HEIGHT_PROMPT_BAR               (0)

typedef struct
{
    wms_cmd_id_e_type           cmd;
    wms_cmd_err_e_type          cmd_err;
} wmsutk_cmd_status_type;

/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   // ��ʼֵ��������Ҫ�����Ի���
   DLGRET_CREATE,
   DLGRET_MSGBOX_OK,
   // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_OK,
   DLGRET_POPMSG,
   DLGRET_CANCELED
} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// UTK Applet ״̬��״̬��
typedef enum _UTKState
{
   UTKST_NONE,
   UTKST_INIT,
#ifdef FEATURE_CARRIER_CHINA_TELCOM   
   UTKST_SPLASH,
#endif //FEATURE_CARRIER_CHINA_TELCOM   
   UTKST_MAIN,
   UTKST_LIST,
   UTKST_INPUT,
   UTKST_DISPLAY,
   UTKST_SENDMSG,
   UTKST_MSG,  
   UTKST_POPMSG,
   UTKST_PLAYTONE,    
   UTKST_EXIT
} UTKState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct UTKMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} UTKMod;

// UTK Applet����ṹ�壺
typedef struct _CUTK
{
    DECLARE_VTBL(IUTK)
    uint32          m_nRefs;          // �������ü�����
    IModule        *m_pModule;
    IDisplay        *m_pDisplay;
    IShell            *m_pShell;

    IALERT    *m_pAlert;

    IConfig        *m_pConfig;

#ifdef FEATURE_ICM
    ICM            *m_pCM;
#else
    ITelephone            *m_pITelephone;
	IPhoneCtl             *m_pIPhoneCtl;
#endif

#ifdef FEATURE_UIM_TOOLKIT_UTK
    uim_tk_proactive_cmd_enum_type   cmd_type;
#endif
    AECHAR            m_wszTitle[MAX_TITLE_CHARS];

    boolean          m_bUserMode;
    int          m_nInputMode;

    AEERect       m_rc;
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog             *m_pActiveDlg;
    // ��ǰ��Ի���ID
    uint16 m_pActiveDlgID;
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRet_Value_e_Type   m_eDlgRet;
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean m_bNotOverwriteDlgRet;
    // Applet ǰһ״̬
    UTKState m_ePreState;
    // Applet ��ǰ״̬
    UTKState m_eCurState;
    // Applet�Ƿ��ڹ���״̬
    boolean m_bSuspending;    
    
    boolean  m_bAppIsReady;

    RGBVAL  m_dwOldBkColor;
    //�������ı�����ı��ݣ���ֹ���𷵻غ�ԭ����������ֶ�ʧ
    AECHAR  bInputeBackup[MAX_STR_ACHARS + 2];

    //�ı�������Ƿ���Ҫ���ݿ���
    boolean  InputeBackupNeeded;

    uint32   select_item[20];
    uint32   level;
    boolean  clr_back;
    uint16              m_wSelectStore; 
    
    uint32  m_dwTimeOut;
    
    // �����ж��Ƿ������˳� UTK
    byte                m_btCursel;

    AECHAR  *m_wszInputTitle;
    // Applet ����״̬
    UTKState   m_eMsgRetState;
    uint32     m_MsgTimeout;
    uint16     m_wMsgID;        // message resource ID
    IPhone     *m_pIPhone;      // Phone interface   
    AECHAR     *m_wszInputText;
    int        m_TextMaxLen;
    int        m_TextMinLen;    
    byte       m_TextQS;  // Command Qualifier  
    uint16     m_PlaytoneMsgID; 
    byte       m_PlaytoneToneID;
    
    IWMS       *m_pwms;  
    wms_client_id_type m_clientId;  
    AEECallback m_callback;  // ����ص�
    wms_report_status_e_type        m_SendStatus; //������Ϣ���ص�״̬��
    wms_cdma_tl_status_e_type       m_SendtlStatus;
    RGBVAL     m_themeColor;    
    IAnnunciator *m_pIAnn;
} CUTK;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
#define UTK_RES_FILE_LANG      (AEE_RES_LANGDIR UTK_RES_FILE)

#define UTK_DATA_OFF                    2
#define MAX_MENU_ITEM                   32

#define MAX_STR_CHARS                   256

#define ALPHA_8BITS                     0x04
//#define ALPHA_7BITS                       0x00
#define ALPHA_UNICODE                   0x08
#define UNICODE_FLAG                               0x80

// �ղ˵�ѡ���
#define MENU_SELECT_NULL                (0xFFFF)
#define TIMEOUT_MS_MSGBOX               (3000)
#define TIMEOUT_MS_QUICK                (300)

// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// UTK_APPIsReadyTimer����
#define APPISREADY_TIMER       200           
#define EVT_APPISREADY   (EVT_USER+1)         

#define EVT_DIALOGTIMEOUT (EVT_USER+3)

#define EVT_OUTAPPDISPLAYDONE (EVT_USER+4)

// ���Ͷ���Ϣʱ����״̬�ص������������¼�����֪ͨ������Ϣ�Ľ��
#define EVT_SEND_STATUS (EVT_USER+12)

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            UTKState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

// �رնԻ����
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                                 }
// ������ײ���ʾ���궨��
#define DRAW_BOTTOMBAR(x)                           \
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
����:
       UTK_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��UTK Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:
       

==============================================================================*/
void UTK_ShowDialog(CUTK *pMe,uint16  dlgResId);

/*==============================================================================
����:
       UTK_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��UTK Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean UTK_RouteDialogEvent(CUTK *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       UTK_ProcessState
˵��:
       UTK Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��UTK Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction UTK_ProcessState(CUTK *pMe);

/*==============================================================================
����: 
       ByteToUnicode
       
˵��: 
       ���������ֽ�����ת��Ϊ Unicode ���ݡ�
       
����: 
       pUnicodeBuf [in]��IMenuCtl �ӿ�ָ�롣
       pBytBuf [out]�� �˵�����
       cmd_type�� uim_tk_proactive_cmd_enum_type
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
void ByteToUnicode(AECHAR *pUnicodeBuf, 
    byte *pBytBuf, 
    byte bytLen, 
    int mode
);

/*==============================================================================
������
       UTK_MsgSendStatusCallBack
˵����
       ����BREW�Ļص���������BREW��������֪ͨApplet����Ϣ����״̬��

������
       pUser {in]��void����ָ��(�˴�ʹ��CUTK �ṹָ��)
       status [in]����Ϣ���ͷ��صĽ���롣
       
����ֵ��
       �ޡ�

��ע��:
       �Ա�BREW���ߵ����ȼ���CM�������С����Է�����ȷ������Ϣ������û���

==============================================================================*/
void UTK_MsgSendStatusCallBack(void * pUser, int status);

/*==============================================================================
������
       UTK_MessageAlert

˵����
       �������ŷ�����Ϣ�ɹ���ʾ����

������
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       bPlay [in]: ����ָʾ��TRUE: ���ţ� ����ֹͣ���š�
       
����ֵ��
       �ޡ�

��ע��:
       
==============================================================================*/
void UTK_MessageAlert(CUTK * pMe, boolean  bPlay); 

/*==============================================================================
����: 
       UTK_APPIsReadyTimer
       
˵��: 
       ��ʱ���ص���������Ҫ���ڿ����û������ٶȡ�
       
����: 
       pme [in]��void *����ָ�룬��ָCUTK�ṹָ�롣
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
void UTK_APPIsReadyTimer(void *pme);

/*==============================================================================
����: 
    UTK_GiveResponse
       
˵��: 
    ������������Ӧ�Ե�ǰ proactive command �Ĵ�������
       
����: 
    pMe [In]: ָ��UTK Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ��
    eCmd [in]: uim_tk_proactive_cmd_enum_type
    bForwad[in]: ָʾ�Ƿ���Ҫ��������һ������
    eResult [in]: �Ե�ǰ proactive command �Ĵ�����
       
����ֵ:
    none
       
��ע:
    ע�⣺��ǰ proactive command ���� CUTK ��Ա����cmd_type��eResult ����һ����
    �������Ҫ�Դ���������Ӧ������ CUTK ��Ա���� ��ʽ����

==============================================================================*/
void UTK_GiveResponse(CUTK * pMe, 
                      uim_tk_proactive_cmd_enum_type eCmd,
                      boolean  bForwad, 
                      uim_tk_general_result_type eResult
                      );

void UTK_GiveResponseEx(CUTK * pMe, 
                        uim_tk_proactive_cmd_enum_type eCmd,
                        boolean  bForwad, 
                        uim_tk_general_result_type eResult,
                        byte addinfo
                        );

/*==============================================================================
����: 
    CUTK_SetUTKMenu
       
˵��: 
    ������������� Setup Menu �� Select Item ����, ������Ӧ��������ֽ���������
    �����˵��б�
       
����: 
    pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pMenu [in]��IMenuCtl �ӿ�ָ�롣
    pwszTitle [out]�� �˵�����(�豣��ʱ����)
    cmd_type�� uim_tk_proactive_cmd_enum_type
       
����ֵ:
    �˵�����
       
��ע:
    ������ֻ���� Setup Menu �� Select Item ����Ĳ˵��ĳ�ʼ����
==============================================================================*/
int CUTK_SetUTKMenu(CUTK *pMe, IMenuCtl *pMenu, 
    AECHAR *pwszTitle, 
    uim_tk_proactive_cmd_enum_type cmd_type
);


/*==============================================================================
����: 
    UTK_ProcessSendCDMASMSCmd
       
˵��: 
    ���������� CDMA SEND SMS ���
       
����: 
    pMe [In]: ָ��UTK Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ��
    pCltMsg [in/out]: ָ�� wms_client_message_s_type �ṹ��ָ�룬���ڷ��ش���
                      ��Ϣ�� wms_client_message_s_type �ṹ��ַ��
    wszAlpha [in/out]: ���ڷ��� Alpha ��ʶ�ַ�����buffer
    nAlphabufLen [in]: ���� Alpha ��ʶ�ַ�����buffer �Ĵ�С����λΪ AECHAR                    
       
����ֵ:
    none
       
��ע:
    �緵��ֵΪ SUCCESS , ���� *pCltMsg ���� NULL , ��ʾ��Ϣ����Ҫ����(����Ҫ��)

==============================================================================*/
int UTK_ProcessSendCDMASMSCmd(CUTK *pMe, 
    wms_client_message_s_type ** pCltMsg,
    AECHAR *wszAlpha,
    int nAlphabufLen
);

/*==============================================================================
����: 
    UTKApp_LocalInformation
       
˵��: 
    ������������� Local Information ����, ������Ӧ��������ֽ���������
    �����˵��б�
       
����: 
    pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pMenu [in]��IMenuCtl �ӿ�ָ�롣
       
����ֵ:
    �˵�����
       
��ע:
    ������ֻ���� Setup Menu �� Select Item ����Ĳ˵��ĳ�ʼ����
==============================================================================*/
void UTKApp_LocalInformation(CUTK *pMe,Local_Information *pLocalInformation);

/*==============================================================================
����: 
    UTK_wmscommandreport
       
˵��: 
    wms �ô˺�����������߻㱨���������
       
����: 
    cmd [in]: wms �������͡�
    user_data[in]: ��������ʱ������û�����ָ�롣
    cmd_err [in]: ����״̬��������͡�
    
����ֵ:
    NULL �� ���������Ľڵ�ָ�롣
       
��ע:

==============================================================================*/
void UTK_wmscommandreport(wms_cmd_id_e_type   cmd,
                          void                *user_data,
                          wms_cmd_err_e_type  cmd_err);
                          
/*==============================================================================
����: 
    UTK_MsgCb
       
˵��: 
    ������Ϊ���� MSG ����¼��Ļص�������
       
����: 
    event [In]: CFG ����¼� ��
    pInfo [In]: wms_msg_event_info_s_type �ṹָ�룬�����¼������Ϣ��
    
       
����ֵ:
    none
       
��ע:
    Posts the Event to itself

==============================================================================*/
void UTK_MsgCb(wms_msg_event_e_type       event,
               wms_msg_event_info_s_type  *pInfo,
               boolean                    *shared);
               
/*==============================================================================
����:
    UTK_ProcessStatus

˵��:
    ������������ wms �ı�����Ϣ��

����:
    pMe [in]: ָ�� UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pRptInfo [in]: wms_submit_report_info_s_type �ṹָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
void UTK_ProcessStatus(CUTK *pMe, wms_submit_report_info_s_type *pRptInfo);

#endif
//UTKPRIV_H

