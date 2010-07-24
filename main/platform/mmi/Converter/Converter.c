/*==============================================================================
// �ļ���
//        Converter.c
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
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------ģ����غ�������---------------------*/

int  ConverterMod_Load(IShell *ps, void *pHelpers, IModule** ppMod);

int  ConverterMod_New(int16  nSize, IShell *ps, void *pHelpers, IModule **ppMod, PFNMODCREATEINST pfnMC, PFNFREEMODDATA pfnMF);

static int ConverterMod_CreateInstance(IModule *po, IShell *pIShell, AEECLSID ClsId,  void ** ppObj);

static uint32 ConverterMod_AddRef(IModule *po);

static uint32 ConverterMod_Release(IModule *po);

static void ConverterMod_FreeResources(IModule  *po, IHeap  *ph, IFileMgr *pfm);

/*----------------------Applet��غ�������---------------------*/
static int  Converter_New(IShell* pIShell, IModule* po, IConverter** ppObj);

static uint32  Converter_AddRef(IConverter *pi);

static uint32  Converter_Release (IConverter *pi);

static boolean Converter_HandleEvent(IConverter *pi, AEEEvent eCode, uint16  wParam, uint32 dwParam);

static int Converter_InitAppData(CConverter *pMe);

static void Converter_FreeAppData(CConverter *pMe);

static void Converter_RunFSM(CConverter *pMe);

static void Converter_APPIsReadyTimer(void *pme);

// ��ʼ����λ������˵�λ��
static void Converter_SetMenuPosition(CConverter *pMe);

/*==============================================================================
                                 ȫ������
==============================================================================*/
    // ��Ҫ��ʾ��
    //        g_dwAEEStdLibEntryȫ�ֱ���������SDK ��
    //        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ㣬��
    //        Ҫ�ƶ��������д��롣
#ifdef AEE_SIMULATOR
       uint32 g_dwAEEStdLibEntry;
#endif


/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
static ConverterMod gConverterMod;
    
static const VTBL(IModule) gModFuncs =
{
    ConverterMod_AddRef,
    ConverterMod_Release,
    ConverterMod_CreateInstance,
    ConverterMod_FreeResources
};


// ֻ����һ��Converterʵ����ÿ�δ���Converter Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CConverter gConverter;

static const VTBL(IConverter) gConverterMethods =
{
    Converter_AddRef,
    Converter_Release,
    Converter_HandleEvent
};

/*==============================================================================
����:
       ConverterMod_Load

˵��:
       ģ��װ�غ�����

����:
       ps [in]��IShell�ӿ�ָ�롣
       pHelpers ��û��ʹ��
       ppMod [out]��

����ֵ:
       int

��ע:

==============================================================================*/
#ifndef BREW_STATIC_APP
__declspec(dllexport) int AEEMod_Load(IShell   *ps,
                                                        void     *pHelpers,
                                                        IModule **ppMod)
#else
int  ConverterMod_Load(IShell *ps, void *pHelpers, IModule** ppMod)
#endif
{
#if !defined(BREW_STATIC_APP) && !defined(AEE_SIMULATOR)
    if (GET_HELPER_VER() != AEESTDLIB_VER)
    {
        return EVERSIONNOTSUPPORT;
    }
#endif
   return ConverterMod_New((sizeof(ConverterMod)), ps, pHelpers, ppMod, NULL, NULL);
}

/*==============================================================================
����: 
       ConverterMod_New
       
˵��: 
       ������ʼ��Converterģ�������Ҳ�������ü����������ֺ�����
       
����: 
       nSize:        û��ʹ��
       ps [In]: IShell�ӿ�ָ��
       pHelpers ��û��ʹ��
       ppMod [out]��
       pfnMC:        û��ʹ��
       pfnMF:        û��ʹ��
       
����ֵ:
       int
       
��ע:
       
==============================================================================*/
int ConverterMod_New(int16  nSize, IShell *ps, void *pHelpers, IModule **ppMod, PFNMODCREATEINST pfnMC, PFNFREEMODDATA pfnMF)
{
    if (NULL == ppMod)
    {
        return EFAILED;
    }

    *ppMod = NULL;

#ifdef AEE_SIMULATOR
        g_dwAEEStdLibEntry = (uint32)pHelpers;
        if (!ps || !pHelpers)
        {
            return EFAILED;
        }
#else

    if (!ps)
    {
        return EFAILED;
    }
#endif

    MEMSET(&gConverterMod,0,sizeof(ConverterMod) + sizeof(coefficient));

    INIT_VTBL(&gConverterMod,IModule,gModFuncs);
    gConverterMod.m_nRefs = 1;
    *ppMod = (IModule *)&gConverterMod;
    return AEE_SUCCESS;
}

/*==============================================================================
����: 
       ConverterMod_AddRef
       
˵��: 
       Converter ģ�����ü�����1�������ص�ǰ����������
       
����: 
       po [in]��IModule�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��Converter ģ���������
       
��ע:
       
==============================================================================*/
static uint32 ConverterMod_AddRef(IModule *po)
{
    return(++((ConverterMod *)po)->m_nRefs);
}

/*==============================================================================
����: 
       ConverterMod_Release
       
˵��: 
       Converter ģ�����ü�����1�������ص�ǰ����������
       
����: 
       po [in]��IModule�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��Converter ģ���������
       
��ע:
       
==============================================================================*/
static uint32 ConverterMod_Release(IModule *po)
{
    if (((ConverterMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((ConverterMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       ConverterMod_CreateInstance

˵��:
        ͨ������(��ʼ��)Converter����������ģ��ʵ����

����:
       pIShell [in]��IShell�ӿڶ���ָ�롣
       po [in]��IModule�ӿڶ���ָ�롣
       ClsId [in]��Ҫ������ʵ������ID
       ppObj [out]:�����ڱ������ʵ����ָ���ָ�������

����ֵ:
       EFAILED;����ʧ�ܡ�
       SUCCESS;�����ɹ���

��ע:
       �����û�ȥ���á�

==============================================================================*/
static int ConverterMod_CreateInstance(IModule *po, IShell *pIShell, AEECLSID ClsId, void ** ppObj)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_CONVERTER)
    {
        return EFAILED;
    }
    
    if(Converter_New(pIShell, po, (IConverter**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����: 
       ConverterMod_FreeResources
       
˵��: 
        �ͷ�Converterģ����Դ�ĺ�����
       
����: 
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣
       
����ֵ:
       ��
       
��ע:
       �����û�ȥ���á�
       
==============================================================================*/
static void ConverterMod_FreeResources(IModule  *po, IHeap  *ph, IFileMgr *pfm)
{
    PARAM_NOT_REF(po)
    PARAM_NOT_REF(ph)
    PARAM_NOT_REF(pfm)
}

/*==============================================================================
����:
       Converter_New

˵��:
       ������ʼ��Converterģ�������Ҳ�������ü����������ֺ�����

����:
       pIShell [In]: IShell�ӿ�ָ��
       po [in]��IModule�ӿڶ���ָ�롣
       ppObj [out]:�����ڱ������ʵ����ָ���ָ�������

����ֵ:
       int

��ע:

==============================================================================*/
static int  Converter_New(IShell* pIShell, IModule* po, IConverter** ppObj)
{
    int retVal = SUCCESS;

    if (0 == gConverter.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&gConverter,  0, sizeof(CConverter) + sizeof(coefficient));
        INIT_VTBL(&gConverter, IConverter, gConverterMethods);

        gConverter.m_nRefs    = 0;
        gConverter.m_pShell    = pIShell;
        gConverter.m_pModule = po;

        (void) ISHELL_AddRef(pIShell);
        (void) IMODULE_AddRef(po);

        retVal = Converter_InitAppData(&gConverter);
    }

    ++gConverter.m_nRefs;
    *ppObj = (IConverter*)&gConverter;
    return retVal; 
}

/*==============================================================================
����:
       Converter_AddRef

˵��:
       Converter Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��IConverter�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��Converter Applet ��������

��ע:

==============================================================================*/
static uint32  Converter_AddRef(IConverter *pi)
{
    register CConverter *pMe = (CConverter*)pi;

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       Converter_Release

˵��:
       �ͷŶ�Coverter Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��ICoverter�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��Coverter Applet��������

��ע:

==============================================================================*/
static uint32  Converter_Release (IConverter *pi)
{
    register CConverter* pMe = (CConverter*)pi;
   
    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if(--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    Converter_FreeAppData(pMe);
    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);
    
   return 0;
}

/*==============================================================================
����:
       Converter_HandleEvent

˵��:
       Converter  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��IConverter�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
static boolean Converter_HandleEvent(IConverter *pi, AEEEvent eCode, uint16  wParam, uint32 dwParam)
{
    CConverter *pMe = (CConverter*)pi;
    AEEAppStart* as = 0;
    switch (eCode)
    {
        case EVT_APP_START:
        {
            pMe->m_bAppIsReady = FALSE;
            as = (AEEAppStart*) dwParam;
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
			if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,AEECLSID_ANNUNCIATOR,(void **)&pMe->m_pIAnn))
		    {

		        return EFAILED;
		    }
			IANNUNCIATOR_SetFieldIsActiveEx(pMe->m_pIAnn,FALSE);
            pMe->m_bSuspending = FALSE;
            Converter_RunFSM(pMe);
            return TRUE;
        }

        case EVT_APP_STOP:
        {
            pMe->m_bSuspending = TRUE;
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            return TRUE;
        }

        case EVT_APP_SUSPEND:
        {
            pMe->m_bSuspending = TRUE;
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            return TRUE;
        }

        case EVT_APP_RESUME:
        {
            as = (AEEAppStart*) dwParam;
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

            Converter_RunFSM(pMe);
            return TRUE;
        }

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pDialog = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return Converter_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            return Converter_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void)Converter_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            Converter_APPIsReadyTimer,
                            pMe);
            return TRUE;
            
        case EVT_APPISREADY:
            (void) Converter_RouteDialogEvent(pMe,eCode,wParam,dwParam);
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
            return Converter_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            
        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void) Converter_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pDialog = NULL;

            if (pMe->m_bSuspending == FALSE)
            {
                Converter_RunFSM(pMe);
            }
            return TRUE;

        default:
            return Converter_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
   return FALSE;
}

/*==============================================================================
����:
       Converter_InitAppData

˵��:
       ��ʼ��Converter Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

==============================================================================*/
static int Converter_InitAppData(CConverter *pMe)
{
    AEEDeviceInfo di;

    if (NULL  == pMe)
    {
        return EFAILED;
    }
    ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
    pMe->m_rc.x             = 0;
    pMe->m_rc.y             = 0;
    pMe->m_rc.dx            = (int16) di.cxScreen;
    pMe->m_rc.dy            = (int16) di.cyScreen;
    pMe->m_bAppIsReady = FALSE;
    pMe->m_ePreState     = CONVERTER_NONE;
    pMe->m_eCurState     = CONVERTER_INIT; 
    pMe->m_pDialog         = NULL;
    pMe->m_eDlgRet        = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    pMe->m_nCtlID          = IDC_NUMBER1;
    pMe->m_selectedItem = 0;
    pMe->dyMenu            = MENUITEM_HEIGHT;
    pMe->basiccoefficient = 0;
    pMe->m_converterMode = CONVERTER_MODE_NONE;
    pMe->b_overflow = FALSE;
    (void)MEMSET(&pMe->m_CurrencyCFG, 0, sizeof(ConverterCurrencyCfg));
    
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                             AEECLSID_CONFIG,
                                             (void **)&pMe->m_pConfig))
    
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }

    if( ISHELL_CreateInstance( pMe->m_pShell,
                             AEECLSID_STATIC,
                             (void **)&pMe->m_pStatic
                         ) != SUCCESS
    )
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }    

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_LISTCTL,
                                 (void **) &pMe->pUnitMenu1))
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_LISTCTL,
                                 (void **) &pMe->pUnitMenu2))
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_TEXTCTL,
                                 (void **) &pMe->pNumber1))
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                 AEECLSID_TEXTCTL,
                                 (void **) &pMe->pNumber2))
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                AEECLSID_LISTCTL, 
                                (void **) &pMe->title))
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }
    
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                AEECLSID_MENUCTL, 
                                (void **) &pMe->currency))
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }
    
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                AEECLSID_TEXTCTL, 
                                (void **) &pMe->coeff1))
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }
    
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                AEECLSID_TEXTCTL, 
                                (void **) &pMe->coeff2)) 
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }
    
    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                AEECLSID_TEXTCTL, 
                                (void **) &pMe->coeff3))
    {
        Converter_FreeAppData( pMe);
        return EFAILED;
    }
    
    Converter_SetMenuPosition(pMe);

    return SUCCESS;
}

/*==============================================================================
����:
       Converter_FreeAppData

˵��:
       �ͷ�Converter Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void Converter_FreeAppData(CConverter *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }
    
    if(pMe->pNumber1)
    {
        ITEXTCTL_Release(pMe->pNumber1);
        pMe->pNumber1 = NULL;
    }

    if(pMe->pNumber2)
    {
        ITEXTCTL_Release(pMe->pNumber2);
        pMe->pNumber2 = NULL;
    }
    
    if(pMe->pUnitMenu1)
    {
        IMENUCTL_Release(pMe->pUnitMenu1);
        pMe->pUnitMenu1 = NULL;
    }
    
    if(pMe->pUnitMenu2)
    {
        IMENUCTL_Release(pMe->pUnitMenu2);
        pMe->pUnitMenu2 = NULL;
    }

    if(pMe->title)
    {
        IMENUCTL_Release(pMe->title);
        pMe->title = NULL;
    }
    
    if(pMe->currency)
    {
        IMENUCTL_Release(pMe->currency);
        pMe->currency = NULL;
    }
    
    if(pMe->coeff1)
    {
        ITEXTCTL_Release(pMe->coeff1);
        pMe->coeff1 = NULL;
    }
    
    if(pMe->coeff2)
    {
        ITEXTCTL_Release(pMe->coeff2);
        pMe->coeff2 = NULL;
    }
    
    if(pMe->coeff3)
    {
        ITEXTCTL_Release(pMe->coeff3);
        pMe->coeff3 = NULL;
    }
    
    if(coefficient)
    {
        FREE(coefficient);
    }
    
    if(pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    } 
    
    if( pMe->m_pStatic != NULL)
    {
        ISTATIC_Release(pMe->m_pStatic);
        pMe->m_pStatic = NULL;        
    }
	if(pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn = NULL;
    }
    
}

/*==============================================================================
����:
       Converter_RunFSM

˵��:
       Converte Applet����״̬�����档

����:
       pMe [in]��ָ��Converte Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void Converter_RunFSM(CConverter *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = Converter_ProcessState(pMe);

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
       Converter_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָConverter�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void Converter_APPIsReadyTimer(void *pme)
{
    CConverter *pMe = (CConverter *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_CONVERTER,
                            EVT_APPISREADY,
                            0,
                            0);
}

/*==============================================================================
������
       Converter_SetMenuPosition
˵����
       ��ʼ����λ������˵�λ�á�
       
������
       pMe [in]��ָ��Converter Applet����ṹ��ָ�롣
       
����ֵ��
       NONE
       
��ע��
       
==============================================================================*/
static void Converter_SetMenuPosition(CConverter *pMe)
{
    AEERect rect = {0};
    
    SETAEERECT(&rect, 
                        pMe->m_rc.x + ARROW_WIDTH + FRAME_SIZE, 
                        //TITLEBAR_HEIGHT + FRAME_SIZE, 
                        pMe->m_rc.y + TITLEBAR_HEIGHT/2 + FRAME_SIZE, 	//Add By zzg 2010_07_08
                        pMe->m_rc.dx - (2*(ARROW_WIDTH + FRAME_SIZE)), 
                        pMe->dyMenu - 2*FRAME_SIZE);
    IMENUCTL_SetRect(pMe->pUnitMenu1, &rect);

    rect.x -= ARROW_WIDTH;
    rect.dx += 2*ARROW_WIDTH;
    rect.y += pMe->dyMenu;		
	rect.dy += 5*FRAME_SIZE;					//Add By zzg 2010_07_07
    ITEXTCTL_SetRect(pMe->pNumber1, &rect);
	
    rect.x += ARROW_WIDTH;
    rect.dx -= 2*ARROW_WIDTH;
    //rect.y += pMe->dyMenu;		
    rect.y += (rect.dy + 5*FRAME_SIZE);			//Add By zzg 2010_07_07
	rect.dy -= 5*FRAME_SIZE; 					//Add By zzg 2010_07_07
    IMENUCTL_SetRect(pMe->pUnitMenu2, &rect);
    
    rect.x -= ARROW_WIDTH;
    rect.dx += 2*ARROW_WIDTH;
    rect.y += pMe->dyMenu;		
	rect.dy += 5*FRAME_SIZE;					//Add By zzg 2010_07_07
    ITEXTCTL_SetRect(pMe->pNumber2, &rect);
	rect.dy -= 5*FRAME_SIZE;					//Add By zzg 2010_07_07
	    
    rect.x += ARROW_WIDTH;
    rect.y = pMe->m_rc.y + TITLEBAR_HEIGHT/2 + FRAME_SIZE;
    rect.dx = pMe->m_rc.dx - (2*ARROW_WIDTH) - 2*FRAME_SIZE;		
    IMENUCTL_SetRect(pMe->title, &rect);
        
    rect.x   = pMe->m_rc.x;
    //rect.y += pMe->dyMenu;    
    rect.y += pMe->dyMenu - FRAME_SIZE;   		//Add By zzg 2010_07_08
    rect.dx = pMe->m_rc.dx;
    rect.dy = pMe->m_rc.dy - TITLEBAR_HEIGHT/2 - BOTTOMBAR_HEIGHT - pMe->dyMenu - STATUSBAR_HEIGHT;
    IMENUCTL_SetRect(pMe->currency, &rect);

    rect.x += FRAME_SIZE;
    rect.y = pMe->m_rc.y + TITLEBAR_HEIGHT/2 + 2*pMe->dyMenu + FRAME_SIZE;
    rect.dx = pMe->m_rc.dx - SCROLLBAR_WIDTH - 2*FRAME_SIZE;
    rect.dy = pMe->dyMenu - 2*FRAME_SIZE;	
    rect.y -= 3*FRAME_SIZE;						//Add By zzg 2010_07_08
    ITEXTCTL_SetRect(pMe->coeff1, &rect);
	rect.y += 3*FRAME_SIZE;						//Add By zzg 2010_07_08
	
    rect.y += (2*pMe->dyMenu);    
    ITEXTCTL_SetRect(pMe->coeff2, &rect);
	
    //rect.y += (2*pMe->dyMenu);	
    rect.y += (2*pMe->dyMenu - FRAME_SIZE);		//Add By zzg 2010_07_08
    ITEXTCTL_SetRect(pMe->coeff3, &rect);
	

	ITEXTCTL_SetProperties(pMe->pNumber1, TP_FRAME|TP_FOCUS_NOSEL|TP_FIXSETRECT);
    ITEXTCTL_SetProperties(pMe->pNumber2, TP_FRAME|TP_FOCUS_NOSEL|TP_FIXSETRECT);	
    ITEXTCTL_SetProperties(pMe->coeff1, TP_FRAME|TP_FOCUS_NOSEL);
    ITEXTCTL_SetProperties(pMe->coeff2, TP_FRAME|TP_FOCUS_NOSEL);
    ITEXTCTL_SetProperties(pMe->coeff3, TP_FRAME|TP_FOCUS_NOSEL);
    ITEXTCTL_SetCursorPos(pMe->pNumber1, TC_CURSOREND);
    ITEXTCTL_SetCursorPos(pMe->pNumber2, TC_CURSOREND);
    ITEXTCTL_SetCursorPos(pMe->coeff1, TC_CURSOREND);
    ITEXTCTL_SetCursorPos(pMe->coeff2, TC_CURSOREND);
    ITEXTCTL_SetCursorPos(pMe->coeff3, TC_CURSOREND);
    IMENUCTL_SetProperties(pMe->pUnitMenu1, MP_NO_REDRAW);
    IMENUCTL_SetProperties(pMe->pUnitMenu2, MP_NO_REDRAW);
    IMENUCTL_SetProperties(pMe->currency, MP_NO_REDRAW);
    IMENUCTL_SetProperties(pMe->title, MP_NO_REDRAW);
    IMENUCTL_SetOemProperties(pMe->title, OEMMP_IDF_ALIGN_CENTER);
    IMENUCTL_SetOemProperties(pMe->pUnitMenu1, OEMMP_IDF_ALIGN_CENTER);
    IMENUCTL_SetOemProperties(pMe->pUnitMenu2, OEMMP_IDF_ALIGN_CENTER);
    IMENUCTL_SetOemProperties(pMe->currency, OEMMP_USE_MENU_STYLE);
}

