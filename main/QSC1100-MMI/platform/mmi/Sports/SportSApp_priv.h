#ifndef _SPORTSAPPPRIV_H_
#define _SPORTSAPPPRIV_H_

/*==============================================================================
// �ļ���
//        SportSApp_priv.h
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//
// �ļ�˵����
//
// ���ߣ�Gemsea
// �������ڣ�2008-11-04
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��    ���ⵥ��  �޸����ݡ�λ�ü�ԭ��
-----------      ----------  --------  -----------------------------------------
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
   #include "SportsMenu.bid"
#else
   #include "OEMClassIds.h"  // Applet Ids
   #ifndef AEECLSID_SPORTSAPP
      #error AEECLSID_SPORTSAPP must be defined
   #endif
#endif

#include "pedometer.brh"
#include "sportsmenu.brh"
#include "walk.brh"
#include "run.brh"

#include "AEEShell.h"
#include "AEEModTable.h"
#include "AEEStdLib.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "OEMClassIDs.h"
//#include "OEMMacros.h"
#include "SportSApp.h"

#include "AEEMenu.h"
#include "AEEConfig.h"
#include "AppComFunc.h"
#include "AEEText.h"
#ifndef WIN32
#include "Oemui.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32
#include "AEEAnnunciator.h"
//#include "AEEPhone.h"//add by lzhl20040823
#include "SoundMenu.h"
#include "OEMText.h"
#include "OEMFeatures.h"
#include "OEMCFGI.h"
#include "AppComFunc.h"
#include "Appscommon.h"
#include "AEERUIM.h"
#include "OEMText.h"
#ifdef FEATRUE_SUPPORT_G_SENSOR
#include "G_Sensor.h"
#include "Oemnvint.h"
#include "OEMCFGI.h"
#endif

#include "fmRadio.h"

/*==============================================================================
                                 ���Ͷ���
==============================================================================*/
#define SPORT_EFS_FILE  "fs:/sys/sports.dat" 
#define  AEE_APPSSPORTSMENU_RES_FILE AEE_RES_LANGDIR SPORTSMENU_RES_FILE
/*define  Walk Animation Resource*/
#define  AEE_APPSSPORT_WALK_RES_FILE AEE_RES_LANGDIR WALK_RES_FILE
/*define  Run Animation Resource*/
#define  AEE_APPSSPORT_RUN_RES_FILE AEE_RES_LANGDIR RUN_RES_FILE
/*define Pedometer  Resource*/
//#define  AEE_APPSSPORT_PEDOMTER_RES_FILE AEE_RES_LANGDIR PEDOMETER_RES_FILE
#define SPORTS_ERR(format, code1, code2, code3) 
#define IDC_PEDOMETER_STATE 1

#define CONTROL_RECT_START_X    (40) 

/*ARABIC*/
#define ARABIC_CONTROL_RECT_START_X   (2)
#define CONTROL_RECT_START_DX        (pMe->m_rc.dx - CONTROL_RECT_START_X - ARABIC_CONTROL_RECT_START_X) //86
#define AUTO_POWER_IMG_DX            (8)

/*DEFINE AVAIBLE*/
#define RUNNING_UPDATE_MSECS  (25)
#define MSECS_PER_DAY         (3600 * 24 * 1000)

// ������ײ���ʾ���궨��
#define SPORTS_MENU_DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}
/*Macro Calorie */
#define SPORT_WALK_CALORIE (160)
#define SPORT_LOW_STEP_CALORIE (200)
#define SPORT_MID_STEP_CALORIE (320)
#define SPORT_QUICK_STEP_CALOIRE (420)
#define SPORT_LOW_RUN_CALORIE (750)
#define SPORT_QUICK_RUN_CALORIE (1000)
/*Macro End Calorie*/
/*Gender Modulus*/
#define SPORT_GENDER_MODULUS_MALE (1)
#define SPORT_GENDER_MODULUS_FEMALE (0.85)
/*Gender end Modulus*/
//add by ydc
#define NEVT_KEY_PRESSS 10000


/*Macro Type Var*/
#define SPORT_EDIT_MAX_LEN (20)
#define SPORT_PED_MAX_TIME_LEN (30)
#define SPORT_USER_NUM (3)
#define SPORT_DISPLAY_ANIMATION_NUM (6)
/*End Type Var*/
/*----------------------�Ի������������������---------------------*/
// �Ի���ر�ʱ����ֵ�б�
typedef enum DLGRetValue
{
    DLGRET_CREATE,
    DLGRET_OK,
    DLGRET_SPORTS,				//IDD_DLG_SPORT
    DLGRET_PEDOMETER,
    DLGRET_PEDOMETER_OPTION,
    DLGRET_PEDOMETER_RECORD,
    DLGRET_PEDOMETER_RECORD_VIEW,
    DLGRET_PERSONAL,
    DLGRET_PERSONAL_OPTION,
    DLGRET_PERSONAL_EDIT,
    DLGRET_EDIT,                           //
    DLGRET_INPUT,                         // IDD_DIALOG_INPUT
    DLGRET_MSGBOX,                  // IDD_DIALOG_MSGBOX
    DLGRET_APP_BGROND,         //	IDD_DIALOG_AP_BGROND
    DLGRET_CANCELED

} DLGRet_Value_e_Type;

/*----------------------״̬�����������������---------------------*/
// SettingMenu Applet ״̬��״̬��
typedef enum _SportsMenuState
{
   SPORT_STATE_NONE,
   SPORT_STATE_INIT,
   SPORT_STATE_MAIN,
   SPORT_STATE_PETOMETER,   
   SPORT_STATE_PETOMETER_OPTION, // Pedometer Option
   SPORT_STATE_PEDOMETER_RECORD,  //Pedometer Record 
   SPORT_STATE_PEDOMETER_RECORD_VIEW,//Pedometer Record View
   SPORT_STATE_PERSONAL,// Personal 
   SPORT_STATE_PERSONAL_OPTION,// Personal option
   SPORT_STATE_PERSONAL_EDIT,
   SPORT_STATE_INPUTED,      //Input 
   SPORT_STATE_MSGBOX,      //msgbox
   SPORT_STATE_APP_BGROUND,  //bgrond run
   SPORT_STATE_EXIT
} SportsMenuState;

// Personal Unit Data Flag ״̬��״̬��

typedef enum _SportUnitData
{
	UNIT_DATA_NAME,
	UNIT_DATA_HEIGHT,
	UNIT_DATA_WEIGHT,
	UNIT_DATA_WALK_STEP,
	UNIT_DATA_RUN_STEP,
	UINT_DATA_BRITHDAY
}SportUnitData;
// Pedometer Applet ״̬��״̬��
typedef enum _PedometerState
{
	PM_IDLE,//START_BACK
	PM_RUNNING,//PAUSE_BACK
	PM_STOPED,//RESUME_BACK
	PM_REST_CONFIRM
}PedometerState;

typedef enum _PedEntryDialogState
{
	/*entry Pedometer Dialog*/
	ENTRY_PED,
	/*entry Pedometer Record Dialog*/
	ENTRY_REC,
	/*entry_Pedometer Personal Dialog*/	
	ENTRY_PERS,
	/*entry_Pedometer Personal Dialog*/
	ENTRY_NONE
	
}PedEntryDialogState;

typedef enum _PedEntryRecordState
{
	/*entry Last Record Flag*/
	REC_LAST,
	/*entry accumulate Record Flag*/
	REC_ACC,
	REC_REST,
	REC_NULL
}PedEntryRecordState;

typedef enum _ProFileUser
{
	USER_ONE = 0,//User one
	USER_TWO,//User Two
	USER_THREE, //User Three
	USER_NONE//temp Profile
}ProFileUser;

typedef enum _BWalkOrRun
{
	STATE_WALK,
	STATE_RUN
}BWalkOrRun;

typedef enum _InputMode
{
    LOCAL_NUMBER_INPUT,  //�ڵ�ǰλ�ñ༭
    OPT_TEXT_INPUT,  //���������text�ؼ��༭
    EDIT_MENU_MODE,  //�˵�ģʽ
    EDIT_GROUP  
}Input_mode;

// ״̬���������ظ�״̬������������ֵ����
typedef enum _NextFSMAction
{
   NFSMACTION_WAIT,
   NFSMACTION_CONTINUE
} NextFSMAction;

// ����ģ����Ϣ��ģ�����ü����Ľṹ
typedef struct _SportsMenuMod
{
    DECLARE_VTBL(IModule)
    uint32 m_nRefs;
} SportsMenuMod;

typedef enum _SportsOptionMsgType
{
    SPORT_SIG_RUN,
    SPORT_RUN_WITH_MUSIC,
    SPORT_RUN_WITH_FM
    
} SportsOptionMsgType;

// SportApp Personal Uint Data����ṹ�壺
typedef struct _CSportSAppUintData
{
	AECHAR  m_pName[SPORT_EDIT_MAX_LEN];
	uint16 m_nHeight;
	uint16 m_nWeight;
	uint16 m_nWalkstep;
	uint16 m_nRunstep;
	uint16 m_nBrithday;
	boolean m_bGender;
}CSportSAppUintData;

// SportApp Pedometer Data����ṹ�壺
typedef struct _CSportSPedoMeterData
{
	AECHAR m_pPlan[SPORT_EDIT_MAX_LEN];
	uint32 m_nStep;
	AECHAR m_nDistance[10];
	AECHAR m_nSpeed[5];
	uint32 m_nSportendtime;
	uint32 m_nTicks;
	uint32 m_nNumDays;
	AECHAR m_dwDispTime[SPORT_PED_MAX_TIME_LEN];
	AECHAR m_nCalorie[10];
	boolean m_suspending;
	boolean m_bPedRunOrWalkState;
	
}CSportSPedoMeterData;

// SportApp Data����ṹ�壺
typedef struct _CSportSAppData
{
	CSportSPedoMeterData m_nSportMeterData;//Last Pedometer Record
	CSportSAppUintData m_nSportUintData;//uint Message  Only  Three Name
	CSportSPedoMeterData m_nAccRecord; // Accumulate Record
	
}CSportSAppData;



// SportAppMenu Data����ṹ�壺
typedef struct _CSportsAppMenu
{
	AECHAR	*m_pUnitDataInputBuf;
	boolean m_bTempEntryView;
	uint16 m_nCtlID; //Control ID
	uint16 m_wMsgResID;
	uint8 animatedIndex; //Animation Image  Index 
	IFileMgr *m_pFileMgr;       // IFileMgr interface
	IStatic *pStatic;
	IMenuCtl* m_pSAppMenu;
	ITimeCtl* m_pSAppTime;
	IImage *imageIconAnimated[SPORT_DISPLAY_ANIMATION_NUM];
	PedEntryRecordState m_nEntryRecordState;
	ProFileUser 	m_nSelProFile;
	ProFileUser     m_nTempSelProFile; //temp Select 
	PedEntryDialogState m_nEntryDialogState;
	PedometerState m_nPedometerState;
	CSportSAppData m_nSportSAppData[SPORT_USER_NUM]; //Three User Data
	SportUnitData m_nUnitDataFlag;
	uint16 m_nUnitDataEditFlag;
        uint16              m_nUnitDataEditPrivFlag;
	SportsMenuState m_PopMsgBoxBackFlag;
	Input_mode     m_nInputMode;
        SportsOptionMsgType m_nMsgTypeFlag;
}CSportsAppMenu;

// SettingMenu Applet����ṹ�壺
typedef struct _CSportsMenu
{
    DECLARE_VTBL(ISportsMenu)
    uint32           m_nRefs;            // �������ü�����
    IModule          *m_pModule;
    IDisplay         *m_pDisplay;
    IShell           *m_pShell;
    AEERect          m_rc;               // ��ǰ��Ի���IDialog�ӿ�ָ��
    IDialog          *m_pActiveDlg;      // ��ǰ��Ի���ID
    uint16           m_pActiveDlgID;     // ��Ի���ر�ʱ�ķ��ؽ��
    DLGRet_Value_e_Type   m_eDlgRet;     // �Ƿ񲻸�д�Ի��򷵻ؽ����һ�������Ҫ��д(FALSE)��
    boolean          m_bNotOverwriteDlgRet;// Applet ǰһ״̬
    SportsMenuState m_ePreState;        // Applet ��ǰ״̬
    SportsMenuState m_eCurState;        // Applet�Ƿ��ڹ���״̬
    boolean          m_bSuspending;      // Applet�Ƿ�׼���ÿ��Դ������������¼����ı�����Ҫ���ڿ��ٰ������������
    boolean          m_bAppIsReady;
    IAnnunciator    *m_pAnn;
    IConfig         *m_pConfig; 
	uint16           m_currDlgId;       //���浱ǰ��ѡ�˵�λ��
    uint16           m_nResID;          //����ҵ�����ID��¼
    uint16           m_nSubDlgId;		    //�μ��˵�λ��
    uint16           m_sSubDlgId;       //�����˵�λ��
     IAnnunciator  *m_pIAnn;
	CSportsAppMenu m_pSportsAppMenu;
	boolean runOnBackgrounds;
	boolean GobackIdleFlag;
    boolean FromScheduleFlag;
	
} CSportsMenu;


/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// Ϊ��ֹ�û����ٰ���������¼�EVT_APPISREADY���¼��ɶ�ʱ��
// SettingMenu_APPIsReadyTimer����
#define APPISREADY_TIMER       200
#define EVT_APPISREADY   (EVT_USER+1)           

// ״̬�ƶ��꣺�Ƚ�ǰһ״̬�õ�ǰ״̬���£��ٽ���ǰ״̬��ΪnextState
#define MOVE_TO_STATE(nextState) {            \
            SportsMenuState tpState;            \
            tpState = nextState;          \
            pMe->m_ePreState = pMe->m_eCurState; \
            pMe->m_eCurState = tpState;        \
        }

#define SPORTS_MENU_SOFK_HIGHT 0
//�����ʺ�������ַ���
#define  BANNER_MAXTXT       8
/*==============================================================================
                                 ȫ������
==============================================================================*/

/*==============================================================================
                                 ���أ���̬������
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
/*----------------------�Ի�����غ�������---------------------*/
/*==============================================================================
����:
       SportSApp_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��SportSApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void SportSApp_ShowDialog(CSportsMenu *pMe,uint16  dlgResId);

/*==============================================================================
����:
    SportSApp_ShowMsgBox

˵��:
    ������״̬����������������Ϣ�Ի�����ʾ��Ϣ��ʾ�û���

����:
    pMe [in]: ָ�� Contact Applet ����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
    wTextResId [in]: ��Ϣ�ı���Դ ID��

����ֵ:
    none

��ע:

==============================================================================*/
extern int SportSApp_ShowMsgBox(CSportsMenu *pMe, uint16 wTextResId);

/*==============================================================================
����:
       SportSApp_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��SettingMenu Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean SportSApp_RouteDialogEvent(CSportsMenu *pMe,AEEEvent eCode,uint16 wParam,uint32 dwParam);

/*----------------------״̬����غ�������---------------------*/
/*==============================================================================
����:
       SportSApp_ProcessState
˵��:
       SportSApp Applet״̬��������������������pMe->m_currState������·����
       ��Ӧ�Ĵ�������

����:
       pMe [In]: ָ��SportSApp Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.

����ֵ:
       NFSMACTION_CONTINUE��ָʾ������״̬��״̬������ֹͣ��
       NFSMACTION_WAIT��ָʾ��Ҫ��ʾ�Ի��������û���Ӧ����״̬����

��ע:

==============================================================================*/
NextFSMAction SportSApp_ProcessState(CSportsMenu *pMe);
#endif
//SETTINGMENUPRIV_H

