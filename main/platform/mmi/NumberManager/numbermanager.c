
/*==============================================================================
// �ļ���
//        NumberManager.c
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
08-12-13                 ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#ifndef AEE_SIMULATOR
#include "customer.h"   //Handset Feature Definitions
#else
#include "BREWSimFeatures.h"   //Simulator Feature Definitions
#endif
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
#include "OEMFeatures.h"
#endif
#if !defined(BREW_STATIC_APP)
// Use the .bid file if compiling as dynamic applet
   #include "NumberManager.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_NUMBERMANAGER
      #error AEECLSID_NUMBERMANAGER must be defined
   #endif
#endif

#include "numbermanager.brh"
#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
#include "NumberManager.h"
#include "OEMFeatures.h"
#include "AppComFunc.h"
#include "Appscommon.h"
#include "OEMText.h"
#include "CallApp.h"
#include "Appscommon_color.brh"
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// �رնԻ����
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                                 }

// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// NumberManager_APPIsReadyTimer����
#define APPISREADY_TIMER       100
#define EVT_APPISREADY      (EVT_USER + 30)

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            NumberManagerState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

// ������ײ���ʾ���궨��
#define NUMBERMANAGER_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}

#define NUMBERMANAGER_CFG_VERSION 1
#define MAX_INPUT_NUM 32
#define MAX_INPUT_URL 128

#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch 
#define NUMBERMANAGER_PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#endif
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
   DLGRET_CANCELED,
   DLGRET_TEXT_INPUT
} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// NumberManager Applet ״̬��״̬��
typedef enum _NumberManagerState
{
   NUMBERMANAGERST_NONE,
   NUMBERMANAGERST_INIT,
   NUMBERMANAGERST_MAIN,
   NUMBERMANAGERST_DISPLAY_NUMBER,
   NUMBERMANAGERST_TEXT_INPUT,
   NUMBERMANAGERST_EXIT,       
} NumberManagerState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct NumberManagerMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} NumberManagerMod;

//������ѡ������ݽṹ
typedef struct _NumberManagerCfg
{
	AECHAR telexchange_num[MAX_INPUT_NUM];        //�ܻ�����
} NumberManagerCfg;

// NumberManager Applet����ṹ�壺
typedef struct _CNumberManager
{
    DECLARE_VTBL(INumberManager)
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
    NumberManagerState m_ePreState;
    // Applet ��ǰ״̬
    NumberManagerState m_eCurState;
    // Applet ����״̬
    NumberManagerState m_eMsgRetState;    
    // Applet�Ƿ��ڹ���״̬
    boolean m_bSuspending;
    // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean m_bAppIsReady;

    uint16 m_currDlgId;       //���浱ǰ��ѡ�˵�λ��
    uint16 m_MsgId;  
    uint16 m_nFldInputID;
    AECHAR m_number[MAX_INPUT_NUM];
    NumberManagerCfg m_NumberManagerCfg;
    char m_URL[MAX_INPUT_URL];    
} CNumberManager;


/*==============================================================================
                                 ��������
==============================================================================*/
static void NumberManager_APPIsReadyTimer(void *pme);

// �Ի��� IDD_NUMBERMANAGER_DIALOG �¼�������
static boolean  HandleMainDialogEvent(CNumberManager *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_DISPLAY_NUMBER_DIALOG �¼�������
static boolean  HandleDisplayNumberDialogEvent(CNumberManager *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
);

// �Ի��� IDD_TEXT_INPUT_DIALOG �¼�������
static boolean  HandleTextInputDialogEvent(CNumberManager *pMe,
   AEEEvent   eCode,
   uint16     wParam,
   uint32     dwParam
);

// ״̬ NUMBERMANAGERST_NONE ������
static NextFSMAction StateNoOperationHandler(CNumberManager *pMe);

// ״̬ NUMBERMANAGERST_INIT ������
static NextFSMAction StateInitHandler(CNumberManager *pMe);

// ״̬ NUMBERMANAGERST_MAIN ������
static NextFSMAction StateMainHandler(CNumberManager *pMe);

// ״̬ NUMBERMANAGERST_DISPLAY_NUMBER ������
static NextFSMAction StateDisplayNumberHandler(CNumberManager *pMe);

// ״̬ NUMBERMANAGERST_TEXT_INPUT ������
static NextFSMAction StateTextInputHandler(CNumberManager *pMe);

// ״̬ NUMBERMANAGERST_EXIT ������
static NextFSMAction StateExitHandler(CNumberManager *pMe);

//���Ŵ�����
static boolean Numbermanager_SendCall(CNumberManager *pMe);

/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
       NumberManager_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��NumberManager Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void NumberManager_ShowDialog(CNumberManager *pMe,uint16  dlgResId);

/*==============================================================================
����:
       NumberManager_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��NumberManager Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean NumberManager_RouteDialogEvent(CNumberManager *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       NumberManager_ProcessState
˵��:
       NumberManager Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��NumberManager Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction NumberManager_ProcessState(CNumberManager *pMe);

/*----------------------ģ����غ�������---------------------*/
#if defined(BREW_STATIC_APP)
PFNMODENTRY NumberManager_GetModInfo(IShell   *ps,
    AEECLSID  **ppClasses,
    AEEAppInfo  **pApps,
    uint16  *pnApps,
    uint16  *pwMinPriv
);
#endif

int  NumberManagerMod_Load(IShell   *pIShell,
    void     *ph,
    IModule **ppMod
);

int  NumberManagerMod_New(int16  nSize,
    IShell   *pIShell,
    void     *ph,
    IModule  **ppMod,
    PFNMODCREATEINST pfnMC,
    PFNFREEMODDATA pfnMF
);

static int  NumberManagerMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
);

static uint32 NumberManagerMod_AddRef(IModule *po);

static uint32 NumberManagerMod_Release(IModule *po);

static void NumberManagerMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
);

/*----------------------Applet��غ�������---------------------*/
static int NumberManager_New(IShell *ps,
    IModule *pIModule,
    INumberManager **ppObj
);

static uint32  NumberManager_AddRef(INumberManager *pi);

static uint32  NumberManager_Release (INumberManager *pi);

static boolean NumberManager_HandleEvent(INumberManager *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
);

static int NumberManager_InitAppData(CNumberManager *pMe);

static void NumberManager_FreeAppData(CNumberManager *pMe);

static void NumberManager_RunFSM(CNumberManager *pMe);

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
static NumberManagerMod gNumberManagerMod;

static const VTBL(IModule) gModFuncs =
{
    NumberManagerMod_AddRef,
    NumberManagerMod_Release,
    NumberManagerMod_CreateInstance,
    NumberManagerMod_FreeResources
};

// ֻ����һ��NumberManagerʵ����ÿ�δ���NumberManager Appletʱ��
// ����ͬһ�ṹָ���BREW�㡣
static CNumberManager gNumberManager;

static const VTBL(INumberManager) gNumberManagerMethods =
{
    NumberManager_AddRef,
    NumberManager_Release,
    NumberManager_HandleEvent
};

/*----------------------ģ����غ�������---------------------*/

/*==============================================================================
����:
       NumberManagerMod_Load

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
int NumberManagerMod_Load(IShell   *pIShell,
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

    return NumberManagerMod_New(sizeof(NumberManagerMod),
                    pIShell,
                    ph,
                    ppMod,
                    NULL,
                    NULL);
}

/*==============================================================================
����:
       NumberManagerMod_New

˵��:
       ������ʼ��NumberManagerģ�������Ҳ�������ü����������ֺ�����

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
int  NumberManagerMod_New(int16  nSize,
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

    MEMSET(&gNumberManagerMod,0,sizeof(NumberManagerMod));

    INIT_VTBL(&gNumberManagerMod,IModule,gModFuncs);
    gNumberManagerMod.m_nRefs = 1;
    *ppMod = (IModule *)&gNumberManagerMod;
    return AEE_SUCCESS;
}

/*==============================================================================
����:
       NumberManagerMod_AddRef

˵��:
       NumberManager ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��NumberManager ģ���������

��ע:

==============================================================================*/
static uint32 NumberManagerMod_AddRef(IModule *po)
{
    return(++((NumberManagerMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       NumberManagerMod_Release

˵��:
       NumberManager ģ�����ü�����1�������ص�ǰ����������

����:
       po [in]��IModule�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��NumberManager ģ���������

��ע:

==============================================================================*/
static uint32 NumberManagerMod_Release(IModule *po)
{
    if (((NumberManagerMod *) po)->m_nRefs == 0)
    {
        return 0;
    }

    return(--((NumberManagerMod *)po)->m_nRefs);
}

/*==============================================================================
����:
       NumberManagerMod_CreateInstance

˵��:
        ͨ������(��ʼ��)NumberManager����������ģ��ʵ����

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
static int  NumberManagerMod_CreateInstance(IModule *po,
    IShell *pIShell,
    AEECLSID ClsId,
    void **ppObj
)
{
    *ppObj = NULL;

    if (ClsId != AEECLSID_NUMBERMANAGER)
    {
        return EFAILED;
    }

    if (NumberManager_New(pIShell, po, (INumberManager**)ppObj) != SUCCESS)
    {
        return EFAILED;
    }
    return SUCCESS;
}

/*==============================================================================
����:
       NumberManagerMod_FreeResources

˵��:
        �ͷ�NumberManagerģ����Դ�ĺ�����

����:
       po ��IModule��
       ph ��IHeap�ӿ�ָ�롣
       pfm :IFileMgr�ӿ�ָ�롣

����ֵ:
       ��

��ע:
       �����û�ȥ���á�

==============================================================================*/
static void NumberManagerMod_FreeResources(IModule  *po,
    IHeap  *ph,
    IFileMgr *pfm
)
{

}

/*----------------------Applet��غ�������---------------------*/
/*==============================================================================
����:
       NumberManager_New

˵��:
        ��ʼ��NumberManager Applet�������ڲ����ݱ�����ͬʱ�����ü�����1��

����:
       ps [in]��IShell�ӿڶ���ָ�롣
       pIModule [in]��pIModule�ӿڶ���ָ�롣
       ppObj [out]:��ָ��INumberManager����ָ���ָ�롣

����ֵ:
       EFAILED��ʧ�ܡ�
       SUCCESS���ɹ���

��ע:
       �����û�ȥ���á�
       ����Applet�ṹ�����ʹ�ö�̬�����ڴ�ķ�ʽ�������������õ���ʹ�þ�̬
       �����ķ�ʽ������Խṹ�Ƚϴ��Appletʹ�þ�̬�����ķ�ʽ�����ڶ�̬����
       �ڴ�ķ�ʽ��Ҫע���������ռ���ͷš�

==============================================================================*/
static int NumberManager_New(IShell *ps,
    IModule *pIModule,
    INumberManager **ppObj
)
{
    int retVal = SUCCESS;

    if (0 == gNumberManager.m_nRefs)
    {
        // Must initialize the object
        MEMSET(&gNumberManager,  0, sizeof(CNumberManager));
        INIT_VTBL(&gNumberManager, INumberManager, gNumberManagerMethods);

        gNumberManager.m_nRefs     = 0;
        gNumberManager.m_pShell    = ps;
        gNumberManager.m_pModule   = pIModule;

        (void) ISHELL_AddRef(ps);
        (void) IMODULE_AddRef(pIModule);

        retVal = NumberManager_InitAppData(&gNumberManager);
    }

    ++gNumberManager.m_nRefs;
    *ppObj = (INumberManager*)&gNumberManager;
    return retVal;
}

/*==============================================================================
����:
       NumberManager_AddRef

˵��:
       NumberManager Applet ���ü�����1�������ص�ǰ����������

����:
       pi [in]��INumberManager�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��NumberManager Applet��������

��ע:

==============================================================================*/
static uint32  NumberManager_AddRef(INumberManager *pi)
{
    register CNumberManager *pMe = (CNumberManager*)pi;

    //ASSERT(pMe != NULL);
    //ASSERT(pMe->m_nRefs > 0);

    return (++pMe->m_nRefs);
}

/*==============================================================================
����:
       NumberManager_Release

˵��:
       �ͷŶ�NumberManager Applet�����ã����ü�����1�������ص�ǰ����������
        �����ǰ��������Ϊ�㣬�����ͷ�Applet�������Դ��

����:
       pi [in]��INumberManager�ӿڶ���ָ�롣

����ֵ:
       ��ǰ��NumberManager Applet��������

��ע:

==============================================================================*/
static uint32  NumberManager_Release (INumberManager *pi)
{
    register CNumberManager *pMe = (CNumberManager*)pi;

    //ASSERT(pMe != NULL);

    if (pMe->m_nRefs == 0)
    {
        return 0;
    }

    if (--pMe->m_nRefs)
    {
        return pMe->m_nRefs;
    }

    // �ͷ�Applet�����Դ
    NumberManager_FreeAppData(pMe);

    (void) ISHELL_Release(pMe->m_pShell);
    (void) IMODULE_Release(pMe->m_pModule);

    // ע�⣺pMe�Ǿ�̬����ռ䣬��˲���Ҫ�ͷš�FREE()
    return 0;
}

/*==============================================================================
����:
       NumberManager_InitAppData

˵��:
       ��ʼ��NumberManager Applet�ṹ����ȱʡֵ��

����:
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       SUCCESS����ʼ���ɹ���
       EFAILED����ʼ��ʧ�ܡ�

��ע:

==============================================================================*/
static int NumberManager_InitAppData(CNumberManager *pMe)
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
    pMe->m_ePreState = NUMBERMANAGERST_NONE;
    pMe->m_eCurState = NUMBERMANAGERST_INIT;
    pMe->m_pActiveDlg = NULL;
    pMe->m_eDlgRet = DLGRET_CREATE;
    pMe->m_bNotOverwriteDlgRet = FALSE;

    pMe->m_currDlgId = 0;
    
    pMe->m_MsgId = 0;
    pMe->m_nFldInputID = 0;  

    return SUCCESS;
}

/*==============================================================================
����:
       NumberManager_FreeAppData

˵��:
       �ͷ�NumberManager Appletʹ�õ������Դ��

����:
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void NumberManager_FreeAppData(CNumberManager *pMe)
{
    if (NULL == pMe)
    {
        return ;
    }

}

/*==============================================================================
����:
       NumberManager_RunFSM

˵��:
       NumberManager Applet����״̬�����档

����:
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void NumberManager_RunFSM(CNumberManager *pMe)
{
    NextFSMAction nextFSMAction = NFSMACTION_WAIT;

    for ( ; ; )
    {
        nextFSMAction = NumberManager_ProcessState(pMe);

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
       NumberManager_HandleEvent

˵��:
       NumberManager  Applet�¼��������������������ڱ�Applet���¼���
       �������ɸú�������

����:
       pi [in]��ָ��INumberManager�ӿ�t�����ָ�롣
       eCode��BREW�¼����롣
       wParam�����¼�����������ݡ�
       dwParam [in]�����¼�����������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
static boolean NumberManager_HandleEvent(INumberManager *pi,
    AEEEvent eCode,
    uint16  wParam,
    uint32 dwParam
)
{
    CNumberManager *pMe = (CNumberManager*)pi;
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
            {
                AEEDeviceInfo di; 
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dy = di.cyScreen;
            }

            pMe->m_bSuspending = FALSE;

            // ��ʼNumberManager״̬��
            NumberManager_RunFSM(pMe);
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
            {
                AEEDeviceInfo di; 
                ISHELL_GetDeviceInfo(pMe->m_pShell,&di);
                pMe->m_rc.dy = di.cyScreen;
            }

            NumberManager_RunFSM(pMe);
            return TRUE;

        case EVT_DIALOG_INIT:
            pMe->m_bAppIsReady = FALSE;
            pMe->m_pActiveDlg = (IDialog*)dwParam;
            pMe->m_pActiveDlgID = wParam;

            return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_START:
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            NumberManager_APPIsReadyTimer,
                            pMe);
            
            return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_USER_REDRAW:
            (void) NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            
            /*
            (void)ISHELL_SetTimer ( pMe->m_pShell,
                            APPISREADY_TIMER,
                            NumberManager_APPIsReadyTimer,
                            pMe);
            */
            return TRUE;

        case EVT_APPISREADY:
            (void) NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_bAppIsReady = TRUE;
            return TRUE;

        case EVT_KEY_PRESS:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY_RELEASE:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY_HELD:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_KEY:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_COMMAND:
			#if !defined(FEATURE_VERSION_K212) && !defined(FEATURE_VERSION_EC99)&&!defined(FEATURE_QVGA_INHERIT_K212)
            if (!pMe->m_bAppIsReady)
            {
                return TRUE;
            }
			#endif
            return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);

        case EVT_DIALOG_END:
            if (wParam == 0)
            {
                return TRUE;
            }
            pMe->m_bAppIsReady = FALSE;
            (void) NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);
            pMe->m_pActiveDlg = NULL;

            // Applet�����𣬲���״̬�����ȴ�Applet���ء���ע�⣺EVT_APP_SUSPEND
            // �¼���EVT_DIALOG_END�¼�ǰ������
            if (pMe->m_bSuspending == FALSE)
            {
                // ��ʼNumberManager״̬��
                NumberManager_RunFSM(pMe);
            }
            return TRUE;
#ifdef FEATURE_LCD_TOUCH_ENABLE//wlh add for LCD touch
		case EVT_PEN_UP:
		//case EVT_PEN_DOWN:
			{
				return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}

		case EVT_USER:
			{
				if((wParam == AVK_SELECT))
				{
					eCode = EVT_KEY;
					//wParam = dwParam;
					//dwParam = 0;
				}
				else if((wParam == AVK_CLR)  || (wParam == AVK_INFO))
				{
					eCode = EVT_KEY;
				}
				return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);
			}
#endif 
        default:
            // �����յ����¼�·������ǰ��ĶԻ����¼���������
            return NumberManager_RouteDialogEvent(pMe,eCode,wParam,dwParam);
    }
}

/*==============================================================================
����:
       NumberManager_APPIsReadyTimer

˵��:
       ��ʱ���ص���������Ҫ���ڿ��ƿ��ٰ�������Ĵ���

����:
       pme [in]��void *����ָ�룬��ָCNumberManager�ṹָ�롣

����ֵ:
       �ޡ�

��ע:

==============================================================================*/
static void NumberManager_APPIsReadyTimer(void *pme)
{
    CNumberManager *pMe = (CNumberManager *)pme;

    if (NULL == pMe)
    {
        return;
    }

    (void) ISHELL_PostEvent( pMe->m_pShell,
                            AEECLSID_NUMBERMANAGER,
                            EVT_APPISREADY,
                            0,
                            0);
}


/*==============================================================================
����:
       NumberManager_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��NumberManager Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void NumberManager_ShowDialog(CNumberManager *pMe,uint16  dlgResId)
{
    int nRet;

    // At most one dialog open at once
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        return;
    }

    nRet = ISHELL_CreateDialog(pMe->m_pShell,AEE_NUMBERMANAGER_RES_FILE,dlgResId,NULL);
}

/*==============================================================================
����:
       NumberManager_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��NumberManager Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean NumberManager_RouteDialogEvent(CNumberManager *pMe,
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
        case IDD_NUMBERMANAGER_DIALOG:
            return HandleMainDialogEvent(pMe,eCode,wParam,dwParam);

        case IDD_DISPLAY_NUMBER_DIALOG:
            return HandleDisplayNumberDialogEvent(pMe,eCode,wParam,dwParam);
            
        case IDD_TEXT_INPUT_DIALOG:
            return HandleTextInputDialogEvent(pMe,eCode,wParam,dwParam);
                        
        default:
            return FALSE;
    }
}




/*==============================================================================
����:
       NumberManager_ProcessState
˵��:
       NumberManager Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��NumberManager Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction NumberManager_ProcessState(CNumberManager *pMe)
{
    NextFSMAction retVal = NFSMACTION_WAIT;

    if (NULL == pMe)
    {
        return retVal;
    }

    // ���ݵ�ǰ״̬������Ӧ��״̬������
    switch (pMe->m_eCurState)
    {
        case NUMBERMANAGERST_NONE:
            retVal = StateNoOperationHandler(pMe);
            break;

        case NUMBERMANAGERST_INIT:
            retVal = StateInitHandler(pMe);
            break;

        case NUMBERMANAGERST_MAIN:
            retVal = StateMainHandler(pMe);
            break;
        
        case NUMBERMANAGERST_DISPLAY_NUMBER:
            retVal = StateDisplayNumberHandler(pMe);
            break;

        case NUMBERMANAGERST_TEXT_INPUT:
            retVal = StateTextInputHandler(pMe);
            break;
                                
        case NUMBERMANAGERST_EXIT:
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
       NUMBERMANAGERST_NONE ״̬������

������
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateNoOperationHandler(CNumberManager *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(NUMBERMANAGERST_EXIT)
    return NFSMACTION_CONTINUE;
} // StateNoOperationHandler


/*==============================================================================
������
       StateInitHandler
˵����
       NUMBERMANAGERST_INIT ״̬������

������
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateInitHandler(CNumberManager *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    MOVE_TO_STATE(NUMBERMANAGERST_MAIN)
    return NFSMACTION_CONTINUE;
} // StateInitHandler


/*==============================================================================
������
       StateMainHandler
˵����
       NUMBERMANAGERST_MAIN ״̬������

������
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateMainHandler(CNumberManager *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            NumberManager_ShowDialog(pMe, IDD_NUMBERMANAGER_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_CANCELED:
            MOVE_TO_STATE(NUMBERMANAGERST_EXIT)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_OK:
            MOVE_TO_STATE(NUMBERMANAGERST_DISPLAY_NUMBER)
            return NFSMACTION_CONTINUE;    
            
        case DLGRET_TEXT_INPUT:
            pMe->m_eMsgRetState = NUMBERMANAGERST_MAIN;
            MOVE_TO_STATE(NUMBERMANAGERST_TEXT_INPUT)
            return NFSMACTION_CONTINUE;                        

        default:
            //ASSERT_NOT_REACHABLE;
            break;
    }

    return NFSMACTION_WAIT;
} // StateMainHandler

/*==============================================================================
������
       StateDisplayNumberHandler
˵����
       NUMBERMANAGERST_DISPLAY_NUMBER ״̬������

������
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateDisplayNumberHandler(CNumberManager *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            NumberManager_ShowDialog(pMe, IDD_DISPLAY_NUMBER_DIALOG);
            return NFSMACTION_WAIT;
            
        case DLGRET_CANCELED:
            MOVE_TO_STATE(NUMBERMANAGERST_MAIN)
            return NFSMACTION_CONTINUE;
            
        case DLGRET_TEXT_INPUT:
            pMe->m_eMsgRetState = NUMBERMANAGERST_DISPLAY_NUMBER;
            MOVE_TO_STATE(NUMBERMANAGERST_TEXT_INPUT)
            return NFSMACTION_CONTINUE;   
                      
        default:
            //ASSERT_NOT_REACHABLE;
            break;
    }

    return NFSMACTION_WAIT;
} // StateMainHandler

/*==============================================================================
������
       StateTextInputHandler
˵����
       NUMBERMANAGERST_TEXT_INPUT ״̬������

������
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateTextInputHandler(CNumberManager *pMe)
{
    if (NULL == pMe)
    {
        return NFSMACTION_WAIT;
    }

    switch(pMe->m_eDlgRet)
    {
        case DLGRET_CREATE:
            pMe->m_bNotOverwriteDlgRet = FALSE;
            NumberManager_ShowDialog(pMe, IDD_TEXT_INPUT_DIALOG);
            return NFSMACTION_WAIT;

        case DLGRET_OK:
            if(0 != WSTRLEN(pMe->m_NumberManagerCfg.telexchange_num)) 
            {  
                MOVE_TO_STATE(NUMBERMANAGERST_DISPLAY_NUMBER)
            }
            else
            {
                MOVE_TO_STATE(NUMBERMANAGERST_MAIN)
            }
            return NFSMACTION_CONTINUE;

        case DLGRET_CANCELED:        	
            MOVE_TO_STATE(pMe->m_eMsgRetState)
            return NFSMACTION_CONTINUE;
            
        default:
            //ASSERT_NOT_REACHABLE;
            break;
    }

    return NFSMACTION_WAIT;
} // StateTextInputHandler

/*==============================================================================
������
       StateExitHandler
˵����
       NUMBERMANAGERST_EXIT ״̬������

������
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��

����ֵ��
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע��

==============================================================================*/
static NextFSMAction StateExitHandler(CNumberManager *pMe)
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
       IDD_NUMBERMANAGER_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  HandleMainDialogEvent(CNumberManager *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IMenuCtl *pMenu = (IMenuCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                    IDC_NUMBERMANAGER_DIALOG);
    if (pMenu == NULL)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
#ifdef FEATURE_SUPPORT_WAP_APP            
            IMENUCTL_AddItem(pMenu, AEE_NUMBERMANAGER_RES_FILE, IDS_LIFE_SEARCHING_TITLE,  IDS_LIFE_SEARCHING_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_NUMBERMANAGER_RES_FILE, IDS_MAP_SERVICE_TITLE,  IDS_MAP_SERVICE_TITLE, NULL, 0);            
#endif //FEATURE_SUPPORT_WAP_APP
            IMENUCTL_AddItem(pMenu, AEE_NUMBERMANAGER_RES_FILE, IDS_BOOK_TICKET_TITLE,  IDS_BOOK_TICKET_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_NUMBERMANAGER_RES_FILE, IDS_BOOK_HOTEL_TITLE, IDS_BOOK_HOTEL_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_NUMBERMANAGER_RES_FILE, IDS_BOOK_RESTAURANT_TITLE, IDS_BOOK_RESTAURANT_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_NUMBERMANAGER_RES_FILE, IDS_COMMUNICATE_ASSISTANT_TITLE,  IDS_COMMUNICATE_ASSISTANT_TITLE, NULL, 0);
            IMENUCTL_AddItem(pMenu, AEE_NUMBERMANAGER_RES_FILE, IDS_TELEPHONE_EXCHANGE_TITLE, IDS_TELEPHONE_EXCHANGE_TITLE, NULL, 0);         
#ifdef FEATURE_SUPPORT_WAP_APP
            IMENUCTL_AddItem(pMenu, AEE_NUMBERMANAGER_RES_FILE, IDS_MEMBER_SPACE_TITLE,  IDS_MEMBER_SPACE_TITLE, NULL, 0);
#endif //FEATURE_SUPPORT_WAP_APP
            IMENUCTL_AddItem(pMenu, AEE_NUMBERMANAGER_RES_FILE, IDS_CALL_NUMBER_MANAGER_TITLE, IDS_CALL_NUMBER_MANAGER_TITLE, NULL, 0);
            return TRUE;

        case EVT_DIALOG_START:
            IMENUCTL_SetProperties(pMenu, MP_UNDERLINE_TITLE|MP_WRAPSCROLL|MP_BIND_ITEM_TO_NUMBER_KEY);
            IMENUCTL_SetOemProperties( pMenu , OEMMP_USE_MENU_STYLE);
            IMENUCTL_SetBottomBarType( pMenu, BTBAR_SELECT_BACK);
            (void) ISHELL_PostEvent( pMe->m_pShell,
                                     AEECLSID_NUMBERMANAGER,
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
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;                                                                           
                                        
                default:
                    break;
            }    
            return TRUE;

        case EVT_COMMAND:
            //pMe->m_currDlgId = IMENUCTL_GetSel(pMenu);
            pMe->m_currDlgId = wParam;
            switch (wParam)
            {
                case IDS_LIFE_SEARCHING_TITLE:                    
                    STRCPY( pMe->m_URL, "http://wap.118114.cn/life");
                    pMe->m_MsgId = IDS_LIFE_SEARCHING_TITLE;
                    CLOSE_DIALOG(DLGRET_OK);
                    return TRUE;  

                case IDS_MAP_SERVICE_TITLE:
                    STRCPY( pMe->m_URL, "http://wap.118114.cn/around");                   
                    pMe->m_MsgId = IDS_MAP_SERVICE_TITLE;
                    CLOSE_DIALOG(DLGRET_OK);
                    return TRUE;  
                    
                case IDS_BOOK_TICKET_TITLE:
                    (void) STRTOWSTR("11843490001",pMe->m_number,sizeof(pMe->m_number));
                    pMe->m_MsgId = IDS_BOOK_TICKET_TITLE;
                    CLOSE_DIALOG(DLGRET_OK);
                    return TRUE;  

                case IDS_BOOK_HOTEL_TITLE:
                    (void) STRTOWSTR("11843490002",pMe->m_number,sizeof(pMe->m_number));
                    pMe->m_MsgId = IDS_BOOK_HOTEL_TITLE;                
                    CLOSE_DIALOG(DLGRET_OK);
                    return TRUE;  

                case IDS_BOOK_RESTAURANT_TITLE:
                    (void) STRTOWSTR("11843490003",pMe->m_number,sizeof(pMe->m_number));
                    pMe->m_MsgId = IDS_BOOK_RESTAURANT_TITLE;                
                    CLOSE_DIALOG(DLGRET_OK);
                    return TRUE;  

                case IDS_COMMUNICATE_ASSISTANT_TITLE:
                    (void) STRTOWSTR("11843490004",pMe->m_number,sizeof(pMe->m_number));
                    pMe->m_MsgId = IDS_COMMUNICATE_ASSISTANT_TITLE;                
                    CLOSE_DIALOG(DLGRET_OK);
                    return TRUE;  

                case IDS_TELEPHONE_EXCHANGE_TITLE:
                    pMe->m_MsgId = IDS_TELEPHONE_EXCHANGE_TITLE;                  
                    (void)ISHELL_GetPrefs(pMe->m_pShell, 
                                AEECLSID_NUMBERMANAGER, 
                                NUMBERMANAGER_CFG_VERSION,
                                &pMe->m_NumberManagerCfg, 
                                sizeof(NumberManagerCfg)); 
                    if(0 == WSTRLEN(pMe->m_NumberManagerCfg.telexchange_num))
                    {        
                        pMe->m_nFldInputID = IDS_TELEPHONE_EXCHANGE_INPUT;
                        CLOSE_DIALOG(DLGRET_TEXT_INPUT);
                    } 
                    else
                    {   
                        WSTRCPY(pMe->m_number, pMe->m_NumberManagerCfg.telexchange_num);  
                        CLOSE_DIALOG(DLGRET_OK);
                    }
                    return TRUE;  

                 case IDS_MEMBER_SPACE_TITLE:
                    STRCPY( pMe->m_URL, "http://wap.118114.cn/member");    
                    pMe->m_MsgId = IDS_MEMBER_SPACE_TITLE;
                    CLOSE_DIALOG(DLGRET_OK);
                    return TRUE;  
                    
                case IDS_CALL_NUMBER_MANAGER_TITLE:
                    (void) STRTOWSTR("118114",pMe->m_number,sizeof(pMe->m_number));
                    pMe->m_MsgId = IDS_CALL_NUMBER_MANAGER_TITLE;                
                    CLOSE_DIALOG(DLGRET_OK);
                    return TRUE;                     

                default:
                    //ASSERT_NOT_REACHABLE;
                    break;
            }
            return TRUE;

        default:
            break;
    }
    return FALSE;
} // HandleMainDialogEvent

/*==============================================================================
������
       HandleDisplayNumberDialogEvent
˵����
       IDD_DISPLAY_NUMBER_DIALOG�Ի����¼�������

������
       pMe [in]��ָ��NumberManager Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static boolean  HandleDisplayNumberDialogEvent(CNumberManager *pMe,
    AEEEvent eCode,
    uint16 wParam,
    uint32 dwParam
)
{
    PARAM_NOT_REF(dwParam)

    IStatic * pStatic = (IStatic*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                     IDC_DISPLAY_NUMBER_DIALOG);

    if (NULL == pStatic)
    {
        return FALSE;
    }

    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;

        case EVT_DIALOG_START:
           {
                AEERect   rc; 
                AECHAR    szBuf[256];  //��̬�ı���ʾ������
                int n = 0;           

                //Draw static                           
                rc = pMe->m_rc;
                rc.y = GetTitleBarHeight(pMe->m_pDisplay);
                rc.dy = rc.dy - GetBottomBarHeight(pMe->m_pDisplay)-GetTitleBarHeight(pMe->m_pDisplay);
                ISTATIC_SetRect(pStatic, &rc);                        
                // ���þ�̬�ı��ؼ�����
                ISTATIC_SetProperties(pStatic, ST_CENTERTEXT|ST_MIDDLETEXT|ST_GRAPHIC_BG);
                ISTATIC_SetBackGround(pStatic, AEE_APPSCOMMONRES_IMAGESFILE, IDI_MENU_BACKGROUND); //added by chengxiao 2009.04.08

                //Draw text                              
                (void)MEMSET( szBuf,(AECHAR) 0, sizeof(szBuf));  

                switch (pMe->m_MsgId)
                {
                    case IDS_TELEPHONE_EXCHANGE_TITLE:
                        (void) ISHELL_LoadResString(pMe->m_pShell,
                                     AEE_NUMBERMANAGER_RES_FILE,
                                     IDS_TELEPHONE_EXCHANGE_NUMBER,
                                     szBuf,
                                     sizeof(szBuf)); 
                        (void)ISHELL_GetPrefs(pMe->m_pShell, 
                                    AEECLSID_NUMBERMANAGER, 
                                    NUMBERMANAGER_CFG_VERSION,
                                    &pMe->m_NumberManagerCfg, 
                                    sizeof(NumberManagerCfg)); 
                        
                        //Draw text                             
                        n = WSTRLEN(szBuf);
                        szBuf[n++] = (AECHAR) ':';             
                        if(0 != WSTRLEN(pMe->m_NumberManagerCfg.telexchange_num))
                        {
                            WSTRCPY(pMe->m_number, pMe->m_NumberManagerCfg.telexchange_num); 
                            WSTRCPY(szBuf + n, pMe->m_number);          
                        } 
                        break;

                    case IDS_LIFE_SEARCHING_TITLE:
                    case IDS_MAP_SERVICE_TITLE:
                    case IDS_MEMBER_SPACE_TITLE:
                        (void) ISHELL_LoadResString(pMe->m_pShell,
                                     AEE_NUMBERMANAGER_RES_FILE,
                                     IDS_CONNECT_TO,
                                     szBuf,
                                     sizeof(szBuf));                         
                        //Draw text                             
                        n = WSTRLEN(szBuf);
                        szBuf[n++] = (AECHAR) ' ';                          
                        if(0 != STRLEN(pMe->m_URL))
                        {
                            STRTOWSTR(pMe->m_URL, szBuf + n, sizeof(szBuf));      
                        }                         
                        break;

                    default:
                        WSTRCPY(szBuf, pMe->m_number);                         
                        break;                    
                }
               
                // ���þ�̬�ı��ؼ��ı�
                (void)ISTATIC_SetText(pStatic, 
                            NULL, 
                            szBuf, 
                            AEE_FONT_NORMAL, 
                            AEE_FONT_NORMAL);          
          
                (void) ISHELL_PostEvent( pMe->m_pShell,
                                         AEECLSID_NUMBERMANAGER,
                                         EVT_USER_REDRAW,
                                         0,
                                         0);
           }
            return TRUE;

        case EVT_USER_REDRAW:
            {
                TitleBar_Param_type  titleParam; 
                AEERect   titlerect; 
                AECHAR    titleBuf[128];  

                //Draw title           
                (void) ISHELL_LoadResString(pMe->m_pShell,
                                AEE_NUMBERMANAGER_RES_FILE,
                                pMe->m_MsgId,
                                titleBuf,
                                sizeof(titleBuf));                                                    
                SETAEERECT(&titlerect, 0, 0, pMe->m_rc.dx, GetTitleBarHeight(pMe->m_pDisplay));
                MEMSET(&titleParam,0,sizeof(TitleBar_Param_type));
                titleParam.prc = &titlerect;
                titleParam.pwszTitle = titleBuf;
                titleParam.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER;        
                DrawTitleBar(pMe->m_pDisplay, &titleParam);
                
                // Draw prompt bar here                
                switch (pMe->m_MsgId)
                {
                    case IDS_TELEPHONE_EXCHANGE_TITLE:
                        NUMBERMANAGER_DRAW_BOTTOMBAR(BTBAR_CALL_MODIFY_BACK);
                        break;

                    case IDS_LIFE_SEARCHING_TITLE:
                    case IDS_MAP_SERVICE_TITLE:
                    case IDS_MEMBER_SPACE_TITLE:
                        NUMBERMANAGER_DRAW_BOTTOMBAR(BTBAR_OK_BACK);                 
                        break;

                    default:
                        NUMBERMANAGER_DRAW_BOTTOMBAR(BTBAR_CALL_BACK);                                   
                        break;                    
                }

                IDISPLAY_Update(pMe->m_pDisplay);   
            } 
            return TRUE;

        case EVT_DIALOG_END:
            return TRUE;

        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;  
                                    
                case AVK_SELECT:
                    switch (pMe->m_MsgId)
                    {                       
                        case IDS_LIFE_SEARCHING_TITLE:
                        case IDS_MAP_SERVICE_TITLE:
                        case IDS_MEMBER_SPACE_TITLE:
                            ISHELL_PostURL(pMe->m_pShell,pMe->m_URL);                 
                            return TRUE; 

                        default:
                            return Numbermanager_SendCall(pMe);         
                    }
                    return TRUE; 
                                                                                            
                case AVK_INFO:
                    if(IDS_TELEPHONE_EXCHANGE_TITLE == pMe->m_MsgId)
                    {
                        pMe->m_nFldInputID = IDS_TELEPHONE_EXCHANGE_MODIFY;
                        CLOSE_DIALOG(DLGRET_TEXT_INPUT);
                        return TRUE; 
                    }
                    break;  
                                                         
                default:
                    break;
            }    
            return TRUE;

        case EVT_COMMAND:
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

				if(NUMBERMANAGER_PT_IN_RECT(wXPos,wYPos,rc))
				{
					if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
					{
						boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_NUMBERMANAGER,EVT_USER,AVK_SELECT,0);
						return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
					{
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_NUMBERMANAGER,EVT_USER,AVK_INFO,0);
						 return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
					{						
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_NUMBERMANAGER,EVT_USER,AVK_CLR,0);
						 return rt;
					}
				}

			}
			break;
#endif
        default:
            break;
    }
    return FALSE;
} // HandleDisplayNumberDialogEvent

/*==============================================================================
������
       IDD_TEXT_INPUT_Handler
˵����
       IDD_TEXT_INPUT_DIALOG�Ի����¼�������
       
������
       pMe [in]��ָ��MainMenu Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�
       
����ֵ��
       TRUE�������¼�������
       FALSE�������¼������ԡ�
       
��ע��
       
==============================================================================*/
static boolean  HandleTextInputDialogEvent(CNumberManager *pMe,
                                   AEEEvent   eCode,
                                   uint16     wParam,
                                   uint32     dwParam)
{
    PARAM_NOT_REF(dwParam)	
    ITextCtl *pTextCtl = (ITextCtl*)IDIALOG_GetControl(pMe->m_pActiveDlg,
                                                      IDC_TEXT_INPUT_DIALOG);
    if (NULL == pTextCtl)
    {
        return FALSE;
    }
    
    switch (eCode)
    {
        case EVT_DIALOG_INIT:
            return TRUE;
            
        case EVT_DIALOG_START: 
           {     
            AEERect rc;
            
            rc = pMe->m_rc;
            rc.dy -= GetBottomBarHeight(pMe->m_pDisplay);
            ITEXTCTL_SetRect(pTextCtl, &rc);            
            ITEXTCTL_SetProperties( pTextCtl,TP_FRAME | TP_FIXSETRECT |TP_EDITNUMBER_PTSTRING|TP_FOCUS_NOSEL|TP_GRAPHIC_BG);
            (void)ITEXTCTL_SetInputMode(pTextCtl,AEE_TM_NUMBERS);
            (void)ITEXTCTL_SetTitle( pTextCtl,
                                    AEE_NUMBERMANAGER_RES_FILE,
                                    pMe->m_nFldInputID,
                                     NULL);
            ITEXTCTL_SetMaxSize( pTextCtl,MAX_INPUT_NUM);
            
            (void)ISHELL_GetPrefs(pMe->m_pShell, 
                        AEECLSID_NUMBERMANAGER, 
                        NUMBERMANAGER_CFG_VERSION,
                        &pMe->m_NumberManagerCfg, 
                        sizeof(NumberManagerCfg));                                      
            if(0 != WSTRLEN(pMe->m_NumberManagerCfg.telexchange_num))
            {
                (void)ITEXTCTL_SetText(pTextCtl, pMe->m_NumberManagerCfg.telexchange_num, -1);
            }

            // For redraw the dialog
            (void)ISHELL_PostEvent( pMe->m_pShell,
                                    AEECLSID_NUMBERMANAGER,
                                    EVT_USER_REDRAW,
                                    0,
                                    0);
           }            
            return TRUE;
            
        case EVT_KEY_RELEASE:
        case EVT_USER_REDRAW:
            {
                AECHAR *pwstrText = ITEXTCTL_GetTextPtr(pTextCtl);
                int nLen=0;
                
                if (NULL != pwstrText)
                {
                    nLen = WSTRLEN(pwstrText);
                }
                
                if (nLen>0)
                {
                    // Option     Delete
                    NUMBERMANAGER_DRAW_BOTTOMBAR(BTBAR_OK_DELETE)
                }
                else
                {
                    // Option     Back
                    NUMBERMANAGER_DRAW_BOTTOMBAR(BTBAR_OK_BACK)
                }
            }        
            IDISPLAY_Update(pMe->m_pDisplay);             
            return TRUE;
                        
        case EVT_DIALOG_END:
            return TRUE;
            
        case EVT_DISPLAYDIALOGTIMEOUT:
            return TRUE;
            
        case EVT_KEY:
            switch(wParam)
            {
                case AVK_CLR:
                    CLOSE_DIALOG(DLGRET_CANCELED);
                    return TRUE;  
                    
                case AVK_SELECT:   
                    if(IDS_TELEPHONE_EXCHANGE_TITLE == pMe->m_MsgId)
                    {                        
                        (void) ITEXTCTL_GetText(pTextCtl,
                                        pMe->m_NumberManagerCfg.telexchange_num,
                                        MAX_INPUT_NUM);
                        (void)ISHELL_SetPrefs(pMe->m_pShell, 
                                    AEECLSID_NUMBERMANAGER, 
                                    NUMBERMANAGER_CFG_VERSION,
                                    &pMe->m_NumberManagerCfg, 
                                    sizeof(NumberManagerCfg));                                      
                         CLOSE_DIALOG(DLGRET_OK)                                  
                    }                                                                        
                    return TRUE;  
                                                            
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

				if(NUMBERMANAGER_PT_IN_RECT(wXPos,wYPos,rc))
				{
					if(wXPos >= rc.x && wXPos < rc.x + (rc.dx/3) )//��
					{
						boolean rt =  ISHELL_PostEvent(pMe->m_pShell,AEECLSID_NUMBERMANAGER,EVT_USER,AVK_SELECT,0);
						return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)   && wXPos < rc.x + (rc.dx/3)*2 )//��
					{
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_NUMBERMANAGER,EVT_USER,AVK_INFO,0);
						 return rt;
					}
					else if(wXPos >= rc.x + (rc.dx/3)*2 && wXPos < rc.x + (rc.dx/3)*3 )//��
					{						
						 boolean rt = ISHELL_PostEvent(pMe->m_pShell,AEECLSID_NUMBERMANAGER,EVT_USER,AVK_CLR,0);
						 return rt;
					}
				}

			}
			break;
#endif        
        default:
            break;
    }
    
    return FALSE;
} // HandleTextInputDialogEvent


/*==================================================================================
FUNCTION Numbermanager_SendCall
Description ����һ������
PARAMETERS:
          *pMe: CNumberManager object pointer
RETURN VALUE:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:
===================================================================================*/
static boolean Numbermanager_SendCall(CNumberManager *pMe)
{
    PARAM_NOT_REF(pMe)
    boolean  bRet = FALSE;        
    ICallApp *pCallApp = NULL;
    
    if(SUCCESS == ISHELL_CreateInstance(pMe->m_pShell, 
                                          AEECLSID_DIALER,
                                           (void**)&pCallApp))
    { 
        // ���ô��������� CallApp Applet
        if (SUCCESS == ICallApp_CallNumber(pCallApp,pMe->m_number))
        {
            bRet = TRUE;         
        }
    }
    if(pCallApp)
    {
        (void)ICallApp_Release(pCallApp);
        pCallApp = NULL;
    }
    return bRet;        
} //Numbermanager_SendCall