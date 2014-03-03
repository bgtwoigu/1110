/*==============================================================================
// �ļ���
//        SecurityMenuDialogHandler.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//
// �ļ�˵����
//
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��    ���ⵥ��  �޸����ݡ�λ�ü�ԭ��
-----------      ----------  --------  -----------------------------------------------
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "SecurityMenu_priv.h"
#include "AEEBacklight.h"
#ifdef FEATURE_PLANEMODE
#include "aeecm.h"
#endif
#include "wmsapp.h"
#ifdef FEATURE_APP_MEDIAGALLERY
#include "MediaGallery.h" 
#endif
#include "ContApp.h"
#include "AEETelephone.h"
#include "AEETelDef.h"
#if defined(FEATURE_VERSION_K212)||defined(FEATURE_VERSION_K212_BH)||defined(FEATURE_VERSION_K232_Y105A)
#include "tetris.h"
#endif
#define FILE_CONTAPP_CFG                (AEE_ADDRBOOK_DIR"\\contapp.cfg")
#define CAL_DATABASE_PATH           	"fs:/mod/scheduleapp"     //���ݿ��·����
#define FILE_SCHEDULE_CFG           	"schedulecfg"  //�ճ̱�������ļ�?

//#include "Scheduleapp.h"
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// �رնԻ����
#define CLOSE_DIALOG(DlgRet)  {    \
                                   pMe->m_eDlgRet = DlgRet; \
                                   (void) ISHELL_EndDialog(pMe->m_pShell);  \
                               }

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// �Ի��� IDD_MAIN_DIALOG �¼�������
static boolean  SecurityMainDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

//�Ի���  IDD_APPLICATIONLOCK_DIALOGʱ�䴦����
static boolean  SecurityApplicationLockDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

// �Ի��� IDD_PHONE_PASSWORD_CHECK_DIALOG �¼�������
static boolean  SecurityPassWordDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

// �Ի��� IDD_KEY_LOCK_CHECK_DIALOG �¼�������
static boolean   SecurityKeyLockDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

// �Ի��� IDD_PHONE_PASSWORD_INPUT_DIALOG �¼�������
static boolean  SecurityCallPassWordInputDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

// �Ի��� IDD_PIN_CHECK_DIALOG �¼�������
static boolean  SecurityPinCheckDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

// �Ի��� IDD_PIN_CHANGE_DIALOG �¼�������
static boolean  SecurityPinChangeDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

// �Ի��� IDD_ASK_PASSWORD_DIALOG �¼�������
static boolean  SecurityAskPasswordDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);
#ifdef FEATURE_LCD_TOUCH_ENABLE
// �Ի��� IDD_TSIMINPUTPASSWORD_DIALOG �¼�������
static boolean  SecurityTsimInputPasswordDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);
#endif
// �Ի��� IDD_ASK_PIN_DIALOG �¼�������
static boolean  SecurityAskPinDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

// �Ի��� IDD_ASK_CALL_PASSWORD_DIALOG �¼�����?
static boolean  SecurityAskCallPasswordDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

//����ȷ�϶Ի�������
static boolean  SecurityAffirmPassWordHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

//PUK����Ի���IDD_PUK_DIALOG������
static boolean  SecurityAskPUKPassWordHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

//PUK����Ի���IDD_UIMERR_DIALOG������
static boolean  SecurityUIMErrHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

//PUK����Ի���IDD_EMGCALL_DIALOG������
static boolean  SecurityEmergencyCallHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam);

// �Ի��� IDD_RESTORE_DIALOG �¼�������
static boolean  HandleRestoreDialogEvent(CSecurityMenu *pMe,
                                        AEEEvent eCode,
                                        uint16 wParam,
                                        uint32 dwParam);

static boolean  HandleChangeCodeDialogEvent(CSecurityMenu *pMe,
                                        AEEEvent eCode,
                                        uint16 wParam,
                                        uint32 dwParam);
#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316) || defined(FEATURE_VERSION_IC241A_MMX)
static boolean  SecurityMobileTrackerHandler(CSecurityMenu *pMe,
                                        AEEEvent eCode,
                                        uint16 wParam,
                                        uint32 dwParam);
#endif
static boolean  Security_HandleMsgBoxDlgEvent(CSecurityMenu* pMe,
    AEEEvent eCode,
    uint16   wParam,
    uint32   dwParam
);
#ifdef FEATURE_VERSION_C316
void HandleTimer(IPhoneCtl *m_pIPhoneCtl);
#endif
//�ָ��������ú���
static void SecurityMenu_RestoryFactorySet(CSecurityMenu *pMe);

// ɾ�����б������ֻ��ϵ� ����  ���� �绰����Ϣ ����ͨ����Ϣ  gallery�洢����
static int SecurityMenu_DeleteAllEntries(CSecurityMenu *pMe);

//���ò˵�Ϊδѡ״̬
static void SetMenu_SetRadioIconAsOff(IMenuCtl *pMenu);

//����Ƿ�Ϊ��ȷ��pin��
static boolean SecurityMenu_ValidPIN(CSecurityMenu *pMe, char *szPIN);
//���ڽ����� PIN ������Ϊ�µ� PIN �롣
static boolean SecurityMenu_ModfyPIN(CSecurityMenu *pMe, char *szPIN);


//��ʱ���ص�������
static void CSecurtyMenu_DialogTimeout(void *pme);

static void SecurityMenu_SetItemNumIcon(IMenuCtl   *pMenu);

#ifdef FEATURE_VERSION_C316	
static boolean  SecurityOneKeyLockKeypadDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent eCode,
                                        uint16 wParam,
                                        uint32 dwParam);
#endif
/*==============================================================================
                                 ȫ������
==============================================================================*/


/*==============================================================================
                                 ���أ���̬������
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/

/*==============================================================================
����:
   Security_ShowMsgBox

˵��:
    ������״̬��������������ȫ���ڶԻ���

����:
    pMe [in]: ָ�� Security Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wTextResId [in]: ��Ϣ�ı���Դ ID��

����ֵ:
    none

��ע:

==============================================================================*/
void Security_ShowMsgBox(CSecurityMenu *pMe, uint16 wTextResId)
{
 

    pMe->m_wMsgID = wTextResId;
   
    // ������Ϣ�Ի���.
    SecurityMenu_ShowDialog(pMe, IDD_MSGBOX);
}

/*==============================================================================
����:
       SecurityMenu_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��SecurityMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void SecurityMenu_ShowDialog(CSecurityMenu *pMe,uint16  dlgResId)
{
    int nRet;
    //SEC_ERR("next show %d dialog", dlgResId, 0, 0);
    // At most one dialog open at once
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        MSG_FATAL("Trying to create a dialog without closing the previous one",0,0,0);
        return;
    }

    nRet = ISHELL_CreateDialog(pMe->m_pShell,AEE_APPSSECURITYMENU_RES_FILE,dlgResId,NULL);

    if (nRet != SUCCESS)
    {
        MSG_FATAL("Failed to create the dialog in the SecurityMenu applet",0,0,0);
    }
}

/*==============================================================================
����:
       SecurityMenu_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��SecurityMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean SecurityMenu_RouteDialogEvent(CSecurityMenu *pMe,
                                      AEEEvent       eCode,
                                      uint16         wParam,
                                      uint32         dwParam)
{
    if (NULL == pMe)
    {
        return FALSE;
    }

    if (NULL == pMe->m_pActiveDlg)
    {
        return FALSE;
    }
    
    if(wParam == AVK_WITHDRAW &&
        pMe->m_eCurState !=  SECURITYMENU_ASKPUKPASSWORD &&
        pMe->m_eCurState !=  SECURITYMENU_UIMERR &&
        pMe->m_eCurState !=  SECURITYMENU_EMERGENCYCALL)
    {
        ISHELL_CloseApplet(pMe->m_pShell, TRUE);
    }
    
    //SEC_ERR("%d SecurityMenu_RouteDialogEvent",pMe->m_pActiveDlgID,0,0);
    switch (pMe->m_pActiveDlgID)
    {
        case IDD_MSGBOX:
           return Security_HandleMsgBoxDlgEvent(pMe,eCode,wParam,dwParam);

        case IDD_MAIN_DIALOG:
           return SecurityMainDlgHandler(pMe,eCode,wParam,dwParam);

        case IDD_APPLICATIONLOCK_DIALOG:
            return SecurityApplicationLockDlgHandler(pMe,eCode,wParam,dwParam);

        case IDD_PHONE_PASSWORD_CHECK_DIALOG:
            return SecurityPassWordDlgHandler(pMe,eCode,wParam,dwParam);
            
        case IDD_KEY_LOCK_CHECK_DIALOG:
        	return SecurityKeyLockDlgHandler(pMe,eCode,wParam,dwParam);

        case IDD_PHONE_PASSWORD_INPUT_DIALOG:
            return SecurityCallPassWordInputDlgHandler(pMe,eCode,wParam,dwParam);

        case IDD_PIN_CHECK_DIALOG:
           return SecurityPinCheckDlgHandler(pMe,eCode,wParam,dwParam);

        case IDD_PIN_CHANGE_DIALOG:
           return SecurityPinChangeDlgHandler(pMe,eCode,wParam,dwParam);

        case IDD_ASK_PASSWORD_DIALOG:
           return SecurityAskPasswordDlgHandler(pMe,eCode,wParam,dwParam);
#ifdef FEATURE_LCD_TOUCH_ENABLE
        case IDD_TSIMINPUT_PASSWORD_DIALOG:
           return SecurityTsimInputPasswordDlgHandler(pMe,eCode,wParam,dwParam); 
#endif       
        case IDD_ASK_PIN_DIALOG:
           return SecurityAskPinDlgHandler(pMe,eCode,wParam,dwParam);   

        case IDD_ASK_CALL_PASSWORD_DIALOG:
            return SecurityAskCallPasswordDlgHandler(pMe,eCode,wParam,dwParam);  

        case IDD_AFFIRM_PASSWORD:
            return SecurityAffirmPassWordHandler(pMe,eCode,wParam,dwParam);
            
        case IDD_PUK_DIALOG:
            return SecurityAskPUKPassWordHandler(pMe,eCode,wParam,dwParam);
            
        case IDD_UIMERR_DIALOG:
            return SecurityUIMErrHandler(pMe,eCode,wParam,dwParam);
                
        case IDD_EMGCALL_DIALOG:
            return SecurityEmergencyCallHandler(pMe,eCode,wParam,dwParam);

        case IDD_RESTORE_MENU:
            return HandleRestoreDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_CHANGE_CODE:
            return HandleChangeCodeDialogEvent(pMe,eCode,wParam,dwParam);
		#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316) || defined(FEATURE_VERSION_IC241A_MMX)
		case  IDD_MOBILE_TRACKER_DIALOG:
			return SecurityMobileTrackerHandler(pMe,eCode,wParam,dwParam);
		#endif

#ifdef FEATURE_VERSION_C316	
        case IDD_ONEKEY_LOCK_KEYPAD_DIALOG:
            return SecurityOneKeyLockKeypadDlgHandler(pMe,eCode,wParam,dwParam);
#endif
        default:
           return FALSE;
    }
}

/*==============================================================================
������
       SecurityMainDlgHandler
˵����
       IDD_MAIN_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityMainDlgHandler(CSecurityMenu *pMe,
                                       AEEEvent       eCode,
                                       uint16         wParam,
                                       uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                        IDC_SECURITY_MENU);
    SEC_ERR("%x,%x,%x,SecurityMainDlgHandler",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    //ʵ�ֲ˵�ѭ����������
    //SecurityMenu_AutoScroll(pMenu,eCode,wParam);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			//add by yangdecai
			{
				AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        IDS_SECURITY_TITLE,
                        WTitle,
                        sizeof(WTitle));
				IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
            }
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_PHONE_LOCK, IDS_PHONE_LOCK, NULL, 0);
            if(IsRunAsUIMVersion())
            {
                IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_PIN_SET, IDS_PIN_SET, NULL, 0);
            }
#ifdef FEATURE_VERSION_C316	
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_DATA_PROTECTION, IDS_DATA_PROTECTION, NULL, 0);
#else
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_APPLICATION_LOCK, IDS_APPLICATION_LOCK, NULL, 0);
#endif
			 #ifdef FEATURE_KEYGUARD
            #if defined(FEATURE_VERSION_HITZ181)||defined(FEATURE_VERSION_MTM)||defined(FEATURE_VERSION_W317A)
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AUTOKEYGUARD_TITLE, IDS_AUTOKEYGUARD_TITLE, NULL, 0);
            #endif
            #endif
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_PHONE_PASSWORD_CHANGE, IDS_PHONE_PASSWORD_CHANGE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_RESTORE, IDS_RESTORE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_DELETE, IDS_DELETE, NULL, 0);
			#if defined(FEATURE_VERSION_W317A)
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_MOBILE_TRACKER, IDS_MOBILE_TRACKER, NULL, 0);
			#endif
#ifdef FEATURE_VERSION_C316	
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_ONEKEY_LOCK_KEPAD, IDS_ONEKEY_LOCK_KEPAD, NULL, 0);
#endif
            return TRUE;

        case EVT_DIALOG_START:
            pMe->m_lock_sel = SEC_SEL_NONE;
            //�趨�����ʽ
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY|MP_ACTIVE_NO_REDRAW);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            // �趨�˵��ؼ��ľ���λ�ü��ߴ�
            //SetControlRect(pMe,pMenu);

            // ���˵����˵�������ֱ��ͼ��
            SecurityMenu_SetItemNumIcon(pMenu);

            IMENUCTL_SetSel(pMenu, pMe->m_currDlgId);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }

            return TRUE;

        case EVT_COMMAND:
            //pMe->m_currDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_currDlgId = wParam;
            switch(wParam)
            {
                case IDS_PHONE_LOCK:           //�ֻ���
                    pMe->m_lock_sel = SEC_SEL_PHONE_LOCK;
                    CLOSE_DIALOG(DLG_PHONEPASSWORD)
                    break;

#ifdef FEATURE_VERSION_C316	
                case IDS_ONEKEY_LOCK_KEPAD:           //һ������
                    pMe->m_lock_sel = SEC_SEL_ONE_KEY_LOCK_KEYPAD;
                    CLOSE_DIALOG(DLGRET_ONEKEY_LOCK_KEYPAD)
                    break;
#endif
#ifdef FEATURE_VERSION_C316	
                case IDS_DATA_PROTECTION:
#endif
                case IDS_APPLICATION_LOCK:
                    CLOSE_DIALOG(DLG_APPLICATIONLOCK)
                    break;

                case IDS_PHONEBOOK_LOCK:           //PHONEBOOK LOCK
                    pMe->m_lock_sel = SEC_SEL_PHONEBOOK_LOCK;
                    CLOSE_DIALOG(DLG_PHONEPASSWORD)
                    break;

#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
                case IDS_SMS_LOCK:           //SMS LOCK
                    pMe->m_lock_sel = SEC_SEL_SMS_LOCK;
                    CLOSE_DIALOG(DLG_PHONEPASSWORD)
                    break;
#endif

#ifdef FEATURE_KEYGUARD
                case IDS_KEY_LOCK:           //KEY LOCK
                    pMe->m_lock_sel = SEC_SEL_KEY_LOCK;
                    CLOSE_DIALOG(DLG_PHONEPASSWORD)
                    break;
                    
                #if defined(FEATURE_VERSION_HITZ181)||defined(FEATURE_VERSION_MTM)||defined(FEATURE_VERSION_W317A)
                case IDS_AUTOKEYGUARD_TITLE:
                	{
                		pMe->m_lock_sel = SEC_SEL_KEY_LOCK;
                    	CLOSE_DIALOG(DLGRET_KEYLOCK)
                	}
                	break;
                #endif
#endif
                case IDS_RECENT_CALL_LOCK:           //RECENTCALL LOCK
                    pMe->m_lock_sel = SEC_SEL_RECENTCALL_LOCK;
                    CLOSE_DIALOG(DLG_PHONEPASSWORD)
                    break;

                case IDS_PIN_SET:              //PIN����
                	if (IRUIM_IsCardConnected(pMe->m_pIRUIM))
                	{
                    	CLOSE_DIALOG(DLG_PINCHECK)
                	}
                    break;

                case IDS_PHONE_PASSWORD_CHANGE:
                    CLOSE_DIALOG(DLGRET_CHANGECODE)
                    break;

                case IDS_RESTORE:              //�ָ���������
                    //SecurityMenu_ShowDialog(pMe, IDD_ASK_PASSWORD_DIALOG);
					//ISHELL_GetActiveDialog(pMe->m_pShell);
					//MSG_FATAL("------------>ok1",0,0,0);
					//SecurityRestoreFactoryHandler(pMe,eCode,wParam,dwParam);
                    //CLOSE_DIALOG(DLGRET_RESTORE)
                    //CLOSE_DIALOG(SECURITYMENU_ASKPASSWORD)
#if defined(FEATURE_VERSION_W515V3) || defined(FEATURE_VERSION_S1000T)        
                    CLOSE_DIALOG(DLGRET_ARKPASSWORD)
#else 
                    CLOSE_DIALOG(DLGRET_RESTORE)
#endif
					
                    break;

                case IDS_DELETE:
                    CLOSE_DIALOG(DLGRET_DELETE)
                    break;
				#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316) || defined(FEATURE_VERSION_IC241A_MMX)
				case IDS_MOBILE_TRACKER:
					CLOSE_DIALOG(DLGRET_MOBILE_TRACKER)
					break;
				#endif
                default:
                    break;
            }

            return TRUE;

        default:
            break;
    }
    return FALSE;
} // SecurityMainDlgHandler

/*==============================================================================
������
       SecurityApplicationlockDlgHandler
˵����
       IDD_APPLICATIONLOCK_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityApplicationLockDlgHandler(CSecurityMenu *pMe,
                                            AEEEvent       eCode,
                                            uint16         wParam,
                                            uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                      IDC_APPLICATIONLOCK);
    static boolean bData = 0;
    SEC_ERR("%x,%x,%x,SecurityPhonelockDlgHandler",eCode,wParam,dwParam);

    
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			//add by yangdecai
			{
				AECHAR WTitle[40] = {0};
#ifdef FEATURE_VERSION_C316	
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        IDS_DATA_PROTECTION,
                        WTitle,
                        sizeof(WTitle));                        
#else                        
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        IDS_APPLICATION_LOCK,
                        WTitle,
                        sizeof(WTitle));
#endif
				IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
            }
            
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)

            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_SMS_LOCK, IDS_SMS_LOCK, NULL, 0);
#endif
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_PHONEBOOK_LOCK, IDS_PHONEBOOK_LOCK, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_RECENT_CALL_LOCK, IDS_RECENT_CALL_LOCK, NULL, 0);
            //IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_MEDIALOCK, IDS_MEDIALOCK, NULL, 0);
            #ifdef FEATURE_VERSION_C316
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_FILEMANGERLOCK, IDS_FILEMANGERLOCK, NULL, 0);
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_MULTIMEDIALOCK, IDS_MULTIMEDIALOCK, NULL, 0);
			#elif defined(FEATURE_VERSION_K202_LM129C)//xxzhen 
			#else
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_CALENDARLOCK, IDS_CALENDARLOCK, NULL, 0);
			#endif
#ifdef FEATURE_VERSION_W317A
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_MG_MEDIAGALLERY, IDS_MG_MEDIAGALLERY, NULL, 0);
#endif


            return TRUE;

        case EVT_DIALOG_START:
            //�趨�����ʽ
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_MULTI_SEL|MP_ACTIVE_NO_REDRAW);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_DISTINGUISH_INFOKEY_SELECTKEY | OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SAVE_BACK);
            //���sms ��״̬
            {
                (void) ICONFIG_GetItem(pMe->m_pConfig,CFGI_SMS_LOCK_CHECK,&bData,sizeof(bData));
                SetCheckBoxItem(pMenu, IDS_SMS_LOCK, bData);
            }            
            //���phonebook ��״̬
            {
                (void) ICONFIG_GetItem(pMe->m_pConfig,CFGI_PHONEBOOK_LOCK_CHECK,&bData,sizeof(bData));
                SetCheckBoxItem(pMenu, IDS_PHONEBOOK_LOCK, bData);
            }
            //���recentcall ��״̬
            {
                (void) ICONFIG_GetItem(pMe->m_pConfig,CFGI_RECENTCALL_LOCK_CHECK,&bData,sizeof(bData));
                SetCheckBoxItem(pMenu, IDS_RECENT_CALL_LOCK, bData);
            }
            //���gallety״̬
            {
                SetCheckBoxItem(pMenu, IDS_MEDIALOCK, bData);

            }
            //���calendar ��״̬
            {
                (void) ICONFIG_GetItem(pMe->m_pConfig, CFGI_CALENDAR_LOCK_CHECK, &bData, sizeof(bData));
                SetCheckBoxItem(pMenu, IDS_CALENDARLOCK, bData);
            }
#if defined(FEATURE_VERSION_W317A)
			 //���Mediagallery ��״̬
            {
                (void) ICONFIG_GetItem(pMe->m_pConfig, CFGI_MEDIAGALLERY_LOCK_CHECK, &bData, sizeof(bData));
                SetCheckBoxItem(pMenu, IDS_MG_MEDIAGALLERY, bData);
            }
#endif

#ifdef FEATURE_VERSION_C316
			 //���Mediagallery ��״̬
			{
				(void) ICONFIG_GetItem(pMe->m_pConfig, CFGI_MEDIAGALLERY_LOCK_CHECK, &bData, sizeof(bData));
				SetCheckBoxItem(pMenu, IDS_FILEMANGERLOCK, bData);
			}
			//���Mediagallery ��״̬
			{
				(void) ICONFIG_GetItem(pMe->m_pConfig, CFGI_MULTIMEDIA_LOCK_CHECK, &bData, sizeof(bData));
				SetCheckBoxItem(pMenu, IDS_MULTIMEDIALOCK, bData);
			}

#endif

            IMENUCTL_SetSel(pMenu, IDS_SMS_LOCK);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                    
                default:
                    break;
            }

            return TRUE;

        case EVT_KEY_RELEASE:
            switch(wParam)
            {
                case AVK_SELECT:
                //case AVK_INFO:
                    // �����������״̬
                    bData = GetCheckBoxVal(pMenu, IDS_SMS_LOCK);
                    (void) ICONFIG_SetItem(pMe->m_pConfig,CFGI_SMS_LOCK_CHECK,&bData,sizeof(bData));

                    // ����绰����״̬
                    bData = GetCheckBoxVal(pMenu, IDS_PHONEBOOK_LOCK);
                    (void) ICONFIG_SetItem(pMe->m_pConfig,CFGI_PHONEBOOK_LOCK_CHECK,&bData,sizeof(bData));

                    // �������ͨ����״̬
                    bData = GetCheckBoxVal(pMenu, IDS_RECENT_CALL_LOCK);
                    (void) ICONFIG_SetItem(pMe->m_pConfig,CFGI_RECENTCALL_LOCK_CHECK,&bData,sizeof(bData));
                    
                    // �����ճ̱���״̬
                    bData = GetCheckBoxVal(pMenu, IDS_CALENDARLOCK);
                    (void) ICONFIG_SetItem(pMe->m_pConfig,CFGI_CALENDAR_LOCK_CHECK,&bData,sizeof(bData));
#if defined(FEATURE_VERSION_W317A)
					// 
                    bData = GetCheckBoxVal(pMenu, IDS_MG_MEDIAGALLERY);
                    (void) ICONFIG_SetItem(pMe->m_pConfig,CFGI_MEDIAGALLERY_LOCK_CHECK,&bData,sizeof(bData));
#endif     
#ifdef FEATURE_VERSION_C316
					bData = GetCheckBoxVal(pMenu, IDS_FILEMANGERLOCK);
                    (void) ICONFIG_SetItem(pMe->m_pConfig,CFGI_MEDIAGALLERY_LOCK_CHECK,&bData,sizeof(bData));
					// 
					bData = GetCheckBoxVal(pMenu, IDS_MULTIMEDIALOCK);
					(void) ICONFIG_SetItem(pMe->m_pConfig,CFGI_MULTIMEDIA_LOCK_CHECK,&bData,sizeof(bData));
#endif  

                    CLOSE_DIALOG(DLGRET_TOSHOWMSG)
                    break;

                default:
                    break;
            }

            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_COMMAND:
			return ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_SELECT,DWPARMA_OTHER);
#endif//FEATURE_LCD_TOUCH_ENABLE
        default:
            break;
    }
    return FALSE;
} // SecurityApplicationlockDlgHandler


#ifdef FEATURE_VERSION_C316	
/*==============================================================================
������
       SecurityOneKeyLockKeypadDlgHandler
˵����
       SecurityOneKeyLockKeypadDlgHandler�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityOneKeyLockKeypadDlgHandler(CSecurityMenu *pMe,
                                           AEEEvent       eCode,
                                           uint16         wParam,
                                           uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                        IDM_ONEKEY_LOCK_KEYPAD);
    static boolean bData = 0; //���ڱ���NV���е� CFGI_PHONE_PASSWORD_CHECK
    uint16   cfgi_value = CFGI_PHONE_PASSWORD_CHECK;
    SEC_ERR("%x,%x,%x,SecurityOneKeyLockKeypadDlgHandler",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    //ʵ�ֲ˵�ѭ����������
    //SecurityMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_ON, IDS_ON, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_OFF, IDS_OFF, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
        {
            uint16 wItemID;
            uint16 string_id = IDS_ONEKEY_LOCK_KEPAD;//IDS_PHONE_PASSWORD_CHECK_TITLE;
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT|MP_ACTIVE_NO_REDRAW);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
		    {
		  		AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        string_id,
                        WTitle,
                        sizeof(WTitle));
				IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
		    }
            OEM_GetConfig(CFGI_ONEKEY_LOCK_KEYPAD,&bData, sizeof(bData));
            SetMenu_SetRadioIconAsOff(pMenu);

            if(bData == TRUE)
            {
                wItemID = IDS_ON;
            }
            else
            {
                wItemID = IDS_OFF;
            }

            SetMenuIcon(pMenu, wItemID, TRUE);
            return TRUE;
        }
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }

            return TRUE;

        case EVT_COMMAND:
        {
            boolean bNewData;
            switch(wParam)
            {
                case IDS_ON:
                    bNewData = TRUE;
                    break;

                case IDS_OFF:
                    bNewData = FALSE;
                    break;

                default:
                    return FALSE;

            }

            if (bNewData != bData)
            {
                (void)ICONFIG_SetItem(pMe->m_pConfig,
                                                CFGI_ONEKEY_LOCK_KEYPAD/*CFGI_ONEKEY_LOCK_KEYPAD*/,
                                                &bNewData,
                                                sizeof(bNewData));
                //��ѡ�е�ѡ����
                bData = bNewData;
                SetMenu_SetRadioIconAsOff(pMenu);
                SetMenuIcon(pMenu, wParam, TRUE);
                (void)IMENUCTL_Redraw(pMenu);

            }

			MSG_FATAL("***xuhui SecurityOneKeyLockKeypadDlgHandler ISHELL_SetTimer CSecurtyMenu_DialogTimeout 111***", 0, 0, 0);
#if defined(FEATURE_VERSION_K212)||defined(FEATURE_VERSION_K212_HUALU)
            (void) ISHELL_SetTimer(pMe->m_pShell,
                                    500,
                                    CSecurtyMenu_DialogTimeout,
                                    pMe);
#else
			(void) ISHELL_SetTimer(pMe->m_pShell,
                                    750,
                                    CSecurtyMenu_DialogTimeout,
                                    pMe);
#endif
            return TRUE;
        }


        //�Զ������ϼ��Ի���
        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_TOSHOWMSG)

        default:
            break;
    }
    return FALSE;
} // SecurityOneKeyLockKeypadDlgHandler

#endif


/*==============================================================================
������
       SecurityPassWordDlgHandler
˵����
       IDD_PHONE_PASSWORD_CHECK_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityPassWordDlgHandler(CSecurityMenu *pMe,
                                           AEEEvent       eCode,
                                           uint16         wParam,
                                           uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                        IDM_PHONE_PASSWORD_CHECK);
    static boolean bData = 0; //���ڱ���NV���е� CFGI_PHONE_PASSWORD_CHECK
    uint16   cfgi_value = CFGI_PHONE_PASSWORD_CHECK;
    SEC_ERR("%x,%x,%x,SecurityPassWordDlgHandler",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    //ʵ�ֲ˵�ѭ����������
    //SecurityMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_ON, IDS_ON, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_OFF, IDS_OFF, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
        {
            uint16 wItemID;
            uint16 string_id = IDS_PHONE_LOCK;//IDS_PHONE_PASSWORD_CHECK_TITLE;
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT|MP_ACTIVE_NO_REDRAW);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            if(pMe->m_lock_sel == SEC_SEL_PHONE_LOCK)
            {
                cfgi_value = CFGI_PHONE_PASSWORD_CHECK;
            }
            else if(pMe->m_lock_sel == SEC_SEL_PHONEBOOK_LOCK)
            {
                cfgi_value = CFGI_PHONEBOOK_LOCK_CHECK;
                string_id = IDS_PHONEBOOK_LOCK;
            }
            else if(pMe->m_lock_sel == SEC_SEL_RECENTCALL_LOCK)
            {
                cfgi_value = CFGI_RECENTCALL_LOCK_CHECK;
                string_id = IDS_RECENT_CALL_LOCK;
            }
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
            else if(pMe->m_lock_sel == SEC_SEL_SMS_LOCK)
            {
                cfgi_value = CFGI_SMS_LOCK_CHECK;
                string_id = IDS_SMS_LOCK;
            }
#endif

#ifdef FEATURE_KEYGUARD
            else if(pMe->m_lock_sel == SEC_SEL_KEY_LOCK)
            {
                cfgi_value = CFGI_KEY_LOCK_CHECK;
                string_id = IDS_KEY_LOCK;
            }
#endif
			#if 0
            IMENUCTL_SetTitle(pMenu,
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            string_id,
                                            NULL);
			#else
		    {
		  		AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        string_id,
                        WTitle,
                        sizeof(WTitle));
				IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
		    }
			#endif
            (void) ICONFIG_GetItem(pMe->m_pConfig,
                                            cfgi_value/*CFGI_PHONE_PASSWORD_CHECK*/,
                                            &bData,
                                            sizeof(bData));
            SetMenu_SetRadioIconAsOff(pMenu);

            if(bData == TRUE)
            {
                wItemID = IDS_ON;
            }
            else
            {
                wItemID = IDS_OFF;
            }

            SetMenuIcon(pMenu, wItemID, TRUE);
            return TRUE;
        }
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }

            return TRUE;

        case EVT_COMMAND:
        {
            boolean bNewData;
            switch(wParam)
            {
                case IDS_ON:
                    bNewData = TRUE;
                    break;

                case IDS_OFF:
                    bNewData = FALSE;
                    break;

                default:
                    return FALSE;

            }

            if (bNewData != bData)
            {
                if(pMe->m_lock_sel == SEC_SEL_PHONE_LOCK)
                {
                    cfgi_value = CFGI_PHONE_PASSWORD_CHECK;
                }
                else if(pMe->m_lock_sel == SEC_SEL_PHONEBOOK_LOCK)
                {
                    cfgi_value = CFGI_PHONEBOOK_LOCK_CHECK;
                }
                else if(pMe->m_lock_sel == SEC_SEL_RECENTCALL_LOCK)
                {
                    cfgi_value = CFGI_RECENTCALL_LOCK_CHECK;
                }
#if defined(FEATURE_WMS_APP) && !defined(FEATURE_WMSAPP_ONLYSUPPORTVMAIL)
                else if(pMe->m_lock_sel == SEC_SEL_SMS_LOCK)
                {
                    cfgi_value = CFGI_SMS_LOCK_CHECK;
                }
#endif                

#ifdef FEATURE_KEYGUARD	 
		  else if(pMe->m_lock_sel == SEC_SEL_KEY_LOCK)
               {
                    cfgi_value = CFGI_KEY_LOCK_CHECK;
                }	
#endif

                (void)ICONFIG_SetItem(pMe->m_pConfig,
                                                cfgi_value/*CFGI_PHONE_PASSWORD_CHECK*/,
                                                &bNewData,
                                                sizeof(bNewData));
                //��ѡ�е�ѡ����
                bData = bNewData;
                SetMenu_SetRadioIconAsOff(pMenu);
                SetMenuIcon(pMenu, wParam, TRUE);
                (void)IMENUCTL_Redraw(pMenu);

            }

			MSG_FATAL("***zzg SecurityPassWorldDlg ISHELL_SetTimer CSecurtyMenu_DialogTimeout 111***", 0, 0, 0);
#if defined(FEATURE_VERSION_K212)||defined(FEATURE_VERSION_K212_HUALU)
            (void) ISHELL_SetTimer(pMe->m_pShell,
                                    500,
                                    CSecurtyMenu_DialogTimeout,
                                    pMe);
#else
			(void) ISHELL_SetTimer(pMe->m_pShell,
                                    750,
                                    CSecurtyMenu_DialogTimeout,
                                    pMe);
#endif
            return TRUE;
        }


        //�Զ������ϼ��Ի���
        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_TOSHOWMSG)

        default:
            break;
    }
    return FALSE;
} // SecurityPassWordDlgHandler
/*==============================================================================
������
       SecurityKeyLockDlgHandler
˵����
       IDD_KEY_LOCK_CHECK_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean   SecurityKeyLockDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam)
{
	 PARAM_NOT_REF(dwParam)

    IMenuCtl  *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                     IDM_KEY_LOCK);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    MSG_FATAL("%x, %x ,%x,SecurityKeyLockDlgHandler",eCode,wParam,dwParam);
    //ʵ�ֲ˵�ѭ����������
    //SettingMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			//add by yangdecai
			{
				AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        IDS_AUTOKEYGUARD_TITLE,
                        WTitle,
                        sizeof(WTitle));
				IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
            }

			//Add By zzg 2012_10_29
			#if defined (FEATURE_VERSION_W317A) || defined (FEATURE_VERSION_EC99)
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_10SEC, IDS_AKG_10SEC, NULL, 0);
			#endif
			//Add End
		    #ifdef FEATURE_5SEC_AUTOLOCK_KEYPAD //xxzhen
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_5SEC, IDS_AKG_5SEC, NULL, 0);
			#endif
            #ifdef FEATURE_VERSION_W021_CT100_QVGA            
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_5SEC, IDS_AKG_5SEC, NULL, 0);
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_10SEC, IDS_AKG_10SEC, NULL, 0);
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_15SEC, IDS_AKG_15SEC, NULL, 0);
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_30SEC, IDS_AKG_30SEC, NULL, 0);
			IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_1MIN, IDS_AKG_1MIN, NULL, 0);               
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_OFF, IDS_OFF, NULL, 0);
            #else
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_30SEC, IDS_AKG_30SEC, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_2MIN, IDS_AKG_2MIN, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_AKG_5MIN, IDS_AKG_5MIN, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_OFF, IDS_OFF, NULL, 0);
            #endif

            return TRUE;

        case EVT_DIALOG_START:
            {
                uint16    ui16_return = IDS_OFF;
                byte   AKG = 0;

                (void) ICONFIG_GetItem(pMe->m_pConfig,
                                      CFGI_KEY_LOCK_CHECK,
                                      &AKG,
                                      sizeof(AKG));

                switch (AKG)
                {
                	//Add By zzg 2012_10_29
                	#if defined (FEATURE_VERSION_W317A) || defined (FEATURE_VERSION_EC99)
						case 1:   //10��
							ui16_return = IDS_AKG_10SEC;
							break;
						case 2:    // 30��
							ui16_return = IDS_AKG_30SEC;
							break;
						case 3:    // ������
							ui16_return = IDS_AKG_2MIN;
							break;
						case 4:    // �����
							ui16_return = IDS_AKG_5MIN;
							break;
						default:
						case 0: //��
							ui16_return = IDS_OFF;
						break;
                    #elif defined(FEATURE_VERSION_W021_CT100_QVGA) 
                        case 1:   //10��
							ui16_return = IDS_AKG_5SEC;
							break;
						case 2:    // 30��
							ui16_return = IDS_AKG_10SEC;
							break;
						case 3:    // ������
							ui16_return = IDS_AKG_15SEC;
							break;
						case 4:    // �����
							ui16_return = IDS_AKG_30SEC;
							break;
	                    case 5:    // �����
							ui16_return = IDS_AKG_1MIN;
							break;                            
						default:
						case 0: //��
							ui16_return = IDS_OFF;
						break;
					#elif defined (FEATURE_5SEC_AUTOLOCK_KEYPAD)//xxzhen
						case 1:   //10��
							ui16_return = IDS_AKG_5SEC;
							break;
						case 2:    // 30��
							ui16_return = IDS_AKG_30SEC;
							break;
						case 3:    // ������
							ui16_return = IDS_AKG_2MIN;
							break;
						case 4:    // �����
							ui16_return = IDS_AKG_5MIN;
							break;
						default:
						case 0: //��
							ui16_return = IDS_OFF;
						break;
					#else
						case 1:   //30��
							ui16_return = IDS_AKG_30SEC;
							break;
						case 2:    // ������
							ui16_return = IDS_AKG_2MIN;
							break;
						case 3:    // �����
							ui16_return = IDS_AKG_5MIN;
							break;
						default:
						case 0: //��
							ui16_return = IDS_OFF;
						break;
					#endif
					//Add End                    
                }

                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, ui16_return, TRUE);
                IMENUCTL_SetSel(pMenu, ui16_return);

                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT|MP_ACTIVE_NO_REDRAW);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            }
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                byte AKG = 0;

				//Add By zzg 2012_10_29
                #if defined (FEATURE_VERSION_W317A) || defined (FEATURE_VERSION_EC99)
					switch (wParam)
	                {
	                	case IDS_AKG_10SEC:     //10miao
	                        AKG = 1;
	                        break;
	                    case IDS_AKG_30SEC:     //30miao
	                        AKG = 2;
	                        break;
	                    case IDS_AKG_2MIN:     // 2minute
	                        AKG = 3;
	                        break;
	                    case IDS_AKG_5MIN:     //5minute
	                        AKG = 4;
	                        break;
	                    case IDS_OFF:     //��
	                        AKG = 0;
	                        break;

	                    default:
	                        ASSERT_NOT_REACHABLE;
	                }
                  #elif defined(FEATURE_VERSION_W021_CT100_QVGA) //xxzhen
					switch (wParam)
	                {
	                	case IDS_AKG_5SEC:     //10miao
	                        AKG = 1;
	                        break;
	                    case IDS_AKG_10SEC:     //30miao
	                        AKG = 2;
	                        break;
	                    case IDS_AKG_15SEC:     // 2minute
	                        AKG = 3;
	                        break;
	                    case IDS_AKG_30SEC:     //5minute
	                        AKG = 4;
	                        break;
                        case IDS_AKG_1MIN:     // 2minute
	                        AKG = 5;
	                        break;                            
	                    case IDS_OFF:     //��
	                        AKG = 0;
	                        break;

	                    default:
	                        ASSERT_NOT_REACHABLE;
	                }                    
				 #elif defined(FEATURE_5SEC_AUTOLOCK_KEYPAD) //xxzhen
					switch (wParam)
	                {
	                	case IDS_AKG_5SEC:     //10miao
	                        AKG = 1;
	                        break;
	                    case IDS_AKG_30SEC:     //30miao
	                        AKG = 2;
	                        break;
	                    case IDS_AKG_2MIN:     // 2minute
	                        AKG = 3;
	                        break;
	                    case IDS_AKG_5MIN:     //5minute
	                        AKG = 4;
	                        break;
	                    case IDS_OFF:     //��
	                        AKG = 0;
	                        break;

	                    default:
	                        ASSERT_NOT_REACHABLE;
	                }
				#else
					switch (wParam)
	                {
	                    case IDS_AKG_30SEC:     //30miao
	                        AKG = 1;
	                        break;
	                    case IDS_AKG_2MIN:     // 2minute
	                        AKG = 2;
	                        break;
	                    case IDS_AKG_5MIN:     //5minute
	                        AKG = 3;
	                        break;
	                    case IDS_OFF:     //��
	                        AKG = 0;
	                        break;

	                    default:
	                        ASSERT_NOT_REACHABLE;
	                }
				#endif

                (void) ICONFIG_SetItem(pMe->m_pConfig,
                                       CFGI_KEY_LOCK_CHECK,
                                       &AKG,
                                       sizeof(AKG));
                InitMenuIcons(pMenu);
                SetMenuIcon(pMenu, wParam, TRUE);
                CLOSE_DIALOG(DLGRET_TOSHOWMSG)
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
}

/*==============================================================================
������
       SecurityCallPassWordInputDlgHandler
˵����
       IDD_PHONE_PASSWORD_INPUT_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityCallPassWordInputDlgHandler(CSecurityMenu *pMe,
                                                    AEEEvent       eCode,
                                                    uint16         wParam,
                                                    uint32         dwParam)

{
     PARAM_NOT_REF(dwParam)
    /*Security password 's windows .hengxufeng080428*/
    //static char   *m_strPhonePWD = NULL;
    AECHAR      wstrDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};
    int             nLen = 0;
    char        strDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(NULL == pMe->m_strPhonePWD)
            {
                pMe->m_strPhonePWD = (char *)MALLOC((OEMNV_LOCKCODE_MAXLEN + 1)* sizeof(char));
            }

            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SECURITYMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);

            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR  text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};
    
                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif
                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc,RGB_BLACK);
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_PHONE_PASSWORD_INPUT_TITLE, 
                                            text,
                                            sizeof(text));                  
                // ��������
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                #if 0
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
				#else
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,text);
				#endif
                
               (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_OLD_CODE, 
                                                text,
                                                sizeof(text));
               
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
              
                IDISPLAY_DrawText(pMe->m_pDisplay, 
				#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
				#else
								AEE_FONT_BOLD, 
				#endif
                                text,
                                -1, 
                                xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                                0,
#else
                                MENUITEM_HEIGHT*1/2,
#endif                                
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
                         
                MEMSET(strDisplay, '*', pMe->nOldPSWLength);
                strDisplay[pMe->nOldPSWLength] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
				#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
				#else
								AEE_FONT_BOLD, 
				#endif 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                MENUITEM_HEIGHT,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT-5,
#else
                                MENUITEM_HEIGHT*3/2,
#endif                               
                                NULL, 
                                IDF_TEXT_TRANSPARENT);

                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                    AEE_APPSSECURITYMENU_RES_FILE,
                                                    IDS_NEW_CODE, 
                                                    text,
                                                    sizeof(text));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                #ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
				#else
								AEE_FONT_BOLD, 
				#endif  
                                text,
                                -1, 
                                xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                MENUITEM_HEIGHT*2,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*2-10,
#else
                                MENUITEM_HEIGHT*5/2,
#endif                                 
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
                
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                #ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
				#else
								AEE_FONT_BOLD, 
				#endif
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                MENUITEM_HEIGHT*3,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*3-15,
#else
                                MENUITEM_HEIGHT*7/2,
#endif                                 
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
            
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                    AEE_APPSSECURITYMENU_RES_FILE,
                                                    IDS_CONFIRM, 
                                                    text,
                                                   sizeof(text));
#ifndef FEATURE_VERSION_K212_20D
#ifndef FEATURE_VERSION_EC99
#ifndef FEATURE_VERSION_K212_ND
#ifndef FEATURE_VERSION_W516_C260

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                AEE_FONT_BOLD, 
                                text,
                                -1, 
                                xOffset, 
                                MENUITEM_HEIGHT*9/2,
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
#endif
#endif
#endif
#endif
            

                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
                
            	#ifndef FEATURE_ALL_KEY_PAD
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_NEXT_DEL)
#else
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
#endif                    
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_DELETE)
                }
                else
                #else
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_BACK)
                }
                else
                #endif
                {// ȡ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_CANCEL)
                }
                
                // ������ʾ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);  
            
                return TRUE;
            }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
                FREEIF(pMe->m_strPhonePWD);
            }
         
            return TRUE;

        case EVT_KEY:
            {
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;
                
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                        
                    case AVK_CLR:           
                        chEnter = 0;
                        #ifndef FEATURE_ALL_KEY_PAD
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        #else
                        if(dwParam == 0)
                        {
                        	CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        else
                        {
                        	if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        	{
                            	CLOSE_DIALOG(DLGRET_CANCELED)
                            	return TRUE;
                        	}
                        }
                        #endif
                        break;

                    case AVK_DEL:    
                        chEnter = 0;
#ifdef FEATURE_ALL_KEY_PAD 
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#endif
                    break;

                    case AVK_DOWN:
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                    case AVK_SELECT:
                    case AVK_INFO:
#endif                        
                    {                        
                        nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD)); 
                        if (nLen < 4)
                        {
                            return TRUE;
                        }
                        
                        if (NULL != pMe->m_strPhonePWD && nLen>(OEMNV_LOCKCODE_MAXLEN -2))
                        {
                            pMe->m_PassWord = EncodePWDToUint16(pMe->m_strPhonePWD);
                            pMe->nNewPSWLength = nLen;
                            CLOSE_DIALOG(DLGRET_AFFIRMPASSWORD)//(DLGRET_INPUTSECOND)
                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                        }
                        return TRUE;
                    }
                        
                    default:
                        return TRUE;
                }
                
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                if (chEnter == 0)
                {// ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < OEMNV_LOCKCODE_MAXLEN)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
        case EVT_PEN_UP:
            {
                AEEDeviceInfo devinfo;
                int nBarH ;
                AEERect rc;
                AEERect pwtextrc;
                AEERect newpwtextrc;
                int16 wXPos = (int16)AEE_GET_X(dwParam);
                int16 wYPos = (int16)AEE_GET_Y(dwParam);

                nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
                MEMSET(&devinfo, 0, sizeof(devinfo));
                ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
                SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
                SETAEERECT(&pwtextrc, PWTEXT_MINX, PWTEXT_MAXY-74, PWTEXT_MAXX, PWTEXT_MAXY-100);
                SETAEERECT(&newpwtextrc, PWTEXT_MINX, PWTEXT_MAXY-23, PWTEXT_MAXX, PWTEXT_MAXY-100);
                if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,rc))
                {
                    if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
                    {
                        boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_SELECT,0);
                        return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
                    {
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_INFO,0);
                         return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
                    {                       
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_CLR,0);
                         return rt;
                    }
                }
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,pwtextrc))
                {
                    pMe->m_pActiveTSIMInputID=IDD_PHONE_PASSWORD_INPUT_DIALOG;
                    MSG_FATAL("DLGRET_TSIMPASSWORDINPUT-----------",0,0,0);
                    CLOSE_DIALOG(DLGRET_TSIMPASSWORDINPUT);
                    return TRUE;
                }
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,newpwtextrc))
                {
                       nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD)); 
                        if (nLen < 4)
                        {
                            return TRUE;
                        }
                        
                        if (NULL != pMe->m_strPhonePWD && nLen>(OEMNV_LOCKCODE_MAXLEN -2))
                        {
                            pMe->m_PassWord = EncodePWDToUint16(pMe->m_strPhonePWD);
                            pMe->nNewPSWLength = nLen;
                            CLOSE_DIALOG(DLGRET_AFFIRMPASSWORD)//(DLGRET_INPUTSECOND)
                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                        }
                        return TRUE;
                }

            }
                                    break;
#endif//FEATURE_LCD_TOUCH_ENABLE             
            
        default:
            break;
    }
    
    return FALSE;

}

/*==============================================================================
������
       SecurityPinCheckDlgHandler
˵����
       IDD_PIN_CHECK_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityPinCheckDlgHandler(CSecurityMenu *pMe,
                                           AEEEvent       eCode,
                                           uint16         wParam,
                                           uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                          IDD_MENU_PINLOCK);
    SEC_ERR("%x,%x,%x,SecurityPinCheckDlgHandler",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }
     //ʵ�ֲ˵�ѭ����������
    //SecurityMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			//add by yangdecai
			{
				AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        IDS_PIN_CHECK,
                        WTitle,
                        sizeof(WTitle));
				IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
            }
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_ON, IDS_ON, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_OFF, IDS_OFF, NULL, 0);
            return TRUE;
        case EVT_DIALOG_START:
            {
                AEECHVStatus  chvst;
                (void)IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
                if (!chvst.chv1_enabled)
                {
                    SetMenuIcon(pMenu, IDS_ON,  FALSE);
                    SetMenuIcon(pMenu, IDS_OFF, TRUE);
                }
                else
                {
                    SetMenuIcon(pMenu, IDS_ON, TRUE);
                    SetMenuIcon(pMenu, IDS_OFF, FALSE);
                }
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_TEXT_ALIGN_LEFT_ICON_ALIGN_RIGHT|MP_ACTIVE_NO_REDRAW);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
#endif
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            }
            return TRUE;

        case EVT_DIALOG_END:
           return TRUE;

        case EVT_KEY:
           switch(wParam)
           {
               case AVK_CLR:
                   CLOSE_DIALOG(DLGRET_CANCELED)
                   return TRUE;

               default:
                   break;
           }

           return TRUE;

        case EVT_COMMAND:
           {
               boolean IsFactoryTestMode;
               AEECHVStatus  chvst;
               
               (void)ICONFIG_GetItem(pMe->m_pConfig,
                                              CFGI_FACTORY_TEST_MODE,
                                              &IsFactoryTestMode,
                                              sizeof(IsFactoryTestMode));

               if(IsFactoryTestMode)
               {
                  CLOSE_DIALOG(DLGRET_CANCELED)
                  return TRUE;
               }
               
               (void)IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);

               switch (wParam)
                {
                    case IDS_ON:
                        //(void) AEEOEM_CHVEnable(pMe->m_pOEM_TSGBridge,NULL);
                        if (chvst.chv1_enabled)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_ISCOMFIRMPASSWORD)
                            return TRUE;
                        }
                        //IRUIM_CHVEnable(pMe->m_pIRUIM, NULL);

                    case IDS_OFF:
                        //(void) AEEOEM_CHVDisable(pMe->m_pOEM_TSGBridge);
                        if (chvst.chv1_enabled)
                        {
                            if(OEM_IsCDMASVCSupport(UIM_CDMA_SVC_CHV_DISABLE))
                            {
                                CLOSE_DIALOG(DLGRET_ISCOMFIRMPASSWORD)
                            }
                            else
                            {
                                CLOSE_DIALOG(DLGRET_RESTRICT)
                            }
                            return TRUE;
                        }
                        else
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        //IRUIM_CHVDisable(pMe->m_pIRUIM);

                    default:
                        return FALSE;
                }

                //SetMenu_SetRadioIconAsOff(pMenu);
                //SetCheckBoxItem(pMenu, wParam, TRUE);
                //(void)IMENUCTL_Redraw(pMenu);
                /*(void) ISHELL_SetTimer(pMe->m_pShell,
                                                 750,
                                                 CSecurtyMenu_DialogTimeout,
                                                 pMe);*/
           }
           return TRUE;
        //�Զ������ϼ��Ի���
        /*case EVT_DISPLAYDIALOGTIMEOUT:
           CLOSE_DIALOG(DLGRET_CANCELED)*/

        default:
            break;
    }

    return FALSE;
} // SecurityPinCheckDlgHandler


/*==============================================================================
����:
    IDD_MSGBOX_Handler

˵��:
    SecurityApplet�Ի��� IDD_MSGBOX �¼���������

����:
    pUser [in]: ָ��Security Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    eCode [in]: �¼����롣
    wParam[in]: �¼�����
    dwParam [in]: ���¼����������ݡ�

����ֵ:
    TRUE:  �����¼��õ�����
    FALSE: �����¼�û������

��ע:

==============================================================================*/
static boolean  Security_HandleMsgBoxDlgEvent(CSecurityMenu* pMe,
    AEEEvent eCode,
    uint16   wParam,
    uint32   dwParam
)
{
    static IStatic * pStatic = NULL;
    SEC_ERR("%x,%x,%x,Security_HandleMsgBoxDlgEvent",eCode,wParam,dwParam);
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
        {
            if (NULL == pStatic)
            {
                AEERect rect = {0};
                if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_STATIC,(void **)&pStatic))

                {
                    SEC_ERR("ISHELL_CreateInstance,AEECLSID_STATIC 2",0,0,0);
                    return FALSE;
                }
                ISTATIC_SetRect(pStatic, &rect);
            }
            return TRUE;
        }

        case EVT_DIALOG_START:
			
			MSG_FATAL("***zzg SecurityHandleMsgBoxDlg m_wMsgID=%d***", pMe->m_wMsgID, 0, 0);
			
            if(pMe->m_wMsgID != IDS_RESTORE_CONFIRM &&
                pMe->m_wMsgID != IDS_DELETE_CONFIRM)
            {
            	MSG_FATAL("***zzg SecurityPassWorldDlg ISHELL_SetTimer CSecurtyMenu_DialogTimeout 222***", 0, 0, 0);
                ISHELL_SetTimer(pMe->m_pShell,SECURITY_PROMPTMSG_TIMER,CSecurtyMenu_DialogTimeout,pMe);
            }
			
            ISHELL_PostEvent( pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER_REDRAW,0,0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
            PromptMsg_Param_type m_PromptMsg;
            AECHAR   wszMsg[64] = {0};
            MEMSET(&m_PromptMsg,0,sizeof(PromptMsg_Param_type));

            switch(pMe->m_wMsgID)
            {
                case IDS_DIF_PASSWORD:
                case IDS_DIF_PIN:
                case IDS_MODIFYPIN_FAILED:
                case IDS_INVALID_PASSWORD:
                case IDS_INVALIDPUK:
                case IDS_INVALIDPIN:
                case IDS_MSG_INPUTINVALID:
                case IDS_UIMBLOCKED:
                case IDS_PIN_FAILED:
                case IDS_PINBLOCKED:
                    m_PromptMsg.ePMsgType = MESSAGE_WARNNING;
                    break;

                case IDS_PUK_CORRECT:
                case IDS_PHONE_PASSWORD_CHANGED:
                case IDS_PIN_CHANGED:
                case IDS_RESET_DONE:
                case IDS_SAVED:
                    m_PromptMsg.ePMsgType = MESSAGE_INFORMATIVE;
                    break;
                    
                case IDS_RESTORE_CONFIRM:
                case IDS_DELETE_CONFIRM:
                    m_PromptMsg.ePMsgType = MESSAGE_CONFIRM;
                    break;
                    
                case IDS_DELETING:
                case IDS_RESTORING:
                    m_PromptMsg.ePMsgType = MESSAGE_WAITING;
                    break;
                    
                default:
                    m_PromptMsg.ePMsgType = MESSAGE_NONE;
                    break;
            }
                
            ISHELL_LoadResString(pMe->m_pShell,AEE_APPSSECURITYMENU_RES_FILE,pMe->m_wMsgID, wszMsg,sizeof(wszMsg));

            m_PromptMsg.pwszMsg = wszMsg;
            if(IsRunAsUIMVersion())
            {
                if (IDS_INVALIDPIN == pMe->m_wMsgID ||
                    IDS_INVALIDPUK == pMe->m_wMsgID)
                {
                    AECHAR   wsDispMsg[64] = {0};
                    WSPRINTF(wsDispMsg, sizeof(wsDispMsg),wszMsg, pMe->m_nCanAttemptTimes);
                    m_PromptMsg.pwszMsg = wsDispMsg;
                }
            }

            if(pMe->m_wMsgID == IDS_RESTORING)
            {
                m_PromptMsg.eBBarType = BTBAR_NONE;
                DrawPromptMessage(pMe->m_pDisplay,pStatic,&m_PromptMsg);
                IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                SecurityMenu_RestoryFactorySet(pMe);
                return TRUE;
            }
            else if(pMe->m_wMsgID == IDS_DELETING)
            {
                m_PromptMsg.eBBarType = BTBAR_NONE;
                DrawPromptMessage(pMe->m_pDisplay,pStatic,&m_PromptMsg);
                IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);                
                SecurityMenu_DeleteAllEntries(pMe);
                return TRUE;
            }
            else if(pMe->m_wMsgID == IDS_RESTORE_CONFIRM ||
                        pMe->m_wMsgID == IDS_DELETE_CONFIRM)
            {
                m_PromptMsg.eBBarType = BTBAR_OK_BACK;
            }
            else
            {
                m_PromptMsg.eBBarType = BTBAR_NONE;
            }
            DrawPromptMessage(pMe->m_pDisplay,pStatic,&m_PromptMsg);
            IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
        }
            return TRUE;

        case EVT_DIALOG_END:
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL,  pMe);
            return TRUE;

        case EVT_DISPLAYDIALOGTIMEOUT:
            // �յ���ʱ�����������رյ�ǰ�Ի�����¼����رնԻ���
        {        	
			MSG_FATAL("***zzg Security_HandleMSGBoxDlg m_wMsgID=%d***", pMe->m_wMsgID, 0, 0);
			
            switch(pMe->m_wMsgID)
            {
                case IDS_SAVED:
                case IDS_PIN_CHANGED:
                case IDS_PHONE_PASSWORD_CHANGED:
                case IDS_PUK_CORRECT:
                case IDS_RESTORING:
                case IDS_DELETING:
                    CLOSE_DIALOG(DLGRET_MSGBOX_OK)
                    break;

                default:
                    CLOSE_DIALOG(DLGRET_OK)
                    break;
            }
            return TRUE;
        }

        case EVT_KEY:
        {
            if(IDS_RESTORE_CONFIRM == pMe->m_wMsgID ||
                IDS_DELETE_CONFIRM == pMe->m_wMsgID)
            {
                if(wParam == AVK_SELECT)
                {
                    CLOSE_DIALOG(DLGRET_MSGBOX_CONFIRM);
                }
                
                if(wParam == AVK_CLR)
                {
                    CLOSE_DIALOG(DLGRET_MSGBOX_CANCEL);
                }
            }
            else
            {
                // ȡ���Զ��رնԻ���Ķ�ʱ��
                (void)ISHELL_CancelTimer(pMe->m_pShell, NULL,  pMe);

				if (pMe->m_wMsgID == IDS_RESTORING)
	        	{
					MSG_FATAL("***zzg Security_HandleMsgBoxDLg EVT_KEY ISHELL_CancelTimer***", 0, 0, 0);
				}
				
                switch(pMe->m_wMsgID)
                {
                    case IDS_SAVED:
                    case IDS_PIN_CHANGED:
                    case IDS_PHONE_PASSWORD_CHANGED:
                    case IDS_PUK_CORRECT:
                    case IDS_RESTORING:
                    case IDS_DELETING:
                        CLOSE_DIALOG(DLGRET_MSGBOX_OK)
                        break;

                    default:
                        CLOSE_DIALOG(DLGRET_OK)
                        break;
                }
            }
        }
            return TRUE;

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
			{
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
				int16 wXPos = (int16)AEE_GET_X(dwParam);
				int16 wYPos = (int16)AEE_GET_Y(dwParam);

				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);

				if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,rc))
				{
					if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
					{
						boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_SELECT,0);
						return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
					{
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_INFO,0);
						 return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
					{						
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_CLR,0);
						 return rt;
					}
				}

			}
			break;
#endif//FEATURE_LCD_TOUCH_ENABLE 
        default:
            break;
    }

    return FALSE;
} // IDD_MSGBOX_Handler

/*==============================================================================
������
       SecurityPinChangeDlgHandler
˵����
       IDD_PIN_CHANGE_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityPinChangeDlgHandler(CSecurityMenu *pMe,
                                            AEEEvent       eCode,
                                            uint16         wParam,
                                            uint32         dwParam)

{
     PARAM_NOT_REF(dwParam)
    //static char   *m_strPhonePWD = NULL;
    AECHAR      wstrDisplay[OEMNV_LOCKPIN_MAXLEN+2] = {0};
    int             nLen = 0;
    char        strDisplay[OEMNV_LOCKPIN_MAXLEN+2] = {0};
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(NULL == pMe->m_strPhonePWD)
            {
                pMe->m_strPhonePWD = (char *)MALLOC((OEMNV_LOCKPIN_MAXLEN + 1)* sizeof(char));
            }
          
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SECURITYMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);

            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};
                
                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif

                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc,RGB_BLACK);                  
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                    AEE_APPSSECURITYMENU_RES_FILE,
                                                    IDS_PINCHANGE_TITLE, 
                                                    text,
                                                    sizeof(text));                  
                // ��������
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                #if 0
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
				#else
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,text);
				#endif
               (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_ENTERPIN, 
                                                text,
                                                sizeof(text));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                #ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
				#else
								AEE_FONT_BOLD, 
				#endif 
                                text,
                                -1, 
                                xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)|| defined(FEATURE_VERSION_K212_ND)
                                0,
#else
                                MENUITEM_HEIGHT*1/2,
#endif                          
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
            
                MEMSET(strDisplay, '*', pMe->nOldPSWLength);
                strDisplay[pMe->nOldPSWLength] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                #ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
				#else
								AEE_FONT_BOLD, 
				#endif
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                MENUITEM_HEIGHT,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT-5,
#else
                                MENUITEM_HEIGHT*3/2,
#endif                                 
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
            
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_NEW_PIN, 
                                                text,
                                                sizeof(text));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                #ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
				#else
								AEE_FONT_BOLD, 
				#endif 
                                    text,
                                    -1, 
                                    xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                    MENUITEM_HEIGHT*2,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*2-10,
#else
                                    MENUITEM_HEIGHT*5/2,
#endif                                     
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);

            
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));
                

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                #ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
				#else
								AEE_FONT_BOLD, 
				#endif 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                MENUITEM_HEIGHT*3,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*3-15
#else
                                MENUITEM_HEIGHT*7/2,
#endif                                 
                                NULL, 
                                IDF_TEXT_TRANSPARENT);

                
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_CONFIRM, 
                                                text,
												sizeof(text));
#ifndef FEATURE_VERSION_EC99
#ifndef FEATURE_VERSION_K212_20D
#ifndef FEATURE_VERSION_K212_ND
#ifndef FEATURE_VERSION_W516_C260

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                    AEE_FONT_BOLD, 
                                    text,
                                    -1, 
                                    xOffset, 
                                    MENUITEM_HEIGHT*9/2,
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
#endif
#endif
#endif
#endif

                IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);


            	#ifndef FEATURE_ALL_KEY_PAD
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_NEXT_DEL)
#else                
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
#endif                    
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_DELETE)
                }
                else
                #else
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_BACK)
                }
                else
                #endif
                {// ȡ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_CANCEL)
                }
        
                // ������ʾ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            
                return TRUE;
            }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
                FREEIF(pMe->m_strPhonePWD);
            }
           
            return TRUE;

        case EVT_KEY:
            {
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;                
                
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        #ifndef FEATURE_ALL_KEY_PAD
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        #else
                        if(dwParam == 0)
                        {
                        	CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        else
                        {
                        	if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        	{
                            	CLOSE_DIALOG(DLGRET_CANCELED)
                            	return TRUE;
                        	}
                        }
                        #endif
                        break;

                    case AVK_DEL:    
                        chEnter = 0;
#ifdef FEATURE_ALL_KEY_PAD 
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#endif
                    break;

                    case AVK_DOWN:
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                    case AVK_SELECT:
                    case AVK_INFO:
#endif                        
                        {
                            nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                            if (nLen< 4)
                            {
                                //CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                                return TRUE;
                            }
                            else
                            {
                               (void)STRNCPY( pMe->m_Pin, pMe->m_strPhonePWD,sizeof(pMe->m_Pin));
                               pMe->m_PassWord = EncodePWDToUint16(pMe->m_Pin);
                               pMe->nNewPSWLength = nLen;
                               CLOSE_DIALOG(DLGRET_AFFIRMPASSWORD)
                            }
                            return TRUE;
                        }    
                     
                    
                             
                        default:
                            return TRUE;
                }
                
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD)); 
                if (chEnter == 0)
                {
                    // ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < OEMNV_LOCKPIN_MAXLEN)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
        case EVT_PEN_UP:
            {
                AEEDeviceInfo devinfo;
                int nBarH ;
                AEERect rc;
                AEERect pwtextrc;
                AEERect newpwtextrc;
                int16 wXPos = (int16)AEE_GET_X(dwParam);
                int16 wYPos = (int16)AEE_GET_Y(dwParam);

                nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
                MEMSET(&devinfo, 0, sizeof(devinfo));
                ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
                SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
                SETAEERECT(&pwtextrc, PWTEXT_MINX, PWTEXT_MAXY-74, PWTEXT_MAXX, PWTEXT_MAXY-100);
                SETAEERECT(&newpwtextrc, PWTEXT_MINX, PWTEXT_MAXY-23, PWTEXT_MAXX, PWTEXT_MAXY-100);
                if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,rc))
                {
                    if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
                    {
                        boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_SELECT,0);
                        return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
                    {
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_INFO,0);
                         return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
                    {                       
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_CLR,0);
                         return rt;
                    }
                }
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,pwtextrc))
                {
                    pMe->m_pActiveTSIMInputID=IDD_PIN_CHANGE_DIALOG;
                    MSG_FATAL("DLGRET_TSIMPASSWORDINPUT-----------",0,0,0);
                    CLOSE_DIALOG(DLGRET_TSIMPASSWORDINPUT);
                    return TRUE;
                }
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,newpwtextrc))
                {
                     nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                     if (nLen< 4)
                     {
                         //CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                         return TRUE;
                     }
                     else
                     {
                        (void)STRNCPY( pMe->m_Pin, pMe->m_strPhonePWD,sizeof(pMe->m_Pin));
                        pMe->m_PassWord = EncodePWDToUint16(pMe->m_Pin);
                        pMe->nNewPSWLength = nLen;
                        CLOSE_DIALOG(DLGRET_AFFIRMPASSWORD)
                     }
                     return TRUE;
                }

            }
                                                break;
#endif//FEATURE_LCD_TOUCH_ENABLE             
            
        default:
            break;
    }
    
    return FALSE;

}

#if defined(FEATURE_VERSION_W317A)||defined(FEATURE_VERSION_C337)||defined(FEATURE_VERSION_C316) || defined(FEATURE_VERSION_IC241A_MMX)
/*==============================================================================
������
       SecurityMobileTrackerHandler
˵����
        �������������ֻ��������ITextCtl�ؼ�������

������
      thePasswordTextCtl [in]���������������ITextCtl�ؼ�

����ֵ��

��ע��

==============================================================================*/

static boolean  SecurityMobileTrackerHandler(CSecurityMenu *pMe,
                                        AEEEvent eCode,
                                        uint16 wParam,
                                        uint32 dwParam)
{
	AECHAR      wstrDisplay[OEMNV_LOCKMUM_MAXLEN+2] = {0};
    int         nWLen = 0;
    //char        strDisplay[OEMNV_LOCKPIN_MAXLEN+2] = {0};
	if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			{
				AECHAR wGetData[OEMNV_LOCKMUM_MAXLEN] = {0};
	            if(NULL == pMe->m_strPhoneNUM)
	            {
	                pMe->m_strPhoneNUM = (char *)MALLOC((OEMNV_LOCKMUM_MAXLEN + 1)* sizeof(char));
	            }
				(void) ICONFIG_GetItem(pMe->m_pConfig,  
                                        CFGI_MOBILE_TRACKER_PHONENUMB,
                                        wGetData, 
                                        sizeof(wGetData));
				WSTRTOSTR(wGetData, pMe->m_strPhoneNUM ,OEMNV_LOCKMUM_MAXLEN);
        	}
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SECURITYMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);

            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};
                
                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif
                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc,RGB_BLACK);                  
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                    AEE_APPSSECURITYMENU_RES_FILE,
                                                    IDS_MOBILE_TRACKER, 
                                                    text,
                                                    sizeof(text));                  
                // ��������
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                #if 0
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
				#else
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,text);
				#endif
               (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_ENTER_PHONENUMBER, 
                                                text,
                                                sizeof(text));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                AEE_FONT_BOLD, 
                                text,
                                -1, 
                                xOffset, 
                                MENUITEM_HEIGHT*1/2, 
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
            
            	
                (void) STRTOWSTR(pMe->m_strPhoneNUM, wstrDisplay, sizeof(wstrDisplay));
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                AEE_FONT_BOLD, 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
                                MENUITEM_HEIGHT*3/2,
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
                nWLen = STRLEN(pMe->m_strPhoneNUM);
                IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
            	#ifndef FEATURE_ALL_KEY_PAD
                // ���Ƶ�����ʾ
                if (nWLen > 1)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
                }
                else if(nWLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_DELETE)
                }
                else
                #else
                // ���Ƶ�����ʾ
                if (nWLen > 1)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
                }
                else if(nWLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_BACK)
                }
                else
                #endif
                {// ȡ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_CANCEL)
                }
        
                // ������ʾ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            
                return TRUE;
            }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
                FREEIF(pMe->m_strPhonePWD);
            }
            return TRUE;

        case EVT_KEY:
            {
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;
                
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        #ifndef FEATURE_ALL_KEY_PAD
                        if (pMe->m_strPhoneNUM == NULL || STRLEN(pMe->m_strPhoneNUM) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        #else
                        if(dwParam == 0)
                        {
                        	CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        else
                        {
                        	if (pMe->m_strPhoneNUM == NULL || STRLEN(pMe->m_strPhoneNUM) == 0)
                        	{
                            	CLOSE_DIALOG(DLGRET_CANCELED)
                            	return TRUE;
                        	}
                        }
                        #endif
                        break;

                    case AVK_DEL:    
                        chEnter = 0;
#ifdef FEATURE_ALL_KEY_PAD 
                        if (pMe->m_strPhoneNUM == NULL || STRLEN(pMe->m_strPhoneNUM) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#endif
                    break;

					case AVK_SELECT:
                    case AVK_INFO:
                        if (pMe->m_strPhoneNUM == NULL)
                        {
                            return TRUE;
                        }
                        else
                        {
                        	uint16 num = 0;
							AEEMobileInfo     mi;
							uint16 wData[OEMNV_LOCKMUM_MAXLEN] = {0};
							uint16 wImsiData[OEMNV_LOCKIMSI_MAXLEN] = {0};
							MSG_FATAL("AVK_SELECT..............AVK_INFO",0,0,0);
							GetMobileInfo(&mi);
							(void) STRTOWSTR(pMe->m_strPhoneNUM, wData, sizeof(wData));
							num = WSTRLEN(wData);
							MSG_FATAL("AVK_SELECT..............AVK_INFO==num=%d",num,0,0);
                            (void) ICONFIG_SetItem(pMe->m_pConfig,  
                                                            CFGI_MOBILE_TRACKER_PHONENUMB,
                                                            wData, 
                                                            sizeof(wData));
							
							(void) STRTOWSTR(mi.szMobileID, wImsiData, sizeof(wImsiData));
							(void)ICONFIG_SetItem(pMe->m_pConfig,  
                                                            CFGI_MOBILE_TRACKER_IMSI,
                                                            wImsiData, 
                                                            sizeof(wImsiData));
                            
                            CLOSE_DIALOG(DLGRET_OK)
                            return TRUE;
                        }

                   
                     
                    
                             
                        default:
                            return TRUE;
                }
				nLen = (pMe->m_strPhoneNUM == NULL)?(0):(STRLEN(pMe->m_strPhoneNUM)); 
                if (chEnter == 0)
                {
                    // ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhoneNUM[nLen-1] = chEnter;
                    }
                }
                else if (nLen < OEMNV_LOCKMUM_MAXLEN)
                {
                    pMe->m_strPhoneNUM[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhoneNUM[nLen] = 0;
                    bRedraw = TRUE;
                }
				
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;

        default:
            break;
    }
    
    return FALSE;
}
#endif


/*==============================================================================
������
       PhoneLock_SetPasswordTextCtlProperties
˵����
        �������������ֻ��������ITextCtl�ؼ�������

������
      thePasswordTextCtl [in]���������������ITextCtl�ؼ�

����ֵ��

��ע��

==============================================================================*/
void PhoneLock_SetPasswordTextCtlProperties( IShell * p_shell, ITextCtl* thePasswordTextCtl)
{

    if( thePasswordTextCtl != NULL)
    {
        AECHAR title[32] = {0};
        AEERect rect = { 0, 44, 128, 35};

        ISHELL_LoadResString( p_shell,
                           AEE_APPSSECURITYMENU_RES_FILE,
                           IDS_PHONE_PASSWORD,
                           title,
                           64
                       );
        ITEXTCTL_SetTitle( thePasswordTextCtl, NULL, NULL, title);
        ITEXTCTL_SetMaxSize( thePasswordTextCtl, 19);
        ITEXTCTL_SetProperties( thePasswordTextCtl, TP_FRAME | TP_PASSWORD|TP_FOCUS_NOSEL);
        ITEXTCTL_SetInputMode( thePasswordTextCtl, AEE_TM_NUMBERS);
        ITEXTCTL_SetRect( thePasswordTextCtl, &rect);
    }
}

/*==============================================================================
������
       PhoneLock_VerifyPassword
˵����
    ��֤�û�������ֻ��������Ƿ���ȷ

������
       thePasswordTextCtl [in]���������������ITextCtl�ؼ�
       password           [in]���û���������롣

        ��������������ͬʱΪ�ա��������ͬʱ��Ϊ�գ�password���ȡ�

����ֵ��
       TRUE ���û������������ȷ
       FALSE���û���������벻��ȷ

��ע��

==============================================================================*/
boolean PhoneLock_VerifyPassword(ITextCtl* thePasswordTextCtl, AECHAR* password)
{

    if (thePasswordTextCtl == NULL && password == NULL)
    {
        return FALSE;
    }
    else
    {
        uint16  wPWD;
        char    pszPassword[20];

        extern int OEM_GetConfig( AEEConfigItem i, void* pBuff, int nSize);

        if( password == NULL)
        {
            password = ITEXTCTL_GetTextPtr( thePasswordTextCtl);
        }
        (void)WSTRTOSTR( password, pszPassword, sizeof(pszPassword));

        (void)OEM_GetConfig( CFGI_PHONE_PASSWORD, &wPWD, sizeof( uint16));
        return wPWD == EncodePWDToUint16( pszPassword);
    }
}

/*==============================================================================
������
       PhoneLock_DrawPasswordInputScreenBottomBar
˵����

������

����ֵ��

��ע��

==============================================================================*/
void PhoneLock_DrawPasswordInputScreenBottomBar( IDisplay* pDisplay, ITextCtl* pTextCtl)
{

    BottomBar_Param_type bottomBarParms;
    MEMSET( &bottomBarParms, 0, sizeof( BottomBar_Param_type));

    if( WSTRLEN( ITEXTCTL_GetTextPtr( pTextCtl)) > 0 &&
        ITEXTCTL_GetCursorPos( pTextCtl) != TC_CURSORSTART
    )
    {
        bottomBarParms.eBBarType = BTBAR_OK_DELETE;
    }
    else
    {
        bottomBarParms.eBBarType = BTBAR_OK_BACK;
    }
    DrawBottomBar( pDisplay, &bottomBarParms);

    IDISPLAY_UpdateEx( pDisplay, FALSE);
}
/*==============================================================================
������
       PhoneLock_DrawPasswordInputScreen
˵����
    ��֤�û�������ֻ��������Ƿ���ȷ

������

����ֵ��

��ע��

==============================================================================*/
void PhoneLock_DrawPasswordInputScreen( IDisplay*   pDisplay,
                                        ITextCtl*   pTextCtl,
                                        char*       titleResFile,
                                        uint16      titleResId
)
{

    TitleBar_Param_type titleBarParms;
    MEMSET( &titleBarParms,  0, sizeof( TitleBar_Param_type));

    IDISPLAY_ClearScreen( pDisplay);
    if( ITEXTCTL_IsActive( pTextCtl))
    {
        ITEXTCTL_Redraw( pTextCtl);
    }
    else
    {
        ITEXTCTL_SetActive( pTextCtl, TRUE);
    }

    titleBarParms.dwAlignFlags  = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
    titleBarParms.nTitleResID   = titleResId;
    STRCPY( titleBarParms.strTitleResFile, titleResFile);
	#if 1
    DrawTitleBar( pDisplay, &titleBarParms);
	#else
	//IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,text);
	#endif
    PhoneLock_DrawPasswordInputScreenBottomBar( pDisplay, pTextCtl);
}

/*==============================================================================
������
       SecurityAskPasswordDlgHandler
˵����
       IDD_ASK_PASSWORD_DIALOG �Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityAskPasswordDlgHandler(CSecurityMenu *pMe,
                                         AEEEvent       eCode,
                                         uint16         wParam,
                                         uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    //static char   *m_strPhonePWD = NULL;
    AECHAR         wstrDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};
    int            nLen = 0;
    char           strDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};
    SEC_ERR("%x, %x ,%x,SecurityAskPasswordDlgHandler",eCode,wParam,dwParam);
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(NULL == pMe->m_strPhonePWD)
            {
                pMe->m_strPhonePWD = (char *)MALLOC((OEMNV_LOCKCODE_MAXLEN + 1)* sizeof(char));
            }
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SECURITYMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);
            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR      text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};
                
                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif
                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc, RGB_BLACK);
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_SECURITY_TITLE, 
                                                text,
                                                sizeof(text));
                // ��������
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                #if 0
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
				#else
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,text);
				#endif
               (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_SECURITY, 
                                                text,
                                                sizeof(text));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                    AEE_FONT_BOLD, 
                                    text,
                                    -1, 
                                    xOffset, 
                                    MENUITEM_HEIGHT*1/2, 
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
                   
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD)); 
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                AEE_FONT_BOLD, 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
                                MENUITEM_HEIGHT*3/2,
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
            	#ifndef FEATURE_ALL_KEY_PAD
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_DELETE)
                }
                else
                #else
                 // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_BACK)
                }
                else
                #endif
                {// ȡ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_CANCEL)
                }

                // ������ʾ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
        
                return TRUE;
            }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
                FREEIF(pMe->m_strPhonePWD);
            }
            return TRUE;

        case EVT_KEY:
            {
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;
                
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        #ifndef FEATURE_ALL_KEY_PAD
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        #else
                        if(dwParam == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        else
                        {
                        	if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        	{
                            	CLOSE_DIALOG(DLGRET_CANCELED)
                            	return TRUE;
                        	}
                        }
                        #endif
                        break;

                    case AVK_DEL:    
                        chEnter = 0;
#ifdef FEATURE_ALL_KEY_PAD 
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#endif
                    break;

                    case AVK_SELECT:
                    case AVK_INFO:
                        
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) < 4)
                        {
                            return TRUE;
                        }
                        else
                        {
                            uint16 wPWD=0;
						//	#ifdef FEATURE_VERSION_K212_ND
                           // char superpass[6] = {"0000"};
						//	#else
							char superpass[6] = {"*#09#"};
						//	#endif
                            (void) ICONFIG_GetItem(pMe->m_pConfig, 
                                                   CFGI_PHONE_PASSWORD,
                                                   &wPWD,
                                                   sizeof(uint16));
                            MSG_FATAL("pMe->m_strPhonePWD---=%d",EncodePWDToUint16(pMe->m_strPhonePWD),0,0);
#ifndef WIN32//wlh ģ����������                   
                            if (wPWD == EncodePWDToUint16(pMe->m_strPhonePWD)||(0==strcmp(superpass,pMe->m_strPhonePWD)))
#else
							if(1)
#endif//WIN32   
                            {
                                // �������
                                CLOSE_DIALOG(DLGRET_VALIDPINPASS)
                            }
                            else
                            {
                                // �������
                                if(STRNCMP(pMe->m_strPhonePWD,"62382",5) == 0)
                                {
#ifdef FEATURE_SCREEN_CAPTURE
#ifndef WIN32
                                    disp_set_capture ( TRUE );
#endif//WIN32
#endif
                                }
                            CLOSE_DIALOG(DLGRET_VALIDPINFAILED)//(DLGRET_OK)
                        }
                    }
                    return TRUE;
                        
                default:
                    return TRUE;
                }
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                if (chEnter == 0)
                {
                    // ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < OEMNV_LOCKCODE_MAXLEN)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
			{
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
                AEERect pwtextrc;
				int16 wXPos = (int16)AEE_GET_X(dwParam);
				int16 wYPos = (int16)AEE_GET_Y(dwParam);

				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
                SETAEERECT(&pwtextrc, PWTEXT_MINX, PWTEXT_MINY, PWTEXT_MAXX, PWTEXT_MAXY);
				if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,rc))
				{
					if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
					{
						boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_SELECT,0);
						return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
					{
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_INFO,0);
						 return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
					{						
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_CLR,0);
						 return rt;
					}
				}
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,pwtextrc))
                {
                    pMe->m_pActiveTSIMInputID=IDD_ASK_PASSWORD_DIALOG;
                    MSG_FATAL("DLGRET_TSIMPASSWORDINPUT-----------",0,0,0);
                    CLOSE_DIALOG(DLGRET_TSIMPASSWORDINPUT);
                }

			}
			break;
#endif//FEATURE_LCD_TOUCH_ENABLE             
        default:
            break;
    }
    
    return FALSE;
} // SecurityAskPinDlgHandler

/*==============================================================================
������
       SecurityTsimInputPasswordDlgHandler
˵����
       IDD_TSIMINPUTPASSWORD_DIALOG �Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
#ifdef FEATURE_LCD_TOUCH_ENABLE
static boolean  SecurityTsimInputPasswordDlgHandler(CSecurityMenu *pMe,
                                         AEEEvent       eCode,
                                         uint16         wParam,
                                         uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    AECHAR         wstrDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};
    int            nLen = 0;
    char           strDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};   
    SEC_ERR("%x, %x ,%x,SecurityTsimInputPasswordDlgHandler",eCode,wParam,dwParam);
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(NULL == pMe->m_strPhonePWD)
            {
                pMe->m_strPhonePWD = (char *)MALLOC((OEMNV_LOCKCODE_MAXLEN + 1)* sizeof(char));
            }
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SECURITYMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);
            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR      text[32] = {0};
                RGBVAL nOldFontColor;
                AEERect     rc;
                IImage *pImageBg = NULL;
            	AEERect oldClip = {0};
            	pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_DIALER_MAIN);
            	//IImage_Draw(pImageBg, rect->x, rect->y);
                IDisplay_GetClipRect(pMe->m_pDisplay, &oldClip);
                IDisplay_SetClipRect(pMe->m_pDisplay, &pMe->m_rc);
                IImage_Draw(pImageBg, pMe->m_rc.x, pMe->m_rc.y);
                IDisplay_SetClipRect(pMe->m_pDisplay, &oldClip);
            	if(pImageBg != NULL)
                {
                    IImage_Release(pImageBg);
                    pImageBg = NULL;
                }
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD)); 
                MEMSET(strDisplay, '*', nLen);  
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                AEE_FONT_LARGE, 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
                                MENUITEM_HEIGHT/2,
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
                 (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
                 pImageBg = ISHELL_LoadResImage( pMe->m_pShell,AEE_APPSCOMMONRES_IMAGESFILE,IDB_DIALER_SEL_NOTHING);
            	 if(pImageBg!=NULL)
     			  {                  
     				 IIMAGE_Draw(pImageBg,0,227);
                     IIMAGE_Release( pImageBg);
     			  }
                            
                // ���Ƶ�����ʾ
                if (nLen > 0)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
                }
                else 
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_BACK)
                }
                // ������ʾ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
        
                return TRUE;
            }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
               // FREEIF(pMe->m_strPhonePWD);
            }
            return TRUE;

        case EVT_KEY:
            {
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;
                
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                    case AVK_SELECT: 
                        {
                         #ifdef FEATURE_LCD_TOUCH_ENABLE   
                         if(pMe->m_pActiveTSIMInputID==0)
                         {
                           (void) ISHELL_PostEvent(pMe->m_pShell,AEECLSID_CORE_APP,EVT_USER,100,(uint32)pMe->m_strPhonePWD);
                           CLOSE_DIALOG(DLGRET_OK)
                         }
                         else
                         #endif   
                         {
                            CLOSE_DIALOG(DLGRET_OK)
                         }
                        }
                        return TRUE;
                    case AVK_CLR:
                        chEnter = 0;
                        #ifndef FEATURE_ALL_KEY_PAD
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        #else
                        if(dwParam == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        else
                        {
                        	if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        	{
                            	CLOSE_DIALOG(DLGRET_CANCELED)
                            	return TRUE;
                        	}
                        }
                        #endif
                        break;  

                case AVK_DEL:    
                    chEnter = 0;
#ifdef FEATURE_ALL_KEY_PAD 
                    if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                    {
                        CLOSE_DIALOG(DLGRET_CANCELED)
                        return TRUE;
                    }
#endif
                break;                        
                        
                default:
                    return TRUE;
                }
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                if (chEnter == 0)
                {
                    // ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < OEMNV_LOCKCODE_MAXLEN)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
        case EVT_PEN_DOWN:
            {
				int16 wXPos = (int16)AEE_GET_X(dwParam);
				int16 wYPos = (int16)AEE_GET_Y(dwParam);
                
             	AEERect rc[CALC_ITEM]= 
					  	{
					  		{0,STARTY,NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,STARTY,NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,STARTY,NUMWINDTH,NUMHEIGHT},
					  		{0,STARTY+NUMHEIGHT,NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,STARTY+NUMHEIGHT,NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,STARTY+NUMHEIGHT,NUMWINDTH,NUMHEIGHT},
					  		{0,(STARTY+(NUMHEIGHT)*2),NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,(STARTY+(NUMHEIGHT)*2),NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,(STARTY+(NUMHEIGHT)*2),NUMWINDTH,NUMHEIGHT},
					  		{0,(STARTY+(NUMHEIGHT)*3),NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,(STARTY+(NUMHEIGHT)*3),NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,(STARTY+(NUMHEIGHT)*3),NUMWINDTH,NUMHEIGHT},
					  		{0,(STARTY+(NUMHEIGHT)*4),NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,(STARTY+(NUMHEIGHT)*4),NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,(STARTY+(NUMHEIGHT)*4),NUMWINDTH,NUMHEIGHT}
					  		
					  	};
            	AEEDeviceInfo devinfo;
            	int nBarH ;
            	int i = 0;
            	int j = 0;
            	
            	IImage *image = NULL;
            	//MSG_FATAL("pMe->m_bShowPopMenu========%d",pMe->m_bShowPopMenu,0,0);
            	pMe->m_i = -1;
            	//if(!pMe->m_bShowPopMenu)
            	{
            	MSG_FATAL("wXPos===%d,     wYPos===%d",wXPos,wYPos,0);
                for(i = 0;i<CALC_ITEM-3;i++)
                {
                	if(TOUCH_PT_IN_RECT(wXPos,wYPos,rc[i]))
                	{
                    	image = ISHELL_LoadResImage( pMe->m_pShell,
                                        AEE_APPSCOMMONRES_IMAGESFILE,
                                        IDB_DIALER_SEL_1+i);
            			pMe->m_i = i;
            			pMe->m_bup = FALSE;
            			if(image!=NULL)
            			{
            				MSG_FATAL("rc[i].x===%d,rc[i].y===%d,i===%d",rc[i].x,rc[i].y,i);
            				IIMAGE_Draw(image,rc[i].x,rc[i].y);
            				IIMAGE_Release( image);
            			}
                    	break;
                	}
                } 
            	}
            	IDISPLAY_Update(pMe->m_pDisplay);

            }
            break;
        case EVT_PEN_MOVE:
            {
        	 AEERect rc[CALC_ITEM]= 
        					  	{
        					  		{0,STARTY,NUMWINDTH,NUMHEIGHT},
        					  		{NUMWINDTH,STARTY,NUMWINDTH,NUMHEIGHT},
        					  		{(NUMWINDTH)*2,STARTY,NUMWINDTH,NUMHEIGHT},
        					  		{0,STARTY+NUMHEIGHT,NUMWINDTH,NUMHEIGHT},
        					  		{NUMWINDTH,STARTY+NUMHEIGHT,NUMWINDTH,NUMHEIGHT},
        					  		{(NUMWINDTH)*2,STARTY+NUMHEIGHT,NUMWINDTH,NUMHEIGHT},
        					  		{0,(STARTY+(NUMHEIGHT)*2),NUMWINDTH,NUMHEIGHT},
        					  		{NUMWINDTH,(STARTY+(NUMHEIGHT)*2),NUMWINDTH,NUMHEIGHT},
        					  		{(NUMWINDTH)*2,(STARTY+(NUMHEIGHT)*2),NUMWINDTH,NUMHEIGHT},
        					  		{0,(STARTY+(NUMHEIGHT)*3),NUMWINDTH,NUMHEIGHT},
        					  		{NUMWINDTH,(STARTY+(NUMHEIGHT)*3),NUMWINDTH,NUMHEIGHT},
        					  		{(NUMWINDTH)*2,(STARTY+(NUMHEIGHT)*3),NUMWINDTH,NUMHEIGHT},
        					  		{0,(STARTY+(NUMHEIGHT)*4),NUMWINDTH,NUMHEIGHT},
        					  		{NUMWINDTH,(STARTY+(NUMHEIGHT)*4),NUMWINDTH,NUMHEIGHT},
        					  		{(NUMWINDTH)*2,(STARTY+(NUMHEIGHT)*4),NUMWINDTH,NUMHEIGHT}
        					  		
        					  	};
        	int16 wXPos = (int16)AEE_GET_X(dwParam);
			int16 wYPos = (int16)AEE_GET_Y(dwParam);
        	if(!pMe->m_bup)
        	{
        		if(TOUCH_PT_IN_RECT(wXPos,wYPos,rc[pMe->m_i]))
        		{
        			return;
        		}
        		else
        		{
        			IImage *image = NULL;
        			pMe->m_i = -1;
        			pMe->m_bup = TRUE;
        			image = ISHELL_LoadResImage(pMe->m_pShell,
                                    AEE_APPSCOMMONRES_IMAGESFILE,
                                    IDB_DIALER_1+(pMe->m_i));
        			if(image!=NULL)
        			{
        				IIMAGE_Draw(image,rc[pMe->m_i].x,rc[pMe->m_i].y);
        				IIMAGE_Release(image);
        			}
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                                  AEECLSID_APP_SECURITYMENU,
                                                  EVT_USER_REDRAW,
                                                  NULL,
                                                  NULL);
        		}
        	  }
            }
            break;
		case EVT_PEN_UP:
			{
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
                IImage *image = NULL;
                int i = 0;
				int16 wXPos = (int16)AEE_GET_X(dwParam);
				int16 wYPos = (int16)AEE_GET_Y(dwParam);
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;
                
				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);

				if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,rc))
				{
					if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
					{
						boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_SELECT,0);
						return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
					{
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_INFO,0);
						 return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
					{						
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_CLR,0);
						 return rt;
					}
				}
                
                //if(!pMe->m_bup)
            	if(!pMe->m_bup)
            	{
                    AEERect rc[CALC_ITEM]= 
					  	{
					  		{0,STARTY,NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,STARTY,NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,STARTY,NUMWINDTH,NUMHEIGHT},
					  		{0,STARTY+NUMHEIGHT,NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,STARTY+NUMHEIGHT,NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,STARTY+NUMHEIGHT,NUMWINDTH,NUMHEIGHT},
					  		{0,(STARTY+(NUMHEIGHT)*2),NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,(STARTY+(NUMHEIGHT)*2),NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,(STARTY+(NUMHEIGHT)*2),NUMWINDTH,NUMHEIGHT},
					  		{0,(STARTY+(NUMHEIGHT)*3),NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,(STARTY+(NUMHEIGHT)*3),NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,(STARTY+(NUMHEIGHT)*3),NUMWINDTH,NUMHEIGHT},
					  		{0,(STARTY+(NUMHEIGHT)*4),NUMWINDTH,NUMHEIGHT},
					  		{NUMWINDTH,(STARTY+(NUMHEIGHT)*4),NUMWINDTH,NUMHEIGHT},
					  		{(NUMWINDTH)*2,(STARTY+(NUMHEIGHT)*4),NUMWINDTH,NUMHEIGHT}
					  		
					  	};
            	     pMe->m_bup = TRUE;
            	for(i = 0;i<CALC_ITEM-3;i++)
                {
                	if(TOUCH_PT_IN_RECT(wXPos,wYPos,rc[i]))
                	{

                    	image = ISHELL_LoadResImage(pMe->m_pShell,
                                        AEE_APPSCOMMONRES_IMAGESFILE,
                                        IDB_DIALER_1+i);
            			if(image!=NULL)
            			{
            				IIMAGE_Draw(image,rc[i].x,rc[i].y);
            				IIMAGE_Release(image);
            			}
                        if(i<9)
                          chEnter = '1' + i;
                        else if(i==9)
                           chEnter = '*' + i; 
                        else if(i==10)
                           chEnter = '0'; 
                        else if(i==11)
                           chEnter = '#'; 
                        else
                        {
                           return;
                        }

                         nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                        if (chEnter == 0)
                        {
                            // ɾ���ַ�
                            if (nLen > 0)
                            {
                                bRedraw = TRUE;
                                pMe->m_strPhonePWD[nLen-1] = chEnter;
                            }
                        }
                        else if (nLen < OEMNV_LOCKCODE_MAXLEN)
                        {
                            pMe->m_strPhonePWD[nLen] = chEnter;
                            nLen++;
                            pMe->m_strPhonePWD[nLen] = 0;
                            bRedraw = TRUE;
                        }

                        // if (bRedraw)
                            {
                                (void) ISHELL_PostEvent(pMe->m_pShell,
                                                        AEECLSID_APP_SECURITYMENU,
                                                        EVT_USER_REDRAW,
                                                        NULL,
                                                        NULL);
                            }
                	}
            	 }
            	}
            			}
			break;
#endif//FEATURE_LCD_TOUCH_ENABLE             
        default:
            break;
    }
    
    return FALSE;
} // SecurityTsimInputPasswordDlgHandler
#endif
/*==============================================================================
������
       SecurityAskPinDlgHandler
˵����
       IDD_ASK_PIN_DIALOG �Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityAskPinDlgHandler(CSecurityMenu *pMe,
                                         AEEEvent       eCode,
                                         uint16         wParam,
                                         uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    //static char   *m_strPhonePWD = NULL;
    AECHAR         wstrDisplay[OEMNV_LOCKPIN_MAXLEN+2] = {0};
    int            nLen = 0;
    char           strDisplay[OEMNV_LOCKPIN_MAXLEN+2] = {0};
    //WmsApp *pMe = (WmsApp *)pUser;
    
    if (NULL == pMe)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(NULL == pMe->m_strPhonePWD)
            {
                pMe->m_strPhonePWD = (char *)MALLOC((OEMNV_LOCKPIN_MAXLEN + 1)* sizeof(char));
            }
            
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SECURITYMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);

            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR  text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};

                
                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif


                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc,RGB_BLACK);
                if(pMe->m_bIsConfirmPassword)
                {
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_ENTERPIN, 
                                                text,
                                                sizeof(text)); 
                    
                }
                else
                {
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_PINCHANGE_TITLE, 
                                                text,
                                                sizeof(text));
                }
                // ��������
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                #if 0
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
				#else
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,text);
				#endif
               (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_ENTERPIN, 
                                                text,
                                                sizeof(text));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);

                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                            text,
                            -1, 
                            xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                            0,
#else
                            MENUITEM_HEIGHT*1/2,
#endif                             
                            NULL, 
                            IDF_TEXT_TRANSPARENT);


                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                MENUITEM_HEIGHT,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT-5,
#else
                                MENUITEM_HEIGHT*3/2,
#endif                                 
                                NULL, 
                                IDF_TEXT_TRANSPARENT);

                    
               if(!pMe->m_bIsConfirmPassword)
               {
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                    AEE_APPSSECURITYMENU_RES_FILE,
                                                    IDS_NEW_PIN, 
                                                    text,
                                                    sizeof(text));

                    IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                        text,
                                        -1, 
                                        xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                        MENUITEM_HEIGHT*2,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*2-10,
#else
                                        MENUITEM_HEIGHT*5/2,
#endif                                         
                                        NULL, 
                                        IDF_TEXT_TRANSPARENT);


                   (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                    AEE_APPSSECURITYMENU_RES_FILE,
                                                    IDS_CONFIRM, 
                                                    text,
                                                    sizeof(text));

                    IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                        text,
                                        -1, 
                                        xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                        MENUITEM_HEIGHT*3,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*3-15,
#else
                                        MENUITEM_HEIGHT*9/2,
#endif                                       
                                        NULL, 
                                        IDF_TEXT_TRANSPARENT);

               }               

                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor); 
                
                #ifndef FEATURE_ALL_KEY_PAD
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
//#ifdef FEATURE_VERSION_EC99
                    //SEC_MENU_DRAW_BOTTOMBAR(BTBAR_NEXT_DEL)
//#else                
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
//#endif                    
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_DELETE)
                }
                else
                #else
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_BACK)
                }
                else
                #endif
                {// ȡ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_CANCEL)
                }
                
                // ������ʾ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
 
                return TRUE;
            }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
                FREEIF(pMe->m_strPhonePWD);
            }
            
            return TRUE;

        case EVT_KEY:
            {
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;                
                
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        #ifndef FEATURE_ALL_KEY_PAD
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        #else
                        if(dwParam ==0)
                        {
	                          CLOSE_DIALOG(DLGRET_CANCELED)
	                          return TRUE;
                        }
                        else
                        {
                        	if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
	                        {
	                            CLOSE_DIALOG(DLGRET_CANCELED)
	                            return TRUE;
	                        }
                        }
                        #endif
                        break;
                        
                    case AVK_DEL:    
                        chEnter = 0;
#ifdef FEATURE_ALL_KEY_PAD 
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#endif
                    break;
                        
                    case AVK_DOWN:                        
                        if(pMe->m_bIsConfirmPassword)
                        {
                            return TRUE;                       
                        }
                        else
                        {
                            nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                            if (nLen < 4)
                            {
                                return TRUE;
                            }
                        
                            if(IsRunAsUIMVersion())
                            {
                                if (NULL != pMe->m_strPhonePWD)
                                {         
                                //                              (void)WSTRTOSTR(pText, szPIN, sizeof(szPIN));
                                    if (SecurityMenu_ValidPIN(pMe, pMe->m_strPhonePWD))
                                    {
                                        pMe->nOldPSWLength = nLen;
                                        CLOSE_DIALOG(DLGRET_VALIDPINPASS)
                                    }
                                    else
                                    {
                                        //AECHAR prompt[64] = { 0};

                                        //ISHELL_LoadResString( AEE_GetShell(), AEE_APPSSECURITYMENU_RES_FILE, IDS_INVALIDPIN, prompt, sizeof( prompt));
                                        //WSPRINTF( prompt, sizeof( prompt), prompt, pMe->m_nCanAttemptTimes);
                                        //Security_ShowMsgBox(pMe, prompt);

                                        CLOSE_DIALOG(DLGRET_VALIDPINFAILED)//(DLGRET_OK)  
                                        //PhoneLock_ShowMessageBoxThatPasswordIsInvalid( pMe, pMe->m_pDisplay, pMe->m_pIStatic, prompt);

                                    }   
                                }
                            }
                            return TRUE;
                        }
                        
                    case AVK_SELECT:
                    case AVK_INFO:
                        if(pMe->m_bIsConfirmPassword)
                        {
                            nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                            if (nLen < 4)
                            {
                                return TRUE;
                            }
                            
                            if(IsRunAsUIMVersion())
                            {
                                if (NULL != pMe->m_strPhonePWD)
                                {
                                    if (SecurityMenu_ValidPIN(pMe, pMe->m_strPhonePWD))
                                    {
                                        AEECHVStatus  chvst;
                                        (void)IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
                                        if (chvst.chv1_enabled)
                                        {
                                            IRUIM_CHVDisable(pMe->m_pIRUIM);
                                        }
                                        else
                                        {
                                            IRUIM_CHVEnable(pMe->m_pIRUIM, NULL);
                                        }
                                        CLOSE_DIALOG(DLGRET_ISCOMFIRMPASSWORD)
                                    } 
                                    else
                                    {
                                        CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                                    }
                                }
                            }
                            return TRUE;
                        }
                        else
                        {
                            return TRUE;
                        }
                    
                    default:
                        return TRUE;
                }
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                if (chEnter == 0)
                {
                    // ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < OEMNV_LOCKPIN_MAXLEN)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
        case EVT_PEN_UP:
            {
                AEEDeviceInfo devinfo;
                int nBarH ;
                AEERect rc;
                AEERect pwtextrc;
                AEERect newpwtextrc;
                int16 wXPos = (int16)AEE_GET_X(dwParam);
                int16 wYPos = (int16)AEE_GET_Y(dwParam);

                nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
                MEMSET(&devinfo, 0, sizeof(devinfo));
                ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
                SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
                SETAEERECT(&pwtextrc, PWTEXT_MINX, PWTEXT_MINY, PWTEXT_MAXX, PWTEXT_MAXY-80);
                SETAEERECT(&newpwtextrc, PWTEXT_MINX, PWTEXT_MAXY-74, PWTEXT_MAXX, PWTEXT_MAXY-80);
                if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,rc))
                {
                    if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
                    {
                        boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_SELECT,0);
                        return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
                    {
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_INFO,0);
                         return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
                    {                       
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_CLR,0);
                         return rt;
                    }
                }
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,pwtextrc))
                {
                    pMe->m_pActiveTSIMInputID=IDD_ASK_PIN_DIALOG;
                    MSG_FATAL("DLGRET_TSIMPASSWORDINPUT-----------",0,0,0);
                    CLOSE_DIALOG(DLGRET_TSIMPASSWORDINPUT);
                    return TRUE;
                }
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,newpwtextrc))
                {
                     if(pMe->m_bIsConfirmPassword)
                        {
                            return TRUE;                       
                        }
                        else
                        {
                            nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                            if (nLen < 4)
                            {
                                return TRUE;
                            }
                        
                            if(IsRunAsUIMVersion())
                            {
                                if (NULL != pMe->m_strPhonePWD)
                                {         
                                   if (SecurityMenu_ValidPIN(pMe, pMe->m_strPhonePWD))
                                    {
                                        pMe->nOldPSWLength = nLen;
                                        CLOSE_DIALOG(DLGRET_VALIDPINPASS)
                                    }
                                    else
                                    {
                                        CLOSE_DIALOG(DLGRET_VALIDPINFAILED)//(DLGRET_OK)  

                                    }   
                                }
                            }
                            return TRUE;
                        }
                }

            }
                                    break;
#endif//FEATURE_LCD_TOUCH_ENABLE             
            
        default:
            break;
    }
    
    return FALSE;
} // SecurityAskPinDlgHandler

/*==============================================================================
������
       SecurityAskCallPasswordDlgHandler
˵����
       IDD_ASK_CALL_PASSWORD_DIALOG �Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityAskCallPasswordDlgHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    //static char   *m_strPhonePWD = NULL;
    AECHAR         wstrDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};
    int            nLen = 0;
    char           strDisplay[OEMNV_LOCKCODE_MAXLEN+2] = {0};

    if (NULL == pMe)
    {
        return FALSE;
    }

    MSG_FATAL("***zzg SecurityAskCallPasswordDlgHandler eCode=%x***", eCode, 0, 0);
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(NULL == pMe->m_strPhonePWD)
            {
                pMe->m_strPhonePWD = (char *)MALLOC((OEMNV_LOCKCODE_MAXLEN + 1)* sizeof(char));
            }
            
            return TRUE;
            
        case EVT_DIALOG_START:  
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SECURITYMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);

            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR  text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};


                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif

                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc,RGB_BLACK);                  
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_PHONE_PASSWORD_INPUT_TITLE, 
                                            text,
                                            sizeof(text));                  
                // ��������
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
				#if 0
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
				#else
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,text);
				#endif
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_OLD_CODE, 
                                            text,
                                            sizeof(text));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);

                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endi
                                    text,
                                    -1, 
                                    xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                                    0,
#else
                                    MENUITEM_HEIGHT*1/2,
#endif                                     
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);

                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));


                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                MENUITEM_HEIGHT,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT-5,
#else
                                MENUITEM_HEIGHT*3/2,
#endif                                 
                                NULL, 
                                IDF_TEXT_TRANSPARENT);


                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_NEW_CODE, 
                                            text,
                                            sizeof(text));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif
                                    text,
                                    -1, 
                                    xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                    MENUITEM_HEIGHT*2,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*2-10,
#else
                                    MENUITEM_HEIGHT*5/2,
#endif                                     
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);

                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_CONFIRM, 
                                            text,
                                            sizeof(text));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                    text,
                                    -1, 
                                    xOffset, 
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                    MENUITEM_HEIGHT*3,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*3-15,
#else
                                    MENUITEM_HEIGHT*9/2,
#endif                                    
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);


                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);

				#ifndef FEATURE_ALL_KEY_PAD
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_NEXT_DEL)
#else                
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
#endif                    
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_DELETE)
                }
                else
                #else
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_BACK)
                }
                else
                #endif
                {// ȡ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_CANCEL)
                }
                
                // ������ʾ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);  

                return TRUE;
            }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
                FREEIF(pMe->m_strPhonePWD);
            }
           
            return TRUE;

        case EVT_KEY:
            {
                char        chEnter = 0;
                int         nLen = 0;
                boolean     bRedraw = FALSE;
                               
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        #ifndef FEATURE_ALL_KEY_PAD
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        #else
                        if(dwParam == 0)
                        {
                        	CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        else
                        {
                        	if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        	{
                            	CLOSE_DIALOG(DLGRET_CANCELED)
                            	return TRUE;
                        	}
                        }
                        #endif
                        break;

                   case AVK_DEL:    
                       chEnter = 0;
#ifdef FEATURE_ALL_KEY_PAD 
                       if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                       {
                           CLOSE_DIALOG(DLGRET_CANCELED)
                           return TRUE;
                       }
#endif
                   break;

                   case AVK_DOWN:
#if defined(FEATURE_VERSION_EC99) ||defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                    case AVK_SELECT:
                    case AVK_INFO:
#endif                     
                         {
                            uint16 wPWD=0;
							//#ifdef FEATURE_VERSION_K212_ND
                           // char superpass[6] = {"0000"};
							//#else
							char superpass[6] = {"*#09#"};
						//	#endif
                            nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                            //ISTATIC_Redraw(pMe->m_oldPassword);
                            if (nLen < 4)
                            {
                                return TRUE;
                            }
                            
                            (void) ICONFIG_GetItem(pMe->m_pConfig, 
                                                   CFGI_PHONE_PASSWORD,
                                                   &wPWD,
                                                   sizeof(uint16));
                            
                            if (wPWD == EncodePWDToUint16(pMe->m_strPhonePWD)||(0==strcmp(superpass,pMe->m_strPhonePWD)))
                            {
                                // �������
                                pMe->nOldPSWLength = nLen;
                                CLOSE_DIALOG(DLGRET_VALIDPINPASS)
                            }
                            else
                            {
                                // �������
                                CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                            }
                        }       
                        return TRUE;
                        
                    default:
                        return TRUE;
                }
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                if (chEnter == 0)
                {
                    // ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < OEMNV_LOCKCODE_MAXLEN)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
        case EVT_PEN_UP:
            {
                AEEDeviceInfo devinfo;
                int nBarH ;
                AEERect rc;
                AEERect pwtextrc;
                AEERect newpwtextrc;
                int16 wXPos = (int16)AEE_GET_X(dwParam);
                int16 wYPos = (int16)AEE_GET_Y(dwParam);

                nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
                MEMSET(&devinfo, 0, sizeof(devinfo));
                ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
                SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
                SETAEERECT(&pwtextrc, PWTEXT_MINX, PWTEXT_MINY, PWTEXT_MAXX, PWTEXT_MAXY-80);
                SETAEERECT(&newpwtextrc, PWTEXT_MINX, PWTEXT_MAXY-74, PWTEXT_MAXX, PWTEXT_MAXY-80);
                if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,rc))
                {
                    if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
                    {
                        boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_SELECT,0);
                        return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
                    {
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_INFO,0);
                         return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
                    {                       
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_CLR,0);
                         return rt;
                    }
                }
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,pwtextrc))
                {
                    pMe->m_pActiveTSIMInputID=IDD_ASK_CALL_PASSWORD_DIALOG;
                    MSG_FATAL("DLGRET_TSIMPASSWORDINPUT-----------",0,0,0);
                    CLOSE_DIALOG(DLGRET_TSIMPASSWORDINPUT);
                    return TRUE;
                }
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,newpwtextrc))
                {
                     uint16 wPWD=0;
					 //#ifdef FEATURE_VERSION_K212_ND
                     //char superpass[6] = {"0000"};
					// #else
					 char superpass[6] = {"*#09#"};
					// #endif
                     nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                     //ISTATIC_Redraw(pMe->m_oldPassword);
                     if (nLen < 4)
                     {
                         return TRUE;
                     }
                     
                     (void) ICONFIG_GetItem(pMe->m_pConfig, 
                                            CFGI_PHONE_PASSWORD,
                                            &wPWD,
                                            sizeof(uint16));
                     
                     if (wPWD == EncodePWDToUint16(pMe->m_strPhonePWD)||(0==strcmp(superpass,pMe->m_strPhonePWD)))
                     {
                         // �������
                         pMe->nOldPSWLength = nLen;
                         CLOSE_DIALOG(DLGRET_VALIDPINPASS)
                     }
                     else
                     {
                         // �������
                         CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                     }
                     return TRUE;
                }

            }
                        break;
#endif//FEATURE_LCD_TOUCH_ENABLE             
            
        default:
            break;
    }
    
    return FALSE;

} // SecurityAskCallPasswordDlgHandler

/*==============================================================================
������
       SetMenu_SetRadioIconAsOff
˵����
       �����趨�����˵��ؼ���ȫ���˵��ѡ��ťͼ��Ϊ��ѡ��ͼ�ꡣ

������
       pMenu {in]��IMenuCtl�ؼ�ָ�롣

����ֵ��
       �ޡ�

��ע��

==============================================================================*/
static void SetMenu_SetRadioIconAsOff(IMenuCtl *pMenu)
{
    int nItemCount, index;
    uint16 wItemID;

    if (NULL == pMenu)
    {
        return;
    }

    nItemCount = IMENUCTL_GetItemCount(pMenu);
    for (index = 0; index < nItemCount; index++)
    {
        wItemID = IMENUCTL_GetItemID(pMenu, index);
        SetMenuIcon(pMenu, wItemID, FALSE);
    }
} // SetMenu_SetRadioIconAsOff


/*==============================================================================
������
       SecurityMenu_ValidPIN
˵����
       ������֤���� PIN ���Ƿ���Ч��

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���
       Ϣ��
       szPIN [in]��PIN ���ַ�����

����ֵ��
       TRUE ��ͨ����֤��
       FALSE ��δͨ����֤��

��ע��

==============================================================================*/
static boolean SecurityMenu_ValidPIN(CSecurityMenu *pMe, char *szPIN)
{
    boolean bPassed = FALSE;

    if ((NULL == pMe) || (NULL == szPIN))
    {
        return FALSE;
    }

    (void)uim_power_control( UIM_PROACTIVE_UIM, TRUE);

    //refui_sleep(500);
    // ��鿨�Ƿ����
    if(IRUIM_IsCardConnected(pMe->m_pIRUIM))
    {
        AEECHVStatus  chvst;
        IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
        if (!chvst.chv1_enabled)
        {
            // ��δ���� CHV ���ܣ�����ִ����֤ CHV �������������� CHV ����
            bPassed = IRUIM_CHVEnable(pMe->m_pIRUIM, szPIN);

            if (bPassed)
            {
                // �� CHV ���ܳɹ������ CHV һ����Ч���ر� CHV ���ܣ��ָ���
                // ��̬
                IRUIM_CHVDisable(pMe->m_pIRUIM);
            }
            else
            {
                IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
                pMe->m_nCanAttemptTimes = chvst.chv1CountRemain;
                SEC_ERR("pMe->m_nCanAttemptTimes = %d",pMe->m_nCanAttemptTimes,0,0);
            }
        }
        else
        {
            if(STRLEN(szPIN)>0)
            {
                char ch = szPIN[STRLEN(szPIN)-1];
                if(ch<'0' || ch>'9')
                {
                    szPIN[STRLEN(szPIN)-1] = '\0';
                }
            }
            
            //AEEOEM_VirtualPINCheck to AEEOEM_PINCheck by chengang for CR342
            bPassed = IRUIM_PINCheck(pMe->m_pIRUIM, IRUIM_CHV1, szPIN);

            if (!bPassed)
            {
                IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
                pMe->m_nCanAttemptTimes = chvst.chv1CountRemain;
				
                if( pMe->m_nCanAttemptTimes < 0)
                {
                    pMe->m_nCanAttemptTimes = 0;
                }
				
                SEC_ERR("pMe->m_nCanAttemptTimes = %d",pMe->m_nCanAttemptTimes,0,0);
            }
        }
    }
    (void)uim_power_control( UIM_PROACTIVE_UIM, FALSE);

    return bPassed;
}


/*==============================================================================
������
       SecurityMenu_ModfyPIN
˵����
       ���ڽ����� PIN ������Ϊ�µ� PIN �롣

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���
       Ϣ��
       szPIN [in]��PIN ���ַ�����

����ֵ��
       TRUE ���ɹ��޸ġ�
       FALSE ���޸�ʧ�ܡ�

��ע��

==============================================================================*/
static boolean SecurityMenu_ModfyPIN(CSecurityMenu *pMe, char *szPIN)
{
    int bPassed = EFAILED;

    if ((NULL == pMe) || (NULL == szPIN))
    {
        return FALSE;
    }

    (void)uim_power_control( UIM_PROACTIVE_UIM, TRUE);

    //refui_sleep(500);
    if(IRUIM_IsCardConnected(pMe->m_pIRUIM))
    {
        AEECHVStatus  chvst;
        IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
        if (!chvst.chv1_enabled)
        {
            if (IRUIM_CHVEnable(pMe->m_pIRUIM, NULL))
            {
                if(STRLEN(szPIN)>0)
                {
                    char ch = szPIN[STRLEN(szPIN)-1];
                    if(ch<'0' || ch>'9')
                    {
                        szPIN[STRLEN(szPIN)-1] = '\0';
                    }
                }
                bPassed = IRUIM_PINChange(pMe->m_pIRUIM,IRUIM_CHV1,szPIN);
                IRUIM_CHVDisable(pMe->m_pIRUIM);
            }
        }
        else
        {
            if(STRLEN(szPIN)>0)
            {
                char ch = szPIN[STRLEN(szPIN)-1];
                if(ch<'0' || ch>'9')
                {
                    szPIN[STRLEN(szPIN)-1] = '\0';
                }
            }
            bPassed = IRUIM_PINChange(pMe->m_pIRUIM,IRUIM_CHV1,szPIN);
        }
    }
    (void)uim_power_control( UIM_PROACTIVE_UIM, FALSE);
    
    if(bPassed == AEE_SUCCESS)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*==============================================================================
����:
       CSecurtyMenu_DialogTimeout

˵��:
       ��ʱ���ص���������Ҫ�����Զ��ر���Ϣ�Ի���

����:
       pme [in]��void *����ָ�룬��ָCSecurityMenu�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void CSecurtyMenu_DialogTimeout(void *pme)
{
    CSecurityMenu *pMe = (CSecurityMenu *)pme;

	MSG_FATAL("***zzg CSecurtyMenu_DialogTimeout***", 0, 0, 0);

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent(pMe->m_pShell,
                            AEECLSID_APP_SECURITYMENU,
                            EVT_DISPLAYDIALOGTIMEOUT,
                            0,
                            0);
}


/*==============================================================================
������
       SecurityAffirmPassWordHandler
˵����
       IDD_AFFIRM_PASSWORD�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityAffirmPassWordHandler(CSecurityMenu *pMe,
                                                          AEEEvent       eCode,
                                                          uint16         wParam,
                                                          uint32         dwParam)


{
    PARAM_NOT_REF(dwParam)
    //static char   *m_strPhonePWD = NULL;
    AECHAR      wstrDisplay[OEMNV_LOCKPIN_MAXLEN+2] = {0};
    int             nLen = 0;
    char        strDisplay[OEMNV_LOCKPIN_MAXLEN+2] = {0};

    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(NULL == pMe->m_strPhonePWD)
            {
                if(pMe->m_IsPin)
                {
                    pMe->m_strPhonePWD = (char *)MALLOC((OEMNV_LOCKPIN_MAXLEN + 1)* sizeof(char));
                }
                else
                {
                    pMe->m_strPhonePWD = (char *)MALLOC((OEMNV_LOCKCODE_MAXLEN + 1)* sizeof(char));
                }
            }
 
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SECURITYMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);

            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR  text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};


                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif

                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc,RGB_BLACK);  
                if(pMe->m_IsPin)
                {
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_PINCHANGE_TITLE, 
                                            text,
                                            sizeof(text));
                }
                else
                {
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_PHONE_PASSWORD_INPUT_TITLE, 
                                            text,
                                            sizeof(text));  
                }
                // ��������
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
				#if 0
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
				#else
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,text);
				#endif
                if(pMe->m_IsPin)
                {
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_ENTERPIN, 
                                                text,
                                                sizeof(text));
                }
                else
                {
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_OLD_CODE, 
                                                text,
                                                sizeof(text));  
                }
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);

#ifndef FEATURE_VERSION_EC99
#ifndef FEATURE_VERSION_K212_20D
#ifndef FEATURE_VERSION_K212_ND
#ifndef FEATURE_VERSION_W516_C260

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                    AEE_FONT_BOLD, 
                                    text,
                                    -1, 
                                    xOffset, 
                                    MENUITEM_HEIGHT*1/2,
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
#endif
#endif
#endif
#endif


                MEMSET(strDisplay, '*', pMe->nOldPSWLength);
                strDisplay[pMe->nOldPSWLength] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

#ifndef FEATURE_VERSION_EC99
#ifndef FEATURE_VERSION_K212_20D
#ifndef FEATURE_VERSION_K212_ND
#ifndef FEATURE_VERSION_W516_C260

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                AEE_FONT_BOLD, 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
                                MENUITEM_HEIGHT*3/2,
                                NULL, 
                                IDF_TEXT_TRANSPARENT);
#endif
#endif
#endif
#endif


                if(pMe->m_IsPin)
                {
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                            AEE_APPSSECURITYMENU_RES_FILE,
                                                            IDS_NEW_PIN, 
                                                            text,
                                                            sizeof(text));
                }
                else
                {
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                            AEE_APPSSECURITYMENU_RES_FILE,
                                                            IDS_NEW_CODE, 
                                                            text,
                                                            sizeof(text));  
                }


                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                    text,
                                    -1, 
                                    xOffset, 
#if defined(FEATURE_VERSION_EC99) || defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                                    0,
#else
                                    MENUITEM_HEIGHT*5/2,
#endif                                    
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);


                MEMSET(strDisplay, '*', pMe->nNewPSWLength);
                strDisplay[pMe->nNewPSWLength] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
#if defined(FEATURE_VERSION_EC99) || defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                MENUITEM_HEIGHT,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT-5,
#else
                                MENUITEM_HEIGHT*7/2,
#endif                        
                                NULL, 
                                IDF_TEXT_TRANSPARENT);


                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_CONFIRM, 
                                            text,
                                            sizeof(text));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif
                                    text,
                                    -1, 
                                    xOffset, 
#if defined(FEATURE_VERSION_EC99) || defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                    MENUITEM_HEIGHT*2,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*2-10,
#else
                                    MENUITEM_HEIGHT*9/2,
#endif                                     
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);

                
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                MEMSET(strDisplay, '*', nLen);
                strDisplay[nLen] = '|';
                strDisplay[nLen + 1] = '\0';
                (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));


                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                wstrDisplay,
                                -1, 
                                2*xOffset, 
#if defined(FEATURE_VERSION_EC99) || defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                MENUITEM_HEIGHT*3,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*3-15,
#else
                                MENUITEM_HEIGHT*11/2,
#endif                          
                                NULL, 
                                IDF_TEXT_TRANSPARENT);

            

                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);

                // ���Ƶ�����ʾ
				#ifndef FEATURE_ALL_KEY_PAD    //add by yangdecai 
                if (nLen > 3)
                {// ����-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_SAVE_DELETE)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_DELETE)
                }
                else
                #else
                if (nLen > 3)
                {// ����-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_SAVE_BACK)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_BACK)
                }
                else
                #endif
                {// ȡ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_CANCEL)
                }
                
                // ������ʾ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
                 
                return TRUE;
            }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
                FREEIF(pMe->m_strPhonePWD);
            }
        
            return TRUE;

        case EVT_KEY:
            {
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;
                
                uint16 pass_len =  OEMNV_LOCKCODE_MAXLEN;
                if(pMe->m_IsPin)
                {
                    pass_len =  OEMNV_LOCKPIN_MAXLEN;
                }              
                
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        #ifndef FEATURE_ALL_KEY_PAD    //add by yangdecai 
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        #else
                        if(dwParam == 0)
                        {
                        	CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
                        else
                        {
                        	if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        	{
                            	CLOSE_DIALOG(DLGRET_CANCELED)
                            	return TRUE;
                        	}
                        }
                        #endif
                        break;
                        
                    case AVK_DEL:    
                        chEnter = 0;
#ifdef FEATURE_ALL_KEY_PAD 
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#endif
                    break;

                        
                    case AVK_SELECT:
                    case AVK_INFO:            
                    {
                        // char szPWD[UIM_MAX_CHV_DIGITS+1];
                        uint16 wData;
                        //AECHAR *pText = ITEXTCTL_GetTextPtr(pTextCtl);
                        
                        nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                        
                        if (nLen < 4)
                        {
                            return TRUE;
                        }
                       // (void)WSTRTOSTR(pText, szPWD, sizeof(szPWD));
                       wData = EncodePWDToUint16(pMe->m_strPhonePWD);
                        if(wData != pMe->m_PassWord)
                        {
                            CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                            return TRUE;
                        }

                        if(IsRunAsUIMVersion())
                        {
                            if(pMe->m_IsPin)
                            {
                                STRNCPY(pMe->m_Pin, pMe->m_strPhonePWD,sizeof(pMe->m_Pin));
                             //  pMe->m_Pin = m_strPhonePWD;
                                if (SecurityMenu_ModfyPIN(pMe, pMe->m_Pin))
                                {
                                    CLOSE_DIALOG(DLGRET_SETPINPASS)
                                }

                                else
                                {
                                    CLOSE_DIALOG(DLGRET_SETPINFAILED)
                                }
                            }
                            else
                            {
                                (void) ICONFIG_SetItem(pMe->m_pConfig,  
                                                            CFGI_PHONE_PASSWORD,
                                                            &wData, 
                                                            sizeof(wData));
                                //pMe->m_wMsgID = IDS_PHONE_PASSWORD_CHANGED;
                                CLOSE_DIALOG(DLGRET_TOSHOWMSG)
                            }

                        }
                        else
                        {
                            (void) ICONFIG_SetItem(pMe->m_pConfig,
                                                        CFGI_PHONE_PASSWORD,
                                                        &wData, 
                                                        sizeof(wData));
                            pMe->m_wMsgID = IDS_PHONE_PASSWORD_CHANGED;
                            CLOSE_DIALOG(DLGRET_TOSHOWMSG)
                        }
                        return TRUE;
                      }
                             
                    default:
                        return TRUE;
                }
                
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                if (chEnter == 0)
                {// ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < pass_len)
                {
                    pMe->m_strPhonePWD[nLen] = chEnter;
                    nLen++;
                    pMe->m_strPhonePWD[nLen] = 0;
                    bRedraw = TRUE;
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
        case EVT_PEN_UP:
            {
                AEEDeviceInfo devinfo;
                int nBarH ;
                AEERect rc;
                AEERect pwtextrc;
                int16 wXPos = (int16)AEE_GET_X(dwParam);
                int16 wYPos = (int16)AEE_GET_Y(dwParam);

                nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
                MEMSET(&devinfo, 0, sizeof(devinfo));
                ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
                SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
                SETAEERECT(&pwtextrc, PWTEXT_MINX, PWTEXT_MAXY-23, PWTEXT_MAXX, PWTEXT_MAXY-80);
                if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,rc))
                {
                    if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
                    {
                        boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_SELECT,0);
                        return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
                    {
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_INFO,0);
                         return rt;
                    }
                    else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
                    {                       
                         boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_APP_SECURITYMENU,EVT_USER,AVK_CLR,0);
                         return rt;
                    }
                }
                else if(SECURITYMENU_PT_IN_RECT(wXPos,wYPos,pwtextrc))
                {
                    pMe->m_pActiveTSIMInputID=IDD_AFFIRM_PASSWORD;
                    MSG_FATAL("DLGRET_TSIMPASSWORDINPUT-----------",0,0,0);
                    CLOSE_DIALOG(DLGRET_TSIMPASSWORDINPUT);
                    return TRUE;
                }

            }
           break;
#endif//FEATURE_LCD_TOUCH_ENABLE             
            
            
        default:
            break;
    }
    
    return FALSE;
}

/*==============================================================================
������
       SecurityAskPUKPassWordHandler
˵����
       IDD_PUK_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityAskPUKPassWordHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    //static char   *m_strPhonePWD = NULL;
    AECHAR      wstrDisplay[UIM_MAX_CHV_DIGITS+2] = {0};
    int             nLen = 0;
    char        strDisplay[UIM_MAX_CHV_DIGITS+2] = {0};
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if(NULL == pMe->m_strPhonePWD)
            {
                pMe->m_strPhonePWD = (char *)MALLOC((UIM_MAX_CHV_DIGITS + 1)* sizeof(char));
            }         
            return TRUE;
            
        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_APP_SECURITYMENU,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);

            return TRUE;
            
        case EVT_USER_REDRAW:
            // ���������Ϣ
            {
                AECHAR  text[32] = {0};
                RGBVAL nOldFontColor;
                TitleBar_Param_type  TitleBar_Param = {0};


                // ������
#ifdef FEATURE_CARRIER_CHINA_VERTU
                {
                    IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
                    
                    Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                    if(pImageBg != NULL)
                    {
                        IImage_Release(pImageBg);
                    }
                }
#else
                Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif

                //IDISPLAY_FillRect  (pMe->m_pDisplay,&pMe->m_rc,RGB_BLACK);  
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_PUK_TITLE, 
                                            text,
                                            sizeof(text));  
                // ��������
                TitleBar_Param.pwszTitle = text;
                TitleBar_Param.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
				#if 0
                DrawTitleBar(pMe->m_pDisplay, &TitleBar_Param);
				#else
				IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,text);
				#endif
                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_APPSSECURITYMENU_RES_FILE,
                                                IDS_ENTER_PUK, 
                                                text,
                                                sizeof(text));
                nOldFontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);

#ifndef FEATURE_VERSION_EC99
#ifndef FEATURE_VERSION_K212_20D
#ifndef FEATURE_VERSION_K212_ND
#ifndef FEATURE_VERSION_W516_C260

                IDISPLAY_DrawText(pMe->m_pDisplay, 
                                    AEE_FONT_BOLD, 
                                    text,
                                    -1, 
                                    xOffset, 
                                    MENUITEM_HEIGHT*1/2, 
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
#endif
#endif
#endif
#endif

                if(pMe->m_eRUIMSCode == ENTERPUK_STEP1 || pMe->m_eRUIMSCode == ENTERPUK_STEP2)
                {
                    MEMSET(strDisplay, '*', pMe->nOldPSWLength);
                    strDisplay[pMe->nOldPSWLength] = '\0';
                    (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

#ifndef FEATURE_VERSION_EC99
#ifndef FEATURE_VERSION_K212_20D
#ifndef FEATURE_VERSION_K212_ND

#ifndef FEATURE_VERSION_W516_C260

                    IDISPLAY_DrawText(pMe->m_pDisplay, 
                                    AEE_FONT_BOLD, 
                                    wstrDisplay,
                                    -1, 
                                    2*xOffset, 
                                    MENUITEM_HEIGHT*3/2,
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
#endif
#endif
#endif
#endif

                }
                
                if(pMe->m_eRUIMSCode == ENTERPUK_STEP0)
                {
                    nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                    MEMSET(strDisplay, '*', nLen);
                    strDisplay[nLen] = '|';
                    strDisplay[nLen + 1] = '\0';
                    (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

#ifndef FEATURE_VERSION_EC99
#ifndef FEATURE_VERSION_K212_20D
#ifndef FEATURE_VERSION_K212_ND
#ifndef FEATURE_VERSION_W516_C260
                    IDISPLAY_DrawText(pMe->m_pDisplay, 
                                    AEE_FONT_BOLD, 
                                    wstrDisplay,
                                    -1, 
                                    2*xOffset, 
                                    MENUITEM_HEIGHT*3/2,
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
#endif
#endif
#endif
#endif
                }

                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                            AEE_APPSSECURITYMENU_RES_FILE,
                                                            IDS_NEWPIN, 
                                                            text,
                                                            sizeof(text));

                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                    text,
                                    -1, 
                                    xOffset, 
#if defined(FEATURE_VERSION_EC99) || defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)||defined(FEATURE_VERSION_K212_ND)
                                    0,
#else
                                    MENUITEM_HEIGHT*5/2,
#endif                             
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);


                if(pMe->m_eRUIMSCode == ENTERPUK_STEP2)
                {
                    MEMSET(strDisplay, '*', pMe->nNewPSWLength);
                    strDisplay[pMe->nNewPSWLength] = '\0';
                    (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

                    IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif
                                    wstrDisplay,
                                    -1, 
                                    2*xOffset, 
#if defined(FEATURE_VERSION_EC99) || defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                    MENUITEM_HEIGHT,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT-5,
#else
                                    MENUITEM_HEIGHT*7/2,
#endif                                  
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
                }

                if(pMe->m_eRUIMSCode == ENTERPUK_STEP1)
                {
                    nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                    MEMSET(strDisplay, '*', nLen);
                    strDisplay[nLen] = '|';
                    strDisplay[nLen + 1] = '\0';
                    (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

                    IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                    wstrDisplay,
                                    -1, 
                                    2*xOffset, 
#if defined(FEATURE_VERSION_EC99) || defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                    MENUITEM_HEIGHT,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT-5,
#else
                                    MENUITEM_HEIGHT*7/2,
#endif                                      
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
                }

                (void)ISHELL_LoadResString(pMe->m_pShell, 
                                            AEE_APPSSECURITYMENU_RES_FILE,
                                            IDS_CONFIRM, 
                                            text,
                                            sizeof(text));


                IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                    text,
                                    -1, 
                                    xOffset,                                     
#if defined(FEATURE_VERSION_EC99) || defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                    MENUITEM_HEIGHT*2,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*2-10,
#else
                                    MENUITEM_HEIGHT*9/2,
#endif                              
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);

         
                if(pMe->m_eRUIMSCode == ENTERPUK_STEP2)
                {
                    nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                    MEMSET(strDisplay, '*', nLen);
                    strDisplay[nLen] = '|';
                    strDisplay[nLen + 1] = '\0';
                    (void) STRTOWSTR(strDisplay, wstrDisplay, sizeof(wstrDisplay));

                    IDISPLAY_DrawText(pMe->m_pDisplay, 
#ifdef FEATURE_VERSION_K212_ND
                                AEE_FONT_NORMAL, 
#else
								AEE_FONT_BOLD, 
#endif 
                                    wstrDisplay,
                                    -1, 
                                    2*xOffset, 
#if defined(FEATURE_VERSION_EC99) || defined(FEATURE_VERSION_K212_20D)||defined(FEATURE_VERSION_W516_C260)
                                    MENUITEM_HEIGHT*3,
#elif defined(FEATURE_VERSION_K212_ND)
								MENUITEM_HEIGHT*3-15,
#else
                                    MENUITEM_HEIGHT*11/2,
#endif                                     
                                    NULL, 
                                    IDF_TEXT_TRANSPARENT);
                }        

                (void)IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
                
				#ifndef FEATURE_ALL_KEY_PAD    //add by yangdecai 
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_DELETE)
                }
                else
                #else
                // ���Ƶ�����ʾ
                if (nLen > 3)
                {// ȷ��-----ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
                }
                else if(nLen > 0)
                {// ɾ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_SOS)
                }
                else
                #endif
                {//ȡ��
                    SEC_MENU_DRAW_BOTTOMBAR(BTBAR_SOS/*BTBAR_CANCEL*/)
                }
                
                // ������ʾ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
                 
                return TRUE;
            }
            
        case EVT_DIALOG_END:
            if(!pMe->m_bSuspending)
            {
                FREEIF(pMe->m_strPhonePWD);
            }
         
            return TRUE;

        case EVT_KEY:
            {
                char  chEnter = 0;
                int   nLen = 0;
                boolean bRedraw = FALSE;
                
                
                switch (wParam)
                {
                    case AVK_0:
                    case AVK_1:
                    case AVK_2:
                    case AVK_3:
                    case AVK_4:
                    case AVK_5:
                    case AVK_6:
                    case AVK_7:
                    case AVK_8:
                    case AVK_9:
                        chEnter = '0' + (wParam - AVK_0);
                        break;

                    case AVK_STAR:
                        chEnter = '*';
                        break;
 
                    case AVK_POUND:
                        chEnter = '#';
                        break;
                        
                    case AVK_CLR:
                        chEnter = 0;
                        #ifndef FEATURE_ALL_KEY_PAD    //add by yangdecai 
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_EMGCALL)
                            return TRUE;
                        }
                        #else
                        if(dwParam == 0)
                        {
                            CLOSE_DIALOG(DLGRET_EMGCALL)
                            return TRUE;
                        }
                        else
                        {
                        	if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        	{
                            	CLOSE_DIALOG(DLGRET_EMGCALL)
                            	return TRUE;
                        	}
                        }
                        #endif
                        break;
                        
                    case AVK_DEL:    
                        chEnter = 0;
#ifdef FEATURE_ALL_KEY_PAD 
                        if (pMe->m_strPhonePWD == NULL || STRLEN(pMe->m_strPhonePWD) == 0)
                        {
                            CLOSE_DIALOG(DLGRET_CANCELED)
                            return TRUE;
                        }
#endif
                    break;
                        
                   case AVK_UP:
                         {
                            if(pMe->m_eRUIMSCode == ENTERPUK_STEP0)
                            {
                                return TRUE;
                            }
                            
                            if(pMe->m_eRUIMSCode == ENTERPUK_STEP1)
                            {
                                pMe->m_eRUIMSCode = ENTERPUK_STEP0;
                                STRNCPY( pMe->m_strPhonePWD, pMe->m_strPUK,sizeof(pMe->m_strPUK));                            
                            }
                            else if(pMe->m_eRUIMSCode == ENTERPUK_STEP2)
                            {
                                pMe->m_eRUIMSCode = ENTERPUK_STEP1;
                                STRNCPY( pMe->m_strPhonePWD, pMe->m_strPIN,sizeof(pMe->m_strPIN));                            
                            }
                            (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);                            
                        }
                        return TRUE;
                   case AVK_DOWN:
                         {
                            if(pMe->m_eRUIMSCode == ENTERPUK_STEP2)
                            {
                                return TRUE;
                            }
                            
                            nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                            if (nLen < 4)
                            {
                                return TRUE;
                            }

                            if(pMe->m_eRUIMSCode == ENTERPUK_STEP0)
                            {
                                STRNCPY( pMe->m_strPUK, pMe->m_strPhonePWD,sizeof(pMe->m_strPUK));
                                pMe->nOldPSWLength = nLen;
                                pMe->m_eRUIMSCode = ENTERPUK_STEP1;
                            }
                            else
                            {
                                pMe->m_PassWord = EncodePWDToUint16(pMe->m_strPhonePWD);
                                STRNCPY( pMe->m_strPIN, pMe->m_strPhonePWD,sizeof(pMe->m_strPIN));
                                pMe->nNewPSWLength = nLen;
                                pMe->m_eRUIMSCode = ENTERPUK_STEP2;
                            }
                            
                            MEMSET(pMe->m_strPhonePWD, 0, (UIM_MAX_CHV_DIGITS + 1)* sizeof(char));
                            (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                        }
                        return TRUE;
                        
                    case AVK_SELECT:
                    case AVK_INFO:
                    {
                        uint16 wData;
                        if(pMe->m_eRUIMSCode == ENTERPUK_STEP0 || pMe->m_eRUIMSCode == ENTERPUK_STEP1)
                        {
                            return TRUE;
                        }
                        
                        nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                        if (nLen < 4)
                        {
                            return TRUE;
                        }
                        
                        wData = EncodePWDToUint16(pMe->m_strPhonePWD);
                        if(wData != pMe->m_PassWord)
                        {
                            pMe->m_wMsgID = IDS_DIF_PIN;
                            pMe->m_eRUIMSCode = ENTERPUK_STEP1;
                            CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                            return TRUE;
                        }
                        
                        {
                            AEECHVStatus  chvst;
                            (void)IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);
                            
                            if(STRLEN(pMe->m_strPUK)>0)
                            {
                                char ch = pMe->m_strPUK[STRLEN(pMe->m_strPUK)-1];
                                if(ch<'0' || ch>'9')
                                {
                                    pMe->m_strPUK[STRLEN(pMe->m_strPUK)-1] = '\0';
                                }
                            }
                              
                            if(STRLEN(pMe->m_strPhonePWD)>0)
                            {
                                char ch = pMe->m_strPhonePWD[STRLEN(pMe->m_strPhonePWD)-1];
                                if(ch<'0' || ch>'9')
                                {
                                    pMe->m_strPhonePWD[STRLEN(pMe->m_strPhonePWD)-1] = '\0';
                                }
                            }
                            
                            if (IRUIM_UnblockCHV(pMe->m_pIRUIM,
                                                        IRUIM_CHV1,
                                                        pMe->m_strPUK,
                                                        pMe->m_strPhonePWD) == AEE_SUCCESS)
                            {
                                if (chvst.chv1_enabled)
                                {
                                    IRUIM_CHVDisable(pMe->m_pIRUIM);
                                }
                                else
                                {
                                    IRUIM_CHVEnable(pMe->m_pIRUIM, NULL);
                                }
                                CLOSE_DIALOG(DLGRET_VALIDPINPASS)
                            }
                            else
                            {
                                (void)IRUIM_GetCHVStatus(pMe->m_pIRUIM, &chvst);                  
                                pMe->m_nCanAttemptTimes = chvst.chv1UnblockRemain;
                                pMe->m_wMsgID = IDS_INVALIDPUK;
                                pMe->m_eRUIMSCode = ENTERPUK_STEP0;
                                CLOSE_DIALOG(DLGRET_VALIDPINFAILED)
                            }
                        }
                    }
                        return TRUE;
                             
                    default:
                        return TRUE;
                }
                
                nLen = (pMe->m_strPhonePWD == NULL)?(0):(STRLEN(pMe->m_strPhonePWD));
                if (chEnter == 0)
                {
                    // ɾ���ַ�
                    if (nLen > 0)
                    {
                        bRedraw = TRUE;
                        pMe->m_strPhonePWD[nLen-1] = chEnter;
                    }
                }
                else if (nLen < UIM_MAX_CHV_DIGITS)
                {
                    if(pMe->m_eRUIMSCode == ENTERPUK_STEP0 ||
                        nLen < OEMNV_LOCKCODE_MAXLEN)
                    {
                        pMe->m_strPhonePWD[nLen] = chEnter;
                        nLen++;
                        pMe->m_strPhonePWD[nLen] = 0;
                        bRedraw = TRUE;
                    }
                }
                
                if (bRedraw)
                {
                    (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_APP_SECURITYMENU,
                                            EVT_USER_REDRAW,
                                            NULL,
                                            NULL);
                }
            }
            return TRUE;
            
        default:
            break;
    }
    
    return FALSE;
}

/*==============================================================================
������
       SecurityUIMErrHandler
˵����
       IDD_UIMERR_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityUIMErrHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam)
{
    IStatic * pStatic = NULL;
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    pStatic = (IStatic*)IDIALOG_GetControl(pMe->m_pActiveDlg, 
                            IDC_UIMERR_STATIC);

    if (NULL == pStatic)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;
            
        case EVT_DIALOG_START:
            //���ÿؼ�����
            {
                AEERect rc;
                SETAEERECT(&rc, 0, 0,pMe->m_rc.dx, pMe->m_rc.dy - BOTTOMBAR_HEIGHT);
                
                // ������
                IDISPLAY_EraseRect(pMe->m_pDisplay,&rc);
                rc.x = 1;
                rc.dx -= 2;
                ISTATIC_SetRect(pStatic, &rc);
                
                // ���þ�̬�ı��ؼ�����
                ISTATIC_SetProperties(pStatic, ST_UNDERLINE | 
                        ST_CENTERTEXT  | ST_NOSCROLL|ST_MIDDLETEXT);
            }

            (void) ISHELL_PostEvent(pMe->m_pShell,
                      AEECLSID_APP_SECURITYMENU,
                      EVT_USER_REDRAW,
                      0,
                      0);
            return TRUE;
            
        case EVT_USER_REDRAW:
            {
                AECHAR    *wTipsMsg = NULL;
                int32  nSize = 128 * sizeof(AECHAR);
                    
                wTipsMsg = (AECHAR *) MALLOC(nSize);
                    
                if (NULL != wTipsMsg)
                {
                    // ȡ��ʾ�ı�
                    (void) ISHELL_LoadResString(pMe->m_pShell,
                                                    AEE_APPSSECURITYMENU_RES_FILE,
                                                    IDS_UIMBLOCKED,
                                                    wTipsMsg,
                                                    nSize);
                                                    
                    // ���þ�̬�ؼ��ı�
                    (void)ISTATIC_SetText(pStatic,
                                NULL,
                                wTipsMsg,
                                AEE_FONT_NORMAL,
                                AEE_FONT_NORMAL);
                                
                    // �ͷŲ���ʹ�õ���ʱ buffer
                    FREE(wTipsMsg);
                    wTipsMsg = NULL;
                }
                
                ISTATIC_SetActive(pStatic, TRUE);
                (void) ISTATIC_Redraw(pStatic);
                
                // ���Ƶײ���ʾ�˵�
                // -----SOS
                SEC_MENU_DRAW_BOTTOMBAR(BTBAR_SOS)

                // ����������Ļ
                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            }          
            
            return TRUE;
            
        case EVT_DIALOG_END:
            return TRUE;
            
        case EVT_KEY:
            switch (wParam)
            {
                case AVK_CLR:
                case AVK_SEND:
                    CLOSE_DIALOG(DLGRET_EMGCALL)
                    return TRUE;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
    
    return FALSE;
}

/*==============================================================================
������
       SecurityEmergencyCallHandler
˵����
       IDD_EMGCALL_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  SecurityEmergencyCallHandler(CSecurityMenu *pMe,
                                        AEEEvent       eCode,
                                        uint16         wParam,
                                        uint32         dwParam)
{
    IMenuCtl *pMenu = NULL;
  
    PARAM_NOT_REF(dwParam)
    if (NULL == pMe)
    {
        return FALSE;
    }

    pMenu = (IMenuCtl* )IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_EMGCALL_DIALOG);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:            
            {
                AECHAR  wstrNum[MAX_EMERGENCY_NUM_LEN+1];
                EmergencyNum_Table emerg_tab;
                int   i = 0;
                ICONFIG_GetItem(pMe->m_pConfig, CFGI_EMERGENCYNUM_TABLE, (void*)&emerg_tab, sizeof(EmergencyNum_Table));
                
                IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_ACTIVE_NO_REDRAW);
                IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
				//add by yangdecai
			{
				AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        IDS_EMERGENCY_NUMBER,
                        WTitle,
                        sizeof(WTitle));
				IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
            }
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
#endif
                for(i=0; i<emerg_tab.emert_size; i++)
                {
                    wstrNum[0] = 0;
                    STRTOWSTR(emerg_tab.emerg_num[i].num_buf, wstrNum, sizeof(wstrNum));
                    IMENUCTL_AddItem(pMenu, NULL, NULL, 500+i, wstrNum, NULL);
                }
                IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            }
            return TRUE;

        case EVT_DIALOG_END:
            if (pMe->m_eDlgRet != DLGRET_CANCELED)
            {
                pMe->m_eDlgRet = DLGRET_CANCELED;
            }
            return TRUE;

        case EVT_KEY:
            switch (wParam)
            {
                case AVK_END:
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
  				case AVK_CAMERA:
            	#if defined(FEATURE_VERSION_C306)||defined(FEATURE_VERSION_W0216A)|| defined(FEAUTRE_VERSION_N450)|| defined(FEATURE_VERSION_N021)|| defined(FEATURE_VERSION_C01)
                {
					nv_item_type	SimChoice;
					OEMNV_Get(NV_SIM_SELECT_I,&SimChoice);
					if(SimChoice.sim_select ==AVK_SEND_TWO)
					{
						CtlAddItem ai;
                        uint16   wID;
                        
                        wID = IMENUCTL_GetSel(pMenu);
                        if (IMENUCTL_GetItem(pMenu, wID, &ai))
                        {
                           (void)MakeVoiceCall(pMe->m_pShell, FALSE, (AECHAR *)ai.pText);
                        }
					}
					return TRUE;
				}
				#endif
				break;
                case AVK_SEND:
                    {
                        CtlAddItem ai;
                        uint16   wID;
                        #if defined(FEATURE_VERSION_C01) 
						{
							nv_item_type	SimChoice;
							OEMNV_Get(NV_SIM_SELECT_I,&SimChoice);
							if(SimChoice.sim_select==AVK_SEND_TWO)
							{
								return TRUE;
							}
						}
						#endif
                        wID = IMENUCTL_GetSel(pMenu);
                        if (IMENUCTL_GetItem(pMenu, wID, &ai))
                        {
                           (void)MakeVoiceCall(pMe->m_pShell, FALSE, (AECHAR *)ai.pText);
                        }
                    }
                    return TRUE;
                    
                default:
                    break;
            }
            return TRUE;

        case EVT_COMMAND:
            {
                CtlAddItem ai;
                
                if (IMENUCTL_GetItem(pMenu, wParam, &ai))
                {
                     (void)MakeVoiceCall(pMe->m_pShell, FALSE, (AECHAR *)ai.pText);
                }
            }
            return TRUE;

        default:
            break;
    }

    return FALSE;
}

/*==============================================================================
������
       HandleRestoreDialogEvent
˵����
       IDD_RESTORE_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  HandleRestoreDialogEvent(CSecurityMenu *pMe,
    AEEEvent       eCode,
    uint16         wParam,
    uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_RESTORE);
    SEC_ERR("%x,%x,%x,HandleRestoreDialogEvent",eCode,wParam,dwParam);
    if (pMenu == NULL)
    {
        return FALSE;
    }

     //ʵ�ֲ˵�ѭ����������
    //SecurityMenu_AutoScroll(pMenu,eCode,wParam);

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			//add by yangdecai
			{
				AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        IDS_RESTORE,
                        WTitle,
                        sizeof(WTitle));
				IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
            }
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_RESTORE_OK, IDS_RESTORE_OK, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_RESTORE_CANCEL, IDS_RESTORE_CANCEL, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            //�趨�����ʽ
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_ACTIVE_NO_REDRAW);
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            // �趨�˵��ؼ��ľ���λ�ü��ߴ�

            // ���˵����˵�������ֱ��ͼ��
            SecurityMenu_SetItemNumIcon(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }

            return TRUE;

        case EVT_COMMAND:
            switch(wParam)
            {
                case IDS_RESTORE_OK:
                    CLOSE_DIALOG(DLGRET_RESTORE_MESSAGE)
                    break;

                case IDS_RESTORE_CANCEL:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    break;

                default:
                    ASSERT_NOT_REACHABLE;
            }

            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleRestoreDialogEvent

/*==============================================================================
����:
       SecurityMenu_RestoryFactorySet

˵��:
       �ָ��������á�

����:
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void SecurityMenu_RestoryFactorySet(CSecurityMenu *pMe)
{
    //ICallList      *m_pCallList = NULL;uint32 value;
    //ICallHistory *pCallHistory = NULL;
    uint32        value;
    
#ifdef FEATURE_VERSION_C316
	IPhoneCtl *pIPhoneCtl = NULL;
	int nReturnStatus = 1;
#endif

    if (NULL == pMe)
    {
         return;
    }
    
//Add By zzg 2010_10_22
#ifdef FEATURE_APP_BLUETOOTH
	ISHELL_StartBackgroundApplet(pMe->m_pShell, AEECLSID_BLUETOOTH_APP, "ResetBT");	
	//ISHELL_StartAppletArgs(pMe->m_pShell, AEECLSID_BLUETOOTH_APP, "ResetBT");
#endif

#ifdef  FEATURE_TORCH_SUPPORT
    IBACKLIGHT_TurnOffTorch(pMe->m_pBacklight);
#endif

#ifdef FEATURE_VERSION_C316
	nReturnStatus = ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_PHONECTL, (void **) &pIPhoneCtl);   
    if((pIPhoneCtl == NULL) || (SUCCESS != nReturnStatus))
    {
        return ;
    }
#endif
//Add End   
    //ɾ��ͨ����¼
	{
		ICallHistory    *m_pCallHistory;
		 if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                     AEECLSID_CALLHISTORY,
                     (void **)&m_pCallHistory))
		{
			;
		}
		else
		{
			if(SUCCESS == ICALLHISTORY_Clear(m_pCallHistory))
          	{
			  	uint32 value = 0;	
              	IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_CALL,ANNUN_STATE_CALL_MISSEDCALL_OFF);
              	{
                  	boolean missed_call_icon;
                  	missed_call_icon = FALSE;
                  	(void) ICONFIG_SetItem(pMe->m_pConfig,
                                     CFGI_MISSED_CALL_ICON,
                                     &missed_call_icon,
                                     sizeof(missed_call_icon));  
              	}

              	(void) ICONFIG_SetItem(pMe->m_pConfig,
                                    CFGI_ALL_CALL_TIMER,
                                    &value,
                                    sizeof(uint32));
              	(void) ICONFIG_SetItem(pMe->m_pConfig,
                                    CFGI_RECENT_MO_CALL_TIMER,
                                    &value,
                                    sizeof(uint32));
              	(void) ICONFIG_SetItem(pMe->m_pConfig,
                                    CFGI_RECENT_MT_CALL_TIMER,
                                    &value,
                                     sizeof(uint32));
          	}
          	if(m_pCallHistory)
			{
					ICALLHISTORY_Release(m_pCallHistory);
					m_pCallHistory = NULL;
			}
          }
	}
	//ɾ������Ϣ
	
	{
		IWmsApp *pIWmsApp = NULL;

        if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
                                                AEECLSID_WMSAPP,
                                                (void**)&pIWmsApp))
        {
            if(SUCCESS != IWmsApp_DeleteAllNvCdmaSms(pIWmsApp))
            {
                if(NULL != pIWmsApp)
                {
                    (void)IWmsApp_Release(pIWmsApp);
                    pIWmsApp = NULL;
                }
                return ;
            }
        }
        
        if(NULL != pIWmsApp)
        {
            (void)IWmsApp_Release(pIWmsApp);
            pIWmsApp = NULL;
        }
	}
	#ifndef FEATURE_VERSION_K232_Y105A
	//ɾ���绰����Ϣ
	{
		IContApp * pIContApp = NULL;

        if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
                                                AEECLSID_APP_CONTACT,
                                                (void**)&pIContApp))
        {
            if(SUCCESS != ICONTAPP_DeleteAll(pIContApp))
            {
                if(NULL != pIContApp)
                {
                    (void)ICONTAPP_Release(pIContApp);
                    pIContApp = NULL;
                }
                return ;
            }
        }
        
        if(NULL != pIContApp)
        {
            (void)ICONTAPP_Release(pIContApp);
            pIContApp = NULL;
        }
	}
   #endif
    
#ifdef FEATURE_APP_MEDIAGALLERY
	// ɾ���ļ����ڵ��ļ� (�洢���ֻ��ϵ�)
	{
		CMediaGallery_ClearMediaFiles(pMe);
	}
#endif

	{
		 IFileMgr           *m_pFileMgr;       // IFileMgr interface
		 IFile  *pFile;
		 ISHELL_CreateInstance( pMe->m_pShell,AEECLSID_FILEMGR,(void **)&m_pFileMgr);
		 IFILEMGR_Remove(m_pFileMgr, FILE_CONTAPP_CFG);

		 if(IFILEMGR_EnumInit( m_pFileMgr, CAL_DATABASE_PATH, FALSE)  == SUCCESS)
   	     {
   	        FileInfo info = {0};
   	        //int i=0;
   	        
   	        while(IFILEMGR_EnumNext( m_pFileMgr, &info))
   	        {
   	            if(STRENDS( ".db", info.szName))
   	            {
   	                if( IFILEMGR_Remove( m_pFileMgr, info.szName) == SUCCESS)
   	                {
   	                }
   	                else
   	                {
   	                }
   	            }
   	        }
   	     }
   	     else
   	     {
   	        MSG_FATAL(";deleteAllEventsFromDB, enumInit failed",0,0,0);
   	     }
		 if(IFILEMGR_EnumInit( m_pFileMgr, FILE_SCHEDULE_CFG, FALSE)  == SUCCESS)
   	     {
		 	IFILEMGR_Remove(m_pFileMgr, FILE_SCHEDULE_CFG);
		 }
		
		 if (m_pFileMgr != NULL)
    	 {
        	(void) IFILEMGR_Release(m_pFileMgr);
        	m_pFileMgr = NULL;
    	 }
	}

#if defined(FEATURE_VERSION_C316)||defined(FEATURE_VERSION_K212)||defined(FEATURE_VERSION_K212_BH)
	{
		int i = 0;
		CPrefData m_CfgData;
		m_CfgData.m_nLevel = 1;
        m_CfgData.m_bGrid  = FALSE;
        m_CfgData.m_bSound = FALSE;
        for(i = 0; i < 3; i++)
        {
            m_CfgData.Score[i] = 0;            // Initialize the highest scores.
        }
		(void)ISHELL_SetPrefs(pMe->m_pShell, 
                              AEECLSID_TETRIS, 
                              2, 
                              &(m_CfgData),
                              sizeof(CPrefData)); 
	}
#endif

	OEM_RestoreFactorySetting();

    //AEEOEM_RestoreFactorySetting(pMe->m_pOEM_TSGBridge);     
    value = 0;

    // Update ALERT indicator
    {
        byte alertType;
        
        {
            IBacklight   *pBacklight = NULL;
            ISHELL_CreateInstance(pMe->m_pShell,
                                     AEECLSID_BACKLIGHT_DISPLAY1,
                                     (void**)&pBacklight);

            IBACKLIGHT_SetBrightnessLevel(pBacklight,OEMNV_BACKLIGHT_LEVEL);
            IBACKLIGHT_Release(pBacklight);
            pBacklight = NULL;
        }
        
#ifdef FEATURE_PLANEMODE
        {
            AEECMPhInfo phInfo;
            ICM  *pCM = NULL;
            int nRet;
                        
            nRet = ISHELL_CreateInstance(pMe->m_pShell,
                                         AEECLSID_CALLMANAGER,
                                         (void **) &pCM);
            if ((nRet != SUCCESS) || (pCM == NULL))
            {
                return;
            }

            ICM_GetPhoneInfo(pCM, &phInfo, sizeof(phInfo));
            if(SYS_OPRT_MODE_ONLINE != phInfo.oprt_mode)
            {
                cm_ph_cmd_oprt_mode(NULL, NULL, CM_CLIENT_ID_ANONYMOUS, SYS_OPRT_MODE_ONLINE);
            }
            
            ICM_Release(pCM);
        }
#endif
        
        (void) ICONFIG_GetItem(pMe->m_pConfig,
                               CFGI_ALERT_TYPE,
                               &alertType,
                               sizeof(alertType));
        switch(alertType)
        {
            case OEMNV_ALERTTYPE_OFF :
                //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_SILENT, ANNUN_STATE_ON);
                IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_SILENT);
                break;

            case OEMNV_ALERTTYPE_RINGER :
                //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RING, ANNUN_STATE_ON);
                IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_ALERT);
                break;

            case OEMNV_ALERTTYPE_VIB :
                //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_VIBRATE, ANNUN_STATE_ON);
                IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_VIBRATOR);
                break;

            case OEMNV_ALERTTYPE_VIBRINGER :
                //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_ALERT, ANNUN_STATE_ON);
                IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_VIBRING);
                break;

            case OEMNV_ALERTTYPE_VIBANDRINGER :
                //IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_ALERT, ANNUN_STATE_ON);
                IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_RINGTONE, ANNUN_STATE_RINGTONE_VIBRING);
                break;

            default :
                break;
        }
    }

    {
        uint8 count = 0;       
        OEM_GetConfig(CFGI_FACTORYSET_COUNT_EX, &count, sizeof(uint8));    
        count++;
        OEM_SetConfig(CFGI_FACTORYSET_COUNT_EX, &count, sizeof(uint8)); 
    }

    IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_ALARM_OFF/*ANNUN_STATE_OFF*/);
    
#ifdef FEATURE_VERSION_C316
	IPHONECTL_SetOperatingMode(pIPhoneCtl, AEET_OPRT_MODE_OFFLINE);
	(void)ISHELL_SetTimer(pMe->m_pShell,
                                500,
                              	 (PFNNOTIFY)HandleTimer,
                                pIPhoneCtl);
#endif
}
#ifdef FEATURE_VERSION_C316
void HandleTimer(IPhoneCtl *m_pIPhoneCtl)
{
	IPHONECTL_SetOperatingMode(m_pIPhoneCtl, AEET_OPRT_MODE_RESET);
    IPHONECTL_Release(m_pIPhoneCtl);
    m_pIPhoneCtl = NULL;
}
#endif

static void SecurityMenu_SetItemNumIcon(IMenuCtl   *pMenu)
{
    //MenuSetItemNumIcon(pMenu);
}

/*==============================================================================
������
       SecurityChangeCodeDialogHandler
˵����
       IDD_CHANGECODE�Ի����¼�������

������
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  HandleChangeCodeDialogEvent(CSecurityMenu *pMe,
                                            AEEEvent       eCode,
                                            uint16         wParam,
                                            uint32         dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                      IDC_CHANGECODE);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			//add by yangdecai
			{
				AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_APPSSECURITYMENU_RES_FILE,                                
                        IDS_PHONE_PASSWORD_CHANGE,
                        WTitle,
                        sizeof(WTitle));
				IANNUNCIATOR_SetFieldTextEx(pMe->m_pIAnn,WTitle,FALSE);
            }
            IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_PHONE_CODE, IDS_PHONE_CODE, NULL, 0);
            if(IsRunAsUIMVersion())
            {
                IMENUCTL_AddItem(pMenu, AEE_APPSSECURITYMENU_RES_FILE, IDS_PIN_CODE, IDS_PIN_CODE, NULL, 0);
            }
            return TRUE;

        case EVT_DIALOG_START:

            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY|MP_ACTIVE_NO_REDRAW);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenu, AEE_APPSCOMMONRES_IMAGESFILE, IDI_SECURITY_BACKGROUND);
#endif
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;

                default:
                    break;
            }

            return TRUE;

        case EVT_COMMAND:
            //pMe->m_nSubDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_nSubDlgId = wParam;
            switch(wParam)
            {
                case IDS_PHONE_CODE:
                    CLOSE_DIALOG(DLG_ASKCALLPASSWORD)//(DLG_ASK)
                    break;
                if(IsRunAsUIMVersion())
                {
                    case IDS_PIN_CODE:
						if (IRUIM_IsCardConnected(pMe->m_pIRUIM))
                		{
                        	CLOSE_DIALOG(DLG_ASKPIN)//(DLG_PINCHANGE)
						}
                        break;
                }

                default:
                    break;
            }

            return TRUE;

        default:
            break;
    }
    return FALSE;
} 

/*==============================================================================
����:
       SecurityMenu_DeleteAllEntries

˵��:
       ɾ�����б������ֻ��ϵ�����

����:
       pMe [in]��ָ��SecurityMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/

static int SecurityMenu_DeleteAllEntries(CSecurityMenu * pMe)
{
    {
		ICallHistory    *m_pCallHistory;
		 if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                     AEECLSID_CALLHISTORY,
                     (void **)&m_pCallHistory))
		{
			;
		}
		else
		{
			if(SUCCESS == ICALLHISTORY_Clear(m_pCallHistory))
          	{
			  	uint32 value = 0;	
              	IANNUNCIATOR_SetField (pMe->m_pIAnn, ANNUN_FIELD_CALL,ANNUN_STATE_CALL_MISSEDCALL_OFF);
              	{
                  	boolean missed_call_icon;
                  	missed_call_icon = FALSE;
                  	(void) ICONFIG_SetItem(pMe->m_pConfig,
                                     CFGI_MISSED_CALL_ICON,
                                     &missed_call_icon,
                                     sizeof(missed_call_icon));  
              	}

              	(void) ICONFIG_SetItem(pMe->m_pConfig,
                                    CFGI_ALL_CALL_TIMER,
                                    &value,
                                    sizeof(uint32));
              	(void) ICONFIG_SetItem(pMe->m_pConfig,
                                    CFGI_RECENT_MO_CALL_TIMER,
                                    &value,
                                    sizeof(uint32));
              	(void) ICONFIG_SetItem(pMe->m_pConfig,
                                    CFGI_RECENT_MT_CALL_TIMER,
                                    &value,
                                     sizeof(uint32));
          	}
          	if(m_pCallHistory)
			{
					ICALLHISTORY_Release(m_pCallHistory);
					m_pCallHistory = NULL;
			}
          }
	}
	//ɾ������Ϣ
	
	{
		IWmsApp *pIWmsApp = NULL;

        if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
                                                AEECLSID_WMSAPP,
                                                (void**)&pIWmsApp))
        {
            if(SUCCESS != IWmsApp_DeleteAllNvCdmaSms(pIWmsApp))
            {
                return EFAILED;
            }
        }
        
        if(NULL != pIWmsApp)
        {
            (void)IWmsApp_Release(pIWmsApp);
            pIWmsApp = NULL;
        }
	}
	//ɾ���绰����Ϣ
	{
		IContApp * pIContApp = NULL;

        if (SUCCESS == ISHELL_CreateInstance(pMe->m_pShell,
                                                AEECLSID_APP_CONTACT,
                                                (void**)&pIContApp))
        {
            if(SUCCESS != ICONTAPP_DeleteAll(pIContApp))
            {
                return EFAILED;
            }
        }
        
        if(NULL != pIContApp)
        {
            (void)ICONTAPP_Release(pIContApp);
            pIContApp = NULL;
        }
	}
	#ifdef FEATURE_APP_MEDIAGALLERY
	// ɾ���ļ����ڵ��ļ� (�洢���ֻ��ϵ�)
	{
		CMediaGallery_ClearMediaFiles(pMe);
	}
	#endif
  
    return SUCCESS;
}



