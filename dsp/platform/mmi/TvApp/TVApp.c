/*==============================================================================
//           CAMERA  A P P L E T  M O D U L E
// �ļ�:
//        TVApp.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        ��������ģ�����ļ�
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
#include "TVApp_priv.h"      // ��Appletͷ�ļ�

/*==============================================================================

  �궨��ͳ���
  
==============================================================================*/


/*==============================================================================

  ����������Ͷ���
  
==============================================================================*/


/*==============================================================================

  ���غ�������
  
==============================================================================*/
//ģ����غ�������

int CTVAppMod_Load(IShell *pIShell, void *ph, IModule **ppMod);

int CTVAppMod_New(int16                nSize, 
                      IShell               *pIShell,
                      void                 *ph,   
                      IModule              **ppMod,
                      PFNMODCREATEINST     pfnMC, 
                      PFNFREEMODDATA       pfnMF);

static uint32 CTVAppMod_AddRef(IModule *po);

static uint32 CTVAppMod_Release(IModule *po);

static int CTVAppMod_CreateInstance(IModule  *po, 
                                        IShell   *pIShell,
                                        AEECLSID ClsId, 
                                        void     **ppObj);

static void CTVAppMod_FreeResources(IModule *po, IHeap *ph, IFileMgr *pfm);

// Applet��غ�������
// Applet�������ͷź���
static int TVApp_New(IShell *ps, IModule *pIModule, ITVApp **ppObj);

static uint32  TVApp_AddRef(ITVApp *pi);

static uint32  TVApp_Release(ITVApp *pi);

// Applet�¼�������
static boolean TVApp_HandleEvent(ITVApp   *pi, 
                                     AEEEvent     eCode,
                                     uint16       wParam, 
                                     uint32       dwParam);

// �ڲ�Applet��������
static int TVApp_InitAppData(CTVApp *pMe);

static void TVApp_FreeAppData(CTVApp *pMe);

// Applet��״̬��������
static void TVApp_RunFSM(CTVApp *pMe);

// ��������֪ͨ�¼��Ĵ�����
//static boolean TVApp_HandleNotifyEvent(CTVApp *pMe, AEENotify *pN);

static void TVApp_APPIsReadyTimer(void *pme);



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
static CTVAppMod gTVAppMod;

static const VTBL(IModule) gModFuncs =
{
    CTVAppMod_AddRef,    
    CTVAppMod_Release,
    CTVAppMod_CreateInstance,
    CTVAppMod_FreeResources
};

// ֻ����һ�� TVApp ʵ����ÿ�δ��� TV Applet ʱ������ͬһ�ṹָ��� BREW �㡣
static CTVApp gTVApp;//={0};

static const VTBL(ITVApp) gTVAppMethods =
{
    TVApp_AddRef,
    TVApp_Release,
    TVApp_HandleEvent
};

/*==============================================================================
����:
       CTVAppMod_Load

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
#ifndef AEE_STATIC
	__declspec(dllexport) int AEEMod_Load(IShell   *pIShell,
										  void	   *ph,
										  IModule  **ppMod)
#else

    int CTVAppMod_Load(IShell   *pIShell,
                           void     *ph,
                           IModule  **ppMod)
#endif
{
    // ���ڶ�̬Applet����֤AEE STD��汾����ȷ�����뽨����ģ��ʱʹ�õĿ�汾ƥ��
    // ����AEE_SIMULATOR�²��ؽ��б���顣
#if !defined(BREW_STATIC_APP) && !defined(AEE_SIMULATOR)
    if(GET_HELPER_VER() != AEESTDLIB_VER)
    {
        return EVERSIONNOTSUPPORT;
    }
#endif
MSG_FATAL("CTVAppMod_Load================================",0,0,0);
    return CTVAppMod_New(sizeof(CTVAppMod), pIShell, ph, ppMod, NULL, NULL);
}

/*==============================================================================
����:
       CTVAppMod_New

˵��:
       ������ʼ��TVAppģ�������Ҳ�������ü����������ֺ�����
  
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
int CTVAppMod_New(int16             nSize,
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
  
    MEMSET(&gTVAppMod,0,sizeof(CTVAppMod));
  
    INIT_VTBL(&gTVAppMod,IModule,gModFuncs);
  
    gTVAppMod.m_nRefs = 1;
  
    *ppMod = (IModule *)&gTVAppMod;
  
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       CTVAppMod_AddRef

˵��:
       TVAppģ�����ü�����1�������ص�ǰ����������
  
����:
       po [in/out]:IModule�ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ��TVAppģ���������
   
��ע:
  
==============================================================================*/
static uint32 CTVAppMod_AddRef(IModule *po)
{
    return(++((CTVAppMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       CTVAppMod_Release

˵��:
       TVAppģ�����ü�����1�������ص�ǰ����������

����:
       po [in/out]:IModule�ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ��TVAppģ���������
   
��ע:
  
==============================================================================*/
static uint32 CTVAppMod_Release(IModule *po)
{
    if (((CTVAppMod *) po)->m_nRefs == 0)
    {
        return 0;
    }
 
    return(--((CTVAppMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       CTVAppMod_CreateInstance

˵��:
       ͨ������(��ʼ��)TVApp����������ģ��ʵ����
  
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
static int CTVAppMod_CreateInstance(IModule   *po,
                                        IShell    *pIShell,
                                        AEECLSID  ClsId,
                                        void      **ppObj)
{
    *ppObj = NULL;
 
    if(ClsId != AEECLSID_TVAPP)
    {
        return EFAILED;
    }
 
    if(TVApp_New(pIShell, po, (ITVApp**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
 
    return SUCCESS;
}

/*==============================================================================
����:
       CTVAppMod_FreeResources

˵��:
       �ͷ�TVAppģ����Դ�ĺ�����

����:
       po :û��ʹ�á�
       IHeap :û��ʹ�á�
       pfm ::û��ʹ�á�
 
����ֵ:
       none
   
��ע:
       �����û�ȥ���á�
  
==============================================================================*/
static void CTVAppMod_FreeResources(IModule *po, IHeap  *ph, IFileMgr *pfm)
{
    PARAM_NOT_REF(po)
    PARAM_NOT_REF(ph)
    PARAM_NOT_REF(pfm)
}

/*==============================================================================
����:
       TVApp_New

˵��:
       ��ʼ�� TVApp Applet �������ڲ����ݱ�����ͬʱ TVApp Applet ���ü�����1��
  
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
static int TVApp_New(IShell *ps, IModule *pIModule, ITVApp **ppObj)
{
    int retVal = SUCCESS;
   // ITvUtil* pITvUtil = NULL;
	//CTVApp* pThis = NULL;
	
    if(0 == gTVApp.m_nRefs)
    {
        MEMSET(&gTVApp, 0, sizeof(gTVApp));
        INIT_VTBL(&gTVApp, ITVApp, gTVAppMethods);
  
        gTVApp.m_nRefs = 0;
        gTVApp.m_pShell = ps;
        gTVApp.m_pModule = pIModule;
        gTVApp.pIMMITv=NULL;
        (void)ISHELL_AddRef(ps);
        (void)IMODULE_AddRef(pIModule);
  
        retVal = TVApp_InitAppData(&gTVApp);
		//pThis = CTvUtil_Constructor(ps);
    }
 
    ++gTVApp.m_nRefs;
 
    *ppObj = (ITVApp*)&gTVApp;
 MSG_FATAL("TVApp_New-----------------%d",retVal,0,0);
    return retVal;
}

/*==============================================================================
����:
       TVApp_AddRef

˵��:
       TVApp Applet ���ü�����1�������ص�ǰ����������
  
����:
       pi [in/out]: ITVApp �ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ�� TVApp Applet ��������
   
��ע:
  
==============================================================================*/
static uint32 TVApp_AddRef(ITVApp *pi)
{
    register CTVApp *pMe = (CTVApp*)pi;
 
    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);
 
    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       TVApp_Release

˵��:
       �ͷŶ� TVApp Applet �����ã����ü�����1�������ص�ǰ����������
       �����ǰ��������Ϊ�㣬�����ͷ� Applet �������Դ��
  
����:
       pi [in/out]: ITVApp �ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ�� TV Applet ��������
   
��ע:
  
==============================================================================*/
static uint32 TVApp_Release(ITVApp *p)
{
    register CTVApp *pMe = (CTVApp*)p;
 
    if(pMe->m_nRefs == 0)
    {
        return 0;
    }
 
    if(--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }
 
    TVApp_FreeAppData(pMe);
 
    (void)ISHELL_Release(pMe->m_pShell);
    (void)IMODULE_Release(pMe->m_pModule);
	if(pMe->pTvSetting)
	{
		FREE(pMe->pTvSetting);
		pMe->pTvSetting = NULL;
	}
 
    // ע��:pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       TVApp_InitAppData

˵��:
       ��ʼ�� TVApp Applet �ṹ����ȱʡֵ��
  
����:
       pMe [in]: ָ�� TVApp Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
 
����ֵ:
       EFAILED: ��ʼ��ʧ�ܡ�
       SUCCESS: ��ʼ���ɹ���
   
��ע:
  
==============================================================================*/
static int TVApp_InitAppData(CTVApp *pMe)
{
    AEEDeviceInfo  di;
	int Ret = 0;
    uint32 m_tv = 1;   
    if(NULL == pMe)
    {
        return EFAILED;
    }
    
    (void)OEM_SetConfig( CFGI_TV_OR_CAMERA,
	                          &m_tv,
	                          sizeof(uint32));
    // initial applet data
    ISHELL_GetDeviceInfo(pMe->m_pShell, &di);
    pMe->m_rc.x       = 0;
    pMe->m_rc.y       = 0;
    pMe->m_rc.dx      = (int16) di.cxScreen;
    pMe->m_rc.dy      = (int16) di.cyScreen;
 
    pMe->m_cxWidth    = di.cxScreen; 
    pMe->m_cyHeight   = di.cyScreen;
 
    pMe->m_bAppIsReady = FALSE;
    pMe->m_pActiveDlg = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    pMe->m_bIsPreview = FALSE;
    
    pMe->m_bCapturePic = FALSE;
    pMe->m_ePreState = STATE_NULL;
    pMe->m_nMainMenuItemSel = IDS_ITEM_TV;
    pMe->m_nSelfTimeItemSel = IDS_SELFTIME_OFF;
    pMe->m_nTVCFG = TVSETChannel;
    pMe->m_sCurrentFileName[0] = '\0';
    pMe->m_sCaptureFileName[0] = '\0';
    pMe->m_bCanCapture = TRUE;
    pMe->m_dwMemTotal = 0;
    pMe->m_dwMemFree = 0;
    pMe->m_barW=0;
    pMe->m_barMAXW=1;
    

    //Video��ʱ��
    pMe->m_dwDispTime = 0;
    pMe->m_nNumDays = 0;
    pMe->m_nTicks = 0;
    pMe->m_bRePreview = FALSE;
    pMe->m_nTVState = TV_START;

    pMe->m_pTV = NULL;  
    pMe->m_pMedia = NULL;
    pMe->m_isFormQuicktest = FALSE;
    pMe->m_sensor_model = -1;
	
	pMe->pTvSetting =(CFG_TvSetting*)MALLOC(sizeof(CFG_TvSetting));
	MEMSET(pMe->pTvSetting,0,sizeof(CFG_TvSetting));
    MEMSET(&pMe->m_CallBack, 0, sizeof(AEECallback));
    
MSG_FATAL("TVApp_InitAppData-----------------START",0,0,0);
   
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                             AEECLSID_CONFIG,
                                             (void **)&pMe->m_pConfig))
    {
        TVApp_FreeAppData(pMe);
        return EFAILED;
    }
 
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_DISPLAY,
                                            (void **)&pMe->m_pDisplay))
    {
        TVApp_FreeAppData(pMe);
		MSG_FATAL("AEECLSID_DISPLAY-----------------%d",EFAILED,0,0);
        return EFAILED;
    } 
    
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                            AEECLSID_FILEMGR, 
                                            (void **)&pMe->m_pFileMgr))
    {
        TVApp_FreeAppData(pMe);
		MSG_FATAL("AEECLSID_FILEMGR-----------------%d",EFAILED,0,0);
        return EFAILED;
    }
    
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                              AEECLSID_GRAPHICS, 
                                              (void **)&pMe->pGraphics))
      {
          TVApp_FreeAppData(pMe);
          MSG_FATAL("AEECLSID_GRAPHICS-----------------%d",EFAILED,0,0);
          return EFAILED;
      }

    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                            AEECLSID_ANNUNCIATOR, 
                                            (void **)&pMe->m_pIAnn))
    {
        TVApp_FreeAppData(pMe);
		MSG_FATAL("AEECLSID_ANNUNCIATOR-----------------%d",EFAILED,0,0);
        return EFAILED;
    }
    
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, AEECLSID_BACKLIGHT, (void**)&pMe->m_pBacklight))
    {
        TVApp_FreeAppData(pMe);
		MSG_FATAL("AEECLSID_BACKLIGHT-----------------%d",EFAILED,0,0);
        return EFAILED;
    }
    
    (void)ICONFIG_GetItem(pMe->m_pConfig,
                          CFGI_BACK_LIGHT,
                         &pMe->m_nBacklightVal,
                          sizeof(pMe->m_nBacklightVal));

#if 0
      {
      int num = 6;
      	int i = 0;
      	int nSize = 0;	 
          MSG_FATAL("CTvUtil_BookmarkOperator_RemoveAll Start",0,0,0); 
              (void)ICONFIG_GetItem(pMe->m_pConfig,
                               CFGI_TV_SETCHANNL,
                               (void*)pMe->pTvSetting,
                               sizeof(CFG_TvSetting));
      	for(; i < num; i++)
      	{
      		nSize = WSTRLEN(pMe->pTvSetting->Bookmark[i].name); 
      		MEMSET (pMe->pTvSetting->Bookmark[i].name, '\0', sizeof(AECHAR) * nSize); 
      
      		nSize = WSTRLEN(pMe->pTvSetting->Bookmark[i].channel); 
      		MEMSET (pMe->pTvSetting->Bookmark[i].channel, '\0', sizeof(AECHAR) * nSize); 
      	}
      	pMe->pTvSetting->Bookmarktotal = 0;
      	pMe->pTvSetting->ChannelCountAble = 0;
           (void)ICONFIG_SetItem(pMe->m_pConfig,
                            CFGI_TV_SETCHANNL,
                            (void*)pMe->pTvSetting,
                            sizeof(CFG_TvSetting));
                 
        }
#endif 
	MSG_FATAL("TVApp_InitAppData-----------------end--%d",SUCCESS,0,0);
    return SUCCESS;

	
} 



/*==============================================================================
����:
       TVApp_FreeAppData

˵��:
       �ͷ� TVApp Applet �����Դ��
  
����:
       pMe [in]: ָ��TVApp Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
 
����ֵ:
       none
   
��ע:
  
==============================================================================*/
static void TVApp_FreeAppData(CTVApp *pMe)
{
    if(NULL == pMe)
    {
        return;
    }
    
    if(pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }
     
    if(pMe->m_pDisplay)
    {
        IDISPLAY_Release(pMe->m_pDisplay);
        pMe->m_pDisplay = NULL;
    }

    if(pMe->pGraphics)
      {
         // IDISPLAY_Release(pMe->pGraphics);
          pMe->pGraphics = NULL;
      }

    if(pMe->m_pTV) 
    {
        ICAMERA_Release(pMe->m_pTV);
        pMe->m_pTV = NULL;
    }

    if(pMe->m_pFileMgr)
    {
        IFILEMGR_Release(pMe->m_pFileMgr);
        pMe->m_pFileMgr = NULL;            
    }

    if(pMe->m_pMedia)
    {
        IMEDIA_Release(pMe->m_pMedia);
        pMe->m_pMedia = NULL;
    }

    if(pMe->m_pIAnn)
    {
        IANNUNCIATOR_Release(pMe->m_pIAnn);
        pMe->m_pIAnn = NULL;
    }
    
    if(pMe->m_pBacklight)
    {
        IBACKLIGHT_Release(pMe->m_pBacklight);
        pMe->m_pBacklight = NULL;
    }
// if SD Dev is existed, stop SD dev while exitting from TV APP to avoid high current<43mA> consume in idle state
    // Note: if SD card detected after Creating ITV interface, it needn't to stop sd card in this way, cause's SD won't be pushed in stack for low dev priority
    if(pMe->m_bMemoryCardExist)
    {
    #ifdef FEATURE_SUPPORT_VC0848
        vc_union_type vc_data;
        vc_data.dev_run.curr_dev = VC_DEV_SD;
        VC_DeviceControl(VC_ITM_DEV_STOP_I, VC_FUNC_CONTROL, &vc_data);
    #endif
    }
}

/*==============================================================================
����:
       TVApp_RunFSM
    
˵��:
       TVApp Applet����״̬�����档
   
����:
       pMe [in]��ָ��TVApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
     
����ֵ:
       �ޡ�
    
��ע:
      
==============================================================================*/
static void TVApp_RunFSM(CTVApp *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;
     
    for( ; ; )
    {
        nextFSMAction = TVApp_ProcessState(pMe);
      
        if(pMe->m_bNotOverwriteDlgRet)
        {
            pMe->m_bNotOverwriteDlgRet = FALSE;
        }
        else
        {
            pMe->m_eDlgRet = DLGRET_CREATE;
        }
      
        if(nextFSMAction == NFSMACTION_WAIT)
        {
            break;
        }
    }
}
    
/*==============================================================================
����:
       TVApp_HandleEvent
    
˵��:
       TVApp Applet�¼��������������������ڱ�Applet���¼����������ɸú�������
   
����:
       pi [in]:ָ��ITVApp�ӿ�t�����ָ�롣
       eCode:BREW�¼����롣
       wParam:�¼�����
       dwParam [in]:��wParam���������ݡ���
     
����ֵ:
       TRUE:�����¼�������
       FALSE:�����¼�û������
    
��ע:
       �ú�����BREW���ã��û����õ��á�
      
==============================================================================*/
static boolean TVApp_HandleEvent(ITVApp  *pi,
                                     AEEEvent     eCode,
                                     uint16       wParam,
                                     uint32       dwParam)
{
    CTVApp *pMe = (CTVApp*)pi;   
    AEEAppStart *as;    
    int  nAscent,nDescent;

    MSG_FATAL("TVApp_HandleEvent-----------------------0x%x",eCode,0,0);
    switch(eCode)
    {
        case EVT_APP_START:
			MSG_FATAL("TVApp_HandleEvent--------EVT_APP_START---------------",0,0,0);
            pMe->m_bAppIsReady = FALSE;
            ASSERT(dwParam != 0);
            as = (AEEAppStart*)dwParam;
            if(NULL != as->pDisplay)
            {
                if(NULL != pMe->m_pDisplay)
                {
                    (void)IDISPLAY_Release(pMe->m_pDisplay);
                    pMe->m_pDisplay = NULL;
                }
          
                pMe->m_pDisplay = as->pDisplay;
                (void)IDISPLAY_AddRef(pMe->m_pDisplay);
            }
            if(as->pszArgs != NULL)
            {
                if(STRCMP(as->pszArgs, "Formquicktest") == 0)
                {
                    pMe->m_isFormQuicktest = TRUE;
                }
            }
            pMe->m_rc = as->rc;
            pMe->m_bSuspending = FALSE;
            // ���ϵͳ�������С
            (void)IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_NORMAL, &nAscent, &nDescent);
            pMe->m_nItemH  = nAscent + nDescent;

            {
                int nDevice;
                nDevice = app_media_scheduler();

                switch(nDevice)
                {
                    case APP_MEDIA_ALLOW:
                        pMe->m_eCurState = STATE_CMAINMENU;                               
                        break;

                    case APP_MEDIA_IMPACT_BY_MP3:                    
                    {          
                    	//Del By zzg 2010_09_01
                    	/*
                    	pMe->m_eCurState = STATE_CPOPMSG;
                        pMe->m_nMsgTimeout = 0; 
                        pMe->m_wMsgID = IDS_CONFIRM_OFF_MP;
                        */
                        //Del End
                        
                        //Add By zzg 2010_09_01
                        pMe->m_eCurState = STATE_CMAINMENU;    
                        break;
                        //Add End                        
                        
                    }

                    case APP_MEDIA_IMPACT_BY_FM:
                        pMe->m_eCurState = STATE_CPOPMSG;
                        pMe->m_nMsgTimeout = 0; 
                        pMe->m_wMsgID = IDS_CONFIRM_OFF_FM;
                        break;

                    default:
                        break;                   
                }
            }

            SetDeviceState(DEVICE_TYPE_CAMERA, DEVICE_CAMERA_STATE_ON);
      
            // ��ʼSettingMenu״̬��
            TVApp_RunFSM(pMe);
            return TRUE;
      
        case EVT_APP_STOP:
            if(pMe->m_pDisplay != NULL)
            {
                (void)IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_bSuspending = TRUE; 
            SetDeviceState(DEVICE_TYPE_CAMERA, DEVICE_CAMERA_STATE_OFF);
            if(pMe->m_bIsPreview == TRUE && pMe->m_pTV)
            {
                //ICAMERAEX_RegisterNotify(pMe->m_pTV,NULL, NULL);
                ICAMERA_Stop(pMe->m_pTV);
            }
            (void)ICONFIG_SetItem(pMe->m_pConfig,
                                  CFGI_BACK_LIGHT,
                                  &pMe->m_nBacklightVal,
                                  sizeof(pMe->m_nBacklightVal));
            IBACKLIGHT_Enable(pMe->m_pBacklight);
            return TRUE;
      
        case EVT_APP_SUSPEND:
            ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);// ȡ�����ж�ʱ��
            
            pMe->m_bSuspending = TRUE;
            if(pMe->m_bIsPreview == TRUE && pMe->m_pTV)
            {
               // ICAMERAEX_RegisterNotify(pMe->m_pTV,NULL, NULL);
                ICAMERA_Stop(pMe->m_pTV);
            }
            
            if(pMe->m_nTVState == TV_START && pMe->m_pTV)
            {
                ICAMERA_Release(pMe->m_pTV);
                pMe->m_pTV = NULL;
            }
            pMe->m_bIsPreview = FALSE;
            return TRUE;
            
        case EVT_ALARM:
            if(pMe->m_nTVState == TV_START && pMe->m_pTV)
            {
                ICAMERA_Release(pMe->m_pTV);
                pMe->m_pTV = NULL;
            }
            pMe->m_bIsPreview = FALSE;
            return TRUE;
            
        case EVT_APP_RESUME:
           // TVApp_InitTVCheck(pMe); 
            as = (AEEAppStart*)dwParam;
            pMe->m_bSuspending = FALSE;
      
            if(pMe->m_pDisplay != NULL)
            {
                (void)IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_pDisplay = as->pDisplay;
            (void)IDISPLAY_AddRef(pMe->m_pDisplay);
			
			pMe->m_nLeftTime = 0;
			
            pMe->m_rc = as->rc;
            {
                byte nBackLight;
                nBackLight = OEMNV_BL_ALWAYS_ON;
                (void)ICONFIG_SetItem(pMe->m_pConfig,
                                      CFGI_BACK_LIGHT,
                                      &nBackLight,
                                      sizeof(nBackLight));
                IBACKLIGHT_Enable(pMe->m_pBacklight);     
            }
            TVApp_RunFSM(pMe);
            return TRUE;
      
        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;   
            return TVApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
      
        case EVT_DIALOG_START:
            (void)ISHELL_SetTimer(pMe->m_pShell,
                                  APPISREADY_TIMER,
                                  TVApp_APPIsReadyTimer,
                                  pMe);
            return TVApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
            
        case EVT_USER_REDRAW:
            (void)TVApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
            return TRUE;
            
        case EVT_CAMERA_NOTIFY:
            TVApp_AppEventNotify(pMe, (int16)wParam, (int16)dwParam);
            return TRUE;
            
        case EVT_APPISREADY:
            pMe->m_bAppIsReady = TRUE;
            return TRUE;
      
        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_KEY:
        case EVT_COMMAND:         
            if(!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			MSG_FATAL("case EVT_COMMAND:  ----------------------",0,0,0);
            return TVApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
      
        case EVT_DIALOG_END:
            if(wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void)TVApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
            pMe->m_pActiveDlg = NULL; 
            
            //Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            //�¼���EVT_DIALOG_END�¼�ǰ������
            if(pMe->m_bSuspending == FALSE)
            {
                //��ʼTVApp״̬��
                TVApp_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//WLH ADD FOR LCD TOUCH

			  case EVT_USER:
				  if(wParam == AVK_CLR)
				  {
					  eCode = EVT_KEY;
				  }
				  else if((wParam == AVK_SELECT) || (wParam == AVK_INFO))
				  {
					  if(dwParam != 0)
						  {
							  eCode = EVT_COMMAND;
							  wParam = dwParam;
							  dwParam = 0;
						  }
						  else
						  {
							  eCode = EVT_KEY;
						  }
				  }
				  return TVApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
			  break;
#endif      
        default:
            //�����յ����¼�·������ǰ��ĶԻ����¼���������
            return TVApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
    }

    return FALSE;
} //TVApp_HandleEvent()

/*==============================================================================
����:
       TVApp_APPIsReadyTimer
  
˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���
 
����:
       pme [in]��void *����ָ�룬��ָCTVApp�ṹָ�롣
   
����ֵ:
       �ޡ�
  
��ע:
    
==============================================================================*/
static void TVApp_APPIsReadyTimer(void *pme)
{
    CTVApp *pMe = (CTVApp *)pme;
    
    if(NULL == pMe)
    {
        return;
    }
    
    (void)ISHELL_PostEvent(pMe->m_pShell,
                           AEECLSID_TVAPP,
                           EVT_APPISREADY,
                           0,
                           0);
}






