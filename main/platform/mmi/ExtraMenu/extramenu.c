
/*==============================================================================
// �ļ���
//        ExtraMenu.c
//        ��Ȩ����(c) 2004 Anylook
//
// �ļ�˵����
//
// ���ߣ�Gemsea
// �������ڣ�2007-11-14
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
07-11-14                 ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#ifndef AEE_SIMULATOR
#include "customer.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif

#if !defined(BREW_STATIC_APP)
// Use the .bid file if compiling as dynamic applet
   #include "extramenu.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_EXTRAMENU
      #error AEECLSID_EXTRAMENU must be defined
   #endif
#endif

#include "extramenu.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
#include "ExtraMenu.h"
#include "OEMFeatures.h"
#include "AppComFunc.h"
#include "Appscommon.h"

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// �رնԻ����
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                                 }

// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// ExtraMenu_APPIsReadyTimer����
#define APPISREADY_TIMER       100                     
#define EVT_APPISREADY   (EVT_USER+1)                  

// �û����¸��½����¼�

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            ExtraMenuState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }


#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#define EXTRAMENU_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif//FEATURE_LCD_TOUCH_ENABLE
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   // ��ʼֵ��������Ҫ�����Ի���
   DLGRET_CREATE,

   // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_OK,
   DLGRET_CANCELED
} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// ExtraMenu Applet ״̬��״̬��
typedef enum _ExtraMenuState
{
   EXTRAMENUST_NONE,
   EXTRAMENUST_INIT,
   EXTRAMENUST_MAIN,
   EXTRAMENUST_EXIT,

} ExtraMenuState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct ExtraMenuMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} ExtraMenuMod;

// ExtraMenu Applet����ṹ�壺
typedef struct _CExtraMenu
{
    DECLARE_VTBL(IExtraMenu)
    uint32          m_nRefs;          // �������ü�����
    IModule        *m_pModule;
    IDisplay        *m_pDisplay;
    IShell            *m_pShell;

    AEERect       m_rc;
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog             *m_pActiveDlg;
    // ��ǰ��Ի���ID
    uint16 m_pActiveDlgID;
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRet_Value_e_Type   m_eDlgRet;
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean m_bNotOverwriteDlgRet;
    // Applet ǰһ״̬
    ExtraMenuState m_ePreState;
    // Applet ��ǰ״̬
    ExtraMenuState m_eCurState;
    // Applet�Ƿ��ڹ���״̬
    boolean m_bSuspending;
    // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean         m_bAppIsReady;

    uint16          m_currDlgId;       //���浱ǰ��ѡ�˵�λ��
} CExtraMenu;


/*==============================================================================
                                 ��������
==============================================================================*/
static void ExtraMenu_APPIsReadyTimer(void *pme);

// �Ի��� IDD_EXTRAS_MENU �¼�������
static boolean  HandleMainDialogEvent(CExtraMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// ״̬ EXTRAMENUST_NONE ������
static NextFSMAction StateNoOperationHandler(CExtraMenu *pMe);

// ״̬ EXTRAMENUST_INIT ������
static NextFSMAction StateInitHandler(CExtraMenu *pMe);

// ״̬ EXTRAMENUST_MAIN ������
static NextFSMAction StateMainHandler(CExtraMenu *pMe);

// ״̬ EXTRAMENUST_EXIT ������
static NextFSMAction StateExitHandler(CExtraMenu *pMe);


/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
       ExtraMenu_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��ExtraMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void ExtraMenu_ShowDialog(CExtraMenu *pMe,uint16  dlgResId);

/*==============================================================================
����:
       ExtraMenu_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��ExtraMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean ExtraMenu_RouteDialogEvent(CExtraMenu *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       ExtraMenu_ProcessState
˵��:
       ExtraMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��ExtraMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction ExtraMenu_ProcessState(CExtraMenu *pMe);

/*----------------------ģ����غ�������---------------------*/

int  ExtraMenuMod_Load(IShell   *pIShell,
    void     *ph,
    IModule **ppMod
);

int  ExtraMenuMod_New(int16  nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
);

static int  ExtraMenuMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
);

static uint32 ExtraMenuMod_AddRef(IModule *po);

static uint32 ExtraMenuMod_Release(IModule *po);

static void ExtraMenuMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
);

/*----------------------Applet��غ�������---------------------*/
static int ExtraMenu_New(IShell *ps,
    IModule *pIModule,
    IExtraMenu **ppObj
);

static uint32  ExtraMenu_AddRef(IExtraMenu *pi);

static uint32  ExtraMenu_Release (IExtraMenu *pi);

static boolean ExtraMenu_HandleEvent(IExtraMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static int ExtraMenu_InitAppData(CExtraMenu *pMe);

static void ExtraMenu_FreeAppData(CExtraMenu *pMe);

static void ExtraMenu_RunFSM(CExtraMenu *pMe);

/*==============================================================================
                                 ȫ������
==============================================================================*/
// ��Ҫ��ʾ��
//        g_dwAEEStdLibEntryȫ�ֱ���������SDK ��
//        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ㣬��
//        Ҫ�ƶ��������д��롣
#ifdef AEE_SIMULATOR
   uint32 g_dwAEEStdLibEntry;
#ifndef AEE_STATIC //warning for dynamic building only
   #error This applet was never intended to be used with the AEE Simulator.
#endif
#endif

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
static ExtraMenuMod gExtraMenuMod;

static const VTBL(IModule) gModFuncs =
{
    ExtraMenuMod_AddRef,
    ExtraMenuMod_Release,
    ExtraMenuMod_CreateInstance,
    ExtraMenuMod_FreeResources
};

// ֻ����һ��ExtraMenuʵ����ÿ�δ���ExtraMenu Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CExtraMenu gExtraMenu;

static const VTBL(IExtraMenu) gExtraMenuMethods =
{
    ExtraMenu_AddRef,
    ExtraMenu_Release,
    ExtraMenu_HandleEvent
};

/*----------------------ģ����غ�������---------------------*/

/*==============================================================================
����:
       ExtraMenuMod_Load

˵��:
       ģ��װ�غ�����

����:
       pIShell [in]��IShell�ӿ�ָ�롣
       ph ��û��ʹ��
       ppMod [out]��

����ֵ:
       int

��ע:

==============================================================================*/
#if defined(AEE_SIMULATOR)&&!defined( AEE_STATIC) //for dynamic building only
__declspec(dllexport) int AEEMod_Load(IShell   *pIShell,
                                                        void     *ph,
                                                        IModule **ppMod)
#else
int ExtraMenuMod_Load(IShell   *pIShell,
                                                   void     *ph,
                                                   IModule **ppMod)
#endif
{
    // ���ڶ�̬Applet����֤AEE STD��汾����ȷ�����뽨����ģ��ʱʹ�õĿ�汾ƥ��
    // ����AEE_SIMULATOR�²��ؽ��б���顣
#if !defined(BREW_STATIC_APP) && !defined(AEE_SIMULATOR)
    if (GET_HELPER_VER() != AEESTDLIB_VER)
    {
        return EVERSIONNOTSUPPORT;
    }
#endif

    return ExtraMenuMod_New(sizeof(ExtraMenuMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*==============================================================================
����:
       ExtraMenuMod_New

˵��:
       ������ʼ��ExtraMenuģ�������Ҳ�������ü����������ֺ�����

����:
       nSize:        û��ʹ��
       pIShell [In]: IShell�ӿ�ָ��
       ph ��û��ʹ��
       ppMod [out]��
       pfnMC:        û��ʹ��
       pfnMF:        û��ʹ��

����ֵ:
       int

��ע:

==============================================================================*/
int  ExtraMenuMod_New(int16  nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
)
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

    // ��Ҫ��ʾ��:
    //        g_dwAEEStdLibEntryȫ�ֱ���������SDK ��
    //        BREW SDK�û����ܸ�д��ֱ��ʹ�ñ�������������AEEStdLib��һ����ڵ�
    //        ��
#ifdef AEE_SIMULATOR
    g_dwAEEStdLibEntry = (uint32)ph;
    if (!pIShell || !ph)
    {
        return EFAILED;
    }
#else
    if (!pIShell)
    {
        return EFAILED;
    }
#endif

    MEMSET(&gExtraMenuMod,0,sizeof(ExtraMenuMod));

    INIT_VTBL(&gExtraMenuMod,IModule,gModFuncs);
    gExtraMenuMod.m_nRefs = 1;
    *ppMod = (IModule *)&gExtraMenuMod;
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       ExtraMenuMod_AddRef

˵��:
       ExtraMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��ExtraMenu ģ���������

��ע:

==============================================================================*/
static uint32 ExtraMenuMod_AddRef(IModule *po)
{
    return(++((ExtraMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       ExtraMenuMod_Release

˵��:
       ExtraMenu ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��ExtraMenu ģ���������

��ע:

==============================================================================*/
static uint32 ExtraMenuMod_Release(IModule *po)
{
    if (((ExtraMenuMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((ExtraMenuMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       ExtraMenuMod_CreateInstance

˵��:
        ͨ������(��ʼ��)ExtraMenu����������ģ��ʵ����

����:
       po [in]��IModule�ӿڶ���ָ�롣
       pIShell [in]��IShell�ӿڶ���ָ�롣
       ClsId [in]��Ҫ������ʵ������ID��
       ppObj [out]:�����ڱ������ʵ����ָ���ָ�������

����ֵ:
       EFAILED;����ʧ�ܡ�
       SUCCESS;�����ɹ���

��ע:
       �����û�ȥ���á�

==============================================================================*/
static int  ExtraMenuMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_EXTRAMENU)
    {
        return EFAILED;
    }

    if (ExtraMenu_New(pIShell, po, (IExtraMenu**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       ExtraMenuMod_FreeResources

˵��:
        �ͷ�ExtraMenuģ����Դ�ĺ�����

����:
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣

����ֵ:
       ��

��ע:
       �����û�ȥ���á�

==============================================================================*/
static void ExtraMenuMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
)
{

}

/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����:
       ExtraMenu_New

˵��:
        ��ʼ��ExtraMenu Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��IExtraMenu����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int ExtraMenu_New(IShell *ps,
    IModule *pIModule,
    IExtraMenu **ppObj
)
{
    int retVal = SUCCESS;

    if (0 == gExtraMenu.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&gExtraMenu,  0, sizeof(CExtraMenu));
        INIT_VTBL(&gExtraMenu, IExtraMenu, gExtraMenuMethods);

        gExtraMenu.m_nRefs     = 0;
        gExtraMenu.m_pShell    = ps;
        gExtraMenu.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = ExtraMenu_InitAppData(&gExtraMenu);
    }

    ++gExtraMenu.m_nRefs;
    *ppObj = (IExtraMenu*)&gExtraMenu;
    return retVal;
}

/*==============================================================================
����:
       ExtraMenu_AddRef

˵��:
       ExtraMenu Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��IExtraMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��ExtraMenu Applet��������

��ע:

==============================================================================*/
static uint32  ExtraMenu_AddRef(IExtraMenu *pi)
{
    register CExtraMenu *pMe = (CExtraMenu*)pi;

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       ExtraMenu_Release

˵��:
       �ͷŶ�ExtraMenu Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��IExtraMenu�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��ExtraMenu Applet��������

��ע:

==============================================================================*/
static uint32  ExtraMenu_Release (IExtraMenu *pi)
{
    register CExtraMenu *pMe = (CExtraMenu*)pi;

    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    // �ͷ�Applet�����Դ
    ExtraMenu_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       ExtraMenu_InitAppData

˵��:
       ��ʼ��ExtraMenu Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��ExtraMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

==============================================================================*/
static int ExtraMenu_InitAppData(CExtraMenu *pMe)
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
    pMe->m_ePreState = EXTRAMENUST_NONE;
    pMe->m_eCurState = EXTRAMENUST_INIT;
    pMe->m_pActiveDlg = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;

    pMe->m_currDlgId = 0;

    return SUCCESS;
}

/*==============================================================================
����:
       ExtraMenu_FreeAppData

˵��:
       �ͷ�ExtraMenu Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��ExtraMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void ExtraMenu_FreeAppData(CExtraMenu *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }

}

/*==============================================================================
����:
       ExtraMenu_RunFSM

˵��:
       ExtraMenu Applet����״̬�����档

����:
       pMe [in]��ָ��ExtraMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void ExtraMenu_RunFSM(CExtraMenu *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = ExtraMenu_ProcessState(pMe);

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
       ExtraMenu_HandleEvent

˵��:
       ExtraMenu  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��IExtraMenu�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
static boolean ExtraMenu_HandleEvent(IExtraMenu *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CExtraMenu *pMe = (CExtraMenu*)pi;
    AEEAppStart *as;

    switch (eCode)
    {
        case EVT_APP_START:
            pMe->m_bAppIsReady = FALSE;
            
            as = (AEEAppStart*)dwParam;
            if (NULL != pMe->m_pDisplay)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            pMe->m_pDisplay = as->pDisplay;
            (void) IDISPLAY_AddRef(pMe->m_pDisplay);
            pMe->m_rc = as->rc;

            pMe->m_bSuspending = FALSE;

            // ��ʼExtraMenu״̬��
            ExtraMenu_RunFSM(pMe);
            return TRUE;

        case EVT_APP_STOP:
            if (pMe->m_pDisplay != NULL)
            {
                (void) IDISPLAY_Release(pMe->m_pDisplay);
                pMe->m_pDisplay = NULL;
            }
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            pMe->m_bSuspending = TRUE;

            return TRUE;

        case EVT_APP_SUSPEND:
            pMe->m_bSuspending = TRUE;
            (void)ISHELL_CancelTimer(pMe->m_pShell, NULL, pMe);
            return TRUE;

        case EVT_APP_RESUME:
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

            ExtraMenu_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            return ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            ExtraMenu_APPIsReadyTimer,
                            pMe);
            return TRUE;

        case EVT_APPISREADY:
            (void) ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_bAppIsReady = TRUE;
            return TRUE;

        case EVT_KEY_PRESS:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY_RELEASE:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY_HELD:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_COMMAND:
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
            return ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void) ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼExtraMenu״̬��
                ExtraMenu_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
			{
				return ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}

		case EVT_USER:
			{
				if((wParam == AVK_SELECT) || (wParam == AVK_INFO))
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
				else if(wParam == AVK_CLR)
				{
					eCode = EVT_KEY;
				}
			}
#endif//FEATURE_LCD_TOUCH_ENABLE
        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return ExtraMenu_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
       ExtraMenu_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCExtraMenu�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void ExtraMenu_APPIsReadyTimer(void *pme)
{
    CExtraMenu *pMe = (CExtraMenu *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_EXTRAMENU,
                            EVT_APPISREADY,
                            0,
                            0);
}


/*==============================================================================
����:
       ExtraMenu_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��ExtraMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void ExtraMenu_ShowDialog(CExtraMenu *pMe,uint16  dlgResId)
{
    int nRet;

    // At most one dialog open at once
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        return;
    }

    nRet = ISHELL_CreateDialog(pMe->m_pShell,AEE_EXTRAMENU_RES_FILE,dlgResId,NULL);
}

/*==============================================================================
����:
       ExtraMenu_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��ExtraMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean ExtraMenu_RouteDialogEvent(CExtraMenu *pMe,
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
        case IDD_EXTRA_DIALOG:
            return HandleMainDialogEvent(pMe,eCode,wParam,dwParam);

        default:
            return FALSE;
    }
}




/*==============================================================================
����:
       ExtraMenu_ProcessState
˵��:
       ExtraMenu Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��ExtraMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction ExtraMenu_ProcessState(CExtraMenu *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case EXTRAMENUST_NONE:
            retVal = StateNoOperationHandler(pMe);
            break;

        case EXTRAMENUST_INIT:
            retVal = StateInitHandler(pMe);
            break;

        case EXTRAMENUST_MAIN:
            retVal = StateMainHandler(pMe);
            break;

        case EXTRAMENUST_EXIT:
            retVal = StateExitHandler(pMe);
            break;


        default:
            break;
    }
    return retVal;
}

/*==============================================================================
������
       StateNoOperationHandler
˵����
       EXTRAMENUST_NONE ״̬������

������
       pMe [in]��ָ��ExtraMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateNoOperationHandler(CExtraMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(EXTRAMENUST_EXIT)
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler


/*==============================================================================
������
       StateInitHandler
˵����
       EXTRAMENUST_INIT ״̬������

������
       pMe [in]��ָ��ExtraMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateInitHandler(CExtraMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(EXTRAMENUST_MAIN)
    return NFSMACTION_CONTINUE;
} // StateInitHandler


/*==============================================================================
������
       StateMainHandler
˵����
       EXTRAMENUST_MAIN ״̬������

������
       pMe [in]��ָ��ExtraMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateMainHandler(CExtraMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            ExtraMenu_ShowDialog(pMe, IDD_EXTRA_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(EXTRAMENUST_EXIT)
            return NFSMACTION_CONTINUE;

        default:
            break;
    }

    return NFSMACTION_WAIT;
} // StateMainHandler


/*==============================================================================
������
       StateExitHandler
˵����
       EXTRAMENUST_EXIT ״̬������

������
       pMe [in]��ָ��ExtraMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateExitHandler(CExtraMenu *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    (void) ISHELL_CloseApplet(pMe->m_pShell, FALSE);

    return NFSMACTION_WAIT;
} // StateExitHandler


/*==============================================================================
������
       HandleMainDialogEvent
˵����
       IDD_EXTRAS_MENU�Ի����¼�������

������
       pMe [in]��ָ��ExtraMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  HandleMainDialogEvent(CExtraMenu *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    static boolean isSelectByNumberKey = FALSE;
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_EXTRA_DIALOG);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
#ifdef FEATURE_SUPPORT_BT_APP
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_BLUETOOTH_TITLE,  IDS_BLUETOOTH_TITLE, NULL, 0);
#endif
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_ALARMCLOCK_TITLE,  IDS_ALARMCLOCK_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_CALENDAR_TITLE, IDS_CALENDAR_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_CALCULATOR_TITLE,  IDS_CALCULATOR_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_STOPWATCH_TITLE,  IDS_STOPWATCH_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_WORLDTIME_TITLE,  IDS_WORLDTIME_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_CONVERTER_TITLE, IDS_CONVERTER_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_TIMEER_TITLE, IDS_TIMEER_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_RECORDER_TITLE,  IDS_RECORDER_TITLE, NULL, 0);

#ifdef FEATURE_MORSE
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_MORSE_TITLE,  IDS_MORSE_TITLE, NULL, 0);
#endif
#if defined( FEATURE_JEWISH_CALENDAR)
            IMENUCTL_AddItem(pMenu, AEE_EXTRAMENU_RES_FILE, IDS_JEWISH_CALENDAR_TITLE, IDS_JEWISH_CALENDAR_TITLE, NULL, 0);
#endif
            return TRUE;

        case EVT_DIALOG_START:
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties( pMenu, OEMMP_USE_MENU_STYLE);
            IMENUCTL_SetBottomBarType( pMenu, BTBAR_SELECT_BACK);

            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_EXTRAMENU,
                                     EVT_USER_REDRAW,
                                     0,
                                     0);
            return TRUE;

        case EVT_USER_REDRAW:
            IMENUCTL_SetSel(pMenu, pMe->m_currDlgId);
            // ͳһ���½���
            IDISPLAY_UpdateEx(pMe->m_pDisplay, FALSE);
            (void)IMENUCTL_Redraw(pMenu);
            return TRUE;

        case EVT_DIALOG_END:
            if(pMe->m_bSuspending)
            {
                if(!isSelectByNumberKey)
                {
                    pMe->m_currDlgId = IMENUCTL_GetSel(pMenu);
                }
                else
                {
                    isSelectByNumberKey = FALSE;
                }
            }
            else
            {
                pMe->m_currDlgId = 0;
            }
            //end added
            return TRUE;

        case EVT_KEY:
            if(wParam == AVK_CLR)
               CLOSE_DIALOG(DLGRET_CANCELED)
            return TRUE;

        case EVT_COMMAND:
            //pMe->m_currDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_currDlgId = wParam;
            if(wParam != IMENUCTL_GetSel(pMenu))
            {
                isSelectByNumberKey = TRUE;
            }
            switch (wParam)
            {
#ifdef FEATURE_SUPPORT_BT_APP
                case IDS_BLUETOOTH_TITLE:
                    ISHELL_StartApplet(pMe->m_pShell, AEECLSID_BTUIAPP);
                    break;
#endif
                case IDS_ALARMCLOCK_TITLE:
                    ISHELL_StartApplet(pMe->m_pShell, AEECLSID_ALARMCLOCK);
                    break;

                case IDS_CALENDAR_TITLE:
                    ISHELL_StartApplet(pMe->m_pShell,AEECLSID_SCHEDULEAPP);
                    break;

                case IDS_CALCULATOR_TITLE:
                    ISHELL_StartApplet(pMe->m_pShell, AEECLSID_CALCAPP);
                    break;

                case IDS_STOPWATCH_TITLE:
                    ISHELL_StartApplet(pMe->m_pShell, AEECLSID_STOPWATCH);
                    break;

                case IDS_WORLDTIME_TITLE:
                    ISHELL_StartApplet(pMe->m_pShell, AEECLSID_WORLDTIME);
                    break;
                    
                case IDS_CONVERTER_TITLE:
                    ISHELL_StartApplet(pMe->m_pShell, AEECLSID_CONVERTER);
                    break;
                    
                case IDS_TIMEER_TITLE:
                    ISHELL_StartApplet(pMe->m_pShell, AEECLSID_APPTIMER);
                    break;
                    

#ifdef FEATURE_MORSE
                case IDS_MORSE_TITLE:
                    ISHELL_StartApplet(pMe->m_pShell, AEECLSID_MORSE);
                    break;
#endif

#if defined( FEATURE_JEWISH_CALENDAR)
                case IDS_JEWISH_CALENDAR_TITLE:
                    ISHELL_StartApplet( pMe->m_pShell, AEECLSID_JEWISH_CALENDAR);
                    break;
#endif

#if defined( FEATURE_RECORDER)
                case IDS_RECORDER_TITLE:
                    ISHELL_StartApplet( pMe->m_pShell, AEECLSID_RECORDER);
                    break;
#endif

                default:
                    break;
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
			{
				AEEDeviceInfo devinfo;
				int nBarH ;
				AEERect rc;
				int16 wXPos = (int16)AEE_GET_X(dwParam);
				int16 wYPos = (int16)AEE_GET_Y(dwParam);

				nBarH = GetBottomBarHeight(pMe->m_pDisplay);
        
				MEMSET(&devinfo, 0, sizeof(devinfo));
				ISHELL_GetDeviceInfo(pMe->m_pShell, &devinfo);
				SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);

				if(EXTRAMENU_PT_IN_RECT(wXPos,wYPos,rc))
				{
					if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
					{
						boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_EXTRAMENU,EVT_USER,AVK_SELECT,0);
						return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
					{
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_EXTRAMENU,EVT_USER,AVK_INFO,0);
						 return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
					{						
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_EXTRAMENU,EVT_USER,AVK_CLR,0);
						 return rt;
					}
				}
			}
			break;
#endif //FEATURE_LCD_TOUCH_ENABLE
        default:
            break;
    }
    return FALSE;
} // HandleMainDialogEvent

