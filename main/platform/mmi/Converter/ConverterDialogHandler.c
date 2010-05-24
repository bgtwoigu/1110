/*==============================================================================
// �ļ���
//        ConverterDialogHandler.c
//        2008-10-28 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2008 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�Gemsea
// �������ڣ�2008-10-28
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��       �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
08-10-28                   Initial version
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "Converter_priv.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// �رնԻ����
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                                 }

#define IS_NOT_NUMBER(c) ((c != 0) && (c != '-') && (c > '9' || c < '0'))

#define FEATURE_RESET_CURRENCY
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// �Ի��� IDD_MSGBOX �¼�������
static boolean  Converter_MsgBoxEvent(CConverter *pMe,
        AEEEvent eCode,
        uint16 wParam,
        uint32 dwParam
);

// �Ի��� IDD_CONVERTERMAIN �¼�������
static boolean  Converter_MainEvent(CConverter *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_CONVERTERCONVERT �¼�������
static boolean  Converter_ConvertEvent(CConverter *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_CHANGECURRENCY �¼�������
static boolean  Converter_ChangeCurrencyEvent(CConverter *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// ���㵥λ������
static double Converter_CalcResult(CConverter *pMe);
// ������֮��ĸ�����
static boolean Converter_FloatToWstr(double source, AECHAR * pdest, int nSize);
// �����������ֱ��
static void Converter_AddChar(CConverter *pMe, char chAdd, char *pDest);
// ��TextCtl��ȡ�ַ��������ݵ��������ֻ�����
static void Converter_GetTextToInputNum(CConverter *pMe, AECHAR *pwstrTemp, char *pDest);

static void Converter_DialogTimeout(void *pme);
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
       Converter_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��Converter Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void Converter_ShowDialog(CConverter *pMe,uint16  dlgResId)
{
    int nRet;

    // At most one dialog open at once
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        ERR("Trying to create a dialog without closing the previous one",0,0,0);
        return;
    }
    
    nRet = ISHELL_CreateDialog(pMe->m_pShell,AEE_CONVERTER_LANGFILE,dlgResId,NULL);

    if (nRet != SUCCESS)
    {
        ERR("Failed to create the dialog in the Converter applet",0,0,0);
    }
}


/*==============================================================================
����:
   Converter_ShowMsgBox

˵��:
    ������״̬����������������Ϣ�Ի�����ʾ��Ϣ��ʾ�û���

����:
    pMe [in]: ָ�� recentcalls Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wTextResId [in]: ��Ϣ�ı���Դ ID��

����ֵ:
    none

��ע:

==============================================================================*/
void Converter_ShowMsgBox(CConverter *pMe, uint16 wTextResId)
{
    pMe->m_wMsgResID = wTextResId;
   
    // ������Ϣ�Ի���.
    Converter_ShowDialog(pMe, IDD_MSGBOX);
}

/*==============================================================================
������
       Converter_RouteDialogEvent
˵����
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
boolean Converter_RouteDialogEvent(CConverter *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    if (NULL == pMe)
    {
        return FALSE;
    }

    if (NULL == pMe->m_pDialog)
    {
        return FALSE;
    }
    
    DBGPRINTF("curstate %d prestate %d dlgret %d Converter_RouteDialogEvent",pMe->m_eCurState,pMe->m_ePreState,pMe->m_eDlgRet);
    
    switch (pMe->m_pActiveDlgID)
    {
        case IDD_MSGBOX:
            DBGPRINTF("IDD_MSGBOX",0,0,0);
            return Converter_MsgBoxEvent(pMe,eCode,wParam,dwParam);

        case IDD_CONVERTERMAIN:
            DBGPRINTF("IDD_CONVERTERMAIN",0,0,0);
            return Converter_MainEvent(pMe,eCode,wParam,dwParam);

        case IDD_CONVERTERCONVERT:
            DBGPRINTF("IDD_CONVERTERCONVERT",0,0,0);
            return Converter_ConvertEvent(pMe,eCode,wParam,dwParam);

        case IDD_CHANGECURRENCY:
            DBGPRINTF("IDD_CHANGECURRENCY",0,0,0);
            return Converter_ChangeCurrencyEvent(pMe,eCode,wParam,dwParam);

        default:
            return FALSE;
    }
}

/*==============================================================================
������
       Converter_MsgBoxEvent
˵����
       IDD_MSGBOX�Ի����¼�������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean  Converter_MsgBoxEvent(CConverter *pMe,
        AEEEvent eCode,
        uint16 wParam,
        uint32 dwParam
)
{
     static IStatic * pStatic = NULL;
    if (NULL == pMe)
    {
        return FALSE;
    }

    switch(eCode)
    {
        case EVT_DIALOG_INIT:
        {
           if (NULL == pStatic)
           {
                AEERect rect = {0};
                if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                                         AEECLSID_STATIC,
                                                         (void **)&pStatic))
                
                {
                    return FALSE;
                }        
                ISTATIC_SetRect(pStatic, &rect);
           }
        }
           return TRUE;

        case EVT_DIALOG_START:
            ISHELL_SetTimer(pMe->m_pShell,CONVERTER_PROMPTMSG_TIMER,Converter_DialogTimeout,pMe);
            (void) ISHELL_PostEventEx(pMe->m_pShell, 
                                    EVTFLG_ASYNC,
                                    AEECLSID_CONVERTER,
                                    EVT_USER_REDRAW,
                                    0, 
                                    0);

            return TRUE;

        case EVT_USER_REDRAW:
            {
                PromptMsg_Param_type  Msg_Param={0};
                AECHAR  wstrText[MSGBOX_MAXTEXTLEN];
                
                // ����Դ�ļ�ȡ��Ϣ����
                (void)ISHELL_LoadResString(pMe->m_pShell,
                                AEE_CONVERTER_LANGFILE,                                
                                pMe->m_wMsgResID,
                                wstrText,
                                sizeof(wstrText));
                                
                Msg_Param.ePMsgType = MESSAGE_INFORMATIVE;
                Msg_Param.pwszMsg = wstrText;
                Msg_Param.eBBarType = BTBAR_OK;
                DrawPromptMessage(pMe->m_pDisplay, pStatic, &Msg_Param);
            }
            // ���½���
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            return TRUE;

        case EVT_DIALOG_END:
            if(pMe->m_wMsgResID == IDS_DONE)
            {
                pMe->m_eDlgRet = DLGRET_MSGBOX_OK;
            }
            else
            {
                pMe->m_eDlgRet = DLGRET_MSGBOX_CANCEL;
            }
            ISTATIC_Release(pStatic);
            pStatic = NULL;
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL,  pMe);
            return TRUE;
            
        case EVT_KEY:
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL,  pMe);
            if(pMe->m_wMsgResID == IDS_DONE)
            {
                CLOSE_DIALOG(DLGRET_MSGBOX_OK);
            }
            else
            {
                CLOSE_DIALOG(DLGRET_MSGBOX_CANCEL);
            }
            return TRUE;
            
        case EVT_DISPLAYDIALOGTIMEOUT:
            if(pMe->m_wMsgResID == IDS_DONE)
            {
                CLOSE_DIALOG(DLGRET_MSGBOX_OK);
            }
            else
            {
                CLOSE_DIALOG(DLGRET_MSGBOX_CANCEL);
            }
            return TRUE;
        
        default:
            break;
    }

    return FALSE;
}

/*==============================================================================
������
       Converter_MainEvent
˵����
       IDD_CONVERTERMAIN�Ի����¼�������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean  Converter_MainEvent(CConverter *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pDialog, IDC_CONVERTERMAIN);
    if (pMenu == NULL)
    {
        return FALSE;
    }
    
    switch(eCode)
    {
        case EVT_DIALOG_INIT:
            IMENUCTL_AddItem(pMenu, AEE_CONVERTER_LANGFILE, IDS_CURRENCY_TITLE, IDS_CURRENCY_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_CONVERTER_LANGFILE, IDS_LENGTH_TITLE, IDS_LENGTH_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_CONVERTER_LANGFILE, IDS_WEIGHT_TITLE, IDS_WEIGHT_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_CONVERTER_LANGFILE, IDS_VOLUME_TITLE, IDS_VOLUME_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_CONVERTER_LANGFILE, IDS_AREA_TITLE, IDS_AREA_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_CONVERTER_LANGFILE, IDS_TEMPERATURE_TITLE, IDS_TEMPERATURE_TITLE, NULL, 0);
            return TRUE;
       
        case EVT_DIALOG_START:
            IMENUCTL_SetProperties(pMenu, MP_BIND_ITEM_TO_NUMBER_KEY |MP_UNDERLINE_TITLE|MP_WRAPSCROLL);
            IMENUCTL_SetOemProperties(pMenu, OEMMP_USE_MENU_STYLE);
            IMENUCTL_SetBottomBarType(pMenu,BTBAR_SELECT_BACK);
            (void)ISHELL_PostEvent(pMe->m_pShell, 
                                            AEECLSID_CONVERTER, 
                                            EVT_USER_REDRAW, 
                                            NULL, 
                                            NULL); 
            return TRUE;
       
        case EVT_USER_REDRAW:
            IMENUCTL_SetSel(pMenu,pMe->m_selectedItem); 
            IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;
         
        case EVT_KEY:
            if(wParam == AVK_CLR)
            {
                CLOSE_DIALOG(DLGRET_CANCELED)
            }
            return TRUE;
          
        case EVT_COMMAND:
            //pMe->m_selectedItem= IMENUCTL_GetSel(pMenu);
            pMe->m_selectedItem= wParam;
            switch(wParam)
            {
                case IDS_CURRENCY_TITLE:
                    CLOSE_DIALOG(DLGRET_CURRENCY)
                    break;
           
                case IDS_LENGTH_TITLE:
                    CLOSE_DIALOG(DLGRET_LENGTH)
                    break;
               
                case IDS_WEIGHT_TITLE:
                    CLOSE_DIALOG(DLGRET_WEIGHT)
                    break;
            
                case IDS_VOLUME_TITLE:
                    CLOSE_DIALOG(DLGRET_VOLUME)
                    break;
               
                case IDS_AREA_TITLE:
                    CLOSE_DIALOG(DLGRET_AREA)
                    break;
    
                case IDS_TEMPERATURE_TITLE:
                    CLOSE_DIALOG(DLGRET_TEMPERATURE)
                    break;
    
                default:
                    break;
            }
            return TRUE;
          
        case EVT_DIALOG_END:
            return TRUE;
       
        default:
            break;
    }
    return FALSE;
}

/*==============================================================================
������
       Converter_ConvertEvent
˵����
       IDD_CONVERTERCONVERT�Ի����¼�������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean  Converter_ConvertEvent(CConverter *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    static char   *m_inputNumber = NULL;
    int              nLen = 0;
    AECHAR       wstrDisplay[MAX_INPUT_NUMBER+3] = {0};
    ERR("%x, %x ,%x,Converter_ConvertEvent",eCode,wParam,dwParam);
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
        {
            int i                = 0;
            if(m_inputNumber == NULL)
            {
                m_inputNumber = (char *)MALLOC((MAX_INPUT_NUMBER + 3)* sizeof(char));
            }
            
            switch(pMe->m_converterMode)
            {
                case CONVERTER_MODE_CURRENCY:
                    {
                        (void)ISHELL_GetPrefs(pMe->m_pShell,
                                            AEECLSID_CONVERTER,
                                            CONVERTER_CFG_CURRENCY,
                                            &pMe->m_CurrencyCFG,
                                            sizeof(ConverterCurrencyCfg));
                
                        for(i=0;i<CONVERTER_CURRENCY_NUMBER;i++)
                        {
                            if(pMe->m_CurrencyCFG.customCurrency[i] <= 0)
                            {
                                pMe->m_CurrencyCFG.customCurrency[i] = coefficient[i];
                            }
                        }
                        
                        for(i=0;i<CONVERTER_CURRENCY_NUMBER;i++)
                        {
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu1,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_CURRENCY_EUR + i,
                                                            IDS_CURRENCY_EUR + i,
                                                            NULL,
                                                            0);
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu2,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_CURRENCY_EUR + i,
                                                            IDS_CURRENCY_EUR + i,
                                                            NULL,
                                                            0);
                        }
                    }
                    break;

                case CONVERTER_MODE_LENGTH:
                    {
                        for(i=0;i<=(IDS_LENGTH_M - IDS_LENGTH_KM);i++)
                        {
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu1,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_LENGTH_KM + i,
                                                            IDS_LENGTH_KM + i,
                                                            NULL,
                                                            0);
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu2,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_LENGTH_KM + i,
                                                            IDS_LENGTH_KM + i,
                                                            NULL,
                                                            0);
                        }
                    }
                    break;

                case CONVERTER_MODE_WEIGHT:
                    {
                        for(i=0;i<=(IDS_WEIGHT_GRAIN - IDS_WEIGHT_OZ);i++)
                        {
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu1,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_WEIGHT_OZ + i,
                                                            IDS_WEIGHT_OZ + i,
                                                            NULL,
                                                            0);
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu2,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_WEIGHT_OZ + i,
                                                            IDS_WEIGHT_OZ + i,
                                                            NULL,
                                                            0);
                        }
                    }
                    break;

                case CONVERTER_MODE_VOLUME:
                    {
                        for(i=0;i<=(IDS_VOLUME_GAL - IDS_VOLUME_CM3);i++)
                        {
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu1,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_VOLUME_CM3 + i,
                                                            IDS_VOLUME_CM3 + i,
                                                            NULL,
                                                            0);
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu2,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_VOLUME_CM3 + i,
                                                            IDS_VOLUME_CM3 + i,
                                                            NULL,
                                                            0);
                        }
                    }
                    break;

                case CONVERTER_MODE_AREA:
                    {
                        for(i=0;i<=(IDS_AREA_ACRE - IDS_AREA_CM2);i++)
                        {
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu1,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_AREA_CM2 + i,
                                                            IDS_AREA_CM2 + i,
                                                            NULL,
                                                            0);
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu2,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_AREA_CM2 + i,
                                                            IDS_AREA_CM2 + i,
                                                            NULL,
                                                            0);
                        }
                    }
                    break;

                case CONVERTER_MODE_TEMPERATURE:
                    {
                        for(i=0;i<=(IDS_TEMPERATURE_F - IDS_TEMPERATURE_C);i++)
                        {
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu1,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_TEMPERATURE_C + i,
                                                            IDS_TEMPERATURE_C + i,
                                                            NULL,
                                                            0);
                            (void)IMENUCTL_AddItem( pMe->pUnitMenu2,
                                                            AEE_CONVERTER_LANGFILE,
                                                            IDS_TEMPERATURE_C + i,
                                                            IDS_TEMPERATURE_C + i,
                                                            NULL,
                                                            0);
                        }
                    }
                    break;

                default:
                    return TRUE;
            }
            
            pMe->m_nCtlID = IDC_NUMBER1;
            return TRUE;
        }
            
        case EVT_DIALOG_START:
        {
            switch(pMe->m_converterMode)
            {
                case CONVERTER_MODE_CURRENCY:
                    {
                        if((pMe->basiccoefficient >= IDS_CURRENCY_EUR) && (pMe->basiccoefficient <= IDS_CURRENCY_ILS))
                        {
                            IMENUCTL_SetSel(pMe->pUnitMenu1, pMe->basiccoefficient);
                        }
                        else
                        {
                            IMENUCTL_SetSel(pMe->pUnitMenu1, IDS_CURRENCY_EUR);
                        }
                        IMENUCTL_SetSel(pMe->pUnitMenu2, IDS_CURRENCY_EUR + 1);
                    }
                    break;
                    
                case CONVERTER_MODE_LENGTH:
                    {
                        IMENUCTL_SetSel(pMe->pUnitMenu1, IDS_LENGTH_KM);
                        IMENUCTL_SetSel(pMe->pUnitMenu2, IDS_LENGTH_KM + 1);
                    }
                    break;
                    
                case CONVERTER_MODE_WEIGHT:
                    {
                        IMENUCTL_SetSel(pMe->pUnitMenu1, IDS_WEIGHT_OZ);
                        IMENUCTL_SetSel(pMe->pUnitMenu2, IDS_WEIGHT_OZ + 1);
                    }
                    break;
                    
                case CONVERTER_MODE_VOLUME:
                    {
                        IMENUCTL_SetSel(pMe->pUnitMenu1, IDS_VOLUME_CM3);
                        IMENUCTL_SetSel(pMe->pUnitMenu2, IDS_VOLUME_CM3 + 1);
                    }
                    break;
                    
                case CONVERTER_MODE_AREA:
                    {
                        IMENUCTL_SetSel(pMe->pUnitMenu1, IDS_AREA_CM2);
                        IMENUCTL_SetSel(pMe->pUnitMenu2, IDS_AREA_CM2 + 1);
                    }
                    break;
                    
                case CONVERTER_MODE_TEMPERATURE:
                    {
                        IMENUCTL_SetSel(pMe->pUnitMenu1, IDS_TEMPERATURE_C);
                        IMENUCTL_SetSel(pMe->pUnitMenu2, IDS_TEMPERATURE_C + 1);
                    }
                    break;

                default:
                    return TRUE;
            }
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_CONVERTER,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);
            return TRUE;
        }
            
        case EVT_USER_REDRAW:
        {
            AECHAR                      text[16]     = {0};
            TitleBar_Param_type     TBarParam = {0};
            BottomBar_Param_type  BBarParam ={0};
            IImage                       *Image;
            AEERect                     rect;
            
            //������
            Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
            // ������
            switch(pMe->m_converterMode)
            {
                case CONVERTER_MODE_CURRENCY:
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                        AEE_CONVERTER_LANGFILE,
                                                        IDS_CURRENCY_TITLE, 
                                                        text,
                                                        sizeof(text));
                    break;
                
                case CONVERTER_MODE_LENGTH:
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                        AEE_CONVERTER_LANGFILE,
                                                        IDS_LENGTH_TITLE, 
                                                        text,
                                                        sizeof(text));
                    break;
                
                case CONVERTER_MODE_WEIGHT:
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                        AEE_CONVERTER_LANGFILE,
                                                        IDS_WEIGHT_TITLE, 
                                                        text,
                                                        sizeof(text));
                    break;
                
                case CONVERTER_MODE_VOLUME:
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                        AEE_CONVERTER_LANGFILE,
                                                        IDS_VOLUME_TITLE, 
                                                        text,
                                                        sizeof(text));
                    break;
                
                case CONVERTER_MODE_AREA:
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                        AEE_CONVERTER_LANGFILE,
                                                        IDS_AREA_TITLE, 
                                                        text,
                                                        sizeof(text));
                    break;
                
                case CONVERTER_MODE_TEMPERATURE:
                    (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                        AEE_CONVERTER_LANGFILE,
                                                        IDS_TEMPERATURE_TITLE, 
                                                        text,
                                                        sizeof(text));
                    break;
                    
                default:
                    break;
            }
            TBarParam.pwszTitle = text;
            TBarParam.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
            DrawTitleBar(pMe->m_pDisplay, &TBarParam);
            //����ͷ
            Image = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_LEFTARROW);
            if(Image)
            {
                IIMAGE_Draw(Image, pMe->m_rc.x, pMe->m_rc.y + TITLEBAR_HEIGHT + (pMe->dyMenu - ARROW_HEIGHT)/2);
                IIMAGE_Draw(Image, pMe->m_rc.x, pMe->m_rc.y + TITLEBAR_HEIGHT + pMe->dyMenu*2 + (pMe->dyMenu - ARROW_HEIGHT)/2);
                IIMAGE_Release(Image);
            }
            
            Image = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_RIGHTARROW);
            if(Image)
            {
                IIMAGE_Draw(Image, pMe->m_rc.dx - ARROW_WIDTH, pMe->m_rc.y + TITLEBAR_HEIGHT + (pMe->dyMenu - ARROW_HEIGHT)/2);
                IIMAGE_Draw(Image, pMe->m_rc.dx - ARROW_WIDTH, pMe->m_rc.y + TITLEBAR_HEIGHT + pMe->dyMenu*2 + (pMe->dyMenu - ARROW_HEIGHT)/2);
                IIMAGE_Release(Image);
            }

            if(pMe->m_converterMode == CONVERTER_MODE_TEMPERATURE)
            {
                Image = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_CONV_POUND);
                if(Image)
                {
                    IIMAGE_Draw(Image, pMe->m_rc.dx - CHARACTOR_PIC_WIDTH - 5*FRAME_SIZE, pMe->m_rc.dy - 2*BOTTOMBAR_HEIGHT);
                    IIMAGE_Release(Image);
                }
            }
            
            Image = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_CONV_STAR);
            if(Image)
            {
                IIMAGE_Draw(Image, pMe->m_rc.x + 5*FRAME_SIZE, pMe->m_rc.dy - 2*BOTTOMBAR_HEIGHT);
                IIMAGE_Release(Image);
                Image = NULL;
            }

            //������
            nLen = STRLEN(m_inputNumber);
            if(pMe->m_nCtlID == IDC_UNIT_MENU1 ||pMe->m_nCtlID == IDC_UNIT_MENU2 ||nLen <= 0)
            {
                if(pMe->m_converterMode == CONVERTER_MODE_CURRENCY)
                {
                    BBarParam.eBBarType = BTBAR_EXCHANGE_BACK;
                }
                else
                {
                    BBarParam.eBBarType = BTBAR_BACK;
                }
            }
            else
            {
                if(pMe->m_converterMode == CONVERTER_MODE_CURRENCY)
                {
                    BBarParam.eBBarType = BTBAR_EXCHANGE_DELETE;
                }
                else
                {
                    BBarParam.eBBarType = BTBAR_DELETE;
                }
            }
            DrawBottomBar(pMe->m_pDisplay, &BBarParam);
            //���¸�����ʾ
            SETAEERECT(&rect, 
                                pMe->m_rc.x, 
                                pMe->m_rc.y + TITLEBAR_HEIGHT + ((int)pMe->m_nCtlID - 2)*pMe->dyMenu, 
                                pMe->m_rc.dx, 
                                pMe->dyMenu);
            if(pMe->m_nCtlID == IDC_UNIT_MENU1 || pMe->m_nCtlID == IDC_UNIT_MENU2)
            {
                rect.x  += ARROW_WIDTH;
                rect.dx -= (2*ARROW_WIDTH);
            }
            IDISPLAY_FillRect(pMe->m_pDisplay, &rect, RGB_ORANGE);
            // ������ʾ
            (void)IMENUCTL_Redraw(pMe->pUnitMenu1);
            (void)IMENUCTL_Redraw(pMe->pUnitMenu2);
            if(pMe->m_nCtlID == IDC_NUMBER1)
            {
                ITEXTCTL_SetActive(pMe->pNumber1, TRUE);
                ITEXTCTL_SetActive(pMe->pNumber2, FALSE);
            }
            else if(pMe->m_nCtlID == IDC_NUMBER2)
            {
                ITEXTCTL_SetActive(pMe->pNumber1, FALSE);
                ITEXTCTL_SetActive(pMe->pNumber2, TRUE);
            }
            else
            {
                ITEXTCTL_SetActive(pMe->pNumber1, FALSE);
                ITEXTCTL_SetActive(pMe->pNumber2, FALSE);
            }
            
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            return TRUE;
        }
            
        case EVT_DIALOG_END:
        {
            IMENUCTL_DeleteAll(pMe->pUnitMenu1);
            IMENUCTL_DeleteAll(pMe->pUnitMenu2);
            if(!pMe->m_bSuspending)
            {
                FREEIF(m_inputNumber);
            }
            return TRUE;
        }

        case EVT_KEY:
        {
            char     chEnter  = 0;
            boolean bRedraw = FALSE;
            boolean bCalc = FALSE;
            uint16   ctlID = 0;
                
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
                {
                    if(pMe->m_nCtlID == IDC_NUMBER1 ||pMe->m_nCtlID == IDC_NUMBER2)
                    {
                        bRedraw = TRUE;
                        bCalc = TRUE;
                        chEnter = '0' + (wParam - AVK_0);
                        
                        Converter_GetTextToInputNum(pMe, wstrDisplay, m_inputNumber);
                        Converter_AddChar(pMe, chEnter, m_inputNumber);
                    }
                }
                    break;
                    
                case AVK_STAR:
                {
                    if(pMe->m_nCtlID == IDC_NUMBER1 ||pMe->m_nCtlID == IDC_NUMBER2)
                    {
                        bRedraw = TRUE;
                        bCalc = TRUE;
                        chEnter = '.';
                        
                        Converter_GetTextToInputNum(pMe, wstrDisplay, m_inputNumber);
                        Converter_AddChar(pMe, chEnter, m_inputNumber);
                    }
                }
                    break;
                    
                case AVK_POUND:
                {
                    if((pMe->m_converterMode == CONVERTER_MODE_TEMPERATURE) &&
                        (pMe->m_nCtlID == IDC_NUMBER1 ||pMe->m_nCtlID == IDC_NUMBER2))
                    {
                        bRedraw = TRUE;
                        bCalc = TRUE;
                        chEnter = '-';

                        Converter_GetTextToInputNum(pMe, wstrDisplay, m_inputNumber);
                        Converter_AddChar(pMe, chEnter, m_inputNumber);
                    }
                    else
                    {
                        return TRUE;
                    }
                }
                    break;

                case AVK_SELECT:
                case AVK_INFO:
                {
                    if(pMe->m_converterMode == CONVERTER_MODE_CURRENCY)
                    {
                        if(pMe->m_nCtlID == IDC_NUMBER1 || pMe->m_nCtlID == IDC_UNIT_MENU1)
                        {
                            pMe->basiccoefficient = IMENUCTL_GetSel(pMe->pUnitMenu1);
                        }
                        else
                        {
                            pMe->basiccoefficient = IMENUCTL_GetSel(pMe->pUnitMenu2);
                        }
                        CLOSE_DIALOG(DLGRET_CHANGECURRENCY)
                        return TRUE;
                    }
                    else
                    {
                        return TRUE;
                    }
                }

                case AVK_CLR:
                {
                    if(pMe->m_nCtlID == IDC_NUMBER1 ||pMe->m_nCtlID == IDC_NUMBER2)
                    {
                        Converter_GetTextToInputNum(pMe, wstrDisplay, m_inputNumber);
                    }
                    else
                    {
                        ITEXTCTL_Reset(pMe->pNumber1);
                        ITEXTCTL_Reset(pMe->pNumber2);
                        CLOSE_DIALOG(DLGRET_CANCELED)
                        return TRUE;
                    }
                    
                    nLen = STRLEN(m_inputNumber);
                    if (nLen == 0 || pMe->m_nCtlID == IDC_UNIT_MENU1 ||pMe->m_nCtlID == IDC_UNIT_MENU1)
                    {
                        if(pMe->m_converterMode == CONVERTER_MODE_CURRENCY)
                        {
                            pMe->basiccoefficient = IDS_CURRENCY_EUR;
                        }
                        ITEXTCTL_Reset(pMe->pNumber1);
                        ITEXTCTL_Reset(pMe->pNumber2);
                        CLOSE_DIALOG(DLGRET_CANCELED)
                        return TRUE;
                    }
                    else
                    {
                        bRedraw = TRUE;
                        bCalc = TRUE;
                        chEnter = 0;
                        if(IS_NOT_NUMBER(m_inputNumber[0]))
                        {
                            MEMSET(m_inputNumber, 0, (MAX_INPUT_NUMBER + 3)*sizeof(char));
                        }
                        else
                        {
                            m_inputNumber[nLen - 1] = chEnter;
                        }
                    }
                }
                    break;
                        
                case AVK_DOWN:
                {
                    bRedraw = TRUE;
                    switch(pMe->m_nCtlID)
                    {
                        case IDC_UNIT_MENU1:
                        {
                            pMe->m_nCtlID = IDC_NUMBER1;
                        }
                            break;
                        
                        case IDC_NUMBER1:
                        {
                            pMe->m_nCtlID = IDC_UNIT_MENU2;
                            ITEXTCTL_GetText(pMe->pNumber1, wstrDisplay, sizeof(wstrDisplay));
                            nLen = WSTRLEN(wstrDisplay);
                            if(wstrDisplay[nLen - 1] == L'.')
                            {
                                wstrDisplay[nLen - 1] = L'\0';
                            }
                            ITEXTCTL_SetText(pMe->pNumber1, wstrDisplay, -1);
                        }
                            break;
                        
                        case IDC_UNIT_MENU2:
                        {
                            pMe->m_nCtlID = IDC_NUMBER2;
                        }
                            break;
                        
                        case IDC_NUMBER2:
                        {
                            pMe->m_nCtlID = IDC_UNIT_MENU1;
                            ITEXTCTL_GetText(pMe->pNumber2, wstrDisplay, sizeof(wstrDisplay));
                            nLen = WSTRLEN(wstrDisplay);
                            if(wstrDisplay[nLen - 1] == L'.')
                            {
                                wstrDisplay[nLen - 1] = L'\0';
                            }
                            ITEXTCTL_SetText(pMe->pNumber2, wstrDisplay, -1);
                        }
                            break;
                    }
                }
                    break;
                    
                case AVK_UP:
                {
                    bRedraw = TRUE;
                    switch(pMe->m_nCtlID)
                    {
                        case IDC_UNIT_MENU1:
                        {
                            pMe->m_nCtlID = IDC_NUMBER2;
                        }
                            break;
                        
                        case IDC_NUMBER1:
                        {
                            pMe->m_nCtlID = IDC_UNIT_MENU1;
                            ITEXTCTL_GetText(pMe->pNumber1, wstrDisplay, sizeof(wstrDisplay));
                            nLen = WSTRLEN(wstrDisplay);
                            if(wstrDisplay[nLen - 1] == L'.')
                            {
                                wstrDisplay[nLen - 1] = L'\0';
                            }
                            ITEXTCTL_SetText(pMe->pNumber1, wstrDisplay, -1);
                        }
                            break;
                        
                        case IDC_UNIT_MENU2:
                        {
                            pMe->m_nCtlID = IDC_NUMBER1;
                        }
                            break;
                        
                        case IDC_NUMBER2:
                        {
                            pMe->m_nCtlID = IDC_UNIT_MENU2;
                            ITEXTCTL_GetText(pMe->pNumber2, wstrDisplay, sizeof(wstrDisplay));
                            nLen = WSTRLEN(wstrDisplay);
                            if(wstrDisplay[nLen - 1] == L'.')
                            {
                                wstrDisplay[nLen - 1] = L'\0';
                            }
                            ITEXTCTL_SetText(pMe->pNumber2, wstrDisplay, -1);
                        }
                            break;
                    }
                } 
                    break;
                        
                case AVK_RIGHT:
                {
                    if(pMe->m_nCtlID == IDC_UNIT_MENU1 || pMe->m_nCtlID == IDC_UNIT_MENU2)
                    {
                        if(pMe->m_nCtlID == IDC_UNIT_MENU1)
                        {
                            ctlID = IMENUCTL_GetSel(pMe->pUnitMenu1);
                        }
                        else
                        {
                            ctlID = IMENUCTL_GetSel(pMe->pUnitMenu2);
                        }
                        switch(pMe->m_converterMode)
                        {
                            case CONVERTER_MODE_CURRENCY:
                                {
                                    if(ctlID < IDS_CURRENCY_ILS)
                                    {
                                        ctlID++;
                                    }
                                    else
                                    {
                                        ctlID = IDS_CURRENCY_EUR;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_LENGTH:
                                {
                                    if(ctlID < IDS_LENGTH_M)
                                    {
                                        ctlID++;
                                    }
                                    else
                                    {
                                        ctlID = IDS_LENGTH_KM;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_WEIGHT:
                                {
                                    if(ctlID < IDS_WEIGHT_GRAIN)
                                    {
                                        ctlID++;
                                    }
                                    else
                                    {
                                        ctlID = IDS_WEIGHT_OZ;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_VOLUME:
                                {
                                    if(ctlID < IDS_VOLUME_GAL)
                                    {
                                        ctlID++;
                                    }
                                    else
                                    {
                                        ctlID = IDS_VOLUME_CM3;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_AREA:
                                {
                                    if(ctlID < IDS_AREA_ACRE)
                                    {
                                        ctlID++;
                                    }
                                    else
                                    {
                                        ctlID = IDS_AREA_CM2;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_TEMPERATURE:
                                {
                                    if(ctlID == IDS_TEMPERATURE_C)
                                    {
                                        ctlID = IDS_TEMPERATURE_F;
                                    }
                                    else
                                    {
                                        ctlID = IDS_TEMPERATURE_C;
                                    }
                                }
                                break;

                            default:
                                return TRUE;
                        }
                        if(pMe->m_nCtlID == IDC_UNIT_MENU1)
                        {
                            IMENUCTL_SetSel(pMe->pUnitMenu1, ctlID);
                        }
                        else
                        {
                            IMENUCTL_SetSel(pMe->pUnitMenu2, ctlID);
                        }
                        bRedraw = TRUE;
                        bCalc = TRUE;
                    }
                }
                    break;
                    
                case AVK_LEFT:
                {
                    if(pMe->m_nCtlID == IDC_UNIT_MENU1 || pMe->m_nCtlID == IDC_UNIT_MENU2)
                    {
                        if(pMe->m_nCtlID == IDC_UNIT_MENU1)
                        {
                            ctlID = IMENUCTL_GetSel(pMe->pUnitMenu1);
                        }
                        else
                        {
                            ctlID = IMENUCTL_GetSel(pMe->pUnitMenu2);
                        }
                        switch(pMe->m_converterMode)
                        {
                            case CONVERTER_MODE_CURRENCY:
                                {
                                    if(ctlID > IDS_CURRENCY_EUR)
                                    {
                                        ctlID--;
                                    }
                                    else
                                    {
                                        ctlID = IDS_CURRENCY_ILS;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_LENGTH:
                                {
                                    if(ctlID > IDS_LENGTH_KM)
                                    {
                                        ctlID--;
                                    }
                                    else
                                    {
                                        ctlID = IDS_LENGTH_M;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_WEIGHT:
                                {
                                    if(ctlID > IDS_WEIGHT_OZ)
                                    {
                                        ctlID--;
                                    }
                                    else
                                    {
                                        ctlID = IDS_WEIGHT_GRAIN;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_VOLUME:
                                {
                                    if(ctlID > IDS_VOLUME_CM3)
                                    {
                                        ctlID--;
                                    }
                                    else
                                    {
                                        ctlID = IDS_VOLUME_GAL;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_AREA:
                                {
                                    if(ctlID > IDS_AREA_CM2)
                                    {
                                        ctlID--;
                                    }
                                    else
                                    {
                                        ctlID = IDS_AREA_ACRE;
                                    }
                                }
                                break;
                                
                            case CONVERTER_MODE_TEMPERATURE:
                                {
                                    if(ctlID == IDS_TEMPERATURE_C)
                                    {
                                        ctlID = IDS_TEMPERATURE_F;
                                    }
                                    else
                                    {
                                        ctlID = IDS_TEMPERATURE_C;
                                    }
                                }
                                break;

                            default:
                                return TRUE;
                        }
                        if(pMe->m_nCtlID == IDC_UNIT_MENU1)
                        {
                            IMENUCTL_SetSel(pMe->pUnitMenu1, ctlID);
                        }
                        else
                        {
                            IMENUCTL_SetSel(pMe->pUnitMenu2, ctlID);
                        }
                        bRedraw = TRUE;
                        bCalc = TRUE;
                    }
                }
                    break;
                    
                default:
                    break;
            }
                
            if (bRedraw)
            {
                if(bCalc)
                {
                    double result;
                    (void) STRTOWSTR(m_inputNumber, wstrDisplay, sizeof(wstrDisplay));
                    if(pMe->m_nCtlID == IDC_NUMBER1 || pMe->m_nCtlID == IDC_UNIT_MENU1)
                    {
                        ITEXTCTL_SetText(pMe->pNumber1, wstrDisplay, -1);
                    }
                    else
                    {
                        ITEXTCTL_SetText(pMe->pNumber2, wstrDisplay, -1);
                    }
                    
                    if(STRLEN(m_inputNumber) == 0)
                    {
                        wstrDisplay[0] = '\0';
                    }
                    else
                    {
                        result = Converter_CalcResult(pMe);
                        if(pMe->b_overflow)
                        {
                            (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                                AEE_CONVERTER_LANGFILE,
                                                                IDS_OVERFLOW, 
                                                                wstrDisplay,
                                                                sizeof(wstrDisplay));
                        }
                        else
                        {
                            (void)Converter_FloatToWstr(result, wstrDisplay, sizeof(wstrDisplay));
                        }
                    }
                    
                    if(pMe->m_nCtlID == IDC_NUMBER1 || pMe->m_nCtlID == IDC_UNIT_MENU1)
                    {
                        ITEXTCTL_SetText(pMe->pNumber2, wstrDisplay, -1);
                    }
                    else
                    {
                        ITEXTCTL_SetText(pMe->pNumber1, wstrDisplay, -1);
                    }
                }
                (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_CONVERTER,
                                            EVT_USER_REDRAW,
                                            0,
                                            0);
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
       Converter_ChangeCurrencyEvent
˵����
       IDD_CHANGECONURRENCY�Ի����¼�������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean  Converter_ChangeCurrencyEvent(CConverter *pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    PARAM_NOT_REF(dwParam)
    static char     *m_inputNumber = NULL;
    static boolean bChangable;
    int                 nLen = 0;
    uint16            coeffNum1 = 7, coeffNum2 = 8, coeffNum3 = 9, basicCurrency = 10;
    uint16            currencymenu[] = {11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40};
    AECHAR          wstrDisplay[MAX_INPUT_NUMBER+3] = {0};
    ERR("%x, %x ,%x,Converter_ChangeCurrencyEvent",eCode,wParam,dwParam);
    
    if (NULL == pMe)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
        {
            int i = 0;
            m_inputNumber = (char *)MALLOC((MAX_INPUT_NUMBER + 3)* sizeof(char));
            bChangable = FALSE;
            
            for(i=0;i<CONVERTER_CURRENCY_NUMBER;i++)
            {
                (void)IMENUCTL_AddItem(pMe->title,
                                                AEE_CONVERTER_LANGFILE,
                                                IDS_CURRENCY_EUR + i,
                                                IDS_CURRENCY_EUR + i,
                                                NULL,
                                                0);
                (void)IMENUCTL_AddItem(pMe->currency,
                                                AEE_CONVERTER_LANGFILE,
                                                IDS_CURRENCY_EUR + i,
                                                currencymenu[2*i],
                                                NULL,
                                                0);
                (void)IMENUCTL_AddItem(pMe->currency,
                                                AEE_CONVERTER_LANGFILE,
                                                IDS_CURRENCY_EUR + i,
                                                currencymenu[2*i + 1],
                                                NULL,
                                                0);
            }
            pMe->m_nCtlID = basicCurrency;
            return TRUE;
        }
                     
        case EVT_DIALOG_START:
        {
            IMENUCTL_SetActive(pMe->currency, FALSE);
            IMENUCTL_SetSel(pMe->title, pMe->basiccoefficient);
            IMENUCTL_SetSel(pMe->currency, currencymenu[0]);
            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_CONVERTER,
                                    EVT_USER_REDRAW,
                                    NULL,
                                    NULL);
            return TRUE;
        }
            
        case EVT_USER_REDRAW:
        {
            double                 source = 0.0;
            AECHAR                      text[32] = {0};
            TitleBar_Param_type     TBarParam = {0};
            BottomBar_Param_type  BBarParam ={0};
            IImage                       *Image;
            uint16                        coefficient1, coefficient2, coefficient3;
            uint16                        uint = 0;
            AEERect                     rect;
            
            //������
            Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, FALSE);
            // ������
            (void)ISHELL_LoadResString(pMe->m_pShell, 
                                                AEE_CONVERTER_LANGFILE,
                                                IDS_CHANGE_TITLE, 
                                                text,
                                                sizeof(text));
            TBarParam.pwszTitle = text;
            TBarParam.dwAlignFlags = IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
            DrawTitleBar(pMe->m_pDisplay, &TBarParam);
            //����ͷ
            Image = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_LEFTARROW);
            IIMAGE_Draw(Image, pMe->m_rc.x, pMe->m_rc.y + TITLEBAR_HEIGHT + (pMe->dyMenu - ARROW_HEIGHT)/2);

            Image = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_RIGHTARROW);
            IIMAGE_Draw(Image, pMe->m_rc.dx - ARROW_WIDTH, pMe->m_rc.y + TITLEBAR_HEIGHT + (pMe->dyMenu - ARROW_HEIGHT)/2);
            if(Image)
            {
                IIMAGE_Release(Image);
                Image = NULL;
            }

            //������
            nLen = STRLEN(m_inputNumber);
            if((pMe->m_nCtlID == basicCurrency) ||
                ((IMENUCTL_GetSel(pMe->currency) - currencymenu[0])/2 + IDS_CURRENCY_EUR == pMe->basiccoefficient))
            {
                BBarParam.eBBarType = BTBAR_BACK;
            }
            else if(bChangable == FALSE)
            {
                BBarParam.eBBarType = BTBAR_EDIT_BACK;
            }
            else if(nLen <= 0)
            {
                BBarParam.eBBarType = BTBAR_BACK;
            }
            else
            {
                BBarParam.eBBarType = BTBAR_OK_DELETE;
            }
            DrawBottomBar(pMe->m_pDisplay, &BBarParam);
            //���¸�����ʾ
            SETAEERECT(&rect, 
                                pMe->m_rc.x, 
                                pMe->m_rc.y + TITLEBAR_HEIGHT, 
                                pMe->m_rc.dx, 
                                pMe->dyMenu);
            if(pMe->m_nCtlID == basicCurrency)
            {
                rect.x  += ARROW_WIDTH;
                rect.dx -= (2*ARROW_WIDTH);
            }
            else
            {
                rect.dx -= SCROLLBAR_WIDTH;
                rect.y += (2*(pMe->m_nCtlID - coeffNum1 + 1)*pMe->dyMenu);
            }
            //���»�����ʾ
            if(bChangable == FALSE)
            {
                uint = IMENUCTL_GetSel(pMe->currency);
                if(pMe->m_nCtlID == basicCurrency)
                {
                    coefficient1 = currencymenu[0];
                    coefficient2 = currencymenu[0] + 1;
                    coefficient3 = currencymenu[0] + 2;
                }
                else
                {
                    coefficient1 = (uint + currencymenu[0])/2 + coeffNum1 - pMe->m_nCtlID;
                    coefficient2 = (uint + currencymenu[0])/2 + coeffNum2 - pMe->m_nCtlID;
                    coefficient3 = (uint + currencymenu[0])/2 + coeffNum3 - pMe->m_nCtlID;
                }
                
                source = pMe->m_CurrencyCFG.customCurrency[coefficient1 - currencymenu[0]] / pMe->m_CurrencyCFG.customCurrency[pMe->basiccoefficient - IDS_CURRENCY_EUR];
                (void)Converter_FloatToWstr(source, wstrDisplay, sizeof(wstrDisplay));
                ITEXTCTL_SetText(pMe->coeff1, wstrDisplay, -1);
                source = pMe->m_CurrencyCFG.customCurrency[coefficient2 - currencymenu[0]] / pMe->m_CurrencyCFG.customCurrency[pMe->basiccoefficient - IDS_CURRENCY_EUR];
                (void)Converter_FloatToWstr(source, wstrDisplay, sizeof(wstrDisplay));
                ITEXTCTL_SetText(pMe->coeff2, wstrDisplay, -1);
                source = pMe->m_CurrencyCFG.customCurrency[coefficient3 - currencymenu[0]] / pMe->m_CurrencyCFG.customCurrency[pMe->basiccoefficient - IDS_CURRENCY_EUR];
                (void)Converter_FloatToWstr(source, wstrDisplay, sizeof(wstrDisplay));
                ITEXTCTL_SetText(pMe->coeff3, wstrDisplay, -1);
            }
            
            (void)IMENUCTL_Redraw(pMe->currency);
            IDISPLAY_FillRect(pMe->m_pDisplay, &rect, RGB_ORANGE);
            if(bChangable == FALSE)
            {
                ITEXTCTL_SetActive(pMe->coeff1, FALSE);
                ITEXTCTL_SetActive(pMe->coeff2, FALSE);
                ITEXTCTL_SetActive(pMe->coeff3, FALSE);
            }
            else
            {
                (void)ITEXTCTL_Redraw(pMe->coeff1);
                (void)ITEXTCTL_Redraw(pMe->coeff2);
                (void)ITEXTCTL_Redraw(pMe->coeff3);
            }
            (void)IMENUCTL_Redraw(pMe->title);
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
                        
            return TRUE;
        }
            
        case EVT_DIALOG_END:
        {
            if (NULL != m_inputNumber)
            {
                FREE(m_inputNumber);
                m_inputNumber = NULL;
            }
            return TRUE;
        }
        
        case EVT_KEY:
        {
            char     chEnter  = 0;
            boolean bRedraw = FALSE;
            uint16   ctlID = 0;
                
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
                {
                    if((pMe->m_nCtlID != basicCurrency) && (bChangable == TRUE))
                    {
                        bRedraw = TRUE;
                        chEnter = '0' + (wParam - AVK_0);
                        
                        Converter_AddChar(pMe, chEnter, m_inputNumber);
                        
                        (void) STRTOWSTR(m_inputNumber, wstrDisplay, sizeof(wstrDisplay));
                        if(pMe->m_nCtlID == coeffNum1)
                        {
                            ITEXTCTL_SetText(pMe->coeff1, wstrDisplay, -1);
                        }
                        if(pMe->m_nCtlID == coeffNum2)
                        {
                            ITEXTCTL_SetText(pMe->coeff2, wstrDisplay, -1);
                        }
                        if(pMe->m_nCtlID == coeffNum3)
                        {
                            ITEXTCTL_SetText(pMe->coeff3, wstrDisplay, -1);
                        }
                    }
                }
                    break;
                    
                case AVK_STAR:
                {
                    if((pMe->m_nCtlID != basicCurrency) && (bChangable == TRUE))
                    {
                        bRedraw = TRUE;
                        chEnter = '.';
                        
                        Converter_AddChar(pMe, chEnter, m_inputNumber);
                        
                        (void) STRTOWSTR(m_inputNumber, wstrDisplay, sizeof(wstrDisplay));
                        if(pMe->m_nCtlID == coeffNum1)
                        {
                            ITEXTCTL_SetText(pMe->coeff1, wstrDisplay, -1);
                        }
                        if(pMe->m_nCtlID == coeffNum2)
                        {
                            ITEXTCTL_SetText(pMe->coeff2, wstrDisplay, -1);
                        }
                        if(pMe->m_nCtlID == coeffNum3)
                        {
                            ITEXTCTL_SetText(pMe->coeff3, wstrDisplay, -1);
                        }
                    }
                }
                    break;

#ifdef FEATURE_RESET_CURRENCY
                /*set backdoor to reset currency to default in case of chaos*/
                case AVK_POUND:
                {
                    if(STRCMP(m_inputNumber, "12101210") == 0)
                    {
                        int i = 0;

                        for(i = 0; i < CONVERTER_CURRENCY_NUMBER; i++)
                        {
                            pMe->m_CurrencyCFG.customCurrency[i] = coefficient[i];
                        }
                        (void) ISHELL_SetPrefs(pMe->m_pShell,
                                                            AEECLSID_CONVERTER,
                                                            CONVERTER_CFG_CURRENCY,
                                                            &pMe->m_CurrencyCFG,
                                                            sizeof(ConverterCurrencyCfg));
                        CLOSE_DIALOG(DLGRET_OK)
                    }
                }
                return TRUE;
#endif

                case AVK_SELECT:
                case AVK_INFO:
                {
                    if(pMe->m_nCtlID != basicCurrency)
                    {
                        if(bChangable == TRUE)
                        {
                            if(pMe->m_nCtlID == coeffNum1)
                            {
                                ITEXTCTL_GetText(pMe->coeff1, wstrDisplay, sizeof(wstrDisplay));
                            }
                            else if(pMe->m_nCtlID == coeffNum2)
                            {
                                ITEXTCTL_GetText(pMe->coeff2, wstrDisplay, sizeof(wstrDisplay));
                            }
                            else
                            {
                                ITEXTCTL_GetText(pMe->coeff3, wstrDisplay, sizeof(wstrDisplay));
                            }

                            if(WSTRLEN(wstrDisplay) > 0)
                            {
                                if(WSTRTOFLOAT(wstrDisplay) <= 0.0)
                                {
                                    CLOSE_DIALOG(DLGRET_ERR)
                                }
                                else
                                {
                                    ctlID = (IMENUCTL_GetSel(pMe->currency) - currencymenu[0])/2;
                                    /*EUR as the standard currency cannot be changed!*/
                                    if(ctlID == 0)
                                    {
                                        pMe->m_CurrencyCFG.customCurrency[pMe->basiccoefficient - IDS_CURRENCY_EUR] = FDIV(pMe->m_CurrencyCFG.customCurrency[ctlID], (WSTRTOFLOAT(wstrDisplay)));
                                    }
                                    else
                                    {
                                        pMe->m_CurrencyCFG.customCurrency[ctlID] = FDIV((WSTRTOFLOAT(wstrDisplay)), pMe->m_CurrencyCFG.customCurrency[pMe->basiccoefficient - IDS_CURRENCY_EUR]);
                                    }
                                    (void) ISHELL_SetPrefs(pMe->m_pShell,
                                                                        AEECLSID_CONVERTER,
                                                                        CONVERTER_CFG_CURRENCY,
                                                                        &pMe->m_CurrencyCFG,
                                                                        sizeof(ConverterCurrencyCfg));
                                    CLOSE_DIALOG(DLGRET_OK)
                                }
                            }
                            return TRUE;
                        }
                        else
                        {
                            ctlID = IMENUCTL_GetSel(pMe->currency);
                            if((ctlID - currencymenu[0])/2 + IDS_CURRENCY_EUR != pMe->basiccoefficient)
                            {
                                BottomBar_Param_type  BBarParam ={0};
                                BBarParam.eBBarType = BTBAR_OK_DELETE;
                                bChangable = TRUE;
                                if(pMe->m_nCtlID == coeffNum1)
                                {
                                    ITEXTCTL_GetText(pMe->coeff1, wstrDisplay, sizeof(wstrDisplay));
                                    ITEXTCTL_SetActive(pMe->coeff1, TRUE);
                                }
                                else if(pMe->m_nCtlID == coeffNum2)
                                {
                                    ITEXTCTL_GetText(pMe->coeff2, wstrDisplay, sizeof(wstrDisplay));
                                    ITEXTCTL_SetActive(pMe->coeff2, TRUE);
                                }
                                else
                                {
                                    ITEXTCTL_GetText(pMe->coeff3, wstrDisplay, sizeof(wstrDisplay));
                                    ITEXTCTL_SetActive(pMe->coeff3, TRUE);
                                }
                                nLen = WSTRLEN(wstrDisplay);
                                (void) WSTRTOSTR(wstrDisplay, m_inputNumber, (MAX_INPUT_NUMBER + 3)*sizeof(char));
                                m_inputNumber[nLen] = 0;
                                DrawBottomBar(pMe->m_pDisplay, &BBarParam);
                                IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
                            }
                        }
                    }
                }
                    break;

                case AVK_CLR:
                {
                    chEnter = 0;
                    nLen = STRLEN(m_inputNumber);
                    if (nLen == 0 ||pMe->m_nCtlID == basicCurrency ||bChangable == FALSE)
                    {
                        CLOSE_DIALOG(DLGRET_CANCELED)
                        return TRUE;
                    }
                    else
                    {
                        bRedraw = TRUE;
                        m_inputNumber[nLen -1] = chEnter;
                        (void) STRTOWSTR(m_inputNumber, wstrDisplay, sizeof(wstrDisplay));
                        if(pMe->m_nCtlID == coeffNum1)
                        {
                            ITEXTCTL_SetText(pMe->coeff1, wstrDisplay, -1);
                        }
                        if(pMe->m_nCtlID == coeffNum2)
                        {
                            ITEXTCTL_SetText(pMe->coeff2, wstrDisplay, -1);
                        }
                        if(pMe->m_nCtlID == coeffNum3)
                        {
                            ITEXTCTL_SetText(pMe->coeff3, wstrDisplay, -1);
                        }
                    }
                }
                    break;
                        
                case AVK_DOWN:
                {
                    bRedraw = TRUE;
                    bChangable = FALSE;
                    
                    if(pMe->m_nCtlID == basicCurrency)
                    {
                        pMe->m_nCtlID = coeffNum1;
                        ctlID = currencymenu[0];
                        IMENUCTL_SetSel(pMe->currency, ctlID);
                        IMENUCTL_SetActive(pMe->currency, TRUE);
                    }
                    else
                    {
                        ctlID = IMENUCTL_GetSel(pMe->currency);
                        if(ctlID<= currencymenu[26])
                        {
                            ctlID += 2;
                            IMENUCTL_SetSel(pMe->currency, ctlID + 1);
                            IMENUCTL_SetSel(pMe->currency, ctlID);
                            if(pMe->m_nCtlID <= coeffNum2)
                            {
                                pMe->m_nCtlID++;
                            }
                        }
                        else
                        {
                            IMENUCTL_SetSel(pMe->currency, currencymenu[0]);
                            IMENUCTL_SetActive(pMe->currency, FALSE);
                            pMe->m_nCtlID = basicCurrency;
                        }
                    }
                }
                    break;
                    
                case AVK_UP:
                {
                    bRedraw = TRUE;
                    bChangable = FALSE;
                    
                    if(pMe->m_nCtlID == basicCurrency)
                    {
                        pMe->m_nCtlID = coeffNum3;
                        ctlID = currencymenu[28];
                        IMENUCTL_SetSel(pMe->currency, ctlID + 1);
                        IMENUCTL_SetSel(pMe->currency, ctlID);
                        IMENUCTL_SetActive(pMe->currency, TRUE);
                    }
                    else
                    {
                        ctlID = IMENUCTL_GetSel(pMe->currency);
                        if(ctlID >= currencymenu[2])
                        {
                            ctlID -= 2;
                            IMENUCTL_SetSel(pMe->currency, ctlID);
                            if(pMe->m_nCtlID > coeffNum1)
                            {
                                pMe->m_nCtlID--;
                            }
                        }
                        else
                        {
                            IMENUCTL_SetActive(pMe->currency, FALSE);
                            pMe->m_nCtlID = basicCurrency;
                        }
                    }
                } 
                    break;
                        
                case AVK_RIGHT:
                {
                    if(pMe->m_nCtlID == basicCurrency)
                    {
                        ctlID = IMENUCTL_GetSel(pMe->title);
                        if(ctlID < IDS_CURRENCY_ILS)
                        {
                            ctlID++;
                        }
                        else
                        {
                            ctlID = IDS_CURRENCY_EUR;
                        }
                        IMENUCTL_SetSel(pMe->title, ctlID);
                        pMe->basiccoefficient = ctlID;
                        bRedraw = TRUE;
                    }
                }
                    break;
                    
                case AVK_LEFT:
                {
                    if(pMe->m_nCtlID == basicCurrency)
                    {
                        ctlID = IMENUCTL_GetSel(pMe->title);
                        if(ctlID > IDS_CURRENCY_EUR)
                        {
                            ctlID--;
                        }
                        else
                        {
                            ctlID = IDS_CURRENCY_ILS;
                        }
                        IMENUCTL_SetSel(pMe->title, ctlID);
                        pMe->basiccoefficient = ctlID;
                        bRedraw = TRUE;
                    }
                }
                    break;
                    
                default:
                    break;
            }
                
            if (bRedraw)
            {
                (void) ISHELL_PostEvent(pMe->m_pShell,
                                            AEECLSID_CONVERTER,
                                            EVT_USER_REDRAW,
                                            0,
                                            0);
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
       Converter_CalcResult
˵����
       ���㵥λ��������
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣
       
����ֵ��
       ������result ��
       
��ע��
       
==============================================================================*/
static double  Converter_CalcResult(CConverter *pMe)
{
    uint16 uint1, uint2;
    double source = 0.0,result = 0.0;
    AECHAR buf[MAX_INPUT_NUMBER + 3] = {0};

    pMe->b_overflow = FALSE;
    if(pMe->m_nCtlID == IDC_NUMBER1 || pMe->m_nCtlID == IDC_UNIT_MENU1)
    {
        uint1 = IMENUCTL_GetSel(pMe->pUnitMenu1);
        uint2 = IMENUCTL_GetSel(pMe->pUnitMenu2);
        ITEXTCTL_GetText(pMe->pNumber1, buf, sizeof(buf));
    }
    else
    {
        uint1 = IMENUCTL_GetSel(pMe->pUnitMenu2);
        uint2 = IMENUCTL_GetSel(pMe->pUnitMenu1);
        ITEXTCTL_GetText(pMe->pNumber2, buf, sizeof(buf));
    }
    
    source = WSTRTOFLOAT(buf);
    //CURRENCY
    if(pMe->m_converterMode == CONVERTER_MODE_CURRENCY)
    {
        result = FMUL(source, FDIV(pMe->m_CurrencyCFG.customCurrency[uint2 - IDS_CURRENCY_EUR], pMe->m_CurrencyCFG.customCurrency[uint1 - IDS_CURRENCY_EUR]));
    }
    //TEMPERATURE
    else if(pMe->m_converterMode == CONVERTER_MODE_TEMPERATURE)
    {
        if(uint1 == uint2)
        {
            result = source;
            if(uint1 == IDS_TEMPERATURE_C)
            {
                if(source < (0 - 273.15))
                {
                    pMe->b_overflow = TRUE;
                }
            }
            else
            {
                if(source < ((0 - 273.15)*1.4+32))
                {
                    pMe->b_overflow = TRUE;
                }
            }
        }
        else
        {
            if(uint1 == IDS_TEMPERATURE_C)
            {
                if(source < (0 - 273.15))
                {
                    pMe->b_overflow = TRUE;
                }
                else
                {
                    result = FMUL(source, FDIV(coefficient[uint2 - IDS_CURRENCY_EUR], coefficient[uint1 - IDS_CURRENCY_EUR]));
                    result = FADD(result, 32);
                }
            }
            else
            {
                source = FSUB(source, 32);
                result =  FMUL(source, FDIV(coefficient[uint2 - IDS_CURRENCY_EUR], coefficient[uint1 - IDS_CURRENCY_EUR]));
                if(result < (0 - 273.15))
                {
                    pMe->b_overflow = TRUE;
                }
            }
        }
    }
    //LENGTH
    //WEIGHT
    //VOLUME
    //AREA
    else
    {
        result = FMUL(source, FDIV(coefficient[uint2 - IDS_CURRENCY_EUR], coefficient[uint1 - IDS_CURRENCY_EUR]));
    }
   
    if(result >= 1000000000.0)
    {
        pMe->b_overflow = TRUE;
    }

    return result;
}

/*==============================================================================
������
       Converter_FloatToWstr
˵����
       ������ֵ��ĸ�������ʾ��
       
������
       source [in]��Դ���֡�
       pdest [in]�����λ�á�
       nSize [in]�������������С��
       
����ֵ��
       NONE
       
��ע��
       
==============================================================================*/
static boolean Converter_FloatToWstr(double source, AECHAR * pdest, int nSize)
{
    char szBuff[32];
    AECHAR ch, *psz, *pszLastZero, *pszSrc, *pszDst;
    boolean bDot;

    if (!pdest || nSize <= 0)
    {
        return FALSE;
    }
    
    MEMSET(pdest, 0, nSize);

    (void)sprintf(szBuff, "%15.6f", source);
    (void)STRTOWSTR(szBuff, pdest, nSize);

    // Clip leading spaces...
    for (pszDst = pszSrc = pdest; (ch = *pszSrc) == ' '; pszSrc++)
            ;

    if (pszSrc != pszDst)
    {
        while (1)
        { //lint !e716
            ch = *pszSrc;
            *pszDst = ch;
            if (!ch)
            {
                break;
            }
            pszDst++;
            pszSrc++;
        }
    }

    // Clip trailing zeros...
    for (bDot = FALSE, pszLastZero = NULL, psz = pdest; (ch = *psz) != 0; psz++)
    {
        if (ch == (AECHAR)'.')
        {
            bDot = TRUE;
        }
        else
        {
            if (bDot)
            {
                if (ch == (AECHAR)'0')
                {
                    if (!pszLastZero)
                    {
                        pszLastZero = psz;
                    }
                }
                else
                {
                    pszLastZero = NULL;
                }
            }
        }
    }

    // If the trailing zero followed the decimal point, clip them both...
    if (pszLastZero)
    {
        if (pszLastZero[-1] == (AECHAR)'.')
        {
            pszLastZero--;
        }
        *pszLastZero = 0;
    }

    //If the trailing dot exists, clip it
    if(psz[-1] == (AECHAR)'.')
    {
        psz--;
        *psz = 0;
    }
    
    return TRUE;
}

/*==============================================================================
����:
       Converter_DialogTimeout

˵��:
       �����������ֱ����

����:
       pme [in]��ָ��Converter Applet����ṹ��ָ�롣
       chAdd [in]����ӵ��ַ���
       pDest [in]���������ֻ�����ָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void Converter_AddChar(CConverter *pMe, char chAdd, char *pDest)
{
    int i = 0, nIntLen = 0, nTailLen = 0;
    char *pstr = NULL;
    boolean bDot = FALSE;
    
    if(IS_NOT_NUMBER(*pDest))
    {
        MEMSET(pDest, 0, (MAX_INPUT_NUMBER + 3)*sizeof(char));
    }
    
    for(pstr = pDest, i = 0; *pstr != 0; i++, pstr++)
    {
        if(*pstr == '.')
        {
            bDot = TRUE;
            break;
        }
    }
    nIntLen = (*pDest == '-')? (i - 1):(i);
    if(TRUE == bDot)
    {
        for (nTailLen = i; *pstr != 0; i++, pstr++)
        {
            ;
        }
        nTailLen = i - nTailLen - 1;
    }
    
    if(chAdd == '.')
    {
        if(nIntLen == 0)
        {
            if(*pDest == 0)
            {
                *pDest = '0';
                pstr++;
            }
            else if(*pDest == '-')
            {
                pDest[1] = '0';
                pstr++;
            }
        }
        else if(bDot)
        {
            return;
        }
    }
    else if(chAdd == '-')
    {
        if(nIntLen == 0)
        {
            if(*pDest == 0)
            {
                *pDest = '-';
                pDest[1] = 0;
            }
            else if(*pDest == '-')
            {
                *pDest = 0;
            }
        }
        return;
    }
    else if((nIntLen == 1) && (!bDot) && (*pDest == '0'))
    {
        *pDest = 0;
        pstr--;
    }
    else if (((TRUE != bDot) && (nIntLen >= MAX_INT_NUMBER) && (chAdd != '.')) || 
            ((TRUE == bDot) && (nTailLen >= MAX_DECIMAL_NUMBER)))
    {
        return;
    }
    
    *pstr = chAdd;
    pstr[1] = 0;
}

/*==============================================================================
����:
       Converter_GetTextToInputNum

˵��:
       ��TextCtl��ȡ�ַ��������ݵ��������ֻ�������

����:
       pme [in]��ָ��Converter Applet����ṹ��ָ�롣
       pwstrTemp [in]����ʱ��Ż�ȡ�����ġ�
       pDest [in]���������ֻ�����ָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void Converter_GetTextToInputNum(CConverter *pMe, AECHAR *pwstrTemp, char *pDest)
{
    if(pMe->m_nCtlID == IDC_NUMBER1)
    {
        ITEXTCTL_GetText(pMe->pNumber1, pwstrTemp, (MAX_INPUT_NUMBER + 3)*sizeof(char));
    }
    else
    {
        ITEXTCTL_GetText(pMe->pNumber2, pwstrTemp, (MAX_INPUT_NUMBER + 3)*sizeof(char));
    }
    (void) WSTRTOSTR(pwstrTemp, pDest, (MAX_INPUT_NUMBER + 3)*sizeof(char));
}

/*==============================================================================
����:
       Converter_DialogTimeout

˵��:
       ��ʱ���ص���������Ҫ�����Զ��ر���Ϣ�Ի���

����:
       pme [in]��void *����ָ�룬��ָCConverter�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void Converter_DialogTimeout(void *pme)
{
    CConverter *pMe = (CConverter *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent(pMe->m_pShell,
                            AEECLSID_CONVERTER,
                            EVT_DISPLAYDIALOGTIMEOUT,
                            0,
                            0);
}

