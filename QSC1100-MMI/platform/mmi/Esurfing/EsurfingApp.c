#ifndef _ESURFING_APP_C_
#define _ESURFING_APP_C_


#include "Esurfing_priv.h"
//#include "MorseAppProt.h"

int EsurfingMod_Load(IShell *pIShell,void *ph,IModule **ppMod);
int EsurfingMod_New(int16  nSize,IShell *pIShell,void *ph,IModule  **ppMod,PFNMODCREATEINST pfnMC,
    				PFNFREEMODDATA pfnMF);
static int EsurfingMod_CreateInstance(IModule *po,IShell *pIShell,AEECLSID ClsId,void **ppObj);
static uint32 EsurfingMod_AddRef(IModule *po);
static uint32 EsurfingMod_Release(IModule *po);
static void EsurfingMod_FreeResources(IModule  *po,IHeap  *ph,IFileMgr *pfm);
static int Esurfing_New(IShell *ps,IModule *pIModule,IEsurfingMenu **ppObj);
static uint32 Esurfing_AddRef(IEsurfingMenu *pi);
static uint32 Esurfing_Release (IEsurfingMenu *pi);
static boolean Esurfing_HandleEvent(IEsurfingMenu *pi,AEEEvent eCode,uint16  wParam,uint32 dwParam);

static void Esurfing_APPIsReadyTimer(void *pme);
static int Esurfing_InitAppData(CEsurfingMenu *pMe);
static void Esurfing_FreeAppData(CEsurfingMenu *pMe);
static void Esurfing_RunFSM(CEsurfingMenu *pMe);

/*-------------------------------------------------------------------
            Static variable Definitions
-------------------------------------------------------------------*/
static EsurfingMod gEsurfingMod;

static const VTBL(IModule) gModFuncs =
{
	EsurfingMod_AddRef,
	EsurfingMod_Release,
	EsurfingMod_CreateInstance,
	EsurfingMod_FreeResources
};


static CEsurfingMenu gEsurfingMenu;
extern boolean gEsurfingBgRun;
static const VTBL(IEsurfingMenu) gEsurfingMethods =
{
	Esurfing_AddRef,
	Esurfing_Release,
	Esurfing_HandleEvent
};

/* =========================================================================
                     FUNCTION DEFINITIONS
   ========================================================================= */
int EsurfingMod_Load(IShell *pIShell,void *ph,IModule **ppMod)
{
	return EsurfingMod_New(sizeof(EsurfingMod),pIShell,ph,ppMod,NULL,NULL);
}

int  EsurfingMod_New(int16 nSize,IShell *pIShell,void *ph,IModule **ppMod,
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

	MEMSET(&gEsurfingMod,0,sizeof(EsurfingMod));
	INIT_VTBL(&gEsurfingMod,IModule,gModFuncs);
	gEsurfingMod.m_nRefs = 1;
	*ppMod = (IModule *)&gEsurfingMod;
	return AEE_SUCCESS;
}

static uint32 EsurfingMod_AddRef(IModule *po)
{
	return(++((EsurfingMod *)po)->m_nRefs);
}

static uint32 EsurfingMod_Release(IModule *po)
{
	if (((EsurfingMod *) po)->m_nRefs == 0)
	{
	    return 0;
	}

	return(--((EsurfingMod *)po)->m_nRefs);
}

static int  EsurfingMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
	*ppObj = NULL;

	if (ClsId != AEECLSID_ESURFING)
	{
	    return EFAILED;
	}

	if (Esurfing_New(pIShell, po,(IEsurfingMenu**)ppObj) != SUCCESS)
	{
	    return EFAILED;
	}
	
	return SUCCESS;
}

static void EsurfingMod_FreeResources(IModule * po,IHeap * ph,IFileMgr * pfm)
{

    PARAM_NOT_REF(po)
    PARAM_NOT_REF(ph)
    PARAM_NOT_REF(pfm)
}

static int Esurfing_New(IShell *ps,IModule *pIModule,IEsurfingMenu **ppObj)
{
	int retVal = SUCCESS;
	if (0 == gEsurfingMenu.m_nRefs)
	{
		// Must initialize the object
		MEMSET(&gEsurfingMenu,  0, sizeof(CEsurfingMenu));
		INIT_VTBL(&gEsurfingMenu, IEsurfingMenu, gEsurfingMethods);

		gEsurfingMenu.m_nRefs     = 0;
		gEsurfingMenu.m_pShell    = ps;
		gEsurfingMenu.m_pModule   = pIModule;

		(void) ISHELL_AddRef(ps);
		(void) IMODULE_AddRef(pIModule);
		retVal = Esurfing_InitAppData(&gEsurfingMenu);
	}

	++gEsurfingMenu.m_nRefs;
	*ppObj = (IEsurfingMenu*)&gEsurfingMenu;
	
	return retVal;
}

static uint32  Esurfing_AddRef(IEsurfingMenu *pi)
{
    register CEsurfingMenu *pMe = (CEsurfingMenu*)pi;

    //ASSERT(pMe != NULL);
    //ASSERT(pMe->m_nRefs > 0);

    return (++pMe->m_nRefs);
}


static uint32  Esurfing_Release (IEsurfingMenu *pi)
{
	register CEsurfingMenu *pMe = (CEsurfingMenu*)pi;

	//ASSERT(pMe != NULL);

	if (pMe->m_nRefs == 0)
	{
	    return 0;
	}

	if (--pMe->m_nRefs)
	{
	    return pMe->m_nRefs;
	}

	Esurfing_FreeAppData(pMe);

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
static int Esurfing_InitAppData(CEsurfingMenu *pMe)
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
	pMe->m_ePreState = ESURFING_STATE_NONE;
	pMe->m_eCurState = ESURFING_STATE_INIT;
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
static void Esurfing_FreeAppData(CEsurfingMenu *pMe)
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
static void Esurfing_RunFSM(CEsurfingMenu *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = Esurfing_ProcessState(pMe);

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
static boolean Esurfing_HandleEvent(IEsurfingMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CEsurfingMenu *pMe = (CEsurfingMenu*)pi;
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
            pMe->m_rc.y = pMe->m_rc.y - ESURFING_MENU_SOFK_HIGHT;
            pMe->m_bSuspending = FALSE;

            Esurfing_RunFSM(pMe);
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
            pMe->m_rc.y = pMe->m_rc.y - ESURFING_MENU_SOFK_HIGHT;
            Esurfing_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;
            return Esurfing_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            return Esurfing_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) Esurfing_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            Esurfing_APPIsReadyTimer,
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
            return Esurfing_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void) Esurfing_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            if (pMe->m_bSuspending == FALSE)
            {
                Esurfing_RunFSM(pMe);
            }
            return TRUE;

        default:
            return Esurfing_RouteDialogEvent(pMe,eCode,wParam,dwParam);
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
static void Esurfing_APPIsReadyTimer(void *pme)
{
	CEsurfingMenu *pMe = (CEsurfingMenu *)pme;

	if (NULL == pMe)
	{
	    return;
	}

	(void) ISHELL_PostEvent( pMe->m_pShell,AEECLSID_ESURFING,EVT_APPISREADY,0,0);
}

#endif
