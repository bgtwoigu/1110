#ifndef CONTAPP_PRIV_H
#define CONTAPP_PRIV_H

/*==============================================================================
// �ļ���
//        ContApp_priv.h
//        2004-06-08 jyh����汾(Draft Version)
//        ��Ȩ����(c) 2004 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2004-06-08
// �������ڣ�jyh
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ    ��      ��   ��   ��     �޸����ݡ�λ�ü�ԭ��
--------      -----------     -----------------------------------------------
04-06-08      jyh             ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#ifndef AEE_SIMULATOR
#include "OEMFeatures.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif

#include "AEE.h"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "AEEConfig.h"
#include "AEEFile.h"
#include "AEEText.h"
#include "AEERinger.h"
#include "AEEDate.h"
#include "AEE_OEM.h"

#if defined(AEE_STATIC)
// For BREW3.1
//#include "AEE_static.h"
#include "OEMClassIDs.h"
//#include "OEMMacros.h"
//#include "uiutils.h"
#include "AppComFunc.h"
#ifndef WIN32
#include "err.h"
#include "ui.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32

#include "AEEVector.h"
#endif

#include "ContApp.h"
#include "contactapp.brh"
#include "CallApp.h"
//#include "SMSApp.h"
#include "Appscommon.h"

#include "OEMCFGI.h"
#ifdef FEATURE_ICM
#include "AEECM.h"
#else
#include "AEETelephone.h"
#include "AEETelDef.h"
#endif
#include "AEEAddrBookExt.h"


// ͼƬ��Դ�ļ�
#include "appscommonimages.brh"

#include "AEEAnnunciator.h"

#include "AEEControls.brh"
#include "AEERUIM.h"
extern sSelectFieldInfo  curSelectFieldInfo;
extern sSelectFieldListNode* pSelectFieldListRoot;
extern sSelectFieldListNode* pCurSelectFieldNode;

#define FEATURE_SMARTSEARCH

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch 
#define CONTAPP_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif

/*==============================================================================
                                 ��������
==============================================================================*/
#ifdef FEATURE_SUPPORT_VCARD
#define DEFAULT_VCARD_NAME  "fs:/address/vCard.vcf"
#define DEFAULT_VCARD_SENDNAME ("F"DEFAULT_VCARD_NAME)
#define VCARD_PATH "fs:/address/\0"
#define VCARD_EXTEND ".vcf"
#define VCARD_PATH_LEN   12
#endif

#define MAX_LEN_WMS_BUF                 (1024)
#define MAX_LEN_COMMBUF                 (256)
#define MAX_LEN_MSGBOX                  (256)
#define MAX_LEN_PASSWORD                (6)
#define MAX_LEN_TITLE                   (20)
#define MAX_LEN_POSITION                (3)

#define TIME_ONE_SEC                    (1000)
#define TIMEOUT_MS_MSGBOX               (3*TIME_ONE_SEC)
#define TIMEOUT_MS_QUICK                (500)
#define TIMEOUT_SMARTKEY                (1000)

#if defined(FEATURE_DISP_160X128)
#define MAX_NUM_MENUPOP                 (5) //(6)
#elif defined(FEATURE_DISP_128X160)
#define MAX_NUM_MENUPOP                 (7) //(6)
#elif defined(FEATURE_DISP_128X128)
#define MAX_NUM_MENUPOP                 (5) //(6)
#elif defined(FEATURE_DISP_176X220)
#define MAX_NUM_MENUPOP                 (7) //(6)
#elif defined(FEATURE_DISP_220X176)
#define MAX_NUM_MENUPOP                 (7) //(6)
#elif defined(FEATURE_DISP_240X320)
#define MAX_NUM_MENUPOP                 (7) //(6)
#elif defined(FEATURE_DISP_320X240)
#define MAX_NUM_MENUPOP                 (7) //(6)
#else
#define MAX_NUM_MENUPOP                 (5) //(6)
#endif
#define MAX_NUM_NAME                    (1)
#define MAX_NUM_NUM                     (5)
#define MAX_NUM_EMAIL                   (2)
#define MAX_NUM_URL                     (2)
#define MAX_NUM_BIRTHDAY                (1)
#define MAX_NUM_NOTES                   (1)
#define MAX_NUM_OTHER                   (1)
#if defined(FEATURE_VERSION_C306)
#define MAX_INPUT_NAME_CARD             (20)
#define MAX_INPUT_NAME_CN               (20)
#define MAX_INPUT_NAME_EN               (20)
#else
#define MAX_INPUT_NAME_CARD             (14)
#define MAX_INPUT_NAME_CN               (16)
#define MAX_INPUT_NAME_EN               (16)
#endif

#define MAX_INPUT_NAME_TH               (32)
#define MAX_INPUT_NUM_CARD              (20)
#define MAX_INPUT_NUM                   (32)
#define MAX_INPUT_EMAIL                 (64)
#define MAX_INPUT_URL                   (128)
#define MAX_INPUT_NOTES                 (128)
#define MAX_INPUT_ADDRESS               (128)

#define MAX_PHONEBOOK_CAPACITY          (500)
#define MAX_RUIMBOOK_CAPACITY           (200)

#define MAX_PHONE_FIELD    (0x0B)
#define MAX_RUIM_FIELD    (0x02)


// �����ļ���
#define FILE_CONTAPP_CFG                (AEE_ADDRBOOK_DIR"\\contapp.cfg")

// �����ļ��汾
#define CFG_VERSION                     (0x0001)

// View type 0 is default
#define CONTCFG_VIEWTYPE_ALL            (0)
#define CONTCFG_VIEWTYPE_PHONE          (1)
#ifdef FEATURE_RUIM_PHONEBOOK
#define CONTCFG_VIEWTYPE_RUIM           (2)
#endif /* FEATURE_RUIM_PHONEBOOK*/
#define CONTCFG_VIEWTYPE_NONE           (3)

// Save type 0 is default
#define CONTCFG_SAVETYPE_SELECT         (0)//����ʱѡ��
#define CONTCFG_SAVETYPE_PHONE          (1)
#define CONTCFG_SAVETYPE_RUIM           (2)

// Default password
#define DEFAULT_PASSWORD                ('\0')

// Onedial default, RUIM ������ʼ��¼��1���������ʹ��0
#define ONEDIAL_NULL                    ('\0')

// �ռ�¼��
#define CONTAPP_RECID_NULL              (0xFFFF)

// �ղ˵�ѡ���
#define MENU_SELECT_NULL                (0xFFFF)

// ���ֶ�����
#define CONTAPP_FLDID_NULL              (0xFFFF)

// RUIM����¼��ʼ��
#define CONTAPP_RUIM_START_IDX          (0x4000)

#define AEE_ADDR_CAT_MAX_CHAR (0x20)

#define CONTAPP_NUMBERFLDS              (4)

// �ֶ����ȼ�
#define PRI_NAME                        (0x01)
#define PRI_NUM_PREF                    (0x02)
#define PRI_NUM_HOME                    (0x03)
#define PRI_NUM_WORK                    (0x04)
#define PRI_NUM_FAX                     (0x05)
#define PRI_NUM_CELL                    (0x06)
#define PRI_NUM_GENERIC                 (0x07)
#define PRI_EMAIL                       (0x08)
#define PRI_URL                         (0x09)
#define PRI_BIRTHDAY                    (0x0A)
#define PRI_NOTES                       (0x0B)
#define PRI_PHOTO                       (0x0C)
#define PRI_RING                        (0x0D)
#define PRI_DEFAULT                     (0xFF)
#if defined(FEATURE_VERSION_C306)
#define AVK_SEND_TWO     2
#elif defined(FEAUTRE_VERSION_N450)
#define AVK_SEND_TWO     2
#else
#define AVK_SEND_TWO     1
#endif

// �ֶ�����
#define AEE_ADDR_KEY_LEFT (0x1)
#define AEE_ADDR_KEY_RIGHT (0x2)
#define AEE_ADDR_KEY_UP (0x3)
#define AEE_ADDR_KEY_DOWN (0x4)


// Error value
#define EEXIST                          (ERROR_USER+1)
#define EMAXRECORD                      (ERROR_USER+2)
#define EMAXFIELD                       (ERROR_USER+3)
#define ENORECORD                       (ERROR_USER+4)

// number fields when create record
#define NUM_CREATE_FLDS                 (2)
#define IDX_NAME_FLD                    (0)
#define IDX_NUM_RUIM_FLD                (1)
#define STEP_COPY                       (10)

// Speed dial Max len
#define MAX_LEN_SPEEDDIAL               (6)
#define RINGID_USESYSTEM                (IDI_RING_MENU_USESYSTEM)

// Ф����Ƭ(��С��ǽֽ�����������ʾ)�������         
#define PHOTO_MAX_FILES                 (30)

// Ф����ƬĿ¼
#define MP_CONTPIC_DIR                  (AEE_SHARED_DIR"/cont")

// Ф��ͼƬ��չ��
#define SUFFIX_PIC                      (".bmp")

#define CONTAPP_APPREADY_MS             (400)

#if defined(FEATURE_DISP_160X128)
#define SEARCH_INPUTBOX_GAP             (6)
#define SEARCH_IMEICON_WIDTH            (28)
#elif defined(FEATURE_DISP_220X176)
#define SEARCH_INPUTBOX_GAP             (6)
#define SEARCH_IMEICON_WIDTH            (38)
#else
#define SEARCH_INPUTBOX_GAP             (6)
#define SEARCH_IMEICON_WIDTH            (28)

#endif
/*==============================================================================
                                 �궨��
==============================================================================*/
// ������ײ���ʾ���궨��
#define CONTAPP_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}

// menu auto scroll
#define MENU_AUTO_SCROLL(p, e, w)   //MenuAutoScroll(p, e, w)

// Select menu common functions
#define MENU_INIT_SELICONS(p)       InitMenuIcons(p);
#define MENU_SET_SELICON(p, i, b)   SetMenuIcon(p, i, b)

#define MENU_SET_COMICON(p)         

// Call back macros
#define CONTAPP_RESUMECB(p)         ISHELL_Resume(pMe->m_pShell, p)

#define FREE_AEEAddrField(cf)           \
        if ((cf).pBuffer != NULL)       \
        {                               \
            FREE((cf).pBuffer);         \
            (cf).pBuffer = NULL;        \
        }

// Current list menu items is zero
#define IS_ZERO_REC()    (0 == IVector_Size(pMe->m_pAddList))

// Indicate whether it is RUIM record
#define IS_RUIM_REC(n)   (n >= CONTAPP_RUIM_START_IDX)

// Indicate whether it is number field
#define IS_NUM_FIELD(n)  (n>=AEE_ADDRFIELD_PHONE_WORK       \
                          && n<= AEE_ADDRFIELD_PHONE_OTHER)

// Indicate whether it is addnew phone record
#define IS_ADDNEW_PHONE_REC(n,m)  (n== STATE_ADDNEW_INPUT       \
                          && m== CONTCFG_SAVETYPE_PHONE)

// Indicate whether it is not name generic fld
#define ISNT_NAME_GENERIC_RING_FLD()  ((pMe->m_nFldInputID != AEE_ADDRFIELD_NAME)       \
                          &&( pMe->m_nFldInputID != AEE_ADDRFIELD_RINGTONE))

// Indicate whether it is not name generic fld
#define IS_VALID_RINGTONE()  (pMe->m_nRingToneID != -1) 
                          
// Indicate whether it is not name generic fld
#define IS_NO_PHONENUMBER()  ((pMe->m_pAddNewMobile == NULL)	\
                          &&( pMe->m_pAddNewHome == NULL)              \
                          &&( pMe->m_pAddNewOffice == NULL)              \
                          &&(pMe->m_pAddNewFax == NULL))
	                         
// Is it the max capacity?
#define IS_MAX_REC(p)    (IADDRBOOK_GetNumRecs(p) >= IADDRBOOK_GetCapacity(p))

// Is it the max fields ?
#define IS_MAX_FLDS(p, f)         \
        (CContApp_GetCurrFldNum(p, f) >= CContApp_GetFldMaxNum(f))

#define FORCE_COMPLETE_COPY()  (pMe->m_bForceComplete = TRUE)
#define FORCE_COMPLETE_DELETE()  (pMe->m_bForceComplete = TRUE)

// Options menu select
#define PUSH_OPTSMENU_SEL(w)   (w = (uint16)pMe->m_wSelectOpts)
#define POP_OPTSMENU_SEL(w)    (pMe->m_wSelectOpts = (uint16)w)

// list menu select
#define PUSH_LISTMENU_SEL(w)   (w = (uint16)pMe->m_wSelectCont)
#define POP_LISTMENU_SEL(w)    (pMe->m_wSelectCont = (uint16)w)

// list menu build index
#define PUSH_LISTMENU_IDX(w)   (w = (uint16)pMe->m_nCurrIdx)
#define POP_LISTMENU_IDX(w)    (pMe->m_nCurrIdx = (uint16)w)
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*--------------------------������Ϣ��Ŀ---------------------------*/
// ������ CFG = Config
typedef enum _ContAppCFG
{
    CONTCFG_LOCKED,
    CONTCFG_VIEWTYPE,
    CONTCFG_SAVETYPE,
    CONTCFG_PASSWORD,
    CONTCFG_SMARTDIAL,
    CONTCFG_ONEDIAL1,
    CONTCFG_ONEDIAL2,
    CONTCFG_ONEDIAL3,
    CONTCFG_ONEDIAL4,
    CONTCFG_ONEDIAL5,
    CONTCFG_ONEDIAL6,
    CONTCFG_ONEDIAL7,
    CONTCFG_ONEDIAL8,
    CONTCFG_ONEDIAL9,
    CONTCFG_GROUP1,
    CONTCFG_GROUP2,
    CONTCFG_GROUP3,
    CONTCFG_GROUP4,
    CONTCFG_GROUP5,
    CONTCFG_GROUP6, 
    CONTCFG_GROUP7, 
#ifdef FEATURE_LANG_CHINESE
    CONTTCFG_QUICKSEARCH_INPUT_MODE,
#endif
    
    CONTCFG_LAST
}ContAppCFG;

typedef enum _ContAppCopyMoveType
{
    COPYALLTOUIM,
    COPYALLTOPHONE,
    MOVEALLTOUIM,
    MOVEALLTOPHONE,
    COPYMULTIPE,
    MOVEMULTIPE,
    COPYMULTIPE_TOUIM,
    COPYMULTIPE_TOPHONE,
    MOVEMULTIPE_TOPHONE,
    MOVEMULTIPE_TOUIM,
    SINGLECOPY,
    SINGLEMOVE
}ContAppCopyMoveType;

typedef enum _InputMode
{
    LOCAL_NUMBER_INPUT,  //�ڵ�ǰλ�ñ༭
    OPT_TEXT_INPUT,  //���������text�ؼ��༭
    EDIT_MENU_MODE,  //�˵�ģʽ
    EDIT_GROUP        
}Input_mode;

typedef enum _AddorEdit
{
    ADDOREDIT_ADD,  //�ڵ�ǰλ�ñ༭
    ADDOREDIT_EDIT  //���������text�ؼ��༭
}AddorEdit;

typedef enum _ContAppDeleteType
{
    DELETE_ALL,
    DELETE_PHONE,
    DELETE_UIMCARD,
    DELETE_MULTIPE
}ContAppDeleteType;

typedef enum _ContAppCopyErrType
{
    FIRST_VALUE, //didn't use
    PHONE_FULL,
    UIMCARD_FULL,
    OTHER_ERR
}ContAppCopyErrType;

typedef enum _ContAppAddFldType
{
    ADDFLDTOPHONE,
    ADDFLDTORUIMCARD
}ContAppAddFldType;

typedef enum _ContAppListFrom
{
    LISTFROMNONE,// list have not started
    LISTFROMNOMAL,//list startes from nomal 
    LISTFROMMAINMENU, // list start from mainmenu
    LISTFROMSEARCH,    // list start from search
    LISTFROMGROUP,       // list start from group
    LISTFROMONEDIAL
}ContAppListFrom;

typedef struct _ContAppOneDialFldPair
{
    ContAppCFG  oneDialIndx;
    int16       fldindx;
    boolean     bMask;
}ContAppOneDialFldPair;


typedef enum _ContAppFieldOptType
{
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
    FIELDSENDSMS,
    FIELDSENDCONTACT,
#endif    
    FIELDCALL,
    FIELDIPCALL
}ContAppFiledOptType;

typedef enum _ContAppSmartStateType
{
    SMART_STATE_IDD_LIST,
    SMART_STATE_IDD_SELECT,
    SMART_STATE_IDD_COPY,
    SMART_STATE_IDD_DELETE,
    SMART_STATE_IDD_SELECT_RECORD
}ContAppSmartStateType;

    
// ������Cache
typedef struct _ContAppCFGCache
{
    AECHAR      wPassWord[MAX_LEN_PASSWORD+2];
    boolean     bLocked;
    byte        nViewType;
    byte        nSaveType;
    boolean     bSmartDial;
    AECHAR      wOneDial1[MAX_INPUT_NUM + 1];
    AECHAR      wOneDial2[MAX_INPUT_NUM + 1];
    AECHAR      wOneDial3[MAX_INPUT_NUM + 1];
    AECHAR      wOneDial4[MAX_INPUT_NUM + 1];
    AECHAR      wOneDial5[MAX_INPUT_NUM + 1];
    AECHAR      wOneDial6[MAX_INPUT_NUM + 1];
    AECHAR      wOneDial7[MAX_INPUT_NUM + 1];
    AECHAR      wOneDial8[MAX_INPUT_NUM + 1];
    AECHAR      wOneDial9[MAX_INPUT_NUM + 1];
    AECHAR      wGroup1[MAX_INPUT_NAME_EN + 1];
    AECHAR      wGroup2[MAX_INPUT_NAME_EN + 1];
    AECHAR      wGroup3[MAX_INPUT_NAME_EN + 1];
    AECHAR      wGroup4[MAX_INPUT_NAME_EN + 1];
    AECHAR      wGroup5[MAX_INPUT_NAME_EN + 1];
    AECHAR      wGroup6[MAX_INPUT_NAME_EN + 1];    
    AECHAR      wGroup7[MAX_INPUT_NAME_EN + 1];
#ifdef FEATURE_LANG_CHINESE
    byte inputMode;
#endif
}ContAppCFGCache;

/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum _DLGRetValue
{
    // ��ʼֵ��������Ҫ�����Ի���
    DLGRET_CREATE,
    DLGRET_MSGBOX_OK,
    DLGRET_YES,
    DLGRET_NO,
    DLGRET_ERR,
    DLGRET_VIEW,
    DLGRET_FIND,
    DLGRET_GROUPVIEW,
    DLGRET_DELETE,
    DLGRET_DELETE_SELECT,
    DLGRET_PHONE_MEMORY, //WUQUQN.TANG 20081014 ADD
    DLGRET_UIM_CARD, //WUQUQN.TANG 20081014 ADD
    DLGRET_DELETE_ALL, //WUQUQN.TANG 20081014 ADD
    DLGRET_EDIT,
    DLGRET_POPNUMFLD,
    DLGRET_SELECT,
    DLGRET_SELECT_FLD,
    DLGRET_MARKCHG,
#ifdef FEATURE_RUIM_PHONEBOOK
    DLGRET_COPYMOVE,
    DLGRET_ALLCOPYMOVE,
    DLGRET_COPY,
    DLGRET_CONTACTS_MANAGEMENT,
    DLGRET_COPYORMOVE,
    DLGRET_CONTACT_EDIT,
#endif /* FEATURE_RUIM_PHONEBOOK */
    DLGRET_ADDNEW,
    DLGRET_CHECKCAPACITY,
    DLGRET_SETTING,
    DLGRET_HELP,
    DLGRET_SETTYPE,
    DLGRET_INPUT_NULL,
    DLGRET_NUM_UNAVAILD,
    DLGRET_SAME_REC,
    DLGRET_SELECTCAT,
    DLGRET_SELBIRTHDAY,
    DLGRET_SETICON,
//    DLGRET_SETRING,
#ifdef FEATURE_RUIM_PHONEBOOK
    DLGRET_MOVE,
    DLGRET_TORUIM,
    DLGRET_TOPHONE,
    DLGRET_SAVETO,
    DLGRET_VIEWTYPE,
#endif /* FEATURE_RUIM_PHONEBOOK */
    DLGRET_SETLOCK,
    DLGRET_ONEDIAL,
    DLGRET_CALL,
    DLGRET_SET,
    DLGRET_CLR,
    DLGRET_SENDSMS,
    DLGRET_IPCALL,
    DLGRET_SENDDIRECTORY,
    DLGRET_SEARCHOTHERDIRECTORY,
    DLGRET_DIRECTORTLIST,
    DLGRET_SEARCH,
    DLGRET_SEARCH_NAME,
    DLGRET_SEARCH_GROUP,
    DLGRET_SELECT_SINGLE_NUMBER,
    // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
    DLGRET_OK,
    DLGRET_CANCELED,
    DLGRET_DETAIL,
    DLGRET_EMAIL_UNAVAILD,
    DLGRET_FULL,
    DLGRET_SENDLISTFULL,
    DLGRET_NO_SELECTION,
    DLGRET_NUMBER_TOOLONG,
#if defined(FEATURE_SUPPORT_BT_APP) && defined(FEATURE_SUPPORT_VCARD)
    DLGRET_SEND_BY_BT,
#endif
#ifdef FEATURE_GIVEUP_EDIT
    DLGRET_GIVEUP_EDIT,
#endif
} DLGRetValue;

/*----------------------״̬�����������������---------------------*/
// ContApp Applet ״̬��״̬��
typedef enum _FSMState
{
    STATE_NONE,			//0x0
    STATE_INIT,			//0x1
    STATE_LOAD,			//0x2
    STATE_LOCKED,		//0x3
    STATE_MAINLIST,		//0x4
    STATE_MAINMENU ,      //0x5
    STATE_OPTS,			//0x6
    STATE_VIEW,			//0x7
    STATE_FIND,			//0x8
    STATE_FIND_LIST,		//0x9
    STATE_FIND_OPTS,	//0x10
    STATE_GROUPVIEW,	//0x11
    STATE_GROUPVIEW_LIST,	//0x12
    STATE_GROUPVIEW_OPTS,	//0x13
    STATE_ADDNEW,			//0x14
    STATE_ADDNEW_INPUT,    //0x15
    STATE_SELECTGROUP,	//0x16
#ifdef FEATURE_RUIM_PHONEBOOK
    STATE_SELECTPOS,		//0x17
#endif /* FEATURE_RUIM_PHONEBOOK */
    STATE_EDIT,				//0x18
    STATE_EDIT_INPUT,		//0x19
    STATE_EDIT_SELECTGROUP,	//0x20
//    STATE_EDIT_RING,	
    STATE_DELETE,	//0x21
#ifdef FEATURE_RUIM_PHONEBOOK
    STATE_COPYMOVE,	//0x22
    STATE_COPYMOVEALL,	//0x23
    STATE_COPY,	//0x24
    STATE_SETTING,	//0x25
    STATE_SAVETO,	//0x26
    STATE_VIEWTYPE,	//0x27
#endif /* FEATURE_RUIM_PHONEBOOK*/
    STATE_CAPACITY,		//0x28
    STATE_ONEDIAL,	//0x29
    STATE_ONEDIAL_OPT,//0x30
    STATE_ONEDIAL_SET,//0x31
    STATE_ONEDIAL_SET_NUMFLDSEL,//0x32
    STATE_SAVEFLD,//0x33
    STATE_SAVEFLD_LIST,//0x34
    STATE_SELECT,//0x35
    STATE_SELECTFLD_VIEW,//0x36
    STATE_SELECT_RETURN,//0x37
    STATE_FINDNUM,//0x38
    STATE_FINDNUM_OPTS, //0x39
    STATE_SMARTCALL, //0x40
    STATE_SMARTCALL_VIEW,	//0x41
    STATE_SEARCH,//0x42
    STATE_SEARCH_NAME,//0x43
    STATE_DETAIL,//0x44
    STATE_SUCCESS, //0x45
    STATE_MSG, //0x46
    STATE_ERROR, //0x47
    STATE_EXIT,  //0x48
    STATE_COPYING,
    STATE_GROUPOPT,
    STATE_GROUPOPT_EDIT,
    STATE_MANAGEMENT,
    STATE_DELETE_SELECT,
    STATE_POPNUMFLD,
    STATE_SELECTOPT,
    STATE_NUMFLDVIEW,
    STATE_DELETING,
    STATE_SELECT_RECORD,
    STATE_DETAIL_MULTI
} FSMState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum _NextFSMAction
{
    NFSMACTION_WAIT,
    NFSMACTION_CONTINUE
} NextFSMAction;


// ContApp Applet����ṹ�壺
typedef struct _CContApp
{
    DECLARE_VTBL(IContApp)
    uint32              m_nRefs;          // �������ü�����
    IModule            *m_pModule;
    IDisplay           *m_pDisplay;
    IShell             *m_pShell;
    AEERect             m_rc;
    int                 m_nLineHeight;    // ��ǰ���и�
    
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog            *m_pActiveDlg;
    
    // ��ǰ��Ի���ID
    uint16              m_pActiveDlgID;
    
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRetValue         m_eDlgRet;
    
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean             m_bNotOverwriteDlgRet;
    
    // Applet ǰһ״̬
    FSMState            m_ePreState;
    
    // Applet ��ǰ״̬
    FSMState            m_eCurState;
    
    // Applet ����״̬
    FSMState            m_eOptsRetState;
    
    // Applet ����״̬
    FSMState            m_eSuccessRetState;
    
    // Applet ����״̬
    FSMState            m_eMsgRetState;

    // Applet ����״̬
    FSMState            m_eBirthdayRetState;

    // Applet ����״̬
    FSMState            m_eEditRetState;
    
    // Applet�Ƿ��ڹ���״̬
    boolean             m_bSuspending;
    
    // Applet�Ƿ��ڻ״̬
    boolean             m_bActive;
    
    /////////////
    // Instance
    /////
    IFileMgr           *m_pFileMgr;       // IFileMgr interface
    IAddrBook          *m_pAddrPhone;     // IADDRBOOK interface for phone
#ifdef FEATURE_RUIM_PHONEBOOK
    IAddrBook          *m_pAddrRUIM;      // IADDRBOOK interface for RUIM
#endif /* FEATURE_RUIM_PHONEBOOK */
    ICallApp         *m_pCallApp;     // Reference UI call instance
    IAddrRec           *m_pCurrRec;       // Current record
    boolean             m_bIsPhoneRec;    // Indicate if m_pCurrRec is Phone record
    
    // ��ǰ�鿴�ĵ绰����¼ Cache �б�
    IVector            *m_pAddList;

    // IVector interfaces that holds a list of AEEAddrField records
    IVector            *m_pFldIv;
    
    // ͨ���ַ���Buffer
    AECHAR              m_szBuf[MAX_LEN_COMMBUF+1];
    AECHAR              m_szFmtBuf[MAX_LEN_COMMBUF+1];
    
    // ָ��MessageBox��Ϣ��ָ�룬��̬�������ͷ�
    AECHAR             *m_pMsgBox_Msg;
    
    // Applet start type
    AppletStartMethod   m_eStartMethod;
    
    // Applet config cache
    ContAppCFGCache     m_sCFGCache;
    
    // Search data
    AECHAR             *m_pSrchData;
    uint16              m_wSearchCont;//��ǰѡ�еļ�¼
    
    // Error resource ID
    uint16              m_wErrStrID;
    
    // message resource ID
    uint16              m_wMsgResID;
    
    // List menu select index
    uint16              m_wSelectCont;//��ǰѡ�еļ�¼
    uint16              m_wSelectFld; //��ǰ��¼��ѡ���ֶ�
    uint16              m_wEditCont;  //��ǰ�༭�ļ�¼
    int                 m_nCurrIdx;
    uint32              m_nEnumIdx;
    int                 m_nCurrMenuIdx;
    // menu select item
    uint16              m_wSelectOpts;
    uint16              m_wSelectFldOpts;
    uint16              m_wSelectSetting;
    uint16              m_wSelectEdit;
    uint16              m_wSelectOneDial;
    uint16              m_wSelectPhoto;
    uint16              m_wSelectStore;
    uint16              m_wSelectCopyMove;
    uint16              m_wDeleteSelectSel;
    uint16              m_wPositionSel;
    uint16              m_wMainMenuSel;
    
    // Options menu state select store
    uint16              m_wOptsStatSel;
    uint16              m_wFindOptsSel;
    uint16              m_wGroupOptsSel;
    
    // list menu state select store
    uint16              m_wManagementSel;
    uint16              m_wMainListSel;
    uint16              m_wOneDialStatSel;
    uint16              m_wGroupViewSel;
    uint16              m_wFindListSel;
    uint16              m_wDeleteListSel;
    
    // list menu current index store
    uint16              m_wMainListIdx;
    uint16              m_wOneDialStatIdx;
    uint16              m_wGroupViewIdx;
    uint16              m_wFindListIdx;
    uint16              m_wDeleteListIdx;
    
    
    // List view type
    byte                m_nViewType;
    byte                m_nSaveType;
    uint16              m_wPosCmd;
    
    // ���ܲ���ʹ�õ����ݡ����д洢�ŵ�ǰ��ͷ������ƥ�������
    uint32             *m_pSmartFindBuf;//��Ҫ��ʾ��ƥ���¼������
    uint32              m_pSmartBufLen;//�ҵ���ƥ��ļ�¼��Ŀ
    int                 m_nCurrSmartIdx;//��ǰ�˵��ĵ�һ����¼
    uint16              m_wSelectSmart;//��ǰѡ�еĲ˵�
    AECHAR              *m_szAlpha;//���ܲ��������Ͻ���ʾ���ַ���
    boolean             m_bInSmartFind;//������Ͻ����ַ���,�������ܲ���ģʽ
    boolean             m_bInsmartnofind;//mark it when don't find the records
    AECHAR              m_lastAlpha[MAX_INPUT_NAME_EN+1];//remember the last alpha when don't find the key
    AECHAR              *m_sSearchStr; // �������������ַ���
    
    // Field input type
    AEEAddrFieldID      m_nFldInputID;
    AECHAR             *m_pFldInputBuf;
    
    AECHAR             *m_pAddNewName;
    AECHAR             *m_pAddNewMobile;
    AECHAR             *m_pAddNewHome;
    AECHAR             *m_pAddNewOffice;
    AECHAR             *m_pAddNewFax;    
    AECHAR             *m_pAddNewEMAIL;
    AECHAR             *m_pAddNewAddress;
    AECHAR             *m_pAddNewURL;
    AECHAR             *m_pAddNewRemark;
    AECHAR             *m_nRingToneID;
    AEEAddrCat          m_nGroupCat;
    uint16                   m_wFieldIndex;
    uint16                   m_wFieldCount;
    dword                   m_wFieldMask;
    uint16                   m_wKeyType;

    AEEAddrFieldID      m_nAddNewFldID;
    AEEAddrFieldID      m_nNumFldType;  //�ֶ�����
    int16               m_wAddnewFocus;
    uint16              m_wFiledOptState;
    
    // For find data
    AEEAddrCat          m_wFindCat;
    AEEAddrFieldID      m_wFindFldID;
    void               *m_pFindData;
    
    // Copy all need variable
    uint16              m_wCopyRecCnt;
    uint16              m_wCopyStartID;
    uint16              m_wCopyCurrID;
    uint16              m_wTotalNum;
    AEECallback         m_nCopyCB;
    boolean             m_bCopyInited;
    boolean             m_bForceComplete;
    int                 m_nFailNum;
#ifdef FEATURE_RUIM_PHONEBOOK
    ContAppCopyMoveType             m_nCopyMoveType;
    ContAppCopyMoveType             m_nCopyOrMove;
#endif /* FEATURE_RUIM_PHONEBOOK */
    // For onoff dialog
    boolean             m_bOnOff;
    
    // For IContApp interface
    //Phone number save type. (see enum PhoneNumberSaveType).
    //will be used in case start  method is save number.
    //This indicates whether the number should be added to the new contact
    // or existing contact or RUIM.
    PhoneNumberSaveType m_eFldSaveType;
    //AECHAR             *m_pSaveEMAIL;
//    AECHAR             *m_pSaveURL;
    
    // Select field type id
    SelectFldType       m_wSelFldType;
    
    // Callback function used with IContApp_SelectField()
    PFNSELECTEDCONTACTCB    m_pSelFldCB;
    
    // Callback function used with vcard select
    PFNNOTIFY           m_pNotify;
    
    // for mailer call back buffer
    AECHAR                m_pBuf[MAX_LEN_WMS_BUF +1];
    
    // For find number used
    AECHAR             *m_pFindNum;
    
    // For store the file list
    AECHAR              m_wFileList[PHOTO_MAX_FILES][AEE_MAX_FILE_NAME];
    int                 m_nFileNum;
    
    // AECHAR for controls title
    AECHAR              m_pTitle[MAX_LEN_TITLE+1];

    IConfig            *m_pConfig;
    boolean             m_bAppReady;

    AECHAR              m_pOneDialBuf[MAX_INPUT_NUM + 1];
    AECHAR              *m_pOneDial;
    AECHAR              m_tmpBuf[MAX_INPUT_NUM + 1];
    boolean       m_bUnLock;
    uint16              m_wSelectGroup;

    byte                m_nSmartStateType;
    IRingerMgr      *m_pRingerMgr;

    // Delete need variable
    uint16              m_wDeleteRecCnt;
    boolean              m_bDeleteInited;
    AEECallback         m_nDeleteCB;
    uint16              m_wDeleteCurrID;   

    //Delete need end 
    ContAppDeleteType  m_nDeleteType;
    ITextCtl            *pText;
    Input_mode     m_nInputMode;
    AddorEdit     m_nAddnewOrEdit;
    int16           m_wselGroupId;
    int16           m_wNewNumFldMask;
    boolean         m_bInputNotComplete;
    boolean         m_nDeleteAllCheck;
    IBitmap * m_eBitmap;
    ContAppCopyErrType      m_eErrType;
    int16           m_eTotalSelectNum;
    ContAppAddFldType            m_eAddFldType;
    ContAppOneDialFldPair        m_eOneDialFld[CONTAPP_NUMBERFLDS];
    ContAppListFrom                         m_eListFrom;    // ����mainmenu ->add �� mainmenu -> contacts ->opt -> add
    //boolean m_bAllowSelChange;                              // ��ֹ�����ö�ѡ�Ĵ򹴵��� IMENUCTL_SetSelEx�������û��Ҫ�� EVT_CTL_SEL_CHANGED �¼���
    boolean m_bIsNotComplete;                                  // �ж� doing�¼��Ƿ��Ѿ����� ���� ���жϡ�
    PromptMsg_e_Type m_eMsgType;
    uint16 m_wDelReturnSelId;                                      // ɾ��һ����¼�󣬷���list��ѡ�е�id
    boolean m_bDelOk;                                          // ɾ��һ����¼�ɹ�
    int m_nleftNum;
#ifdef FEATURE_GIVEUP_EDIT
	boolean m_bIsEndKey;
#endif
#ifdef FEATURE_LANG_CHINESE
    uint16 m_nInputModeTable[2];                                   // only support two input mode: AEE_TM_LETTERS and AEE_TM_NUMBERS
    byte  m_nCurrentInputMode;                                  
    boolean m_bIsEndKey;
    
#endif
    char  *m_strPhonePWD;
    IAnnunciator *m_pIAnn;
	IRUIM        *m_pIRUIM;
	boolean      m_isdel;
} CContApp;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
//For BREW3.0 support the muti-language re-define the RES maco
#define CONTAPP_RES_FILE_LANG  AEE_RES_LANGDIR CONTACTAPP_RES_FILE

// �û����¸��½����¼�
#define EVT_USER_DIALOG_TIMEOUT   (EVT_USER+11)
#define EVT_USER_COMPLETE         (EVT_USER+12)
#define EVT_USER_ERR              (EVT_USER+13)
#define EVT_USER_MAX              (EVT_USER+14)

// �رնԻ����
#define CLOSE_DIALOG(DlgRet)                         \
        {                                            \
            pMe->m_eDlgRet = DlgRet;                 \
            (void) ISHELL_EndDialog(pMe->m_pShell);  \
        }

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState)                 \
        {                                        \
            FSMState tpState;                    \
            tpState = nextState;                 \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;          \
        }

//
// Conditional Debug Output
//
#define FARF_STATE            1
#define FARF_EVENT            1
#define FARF_ADDR             1

#ifdef FARF
#undef FARF
#endif

#define FARF(x, p)

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
       CContApp_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��ContApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       SUCCESS: �����Ի���ɹ�
       ��SUCCESS: �����Ի���ʧ��

��ע:
       

==============================================================================*/
int CContApp_ShowDialog(CContApp *pMe, uint16  dlgResId);

/*==============================================================================
����:
    CContApp_ShowMsgBox

˵��:
    ������״̬����������������Ϣ�Ի�����ʾ��Ϣ��ʾ�û���

����:
    pMe [in]: ָ�� Contact Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wTextResId [in]: ��Ϣ�ı���Դ ID��

����ֵ:
    none

��ע:

==============================================================================*/
int CContApp_ShowMsgBox(CContApp *pMe, uint16 wTextResId);
                             
/*==============================================================================
����:
       CContApp_ShowYesNoDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��CContApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       msgResId [in]���Ի����ַ�����ԴID���������ݸ�ID��ʾ��ʾ���ݡ�
       msg : ָ����Ҫ��ʾ���ַ�����������msgResId
       msglen: ��ʵ���ַ�������wstrlen����.-1 Auto caculate the lenght

����ֵ:
       int : SUCCESS if create dialog

��ע:
       

==============================================================================*/
int  CContApp_ShowYesNoDialog( CContApp   *pMe, 
                               uint16      msgResId ,
                               AECHAR     *msg, 
                               int         msglen );
                               
/*==============================================================================
����:
       CContApp_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��ContApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean CContApp_RouteDialogEvent( CContApp  *pMe,
                                   AEEEvent   eCode,
                                   uint16     wParam,
                                   uint32     dwParam);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       CContApp_ProcessState
˵��:
       ContApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��ContApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction CContApp_ProcessState(CContApp *pMe);

/*==============================================================================
����: 
       CContApp_GetConfig
       
˵��: 
       ��ȡConfig��Ŀ��ֵ
       
����: 
       pMe       [in] :ָ��ContApp Applet����ṹ��ָ�롣
       eCFG      [in] :��ȡ��������
       data      [out]:���ݴ洢��
       dataLen   [in] :���ݴ洢���Ĵ�С,in bytes
       
����ֵ:
       SUCCESS : �����ɹ�
       EFAILED : ����ʧ��
       EBADPARM: ��������
       
��ע:
       
==============================================================================*/
int CContApp_GetConfig( CContApp        *pMe, 
                        ContAppCFG       eCFG, 
                        void            *data,
                        int              dataLen);
                       
/*==============================================================================
����: 
       CContApp_SetConfig
       
˵��: 
       ����Config��Ŀ��ֵ
       
����: 
       pMe       [in] :ָ��ContApp Applet����ṹ��ָ�롣
       eCFG      [in] :��ȡ��������
       data      [out]:���ݴ洢��
       dataLen   [in] :���ݴ洢���Ĵ�С,in bytes
       
����ֵ:
       SUCCESS : �����ɹ�
       EFAILED : ����ʧ��
       EBADPARM: ��������
       
��ע:
       
==============================================================================*/
int CContApp_SetConfig(CContApp        *pMe, 
                       ContAppCFG       eCFG, 
                       void            *data,
                       int              dataLen);
                       
/*=============================================================================

FUNCTION:CContApp_LoadPhoneBook

DESCRIPTION:
    ����绰����¼
    
PARAMETERS:
    pMe       :
    nViewType : Load the record from where
    
RETURN VALUE:
    SUCCESS 
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int CContApp_LoadPhoneBook(CContApp *pMe, byte nViewType);

/*=============================================================================

FUNCTION:CContApp_LoadbySmart

DESCRIPTION:
    �������ܲ��ҵļ�¼���绰��IVector�ӿ�
    
PARAMETERS:
    pMe   :
    pStr  : search data Alpha string '*' is wildcard
    
RETURN VALUE:
    SUCCESS 
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int CContApp_LoadbySmart(CContApp *pMe, AECHAR *pStr);

/*=============================================================================

FUNCTION:CContApp_FreeAddrFlds

DESCRIPTION: 
    �ͷŴ洢��ǰ��¼�ֶ���Ϣ��IVector�ӿ� m_pFldIv �Ŀռ�

PARAMETERS:
   pMe:

RETURN VALUE:
    None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:
=============================================================================*/
void CContApp_FreeAddrFlds(CContApp *pMe);

/*=============================================================================

FUNCTION: CContApp_SaveField

DESCRIPTION:
    Saves the field info into the
    IAddrRec at pMe->m_pFldIv[idx]
   
PARAMETERS:
    pMe    : pointer to CContApp object
    idx    : index of field in m_pFldIv
    wContID: Contact ID of saved
    wFldID : type of field
    pData  : data of field
    
RETURN VALUE:
    SUCCESS
   
COMMENTS:

SIDE EFFECTS:
    pMe->m_pFldIv
    
SEE ALSO:
=============================================================================*/
int CContApp_SaveField( CContApp       *pMe,
                        uint16          wContID,
                        uint16          idx,
                        AEEAddrFieldID  wFldID,
                        void           *pData);

/*=============================================================================

FUNCTION: CContApp_DeletePhoneFld

DESCRIPTION:
    Deletes the field pMe->m_fieldSelId from the current record in phone
    
PARAMETERS:
    pMe    : pointer to CContApp object
    wContID: contact id of delete field
    idx    : delete index in m_pFldIv
    
RETURN VALUE:
    boolean: TRUE if successful otherwise false
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:
=============================================================================*/
int CContApp_DeletePhoneFld(CContApp *pMe, uint16 wContID, uint16 idx);

/*=============================================================================

FUNCTION:CContApp_GetFldBuf

DESCRIPTION:
    ��ȡ�ƶ��ֶε� Field ����Buffer
    
PARAMETERS:
    pMe     :
    idx     : ѡ���ֶε� index
    
RETURN VALUE:
    SUCCESS: 
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
void *CContApp_GetFldBuf(CContApp *pMe, int idx);

/*=============================================================================

FUNCTION:CContApp_LoadAddrFlds

DESCRIPTION:
    ����¼���ֶ��������뵽 m_pFldIv ��
    
PARAMETERS:
    pMe   :
    wRecID: The ID suit of the OEM
    eFldType: ������ֶ����ͣ���������Ч SELECT_ALL ����ȫ��
    
RETURN VALUE:
    SUCCESS:
    
COMMENTS:
    
SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int CContApp_LoadAddrFlds( CContApp      *pMe,
                           uint16         wContID,
                           SelectFldType  eFldType);

/*=============================================================================

FUNCTION: CContApp_DeleteCont

DESCRIPTION:
    Deletes the record according the cont ID
    
PARAMETERS:
    pMe     : pointer to CContApp object
    wContID : the cont id deleted
    
RETURN VALUE:
    SUCCESS
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:
=============================================================================*/
int CContApp_DeleteCont(CContApp *pMe, uint16 wContID);

/*=============================================================================

FUNCTION:CContApp_GetFldID

DESCRIPTION:
    ��ȡ�ƶ��ֶε� Field ID ����
    
PARAMETERS:
    pMe     :
    idx     : ѡ���ֶε� index
        
RETURN VALUE:
    SUCCESS: 
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
AEEAddrFieldID CContApp_GetFldID(CContApp *pMe, int idx);
                        
/*=============================================================================
FUNCTION: CContApp_AddField

DESCRIPTION:
    ����һ���ֶ�
    This function will add a new field to the current contact.
   
PARAMETERS:
    pMe    : pointer to CContApp object
    wContID: contact id of add field
    wFldID : type of field
    pData  : data of field
    
RETURN VALUE:
    SUCCESS  : if successful otherwise EFAILED
    EBADPARM : if parameter is bad
    EEXIST   : if the content have exsit
    ENOMEMORY: if no memory
    EMAXFIELD: if reach the max field
   
COMMENTS:

SIDE EFFECTS:
    pMe->m_pFldIv
    
SEE ALSO:
=============================================================================*/
int CContApp_AddField( CContApp       *pMe,
                       uint16          wContID,
                       AEEAddrFieldID  wFldID,
                       void           *pData);

/*=============================================================================

FUNCTION:CContApp_GetBufbyFldID

DESCRIPTION:
    ��ȡָ���ֶε� Field ����Buffer
    
PARAMETERS:
    pMe     :
    wFldID  : ѡ���ֶε�field ID
    
RETURN VALUE:
    SUCCESS: 
    
COMMENTS:
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
void *CContApp_GetBufbyFldID(CContApp *pMe, AEEAddrFieldID wFldID);

/*=============================================================================

FUNCTION:CContApp_GetCurrFldNum

DESCRIPTION:
    ��ö�Ӧ Field �ĵ�ǰ����
    
PARAMETERS:
    pMe     :
    nFldID  : �ֶ�����
    
RETURN VALUE:
    The number of the fields
    
COMMENTS:
    
SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
uint16 CContApp_GetCurrFldNum(CContApp *pMe, AEEAddrFieldID wFldID);

/*=============================================================================

FUNCTION:CContApp_GetFldMaxNum

DESCRIPTION:
    ��ö�Ӧ Field �����ߴ�
    
PARAMETERS:
    nFldID  : �ֶ�����
    
RETURN VALUE:
    The max number of the fields
    
COMMENTS:
    
SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
uint16 CContApp_GetFldMaxNum(AEEAddrFieldID wFldID);

#ifdef FEATURE_RUIM_PHONEBOOK
/*=============================================================================

FUNCTION:CContApp_CopyField

DESCRIPTION: 
    ���Ƶ�ǰѡ����ֶε���һ�棨RUIM��¼���Ƶ��������������Ƶ�����
    �˺������Զ�ʶ���¼�Ĵ洢λ��

PARAMETERS:
    pMe    :
    wContID: the Cont ID of current record
    wFldIdx: the index of field that will be copied

RETURN VALUE:
    SUCCESS
    EMAXRECORD : target device is full
    EMAXFIELD  : Reach the max fields number
    EEXIST     : if field have exist
    EBADPARM   : if parameter is bad
    
COMMENTS:
    �˺������� m_pFldIv �ǿ��õ�
    
SIDE EFFECTS:
SEE ALSO:
=============================================================================*/
int CContApp_CopyField(CContApp *pMe, uint16 wContID, uint16 wFldIdx);
#endif /* FEATURE_RUIM_PHONEBOOK */

/*==============================================================================
����:
    CContApp_LoadByCondition

˵��:
    ���������趨�������������������ȫ���绰����¼ Cache ��

����:
    pMe [in]: ָ�� CContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wDataSize[in]: �������ݴ�С��
         
����ֵ:
    SUCCESS ���ɹ�
    ����ֵʧ��

��ע:
    ���� pMe->m_wFindCat, pMe->m_wFindFldID, pMe->m_pFindData ���ڵ��ñ�����ǰ
    ���ú�, pMe->m_pFindData �Ĵ�С�Բ��� wDataSize ���롣
    ������������ΧΪȫ���绰����¼(����),������ڶ��� pMe->m_pAddList

==============================================================================*/
int CContApp_LoadByCondition(CContApp *pMe, uint16 wDataSize);


/*=============================================================================

FUNCTION:CContApp_LoadByName

DESCRIPTION:
    ���������Ҽ�¼
    
PARAMETERS:
    pMe   :
    pName : search name data
    
RETURN VALUE:
    SUCCESS 
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int CContApp_LoadByName(CContApp *pMe, AECHAR *pName);

/*=============================================================================

FUNCTION:CContApp_LoadByCat

DESCRIPTION:
    ���������Ҽ�¼
    
PARAMETERS:
    pMe   :
    pName : search name data
    
RETURN VALUE:
    SUCCESS 
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int CContApp_LoadByCat(CContApp *pMe, AEEAddrCat wCategory);

/*=============================================================================

FUNCTION: CContApp_SetCat

DESCRIPTION:
    Set current select cont category
   
PARAMETERS:
    pMe    : pointer to CContApp object
    wContID: Contact ID of saved
    wCat   : category that be set
    
RETURN VALUE:
    SUCCESS
   
COMMENTS:

SIDE EFFECTS:
    
SEE ALSO:
=============================================================================*/
int CContApp_SetCat(CContApp *pMe, uint16 wContID, AEEAddrCat wCat);

/*=============================================================================

FUNCTION: CContApp_GetCat

DESCRIPTION:
    Get current select cont category
   
PARAMETERS:
    pMe    : pointer to CContApp object
    wContID: Contact ID of saved
    
RETURN VALUE:
    SUCCESS
   
COMMENTS:

SIDE EFFECTS:
    
SEE ALSO:
=============================================================================*/
AEEAddrCat CContApp_GetCat(CContApp *pMe, uint16 wContID);

/*=============================================================================

FUNCTION:CContApp_CopyAllToRUIM

DESCRIPTION: 
    ����ǰ�����ļ�¼ȫ�����Ƶ� RUIM ��

PARAMETERS:
    pMe    :
    wStartID: the Cont ID of Start copy
    
RETURN VALUE:
    SUCCESS   : 
    EMAXRECORD: if the target device is full
    
COMMENTS:
    
SIDE EFFECTS:
SEE ALSO:
=============================================================================*/
int CContApp_CopyAllToRUIM(CContApp *pMe, uint16 wStartID);

/*=============================================================================

FUNCTION:CContApp_CopyAllToPhone

DESCRIPTION: 
    ����ǰ�����ļ�¼ȫ�����Ƶ� RUIM ��

PARAMETERS:
    pMe    :
    wStartID: the Cont ID of Start copy
    
RETURN VALUE:
    SUCCESS   : 
    EMAXRECORD: if the target device is full
    
COMMENTS:
    
SIDE EFFECTS:
SEE ALSO:
=============================================================================*/
int CContApp_CopyAllToPhone(CContApp *pMe, uint16 wStartID);

/*=============================================================================

FUNCTION:CContApp_MergeField

DESCRIPTION: 
    �����ֶε�����

PARAMETERS:
    pMe     :
    pNameFld: name field
    pNumFld : number field

RETURN VALUE:
    SUCCESS   : 
    EMAXFIELD : if field type is full in current record
    EEXIST    : if field have exist
    EBADPARM  : if parameter is bad
    
COMMENTS:

SIDE EFFECTS:
SEE ALSO:
=============================================================================*/
int CContApp_MergeField( CContApp       *pMe, 
                         uint16          wRecID,
                         void           *pFldData,
                         AEEAddrFieldID  wFldID);

/*=============================================================================

FUNCTION:CContApp_LoadByNum

DESCRIPTION:
    ��������Ҽ�¼
    
PARAMETERS:
    pMe  :
    pNum :
        
RETURN VALUE:
    SUCCESS 
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int CContApp_LoadByNum(CContApp *pMe, AECHAR *pNum);

/*=============================================================================

FUNCTION:CContApp_MakeCall

DESCRIPTION:
    ��ָ�����ֶη������
    
PARAMETERS:
    pMe     :
    idx     : ѡ���ֶε� index
        
RETURN VALUE:
    SUCCESS: 
    
COMMENTS:
    This function only used in view dialog and.
    Assume the m_pFldIv is available.
    
SIDE EFFECTS:
    
SEE ALSO:

=============================================================================*/
int CContApp_MakeCall(CContApp *pMe, int idx);

/*=============================================================================

FUNCTION: WStr2Alpha

DESCRIPTION: 
    ���ַ���ת��Ϊ��Ӧ�İ����ַ���
    
PARAMETERS:
    pDest:
    nLen : Ŀ���ַ������� in AECHAR
    pSrc :
    
RETURN VALUE:
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:
=============================================================================*/
//void WStr2Alpha(AECHAR *pDest, int nLen , AECHAR *pSrc);

/*=============================================================================

FUNCTION: WStr2Letter

DESCRIPTION: 
    ���ַ���ת��Ϊ��Ӧ�� ASCII �ַ���
    
PARAMETERS:
    pDest:
    nLen : Ŀ���ַ������� in AECHAR
    pSrc :
    
RETURN VALUE:
    
COMMENTS:

SIDE EFFECTS:

SEE ALSO:
=============================================================================*/
//void WStr2Letter(AECHAR *pDest, int nLen , AECHAR *pSrc);

/*=============================================================================

FUNCTION:CContApp_LoadIconInfo

DESCRIPTION:
    ����Icon����Ϣ
    
PARAMETERS:
    pMe     :

RETURN VALUE:
    SUCCESS: 
    EFAILED: If the number of current cache is 0
    
COMMENTS:
    
SIDE EFFECTS:
    pMe->m_wSelectFld
    pMe->m_wSelectCont
    
SEE ALSO:

=============================================================================*/
int CContApp_LoadIconInfo(CContApp *pMe);

/*==============================================================================

����: CContApp_CmpRegionNumber
       
˵��: 
    ƥ����������
       
����: 
    pAddrRec: 
    pNum    : 
    
����ֵ:
    boolean
       
��ע:
       
==============================================================================*/
boolean  CContApp_CmpRegionNumber(IAddrRec *pAddrRec, AECHAR *pNumber);

/*==============================================================================

����: CContApp_BuildContInfo
       
˵��: 
    ���� Contact info �ṹ
       
����: 
    pAddrRec  : 
    pContInfo : 
    
����ֵ:
    None
       
��ע:
       
==============================================================================*/
void CContApp_BuildContInfo(IAddrRec *pAddrRec, ContInfo *pContInfo);

/*==============================================================================

����: CContApp_CmpNumber
       
˵��: 
    ��ȷƥ�����
       
����: 
    pAddrRec: 
    pNum    : 
    
����ֵ:
    boolean
       
��ע:
       
==============================================================================*/
boolean CContApp_CmpNumber(IAddrRec *pAddrRec, AECHAR *pNumber);

/*=============================================================================

FUNCTION:CContApp_DeleteMultipe

DESCRIPTION: 
    ����ǰ�����ļ�¼ȫ�����Ƶ� RUIM ��

PARAMETERS:
    pMe    :
    wStartID: the Cont ID of Start copy
    
RETURN VALUE:
    SUCCESS   : 
    EMAXRECORD: if the target device is full
    ENORECORD : if no record to copy
    
COMMENTS:
    
SIDE EFFECTS:
SEE ALSO:
=============================================================================*/
int CContApp_DeleteMultipe(CContApp *pMe);

/*=============================================================================

FUNCTION:CContApp_CopyMultipe

DESCRIPTION: 
    copy or move
    
PARAMETERS:
    pMe    :
    wStartID: the Cont ID of Start copy
    
RETURN VALUE:
    SUCCESS   : 
    EMAXRECORD: if the target device is full
    ENORECORD : if no record to copy
    
COMMENTS:
    
SIDE EFFECTS:
SEE ALSO:
=============================================================================*/

int CContApp_CopyMultipe(CContApp *pMe);
#endif// CONTAPP_PRIV_H

