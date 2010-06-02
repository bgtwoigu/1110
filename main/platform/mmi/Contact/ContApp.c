/*==============================================================================
// �ļ���
//        ContApp.c
//        2004-06-09 jyh����汾(Draft Version)
//        ��Ȩ����(c) 2004 Anylook
//        
// �ļ�˵����
//        
// ���ߣ�2004-06-09
// �������ڣ�jyh
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ    ��      ��   ��   ��     �޸����ݡ�λ�ü�ԭ��
--------      -----------     -----------------------------------------------
04-06-09      jyh             ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "ContApp_priv.h"
#include "ContAppCommon.h"
#include "ContApp_vCard.h"
#ifndef WIN32
#include "db.h"
#endif
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
/*lint -e506 -e774 -e737*/
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------Applet��غ�������---------------------*/
#if defined(AEE_STATIC)
extern int ContApp_Load(IShell *ps, void * pHelpers, IModule ** pMod);
static int ContApp_CreateInstance( AEECLSID    ClsId,
                                        IShell     *pIShell,
                                        IModule    *po,
                                        void      **ppObj);
#endif

static int ContApp_New(IShell * pIShell,IModule * po,void ** ppObj);

static uint32 IContApp_AddRef(IContApp *pi);

static uint32 IContApp_Release(IContApp *pi);

static boolean IContApp_HandleEvent( IContApp *pi,
                                      AEEEvent   eCode,
                                      uint16     wParam,
                                      uint32     dwParam);

static int CContApp_InitAppData(CContApp *pMe);

static void CContApp_FreeAppData(CContApp *pMe);

static void CContApp_RunFSM(CContApp *pMe);

static int IContApp_SaveNumber( IContApp *pi, 
                                     AECHAR *number,
                                     AECHAR *E_Mail,    
                                     AECHAR *URL,                                     
                                     PhoneSaveType saveType);

static int      IContApp_FindNumber(IContApp *pi, AECHAR *searchStr);

static int      IContApp_SelectField( IContApp *pi, 
                                      PFNSELECTEDCONTACTCB fnCB, 
                                      SelectFldType FldType);
static int      IContApp_NumberLookup( IContApp   *pi,
                                       AECHAR *pNumber,
                                       ContInfo *pContInfo);
static int      IContApp_SpeedDialLookup( IContApp   *pi,
                                          AECHAR     *pSpeedDial,
                                          AECHAR     *pNumber,
                                          uint16      wNumLen);
static int      IContApp_SpeedDialCall(IContApp *pi, AECHAR *pSpeedDial);

static int      IContApp_MainMenu(IContApp *pi);
                                        
static int      IContApp_DeleteAll(IContApp *pi);

#ifdef FEATURE_SUPPORT_VCARD
static int      IContApp_ReadVcfFile(IContApp *pi, char* fileName, int * cnt);

static int      IContApp_CreateVcfFile(IContApp *pi, int contId, char* fileName);

static int      IContApp_CreateVcfFileEx(IContApp *pi, AEEAddrField *pFields, int nFldCnt, char* fileName);

static int      IContApp_CheckCapacity(IContApp *pi, boolean *isFull);

#endif

// ContApp helper functions
////////////
static int      CContApp_Start(CContApp *pMe);
static void     CContApp_AppIsReadyCB(void *pUser);
static int      CContApp_ReadCFG( IFileMgr *pFileMgr, ContAppCFGCache *pCFGCache);
static int      CContApp_WriteCFG( IFileMgr *pFileMgr,  ContAppCFGCache *pCFGCache);
static void     CContApp_CFGCacheInit(ContAppCFGCache *pCFGCache);
static char     *CContApp_BuildStartArgs( AppletStartMethod method,
                                          void             *saveType,
                                          void             *arg);
static void     CContApp_DecodeStartArgs(CContApp *pMe, char *args);
static boolean  CContApp_WStrStrEx( AECHAR *pHaystack, AECHAR *pNeedle);
static int      CContApp_OneDialCall(CContApp *pMe, AECHAR *pOneDial);
static int      CContApp_GetOneDial( CContApp   *pMe,
                                     AECHAR     *pSpeedDial,
                                     AECHAR     *pNumber,
                                     uint16      wNumLen);
static int      CContApp_GetSmartDial( CContApp   *pMe,
                                       AECHAR     *pSpeedDial,
                                       AECHAR     *pNumber,
                                       uint16      wNumLen);
static int      CContApp_GetNumber( IAddrRec   *pAddrRec,
                                    AECHAR     *pNumber,
                                    uint16      wNumLen);
                               

/*���ٲ����н����Զ��л���������Gemsea @2008.08.08*/
void CContApp_SmartMenuSetFocus(void *pUser);


/*==============================================================================
                                 ȫ������
==============================================================================*/
sSelectFieldListNode* pSelectFieldListRoot = NULL;
sSelectFieldListNode* pCurSelectFieldNode = NULL;


/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
#if defined(AEE_STATIC)
/*

#define CONTAPP_METHODS                 \
        {                               \
             IContApp_AddRef,           \
             IContApp_Release,          \
             IContApp_HandleEvent,      \
             IContApp_SaveNumber,       \
             IContApp_FindNumber,       \
             IContApp_SelectField,      \
             IContApp_NumberLookup,     \
             IContApp_SpeedDialLookup,  \
             IContApp_SpeedDialCall,    \
             IContApp_MainMenu,         \
             IContApp_DeleteAll         \
#ifdef FEATURE_SUPPORT_BT_APP           \
             ,IContApp_ReadVcfFile      \
#endif                                  \
        }
*/
// ֻ����һ��ContAppʵ����ÿ�δ���ContApp Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CContApp gContApp;

static const VTBL(IContApp) gContAppMethods = /*CONTAPP_METHODS;*/
            {                               
             IContApp_AddRef,           
             IContApp_Release,          
             IContApp_HandleEvent,      
             IContApp_SaveNumber,       
             IContApp_FindNumber,       
             IContApp_SelectField,      
             IContApp_NumberLookup,     
             IContApp_SpeedDialLookup,  
             IContApp_SpeedDialCall,    
             IContApp_MainMenu,         
             IContApp_DeleteAll         
#ifdef FEATURE_SUPPORT_VCARD           
             ,IContApp_ReadVcfFile
             ,IContApp_CreateVcfFile
             ,IContApp_CreateVcfFileEx
             ,IContApp_CheckCapacity
#endif                                  
        };

#endif

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------Applet��غ�������---------------------*/
#if defined(AEE_STATIC)

/*==============================================================================
����: 
       ContApp_Load(IShell *ps, void * pHelpers, IModule ** pMod)

˵��:  
        ��BREW�����Գ�ʼ��̬ģ��ı�׼������

����: 
       ps [in]��IShell�ӿڶ���ָ�롣
       pHelpers [in]��
       pIModule [in]��pIModule�ӿڶ���ָ�롣

����ֵ:
       ��0��ʧ�ܡ�
       0���ɹ���
       
��ע:

==============================================================================*/
int ContApp_Load(IShell *ps, void * pHelpers, IModule ** pMod)
{
    return(AEEStaticMod_New( (int16)(sizeof(AEEMod)),
                             ps ,
                             pHelpers,
                             pMod, 
                             (PFNMODCREATEINST)ContApp_CreateInstance,
                             (PFNFREEMODDATA)NULL));
} // ContApp_Load
#endif

/*==============================================================================
����: 
       ContApp_CreateInstance

˵��:  
       This function is invoked while the app is being loaded. All Modules must 
       provide this function. Ensure to retain the same name and parameters for 
       this function

����: 
       ClsId:    [in]: Specifies the ClassID of the applet which is being loaded
       pIShell:  [in]: Contains pointer to the IShell interface. 
       pIModule: [in]: Contains pointer to the IModule interface to the current 
                       module to whichthis app belongs
       ppObj:   [out]: On return, *ppObj must point to a valid AEEApplet 
                       structure. Allocation of memory for this structure and 
                       initializing the base data members is done by 
                       AEEApplet_New().

����ֵ:

��ע:

==============================================================================*/
#if defined(AEE_STATIC)
static int ContApp_CreateInstance( AEECLSID    ClsId,
                                   IShell     *pIShell,
                                   IModule    *po,
                                   void      **ppObj)
#else
int AEEClsCreateInstance( AEECLSID   ClsId,
                          IShell    *pIShell,
                          IModule   *po,
                          void     **ppObj)
#endif
{
    if (ClsId == AEECLSID_APP_CONTACT)
    {
        return ContApp_New(pIShell, po, ppObj);
    }
    else
    {
        return EUNSUPPORTED;
    }
} // ContApp_CreateInstance

/*==============================================================================
����: 
       ContApp_New
       
˵��: 
        ��������ʵ����
       
����: 
       ps [in]��IShell�ӿڶ���ָ�롣
       po [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:ָ��IContApp����ָ���ָ�롣
       
����ֵ:
       SUCCESS���ɹ���
       
��ע:
       
==============================================================================*/
static int ContApp_New(IShell * pIShell, IModule * po, void ** ppObj)
{
    CContApp *pMe = NULL;
    int retVal = SUCCESS;
    
    if ((NULL == ppObj) || (NULL == pIShell) || (NULL == po))
    {
        return EFAILED;
    }

#if defined(AEE_STATIC)
    pMe = &gContApp;
#else
    pMe = (CContApp *)MALLOC(sizeof(CContApp) + sizeof(VTBL(IContApp)));
    if (NULL == pMe)
    {
        return EFAILED;
    }
#endif
    if (0 == pMe->m_nRefs)
    {
#if defined(AEE_STATIC)
        // Must initialize the object
        MEMSET(pMe,  0, sizeof(CContApp));
        INIT_VTBL(pMe, IContApp, gContAppMethods);
#else
        //NOTE: In the dynamic applet, the extend classes will re-malloc the 
        //      memery everytime in startapplet. So it run in simulator for
        //      test only. If you want run in simulator correcttly please 
        //      modify the following code.
        VTBL(IContApp) *pMod = (VTBL(IContApp)*)((byte *)pMe +sizeof(CContApp));
        pMod->AddRef      = IContApp_AddRef;
        pMod->Release     = IContApp_Release;
        pMod->HandleEvent = IContApp_HandleEvent;
        INIT_VTBL(pMe, IContApp, *pMod);
#endif
        
        pMe->m_nRefs    = 0;
        pMe->m_pShell   = pIShell;
        pMe->m_pModule  = po;
    
        (void) ISHELL_AddRef(pMe->m_pShell);
        (void) IMODULE_AddRef(pMe->m_pModule);
    
        retVal = CContApp_InitAppData(pMe);
        if( SUCCESS != retVal )
        {
            CContApp_FreeAppData(pMe);
            return retVal;
        }
    }
    
    ++pMe->m_nRefs;
    *ppObj = (IContApp*)pMe;
    return retVal;
} // ContApp_Load

/*==============================================================================
����: 
       IContApp_AddRef
       
˵��: 
       ContApp Applet ���ü�����1�������ص�ǰ����������
       
����: 
       pi [in]��IContApp�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��ContApp Applet��������
       
��ע:
       
==============================================================================*/
static uint32  IContApp_AddRef(IContApp *pi)
{
    CContApp *pMe = (CContApp*)pi;
    
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
    ASSERT(pMe->m_nRefs > 0);
#endif
    
    return (++pMe->m_nRefs);
} // IContApp_AddRef

/*==============================================================================
����: 
       IContApp_Release
       
˵��: 
       �ͷŶ�ContApp Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��
       
����: 
       pi [in]��IContApp�ӿڶ���ָ�롣
       
����ֵ:
       ��ǰ��ContApp Applet��������
       
��ע:
       
==============================================================================*/
static uint32  IContApp_Release (IContApp *pi)
{
    CContApp *pMe = (CContApp*)pi;

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    if (pMe->m_nRefs == 0)
    {
        return 0;
    }
    
    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    CContApp_FreeSelectFieldListNode();
    
    // �ͷ�Applet�����Դ
    CContApp_FreeAppData(pMe);
    
    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);
    
    // ע�⣺���pMe�Ǿ�̬����ռ䣬����Ҫ�ͷš�FREE()
#if !defined(AEE_STATIC)
    FREE_VTBL(pMe, IContApp);
    FREE(pMe);
#endif
    return 0;
} // IContApp_Release

/*==============================================================================
����: 
       CContApp_InitAppData
       
˵��: 
       ��ʼ��ContApp Applet�ṹ����ȱʡֵ��
       
����: 
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�
       
��ע:
       
==============================================================================*/
static int CContApp_InitAppData(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif

    // Create IDisplay instance
    if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
                                           AEECLSID_DISPLAY,
                                           (void **)&pMe->m_pDisplay))
    {
        return EFAILED;
    }
    else
    {
        // �������������и�
        pMe->m_nLineHeight = IDISPLAY_GetFontMetrics( pMe->m_pDisplay,
                                                      AEE_FONT_NORMAL,
                                                      NULL,
                                                      NULL );
    }
    
    // Create IVector instance
    if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
                                           AEECLSID_VECTOR,
                                           (void **)&pMe->m_pAddList))
    {
        return EFAILED;
    }

    // Create IVector instance
    if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
                                           AEECLSID_VECTOR,
                                           (void **)&pMe->m_pFldIv))
    {
        return EFAILED;
    }
    
    if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
                                           AEECLSID_FILEMGR,
                                           (void **)&pMe->m_pFileMgr))
    {
        return EFAILED;
    }
    
    // Create IAddrbokk instance for phone
    if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
                                           AEECLSID_ADDRBOOK,
                                           (void **)&pMe->m_pAddrPhone))
    {
        return EFAILED;
    }

#ifdef FEATURE_RUIM_PHONEBOOK
    if( IsRunAsUIMVersion() )
    {    // Create IAddrbokk instance for RUIM
        if ( SUCCESS != ISHELL_CreateInstance( pMe->m_pShell,
                                               AEECLSID_ADDRBOOK_RUIM,
                                               (void **)&pMe->m_pAddrRUIM))
        {
            return EFAILED;
        }
    }
#endif//FEATURE_RUIM_PHONEBOOK

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell, 
                                            AEECLSID_CONFIG, 
                                            (void **)&pMe->m_pConfig))
    {
        return EFAILED;
    }

    if (AEE_SUCCESS != ISHELL_CreateInstance(pMe->m_pShell,
                                        AEECLSID_RINGERMGR,
                                        (void **)&pMe->m_pRingerMgr))
    {
        return EFAILED;
    }

    return SUCCESS;
} // CContApp_InitAppData

/*==============================================================================
����: 
       CContApp_FreeAppData
       
˵��: 
       �ͷ�ContApp Appletʹ�õ������Դ��
       
����: 
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void CContApp_FreeAppData(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    /*ȡ�����ٲ����н����Զ��л�������*/
     (void) ISHELL_CancelTimer( pMe->m_pShell,
                                                                    CContApp_SmartMenuSetFocus,
                                                                    (void *)pMe);

    FREEIF(pMe->m_pMsgBox_Msg);
    FREEIF(pMe->m_pSrchData);
    FREEIF(pMe->m_pFldInputBuf);
    FREEIF(pMe->m_pAddNewName);
    FREEIF(pMe->m_pAddNewMobile);
    FREEIF(pMe->m_pAddNewHome);
    FREEIF(pMe->m_pAddNewOffice);
    FREEIF(pMe->m_pAddNewFax);
    FREEIF(pMe->m_pAddNewEMAIL);
    FREEIF(pMe->m_pAddNewAddress);
    FREEIF(pMe->m_pAddNewURL);
    FREEIF(pMe->m_pAddNewRemark);
    FREEIF(pMe->m_pFindData);
    FREEIF(pMe->m_pSmartFindBuf);
    FREEIF(pMe->m_pFindNum);
    FREEIF(pMe->m_pOneDial);
    FREEIF(pMe->m_szAlpha);
    FREEIF(pMe->m_sSearchStr);
    FREEIF(pMe->m_strPhonePWD);
    FREEIF(pMe->m_nRingToneID);
    
    if (pMe->m_pCurrRec != NULL) 
    {
        IADDRREC_Release(pMe->m_pCurrRec);
        pMe->m_pCurrRec = NULL;
    }
    
    if (pMe->m_pDisplay != NULL)
    {
        (void) IDISPLAY_Release(pMe->m_pDisplay);
        pMe->m_pDisplay = NULL;
    }
    
    if (pMe->m_pAddList != NULL)
    {
        (void) IVector_Release(pMe->m_pAddList);
        pMe->m_pAddList = NULL;
    }

    if (pMe->m_pFldIv != NULL)
    {
        CContApp_FreeAddrFlds(pMe);
        (void) IVector_Release(pMe->m_pFldIv);
        pMe->m_pFldIv = NULL;
    }
    
    if (pMe->m_pFileMgr != NULL)
    {
        (void) IFILEMGR_Release(pMe->m_pFileMgr);
        pMe->m_pFileMgr = NULL;
    }
    
    if (pMe->m_pAddrPhone != NULL)
    {
        (void) IADDRBOOK_Release(pMe->m_pAddrPhone);
        pMe->m_pAddrPhone = NULL;
    }
    
#ifdef FEATURE_RUIM_PHONEBOOK
    if( IsRunAsUIMVersion() )
    {
        if (pMe->m_pAddrRUIM != NULL)
        {
            (void) IADDRBOOK_Release(pMe->m_pAddrRUIM);
            pMe->m_pAddrRUIM = NULL;
        }
    }
#endif// FEATURE_RUIM_PHONEBOOK

    if (pMe->m_pConfig)
    {
        ICONFIG_Release(pMe->m_pConfig);
        pMe->m_pConfig = NULL;
    }

    if (pMe->m_pRingerMgr)
    {
        IRINGERMGR_Release(pMe->m_pRingerMgr);
        pMe->m_pRingerMgr = NULL;
    }

    if (NULL != pMe->m_eBitmap)
    {
        IBITMAP_Release(pMe->m_eBitmap);
        pMe->m_eBitmap = NULL;
    }
} // CContApp_FreeAppData

/*==============================================================================
����: 
       CContApp_RunFSM
       
˵��: 
       ContApp Applet����״̬�����档
       
����: 
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       �ޡ�
       
��ע:
       
==============================================================================*/
static void CContApp_RunFSM(CContApp *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif

    for ( ; ; )
    {
        nextFSMAction = CContApp_ProcessState(pMe);
        
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
} // CContApp_RunFSM

/*==============================================================================
����: 
       IContApp_HandleEvent
       
˵��: 
       ContApp  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������
       
����: 
       pi [in]��ָ��IContApp�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�
       
����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������
       
��ע:
       
==============================================================================*/
static boolean  IContApp_HandleEvent( IContApp   *pi,
                                      AEEEvent   eCode,
                                      uint16     wParam,
                                      uint32     dwParam)
{
    CContApp *pMe = (CContApp*)pi;
    AEEAppStart *as;
    AEEDeviceInfo di;

    ISHELL_GetDeviceInfo(pMe->m_pShell,&di);    
    
    FARF(EVENT,("Handle evt 0x%x, w %x, dw %x",eCode, wParam, dwParam ));
    
    switch (eCode)
    {
        case EVT_APP_START:
#if defined(AEE_STATIC)
            ASSERT(dwParam != 0);
#endif
            as = (AEEAppStart*)dwParam;
            pMe->m_rc = as->rc;
            pMe->m_rc.dy = di.cyScreen; //�ڵ�ǰ״̬����ʾ�ź�����Ҫ����ȡdy�Ĵ�С            
            
            pMe->m_bSuspending = FALSE;
            
            pMe->m_bActive = TRUE;

            pMe->m_bUnLock = FALSE;

            (void)CContApp_Start(pMe);
            ERR("CCONTAPP_START END",0,0,0);
            // Decode the arguments
            CContApp_DecodeStartArgs(pMe, (char *) as->pszArgs);
            
            if ((pMe->m_eStartMethod == STARTMETHOD_FINDNUMBER) ||
                (pMe->m_eStartMethod == STARTMETHOD_SMARTCALL))
            {
                pMe->m_bUnLock = TRUE;
            }

#if defined(FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK)
#if defined( FEATURE_RUIM_PHONEBOOK)
            if (!PhoneBookCache_IsIninited())
            {
                if ((pMe->m_eStartMethod == STARTMETHOD_FINDNUMBER) || 
                    (pMe->m_eStartMethod == STARTMETHOD_SAVEFIELD) ||
                    (pMe->m_eStartMethod == STARTMETHOD_SELECTFIELD) ||
                    (pMe->m_eStartMethod == STARTMETHOD_SMARTCALL))
                {
                    pMe->m_eCurState = STATE_EXIT;
                }

                if ((pMe->m_eStartMethod == STARTMETHOD_NORMAL) ||
                    (pMe->m_eStartMethod == STARTMETHOD_MAINMENU))
                {
                    pMe->m_eDlgRet = DLGRET_CREATE;
                    pMe->m_eCurState = STATE_INIT;
                }
            }
#endif /* FEATURE_RUIM_PHONEBOOK */
#endif /* FEATURE_INIT_RUIM_SMSandADD_BYUIMTASK*/            
            
            // ��ʼContApp״̬��
            CContApp_RunFSM(pMe);
            return TRUE;
#if defined(FEATURE_SUPPORT_BT_APP) && defined(FEATURE_SUPPORT_VCARD) 
        case EVT_NEWMSGIN:/*test only*/
        {
            ERR("EVT_NEWMSGIN",dwParam,0,0);
            if(dwParam)
            {
                char name[128];
                int cnt=0;
                STRCPY(name,(char*)dwParam);
                DBGPRINTF("EVT_NEWMSGIN %s",name);
                ContApp_ReadVcfFile(pMe, name, &cnt);
            }
            return TRUE;
        }
#endif
        case EVT_FLIP:
            if( !wParam)
            {
                if(pMe->m_eCurState != STATE_COPYING)
                {
                     (void) ISHELL_CloseApplet(pMe->m_pShell,FALSE);
                     return TRUE;
                }
                else
                {
                     return CContApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);                
                }
            }
            return TRUE;
            
        case EVT_APP_STOP:
            pMe->m_bSuspending = TRUE;
            pMe->m_bActive = FALSE;
			
            // תΪ��̨ģʽ
//            *((boolean *)dwParam) = FALSE;
		 
            (void)ISHELL_CancelTimer(pMe->m_pShell, CContApp_AppIsReadyCB, pMe);
            return TRUE;
            
        case EVT_APP_SUSPEND:
            pMe->m_bSuspending = TRUE;
            (void)ISHELL_CancelTimer(pMe->m_pShell, CContApp_AppIsReadyCB, pMe);
            return TRUE;
            
        case EVT_APP_RESUME:
#if defined(AEE_STATIC)
            ASSERT(dwParam != 0);
#endif
            as = (AEEAppStart*)dwParam;
            pMe->m_bSuspending = FALSE;
            
            pMe->m_rc = as->rc;
            pMe->m_rc.dy = di.cyScreen; //�ڵ�ǰ״̬����ʾ�ź�����Ҫ����ȡdy�Ĵ�С            

            if(!(LISTFROMSEARCH == pMe->m_eListFrom
                || LISTFROMGROUP == pMe->m_eListFrom
                || STATE_COPY == pMe->m_eCurState
                || STATE_DELETE == pMe->m_eCurState
                || STATE_SELECT_RECORD == pMe->m_eCurState))
            {
                CContApp_Phonebook_Load(pMe);
            }

            if(CContApp_IsPopState(pMe))
            {
                pMe->m_eCurState = pMe->m_ePreState;
            }
            
            CContApp_RunFSM(pMe);
            return TRUE;
            
        case EVT_DIALOG_INIT:
            // ���¶Ի�����Ʋ�����Ϣ������
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;
            pMe->m_bAppReady = FALSE;
            return CContApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            
        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            
            (void) CContApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;
            
            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼContApp״̬��
                CContApp_RunFSM(pMe);
            }

            return TRUE;
        
        case EVT_USER_REDRAW:
            (void) CContApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer( pMe->m_pShell,
                                   CONTAPP_APPREADY_MS,
                                   CContApp_AppIsReadyCB,
                                   pMe);
            return TRUE;
            
        case EVT_KEY_PRESS:
        case EVT_KEY:
        case EVT_KEY_RELEASE:
        case EVT_COMMAND:
            if(!pMe->m_bAppReady)
            {
              // �ڱ༭�������״̬����Ϊһ��ȥtext�ؼ����ڼ���״̬��text�ؼ����Խ��յ��ײ�dialog
              // �ַ���key�¼���������Ϊ�����timer�Ĵ��ڣ�ʹ���ϲ��dialog��timer����֮ǰ���ܽ��յ�key�¼���
              //���һ����EDIT״̬�Ϳ��ٰ�����text�ؼ�����յ�key�¼�������Ӧλ�ð��ַ���ʾ�������������Ա��Ŀ��
              //��������ʾ�ַ����Ǵ��������dialog�������ͻ���ɳ�ͻ��
              if(pMe->m_eCurState != STATE_EDIT && pMe->m_eCurState != STATE_ADDNEW)
              {
                return TRUE;
              }
            }
            return CContApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
			{
				return CContApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}

		case EVT_USER:
			{
				if((wParam == AVK_SELECT) || (wParam == AVK_INFO))
				{
					eCode = EVT_COMMAND;
					wParam = dwParam;
					dwParam = 0;
				}
				else if(wParam == AVK_CLR)
				{
					eCode = EVT_KEY;
				}
				return CContApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}
#endif            
        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return CContApp_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
} // IContApp_HandleEvent

///////////////////////////
// ContApp helper functions
////////////

/*==============================================================================
����: 
       CContApp_Start
       
˵��: 
       ��ʼ��ContApp Applet�ṹ����ȱʡֵ��
       
����: 
       pMe [in]��ָ��ContApp Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       
����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�
       
��ע:
       
==============================================================================*/
static int CContApp_Start(CContApp *pMe)
{
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    pMe->m_eStartMethod        = STARTMETHOD_NORMAL;
    pMe->m_ePreState           = STATE_NONE;
    pMe->m_eCurState           = STATE_LOAD;
    pMe->m_pActiveDlg          = NULL;
    pMe->m_eDlgRet             = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    
    pMe->m_bSuspending         = FALSE;
    
    pMe->m_wSelectCont         = CONTAPP_RECID_NULL;
    pMe->m_wSearchCont        = CONTAPP_RECID_NULL;
    pMe->m_wSelectSmart        = CONTAPP_RECID_NULL;
    pMe->m_wSelectStore        = MENU_SELECT_NULL;
    pMe->m_bAppReady           = FALSE;
    pMe->m_strPhonePWD        = NULL;

//guoys modified for PR 4.2.1 begin
#ifdef FEATURE_RUIM_PHONEBOOK
    if (IsRunAsUIMVersion())
    {
        (void)CContApp_GetConfig( pMe, CONTCFG_SAVETYPE, &pMe->m_nSaveType, sizeof(byte));
    }
    else
    {
        pMe->m_nSaveType    = CONTCFG_SAVETYPE_PHONE;
    }
#else            
    {
        pMe->m_nSaveType    = CONTCFG_SAVETYPE_PHONE;
    }
#endif// FEATURE_RUIM_PHONEBOOK
//guoys modified for PR 4.2.1 end

    pMe->m_pAddNewName = NULL;
    pMe->m_pAddNewMobile =NULL;
    pMe->m_pAddNewHome = NULL;
    pMe->m_pAddNewOffice = NULL;
    pMe->m_pAddNewFax = NULL;
    pMe->m_pAddNewEMAIL = NULL;
    pMe->m_pAddNewAddress = NULL;
    pMe->m_pAddNewURL = NULL;
    pMe->m_pAddNewRemark = NULL;
    pMe->m_nRingToneID = NULL;
    pMe->m_nGroupCat = 0;    
//#if defined FEATURE_CARRIER_THAILAND_HUTCH
    pMe->m_wSelectGroup =0;
//#endif    
    pMe->m_nSmartStateType = SMART_STATE_IDD_LIST;
#ifdef FEATURE_LANG_CHINESE
    pMe->m_nInputModeTable[0] = AEE_TM_LETTERS;
    pMe->m_nInputModeTable[1] = AEE_TM_NUMBERS;
    pMe->m_nCurrentInputMode = 0;
#endif
    CContApp_CFGCacheInit(&pMe->m_sCFGCache);
	
    // Read the config file
    (void)CContApp_ReadCFG(pMe->m_pFileMgr, &pMe->m_sCFGCache);
    

    return SUCCESS;
} // CContApp_Start


/*==============================================================================
����:
       SoundMenu_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCSoundMenu�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void CContApp_AppIsReadyCB(void *pUser)
{
    CContApp *pMe = (CContApp *)pUser;
    if (NULL == pMe)
    {
        return;
    }
    
    pMe->m_bAppReady = TRUE;
}// CContApp_AppIsReadyCB

/*==============================================================================
����: 
       CContApp_ReadCFG
       
˵��: 
       �������ļ��е������ö���Cache��
       
����: 
       pFileMgr [in]��ָ��IFileMgr����ṹ��ָ�롣
       pCFGCache [out]:�洢���ݵ�Cache
       
����ֵ:
       SUCCESS : �����ɹ�
       EFAILED : ����ʧ��
       EBADPARM: ��������
       
��ע:
       
==============================================================================*/
static int CContApp_ReadCFG(IFileMgr *pFileMgr, ContAppCFGCache *pCFGCache)
{
    IFile  *pFile;
    uint16  version;
    
    if(pCFGCache == NULL || pFileMgr == NULL)
    {
        return EBADPARM;
    }
    
    ERR("READCFG",0,0,0);
    // �������ļ�
    pFile = IFILEMGR_OpenFile(pFileMgr, FILE_CONTAPP_CFG, _OFM_READ);
    if(pFile == NULL)
    {
        ERR("FILE IS NULL",0,0,0);
        // ��δ������ʹ��Ĭ��ֵ
        return SUCCESS;
    } //else ���ļ�����ִ��
    
    // ���ļ���ʼ��ȡ����
    if(SUCCESS != IFILE_Seek(pFile, _SEEK_START, 0))
    {
        (void)IFILE_Release(pFile);
        return EFAILED;
    }
    
    // ��ȡ�汾��
    if(sizeof(version) != IFILE_Read(pFile, &version, sizeof(version)))
    {
        (void)IFILE_Release(pFile);
        return EFAILED;
    }
    
    // У��汾��
    if(version != CFG_VERSION)
    {
        // �汾�Ų���ȣ����Դ��ļ���ʹ��Ĭ��ֵ
        (void)IFILE_Release(pFile);
        return SUCCESS;
    }
    
    // ��ȡ������Ϣ
    if(sizeof(ContAppCFGCache) != IFILE_Read( pFile, 
                                              pCFGCache, 
                                              sizeof(ContAppCFGCache)))
    {
        (void)IFILE_Release(pFile);
        return EFAILED;
    }
    
    (void)IFILE_Release(pFile);
    return SUCCESS;
} // CContApp_ReadCFG

/*==============================================================================
����: 
       CContApp_WriteCFG
       
˵��: 
       ��Cache�е�����д�������ļ���
       
����: 
       pFileMgr [in]��ָ��IFileMgr����ṹ��ָ�롣
       pCFGCache [out]:�洢���ݵ�Cache
       
����ֵ:
       SUCCESS : �����ɹ�
       EFAILED : ����ʧ��
       EBADPARM: ��������
       
��ע:
       
==============================================================================*/
static int CContApp_WriteCFG(IFileMgr *pFileMgr, ContAppCFGCache *pCFGCache)
{
    IFile  *pFile;
    uint16  version = CFG_VERSION;
    
    if(pCFGCache == NULL || pFileMgr == NULL)
    {
        return EBADPARM;
    }
    
    // �������ļ�
    pFile = IFILEMGR_OpenFile(pFileMgr, FILE_CONTAPP_CFG, _OFM_READWRITE);
    if(pFile == NULL)
    {
        // ���������ļ�
        pFile = IFILEMGR_OpenFile(pFileMgr, FILE_CONTAPP_CFG, _OFM_CREATE);
        ERR("CREATE cfg FILE",0,0,0);
        if(pFile == NULL)
        {
            ERR("CREATE cfg FILE FAIL",0,0,0);
            // ���������ļ�ʧ��
            return EFAILED;
        }//else ���ļ�����ִ��
    }//else ���ļ�����ִ��
    
    // У��汾��
    if(SUCCESS != IFILE_Seek(pFile, _SEEK_START, 0))
    {
        (void)IFILE_Release(pFile);
        return EFAILED;
    }
    
    // д�汾��
    if(sizeof(version) != IFILE_Write(pFile, &version, sizeof(version)))
    {
        (void)IFILE_Release(pFile);
        return EFAILED;
    }
    
    // д������Ϣ
    if(sizeof(ContAppCFGCache) != IFILE_Write( pFile,
                                               pCFGCache,
                                               sizeof(ContAppCFGCache)))
    {
        (void)IFILE_Release(pFile);
        return EFAILED;
    }
    
    (void)IFILE_Release(pFile);
    return SUCCESS;
} // CContApp_WriteCFG

/*==============================================================================
����: 
       CContApp_CFGCacheInit
       
˵��: 
       ��ʼ��������Cache
       
����: 
       pCFGCache [int]:�洢���ݵ�Cache
       
����ֵ:
       
��ע:
       
==============================================================================*/
static void CContApp_CFGCacheInit(ContAppCFGCache *pCFGCache)
{
    ASSERT(pCFGCache != NULL);
    
    pCFGCache->wPassWord[0] = DEFAULT_PASSWORD;
    pCFGCache->bLocked      = FALSE;
    pCFGCache->nViewType    = CONTCFG_VIEWTYPE_ALL;
    if (IsRunAsUIMVersion())
    {
        pCFGCache->nSaveType    = CONTCFG_SAVETYPE_SELECT;
    }
    else
    {
        pCFGCache->nSaveType    =     CONTCFG_SAVETYPE_PHONE;
    }
    pCFGCache->bSmartDial   = TRUE;
    pCFGCache->wOneDial1[0] = ONEDIAL_NULL;
    pCFGCache->wOneDial2[0] = ONEDIAL_NULL;
    pCFGCache->wOneDial3[0] = ONEDIAL_NULL;
    pCFGCache->wOneDial4[0] = ONEDIAL_NULL;
    pCFGCache->wOneDial5[0] = ONEDIAL_NULL;
    pCFGCache->wOneDial6[0] = ONEDIAL_NULL;
    pCFGCache->wOneDial7[0] = ONEDIAL_NULL;
    pCFGCache->wOneDial8[0] = ONEDIAL_NULL;
    pCFGCache->wOneDial9[0] = ONEDIAL_NULL;
//#if defined FEATURE_CARRIER_THAILAND_HUTCH
    pCFGCache->wGroup1[0] = ONEDIAL_NULL;
    pCFGCache->wGroup2[0] = ONEDIAL_NULL;
    pCFGCache->wGroup3[0] = ONEDIAL_NULL;
    pCFGCache->wGroup4[0] = ONEDIAL_NULL;
    pCFGCache->wGroup5[0] = ONEDIAL_NULL;
    pCFGCache->wGroup6[0] = ONEDIAL_NULL; 
    pCFGCache->wGroup7[0] = ONEDIAL_NULL; 
#ifdef FEATURE_LANG_CHINESE
    pCFGCache->inputMode = 0;
#endif
//#endif
} // CContApp_CFGCacheInit

/*==============================================================================
����: 
       CContApp_GetConfig
       
˵��: 
       ��ȡConfig��Ŀ��ֵ
       
����: 
       pMe       [in] :ָ��ContApp Applet����ṹ��ָ�롣
       eCFG      [in] :��ȡ��������
       data      [out]:���ݴ洢��
       dataLen   [in] :���ݴ洢���Ĵ�С,in bytes
       
����ֵ:
       SUCCESS : �����ɹ�
       EFAILED : ����ʧ��
       EBADPARM: ��������
       
��ע:
       
==============================================================================*/
int CContApp_GetConfig( CContApp        *pMe, 
                        ContAppCFG       eCFG, 
                        void            *data,
                        int              dataLen)
{
    ContAppCFGCache *pCFGCache;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    pCFGCache = &pMe->m_sCFGCache;
    if(pCFGCache == NULL)
    {
        return EBADPARM;
    }
    
    // ����������
    switch(eCFG)
    {
        case CONTCFG_LOCKED:
            // У�����ݳ���
            if(dataLen != sizeof(pCFGCache->bLocked))
            {
                break;
            }
            
            *(boolean *)data = pCFGCache->bLocked;
            return SUCCESS;
            
        case CONTCFG_VIEWTYPE:
            // У�����ݳ���
            if(dataLen != sizeof(pCFGCache->nViewType))
            {
                break;
            }
            
            *(byte *)data = pCFGCache->nViewType;
            return SUCCESS;
             
        case CONTCFG_SAVETYPE:
            // У�����ݳ���
            if(dataLen != sizeof(pCFGCache->nSaveType))
            {
                break;
            }
            
            *(byte *)data = pCFGCache->nSaveType;
            return SUCCESS;
            
        case CONTCFG_PASSWORD:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wPassWord))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wPassWord);
            return SUCCESS;
            
        case CONTCFG_SMARTDIAL:
            // У�����ݳ���
            if(dataLen != sizeof(pCFGCache->bSmartDial))
            {
                break;
            }
            
            *(boolean *)data = pCFGCache->bSmartDial;
            return SUCCESS;
            
        case CONTCFG_ONEDIAL1:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wOneDial1))
            {
                break;
            }

            WSTRCPY((AECHAR *)data, pCFGCache->wOneDial1);
            return SUCCESS;
            
        case CONTCFG_ONEDIAL2:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wOneDial2))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wOneDial2);
            return SUCCESS;
            
        case CONTCFG_ONEDIAL3:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wOneDial3))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wOneDial3);
            return SUCCESS;
            
        case CONTCFG_ONEDIAL4:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wOneDial4))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wOneDial4);
            return SUCCESS;
            
        case CONTCFG_ONEDIAL5:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wOneDial5))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wOneDial5);
            return SUCCESS;
            
        case CONTCFG_ONEDIAL6:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wOneDial6))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wOneDial6);
            return SUCCESS;
            
        case CONTCFG_ONEDIAL7:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wOneDial7))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wOneDial7);
            return SUCCESS;
            
        case CONTCFG_ONEDIAL8:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wOneDial8))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wOneDial8);
            return SUCCESS;
            
        case CONTCFG_ONEDIAL9:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wOneDial9))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wOneDial9);
            return SUCCESS;

//#if defined FEATURE_CARRIER_THAILAND_HUTCH
        case CONTCFG_GROUP1:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wGroup1))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wGroup1);
            return SUCCESS;
            
        case CONTCFG_GROUP2:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wGroup2))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wGroup2);
            return SUCCESS;
            
        case CONTCFG_GROUP3:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wGroup3))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wGroup3);
            return SUCCESS;
        
        case CONTCFG_GROUP4:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wGroup4))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wGroup4);
            return SUCCESS;
            
        case CONTCFG_GROUP5:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wGroup5))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wGroup5);
            return SUCCESS;
            
        case CONTCFG_GROUP6:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wGroup6))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wGroup6);
            return SUCCESS;
        case CONTCFG_GROUP7:
            // У�����ݳ���
            if(dataLen < WSTRSIZE(pCFGCache->wGroup7))
            {
                break;
            }
            
            WSTRCPY((AECHAR *)data, pCFGCache->wGroup7);
            return SUCCESS;
#ifdef FEATURE_LANG_CHINESE
        case CONTTCFG_QUICKSEARCH_INPUT_MODE:
            // У�����ݳ���
            if(dataLen != sizeof(pCFGCache->inputMode))
            {
                break;
            }
            
            *(byte *)data = pCFGCache->inputMode;
            return SUCCESS;
#endif
//#endif
        default:
            break;
    }
    return EFAILED;
} // CContApp_GetConfig

/*==============================================================================
����: 
       CContApp_SetConfig
       
˵��: 
       ����Config��Ŀ��ֵ
       
����: 
       pMe       [in] :ָ��ContApp Applet����ṹ��ָ�롣
       eCFG      [in] :��ȡ��������
       data      [out]:���ݴ洢��
       dataLen   [in] :���ݴ洢���Ĵ�С,in bytes
       
����ֵ:
       SUCCESS : �����ɹ�
       EFAILED : ����ʧ��
       EBADPARM: ��������
       
��ע:
       
==============================================================================*/
int CContApp_SetConfig(CContApp        *pMe, 
                       ContAppCFG       eCFG, 
                       void            *data,
                       int              dataLen)
{
    ContAppCFGCache *pCFGCache;
#if defined(AEE_STATIC)
    ASSERT(pMe != NULL);
#endif
    
    pCFGCache = &pMe->m_sCFGCache;
    if(pCFGCache == NULL)
    {
        ERR("pCFGCache == NULL",0,0,0);
        return EBADPARM;
    }

    ERR("eCFG = %d",eCFG,0,0);
    DBGPRINTF("data = %s",(AECHAR *)data,0,0);
    // ����������
    switch(eCFG)
    {
        case CONTCFG_LOCKED:
            // У�����ݳ���
            if(dataLen != sizeof(pCFGCache->bLocked))
            {
                return EFAILED;
            }
            
            pCFGCache->bLocked = *(boolean *)data;
            break;
            
        case CONTCFG_VIEWTYPE:
            // У�����ݳ���
            if(dataLen != sizeof(pCFGCache->nViewType))
            {
                return EFAILED;
            }
            
            pCFGCache->nViewType = *(byte *)data;
            break;
             
        case CONTCFG_SAVETYPE:
            // У�����ݳ���
            if(dataLen != sizeof(pCFGCache->nSaveType))
            {
                return EFAILED;
            }
            
            pCFGCache->nSaveType = *(byte *)data;
            break;
            
        case CONTCFG_PASSWORD:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_LEN_PASSWORD)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wPassWord, (AECHAR *)data);
            break;
            
        case CONTCFG_SMARTDIAL:
            // У�����ݳ���
            if(dataLen != sizeof(pCFGCache->bSmartDial))
            {
                return EFAILED;
            }
            
            pCFGCache->bSmartDial = *(boolean *)data;
            break;
            
        case CONTCFG_ONEDIAL1:
            // У�����ݳ���
            ERR("dataLen=%d",WSTRLEN((AECHAR *)data),0,0);
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NUM)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wOneDial1, (AECHAR *)data);
            break;
            
        case CONTCFG_ONEDIAL2:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NUM)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wOneDial2, (AECHAR *)data);
            break;
            
        case CONTCFG_ONEDIAL3:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NUM)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wOneDial3, (AECHAR *)data);
            break;
            
        case CONTCFG_ONEDIAL4:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NUM)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wOneDial4, (AECHAR *)data);
            break;
            
        case CONTCFG_ONEDIAL5:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NUM)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wOneDial5, (AECHAR *)data);
            break;
            
        case CONTCFG_ONEDIAL6:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NUM)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wOneDial6, (AECHAR *)data);
            break;
        
        case CONTCFG_ONEDIAL7:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NUM)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wOneDial7, (AECHAR *)data);
            break;
            
        case CONTCFG_ONEDIAL8:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NUM)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wOneDial8, (AECHAR *)data);
            break;
            
        case CONTCFG_ONEDIAL9:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NUM)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wOneDial9, (AECHAR *)data);
            break;

//#if defined FEATURE_CARRIER_THAILAND_HUTCH
        case CONTCFG_GROUP1:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NAME_EN)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wGroup1, (AECHAR *)data);
            break;
            
        case CONTCFG_GROUP2:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NAME_EN)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wGroup2, (AECHAR *)data);
            break;
            
        case CONTCFG_GROUP3:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NAME_EN)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wGroup3, (AECHAR *)data);
            break;
        
        case CONTCFG_GROUP4:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NAME_EN)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wGroup4, (AECHAR *)data);
            break;
            
        case CONTCFG_GROUP5:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NAME_EN)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wGroup5, (AECHAR *)data);
            break;
            
        case CONTCFG_GROUP6:
            // У�����ݳ���
            if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NAME_EN)
            {
                return EFAILED;
            }
            
            WSTRCPY(pCFGCache->wGroup6, (AECHAR *)data);
            break;
       case CONTCFG_GROUP7:
           // У�����ݳ���
           if(WSTRLEN((AECHAR *)data) > MAX_INPUT_NAME_EN)
           {
               return EFAILED;
           }
           
           WSTRCPY(pCFGCache->wGroup7, (AECHAR *)data);
           break;
//#endif       
#ifdef FEATURE_LANG_CHINESE
       case CONTTCFG_QUICKSEARCH_INPUT_MODE:
       // У�����ݳ���
       if(dataLen != sizeof(pCFGCache->inputMode))
       {
           return EFAILED;
       }
       
       pCFGCache->inputMode = *(byte *)data;
       break;


#endif
        default:
            return EFAILED;
    }
    
    // Write the config data
    return CContApp_WriteCFG(pMe->m_pFileMgr, pCFGCache);
} // CContApp_SetConfig

/*=============================================================================
FUNCTION: CContApp_BuildStartArgs

DESCRIPTION: Builds a startup argument string

PARAMETERS:
   method: desired 'startup method'.  
   saveType: Phone number save Type(Only useful in case method is
             STARTMETHOD_SAVEFIELD All other methods should pass this as null
   *arg: optional startup method specific argument

RETURN VALUE:
    char *:  Encoded startup argument string that may be used with
             ISHELL_StartAppletArgs().  

             If non-NULL, this string must be deallocated by the caller

COMMENTS:

SIDE EFFECTS:

SEE ALSO:
   CContactApp_SaveNumber()
   CContactApp_FindNumber()
   CContactApp_SelectField()

=============================================================================*/
static char *CContApp_BuildStartArgs( AppletStartMethod method,
                                      void             *saveType,
                                      void             *arg)
{
    char *str = NULL;
    switch (method) 
    {
        case STARTMETHOD_NORMAL:
            break;
        
        case STARTMETHOD_SAVEFIELD:
            if ((arg != NULL)&& (saveType!=NULL)) 
            {
                uint32 len;
                
                len = (uint32) WSTRLEN((AECHAR*) arg);
                
                // Note:UTF8 
                // encoding can occupy anything from 0.5x to 4x more space
                // than Unicode encoding of the same string.   We take the
                // paranoid (and wasteful!) approach and always assume it will
                // take 4x the space
                //In the first char we will store the method and the second char
                // we will store the save type. Hence we have 2 in the begining
                str = MALLOC(1 + len * 4);
                
                // Save the start type at first character
                *str = STARTARGPREFIX_SAVENUMBER;
                
                if (str) 
                {
                    //store the saveType in the second character
                    switch(*(PhoneSaveType *)saveType)
                    {
                        case ADD_FIELD:
                            str[1] = STARTARGPREFIX_ADD_FLD;
                            break;
                        
                        case SAVE_NUMBER:
                            str[1] = STARTARGPREFIX_SAVE_NUMBER;
                            break;
                            
                        case SAVE_EMAIL:
                            str[1] = STARTARGPREFIX_SAVE_EMAIL;
                            break;
                            
                        case SAVE_URL:
                        default:
                            str[1] = STARTARGPREFIX_SAVE_URL;
                            break;
                    }
                    
                    (void)WSTRTOUTF8((AECHAR *) arg, 
                                     (int)      len, 
                                     (byte *)  &str[2], 
                                     (int)     (len*4));
                }
            }
            break;
            
        case STARTMETHOD_FINDNUMBER:
            if (arg != NULL) 
            {
                uint32 len;
                
                len = (uint32) WSTRLEN((AECHAR*) arg);
                
                // Note: 
                // UTF8 encoding can occupy anything from 0.5x to 4x more space
                // than Unicode encoding of the same string.   We take the
                // paranoid (and wasteful!) approach and always assume it will
                // take 4x the space
                str = MALLOC(1 + len * 4);
                
                if (str) 
                {
                    *str = STARTARGPREFIX_FINDNUMBER;
                    
                    (void) WSTRTOUTF8((AECHAR *) arg,
                                      (int)      len, 
                                      (byte *)  &str[1], 
                                      (int)     (len*4));
                }
            }
            break;
            
        case STARTMETHOD_SELECTFIELD:
            if (arg != NULL) 
            {
                SelectFldType selectFld;
                char buf[20];
                
                if ( saveType == NULL )
                {
                    selectFld = SINGLE_SELECT_NUMBER;
                }
                else
                {
                    selectFld = *((SelectFldType*)saveType);
                }

                if(*((SelectFldType*)saveType) == MULTI_SELECT_GROUPLIST)
                {
                    SPRINTF(buf, "%c%c%d", 
                        STARTARGPREFIX_GROUPLIST, selectFld, (uint32) arg);
                }
                else if(*((SelectFldType*)saveType) == SINGLE_SELECT_GROUPLIST)
                {
                    SPRINTF(buf, "%c%c%d", 
                        STARTARGPREFIX_GROUPLIST, selectFld, (uint32) arg);
                }
                else
                {
                    SPRINTF(buf, "%c%c%d", 
                        STARTARGPREFIX_SELECTFIELD, selectFld, (uint32) arg);
                }
                
                str = STRDUP(buf);
            }
            break;
            
        case STARTMETHOD_SMARTCALL:
            if (arg != NULL) 
            {
                uint32 len;
                
                len = (uint32) WSTRLEN((AECHAR*) arg);
                
                // Note: 
                // UTF8 encoding can occupy anything from 0.5x to 4x more space
                // than Unicode encoding of the same string.   We take the
                // paranoid (and wasteful!) approach and always assume it will
                // take 4x the space
                str = MALLOC(1 + len * 4);
                
                if (str) 
                {
                    *str = STARTARGPREFIX_SMARTCALL;
                    
                    (void) WSTRTOUTF8((AECHAR *) arg,
                                      (int)      len, 
                                      (byte *)  &str[1], 
                                      (int)     (len*4));
                }
            }
            break;

         case STARTMETHOD_MAINMENU:
                str = MALLOC(1);
                *str = STARTARGPREFIX_MAINMENU;
      	     break;
            
        default:
            ASSERT_NOT_REACHABLE;
            break;
    }
    
    return str;
}

/*=============================================================================
FUNCTION: CContApp_DecodeStartArgs

DESCRIPTION:  Decodes a startup argument string passed into EVT_APP_START

PARAMETERS:
   *pMe:  CContactApp object pointer
   *args: startup argument string

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:
   Sets the CContApp::m_eStartMethod field and possibly other fields
   depending on the contents of the startup argument string.

SEE ALSO:
=============================================================================*/
static void CContApp_DecodeStartArgs(CContApp *pMe, char *args)
{
    uint32  len;
    AECHAR *wStr;
    
    if ( pMe == NULL ) 
    {
        return;
    }
    
    if (NULL == args) 
    {
        return;
    }
    
    len = STRLEN(args);
    if (len < 1) 
    {
        return;
    }
    
    // Default to 'normal start method'
    pMe->m_eStartMethod        = STARTMETHOD_NORMAL;
    pMe->m_ePreState           = STATE_NONE;
    pMe->m_eCurState           = STATE_LOAD;
    pMe->m_pActiveDlg          = NULL;
    pMe->m_eDlgRet             = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;
    
    pMe->m_wSelectCont         = CONTAPP_RECID_NULL;
    pMe->m_wSelectSmart        = CONTAPP_RECID_NULL;
    
    switch (*args) 
    {
        case STARTARGPREFIX_SAVENUMBER:
            if (len < 2) 
            {
                // invalid argument length
                break;
            }
            
            len = STRLEN(&args[2]) + 1;
            
            // Note:
            // UTF8 encoding can occupy anything from 0.5x to 4x more space
            // than Unicode encoding of the same string.   We take the
            // paranoid (and wasteful!) approach and always assume that the
            // Unicode string will take 2x the length of the UTF8 string
            wStr = MALLOC(len * sizeof(AECHAR));
            
            if (NULL == wStr) 
            {
                break;
            }
            
            (void) UTF8TOWSTR((byte *) &args[2],
                              (int) len,  
                              wStr, 
                              (int)(len*sizeof(AECHAR)));
            
            
            //decode the first character where the method is stored
            //pMe->m_eStartMethod = STARTMETHOD_SAVEFIELD;  // move to switch follow
           
            //decode the second character where the PhoneSaveType is stored
            switch(args[1])
            {
                case STARTARGPREFIX_ADD_FLD:
                    pMe->m_eFldSaveType = ADD_FIELD;
                    FREEIF(pMe->m_pFldInputBuf);
                    pMe->m_pFldInputBuf =  wStr;
                    pMe->m_eStartMethod = STARTMETHOD_ADDFIELD;
                    break;
                
                case STARTARGPREFIX_SAVE_NUMBER:
                    pMe->m_eFldSaveType = SAVE_NUMBER;
                    FREEIF(pMe->m_pAddNewMobile);
                    pMe->m_pAddNewMobile =  wStr;
                    pMe->m_nAddNewFldID = AEE_ADDRFIELD_PHONE_GENERIC;
                    pMe->m_eStartMethod = STARTMETHOD_SAVEFIELD;
                    break;
                    
                case STARTARGPREFIX_SAVE_EMAIL:                    
                    pMe->m_eFldSaveType = SAVE_EMAIL;
                    //FREEIF(pMe->m_pAddNewEMAIL);
                    //pMe->m_pAddNewEMAIL =  wStr;
                    FREEIF(pMe->m_pFldInputBuf);
                    pMe->m_pFldInputBuf =  wStr;
                    pMe->m_nAddNewFldID = AEE_ADDRFIELD_EMAIL;
                    pMe->m_eStartMethod = STARTMETHOD_ADDEMAIL;
                    break;
                    
                case STARTARGPREFIX_SAVE_URL:
                default:
                    pMe->m_eFldSaveType = SAVE_URL;
                    //FREEIF(pMe->m_pAddNewURL);
                    //pMe->m_pAddNewURL =  wStr;
                    FREEIF(pMe->m_pFldInputBuf);
                    pMe->m_pFldInputBuf =  wStr;
                    pMe->m_nAddNewFldID = AEE_ADDRFIELD_URL;
                    pMe->m_eStartMethod = STARTMETHOD_ADDURL;
                    break;
            }

            break;
            
        //If the start method is find number
        case STARTARGPREFIX_FINDNUMBER:
            if (len < 2) 
            {
                // invalid argument length
                break;
            }
            
            len = STRLEN(&args[1]) + 1;
            
            // Note: UTF8 encoding can occupy anything from 0.5x to 4x more space
            //       than Unicode encoding of the same string.   We take the
            //       paranoid (and wasteful!) approach and always assume that the
            //       Unicode string will take 2x the length of the UTF8 string
            wStr = MALLOC(len * sizeof(AECHAR));
            
            if (NULL == wStr) 
            {
                break;
            }
            
            (void) UTF8TOWSTR((byte *)&args[1],
                              (int)len, 
                              wStr, 
                              (int)(len*sizeof(AECHAR)));
            
            pMe->m_eStartMethod = STARTMETHOD_FINDNUMBER;
            FREEIF(pMe->m_pFindNum);
            pMe->m_pFindNum     = wStr;
            break;

        case STARTARGPREFIX_GROUPLIST:
            pMe->m_eStartMethod = STARTMETHOD_GROUPLIST;
            pMe->m_wSelFldType  =(SelectFldType)args[1];
            pMe->m_pSelFldCB    =(PFNSELECTEDCONTACTCB)ATOI(args + 2);
            break;
            
        case STARTARGPREFIX_SELECTFIELD:
            pMe->m_eStartMethod = STARTMETHOD_SELECTFIELD;
            pMe->m_wSelFldType  =(SelectFldType)args[1];
            pMe->m_pSelFldCB    =(PFNSELECTEDCONTACTCB)ATOI(args + 2);
            break;
            
        //If the start method is Smart call
        case STARTARGPREFIX_SMARTCALL:
            if (len < 2) 
            {
                // invalid argument length
                break;
            }
            
            len = STRLEN(&args[1]) + 1;
            
            // Note: UTF8 encoding can occupy anything from 0.5x to 4x more space
            //       than Unicode encoding of the same string.   We take the
            //       paranoid (and wasteful!) approach and always assume that the
            //       Unicode string will take 2x the length of the UTF8 string
            wStr = MALLOC(len * sizeof(AECHAR));
            
            if (NULL == wStr) 
            {
                break;
            }
            
            (void) UTF8TOWSTR((byte *)&args[1],
                              (int)len, 
                              wStr, 
                              (int)(len*sizeof(AECHAR)));
            
            pMe->m_eStartMethod = STARTMETHOD_SMARTCALL;
            FREEIF(pMe->m_pFindNum);
            pMe->m_pFindNum     = wStr;
            break;

        //If the start method is Mainmenu
        case STARTARGPREFIX_MAINMENU:
            pMe->m_eStartMethod = STARTMETHOD_MAINMENU;
            break;			
            
        default:
            break;
    }
}

/*==============================================================================

����: IContApp_SaveNumber
       
˵��: 
    �ⲿ���ñ����¼�ӿ�
       
����: 
    pi      [in]:IContApp�ӿڶ���ָ�롣
    number  [in]:
    method  [in]:
    saveType[in]:    
       
����ֵ:
    SUCCESS:
       
��ע:
       
==============================================================================*/
static int IContApp_SaveNumber( IContApp            *pi, 
                                AECHAR              *Number,
                                AECHAR              *E_Mail,     
                                AECHAR              *URL,                                
                                PhoneSaveType  saveType)
{
    CContApp *pMe = (CContApp*)pi;
    char     *args = NULL;
    int       nRet;
    
    if ( pMe == NULL ) 
    {
        return EFAILED;
    }

    if ((NULL == Number) && (NULL == E_Mail) &&(NULL == URL))
    {
        return EFAILED;
    }

    if (NULL != Number)
    {
        args = CContApp_BuildStartArgs( STARTMETHOD_SAVEFIELD, &saveType, Number);
    }
    else if (NULL != E_Mail)
    {
        args = CContApp_BuildStartArgs( STARTMETHOD_SAVEFIELD, &saveType, E_Mail);
    }
    else   if (NULL != URL)
    {
        args = CContApp_BuildStartArgs( STARTMETHOD_SAVEFIELD, &saveType, URL);
    }
    
    pMe->m_eStartMethod = STARTMETHOD_SAVEFIELD;
    
    if (TRUE == pMe->m_bActive) 
    {
        // Decode the arguments if applet have run
        CContApp_DecodeStartArgs(pMe, args);
    }
    
    nRet = ISHELL_StartAppletArgs(  pMe->m_pShell,
                                    AEECLSID_APP_CONTACT, 
                                    args);
    
    if (args) 
    {
        FREE(args);
    }
    
    return nRet;
} // IContApp_SaveNumber

/*==============================================================================

����: IContApp_FindNumber
       
˵��: 
    �ⲿ���ò��Ҽ�¼�ӿ�
       
����: 
    pi       [in]:IContApp�ӿڶ���ָ�롣
    searchStr[in]:
    
����ֵ:
    SUCCESS:
       
��ע:
       
==============================================================================*/
static int IContApp_FindNumber(IContApp *pi, AECHAR *searchStr)
{
    CContApp *pMe = (CContApp*)pi;
    char        *args;
    int          nRet;

    if ( pMe == NULL ) 
    {
        return EFAILED;
    }

    if (NULL == searchStr || WSTRLEN(searchStr) <= 0) 
    {
        return EFAILED;
    }
    
    args = CContApp_BuildStartArgs( STARTMETHOD_FINDNUMBER,
                                    NULL,
                                    searchStr);
    
    if (TRUE == pMe->m_bActive) 
    {
        // Decode the arguments if applet have run
        CContApp_DecodeStartArgs(pMe, args);
    }
    
    nRet = ISHELL_StartAppletArgs( pMe->m_pShell,
                                   AEECLSID_APP_CONTACT,
                                   args);
    
    if (args) 
    {
        FREE(args);
    }
    
    return nRet;
} // IContApp_FindNumber

/*==============================================================================

����: IContApp_MainMenu
       
˵��: 
    �ⲿ���ò��Ҽ�¼�ӿ�
       
����: 
    pi       [in]:IContApp�ӿڶ���ָ�롣
    searchStr[in]:
    
����ֵ:
    SUCCESS:
       
��ע:
       
==============================================================================*/
static int IContApp_MainMenu(IContApp *pi)
{
    CContApp *pMe = (CContApp*)pi;
    char        *args;
    int          nRet;

    if ( pMe == NULL ) 
    {
        return EFAILED;
    }
   
    args = CContApp_BuildStartArgs( STARTMETHOD_MAINMENU,
                                    NULL,
                                    NULL);
    
    if (TRUE == pMe->m_bActive) 
    {
        // Decode the arguments if applet have run
        CContApp_DecodeStartArgs(pMe, args);
    }
    
    nRet = ISHELL_StartAppletArgs( pMe->m_pShell,
                                   AEECLSID_APP_CONTACT,
                                   args);
    
    if (args) 
    {
        FREE(args);
    }
    
    return nRet;
} // IContApp_FindNumber

/*==============================================================================

����: IContApp_SelectField
       
˵��: 
    �ⲿ����ѡ���ֶμ�¼�ӿ�
       
����: 
    pi     [in]:IContApp�ӿڶ���ָ�롣
    fnCB   [in]:
    FldType[in]:
    
����ֵ:
    SUCCESS:
       
��ע:
       
==============================================================================*/
static int IContApp_SelectField( IContApp            *pi, 
                                 PFNSELECTEDCONTACTCB fnCB, 
                                 SelectFldType        FldType)
{
    CContApp *pMe = (CContApp*)pi;
    char        *args;
    int          nRet;

    if ( pMe == NULL ) 
    {
        return EFAILED;
    }
    
    args = CContApp_BuildStartArgs( STARTMETHOD_SELECTFIELD,
                                   &FldType,
                                   (void *) (uint32) fnCB);
    
    if (TRUE == pMe->m_bActive) 
    {
        // Decode the arguments if applet have run
        CContApp_DecodeStartArgs(pMe, args);
    }
    
    nRet = ISHELL_StartAppletArgs( pMe->m_pShell,
                                   AEECLSID_APP_CONTACT,
                                   args);
            
    if (args) 
    {
        FREE(args);
    }
    
    return nRet;
} // IContApp_SelectField

/*==============================================================================

����: IContApp_NumberLookup
       
˵��: 
    ���Ҷ�Ӧ�������������Ϣ
       
����: 
    pi       [in]    : IContApp�ӿڶ���ָ�롣
    pNumber  [in]    : Number
    pContInfo[in/out]: ��ѯ������Ϣ���
����ֵ:
    SUCCESS: ������ڸü�¼
       
��ע:
       
==============================================================================*/
static int IContApp_NumberLookup( IContApp   *pi,
                                  AECHAR     *pNumber,
                                  ContInfo   *pContInfo)
{
    PARAM_NOT_REF(pi)
    IAddrBook  *pAddr = NULL;
    AEEAddCacheInfo info = {0};
    AEEAddCacheInfo *pUseinfo = NULL;
#ifdef FEATURE_RUIM_PHONEBOOK
    AEEAddCacheInfo uiminfo = {0};
    boolean    bNeedSearchUIM = FALSE;
#endif
    
    int nRet;
    
    if ((pNumber == NULL) || (NULL == pContInfo))
    {
        return EFAILED;
    }
    if (WSTRLEN(pNumber) <= 1)
    {
        // С��3λ�ĺ��벻����
        return EFAILED;
    }

    // Create IAddrbokk instance for phone
    if (SUCCESS != ISHELL_CreateInstance((AEE_GetShell()),
                                         AEECLSID_ADDRBOOK,
                                         (void **)&pAddr))
    {
        return EFAILED;
    }
    
    nRet = IADDRBOOK_GetCacheinfoByNumber(pAddr, pNumber, &info, ContApp_NumberMatch);

    if(SUCCESS == nRet)
    {
        CContApp_GetRingtone(pAddr, pContInfo->ringName, info.wRecID);
    }
    
    (void) IADDRBOOK_Release(pAddr);
    pAddr = NULL;
    
    if (nRet != SUCCESS)
    {
#ifdef FEATURE_RUIM_PHONEBOOK
        if (IsRunAsUIMVersion())
        {
            bNeedSearchUIM = TRUE;
        }
#endif
        
    }
#ifdef FEATURE_RUIM_PHONEBOOK
    else if ((info.szNumber != NULL) &&
             (ContApp_NumberMatch(info.szNumber, pNumber, NULL) != NUMBERMATCH_EQUAL) &&
             IsRunAsUIMVersion())
    {
        pUseinfo = &info;
        bNeedSearchUIM = TRUE;
    }
#endif    
    else
    {
        pUseinfo = &info;
    }
    
#ifdef FEATURE_RUIM_PHONEBOOK
    if (bNeedSearchUIM)
    {
        if (SUCCESS != ISHELL_CreateInstance((AEE_GetShell()),
                                             AEECLSID_ADDRBOOK_RUIM,
                                             (void **)&pAddr))
        {
            FREEIF(info.szName);
            FREEIF(info.szNumber);
            return EFAILED;
        }
        
        nRet = IADDRBOOK_GetCacheinfoByNumber(pAddr, pNumber, &uiminfo, ContApp_NumberMatch);
        (void) IADDRBOOK_Release(pAddr);
        pAddr = NULL;
        
        if (nRet == SUCCESS)
        {
            int nLen_uim = 0;
            int nLen_Ph = 0;
            Numbermatch_e_Type eRet_uim;
            Numbermatch_e_Type eRet_ph;
            
            eRet_uim = ContApp_NumberMatch(uiminfo.szNumber, pNumber, &nLen_uim);
            if (pUseinfo != NULL)
            {
                eRet_ph = ContApp_NumberMatch(pUseinfo->szNumber, pNumber, &nLen_Ph);
            }
            
            switch (eRet_uim)
            {
                // ƥ������
                case NUMBERMATCH_EQUAL:
                    pUseinfo = &uiminfo;
                    break;
                    
                case NUMBERMATCH_WSTR1_ISTAIL_OF_WSTR2:
                    if (NULL == pUseinfo)
                    {
                        pUseinfo = &uiminfo;
                    }
                    else
                    {
                        switch (eRet_ph)
                        {
                            case NUMBERMATCH_WSTR1_ISTAIL_OF_WSTR2:
                                if (nLen_uim>nLen_Ph)
                                {// UIM �ϵĺ����ƥ��
                                    pUseinfo= &uiminfo;
                                }
                                break;
                                
                            case NUMBERMATCH_WSTR2_ISTAIL_OF_WSTR1:
                            default:
                                break;
                        }
                    }
                    break;

                case NUMBERMATCH_WSTR2_ISTAIL_OF_WSTR1:
                    if (NULL == pUseinfo)
                    {
                        pUseinfo = &uiminfo;
                    }
                    break;
                
                default:
                    break;
            }
        }
    }
#endif
    
    nRet = EFAILED;
    if (NULL != pUseinfo)
    {
        if(IS_RUIM_REC(pUseinfo->wRecID))
        {
            pContInfo->ringName[0] = (AECHAR)'\0';
        }
        
        if (NULL != pUseinfo->szName)
        {
            if (pContInfo->wNameLen > WSTRLEN(pUseinfo->szName))
            {
                (void)WSTRCPY(pContInfo->pName, pUseinfo->szName);
            }
        }
        nRet = SUCCESS;
    }
    FREEIF(info.szName);
    FREEIF(info.szNumber);
#ifdef FEATURE_RUIM_PHONEBOOK
    FREEIF(uiminfo.szName);
    FREEIF(uiminfo.szNumber);
#endif    
    
   return nRet;
} // IContApp_NumberLookup

/*==============================================================================

����: CContApp_CmpNumber
       
˵��: 
    ��ȷƥ�����
       
����: 
    pAddrRec: 
    pNum    : 
    
����ֵ:
    boolean
       
��ע:
       
==============================================================================*/
boolean CContApp_CmpNumber(IAddrRec *pAddrRec, AECHAR *pNumber)
{
    int           i;
    int           nFldsNum;
    AEEAddrField *pFld;
    
    //if(NULL == pAddrRec || pNum == NULL)
    //{
    //    return FALSE;
    //}
    
    nFldsNum = IADDRREC_GetFieldCount(pAddrRec);
    for(i=0; i<nFldsNum; i++)
    {
        pFld = IADDRREC_GetField( pAddrRec, i );
        if(NULL == pFld)
        {
            continue;
        }
        
        if(IS_NUM_FIELD(pFld->fID))
        {
            if(0 == WSTRCMP(pFld->pBuffer, pNumber))
            {
                return TRUE;
            }
        }
    }
    
    return FALSE;
}// CContApp_CmpNumber


/*==============================================================================

����: CContApp_CmpRegionNumber
       
˵��: 
    ƥ����������
       
����: 
    pAddrRec: 
    pNum    : 
    
����ֵ:
    boolean
       
��ע:
       
==============================================================================*/
boolean CContApp_CmpRegionNumber(IAddrRec *pAddrRec, AECHAR *pNumber)
{
    int           i;
    int           nFldsNum;
    AEEAddrField *pFld;
    AECHAR       *pStr;
    
    //if(NULL == pAddrRec || pNum == NULL)
    //{
    //    return FALSE;
    //}
    
    nFldsNum = IADDRREC_GetFieldCount(pAddrRec);
    for(i=0; i<nFldsNum; i++)
    {
        pFld = IADDRREC_GetField( pAddrRec, i );
        if(NULL == pFld)
        {
            continue;
        }
        
        if(IS_NUM_FIELD(pFld->fID))
        {
            pStr = (AECHAR *)pFld->pBuffer;
            if(CContApp_WStrStrEx(pStr, pNumber))
            {
#ifdef FEATURE_PERU_VERSION
//correctly inquire Peruvian cellular phone number in phone book
            	if(WSTRLEN(pNumber) >= 7)

            	{
            	     return TRUE;
                }
#else            
                if(pStr[0]=='0' || pStr[0]=='+')
                {
                    return TRUE;
                }
                else if(WSTRLEN(pNumber) >= 10)
                {
                    return TRUE;
                }
#endif //FEATURE_PERU_VERSION                                
                break;
            }
            else if(CContApp_WStrStrEx(pNumber, pStr))
            {
                if(pNumber[0]=='0' || pNumber[0]=='+')
                {
                    return TRUE;
                }
                else if(WSTRLEN(pStr) >= 10)
                {
                    return TRUE;
                }
                
                break;
            }
        }
    }
    
    return FALSE;
}// CContApp_CmpRegionNumber

/*=============================================================================
FUNCTION: CContApp_WStrStrEx

DESCRIPTION:
   Widestring strstr() function with case sensitive compare flag.

PARAMETERS:
   *pHaystack: string in which pNeedle is found
   *pNeedle:  string which is searched in pHaystack

RETURN VALUE:
boolean:
   Returns true if pNeedle is found in pHaystack. 

COMMENTS:

SIDE EFFECTS:

=============================================================================*/
static boolean CContApp_WStrStrEx( AECHAR *pHaystack, AECHAR *pNeedle)
{
   AECHAR *pTempNdl, *pTempHay;
   AECHAR char1[2],char2[2];

   if((pHaystack == NULL)||(pNeedle == NULL))
   {
       return FALSE;
   }

   char1[1] = char2[1] = (AECHAR)0;

   for (;;)
   {
      pTempNdl = pNeedle;
      pTempHay = pHaystack;


      if ((AECHAR)0 == *pHaystack)
      {
         return FALSE;
      }
      
      for(;;)
      {
         char1[0] = *pTempHay;
         char2[0] = *pTempNdl;
         
         // Compare the characters
         if ((char1[0] == char2[0] )&&(char1[0]!=(AECHAR)0))
         {
            pTempNdl++;
            pTempHay++;
         }
         else
         {
            break;
         }
      }
      
      if ((AECHAR)0 == *pTempNdl) {
         return TRUE;
      }
      
      pHaystack++;
   }
}// CContApp_WStrStrEx


/*==============================================================================

����: CContApp_BuildContInfo
       
˵��: 
    ���� Contact info �ṹ
       
����: 
    pAddrRec  : 
    pContInfo : 
    
����ֵ:
    None
       
��ע:
       
==============================================================================*/
void CContApp_BuildContInfo(IAddrRec *pAddrRec, ContInfo *pContInfo)
{
    int           i;
    int           nFldsNum;
    AEEAddrField *pFld;
    
    if(NULL == pAddrRec || pContInfo == NULL)
    {
        ERR("NULL", 0, 0, 0);
        return;
    }
    
    nFldsNum = IADDRREC_GetFieldCount(pAddrRec);
    for(i=0; i<nFldsNum; i++)
    {
        pFld = IADDRREC_GetField( pAddrRec, i );
        if(NULL == pFld)
        {
            continue;
        }
        
        switch(pFld->fID)
        {
            case AEE_ADDRFIELD_NAME:
                if(pContInfo->pName)
                {
                    WSTRNCOPYN(pContInfo->pName,
                               (int)pContInfo->wNameLen,
                               (AECHAR *)pFld->pBuffer,
                               -1);
                }
                break;
           //guoys modified for add ringtone     
            case AEE_ADDRFIELD_RINGTONE:
                if(pFld->pBuffer != NULL)
                {
                    WSTRCPY(pContInfo->ringName, (AECHAR*)pFld->pBuffer);
                }
                break;
            
            case AEE_ADDRFIELD_PHOTO:
            {
                char szName[AEE_MAX_FILE_NAME] = {0};
                int  len;
                
                len = WSTRLEN(pFld->pBuffer);
                
                if(len == 0)
                {
                    break;
                }
                
                // Generate the filename
                STRCPY(pContInfo->szPhoto, MP_CONTPIC_DIR);
                STRCAT(pContInfo->szPhoto, DIRECTORY_STR);
                (void)WSTRTOUTF8( (AECHAR *) pFld->pBuffer,
                                  (int)      len, 
                                  (byte *)   szName, 
                                  (int)      sizeof(szName));;
                STRCAT(pContInfo->szPhoto, szName);
                break;
            }
            
            default:
                break;
        }
    }
}// CContApp_BuildContInfo

/*==============================================================================

����: IContApp_SpeedDialLookup
       
˵��: 
    ���Ҷ�Ӧ�Ŀ��ٲ�����Ϣ
       
����: 
    pMe       [in]: IContApp�ӿڶ���ָ�롣
    pSpeedDial[in]: Speed dial text
    pNumber   [in]: Number
    wNumLen   [in]: ��ѯ������Ϣ���buffer���ȣ�in AECHAR��
    
����ֵ:
    SUCCESS: ������ڸü�¼
       
��ע:
       
==============================================================================*/
static int IContApp_SpeedDialLookup( IContApp   *pi,
                                     AECHAR     *pSpeedDial,
                                     AECHAR     *pNumber,
                                     uint16      wNumLen)
{
    CContApp *pMe = (CContApp*)pi;
    
    if ( pMe == NULL || pSpeedDial == NULL) 
    {
        return EFAILED;
    }
    
    // Nothing to look up
    if(WSTRLEN(pSpeedDial) == 0)
    {
        return EFAILED;
    }
    
    // OneDial ��������
    if(WSTRLEN(pSpeedDial) == 1)
    {
        return CContApp_GetOneDial(pMe, pSpeedDial, pNumber, wNumLen);
    }
    else if(WSTRLEN(pSpeedDial) <= MAX_LEN_SPEEDDIAL)
    {
        if(0 != CContApp_GetSmartDial(pMe, pSpeedDial, pNumber, wNumLen))
        {
            return SUCCESS;
        }
    }// else it is not speed dial then return EFAILED
    
    return EFAILED;
} // IContApp_SpeedDialLookup

/*==============================================================================

����: IContApp_SpeedDialCall
       
˵��: 
    ���ж�Ӧ�Ŀ��ٲ�����Ϣ
       
����: 
    pMe       [in]: IContApp�ӿڶ���ָ�롣
    pSpeedDial[in]: Speed dial text
    
����ֵ:
    SUCCESS: ������ڸü�¼
       
��ע:
       
==============================================================================*/
static int IContApp_SpeedDialCall(IContApp *pi, AECHAR *pSpeedDial)
{
    CContApp *pMe = (CContApp*)pi;
    AECHAR    pNumber[MAX_INPUT_NUM+1];
    int       nCnt;
    char     *args;
    int       nRet;
    
    if ( pMe == NULL || pSpeedDial == NULL) 
    {
        return EFAILED;
    }

    // Nothing to look up
    if(WSTRLEN(pSpeedDial) == 0)
    {
        return EFAILED;
    }

    // One dial call not smart call
    if(WSTRLEN(pSpeedDial) == 1)
    {
        return CContApp_OneDialCall(pMe, pSpeedDial);
    }
    
    // Check the smart call
    nCnt = CContApp_GetSmartDial(pMe, pSpeedDial, pNumber, MAX_INPUT_NUM);
    if(0 == nCnt)
    {
        return EFAILED;
    }
    else if(1 == nCnt)
    {
#if 1
        WSTRLCPY(pSpeedDial,pNumber,MAX_INPUT_NUM);
#else
        // Call this number
        // Create the Call instance
        if ( SUCCESS != ISHELL_CreateInstance( (AEE_GetShell()),
                                               AEECLSID_DIALER,
                                               (void **)&pMe->m_pCallApp))
        {
            return EFAILED;
        }
        
        // Make a voice call
        ICallApp_CallNumber(pMe->m_pCallApp, pNumber);
        
        if (pMe->m_pCallApp != NULL) 
        {
            ICallApp_Release(pMe->m_pCallApp);
            pMe->m_pCallApp = NULL;
        }
#endif
        return SUCCESS;
    }// else start the contacts applet
    
    // Smart call
    args = CContApp_BuildStartArgs( STARTMETHOD_SMARTCALL,
                                    NULL,
                                    pSpeedDial);
    
    if (TRUE == pMe->m_bActive) 
    {
        // Decode the arguments if applet have run
        CContApp_DecodeStartArgs(pMe, args);
    }
    
    nRet = ISHELL_StartAppletArgs( AEE_GetShell(),
                                   AEECLSID_APP_CONTACT,
                                   args);
    
    if (args) 
    {
        FREE(args);
    }
    
    return nRet;
} // IContApp_SpeedDialCall

/*==============================================================================

����: CContApp_OneDialCall
       
˵��: 
    ���ж�Ӧ�Ŀ��ٲ�����Ϣ
       
����: 
    pMe     [in]: CContApp�ӿڶ���ָ�롣
    pOneDial[in]: Speed dial text
    
����ֵ:
    SUCCESS: ������ڸü�¼
       
��ע:
       
==============================================================================*/
static int CContApp_OneDialCall(CContApp *pMe, AECHAR *pOneDial)
{
    AECHAR pNumber[MAX_INPUT_NUM+1];
    
    if ( pMe == NULL || pOneDial == NULL) 
    {
        return EFAILED;
    }
    
    FARF(ADDR, ("Contact application Make one dial Call"));
    
    // Get the onedial number
    if(SUCCESS != CContApp_GetOneDial(pMe, pOneDial, pNumber, MAX_INPUT_NUM))
    {
        return EFAILED;
    }
#if 1
    WSTRLCPY(pOneDial,pNumber,MAX_INPUT_NUM);
#else
    // Create the Call instance
    if ( SUCCESS != ISHELL_CreateInstance( (AEE_GetShell()),
                                           AEECLSID_DIALER,
                                           (void **)&pMe->m_pCallApp))
    {
        return EFAILED;
    }
    
    // Make a voice call
    ICallApp_CallNumber(pMe->m_pCallApp, pNumber);
    
    if (pMe->m_pCallApp != NULL) 
    {
        ICallApp_Release(pMe->m_pCallApp);
        pMe->m_pCallApp = NULL;
    }
#endif
    return SUCCESS;
} // CContApp_OneDialCall


/*==============================================================================

����: CContApp_GetOneDial
       
˵��: 
    ��ȡ�������ŵ���Ϣ
       
����: 
    pMe       [in]: IContApp�ӿڶ���ָ�롣
    pSpeedDial[in]: Speed dial text
    pNumber   [in]: Number
    wNumLen   [in]: ��ѯ������Ϣ���buffer���ȣ�in AECHAR��
    
����ֵ:
    SUCCESS: ������ڸü�¼
       
��ע:
       
==============================================================================*/
static int CContApp_GetOneDial( CContApp   *pMe,
                                AECHAR     *pSpeedDial,
                                AECHAR     *pNumber,
                                uint16      wNumLen)
{
    uint16        wIndex;
    
    wIndex = (uint16)WSTRTOFLOAT(pSpeedDial);
    
    FARF(ADDR, ("Get Onedial %s %d", pSpeedDial, wIndex));
    
    // No index 0 in one dial
    if(wIndex == 0)
    {
        return EFAILED;
    }
    
    // Get the config data
    if(pMe->m_pFileMgr)
    {
        CContApp_CFGCacheInit(&pMe->m_sCFGCache);
        // Read the config file
        (void)CContApp_ReadCFG(pMe->m_pFileMgr, &pMe->m_sCFGCache);
        // Get config
        wIndex = wIndex-2;
        (void)CContApp_GetConfig( pMe, 
                                 (ContAppCFG)(CONTCFG_ONEDIAL1 + wIndex),
                                 pMe->m_pOneDialBuf,
                                  sizeof(pMe->m_pOneDialBuf));
        
        // No onedial data
        if(pMe->m_pOneDialBuf[0] == ONEDIAL_NULL)
        {
            FARF(ADDR, ("Failed to get ONEDIAL %d", wIndex));
            return EFAILED;
        }

        if(pNumber)
        {
            WSTRNCOPYN( pNumber, (int)wNumLen, pMe->m_pOneDialBuf, -1);
        } 
        else
        {
            return EFAILED;
        }
    }
    
    return SUCCESS;
}// CContApp_GetOneDial

/*==============================================================================

����: CContApp_GetSmartDial
       
˵��: 
    ��ȡ���ܲ��ŵ���Ϣ
       
����: 
    pMe       [in]: IContApp�ӿڶ���ָ�롣
    pSpeedDial[in]: Speed dial text
    pNumber   [in]: Number
    wNumLen   [in]: ��ѯ������Ϣ���buffer���ȣ�in AECHAR��
    
����ֵ:
    int : ƥ��ļ�¼��
       
��ע:
       
==============================================================================*/
static int CContApp_GetSmartDial( CContApp   *pMe,
                                  AECHAR     *pSpeedDial,
                                  AECHAR     *pNumber,
                                  uint16      wNumLen)
{
    IAddrBook  *pAddrPhone;     // IADDRBOOK interface for phone
#ifdef FEATURE_RUIM_PHONEBOOK
    IAddrBook  *pAddrRUIM;      // IADDRBOOK interface for RUIM
    AEEAddrField *pFld;
#endif// FEATURE_RUIM_PHONEBOOK
    IAddrRec     *pAddrRec;
    void         *pCache;
    AECHAR       *pName;
    uint16        wRecID;
    boolean       bSmartDial;
    AECHAR        alpha[MAX_INPUT_NAME_EN +1];
    int           nRet;
    int           nCnt   = 0;
    boolean       bFound = FALSE;
    
    // Get the config data
    if(pMe->m_pFileMgr)
    {
        CContApp_CFGCacheInit(&pMe->m_sCFGCache);
        // Read the config file
        (void)CContApp_ReadCFG(pMe->m_pFileMgr, &pMe->m_sCFGCache);
    }
    else
    {
        return(0);
    }
    (void)CContApp_GetConfig( pMe, 
                              CONTCFG_SMARTDIAL,
                             &bSmartDial,
                              sizeof(bSmartDial));
    if(FALSE == bSmartDial)
    {
        return(0);
    }
    
#ifdef FEATURE_RUIM_PHONEBOOK
    if( IsRunAsUIMVersion() )
    {
        // Create IAddrbokk instance for Ruim
        if ( SUCCESS != ISHELL_CreateInstance( (AEE_GetShell()),
                                               AEECLSID_ADDRBOOK_RUIM,
                                               (void **)&pAddrRUIM))
        {
            return (0);
        }
        
        // ��RUIM ��Cache�����¼
        nRet = IADDRBOOK_EnumCacheInit( pAddrRUIM,
                                        AEE_ADDR_CAT_NONE,
                                        AEE_ADDRFIELD_NONE,
                                        NULL,
                                        0);
        if(SUCCESS != nRet)
        {
            FARF(ADDR, ("Failed to Enum init %d", nRet));
            (void) IADDRBOOK_Release(pAddrRUIM);
            return(0);
        }
        
        for(;;)
        {
            // Search the cache
            if(SUCCESS != IADDRBOOK_EnumNextCache(pAddrRUIM, &pCache))
            {
                break;
            }
            
            // Find the record
            wRecID = IADDRBOOK_ExtractCache( pAddrRUIM, 
                                             pCache, 
                                            &pName, 
                                             NULL);
            // Convert to alpha
            WStr2Alpha(alpha, MAX_INPUT_NAME_EN, pName);
            
            if(0 != WSTRCMP(alpha, pSpeedDial))
            {
                continue;
            }
            
            // Increase the count
            nCnt++;
            
            // Whether or not have find the record
            if(bFound)
            {
                continue;
            }
            bFound = TRUE;
            
            pAddrRec = IADDRBOOK_GetRecByID(pAddrRUIM, wRecID);
            if(pAddrRec)
            {
                // Get number field
                pFld = IADDRREC_GetField(pAddrRec, IDX_NUM_RUIM_FLD);
                if(pFld == NULL)
                {
                    IADDRREC_Release(pAddrRec);
                    return(0);
                }
                
                // Copy number string
                if(pNumber)
                {
                    WSTRNCOPYN( pNumber,
                               (int)wNumLen,
                               (AECHAR *)pFld->pBuffer,
                                -1);
                }
                
                IADDRREC_Release(pAddrRec);
                pAddrRec = NULL;
            }
            else
            {
                nCnt--;
                bFound = FALSE;
            }
        }
        
        if(pAddrRUIM)
        {
            (void) IADDRBOOK_Release(pAddrRUIM);
        }
    }
#endif// FEATURE_RUIM_PHONEBOOK

    // Create IAddrbokk instance for phone
    if ( SUCCESS != ISHELL_CreateInstance( (AEE_GetShell()),
                                           AEECLSID_ADDRBOOK,
                                           (void **)&pAddrPhone))
    {
        return (0);
    }
    
    // �ӵ绰��Cache�����¼
    nRet = IADDRBOOK_EnumCacheInit( pAddrPhone,
                                    AEE_ADDR_CAT_NONE,
                                    AEE_ADDRFIELD_NONE,
                                    NULL,
                                    0);
    if(SUCCESS != nRet)
    {
        FARF(ADDR, ("Failed to Enum init %d", nRet));
        (void) IADDRBOOK_Release(pAddrPhone);
        return(nCnt);
    }
    
    for(;;)
    {
        // Search the cache
        if(SUCCESS != IADDRBOOK_EnumNextCache(pAddrPhone, &pCache))
        {
            break;
        }
        
        // Find the record
        wRecID = IADDRBOOK_ExtractCache( pAddrPhone, 
                                         pCache, 
                                        &pName, 
                                         NULL);
        // Convert to alpha
        WStr2Alpha(alpha, MAX_INPUT_NAME_EN, pName);
        
        if(0 != WSTRCMP(alpha, pSpeedDial))
        {
            continue;
        }
        
        // Increase the count
        nCnt++;
        
        // Whether or not have find the record
        if(bFound)
        {
            continue;
        }
        bFound = TRUE;
        
        pAddrRec = IADDRBOOK_GetRecByID(pAddrPhone, wRecID);
        if(pAddrRec)
        {
            nRet = CContApp_GetNumber(pAddrRec, pNumber, wNumLen);
            IADDRREC_Release(pAddrRec);
            pAddrRec = NULL;
            if(nRet != SUCCESS)
            {
                nCnt--;
                bFound = FALSE;
            }
        }
        else
        {
            nCnt--;
            bFound = FALSE;
        }
    }
    
    if(pAddrPhone)
    {
        (void) IADDRBOOK_Release(pAddrPhone);
    }

    return nCnt;
}// CContApp_GetSmartDial

/*==============================================================================

����: CContApp_GetNumber
       
˵��: 
    ��ȡ��¼�е�Number�ֶ�
       
����: 
    pAddrRec  : 
    pNumber   [in]: Number
    wNumLen   [in]: ��ѯ������Ϣ���buffer���ȣ�in AECHAR��
    
����ֵ:
    None
       
��ע:
       
==============================================================================*/
static int CContApp_GetNumber( IAddrRec   *pAddrRec,
                               AECHAR     *pNumber,
                               uint16      wNumLen)
{
    int           i;
    int           nFldsNum;
    AEEAddrField *pFld;
    
    nFldsNum = IADDRREC_GetFieldCount(pAddrRec);
    for(i=0; i<nFldsNum; i++)
    {
        pFld = IADDRREC_GetField( pAddrRec, i );
        if(NULL == pFld)
        {
            continue;
        }
        
        // Is it number field
        if(IS_NUM_FIELD(pFld->fID))
        {
            // Copy number string
            if(pNumber)
            {
                WSTRNCOPYN( pNumber,
                           (int)wNumLen,
                           (AECHAR *)pFld->pBuffer,
                            -1);
            }
            
            return SUCCESS;
        }
    }
    
    return EFAILED;
}// CContApp_GetNumber


static int IContApp_DeleteAll(IContApp *pi)
{
    CContApp *pMe = (CContApp*)pi;
    
    IADDRBOOK_RemoveAllRecs(pMe->m_pAddrPhone);
/*
#ifdef FEATURE_RUIM_PHONEBOOK
   if(IsRunAsUIMVersion())
   {
       IADDRBOOK_RemoveAllRecs(pMe->m_pAddrRUIM);
   }
#endif

    IVector_RemoveAllElements(pMe->m_pAddList);
*/                 
    return SUCCESS;
}

#ifdef FEATURE_SUPPORT_VCARD
static int  IContApp_ReadVcfFile(IContApp *pi, char* fileName, int * cnt)
{
    CContApp *pMe = (CContApp*)pi;
    
    if(ContApp_ReadVcfFile(pMe, fileName, cnt))
    {
        return SUCCESS;
    }

    return EFAILED;
}

static int  IContApp_CreateVcfFile(IContApp *pi, int contId, char* fileName)
{
    CContApp *pMe = (CContApp*)pi;
    
    if(Vcard_CreateVcfFile(pMe, contId, fileName))
    {
        return SUCCESS;
    }

    return EFAILED;
}


static int  IContApp_CreateVcfFileEx(IContApp *pi, AEEAddrField *pFields, int nFldCnt, char* fileName)
{
    CContApp *pMe = (CContApp*)pi;
    
    if(Vcard_CreateVcfFileEx(pMe, pFields, nFldCnt, fileName))
    {
        return SUCCESS;
    }

    return EFAILED;
}

static int   IContApp_CheckCapacity(IContApp *pi, boolean *isFull)
{
    CContApp *pMe = (CContApp*)pi;

    if(IADDRBOOK_GetNumRecs(pMe->m_pAddrPhone) >= IADDRBOOK_GetCapacity(pMe->m_pAddrPhone))
    {
        *isFull = TRUE;
    }
    else
    {
        *isFull = FALSE;
    }

    return SUCCESS;
    
}

#endif
