/*==============================================================================
// �ļ���
//        ClockAppsDialogHandler.c
//        2007-11-12 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook
//
// �ļ�˵����
//
// ���ߣ�2007-11-12
// �������ڣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-12                 ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "ClockApps_priv.h"
#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "g_sensor.h"
#endif

#ifdef FEATURE_APP_MUSICPLAYER
#include "MusicPlayer.h"
#endif 
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// t ��ʾ��ʾ��������
// ������ײ���ʾ���궨��
#define DRAW_PROMPT_BAR(x)                          \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}

//#define  MAX_STATE_ITEM     (2)
#define  ITEM_STATE_ON      (1)
#define  ITEM_STATE_OFF     (2)

//#define  MAX_REPMODE_ITEM   (10)
#define  ITEM_REP_MODE_1    (3)
#define  ITEM_REP_MODE_2    (4)
#define  ITEM_REP_MODE_3    (5)
#define  ITEM_REP_MODE_4    (6)
#define  ITEM_REP_MODE_5    (7)
#define  ITEM_REP_MODE_6    (8)
#define  ITEM_REP_MODE_7    (9)
#define  ITEM_REP_MODE_8    (10)
#define  ITEM_REP_MODE_9    (11)
#define  ITEM_REP_MODE_10   (12)
#define  ITEM_REP_MODE_11   (13)

//#define MAX_SNOOZE_ITEM
#define ITEM_SNOOZE_1         (14)
#define ITEM_SNOOZE_2         (15)
#define ITEM_SNOOZE_3         (16)
#define ITEM_SNOOZE_4         (17)

#define PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/


/*==============================================================================
                                 ��������
==============================================================================*/
// �Ի��� IDD_ALARMCLOCK_DLG �¼�������
static boolean  HandleAlarmMainDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_ALARMCLOCK_OPTION �¼�������
static boolean  HandleAlarmOptionDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_ALARMCLOCK_SUB �¼�������
static boolean  HandleAlarmSubDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_ALARMTIME_MESSEGE �¼�������
static boolean  HandleAlarmTimeMessegeDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_ALARMTIME_REACH �¼�������
static boolean  HandleAlarmTimeReachDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

static boolean HandleAlarmDelConfirmEvent(CClockApps *pMe, 
    AEEEvent eCode, 
    uint16 wParam, 
    uint32 dwParam
);

static boolean HandleAlarmMsgBox(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);


// ����Clock�༭����Ӧ�÷�����л��ؼ�
static boolean CClockApps_HandleKeyEvent(CClockApps *pMe, uint16 wParam);

// ����Clock�༭����Ӧ��COMMAND��������������
static boolean CClockApps_HandleCmdEvent(CClockApps *pMe);

// ���ڸ������Ӷ�ʱ��������
static void CClockApps_UpdateAlarmTimer(CClockApps *pMe, AlarmType eAlarmType);

//��ʱ��������������AlarmClock Applet ���� EVT_ALARMOFF �¼�
static void CClockApps_DeActivate(CClockApps *pme);

//��ʱ�����������������clock ICON
static void CClockApps_AniClockImg(CClockApps *pme);

static void ClockNotifyMP3PlayerAlertEvent(CClockApps *pMe, boolean toStartAlert);
#ifdef FEATURE_SUB_LCD
//���ǹر�ʱ��С���ϻ�����ʾ��Ϣ
static void CClockApps_DispalySub(CClockApps *pMe,
    AECHAR *subTitle,
    AECHAR *subTime,
    AECHAR *subMesssge
);
#endif //FEATURE_SUB_LCD

//������ʾ�Ի�����Ϻ�Ļص�
static void CClockApps_SaveAlertTimeout(void *pme);

//��������ʾ��Ϣ
static void CClockApps_DrawSaveAlert(CClockApps *pMe);

static void CClockApps_Set_CTL(CClockApps *pMe);

static void CClockApps_Draw_Arrow(CClockApps *pMe, AEERect* pRect,int i);

static void CClockApps_DialogTimeout(void *pme);



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
       ClockApps_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��ClockApps Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:

==============================================================================*/
void ClockApps_ShowDialog(CClockApps *pMe,uint16  dlgResId)
{
    int nRet;

    // At most one dialog open at once
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        ERR("Trying to create a dialog without closing the previous one",0,0,0);
        return;
    }else
    {
        ERR("dlgResId %d",dlgResId,0,0);
    }

    nRet = ISHELL_CreateDialog(pMe->m_pShell,AEE_CLOCKAPPS_RES_FILE,dlgResId,NULL);

    if (nRet != SUCCESS)
    {
        ERR("Failed to create the dialog in the ClockApps applet",0,0,0);
    }
}

/*==============================================================================
����:
       ClockApps_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��ClockApps Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean ClockApps_RouteDialogEvent(CClockApps *pMe,
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

    switch (pMe->m_pActiveDlgID)
    {
        case IDD_ALARMCLOCK_DLG:
            return HandleAlarmMainDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_ALARMCLOCK_OPTION:
            return HandleAlarmOptionDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_ALARMCLOCK_SUB:
            return HandleAlarmSubDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_ALARMCLOCK_MESSEGE:
            return HandleAlarmTimeMessegeDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_ALARMTIME_REACH:
            return HandleAlarmTimeReachDialogEvent(pMe,eCode,wParam,dwParam);
            
        case IDD_DELCONFIRM:
            return HandleAlarmDelConfirmEvent(pMe, eCode, wParam, dwParam);

        case IDD_WARNING:
            return HandleAlarmMsgBox(pMe, eCode, wParam, dwParam);

        default:
            return FALSE;
    }
}

/*==============================================================================
������
       HandleAlarmMainDialogEvent
˵����
       IDD_ALARMCLOCK_DLG�Ի����¼�������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
#define gotoNextState( dlgRetrun)   pMe->m_currDlgId = IMENUCTL_GetSel( pMenuAlarmList); \
                                    pMe->m_eCurAlarmType = pMe->m_currDlgId - IDC_MENU_ALARMCLOCK_LIST_ITEM_1; \
                                    CLOSE_DIALOG( dlgRetrun)

static boolean  HandleAlarmMainDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{


    IMenuCtl* pMenuAlarmList = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_MENU_ALARMCLOCK_LIST);
    if( pMenuAlarmList == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
//            IMENUCTL_SetProperties( pMenuAlarmList, IMENUCTL_GetProperties( pMenuAlarmList) | MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties( pMenuAlarmList, OEMMP_DISTINGUISH_INFOKEY_SELECTKEY |OEMMP_USE_MENU_STYLE);
#ifdef FEATURE_CARRIER_CHINA_VERTU
            IMENUCTL_SetBackGround(pMenuAlarmList, AEE_APPSCOMMONRES_IMAGESFILE, IDI_ALARM_BACKGROUND);
#endif
            //IMENUCTL_SetBottomBarType( pMenuAlarmList, BTBAR_OPTION_BACK);
            return TRUE;

        case EVT_DIALOG_START:
            pMe->m_return = 1;
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_ALARMCLOCK,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
        {
            AECHAR  alarmClockInfo[64]  = {0};
            int     length              = 0;
            int     i                   = 0;
            uint32  MenuSel;
            if(pMe->m_pIAnn != NULL)
            {
			    IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);	
            }
            //��ȡ������ѡ������ݽṹ
            (void) ISHELL_GetPrefs( pMe->m_pShell,
                                    AEECLSID_ALARMCLOCK,
                                    CLOCK_CFG_VERSION,
                                    &pMe->m_ClockCfg,
                                    sizeof(ClockAppCfg)
                               );

            for( i = 0; i < NUM_OF_ALARMCLOCK ; i++)
            {
                uint16 Ret;
                switch (pMe->m_ClockCfg.RepMode[i])
                {
                    case WEEK_ALARM_REP1:   // ÿ��
                        Ret = IDS_REP_MODE_0;
                        break;

                    case WEEK_ALARM_REP2: // ��һ~~����
                        Ret = IDS_REP_MODE_1;
                        break;

                    case WEEK_ALARM_REP3: // ��һ~~����
                        Ret = IDS_REP_MODE_2;
                        break;

                    case WEEK_ALARM_REP4: // ��һ
                        Ret = IDS_REP_MODE_3;
                        break;

                    case WEEK_ALARM_REP5: // �ܶ�
                        Ret = IDS_REP_MODE_4;
                        break;

                    case WEEK_ALARM_REP6: // ����
                        Ret = IDS_REP_MODE_5;
                        break;

                    case WEEK_ALARM_REP7: // ����
                        Ret = IDS_REP_MODE_6;
                        break;

                    case WEEK_ALARM_REP8: // ����
                        Ret = IDS_REP_MODE_7;
                        break;

                    case WEEK_ALARM_REP9: // ����
                        Ret = IDS_REP_MODE_8;
                        break;

                    case WEEK_ALARM_REP10: // ����
                        Ret = IDS_REP_MODE_9;
                        break;
                        
                    case WEEK_ALARM_REP11:   // һ��
                        Ret = IDS_REP_MODE_10;
                        break;
                        
                    default:
                        Ret = IDS_REP_MODE_0;
                        break;
                }            

                if( pMe->m_ClockCfg.dwWATime[i] > 0 || pMe->m_ClockCfg.bStateOn[i] || pMe->m_ClockCfg.RepMode[i] || pMe->m_ClockCfg.Snooze[i])
                {
                    MEMSET(alarmClockInfo,0X00,sizeof(alarmClockInfo));
                    length = ISHELL_LoadResString( pMe->m_pShell,
                                            AEE_CLOCKAPPS_RES_FILE,
                                            Ret,
                                            alarmClockInfo,
                                            (64 - length) << 1
                                        );
                    alarmClockInfo[length ++] = (AECHAR)' ';

                    #if defined( FEATURE_ONCE_ALARM)                 
                        Appscommon_FormatTimeString( pMe->m_ClockCfg.dwWATime[i],
                                             alarmClockInfo + length,
                                             18
                                         );
                    #else
                        Appscommon_FormatTimeString( pMe->m_ClockCfg.dwWATime[i] / 1000,
                                             alarmClockInfo + length,
                                             18
                                         );
                    #endif

                    length = WSTRLEN( alarmClockInfo);
                    alarmClockInfo[length ++] = (AECHAR)' ';
#if defined( FEATURE_ON_OFF_USE_LOCAL_LANGUAGE)
                    if( pMe->m_ClockCfg.bStateOn[i])
                    {
                        AECHAR Alarmstate[64];
                        int Alarmstate_len;
                        int i;
                        
                        (void) ISHELL_LoadResString(pMe->m_pShell,
                                                    AEE_CLOCKAPPS_RES_FILE,
                                                    IDS_ALARM_ON,
                                                    Alarmstate,
                                                    sizeof(Alarmstate));
                                           
   
                        Alarmstate_len = WSTRLEN( Alarmstate);  
                        for(i=0;i<Alarmstate_len; i++)
                        {
                          alarmClockInfo[length ++] = Alarmstate[i];
                        }
                    }
                    else
                    {
                        AECHAR Alarmstate[64];
                        int Alarmstate_len;
                        int i;
                        
                        (void) ISHELL_LoadResString(pMe->m_pShell,
                                                    AEE_CLOCKAPPS_RES_FILE,
                                                    IDS_ALARM_OFF,
                                                    Alarmstate,
                                                    sizeof(Alarmstate));
                                           
   
                        Alarmstate_len = WSTRLEN( Alarmstate);  
                        for(i=0;i<Alarmstate_len; i++)
                        {
                          alarmClockInfo[length ++] = Alarmstate[i];
                        }
                    }
#else
                    if( pMe->m_ClockCfg.bStateOn[i])
                    {
                        alarmClockInfo[length ++] = (AECHAR)'O';
                        alarmClockInfo[length ++] = (AECHAR)'N';
                    }
                    else
                    {
                        alarmClockInfo[length ++] = (AECHAR)'O';
                        alarmClockInfo[length ++] = (AECHAR)'F';
                        alarmClockInfo[length ++] = (AECHAR)'F';
                    }
#endif
                    
                    alarmClockInfo[length] = (AECHAR)0;
                    IMENUCTL_SetItemText( pMenuAlarmList, IDC_MENU_ALARMCLOCK_LIST_ITEM_1 + i, 0, 0, alarmClockInfo);
                }
            }
            MenuSel = IMENUCTL_GetSel( pMenuAlarmList);
			{
				AECHAR WTitle[40] = {0};
				(void)ISHELL_LoadResString(pMe->m_pShell,
                        AEE_CLOCKAPPS_RES_FILE,                                
                        IDS_ALARMCLOCK_TITLE,
                        WTitle,
                        sizeof(WTitle));
                if(pMe->m_pIAnn != NULL)
                {
				    IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
                }
            }
            if(pMe->m_ClockCfg.RepMode[MenuSel - IDC_MENU_ALARMCLOCK_LIST_ITEM_1])
            {
                IMENUCTL_SetBottomBarType( pMenuAlarmList, BTBAR_OPTION_BACK);
            }
            else
            {
                IMENUCTL_SetBottomBarType( pMenuAlarmList, BTBAR_ADD_BACK);
            }
            IMENUCTL_SetSel(pMenuAlarmList, pMe->m_currDlgId);
            (void)IMENUCTL_Redraw(pMenuAlarmList);
        }
        return TRUE;

        case EVT_CTL_SEL_CHANGED:
            {
                uint32 MenuSel;
                
                (void) ISHELL_GetPrefs( pMe->m_pShell,
                                        AEECLSID_ALARMCLOCK,
                                        CLOCK_CFG_VERSION,
                                        &pMe->m_ClockCfg,
                                        sizeof(ClockAppCfg));  
                
                MenuSel = IMENUCTL_GetSel( pMenuAlarmList);
                if(pMe->m_ClockCfg.RepMode[MenuSel - IDC_MENU_ALARMCLOCK_LIST_ITEM_1])
                {
                    IMENUCTL_SetBottomBarType( pMenuAlarmList, BTBAR_OPTION_BACK);
                }
                else
                {
                    IMENUCTL_SetBottomBarType( pMenuAlarmList, BTBAR_ADD_BACK);
                } 
            }
            (void)IMENUCTL_Redraw(pMenuAlarmList);
            return TRUE;
        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
        {
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    break;

#if defined( AEE_SIMULATOR)
                case AVK_SOFT1:
                    gotoNextState( DLGRET_ALARMCLOCK_OPTION)
                    break;
#else
                case AVK_INFO:
                    gotoNextState( DLGRET_ALARMCLOCK_SUB)
                    break;
#endif

            }
            return TRUE;
        }

        case EVT_COMMAND:
#if defined( AEE_SIMULATOR)
            gotoNextState( DLGRET_ALARMCLOCK_SUB)
#else
            {
                uint32 MenuSel;
                
                (void) ISHELL_GetPrefs( pMe->m_pShell,
                                        AEECLSID_ALARMCLOCK,
                                        CLOCK_CFG_VERSION,
                                        &pMe->m_ClockCfg,
                                        sizeof(ClockAppCfg));  
                
                MenuSel = IMENUCTL_GetSel( pMenuAlarmList);
                if(pMe->m_ClockCfg.RepMode[MenuSel - IDC_MENU_ALARMCLOCK_LIST_ITEM_1])
                {
                    gotoNextState( DLGRET_ALARMCLOCK_OPTION)
                }
                else
                {
                    gotoNextState( DLGRET_ALARMCLOCK_SUB)
                } 
            }
#endif
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleAlarmMainDialogEvent

static boolean  HandleAlarmOptionDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{


    IMenuCtl* pMenuOption = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_MENU_OPTION);
    if( pMenuOption == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
        {
            IMENUCTL_AddItem( pMenuOption, APPSCOMMON_RES_LANG_FILE, IDS_EDIT, IDS_EDIT, 0, 0);
            if( pMe->m_ClockCfg.bStateOn[pMe->m_eCurAlarmType])
            {
                IMENUCTL_AddItem( pMenuOption,
                            AEE_CLOCKAPPS_RES_FILE,
                            IDS_ALARM_OFF,
                            IDS_ALARM_OFF,
                            0,
                            0
                        );
            }
            else if( pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] > 0)
            {
                IMENUCTL_AddItem( pMenuOption,
                            AEE_CLOCKAPPS_RES_FILE,
                            IDS_ALARM_ON,
                            IDS_ALARM_ON,
                            0,
                            0
                        );
            }

            IMENUCTL_AddItem( pMenuOption, AEE_CLOCKAPPS_RES_FILE, IDS_CLEAR, IDS_CLEAR, 0, 0);
            IMENUCTL_AddItem( pMenuOption, AEE_CLOCKAPPS_RES_FILE, IDS_CLEAR_ALL, IDS_CLEAR_ALL, 0, 0);
        }
        return TRUE;

        case EVT_DIALOG_START:
        {
            pMe->m_return = 4;
            IMENUCTL_SetPopMenuRect( pMenuOption);
            IMENUCTL_SetProperties( pMenuOption, MP_WRAPSCROLL | MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetBottomBarType( pMenuOption, BTBAR_SELECT_BACK);
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_ALARMCLOCK,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
        }
        return TRUE;

        case EVT_USER_REDRAW:
        {
            (void)IMENUCTL_Redraw( pMenuOption);
        }
        return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
        {
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
            }
        }
        return TRUE;

        case EVT_COMMAND:
        {
            switch(wParam)
            {
                case IDS_EDIT:
                {
                    CLOSE_DIALOG( DLGRET_ALARMCLOCK_SUB);
                    return TRUE;
                }
                break;

                case IDS_ALARM_ON:
                case IDS_ALARM_OFF:
                {
                    pMe->m_ClockCfg.bStateOn[pMe->m_eCurAlarmType] = wParam == IDS_ALARM_ON ? 1:0;

#if defined( FEATURE_ONCE_ALARM)
                    if( pMe->m_ClockCfg.bStateOn[pMe->m_eCurAlarmType] &&
                        pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] == WEEK_ALARM_REP11
                    )
                    {
                        uint32      sec     = pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] % (24*3600);
                        uint32      now     = GETTIMESECONDS();
                        uint32      today   = 0;
                        JulianType  jdate   = {0};

                        GETJULIANDATE( now, &jdate);
                        jdate.wHour = jdate.wMinute = jdate.wSecond = 0;
                        today = JULIANTOSECONDS( &jdate);

                        sec += today;
                        if( sec < now)
                        {
                            sec += 24*60*60;
                        }
                        pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] = sec;
                    }
#endif
                }
                break;
                
                case IDS_CLEAR:
                    pMe->m_DelAll = FALSE;
                    CLOSE_DIALOG(DLGRET_DELCONFIRM)
                    return TRUE;
                    
                case IDS_CLEAR_ALL:
                    pMe->m_DelAll = TRUE;
                    CLOSE_DIALOG(DLGRET_DELCONFIRM)
                    return TRUE;

            }
            (void) ISHELL_SetPrefs(pMe->m_pShell,
                           AEECLSID_ALARMCLOCK,
                           CLOCK_CFG_VERSION,
                           &pMe->m_ClockCfg,
                           sizeof(ClockAppCfg)
                       );

            {
                int     i           = 0;
                boolean allClosed   = TRUE;
                for( i = 0; i < NUM_OF_ALARMCLOCK; i ++)
                {
                    if( pMe->m_ClockCfg.bStateOn[i])
                    {
                        allClosed = FALSE;
                    }
                    CClockApps_UpdateAlarmTimer(pMe, i);
                }

#if defined( FEATURE_POWERDOWN_ALARM)
                registerPowerdownAlarmclock();
#endif

                if( allClosed)
                {
                    if(pMe->m_pIAnn != NULL)
                    {
					    IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
                        IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_ALARM_OFF/*ANNUN_STATE_OFF*/);
                    }
                }
            }
            CLOSE_DIALOG( DLGRET_CANCELED)
        }
        return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleAlarmOptionDialogEvent

/*==============================================================================
������
       HandleAlarmSubDialogEvent
˵����
       IDD_ALARMCLOCK_SUB�Ի����¼�������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  HandleAlarmSubDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    static byte timeFormatType = 0;
    debug("%x, %x ,%x,HandleAlarmSubDialogEvent",eCode,wParam,dwParam);

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch

		if (eCode == EVT_PEN_UP)
		{
			int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
			int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
			AEERect bottomBarRect;
			//int ht;
			int nBarH ;
			AEEDeviceInfo devinfo;
			nBarH = GetBottomBarHeight(pMe->m_pDisplay);
			
			MEMSET(&devinfo, 0, sizeof(devinfo));
			ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
			SETAEERECT(&bottomBarRect, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
			if( TOUCH_PT_IN_RECT(wXPos, wYPos, bottomBarRect))
			{
				if(wXPos >= bottomBarRect.x + (bottomBarRect.dx/3)*2 && wXPos < bottomBarRect.x + (bottomBarRect.dx/3)*3 )//��
				{						
					boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_ALARMCLOCK,EVT_USER,AVK_CLR,0);
					return rt;
				}
				else if((wXPos >= bottomBarRect.x) && (wXPos < bottomBarRect.x + (bottomBarRect.dx/3)))//��
				{						
					//boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_ALARMCLOCK,EVT_USER,AVK_SELECT,0);
					//MSG_FATAL("AEECLSID_ALARMCLOCK............",0,0,0);
					//return rt;
					eCode = EVT_KEY_RELEASE;
					wParam = AVK_SELECT;
				}
			}	
			
			if (TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->rectLine[0]))
			{
				if (IMENUCTL_IsActive(pMe->m_pState))
				{
					if (TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->pL_Rect1))
					{
						eCode = EVT_KEY;
						wParam = AVK_LEFT;
					}
					else if(TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->pR_Rect1))
					{
						eCode = EVT_KEY;
						wParam = AVK_RIGHT;
					}
				}
				else
				{
					//eCode = EVT_KEY_PRESS;
					pMe->m_nCtlID = 1;
	
					wParam = AVK_INFO;
			
					IMENUCTL_SetActive(pMe->m_pState,TRUE);
					IMENUCTL_SetActive(pMe->m_pRepMode,FALSE);
					ITIMECTL_SetActive(pMe->m_pTime,FALSE);
					IMENUCTL_SetActive(pMe->m_pSnooze,FALSE);
					(void) ISHELL_PostEvent( pMe->m_pShell,
						AEECLSID_ALARMCLOCK,
						EVT_FOCUS_SWITCH,
						wParam,
						0);
					 return TRUE;
				
				}
			}
			else if (TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->rectLine[1]))
			{
				if (ITIMECTL_IsActive(pMe->m_pTime))
				{
					if (wXPos<60)
					{
						eCode = EVT_KEY;
						wParam = AVK_LEFT;
					}
					else if(wXPos>148)
					{
						eCode = EVT_KEY;
						wParam = AVK_RIGHT;
					}
				}
				else
				{
					//eCode = EVT_KEY_PRESS;
					pMe->m_nCtlID = 2;
			
					wParam = AVK_INFO;
	
					IMENUCTL_SetActive(pMe->m_pState,FALSE);
					IMENUCTL_SetActive(pMe->m_pRepMode,FALSE);
					ITIMECTL_SetActive(pMe->m_pTime,TRUE);
					IMENUCTL_SetActive(pMe->m_pSnooze,FALSE);
					(void) ISHELL_PostEvent( pMe->m_pShell,
						AEECLSID_ALARMCLOCK,
						EVT_FOCUS_SWITCH,
						wParam,
						0);
					return TRUE;
				}
			}
			else if (TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->rectLine[2]))
			{
				if (IMENUCTL_IsActive(pMe->m_pRepMode))
				{
					if (TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->pL_Rect2))
					{
						eCode = EVT_KEY;
						wParam = AVK_LEFT;
					}
					else if(TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->pR_Rect2))
					{
						eCode = EVT_KEY;
						wParam = AVK_RIGHT;
					}
				}
				else
				{
					//eCode = EVT_KEY_PRESS;
					pMe->m_nCtlID = 3;
	
					wParam = AVK_INFO;
	
				   IMENUCTL_SetActive(pMe->m_pRepMode,TRUE);
				   ITIMECTL_SetActive(pMe->m_pTime,FALSE);
				   IMENUCTL_SetActive(pMe->m_pState,FALSE);
				   IMENUCTL_SetActive(pMe->m_pSnooze,FALSE);
				   (void) ISHELL_PostEvent( pMe->m_pShell,
					   AEECLSID_ALARMCLOCK,
					   EVT_FOCUS_SWITCH,
					   wParam,
					   0);
				   return TRUE;
				}
	
			}
			else if (TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->rectLine[3]))
			{
				if (IMENUCTL_IsActive(pMe->m_pSnooze))
				{
					if (TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->pL_Rect3))
					{
						eCode = EVT_KEY;
						wParam = AVK_LEFT;
					}
					else if(TOUCH_PT_IN_RECT(wXPos, wYPos, pMe->pR_Rect3))
					{
						eCode = EVT_KEY;
						wParam = AVK_RIGHT;
					}
				}
				else
				{
					//eCode = EVT_KEY_PRESS;
					pMe->m_nCtlID = 4;
	
					wParam = AVK_INFO;
	
					IMENUCTL_SetActive(pMe->m_pRepMode,FALSE);
					ITIMECTL_SetActive(pMe->m_pTime,FALSE);
					IMENUCTL_SetActive(pMe->m_pState,FALSE);
					IMENUCTL_SetActive(pMe->m_pSnooze,TRUE);
					(void) ISHELL_PostEvent( pMe->m_pShell,
						AEECLSID_ALARMCLOCK,
						EVT_FOCUS_SWITCH,
						wParam,
						0);
					return TRUE;
				}
			}
		}
#endif


	if(eCode == EVT_DIALOG_INIT)
    {
        return TRUE;
    }

    if(eCode == EVT_DIALOG_START)
    {
        uint32 nItemID = 0, nstrID = 0;
       // extern int OEM_GetConfig( AEEConfigItem i, void * pBuff, int nSize);
        //OEM_GetConfig( CFGI_TIME_FORMAT, &timeFormatType, sizeof( byte));
        (void) ICONFIG_GetItem( pMe->m_pConfig,
                               CFGI_TIME_FORMAT,
                               &timeFormatType,
                               sizeof(timeFormatType));        
#if defined( AEE_SIMULATOR)
        timeFormatType = OEMNV_TIMEFORM_AMPM;
#endif

        pMe->m_return = 2;
        debug("pMe->m_return = 2");
        
        for(nstrID = IDS_STATE_ON, nItemID = ITEM_STATE_ON; nItemID <= ITEM_STATE_OFF; nstrID--, nItemID++)
        {
            (void)IMENUCTL_AddItem(pMe->m_pState,
                                   AEE_CLOCKAPPS_RES_FILE,
                                   nstrID,
                                   nItemID,
                                   NULL,
                                   0);
        }
        
#if defined( FEATURE_CARRIER_ISRAEL_PELEPHONE)
        (void)IMENUCTL_AddItem(pMe->m_pRepMode,
                               AEE_CLOCKAPPS_RES_FILE,
                               IDS_REP_MODE_0,
                               ITEM_REP_MODE_1,
                               NULL,
                               0);
        for(nstrID = IDS_REP_MODE_3, nItemID = ITEM_REP_MODE_4; nItemID <= ITEM_REP_MODE_10; nstrID++, nItemID++)
#else
        for(nstrID = IDS_REP_MODE_0, nItemID = ITEM_REP_MODE_1; nItemID <= ITEM_REP_MODE_10; nstrID++, nItemID++)
#endif
        {
            (void)IMENUCTL_AddItem(pMe->m_pRepMode,
                                   AEE_CLOCKAPPS_RES_FILE,
                                   nstrID,
                                   nItemID,
                                   NULL,
                                   0);
        }
#if defined( FEATURE_CARRIER_ISRAEL_PELEPHONE)
        for(nstrID = IDS_REP_MODE_1, nItemID = ITEM_REP_MODE_2; nItemID <= ITEM_REP_MODE_3; nstrID++, nItemID++)
        {
            (void)IMENUCTL_AddItem(pMe->m_pRepMode,
                                   AEE_CLOCKAPPS_RES_FILE,
                                   nstrID,
                                   nItemID,
                                   NULL,
                                   0);
        }
#endif
#if defined( FEATURE_ONCE_ALARM)
        (void)IMENUCTL_AddItem(pMe->m_pRepMode,
                               AEE_CLOCKAPPS_RES_FILE,
                               IDS_REP_MODE_10,
                               ITEM_REP_MODE_11,
                               NULL,
                               0);
#endif
    
        for(nstrID = IDS_SNOOZE_1, nItemID = ITEM_SNOOZE_1; nItemID <= ITEM_SNOOZE_4; nstrID++, nItemID++)
        {
            (void)IMENUCTL_AddItem(pMe->m_pSnooze,
                                   AEE_CLOCKAPPS_RES_FILE,
                                   nstrID,
                                   nItemID,
                                   NULL,
                                   0);
        }

        //ʱ��ؼ�����ʾ����,��activeʱ����
        ITIMECTL_SetFont( pMe->m_pTime, AEE_FONT_BOLD, AEE_FONT_BOLD);
        ITIMECTL_SetProperties(pMe->m_pTime, TP_NO_SECONDS | TP_AUTOREDRAW);
        ITIMECTL_SetOemProperties( pMe->m_pTime, TP_OEM_COUNTDOWNCTL_EDITABLE|(timeFormatType==OEMNV_TIMEFORM_AMPM?TP_OEM_COUNTDOWNCTL_12_FORMAT:0));
        IMENUCTL_SetOemProperties(pMe->m_pState, OEMMP_SWITCHNAVIGATIONKEY | OEMMP_IDF_ALIGN_CENTER);
        IMENUCTL_SetOemProperties(pMe->m_pRepMode, OEMMP_SWITCHNAVIGATIONKEY | OEMMP_IDF_ALIGN_CENTER);
        IMENUCTL_SetOemProperties(pMe->m_pSnooze, OEMMP_SWITCHNAVIGATIONKEY | OEMMP_IDF_ALIGN_CENTER);

        //��¼����ʱ��
#if defined( FEATURE_ONCE_ALARM)
        if( pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] == WEEK_ALARM_REP11)
        {
            pMe->m_dwDispTime = pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] % (24*3600);
        }
        else
        {
            pMe->m_dwDispTime = pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType];
        }
        if( !pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType])
        {
            pMe->m_dwDispTime = 8 * 60 * 60;
        }
        pMe->m_dwDispTime *= 1000;
#else
        pMe->m_dwDispTime = pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType];
        if( !pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType])
        {
            pMe->m_dwDispTime = 8 * 60 * 60 * 1000;
        }
#endif
        pMe->m_dwHour = (pMe->m_dwDispTime/MSSEC_PER_HOUR) * MSSEC_PER_HOUR;
        pMe->m_dwMin = pMe->m_dwDispTime%MSSEC_PER_HOUR;
        
        (void) ISHELL_PostEvent( pMe->m_pShell,
                                 AEECLSID_ALARMCLOCK,
                                 EVT_USER_REDRAW,
                                 0,
                                 0);
        IDIALOG_SetProperties(pMe->m_pActiveDlg,DLG_NOT_SET_FOCUS_AUTO);
        return TRUE;
    }

    if(eCode == EVT_USER_REDRAW)
    {
        {
            AEEMenuColors color;        //List Menu ���ı���������ɫ
            AECHAR      wszState[8];    //���ӿ���״̬
            AECHAR      wszTime[8];     //ʱ��
            AECHAR      wszMode[8];     //�ظ���ʽ
            AECHAR      wszSnooze[8];   //snooze

            int title_hight = GetTitleBarHeight(pMe->m_pDisplay);
            int bottomheight = GetBottomBarHeight(pMe->m_pDisplay);
            int fontHeight = IDISPLAY_GetFontMetrics( pMe->m_pDisplay, AEE_FONT_BOLD, NULL, NULL);
			#if 0  //add by yangdecai
            int lineSpaceV = ( pMe->m_rc.dy - title_hight - bottomheight - fontHeight * 4) / 5;
			#else
			int lineSpaceV = ( pMe->m_rc.dy - bottomheight - fontHeight * 4) / 5;
			#endif

            int     i           = 0;
            int     width       = 0;
            int     widthMax    = 0;
            AECHAR* label[4]    = { 0};
            
            label[0] = wszState;
            label[1] = wszTime;
            label[2] = wszMode;
            label[3] = wszSnooze;
            MEMSET(wszState,0,sizeof(wszState));
            MEMSET(wszTime,0,sizeof(wszTime));
            MEMSET(wszMode,0,sizeof(wszMode));
            MEMSET(wszSnooze,0,sizeof(wszSnooze));

            //��ʼ��List Menu����ɫ
            color.wMask = MC_SEL_TEXT | MC_SEL_BACK | MC_TEXT | MC_BACK;
            color.cBack = RGB_WHITE;
            color.cText =  0x84848400;
            color.cSelBack = RGB_WHITE;
            color.cSelText = RGB_BLACK;

            //����LIST menu�ؼ�����ɫ
            IMENUCTL_SetColors(pMe->m_pState, &color);
            IMENUCTL_SetColors(pMe->m_pRepMode, &color);
            IMENUCTL_SetColors(pMe->m_pSnooze, &color);

            //��䱳����ɫ
#ifdef FEATURE_CARRIER_CHINA_VERTU
            {
                IImage *pImageBg = ISHELL_LoadResImage(pMe->m_pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_ALARM_BACKGROUND);
                
                Appscommon_ResetBackground(pMe->m_pDisplay, pImageBg, APPSCOMMON_BG_COLOR, &pMe->m_rc, 0, 0);
                if(pImageBg != NULL)
                {
                    IImage_Release(pImageBg);
                }
            }
#else
            Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
#endif

            //Draw title
            {
                TitleBar_Param_type title;
                AECHAR wszTitle[16] = {0};
                ISHELL_LoadResString(pMe->m_pShell,AEE_CLOCKAPPS_RES_FILE,IDS_ALARMCLOCK_1 + pMe->m_eCurAlarmType, wszTitle, sizeof(wszTitle));
                MEMSET(&title,0,sizeof(TitleBar_Param_type));
                title.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
                title.pwszTitle = wszTitle;
				#if 0
                DrawTitleBar(pMe->m_pDisplay,&title);
				#else
                if(pMe->m_pIAnn != NULL)
                {
				    IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,wszTitle);
                }
				#endif
            }

            //״̬����
            (void) ISHELL_LoadResString(pMe->m_pShell,
                                        AEE_CLOCKAPPS_RES_FILE,
                                        IDS_CLOCK_STATE,
                                        wszState,
                                        sizeof(wszState));

            //ʱ�����
            (void) ISHELL_LoadResString(pMe->m_pShell,
                                        AEE_CLOCKAPPS_RES_FILE,
                                        IDS_CLOCK_TIME,
                                        wszTime,
                                        sizeof(wszTime));

            //��ʽ����
            (void) ISHELL_LoadResString(pMe->m_pShell,
                                        AEE_CLOCKAPPS_RES_FILE,
                                        IDS_REPEAT_MODE,
                                        wszMode,
                                        sizeof(wszMode));

            //SNOOZE����
            (void) ISHELL_LoadResString(pMe->m_pShell,
                                        AEE_CLOCKAPPS_RES_FILE,
                                        IDS_CLOCK_SNOOZE,
                                        wszSnooze,
                                        sizeof(wszSnooze));                                     

            IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);

            for( i = 0; i < 4; i ++)
            {
                int x = 0;
                width = IDISPLAY_MeasureText( pMe->m_pDisplay, AEE_FONT_NORMAL, label[i]);

                if( widthMax < width)
                {
                    widthMax = width;
                }
#if defined( FEATURE_CARRIER_SUDAN_SUDATEL)
#if defined( AEE_SIMULATOR)
                if( 1)
#else
                if( pMe->language == NV_LANGUAGE_ARABIC)
#endif
                {
                    x = pMe->m_rc.dx - width;
                }
#endif
				#if 0  //add by yangdecai
                SETAEERECT( &pMe->rectLine[i],
                            x,
                            title_hight + (fontHeight + lineSpaceV) * i + lineSpaceV,
                            width,
                            fontHeight
                        );
				#else
				SETAEERECT( &pMe->rectLine[i],
                            x,
                            (fontHeight + lineSpaceV) * i + lineSpaceV,
                            width,
                            fontHeight
                        );
				#endif

                IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL, label[i],-1, pMe->rectLine[i].x, pMe->rectLine[i].y, 0, IDF_TEXT_TRANSPARENT);
            }

            for( i = 0; i < 4; i ++)
            {
                pMe->rectLine[i].dx = widthMax;
            }

            IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_BLACK);
            {
                IControl* pControl[4] = { 0}; 
                        
                pControl[0] = (IControl*)pMe->m_pState;
                pControl[1] = (IControl*)pMe->m_pTime;
                pControl[2] = (IControl*)pMe->m_pRepMode;
                pControl[3] = (IControl*)pMe->m_pSnooze;
                
                for( i = 0; i < 4; i ++)
                {
                    int x = pMe->rectLine[i].dx + 2;
                    width = pMe->m_rc.dx - pMe->rectLine[i].dx - 3;

    #if defined( FEATURE_CARRIER_SUDAN_SUDATEL)
    #if defined( AEE_SIMULATOR)
                    if( 1)
    #else
                    if( pMe->language == NV_LANGUAGE_ARABIC)
    #endif
                    {
                        x = 1;
                    }
    #endif

                    SETAEERECT( &pMe->rectLine[i], x, pMe->rectLine[i].y - 2, width, pMe->rectLine[i].dy + 4);
                    IDISPLAY_FillRect( pMe->m_pDisplay, &pMe->rectLine[i], RGB_WHITE);
                    if( i == 0 || i == 2 || i == 3)
                    {
                    
                        SETAEERECT( &pMe->rectLine[i], x + LEFT_ICON_WHID, pMe->rectLine[i].y, width - LEFT_ICON_HEIGHT, pMe->rectLine[i].dy);
                        CClockApps_Draw_Arrow( pMe, &pMe->rectLine[i],i);
						
                        SETAEERECT( &pMe->rectLine[i], x + LEFT_ICON_WHID-(LEFT_ICON_WHID-8), pMe->rectLine[i].y, width - LEFT_ICON_HEIGHT+(LEFT_ICON_HEIGHT-16), pMe->rectLine[i].dy);
                    }
                    
                    ICONTROL_SetRect( pControl[i], &pMe->rectLine[i]);
                }
            }
            
            //���ø�����ʱ��,��ˢ����Ļ
            (void)ITIMECTL_SetTimeEx(pMe->m_pTime, pMe->m_dwDispTime, TRUE);
            
            if( pMe->m_nCtlID == IDC_CLOCK_TIME && timeFormatType == OEMNV_TIMEFORM_AMPM)
            {
                if( ITIMECTL_GetTime( pMe->m_pTime) >= 12*3600000)
                {
                    CLOCK_DRAW_BOTTOMBAR( BTBAR_SAVE_AM_BACK);
                }
                else
                {
                    CLOCK_DRAW_BOTTOMBAR( BTBAR_SAVE_PM_BACK);
                }
            }
            else
            {
                CLOCK_DRAW_BOTTOMBAR( BTBAR_SAVE_BACK);
            }
            //�����ظ���ʽѡ��ʽListѡ��
#if defined( FEATURE_ONCE_ALARM)            
            if(IMENUCTL_GetSel(pMe->m_pRepMode) == WEEK_ALARM_REP11)
            {
                IMENUCTL_SetSel(pMe->m_pSnooze,ITEM_SNOOZE_4);
            }
            else
#endif //defined( FEATURE_ONCE_ALARM)         
            {
                if(pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType] == 5*60)
                {
                    IMENUCTL_SetSel( pMe->m_pSnooze, ITEM_SNOOZE_1);
                }
                else if(pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType] == 10*60)
                {
                    IMENUCTL_SetSel( pMe->m_pSnooze, ITEM_SNOOZE_2);
                }
                else if(pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType] == 15*60)
                {
                    IMENUCTL_SetSel( pMe->m_pSnooze, ITEM_SNOOZE_3);
                }
                else
                {
                    IMENUCTL_SetSel( pMe->m_pSnooze, ITEM_SNOOZE_4);
                }
            }
            
            IMENUCTL_SetSel( pMe->m_pRepMode, pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType]);
            
            //STATEΪ��ʱ,״̬ѡ��"��"
            IMENUCTL_SetSel(pMe->m_pState, pMe->m_ClockCfg.bStateOn[pMe->m_eCurAlarmType] ? ITEM_STATE_ON : ITEM_STATE_OFF);
            // ͳһ���½���
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);

            CClockApps_Set_CTL(pMe);
            (void)IMENUCTL_Redraw(pMe->m_pState);
            (void)IMENUCTL_Redraw(pMe->m_pRepMode);
            (void)ITIMECTL_Redraw(pMe->m_pTime);
            (void)IMENUCTL_Redraw(pMe->m_pSnooze);

        }
        return TRUE;
    }

    if(eCode == EVT_DIALOG_END)
    {
        //���ÿؼ��л�ָʾֵ
        pMe->m_nCtlID = IDC_CLOCK_ST_LIST;   //�ؼ�ID
        pMe->m_nTimeCtlCount = 0;       //0��������Сʱ��,1�����ڷ�����
        pMe->m_nNumKeyCount = 0;        //���ñ༭ΪСʱ��ʮλ
        return TRUE;
    }
    
#if !defined( AEE_SIMULATOR)
    if(((eCode == EVT_KEY_PRESS) || (eCode == EVT_KEY))
      && (wParam == AVK_SELECT))
    {
         return TRUE;
    }
#endif

#if defined( AEE_SIMULATOR)
    if( ( (eCode == EVT_KEY_RELEASE) && (wParam == AVK_SOFT1)) ||
        ( eCode == EVT_KEY && wParam == AVK_SELECT && ((timeFormatType == OEMNV_TIMEFORM_24HR && pMe->m_nCtlID == IDC_CLOCK_TIME) || pMe->m_nCtlID != IDC_CLOCK_TIME)))
#else
    if( ( (eCode == EVT_KEY_RELEASE) && (wParam == AVK_SELECT)) ||
        ( eCode == EVT_KEY && (wParam == AVK_INFO || wParam == AVK_STAR) && ((timeFormatType == OEMNV_TIMEFORM_24HR && pMe->m_nCtlID == IDC_CLOCK_TIME) || pMe->m_nCtlID != IDC_CLOCK_TIME)))
#endif
    {
        if(CClockApps_HandleCmdEvent(pMe))
        {
            //�洢������ѡ������ݽṹ
            (void) ISHELL_SetPrefs(pMe->m_pShell,
                                   AEECLSID_ALARMCLOCK,
                                   CLOCK_CFG_VERSION,
                                   &pMe->m_ClockCfg,
                                   sizeof(ClockAppCfg));

            CClockApps_UpdateAlarmTimer(pMe, pMe->m_eCurAlarmType);
#if defined( FEATURE_POWERDOWN_ALARM)
            registerPowerdownAlarmclock();
#endif

            CLOSE_DIALOG(DLGRET_OK)
        }
        return TRUE;
    }

    if (ITIMECTL_IsActive(pMe->m_pTime))
    {
#if MIN_BREW_VERSION(3,0)
            // do not want to handle au
            if (((dwParam & KB_AUTOREPEAT) != 0) &&
                ((wParam == AVK_UP) ||
                (wParam == AVK_DOWN) ||
                (wParam == AVK_LEFT) ||
                (wParam == AVK_RIGHT)))
            {
                return TRUE;
            }
#endif
        if(ITIMECTL_HandleEvent(pMe->m_pTime, eCode, wParam, dwParam))
        {
            return TRUE;
        }
    }

    if (IMENUCTL_IsActive(pMe->m_pState))
    {
        if(IMENUCTL_HandleEvent(pMe->m_pState, eCode, wParam, dwParam))
        {
            return TRUE;
        }
    }

    if (IMENUCTL_IsActive(pMe->m_pRepMode))
    {
        if(IMENUCTL_HandleEvent(pMe->m_pRepMode, eCode, wParam, dwParam))
        {
            return TRUE;
        }
    }
    
    if (IMENUCTL_IsActive(pMe->m_pSnooze))
    {
        if(IMENUCTL_HandleEvent(pMe->m_pSnooze, eCode, wParam, dwParam))
        {
            return TRUE;
        }
    }    

    switch (eCode)
    {
        case EVT_KEY:
        {
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    return TRUE;
            }
            return TRUE;
        }

        case EVT_KEY_PRESS:
        {
            //���͸��Զ����¼�EVT_FOCUS_SWITCH����,��Ϊ�˱����л��ؼ���ͬʱ
            //�ؼ���ǰ��������¼�
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_ALARMCLOCK,
                                     EVT_FOCUS_SWITCH,
                                     wParam,
                                     0);
            return TRUE;
        }
        case EVT_FOCUS_SWITCH:
        {
            //����Clock�༭����Ӧ�����Ҽ��л��ؼ�
            CClockApps_HandleKeyEvent(pMe, wParam);

            if( wParam == AVK_UP || wParam == AVK_DOWN || wParam == AVK_INFO || wParam == AVK_STAR)
            {
                if( pMe->m_nCtlID == IDC_CLOCK_TIME && timeFormatType == OEMNV_TIMEFORM_AMPM)
                {
                    if( ITIMECTL_GetTime( pMe->m_pTime) >= 12*3600000)
                    {
                        CLOCK_DRAW_BOTTOMBAR( BTBAR_SAVE_AM_BACK);
                    }
                    else
                    {
                        CLOCK_DRAW_BOTTOMBAR( BTBAR_SAVE_PM_BACK);
                    }
                }
                else
                {
                    CLOCK_DRAW_BOTTOMBAR( BTBAR_SAVE_BACK);
                }
            }
            return TRUE;
        }

        case EVT_COMMAND:
        {
            switch(wParam)
            {
                case ITEM_STATE_OFF:
                case ITEM_STATE_ON:
                    return (IMENUCTL_HandleEvent(pMe->m_pState,
                                                 EVT_KEY,
                                                 AVK_DOWN,
                                                 0));

                case ITEM_REP_MODE_1:
                case ITEM_REP_MODE_2:
                case ITEM_REP_MODE_3:
                case ITEM_REP_MODE_4:
                case ITEM_REP_MODE_5:
                case ITEM_REP_MODE_6:
                case ITEM_REP_MODE_7:
                case ITEM_REP_MODE_8:
                case ITEM_REP_MODE_9:
                case ITEM_REP_MODE_10:
                case ITEM_REP_MODE_11:    
                    return (IMENUCTL_HandleEvent(pMe->m_pRepMode,
                                                 EVT_KEY,
                                                 AVK_DOWN,
                                                 0));
                                                 
                case ITEM_SNOOZE_1:
                case ITEM_SNOOZE_2:
                case ITEM_SNOOZE_3:
                	return (IMENUCTL_HandleEvent(pMe->m_pSnooze,
                	                             EVT_KEY,
                	                             AVK_DOWN,
                	                             0));
            }
            return TRUE;
        }
    }

    return FALSE;
} // HandleAlarmSubDialogEvent
/*==============================================================================
������
       HandleAlarmTimeMessegeDialogEvent
˵����
       IDD_ALARMTIME_MESSEGE�Ի����¼�������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  HandleAlarmTimeMessegeDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{

    static boolean saveAlertDrawn = FALSE;

    //��ʾ��Ϣ �ı��ؼ�
    pMe->m_pText = (ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_CLOCK_MESSEGE);

    if (pMe->m_pText == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                    AEECLSID_STATIC,(void **)&pMe->m_pStatic))
            {
                ERR("ISHELL_CreateInstance,AEECLSID_STATIC 2",0,0,0);
                return FALSE;
            }
            saveAlertDrawn = FALSE;
            return TRUE;

        case EVT_DIALOG_START:
            {
                AEERect rc;
                rc.x = 0;
                rc.y = 0;
                rc.dx = pMe->m_rc.dx;
                rc.dy = pMe->m_rc.dy - (BOTTOM_IMG_HIGHT+1);

                pMe->m_return = 3;
                ITEXTCTL_SetRect(pMe->m_pText, &rc);
                ITEXTCTL_SetTitle(pMe->m_pText,0,0,L" ");
                //��*���л����뷨
                ITEXTCTL_SetProperties(pMe->m_pText, TP_MULTILINE|TP_FRAME|TP_FIXSETRECT|TP_STARKEY_SWITCH|TP_FOCUS_NOSEL);
                //(void)ITEXTCTL_SetInputMode(pMe->m_pText, AEE_TM_TSIM);

                ITEXTCTL_SetMaxSize(pMe->m_pText, MAX_ALARM_INFO);

                //�����ı�
                (void)ITEXTCTL_SetText(pMe->m_pText,
                            pMe->m_ClockCfg.wszInfo[pMe->m_eCurAlarmType],
                            -1);

                (void) ISHELL_PostEvent( pMe->m_pShell,
                                         AEECLSID_ALARMCLOCK,
                                         EVT_USER_REDRAW,
                                         0,
                                         0);
            }
            return TRUE;

        case EVT_KEY_RELEASE:
        case EVT_USER_REDRAW:
            // ���Ƶײ�������ʾ��

            // ͳһ���½���
            #ifndef FEATURE_ALL_KEY_PAD    //add by yangdecai 
            if (WSTRLEN(ITEXTCTL_GetTextPtr(pMe->m_pText))>0)
            {
                // Option     Delete
                DRAW_PROMPT_BAR(BTBAR_SAVE_DELETE)
            }
            else
            #endif
            {
                // Option     Back
                DRAW_PROMPT_BAR(BTBAR_SAVE_BACK)
            }
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            return TRUE;

        case EVT_DIALOG_END:
            if (pMe->m_bSuspending)
            {
                AECHAR      getBufInfo[MAX_ALARM_INFO + 1];

                //������ʾ��Ϣ
                (void)ITEXTCTL_GetText(pMe->m_pText,
                                getBufInfo, MAX_ALARM_INFO + 1);
                WSTRCPY(pMe->m_ClockCfg.wszInfo[pMe->m_eCurAlarmType],
                        getBufInfo);
            }
            (void) ISHELL_CancelTimer(pMe->m_pShell,
                                      CClockApps_SaveAlertTimeout,
                                      pMe);
            ISTATIC_Release(pMe->m_pStatic);
            pMe->m_pStatic = NULL;
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    break;
#if 1
                case AVK_SELECT:
                    if( !saveAlertDrawn)
                    {
                        AECHAR      getBufInfo[MAX_ALARM_INFO + 1];

                        saveAlertDrawn = TRUE;

                        //������ʾ��Ϣ
                        (void)ITEXTCTL_GetText(pMe->m_pText,
                                        getBufInfo, MAX_ALARM_INFO + 1);
                        WSTRCPY(pMe->m_ClockCfg.wszInfo[pMe->m_eCurAlarmType],
                                getBufInfo);

                        //�洢������ѡ������ݽṹ
                        (void) ISHELL_SetPrefs(pMe->m_pShell,
                                               AEECLSID_ALARMCLOCK,
                                               CLOCK_CFG_VERSION,
                                               &pMe->m_ClockCfg,
                                               sizeof(ClockAppCfg));

                        /*(void) ISHELL_GetPrefs(pMe->m_pShell,
                                               AEECLSID_ALARMCLOCK,
                                               CLOCK_CFG_VERSION,
                                               &pMe->m_ClockCfg,
                                               sizeof(ClockAppCfg));*/

                        //ѡ����������ʱ,�������Ӷ�ʱ��������
                        //��ȡ��ѡ������ʱ����ȡ�����Ӷ�ʱ��,����ICONͼ��
                        CClockApps_UpdateAlarmTimer(pMe, pMe->m_eCurAlarmType);
                        //��������ʾ��Ϣ
                        CClockApps_DrawSaveAlert(pMe);
                    }
                    break;
#endif
                default:
                    break;
            }
            return TRUE;

        //������ʾ�Ի�����Ϻ�Ļص�
        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_OK)
            return TRUE;
#if 0
        case EVT_COMMAND:
            switch(wParam)
            {
                case AVK_SELECT:
                    {
                        AECHAR      getBufInfo[MAX_ALARM_INFO+1];
                        MEMSET(getBufInfo,0,sizeof(getBufInfo));
                        //������ʾ��Ϣ
                        (void)ITEXTCTL_GetText(pMe->m_pText,
                                        getBufInfo, MAX_ALARM_INFO + 1);
                        WSTRCPY(pMe->m_ClockCfg.wszInfo[pMe->m_eCurAlarmType],
                                getBufInfo);

                        //�洢������ѡ������ݽṹ
                        (void) ISHELL_SetPrefs(pMe->m_pShell,
                                               AEECLSID_ALARMCLOCK,
                                               CLOCK_CFG_VERSION,
                                               &pMe->m_ClockCfg,
                                               sizeof(ClockAppCfg));

                        //ѡ����������ʱ,�������Ӷ�ʱ��������
                        //��ȡ��ѡ������ʱ����ȡ�����Ӷ�ʱ��,����ICONͼ��
                        CClockApps_UpdateAlarmTimer(pMe, pMe->m_eCurAlarmType);
                        //��������ʾ��Ϣ
                        CClockApps_DrawSaveAlert(pMe);
                    }
                    break;

                default:
                    break;
            }
            return TRUE;
#endif
        default:
            break;
    }
    return FALSE;
} // HandleAlarmTimeMessegeDialogEvent

//extern boolean is_g_bMp3PlayerRun();
static boolean alarm_time_reach = FALSE; 
static int16 alarm_shake_time = 0;

static void alarm_shake_open(void)
{
#ifdef FEATRUE_SUPPORT_G_SENSOR

    dword shake;
    
    alarm_time_reach = TRUE;

    if((MMI_GSENSOR_SHAKE_OPEN == mmi_g_sensor_state) 
        ||(MMI_GSENSOR_SHAKE_OPEN_IN_IDLE == mmi_g_sensor_state))   //return if sensor has been open.
    {
        return;
    }
    OEM_GetConfig(CFGI_GSENSOR,&shake,sizeof(shake));
    if(shake & OEMNV_SHAKE_SNOOZE_ALARM_MASK)
    {
#ifdef FEATRUE_SUPPORT_G_SENSOR
        mmi_g_sensor_process(G_SENSOR_ENABLE | G_SENSOR_SHAKE);
#endif
        mmi_g_sensor_state = MMI_GSENSOR_SHAKE_OPEN;
    } 
    #endif
}
static void alarm_shake_close(void)
{
#ifdef FEATRUE_SUPPORT_G_SENSOR

    boolean b_FMBackground = FALSE;
    dword shake;
    
    alarm_time_reach = FALSE;

    if((MMI_GSENSOR_SHAKE_CLOSE == mmi_g_sensor_state) 
    ||(MMI_GSENSOR_SHAKE_CLOSE_IN_IDLE == mmi_g_sensor_state))  //return if sensor has been closed.
    {
        return;
    }
    OEM_GetConfig(CFGI_GSENSOR,&shake,sizeof(shake));
    OEM_GetConfig(CFGI_FM_BACKGROUND,&b_FMBackground, sizeof(b_FMBackground));

    if(shake & OEMNV_SHAKE_SNOOZE_ALARM_MASK)
    {   
        //do close shake if FM or MP3 is playing.
        if(!(((shake & OEMNV_SHAKE_FM_RADIO_MASK) && (TRUE == b_FMBackground))
            ||((shake & OEMNV_SHAKE_MUSIC_MASK) && (MP3STATUS_NONE != GetMp3PlayerStatus()))))
        {
#ifdef FEATRUE_SUPPORT_G_SENSOR    
            mmi_g_sensor_process(G_SENSOR_SHAKE_DISABLE);   
#endif
            mmi_g_sensor_state = MMI_GSENSOR_SHAKE_CLOSE;
        }
    }
#endif    
}

boolean is_alarm_time_reach()
{
    return alarm_time_reach;
}

/*==============================================================================
������
       HandleAlarmTimeReachDialogEvent
˵����
       IDD_ALARMTIME_REACH�Ի����¼�������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼��������??
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  HandleAlarmTimeReachDialogEvent(CClockApps *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{

    static byte keyBeepVolumeSetting = 0;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
			if (eCode == EVT_PEN_UP)
			{
				int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
				int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
				AEERect bottomBarRect;
				//int ht;
				int nBarH ;
				AEEDeviceInfo devinfo;
				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
				
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&bottomBarRect, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
				if( TOUCH_PT_IN_RECT(wXPos, wYPos, bottomBarRect))
				{
					if(wXPos >= bottomBarRect.x + (bottomBarRect.dx/3)*2 && wXPos < bottomBarRect.x + (bottomBarRect.dx/3)*3 )//��
					{						
						boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_ALARMCLOCK,EVT_USER,AVK_CLR,0);
						return rt;
					}
					else if((wXPos >= bottomBarRect.x) && (wXPos < bottomBarRect.x + (bottomBarRect.dx/3)))//��
					{						
						//boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_ALARMCLOCK,EVT_USER,AVK_SELECT,0);
						//MSG_FATAL("AEECLSID_ALARMCLOCK............",0,0,0);
						//return rt;
						eCode = EVT_KEY;
						wParam = AVK_SELECT;
					}
				}
			}
#endif

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
        {
            alarm_shake_open();
            {
                byte mute = OEMSOUND_MUTE_VOL;
                ICONFIG_GetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &keyBeepVolumeSetting, sizeof(byte));
                ICONFIG_SetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &mute, sizeof(byte));
            }
            if( !pMe->m_bAlarmOff)
            {

                //�������庯���������л��Ʊ�֤�����������
                if (ClockApps_CanAlert(pMe))
                {
#if 0
                    IBACKLIGHT_Enable(pMe->m_pBacklight);
#endif

#ifdef FEATURE_LED_CONTROL
                    IBACKLIGHT_SigLedEnable( pMe->m_pBacklight, SIG_LED_ALARM);
#endif
                }
            }

            (void) ISHELL_PostEvent(pMe->m_pShell,
                                    AEECLSID_ALARMCLOCK,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
            return TRUE;
        }
        case EVT_USER_REDRAW:
            {
                uint32  dwAlarmTime;         //�û����õ�����ʱ�� ��λ:����
                AECHAR  wszTime[16];        //ʱ��
#ifdef FEATURE_SUB_LCD
                AECHAR  subTitleBuf[8];     //С�������ı��Ĵ洢buf
                AECHAR  subTimeBuf[8];      //С��ʱ���ı��Ĵ洢buf
                AECHAR  subMesssgeBuf[MAX_ALARM_INFO+1];//С����ʾ��Ϣ�ı��Ĵ洢buf
#endif

                MEMSET(wszTime,0,sizeof(wszTime));
#ifdef FEATURE_SUB_LCD
                MEMSET(subTitleBuf,0,sizeof(subTitleBuf));
                MEMSET(subTimeBuf,0,sizeof(subTimeBuf));
                WSTRCPY(subTitleBuf, wszTitle);
#endif //FEATURE_SUB_LCD

                //��ȡ������ѡ������ݽṹ
                (void) ISHELL_GetPrefs(pMe->m_pShell,
                                       AEECLSID_ALARMCLOCK,
                                       CLOCK_CFG_VERSION,
                                       &pMe->m_ClockCfg,
                                       sizeof(ClockAppCfg));


                // ȡ�û����õ�����ʱ�� ��λ:����
#if defined( FEATURE_ONCE_ALARM)
                dwAlarmTime = pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType];
#else
                dwAlarmTime = pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] / 1000;
#endif

                Appscommon_FormatTimeString( dwAlarmTime, wszTime, sizeof( wszTime));

#ifdef FEATURE_SUB_LCD
                WSTRCPY(subTimeBuf, wszTime);
                if (bFlipClosed)
                {
                    //���ǹر�ʱ��С���ϻ�����ʾ��Ϣ.
                    CClockApps_DispalySub(pMe, subTitleBuf, subTimeBuf,
                                          subMesssgeBuf);
                }
#endif
                //���������ͼ��
                CClockApps_AniClockImg(pMe);

#if 0  //���ﲻ�������趨ʱ�䣬����ǰʱ�䣬������ CClockApps_AniclockImg ��ʵ��
                IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
                
                (void)IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_BOLD,
                                      wszTime,-1,0,pMe->m_rc.dy -
                                      GetBottomBarHeight( pMe->m_pDisplay) -
                                      IDISPLAY_GetFontMetrics( pMe->m_pDisplay, AEE_FONT_BOLD, 0, 0), NULL,
                                      IDF_ALIGN_CENTER | IDF_TEXT_TRANSPARENT);
             
                IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_BLACK);
#endif

#if !defined( AEE_SIMULATOR)
                if( !pMe->m_bAlarmOff)
                {
                    uint16  ring_id = 1;
                    byte    profilenum;
                    ringID  ringid[PROFILENUMBER];

                     (void) ISHELL_SendEvent(pMe->m_pShell,
                                             AEECLSID_APP_FMRADIO,
                                             EVT_ALARM,
                                             TRUE,
                                             0
                                         );

                    ClockNotifyMP3PlayerAlertEvent(pMe, TRUE);
                    OEM_GetConfig( CFGI_ALARM_RINGER, &ring_id, sizeof(ring_id));

                    ICONFIG_GetItem(pMe->m_pConfig, CFGI_PROFILE_CUR_NUMBER, &profilenum, sizeof(profilenum));
                    ICONFIG_GetItem(pMe->m_pConfig, CFGI_PROFILE_ALARM_RINGER, (void*)ringid, sizeof(ringid));
                    if(ringid[profilenum].ringType == OEMNV_MID_RINGER)
                    {
                        IALERT_StartRingerAlert(pMe->m_pAlert, ring_id);
                    }
                    else
                    {
                        if ((IALERT_StartMp3Alert(pMe->m_pAlert, ringid[profilenum].szMusicname, ALERT_NORMAL_SND) != SUCCESS))
                        {
                            IALERT_StartRingerAlert(pMe->m_pAlert, (uint32)ring_id);
                        }                        
                    }
                    (void) ISHELL_SetTimer(pMe->m_pShell,
                                    MAX_ALARM_MSECS,
                                    (PFNNOTIFY) CClockApps_DeActivate,
                                    pMe);
                    ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)CClockApps_Snooze, pMe);
                    (void) ISHELL_SetTimer(pMe->m_pShell,
                                    MAX_ALARM_MSECS,
                                    (PFNNOTIFY) CClockApps_Snooze,
                                    pMe);                    
                }
#endif

                pMe->m_bWeekAlarmEnabled = FALSE;
            }
            return TRUE;

        case EVT_DIALOG_END:
#if 0
            IBACKLIGHT_Enable(pMe->m_pBacklight);
#endif
#ifdef FEATURE_LED_CONTROL
            IBACKLIGHT_SigLedDisable( pMe->m_pBacklight);
#endif
#if !defined( AEE_SIMULATOR)
            IALERT_StopRingerAlert(pMe->m_pAlert);
            IALERT_StopMp3Alert(pMe->m_pAlert);
#endif

             (void) ISHELL_PostEventEx(pMe->m_pShell,
                                     0,
                                     AEECLSID_APP_FMRADIO,
                                     EVT_ALARM,
                                     FALSE,
                                     0
                                 );
            ClockNotifyMP3PlayerAlertEvent(pMe, FALSE);
#ifdef FEATURE_SUB_LCD
            if (pMe->m_pWallSubImage)
            {
                IBITMAP_Release(pMe->m_pWallSubImage);
                pMe->m_pWallSubImage=NULL;
            }
#endif
            ICONFIG_SetItem( pMe->m_pConfig, CFGI_BEEP_VOL, &keyBeepVolumeSetting, sizeof(byte));
            return TRUE;

        case EVT_GSENSOR_SHAKE: 
        {
            switch(wParam)
            {
                case AVK_GSENSOR_FORWARD: 
                case AVK_GSENSOR_BACKWARD:
                {
                    boolean isShake0n = FALSE; 
                    (void) ICONFIG_GetItem(pMe->m_pConfig,
                                            CFGI_SHAKE_SNOOZE_ALARM_CHECK,
                                            &isShake0n, 
                                            sizeof(boolean));
                    if(isShake0n)
                    {
                        alarm_shake_time++;
                    }
                    if(2 <= alarm_shake_time)
                    {
                        CClockApps_Snooze(pMe);                       
                        alarm_shake_time = 0;
                        alarm_shake_close();
                        return TRUE;
                    }
                    else
                    {
                        return TRUE;
                    }
                    break;
                }
            }
            return TRUE;    
        }
        
        case EVT_KEY:
        {
            alarm_shake_close();       
            //�򿪷��ǲ��ر�����,��ΪҪ��ʾ��ʾ��Ϣ;��������Ҳ���ر�����
            switch(wParam)
            {
                case AVK_CLR:
                {
#if !defined( AEE_SIMULATOR)
                    pMe->m_Snooze = FALSE;
                    ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)CClockApps_Snooze, pMe);
                    CClockApps_DeActivate( pMe);
#endif
                    return TRUE;
                }
                default:
                case AVK_SELECT:
                {
                    CClockApps_Snooze(pMe);
                    return TRUE;
                }
                
            }
            return TRUE;
        }

/*        case EVT_PEN_DOWN_1:
            IALERT_StopAlarmClockAlert(pMe->m_pAlert);
            CLOSE_DIALOG(DLGRET_CANCELED)
            return TRUE;
*/
//        case EVT_BK_CONTINUE_ON:
 //           return TRUE;

        case EVT_COMMAND:
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleAlarmTimeReachDialogEvent

/*==============================================================================
������
       HandleAlarmDelConfirmEvent
˵����


������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean HandleAlarmDelConfirmEvent(CClockApps *pMe,
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

   
    switch(eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
            (void) ISHELL_PostEventEx(pMe->m_pShell, 
                                    EVTFLG_ASYNC,
                                    AEECLSID_ALARMCLOCK,
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
                                AEE_CLOCKAPPS_RES_FILE,
                                IDS_DELCONFIRM,
                                wstrText,
                                sizeof(wstrText));
                                
                Msg_Param.ePMsgType = MESSAGE_CONFIRM;
                Msg_Param.pwszMsg = wstrText;
                Msg_Param.eBBarType = BTBAR_OK_BACK;
                DrawPromptMessage(pMe->m_pDisplay, pStatic, &Msg_Param);
            }
            // ���½���
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);           
            return TRUE;

        case EVT_DIALOG_END:
            if(pStatic != NULL)
            {
                ISTATIC_Release(pStatic);
                pStatic = NULL;
            }
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_SELECT:
                    CLOSE_DIALOG(DLGRET_OK)
                    break;
                    
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED)
                    break;
                    
                default:
                    break;
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
			{
				int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
				int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
				AEERect bottomBarRect;
				//int ht;
				int nBarH ;
				AEEDeviceInfo devinfo;
				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
				
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&bottomBarRect, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
				if( TOUCH_PT_IN_RECT(wXPos, wYPos, bottomBarRect))
				{
					if(wXPos >= bottomBarRect.x + (bottomBarRect.dx/3)*2 && wXPos < bottomBarRect.x + (bottomBarRect.dx/3)*3 )//��
					{						
						CLOSE_DIALOG(DLGRET_CANCELED)
                    	break;
					}
					else if((wXPos >= bottomBarRect.x) && (wXPos < bottomBarRect.x + (bottomBarRect.dx/3)))//��
					{						
						CLOSE_DIALOG(DLGRET_OK)
                    	break;
					}
				}	
			}
		break;
#endif
        default:
            break;
    }

    return FALSE;
}

/*==============================================================================
������
       HandleWarningMessegeDialogEvent
˵����
       IDD_WARNING_MESSEGE�Ի����¼�������

������
       pMe [in]��ָ��SettingMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean HandleAlarmMsgBox(CClockApps *pMe,
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
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
            ISHELL_SetTimer(pMe->m_pShell,PROMPTMSG_TIMER,CClockApps_DialogTimeout,pMe);
            ISHELL_PostEvent( pMe->m_pShell,AEECLSID_ALARMCLOCK,EVT_USER_REDRAW,0,0);
            return TRUE;

        case EVT_USER_REDRAW:
            //ɾ������
            if(!pMe->m_DelAll)
            {
                pMe->m_ClockCfg.bStateOn[pMe->m_eCurAlarmType]   = FALSE;
                pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType]    = 0;
                pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType]   = 0;
                pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType]     = 0;
            }
            else
            {
                MEMSET( &pMe->m_ClockCfg, 0, sizeof( pMe->m_ClockCfg));
            }
            (void) ISHELL_SetPrefs(pMe->m_pShell,
                           AEECLSID_ALARMCLOCK,
                           CLOCK_CFG_VERSION,
                           &pMe->m_ClockCfg,
                           sizeof(ClockAppCfg));  
            {
                int      i           = 0;
                boolean  allClosed   = TRUE;
                for( i = 0; i < NUM_OF_ALARMCLOCK; i ++)
                {
                    if( pMe->m_ClockCfg.bStateOn[i])
                    {
                        allClosed = FALSE;
                    }
                    CClockApps_UpdateAlarmTimer(pMe, i);
                }
                if((allClosed) && (pMe->m_pIAnn != NULL))
                {
					IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
                    IANNUNCIATOR_SetField(pMe->m_pIAnn, ANNUN_FIELD_ALARM, ANNUN_STATE_ALARM_OFF);
                }
            }               
            //����ʾ��Ϣ
            {
                PromptMsg_Param_type m_PromptMsg;
                
                MEMSET(&m_PromptMsg,0,sizeof(PromptMsg_Param_type));
                
                m_PromptMsg.nMsgResID= IDS_DONE;
                m_PromptMsg.ePMsgType = MESSAGE_WARNNING;
                STRLCPY(m_PromptMsg.strMsgResFile, AEE_CLOCKAPPS_RES_FILE,MAX_FILE_NAME);
                m_PromptMsg.eBBarType = BTBAR_NONE;
                
                DrawPromptMessage(pMe->m_pDisplay,pStatic,&m_PromptMsg);
                IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
                return TRUE;
            }

        case EVT_DIALOG_END:
            if(pStatic != NULL)
            {
                ISTATIC_Release(pStatic);
                pStatic = NULL;
            }
            ISHELL_CancelTimer(pMe->m_pShell, NULL,  pMe);
            return TRUE;

        case EVT_DISPLAYDIALOGTIMEOUT:
            CLOSE_DIALOG(DLGRET_MSGBOX_OK)
            return TRUE;

        case EVT_KEY:
            ISHELL_CancelTimer(pMe->m_pShell, NULL,  pMe);
            CLOSE_DIALOG(DLGRET_MSGBOX_OK)
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
			{
				int16 wXPos = (int16)AEE_GET_X((const char *)dwParam);
				int16 wYPos = (int16)AEE_GET_Y((const char *)dwParam);
				AEERect bottomBarRect;
				//int ht;
				int nBarH ;
				AEEDeviceInfo devinfo;
				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
				
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&bottomBarRect, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
				if( TOUCH_PT_IN_RECT(wXPos, wYPos, bottomBarRect))
				{
					if(wXPos >= bottomBarRect.x + (bottomBarRect.dx/3)*2 && wXPos < bottomBarRect.x + (bottomBarRect.dx/3)*3 )//��
					{						
						ISHELL_CancelTimer(pMe->m_pShell, NULL,  pMe);
            			CLOSE_DIALOG(DLGRET_MSGBOX_OK)
            			return TRUE;
					}
					
				}	
			}
		break;
#endif

        default:
            break;
    }

    return FALSE;
}

/*==============================================================================
����:
       CClockApps_DialogTimeout

˵��:
       ��ʱ���ص���������Ҫ�����Զ��ر���Ϣ�Ի���

����:
       pme [in]��void *����ָ�룬��ָCClockApps�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void CClockApps_DialogTimeout(void *pme)
{
    CClockApps *pMe = (CClockApps *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent(pMe->m_pShell,
                            AEECLSID_ALARMCLOCK,
                            EVT_DISPLAYDIALOGTIMEOUT,
                            0,
                            0);
}

/*==============================================================================
������
       CClockApps_UpdateAlarmTimer
˵����
       ���ڸ������Ӷ�ʱ��������

������
       pMe [in]��ָ��ClockApps Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eAlarmType [in]: ��������

����ֵ��
       void

��ע��

==============================================================================*/
static void CClockApps_UpdateAlarmTimer(CClockApps *pMe, AlarmType eAlarmType)
{
    uint32 dwNow;       //��ǰ��ʱ��(��)
    uint32 testtime;
    uint32 dwAlarmTime; //�û����õ�����ʱ�� ��λ:����
    uint32 dwTepTime;
    boolean bSetTimer = FALSE;  //ָʾ�Ƿ��Ѿ������˿�������
    JulianType  jDate;
#ifdef FEATURE_UIALARM
    IAlarm *pAlarm = NULL;
#endif

    debug("Clock_UpdateSt");

    if (pMe == NULL)
    {
        return ;
    }

    //��ʾ�Ƿ���������
    if (pMe->m_ClockCfg.bStateOn[eAlarmType])     //�����Ѿ������˿���
    {
        debug("pMe->m_ClockCfg.bStateOn[eAlarmType]%d",pMe->m_ClockCfg.bStateOn[eAlarmType]);
        bSetTimer = TRUE;
    }

#ifdef FEATURE_UIALARM
    if (SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_UIALARM,
        (void**)&pAlarm))
    {
        return;
    }else
    {
    }
#endif

    if (!bSetTimer) // ȡ����ʱ��
    {
#ifdef FEATURE_UIALARM
        (void)IAlarm_CancelAlarm(pAlarm,
                                      AEECLSID_ALARMCLOCK,
                                      (uint16)eAlarmType);
        IAlarm_Release(pAlarm);
#else
        (void) ISHELL_CancelAlarm(pMe->m_pShell,
                                  AEECLSID_ALARMCLOCK,
                                  (uint16)eAlarmType);
#endif

        return;
    }


    //����ʵ�ʶ�ʱʱ��,������AEEAlarm���ж�ʱ������ʵ�ʹ���
    //ȡ��ǰʱ��
    dwNow = GETTIMESECONDS();
    GETJULIANDATE(dwNow, &jDate);

    // ȡ�û����õ�����ʱ�� ��λ:����
    dwAlarmTime = pMe->m_ClockCfg.dwWATime[eAlarmType];
    // ��ʱ��ת��Ϊ��
#if !defined( FEATURE_ONCE_ALARM)
	dwAlarmTime = dwAlarmTime / 1000;
#endif
    //ֻȡjDate������ֵ
    jDate.wHour = 0;
    jDate.wMinute = 0;
    jDate.wSecond = 0;

    testtime = JULIANTOSECONDS(&jDate);


#if defined( FEATURE_ONCE_ALARM)
	if( pMe->m_ClockCfg.RepMode[eAlarmType] == WEEK_ALARM_REP11)
	{
		dwTepTime = dwAlarmTime;
	}
	else
	{
		dwTepTime = dwAlarmTime + testtime;
	}
#else
	dwTepTime = dwAlarmTime + testtime;
#endif

    if (dwTepTime > dwNow)
    {
        // ��������ʱ��δ��
        dwTepTime -= dwNow;
	}
	else
	{
		// ��������ʱ���ѹ����趨Ϊ����
#if defined( FEATURE_ONCE_ALARM)
		if( pMe->m_ClockCfg.RepMode[eAlarmType] == WEEK_ALARM_REP11)
		{
			pMe->m_ClockCfg.bStateOn[eAlarmType] = FALSE;

#ifdef FEATURE_UIALARM
			IAlarm_Release(pAlarm);
#endif
			return;
		}
		else
#endif
		{
			dwTepTime = dwTepTime + 24 * 60 * 60 - dwNow;
		}
    }

    if (dwTepTime == 0)
    {
        dwTepTime = 1;
    }



#ifdef FEATURE_UIALARM
    //��������ʱ�估����
    (void) IAlarm_SetAlarm(pAlarm,
                            AEECLSID_ALARMCLOCK,
                            (uint16)eAlarmType,
                            dwTepTime);

    debug("dwTepTime = %d;eAlarmType = %d",(dwTepTime),eAlarmType);
    IAlarm_Release(pAlarm);

#else
    (void) ISHELL_SetAlarm(pMe->m_pShell,
                            AEECLSID_ALARMCLOCK,
                            (uint16)eAlarmType,
                            dwTepTime);
#endif

    debug("Clock_UpdateEnd");
}


/*==============================================================================
������
       CClockApps_DeActivate
˵����
       ��ʱ��������������AlarmClock Applet ���� EVT_ALARMOFF �¼�����
       EVT_ALARMOFF �¼�ֹͣ������ʾ��ֹͣ��Ļ��˸��ʾ��
       ȡ��ˢ�½����ȫ����ʱ�����������ر���ʾ���档

������
       pMe [in]��ָ�� AlarmClock Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       ��

��ע��

==============================================================================*/
static void CClockApps_DeActivate(CClockApps *pMe)
{
    if (pMe == NULL)
    {
        return ;
    }

    alarm_time_reach = FALSE;
    ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)CClockApps_DeActivate, pMe);

    pMe->m_bAlarmOff = TRUE;
    CLOSE_DIALOG(DLGRET_CANCELED)
}

/*==============================================================================
������
       CClockApps_Snooze
˵����
       ��ʱ��������������AlarmClock Applet ���� EVT_ALARMOFF �¼�����
       EVT_ALARMOFF �¼�ֹͣ������ʾ��ֹͣ��Ļ��˸��ʾ��
       ȡ��ˢ�½����ȫ����ʱ�����������ر���ʾ���档

������
       pMe [in]��ָ�� AlarmClock Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       ��

��ע��

==============================================================================*/
void CClockApps_Snooze(CClockApps *pMe)
{
    uint32 snoozetime;
    uint32 dwnow;
#ifdef FEATURE_UIALARM
    IAlarm *pAlarm = NULL;
#endif    
    
    if (pMe == NULL)
    {
        return ;
    }
    
    (void) ISHELL_GetPrefs(pMe->m_pShell,
                           AEECLSID_ALARMCLOCK,
                           CLOCK_CFG_VERSION,
                           &pMe->m_ClockCfg,
                           sizeof(ClockAppCfg));    
    ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)CClockApps_Snooze, pMe);
    if(!pMe->m_bSuspending)
    {
         if(pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType] == 0)
        {
            CClockApps_DeActivate( pMe);
            return ;        
        }
        else
        {
            ISHELL_CancelTimer(pMe->m_pShell, (PFNNOTIFY)CClockApps_DeActivate, pMe);
        }  
    }
    
#ifdef FEATURE_UIALARM
    if (SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                        AEECLSID_UIALARM,
                                        (void**)&pAlarm))
    {
        return;
    }
#endif
    dwnow = GETTIMESECONDS();
    
//#if defined(FEATURE_PROJECT_M8) || defined(FEATURE_PROJECT_SMART)
#if defined(FEATURE_SNOOZETIME)
	snoozetime = dwnow%60;
	snoozetime = pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType] - snoozetime;
#else
	snoozetime = dwnow%60;
    snoozetime = 60 - snoozetime;
    snoozetime = snoozetime +  pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType];
#endif    

#ifdef FEATURE_UIALARM
    //��������ʱ�估����
    (void) IAlarm_SetAlarm(pAlarm,
                            AEECLSID_ALARMCLOCK,
                            (uint16)pMe->m_eCurAlarmType,
                            snoozetime);

    IAlarm_Release(pAlarm);
    
#else
    (void) ISHELL_SetAlarm(pMe->m_pShell,
                            AEECLSID_ALARMCLOCK,
                            (uint16)pMe->m_eCurAlarmType,
                            snoozetime);
#endif


    ERR("YKMILAN COME HERE %d   cfg time %d",snoozetime,pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType],NULL);
    CLOSE_DIALOG(DLGRET_CANCELED)
    
}

/*==============================================================================
������
       CClockApps_AniClockImg
˵����
       ��ʱ�������������򻭻��clock ICON

������
       pMe [in]��ָ�� AlarmClock Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       ��

��ע��

==============================================================================*/
static void CClockApps_AniClockImg(CClockApps *pMe)
{

    //AEERect rect = {0};
    Appscommon_ResetBackgroundEx(pMe->m_pDisplay, &pMe->m_rc, TRUE);
    //IDISPLAY_FillRect( pMe->m_pDisplay, &rect, RGB_BLACK);

    //Draw title bar and bottom bar
    {
        TitleBar_Param_type title;
        AECHAR wszTitle[16] = {0};
        ISHELL_LoadResString( pMe->m_pShell, AEE_CLOCKAPPS_RES_FILE,IDS_ALARMCLOCK_1 + pMe->m_eCurAlarmType, wszTitle, sizeof(wszTitle));
        MEMSET(&title,0,sizeof(TitleBar_Param_type));
        //title.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER;
        title.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
        title.pwszTitle = wszTitle;
		#if 0
        DrawTitleBar(pMe->m_pDisplay,&title);
		#else
        if(pMe->m_pIAnn != NULL)
        {
		    IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,wszTitle);
        }
		#endif
        (void) ISHELL_GetPrefs(pMe->m_pShell,
                               AEECLSID_ALARMCLOCK,
                               CLOCK_CFG_VERSION,
                               &pMe->m_ClockCfg,
                               sizeof(ClockAppCfg));
        if(0 == pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType])
        {
            CLOCK_DRAW_BOTTOMBAR(BTBAR_STOP);        
        }
        else
        {    
            CLOCK_DRAW_BOTTOMBAR(BTBAR_SNOOZE_STOP);
        }
    }
    //Draw the picture
    {
		IImage* pResImg = ISHELL_LoadResImage( pMe->m_pShell, CLOCKAPPS_IMAGES_RES_FILE, IDI_ALARMCLOCK);
        
        if( pResImg != NULL)
        {
            AEEImageInfo ImageInfo;
            
            IIMAGE_GetInfo(pResImg, &ImageInfo);
            IIMAGE_Draw( pResImg, (pMe->m_rc.dx - ImageInfo.cx)/2, TITLEBAR_HEIGHT + (pMe->m_rc.dy - BOTTOMBAR_HEIGHT - ImageInfo.cy)/2);
            //IIMAGE_Draw( pResImg, 0, GetTitleBarHeight( pMe->m_pDisplay));
            IIMAGE_Release( pResImg);
            pResImg = NULL;
        }
    }
    {
        AEERect     rc = {0};
        AECHAR      wszTime[16] = {0};
        RGBVAL      nOldFontColor;
        
        SETAEERECT(&rc, 0, TITLEBAR_HEIGHT, pMe->m_rc.dx, TITLEBAR_HEIGHT);
        Appscommon_FormatTimeString(GETTIMESECONDS(), wszTime, sizeof(wszTime));
        
        nOldFontColor = IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
        IDISPLAY_DrawText(pMe->m_pDisplay, AEE_FONT_NORMAL, wszTime, -1, 0, 0, 
                            &rc, IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_TEXT_TRANSPARENT);
        (void)IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, nOldFontColor);
    }
    IDISPLAY_Update( pMe->m_pDisplay);
}


/*==============================================================================
������
       CClockApps_HandleKeyEvent

˵����
       ����Clock�༭����Ӧ�����Ҽ��л��ؼ�.

������
       pMe [in]��ָ��Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       wParam[in]: ��������

����ֵ��
       boolean��

��ע��:

==============================================================================*/
static boolean CClockApps_HandleKeyEvent(CClockApps *pMe, uint16 wParam)
{
    if (pMe == NULL)
    {
        return FALSE;
    }
    debug("%x, %d ,%d,CClockApps_HandleKeyEvent",wParam,pMe->m_nCtlID,pMe->m_nTimeCtlCount);
    switch(wParam)
    {
        case AVK_DOWN:
            if(pMe->m_nCtlID == IDC_CLOCK_ST_LIST)     //�ؼ�Ϊ״̬,�л����ظ���ʽ
            {
                pMe->m_nCtlID = IDC_CLOCK_TIME;
            }
            else if(pMe->m_nCtlID == IDC_REP_MOD_LIST) //�ظ���ʽ
            {
                //ʱ��ؼ�����ʾ����,��activeʱ����
                ITIMECTL_SetProperties(pMe->m_pTime,
                                       TP_NO_SECONDS | TP_AUTOREDRAW);
                //�༭��������Ϊ����
                ITIMECTL_SetEditField(pMe->m_pTime, ITF_HOUR);

                //pMe->m_nCtlID = IDC_CLOCK_ST_LIST;
                pMe->m_nCtlID = IDC_CLOCK_SNOOZE;
                //pMe->m_nTimeCtlCount = 0;  //0��������Сʱ��,1�����ڷ�����
                //pMe->m_nCtlID--;
            }
            else if(pMe->m_nCtlID == IDC_CLOCK_TIME)/*down*/   //ʱ������
            {
                pMe->m_nCtlID = IDC_REP_MOD_LIST;
                pMe->m_nNumKeyCount = 0;    //���ñ�־
                pMe->m_nTimeCtlCount = 0;
                //if(pMe->m_nTimeCtlCount == 1)  //�����ڷ�����,�л����㵽״̬
                //{
                //    pMe->m_nCtlID = IDC_REP_MOD_LIST;
                //    pMe->m_nNumKeyCount = 0;    //���ñ�־
                //    pMe->m_nTimeCtlCount = 0;
                //}
                //else        //������Сʱ��,�л����㵽����
                //{
                //    pMe->m_nTimeCtlCount = 1;
                //    pMe->m_nNumKeyCount = 2;    //�༭Сʱ��ʮλ
                //}
            }
            else if(pMe->m_nCtlID == IDC_CLOCK_SNOOZE)
            {
            	pMe->m_nCtlID = IDC_CLOCK_ST_LIST;
            }
            else
            {
                pMe->m_nCtlID--;
            }
#if 0
            //���ÿؼ�����
            if (pMe->m_nCtlID == IDC_CLOCK_ST_LIST)
            {
                IMENUCTL_SetActive(pMe->m_pState, TRUE);
                ITIMECTL_SetActive(pMe->m_pTime, FALSE);
                IMENUCTL_SetActive(pMe->m_pRepMode, FALSE);
            }
            else if (pMe->m_nCtlID == IDC_CLOCK_TIME)
            {
                IMENUCTL_SetActive(pMe->m_pState, FALSE);
                ITIMECTL_SetActive(pMe->m_pTime, TRUE);
                IMENUCTL_SetActive(pMe->m_pRepMode, FALSE);
            }
            else
            {
                IMENUCTL_SetActive(pMe->m_pState, FALSE);
                ITIMECTL_SetActive(pMe->m_pTime, FALSE);
                IMENUCTL_SetActive(pMe->m_pRepMode, TRUE);
            }
#else
            CClockApps_Set_CTL(pMe);
#endif
            return TRUE;

        case AVK_UP:
            if(pMe->m_nCtlID == IDC_REP_MOD_LIST) //�ظ���ʽ�ؼ�,�л���״̬
            {
                pMe->m_nCtlID = IDC_CLOCK_TIME;
            }
            else if(pMe->m_nCtlID == IDC_CLOCK_ST_LIST)    //״̬
            {
                //ʱ��ؼ�����ʾ����,��activeʱ����
                ITIMECTL_SetProperties(pMe->m_pTime,
                                       TP_NO_SECONDS | TP_AUTOREDRAW);
                //�༭��������ΪСʱ
                ITIMECTL_SetEditField(pMe->m_pTime, ITF_HOUR);

                //pMe->m_nCtlID = IDC_REP_MOD_LIST;
                pMe->m_nCtlID = IDC_CLOCK_SNOOZE;
                //pMe->m_nTimeCtlCount = 0;  //0��������Сʱ��,1�����ڷ�����
                //pMe->m_nCtlID++;
            }
            else if(pMe->m_nCtlID == IDC_CLOCK_TIME)/*up*/   //ʱ������
            {
                pMe->m_nCtlID = IDC_CLOCK_ST_LIST;
                pMe->m_nNumKeyCount = 0;    //���ñ�־
                pMe->m_nTimeCtlCount  = 0;
                //(void)ITIMECTL_SetTime(pMe->m_pTime, pMe->m_dwDispTime);
                //if(pMe->m_nTimeCtlCount == 0)  //������Сʱ��,�л����㵽�ظ���ʽ
                //{
                //    pMe->m_nCtlID = IDC_CLOCK_ST_LIST;
                //    pMe->m_nNumKeyCount = 0;    //���ñ�־
                //    pMe->m_nTimeCtlCount  = 0;
                //}
                //else        //�����ڷ�����,�л����㵽Сʱ
                //{
                //    pMe->m_nTimeCtlCount = 0;
                //    pMe->m_nNumKeyCount = 0;//�༭Сʱ��ʮλ
                //}
            }
            else if(pMe->m_nCtlID == IDC_CLOCK_SNOOZE)
            {
                pMe->m_nCtlID = IDC_REP_MOD_LIST;   
            }
            else
            {
                pMe->m_nCtlID++;
            }
#if 0
            //���ÿؼ�����
            if (pMe->m_nCtlID == IDC_CLOCK_ST_LIST)
            {
                IMENUCTL_SetActive(pMe->m_pState, TRUE);
                ITIMECTL_SetActive(pMe->m_pTime, FALSE);
                IMENUCTL_SetActive(pMe->m_pRepMode, FALSE);
            }
            else if (pMe->m_nCtlID == IDC_CLOCK_TIME)
            {
                IMENUCTL_SetActive(pMe->m_pState, FALSE);
                ITIMECTL_SetActive(pMe->m_pTime, TRUE);
                IMENUCTL_SetActive(pMe->m_pRepMode, FALSE);
            }
            else
            {
                IMENUCTL_SetActive(pMe->m_pState, FALSE);
                ITIMECTL_SetActive(pMe->m_pTime, FALSE);
                IMENUCTL_SetActive(pMe->m_pRepMode, TRUE);
            }
#else
            CClockApps_Set_CTL(pMe);
#endif
            return TRUE;

        case AVK_LEFT:
        case AVK_RIGHT:
            debug("IDC_CLOCK_TIME %d %d %d",pMe->m_nCtlID,pMe->m_nTimeCtlCount,pMe->m_nNumKeyCount);
            //��ʱ��ؼ�����������,���԰����¼������õ�ʱ��
            if(pMe->m_nCtlID == IDC_CLOCK_TIME)   //ʱ��ؼ�
            {
                if(pMe->m_nTimeCtlCount == 0)
                {
                    pMe->m_nTimeCtlCount = 1;
                    pMe->m_nNumKeyCount = 2;
                }
                else if(pMe->m_nTimeCtlCount == 1)
                {
                    pMe->m_nTimeCtlCount = 0;
                    pMe->m_nNumKeyCount = 0;
                }
            }
#if defined( FEATURE_ONCE_ALARM)            
            if(pMe->m_nCtlID == IDC_CLOCK_SNOOZE)
            {
                if(IMENUCTL_GetSel(pMe->m_pRepMode) == ITEM_REP_MODE_11)
                {
                    IMENUCTL_SetSel(pMe->m_pSnooze,ITEM_SNOOZE_4);
                }
            }

            if(pMe->m_nCtlID == IDC_REP_MOD_LIST)
            {
                if(IMENUCTL_GetSel(pMe->m_pRepMode) == ITEM_REP_MODE_11)
                {
                    IMENUCTL_SetSel(pMe->m_pSnooze,ITEM_SNOOZE_4);
                }
            }            
#endif //defined( FEATURE_ONCE_ALARM)         
            return TRUE;

        default:
            break;
    }
    return TRUE;

}//end CClockApps_HandleKeyEvent
/*==============================================================================
������
       CClockApps_HandleCmdEvent

˵����
       ����Clock�༭����Ӧ��COMMAND����SELECT��������������.

������
       pMe [in]��ָ��Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��


����ֵ��
       boolean��

��ע��:

==============================================================================*/
static boolean CClockApps_HandleCmdEvent(CClockApps *pMe)
{
    if (pMe == NULL)
    {
        return FALSE;
    }

    //����STATE��ֵ
    if(IMENUCTL_GetSel(pMe->m_pState) == ITEM_STATE_ON)
    {
        pMe->m_ClockCfg.bStateOn[pMe->m_eCurAlarmType] = TRUE;
    }
    else
    {
        pMe->m_ClockCfg.bStateOn[pMe->m_eCurAlarmType] = FALSE;
    }

    //�����������õ�ʱ��
    {
        int32 dwMs = ITIMECTL_GetTime(pMe->m_pTime);

        dwMs -= (dwMs % 60000);
#if defined( FEATURE_ONCE_ALARM)
		pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] = dwMs / 1000;
#else
		pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] = dwMs;
#endif
    }
    //pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] = ITIMECTL_GetTime(pMe->m_pTime);
    //�����ظ���ʽ��ֵ
    switch(IMENUCTL_GetSel(pMe->m_pRepMode))
    {
        default:
        case ITEM_REP_MODE_1:   //ÿ��
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP1;
            break;
        case ITEM_REP_MODE_2:   //��һ~����
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP2;
            break;
        case ITEM_REP_MODE_3:   //��һ~����
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP3;
            break;
        case ITEM_REP_MODE_4:   //��һ
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP4;
            break;
        case ITEM_REP_MODE_5:   //�ܶ�
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP5;
            break;
        case ITEM_REP_MODE_6:   //����
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP6;
            break;
        case ITEM_REP_MODE_7:   //����
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP7;
            break;
        case ITEM_REP_MODE_8:   //����
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP8;
            break;
        case ITEM_REP_MODE_9:   //����
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP9;
            break;
        case ITEM_REP_MODE_10:   //����
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP10;
            break;

#if defined( FEATURE_ONCE_ALARM)
        case ITEM_REP_MODE_11:   //����
            pMe->m_ClockCfg.RepMode[pMe->m_eCurAlarmType] = WEEK_ALARM_REP11;

            {
                uint32      now     = GETTIMESECONDS();
                uint32      today   = 0;
                JulianType  jdate   = {0};

                GETJULIANDATE( now, &jdate);
                jdate.wHour = jdate.wMinute = jdate.wSecond = 0;
                today = JULIANTOSECONDS( &jdate);

                pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] += today;
                if( pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] < now)
                {
                    pMe->m_ClockCfg.dwWATime[pMe->m_eCurAlarmType] += 24*60*60;
                }
            }
            break;
#endif
    }
    switch(IMENUCTL_GetSel(pMe->m_pSnooze))
    {
        default:
        case ITEM_SNOOZE_1:   //5���� 
            pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType] = 5*60;
            break;
            
        case ITEM_SNOOZE_2:   //10����
            pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType] = 10*60;
            break;
            
        case ITEM_SNOOZE_3:   //15����
            pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType] = 15*60;
            break;

        case ITEM_SNOOZE_4:   //�ر�
            pMe->m_ClockCfg.Snooze[pMe->m_eCurAlarmType] = 0;
            break;
            
    }

    return TRUE;
}//end CClockApps_HandleCmdEvent
#ifdef FEATURE_SUB_LCD
/*==============================================================================
������
       CClockApps_DispalySub

˵����
       ���ǹر�ʱ��С���ϻ�����ʾ��Ϣ.

������
       pMe [in]��ָ��Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       subTitle [in]: ����buf��ָ��
       subTime [in]: ʱ��buf��ָ��
       subMesssgeBuf [in]: ��ʾ��Ϣbufָ��
����ֵ��
       void

��ע��:

==============================================================================*/
static void CClockApps_DispalySub(CClockApps *pMe,
    AECHAR *subTitle,
    AECHAR *subTime,
    AECHAR *subMesssge
)
{
    int     totalCh;
    AECHAR  subTmpBuf[MAX_ALARM_INFO];
    AEEDeviceInfo   di;
    AEERect         RectSub;
    char    Subwallstring[LENTH_OF_WALLPAPER_NAME];

    if (pMe == NULL)
    {
        return;
    }

    MEMSET(subTmpBuf,0,sizeof(subTmpBuf));

    ISHELL_GetDeviceInfo(pMe->m_pShell, &di);

    SETAEERECT(&RectSub, 0, 0, di.cxAltScreen,
               HEIGHT_OF_FONT);

    IDISPLAY_ClearScreen(pMe->m_pDisplaySub);

    (void) ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_SUBWALL,
                          Subwallstring,
                          sizeof(Subwallstring));

    if('\0' != Subwallstring[0])
    {
        if(pMe->m_pWallSubImage)
        {
            IBITMAP_Release(pMe->m_pWallSubImage);
            pMe->m_pWallSubImage = NULL;
        }
        pMe->m_pWallSubImage = ISHELL_LoadBitmap(pMe->m_pShell,Subwallstring);
    }

    if(NULL == pMe->m_pWallSubImage)
    {
        pMe->m_pWallSubImage = ISHELL_LoadBitmap(pMe->m_pShell,
                                        "image\\wallpaper\\s1.bmp");
    }

    if (pMe->m_pWallSubImage)
    {
        IDISPLAY_BitBlt(pMe->m_pDisplaySub, 0, 0,
                           di.cxAltScreen, di.cyAltScreen,
                           pMe->m_pWallSubImage, 0, 0, AEE_RO_COPY);
    }

    //С������Title
    (void) IDISPLAY_DrawText(pMe->m_pDisplaySub,
                AEE_FONT_BOLD, subTitle, AECHAR_OF_STATIC_INFO,
                3, 0, &RectSub,
                IDF_ALIGN_TOP |IDF_TEXT_TRANSPARENT);

    //С������ʱ��
    (void) IDISPLAY_DrawText(pMe->m_pDisplaySub,
                AEE_FONT_BOLD, subTime, AECHAR_OF_STATIC_INFO,
                di.cxAltScreen/2+3, 0, &RectSub,
                IDF_ALIGN_TOP |IDF_TEXT_TRANSPARENT);

    //С��������ʾ��Ϣ
    RectSub.x = 0;
    RectSub.y = HEIGHT_OF_FONT;
    RectSub.dx = di.cxAltScreen;
    RectSub.dy = di.cyAltScreen - HEIGHT_OF_FONT;

    totalCh = WSTRLEN(subMesssge);  //��Ҫ�����ַ����ܳ��ȣ���ȷ���Ƿ�Ҫ���С�

    if(totalCh > 6)     //��������,����һ����໭6���ַ�
    {
        WSTRNCOPYN(subTmpBuf, MAX_ALARM_INFO, subMesssge, 6);
        (void) IDISPLAY_DrawText(pMe->m_pDisplaySub,
                    AEE_FONT_NORMAL, subTmpBuf, AECHAR_OF_STATIC_INFO,
                    0, RectSub.y+3, &RectSub,
                    IDF_ALIGN_CENTER |IDF_TEXT_TRANSPARENT);

        subMesssge = subMesssge + 6;    //ָ���Ƶ�ʣ�µ�2����1���ַ�
        MEMSET(subTmpBuf,0,sizeof(subTmpBuf));
        WSTRNCOPYN(subTmpBuf, MAX_ALARM_INFO, subMesssge, 2);
        (void) IDISPLAY_DrawText(pMe->m_pDisplaySub,
                    AEE_FONT_NORMAL, subTmpBuf, AECHAR_OF_STATIC_INFO,
                    0, RectSub.y+3+HEIGHT_OF_FONT, &RectSub,
                    IDF_ALIGN_CENTER |IDF_TEXT_TRANSPARENT);

    }
    else
    {
        (void) IDISPLAY_DrawText(pMe->m_pDisplaySub,
                    AEE_FONT_NORMAL, subMesssge, AECHAR_OF_STATIC_INFO,
                    0, RectSub.y+3, &RectSub,
                    IDF_ALIGN_CENTER |IDF_TEXT_TRANSPARENT);
    }

    //ˢ��С��
    IDISPLAY_UpdateEx(pMe->m_pDisplaySub, FALSE);
}
#endif //FEATURE_SUB_LCD


/*==============================================================================
������
       CClockApps_SaveAlertTimeout

˵����
       ������ʾ�Ի�����Ϻ�Ļص�

������
       pme [in]��void

����ֵ��
       void

��ע��:

==============================================================================*/
static void CClockApps_SaveAlertTimeout(void *pme)
{
    CClockApps *pMe = (CClockApps *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent(pMe->m_pShell,
                            AEECLSID_ALARMCLOCK,
                            EVT_DISPLAYDIALOGTIMEOUT,
                            0,
                            0);
}

/*==============================================================================
������
       CClockApps_DrawSaveAlert

˵����
       ��������ʾ��Ϣ

������
       pMe [in]��ָ��Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       void

��ע��:

==============================================================================*/
static void CClockApps_DrawSaveAlert(CClockApps *pMe)
{
#if 1
    PromptMsg_Param_type m_PromptMsg;
    AEERect rect = {0};
    ISTATIC_SetRect(pMe->m_pStatic, &rect);
    debug("%x CClockApps_DrawSaveAlert",pMe->m_pStatic);
    MEMSET(&m_PromptMsg,0,sizeof(PromptMsg_Param_type));
    m_PromptMsg.nMsgResID= IDS_CLOCK_SAVE_ALERT;
    m_PromptMsg.ePMsgType = MESSAGE_WARNNING;
    STRLCPY(m_PromptMsg.strMsgResFile, AEE_CLOCKAPPS_RES_FILE,MAX_FILE_NAME);
    m_PromptMsg.eBBarType = BTBAR_BACK;
    DrawPromptMessage(pMe->m_pDisplay,pMe->m_pStatic,&m_PromptMsg);
    IDISPLAY_UpdateEx(pMe->m_pDisplay,FALSE);
    ISHELL_SetTimer(pMe->m_pShell,CLOCK_SAVE_ALERT_MSECS, CClockApps_SaveAlertTimeout, pMe);
#else
    int         nFontHeight;
    AEERect     cRect;
    AECHAR      wideBuf[24];     //��ʱ�洢�ַ���buf

    if (NULL == pMe)
    {
        return;
    }

    MEMSET(wideBuf,0,sizeof(wideBuf));

    nFontHeight = IDISPLAY_GetFontMetrics( pMe->m_pDisplay,
                                           AEE_FONT_BOLD,
                                           NULL,
                                           NULL);
    cRect.x = SAVE_ALERT_X;
    cRect.y = pMe->m_rc.dy /2;
    cRect.dx = pMe->m_rc.dx - 2*SAVE_ALERT_X;
    cRect.dy = 2*nFontHeight;

    IDISPLAY_EraseRect(pMe->m_pDisplay, &cRect);

    //��ʾ�ı�
    (void) ISHELL_LoadResString(pMe->m_pShell,
                                AEE_CLOCKAPPS_RES_FILE,
                                IDS_CLOCK_SAVE_ALERT,
                                wideBuf,
                                sizeof(wideBuf));

    (void)IDISPLAY_DrawText(pMe->m_pDisplay,
                            AEE_FONT_BOLD,
                            wideBuf,
                            sizeof(wideBuf),
                            0,
                            0,
                            &cRect,
                            IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER);

    IDISPLAY_DrawRect(pMe->m_pDisplay,
                      &cRect,
                      MAKE_RGB(0,17,247),
                      MAKE_RGB(255,255,255),
                      IDF_RECT_FRAME);
    cRect.x +=  1;
    cRect.y +=  1;
    cRect.dx -= 2;
    cRect.dy -= 2;

    IDISPLAY_DrawRect(pMe->m_pDisplay,
                      &cRect,
                      MAKE_RGB(62,165,253),
                      MAKE_RGB(255,255,255),
                      IDF_RECT_FRAME);

    cRect.x +=  1;
    cRect.y +=  1;
    cRect.dx -= 2;
    cRect.dy -= 2;

    IDISPLAY_DrawRect(pMe->m_pDisplay,
                      &cRect,
                      MAKE_RGB(169,213,250),
                      MAKE_RGB(255,255,255),
                      IDF_RECT_FRAME);

    // ͳһ���½���
    IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);

    //���ñ�����ʾ��Ϣ��ʱ��
    (void) ISHELL_SetTimer(pMe->m_pShell,
                          CLOCK_SAVE_ALERT_MSECS,
                          CClockApps_SaveAlertTimeout,
                          pMe);

#endif
}

static void CClockApps_Set_CTL(CClockApps *pMe)
{

    int currentRectIndex = 0;
    int i                = 0;

    AEERect            rect        = {0};
    Theme_Param_type   themeParms  = {0};
    Appscom_GetThemeParameters( &themeParms);

    //���ÿؼ�����
    if (pMe->m_nCtlID == IDC_CLOCK_ST_LIST)
    {
        currentRectIndex = 0;

        IMENUCTL_SetActive(pMe->m_pState, TRUE);
        ITIMECTL_SetActive(pMe->m_pTime, FALSE);
        IMENUCTL_SetActive(pMe->m_pRepMode, FALSE);
        IMENUCTL_SetActive(pMe->m_pSnooze,FALSE);
    }
    else if (pMe->m_nCtlID == IDC_CLOCK_TIME)
    {
        currentRectIndex = 1;

        IMENUCTL_SetActive(pMe->m_pState, FALSE);
        ITIMECTL_SetActive(pMe->m_pTime, TRUE);
        IMENUCTL_SetActive(pMe->m_pRepMode, FALSE);
        IMENUCTL_SetActive(pMe->m_pSnooze, FALSE);
    }
    else if (pMe->m_nCtlID == IDC_REP_MOD_LIST)
    {
        currentRectIndex = 2;

        IMENUCTL_SetActive(pMe->m_pState, FALSE);
        ITIMECTL_SetActive(pMe->m_pTime, FALSE);
        IMENUCTL_SetActive(pMe->m_pRepMode, TRUE);
        IMENUCTL_SetActive(pMe->m_pSnooze, FALSE);
    }
    else
    {
        currentRectIndex = 3;
        IMENUCTL_SetActive(pMe->m_pState, FALSE);
        ITIMECTL_SetActive(pMe->m_pTime, FALSE);
        IMENUCTL_SetActive(pMe->m_pRepMode, FALSE);
        IMENUCTL_SetActive(pMe->m_pSnooze, TRUE);
    }       

    for( i = 0; i < 4; i ++)
    {
        int offset[] = { 8, 0, 8, 8};
        SETAEERECT( &rect,
                pMe->rectLine[i].x - 1 - offset[i],
                pMe->rectLine[i].y - 1,
                pMe->rectLine[i].dx + 2 + offset[i] * 2,
                pMe->rectLine[i].dy + 2
            );
        IDISPLAY_DrawRect( pMe->m_pDisplay,
                           &rect,
                           i == currentRectIndex ? themeParms.themeColor : 0,
                           0,
                           IDF_RECT_FRAME
                       );
    }
}

static void CClockApps_Draw_Arrow(CClockApps *pMe, AEERect* pRect,int i)
{
	IImage      *pR_ResImg = NULL;
	IImage      *pL_ResImg = NULL;
	AEEImageInfo  imageInfo  = {0};

	pL_ResImg  = ISHELL_LoadResImage(pMe->m_pShell,AEE_APPSCOMMONRES_IMAGESFILE,IDB_LEFTARROW);
	pR_ResImg = ISHELL_LoadResImage(pMe->m_pShell,AEE_APPSCOMMONRES_IMAGESFILE,IDB_RIGHTARROW);

    if(pR_ResImg != NULL)
    {
        IIMAGE_GetInfo( pR_ResImg, &imageInfo);
        IIMAGE_Draw( pR_ResImg, pRect->x + pRect->dx, pRect->y + (pRect->dy - imageInfo.cy) / 2);
        IIMAGE_Release(pR_ResImg);
    }

    if(pL_ResImg != NULL)
    {
        IIMAGE_GetInfo( pL_ResImg, &imageInfo);
        IIMAGE_Draw( pL_ResImg, pRect->x - imageInfo.cx, pRect->y + (pRect->dy - imageInfo.cy) / 2);
        IIMAGE_Release(pL_ResImg);
    }
	#ifdef FEATURE_LCD_TOUCH_ENABLE
	if(i==0)
	{
		SETAEERECT(&pMe->pL_Rect1, pRect->x - imageInfo.cx, pRect->y + (pRect->dy - imageInfo.cy) / 2, imageInfo.cx, imageInfo.cy);
		SETAEERECT(&pMe->pR_Rect1, pRect->x + pRect->dx, pRect->y + (pRect->dy - imageInfo.cy) / 2, imageInfo.cx, imageInfo.cy);
	}
	else if(i == 2)
	{
		SETAEERECT(&pMe->pL_Rect2, pRect->x - imageInfo.cx, pRect->y + (pRect->dy - imageInfo.cy) / 2, imageInfo.cx, imageInfo.cy);
		SETAEERECT(&pMe->pR_Rect2, pRect->x + pRect->dx, pRect->y + (pRect->dy - imageInfo.cy) / 2, imageInfo.cx, imageInfo.cy);
	}
	else if(i == 3)
	{
		SETAEERECT(&pMe->pL_Rect3, pRect->x - imageInfo.cx, pRect->y + (pRect->dy - imageInfo.cy) / 2, imageInfo.cx, imageInfo.cy);
		SETAEERECT(&pMe->pR_Rect3, pRect->x + pRect->dx, pRect->y + (pRect->dy - imageInfo.cy) / 2, imageInfo.cx, imageInfo.cy);
	}
	#endif

}
static void ClockNotifyMP3PlayerAlertEvent(CClockApps *pMe, boolean toStartAlert)
{
 #ifdef FEATURE_APP_MUSICPLAYER
    if(GetMp3PlayerStatus() == MP3STATUS_RUNONBACKGROUND)
    {
        ISHELL_SendEvent(pMe->m_pShell, 
                         AEECLSID_APP_MUSICPLAYER,
                         EVT_ALARM,
                         toStartAlert,
                         0);
    }
 #endif
}
