#ifndef CONVERTERPRIV_H
#define CONVERTERPRIV_H

/*==============================================================================
// �ļ���
//        Converter_priv.h
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
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
08-10-28                   Initial version
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
   #include "converter.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_CONVERTER
      #error AEECLSID_CONVERTER must be defined
   #endif
#endif

#ifndef WIN32
#ifdef FEATURE_COLOR_DISPLAY
#include "appscommon_color.brh"
#else
#include "appscommon_momo.brh"
#endif
#else
#include "appscommon_color.brh"
#endif//WIN32
#include "Converter.brh"
#include "AEEShell.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "Converter.h"
#include "OEMCFGI.h"
#include "Converter.h"
#include "AEEMenu.h"
#include "AEEConfig.h"
#include "AppComFunc.h"
#include "OEMFeatures.h"
#include "Appscommon.h"
#include "AEE.h"
#ifndef WIN32
#include "ERR.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32
#include "AEE_OEM.h"
#include "AEEText.h"

#define CONVERTER_CURRENCY_NUMBER (15) //�����������
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
   // ��ʼֵ��������Ҫ�����Ի���
   DLGRET_CREATE,

   // �Ի���IDD_CONVERTERMAIN�����ض�ֵ
   DLGRET_CURRENCY,
   DLGRET_LENGTH,
   DLGRET_WEIGHT,
   DLGRET_VOLUME,
   DLGRET_AREA,
   DLGRET_TEMPERATURE,
   //�Ի���IDD_CHANGECURRENCY�����ض�ֵ
   DLGRET_CHANGECURRENCY,
   DLGRET_ERR,
   DLGRET_MSGBOX_OK,
   DLGRET_MSGBOX_CANCEL,
   // �Ի���ر�ʱ�ɷ��ص�ͨ��ֵ
   DLGRET_OK,
   DLGRET_CANCELED,
   
} DLGRetValue;

/*----------------------״̬�����������������---------------------*/
// Cnverter Applet ״̬��״̬��
typedef enum _ConverterState
{
    CONVERTER_NONE,
    CONVERTER_INIT,
    CONVERTER_MAIN,
    CONVERTER_CONVERT,
    CONVERTER_CHANGECURRENCY,
    CONVERTER_EXIT
} ConverterState;

// ״̬���������ظ�״̬������������ֵ����
typedef enum NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct ConverterMod
{
    DECLARE_VTBL(IModule)
    uint32   m_nRefs;
} ConverterMod;

typedef struct _ConverterCurrencyCfg
{
    double customCurrency[CONVERTER_CURRENCY_NUMBER];
} ConverterCurrencyCfg;

typedef enum
{
    CONVERTER_MODE_NONE,
    CONVERTER_MODE_CURRENCY,
    CONVERTER_MODE_LENGTH,
    CONVERTER_MODE_WEIGHT,
    CONVERTER_MODE_VOLUME,
    CONVERTER_MODE_AREA,
    CONVERTER_MODE_TEMPERATURE
} ConverterMode;

// Converter Applet����ṹ�壺
typedef struct _CConverter
{
    DECLARE_VTBL(IConverter)
    AEECLSID         clsID;
    uint32              m_nRefs; // �������ü�����
    IModule            *m_pModule;
    IDisplay            *m_pDisplay;
    IShell               *m_pShell;
    IStatic             *m_pStatic;
    IConfig             *m_pConfig;
    AEERect           m_rc;
    // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog             *m_pDialog;
    // ��ǰ��Ի���ID
    uint16              m_pActiveDlgID;
    // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRetValue      m_eDlgRet;
    // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean            m_bNotOverwriteDlgRet;
    // Applet ǰһ״̬
    ConverterState  m_ePreState;
    // Applet ��ǰ״̬
    ConverterState  m_eCurState;
    // Applet�Ƿ��ڹ���״̬
    boolean            m_bSuspending;
    // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean            m_bAppIsReady;
    
    IMenuCtl           *pUnitMenu1; //Դ��λָ��
    IMenuCtl           *pUnitMenu2; //Ŀ�굥λָ��
    ITextCtl            *pNumber1;   //Դ��ֵ��ʾָ��
    ITextCtl            *pNumber2;   //Ŀ����ֵ��ʾָ��
    IMenuCtl           *title;
    IMenuCtl           *currency;
    ITextCtl            *coeff1;
    ITextCtl            *coeff2;
    ITextCtl            *coeff3;
    uint16               basiccoefficient;
    uint16               m_wMsgResID;
    ConverterCurrencyCfg m_CurrencyCFG;
    int                    dyMenu;        //Menu�߶�
    uint16               m_nCtlID;      //�ؼ�ID
    uint16               m_selectedItem; //�˵���¼��ǰѡ�����
    ConverterMode   m_converterMode; //�л���λ����ģʽ
    boolean             b_overflow; //������
} CConverter;

/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// ���� BREW 3 ����Ҫ���ض�����Դ�ļ���
#if defined( AEE_SIMULATOR)
    #define  AEE_CONVERTER_LANGFILE "fs:/mod/converter/en/converter.bar"
#else
    #define  AEE_CONVERTER_LANGFILE (AEE_RES_LANGDIR CONVERTER_RES_FILE)
#endif

// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// Converter_APPIsReadyTimer����

#define APPISREADY_TIMER       200            
#define EVT_APPISREADY   (EVT_USER+1)         
#define EVT_USER_CLOSEAPP (EVT_USER+13)

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            ConverterState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

// �رնԻ����
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                                 }

#define CONVERTER_CFG_CURRENCY   0x1000

#define FRAME_SIZE                 (1)
#define IDC_UNIT_MENU1          (2)
#define IDC_NUMBER1               (3)
#define IDC_UNIT_MENU2          (4)
#define IDC_NUMBER2               (5)

#define MAX_INPUT_NUMBER     (15)
#define MAX_DECIMAL_NUMBER (6)
#define MAX_INT_NUMBER         (9)
#define CONVERTER_PROMPTMSG_TIMER (3000)

#define ARROW_WIDTH             (9)
#define ARROW_HEIGHT            (13)
#define CHARACTOR_PIC_WIDTH (32)
#define RGB_ORANGE                (MAKE_RGB(0xFF,0x90,0x30))


/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/
    //��λ����ϵ��
    static double coefficient[] = {
    //CURRENCY����ŷԪΪ�������
    1.0,             // ŷԪ
    116.07,        // ��Ԫ
    8.8872,        // �����
    1.3000,        // ��Ԫ
    43.425,        // ̨��Ԫ
    0.9396,        // Ӣ��
    1.4776,        // ��ʿ����
    10.818,        // ������
    7.4452,        // �������
    9.1598,        // Ų������
    1.9421,        // �¼���Ԫ
    63.661,        // ӡ��¬��
    4.7526,        // ����������ķ
    1748.8,        // ��Ԫ
    5.0279,        // ��ɫ����л�˶�
    //LENGTH������Ϊ�������
    0.001,
    39.37,
    3.2808,
    1.0936,
    0.0006214,
    1000.0,
    100.0,
    1.0,
    //WEIGHT����ǧ��Ϊ�������
    35.273,
    1000.0,
    2.20459,
    1.0,
    0.001,
    15432.0,
    //VOLUME������Ϊ�������
    1000.0,
    0.001,
    1.0,
    61.027,
    0.0353165,
    0.0013082,
    0.264186,
    //AREA����ƽ����Ϊ�������
    10000.0,
    1.0,
    0.01,
    1550,
    10.764,
    0.0002471,
    //TEMPERATURE�������϶�Ϊ�����ĸ
    1.0,
    1.4
    };

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
       Converter_ShowMsgBox
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��Converter Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       wTextResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:
       

==============================================================================*/

void Converter_ShowMsgBox(CConverter *pMe, uint16 wTextResId);

/*==============================================================================
����:
       Converter_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��Converter Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:
       

==============================================================================*/
void Converter_ShowDialog(CConverter *pMe,uint16  dlgResId);

/*==============================================================================
����:
       Converter_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��Converter Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean Converter_RouteDialogEvent(CConverter *pMe,
    AEEEvent    eCode,
    uint16  wParam,
    uint32 dwParam
);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       Converter_ProcessState
˵��:
       Converter Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��Converter Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction Converter_ProcessState(CConverter *pMe);

#endif
//CONVERTERPRIV_H

