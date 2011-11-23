#ifndef WMSAPP_PRIV_H
#define WMSAPP_PRIV_H
/*==============================================================================
// �ļ���WMSApp_priv.h
//        2007-10-15��ʼ�汾(Init Version)
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        
// ���ߣ�
// �������ڣ�2007-10-15
// ��ǰ�汾��Init Version
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------   -----------------------------------------------
2007-10-15                    ��ʼ�汾
==============================================================================*/

/*==============================================================================
                             
                             ���ļ��������ⲿ�ļ�
                             
==============================================================================*/
#ifndef AEE_SIMULATOR
#include "customer.h"         //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"  // Simulator Feature Definitions
#endif

#if !defined(BREW_STATIC_APP)
#include "WMSAPP.BID"
#else
#include "OEMClassIds.h"      // Applet Ids
#ifndef AEECLSID_WMSAPP
#error AEECLSID_WMSAPP must be defined before including WMSApp.h
#endif
#endif
#include "WMSApp.h"
#include "AEEHeap.h"          // AEE Memory Allocation Services
#include "AEEMenu.h"          // AEE Menu Services
#include "AEEFile.h"          // AEE File Services
#include "AEEText.h"          // AEE Text Services
#include "AEEStdLib.h"        // AEE StdLib Services

#include "AEEAlert.h"         // IAlert Services
#include "AEEAppGen.h"        // AEEApplet Declaration
// callback structure definitions.
#include "AEEVector.h"
#include "AEEAnnunciator.h"
#include "AEEBacklight.h"

#include "wms.brh"          // Applet Resource Header

#include "AEEConfig.h"
#include "wms.h"
#include "AEEWMS.h"
#ifdef FEATURE_ICM
#include "AEECM.h"
#else
#include "AEETelephone.h"
#include "AEETelDef.h"
#endif
#include "OEMWMS_CacheInfo.h"
#include "OEMHeap.h"
#include "wmsi.h"

#include "Appscommon.h"
// ͼƬ��Դ�ļ�
#include "appscommonimages.brh"

#include "BREWVersion.h"
#include "oemcfgi.h"
#include "AEESound.h"
#include "AEE_OEM.h"
#include "AEERUIM.h"

#ifdef CUST_EDITION  /*add by miaoxiaoming*/
#include "OEMSVC.h"
#endif

#ifdef FEATURE_USES_MMS
#include "MediaGallery.h"
#include "WmsMms.h"
#include "AEEMedia.h"
#define MMSFILE_DIR      "fs:/user/mms"
#endif

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define WMSAPP_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif
#define NMSGLEN    153

#define SYS_FREEIF(p)                 if (p) { sys_free(p); (p) = NULL; }


/*==============================================================================
                                 
                                ����������Ͷ���
                                 
==============================================================================*/
typedef struct
{
    wms_cmd_id_e_type           cmd;
    wms_cmd_err_e_type          cmd_err;
} wmsapp_cmd_status_type;

typedef struct _CMultiSendItemInfo
{ 
    // ������Ϣ������
    AECHAR      m_szTo[MAX_PH_DIGITS+1];
    
    // ������Ϣ�������ڵ绰���ж�Ӧ������
    AECHAR      m_szName[MAX_TITLE_LEN+1]; 
} CMultiSendItemInfo;

// ����Ϣ��Ϣ���ݽṹ---�� WMS Applet �ڲ�ʹ��
typedef struct 
{
    AECHAR                      m_szCallBkNum[MAX_PH_DIGITS+1];     // Call Back Number
    AECHAR                      m_szNum[MAX_PH_DIGITS+1];           // ����/������Ϣ������
    AECHAR                      *m_szMessage;       // ��Ϣ��
    uint32                      m_dwTime;           // ��Ϣʱ���
    wms_priority_e_type         m_epriority;        // ��Ϣ���ȼ�
    wms_privacy_e_type          m_eprivacy;         // ��Ϣ�ܼ�
    wms_message_tag_e_type      msg_tag;            // ��Ϣ���
    wms_language_e_type         language;           // ����ָʾ
    boolean                     m_bDeliveryReport;  // �Ƿ���Ҫ��Ϣ��ִ,�����Ͷ���ʱ����
#ifdef FEATURE_SMS_UDH
    uint16                      msg_ref;            // ��Ϣ�ο���
    uint8                       total_sm;           // ��Ϣ���ݰ�����
    uint8                       seq_num;            // ��Ϣ���
#endif
    uint8                       m_NumMsgs;          // ��Ϣ��Ŀ(������֪ͨ����)
    wms_memory_store_e_type     mem_store;          // ��Ϣ�洢λ��
} WMSMessageStruct;

// ����Ϣ���ýṹ WMSApp configuration structure.
typedef struct WMSAPP_Config_Type
{
    // ���Ͷ��Ŵ洢ѡ��
    boolean                 m_bAutoSave;   
    
    // �յ�����ʱ�����趨�ռ����Ƿ��Զ�ɾ���Ѷ���Ϣ�ڳ��ռ�
    boolean                 m_bAutoDelete;

#ifdef FEATURE_SMS_UDH
    uint8                   m_msg_ref;
#endif    
    
    uint16                  m_msgid;
    
    // ����ָʾ������Ϣ�Ƿ��ʼ�������ض�ֵ WMS_CFG_VERSION ��ʾ�ѳ�ʼ��
    int                     nInitIndictor;  
} WMSAPP_Config_Type;
#ifdef FEATURE_USES_MMS
typedef struct
{
    uint32 nResIndex;
    char* type;
}WSP_MMS_RESOURCEINDEXTYPE;

typedef struct
{
    uint16 nIndex;
    uint16 nCount;
    boolean isDoing;
    WSP_MMS_RESOURCEINDEXTYPE data[10];
}WSP_MMS_RESOURCEDATA;

typedef struct 
{
    WSP_MMS_RESOURCEDATA textData;
    WSP_MMS_RESOURCEDATA imageData;
    WSP_MMS_RESOURCEDATA soundData;
    WSP_MMS_RESOURCEDATA videoData;
    WSP_MMS_RESOURCEDATA otherData;
    uint32               nIndex;
}WSP_MMS_RESOURCE;
#endif

// ����������Ϣ�б�˵�ҳ�ķ�ʽ
typedef enum _MAKE_MSGList_Mode
{
   MAKEMSGLIST_INIT,            // ���б�ʼ������
   MAKEMSGLIST_BACKONE,         // �˵��б����һ��
   MAKEMSGLIST_NEXTONE,         // �˵��б�ǰ��һ��
   MAKEMSGLIST_RESUMECURPAGE,   // ʹ�õ�ǰ�˵��б�
   MAKEMSGLIST_PREPAGE,         // �˵��б�ص�ǰһҳ
   MAKEMSGLIST_NEXTPAGE         // �˵��б�ǰ��һҳ
} MAKE_MSGList_Mode;

// ���ڽ���Ⱥ�����յ�ַ�б�˵��ķ�ʽ
typedef enum _MAKE_ADDList_Mode
{
   MAKEADDLIST_NONE,            // ���ڷ�������
   MAKEADDLIST_INIT,            // ���б�ʼ������
   MAKEADDLIST_BACKONE,         // �˵��б����һ��
   MAKEADDLIST_NEXTONE,         // �˵��б�ǰ��һ��
   MAKEADDLIST_RESUMECURPAGE    // ʹ�õ�ǰ�˵��б�ҳ
} MAKE_ADDList_Mode;

// ��Ϣ�༭ʱ��������
typedef enum Insert_Type
{
    INSERT_NONE,        // ��Ч��������
    INSERT_EDITWMS,     // �༭����ʱ����
    INSERT_EDITTEPLATE  // �༭ģ��ʱ����
} Insert_Type;

// �ӵ绰��ȡ����ʱ��������
typedef enum GetAddr_Type
{
    GETADDR_NONE,       // �޲���
    GETADDR_INSERT,     // ��������ַ
    GETADDR_ONEADD,     // ѡȡ���������ַ
    GETADDR_MULTIADD,  // ѡȡ���������ַ
    GETADDR_GROUPlIST,    // Ⱥ���б�
    GETADDR_GROUPLIST_ONEADD // ��Ⱥ�鵥ѡ
} GetAddr_Type;

typedef enum Wms_e_Copy_Type
{
    NONECOPY,
    MOVETORUIM,
    MOVETOPHONEINBOX,
    COPYTORUIM,
    COPYTOPHONEINBOXE,  // ��RUIM��һ����Ϣ���Ƶ������ռ���
    COPYALLMTTORUIM,    // �������ռ�����ȫ����Ϣ���Ƶ�RUIM
    COPYALLRUIMTOPHONE, // ��RUIMȫ����Ϣ���Ƶ�������
    MOVEALLMTTORUIM,    // �������ռ�����ȫ����Ϣ�ƶ���RUIM
    MOVEALLRUIMTOPHONE  // ��RUIMȫ����Ϣ�ƶ���������
} Wms_e_Copy_Type;

typedef enum Wms_e_ContinueSend_Type
{
    NONE_CONTINUE = 0,
    CONTINUE_CURPERSON,
    CONTINUE_NEXTPERSON
} Wms_e_ContinueSend_Type;

//ɾ����Ϣ��ʽ
typedef enum _Erase_SMS_e_Type
{
    ERASE_DRAFT_ONE,        //�ݸ���һ����Ϣ
    ERASE_INBOXES_ONE,      //�ռ���һ����Ϣ
    ERASE_READMSG,          //ɾ���Ѷ���Ϣ
    ERASE_VOICEMAIL_ONE,    //һ��������Ϣ
    ERASE_OUTBOX_ONE,       //������һ����Ϣ
    ERASE_RESERVED_ONE,     //һ��ԤԼ��Ϣ
    ERASE_RESERVEFAILED_ONE,//һ��ʧ��ԤԼ��Ϣ
    ERASE_TEMPLATE_ONE,     //һ��ģ����Ϣ

    CLEAR_PHONE,            //�ռ��仰����Ϣ

#ifdef FEATURE_CDSMS_RUIM
    CLEAR_RUIM,             // UIM������Ϣ
#endif
    CLEAR_OUTBOX,           //������ȫ����Ϣ

    CLEAR_DRAFT,            //ȫ���ݸ���Ϣ
    
    CLEAR_RESERVED,         //ȫ��ԤԼ��Ϣ
    CLEAR_RESERVEFAILED,    //ȫ��ԤԼʧ����Ϣ
    CLEAR_ALLRESERVED,      //ȫ��ԤԼ��Ϣ������δ�����Լ�ʧ�ܵ���Ϣ

    CLEAR_INBOXES,          //�ռ�������Ϣ(����+UIM)
    CLEAR_ALL               //ȫ��ɾ��
#ifdef FEATURE_USES_MMS
    ,CLEAR_INBOX_MMS
    ,ERASE_INBOX_MMS_ONE
    ,CLEAR_OUTBOX_MMS
    ,ERASE_OUTBOX_MMS_ONE
#endif
} Erase_SMS_e_Type;

typedef enum
{
    OPT_VIA_VIEWMSG=0x00,
    OPT_VIA_LISTMSG
} WMS_OPT_e_type;

typedef enum
{
    EXTRACT_NUM=0x00,
    EXTRACT_EMAILADD,
    EXTRACT_URL
} WMS_Extract_e_type;

typedef enum
{
    WMSAPP_STOP=0x00,
    WMSAPP_RUNNING,
    WMSAPP_SUSPEND
} WMSAPP_STATUS_e_type;

typedef enum
{
    DT_SEL_NONE = 0x00,
    DT_SEL_YEAR,
    DT_SEL_MONTH,
    DT_SEL_DAY,
    DT_SEL_HOUR,
    DT_SEL_MIN,
    DT_SEL_TIMEFMT
} DT_Sel_e_type;

typedef struct RESERVEDMSG_DT
{
    uint32  dwSecs;         // �����ʾ��ʱ��
    AECHAR  wstrYear[5];    // ���ַ���,�� 2008
    AECHAR  wstrMonth[3];   // ���ַ���,�� 7
    AECHAR  wstrDay[3];     // ���ַ���,�� 3
    AECHAR  wstrHour[3];    // ʱ�ַ���,�� 15
    AECHAR  wstrMin[3];     // ���ַ���,�� 59
    byte    btTimeFmt;      // ʱ���ʽ
#ifdef FEATURE_TIME_DATA_SETTING
    byte    btDateFmt;      // ���ڸ�ʽ
#endif 
    int     nSelIdx;        // ��ǰѡ��������  
    DT_Sel_e_type eSel[6];  // �ɹ�ѡ�����б�
    DT_Sel_e_type ePreSel;  // ǰһѡ����
    boolean       bAm;      // TRUE: ���� FALSE: ����
    boolean       bFirst;   // �Ƿ��״α༭��ǰ��
} RESERVEDMSG_DT;


// Structure to carry the WMS Applet module information and the object reference count.
typedef struct CWMSMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} CWMSMod;

typedef enum
{
    SEND_MSG_DRAFT,         // ���Ͳݸ���Ϣ
    SEND_MSG_NEW,           // �½�
    SEND_MSG_REPLY,         // �ظ�
    SEND_MSG_FORWARD,       // ת��
    SEND_MSG_RESEND,        // �ط�
    SEND_MSG_TEMPLATE,      // ����ģ��
    SEND_MSG_RESERVE,       // ԤԼ��Ϣ 
    SEND_MSG_EDITRESERVE    // �༭ԤԼ��Ϣ        
} Create_SMS_e_Type;

// �Ի����¼�������ָ�����Ͷ���
typedef boolean (*WMSPFNDLGHANDLER)(void *pUser, AEEEvent evt, uint16 w, uint32 dw);


// WMS Applet״̬��
typedef enum WMSAPPState
{
    // ��״̬
    WMSST_NONE = 0,
    
    // ��ʼ��״̬
    WMSST_INIT,
    
    // ����Ϣ������״̬
    WMSST_MAIN,
    
    // ������֤
    WMSST_CHKPWD,
    
    // �ռ���(Phone+UIM)����״̬
    WMSST_INBOXES,
    
    // ���ڲ鿴�ռ�������Ϣ��״̬
    WMSST_VIEWINBOXMSG,  
    
    // �鿴�ռ�����Ϣ��Ը���Ϣ�Ĵ���ѡ��
    WMSST_INBOXMSGOPTS,
    
#ifdef FEATURE_CDSMS_RUIM
    // ���ڻ�������Ϣ���ơ��ƶ�ǰ�û�ȷ��
    WMSST_MSGCOPYCONFIRM,

    // ���ڻ�������Ϣ���ơ��ƶ�
    WMSST_MSGCOPY,
#endif    
    
    // �����û�������Ϣʱ������շ������״̬
    WMSST_TONUMLIST,
    
    // �����û�������Ϣʱ�༭��Ϣ�ı���״̬
    WMSST_WRITEMSG,
    
    // ��Ϣ����ѡ��
    WMSST_SENDOPTS,
    
    // ������ʾ�û�����Ϣ��״̬
    WMSST_SENDING, 
    
    // ��ʾ�������б��״̬
    WMSST_OUTBOX,
   
    // ���ڲ鿴����������Ϣ��״̬
    WMSST_VIEWOUTBOXMSG,
    
    // �鿴��������Ϣ��Ը���Ϣ�Ĵ���ѡ��
    WMSST_OUTMSGOPTS,  
    
    // ��ʾ�ղؼ���Ϣ�б��״̬
    WMSST_DRAFT, 
    
    // ���ڲ鿴�ݸ�������Ϣ��״̬
    WMSST_VIEWDRAFTMSG, 
    
    // �鿴�ݸ�������Ϣ��Ը���Ϣ�Ĵ���ѡ��
    WMSST_DRAFTMSGOPTS, 

#ifdef FEATURE_CONTACT_APP
    // ��ȡ����
    WMSST_EXTARCTDETAILS,
    
    // ��ȡ�����������嵥
    WMSST_EXTARCTEDITEMLIST,
    
    // �����ȡ����������Ĳ���ѡ��
    WMSST_EXTARCTEDITEMOPT,
#endif
    
    // ����ɾ��������Ϣ
    WMSST_DELETEMSGS, 
    
#if defined(FEATURE_CDSMS_RUIM)
    // ��Ϣ�ڻ����以��
    WMSST_COPYMSGS,
#endif    
    
    // ����ɾ����Ϣȷ��
    WMSST_DELMSGCONFIRM, 
    
    // ���ڲ�������
    WMSST_MSGSETTING, 
    
    // ԤԼ��Ϣ
    WMSST_RESERVEDMSG,
    
    // ԤԼ��Ϣʱ������
    WMSST_RSVDMSGTIME,
    
    // ԤԼ��Ϣ�б�
    WMSST_RESERVEDMSGS,
    
    // �鿴ĳ��ԤԼ��Ϣ
    WMSST_VIEWRESERVEDMSG,

    // ĳ��ԤԼ��Ϣ����ز���ѡ��
    WMSST_RESERVEDMSGOPT,
    
    // ԤԼ����ʱ�䵽��ʾ
    WMSST_RESERVEDMSGALERT,

    // �����������˵�
    WMSST_VOICEMAIL, 
    
    // �鿴��������
    WMSST_VIEWVOICEMAIL, 
    
    // ��ʾ���������������
    WMSST_VMNUMALERT, 
    
    // ���������������
    WMSST_VMNUM, 
    
    // ������Ϣ����ѡ��
    WMSST_VOICEMAILOPTS,  
    
    // ��Ϣģ���б�
    WMSST_TEMPLATES,
    
    // ���ĳ��ģ��Ŀ�ѡ����
    WMSST_TEMPLATESOPTS,
    
    // �鿴ĳ����Ϣģ��
    WMSST_VIEWTEMPLATE,
    
    // ��Ϣģ��༭
    WMSST_EDITTEMPLATE,
    
    WMSST_AUTOSAVE,
    
    // ���ͱ��濪������
    WMSST_REPORTS,
    
#ifdef FEATURE_AUTOREPLACE
    // �ռ�����ʱ���Ƿ�����������Ϣ�Զ��滻�����Ϣ��������
    WMSST_AUTOREPLACE,
#endif    
    
    // Callback Number ��������
    WMSST_CALLBACKNUMSWITCH,
    
    //����ģʽ����
    WMSST_SENDMODE,
    
    // Callback Number ����
    WMSST_CALLBACKNUMSET,
    
    // ���ն��Ŵ洢λ������
    WMSST_STORAGE,
    
    WMSST_EDITCALLREJECTSMS,
   
    // �洢��״̬
    WMSST_MEMSTATUS,

    //��Ϣ����״̬
    WMSST_MANAGEMENT,
    
    //������Ϣ״̬
    WMSST_COPYINBOX,
    
    //�ƶ���Ϣ״̬
    WMSST_MOVEINBOX,
    
    // ��Ϣ��Ч������
    WMSST_MSGVALIDITY, 
    
    // ��Ϣ���ȼ�����
    WMSST_PRIORITY,
    
    WMSST_AUTODELETE,
    
    WMSST_RESERVEDMSGALERTTIMEOUT,
    
    // �Ƿ����δ��ɵķ�������ȷ��
    WMSST_CONTINUESEND_QUERY,

    // ѡ��绰������Դ
    WMSST_SELECTFROM,
    
    //���ڷ���ʧ���Ƿ��ط�  add by yangdecai
    WMSST_RESENDCONFIRM,
	//�����¶�Ϣ��ʾ��Ϣ��add by yangdecai
	WMSST_WMSNEW,
	//���ڵ�����Ϣadd by yangdecai
	WMSST_POPMSG,

	//������ʾFLASH SMS��Ϣ
	WMSST_FLASHSMS,

#ifdef FEATURE_USES_MMS
    // ��ʾ���ŷ������б��״̬
    WMSST_MMSNOTIFY,
    WMSST_MMSDELIVERYREPORT,
    WMSST_READREPLY,
    WMSST_REPORTALLOWED,
    WMSST_SENDERVISIBILITY,
    WMSST_INBOX_MMS,
    WMSST_OUTBOX_MMS,
    WMSST_VIEWINBOXMSG_MMS,
    WMSST_VIEWOUTBOXMSG_MMS,
    WMSST_OUTMSGMMSOPTS,
    WMSST_INMSGMMSOPTS,

	WMSST_GETTING,
	
#endif

    // �˳�����Ӧ��
    WMSST_EXIT
} WMSAPPState;

// ״̬���������ظ���״̬��������ֵ
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// �Ի�����ܵķ���ֵ�б�
typedef enum DLGRetValue
{
   // ��ʼ��ֵ��ָʾ��Ҫ�����Ի���
   DLGRET_CREATE,         
   DLGRET_CANCELED,
   DLGRET_OK, 
   DLGRET_INFO,
   DLGRET_MSGBOX_OK,
   DLGRET_OPT,

   // IDD_MAIN specific return values
   DLGRET_INBOXES,
   DLGRET_WRITEMSG,
   DLGRET_OUTBOX,
   DLGRET_VIEWVOICEMSG,
   DLGRET_DRAFT,
   DLGRET_TEMPLATES,
   DLGRET_MEMSTATUS,
   DLGRET_DELETEMSGS,
   DLGRET_MANAGEMENT,
   DLGRET_COPYINBOX,
   DLGRET_MOVEINBOX,
   DLGRET_SETTINGS,
   DLGRET_RESERVEDMSG,
   
   DLGRET_NEWRESERVEDMSG,
   DLGRET_RESERVEDMSGLST,
   DLGRET_FAILEDRSVDMSGS,
   DLGRET_YEARERR,
   DLGRET_DATEERR,

   // IDD_TEXT specific return values
   DLGRET_PHONE,
   DLGRET_RUIM,
   
   DLGRET_CLEARINBOXES,
   DLGRET_CLEAROUTBOX,
   DLGRET_CLEARDRAFT,
   DLGRET_CLEARRUIM,
   DLGRET_CLEARPHONE,
   DLGRET_CLEARRESERVED,
   DLGRET_CLEARALL,

   DLGRET_COPYMSGS,
   DLGRET_COPYINBOXMSGTORUIM,
   DLGRET_COPYRUIMMSGTOINBOX,
   DLGRET_MOVEINBOXMSGTORUIM,
   DLGRET_MOVERUIMMSGTOINBOX,
   
   DLGRET_SETTINGAUTOSAVE,
   DLGRET_PRIORITY,
   DLGRET_REPORTS,
#ifdef FEATURE_AUTOREPLACE
   DLGRET_AUTOREPLACE,
#endif   
   DLGRET_CALLBACKNUM,
   DLGRET_SENDMODE,
   DLGRET_STORAGE,
   DLGRET_MSGVALIDITY,
   DLGRET_EDITCALLREJECTSMS,
   DLGRET_AUTODELETE,
   DLGRET_RESERVEDMSGALERTTIMEOUT,
   
   DLGRET_WRITEMSGOPTS,
   DLGRET_TEXTFULL,
   DLGRET_SEND,
   DLGRET_RESEND,
   DLGRET_SENDOPT,
   DLGRET_INSERTTEMPLATES,
   DLGRET_SAVETODRAFT,
   DLGRET_SAVEMSG,

   DLGRET_EDIT,
   DLGRET_DELETE,
   DLGRET_DELETEREADMSG,
   DLGRET_ADDNEW,
   DLGRET_VIEW,
   
   DLGRET_SAVE,
   DLGRET_REPLY,
   DLGRET_FORWARD,
#ifdef FEATURE_CONTACT_APP
   DLGRET_EXTACTDETAILS,
   DLGRET_SAVENUM,
   DLGRET_SAVEEMAIL,
   DLGRET_SAVEURL,
#ifdef FEATURE_USES_MMS
   DLGRET_GETMMS,
   DLGRET_GET,
#endif
#endif   
   DLGRET_COPYTORUIM,
   DLGRET_COPYTOPNONE,
   DLGRET_MOVETORUIM,
   DLGRET_MOVETOPNONE,
   
   DLGRET_END,
   
   DLGRET_VMNUM,
   DLGRET_CALLVMNUM,
   
   DLGRET_LOAD,
   DLGRET_LOADOK,
   DLGRET_LOADFAILED,
   DLGRET_LOADCANCELED,
   
   DLGRET_DONE,
   DLGRET_PWDERR,
   DLGRET_PWDOK,

   DLGRET_SELECTFROMOPT,
   DLGRET_EXIT_EDITOR,
   DLGRET_SAVEDRAFT
   ,DLGRET_RESENDCONFIRM
   ,DLGRET_SMSNEW      //add by yangdecai  2010-07-28
   ,DLGRET_SMSVIEWS
   ,DLGGET_SMSNEW_OK
   ,DLGRET_FLASHSMS
   ,DLGGET_FLASHSMS_END
#ifdef FEATURE_USES_MMS
   ,DLGRET_MMSNOTIFY
   ,DLGRET_MMSDELIVERYREPORT
   ,DLGRET_READREPLY
   ,DLGRET_REPORTALLOWED
   ,DLGRET_SENDERVISIBILITY
   ,DLGRET_INBOX_MMS
   ,DLGRET_OUTBOX_MMS
   ,DLGRET_CLEARINBOX_MMS
   ,DLGRET_CLEAROUTBOX_MMS
#endif
/*   
   ,DLGRET_INSERTPICTURE //add by xuhui 2011/08/01
   ,DLGRET_INSERTPICTURE_PRESET //add by xuhui 2011/08/01
   ,DLGRET_INSERTPICTURE_NEW //add by xuhui 2011/08/01
   ,DLGRET_INSERTVIDEO //add by xuhui 2011/08/01
   ,DLGRET_INSERTVIDEO_PRESET //add by xuhui 2011/08/01
   ,DLGRET_INSERTVIDEO_NEW //add by xuhui 2011/08/01
   ,DLGRET_INSERTSOUND //add by xuhui 2011/08/01
   ,DLGRET_INSERTSOUND_PRESET //add by xuhui 2011/08/01
   ,DLGRET_INSERTSOUND_NEW //add by xuhui 2011/08/01
   ,DLGRET_INSERTFILE //add by xuhui 2011/08/01*/
} DLGRetValue;

#ifdef FEATURE_USES_MMS
typedef enum
{
    PLAYER_IDLE   =  0,	
	PLAYER_STOP,	 
	PLAYER_PLAY,		
	PLAYER_PAUSE,
	PLAYER_ERROR
} MMSMediaPlayerState;
#endif

#ifdef _MSC_VER
#pragma pack(push,1)
#endif

typedef struct WmsApp
{
    DECLARE_VTBL(IWmsApp)
    uint32                          m_nRefs;
    IModule                         *m_pModule;
    IDisplay                        *m_pDisplay;
    IShell                          *m_pShell;

    IConfig                         *m_pConfig;

    // ----------------------------------------------------
    IWMS                            *m_pwms;
    wms_client_id_type              m_clientId;	

#ifdef FEATURE_ICM
    ICM                             *m_pICM;
#else
    ITelephone                      *m_pITelephone;                         // instance of ICM
#endif
    IStatic                         *m_pStatic;
    IAnnunciator                    *m_pIAnn;
    IALERT                          *m_pAlert;
    IMenuCtl                        *m_pMenu;
    IImage                          *m_pImage;
    //IImage                          *m_pBgImage;
    
    wms_message_mode_e_type         m_mode;
    
    wms_message_index_type          m_wCurindex;                // ��ǰ��Ϣ�ڴ洢�ռ��е�����
    wms_memory_store_e_type         m_eCurStore;                // ��ǰ�洢�ռ�
    wms_box_e_type                  m_eMBoxType;                // ��ǰ��Ϣ������
    AEECallback                     m_callback;                 // ����ص�
    
    // ��Ϣ�ڲ����ݽṹ��Ա
    WMSMessageStruct                m_msCur;                    // ��ǰ���ڴ������Ϣ
    WMSMessageStruct                m_msSend;                   // רע�ڵ�ǰ�༭����Ϣ
    
    boolean                         m_bCdmaWmsReady;            // CDMA �����Ƿ����
    wms_memory_status_s_type        m_memruimStatuse;           // ruim ���Ŵ洢״̬
    wms_routes_s_type               m_routes;
    wms_client_ts_data_s_type       m_CltTsdata;
    wms_cache_info_node             *m_CurMsgNodes[LONGSMS_MAX_PACKAGES];   // ��ǰ�������Ϣ���б�
    WMSMessageStruct                *m_CurMsgNodesMS[LONGSMS_MAX_PACKAGES]; // ��ǰ�������Ϣ���б�
    int                             m_idxCur;           // ���� m_CurMsgNodes �ĵ�ǰ������
    wms_client_message_s_type       *m_pCurCltPhrase;   // ���ڳ�����
    wms_client_message_s_type       **m_pCurSendCltMsg; // ���ڷ��͸��ض��������Ϣ�б�
    int                             m_idxCurSend;       // ��ǰ���͵���Ϣ�� m_pCurSendCltMsg �е�����
    int                             m_nSendItems;       // m_pCurSendCltMsg ������������
    IVector                         *m_pSendList;
    IVector                         *m_pSaveNumList;
    IVector                         *m_pUserDataMOList; // ��ǰ���͵��û������б�
    int                             m_idxUserdata;      // ��ǰ���͵��û��������û������б��е�������
    AEERect                         m_rc; 
    boolean                         m_bMuitiSend;       // ��ǰ�Ƿ�Ⱥ����Ϣ

    WMSAPPState                     m_prevState;        // Appletǰһ״̬
    WMSAPPState                     m_currState;        // Applet��ǰ״̬
    WMSAPPState                     m_stchkpwdbk;       // Applet������֤ͨ����ǰ��״̬
    WMSAPPState                     m_stcontinuesendbk; // Applet�������Ͷ��ź�ǰ����״̬

    // ������Ϣ�ı��༭״̬ǰ��״̬
    WMSAPPState                     m_STSwitchToEditMsg;
    
    // �Ի���ر�ʱ����ֵ
    DLGRetValue                     m_eDlgReturn;

    // ��ǰ��Ի����IDialog�ӿ�ָ��
    IDialog                         *m_pActiveIDlg;

    // ��ǰ��Ի���� ID 
    uint16                          m_wActiveDlgID;
    
    // ��ǰ��Ի�����¼�������ָ��
    WMSPFNDLGHANDLER                m_pDialogHandler;

    // ǰһ�˵�ѡ����
    uint16                          m_wPrevMenuSel;
    
    // ���˵�ѡ����
    uint16                          m_wMainMenuSel;
    
    // �ַ�����Դ ID
    uint16                          m_wMsgResID;

    // POP �Ի�������ID
    PromptMsg_e_Type                m_ePMsgType;

    WMSAPP_STATUS_e_type            m_eAppStatus;               // Applet ��ǰ����״̬

    Create_SMS_e_Type               m_eCreateWMSType;           // ������Ϣ�Ĵ�������
    

    WMSAPP_Config_Type              m_cfg;                      //�����ļ�ϵͳ����������
    
    Send_OPT_e_Type                 m_SendOPT;                  //��ǰ������Ϣѡ��
    wms_report_status_e_type        m_SendStatus;               //������Ϣ���ص�״̬��
    wms_report_status_e_type        m_GetStatus;               //������Ϣ���ص�״̬��
    wms_cdma_tl_status_e_type       m_SendtlStatus;

    boolean                         m_bNaturalStart;            //�Ƿ񳣹�����
    boolean                         m_bDoNotOverwriteDlgResult; //�Ƿ���д�Ի�����

    Wms_e_Copy_Type                 m_CopyType; 
    
    Erase_SMS_e_Type                m_eEraseWMSType;            //ɾ����Ϣģʽ
    MAKE_MSGList_Mode               m_eMakeListMode;            //������Ϣ�˵��б�ķ�ʽ
    uint16                          m_wCurPageStarxuhao;        //��ǰ��Ϣҳ����ڵ����
    uint16                          m_wSelItemxuhao;            //��ǰ��Ϣҳѡ����ڵ����
    
    MAKE_ADDList_Mode               m_eMakeAddListMode;         //������ַ�˵��б�ķ�ʽ
    int                             m_nCurAddPage;              //��ǰ��ַҳ���
    uint16                          m_CurAddID;                 //��ǰѡ�еĺ����ڲ˵��б��е� ID
    
    int32                           m_dwInsertPos;              //�绰�������λ��
    uint16                          m_wCurTemplate;             //��ǰѡ�е�ģ��
    uint16                          m_wInsertTemplate;          //��ǰѡ�в����ģ��
    boolean                         m_bAddTemplate;             //��ǰ�༭��ģ���Ƿ�����
    Insert_Type                     m_eInsertType;              //��ǰ�����������
    
    boolean                         m_bTextFullAlert;           // �Ƿ���ʾ�û� Text is full
    
    boolean                         m_bCallVMNum;               // �Ƿ񲦴���������
    WMS_OPT_e_type                  m_eOptType;                 // ����ѡ��ķ�ʽ
    WMS_Extract_e_type              m_ExtractType;              // ��ȡȡ��������
    int32                           m_CurExtractItem;           // ��ǰѡ�е���ȡ��
    
    boolean                         m_bNeedContinueSend;        // ���ŷ��ͱ���ֹ���Ƿ���Ҫ��������
    Wms_e_ContinueSend_Type         m_ContinueSendType;         // ������������
    IBacklight*                     m_pBacklight;
    char                            *m_pszArgs;                 // ��������
    
    boolean                         m_refresh_in_progress;      // UIM �Ƿ��ڸ��¶���
    
    RESERVEDMSG_DT                  m_rsvDateTime;              // ԤԼ����ʱ��
    
    int                             m_nMOmaxChars;              // ���ڵ�ǰ�༭����ʱ�������������ַ���
    AEETextInputMode                m_CurInputMode;             // ��ǰ�༭����ʱʹ�õ����뷨
    int                             m_SucNum;                   //���ͳɹ�����
    int                             m_FailNum;                  //����ʧ������
    boolean                         m_CbVal;
    boolean                         m_isPopMenu;                //�жϵ�ǰ�Ƿ���pop menu ״̬
    boolean                         m_bDCDisconnectedInSending; // ��Ϣ�������ڵȴ�����Ĺ����� DC �Ƿ񱻹Ҷ�
    int                             m_nDisconnectedInSendingRetryTimes; // ��Ϣ�������ڵȴ�����Ĺ����� DC �Ǳ��ҶϺ����Է��͵Ĵ���
    boolean                         m_bSending;                 // ��ǰ�Ƿ�����Ϣ
    int                             m_nCopyNum;                 // ��ǰ���ƶ����ƵĶ������� 
    boolean                         m_bSaveFailedMsg;           // �Ƿ񱣴������ʧ�ܵĶ���
    char                             *m_strPhonePWD;
    ISound                          *m_pSound;
	boolean                         m_bisSendSecond;           //����ʧ���Ƿ��Ѿ��ط�
	boolean                         m_bActive;                 //add by yangdecai 2010-07-28
	IRUIM                           *m_pIRUIM;  
	boolean                         m_bincommend;
	boolean                         m_bwriteclr;
	#ifdef FEATURE_FLASH_SMS
	boolean                         m_bflash_sms;              //add by yangdecai 2011-04-01
	#endif
#ifdef FEATURE_USES_MMS      
    MMS_WSP_DEC_DATA                m_DecData;
    MMS_WSP_ENCODE_SEND             m_EncData;
    wms_msg_event_info_s_type       *m_pMsgEvent;
    IImage                          *m_pMMSImage;
    IImage                          *m_pMMSSOUND;
    IImage                          *m_pMMSVIDEO;
    AECHAR                          *m_MMSData;
    boolean                         m_isMMS;
    IFileMgr                        *m_pIFileMgr;
    boolean                         m_isCheckMMSNotify;
    boolean                         m_isMMSNotify;
    int8                            m_isMMSDeliveryReport;
    int8                            m_isMMSReadReply;
    int8                            m_isMMSReporyAllowed;
    int8                            m_isMMSSenderVisibility;
    int                             m_nDlgID;
    IMedia                          *m_pMedia;
    WSP_MMS_RESOURCE                m_ResData;
    uint8                          m_wSelectStore;
    boolean                        m_pMMSMenuHasFocus;
	MMSMediaPlayerState      	   m_CurrentState;                    /*����״̬*/
#endif    
} WmsApp;

/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
// ����������Ϣ�뻰������Ϣ�ϲ�����ʱ���ڵ�������Ż��ڽ�����Ϣ�б�ʱ�������ͻ��
// ������ﶨ��һ�� RUIM ����Ϣ�˵��� ID �Ļ��㣬����ͬһ�����Ĳ˵��RUIM ����
// Ϣ�˵��� ID ��ֵ��Ȼ����Ĵ� RUIM_MSGINDEX_BASE ��
#define RUIM_MSGINDEX_BASE                  (1000)

// ���� BREW 3 ����Ҫ���ض�����Դ�ļ���
#define  AEE_WMSAPPRES_LANGFILE (AEE_RES_LANGDIR WMS_RES_FILE)

// �Զ��Ͽ�ҵ�����ӵ�ʱ�䣺��
#define WMSAPP_AUTO_DISCONNECT_TIME         (5)

// ���ڳ����ţ�δ�յ������ݰ����ı���ʾ����
#ifdef FEATURE_SUPPORT_ID
#define DATA_NOT_RECEIVED    "<...>"
#else
#define DATA_NOT_RECEIVED    "(Data not received)"
#endif
#if defined(FEATURE_VERSION_C306)||defined(FEATURE_VERSION_W0216A)
#define AVK_SEND_TWO     2
#elif defined(FEAUTRE_VERSION_N450)|| defined(FEATURE_VERSION_N021)|| defined(FEATURE_VERSION_C01)
#define AVK_SEND_TWO     2
#else
#define AVK_SEND_TWO     1
#endif

// ���ýӿڵ�Ŀ��
#define STARTARGPREFIX_SENDTEXTMESSAGE      'S'
#define STARTARGPREFIX_WRITENEWMESSAGE      'W'
#define STARTARGPREFIX_SHOWINBOXLIST        'L'
#define STARTARGPREFIX_VIEWVMAIL            'V'
#define STARTARGPREFIX_POWERONSTART         'P'

// �ڲ��ǽӿڵ�������-ԤԼ��������
#define STARTARGPREFIX_RESERVEDMSGSTART     'R'

// һ���˵�����LCD����ͬʱ��ʾ�������ַ�����
#define  MAXCHARS_ONEMENUITEMCANDISPLAY     (16)

// ������Ϣ�б�˵�ʱһҳ�������Ŀ��
#define MAXITEMS_ONEPAGE                    (7)

// ����Ⱥ��ʱ����������ĵ�ַ��
#define MAXNUM_MULTISEND                    (20)

// �����ڲ��������ݽṹ�汾��
#define WMS_CFG_VERSION                     (2)

// ��Ϣ�˵���ID����
#define MSG_CMD_BASE                        (5000)
#ifdef FEATURE_APP_MUSICPLAYER
#define TIME_MS_SMSVIBRATE                  (2000)
#endif
// �˺��õ�ǰ״̬������ǰ״̬������nextState״̬���µ�ǰ״̬
#define MOVE_TO_STATE(nextState)            \
{                                           \
    WMSAPPState tpState;                    \
    tpState = nextState;                    \
    pMe->m_prevState = pMe->m_currState;    \
    pMe->m_currState = tpState;             \
}

//�رնԻ���ĺ�
#define CLOSE_DIALOG(dlgResult)             \
{                                           \
    pMe->m_eDlgReturn = dlgResult;          \
    (void) ISHELL_EndDialog(pMe->m_pShell); \
}

#ifdef _MSC_VER
#pragma pack(pop)
#endif


/*==============================================================================

                                 ��������

==============================================================================*/
/*==============================================================================
����:
    WmsApp_StartSendingMessage

˵��:
    ʹ�� IWMS �ӿڣ���ʼ������Ϣ��

����:
    pMe [in]:ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    �ޡ�

��ע:
    ��Ϣ���ͷ�ʽ����Ϣ���ȶ�������Ϣ�����������ӽ����ŵ�������Ϣ����̽��
        
==============================================================================*/
int WmsApp_StartSendingMessage(WmsApp *pMe, boolean  bIsMultiSend);

/*==============================================================================
����:
    WmsApp_MultSendMsgTimer

˵��:
    Ⱥ����Ϣʱ����Ϣ���ͼ�ļ��ʱ�䶨ʱ��������������һ����Ϣ�ķ��͡�

����:
    pme [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
void WmsApp_MultSendMsgTimer(void *pme);

/*==============================================================================
����:
    WmsApp_ReSendMsgTimer

˵��:
    �ط���Ϣʱ����Ϣ���ͼ�ļ��ʱ�䶨ʱ��������������һ��Ϣ���ݰ��ķ��͡�

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
void WmsApp_ReSendMsgTimer(void *pme);

/*==============================================================================
����:
    WmsApp_SendReservedMsgTimer

˵��:
    ����ԤԼ��Ϣʱ����Ϣ���ͼ�ļ��ʱ�䶨ʱ��������������һ��Ϣ���ݰ��ķ��͡�

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
    �ޡ�

��ע:

==============================================================================*/
void WmsApp_SendReservedMsgTimer(void *pme);

/*==============================================================================
������
    WmsApp_GetAddItem

˵����
    ��������ȡ��ָ�������ĺ��롣

������
    pIV [in]��ָ��IVector�ӿڶ����ָ�롣
    wIdx [in]�������š�

����ֵ��

��ע:
       
==============================================================================*/
CMultiSendItemInfo * WmsApp_GetAddItem(IVector *pIV, uint16 wIdx);


/*==============================================================================
����:
    WmsApp_UpdateMenuList

˵��:
    ���µ�ǰ pMe->m_eMBoxType ָ����������Ϣ�˵��б�

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pMenu [in/out]: IMenuCtl�ӿ�ָ�롣

����ֵ:
    none

��ע:
    New
        
==============================================================================*/
void WmsApp_UpdateMenuList(WmsApp *pMe, IMenuCtl *pMenu);

/*==============================================================================
����:
    WmsApp_ProcessState

˵��:
    ���� pMe->m_currState, ������Ӧ��״̬��������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    NFSMACTION_CONTINUE: ָʾ��ͣ״̬����
    NFSMACTION_WAIT: ָʾֹͣ״̬����

��ע:

==============================================================================*/
NextFSMAction WmsApp_ProcessState(WmsApp *pMe);

/*==============================================================================
����:
    WmsApp_ShowDialog

˵��:
    ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
    pMe [in]:ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    dlgResId [in]:�Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
    none

��ע:

==============================================================================*/
void WmsApp_ShowDialog(WmsApp  *pMe,  uint16 dlgResId);

/*==============================================================================
����:
    WmsApp_ShowMsgBox

˵��:
    ������״̬����������������Ϣ�Ի�����ʾ��Ϣ��ʾ�û���

����:
    pMe [in]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wTextResId [in]: ��Ϣ�ı���Դ ID��

����ֵ:
    none

��ע:

==============================================================================*/
void WmsApp_ShowMsgBox(WmsApp *pMe, uint16 wTextResId);

/*==============================================================================
����:
    WmsApp_SetDialogHandler
       
˵��:
    �������� pMe->m_wActiveDlgID �趨�Ի��¼�������ָ�� m_pDialogHandler ��Ӧֵ��

����:
    pMe [In]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
    ���Ҳ�����Ӧ�ĶԻ�������, m_pDialogHandler ����Ϊ NULL
==============================================================================*/
void WmsApp_SetDialogHandler(WmsApp *pMe);

/*==============================================================================
����:
    WmsApp_RouteDialogEvt

˵��:
    ������ BREW �¼�·�ɸ���ǰ��Ի�����¼���������

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    eCode [in]: �¼����롣
    dwParam [in]: ���¼�eCode���������ݡ�

����ֵ:
    TRUE:�����¼�������
    FALSE:�����¼�û������

��ע:

==============================================================================*/
boolean WmsApp_RouteDialogEvt(WmsApp *pMe, 
                              AEEEvent eCode, 
                              uint16  wParam, 
                              uint32  dwParam);

/*==============================================================================
����: 
    WmsApp_wmscommandreport
       
˵��: 
    WMS �ô˺�����������߻㱨���������
       
����: 
    cmd [in]: wms �������͡�
    user_data[in]: ��������ʱ������û�����ָ�롣
    cmd_err [in]: ����״̬��������͡�
    
����ֵ:
    none
       
��ע:

==============================================================================*/
void WmsApp_wmscommandreport(wms_cmd_id_e_type   cmd,
                             void                *user_data,
                             wms_cmd_err_e_type  cmd_err);

/*==============================================================================
����:
    WmsApp_FreeSaveNumList

˵��:
    �ͷ������е�ȫ�����ݡ�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeSaveNumList(WmsApp *pMe);

/*==============================================================================
����:
    WmsApp_FreeMultiSendList

˵��:
    �ͷ������е�ȫ�����ݡ�

����:
    pIV [in]: ָ��IVector�ӿڶ����ָ�롣

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeMultiSendList(IVector *pIV);

/*==============================================================================
����:
    WmsApp_FreeMsgNodeMs

˵��:
    �����ͷ� WmsApp �ṹ�� m_CurMsgNodesMS ��Ա��������ݡ�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeMsgNodeMs(WmsApp *pMe);

/*==============================================================================
����:
    WmsApp_FreeSendClentMsgList

˵��:
    �ͷŷ��Ϳͻ���Ϣ���е�ȫ�������

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeSendClentMsgList(WmsApp * pMe);

/*==============================================================================
����:
    WmsApp_CombinateMsg

˵��:
    ������ WmsApp �ṹ�� m_CurMsgNodesMS ��Ա�������Ϣ���ݺϲ��� m_msCur �С�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_CombinateMsg(WmsApp *pMe);

/*==============================================================================
����:
    WmsApp_CopyCurMsgForSend

˵��:
    ��������ǰ�������Ϣ���ݸ��Ƶ� m_msSend �С�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_CopyCurMsgForSend(WmsApp *pMe);

/*==============================================================================
����:
    WmsApp_FreeUserDataMOList

˵��:
    �����ͷ� WmsApp �ṹ�� m_pUserDataMOList ��Ա��������ݡ�

����:
    pIV [in]: ָ��IVector�ӿڶ����ָ�롣

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_FreeUserDataMOList(IVector *pIV);

/*==============================================================================
����:
    WmsApp_PrepareUserDataMOList

˵��:
    ��������ǰ���͵���Ϣ���� m_msSend ת���� m_pUserDataMOList �С�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_PrepareUserDataMOList(WmsApp *pMe);

/*==============================================================================
����:
    WmsApp_GetClientMsgMO

˵��:
    �������� wms_cdma_user_data_s_type �ṹ��Ϣ���� wms_client_message_s_type ��
    �� MO ��Ϣ��

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
    �������� buffer ϵ��̬���䣬�ɵ����߸����ͷš�

==============================================================================*/
wms_client_message_s_type *WmsApp_GetClientMsgMO(WmsApp *pMe, boolean bSend);

/*==============================================================================
����:
    WmsApp_BuildSendClentMsgList

˵��:
    �������Ϳͻ���Ϣ�б�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:
       
==============================================================================*/
void WmsApp_BuildSendClentMsgList(WmsApp * pMe);

/*==============================================================================
����:
    WmsApp_ProcessStatus

˵��:
    ������������ wms �ı�����Ϣ��

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pRptInfo [in]: wms_submit_report_info_s_type �ṹָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
void WmsApp_ProcessStatus(WmsApp *pMe, wms_submit_report_info_s_type *pRptInfo);


#ifdef FEATURE_USES_MMS
/*==============================================================================
����:
    WmsApp_ProcessMMSStatus

˵��:
    ������������ wms �ı�����Ϣ��

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:

==============================================================================*/

void WmsApp_ProcessMMSStatus(WmsApp *pMe);

#endif

/*==============================================================================
����:
    WmsApp_MemberReset

˵��:
    ������ WmsApp �ṹ���ֳ�Ա�����������á�

����:
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
    none

��ע:

==============================================================================*/
void WmsApp_MemberReset(WmsApp *pMe);

/*==============================================================================
������
    WmsApp_UpdateAnnunciators

˵����
    ���ڵ�ǰ��������(���������ʼ�)����������ʾͼ�ꡣ

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    �ޡ�

��ע��
    ��ʹApplet������Ҳ����¡�����֪ͨ��Ϣͼ��������ʾ��
        
==============================================================================*/
void WmsApp_UpdateAnnunciators(WmsApp * pMe);

/*==============================================================================
������
    WmsApp_GetmemAlertID

˵����
    ������ȡ��ǰ�洢�ռ�澯�ַ�������Դ�ļ��е� ID��

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    eBox[in]: wms_box_e_type

����ֵ��
    �澯�ַ�������Դ�ļ��е� ID��

��ע��
    ������ֵΪ0����ʾ����Ҫ�澯
        
==============================================================================*/
uint16 WmsApp_GetmemAlertID(WmsApp * pMe, wms_box_e_type eBox);

/*==============================================================================
������
    WmsApp_PlaySMSAlert

˵����
    �����������Ŷ�����ʾ��

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    bsmsin[in]: TRUE - ����  FALSE - ȥ��

����ֵ��
    none

��ע��
        
==============================================================================*/
void WmsApp_PlaySMSAlert(WmsApp * pMe, boolean bsmsin);

/*==============================================================================
������
    WmsApp_CurmessageIsFullSendout

˵����
    ���������жϵ�ǰ���͵���Ϣ�Ƿ�����ϡ�

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    TRUE - ���
    FALSE - ��δ���

��ע��
        
==============================================================================*/
boolean WmsApp_CurmessageIsFullSendout(WmsApp * pMe);

/*==============================================================================
������
    WmsApp_IsNeedContinueSendTask

˵����
    ���������ж��Ƿ���Ҫ����δ�����ŷ�������

������
    pMe [in]: ָ�� WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    TRUE - ��δ�����ŷ������������
    FALSE - ��δ�����ŷ�������

��ע��
    �Ƿ���δ�����ŷ��������ж���������Ϊ:
    1�����ñ�����ʱ��ǰ״̬���账�� WMSST_SENDING ;
    2�����͵��ض�����Ķ������ݰ��Ƿ������;
    3��Ⱥ���Ƿ������;
        
==============================================================================*/
boolean WmsApp_IsNeedContinueSendTask(WmsApp * pMe);

/*==============================================================================
������
    WmsApp_ReservedMsgSetTimer

˵����
    �����������ԤԼ�����б�, ���ڹ��ڵĶ������޸�����, ������ЧԤԼ���ţ�����
    ԤԼ���Ŷ�ʱ����

������
    pMe [in]: ָ�� WMS Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
    none

��ע��
        
==============================================================================*/
void WmsApp_ReservedMsgSetTimer(WmsApp * pMe);


#ifdef FEATURE_USES_MMS
/*==============================================================================
����:
    WmsApp_UpdateMenuList_MMS

˵��:
    ���µ�ǰ pMe->m_eMBoxType ָ����������Ϣ�˵��б�

����:
    pMe [in]: ָ��WMS Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    pMenu [in/out]: IMenuCtl�ӿ�ָ�롣

����ֵ:
    none

��ע:
    New
        
==============================================================================*/
void WmsApp_UpdateMenuList_MMS(WmsApp *pMe, IMenuCtl *pMenu);
#endif
void WMSDialog_KeypadLock(boolean block);

#endif // end WMSAPP_PRIV_H
