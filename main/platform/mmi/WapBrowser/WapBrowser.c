/*==============================================================================
//           WapBrowser  A P P L E T  M O D U L E
// �ļ�:
//        WapBrowserApp.c
//        ��Ȩ����(c) 2011 WaterWorld Tech. CO.,LTD.
//        
// �ļ�˵����
//        �����ģ�����ļ�
//                 
//
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��       �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------    -----------------------------------------------

==============================================================================*/


/*==============================================================================

  ���ļ��������ⲿ�ļ�
  
==============================================================================*/
#include "WapBrowser_priv.h"      // ��Appletͷ�ļ�

/*==============================================================================

  �궨��ͳ���
  
==============================================================================*/

#define APP_ISHELL (((AEEApplet*)Brew_GetAppInstance())->m_pIShell)
//#define AEECLSID_NF3							(0x0100711E)
#define EVT_USER_BASE				(EVT_USER + 0x4100)
/* NetFront3 User Events */
#define EVT_NF3_BASE				(EVT_USER_BASE + 0x1100)
#define EVT_APP_RECEIVE_START		(EVT_NF3_BASE + 0x01)
/* NetFront3 Sub Events */
#define EVT_SUBEVT_BASE				(0x00)
#define EVT_RECEIVE_WAPPUSH			(EVT_SUBEVT_BASE + 0x11)


typedef struct _param tparam;

enum {
#if 1 /*APP_CONFIG_USE_EVENTLISTENER*/
	APP_ORIGINATOR_ALLAPPSELECT = -1,
#endif /*APP_CONFIG_USE_EVENTLISTENER*/
	APP_ORIGINATOR_TASK = 0,
	APP_ORIGINATOR_MANAGER,
	APP_ORIGINATOR_LAUNCHER,
	APP_ORIGINATOR_BROWSER,
	APP_ORIGINATOR_MAILER,
	APP_ORIGINATOR_DATAFOLDER,
	APP_ORIGINATOR_ADDRESSBOOK,
	APP_ORIGINATOR_MEDIAPLAYER,
	APP_ORIGINATOR_IrDA,
	APP_ORIGINATOR_BLUETOOTH,
	APP_ORIGINATOR_MMS,
	APP_ORIGINATOR_SISL,
	APP_ORIGINATOR_SISLDAEMON,
	APP_ORIGINATOR_NETMANAGER,
	APP_ORIGINATOR_MIMEVIEWER,
	APP_ORIGINATOR_CALENDAR,
	APP_ORIGINATOR_MMSDAEMON,
	APP_ORIGINATOR_WAPPUSHDAEMON,
	APP_ORIGINATOR_DOWNLOAD,
	APP_ORIGINATOR_JAVA,
	APP_ORIGINATOR_EVENTLISTENER,
	APP_ORIGINATOR_TELCALL,
	APP_ORIGINATOR_JAMSTUB,
	APP_ORIGINATOR_PDFVIEWER,
	APP_ORIGINATOR_DOCVIEWER,
#ifdef APP_CONFIG_USE_DOWNLOAD_DAEMON
	APP_ORIGINATOR_DOWNLOADDAEMON,
	APP_ORIGINATOR_DOWNLOADEXT,
#endif
	APP_ORIGINATORS
};
#define	APPLICATION_MSGBASE_SYSTEM			0x00000

enum{
	APP_BROWSER_START_MAINMENU = APPLICATION_MSGBASE_SYSTEM,
	APP_BROWSER_START_HOME,		/* arg: not use */
	APP_BROWSER_START_BY_MAINMENU, /* arg : slim_int IDM_BROWSER_XXX */
	APP_BROWSER_START_URL,			/* arg: TString url */
	APP_BROWSER_START_PAGESAVEINFO,	/* arg : TPageSaveInfo (slim_handle) */
	APP_BROWSER_START_BY_PUSH,			/* arg: TString url */
	APP_BROWSER_RESULT_SAVEFILE,	/* arg: TString file name */
#ifdef WIRELESS_BROWSER_CONFIG_USE_FILEUPLOAD
	APP_BROWSER_RESULT_FILESELECT,	/* arg: TString file name */
#endif
	APP_BROWSER_RESULT_ADDRESS_BOOK,	/* arg : TString add addressbook */
#ifdef WIRELESS_BROWSER_CONFIG_USE_VBOOKMARK
	APP_BROWSER_VBOOKMARK_IMPORT,				/* arg: TString file name */
#endif
	APP_BROWSER_REQUEST_BOOKMARKLIST_UPDATE,	/* arg: not use */
#ifdef APP_CONFIG_USE_DRM
	APP_BROWSER_START_RIGHTS,
	APP_BROWSER_START_RIGHTS_ONMEMORY,
#endif
#ifdef APP_CONFIG_USE_CASTING
	APP_BROWSER_START_URL_WITH_POST,
#endif
	APP_BROWSER_RES_INSTALL,
	APP_BROWSER_STARTS
};
enum {
	WIRELESS_MESSAGING_START_MAINMENU = APPLICATION_MSGBASE_SYSTEM,
	WIRELESS_MESSAGING_END_ADDRESS_BOOK,
	WIRELESS_MESSAGING_END_SELECT_FILE,
	WIRELESS_MESSAGING_EXTERNAL,
	WIRELESS_MESSAGING_MESSAGES
};
enum {
	WIRELESS_MAILER_START_MAINMENU = WIRELESS_MESSAGING_MESSAGES,
	WIRELESS_MAILER_START_RECEIVE,
	WIRELESS_MAILER_START_INBOX,
	WIRELESS_MAILER_START_OUTBOX,
	WIRELESS_MAILER_START_SENTMAIL,
	WIRELESS_MAILER_START_COMPOSER,	/* arg: TString mailt to url */
	WIRELESS_MAILER_START_ACCOUNT,
	WIRELESS_MAILER_START_OPTION_MENU,
	WIRELESS_MAILER_START_AUTO_RECEIVE,
	WIRELESS_MAILER_START_COMPOSER_ATTACH,
	WIRELESS_MAILER_START_OPEN_SETTING_SITE,
	WIRELESS_MAILER_START_NEWMAIL_CONFIRM,
	WIRELESS_MAILER_START_MAILBOX_CONFIRM,
	WIRELESS_MAILER_START_EMAIL_CANCEL_OVER,
	WIRELESS_MAILER_END_DAEMON,
	WIRELESS_MAILER_END_DATAFOLDER,
	WIRELESS_MAILER_END_CANCEL_BGRECV,
	WIRELESS_MAILER_EXTERNAL,
	WIRELESS_MAILER_MESSAGES
};
enum {
	APP_MMS_START_BASE = WIRELESS_MESSAGING_MESSAGES,
	APP_MMS_START_SUBTYPE_NORMAL,
	APP_MMS_END_SELECT_FILE2, /* From mmsSMIL (SMIL Editor) */
	APP_MMS_START_ATTACH,
	APP_MMS_EXTERNAL,
	APP_MMS_START_COMPOSER,
	APP_MMS_START_TEXT_COMPOSER,
	APP_MMS_START_VCARD,
	APP_MMS_START_VCALENDER,
	APP_MMS_START_TELNUM,
	APP_MMS_START_SMS_CLASS0,
	APP_MMS_START_INBOX,
	APP_MMS_CMD,
	APP_MMS_SEND_DONE,
	APP_MMS_SMSSEND_DONE,
	APP_MMS_RCV_DONE,
	APP_MMS_SEND_CANCEL_DONE,
	APP_MMS_SMSSEND_CANCEL_DONE,
	APP_MMS_RCV_NOTIFYCATION,
	APP_MMS_RCV_CANCEL_DONE,
	APP_MMS_MSG_MOVE_DONE,
	APP_MMS_SETTING_DONE,
	APP_MMS_SETTING_RESET_DONE,
	APP_MMS_MSG_DELETE_DONE,
	APP_MMS_MSG_RESET_DONE,
	APP_MMS_START_COMPOSER_BROWSER,
	APP_MMS_START_COMPOSER_BROWSER_ANCHOR,
	APP_MMS_RCV_UPDATE,
	APP_MMS_ZAP_MMSCOMMAND_DONE,
	APP_MMS_START_SLIDE,
#ifdef SLIM_CONFIG_USE_MMBOX
	APP_MMS_SVMAIL_LISTRCV_DONE,
	APP_MMS_SVMAIL_LISTRCV_CANCEL_DONE,
	APP_MMS_SVMAIL_RECEIVE_DONE,
	APP_MMS_SVMAIL_RECEIVE_CANCEL_DONE,
	APP_MMS_SVMAIL_DELETE_DONE,
	APP_MMS_SVMAIL_DELETE_CANCEL_DONE,
	APP_MMS_SVMAIL_FORWARD_DONE,
	APP_MMS_VIEWER_SVMAIL_DELETE_DONE,
	APP_MMS_VIEWER_SVMAIL_DELETE_CANCEL_DONE,
	APP_MMS_SUMMARY_SVMAIL_DELETE_DONE,
	APP_MMS_SUMMARY_SVMAIL_DELETE_CANCEL_DONE,
#endif /* SLIM_CONFIG_USE_MMBOX */
	APP_MMS_MESSAGES
};

void init_menu(CWapBrowserApp * pMe);
void draw_menu(CWapBrowserApp * pMe);
void move_cursor(CWapBrowserApp *pMe, int inc);
void start_nf3(CWapBrowserApp *pMe);

//boolean NF3Test_HandleEvent( AEEApplet * pme, AEEEvent eCode,uint16 wParam, uint32 dwParam);

/*==============================================================================

  ����������Ͷ���
  
==============================================================================*/


/*==============================================================================

  ���غ�������
  
==============================================================================*/
//ģ����غ�������


int CWapBrowserAppMod_Load(IShell *pIShell, void *ph, IModule **ppMod);

int CWapBrowserAppMod_New(int16                nSize, 
                      IShell               *pIShell,
                      void                 *ph,   
                      IModule              **ppMod,
                      PFNMODCREATEINST     pfnMC, 
                      PFNFREEMODDATA       pfnMF);

static uint32 CWapBrowserAppMod_AddRef(IModule *po);

static uint32 CWapBrowserAppMod_Release(IModule *po);

static int CWapBrowserAppMod_CreateInstance(IModule  *po, 
                                        IShell   *pIShell,
                                        AEECLSID ClsId, 
                                        void     **ppObj);

static void CWapBrowserAppMod_FreeResources(IModule *po, IHeap *ph, IFileMgr *pfm);

// Applet��غ�������
// Applet�������ͷź���
static int WapBrowserApp_New(IShell *ps, IModule *pIModule, IWapBrowserApp **ppObj);

static uint32  WapBrowserApp_AddRef(IWapBrowserApp *pi);

static uint32  WapBrowserApp_Release(IWapBrowserApp *pi);

// Applet�¼�������
static boolean WapBrowserApp_HandleEvent(IWapBrowserApp   *pi, 
                                     AEEEvent     eCode,
                                     uint16       wParam, 
                                     uint32       dwParam);

// �ڲ�Applet��������
static int WapBrowserApp_InitAppData(CWapBrowserApp *pMe);

static void WapBrowserApp_FreeAppData(CWapBrowserApp *pMe);

// Applet��״̬��������
static void WapBrowserApp_RunFSM(CWapBrowserApp *pMe);

// ��������֪ͨ�¼��Ĵ�����
//static boolean TVApp_HandleNotifyEvent(CTVApp *pMe, AEENotify *pN);

static void WapBrowserApp_APPIsReadyTimer(void *pme);



/*==============================================================================
ȫ������
==============================================================================*/
// ��Ҫ��ʾ::
//        g_dwAEEStdLibEntryȫ�ֱ���������SDK !
//        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ㣬��
//        Ҫ�ƶ��������д��롣
#ifdef AEE_SIMULATOR
uint32 g_dwAEEStdLibEntry;
//   #error The TV applet was never intended to be used with the AEE Simulator.
#endif

/*==============================================================================

  ���أ���̬������
  
==============================================================================*/

enum {
	mStartMode_BrowserMenu,
	mStartMode_BrowserHome,
	mStartMode_BrowserURL,
	mStartMode_BrowserPushRcv,
	mStartMode_BrowserOTABookmarkRcv,
	mStartMode_BrowserOTARcv,
	mStartMode_Background,
	mStartMode_All
};

static char *gMenuTexts[] = {
	" Start: Brw  Menu",
	" Start: Brw  Home",
	" Start: Brw  URL",
	" Start: Brw  Push Received",
	" Start: Brw  OTA bookmark Received",
	" Start: Brw  OTA Received",
	" Background",
	NULL
};

static int gMenuItemNr, gLcdItemNr, gCharHeight, gMargin;
static int gTopItem, gCurItem;
AEERect lcd_r;

void init_menu(CWapBrowserApp *pMe)
{
	AEEDeviceInfo di;
	char **p;
	int a, d;

	p = gMenuTexts;
	gMenuItemNr = 0;
	while (*p++) gMenuItemNr++;
	gTopItem = gCurItem = 0;
	ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
	lcd_r.x = 0;
	lcd_r.y = 0;
	lcd_r.dx = di.cxScreen;
	lcd_r.dy = di.cyScreen;
	IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_NORMAL, &a, &d);
	gCharHeight = a + d;
	gLcdItemNr = lcd_r.dy / gCharHeight;
	gMargin = lcd_r.dy - gLcdItemNr * gCharHeight;
}

void draw_menu(CWapBrowserApp *pMe)
{
	int i, n;
	AEERect r;
	AECHAR wstr[64];
	RGBVAL bg, fg;
	int count=0;

	for (i=gTopItem; i<gMenuItemNr; i++,count++) {
		if (i == gCurItem) {
			bg = MAKE_RGB(0, 0, 255);
			fg = MAKE_RGB(255, 255, 255);
		}
		else {
			bg = MAKE_RGB(255, 255, 255);
			fg = MAKE_RGB(0, 0, 0);
		}
		r = lcd_r;
		r.y += count * gCharHeight;
		r.dy = gCharHeight;
		n = STRLEN(gMenuTexts[i]);
		UTF8TOWSTR((void*)gMenuTexts[i], n, wstr, 64 * sizeof(AECHAR));
		IDISPLAY_DrawRect(pMe->m_pDisplay, &r, 0, bg, IDF_RECT_FILL);
		IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, fg);
		IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL, wstr, n, 0, r.y, NULL, IDF_TEXT_TRANSPARENT);
	}
	r.y = lcd_r.y + lcd_r.dy - gMargin;
	r.dy = gMargin;
	IDISPLAY_DrawRect(pMe->m_pDisplay, &r, 0, MAKE_RGB(255, 255, 255), IDF_RECT_FILL);

	IDISPLAY_Update (pMe->m_pDisplay);
}

void move_cursor(CWapBrowserApp *pMe, int inc)
{
	if (inc == 1) {
		if (gCurItem < gMenuItemNr - 1) {
			gCurItem++;
			if (gCurItem - gTopItem + 1 > gLcdItemNr)
				gTopItem++;
			draw_menu(pMe);
		}
		return;
	}
	if (inc == -1) {
		if (gCurItem > 0) {
			gCurItem--;
			if (gCurItem < gTopItem)
				gTopItem--;
			draw_menu(pMe);
		}
		return;
	}
}

typedef struct {
    char	*title;
    char	*url;
} StartupInfoURL;

#define AUTO_RECV_FLAG_IMAIL    0x0001
#define AUTO_RECV_FLAG_MSGR     0x0004
#define AUTO_RECV_FLAG_MSGF     0x0008

typedef struct {
	char *fMailAddress;
	char *fSubject;
	char *fContent;
	char *fFilePath;
}StartupInfoEmailComposition;

typedef struct{
	char *fStr;
}StartupInfoMMSComposition;

typedef enum {
	JAM_APP_TYPE_UNKOWN = -1,
	JAM_APP_TYPE_CLDC = 0,
	JAM_APP_TYPE_MIDP,
	JAM_APP_TYPE_DOJA,
	JAM_APP_TYPE_DIR,

	JAM_APP_TYPES
} TJamAppType_;
typedef struct{
	TJamAppType_ fType;
	char *fAdfUrl;
	int fAdfUrlLen;
	byte *fAdfContent;
	int fAdfContentSize;
	char *fUserName;
	int fUserNameLen;
	char *fPassword;
	int fPasswordLen;
	byte *fSessionID;
	int fSessionIDLen;
}StartupInfoJava;

typedef struct {
	int appid_fx;
	int subtype_keycode;
	int par_keychar;
	union {  
		StartupInfoURL	url_info;
		StartupInfoEmailComposition	emailcomposition_info;
		StartupInfoMMSComposition	mmscomposition_info;
		StartupInfoJava				java_info;
    	} fInfo;
} StartInfo;

static StartInfo start_info;
static char *address = "sun@access.co.jp";
enum {
	JAM_START_MODE_LIST = 0,
	JAM_START_MODE_INSTALL,
	JAM_START_MODE_EXEC,
	JAM_START_MODE_EXEC_EMBED,
	JAM_START_MODE_TCK,
	JAM_START_MODE_OTA_TCK,
	JAM_START_MODE_BOOT,
	JAM_START_MODE_BATCH_INSTALL,
	/* MIDP only */
	JAM_START_MODE_PUSH,
	/* DoJa only */
	JAM_START_MODE_CHECK_EXEC,
	JAM_START_MODE_STANDBY_START,
	JAM_START_MODE_STANDBY_LIST,
	JAM_START_MODE_STANDBY_RELEASE,
	JAM_START_MODE_AUTO_START,
#ifdef JAM_CONFIG_DOJA_ENABLE_START_AUTO_START_FAILURE_LOG
	JAM_START_MODE_AUTO_START_FAILURE_LOG,
#endif /* JAM_CONFIG_DOJA_ENABLE_START_AUTO_START_FAILURE_LOG */
#ifdef SLIM_CONFIG_USE_DESKTOPICON
	JAM_START_MODE_EXEC_ID,
#endif /* SLIM_CONFIG_USE_DESKTOPICON */
  /* #if defined (JLT_CONFIG_DOJA) && (30 <= JLT_CONFIG_DOJA) */
	JAM_START_MODE_MSGAPP_START,
	JAM_START_MODE_MSGAPP_STATUS,
  /* #endif */
#ifdef JLT_CONFIG_DOJA_USE_LOCATION
	JAM_START_MODE_LOCATION_EXEC,
#endif
	/* DoJa only end */

	JAM_START_MODE
};
enum {
	APP_EVENT_APPCONTROL = 0,
	APP_EVENT_START,
	APP_EVENT_STOP,
	APP_EVENT_SUSPEND,
	APP_EVENT_RESUME,
	APP_EVENT_STOP_CONFIRM,
	APP_EVENT_INTERRUPT,
	APP_EVENT_FINALIZE,
	APP_EVENT_LOW_BATTERY,
	APP_EVENT_SIM,
	APP_EVENT_IME,
	APP_EVENT_JAVA_NOTIFY,
	APP_EVENT_ADDRESSBOOK,
	APP_EVENT_SMS,
	APP_EVENT_BROWSEFILE,
	APP_EVENT_REGIST_BOOKMARK,
	APP_EVENT_MAIL_INCOMING,
	APP_EVENT_MAIL_CANCEL,
	APP_EVENT_IRMCOBEX_CANCEL,
	APP_EVENT_FLUSH_PAINT,
	APP_EVENT_TELCALL,
	APP_EVENT_SMS_RECEIVED,
	APP_EVENT_SMS_SENT,
	APP_EVENT_EMERGENCY_CALL,
	APP_EVENT_MAIL_REMAIN_ON_SERVER,
	APP_EVENT_EXTENDED_BASE = 0x10000,
	APP_EVENT_EXTENDED_JAVA_BASE = 0x20000,
	APP_EVENTS
};

#define APP_EVENT_IMAIL		0x01
/*#define APP_EVENT_IMMS		0x02*/
#define APP_EVENT_MESSAGE_R	0x04
#define APP_EVENT_MESSAGE_F	0x08

typedef struct {
    int32    fEventID;
    int32    fEventFlag;   /* bit-wise OR of EXTENDED_EVENT_FLAG_XXX */
    char    *fTransactionID;    /* only used for APP_EVENT_MAIL_INCOMING */
    char    *fContentLocation;  /* only used for APP_EVENT_MAIL_INCOMING */
} ExtendedEvent;

ExtendedEvent eevt;

/*#define NF3_TEST_SI_RECEIVE*/
#define NF3_TEST_SI_RECEIVE_FAIL_2

static void test_Push_Msg_Rcv(CWapBrowserApp *pMe)
{
#if 0
    nf3_wap_push_info smspus_data;

    uint8 sms[] = {
		0x73,0x06,0x1A,0xAE,0xB4,0x87,0xAF,0x82,0x45,0x6E,0x63,0x6F,0x64,0x69,0x6E,0x67,0x2D,0x56,0x65,
		0x72,0x73,0x69,0x6F,0x6E,0x00,0x31,0x2E,0x32,0x00,0x02, 0x05, 0x6a, 0x00, 0x45, 0xc6, 0x08, 0x0c, 
		0x03, 0x77, 0x61, 0x70, 0x2e, 0x79, 0x61, 0x68, 0x6f,0x6f, 0x00, 0x85, 0x0a, 0xc3, 0x07, 0x20, 0x03, 
		0x10, 0x06, 0x10, 0x17, 0x03, 0x10, 0xc3, 0x07, 0x20,0x04, 0x10, 0x06, 0x10, 0x17, 0x03, 0x01, 0x03, 
		0x20, 0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20,0x64, 0x69, 0x73, 0x70, 0x6c, 0x61, 0x79, 0x65,
		0x64, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2e, 0x20, 0x00,0x01, 0x01
        };

    int smssize = sizeof(sms);

    MEMSET( &smspus_data, 0x00, sizeof(nf3_wap_push_info) );
    smspus_data.sm_len = smssize;
    smspus_data.sm_data = (uint8*) MALLOC(smssize);
    MEMSET( smspus_data.sm_data, 0x00, smssize);
    MEMCPY( smspus_data.sm_data , sms, smssize);

    ISHELL_SendEvent(pMe->m_pIShell, AEECLSID_NF3, (AEEEvent)EVT_APP_RECEIVE_START, 
                        (uint16)EVT_RECEIVE_WAPPUSH, (uint32)&smspus_data);

    FREEIF(smspus_data.sm_data);
#else
	;
#endif
}

static void test_OTA_Bookmark_Rcv(CWapBrowserApp *pMe)
{
    char title[] = {'t','e','s','t','\0','\0','\0','\0'};
    char url[]   = {'h','t','t','p',':','/','/','t','e','s','t','.','c','o','m','\0','\0','\0'};
    TBookmarkObj ota_data;
             
    ota_data.fTitle = title;
    ota_data.fURL = url;
    ota_data.fProtected = FALSE;
    ota_data.fRet = FALSE;
             
    ISHELL_SendEvent(pMe->m_pShell, AEECLSID_NF3, EVT_USER, EVT_NF3_ADD_BOOKMARK_OTA, (uint32)&ota_data);    
}

static void test_OTA_Rcv(CWapBrowserApp *pMe)
{
    otap_browser_t ota_data;
    otap_pxlogical_t proxy;
    otap_pxphysical_t pxphysical;
    otap_port_t port;
    otap_resource_t bookmark;
    
    char name[] = {'c','o','n','n','\0','\0'};
    char NAPId[] = {'z','t','e','\0','\0','\0'};
    char pxaddress[] = {'p','r','o','x','y','\0'};
    char bm_title[] = {'t','e','s','t','\0','\0','\0','\0'};
    char bm_url[]   = {'h','t','t','p',':','/','/','t','e','s','t','1','.','c','o','m','\0','\0','\0'};
  
    MEMSET( &ota_data, 0x00, sizeof(otap_browser_t) );
    MEMSET( &proxy, 0x00, sizeof(otap_pxlogical_t) );
    MEMSET( &pxphysical, 0x00, sizeof(otap_pxphysical_t) );
    MEMSET( &port, 0x00, sizeof(otap_port_t) );
    MEMSET( &bookmark, 0x00, sizeof(otap_resource_t) );

    /* bookmark */
    ota_data.pBookmark = &bookmark;
    bookmark.pszName = bm_title;
    bookmark.pszURI = bm_url;
    bookmark.bStartPage = FALSE;
    bookmark.pNext = NULL;

    /* connection */
    ota_data.pszName = name;
    ota_data.pProxy = &proxy;
    proxy.pPxphysical = &pxphysical;
    pxphysical.pszPxAddr = pxaddress;
    pxphysical.pPort = &port;
    port.uPortNBR = 80;
    pxphysical.uProfileNum = 1;    
    
    ISHELL_SendEvent(pMe->m_pShell, AEECLSID_NF3, EVT_USER, EVT_PUSH_DATA_IND, (uint32)&ota_data); 
}

void start_nf3(CWapBrowserApp *pMe)
{
	char	buf[12];

	MEMSET(&start_info, 0, sizeof(StartInfo));
	switch (gCurItem) {
	case mStartMode_BrowserMenu:
		start_info.appid_fx = APP_ORIGINATOR_BROWSER;
		start_info.subtype_keycode = APP_BROWSER_START_BY_MAINMENU;
		start_info.par_keychar = 0;	
		break;
	case mStartMode_BrowserHome:
		start_info.appid_fx = APP_ORIGINATOR_BROWSER;
		start_info.subtype_keycode = APP_BROWSER_START_HOME;
		start_info.par_keychar = 0;		
		break;
	case mStartMode_BrowserURL:
	        start_info.appid_fx = APP_ORIGINATOR_BROWSER;
	        start_info.subtype_keycode = APP_BROWSER_START_URL;
	        start_info.par_keychar = 0;
	        start_info.fInfo.url_info.title = "zte";
	        start_info.fInfo.url_info.url = "http://www.zte.com.cn";
		break;
	case mStartMode_BrowserPushRcv:
	{
            test_Push_Msg_Rcv(pMe);
            break;
	}
       case mStartMode_BrowserOTABookmarkRcv:
       {
            test_OTA_Bookmark_Rcv(pMe);  
            break;
       }
       case mStartMode_BrowserOTARcv:
       {
            test_OTA_Rcv(pMe);
            break;
       }

	default:
		start_info.appid_fx = APP_ORIGINATOR_BROWSER;
		start_info.subtype_keycode = APP_BROWSER_START_BY_MAINMENU;
		start_info.par_keychar = 0;
		break;
	}
	SPRINTF(buf, "%p", &start_info);
	ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_NF3, buf);
}


static CWapBrowserAppMod gWapBrowserAppMod;

static const VTBL(IModule) gModFuncs =
{
    CWapBrowserAppMod_AddRef,    
    CWapBrowserAppMod_Release,
    CWapBrowserAppMod_CreateInstance,
    CWapBrowserAppMod_FreeResources
};

// ֻ����һ�� WapBrowserApp ʵ����ÿ�δ��� WapBrowser Applet ʱ������ͬһ�ṹָ��� BREW �㡣
static CWapBrowserApp gWapBrowserApp;//={0};

static const VTBL(IWapBrowserApp) gWapBrowserAppMethods =
{
    WapBrowserApp_AddRef,
    WapBrowserApp_Release,
    WapBrowserApp_HandleEvent
};

/*==============================================================================
����:
       CWapBrowserAppMod_Load

˵��:
       ģ��װ�غ�����

����:
       pIShell [in]:IShell�ӿ�ָ�롣
       ph :û��ʹ��
       ppMod [out]:
 
����ֵ:
       int
   
��ע:
  
==============================================================================*/
int CWapBrowserAppMod_Load(IShell   *pIShell,
                           void     *ph,
                           IModule  **ppMod)
{
    // ���ڶ�̬Applet����֤AEE STD��汾����ȷ�����뽨����ģ��ʱʹ�õĿ�汾ƥ��
    // ����AEE_SIMULATOR�²��ؽ��б���顣
    MSG_FATAL("CWapBrowserAppMod_Load...........Start!!!!",0,0,0);
    if(ph != NULL && ppMod !=NULL && pIShell !=NULL)
    {
    	MSG_FATAL("CWapBrowserAppMod_Load...........Start!!!!1111",0,0,0);
    }
#if !defined(BREW_STATIC_APP) && !defined(AEE_SIMULATOR)
    if(GET_HELPER_VER() != AEESTDLIB_VER)
    {
    	MSG_FATAL("CWapBrowserAppMod_Load...........Start!!!!2222",0,0,0);
        return EVERSIONNOTSUPPORT;
    }
#endif
	MSG_FATAL("CWapBrowserAppMod_Load================================",0,0,0);
    return CWapBrowserAppMod_New(sizeof(CWapBrowserAppMod), pIShell, ph, ppMod, NULL, NULL);
}

/*==============================================================================
����:
       CWapBrowserAppMod_New

˵��:
       ������ʼ��WapBrowserAppģ�������Ҳ�������ü����������ֺ�����
  
����:
       nSize: û��ʹ��
       pIShell [in]: IShell �ӿ�ָ�롣
       ph : û��ʹ��
       ppMod [out]:
       pfnMC: û��ʹ��
       pfnMF: û��ʹ��
 
����ֵ:
       int
   
��ע:
  
==============================================================================*/
int CWapBrowserAppMod_New(int16             nSize,
                      IShell            *pIShell,
                      void              *ph,
                      IModule           **ppMod,            
                      PFNMODCREATEINST  pfnMC,
                      PFNFREEMODDATA    pfnMF)
{
    PARAM_NOT_REF(nSize)
    PARAM_NOT_REF(pfnMC)
    PARAM_NOT_REF(pfnMF)
#ifndef AEE_SIMULATOR
    PARAM_NOT_REF(ph)
#endif
    if(!ppMod)
    {
        return EFAILED;
    }
  
    *ppMod = NULL;
  
// ��Ҫ��ʾ:
//        g_dwAEEStdLibEntryȫ�ֱ���������SDK !
//        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ�
//        ��
#ifdef AEE_SIMULATOR
    g_dwAEEStdLibEntry = (uint32)ph;
    if(!pIShell || !ph)
    {
        return EFAILED;
    }
#else
    if(!pIShell)
    {
        return EFAILED;
    }
#endif
  
    MEMSET(&gWapBrowserAppMod,0,sizeof(CWapBrowserAppMod));
  
    INIT_VTBL(&gWapBrowserAppMod,IModule,gModFuncs);
  
    gWapBrowserAppMod.m_nRefs = 1;
  
    *ppMod = (IModule *)&gWapBrowserAppMod;
  
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       CWapBrowserAppMod_AddRef

˵��:
       WapBrowserAppģ�����ü�����1�������ص�ǰ����������
  
����:
       po [in/out]:IModule�ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ��WapBrowserAppģ���������
   
��ע:
  
==============================================================================*/
static uint32 CWapBrowserAppMod_AddRef(IModule *po)
{
    return(++((CWapBrowserAppMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       CWapBrowserAppMod_Release

˵��:
       WapBrowserAppģ�����ü�����1�������ص�ǰ����������

����:
       po [in/out]:IModule�ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ��TVAppģ���������
   
��ע:
  
==============================================================================*/
static uint32 CWapBrowserAppMod_Release(IModule *po)
{
    if (((CWapBrowserAppMod *) po)->m_nRefs == 0)
    {
        return 0;
    }
 
    return(--((CWapBrowserAppMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       CWapBrowserAppMod_CreateInstance

˵��:
       ͨ������(��ʼ��)WapBrowserApp����������ģ��ʵ����
  
����:
       po [in]:IModule�ӿڶ���ָ�롣
       pIShell [in]:IShell�ӿڶ���ָ�롣
       ClsId [in]:Ҫ������ʵ������ID��
       ppObj [out]::���ڱ������ʵ����ָ���ָ�������
 
����ֵ:
       none
   
��ע:
       �����û�ȥ���á�
  
==============================================================================*/
static int CWapBrowserAppMod_CreateInstance(IModule   *po,
                                        IShell    *pIShell,
                                        AEECLSID  ClsId,
                                        void      **ppObj)
{
    *ppObj = NULL;
 
    if(ClsId != AEECLSID_TVAPP)
    {
        return EFAILED;
    }
 
    if(WapBrowserApp_New(pIShell, po, (IWapBrowserApp**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
 
    return SUCCESS;
}

/*==============================================================================
����:
       CWapBrowserAppMod_FreeResources

˵��:
       �ͷ�WapBrowserAppģ����Դ�ĺ�����

����:
       po :û��ʹ�á�
       IHeap :û��ʹ�á�
       pfm ::û��ʹ�á�
 
����ֵ:
       none
   
��ע:
       �����û�ȥ���á�
  
==============================================================================*/
static void CWapBrowserAppMod_FreeResources(IModule *po, IHeap  *ph, IFileMgr *pfm)
{
    MSG_FATAL("po address = %x", po, 0, 0);
    MSG_FATAL("ph address = %x", ph, 0, 0);
    MSG_FATAL("pfm address = %x", pfm, 0, 0);
    PARAM_NOT_REF(po)
    PARAM_NOT_REF(ph)
    PARAM_NOT_REF(pfm)
}

/*==============================================================================
����:
       WapBrowserApp_New

˵��:
       ��ʼ�� WapBrowserApp Applet �������ڲ����ݱ�����ͬʱ WapBrowserApp Applet ���ü�����1��
  
����:
       ps [in]: IShell�ӿڶ���ָ�롣
       pIModule [in/out]: pIModule�ӿڶ���ָ�롣
       ppObj [out]: ָ�� ITVApp ����ָ���ָ�롣
 
����ֵ:
       EFAILED: ʧ�ܡ�
       SUCCESS: �ɹ���
   
��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�
  
==============================================================================*/
static int WapBrowserApp_New(IShell *ps, IModule *pIModule, IWapBrowserApp **ppObj)
{
    int retVal = SUCCESS;
   // ITvUtil* pITvUtil = NULL;
	//CTVApp* pThis = NULL;
	
    if(0 == gWapBrowserApp.m_nRefs)
    {
        MEMSET(&gWapBrowserApp, 0, sizeof(gWapBrowserApp));
        INIT_VTBL(&gWapBrowserApp, IWapBrowserApp, gWapBrowserAppMethods);
  
        gWapBrowserApp.m_nRefs = 0;
        gWapBrowserApp.m_pShell = ps;
        gWapBrowserApp.m_pModule = pIModule;
        //gWapBrowserApp.pIMMITv=NULL;
        (void)ISHELL_AddRef(ps);
        (void)IMODULE_AddRef(pIModule);
  
        retVal = WapBrowserApp_InitAppData(&gWapBrowserApp);
		
    }
 
    ++gWapBrowserApp.m_nRefs;
 
    *ppObj = (IWapBrowserApp*)&gWapBrowserApp;

    return retVal;
}

/*==============================================================================
����:
       WapBrowserApp_AddRef

˵��:
       WapBrowserApp Applet ���ü�����1�������ص�ǰ����������
  
����:
       pi [in/out]: IWapBrowserApp �ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ�� WapBrowserApp Applet ��������
   
��ע:
  
==============================================================================*/
static uint32 WapBrowserApp_AddRef(IWapBrowserApp *pi)
{
    register CWapBrowserApp *pMe = (CWapBrowserApp*)pi;
 
    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);
 
    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       WapBrowserApp_Release

˵��:
       �ͷŶ� WapBrowserApp Applet �����ã����ü�����1�������ص�ǰ����������
       �����ǰ��������Ϊ�㣬�����ͷ� Applet �������Դ��
  
����:
       pi [in/out]: IWapBrowserApp �ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ�� TV Applet ��������
   
��ע:
  
==============================================================================*/
static uint32 WapBrowserApp_Release(IWapBrowserApp *p)
{
    register CWapBrowserApp *pMe = (CWapBrowserApp*)p;
 
    if(pMe->m_nRefs == 0)
    {
        return 0;
    }
 
    if(--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }
 
    WapBrowserApp_FreeAppData(pMe);
    MSG_FATAL("m_pShell address = %x", pMe->m_pShell, 0, 0);
    MSG_FATAL("m_pModule address = %x", pMe->m_pModule, 0, 0);
    (void)ISHELL_Release(pMe->m_pShell);
    (void)IMODULE_Release(pMe->m_pModule);
 
    // ע��:pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       WapBrowserApp_InitAppData

˵��:
       ��ʼ�� WapBrowserApp Applet �ṹ����ȱʡֵ��
  
����:
       pMe [in]: ָ�� WapBrowserApp Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
 
����ֵ:
       EFAILED: ��ʼ��ʧ�ܡ�
       SUCCESS: ��ʼ���ɹ���
   
��ע:
  
==============================================================================*/
static int WapBrowserApp_InitAppData(CWapBrowserApp *pMe)
{
    AEEDeviceInfo  di;
    if(NULL == pMe)
    {
        return EFAILED;
    }
    ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
    pMe->m_rc.x       = 0;
    pMe->m_rc.y       = 0;
    pMe->m_rc.dx      = (int16) di.cxScreen;
    pMe->m_rc.dy      = (int16) di.cyScreen;

	if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_DISPLAY,
                                            (void **)&pMe->m_pDisplay))
    {
        WapBrowserApp_FreeAppData(pMe);
		MSG_FATAL("AEECLSID_DISPLAY-----------------%d",EFAILED,0,0);
        return EFAILED;
    } 
    return SUCCESS;

	
} 



/*==============================================================================
����:
       WapBrowserApp_FreeAppData

˵��:
       �ͷ� WapBrowserApp Applet �����Դ��
  
����:
       pMe [in]: ָ��WapBrowserApp Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
 
����ֵ:
       none
   
��ע:
  
==============================================================================*/
static void WapBrowserApp_FreeAppData(CWapBrowserApp *pMe)
{
    if(NULL == pMe)
    {
        return;
    }
    
    
}

/*==============================================================================
����:
       WapBrowserApp_RunFSM
    
˵��:
       WapBrowserApp Applet����״̬�����档
   
����:
       pMe [in]��ָ��WapBrowserApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
     
����ֵ:
       �ޡ�
    
��ע:
      
==============================================================================*/
static void WapBrowserApp_RunFSM(CWapBrowserApp *pMe)
{
    //NextFSMAction nextFSMAction = NFSMACTION_WAIT;
     
    for( ; ; )
    {
        //nextFSMAction = WapBrowserApp_ProcessState(pMe);
      
        if(pMe->m_bNotOverwriteDlgRet)
        {
            pMe->m_bNotOverwriteDlgRet = FALSE;
        }
        else
        {
            //pMe->m_eDlgRet = DLGRET_CREATE;
        }
      
       // if(nextFSMAction == NFSMACTION_WAIT)
        //{
        //    break;
        //}
    }
}
    
/*==============================================================================
����:
       WapBrowserApp_HandleEvent
    
˵��:
       WapBrowserApp Applet�¼��������������������ڱ�Applet���¼����������ɸú�������
   
����:
       pi [in]:ָ��IWapBrowserApp�ӿ�t�����ָ�롣
       eCode:BREW�¼����롣
       wParam:�¼�����
       dwParam [in]:��wParam���������ݡ���
     
����ֵ:
       TRUE:�����¼�������
       FALSE:�����¼�û������
    
��ע:
       �ú�����BREW���ã��û����õ��á�
      
==============================================================================*/
static boolean WapBrowserApp_HandleEvent(IWapBrowserApp  *pi,
                                     AEEEvent     eCode,
                                     uint16       wParam,
                                     uint32       dwParam)
{
    CWapBrowserApp *pMe = (CWapBrowserApp*)pi;   
    AEEAppStart *as;    
    int  nAscent,nDescent;

    MSG_FATAL("WapBrowserApp_HandleEvent-----------------------0x%x",eCode,0,0);
    switch(eCode)
    {
        case EVT_APP_START:
			MSG_FATAL("WapBrowserApp_HandleEvent--------EVT_APP_START---------------",0,0,0);
            init_menu(pMe);
			draw_menu(pMe);
			MSG_FATAL("WapBrowserApp_HandleEvent--------EVT_APP_START----------end  ",0,0,0);
			IDISPLAY_Update (pMe->m_pDisplay);
            return TRUE;
      
        case EVT_APP_STOP:
            return TRUE;
      
        case EVT_APP_SUSPEND:
            
            return TRUE;
            
        case EVT_ALARM:
            return TRUE;
            
        case EVT_APP_RESUME:
            return TRUE;
      
        case EVT_DIALOG_INIT:
        case EVT_DIALOG_START:
        case EVT_USER_REDRAW:
            return TRUE;
            
        case EVT_CAMERA_NOTIFY:
            return TRUE;
            
        case EVT_APPISREADY:
            return TRUE;
      
        case EVT_KEY_PRESS:
        {
        	switch (wParam) {
				case AVK_UP:
					move_cursor(pMe, -1);
					break;
				case AVK_DOWN:
					move_cursor(pMe, 1);
					break;
				case AVK_SELECT:
					start_nf3(pMe);
					break;
				case AVK_CLR:
					ISHELL_CloseApplet(pMe->m_pShell, FALSE);
					break;
				case AVK_2:
					ISHELL_SendEvent(pMe->m_pShell, AEECLSID_NF3, EVT_USER, 32001, 0);
					break;
				default:
					break;
			}
		}
		return TRUE;
        case EVT_KEY_RELEASE:
        case EVT_KEY:
        case EVT_COMMAND:         
        case EVT_DIALOG_END:
            return TRUE;
   
        default:
            //�����յ����¼�·������ǰ��ĶԻ����¼���������
            //return WapBrowserApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
            break;
    }

    return FALSE;
} //TVApp_HandleEvent()

/*==============================================================================
����:
       WapBrowserApp_APPIsReadyTimer
  
˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���
 
����:
       pme [in]��void *����ָ�룬��ָCWapBrowserApp�ṹָ�롣
   
����ֵ:
       �ޡ�
  
��ע:
    
==============================================================================*/
static void WapBrowserApp_APPIsReadyTimer(void *pme)
{
    CWapBrowserApp *pMe = (CWapBrowserApp *)pme;
    
    if(NULL == pMe)
    {
        return;
    }

}

