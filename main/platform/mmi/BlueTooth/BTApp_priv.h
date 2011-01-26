#ifndef BTAPPPRIV_H
#define BTAPPPRIV_H

/*==============================================================================
// �ļ���
//        BTApp_priv.h
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//
// �ļ�˵����
//
// ���ߣ�zzg
// �������ڣ�2011-01-04
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��    ���ⵥ��  �޸����ݡ�λ�ü�ԭ��
-----------      ----------  --------  -----------------------------------------
2011-01-04                  ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#ifndef AEE_SIMULATOR
#include "customer.h"   		//Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif

#if !defined(BREW_STATIC_APP)
// Use the .bid file if compiling as dynamic applet
   #include "BTApp.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids 
   #ifndef AEECLSID_BLUETOOTH_APP
      #error AEECLSID_BLUETOOTH_APP must be defined
   #endif
#endif

#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
//#include "OEMMacros.h"

#include "AEEMenu.h"
#include "AEEConfig.h"
#include "AppComFunc.h"
#include "AEEText.h"
#include "AEEAnnunciator.h"
//#include "AEEPhone.h"//add by lzhl20040823
#include "OEMText.h"
#include "OEMFeatures.h"
#include "OEMCFGI.h"
#include "AppComFunc.h"
#include "Appscommon.h"
#include "AEERUIM.h"
#include "OEMText.h"
#include "OEMNV.h"


#include "BTApp.h"

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
//efine AEE_APPSBTAPP_RES_FILE AEE_RES_LANGDIR BTAPP_RES_FILE	//Add By zzg 2010_12_10

#define BTAPP_ERR(format, code1, code2, code3) MSG_FATAL(format, code1, code2, code3)

// ������ײ���ʾ���궨��
#define BTAPP_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}
/*----------------------�Ի������������������---------------------*/

// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
    DLGRET_CREATE,					//IDD_BT_MAINMENU
    DLGRET_OK,  
    DLGRET_CANCELED,			
	
	DLGRET_DEVICESRH,				//IDD_DEVICE_SEARCH
	DLGRET_SRHRESULT,				//IDD_SEARCH_RESULT
	DLGRET_DEVICEINFO,				//IDD_BT_DEVICE_INFO
	DLGRET_DEVICEINFO_OPITION,		//IDD_BT_DEVICE_INFO_OPITION
	DLGRET_BOND_OPITION,			//IDD_BT_BOND_OPITION
	DLGRET_OPITION,					//IDD_BT_OPITION

	DLGRET_DEVICE_LIST,				//IDD_DEVICE_LIST
	DLGRET_BOND_LIST,				//IDD_BOND_LIST
	DLGRET_ALL_LIST,				//IDD_ALL_LIST
	DLGRET_CONNECT_LIST,			//IDD_CONNECT_LIST
	
	DLGRET_MYINFO,					//IDD_MYINFO
	DLGRET_MYINFO_OPITION,			//IDD_MYINFO_OPITION
	DLGRET_EDIT_NAME,				//IDD_BT_EDITNAME
	DLGRET_EDIT_SHORTNAME,			//IDD_BT_EDIT_SHORTNAME
	DLGRET_EDIT_MANUDATA,			//IDD_BT_EDIT_MANUDATA
	DLGRET_SET_SECURITY,			//IDD_BT_SECURITY
	DLGRET_SET_DISCOVERABLE,		//IDD_BT_DISCOVERABLE
	DLGRET_SET_IOCAPABILITY,		//IDD_BT_IO_CAPABILITY
	DLGRET_SET_DEBUGKEY,			//IDD_BT_DEBUG_KEY
	DLGRET_BT_MSGBOX,				//IDD_BT_MSGBOX
	DLGRET_PROMPT,            		//IDD_PROMPT_MESSEGE

	DLGRET_BT_SEND_FILE,			//IDD_BT_SEND_FILE
	DLGRET_BT_OBEX_LIST_SERVERS,	//IDD_BT_OBEX_LIST_SERVERS
	DLGRET_BT_FILE_PROGRESS,		//IDD_BT_FILE_PROGRESS

	DLGRET_BT_EDIT				//IDD_BT_EDIT	
} DLGRet_Value_e_Type;


/*----------------------״̬�����������������---------------------*/
// BTApp Applet ״̬��״̬��
typedef enum _BTAppState
{
	BTAPPST_NONE,
	BTAPPST_INIT,
	BTAPPST_MAIN,

	BTAPPST_DEVICESRH,
	BTAPPST_SRHRESULT,
	BTAPPST_DEVICEINFO,			
	BTAPPST_DEVICEINFO_OPITION,	
	BTAPPST_BOND_OPITION,			//IDD_BT_BOND_OPITION
	BTAPPST_OPITION,				//IDD_BT_OPITION

	BTAPPST_DEVICE_LIST,
	BTAPPST_BOND_LIST,
	BTAPPST_ALL_LIST,
	BTAPPST_CONNECT_LIST,

	BTAPPST_MYINFO,
	BTAPPST_MYINFO_OPITION,
	BTAPPST_EDIT_NAME,
	BTAPPST_EDIT_SHORTNAME,
	BTAPPST_EDIT_MANUDATA,
	BTAPPST_SET_SECURITY,
	BTAPPST_SET_DISCOVERABLE,
	BTAPPST_SET_IOCAPABILITY,
	BTAPPST_SET_DEBUGKEY,
	BTAPPST_BT_MSGBOX,
	BTAPPST_PROMPT,

	BTAPPST_BT_EDIT,   

	BTAPPST_BT_SEND_FILE,
	BTAPPST_BT_OBEX_LIST_SERVERS,
	BTAPPST_BT_FILE_PROGRESS,

	BTAPPST_EXIT
} BTAppState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct BTAppMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} BTAppMod;



typedef struct _IBTApp  IBTApp;
 QINTERFACE(IBTApp)
{
    INHERIT_IApplet(IBTApp);
};


/*==============================================================================
                                 ��ӿں�������
==============================================================================*/
#define IBTApp_AddRef(p)                GET_PVTBL(p,IBTApp)->AddRef(p)

#define IBTApp_Release(p)                GET_PVTBL(p,IBTApp)->Release(p)

#define IBTApp_HandleEvent(p,ec,wp,dw) \
GET_PVTBL(p,IBTApp)->HandleEvent(p, ec, wp, dw)




typedef struct _CBTApp
{
  DECLARE_VTBL(IBTApp)
  	
  //AEEApplet       a;
  IShell           *m_pShell;
  
  IDisplay*       m_pIDisplay;

  AEERect         m_rect;
  uint16          m_nColorDepth;

  //Add For BT_MSG_BOX/BT_PROMPT
  uint16          m_msg_id;
  uint16		  m_msg_state_id;
  boolean		  m_bNeedStr;
  AECHAR  		  wMsgBuf[MSGBOX_MAXTEXTLEN];

  uint16          m_prompt_id;
  uint16		  m_prompt_state_id;
  boolean		  m_bPromptNeedStr;
  AECHAR  		  wPromptBuf[MSGBOX_MAXTEXTLEN];  

  uint16          m_edit_id;
  uint16		  m_edit_state_id;
  boolean		  m_bEditNeedStr;
  AECHAR  		  wEditBuf[MSGBOX_MAXTEXTLEN];  

  uint16		  m_dialog_id;

  boolean		  m_app_flag;			//��ʾ�Ƿ�ǰӦ��ʱBLUETOOTHAPP
  uint16  		  m_user_wParam;		//�洢EVT_USER��WPARAM


  AEEBTProgressInfo m_fileprogInfo;
  //Add End

  //Add By zzg 2011_01_04
  uint32           m_nRefs;            // �������ü�����
  IModule          *m_pModule;             
  IDialog          *m_pActiveDlg;      // ��ǰ��Ի���ID
  uint16           m_pActiveDlgID;     // ��Ի���ر�ʱ�ķ��ؽ��
  DLGRet_Value_e_Type   m_eDlgRet;     // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
  boolean          m_bNotOverwriteDlgRet;// Applet ǰһ״̬
  BTAppState 		m_ePreState;        // Applet ��ǰ״̬
  BTAppState 		m_eCurState;        // Applet�Ƿ��ڹ���״̬
  boolean          m_bSuspending;      // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
  boolean          m_bAppIsReady;

  uint16           m_currDlgId;       //���浱ǰ��ѡ�˵�λ��
  uint16           m_nSubDlgId;		    //�μ��˵�λ��
  uint16           m_sSubDlgId;       //�����˵�λ��

  IRUIM            *m_pIRUIM;
  //Add End

  IMenuCtl*       m_pIMenu;
  IMenuCtl*       m_pISoftMenu;
  IStatic*        m_pStatic;
  ITextCtl*       m_pText;

  IAnnunciator    *m_pIAnn;		//Add By zzg 2010_10_29
  IConfig         *m_pConfig;	//Add By zzg 2010_11_23 

  BTAppMenuType   menuStack[ MENU_STACK_DEPTH ];
  uint16          uMenuSel[ MENU_STACK_DEPTH ];

  char			  m_pfilepath[AEEBT_MAX_FILE_NAME];
  
  uint8           uTopMenu;
  BTApp_Enabling  mEnablingType;
  AG              mAG;
  NA              mNA;
  SD              mSD;
  RM              mRM;
  SPP             mSPP;
  L2              mL2;
#ifdef FEATURE_BT_EXTPF_AV
  A2DP            mA2DP;
  AVRCP           mAVRCP;
#endif
#ifdef FEATURE_BT_EXTPF_OPP
  OPP             mOPP;
#endif
#ifdef FEATURE_BT_EXTPF_FTP
  FTP             mFTP;
#endif
#ifdef FEATURE_APP_TEST_AUTOMATION
#error code not present
#endif  //FEATURE_APP_TEST_AUTOMATION


#ifdef FEATURE_BT_EXTPF_BIP
  BIP             mBIP;
#endif
#ifdef FEATURE_IOBEX
  OBEX            mOBEX;
#endif
#ifdef FEATURE_BT_EXTPF_BPP
  BPP             mBPP;
#endif
#ifdef FEATURE_BT_EXTPF_PBAP
  PBAP            mPBAP;
#endif
#ifdef FEATURE_BT_EXTPF_CTP
  CTP             mCTP;
  ICP             mICP;
#endif

#ifdef FEATURE_BT_EXTPF_SAP
#error code not present
#endif

#ifdef FEATURE_BT_EXTPF_HID_HOST
#error code not present
#endif
  ITelephone*     pIPhone;
  boolean         bFirstLaunch;
  boolean         bEnableAtStart;
  boolean         bSuspended;
  boolean         bBusyIconUp;
  boolean         bVocoderOn;
  boolean         bConfigChanged;
  uint16          uCurrMsgId;
  
  boolean		  bStartFromOtherApp;				//Add By zzg 2010_11_09  
  boolean		  bUpdateProgress;					//Add By zzg 2010_11_27
  
  AECHAR*         pText1;
  AECHAR*         pText2;
  uint8*          pMem;
#ifdef FEATURE_BT_2_1
  boolean         bRemoteOOBRead; 
#endif
  boolean         bStartBTaplication;
} CBTApp;



/* Dialog Event Handler function pointer structure */
typedef boolean (*PFNDLGHANDLER) (void *pUser,AEEEvent evt,uint16 w,uint32 dw );

extern boolean BTApp_BuildMenu( CBTApp* pMe, BTAppMenuType menu );
extern boolean BTApp_BuildTopMenu( CBTApp* pMe );
extern boolean BTApp_HandleClearKey( CBTApp *pMe );
extern boolean BTApp_WriteConfigFile( CBTApp* pMe );
extern boolean BTApp_AGInit( CBTApp *pMe );
extern void    BTApp_AGDeInit( IApplet *pi  );
extern void    BTApp_DeregisterAGNotif( CBTApp *pMe );
extern void    BTApp_EnableBT( CBTApp* pMe );
extern void    BTApp_DisableBT( CBTApp* pMe );


//Add By zzg 2011_01_10
extern void BTApp_LocalOOBCreated( CBTApp* pMe );
extern void BTApp_CancelMsgBox( CBTApp* pMe );
extern void BTApp_CancelDevNameRequest( CBTApp* pMe );
extern int BTApp_RemoteOOBWrite( CBTApp* pMe,  AEEBTDeviceInfo *pDev );
extern void BTApp_DoRemoveOne( CBTApp* pMe );
extern void BTApp_PadString(char* Passkey, int passKeyLen);
extern boolean BTApp_UserConfirm( CBTApp* pMe, boolean bConfirmed);
extern void BTApp_BuildPrompt( CBTApp* pMe, BTAppMenuType menu );
extern boolean BTApp_DoUnbondAll( CBTApp* pMe );
extern boolean BTApp_DoRemoveAll( CBTApp* pMe );
extern void BTApp_DoUnbondOne( CBTApp* pMe );
//Add End


extern const AEEBTBDAddr NULL_BD_ADDR;


 
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            BTAppState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }


#define APPISREADY_TIMER       100             
#define EVT_APPISREADY   (EVT_USER+1)  

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
       BTApp_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��BTApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void BTApp_ShowDialog(CBTApp *pMe, uint16  dlgResId);

/*==============================================================================
����:
       BTApp_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��BTApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean BTApp_RouteDialogEvent(CBTApp *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       BTApp_ProcessState
˵��:
       BTApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��BTApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction BTApp_ProcessState(CBTApp *pMe);
#endif
//BTAPPPRIV_H

