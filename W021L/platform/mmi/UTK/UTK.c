/*==============================================================================
// �ļ���
//        UTK.c
//        2007-11-13 ��ϲ�����汾(Draft Version)
//        ��Ȩ����(c) 2004 Anylook
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
#include "UTK_priv.h"
//#include "AEEPrompt.h"
#include "UTKUtils.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
#define EVT_DISPLAYOUTAPPMSG (EVT_USER+20)

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------ģ����غ�������---------------------*/
/*----------------------Applet��غ�������---------------------*/
int UTK_Load(IShell *ps, void * pHelpers, IModule ** pMod);
#if defined(AEE_STATIC)
static int UTK_CreateInstance(AEECLSID    ClsId,
                              IShell     *pIShell,
                              IModule    *po,
                              void      **ppObj);
#endif

static int UTK_New(IShell *ps, IModule *pIModule, void **ppObj);

static uint32  UTK_AddRef(IUTK *pi);

static uint32  UTK_Release (IUTK *pi);

static boolean UTK_HandleEvent(IUTK *pi,
                               AEEEvent eCode,
                               uint16  wParam,
                               uint32 dwParam);
static int UTK_InitAppData(CUTK *pMe);

static void UTK_FreeAppData(CUTK *pMe);

static void UTK_RunFSM(CUTK *pMe);

static void UTK_MAKE_UTK_CALL(CUTK *pMe);

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
#if defined(AEE_STATIC)  
#define UTK_METHODS           \
        {                     \
            UTK_AddRef,       \
            UTK_Release,      \
            UTK_HandleEvent   \
        }
        
// ֻ����һ��UTKʵ����ÿ�δ���UTK Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CUTK gUTK;

static const VTBL(IUTK) gUTKMethods = UTK_METHODS;
#endif

/*----------------------ģ����غ�������---------------------*/


/*----------------------Applet��غ�������---------------------*/
//===========================================================================
//
// DESCRIPTION
//		Standard function called by BREW to initialize the static module.
//
// Return Value:
//	   0 - Success, non-0 on failure
//	
//===========================================================================
#if defined(AEE_STATIC)
int UTK_Load(IShell *ps, void * pHelpers, IModule ** pMod)
{
   return(AEEStaticMod_New((int16)(sizeof(AEEMod)),ps,pHelpers,pMod,(PFNMODCREATEINST)UTK_CreateInstance,(PFNFREEMODDATA)NULL));
}
#endif


//===========================================================================
// 
// DESCRIPTION
//		This function is invoked while the app is being loaded. All Modules must provide this 
// 	function. Ensure to retain the same name and parameters for this function.
//
// PARAMETERS
// 	ClsId: [in]: Specifies the ClassID of the applet which is being loaded
// 	pIShell: [in]: Contains pointer to the IShell interface. 
// 	pIModule: [in]: Contains pointer to the IModule interface to the current module to which
// 	this app belongs
// 	ppObj: [out]: On return, *ppObj must point to a valid AEEApplet structure. Allocation
// 	of memory for this structure and initializing the base data members is done by AEEApplet_New().
//
// Return Value:
//		
//===========================================================================
#if defined(AEE_STATIC)
static int UTK_CreateInstance(AEECLSID    ClsId,
                              IShell     *pIShell,
                              IModule    *po,
                              void      **ppObj)
#else
int AEEClsCreateInstance(AEECLSID ClsId,IShell * pIShell,IModule * po,void ** ppObj)
#endif
{
    if (ClsId == AEECLSID_APP_UTK)
    {
        return UTK_New(pIShell, po, ppObj);
    }
    else
    {
        return EUNSUPPORTED;
    }
}
/*==============================================================================
����: 
       UTK_New
       
˵��: 
        ��ʼ��UTK Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��
       
����: 
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IUTK����ָ���ָ�롣
       
����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���
       
��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�
       
==============================================================================*/
#include "err.h"
static int UTK_New(IShell *ps, IModule *pIModule, void **ppObj)
{
    CUTK     *pMe;
    int retVal = SUCCESS;
    
    if (!ps || !pIModule || !ppObj)
        return EFAILED;

#if defined(AEE_STATIC)
    pMe = &gUTK;    
#else
    pMe = (CUTK *)MALLOC(sizeof(CUTK) + sizeof(VTBL(IUTK)));
    if (!pMe)        
        return EFAILED;
#endif  
    ERR("miaoxiaoming:UTK_New start here",0,0,0);
    if (0 == pMe->m_nRefs)
    {
#if defined(AEE_STATIC)        
        // Must initialize the object
        MEMSET(pMe,  0, sizeof(CUTK));
        INIT_VTBL(pMe, IUTK, gUTKMethods);
#else
//NOTE: In the dynamic applet, the extend classes will re-malloc the memery.
//      So it run in simulator for test only. If you want run in simulator correcttly
//      Please modify the following code.
        VTBL(IUTK) gUTKMethods;
        VTBL(IUTK) *pMod;
        gUTKMethods.AddRef      =  UTK_AddRef;
        gUTKMethods.Release     =  UTK_Release;
        gUTKMethods.HandleEvent =  UTK_HandleEvent;
        
        pMod = (VTBL(IUTK) *)((byte *)pMe + sizeof(CUTK));
        MEMCPY((byte *)pMod, (byte *)&gUTKMethods, sizeof(VTBL(IUTK)));
        INIT_VTBL(pMe, IUTK, *pMod);
#endif  
        pMe->m_nRefs     = 0;
        pMe->m_pShell    = ps;
        pMe->m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = UTK_InitAppData(pMe);
        
        if ( SUCCESS != retVal )
        {
        	UTK_FreeAppData(pMe);
        	return retVal;
        }
    }

    ++pMe->m_nRefs;
    *ppObj = (IUTK*)pMe;
    return retVal;
}

/*==============================================================================
����: 
       UTK_AddRef
       
˵��: 
       UTK Applet ���ü�����1�������ص�ǰ����������
       
����: 
       pi [in]��IUTK�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��UTK Applet��������
       
��ע:
       
==============================================================================*/
static uint32  UTK_AddRef(IUTK *pi)
{
    register CUTK *pMe = (CUTK*)pi;

    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);

    return (++pMe->m_nRefs);
}

/*==============================================================================
����: 
       UTK_Release
       
˵��: 
       �ͷŶ�UTK Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��
       
����: 
       pi [in]��IUTK�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��UTK Applet��������
       
��ע:
       
==============================================================================*/
static uint32  UTK_Release (IUTK *pi)
{
    register CUTK *pMe = (CUTK*)pi;

    ASSERT(pMe != NULL);

    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    // �ͷ�Applet�����Դ
    UTK_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����: 
       UTK_InitAppData
       
˵��: 
       ��ʼ��UTK Applet�ṹ����ȱʡֵ��
       
����: 
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�
       
��ע:
       
==============================================================================*/
static int UTK_InitAppData(CUTK *pMe)
{
    AEEDeviceInfo  di;

    if (NULL  == pMe)
    {
        return EFAILED;
    }
    ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
    pMe->m_rc.x       = 0;
    pMe->m_rc.y       = 0;
    pMe->m_rc.dx      = (int16) di.cxScreen;
    pMe->m_rc.dy      = (int16) di.cyScreen;
    pMe->m_ePreState = UTKST_NONE;
    pMe->m_eCurState = UTKST_INIT;
    pMe->m_pActiveDlg = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    pMe->cmd_type = (uim_tk_proactive_cmd_enum_type)UIM_TK_SETUP_MENU;
    pMe->m_wSelectStore        = MENU_SELECT_NULL;    
    pMe->m_bNormalExit = TRUE;
    
    //��Ϊ����������κ�ʱ����������Ĭ����Ҫ���𱸷� 
    pMe->InputeBackupNeeded = TRUE;
    MEMSET(pMe->bInputeBackup, 0, (MAX_STR_ACHARS+2) * sizeof(AECHAR));
    
    pMe->m_wszInputTitle = NULL;
    pMe->m_wszInputText = NULL;
    pMe->m_TextMaxLen = 0;
    pMe->m_TextMinLen = 0;  
    pMe->m_PlaytoneMsgID = 0;   
    pMe->m_PlaytoneToneID = 0;     
    
    // ������Ҫ�Ľӿ�
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_ALERT,
            (void **)&pMe->m_pAlert) != SUCCESS)
    {
        ERR("Failed to create AEECLSID_ALERT",0,0,0);
        return EFAILED;
    }
    
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_CONFIG,
            (void **)&pMe->m_pConfig) != SUCCESS)
    {
        ERR("Failed to create AEECLSID_CONFIG",0,0,0);
        return EFAILED;
    }
    
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_TELEPHONE ,
            (void **)&pMe->m_pITelephone) != SUCCESS)
    {
        ERR("Failed to create AEECLSID_CALLMANAGER",0,0,0);
        return EFAILED;
    }
     
    if (ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_WMS,
            (void **)&pMe->m_pwms) != SUCCESS)
    {
        ERR("Failed to create AEECLSID_PHONE",0,0,0);
        return EFAILED;
    } 
    
    CALLBACK_Init(&pMe->m_callback, 
                  (PFNNOTIFY) UTK_wmscommandreport,
                  (void*)pMe);
    (void)IWMS_Init(pMe->m_pwms, WMS_CLIENT_TYPE_CAT, &pMe->m_clientId); 
    (void)IWMS_RegisterMsgCb(pMe->m_pwms, pMe->m_clientId, UTK_MsgCb);
    (void)IWMS_Activate(pMe->m_pwms,pMe->m_clientId);
#ifdef FEATURE_FUNCS_THEME   
    {
        // ��ȡ�������
        Theme_Param_type    Theme_Param;
        Appscom_GetThemeParameters(&Theme_Param);
        pMe->m_themeColor = Theme_Param.themeColor;        
    }
#endif /* FEATURE_FUNCS_THEME */    
	if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_ANNUNCIATOR,(void **)&pMe->m_pIAnn))
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����: 
       UTK_FreeAppData
       
˵��: 
       �ͷ�UTK Appletʹ�õ������Դ��
       
����: 
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void UTK_FreeAppData(CUTK *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }
    
    if (NULL != pMe->m_wszInputTitle)
    {
        FREE(pMe->m_wszInputTitle);
        pMe->m_wszInputTitle = NULL;
    }
    
    if (NULL != pMe->m_pAlert)
    {
        IALERT_Release(pMe->m_pAlert);
        pMe->m_pAlert = NULL;
    }
    
    if (NULL != pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }
    
    if (NULL != pMe->m_pITelephone)
    {
        ITELEPHONE_Release(pMe->m_pITelephone);
        pMe->m_pITelephone = NULL;
    }
    
    if (NULL != pMe->m_wszInputText)
    {
        FREE(pMe->m_wszInputText);
        pMe->m_wszInputText = NULL;
    }   

    if (NULL != pMe->m_pwms)
    {
        IWMS_Deactivate(pMe->m_pwms,pMe->m_clientId);
        IWMS_Release(pMe->m_pwms);
        pMe->m_pwms = NULL;
    }
	if(pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn = NULL;
    }
    return;
}

/*==============================================================================
����: 
       UTK_RunFSM
       
˵��: 
       UTK Applet����״̬�����档
       
����: 
       pMe [in]��ָ��UTK Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void UTK_RunFSM(CUTK *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = UTK_ProcessState(pMe);

        if (pMe->m_bNotOverwriteDlgRet)
        {
            pMe->m_bNotOverwriteDlgRet = FALSE;
        }
        else
        {
            pMe->m_eDlgRet = DLGRET_CREATE;
        }

        if (nextFSMAction == NFSMACTION_WAIT)
        {
            break;
        }
    }
}

/*==============================================================================
����: 
       UTK_HandleEvent
       
˵��: 
       UTK  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������
       
����: 
       pi [in]��ָ��IUTK�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�
       
����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������
       
��ע:
       
==============================================================================*/
static boolean UTK_HandleEvent(IUTK *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CUTK *pMe = (CUTK*)pi;
    AEEAppStart *as;
    UTKState  eTempState = UTKST_NONE;
    uim_tk_proactive_cmd_enum_type   old_cmd_type;
    
    // ����ʱ��ʼ��UIM UTK ����ʱ���յ� UIM_TK_SETUP_MENU ����ʱ���Ը��¼�
    //static boolean bIsInitUTK = TRUE;
    //ERR("UTK CODE=%x,wParam = %x",eCode,wParam,0);
    
    switch (eCode)
    {
        case EVT_APP_START:
            ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
            if (NULL != pMe->m_pDisplay)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            {
                AEEDeviceInfo di; 
                
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dy = di.cyScreen;
            }

            pMe->m_bSuspending = FALSE; 
            // ��ʼUTK״̬��
            UTK_RunFSM(pMe);        
            return TRUE;

        case EVT_APP_STOP:
            if (pMe->m_pDisplay != NULL)
            {
                // �ָ�����ɫֵ
                (void)IDISPLAY_SetColor(pMe->m_pDisplay, 
                    CLR_USER_BACKGROUND, pMe->m_dwOldBkColor);

                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }             
            OEMSOUND_Sound_Stop(SND_PRIO_MED);
            IALERT_StopRingerAlert(pMe->m_pAlert);
            IALERT_StopSMSAlert(pMe->m_pAlert); 
            IALERT_StopMp3Alert(pMe->m_pAlert);
            pMe->m_bSuspending = TRUE;
            if (!pMe->m_bNormalExit)
            {// �������˳�
                UTK_GiveResponse(pMe, pMe->cmd_type, FALSE, UIM_TK_PROACTIVE_RUIM_SESSION_TERMINATED_BY_USER);
            } 
            return TRUE;

        case EVT_APP_SUSPEND:
            //����ʱ���ر�UTK
            pMe->m_bSuspending = TRUE;
            return TRUE;

        case EVT_APP_RESUME:
            ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
            pMe->m_bSuspending = FALSE;

            if (pMe->m_pDisplay != NULL)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            {
                AEEDeviceInfo di; 
                
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dy = di.cyScreen;
            }
            ERR("start EVT_APP_RESUME-UTK_RunFSM",0,0,0);   
            UTK_RunFSM(pMe);
            ERR("end EVT_APP_RESUME-UTK_RunFSM",0,0,0);             
            return TRUE;

        case EVT_DIALOG_INIT:
            if (wParam == OEM_IME_DIALOG)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            pMe->m_dwOldBkColor = IDISPLAY_SetColor(pMe->m_pDisplay ,
                    CLR_USER_BACKGROUND, RGB_WHITE);
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;
            ERR("start UTK_RouteDialogEvent",0,0,0);   
            return UTK_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            if (wParam == OEM_IME_DIALOG)
            {
                return TRUE;
            }
            return UTK_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) UTK_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void) ISHELL_SetTimer(pMe->m_pShell,
                     APPISREADY_TIMER,
                     UTK_APPIsReadyTimer,
                     pMe);
            return TRUE;

        case EVT_APPISREADY:
            pMe->m_bAppIsReady = TRUE;
            return TRUE;
            
        case EVT_SEND_STATUS:
            return UTK_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_KEY:
        case EVT_COMMAND: 
            if (pMe->m_bAppIsReady == FALSE)
            {
                return TRUE;
            }
            return UTK_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if ((wParam == OEM_IME_DIALOG) &&
                (pMe->m_pActiveDlg != (IDialog *)dwParam))
            {
                (void) ISHELL_PostEvent(pMe->m_pShell,
                                        AEECLSID_APP_UTK,
                                        EVT_USER_REDRAW,
                                        0,
                                        0);
                return TRUE;
            }
            if (wParam == 0)
            {
                return TRUE;
            }
            (void) UTK_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL; 
            pMe->m_bAppIsReady = FALSE;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼUTK״̬��
                UTK_RunFSM(pMe);
            }
            return TRUE;
            
        case EVT_BACKUTK:
            (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_UTK);
            return TRUE;

        case EVT_RUIM_PROACTIVE:
            old_cmd_type = pMe->cmd_type;
            pMe->cmd_type = (uim_tk_proactive_cmd_enum_type)wParam;
            switch (pMe->cmd_type)
            {
                case UIM_TK_REFRESH:
                    // Refresh �����ǿ������ݷ����˱仯��Ҫ���ն���֮ͬ���������ն�û�л��濨��
                    // ���ݵĲ��ֿɲ��账����ÿ�ζ�ȡʱ���ӿ��϶��������ն˻����˵Ŀ��ϲ�����
                    // �ݣ����ն����յ��������Ӧ���´ӿ��϶�ȡ�����뿨ͬ����
                    {
                        RefreshCmdType *pCmd=NULL;
                        byte *pdata;
                        uim_tk_general_result_type eResult = UIM_TK_CMD_PERFORMED_SUCCESSFULLY;
                        uint16   wMask = 0;
                        
                        pdata = UTK_GetCmddata(pMe->cmd_type);
                        pCmd = (RefreshCmdType *)MALLOC(sizeof(RefreshCmdType));
                        if ((NULL == pdata) || (NULL == pCmd))
                        {
                            UTK_GiveResponse(pMe, pMe->cmd_type, FALSE, UIM_TK_COMMAND_BEYOND_TERMINALS_CAPABILITIES);
                            return TRUE;
                        }
                        
                        DecodeRefreshData(pdata, pCmd);
                        
                        switch(pCmd->cmd_qualifier)
                        {
                            // ��ʼ����ȫ�ļ����
                            case UIM_TK_NAA_INITIALIZATION_AND_FULL_FILE_CHANGE_NOTIFICATION:
                                
                            // ��ʼ�����ļ����
                            case UIM_TK_NAA_INITIALISATION_AND_FILE_CHANGE_NOTIFICATION:
                                
                            // SIM����ʼ��    
                            case UIM_TK_NAA_INITIALISATION:
                                wMask = REFRESHMASK_UIMALL;
                                break;
                                
                            // �ļ����
                            case UIM_TK_FILE_CHANGE_NOTIFICATION:
                                // ��������˵��ļ��Ŀǰֻ������źͿ��绰��
                                if ((pCmd->fileList.numFiles>0) &&
                                    (pCmd->fileList.numBytes>=4))
                                {
                                    int i,j,k,pos=2;
                                    boolean bValid=FALSE;
                                    
                                    for (i=0; i<pCmd->fileList.numFiles; i++)
                                    {
                                        if (i==pCmd->fileList.numFiles-1)
                                        {// ���һ��
                                            j=pCmd->fileList.numBytes-2;
                                            k=pCmd->fileList.numBytes-1;
                                            
                                            if (pCmd->fileList.fileData[j]==0x6F &&
                                                pCmd->fileList.fileData[k]==0x3C)
                                            {// ����
                                                wMask |= REFRESHMASK_UIMSMS;
                                            }
                                            else if (pCmd->fileList.fileData[j]==0x6F &&
                                                pCmd->fileList.fileData[k]==0x3A)
                                            {// ���绰��
                                                wMask |= REFRESHMASK_UIMADD;
                                            }
                                        }
                                        else
                                        {
                                            bValid=FALSE;
                                            for(;pos<pCmd->fileList.numFiles;)
                                            {
                                                if (pCmd->fileList.fileData[pos]==0x3F &&
                                                    pCmd->fileList.fileData[pos+1]==0x00)
                                                {
                                                    bValid=TRUE;
                                                    break;
                                                }
                                                pos++;
                                            }
                                            if (bValid)
                                            {
                                                j=pos-2;
                                                k=pos-1;
                                                
                                                // �����ļ�����ʼ��ǣ����ڷ�����һ�ļ���
                                                pos+=2;
                                                
                                                if (pCmd->fileList.fileData[j]==0x6F &&
                                                    pCmd->fileList.fileData[k]==0x3C)
                                                {// ����
                                                    wMask |= REFRESHMASK_UIMSMS;
                                                }
                                                else if (pCmd->fileList.fileData[j]==0x6F &&
                                                    pCmd->fileList.fileData[k]==0x3A)
                                                {// ���绰��
                                                    wMask |= REFRESHMASK_UIMADD;
                                                }
                                            }
                                            else
                                            {
                                                break;
                                            }
                                        }
                                    }
                                }
                                break;
                                
                            // SIM����λ    
                            case UIM_TK_NAA_RESET:
                                wMask = REFRESHMASK_UIMRESET;
                                break;
                                
                            default:
                                eResult = UIM_TK_COMMAND_BEYOND_TERMINALS_CAPABILITIES;
                                break;
                                
                        }
                        
                        FREEIF(pCmd);
                        UTK_GiveResponse(pMe, pMe->cmd_type, FALSE, eResult);
                        if (wMask != 0)
                        {// ���¹������� Ilde ȥ����
                            (void) ISHELL_PostEvent( pMe->m_pShell,
                                                     AEECLSID_CORE_APP,
                                                     EVT_UTKREFRESH,
                                                     wMask,
                                                     0);
                            
                            // ����ر�ȫ�� Applet, �Ա����ͷŶԿ��绰���ӿ�                         
                            (void)ISHELL_CloseApplet(pMe->m_pShell, TRUE);
                        }
                    }
                    return TRUE;

                case UIM_TK_PLAY_TONE:                
                    eTempState = UTKST_PLAYTONE;                     
                    break;
                    
                case UIM_TK_SETUP_CALL:
                    {
                        Setup_Call  *pstCall;
                        byte        *pdata;                        
						AEETCalls po;
						
                        pstCall = UTK_Get_Setup_Call_Buf();
                        pdata = UTK_GetCmddata(pMe->cmd_type);
                        
                        if (NULL == pstCall || NULL == pdata)
                        {
                            UTK_GiveResponse(pMe, pMe->cmd_type, FALSE, UIM_TK_CMD_PERFORMED_SUCCESSFULLY);
                            break;
                        }
                        
                        MEMSET(pstCall, 0, sizeof(Setup_Call));
                        (void)UTK_DecodeSetupCallCmdData(pdata, pstCall);
                        						
						if(SUCCESS != ITELEPHONE_GetCalls(pMe->m_pITelephone, &po,sizeof(AEETCalls)))
						{
							break;
						}
                        if ((po.dwCount>0)&&
                            ((pstCall->cmd_describe.command_restricttag == 0x00) ||
                             (pstCall->cmd_describe.command_restricttag == 0x01)) &&
                            (WSTRLEN(pstCall->wszAlpha) == 0))
                        {
                            pMe->m_eMsgRetState = UTKST_MAIN;
                            pMe->m_wMsgID = IDS_CALLINFO;     
                            pMe->m_MsgTimeout = TIMEOUT_MS_MSGBOX;                           	
                            eTempState = UTKST_MSG;
                        }
                        else if(WSTRLEN(pstCall->wszDialNum)>0)
                        {
                            UTK_MAKE_UTK_CALL(pMe);
                        }
                        UTK_GiveResponse(pMe, pMe->cmd_type, FALSE, UIM_TK_CMD_PERFORMED_SUCCESSFULLY);
                    }
                    break;
                
                case UIM_TK_DISPLAY_TEXT:
                    eTempState = UTKST_DISPLAY;
                    break;
                
                case UIM_TK_GET_INPUT:
                    eTempState = UTKST_INPUT;                                 
                    //����������������ı������룬�������ı��򣬷�ֹ�ڶ����ı���ѵ�һ���ı����е�����������
                    if(pMe->m_pActiveDlgID == IDD_INPUT_DIALOG)
                    {
                       pMe->InputeBackupNeeded = FALSE; 
                    }                     
                    break;
                
                case UIM_TK_SELECT_ITEM:
                    eTempState = UTKST_LIST;
                    break;
                
                case UIM_TK_SETUP_MENU:                  
                case UIM_TK_END_PROACTIVE_SESSION:                 
                    pMe->cmd_type = UIM_TK_SETUP_MENU;
                    if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_APP_UTK)
                    {
                        // ֻ����µ�ǰ״̬���������� Applet
                        pMe->m_eCurState = UTKST_MAIN;
                        return TRUE;
                    }
                    else
                    {
                        eTempState = UTKST_MAIN;
                    }
                    break;
                
                case UIM_TK_CDMA_SEND_SHORT_MSG:
                    ERR("UIM_TK_CDMA_SEND_SHORT_MSG",0,0,0);
                    eTempState = UTKST_SENDMSG;
                    break;
                
                case UIM_TK_PROVIDE_LOCAL_INFO: 
                    UTK_GiveResponse(pMe, pMe->cmd_type, TRUE, UIM_TK_CMD_PERFORMED_SUCCESSFULLY);  
                    break;                                 
                                  
                default:
                    break;
            }

            if (eTempState == UTKST_NONE)
            {
                return TRUE;
            }
            
            // ���µ�ǰ״̬
            pMe->m_eCurState = eTempState;
            
            // ����ǰ�Applet���Ǳ�Applet������Ը��¼���ֱ�ӷ���TRUE
            if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_APP_UTK)
            {
                if((old_cmd_type == UIM_TK_SETUP_CALL) && (pMe->m_bSuspending == TRUE))
                {               
                    return TRUE;
                }
                else
                {
                    (void)ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APP_UTK);
                }
                return TRUE;
            }
            else
            {
                // Applet �Ѿ��������С�״̬�Ѿ����£������رնԻ�����
                // ִ��״̬������ ʱ�������µ�״̬��
                CLOSE_DIALOG(DLGRET_CREATE);
            }
            return TRUE;
            
        case EVT_WMS_CMD_STATUS:
            (void)UTK_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            FREEIF(dwParam);

            return TRUE;
            
        case EVT_WMS_MSG_SUBMIT_REPORT:
        case EVT_WMS_MSG_SEND:
            if (((wms_msg_event_info_s_type *)dwParam)->submit_report_info.client_id == WMS_CLIENT_TYPE_CAT)
            {
                UTK_ProcessStatus(pMe, &((wms_msg_event_info_s_type *)dwParam)->submit_report_info);
            }
            FREEIF((wms_msg_event_info_s_type*)dwParam);
            return TRUE;
            
        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return UTK_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

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
)
{
    byte i, high, low;
    boolean bUincode;

    bUincode = FALSE;
    if ((mode && pBytBuf[0] == UNICODE_FLAG) ||
        (!mode && pBytBuf[0] == ALPHA_UNICODE))
    {
        bUincode = TRUE;
    }
    
    if (!mode || pBytBuf[0] == UNICODE_FLAG)
    {
        pBytBuf++;
        bytLen--;
    }

    for (i=0; i<bytLen; )
    {
        if (bUincode)
        {
            high = pBytBuf[i++];
        }
        else
        {
            high = 0;
        }

        if (i < bytLen)
        {
            low = pBytBuf[i++];
        }
        else
        {
            break;
        }

        *pUnicodeBuf++ = (high<<8) | low; //lint !e539
    }
    *pUnicodeBuf = 0;
}

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
void UTK_MsgSendStatusCallBack(void * pUser, int status)
{
    CUTK *pMe = (CUTK *) pUser;
    
    if (pMe == NULL)
    {
        return;
    }
    
    if (ISHELL_ActiveApplet(pMe->m_pShell) != AEECLSID_APP_UTK)
    {
        return;
    }
    
    (void) ISHELL_PostEvent(pMe->m_pShell,
                AEECLSID_APP_UTK,
                EVT_SEND_STATUS,
                0,
                (uint32)status);
}

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
void UTK_MessageAlert(CUTK * pMe, boolean  bPlay)
{
    byte bSmsAlertType = 0;
    uint16 ringerID = 78;
    
    if ( NULL == pMe)
    {
        return;
    }
    
    if (!bPlay)
    {
        IALERT_StopSMSAlert(pMe->m_pAlert);
        IALERT_StopMp3Alert(pMe->m_pAlert);
        return;
    }
    (void) ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_SMS_RINGER,
                          &bSmsAlertType,
                          sizeof(bSmsAlertType));

    (void) ICONFIG_GetItem( pMe->m_pConfig,
                           CFGI_SMS_RINGER_ID,
                           &ringerID,
                           sizeof(ringerID));

    if (bSmsAlertType)
    {
    	
		AEETCalls po;
		
		if(SUCCESS != ITELEPHONE_GetCalls(pMe->m_pITelephone, &po,sizeof(AEETCalls)))
		{
			return;
		}
		
        if (po.dwCount>0)
        {
            (void)ISHELL_Beep(pMe->m_pShell,BEEP_MSG,TRUE);
            return;
        }    
        IALERT_StartSMSAlert(pMe->m_pAlert, ringerID);      
    }
}

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
void UTK_APPIsReadyTimer(void *pme)
{
    CUTK *pMe = (CUTK *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_APP_UTK,
                            EVT_APPISREADY,
                            0,
                            0);
}

/*===========================================================================
FUNCTION HexToDialStr

DESCRIPTION
  translate the hex code to Dial String,Create by Azlun.Xie 20060604
    
DEPENDENCIES
  None

RETURN VALUE
  finish digit count

SIDE EFFECTS
  None
===========================================================================*/
int HexToDialStr(byte *src,char *dst,int num_dig)
{
 char low_digit=0x00;
 char hig_digit=0x00;
 int finish_count=0;

 if(!src||!dst||num_dig<=0)
 {
    return 0;
    ERR("HexToDialStr:!src||!dst||num_dig<=0:(%X,%X,%d)",src,dst,num_dig);
 }

 do
 {
    low_digit= (char) ( *src     &0x0F)+'0';
    hig_digit= (char)(((*src)>>4)&0x0F)+'0';
    ERR("HexToDialStr:Src:0x%02X: Str:%c%c",*src,low_digit,hig_digit);

    //step forward
    src++;

    if('0'<=low_digit&&low_digit<='9')
    {
        *(dst++) =low_digit;
        finish_count++ ;
    }
    else if('0'+0x0A==low_digit)
    {
        *(dst++) ='*';
        finish_count++ ;
    }
    else if('0'+0x0B==low_digit)
    {
        *(dst++) ='#';
        finish_count++ ;
    }
    else if('0'+0x0C==low_digit)
    {
        *(dst++) ='+';
        finish_count++ ;
    }
    else
    {
        ERR("HexToDialStr Src End with 0x%2X",low_digit,0,0);
        break;
    } 
    if('0'<=hig_digit&&hig_digit<='9')
    {
        *(dst++) =hig_digit;
        finish_count++ ;
    }
    else if('0'+0x0A==hig_digit)
    {
        *(dst++) ='*';
        finish_count++ ;
    }
    else if('0'+0x0B==hig_digit)
    {
        *(dst++) ='#';
        finish_count++ ;
    }
    else if('0'+0x0C==hig_digit)
    {
        *(dst++) ='+';
        finish_count++ ;
    }
    else
    {
        ERR("HexToDialStr Src End with 0x%2X",hig_digit,0,0);
        break;
    } 
 }while(finish_count<num_dig);
 
 *dst='\0';//end the dialstring
 
 return finish_count;
}

static void UTK_MAKE_UTK_CALL(CUTK *pMe)
{
    PARAM_NOT_REF(pMe) 
    ICallApp *pCallApp = NULL;
    
    if(SUCCESS == ISHELL_CreateInstance(pMe->m_pShell, 
                                          AEECLSID_DIALER,
                                           (void**)&pCallApp))
    { 
        ICallApp_Make_UTK_Call_F(pCallApp);
    }
    if(pCallApp)
    {
        (void)ICallApp_Release(pCallApp);
        pCallApp = NULL;
    }
}

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
                          wms_cmd_err_e_type  cmd_err)
{
    CUTK * pMe = (CUTK *) (user_data);
  
    if (pMe != NULL)
    {
        wmsutk_cmd_status_type *cmd_status = MALLOC(sizeof(wmsutk_cmd_status_type));
        
        if (cmd_status == NULL)
        {
            return;
        }
        cmd_status->cmd = cmd;
        cmd_status->cmd_err = cmd_err;
        
        (void)ISHELL_PostEventEx(pMe->m_pShell, 
                                 EVTFLG_ASYNC,
                                 AEECLSID_APP_UTK,
                                 EVT_WMS_CMD_STATUS, 
                                 0, 
                                 (uint32)cmd_status);
    }
}

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
               boolean                    *shared)
{
    wms_msg_event_info_s_type   *pInfobuf=NULL;
    IShell                      *pShell = AEE_GetShell();
    uint8                       btRet; 
    AEEEvent                    evt=0;
    
    if (pShell == NULL)
    {
        MSG_ERROR("WMSAPP: pShell = NULL",0,0,0);
        return;
    }
    
    pInfobuf = MALLOC(sizeof(wms_msg_event_info_s_type));
    if (pInfobuf == NULL)
    {
        MSG_ERROR("WMSAPP: msg_event_buf = NULL",0,0,0);
        return;
    }
    
    (void)MEMCPY(pInfobuf, pInfo, sizeof(wms_msg_event_info_s_type));
    MSG_ERROR("UTK_MsgCb: %d", event, 0, 0);
    switch (event)
    {
        case WMS_MSG_EVENT_SEND:
            if ((pInfobuf->submit_report_info.client_id == WMS_CLIENT_TYPE_CAT) &&
                (pInfobuf->submit_report_info.report_status != WMS_RPT_OK))
            {
                evt = EVT_WMS_MSG_SEND;
            }
            break;
            
        case WMS_MSG_EVENT_SUBMIT_REPORT:
            pInfobuf->submit_report_info.alpha_id.data = (uint8 *)MALLOC(WMS_ALPHA_ID_MAX);
            pInfobuf->submit_report_info.alpha_id.len = pInfo->submit_report_info.alpha_id.len;
            
            if ((pInfo->submit_report_info.alpha_id.data != NULL) &&
                (pInfo->submit_report_info.alpha_id.len > 0))
            {
                (void)MEMCPY(pInfobuf->submit_report_info.alpha_id.data,
                            pInfo->submit_report_info.alpha_id.data,
                            pInfo->submit_report_info.alpha_id.len);
            }
            
            evt = EVT_WMS_MSG_SUBMIT_REPORT;
            break;
            
        default:
            MSG_ERROR("Invalid msg event %d", event, 0,0);
            break;
    } // switch
    
    if (evt != 0)
    {
        btRet = ISHELL_PostEventEx(pShell,
                                   EVTFLG_ASYNC, 
                                   AEECLSID_APP_UTK, 
                                   evt,
                                   0, 
                                   (uint32) pInfobuf);
                        
        if (btRet == FALSE)
        {
            FREEIF(pInfobuf);
            MSG_HIGH("Event  %x could not be posted!", evt,0,0);
        }
    }
    else
    {
        FREEIF(pInfobuf);
    }
} // UTK_MsgCb() 

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
void UTK_ProcessStatus(CUTK *pMe, wms_submit_report_info_s_type *pRptInfo)
{
    pMe->m_SendStatus = pRptInfo->report_status;
    pMe->m_SendtlStatus = pRptInfo->cause_info.tl_status;
    
    switch (pRptInfo->report_status)
    {
        case WMS_RPT_OK:
        case WMS_RPT_LL_ERROR:
        
        case WMS_RPT_OUT_OF_RESOURCES:
        case WMS_RPT_SMR_NO_RESOURCES:

        case WMS_RPT_NETWORK_NOT_READY:

        case WMS_RPT_PHONE_NOT_READY:

        case WMS_RPT_NO_ACK:

        case WMS_RPT_CDMA_TL_ERROR:
            break;

        case WMS_RPT_ACCESS_TOO_LARGE:
            return;

        case WMS_RPT_DC_TOO_LARGE:

        case WMS_RPT_NOT_ALLOWED_IN_AMPS:
            
        case WMS_RPT_CANNOT_SEND_BROADCAST:
            
        case WMS_RPT_RP_ERROR:
            
        case WMS_RPT_CP_ERROR:
            
        case WMS_RPT_SMR_TR1M_TIMEOUT:
            
        case WMS_RPT_SMR_TR2M_TIMEOUT:
            
        // Any other error
        default:
            break;
    }
    
    ERR("pRptInfo->report_status = %d", pRptInfo->report_status, 0, 0);
    
    if ((pMe->m_pActiveDlgID == IDD_SENDMSG_DIALOG) &&
        (ISHELL_ActiveApplet(pMe->m_pShell) == AEECLSID_APP_UTK))
    {
        (void)ISHELL_PostEventEx(pMe->m_pShell,
                                 EVTFLG_ASYNC, 
                                 AEECLSID_APP_UTK, 
                                 EVT_SEND_STATUS,
                                 0, 
                                 0);
    }
}

