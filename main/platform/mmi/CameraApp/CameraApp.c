/*==============================================================================
//           CAMERA  A P P L E T  M O D U L E
// �ļ�:
//        CameraApp.c
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
#include "CameraApp_priv.h"      // ��Appletͷ�ļ�

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

int CCameraAppMod_Load(IShell *pIShell, void *ph, IModule **ppMod);

int CCameraAppMod_New(int16                nSize, 
                      IShell               *pIShell,
                      void                 *ph,   
                      IModule              **ppMod,
                      PFNMODCREATEINST     pfnMC, 
                      PFNFREEMODDATA       pfnMF);

static uint32 CCameraAppMod_AddRef(IModule *po);

static uint32 CCameraAppMod_Release(IModule *po);

static int CCameraAppMod_CreateInstance(IModule  *po, 
                                        IShell   *pIShell,
                                        AEECLSID ClsId, 
                                        void     **ppObj);

static void CCameraAppMod_FreeResources(IModule *po, IHeap *ph, IFileMgr *pfm);

// Applet��غ�������
// Applet�������ͷź���
static int CameraApp_New(IShell *ps, IModule *pIModule, ICameraApp **ppObj);

static uint32  CameraApp_AddRef(ICameraApp *pi);

static uint32  CameraApp_Release(ICameraApp *pi);

// Applet�¼�������
static boolean CameraApp_HandleEvent(ICameraApp   *pi, 
                                     AEEEvent     eCode,
                                     uint16       wParam, 
                                     uint32       dwParam);

// �ڲ�Applet��������
static int CameraApp_InitAppData(CCameraApp *pMe);

static void CameraApp_FreeAppData(CCameraApp *pMe);

// Applet��״̬��������
static void CameraApp_RunFSM(CCameraApp *pMe);

// ��������֪ͨ�¼��Ĵ�����
//static boolean CameraApp_HandleNotifyEvent(CCameraApp *pMe, AEENotify *pN);

static void CameraApp_APPIsReadyTimer(void *pme);

/*==============================================================================
ȫ������
==============================================================================*/
// ��Ҫ��ʾ::
//        g_dwAEEStdLibEntryȫ�ֱ���������SDK !
//        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ㣬��
//        Ҫ�ƶ��������д��롣
#ifdef AEE_SIMULATOR
uint32 g_dwAEEStdLibEntry;
//   #error The Camera applet was never intended to be used with the AEE Simulator.
#endif

/*==============================================================================

  ���أ���̬������
  
==============================================================================*/
static CCameraAppMod gCameraAppMod;

static const VTBL(IModule) gModFuncs =
{
    CCameraAppMod_AddRef,    
    CCameraAppMod_Release,
    CCameraAppMod_CreateInstance,
    CCameraAppMod_FreeResources
};

// ֻ����һ�� CameraApp ʵ����ÿ�δ��� Camera Applet ʱ������ͬһ�ṹָ��� BREW �㡣
static CCameraApp gCameraApp;//={0};

static const VTBL(ICameraApp) gCameraAppMethods =
{
    CameraApp_AddRef,
    CameraApp_Release,
    CameraApp_HandleEvent
};

/*==============================================================================
����:
       CCameraAppMod_Load

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
                                      void     *ph,
                                      IModule  **ppMod)
#else
    int CCameraAppMod_Load(IShell   *pIShell,
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
    return CCameraAppMod_New(sizeof(CCameraAppMod), pIShell, ph, ppMod, NULL, NULL);
}

/*==============================================================================
����:
       CCameraAppMod_New

˵��:
       ������ʼ��CameraAppģ�������Ҳ�������ü����������ֺ�����
  
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
int CCameraAppMod_New(int16             nSize,
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
  
    MEMSET(&gCameraAppMod,0,sizeof(CCameraAppMod));
  
    INIT_VTBL(&gCameraAppMod,IModule,gModFuncs);
  
    gCameraAppMod.m_nRefs = 1;
  
    *ppMod = (IModule *)&gCameraAppMod;
  
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       CCameraAppMod_AddRef

˵��:
       CameraAppģ�����ü�����1�������ص�ǰ����������
  
����:
       po [in/out]:IModule�ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ��CameraAppģ���������
   
��ע:
  
==============================================================================*/
static uint32 CCameraAppMod_AddRef(IModule *po)
{
    return(++((CCameraAppMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       CCameraAppMod_Release

˵��:
       CameraAppģ�����ü�����1�������ص�ǰ����������

����:
       po [in/out]:IModule�ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ��CameraAppģ���������
   
��ע:
  
==============================================================================*/
static uint32 CCameraAppMod_Release(IModule *po)
{
    if (((CCameraAppMod *) po)->m_nRefs == 0)
    {
        return 0;
    }
 
    return(--((CCameraAppMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       CCameraAppMod_CreateInstance

˵��:
       ͨ������(��ʼ��)CameraApp����������ģ��ʵ����
  
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
static int CCameraAppMod_CreateInstance(IModule   *po,
                                        IShell    *pIShell,
                                        AEECLSID  ClsId,
                                        void      **ppObj)
{
    *ppObj = NULL;
 
    if(ClsId != AEECLSID_APP_CAMERA)
    {
        return EFAILED;
    }
 
    if(CameraApp_New(pIShell, po, (ICameraApp**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
 
    return SUCCESS;
}

/*==============================================================================
����:
       CCameraAppMod_FreeResources

˵��:
       �ͷ�CameraAppģ����Դ�ĺ�����

����:
       po :û��ʹ�á�
       IHeap :û��ʹ�á�
       pfm ::û��ʹ�á�
 
����ֵ:
       none
   
��ע:
       �����û�ȥ���á�
  
==============================================================================*/
static void CCameraAppMod_FreeResources(IModule *po, IHeap  *ph, IFileMgr *pfm)
{
    PARAM_NOT_REF(po)
    PARAM_NOT_REF(ph)
    PARAM_NOT_REF(pfm)
}

/*==============================================================================
����:
       CameraApp_New

˵��:
       ��ʼ�� CameraApp Applet �������ڲ����ݱ�����ͬʱ CameraApp Applet ���ü�����1��
  
����:
       ps [in]: IShell�ӿڶ���ָ�롣
       pIModule [in/out]: pIModule�ӿڶ���ָ�롣
       ppObj [out]: ָ�� ICameraApp ����ָ���ָ�롣
 
����ֵ:
       EFAILED: ʧ�ܡ�
       SUCCESS: �ɹ���
   
��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�
  
==============================================================================*/
static int CameraApp_New(IShell *ps, IModule *pIModule, ICameraApp **ppObj)
{
    int retVal = SUCCESS;
 
    if(0 == gCameraApp.m_nRefs)
    {
        MEMSET(&gCameraApp, 0, sizeof(gCameraApp));
        INIT_VTBL(&gCameraApp, ICameraApp, gCameraAppMethods);
  
        gCameraApp.m_nRefs = 0;
        gCameraApp.m_pShell = ps;
        gCameraApp.m_pModule = pIModule;
  
        (void)ISHELL_AddRef(ps);
        (void)IMODULE_AddRef(pIModule);
  
        retVal = CameraApp_InitAppData(&gCameraApp);
    }
 
    ++gCameraApp.m_nRefs;
 
    *ppObj = (ICameraApp*)&gCameraApp;
 
    return retVal;
}

/*==============================================================================
����:
       CameraApp_AddRef

˵��:
       CameraApp Applet ���ü�����1�������ص�ǰ����������
  
����:
       pi [in/out]: ICameraApp �ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ�� CameraApp Applet ��������
   
��ע:
  
==============================================================================*/
static uint32 CameraApp_AddRef(ICameraApp *pi)
{
    register CCameraApp *pMe = (CCameraApp*)pi;
 
    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);
 
    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       CameraApp_Release

˵��:
       �ͷŶ� CameraApp Applet �����ã����ü�����1�������ص�ǰ����������
       �����ǰ��������Ϊ�㣬�����ͷ� Applet �������Դ��
  
����:
       pi [in/out]: ICameraApp �ӿڶ���ָ�롣
 
����ֵ:
       ��ǰ�� Camera Applet ��������
   
��ע:
  
==============================================================================*/
static uint32 CameraApp_Release(ICameraApp *p)
{
    register CCameraApp *pMe = (CCameraApp*)p;
 
    if(pMe->m_nRefs == 0)
    {
        return 0;
    }
 
    if(--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }
 
    CameraApp_FreeAppData(pMe);
 
    (void)ISHELL_Release(pMe->m_pShell);
    (void)IMODULE_Release(pMe->m_pModule);
 
    // ע��:pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       CameraApp_InitAppData

˵��:
       ��ʼ�� CameraApp Applet �ṹ����ȱʡֵ��
  
����:
       pMe [in]: ָ�� CameraApp Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
 
����ֵ:
       EFAILED: ��ʼ��ʧ�ܡ�
       SUCCESS: ��ʼ���ɹ���
   
��ע:
  
==============================================================================*/
static int CameraApp_InitAppData(CCameraApp *pMe)
{
    AEEDeviceInfo  di;
#ifdef FEATURE_ANALOG_TV
    uint32 m_tv = 0;
#endif
    if(NULL == pMe)
    {
        return EFAILED;
    }
#ifdef FEATURE_ANALOG_TV
    (void)OEM_SetConfig( CFGI_TV_OR_CAMERA,
	                          &m_tv,
	                          sizeof(uint32));
#endif
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
    pMe->m_isPicCamera = FALSE;
    
    pMe->m_bCapturePic = FALSE;
    pMe->m_ePreState = STATE_NULL;
    pMe->m_nMainMenuItemSel = IDS_ITEM_CAMERA;
    pMe->m_nSelfTimeItemSel = IDS_SELFTIME_OFF;
    pMe->m_nCameraCFG = CAMERACFGFIRST;
    pMe->m_sCurrentFileName[0] = '\0';
    pMe->m_sCaptureFileName[0] = '\0';
    pMe->m_bCanCapture = TRUE;
    pMe->m_dwMemTotal = 0;
    pMe->m_dwMemFree = 0;
    

    //Video��ʱ��
    pMe->m_dwDispTime = 0;
    pMe->m_nNumDays = 0;
    pMe->m_nTicks = 0;
    pMe->m_bRePreview = FALSE;
    pMe->m_nCameraState = CAM_START;

    pMe->m_pCamera = NULL;  
    pMe->m_pMedia = NULL;
    pMe->m_isFormQuicktest = FALSE;
#ifdef FEATURE_USES_MMS  
    pMe->m_isFormMMS = FALSE;
#endif

#ifdef CAMERA_RECORD_DEBUG
    pMe->m_isStartFromFacebook = TRUE;
    pMe->m_isRecordMode = TRUE;
#else
	pMe->m_isStartFromFacebook = FALSE;
    pMe->m_isRecordMode = FALSE;
#endif
    pMe->m_sensor_model = -1;
    MEMSET(&pMe->m_CallBack, 0, sizeof(AEECallback));

    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                             AEECLSID_CONFIG,
                                             (void **)&pMe->m_pConfig))
    {
        CameraApp_FreeAppData(pMe);
        return EFAILED;
    }

#ifndef FEATURE_DSP
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                            AEECLSID_DISPLAY,
                                            (void **)&pMe->m_pDisplay))
    {
        CameraApp_FreeAppData(pMe);
        return EFAILED;
    }
#endif
    
    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                            AEECLSID_FILEMGR, 
                                            (void **)&pMe->m_pFileMgr))
    {
        CameraApp_FreeAppData(pMe);
        return EFAILED;
    }

    if(AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                            AEECLSID_ANNUNCIATOR, 
                                            (void **)&pMe->m_pIAnn))
    {
        CameraApp_FreeAppData(pMe);
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       CameraApp_FreeAppData

˵��:
       �ͷ� CameraApp Applet �����Դ��
  
����:
       pMe [in]: ָ��CameraApp Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
 
����ֵ:
       none
   
��ע:
  
==============================================================================*/
static void CameraApp_FreeAppData(CCameraApp *pMe)
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

    if(pMe->m_pCamera)
    {
        ICAMERA_Release(pMe->m_pCamera);
        pMe->m_pCamera = NULL;
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
// if SD Dev is existed, stop SD dev while exitting from Camera APP to avoid high current<43mA> consume in idle state
    // Note: if SD card detected after Creating ICamera interface, it needn't to stop sd card in this way, cause's SD won't be pushed in stack for low dev priority
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
       CameraApp_RunFSM
    
˵��:
       CameraApp Applet����״̬�����档
   
����:
       pMe [in]��ָ��CameraApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
     
����ֵ:
       �ޡ�
    
��ע:
      
==============================================================================*/
static void CameraApp_RunFSM(CCameraApp *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;
     
    for( ; ; )
    {
        nextFSMAction = CameraApp_ProcessState(pMe);
      
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
       CameraApp_HandleEvent
    
˵��:
       CameraApp Applet�¼��������������������ڱ�Applet���¼����������ɸú�������
   
����:
       pi [in]:ָ��ICameraApp�ӿ�t�����ָ�롣
       eCode:BREW�¼����롣
       wParam:�¼�����
       dwParam [in]:��wParam���������ݡ���
     
����ֵ:
       TRUE:�����¼�������
       FALSE:�����¼�û������
    
��ע:
       �ú�����BREW���ã��û����õ��á�
      
==============================================================================*/
static boolean CameraApp_HandleEvent(ICameraApp  *pi,
                                     AEEEvent     eCode,
                                     uint16       wParam,
                                     uint32       dwParam)
{
    CCameraApp *pMe = (CCameraApp*)pi;   
    AEEAppStart *as;    
    int  nAscent,nDescent;


    switch(eCode)
    {
        case EVT_APP_START:
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
                else if( STRCMP(as->pszArgs, "facebook") == 0 )
                {
					pMe->m_isStartFromFacebook = TRUE;
                }
                else if( STRCMP(as->pszArgs, "record") == 0 )
                {
					pMe->m_isRecordMode = TRUE;
                }
#ifdef FEATURE_USES_MMS                  
                else if( STRCMP(as->pszArgs, "MMS") == 0 )
                {
					pMe->m_isFormMMS = TRUE;
                }           
#endif                
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
						if ((pMe->m_isRecordMode == TRUE)
#ifdef FEATURE_USES_MMS  
                            || (pMe->m_isFormMMS)
#endif                            
                            )
						{
							pMe->m_eCurState = STATE_CPOPMSG;
						}
						else
						{
                        	pMe->m_eCurState = STATE_CMAINMENU;                               
						}
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
                        //pMe->m_eCurState = STATE_CMAINMENU;   
                        
						if ((pMe->m_isRecordMode == TRUE)
#ifdef FEATURE_USES_MMS  
                            || (pMe->m_isFormMMS)
#endif                            
                            )
						{
							pMe->m_eCurState = STATE_CPOPMSG;
						}
						else
						{
                        	pMe->m_eCurState = STATE_CMAINMENU;                               
						}
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
            CameraApp_RunFSM(pMe);
            return TRUE;
      
        case EVT_APP_STOP:
            if(pMe->m_pDisplay != NULL)
            {
                (void)IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_bSuspending = TRUE; 
            SetDeviceState(DEVICE_TYPE_CAMERA, DEVICE_CAMERA_STATE_OFF);
            if(pMe->m_bIsPreview == TRUE && pMe->m_pCamera)
            {
#ifndef FEATURE_DSP
                ICAMERA_RegisterNotify(pMe->m_pCamera,NULL, NULL);
#endif
                ICAMERA_Stop(pMe->m_pCamera);
            }
            return TRUE;
      
        case EVT_APP_SUSPEND:
            ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);// ȡ�����ж�ʱ��
            
            pMe->m_bSuspending = TRUE;
            if(pMe->m_bIsPreview == TRUE && pMe->m_pCamera)
            {
#ifndef FEATURE_DSP
                ICAMERA_RegisterNotify(pMe->m_pCamera,NULL, NULL);
#endif
                ICAMERA_Stop(pMe->m_pCamera);
            }
            
            if(pMe->m_nCameraState == CAM_START && pMe->m_pCamera)
            {
                ICAMERA_Release(pMe->m_pCamera);
                pMe->m_pCamera = NULL;
            }
            pMe->m_bIsPreview = FALSE;
            return TRUE;
            
        case EVT_ALARM:
            if(pMe->m_nCameraState == CAM_START && pMe->m_pCamera)
            {
                ICAMERA_Release(pMe->m_pCamera);
                pMe->m_pCamera = NULL;
            }
            pMe->m_bIsPreview = FALSE;
            return TRUE;
            
        case EVT_APP_RESUME:
            CameraApp_InitCameraCheck(pMe); 
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
            CameraApp_RunFSM(pMe);
            return TRUE;
      
        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;   
            return CameraApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
      
        case EVT_DIALOG_START:
            (void)ISHELL_SetTimer(pMe->m_pShell,
                                  APPISREADY_TIMER,
                                  CameraApp_APPIsReadyTimer,
                                  pMe);
            return CameraApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
            
        case EVT_USER_REDRAW:
            (void)CameraApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
            return TRUE;
            
        case EVT_CAMERA_NOTIFY:
            CameraApp_AppEventNotify(pMe, (int16)wParam, (int16)dwParam);
            return TRUE;
            
        case EVT_APPISREADY:
            pMe->m_bAppIsReady = TRUE;
            return TRUE;
      
        case EVT_KEY_PRESS:
        case EVT_KEY_RELEASE:
        case EVT_KEY:
        case EVT_COMMAND:    
#ifdef FEATURE_LCD_TOUCH_ENABLE
		case EVT_PEN_UP:
#endif          
            if(!pMe->m_bAppIsReady)
            {
                return TRUE;
            }

            return CameraApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
      
        case EVT_DIALOG_END:
            if(wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void)CameraApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
            pMe->m_pActiveDlg = NULL; 
            
            //Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            //�¼���EVT_DIALOG_END�¼�ǰ������
            if(pMe->m_bSuspending == FALSE)
            {
                //��ʼCameraApp״̬��
                CameraApp_RunFSM(pMe);
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
				  return CameraApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
			  break;
#endif      
        default:
            //�����յ����¼�·������ǰ��ĶԻ����¼���������
            return CameraApp_RouteDialogEvent(pMe, eCode, wParam, dwParam);
    }

    return FALSE;
} //CameraApp_HandleEvent()

/*==============================================================================
����:
       CameraApp_APPIsReadyTimer
  
˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���
 
����:
       pme [in]��void *����ָ�룬��ָCCameraApp�ṹָ�롣
   
����ֵ:
       �ޡ�
  
��ע:
    
==============================================================================*/
static void CameraApp_APPIsReadyTimer(void *pme)
{
    CCameraApp *pMe = (CCameraApp *)pme;
    
    if(NULL == pMe)
    {
        return;
    }
    
    (void)ISHELL_PostEvent(pMe->m_pShell,
                           AEECLSID_APP_CAMERA,
                           EVT_APPISREADY,
                           0,
                           0);
}


