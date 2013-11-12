/*==============================================================================
// �ļ���
//        imizoneDialogHandler.c
//        ��Ȩ����(c) 2007 Anylook
//
// �ļ�˵����
//
// �������ڣ�2007-11-13
// ���ߣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/
/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
08-01-10          ��ϲ��      ʵ�ֿ��ٲ��ԵĻ�������
07-11-13                ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "mizone_priv.h"
#if defined(FEATURE_WMS_APP)
#include "WMSApp.h"
#endif
#include "mobile.h"

#define RGBA_BLACK             MAKE_RGBA(0,0,0,255)

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
// �Ի��� IDD_MAINMENU �¼�������
static boolean  MiZone_MainMenuHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_ACTIVATE �¼�������
static boolean  MiZone_ACTIVATEHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_ENDACTIVATE �¼�������
static boolean  MiZone_ENDACTIVATEHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_ASTROLOGY �¼�������
static boolean  MiZone_ASTROLOGYHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_ASTROLOGY �¼�������
static boolean  MiZone_ServicesmenuHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_ENDPULL�¼�������
static boolean  MiZone_ENDPULLHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_INPUTSEND�¼�������
static boolean  MiZone_INPUTSENDHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_SENDNAME�¼�������
static boolean  MiZone_SENDNAMEHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

static void Mizone_POPTimeOut(CMiZone *pMe);

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
������mizone_UpdateActiveDialogInfo

˵����
       ����mizone Applet�����ڹؼ��ĶԻ��������

������
       pMe [in]��ָ��mizone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam [in]����Ի�����ԴID��
       dwParam [in]:��ָ���Ի����IDialog�ӿ�ָ�롣

����ֵ��
        ��

��ע��:
       ���º��mizone Apple�Ի������ԡ�mizone_RouteDialogEvent������
       ������Щ���԰ѶԻ����¼�·������Ӧ�Ĵ�������
       �˺�������mizone_HandleEvent���á�

==============================================================================*/
void MiZone_UpdateActiveDialogInfo
(
    CMiZone *pMe,
    uint16  wParam,
    uint32 dwParam
)
{
    if (NULL == pMe)
    {
        return;
    }

    switch (wParam)
    {
        case IDD_MAINMENU:
        case IDD_MIZONEACTIVATE: 
        case IDD_ENDACTIVATE:
        case IDD_ASTROLOGY:
        case IDD_SERVICESMENU:
        case IDD_ENDPULL:
        case IDD_INPUTSEND:
        case IDD_SENDNAME:
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;
            return;

        default:
            return;
    }
}

/*==============================================================================
����:
       MiZone_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��mizone Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void MiZone_ShowDialog(CMiZone *pMe,uint16  dlgResId)
{
    int nRet;

    // At most one dialog open at once
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        return;
    }

    nRet = ISHELL_CreateDialog(pMe->m_pShell,AEE_MIZONE_RES_FILE,dlgResId,NULL);
}

/*==============================================================================
����:
       mizone_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��mizone Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
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
)
{
    if (NULL == pMe)
    {
        return FALSE;
    }
    if (NULL == pMe->m_pActiveDlg)
    {
        return FALSE;
    }
    MSG_FATAL("***MiZone_RouteDialogEvent****pMe->m_pActiveDlgID=%d",pMe->m_pActiveDlgID,0,0);
    switch (pMe->m_pActiveDlgID)
    {
        case IDD_MAINMENU:
            return MiZone_MainMenuHandler(pMe,eCode,wParam,dwParam);
            
        case IDD_MIZONEACTIVATE:
            return MiZone_ACTIVATEHandler(pMe,eCode,wParam,dwParam);

        case IDD_ENDACTIVATE:
            return MiZone_ENDACTIVATEHandler(pMe,eCode,wParam,dwParam);

        case IDD_ASTROLOGY:
            return MiZone_ASTROLOGYHandler(pMe,eCode,wParam,dwParam);

        case IDD_SERVICESMENU:
            return MiZone_ServicesmenuHandler(pMe,eCode,wParam,dwParam);

        case IDD_ENDPULL:
            return MiZone_ENDPULLHandler(pMe,eCode,wParam,dwParam);

        case IDD_INPUTSEND:
            return MiZone_INPUTSENDHandler(pMe,eCode,wParam,dwParam);

        case IDD_SENDNAME:
            return MiZone_SENDNAMEHandler(pMe,eCode,wParam,dwParam);

        default:
            return FALSE;
    }
}

/*==============================================================================
������
       MiZone_MainMenuHandler
˵����
       IDD_MAINMENU�Ի����¼�������

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  MiZone_MainMenuHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_MAINMENU);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    MSG_FATAL("***MiZone_MainMenuHandler****",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			{
				AECHAR WTitle[40] = {0};
                int i;
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_MIZONE_RES_FILE,                                
                        IDS_MIZONETITLE,
                        WTitle,
                        sizeof(WTitle));
				//IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);				
                for(i=0; i<12; i++)
                {
                    (void)IMENUCTL_AddItem(pMenu,
                                           AEE_MIZONE_RES_FILE,
                                           IDS_FUNZONE_1+i,
                                           IDS_FUNZONE_1+i,
                                           NULL,
                                           0);
                }
                return TRUE;
            }

        case EVT_DIALOG_START:
            IMENUCTL_SetProperties(pMenu,MP_UNDERLINE_TITLE|MP_WRAPSCROLL);

            IMENUCTL_SetSel(pMenu, pMe->m_mainMenuSel);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_MiZone,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
            AEERect rc;
            SETAEERECT(&rc, 0, 0, pMe->m_rc.dx, pMe->m_rc.dy-GetBottomBarHeight(pMe->m_pDisplay));
            (void)IMENUCTL_SetRect(pMenu, &rc);
            (void)IMENUCTL_Redraw(pMenu);
            MIZONE_DRAW_BOTTOMBAR(BTBAR_MIZONE_SELECT_BACK);
            // ������ʾ
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
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
            pMe->m_mainMenuSel = IMENUCTL_GetSel(pMenu);
            pMe->m_pActiveitem = wParam;
            if(pMe->m_pActiveitem == IDS_FUNZONE_12)
            {
               CLOSE_DIALOG(DLGRET_MIZONESERVICES)  
            }
            else
            {
               CLOSE_DIALOG(DLGRET_MIZONEACTIVATE)
            }
                
            #if 0
            switch (wParam)
            {
                case IDS_FUNZONE_1:       //��������
                    CLOSE_DIALOG(DLGRET_MIZONEACTIVATE)
                    break;

                default:
                    ASSERT_NOT_REACHABLE;
            } 
            #endif
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // mizone_MainMenuHandler

/*==============================================================================
������
       MiZone_ACTIVATEHandler
˵����
       IDD_MAINMENU�Ի����¼�������

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  MiZone_ACTIVATEHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    static IStatic * pStatic = NULL;
    PARAM_NOT_REF(dwParam)
    MSG_FATAL("***MiZone_ACTIVATEHandler****",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			{
                if (NULL == pStatic)
                 {
                     AEERect rect = {0};
                     if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_STATIC,(void **)&pStatic))
                     {
                         return FALSE;
                     }
                     ISTATIC_SetRect(pStatic, &rect);
                 }
                return TRUE;
            }

        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_MiZone,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
           AEERect  Rect;                      
           AECHAR * pTemp;                
           AECHAR   ActivateTEXT[1200];    
           uint16   ActivateId;
           int      i;
           
           Rect.x = 0;
           Rect.y = 0;
           Rect.dx = pMe->m_rc.dx;
           Rect.dy = pMe->m_rc.dy - GetBottomBarHeight(pMe->m_pDisplay);
        
           for(i=0; i<11; i++)
           {
             if(pMe->m_pActiveitem== IDS_FUNZONE_1+i) 
             {
                ActivateId=IDS_FUNZONE_ACTIVATE1+i;
             }
           }
           MSG_FATAL("***ActivateId=%d",ActivateId,0,0);
           (void)ISHELL_LoadResString(pMe->m_pShell, 
                                      AEE_MIZONE_RES_FILE, 
                                      ActivateId, 
                                      ActivateTEXT, 
                                      sizeof(ActivateTEXT));                            
                                       
           pTemp = WSTRCHR(ActivateTEXT,(AECHAR)'\\');
           while (pTemp)
           {
               *pTemp = (AECHAR)'\n';
               pTemp  = WSTRCHR(ActivateTEXT, (AECHAR)'\\');
           }                                   // replace '\' in the string to '\n'.
           
           ISTATIC_SetRect(pMe->m_pStatic, &Rect);
           ISTATIC_SetProperties(pMe->m_pStatic, ST_CENTERTITLE | ST_UNDERLINE | ST_NOSCROLL | ST_GRAPHIC_BG);
           ISTATIC_SetBackGround(pMe->m_pStatic, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BACKGROUND);
           (void)ISTATIC_SetText(pMe->m_pStatic, 
                                 NULL, 
                                 ActivateTEXT,
                                 AEE_FONT_NORMAL,
                                 AEE_FONT_NORMAL);
           ISTATIC_SetActive(pMe->m_pStatic, TRUE);
           ISTATIC_Redraw(pMe->m_pStatic);     // Display the help information.
           MSG_FATAL("pMe->m_pStatic.....TRUE",0,0,0);
           FREE(pTemp);
           MIZONE_DRAW_BOTTOMBAR(BTBAR_ACTIVATE_BACK);
           IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
            return TRUE;
        }
        case EVT_DIALOG_END:
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {   case AVK_SELECT:
                   {
                        MSG_FATAL("****IRUIM_IsCardConnected(pMe->m_pIRUIM)=%d",IRUIM_IsCardConnected(pMe->m_pIRUIM),0,0);
                        if (FALSE==IRUIM_IsCardConnected(pMe->m_pIRUIM))
                        {
                          PromptMsg_Param_type m_PromptMsg;
                          MEMSET(&m_PromptMsg,0,sizeof(PromptMsg_Param_type));
                          m_PromptMsg.nMsgResID= IDS_NOUIM;   
                          m_PromptMsg.ePMsgType = MESSAGE_WARNNING;
                          STRLCPY(m_PromptMsg.strMsgResFile, AEE_MIZONE_RES_FILE,MAX_FILE_NAME);
                          m_PromptMsg.eBBarType = BTBAR_NONE;
                          DrawPromptMessage(pMe->m_pDisplay,pStatic,&m_PromptMsg);
                          IDISPLAY_UpdateEx(pMe->m_pDisplay,TRUE);
                          (void)ISHELL_SetTimer(pMe->m_pShell,1000, (PFNNOTIFY)Mizone_POPTimeOut,pMe);
                          return FALSE;
                        }
                        if(pMe->m_pActiveitem == IDS_FUNZONE_5)
                        {
                          CLOSE_DIALOG(DLGRET_MIZONEASTROLOGY)  
                        }
                        else
                        {
                            char SMSINFO[MAS_BREWSETINT_STRING] = {0};
                             if(pMe->m_pActiveitem == IDS_FUNZONE_1)
                             STRCPY(SMSINFO,"MMCRI"); 
                             else if(pMe->m_pActiveitem == IDS_FUNZONE_2)
                             STRCPY(SMSINFO,"MMLG"); 
                             else if(pMe->m_pActiveitem == IDS_FUNZONE_3)
                             STRCPY(SMSINFO,"MMNJ");  
                             else if(pMe->m_pActiveitem == IDS_FUNZONE_4)
                             STRCPY(SMSINFO,"MMMET"); 
                             else if(pMe->m_pActiveitem == IDS_FUNZONE_6)
                             STRCPY(SMSINFO,"MMAOL"); 
                             else if(pMe->m_pActiveitem == IDS_FUNZONE_7)
                             STRCPY(SMSINFO,"MMDMK"); 
                             else if(pMe->m_pActiveitem == IDS_FUNZONE_8)
                             STRCPY(SMSINFO,"MMVCHAT"); 
                             else if(pMe->m_pActiveitem == IDS_FUNZONE_9)
                             STRCPY(SMSINFO,"MMDEVO");
                             else if(pMe->m_pActiveitem == IDS_FUNZONE_10)
                             STRCPY(SMSINFO,"MMWELL");
                             else if(pMe->m_pActiveitem == IDS_FUNZONE_11)
                             STRCPY(SMSINFO,"MMNEWS");
#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)                             
                            ICONFIG_SetItem(pMe->m_pConfig, CFGI_MIZONE_SMSINFO,SMSINFO, sizeof(SMSINFO)); 
#endif
                            if(Mizone_SendSms(pMe)==SUCCESS)
                            {
                               CLOSE_DIALOG(DLGRET_ENDACTIVATE)
                            }
                        }
                    return TRUE; 
                   }
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                case AVK_UP:
                case AVK_DOWN:
                    ISTATIC_HandleEvent(pMe->m_pStatic, eCode, wParam, dwParam);
                    return TRUE;
                default:
                    break;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // MiZone_ACTIVATEHandler


/*==============================================================================
������
       MiZone_ENDACTIVATEHandler
˵����
       IDD_MAINMENU�Ի����¼�������

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  MiZone_ENDACTIVATEHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)
    MSG_FATAL("***MiZone_ENDACTIVATEHandler****",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			{
                return TRUE;
            }

        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_MiZone,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
           AEERect  Rect;                      
           AECHAR * pTemp;                
           AECHAR   ActivateTEXT[1200];
           AECHAR   text[1200];
           char     strDate[64];
           AECHAR	fmt_str[128];
           uint16   ActivateId;
           int      i;
           
           Rect.x = 0;
           Rect.y = 0;
           Rect.dx = pMe->m_rc.dx;
           Rect.dy = pMe->m_rc.dy - GetBottomBarHeight(pMe->m_pDisplay);
        
           for(i=0; i<11; i++)
           {
             if(pMe->m_pActiveitem== IDS_FUNZONE_1+i) 
             {
                ActivateId=IDS_FUNZONE_ENDACTIVATE1_1+i;
             }
           }
           MSG_FATAL("***ActivateId=%d",ActivateId,0,0);
           (void)ISHELL_LoadResString(pMe->m_pShell, 
                                      AEE_MIZONE_RES_FILE, 
                                      ActivateId, 
                                      ActivateTEXT, 
                                      sizeof(ActivateTEXT));     
#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)
           OEM_GetConfig(CFGI_MIZONE_NUM, strDate, sizeof(strDate));           
           STRTOWSTR(strDate, fmt_str, sizeof(fmt_str));            
           WSPRINTF(text, sizeof(text), ActivateTEXT,  fmt_str);
#endif           
                                       
           //pTemp = WSTRCHR(ActivateTEXT,(AECHAR)'\\');
           pTemp = WSTRCHR(text,(AECHAR)'\\');
           
           while (pTemp)
           {
               *pTemp = (AECHAR)'\n';
               //pTemp  = WSTRCHR(ActivateTEXT, (AECHAR)'\\');
               pTemp  = WSTRCHR(text, (AECHAR)'\\');
           }                                   // replace '\' in the string to '\n'.
           
           ISTATIC_SetRect(pMe->m_pStatic, &Rect);
           ISTATIC_SetProperties(pMe->m_pStatic, ST_CENTERTITLE | ST_UNDERLINE | ST_NOSCROLL | ST_GRAPHIC_BG);
           ISTATIC_SetBackGround(pMe->m_pStatic, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BACKGROUND);
           (void)ISTATIC_SetText(pMe->m_pStatic, 
                                 NULL, 
                                 //ActivateTEXT,
                                 text,
                                 AEE_FONT_NORMAL,
                                 AEE_FONT_NORMAL);
           ISTATIC_SetActive(pMe->m_pStatic, TRUE);
           ISTATIC_Redraw(pMe->m_pStatic);     // Display the help information.
           MSG_FATAL("pMe->m_pStatic.....TRUE",0,0,0);
           FREE(pTemp);
           MIZONE_DRAW_BOTTOMBAR(BTBAR_FUNZONE_BACK);
           IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
            return TRUE;
        }
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                    
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                case AVK_UP:
                case AVK_DOWN:
                    ISTATIC_HandleEvent(pMe->m_pStatic, eCode, wParam, dwParam);
                    return TRUE;
                default:
                    break;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // MiZone_ENDACTIVATEHandler



/*==============================================================================
������
       MiZone_ASTROLOGYHandler
˵����
       IDD_MAINMENU�Ի����¼�������

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  MiZone_ASTROLOGYHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    ITextCtl *pIText= (ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_DDMM_TEXT); 
    PARAM_NOT_REF(dwParam)
    MSG_FATAL("***MiZone_ASTROLOGYHandler****",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			{
                return TRUE;
            }

        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_MiZone,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
           AEERect  Rect,rc;                      
           AECHAR * pTemp;                
           AECHAR   ActivateTEXT[1200]; 
           //ITextCtl *pIText=NULL;
           //IControl *pControl = IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_DDMM_TEXT);
           //static ITextCtl*   ddmmtext   =(ITextCtl*)pControl;   
           uint16   ActivateId;
           //int      i;
           
           Rect.x = 0;
           Rect.y = 0;
           Rect.dx = pMe->m_rc.dx;
           Rect.dy = pMe->m_rc.dy - 2*GetBottomBarHeight(pMe->m_pDisplay);

           rc.x = 0;
           rc.y = pMe->m_rc.dy - 2*GetBottomBarHeight(pMe->m_pDisplay);
           rc.dx = pMe->m_rc.dx;
           rc.dy = GetBottomBarHeight(pMe->m_pDisplay);

           //pIText = (ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_DDMM_TEXT);
           
           ActivateId=IDS_FUNZONE_ASTROLOGY;
           MSG_FATAL("***ActivateId=%d",ActivateId,0,0);
           (void)ISHELL_LoadResString(pMe->m_pShell, 
                                      AEE_MIZONE_RES_FILE, 
                                      ActivateId, 
                                      ActivateTEXT, 
                                      sizeof(ActivateTEXT));                            
                                       
           pTemp = WSTRCHR(ActivateTEXT,(AECHAR)'\\');
           while (pTemp)
           {
               *pTemp = (AECHAR)'\n';
               pTemp  = WSTRCHR(ActivateTEXT, (AECHAR)'\\');
           }                                   // replace '\' in the string to '\n'.
           
           ISTATIC_SetRect(pMe->m_pStatic, &Rect);
           ISTATIC_SetProperties(pMe->m_pStatic, ST_CENTERTITLE | ST_UNDERLINE | ST_NOSCROLL | ST_GRAPHIC_BG);
           ISTATIC_SetBackGround(pMe->m_pStatic, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BACKGROUND);
           (void)ISTATIC_SetText(pMe->m_pStatic, 
                                 NULL, 
                                 ActivateTEXT,
                                 AEE_FONT_NORMAL,
                                 AEE_FONT_NORMAL);

           IDISPLAY_FillRect( pMe->m_pDisplay, &rc, RGBA_BLACK);

           ITEXTCTL_SetRect(pIText, &rc);
           ITEXTCTL_Redraw(pIText);
           
          // ICONTROL_SetRect(pIText, &rc);
          // ICONTROL_Redraw(pIText);  AECHAR *pwstrText = ITEXTCTL_GetTextPtr(pIText);
           
           ISTATIC_SetActive(pMe->m_pStatic, TRUE);
           ISTATIC_Redraw(pMe->m_pStatic);     // Display the help information.

           
           MSG_FATAL("pMe->m_pStatic.....TRUE",0,0,0);
           FREE(pTemp);
           MIZONE_DRAW_BOTTOMBAR(BTBAR_OK_BACK);
           IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
            return TRUE;
        }
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT:
                    {
                       char SMSINFO[MAS_BREWSETINT_STRING] = {0};
                       char strBuf[MAS_BREWSETINT_STRING] = {0};
                       AECHAR *pwsText = ITEXTCTL_GetTextPtr(pIText);
                       WSTRTOSTR(pwsText,strBuf,sizeof(strBuf));

                       if (WSTRLEN(pwsText) == 0){
                            return  FALSE;
                       }
                       
                       STRCPY(SMSINFO,"MMDOB");
                       STRCAT(SMSINFO, strBuf);
#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)                       
                       ICONFIG_SetItem(pMe->m_pConfig, CFGI_MIZONE_SMSINFO,SMSINFO, sizeof(SMSINFO)); 
#endif
                      
                      if(Mizone_SendSms(pMe)==SUCCESS)
                      {
                        CLOSE_DIALOG(DLGRET_ENDACTIVATE)
                      }
                      return TRUE;
                    }
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                case AVK_UP:
                case AVK_DOWN:
                    ISTATIC_HandleEvent(pMe->m_pStatic, eCode, wParam, dwParam);
                    return TRUE;
                default:
                    break;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // MiZone_ASTROLOGYHandler


/*==============================================================================
������
       MiZone_MainMenuHandler
˵����
       IDD_MAINMENU�Ի����¼�������

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  MiZone_ServicesmenuHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
   // PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_SERVICESMENU);
    PARAM_NOT_REF(dwParam)
    if (pMenu == NULL)
    {
        return FALSE;
    }
    MSG_FATAL("***MiZone_ServicesmenuHandler****",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			{	
                int i;
                for(i=0; i<7; i++)
                {
                    (void)IMENUCTL_AddItem(pMenu,
                                           AEE_MIZONE_RES_FILE,
                                           IDS_FUNZONE_PULLSERVICES_1+i,
                                           IDS_FUNZONE_PULLSERVICES_1+i,
                                           NULL,
                                           0);
                }
                return TRUE;
            }

        case EVT_DIALOG_START:
            IMENUCTL_SetProperties(pMenu,MP_UNDERLINE_TITLE|MP_WRAPSCROLL);

            IMENUCTL_SetSel(pMenu, pMe->m_mainMenuSel);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_MiZone,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
            AEERect rc;
            SETAEERECT(&rc, 0, 0, pMe->m_rc.dx, pMe->m_rc.dy-GetBottomBarHeight(pMe->m_pDisplay));
            (void)IMENUCTL_SetRect(pMenu, &rc);
            (void)IMENUCTL_Redraw(pMenu);
            MIZONE_DRAW_BOTTOMBAR(BTBAR_MIZONE_SELECT_BACK);
            // ������ʾ
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
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
            pMe->m_mainMenuSel = IMENUCTL_GetSel(pMenu);
            pMe->m_pActiveitem = wParam;
            CLOSE_DIALOG(DLGRET_MIZONEENDPULL)
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // MiZone_ServicesmenuHandler



/*==============================================================================
������
       MiZone_ENDPULLHandler
˵����
       IDD_MAINMENU�Ի����¼�������

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  MiZone_ENDPULLHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    static IStatic * pStatic = NULL;
    PARAM_NOT_REF(dwParam)
    MSG_FATAL("***MiZone_ENDACTIVATEHandler****",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			{
               if (NULL == pStatic)
                {
                    AEERect rect = {0};
                    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_STATIC,(void **)&pStatic))
                    {
                        return FALSE;
                    }
                    ISTATIC_SetRect(pStatic, &rect);
                }
                return TRUE;
            }

        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_MiZone,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
           AEERect  Rect;                      
           AECHAR * pTemp;                
           AECHAR   ActivateTEXT[1200];   
           uint16   ActivateId;
           int      i;
           
           Rect.x = 0;
           Rect.y = 0;
           Rect.dx = pMe->m_rc.dx;
           Rect.dy = pMe->m_rc.dy - GetBottomBarHeight(pMe->m_pDisplay);
        
           for(i=0; i<7; i++)
           {
             //if(2 == i || 3 == i)
             //{
             //  continue;
             //}
             if(pMe->m_pActiveitem== IDS_FUNZONE_PULLSERVICES_1+i) 
             {
                ActivateId=IDS_FUNZONE_ENDPULL_1+i;
             }
           }
           MSG_FATAL("***ActivateId=%d",ActivateId,0,0);
           (void)ISHELL_LoadResString(pMe->m_pShell, 
                                      AEE_MIZONE_RES_FILE, 
                                      ActivateId, 
                                      ActivateTEXT, 
                                      sizeof(ActivateTEXT));                   
                                       
           pTemp = WSTRCHR(ActivateTEXT,(AECHAR)'\\');
           while (pTemp)
           {
               *pTemp = (AECHAR)'\n';
               pTemp  = WSTRCHR(ActivateTEXT, (AECHAR)'\\');
           }                                   // replace '\' in the string to '\n'.
           
           ISTATIC_SetRect(pMe->m_pStatic, &Rect);
           ISTATIC_SetProperties(pMe->m_pStatic, ST_CENTERTITLE | ST_UNDERLINE | ST_NOSCROLL | ST_GRAPHIC_BG);
           ISTATIC_SetBackGround(pMe->m_pStatic, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BACKGROUND);
           (void)ISTATIC_SetText(pMe->m_pStatic, 
                                 NULL, 
                                 ActivateTEXT,
                                 AEE_FONT_NORMAL,
                                 AEE_FONT_NORMAL);
           ISTATIC_SetActive(pMe->m_pStatic, TRUE);
           ISTATIC_Redraw(pMe->m_pStatic);     // Display the help information.
           MSG_FATAL("pMe->m_pStatic.....TRUE",0,0,0);
           FREE(pTemp);
           if(pMe->m_pActiveitem ==IDS_FUNZONE_PULLSERVICES_3 || pMe->m_pActiveitem ==IDS_FUNZONE_PULLSERVICES_4)
           {
             MIZONE_DRAW_BOTTOMBAR(BTBAR_OK_BACK);
           }
           else
           {
             MIZONE_DRAW_BOTTOMBAR(BTBAR_SEND_BACK);
           }
           IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
            return TRUE;
        }
        case EVT_DIALOG_END:
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT:
                     MSG_FATAL("*****pMe->m_pActiveitem=%d",pMe->m_pActiveitem,0,0);
                     if (FALSE==IRUIM_IsCardConnected(pMe->m_pIRUIM))
                     {
                       PromptMsg_Param_type m_PromptMsg;
                       MEMSET(&m_PromptMsg,0,sizeof(PromptMsg_Param_type));
                       m_PromptMsg.nMsgResID= IDS_NOUIM;   
                       m_PromptMsg.ePMsgType = MESSAGE_WARNNING;
                       STRLCPY(m_PromptMsg.strMsgResFile, AEE_MIZONE_RES_FILE,MAX_FILE_NAME);
                       m_PromptMsg.eBBarType = BTBAR_NONE;
                       DrawPromptMessage(pMe->m_pDisplay,pStatic,&m_PromptMsg);
                       IDISPLAY_UpdateEx(pMe->m_pDisplay,TRUE);
                       (void)ISHELL_SetTimer(pMe->m_pShell,1000, (PFNNOTIFY)Mizone_POPTimeOut,pMe);
                       return FALSE;
                     }
                        
                     if(pMe->m_pActiveitem ==IDS_FUNZONE_PULLSERVICES_3)
                     {
                        CLOSE_DIALOG(DLGRET_MIZONESENDNAME)
                     }
                     else if(pMe->m_pActiveitem ==IDS_FUNZONE_PULLSERVICES_4)
                     {
                        CLOSE_DIALOG(DLGRET_MIZONEINPUTSEND)
                     }
                     else
                     {
                            char SMSINFO[MAS_BREWSETINT_STRING] = {0};
                            if(pMe->m_pActiveitem == IDS_FUNZONE_PULLSERVICES_1)
                            STRCPY(SMSINFO,"MMP2ACRI"); 
                            else if(pMe->m_pActiveitem == IDS_FUNZONE_PULLSERVICES_2)
                            STRCPY(SMSINFO,"MMP2ANEWS");  
                            else if(pMe->m_pActiveitem == IDS_FUNZONE_PULLSERVICES_5)
                            STRCPY(SMSINFO,"MMP2AFFACT"); 
                            else if(pMe->m_pActiveitem == IDS_FUNZONE_PULLSERVICES_6)
                            STRCPY(SMSINFO,"MMP2AHEALTH"); 
                            else if(pMe->m_pActiveitem == IDS_FUNZONE_PULLSERVICES_7)
                            STRCPY(SMSINFO,"MMP2APTEST"); 
#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)                            
                            ICONFIG_SetItem(pMe->m_pConfig, CFGI_MIZONE_SMSINFO,SMSINFO, sizeof(SMSINFO)); 
#endif
                            if(Mizone_SendSms(pMe)==SUCCESS)
                            {
                               CLOSE_DIALOG(DLGRET_CANCELED)
                            }
                     }
                    return TRUE;
                    
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                case AVK_UP:
                case AVK_DOWN:
                    ISTATIC_HandleEvent(pMe->m_pStatic, eCode, wParam, dwParam);
                    return TRUE;
                default:
                    break;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // MiZone_ENDACTIVATEHandler

/*==============================================================================
������
       MiZone_INPUTSENDHandler
˵����
       IDD_MAINMENU�Ի����¼�������

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  MiZone_INPUTSENDHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    ITextCtl *pIText = (ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_DATEINPUTTEXT);
    PARAM_NOT_REF(dwParam)
    MSG_FATAL("***MiZone_INPUTSENDHandler****",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			{
                return TRUE;
            }

        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_MiZone,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
               AEERect  Rect,rc;                      
               AECHAR * pTemp;                
               AECHAR   ActivateTEXT[1200];
               //ITextCtl *pIText = NULL;   
               //IControl *pControl = IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_DDMM_TEXT);
               //static ITextCtl*   ddmmtext   =(ITextCtl*)pControl;   
               uint16   ActivateId;
               //int      i;
               
               Rect.x = 0;
               Rect.y = 0;
               Rect.dx = pMe->m_rc.dx;
               Rect.dy = pMe->m_rc.dy - 2*GetBottomBarHeight(pMe->m_pDisplay);

               rc.x = 0;
               rc.y = pMe->m_rc.dy - 2*GetBottomBarHeight(pMe->m_pDisplay);
               rc.dx = pMe->m_rc.dx;
               rc.dy = GetBottomBarHeight(pMe->m_pDisplay);

              // pIText = (ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_DATEINPUTTEXT);
               
               ActivateId=IDS_FUNZONE_ASTROLOGYSEND;
               MSG_FATAL("***ActivateId=%d",ActivateId,0,0);
               (void)ISHELL_LoadResString(pMe->m_pShell, 
                                          AEE_MIZONE_RES_FILE, 
                                          ActivateId, 
                                          ActivateTEXT, 
                                          sizeof(ActivateTEXT));                            
                                           
               pTemp = WSTRCHR(ActivateTEXT,(AECHAR)'\\');
               while (pTemp)
               {
                   *pTemp = (AECHAR)'\n';
                   pTemp  = WSTRCHR(ActivateTEXT, (AECHAR)'\\');
               }                                   // replace '\' in the string to '\n'.
               
               ISTATIC_SetRect(pMe->m_pStatic, &Rect);
               ISTATIC_SetProperties(pMe->m_pStatic, ST_CENTERTITLE | ST_UNDERLINE | ST_NOSCROLL | ST_GRAPHIC_BG);
               ISTATIC_SetBackGround(pMe->m_pStatic, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BACKGROUND);
               //ISTATIC_SetFontColor(pMe->m_pStatic, RGB_WHITE);
               (void)ISTATIC_SetText(pMe->m_pStatic, 
                                     NULL, 
                                     ActivateTEXT,
                                     AEE_FONT_NORMAL,
                                     AEE_FONT_NORMAL);

               //IDISPLAY_FillRect( pMe->m_pDisplay, &rc, RGBA_BLACK);

               ITEXTCTL_SetRect(pIText, &rc);
               ITEXTCTL_Redraw(pIText);
               
              // ICONTROL_SetRect(pIText, &rc);
              // ICONTROL_Redraw(pIText);  AECHAR *pwstrText = ITEXTCTL_GetTextPtr(pIText);
               
               ISTATIC_SetActive(pMe->m_pStatic, TRUE);
               ISTATIC_Redraw(pMe->m_pStatic);     // Display the help information.

               
               MSG_FATAL("pMe->m_pStatic.....TRUE",0,0,0);
               FREE(pTemp);
           MIZONE_DRAW_BOTTOMBAR(BTBAR_SEND_BACK);
           IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
            return TRUE;
        }
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT:
                    {
                      char SMSINFO[MAS_BREWSETINT_STRING] = {0};
                      char strBuf[MAS_BREWSETINT_STRING] = {0};
                      AECHAR *pwsText = ITEXTCTL_GetTextPtr(pIText);
                      WSTRTOSTR(pwsText,strBuf,sizeof(strBuf));

                      if (WSTRLEN(pwsText) == 0){
                            return  FALSE;
                      }
                      
                      STRCPY(SMSINFO,"MMP2AAST");
                      STRCAT(SMSINFO, strBuf);
                      
#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)                       
                      ICONFIG_SetItem(pMe->m_pConfig, CFGI_MIZONE_SMSINFO,SMSINFO, sizeof(SMSINFO)); 
#endif
                      
                      if(Mizone_SendSms(pMe)==SUCCESS)
                      {
                        CLOSE_DIALOG(DLGRET_CANCELED)
                      }
                      return TRUE;
                    }
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                case AVK_UP:
                case AVK_DOWN:
                    ISTATIC_HandleEvent(pMe->m_pStatic, eCode, wParam, dwParam);
                    return TRUE;
                default:
                    break;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // MiZone_INPUTSENDHandler


/*==============================================================================
������
       MiZone_SENDNAMEHandler
˵����
       IDD_MAINMENU�Ի����¼�������

������
       pMe [in]��ָ��CMiZone Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  MiZone_SENDNAMEHandler(CMiZone *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    ITextCtl *pITextFirstName=(ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_FIRSTNAMETEXT);
    ITextCtl *pITextpartnerName=(ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_PARTNERNAMETEXT);
    PARAM_NOT_REF(dwParam)
    MSG_FATAL("***MiZone_SENDNAMEHandler****",0,0,0);
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
			{
                return TRUE;
            }

        case EVT_DIALOG_START:
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_MiZone,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
               AECHAR  strfristname[50]={0};
               AECHAR  strpartnername[50]={0};
               AECHAR  strtep[50]={0};
               AEERect oldClip = {0};
               AEERect  RectTextFirst = {0};
               AEERect  Rectpartner = {0};
               AEERect  rcfristname = {0};
               AEERect  rcpartnername = {0};
               AEERect  rctep = {0}; 

               Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
              // IDISPLAY_FillRect(pMe->m_pDisplay,&pMe->m_rc, RGB_BLACK);
             
              //** pITextFirstName=(ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_FIRSTNAMETEXT);
              //** pITextpartnerName=(ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg, IDC_PARTNERNAMETEXT);
              
               
               rcfristname.x = 0;
               rcfristname.y = 0;
               rcfristname.dx = pMe->m_rc.dx;
               rcfristname.dy = GetBottomBarHeight(pMe->m_pDisplay);
               
               RectTextFirst = rcfristname;
               RectTextFirst.y=GetBottomBarHeight(pMe->m_pDisplay);
               RectTextFirst.dy=GetBottomBarHeight(pMe->m_pDisplay);

               rcpartnername  = rcfristname;
               rcpartnername.y= 2*GetBottomBarHeight(pMe->m_pDisplay);
               
               Rectpartner = RectTextFirst;
               Rectpartner.y=4*GetBottomBarHeight(pMe->m_pDisplay);

               rctep = rcfristname;
               rctep.y = 5*GetBottomBarHeight(pMe->m_pDisplay);

               ITEXTCTL_SetProperties(pITextFirstName, TP_STARKEY_SWITCH | TP_FIXOEM|TP_FOCUS_NOSEL|TP_GRAPHIC_BG);
               ITEXTCTL_SetProperties(pITextpartnerName, TP_STARKEY_SWITCH | TP_FIXOEM|TP_FOCUS_NOSEL|TP_GRAPHIC_BG);

               //ITEXTCTL_SetProperties(pITextFirstName, TP_FIXSETRECT|TP_FIXOEM|TP_FOCUS_NOSEL|TP_STARKEY_SWITCH|TP_GRAPHIC_BG);
               //ITEXTCTL_SetProperties(pITextpartnerName, TP_FIXSETRECT|TP_GRAPHIC_BG);
               IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
               MSG_FATAL("****RectTextFirst.y=%d--RectTextFirst.dx=%d---RectTextFirst.dy=%d",RectTextFirst.y,RectTextFirst.dx,RectTextFirst.dy);
               ITEXTCTL_SetRect(pITextFirstName, &RectTextFirst);
               ITEXTCTL_Redraw(pITextFirstName);
                
               ITEXTCTL_SetRect(pITextpartnerName, &Rectpartner);
               ITEXTCTL_Redraw(pITextpartnerName);
               
               ITEXTCTL_SetActive(pITextFirstName, TRUE);
               (void)ISHELL_LoadResString(pMe->m_pShell, AEE_MIZONE_RES_FILE,
                                IDS_FLAMES_FIRSTNAME, strfristname,sizeof(strfristname));
               #if 0
               (void)ISHELL_LoadResString(pMe->m_pShell, AEE_MIZONE_RES_FILE,
                                IDS_FLAMES_PARTNERNAME, strpartnername,sizeof(strpartnername));
               (void)ISHELL_LoadResString(pMe->m_pShell, AEE_MIZONE_RES_FILE,
                                IDS_FLAMES_TIP, strtep,sizeof(strtep));

               IDisplay_GetClipRect(pMe->m_pDisplay, &oldClip);
               IDISPLAY_DrawText( pMe->m_pDisplay, 
                                AEE_FONT_BOLD, strfristname,
                                -1, 1, rcfristname.y, &rcfristname, 
                                IDF_TEXT_TRANSPARENT|IDF_ALIGN_LEFT);

               IDISPLAY_DrawText( pMe->m_pDisplay, 
                                AEE_FONT_BOLD, strpartnername,
                                -1, 1, rcpartnername.y, &rcpartnername, 
                                IDF_TEXT_TRANSPARENT|IDF_ALIGN_LEFT);

               IDISPLAY_DrawText( pMe->m_pDisplay, 
                                AEE_FONT_BOLD, strtep,
                                -1, 1, rctep.y, &rctep, 
                                IDF_TEXT_TRANSPARENT|IDF_ALIGN_LEFT);
               IDisplay_SetClipRect(pMe->m_pDisplay, &oldClip);
               #else
               IDISPLAY_DrawText( pMe->m_pDisplay, 
                                AEE_FONT_NORMAL, strfristname,
                                -1, 1, rcfristname.y, &rcfristname, 
                                IDF_TEXT_TRANSPARENT|IDF_ALIGN_LEFT);

               IDISPLAY_DrawText( pMe->m_pDisplay, 
                                AEE_FONT_NORMAL, L"Type your partner's",
                                -1, 1, rcpartnername.y, &rcpartnername, 
                                IDF_TEXT_TRANSPARENT|IDF_ALIGN_LEFT);
               
               rcpartnername.y=rcpartnername.y+GetBottomBarHeight(pMe->m_pDisplay);
               IDISPLAY_DrawText( pMe->m_pDisplay, 
                                AEE_FONT_NORMAL, L"first name:",
                                -1, 1, rcpartnername.y, &rcpartnername, 
                                IDF_TEXT_TRANSPARENT|IDF_ALIGN_LEFT);

               IDISPLAY_DrawText( pMe->m_pDisplay, 
                                AEE_FONT_NORMAL, L"Cost of request: Rs.3",
                                -1, 1, rctep.y, &rctep, 
                                IDF_TEXT_TRANSPARENT|IDF_ALIGN_LEFT);
               
               rctep.y=rctep.y+GetBottomBarHeight(pMe->m_pDisplay);
               IDISPLAY_DrawText( pMe->m_pDisplay, 
                                AEE_FONT_NORMAL, L"per SMS.",
                                -1, 1, rctep.y, &rctep, 
                                IDF_TEXT_TRANSPARENT|IDF_ALIGN_LEFT);
               #endif
              
            IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_BLACK);//�ָ��ı���ʾ��ɫ
            MIZONE_DRAW_BOTTOMBAR(BTBAR_SEND_BACK);
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE); 
            return TRUE;
        }
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT:
                    {
                      char SMSINFO[MAS_BREWSETINT_STRING] = {0};
                      char strFirstName[MAS_BREWSETINT_STRING]={0};
                      char strpartnerName[MAS_BREWSETINT_STRING]={0};
                      AECHAR *pwsTextFirstName = ITEXTCTL_GetTextPtr(pITextFirstName);
                      AECHAR *pwsTextpartnerName = ITEXTCTL_GetTextPtr(pITextpartnerName);

                      if ((WSTRLEN(pwsTextFirstName) == 0) || (WSTRLEN(pwsTextpartnerName) == 0)){
                            return FALSE;
                      }
                      
                      WSTRTOSTR(pwsTextFirstName,strFirstName,sizeof(strFirstName));
                      WSTRTOSTR(pwsTextpartnerName,strpartnerName,sizeof(strpartnerName));
                      STRCPY(SMSINFO,"MMP2AFLAME");
                      STRCAT(SMSINFO,strFirstName);
                      STRCAT(SMSINFO,strpartnerName);
#if defined (FEATURE_VERSION_C337) || defined(FEATURE_VERSION_IC241A_MMX)                       
                      ICONFIG_SetItem(pMe->m_pConfig, CFGI_MIZONE_SMSINFO,SMSINFO, sizeof(SMSINFO)); 
#endif
                 
                     if(Mizone_SendSms(pMe)==SUCCESS)
                     {
                       CLOSE_DIALOG(DLGRET_CANCELED)
                     }
                     return TRUE;
                    }
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
                    
                default:
                    break;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // MiZone_SENDNAMEHandler

/*==============================================================================
������
    Mizone_SendSms

˵����
    ������������ע����Ϣ��

������
    pMe [in]��ָ��MIZONE Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ��
    int 

��ע:

==============================================================================*/
int Mizone_SendSms(CMiZone *pme)
{
    int  result = SUCCESS;
    IWmsApp *pIWmsApp = NULL;
	AECHAR  wstrType[2] = {(AECHAR)MIZONE_MSG, 0};
	
    MSG_FATAL("START Mizone_SendSms",0,0,0);
    
    if (pme == NULL)
    {
        return EFAILED;
    }
            
    result = ISHELL_CreateInstance(pme->m_pShell,
                                 AEECLSID_WMSAPP,
                                 (void **) &pIWmsApp);
    if ((result == SUCCESS) && (NULL != pIWmsApp))
    {
        result = IWmsApp_SendSpecMessage(pIWmsApp, wstrType);
        IWmsApp_Release(pIWmsApp);
    }
    
    MSG_FATAL("END CoreApp_MobileTracker==%d",result,0,0);
    return result;
   
   
}
static void Mizone_POPTimeOut(CMiZone *pMe)
{
    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent(pMe->m_pShell,
                            AEECLSID_MiZone,
                            EVT_USER_REDRAW,
                            0,
                            0);
}






#if 0
void CoreApp_MobileTrackerTimer(void *pme)
{
	CCoreApp *pMe = (CCoreApp *)pme;
   
  	 if (NULL == pMe)
   	{
      	return;
   	}
   
   	// ����EVT_DISPLAYDIALOGTIMEOUT�¼�
  	(void) ISHELL_PostEvent(pMe->a.m_pIShell,
                          AEECLSID_MiZone,
                          EVT_MOBILE_TRACKER,
                          0,
                          0);
}
#endif

