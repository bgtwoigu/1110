#ifndef _CUSTSERVICE_APP_C_
#define _CUSTSERVICE_APP_C_

#include "Custservice_priv.h"
//#include "MorseAppProt.h"

int CustServiceMod_Load(IShell *pIShell,void *ph,IModule **ppMod);
int CustServiceMod_New(int16  nSize,IShell *pIShell,void *ph,IModule  **ppMod,PFNMODCREATEINST pfnMC,
    				PFNFREEMODDATA pfnMF);
static int CustServiceMod_CreateInstance(IModule *po,IShell *pIShell,AEECLSID ClsId,void **ppObj);
static uint32 CustServiceMod_AddRef(IModule *po);
static uint32 CustServiceMod_Release(IModule *po);
static void CustServiceMod_FreeResources(IModule  *po,IHeap  *ph,IFileMgr *pfm);
static int CustService_New(IShell *ps,IModule *pIModule,ICustServiceMenu **ppObj);
static uint32 CustService_AddRef(ICustServiceMenu *pi);
static uint32 CustService_Release (ICustServiceMenu *pi);
static boolean CustService_HandleEvent(ICustServiceMenu *pi,AEEEvent eCode,uint16  wParam,uint32 dwParam);

static void CustService_APPIsReadyTimer(void *pme);
static int CustService_InitAppData(CCustServiceMenu *pMe);
static void CustService_FreeAppData(CCustServiceMenu *pMe);
static void CustService_RunFSM(CCustServiceMenu *pMe);

/*-------------------------------------------------------------------
            Static variable Definitions
-------------------------------------------------------------------*/
static CustServiceMod gCustServiceMod;

static const VTBL(IModule) gModFuncs =
{
	CustServiceMod_AddRef,
	CustServiceMod_Release,
	CustServiceMod_CreateInstance,
	CustServiceMod_FreeResources
};


static CCustServiceMenu gCustServiceMenu;
extern boolean gCustServiceBgRun;
static const VTBL(ICustServiceMenu) gCustServiceMethods =
{
	CustService_AddRef,
	CustService_Release,
	CustService_HandleEvent
};

/* =========================================================================
                     FUNCTION DEFINITIONS
   ========================================================================= */

/*==============================================================================
       SportsMod_Load
==============================================================================*/
int CustServiceMod_Load(IShell *pIShell,void *ph,IModule **ppMod)
{
	return CustServiceMod_New(sizeof(CustServiceMod),pIShell,ph,ppMod,NULL,NULL);
}


int  CustServiceMod_New(int16 nSize,IShell *pIShell,void *ph,IModule **ppMod,
 					PFNMODCREATEINST pfnMC,PFNFREEMODDATA pfnMF)
{
	PARAM_NOT_REF(nSize)
	PARAM_NOT_REF(pfnMC)
	PARAM_NOT_REF(pfnMF)
	
	#ifndef AEE_SIMULATOR
	PARAM_NOT_REF(ph)
	#endif
	
	if (NULL == ppMod)
	{
	    return EFAILED;
	}
	
	*ppMod = NULL;
    
	if (!pIShell)
	{
	    return EFAILED;
	}
	
	MEMSET(&gCustServiceMod,0,sizeof(CustServiceMod));
	INIT_VTBL(&gCustServiceMod,IModule,gModFuncs);
	gCustServiceMod.m_nRefs = 1;
	*ppMod = (IModule *)&gCustServiceMod;
	return AEE_SUCCESS;
}

/*==============================================================================
o����y:
       SportsMod_AddRef

?��?��:
       Sports ?��?����y��???��y?��1��?2�顤��??�̡�?���?��y��?��y?��

2?��y:
       po [in]��oIModule?��?��???��?????��

����???��:
       �̡�?��??Sports ?��?����?��y��?��y

��?����:

==============================================================================*/
static uint32 CustServiceMod_AddRef(IModule *po)
{
	return(++((CustServiceMod *)po)->m_nRefs);
}

/*==============================================================================
o����y:
       SportsMod_Release

?��?��:
       Sports ?��?����y��???��y??1��?2�顤��??�̡�?���?��y��?��y?��

2?��y:
       po [in]��oIModule?��?��???��?????��

����???��:
       �̡�?��??Sports ?��?����?��y��?��y

��?����:

==============================================================================*/
static uint32 CustServiceMod_Release(IModule *po)
{
	if (((CustServiceMod *) po)->m_nRefs == 0)
	{
	    return 0;
	}

	return(--((CustServiceMod *)po)->m_nRefs);
}

/*==============================================================================
o����y:
       SportsMod_CreateInstance

?��?��:
        ����1y���?��(3?��??��)Sports???��������?��?��?�����̨�y?��

2?��y:
       po [in]��oIModule?��?��???��?????��
       pIShell [in]��oIShell?��?��???��?????��
       ClsId [in]��o��a���?����?���̨�y��?����ID?��
       ppObj [out]:��o��?���������????�����̨�y��?????��?????��?��??��

����???��:
       EFAILED;2�����¨���㨹?��
       SUCCESS;2������3��1|?��

��?����:
       2?D����??�쨨���̡¨�??��

==============================================================================*/
static int  CustServiceMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
	*ppObj = NULL;

	if (ClsId != AEECLSID_CUSTSERVICE)
	{
	    return EFAILED;
	}

	if (CustService_New(pIShell, po,(ICustServiceMenu**)ppObj) != SUCCESS)
	{
	    return EFAILED;
	}
	
	return SUCCESS;
}

/*==============================================================================
o����y:
       SportsMod_FreeResources

?��?��:
        ������?Sports?��?������?���?o����y?��

2?��y:
       po ��oIModule?��
       ph ��oIHeap?��?��?????��
       pfm :IFileMgr?��?��?????��

����???��:
       ?T

��?����:
       2?D����??�쨨���̡¨�??��

==============================================================================*/
static void CustServiceMod_FreeResources(IModule * po,IHeap * ph,IFileMgr * pfm)
{

    PARAM_NOT_REF(po)
    PARAM_NOT_REF(ph)
    PARAM_NOT_REF(pfm)
}

static int CustService_New(IShell *ps,IModule *pIModule,ICustServiceMenu **ppObj)
{
	int retVal = SUCCESS;
	if (0 == gCustServiceMenu.m_nRefs)
	{
		// Must initialize the object
		MEMSET(&gCustServiceMenu,  0, sizeof(CCustServiceMenu));
		INIT_VTBL(&gCustServiceMenu, ICustServiceMenu, gCustServiceMethods);

		gCustServiceMenu.m_nRefs     = 0;
		gCustServiceMenu.m_pShell    = ps;
		gCustServiceMenu.m_pModule   = pIModule;

		(void) ISHELL_AddRef(ps);
		(void) IMODULE_AddRef(pIModule);
		retVal = CustService_InitAppData(&gCustServiceMenu);
	}

	++gCustServiceMenu.m_nRefs;
	*ppObj = (ICustServiceMenu*)&gCustServiceMenu;
	
	return retVal;
}

static uint32  CustService_AddRef(ICustServiceMenu *pi)
{
    register CCustServiceMenu *pMe = (CCustServiceMenu*)pi;

    //ASSERT(pMe != NULL);
    //ASSERT(pMe->m_nRefs > 0);

    return (++pMe->m_nRefs);
}

static uint32  CustService_Release (ICustServiceMenu *pi)
{
	register CCustServiceMenu *pMe = (CCustServiceMenu*)pi;

	//ASSERT(pMe != NULL);

	if (pMe->m_nRefs == 0)
	{
	    return 0;
	}

	if (--pMe->m_nRefs)
	{
	    return pMe->m_nRefs;
	}

	CustService_FreeAppData(pMe);

	(void) ISHELL_Release(pMe->m_pShell);
	(void) IMODULE_Release(pMe->m_pModule);

	return 0;
}

/*==============================================================================
o����y:
       SportSApp_InitAppData

?��?��:
       3?��??��SportsMenu Applet?��11��y?Y������??��?��

2?��y:
       pMe [in]��o???��CSportsMenu Applet???��?��11��??????��???��11�㨹o?D?3��D����?��??��D??��?��

����???��:
       SUCCESS��o3?��??��3��1|?��
       EFAILED��o3?��??������㨹?��

��?����:

==============================================================================*/
static int CustService_InitAppData(CCustServiceMenu *pMe)
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

	pMe->m_bAppIsReady = FALSE;
	pMe->m_ePreState = CUSTSERVICE_STATE_NONE;
	pMe->m_eCurState = CUSTSERVICE_STATE_INIT;
	pMe->m_pActiveDlg = NULL;
	pMe->m_eDlgRet = DLGRET_CREATE;
	pMe->m_bNotOverwriteDlgRet = FALSE;
	pMe->m_currDlgId = 0;
	pMe->m_nSubDlgId = 0;
	/*Init Morse App Data*/
	//CustService_CodeInitApp();
    return SUCCESS;
}

/*==============================================================================
o����y:
       SportSApp_FreeAppData

?��?��:
       ������?SettingMenu Applet��1��?��??��1?����?��?��

2?��y:
       pMe [in]��o???��SettingMenu Applet???��?��11��??????��???��11�㨹o?D?3��D����?��??��D??��?��

����???��:
       ?T?��

��?����:

==============================================================================*/
static void CustService_FreeAppData(CCustServiceMenu *pMe)
{
	if (NULL == pMe)
	{
	    return;
	}
	
	ISHELL_CancelTimer(pMe->m_pShell,NULL,pMe);
}

/*==============================================================================
o����y:
       SportSApp_RunFSM

?��?��:
       SportsMenu Applet��D?T���䨬??����y???��

2?��y:
       pMe [in]��o???��SettingMenu Applet???��?��11��??????��???��11�㨹o?D?3��D����?��??��D??��?��

����???��:
       ?T?��

��?����:

==============================================================================*/
static void CustService_RunFSM(CCustServiceMenu *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = CustService_ProcessState(pMe);

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
o����y:
       SportSApp_HandleEvent

?��?��:
       SportsApp  Applet��??t��|����?��o����y?��?����D��?������?Applet��?��??t��?
       ?????-����??o����y��|����?��

2?��y:
       pi [in]��o???��ISportMenu?��?��t???����??????��
       eCode��oBREW��??t�䨲???��
       wParam��o��?��??t?��1?��a��?��y?Y?��
       dwParam [in]��o��?��??t?��1?��a��?��y?Y?��

����???��:
       TRUE��o��?��?��??t��?��|����?��
       FALSE��o��?��?��??t??��?��|����?��

��?����:

==============================================================================*/
static boolean CustService_HandleEvent(ICustServiceMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CCustServiceMenu *pMe = (CCustServiceMenu*)pi;
    AEEAppStart *as;

    switch (eCode)
    {
        case EVT_APP_START:

            pMe->m_bAppIsReady = FALSE;
            //ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
            if (NULL != pMe->m_pDisplay)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;
            pMe->m_rc.y = pMe->m_rc.y - CUSTSERVICE_MENU_SOFK_HIGHT;
            pMe->m_bSuspending = FALSE;

            CustService_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:

    		if (pMe->m_pDisplay != NULL)
    		{
    			(void) IDISPLAY_Release(pMe->m_pDisplay);
    			pMe->m_pDisplay = NULL;
    		}
            pMe->m_bSuspending = TRUE;
            return TRUE;

        case EVT_APP_SUSPEND:
            pMe->m_bSuspending = TRUE;

            return TRUE;

        case EVT_APP_RESUME:
            //ASSERT(dwParam != 0);
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
            pMe->m_rc.y = pMe->m_rc.y - CUSTSERVICE_MENU_SOFK_HIGHT;
            CustService_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;
            return CustService_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            return CustService_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) CustService_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            CustService_APPIsReadyTimer,
                            pMe);
            return TRUE;

        case EVT_APPISREADY:
            pMe->m_bAppIsReady = TRUE;
            return TRUE;

        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_KEY:
        case EVT_COMMAND:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return CustService_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void) CustService_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            if (pMe->m_bSuspending == FALSE)
            {
                CustService_RunFSM(pMe);
            }
            return TRUE;

        default:
            return CustService_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
o����y:
       SportSApp_APPIsReadyTimer

?��?��:
       ?������?��??�̡�o����y?��?�¨�a��?����?????��?�����?��?����a��?��|����?��

2?��y:
       pme [in]��ovoid *����D��????��?���??SportMenu?��11?????��

����???��:
       ?T?��

��?����:

==============================================================================*/
static void CustService_APPIsReadyTimer(void *pme)
{
	CCustServiceMenu *pMe = (CCustServiceMenu *)pme;

	if (NULL == pMe)
	{
	    return;
	}

	(void) ISHELL_PostEvent( pMe->m_pShell,AEECLSID_CUSTSERVICE,EVT_APPISREADY,0,0);
}

#endif
