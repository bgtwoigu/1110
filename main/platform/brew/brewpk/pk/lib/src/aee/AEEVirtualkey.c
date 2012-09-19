/*=============================================================================

FILE: AEEVirtualkey.c

SERVICES: virtual key for cc01 project

GENERAL DESCRIPTION:

PUBLIC CLASSES AND STATIC FUNCTIONS:

INITIALIZATION AND SEQUENCING REQUIREMENTS:

(c) COPYRIGHT 2004, 2006 TCL Incorporated.
                    All Rights Reserved.

                    TCL Proprietary
=============================================================================*/
/*=============================================================================
                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when         who     what, where, why
----------   ---     ----------------------------------------------------------
02/03/2005   whewei   Add zhuyin keypad.
27/10/2004   zp       ���������ȫ����ͼƬ�滻ʵ�֡�
06/07/2004   xyue     ���ݸ�ͨ�Կؼ��Ĵ���˼�룬���Ƹÿؼ���PEN�¼��Ĵ���
15/06/2004   XYUE     �������û��Լ�����ļ��̵���ʾ
15/06/2004   XYUE     �Բ��Ž��������
14/06/2004   XYUE     ����PEN_EVENT_MOVE�¼�
01/06/2004   xyue     ���ӶԼ��̵ı�Ե�ش��Ĵ���
20/04/2004   xyue     ���ӶԲ��Ž��������
19/04/2004   XYUE     ���Ӷ�ָ��ļ��Ͷ���С�ռ���ж�
13/04/2004   XYUE     ADD CALL NUMBER FUNCTION INTO SETKEYPAD ;
09/04/2004   XYUE     add function for char,number,symbol,especial for call number interface
17/03/2004   XYUE     chang it for first edition
03/03/2004   XYUE     Created edit history.  Using Virtual key in CC01 Project
=============================================================================*/
/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "OEMFeatures.h"
#include "AEE_OEMEvent.h"
#include "AEEDisp.h"
#include "AEE.h"
#include "AEE_OEM.h"
#include "AEEStdLib.h"
#include "AEEShell.h"
#include "AEEVirtualkey.h"
#include "AEEImage.h"
#include "OEMClassIds.h"
#include "AEEPen.h"
#include "AEEModTable.h"
#include "AEEStatic_priv.h"
#include "AEEModTable.h"
#include "AEEPointerHelpers.h"


#include "Appscommon.h"
#include "appscommonimages.brh"

#define AEEFS_SHAREDVK_DIR "fs:/image/virtualkey/"

//#include "aeecontrols_res.h"
//#include "AEE_OEMEvent.h"
/*===========================================================================

����

===========================================================================*/

#define MAX_NUMBER               50                  //Ԥ��50���������
//#define FONT_IMAGE_WIDTH       10                  //������
//#define BG_IMAGE_WIDTH         55                  //����ͼƬ�Ŀ��
//#define BG_IMAGE_HEIGHT        40                  //����ͼƬ�ĸ߶�
//#define VirtualKey_RES_FILE    AEE_RES_LANGDIR OEMIMAGES_RES_FILE
#define FontHeight               13
#define FontWidth                8
#define DAIL_IMAGE_MINWIDTH      176
#define DAIL_IMAGE_MINHEIGHT     108
#define CALC_IMAGE_MINWIDTH      176
#define CALC_IMAGE_MINHEIGHT     140
#define PY_KEYPAD_MINWIDTH       176
#ifndef WIN32
#define PY_KEYPAD_MINHEIGHT      59
#else
#define PY_KEYPAD_MINHEIGHT      63
#endif
#define OTHER_KEYPAD_MINWIDTH    240
#ifndef WIN32
#define OTHER_KEYPAD_MINHEIGHT   106
#else
#define OTHER_KEYPAD_MINHEIGHT   83
#endif

#define MAX_DAIL_NUMBER          12
#define MAX_CALC_NUMBER          25
#define MAX_PYKEYPAD_NUMBER      33
#define MAX_OTHERKEYPAD_NUMBER   45
/*==========================================================================

��������

===========================================================================*/
typedef struct _VkeyCtl
{
   DECLARE_VTBL(IVkeyCtl)  //DECLARE our virtual table,by doing so,we have placed function pointers
                           //in the begining of structure
   //class member variable 
   uint32           m_nRefs;                 // reference count
   AEERect          m_rc;                    // ���̾�������
   AEERect          GridRect[MAX_NUMBER];    //�������
   IShell *         m_pIShell;               // shell interface
   IDisplay *       m_pIDisplay;             //predefine display interface 
   boolean          m_bActive ;              //����״̬
   uint32           m_dwProps;               //�������������    
   PFNAEEEVENT      m_pfnEvent;              //�ص�������ָ��
   void *           m_pUserEvent;            //�ص������Ĳ���
   //AEECLSID         VarID;                 //��ȡ��ǰ���APPLET��ID����  
   IImage *         m_KeyPadNormal;      //������������ı���ͼƬָ��   
   IImage  *        m_KeyPadDown;        //�����������ڵ������¸ı�ѡ�е����ֵı���ͼƬָ��
   IImage *         m_pFont;                 //��ʾ������ͼƬָ��  
   AEERect          m_dial_rc[MAX_DAIL_NUMBER];           //���ò��Ž������15��С���θ�  
   AEERect          m_calc_rc[MAX_CALC_NUMBER];           //���ü��������
  
   RGBVAL           m_Bground;               //�û����õı���ͼƬ
   RGBVAL           m_Fillclr;               //�û��������ı���ͼƬ
   AEEVKeyPad       m_keypad;                //�û�����ļ���
   AEERect          UsrGridRc[60];           //�û�������������
   uint32           m_oldkey;
   uint32           m_curpros;
   boolean          b_multenter;
}VkeyCtl;

/*==============================================================================

��̬����

==============================================================================*/
//���ּ���

static AEEVKeyItem_Own  VNumberKeyItem[15] =
{
    {EVT_CHAR, '1', {'1'}},
    {EVT_CHAR, '2', {'2'}},
    {EVT_CHAR, '3', {'3'}},
    {EVT_CHAR, '4', {'4'}},
    {EVT_CHAR, ' ', {' '}},
        
    {EVT_CHAR, '5', {'5'}},
    {EVT_CHAR, '6', {'6'}},
    {EVT_CHAR, '7', {'7'}},
    {EVT_CHAR, '8', {'8'}},
    //{EVT_CHAR, '\n', {'\n'}},
    {EVT_CHAR, '+', {'+'}},
    
    {EVT_CHAR, '9', {'9'}},
    {EVT_CHAR, '0', {'0'}},
    //{EVT_CHAR, '.', {'.'}},    
    //{EVT_CHAR, ',', {','}},
    {EVT_CHAR, '*', {'*'}},
    {EVT_CHAR, '#', {'#'}},    
    {EVT_CHAR, ' ', {' '}}    
};

static AEETHAIVKeyItem_Own  VNumberTHAIKeyItem[15] =
{
    {EVT_CHAR, '1', {'1'},'1'},
    {EVT_CHAR, '2', {0x0E01,0x0E02,0x0E03,0x0E04,0x0E05,0x0E06,0x0E07,0x0E08,0x0E09,'2'},'10'},
    {EVT_CHAR, '3', {0x0E0A,0x0E0B,0x0E0C,0x0E0D,0x0E0E,0x0E0F,0x0E10,'3'},'8'},
    {EVT_CHAR, '4', {0x0E11,0x0E12,0x0E13,0x0E14,0x0E15,0x0E16,0x0E17,'4'},'8'},
    {EVT_CHAR, ' ', {' '},'1'},
        
    {EVT_CHAR, '5', {0x0E18,0x0E19,0x0E1A,0x0E1B,0x0E1C,0x0E1D,0x0E1E,'5'},'8'},
    {EVT_CHAR, '6', {0x0E1F,0x0E20,0x0E21,0x0E22,0x0E23,0x0E25,0x0E27,'6'},'8'},
    {EVT_CHAR, '7', {0x0E28,0x0E29,0x0E2A,0x0E2B,0x0E2C,0x0E2D,0x0E2E,'7'},'8'},
    {EVT_CHAR, '8', {0x0E31,0x0E34,0x0E35,0x0E36,0x0E37,0x0E38,0x0E39,0x0E30,'8'},'9'},
    //{EVT_CHAR, '\n', {'\n'}},
    {EVT_CHAR, '+', {'+'},'1'},
    
    {EVT_CHAR, '9', {0x0E32,0x0E33,0x0E40,0x0E41,0x0E42,0x0E43,0x0E44,0x0E46,0x0E24,'9'},'10'},
    {EVT_CHAR, '0', {0x0E48,0x0E49,0x0E4A,0x0E4B,0x0E47,0x0E4C,0x0E39,0x0E30,'0'},'9'},
    //{EVT_CHAR, '.', {'.'}},    
    //{EVT_CHAR, ',', {','}},
    {EVT_CHAR, '*', {'*'},'1'},
    {EVT_CHAR, '#', {'#'},'1'},    
    {EVT_CHAR, ' ', {' '},'1'}    
};

typedef enum
{   
	  /* DEVANAGARI SIGNS */
	MYNAMMAR_DEVANAGARI_S_DANDA		= 0x0964,
	MYNAMMAR_DEVANAGARI_S_DDANDA	= 0x0965,
			
    MYNAMMAR_C_KA   = 0x1000,
    MYNAMMAR_C_KHA  = 0x1001,
    MYNAMMAR_C_GA   = 0x1002,
    MYNAMMAR_C_GHA  = 0x1003,
    MYNAMMAR_C_NGA  = 0x1004,
    MYNAMMAR_C_CA   = 0x1005,
    MYNAMMAR_C_CHA  = 0x1006,
    MYNAMMAR_C_JA   = 0x1007,
    MYNAMMAR_C_JHA  = 0x1008,
    MYNAMMAR_C_NYA  = 0x1009,
    MYNAMMAR_C_NNYA = 0x100A,
    MYNAMMAR_C_TTA  = 0x100B,
    MYNAMMAR_C_TTHA = 0x100C,
    MYNAMMAR_C_DDA  = 0x100D,
    MYNAMMAR_C_DDHA = 0x100E,
    MYNAMMAR_C_NNA  = 0x100F,
    MYNAMMAR_C_TA   = 0x1010,
    MYNAMMAR_C_THA  = 0x1011,
    MYNAMMAR_C_DA   = 0x1012,
    MYNAMMAR_C_DHA  = 0x1013,
    MYNAMMAR_C_NA   = 0x1014,
    MYNAMMAR_C_PA   = 0x1015,
    MYNAMMAR_C_PHA  = 0x1016,
    MYNAMMAR_C_BA   = 0x1017,
    MYNAMMAR_C_BHA  = 0x1018,
    MYNAMMAR_C_MA   = 0x1019,
    MYNAMMAR_C_YA   = 0x101A,
    MYNAMMAR_C_RA   = 0x101B,
    MYNAMMAR_C_LA   = 0x101C,
    MYNAMMAR_C_WA   = 0x101D,
    MYNAMMAR_C_SA   = 0x101E,
    MYNAMMAR_C_HA   = 0x101F,	
    MYNAMMAR_C_LLA  = 0x1020,	
	
    MYNAMMAR_IV_A   = 0x1021,
    MYNAMMAR_IV_SA  = 0x1022,
    MYNAMMAR_IV_I   = 0x1023,
    MYNAMMAR_IV_II  = 0x1024,
    MYNAMMAR_IV_U   = 0x1025,
    MYNAMMAR_IV_UU  = 0x1026,
    MYNAMMAR_IV_E   = 0x1027,
    MYNAMMAR_IV_ME  = 0x1028,
    MYNAMMAR_IV_O   = 0x1029,
    MYNAMMAR_IV_AU  = 0x102A,	

	MYNAMMAR_DV_TAA = 0x102B,
    MYNAMMAR_DV_AA  = 0x102C,
    MYNAMMAR_DV_I   = 0x102D,
    MYNAMMAR_DV_II  = 0x102E,
    MYNAMMAR_DV_U   = 0x102F,
    MYNAMMAR_DV_UU  = 0x1030,
    MYNAMMAR_DV_E   = 0x1031,
    MYNAMMAR_DV_AI  = 0x1032,
    MYNAMMAR_DV_MON_II  = 0x1033,
    MYNAMMAR_DV_MON_O   = 0x1034,
    MYNAMMAR_DV_E_ABOVE = 0x1035,

	
    MYNAMMAR_S_ANUSVARA = 0x1036,
    MYNAMMAR_S_DOT      = 0x1037,
    MYNAMMAR_S_VISARGA  = 0x1038,
    MYNAMMAR_S_VIRAMA   = 0x1039,
    MYNAMMAR_S_ASAT     = 0x103A,
    
    MYNAMMAR_CS_YA      = 0x103B,
    MYNAMMAR_CS_RA      = 0x103C,
    MYNAMMAR_CS_WA      = 0x103D,
    MYNAMMAR_CS_HA      = 0x103E,
    
    MYNAMMAR_C_GREAT_SA = 0x103F,

    MYNAMMAR_D_ZERO  = 0x1040, /* MYNAMMAR digital 0 */
    MYNAMMAR_D_ONE   = 0x1041, /* MYNAMMAR digital 1 */
    MYNAMMAR_D_TWO   = 0x1042, /* MYNAMMAR digital 2 */
    MYNAMMAR_D_THREE = 0x1043, /* MYNAMMAR digital 3 */
    MYNAMMAR_D_FOUR  = 0x1044, /* MYNAMMAR digital 4 */
    MYNAMMAR_D_FIVE  = 0x1045, /* MYNAMMAR digital 5 */
    MYNAMMAR_D_SIX   = 0x1046, /* MYNAMMAR digital 6 */
    MYNAMMAR_D_SEVEN = 0x1047, /* MYNAMMAR digital 7 */
    MYNAMMAR_D_EIGHT = 0x1048, /* MYNAMMAR digital 8 */
    MYNAMMAR_D_NINE  = 0x1049, /* MYNAMMAR digital 9 */

    MYNAMMAR_S_LITTLE_SECTION = 0x104A,
    MYNAMMAR_S_SECTION        = 0x104B,
    
    MYNAMMAR_S_LOCATIVE  = 0x104C,
    MYNAMMAR_S_COMPLETED = 0x104D,
    MYNAMMAR_S_AFOREMENTONED = 0x104E,
    MYNAMMAR_S_GENITIVE      = 0x104F,
    
    MYNAMMAR_L_SHA = 0x1050,
    MYNAMMAR_L_SSA = 0x1051,
    MYNAMMAR_L_VR  = 0x1052,
    MYNAMMAR_L_VRR = 0x1053,
    MYNAMMAR_L_VL  = 0x1054,
    MYNAMMAR_L_VLL = 0x1055,
    MYNAMMAR_VS_VR = 0x1056,
    MYNAMMAR_VS_VRR = 0x1057,
    MYNAMMAR_VS_VL  = 0x1058,
    MYNAMMAR_VS_VLL = 0x1059,
    
    MYNAMMAR_L_M_NGA = 0x105A,
    MYNAMMAR_L_M_JHA = 0x105B,
    MYNAMMAR_L_M_BBA = 0x105C,
    MYNAMMAR_L_M_BBE = 0x105D,

    MYNAMMAR_CS_MON_MEDIALNA = 0x105E,
    MYNAMMAR_CS_MON_MEDIALMA = 0x105F,
    MYNAMMAR_CS_MON_MEDIALLA = 0x1060,
    
    MYNAMMAR_LS_KAREN_SHA         = 0x1061,
    MYNAMMAR_VS_SGAW_KAREN_EU     = 0x1062,
    MYNAMMAR_TM_SGAW_KAREN_HATHI  = 0x1063,
    MYNAMMAR_TM_SGAW_KAREN_KE_PHO = 0x1064,

    MYNAMMAR_L_WPK_THA = 0x1065,
    MYNAMMAR_L_WPK_PWA = 0x1066,
    
    MYNAMMAR_VS_WPK_EU = 0x1067,
    MYNAMMAR_VS_WPK_UE = 0x1068, 
    
    MYNAMMAR_S_WPK_TONE1 = 0x1069, 
    MYNAMMAR_S_WPK_TONE2 = 0x106A, 
    MYNAMMAR_S_WPK_TONE3 = 0x106B, 
    MYNAMMAR_S_WPK_TONE4 = 0x106C, 
    MYNAMMAR_S_WPK_TONE5 = 0x106D, 
    MYNAMMAR_L_EPK_NNA = 0x106E, 
    MYNAMMAR_L_EPK_YWA = 0x106F, 
    
    MYNAMMAR_L_EPK_GHWA     = 0x1070,
    
    MYNAMMAR_VS_GEBA_KAREN1 = 0x1071, 
    
    MYNAMMAR_VSK_OE = 0x1072, 
    MYNAMMAR_VSK_U  = 0x1073, 
    MYNAMMAR_VSK_EE = 0x1074, 
    
    MYNAMMAR_LS_KA  = 0x1075,
    MYNAMMAR_LS_KHA = 0x1076,
    MYNAMMAR_LS_GA  = 0x1077,
    MYNAMMAR_LS_CA  = 0x1078,
    MYNAMMAR_LS_ZA  = 0x1079,
    MYNAMMAR_LS_NYA = 0x107A,
    MYNAMMAR_LS_DA  = 0x107B,
    MYNAMMAR_LS_NA  = 0x107C,
    MYNAMMAR_LS_PHA = 0x107D,
    MYNAMMAR_LS_FA  = 0x107E,
    MYNAMMAR_LS_BA  = 0x107F,
    MYNAMMAR_LS_THA = 0x1080,
    MYNAMMAR_LS_HA  = 0x1081,
  
    MYNAMMAR_CSSM_WA = 0x1082, 
    MYNAMMAR_VSS_AA  = 0x1083, 
    MYNAMMAR_VSS_E   = 0x1084, 
    MYNAMMAR_VSS_EA  = 0x1085, 
    MYNAMMAR_VSS_FY  = 0x1086, 
    MYNAMMAR_SS_TONE2 = 0x1087, 
    MYNAMMAR_SS_TONE3 = 0x1088, 
    MYNAMMAR_SS_TONE5 = 0x1089, 
    MYNAMMAR_SS_TONE6 = 0x108A, 
    MYNAMMAR_SSC_TONE2 = 0x108B, 
    MYNAMMAR_SSC_TONE3 = 0x108C, 
    
    MYNAMMAR_SSC_EMPHATICTONE = 0x108D, 
    
    MYNAMMAR_LRP_FA    = 0x108E, 
    MYNAMMAR_SRP_TONE5 = 0x108F, 
    
    MYNAMMAR_SD_ZERO  = 0x1090, /* MYNAMMAR digital 0 */
    MYNAMMAR_SD_ONE   = 0x1091, /* MYNAMMAR digital 1 */
    MYNAMMAR_SD_TWO   = 0x1092, /* MYNAMMAR digital 2 */
    MYNAMMAR_SD_THREE = 0x1093, /* MYNAMMAR digital 3 */
    MYNAMMAR_SD_FOUR  = 0x1094, /* MYNAMMAR digital 4 */
    MYNAMMAR_SD_FIVE  = 0x1095, /* MYNAMMAR digital 5 */
    MYNAMMAR_SD_SIX   = 0x1096, /* MYNAMMAR digital 6 */
    MYNAMMAR_SD_SEVEN = 0x1097, /* MYNAMMAR digital 7 */
    MYNAMMAR_SD_EIGHT = 0x1098, /* MYNAMMAR digital 8 */
    MYNAMMAR_SD_NINE  = 0x1099, /* MYNAMMAR digital 9 */
    
    MYNAMMAR_SK_TONE1 = 0x109A, /* sign */
    MYNAMMAR_SK_TONE3 = 0x109B,
    
    MYNAMMAR_VSA_A  = 0x109C,
    MYNAMMAR_VSA_AI = 0x109D,
    MYNAMMAR_SS_ONE = 0x109E,
    MYNAMMAR_SS_EXCLAMATION = 0x109F
    
}Mynammar_characters_enum;


static AEETHAIVKeyItem_Own  VNumberMYNAMMAKeyItem[15] =
{
    {EVT_CHAR, '1', {MYNAMMAR_C_KA, MYNAMMAR_C_KHA, MYNAMMAR_C_GA, MYNAMMAR_C_GHA, MYNAMMAR_C_NGA, MYNAMMAR_D_ONE, '1'},'7'},
    {EVT_CHAR, '2', {MYNAMMAR_C_CA, MYNAMMAR_C_CHA, MYNAMMAR_C_JA, MYNAMMAR_C_JHA, MYNAMMAR_C_NYA, MYNAMMAR_C_NNYA, MYNAMMAR_D_TWO, '2'},'8'},
    {EVT_CHAR, '3', {MYNAMMAR_C_TTA, MYNAMMAR_C_TTHA, MYNAMMAR_C_DDA, MYNAMMAR_C_DDHA, MYNAMMAR_C_NNA, MYNAMMAR_D_THREE, '3'},'7'},
    {EVT_CHAR, '4', {MYNAMMAR_C_TA, MYNAMMAR_C_THA, MYNAMMAR_C_DA, MYNAMMAR_C_DHA, MYNAMMAR_C_NA, MYNAMMAR_D_FOUR, '4'},'7'},
    {EVT_CHAR, ' ', {' '},'1'},
        
    {EVT_CHAR, '5', {MYNAMMAR_C_PA, MYNAMMAR_C_PHA, MYNAMMAR_C_BA, MYNAMMAR_C_BHA, MYNAMMAR_C_MA, MYNAMMAR_D_FIVE, '5'},'7'},
    {EVT_CHAR, '6', {MYNAMMAR_C_YA, MYNAMMAR_C_RA, MYNAMMAR_C_LA, MYNAMMAR_C_WA, MYNAMMAR_C_SA, MYNAMMAR_D_SIX,  '6'},'7'},
    {EVT_CHAR, '7', {MYNAMMAR_C_HA, MYNAMMAR_C_LLA, MYNAMMAR_C_GREAT_SA, MYNAMMAR_D_SEVEN, '7'},'5'},
    {EVT_CHAR, '8', {MYNAMMAR_IV_A, MYNAMMAR_IV_SA, MYNAMMAR_IV_I, MYNAMMAR_IV_II, MYNAMMAR_IV_U, MYNAMMAR_IV_UU, MYNAMMAR_IV_E, MYNAMMAR_IV_ME, MYNAMMAR_IV_O, MYNAMMAR_IV_AU, MYNAMMAR_D_EIGHT, '8'},'12'},
    //{EVT_CHAR, '\n', {'\n'}},
    {EVT_CHAR, '+', {'+'},'1'},
    
    {EVT_CHAR, '9', {MYNAMMAR_DV_TAA, MYNAMMAR_DV_AA, MYNAMMAR_DV_I, MYNAMMAR_DV_II, MYNAMMAR_DV_U, MYNAMMAR_DV_UU, MYNAMMAR_DV_E, MYNAMMAR_DV_AI, MYNAMMAR_DV_MON_II, MYNAMMAR_DV_MON_O, MYNAMMAR_DV_E_ABOVE, MYNAMMAR_D_NINE, '9'},'13'},
    {EVT_CHAR, '0', {' ', MYNAMMAR_S_VIRAMA, MYNAMMAR_S_ANUSVARA, MYNAMMAR_S_DOT, MYNAMMAR_S_VISARGA, MYNAMMAR_S_ASAT, MYNAMMAR_CS_YA, MYNAMMAR_CS_RA, MYNAMMAR_CS_WA, MYNAMMAR_CS_HA, '.', MYNAMMAR_S_LITTLE_SECTION, MYNAMMAR_S_SECTION, MYNAMMAR_S_LOCATIVE, MYNAMMAR_S_COMPLETED, MYNAMMAR_S_AFOREMENTONED, MYNAMMAR_S_GENITIVE, MYNAMMAR_D_ZERO, '0'},'19'},
    //{EVT_CHAR, '.', {'.'}},    
    //{EVT_CHAR, ',', {','}},
    {EVT_CHAR, '*', {'*'},'1'},
    {EVT_CHAR, '#', {'#'},'1'},    
    {EVT_CHAR, ' ', {' '},'1'}    
};
/*static AEEVKeyItem_Own  VNumberKeyItem[10] =
{
    {EVT_CHAR, '1', {'1'}},
    {EVT_CHAR, '2', {'2'}},
    {EVT_CHAR, '3', {'3'}},
    {EVT_CHAR, '4', {'4'}},
    {EVT_CHAR, '5', {'5'}},
    {EVT_CHAR, '6', {'6'}},
    {EVT_CHAR, '7', {'7'}},
    {EVT_CHAR, '8', {'8'}},
    {EVT_CHAR, '9', {'9'}},
    {EVT_CHAR, '0', {'0'}}
};
*/
static AEEVKeyPad_Own VNumberKeyPad = {3,5};
//Сд��ĸ����

static AEEVKeyItem_Own  VLCharKeyItem[MAX_PYKEYPAD_NUMBER] =
{
    {EVT_CHAR, 'q', {'q'}},
    {EVT_CHAR, 'w', {'w'}},
    {EVT_CHAR, 'e', {'e'}},
    {EVT_CHAR, 'r', {'r'}},
    {EVT_CHAR, 't', {'t'}},
    {EVT_CHAR, 'y', {'y'}},
    {EVT_CHAR, 'u', {'u'}},
        
    {EVT_CHAR, 'i', {'i'}},
    {EVT_CHAR, 'o', {'o'}},
    {EVT_CHAR, 'p', {'p'}},    
    {EVT_CHAR, 'a', {'a'}},
    {EVT_CHAR, 's', {'s'}},
    {EVT_CHAR, 'd', {'d'}},
    {EVT_CHAR, 'f', {'f'}},
        
    {EVT_CHAR, 'g', {'g'}},
    {EVT_CHAR, 'h', {'h'}},
    {EVT_CHAR, 'j', {'j'}},
    {EVT_CHAR, 'k', {'k'}},
    {EVT_CHAR, 'l', {'l'}},    
    {EVT_CHAR, 'z', {'z'}},
    {EVT_CHAR, 'x', {'x'}},
        
    {EVT_CHAR, 'c', {'c'}},
    {EVT_CHAR, 'v', {'v'}},
    {EVT_CHAR, 'b', {'b'}},
    {EVT_CHAR, 'n', {'n'}},
    {EVT_CHAR, 'm', {'m'}},    
    {EVT_CHAR, ',', {','}},
    {EVT_CHAR, 0x3002, {0x3002}},
        
    {EVT_CHAR, '?', {'?'}},
    {EVT_CHAR, '!', {'!'}},
    {EVT_CHAR, ' ', {' '}},
    {EVT_CHAR, '\n', {'\n'}} 
};

/*
static AEEVKeyItem_Own  VLCharKeyItem[MAX_PYKEYPAD_NUMBER] =
{
    {EVT_CHAR, 'q', {'q'}},
    {EVT_CHAR, 'w', {'w'}},
    {EVT_CHAR, 'e', {'e'}},
    {EVT_CHAR, 'r', {'r'}},
    {EVT_CHAR, 't', {'t'}},
    {EVT_CHAR, 'y', {'y'}},
    {EVT_CHAR, 'u', {'u'}},
    {EVT_CHAR, 'i', {'i'}},
    {EVT_CHAR, 'o', {'o'}},
    {EVT_CHAR, 'p', {'p'}},
    {EVT_CHAR, 0x3002, {0x3002}},
    
    {EVT_CHAR, 'a', {'a'}},
    {EVT_CHAR, 's', {'s'}},
    {EVT_CHAR, 'd', {'d'}},
    {EVT_CHAR, 'f', {'f'}},
    {EVT_CHAR, 'g', {'g'}},
    {EVT_CHAR, 'h', {'h'}},
    {EVT_CHAR, 'j', {'j'}},
    {EVT_CHAR, 'k', {'k'}},
    {EVT_CHAR, 'l', {'l'}},
    {EVT_CHAR, ',', {','}},
    {EVT_CHAR, 0x3001, {0x3001}},
    
    {EVT_CHAR, 'z', {'z'}},
    {EVT_CHAR, 'x', {'x'}},
    {EVT_CHAR, 'c', {'c'}},
    {EVT_CHAR, 'v', {'v'}},
    {EVT_CHAR, 'b', {'b'}},
    {EVT_CHAR, 'n', {'n'}},
    {EVT_CHAR, 'm', {'m'}},
    {EVT_CHAR, ':', {':'}},
    {EVT_CHAR, ';', {';'}},
    {EVT_CHAR, '?', {'?'}},
    {EVT_CHAR, '!', {'!'}}
}; */
static AEEVKeyPad_Own VLCharKeyPad = {5,7};

//��д��ĸ����

static AEEVKeyItem_Own  VBCharKeyItem[MAX_PYKEYPAD_NUMBER] =
{
    {EVT_CHAR, 'Q', {'Q'}},
    {EVT_CHAR, 'W', {'W'}},
    {EVT_CHAR, 'E', {'E'}},
    {EVT_CHAR, 'R', {'R'}},
    {EVT_CHAR, 'T', {'T'}},
    {EVT_CHAR, 'Y', {'Y'}},
    {EVT_CHAR, 'U', {'U'}},
    {EVT_CHAR, 'I', {'I'}},
    {EVT_CHAR, 'O', {'O'}},
    {EVT_CHAR, 'P', {'P'}},
    {EVT_CHAR, '-', {'-'}},
    
    {EVT_CHAR, 'A', {'A'}},
    {EVT_CHAR, 'S', {'S'}},
    {EVT_CHAR, 'D', {'D'}},
    {EVT_CHAR, 'F', {'F'}},
    {EVT_CHAR, 'G', {'G'}},
    {EVT_CHAR, 'H', {'H'}},
    {EVT_CHAR, 'J', {'J'}},
    {EVT_CHAR, 'K', {'K'}},
    {EVT_CHAR, 'L', {'L'}},
    {EVT_CHAR, ',', {','}},
    {EVT_CHAR, '.', {'.'}},
    
    {EVT_CHAR, 'Z', {'Z'}},
    {EVT_CHAR, 'X', {'X'}},
    {EVT_CHAR, 'C', {'C'}},
    {EVT_CHAR, 'V', {'V'}},
    {EVT_CHAR, 'B', {'B'}},
    {EVT_CHAR, 'N', {'N'}},
    {EVT_CHAR, 'M', {'M'}},
    {EVT_CHAR, ':', {':'}},
    {EVT_CHAR, ';', {';'}},
    {EVT_CHAR, '?', {'?'}},
    {EVT_CHAR, '!', {'!'}}
};
static AEEVKeyPad_Own VBCharKeyPad = {3,11};                                      
//SYMBOL KEYPAD 

static AEEVKeyItem_Own  VSymbolKeyItem[MAX_OTHERKEYPAD_NUMBER] =
{
    {EVT_CHAR, ':', {':'}},
    {EVT_CHAR, ',', {','}},
    {EVT_CHAR, ';', {';'}}, 
    {EVT_CHAR, '.', {'.'}},
    {EVT_CHAR, 0x3002, {0x3002}},
    {EVT_CHAR, '?', {'?'}},    
    {EVT_CHAR, '!', {'!'}},

    {EVT_CHAR, '(', {'('}},
    {EVT_CHAR, ')', {')'}},
    {EVT_CHAR, '@', {'@'}},
    {EVT_CHAR, 0x2026, {0x2026}},
    {EVT_CHAR, 0x3001, {0x3001}},
    {EVT_CHAR, '/', {'/'}},
    {EVT_CHAR, '\\', {'\\'}},    

    {EVT_CHAR, '|', {'|'}},
    {EVT_CHAR, '^', {'^'}},
    {EVT_CHAR, '<', {'<'}},
    {EVT_CHAR, '>', {'>'}},
    {EVT_CHAR, '+', {'+'}}, 
    {EVT_CHAR, '-', {'-'}},
    {EVT_CHAR, '_', {'_'}},
    
    {EVT_CHAR, '=', {'='}},
    {EVT_CHAR, '~', {'~'}},
    {EVT_CHAR, 0x201c, {0x201c}},
    {EVT_CHAR, 0x201d, {0x201d}},
    {EVT_CHAR, '{', {'{'}},
    {EVT_CHAR, '}', {'}'}}, 
    {EVT_CHAR, '*', {'*'}},

    {EVT_CHAR, '#', {'#'}},
    {EVT_CHAR, '%', {'%'}},
    {EVT_CHAR, '$', {'$'}},
    {EVT_CHAR, '&', {'&'}},
    {EVT_CHAR, 0x2018, {0x2018}},
    {EVT_CHAR, 0x2019, {0x2019}},
    {EVT_CHAR, '[', {'['}},
        
    {EVT_CHAR, ']', {']'}},
    {EVT_CHAR, 0x300a, {0x300a}},
    {EVT_CHAR, 0x300b, {0x300b}},
    {EVT_CHAR, ' ', {' '}},    
    {EVT_CHAR, '\n', {'\n'}}      
};

/*
static AEEVKeyItem_Own  VSymbolKeyItem[MAX_OTHERKEYPAD_NUMBER] =
{
    {EVT_CHAR, ',', {','}},
    {EVT_CHAR, 0x3002, {0x3002}},
    {EVT_CHAR, '!', {'!'}},
    {EVT_CHAR, '?', {'?'}},
    {EVT_CHAR, 0x201c, {0x201c}},
    {EVT_CHAR, 0x201d, {0x201d}},
    {EVT_CHAR, 0x2018, {0x2018}},
    {EVT_CHAR, 0x2019, {0x2019}},
    {EVT_CHAR, ';', {';'}},
    {EVT_CHAR, ':', {':'}},
    {EVT_CHAR, 0x3001, {0x3001}},
    
    {EVT_CHAR, '~', {'~'}},
    {EVT_CHAR, 0x2026, {0x2026}},
    {EVT_CHAR, '@', {'@'}},
    {EVT_CHAR, '#', {'#'}},
    {EVT_CHAR, '$', {'$'}},
    {EVT_CHAR, '%', {'%'}},
    {EVT_CHAR, '^', {'^'}},
    {EVT_CHAR, '&', {'&'}},
    {EVT_CHAR, '*', {'*'}},
    {EVT_CHAR, '(', {'('}},
    {EVT_CHAR, ')', {')'}},
    
    {EVT_CHAR, '"', {'"'}},
    {EVT_CHAR, 0x00a3, {0x00a3}},
    {EVT_CHAR, 0x2103, {0x2103}},
    {EVT_CHAR, '.', {'.'}},
    {EVT_CHAR, 0x00a5, {0x00a5}},
    {EVT_CHAR, '+', {'+'}}, 
    {EVT_CHAR, '-', {'-'}},
    {EVT_CHAR, '|', {'|'}},
    {EVT_CHAR, '/', {'/'}},
    {EVT_CHAR, '=', {'='}},
    {EVT_CHAR, '_', {'_'}},
    
    {EVT_CHAR, '`', {'`'}},
    {EVT_CHAR, 0x300a, {0x300a}},
    {EVT_CHAR, 0x300b, {0x300b}},
    {EVT_CHAR, '[', {'['}},
    {EVT_CHAR, ']', {']'}},
    {EVT_CHAR, '{', {'{'}},
    {EVT_CHAR, '}', {'}'}},
    {EVT_CHAR, '<', {'<'}},
    {EVT_CHAR, '>', {'>'}},
    {EVT_CHAR, '\\', {'\\'}},
    {EVT_CHAR, 0x00a2, {0x00a2}}
};
*/
static AEEVKeyPad_Own VSymbolKeyPad = {6,7};

//������Сд��ĸ    

                                      
static AEEVKeyItem_Own  VNumLCharKeyItem[MAX_OTHERKEYPAD_NUMBER] =
{
    {EVT_CHAR, 'q', {'q'}},
    {EVT_CHAR, 'w', {'w'}},
    {EVT_CHAR, 'e', {'e'}},
    {EVT_CHAR, 'r', {'r'}},
    {EVT_CHAR, 't', {'t'}},
    {EVT_CHAR, 'y', {'y'}},
    {EVT_CHAR, 'u', {'u'}},

    {EVT_CHAR, 'i', {'i'}},
    {EVT_CHAR, 'o', {'o'}},
    {EVT_CHAR, 'p', {'p'}},
    {EVT_CHAR, 'a', {'a'}},
    {EVT_CHAR, 's', {'s'}},
    {EVT_CHAR, 'd', {'d'}},
    {EVT_CHAR, 'f', {'f'}},

    {EVT_CHAR, 'g', {'g'}},
    {EVT_CHAR, 'h', {'h'}},
    {EVT_CHAR, 'j', {'j'}},
    {EVT_CHAR, 'k', {'k'}},
    {EVT_CHAR, 'l', {'l'}},
    {EVT_CHAR, 'z', {'z'}},
    {EVT_CHAR, 'x', {'x'}},

    {EVT_CHAR, 'c', {'c'}},
    {EVT_CHAR, 'v', {'v'}},
    {EVT_CHAR, 'b', {'b'}},
    {EVT_CHAR, 'n', {'n'}},
    {EVT_CHAR, 'm', {'m'}},
    {EVT_CHAR, ':', {':'}}, 
    {EVT_CHAR, ',', {','}},

    {EVT_CHAR, '.', {'.'}},
    {EVT_CHAR, '?', {'?'}},
    {EVT_CHAR, '!', {'!'}}, 
    {EVT_CHAR, '(', {'('}},
    {EVT_CHAR, ')', {')'}},
    {EVT_CHAR, '-', {'-'}},    
    {EVT_CHAR, '_', {'_'}},

    {EVT_CHAR, '/', {'/'}},
    {EVT_CHAR, '@', {'@'}}, 
    {EVT_CHAR, ' ', {' '}},    
    {EVT_CHAR, '\n', {'\n'}}
    
};  

/*                                      
static AEEVKeyItem_Own  VNumLCharKeyItem[MAX_OTHERKEYPAD_NUMBER] =
{
    {EVT_CHAR, '1', {'1'}},
    {EVT_CHAR, '2', {'2'}},
    {EVT_CHAR, '3', {'3'}},
    {EVT_CHAR, '4', {'4'}},
    {EVT_CHAR, '5', {'5'}},
    {EVT_CHAR, '6', {'6'}},
    {EVT_CHAR, '7', {'7'}},
    {EVT_CHAR, '8', {'8'}},
    {EVT_CHAR, '9', {'9'}},
    {EVT_CHAR, '0', {'0'}},
    {EVT_CHAR, '@', {'@'}},
    
    {EVT_CHAR, 'q', {'q'}},
    {EVT_CHAR, 'w', {'w'}},
    {EVT_CHAR, 'e', {'e'}},
    {EVT_CHAR, 'r', {'r'}},
    {EVT_CHAR, 't', {'t'}},
    {EVT_CHAR, 'y', {'y'}},
    {EVT_CHAR, 'u', {'u'}},
    {EVT_CHAR, 'i', {'i'}},
    {EVT_CHAR, 'o', {'o'}},
    {EVT_CHAR, 'p', {'p'}},
    {EVT_CHAR, '-', {'-'}},
    
    {EVT_CHAR, 'a', {'a'}},
    {EVT_CHAR, 's', {'s'}},
    {EVT_CHAR, 'd', {'d'}},
    {EVT_CHAR, 'f', {'f'}},
    {EVT_CHAR, 'g', {'g'}},
    {EVT_CHAR, 'h', {'h'}},
    {EVT_CHAR, 'j', {'j'}},
    {EVT_CHAR, 'k', {'k'}},
    {EVT_CHAR, 'l', {'l'}},
    {EVT_CHAR, ',', {','}},
    {EVT_CHAR, '.', {'.'}},
    
    {EVT_CHAR, 'z', {'z'}},
    {EVT_CHAR, 'x', {'x'}},
    {EVT_CHAR, 'c', {'c'}},
    {EVT_CHAR, 'v', {'v'}},
    {EVT_CHAR, 'b', {'b'}},
    {EVT_CHAR, 'n', {'n'}},
    {EVT_CHAR, 'm', {'m'}},
    {EVT_CHAR, ':', {':'}},
    {EVT_CHAR, ';', {';'}},
    {EVT_CHAR, '?', {'?'}},
    {EVT_CHAR, 0x6362, {0x6362}}
};  
*/
//static AEEVKeyPad_Own VNumLCharKeyPad = {4,11}; 
static AEEVKeyPad_Own VNumLCharKeyPad = {6,7};                            
//�������д��ĸ

static AEEVKeyItem_Own  VNumBCharKeyItem[MAX_OTHERKEYPAD_NUMBER] =  
{
    {EVT_CHAR, 'Q', {'Q'}},
    {EVT_CHAR, 'W', {'W'}},
    {EVT_CHAR, 'E', {'E'}},
    {EVT_CHAR, 'R', {'R'}},
    {EVT_CHAR, 'T', {'T'}},
    {EVT_CHAR, 'Y', {'Y'}},
    {EVT_CHAR, 'U', {'U'}},

    {EVT_CHAR, 'I', {'I'}},
    {EVT_CHAR, 'O', {'O'}},
    {EVT_CHAR, 'P', {'P'}},
    {EVT_CHAR, 'A', {'A'}},
    {EVT_CHAR, 'S', {'S'}},
    {EVT_CHAR, 'D', {'D'}},
    {EVT_CHAR, 'F', {'F'}},

    {EVT_CHAR, 'G', {'G'}},
    {EVT_CHAR, 'H', {'H'}},
    {EVT_CHAR, 'J', {'J'}},
    {EVT_CHAR, 'K', {'K'}},
    {EVT_CHAR, 'L', {'L'}},
    {EVT_CHAR, 'Z', {'Z'}},
    {EVT_CHAR, 'X', {'X'}},

    {EVT_CHAR, 'C', {'C'}},
    {EVT_CHAR, 'V', {'V'}},
    {EVT_CHAR, 'B', {'B'}},
    {EVT_CHAR, 'N', {'N'}},
    {EVT_CHAR, 'M', {'M'}},
    {EVT_CHAR, ':', {':'}}, 
    {EVT_CHAR, ',', {','}},

    {EVT_CHAR, '.', {'.'}},
    {EVT_CHAR, '?', {'?'}},
    {EVT_CHAR, '!', {'!'}}, 
    {EVT_CHAR, '(', {'('}},
    {EVT_CHAR, ')', {')'}},
    {EVT_CHAR, '-', {'-'}},    
    {EVT_CHAR, '_', {'_'}},

    {EVT_CHAR, '/', {'/'}},
    {EVT_CHAR, '@', {'@'}}, 
    {EVT_CHAR, ' ', {' '}},    
    {EVT_CHAR, '\n', {'\n'}}


} ;

/*
static AEEVKeyItem_Own  VNumBCharKeyItem[MAX_OTHERKEYPAD_NUMBER] =  
{
    {EVT_CHAR, '1', {'1'}},
    {EVT_CHAR, '2', {'2'}},
    {EVT_CHAR, '3', {'3'}},
    {EVT_CHAR, '4', {'4'}},
    {EVT_CHAR, '5', {'5'}},
    {EVT_CHAR, '6', {'6'}},
    {EVT_CHAR, '7', {'7'}},
    {EVT_CHAR, '8', {'8'}},
    {EVT_CHAR, '9', {'9'}},
    {EVT_CHAR, '0', {'0'}},
    {EVT_CHAR, '@', {'@'}},
    
    {EVT_CHAR, 'Q', {'Q'}},
    {EVT_CHAR, 'W', {'W'}},
    {EVT_CHAR, 'E', {'E'}},
    {EVT_CHAR, 'R', {'R'}},
    {EVT_CHAR, 'T', {'T'}},
    {EVT_CHAR, 'Y', {'Y'}},
    {EVT_CHAR, 'U', {'U'}},
    {EVT_CHAR, 'I', {'I'}},
    {EVT_CHAR, 'O', {'O'}},
    {EVT_CHAR, 'P', {'P'}},
    {EVT_CHAR, '-', {'-'}},
    
    {EVT_CHAR, 'A', {'A'}},
    {EVT_CHAR, 'S', {'S'}},
    {EVT_CHAR, 'D', {'D'}},
    {EVT_CHAR, 'F', {'F'}},
    {EVT_CHAR, 'G', {'G'}},
    {EVT_CHAR, 'H', {'H'}},
    {EVT_CHAR, 'J', {'J'}},
    {EVT_CHAR, 'K', {'K'}},
    {EVT_CHAR, 'L', {'L'}},
    {EVT_CHAR, ',', {','}},
    {EVT_CHAR, '.', {'.'}},
    
    {EVT_CHAR, 'Z', {'Z'}},
    {EVT_CHAR, 'X', {'X'}},
    {EVT_CHAR, 'C', {'C'}},
    {EVT_CHAR, 'V', {'V'}},
    {EVT_CHAR, 'B', {'B'}},
    {EVT_CHAR, 'N', {'N'}},
    {EVT_CHAR, 'M', {'M'}},
    {EVT_CHAR, ':', {':'}},
    {EVT_CHAR, ';', {';'}},
    {EVT_CHAR, '?', {'?'}},
    {EVT_CHAR, 0x6362, {0x6362}}

} ;
*/
//static AEEVKeyPad_Own VNumBCharKeyPad = {4,11};
static AEEVKeyPad_Own VNumBCharKeyPad = {6,7};  

//tcl whewei050302 add for zhuyin begin
//ע������

static AEEVKeyItem_Own VZhuyinKeyItem[MAX_OTHERKEYPAD_NUMBER] = 
{
    {EVT_CHAR, 0xf405, {0xf405}},
    {EVT_CHAR, 0xf409, {0xf409}},
    {EVT_CHAR, ':', {':'}},
    {EVT_CHAR, ';', {';'}},
    {EVT_CHAR, 0xf413, {0xf413}},
    {EVT_CHAR, 0x3001, {0x3001}},
    {EVT_CHAR, '!', {'!'}},
    {EVT_CHAR, 0xf41a, {0xf41a}},   
    {EVT_CHAR, 0xf41e, {0xf41e}},
    {EVT_CHAR, 0xf422, {0xf422}},
    {EVT_CHAR, 0xf426, {0xf426}},
       
    {EVT_CHAR, 0xf406, {0xf406}},
    {EVT_CHAR, 0xf40a, {0xf40a}},
    {EVT_CHAR, 0xf40d, {0xf40d}},
    {EVT_CHAR, 0xf410, {0xf410}},
    {EVT_CHAR, 0xf414, {0xf414}},
    {EVT_CHAR, 0xf417, {0xf417}},
    {EVT_CHAR, 0xf427, {0xf427}},
    {EVT_CHAR, 0xf41b, {0xf41b}},
    {EVT_CHAR, 0xf41f, {0xf41f}},
    {EVT_CHAR, 0xf423, {0xf423}},
    {EVT_CHAR, ',', {','}},
    
    {EVT_CHAR, 0xf407, {0xf407}},
    {EVT_CHAR, 0xf40b, {0xf40b}},
    {EVT_CHAR, 0xf40e, {0xf40e}},
    {EVT_CHAR, 0xf411, {0xf411}},
    {EVT_CHAR, 0xf415, {0xf415}},
    {EVT_CHAR, 0xf418, {0xf418}},
    {EVT_CHAR, 0xf428, {0xf428}},
    {EVT_CHAR, 0xf41c, {0xf41c}},
    {EVT_CHAR, 0xf420, {0xf420}},
    {EVT_CHAR, 0xf424, {0xf424}},
    {EVT_CHAR, 0x3002, {0x3002}},    
    
    {EVT_CHAR, 0xf408, {0xf408}},
    {EVT_CHAR, 0xf40c, {0xf40c}},
    {EVT_CHAR, 0xf40f, {0xf40f}},
    {EVT_CHAR, 0xf412, {0xf412}},
    {EVT_CHAR, 0xf416, {0xf416}},
    {EVT_CHAR, 0xf419, {0xf419}},
    {EVT_CHAR, 0xf429, {0xf429}},
    {EVT_CHAR, 0xf41d, {0xf41d}},
    {EVT_CHAR, 0xf421, {0xf421}},
    {EVT_CHAR, 0xf425, {0xf425}},
    {EVT_CHAR, '?', {'?'}}   
};

static AEEVKeyPad_Own VZhuyinKeyPad = {4,11};
//tcl whewei050302 add for zhuyin end

//���ż���

static AECHAR  DialNumber[4][3] = {
                                    {'1','2','3'},
                                    {'4','5','6'},
                                    {'7','8','9'},
                                    {'*','0','#'}
                                   };

static AECHAR  CalcNumber[5][5] = {
                                     {'\0',13,14,10,11},
                                     {'S','7','8','9','+'},
                                     {'R','4','5','6','-'},
                                     {'C','1','2','3','*'},
                                     {12,'0','.','=','/'}
};
/*=================================================================================
��������
==================================================================================*/
static uint32    IVkeyCtl_AddRef(IVkeyCtl *po);
static uint32    IVkeyCtl_Release(IVkeyCtl *po);

static boolean   IVkeyCtl_HandleEvent(IVkeyCtl *po,AEEEvent evt, uint16 wp, uint32 dwp );
static boolean   IVkeyCtl_Redraw(IVkeyCtl *po);
static void      IVkeyCtl_SetActive(IVkeyCtl *po,boolean bActive);
static boolean   IVkeyCtl_IsActive(IVkeyCtl *po);
static void      IVkeyCtl_SetRect(IVkeyCtl *po,const AEERect * prc);
static void      IVkeyCtl_GetRect(IVkeyCtl *po, AEERect * prc);
static void      IVkeyCtl_SetProperties(IVkeyCtl *po,uint32 dwProps);
static uint32    IVkeyCtl_GetProperties(IVkeyCtl *po);
static void      IVkeyCtl_Reset(IVkeyCtl *po);
//�����¼�����
static void      IVkeyCtl_SetEventHandler(IVkeyCtl * po, PFNAEEEVENT pfn, void * pUser);
static boolean   VkeyCtl_CallEventHandler(VkeyCtl * pme, AEEEvent evt, uint16 wp, uint32 dwp);
//���ü���
static void      IVkeyCtl_SetKeyPad(IVkeyCtl * po,AEERect * prc,uint32  m_dwProps);
static void      VkeyCtl_SetNumberPad(VkeyCtl * pme,AEERect * prc);
static void      VkeyCtl_SetBigCharPad(VkeyCtl * pme,AEERect * prc);
static void      VkeyCtl_SetLittleCharPad(VkeyCtl * pme,AEERect * prc);
static void      VkeyCtl_SetSymbolPad(VkeyCtl * pme,AEERect * prc);
static void      VkeyCtl_SetNumberLittleCharPad(VkeyCtl * pme,AEERect * prc);
static void      VkeyCtl_SetNumberBigCharPad(VkeyCtl * pme,AEERect * prc);
static void      VkeyCtl_SetDialNumberPad(VkeyCtl* pme, AEERect *prc);
static void      VkeyCtl_SetCalculatorPad(VkeyCtl* pme, AEERect *prc);
static void       VkeyCtl_SetZhuyinCharPad(VkeyCtl* pme, AEERect *prc);//tcl whewei050302 add
static void      VkeyCtl_SetTHAIPad(VkeyCtl * pme,AEERect * prc);// add by pengyuangui 20120919
static void      VkeyCtl_SetMYNAMMAPad(VkeyCtl * pme,AEERect * prc);// add by pengyuangui 20120919


//�����û��Լ�����ļ���
static void      IVkeyCtl_SetColor(IVkeyCtl * po, RGBVAL Background , RGBVAL  ClrFill);
static int       IVkeyCtl_SetUserKeyPad(IVkeyCtl * po, AEERect *prc, AEEVKeyPad  *usr_pad);
static void      VkeyCtl_SetUserKeyPad(VkeyCtl * pme, AEERect *prc);
//ӳ����Ӧ�Ĳ���
static boolean      VkeyCtl_MapParam(VkeyCtl * pme,int cx,int cy,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapNumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapBigChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapLittleChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapSymbol(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapNumberLittleChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapNumberBigChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapUserKeyPad(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapDialNumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapCalcNumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);
static boolean      VkeyCtl_MapZhuyinChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);//tcl whewei050302 add
static boolean      VkeyCtl_MapTHAINumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);  // add by pengyuangui 20120918
static boolean      VkeyCtl_MapMYANMRANumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp);  // add by pengyuangui 20120918


/*���ż��̵��¼����ݴ�����*/

//���õ��Ч��
static void      VkeyCtl_HitPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitNumberPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitBigCharPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitLittleCharPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitSymbolPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitNumberLittleCharPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitNumberBigCharPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitDialNumberPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitCalcNumberPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitUserKeyPad(VkeyCtl* pme, int x, int y,boolean sign);
static void      VkeyCtl_HitZhuyinCharPad(VkeyCtl* pme, int x, int y,boolean sign);


//��Ҫ�Ĳ��亯��
static void      Virtualkey_keypadtimer(void *pUser);
static boolean   IsInRect(int x,int y,AEERect *prc);
/*=================================================================================*/
//---------------------------------------------------------------------
// Global Constant Definitions
//---------------------------------------------------------------------
static const VTBL(IVkeyCtl) gQVkeyCtlFuncs = 
{
    IVkeyCtl_AddRef,
    IVkeyCtl_Release,  
   
    IVkeyCtl_HandleEvent,
    IVkeyCtl_Redraw,
    IVkeyCtl_SetActive,
    IVkeyCtl_IsActive,
    IVkeyCtl_SetRect,
    IVkeyCtl_GetRect,
    IVkeyCtl_SetProperties,
    IVkeyCtl_GetProperties,
    IVkeyCtl_Reset,
   
    IVkeyCtl_SetEventHandler,
    IVkeyCtl_SetKeyPad,
    IVkeyCtl_SetColor,
    IVkeyCtl_SetUserKeyPad
};   
/*===========================================================================
Function: VkeyCtl_New

Description:
   Creates a new instance of the IVkey object

Parameters:
    ps: IShell pointer
    cls: class id
    ppObj: IVkey double pointer

Return Value:
   SUCCESS if the object was created successfully

Comments:
   None

Side Effects:
   None

See Also:
   None

===========================================================================*/

int VkeyCtl_New(IShell * pIShell, AEECLSID cls, void ** ppobj)
{
    VkeyCtl *      pme = NULL;
    int            i;
    IBitmap *      pbmDst;
    AEEBitmapInfo  bi;
    
    //���ID�����Ƿ�һ��
    if (cls != AEECLSID_VKEY_CONTROL) 
    {
        return ECLASSNOTSUPPORT;
    }
    
    //���ָ���Ƿ�Ϊ��
    if (!pIShell || !ppobj)
    {
        return(EBADPARM);
    }
          
      
    *ppobj = NULL;
        
    /*lint -e740 -e806 -e826*/
    if ((pme = (VkeyCtl *)AEE_NewClass( (IBaseVtbl *)&gQVkeyCtlFuncs, sizeof(VkeyCtl) )) == NULL)
    {
        return(ENOMEMORY);  
    }
    /*lint e740 e806 e826*/
          
    ISHELL_AddRef(pIShell);  
        
    // initialize object data
    pme->m_pIShell       = pIShell;
    pme->m_nRefs         = 1;
    
    // create IDisplay interface        
    i = ISHELL_CreateInstance(pme->m_pIShell, 
                              AEECLSID_DISPLAY, 
                              (void **)&pme->m_pIDisplay);
    if (!pme->m_pIDisplay)
    {
        (void)IVkeyCtl_Release((IVkeyCtl*)pme);
        return(i);
    }   
    
    // get display metrics
    //��APPLET��GET������Ϣ�������ֿ�
    /*====================================================*/
    pbmDst = IDISPLAY_GetDestination(pme->m_pIDisplay);
    if (!pbmDst)
    {
        (void)IVkeyCtl_Release((IVkeyCtl *)pme);
        return EFAILED;
    }
    i = IBITMAP_GetInfo(pbmDst, &bi, sizeof(bi));
    IBITMAP_Release(pbmDst);
    if (SUCCESS != i) 
    {
        (void)IVkeyCtl_Release((IVkeyCtl *)pme);
        return i;
    }    
    
    SETAEERECT(&(pme->m_rc), 0, 0, bi.cx, bi.cy); 
        /*====================================================*/
        //����Ĭ�ϵı���ɫ�����ɫ
    pme->m_Bground = RGB_NORMAL;
    pme->m_Fillclr = RGB_DOWN;
	pme->m_KeyPadNormal = NULL;
	pme->m_KeyPadDown = NULL;
    *ppobj = (IVkeyCtl*)pme;   
    return(0);             
}
/*=====================================================================

Public Method - Increments the reference count on the date class object.

======================================================================*/
static uint32 IVkeyCtl_AddRef(IVkeyCtl * po)
{
    return(++((VkeyCtl*)po)->m_nRefs);
}
/*=====================================================================

Public Method - Decrements the reference count.  When the count reaches
0, the class is freed.
�����ô˽ӿڵ��Լ��ļ������������Ϊ0 �ͷ�������صĽӿ�
======================================================================*/
static uint32 IVkeyCtl_Release(IVkeyCtl * po)
{
    VkeyCtl * pme = (VkeyCtl*)po;

    if (pme->m_nRefs)
    {
        if (--pme->m_nRefs)
        {
            return(pme->m_nRefs);
        }         
        
        if (pme->m_KeyPadNormal != NULL)
        {
            (void)IIMAGE_Release(pme->m_KeyPadNormal);
        }
        if (pme->m_KeyPadDown != NULL)
        {
            (void)IIMAGE_Release(pme->m_KeyPadDown);
        }
        
        (void)aee_releaseobj((void **)&pme->m_pIDisplay);
        (void)aee_releaseobj((void **)&pme->m_pIShell);

      
        if (pme->m_pFont)
        {
            (void)aee_releaseobj((void **)&pme->m_pFont);
        }      
      
        FREE(pme);
    }
    return(0);
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/*=====================================================================

Public Method - Event handler for the vkey control 
FUNCTION IVkeyCtl_HandleEvent

DESCRIPTION
    This is the EventHandler for this control. All events to this control are 
    handled in this function. 

PROTOTYPE:
    boolean IVkeyCtl_HandleEvent(IVkeyCtl * pi, AEEEvent eCode, uint16 wParam, uint32 dwParam)

PARAMETERS:
    pi: Pointer to the virtual key structure. This structure contains information 
    specific to this control. 

    ecode: Specifies the Event sent to this control

   wParam, dwParam: Event specific data.

DEPENDENCIES
  none

RETURN VALUE
  TRUE: If the control has processed the event
  FALSE: If the control did not process the event

SIDE EFFECTS
  none

======================================================================*/
static boolean   IVkeyCtl_HandleEvent(IVkeyCtl *po,AEEEvent evt, uint16 wp, uint32 dwp )
{
    
    int x, y;                             //�������ϵĵ�����
    static int down_x, down_y;
    static boolean MatchPenEvent = FALSE;
    VkeyCtl* pme = (VkeyCtl*)po;   
   
    /*=================================================
   
    �����������Ľ�������
    1:����״̬
    2������¼�
    3���ڼ�������
    =================================================*/
    if (!pme->m_bActive)
    {
        return(FALSE);     // did not handle the event
    }
    switch ( evt )
    {
        case EVT_KEY:
            switch ( wp )
            {
                case AVK_UP:
                    (void) AEE_Event(EVT_CTL_TAB, 0, dwp);
                    return TRUE;
                
                case AVK_DOWN:
                    (void) AEE_Event(EVT_CTL_TAB, 1, dwp);
                    return TRUE;
                default:
                    break;
            }
            return FALSE;            
        default :
            break;
        
    }
    if ( (evt != EVT_PEN_DOWN ) && (evt != EVT_PEN_UP) )
    {
        return (FALSE);
    }
    
    /*�޸�PEN�ƶ���ɵİ������ܵ���*/ 
    switch (evt)
    {
        //��¼PEN����DOWN������
        case EVT_PEN_DOWN :
        {
            x = AEE_GET_XPOS(dwp);
            y = AEE_GET_YPOS(dwp);
            if (!IsInRect(x, y, &pme->m_rc)) 
            {
                return (FALSE);
            }         
            down_x = x;
            down_y = y;
            VkeyCtl_HitPad(pme, x, y,TRUE);
            MatchPenEvent = TRUE;
            return TRUE;
        }
        
        //�������е�EVT_PEN_MOVE�¼�
        case EVT_PEN_MOVE:
        {
            if (MatchPenEvent == FALSE)
            {
                return FALSE;
            }
            /*��move�����в���Ҫ��MatchPenEvent�¼���λ*/
            return TRUE;
        }
        
        /*�������ʲ������͸�λ�������¼�*/
        case EVT_PEN_UP:
        {
        	int       pen_x, pen_y;
        	AEEEvent  upevt;
        	uint16    upwp;
        	
        	pen_x = AEE_GET_XPOS(dwp);
            pen_y = AEE_GET_YPOS(dwp);
            if (MatchPenEvent == FALSE)
            {
                return FALSE;//�����ƥ��Ͳ�����            
            }
            MatchPenEvent = FALSE;
            
            VkeyCtl_HitPad(pme, down_x, down_y,FALSE);
            
            //���Ӱ�䲻�ɹ��ͷ��ز��ٴ���
            if ((FALSE == VkeyCtl_MapParam(pme, down_x, down_y, &evt, &wp))
            || (FALSE == VkeyCtl_MapParam(pme, pen_x, pen_y, &upevt, &upwp))
            || (evt != upevt) || (wp != upwp))
            {
                return TRUE; //���û����õļ����ڣ���û��ȡ��ֵ���ؾͿ�����
            }
           
            //���ע���˻ص����������¼����ݸ��ص�����
            if (pme->m_pfnEvent)
            {        
               (void) VkeyCtl_CallEventHandler(pme,  evt, wp, dwp);
               return TRUE;
            }         
            
            //���û��ע��ص��������ͽ��¼����ݸ���ǰ���APPLET
            (void) AEE_Event(evt, wp, dwp);
            return TRUE;
        }
        
        default:
            break;
            
    }
                         
    return FALSE;
}
/*=====================================================================

IVkeyCtl_Redraw()

Description:
    This function instructs the vkey control object to redraw its contents. 

Prototype:
    boolean IVkeyCtl_Redraw(IVkeyCtl * pIDateCtl)

Parameters:
    pIDateCtl:  Pointer to the IVkeyCtl Interface object.

Return Value:
    TRUE, if the vkey control was redrawn.

    FALSE, if otherwise.

Comments:
    None


See Also:
    None

======================================================================*/
static boolean   IVkeyCtl_Redraw(IVkeyCtl *po)
{   
    VkeyCtl* pme = (VkeyCtl*)po;
   
   
    //Ҫ�ж�active ��״̬
    if (pme->m_bActive)
    {
        IVkeyCtl_SetKeyPad(po,&pme->m_rc,pme->m_dwProps);
        return TRUE;
    }
   
    return FALSE;
}
/*=====================================================================
IVkeyCtl_SetActive()

Description:
    This function is used to make a vkey control object active. Only an active vkey
control object handles the events sent to it. An inactive vkey control object just
ignores the events.

Prototype:
    void IVkeyCtl_SetActive(IVkeyCtl * pIDateCtl, boolean bActive)

Parameters:
    pIDateCtl:  Pointer to the IVkeyCtl Interface object.
    bActive:  Boolean flag that specifies whether to activate (TRUE) or deactivate
                (FALSE) the date control

Return Value:
    None

Comments:
    None
======================================================================*/
static void IVkeyCtl_SetActive(IVkeyCtl *po,boolean bActive)
{
    VkeyCtl* pme=(VkeyCtl*)po;
 
    //�����TRUE�ͼ���  
    if (bActive == TRUE)
    {
        pme->m_bActive = TRUE;
        IVkeyCtl_SetKeyPad(po, &pme->m_rc, pme->m_dwProps);
    }
    //�����FALSE�͸�λ
    else if (bActive == FALSE)
    {
        IVkeyCtl_Reset(po);
    }
    return ;
}
/*=====================================================================

IVkeyCtl_IsActive()

Description:
    This function returns whether the vkey control object is active or not. The
active state is indicated by a return value of TRUE whereas the inactive state is
indicated by a return value of FALSE.

Prototype:
    boolean IVkeyCtl_IsActive(IVkeyCtl * pIDateCtl)

Parameters:
    pIDateCtl:  Pointer to the IVkeyCtl Interface object.

Return Value:
    TRUE, if the vkey control is active.

    FALSE, if otherwise.

Comments:
    None
 
======================================================================*/
static boolean   IVkeyCtl_IsActive(IVkeyCtl *po)
{
    VkeyCtl* pme = (VkeyCtl*)po;
    return (pme->m_bActive);

}
/*=====================================================================
IVkeyCtl_SetRect()

Description:
    This function can be used to set the specified rectangle as the bounding rectangle
of the vkey control object. 

Prototype:
    void IVkeyCtl_SetRect(IVkeyCtl * pIDateCtl, const AEERect * prc)

Parameters:
    pIDateCtl:  Pointer to the IVkeyCtl Interface object.
    prc:  Bounding rectangle for the vkey control object.

Return Value:
    None

======================================================================*/
static void      IVkeyCtl_SetRect(IVkeyCtl *po,const AEERect * prc)
{
    VkeyCtl* pme = (VkeyCtl*)po;
    if (prc)
    {
        pme->m_rc=*prc;
    }
    return ;
}
/*=====================================================================
IVkeyCtl_GetRect()

Description:
    This function fills the given pointer to an AEERect structure with the coordinates
of the current bounding rectangle of the vkey control object. This is particularly
useful after a control is created to determine its optimal/default size and position.

Prototype:
    void IVkeyCtl_GetRect(IVkeyCtl * pIDateCtl, AEERect * prc)

Parameters:
    pIDateCtl:  Pointer to the IDateCtl Interface object.
    prc:  Rectangle to be filled with the coordinates of the date control object.

Return Value:
    None

Comments:
    None

======================================================================*/
static void      IVkeyCtl_GetRect(IVkeyCtl *po, AEERect * prc)
{
    VkeyCtl* pme = (VkeyCtl*)po;
    if (prc)
    {
        *prc = pme->m_rc;
    }
    return ;
}
/*=====================================================================

IDATECTL_SetProperties()

Description:
    This function sets vkey control-specific properties or flags. 
Prototype:
    void IVkeyCtl_SetProperties(IVkeyCtl * pIDateCtl, uint32 dwProps)

Parameters:
    pIDateCtl:  Pointer to the IVkeyCtl Interface object.
    dwProps:  32-bit set of flags/properties.

Return Value:
    None
======================================================================*/
static void  IVkeyCtl_SetProperties(IVkeyCtl *po,uint32 dwProps)
{
    VkeyCtl* pme = (VkeyCtl*)po;
    pme->m_dwProps = dwProps;
    return ;
}

/*=====================================================================
IVkeyCtl_GetProperties()

Description:
    This function returns the vkey control-specific properties or flags. 

Prototype:
    uint32 IVkeyCtl_GetProperties(IVkeyCtl * pIDateCtl)

Parameters:
    pIDateCtl:  Pointer to the IVkeyCtl Interface object.

Return Value:
    0 (zero) under all conditions.
======================================================================*/
static uint32    IVkeyCtl_GetProperties(IVkeyCtl *po)
{
    VkeyCtl* pme = (VkeyCtl*)po;
    return (pme->m_dwProps);

}
/*=====================================================================

IVkeyCtl_Reset()

Description:
    This function instructs the vkey control to reset (free/delete) its contents
and to immediately leave active/focus mode.

Prototype:
    void IDATECTL_Reset(IDateCtl * pIDateCtl)

Parameters:
    pIDateCtl:  Pointer to the IVkeyCtl Interface object.

Return Value:
    None

Comments:
    This function makes the control inactive. An inactive control doesn't handle
the events sent to it.

======================================================================*/
static void IVkeyCtl_Reset(IVkeyCtl *po)
{
    VkeyCtl* pme=(VkeyCtl*)po;
      
    pme->m_bActive = FALSE;
    return ;
}
/*=====================================================================
IVkeyCtl_SetEventHandler

Description:
�����¼������� 
Prototype:
    static void IVkeyCtl_SetEventHandler(IVkeyCtl * po, PFNAEEEVENT pfn, void * pUser)
Parameters:
IVkeyCtl * po:   Pointer to the IVkeyCtl Interface object.
PFNAEEEVENT pfn: �ص�����ָ��
void * pUser��   �ص�����Ҫ������
Return Value:
    None
======================================================================*/
static void IVkeyCtl_SetEventHandler(IVkeyCtl * po, PFNAEEEVENT pfn, void * pUser)
{
    VkeyCtl* pme = (VkeyCtl*)po;
    pme->m_pfnEvent      = pfn;
    pme->m_pUserEvent    = pUser;
    return ;
}
/*=====================================================================

IVkeyCtl_SetKeyPad()

Description:
    ���ü��̵����࣬��С����

Prototype:
    void  IVkeyCtl_SetKeyPad(IVkeyCtl * po,AEERect * prc,uint32  m_dwProps)

Parameters:
    po:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
    m_dwProps�����̵���������

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�
======================================================================*/
static void      IVkeyCtl_SetKeyPad(IVkeyCtl * po,AEERect * prc,uint32  m_dwProps)
{
    VkeyCtl* pme = (VkeyCtl*)po;
    pme->m_dwProps = m_dwProps;
    
    if(!prc)
    {
        return;
    }   
    pme->m_rc = *prc;

    if (!pme->m_bActive)
       return;
    
    switch (m_dwProps)
    {
        case AEE_VKEY_NUMBER:
            VkeyCtl_SetNumberPad(pme,prc);
            break;
        case AEE_VKEY_BIGCHAR:
            VkeyCtl_SetBigCharPad(pme,prc);
            break;
        case AEE_VKEY_LITTLECHAR:
            DBGPRINTF("AEE_VKEY_LITTLECHAR-----------1");
            VkeyCtl_SetLittleCharPad(pme,prc);
            break;
        case AEE_VKEY_SYMBOL:
            VkeyCtl_SetSymbolPad(pme,prc);
            break;
        case AEE_VKEY_NUMBER_LITTLECHAR:
            DBGPRINTF("AEE_VKEY_NUMBER_LITTLECHAR    1"); 
            VkeyCtl_SetNumberLittleCharPad(pme,prc);
            break;
        case AEE_VKEY_NUMBER_BIGCHAR:
            VkeyCtl_SetNumberBigCharPad(pme,prc);
            break;
        case AEE_VKEY_DIALNUMBER:
            VkeyCtl_SetDialNumberPad(pme, prc);
            break;
        case AEE_VKEY_CALCULATOR:
            VkeyCtl_SetCalculatorPad(pme, prc);
            break;
        case AEE_VKEY_ZHUYIN:
            VkeyCtl_SetZhuyinCharPad(pme, prc);
            break;
        case AEE_VKEY_USERKEYPAD:
            VkeyCtl_SetUserKeyPad(pme, prc);
            break;
        case AEE_VKEY_THAINUMBER:
            VkeyCtl_SetTHAIPad(pme,prc);
            break;
        case AEE_VKEY_MYANMRANUMBER:
            VkeyCtl_SetMYNAMMAPad(pme,prc);  
            break;
        default:
            break;
    }
    
    return;
}
/*=====================================================================

IVkeyCtl_SetKeyPad()

Description:
    �������ּ���

Prototype:
    void  VkeyCtl_SetNumberPad(VkeyCtl * pme,AEERect * prc)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
   

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�

======================================================================*/
static void      VkeyCtl_SetNumberPad(VkeyCtl * pme,AEERect * prc)
{
    int   i, j, count = 0;
    
    if(pme == NULL)
    {
        return;
    }
    
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    if ((pme->m_rc.dx < OTHER_KEYPAD_MINWIDTH)
       || (pme->m_rc.dy < OTHER_KEYPAD_MINHEIGHT))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
    
    if (pme->m_KeyPadNormal != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadNormal);
    }
    if (pme->m_KeyPadDown != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadDown);
    }
    pme->m_KeyPadNormal = ISHELL_LoadResImage(pme->m_pIShell,
                                           AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_DIGITUNPRESS);
    pme->m_KeyPadDown = ISHELL_LoadResImage(pme->m_pIShell,
                                        AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_DIGITPRESS);
    if ((pme->m_KeyPadNormal == NULL)
       || (pme->m_KeyPadDown == NULL))
    {
        return;
    }
    
    IIMAGE_Draw(pme->m_KeyPadNormal, pme->m_rc.x, pme->m_rc.y);

    for(i = 0; i < VNumberKeyPad.row; i++)
    {
        for(j = 0; j < VNumberKeyPad.coloum; j++)
        {
            pme->GridRect[count].x = (int16)(pme->m_rc.x + 3*j + 45 * j);
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 4):(pme->m_rc.y+8*i + 44 * i));
            pme->GridRect[count].dx = 45;
            pme->GridRect[count].dy = 44;//((i == 0)?18:19);
            count++;
        }
     }
    
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
}
#if 0
{
    int i,j,count;
    int rc_x,rc_y,rc_dx,rc_dy;
    boolean sign = FALSE;//��ʾ͹��
    AEERect temp_rc;
        
    count = 0;
    rc_x = prc->x;
    rc_y = prc->y;
    rc_dx = prc->dx/5;
    rc_dy = prc->dy/2;
    if ((rc_dx < FontWidth) || (rc_dy < FontHeight))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }      
   
    /*�������е����ּ���*/
    for (i = 0; i < VNumberKeyPad.row; i++)
    {
        for(j = 0; j < VNumberKeyPad.coloum; j++,count++)
        {
            SETAEERECT(&temp_rc, rc_x, rc_y, rc_dx-1,rc_dy-1);
            SETAEERECT(&pme->GridRect[count], rc_x, rc_y, rc_dx,rc_dy);
            (void)IDISPLAY_FrameButton ( pme->m_pIDisplay, 
                                         &temp_rc, 
                                         sign, 
                                         pme->m_Bground );
                                                 
            (void)IDISPLAY_DrawText ( pme->m_pIDisplay, 
                                      AEE_FONT_NORMAL, 
                                      VNumberKeyItem[count].name, 
                                      1, 
                                      0, 
                                      0, 
                                      &pme->GridRect[count], 
                                      IDF_ALIGN_MIDDLE|IDF_ALIGN_CENTER |IDF_TEXT_TRANSPARENT 
                                     ) ;

                                                                    
            rc_x = rc_x + rc_dx;
        }
        rc_x = prc->x;
        rc_y = rc_y + rc_dy;
    
    }
   
    IDISPLAY_Update(pme->m_pIDisplay) ;
   
    return;
    
}
#endif
/*=====================================================================
VkeyCtl_SetLittleCharPad()

Description:
    ����Сд��ĸ���� 

Prototype:
    void  VkeyCtl_SetLittleCharPad(VkeyCtl * pme,AEERect * prc)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
   

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�

======================================================================*/
static void      VkeyCtl_SetLittleCharPad(VkeyCtl * pme,AEERect * prc)
{
    int   i, j, count = 0;
    DBGPRINTF("VkeyCtl_SetLittleCharPad------");
    if(pme == NULL)
    {
        return;
    }
    DBGPRINTF("VkeyCtl_SetLittleCharPad------2");
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    
    if ((pme->m_rc.dx < OTHER_KEYPAD_MINWIDTH)
       || (pme->m_rc.dy < OTHER_KEYPAD_MINHEIGHT))
    {
        DBGPRINTF("VkeyCtl_SetLittleCharPad------3");
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
    
    if (pme->m_KeyPadNormal != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadNormal);
    }
    if (pme->m_KeyPadDown != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadDown);
    }
    pme->m_KeyPadNormal = ISHELL_LoadResImage(pme->m_pIShell,
                                           AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_PINYINUNPRESS);
    pme->m_KeyPadDown = ISHELL_LoadResImage(pme->m_pIShell,
                                         AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_PINYINPRESS);
    DBGPRINTF("VkeyCtl_SetLittleCharPad------4");
	if ((pme->m_KeyPadNormal == NULL)
       || (pme->m_KeyPadDown == NULL))
    {
        return;
    }
    DBGPRINTF("VkeyCtl_SetLittleCharPad------5");
    IIMAGE_Draw(pme->m_KeyPadNormal, pme->m_rc.x, pme->m_rc.y);
    
    for(i = 1; i <= VLCharKeyPad.row; i++)
    {
        for(j = 0; j < VLCharKeyPad.coloum; j++)
        {
            if(count==31)
            {
            pme->GridRect[count].x = (int16)(pme->m_rc.x+ 5*(j+1)+ 29 * j-2)+10;
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 1):(pme->m_rc.y +2+ 25 * i));
            pme->GridRect[count].dx = 50;
            pme->GridRect[count].dy = 25;
            }
            else if(count==30)
            {
            pme->GridRect[count].x = (int16)(pme->m_rc.x+ 5*(j+1)+ 29 * j-2);
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 1):(pme->m_rc.y +2+ 25 * i));
            pme->GridRect[count].dx = 50;
            pme->GridRect[count].dy = 25;
            }
            else
            {
            pme->GridRect[count].x = (int16)(pme->m_rc.x+ 5*(j+1)+ 29 * j-2);
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 1):(pme->m_rc.y +2+ 25 * i));
            pme->GridRect[count].dx = 29;
            pme->GridRect[count].dy = 25;
            }
            count++;
            DBGPRINTF("VkeyCtl_SetLittleCharPad-----pme->GridRect[count].x=%d--pme->GridRect[count].y",pme->GridRect[count].x,pme->GridRect[count].y);
            DBGPRINTF("VkeyCtl_SetLittleCharPad-----pme->GridRect[count].dx=%d--pme->GridRect[count].dy",pme->GridRect[count].dx,pme->GridRect[count].dy);
        }
    }
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
}
/*=====================================================================
VkeyCtl_SetBigCharPad()

Description:
    ���ô�д��ĸ����

Prototype:
    void  VkeyCtl_SetBigCharPad(VkeyCtl * pme,AEERect * prc)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
   

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�

======================================================================*/
static void  VkeyCtl_SetBigCharPad(VkeyCtl * pme,AEERect * prc)
{
    int i,j,count;
    int rc_x,rc_y,rc_dx,rc_dy;  
    boolean sign = FALSE;//͹���ʾ
    AEERect temp_rc;
        
    count = 0;
    rc_x = prc->x;
    rc_y = prc->y;
    rc_dx = prc->dx/10;
    rc_dy = prc->dy/3;
    if ((rc_dx < FontWidth) || (rc_dy < FontHeight))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
      
   
    /*����ʮ�е���ĸ����*/
    for (i = 0; i < VBCharKeyPad.row; i++)
    {
        for(j = 0; j < VBCharKeyPad.coloum; j++,count++)
        {
            SETAEERECT(&pme->GridRect[count], rc_x, rc_y, rc_dx,rc_dy);
            SETAEERECT(&temp_rc, rc_x, rc_y, rc_dx-1,rc_dy-1);
            (void)IDISPLAY_FrameButton ( pme->m_pIDisplay, 
                                         &temp_rc, 
                                         sign, 
                                         pme->m_Bground ); 
            (void)IDISPLAY_DrawText ( pme->m_pIDisplay, 
                                      AEE_FONT_NORMAL, 
                                      VBCharKeyItem[count].name, 
                                      1, 
                                      0, 
                                      0, 
                                      &pme->GridRect[count], 
                                      IDF_ALIGN_MIDDLE|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT  
                                    ) ;
            rc_x = rc_x + rc_dx;
        }
        rc_x = prc->x;
        rc_y = rc_y + rc_dy;
    
    }
    IDISPLAY_Update(pme->m_pIDisplay) ;
    return;
}
/*=====================================================================
VkeyCtl_SetSymbolPad()

Description:
    ���÷��ż���

Prototype:
    void  VkeyCtl_SetSymbolPad(VkeyCtl * pme,AEERect * prc)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
   

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�

======================================================================*/
static void      VkeyCtl_SetSymbolPad(VkeyCtl * pme,AEERect * prc)
{
    int   i, j, count = 0;
    
    if(pme == NULL)
    {
        return;
    }
    
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    if ((pme->m_rc.dx < OTHER_KEYPAD_MINWIDTH)
       || (pme->m_rc.dy < OTHER_KEYPAD_MINHEIGHT))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
    
    if (pme->m_KeyPadNormal != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadNormal);
    }
    if (pme->m_KeyPadDown != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadDown);
    }
    pme->m_KeyPadNormal = ISHELL_LoadResImage(pme->m_pIShell,
                                           AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_SYMBOLUNPRESS);
    pme->m_KeyPadDown = ISHELL_LoadResImage(pme->m_pIShell,
                                        AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_SYMBOLPRESS);
    if ((pme->m_KeyPadNormal == NULL)
       || (pme->m_KeyPadDown == NULL))
    {
        return;
    }
    
    IIMAGE_Draw(pme->m_KeyPadNormal, pme->m_rc.x, pme->m_rc.y);
    DBGPRINTF("VkeyCtl_SetSymbolPad-----pme->m_rc.x=%d----pme->m_rc.y=%d",pme->m_rc.x,pme->m_rc.y);

    for(i = 0; i < VNumLCharKeyPad.row; i++)
    {
        for(j = 0; j < VNumLCharKeyPad.coloum; j++)
        {
            pme->GridRect[count].x = (int16)(pme->m_rc.x + 5*(j+1) + 29 * j-2);
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 1):(pme->m_rc.y + 25 * i));
            pme->GridRect[count].dx = 29;
            pme->GridRect[count].dy = 25;//((i == 0)?18:19);
            DBGPRINTF("VkeyCtl_SetSymbolPad-----pme->GridRect[count].x=%d----pme->GridRect[count].y=%d",pme->GridRect[count].x,pme->GridRect[count].y); 
            DBGPRINTF("VkeyCtl_SetSymbolPad-----pme->GridRect[count].dx=%d----pme->GridRect[count].dy=%d",pme->GridRect[count].dx,pme->GridRect[count].dy); 
            count++;
        }
     }
    
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
}
/*=====================================================================
VkeyCtl_SetNumberLittleCharPad()

Description:
    ����������Сд��ĸ���̼���

Prototype:
    void  VkeyCtl_SetNumberLittleCharPad(VkeyCtl * pme,AEERect * prc)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
   

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�

======================================================================*/
static void      VkeyCtl_SetNumberLittleCharPad(VkeyCtl * pme,AEERect * prc)
{
    int   i, j, count = 0;
    
    if(pme == NULL)
    {
        return;
    }
    
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    if ((pme->m_rc.dx < OTHER_KEYPAD_MINWIDTH)
       || (pme->m_rc.dy < OTHER_KEYPAD_MINHEIGHT))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
    
    if (pme->m_KeyPadNormal != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadNormal);
    }
    if (pme->m_KeyPadDown != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadDown);
    }
    pme->m_KeyPadNormal = ISHELL_LoadResImage(pme->m_pIShell,
                                           AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_LOWERENUNGPRESS);
    pme->m_KeyPadDown = ISHELL_LoadResImage(pme->m_pIShell,
                                         AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_LOWERENGPRESS);
    if ((pme->m_KeyPadNormal == NULL)
       || (pme->m_KeyPadDown == NULL))
    {
        DBGPRINTF("pme->m_KeyPadNormal   -is NULL"); 
        return;
    }
    
    IIMAGE_Draw(pme->m_KeyPadNormal, pme->m_rc.x, pme->m_rc.y);
    
    for(i = 0; i < VNumLCharKeyPad.row; i++)
    {
        for(j = 0; j < VNumLCharKeyPad.coloum; j++)
        {
            pme->GridRect[count].x = (int16)(pme->m_rc.x+ 5*(j+1)+ 29 * j-2);
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 1):(pme->m_rc.y +2+ 25 * i));
            pme->GridRect[count].dx = 29;
            pme->GridRect[count].dy = 25;//((i == 0)?18:19);
            count++;
        }
    }
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
}
/*=====================================================================
VkeyCtl_SetNumberBigCharPad()

Description:
    �����������д��ĸ���̼���

Prototype:
    void  VkeyCtl_SetNumberBigCharPad(VkeyCtl * pme,AEERect * prc)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
   

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�

======================================================================*/
static void      VkeyCtl_SetNumberBigCharPad(VkeyCtl * pme,AEERect * prc)
{
    int   i, j, count = 0;
    
    if(pme == NULL)
    {
        return;
    }
    
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    if ((pme->m_rc.dx < OTHER_KEYPAD_MINWIDTH)
       || (pme->m_rc.dy < OTHER_KEYPAD_MINHEIGHT))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
    
    if (pme->m_KeyPadNormal != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadNormal);
    }
    if (pme->m_KeyPadDown != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadDown);
    }
    pme->m_KeyPadNormal = ISHELL_LoadResImage(pme->m_pIShell,
                                           AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_UPPERENGUNPRESS);
    pme->m_KeyPadDown = ISHELL_LoadResImage(pme->m_pIShell,
                                        AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_UPPERENGPRESS);
    if ((pme->m_KeyPadNormal == NULL)
       || (pme->m_KeyPadDown == NULL))
    {
        return;
    }
    
    IIMAGE_Draw(pme->m_KeyPadNormal, pme->m_rc.x, pme->m_rc.y);
    
    for(i = 0; i < VNumBCharKeyPad.row; i++)
    {
        for(j = 0; j < VNumBCharKeyPad.coloum; j++)
        {
            pme->GridRect[count].x = (int16)(pme->m_rc.x + 5*(j+1) + 29 * j-2);
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 1):(pme->m_rc.y + 25 * i));
            pme->GridRect[count].dx = 29;
            pme->GridRect[count].dy = 25;//((i == 0)?18:19);
            count++;
        }
    }
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
}
/*=====================================================================
VkeyCtl_SeZhuyinCharPad()

Description:
    ����ע������

Prototype:
    void  VkeyCtl_SetZhuyinCharPad(VkeyCtl * pme,AEERect * prc)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
   

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�

======================================================================*/
static void      VkeyCtl_SetZhuyinCharPad(VkeyCtl * pme,AEERect * prc)
{
    int   i, j, count = 0;
    
    if(pme == NULL)
    {
        return;
    }
    
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    
    if ((pme->m_rc.dx < OTHER_KEYPAD_MINWIDTH)
       || (pme->m_rc.dy < OTHER_KEYPAD_MINHEIGHT))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
    
    if (pme->m_KeyPadNormal != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadNormal);
    }
    if (pme->m_KeyPadDown != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadDown);
    }
    pme->m_KeyPadNormal = ISHELL_LoadImage(pme->m_pIShell,
                                           AEEFS_SHAREDVK_DIR"zhuyinkeypad.bmp");
    pme->m_KeyPadDown = ISHELL_LoadImage(pme->m_pIShell,
                                         AEEFS_SHAREDVK_DIR"zhuyinkeypaddown.bmp");
    if ((pme->m_KeyPadNormal == NULL)
       || (pme->m_KeyPadDown == NULL))
    {
        return;
    }
    
    IIMAGE_Draw(pme->m_KeyPadNormal, pme->m_rc.x, pme->m_rc.y);
    
    for(i = 0; i < VZhuyinKeyPad.row; i++)
    {
        for(j = 0; j < VZhuyinKeyPad.coloum; j++)
        {
            pme->GridRect[count].x = (int16)(pme->m_rc.x + 6 + 15 * j);
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 1):(pme->m_rc.y + 21 * i));
            pme->GridRect[count].dx = 15;
            pme->GridRect[count].dy = ((i == 0)?18:19);
            count++;
        }
    }
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
}

/*=====================================================================

IVkeyCtl_SetKeyPad()

Description:
    �������ּ���

Prototype:
    void  VkeyCtl_SetTHAIPad(VkeyCtl * pme,AEERect * prc)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
   

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�

======================================================================*/
static void      VkeyCtl_SetTHAIPad(VkeyCtl * pme,AEERect * prc)
{
    int   i, j, count = 0;
    
    if(pme == NULL)
    {
        return;
    }
    
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    if ((pme->m_rc.dx < OTHER_KEYPAD_MINWIDTH)
       || (pme->m_rc.dy < OTHER_KEYPAD_MINHEIGHT))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
    
    if (pme->m_KeyPadNormal != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadNormal);
    }
    if (pme->m_KeyPadDown != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadDown);
    }
    pme->m_KeyPadNormal = ISHELL_LoadResImage(pme->m_pIShell,
                                           AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_THAIUNPRESS);
    pme->m_KeyPadDown = ISHELL_LoadResImage(pme->m_pIShell,
                                        AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_THAIPRESS);
    if ((pme->m_KeyPadNormal == NULL)
       || (pme->m_KeyPadDown == NULL))
    {
        return;
    }
    
    IIMAGE_Draw(pme->m_KeyPadNormal, pme->m_rc.x, pme->m_rc.y);

    for(i = 0; i < VNumberKeyPad.row; i++)
    {
        for(j = 0; j < VNumberKeyPad.coloum; j++)
        {
            pme->GridRect[count].x = (int16)(pme->m_rc.x + 3*j + 45 * j);
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 4):(pme->m_rc.y+8*i + 44 * i));
            pme->GridRect[count].dx = 45;
            pme->GridRect[count].dy = 44;//((i == 0)?18:19);
            count++;
        }
     }
    
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
}

/*=====================================================================

IVkeyCtl_SetKeyPad()

Description:
    �������ּ���

Prototype:
    void  VkeyCtl_SetMYNAMMAPad(VkeyCtl * pme,AEERect * prc)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    prc: ������Ҫռ�еľ��δ�С
   

Return Value:
    None
Comments:
    ������õľ���̫С�����������ã�

======================================================================*/
static void      VkeyCtl_SetMYNAMMAPad(VkeyCtl * pme,AEERect * prc)
{
    int   i, j, count = 0;
    
    if(pme == NULL)
    {
        return;
    }
    
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    if ((pme->m_rc.dx < OTHER_KEYPAD_MINWIDTH)
       || (pme->m_rc.dy < OTHER_KEYPAD_MINHEIGHT))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
    
    if (pme->m_KeyPadNormal != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadNormal);
    }
    if (pme->m_KeyPadDown != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadDown);
    }
    pme->m_KeyPadNormal = ISHELL_LoadResImage(pme->m_pIShell,
                                           AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_MYNAMMAUNPRESS);
    pme->m_KeyPadDown = ISHELL_LoadResImage(pme->m_pIShell,
                                        AEE_APPSCOMMONRES_IMAGESFILE,IDB_MODE_IME_MYNAMMAPRESS);
    if ((pme->m_KeyPadNormal == NULL)
       || (pme->m_KeyPadDown == NULL))
    {
        return;
    }
    
    IIMAGE_Draw(pme->m_KeyPadNormal, pme->m_rc.x, pme->m_rc.y);

    for(i = 0; i < VNumberKeyPad.row; i++)
    {
        for(j = 0; j < VNumberKeyPad.coloum; j++)
        {
            pme->GridRect[count].x = (int16)(pme->m_rc.x + 3*j + 45 * j);
            pme->GridRect[count].y = (int16)((i == 0)?(pme->m_rc.y + 4):(pme->m_rc.y+8*i + 44 * i));
            pme->GridRect[count].dx = 45;
            pme->GridRect[count].dy = 44;//((i == 0)?18:19);
            count++;
        }
     }
    
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
}

/*=====================================================================
VkeyCtl_CallEventHandler()

Description:
    �����¼�������

Prototype:
    void  VkeyCtl_CallEventHandler(VkeyCtl * pme, AEEEvent evt, uint16 wp, uint32 dwp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    evt: ָ���Ļص� Event �¼�
    wp,dwp:Event specific data.
    

Return Value:
    None

======================================================================*/
static boolean VkeyCtl_CallEventHandler(VkeyCtl * pme, AEEEvent evt, uint16 wp, uint32 dwp)
{
    boolean     bRet;

    bRet = pme->m_pfnEvent(pme->m_pUserEvent, evt, wp, dwp);
   
    return(bRet);
}

/*=====================================================================
VkeyCtl_MapParam()

Description:
    �������¼��¸��ֲ�ͬ����ӳ��ֵ

Prototype:
    boolean VkeyCtl_MapParam(VkeyCtl * pme,int cx,int cy,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    cx,cy: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

======================================================================*/

static boolean VkeyCtl_MapParam(VkeyCtl * pme,int cx,int cy,AEEEvent *evt,uint16 *wp)
{
    boolean ret;  //���ؽ��
    
    if( !IsInRect(cx,cy,&pme->m_rc) )
    {
       return FALSE;
    }
      
  
    switch (pme->m_dwProps)
    {
        case AEE_VKEY_NUMBER:
        {
            ret = VkeyCtl_MapNumber(pme,cx,cy,evt,wp);
            return ret;
        }
           
        case AEE_VKEY_BIGCHAR:
        {
            ret = VkeyCtl_MapBigChar(pme,cx,cy,evt,wp);
            return ret;
        }
           
        case AEE_VKEY_LITTLECHAR:
        {
            DBGPRINTF("AEE_VKEY_LITTLECHAR-----------1");
            ret = VkeyCtl_MapLittleChar(pme,cx,cy,evt,wp);
            return ret;
        }
           
        case AEE_VKEY_SYMBOL:
        {
            ret = VkeyCtl_MapSymbol(pme,cx,cy,evt,wp);
            return ret;
        }
           
        case AEE_VKEY_NUMBER_LITTLECHAR:
        {
            DBGPRINTF("AEE_VKEY_NUMBER_LITTLECHAR    1"); 
            ret = VkeyCtl_MapNumberLittleChar(pme,cx,cy,evt,wp);
            return ret;
        }
           
        case AEE_VKEY_NUMBER_BIGCHAR:
        {
            ret = VkeyCtl_MapNumberBigChar(pme,cx,cy,evt,wp);
            return ret;
        }
           
        case AEE_VKEY_USERKEYPAD:
        {
            ret = VkeyCtl_MapUserKeyPad(pme,cx,cy,evt,wp);
            return ret;
        }
           
        case AEE_VKEY_DIALNUMBER:
        {
            ret = VkeyCtl_MapDialNumber(pme,cx,cy,evt,wp);
            return ret;
        }
        
        case AEE_VKEY_CALCULATOR:
        {
            ret = VkeyCtl_MapCalcNumber(pme,cx,cy,evt,wp);
            return ret;
        }

        case AEE_VKEY_ZHUYIN:
        {
            ret = VkeyCtl_MapZhuyinChar(pme,cx,cy,evt,wp);
            return ret;
        }

        case AEE_VKEY_THAINUMBER:
        {
            ret = VkeyCtl_MapTHAINumber(pme,cx,cy,evt,wp);
            return ret;
        }

        case AEE_VKEY_MYANMRANUMBER:
        {
            ret = VkeyCtl_MapMYANMRANumber(pme,cx,cy,evt,wp);
            return ret;
        }
        default:
           break;
    }
  
    return FALSE;
}
/*=====================================================================
help function 
�ж�һ���������Ƿ���һ��������
======================================================================*/
static boolean IsInRect(int x,int y,AEERect *prc)
{
    DBGPRINTF("IsInRect-----x=%d----prc->x=%d---prc->maxx=%d",x,prc->x,prc->x+prc->dx-1);
    DBGPRINTF("IsInRect-----y=%d----prc->y=%d---prc->maxy=%d",y,prc->y,prc->y+prc->dy-1);
    if (prc->x > x || x > (prc->x+prc->dx-1))
    {
        return FALSE;
    }
       
       
    if (prc->y > y || y > (prc->y+prc->dy-1))
    {
        return FALSE;
    }
       
       
    return TRUE;
}

/*=====================================================================
VkeyCtl_MapNumber()

Description:
    ���������ּ��̵�ӳ���¼�

Prototype:
    boolean VkeyCtl_MapNumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

======================================================================*/
static boolean VkeyCtl_MapNumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i,j,count;
    //boolean outloop = FALSE;  //�Ѿ�����������
    
    count = 0;
   
    /* 2*5�ļ��� */
    for (i = 0; i < VNumberKeyPad.row; i++)
    {
        for (j = 0; j < VNumberKeyPad.coloum; j++,count++)
        {
            if( IsInRect(x,y,&pme->GridRect[count]) )
            {
                if(count != 4)
                {
                 *wp = VNumberKeyItem[count].wp;
                 *evt = VNumberKeyItem[count].evt;
                 return TRUE;
                }
            }
            
        }//end   for (j = 0;j < 5;j++,count++) 
            
    }//end for (i = 0;i < 2;i++)   
    return FALSE;

}
/*=====================================================================
VkeyCtl_MapBigChar()

Description:
    ��������д��ĸ���̵�ӳ���¼�

Prototype:
    boolean VkeyCtl_MapBigChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

======================================================================*/
static boolean VkeyCtl_MapBigChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i,j,count;
    //boolean outloop = FALSE;              
    count = 0;
   
    /*3*10�ļ���*/
    for (i = 0; i < VBCharKeyPad.row; i++)
    {
        for (j = 0; j < VBCharKeyPad.coloum; j++,count++)
        {
            if ( IsInRect(x,y,&pme->GridRect[count]) )
            {
                *wp = VBCharKeyItem[count].wp;
                *evt = VBCharKeyItem[count].evt;
                return TRUE;
            }
            
        } 
        
    }   
    return FALSE;
}
/*=====================================================================
VkeyCtl_MapLittleChar()

Description:
    ������Сд��ĸ���̵�ӳ���¼�

Prototype:
    boolean VkeyCtl_MapLittleChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

======================================================================*/
static boolean VkeyCtl_MapLittleChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i,j,count;
    //boolean outloop = FALSE;      
    count = 0;
    DBGPRINTF("VkeyCtl_MapLittleChar-----1");
    /* 3*10�ļ��� */
    for (i = 1; i <= VLCharKeyPad.row; i++)
    {
        for (j = 0; j < VLCharKeyPad.coloum; j++,count++)
        {
            if ( IsInRect(x,y,&pme->GridRect[count]) )
            {
                DBGPRINTF("VkeyCtl_MapLittleChar-----2");
                *wp = VLCharKeyItem[count].wp;
                *evt = VLCharKeyItem[count].evt;
                return TRUE;
            }           
        }   
    }
    return FALSE;
}
/*=====================================================================
VkeyCtl_MapSymbol()

Description:
    ���������ż��̵�ӳ���¼�

Prototype:
    boolean VkeyCtl_MapSymbol(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

======================================================================*/
static boolean VkeyCtl_MapSymbol(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i,j,count;
    //boolean outloop = FALSE;
    count = 0;
   
    /* 4*10�ļ��� */
    for (i = 0; i < VSymbolKeyPad.row; i++)
    { 
        for (j = 0; j < VSymbolKeyPad.coloum; j++,count++)
        {
            if ( IsInRect(x,y,&pme->GridRect[count]) )
            {
                if(count < 40)
                {
                *wp = VSymbolKeyItem[count].wp;
                *evt = VSymbolKeyItem[count].evt;
                return TRUE;
                }
            }               
        }
    }
    return FALSE;

}
/////////////////////////////////////////
/*=======================================================================================
VkeyCtl_MapNumberLittleChar()

Description:
    ������������Сд��ĸ����ӳ���¼�

Prototype:
    boolean VkeyCtl_MapNumberLittleChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

=======================================================================================*/
static boolean  VkeyCtl_MapNumberLittleChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i,j,count;
    //boolean outloop = FALSE;
    count = 0;
  
    /* 4*10�ļ��� */
    for (i = 0;i < VNumLCharKeyPad.row;i++)
    { 
        for (j = 0;j < VNumLCharKeyPad.coloum;j++,count++)
        {
            if ( IsInRect(x,y,&pme->GridRect[count]) )
            {
                if(count < 40)
                {
                *wp = VNumLCharKeyItem[count].wp;
                *evt = VNumLCharKeyItem[count].evt;
                return TRUE;
                 }
               
            }               
        }
    }
    return FALSE;
}
/*=======================================================================================
VkeyCtl_MapNumberBigChar()

Description:
    �������������д��ĸ����ӳ���¼�

Prototype:
    boolean VkeyCtl_MapNumberBigChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

=======================================================================================*/
static boolean      VkeyCtl_MapNumberBigChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i,j,count;
    //boolean outloop = FALSE;
    count = 0;
   
    /* 4*10�ļ���*/
    for (i = 0; i < VNumBCharKeyPad.row; i++)
    { 
        for (j = 0; j < VNumBCharKeyPad.coloum; j++,count++)
        {
            if ( IsInRect(x,y,&pme->GridRect[count]) )
            {
                if(count < 40)
                {
                *wp = VNumBCharKeyItem[count].wp;
                *evt = VNumBCharKeyItem[count].evt;
                return TRUE;
                }
            }               
        }
    }
    return FALSE;
}
/*========================================================================
VkeyCtl_MapUserKeyPad()

Description:
    �������û��Լ�������¼�

Prototype:
    boolean VkeyCtl_MapNumberBigChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

========================================================================*/
static boolean VkeyCtl_MapUserKeyPad(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i = (pme->m_keypad).row;
    int j = (pme->m_keypad).coloum;
    int count=0,loop_row,loop_coloum;
    //boolean sign = FALSE;
    
    for (loop_row = 0; loop_row < i; loop_row++)
    {
        for (loop_coloum = 0;loop_coloum < j; loop_coloum++, count++)
        {
            if ( IsInRect(x, y, &pme->UsrGridRc[count]) )
            {
                *evt = ((pme->m_keypad).pItem)->evt;
                *wp = ((pme->m_keypad).pItem)->wp;
                (pme->m_keypad).pItem -= count;
                return TRUE;
            }
            (pme->m_keypad).pItem ++;
        }               
    }        
    return FALSE;
}

/*========================================================================
VkeyCtl_MapDialNumber()

Description:
    ���������Ž�����¼�

Prototype:
    boolean VkeyCtl_MapDialNumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

========================================================================*/
static boolean VkeyCtl_MapDialNumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    //int i = (pme->m_keypad).row;
    //int j = (pme->m_keypad).coloum;
    int count=0,loop_row,loop_coloum;
    //boolean sign = FALSE;
    
    for (loop_row = 0; loop_row < 4; loop_row++)
    {
        for (loop_coloum = 0;loop_coloum < 3; loop_coloum++, count++)
        {
            if ( IsInRect(x, y, &pme->m_dial_rc[count]) )
            {
                *evt = EVT_CHAR;
                *wp = DialNumber[loop_row][loop_coloum];
                return TRUE;
            }           
        }               
    }
    
    return FALSE;
}
/*========================================================================
VkeyCtl_MapCalcNumber()

Description:
    ���������Ž�����¼�

Prototype:
    boolean VkeyCtl_MapCalcNumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

========================================================================*/
static boolean VkeyCtl_MapCalcNumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    //int i = (pme->m_keypad).row;
    //int j = (pme->m_keypad).coloum;
    int count=0,loop_row,loop_coloum;
    //boolean sign = FALSE;
    
    for (loop_row = 0; loop_row < 5; loop_row++)
    {
        for (loop_coloum = 0;loop_coloum < 5; loop_coloum++, count++)
        {
            if ( IsInRect(x, y, &pme->m_calc_rc[count]) )
            {
                *evt = EVT_CHAR;
                *wp = CalcNumber[loop_row][loop_coloum];
                return TRUE;
            }           
        }               
    }
    
    return FALSE;
}
/*=======================================================================================
VkeyCtl_MapZhuyinChar()

Description:
    ������ע������ӳ���¼�

Prototype:
    boolean VkeyCtl_MapZhuyinChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

=======================================================================================*/
static boolean  VkeyCtl_MapZhuyinChar(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i,j,count;
    //boolean outloop = FALSE;
    count = 0;
  
    /* 4*10�ļ��� */
    for (i = 0;i < VZhuyinKeyPad.row;i++)
    { 
        for (j = 0;j < VZhuyinKeyPad.coloum;j++,count++)
        {
            if ( IsInRect(x,y,&pme->GridRect[count]) )
            {
                *wp = VZhuyinKeyItem[count].wp;
                *evt = VZhuyinKeyItem[count].evt;
                return TRUE;
            }               
        }
    }
    return FALSE;
}
/*=====================================================================
VkeyCtl_MapTHAINumber()

Description:
    ���������ּ��̵�ӳ���¼�

Prototype:
    boolean VkeyCtl_MapTHAINumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

======================================================================*/
static boolean VkeyCtl_MapTHAINumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i,j,count;
    //boolean outloop = FALSE;  //�Ѿ�����������
    
    count = 0;
   
    /* 2*5�ļ��� */
    for (i = 0; i < VNumberKeyPad.row; i++)
    {
        for (j = 0; j < VNumberKeyPad.coloum; j++,count++)
        {
            if( IsInRect(x,y,&pme->GridRect[count]) )
            {
                if(count != 4)
                {
                    #if 0
                    DBGPRINTF("VkeyCtl_MapTHAINumber---pme->m_oldkey=%d----count=%d",pme->m_oldkey,count);
                    if(pme->m_oldkey != count)
                    {
                        pme->m_oldkey=count;  
                        //*wp = VNumberTHAIKeyItem[count].wp;
                        *wp = VNumberTHAIKeyItem[count].name[0];
                        *evt = VNumberTHAIKeyItem[count].evt;                        
                    }
                    else
                    {
                       AEE_CancelTimer(Virtualkey_keypadtimer,pme); 
                        DBGPRINTF("VkeyCtl_MapTHAINumber---pme->b_multenter=%d----pme->m_curpros=%d-----[count].max=%d",pme->b_multenter,pme->m_curpros,VNumberTHAIKeyItem[count].m_Itemmax);
                       if(pme->b_multenter == FALSE)
                       {
                        *wp = VNumberTHAIKeyItem[count].name[0];
                        *evt = VNumberTHAIKeyItem[count].evt;  
                       }
                       else
                       {
                          *wp = VNumberTHAIKeyItem[count].name[pme->m_curpros];
                          DBGPRINTF("VkeyCtl_MapTHAINumber---VNumberTHAIKeyItem[count].name[pme->m_curpros]=%d",VNumberTHAIKeyItem[count].name[pme->m_curpros]);
                          *evt = VNumberTHAIKeyItem[count].evt;
                       }
                       if(pme->m_curpros<VNumberTHAIKeyItem[count].m_Itemmax)
                      	{
                      		pme->m_curpros ++;
                      	}
                      	else
                      	{
                      		pme->m_curpros = 0;
                      	} 
                        pme->b_multenter = TRUE;
                        AEE_SetTimer(1000,Virtualkey_keypadtimer,pme);
                    }
                    #else
                        *wp = VNumberKeyItem[count].wp;
                        *evt = VNumberKeyItem[count].evt;
                    #endif
                 return TRUE; 
                }
            }
            
        }//end   for (j = 0;j < 5;j++,count++) 
            
    }//end for (i = 0;i < 2;i++)   
    return FALSE;

}
/*=====================================================================
VkeyCtl_MapMYANMRANumber()

Description:
    ���������ּ��̵�ӳ���¼�

Prototype:
    boolean VkeyCtl_MapMYANMRANumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    evt,wp����Ҫӳ���ֵ
    

Return Value:
    TRUE:ӳ��ɹ�
    FALSE:ӳ��ʧ��

======================================================================*/
static boolean VkeyCtl_MapMYANMRANumber(VkeyCtl * pme,int x,int y,AEEEvent *evt,uint16 *wp)
{
    int i,j,count;
    //boolean outloop = FALSE;  //�Ѿ�����������
    
    count = 0;
   
    /* 2*5�ļ��� */
    for (i = 0; i < VNumberKeyPad.row; i++)
    {
        for (j = 0; j < VNumberKeyPad.coloum; j++,count++)
        {
            if( IsInRect(x,y,&pme->GridRect[count]) )
            {
                if(count != 4)
                {
                    #if 0
                    DBGPRINTF("VNumberMYNAMMAKeyItem---pme->m_oldkey=%d----count=%d---VNumberTHAIKeyItem[count].wp=%d",pme->m_oldkey,count,VNumberTHAIKeyItem[count].wp);
                    if(pme->m_oldkey != count)
                    {
                        pme->m_oldkey=count;  
                        //*wp = VNumberTHAIKeyItem[count].wp;
                        *wp = VNumberMYNAMMAKeyItem[count].name[0];
                        *evt = VNumberMYNAMMAKeyItem[count].evt;                        
                    }
                    else
                    {
                       //AEE_CancelTimer(Virtualkey_keypadtimer,pme); 
                        DBGPRINTF("VNumberMYNAMMAKeyItem---pme->b_multenter=%d----pme->m_curpros=%d-----[count].max=%d",pme->b_multenter,pme->m_curpros,VNumberMYNAMMAKeyItem[count].m_Itemmax);
                      /* if(pme->b_multenter == FALSE)
                       {
                        *wp = VNumberMYNAMMAKeyItem[count].name[0];
                        *evt = VNumberMYNAMMAKeyItem[count].evt;  
                       }
                       else */
                       {
                          *wp = VNumberMYNAMMAKeyItem[count].name[pme->m_curpros];
                          DBGPRINTF("VNumberMYNAMMAKeyItem---VNumberMYNAMMAKeyItem[count].name[pme->m_curpros]=%d",VNumberMYNAMMAKeyItem[count].name[pme->m_curpros]);
                          *evt = VNumberMYNAMMAKeyItem[count].evt;
                       }
                       if(pme->m_curpros<VNumberMYNAMMAKeyItem[count].m_Itemmax)
                      	{
                      		pme->m_curpros ++;
                      	}
                      	else
                      	{
                      		pme->m_curpros = 0;
                      	} 
                        pme->b_multenter = TRUE;
                        //AEE_SetTimer(1000,Virtualkey_keypadtimer,pme);
                    }
                    #else
                        *wp = VNumberKeyItem[count].wp;
                        *evt = VNumberKeyItem[count].evt;
                    #endif
                 return TRUE; 
                }
            }
            
        }//end   for (j = 0;j < 5;j++,count++) 
            
    }//end for (i = 0;i < 2;i++)   
    return FALSE;

}

static void Virtualkey_keypadtimer(void *pUser)
{
	register VkeyCtl *pContext = (VkeyCtl *) pUser;
	pContext->m_curpros = 0;
	pContext->b_multenter = FALSE;
}

/*========================================================================
VkeyCtl_HitPad()

Description:
    ���ü��̵Ļ���Ч��

Prototype:
    void VkeyCtl_HitPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��TRUE��ʾDOWN��FALSE��ʾUP
    
Return Value:
    none

=========================================================================*/
static void      VkeyCtl_HitPad(VkeyCtl* pme, int x, int y,boolean sign)
{
    switch (pme->m_dwProps)
    {
        case AEE_VKEY_NUMBER:
            VkeyCtl_HitNumberPad(pme, x, y, sign);
            break;
        case AEE_VKEY_BIGCHAR:
            //VkeyCtl_HitNumberPad(pme, x, y, sign);
            VkeyCtl_HitBigCharPad(pme, x, y, sign);
            break;
        case AEE_VKEY_LITTLECHAR:
            //VkeyCtl_HitNumberPad(pme, x, y, sign);
            DBGPRINTF("AEE_VKEY_LITTLECHAR-----------1");
            VkeyCtl_HitLittleCharPad(pme, x, y, sign);
            break;
        case AEE_VKEY_SYMBOL:
            VkeyCtl_HitSymbolPad(pme, x, y, sign);
            break;
        case AEE_VKEY_NUMBER_LITTLECHAR:
            DBGPRINTF("AEE_VKEY_NUMBER_LITTLECHAR    1"); 
            VkeyCtl_HitNumberLittleCharPad(pme, x, y, sign);
            break;
        case AEE_VKEY_NUMBER_BIGCHAR:
            VkeyCtl_HitNumberBigCharPad(pme, x, y, sign);
            break;
        case AEE_VKEY_DIALNUMBER:
            VkeyCtl_HitDialNumberPad(pme, x, y, sign);
            break;
        case AEE_VKEY_CALCULATOR:
            VkeyCtl_HitCalcNumberPad(pme, x, y, sign);
            break;
        case AEE_VKEY_ZHUYIN:
            VkeyCtl_HitZhuyinCharPad(pme, x, y, sign);
            break;
        case AEE_VKEY_USERKEYPAD:
            VkeyCtl_HitUserKeyPad(pme, x, y, sign);
            break;
        case AEE_VKEY_THAINUMBER:
            VkeyCtl_HitNumberPad(pme, x, y, sign);
            break;
        case AEE_VKEY_MYANMRANUMBER:
            VkeyCtl_HitNumberPad(pme, x, y, sign);
            break;    
        default:
           break;
    }
    return;
}
/*=======================================================================
VkeyCtl_HitNumberPad()

Description:
    �������ּ��̱�����Ч��

Prototype:
    void VkeyCtl_HitNumberPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none

========================================================================*/
static void VkeyCtl_HitNumberPad(VkeyCtl * pme, int x, int y, boolean sign)
{
    if (pme)
    {
        int   i;
        for (i = 0; i < 15; i++)
        {
            if (IsInRect(x, y, &pme->GridRect[i]))
            {
                IImage *     pImage;
                pImage = NULL;
                if (sign)
                {
                    pImage = pme->m_KeyPadDown;
                }
                else
                {
                    pImage = pme->m_KeyPadNormal;
                }
                
                if (pImage == NULL)
                {
                    return;
                }
                
                IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->GridRect[i].x - pme->m_rc.x,
                               pme->GridRect[i].y - pme->m_rc.y);
               
                IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               45,
                               //((i < 11)?18:19)
                               44);
                IIMAGE_Draw(pImage,
                            pme->GridRect[i].x,
                            pme->GridRect[i].y);
                IDISPLAY_Update(pme->m_pIDisplay);
            }
        }
    }
    
    #if 0
    int i,j,count;
    //boolean outloop=FALSE;
    AEERect temp_rc;
    
    count = 0;
    
    for (i = 0; i < VNumberKeyPad.row; i++)
    {
        for (j = 0; j < VNumberKeyPad.coloum; j++,count++)
        {
            if( IsInRect(x,y,&pme->GridRect[count]) )
            {
                SETAEERECT(&temp_rc, pme->GridRect[count].x, pme->GridRect[count].y,
                             pme->GridRect[count].dx-1, pme->GridRect[count].dy-1);
                if (sign == TRUE)
                {
                    (void)IDISPLAY_FrameButton ( pme->m_pIDisplay, 
                                                 &temp_rc, 
                                                 sign, 
                                                 pme->m_Fillclr );
                    (void)IDISPLAY_DrawText ( pme->m_pIDisplay, 
                                               AEE_FONT_BOLD, 
                                               (VNumberKeyItem[count].name), 
                                               1, 
                                               0, 
                                               0, 
                                               &pme->GridRect[count], 
                                               IDF_ALIGN_MIDDLE|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT  
                                             ) ;
                    
                }
                else if (sign == FALSE)
                {
                    (void)IDISPLAY_FrameButton ( pme->m_pIDisplay, 
                                                 &temp_rc, 
                                                 sign, 
                                                 pme->m_Bground );
                    (void)IDISPLAY_DrawText ( pme->m_pIDisplay, 
                                               AEE_FONT_NORMAL, 
                                               (VNumberKeyItem[count].name), 
                                               1, 
                                               0, 
                                               0, 
                                               &pme->GridRect[count], 
                                               IDF_ALIGN_MIDDLE|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT  
                                             ) ;
                }
                
                //pme->GridRect[count] = temp_rc;
                IDISPLAY_Update(pme->m_pIDisplay) ;
                return ;
                //ʹ��return ��������������� xyue edit it 
            }
            
        }//end   for (j = 0;j < 5;j++,count++)    
    
    }//end for (i = 0;i < 2;i++)
#endif    
    return ;
}

/*=======================================================================
VkeyCtl_HitBigCharPad()

Description:
    ���ô�д��ĸ���̱�����Ч��

Prototype:
    void VkeyCtl_HitBigCharPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none



========================================================================*/
static void      VkeyCtl_HitBigCharPad(VkeyCtl* pme, int x, int y,boolean sign)
{
    int i,j,count;
   // boolean outloop=FALSE;
    AEERect temp_rc;
    
    count = 0;
    
    for (i = 0; i < VBCharKeyPad.row; i++)
    {
        for (j = 0; j < VBCharKeyPad.coloum; j++,count++)
        {
            if( IsInRect(x,y,&pme->GridRect[count]) )
            {
                SETAEERECT(&temp_rc, pme->GridRect[count].x, pme->GridRect[count].y,
                            pme->GridRect[count].dx-1, pme->GridRect[count].dy-1);
                if (sign == TRUE)
                {
                    (void)IDISPLAY_FrameButton ( pme->m_pIDisplay, 
                                                 &temp_rc, 
                                                 sign, 
                                                 pme->m_Fillclr );
                    (void)IDISPLAY_DrawText ( pme->m_pIDisplay, 
                                               AEE_FONT_BOLD, 
                                               (VBCharKeyItem[count].name), 
                                               1, 
                                               0, 
                                               0, 
                                               &pme->GridRect[count], 
                                               IDF_ALIGN_MIDDLE|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT  
                                             ) ;
                    
                }
                else if (sign == FALSE)
                {
                    (void)IDISPLAY_FrameButton ( pme->m_pIDisplay, 
                                                 &temp_rc, 
                                                 sign, 
                                                 pme->m_Bground );
                    (void)IDISPLAY_DrawText ( pme->m_pIDisplay, 
                                               AEE_FONT_NORMAL, 
                                               (VBCharKeyItem[count].name), 
                                               1, 
                                               0, 
                                               0, 
                                               &pme->GridRect[count], 
                                               IDF_ALIGN_MIDDLE|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT  
                                             ) ;
                    
                }
                
                //pme->GridRect[count] = temp_rc;
                IDISPLAY_Update(pme->m_pIDisplay) ;
                return ;
                //xyue edit
            }
            
        }//end   for (j = 0;j < 5;j++,count++) 
      
    }//end for (i = 0;i < 2;i++)
    return ;
}
/*=======================================================================
VkeyCtl_HitLittleCharPad()

Description:
    ����Сд��ĸ���̱�����Ч��

Prototype:
    void VkeyCtl_HitLittleCharPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none

========================================================================*/
static void      VkeyCtl_HitLittleCharPad(VkeyCtl* pme, int x, int y,boolean sign)
{
     DBGPRINTF("VkeyCtl_HitLittleCharPad-----------1");
    if (pme)
    {
        int   i;
        for (i = 0; i < MAX_PYKEYPAD_NUMBER-2; i++)
        {
            
            DBGPRINTF("VkeyCtl_HitLittleCharPad-----------2");
            if (IsInRect(x, y, &pme->GridRect[i]))
            {
                IImage *     pImage;
                pImage = NULL;
                if (sign)
                {
                    pImage = pme->m_KeyPadDown;
                }
                else
                {
                    pImage = pme->m_KeyPadNormal;
                }
                
                if (pImage == NULL)
                {
                    return;
                }
                DBGPRINTF("VkeyCtl_HitLittleCharPad-----------3");
                IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->GridRect[i].x - pme->m_rc.x,
                               pme->GridRect[i].y - pme->m_rc.y);
                IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               30,
                               25);
                if(i > 30)
                {
                 IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               50,
                               25);
                }
                IIMAGE_Draw(pImage,
                            pme->GridRect[i].x,
                            pme->GridRect[i].y);
                IDISPLAY_Update(pme->m_pIDisplay);
            }
        }
    }
    
    return;
}
/*=======================================================================
VkeyCtl_HitSymbolPad()

Description:
    ���÷��ż��̱�����Ч��

Prototype:
    void VkeyCtl_HitSymbolPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none

========================================================================*/
static void      VkeyCtl_HitSymbolPad(VkeyCtl* pme, int x, int y,boolean sign)
{
    if (pme)
    {
        int   i;
        for (i = 0; i < MAX_OTHERKEYPAD_NUMBER; i++)
        {
            if (IsInRect(x, y, &pme->GridRect[i]))
            {
                IImage *     pImage;
                pImage = NULL;
                if (sign)
                {
                    pImage = pme->m_KeyPadDown;
                }
                else
                {
                    pImage = pme->m_KeyPadNormal;
                }
                
                if (pImage == NULL)
                {
                    return;
                }
                
               
                if(i > 39)
                {
                 IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->GridRect[40].x - pme->m_rc.x,
                               pme->GridRect[40].y - pme->m_rc.y);    
                 IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               65,
                               //((i < 11)?18:19)
                               25);
                 IIMAGE_Draw(pImage,
                            pme->GridRect[40].x,
                            pme->GridRect[40].y);
                }
                else
                {
                 IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->GridRect[i].x - pme->m_rc.x,
                               pme->GridRect[i].y - pme->m_rc.y);   
                 IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               30,
                               //((i < 11)?18:19)
                               25);   
                 IIMAGE_Draw(pImage,
                            pme->GridRect[i].x,
                            pme->GridRect[i].y);
                }                
                
                IDISPLAY_Update(pme->m_pIDisplay);
            }
        }
    }
    
    return;
}
/*=======================================================================
VkeyCtl_HitNumberLittleCharPad()

Description:
    ��������&Сд��ĸ���̱�����Ч��

Prototype:
    void VkeyCtl_HitNumberLittleCharPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none

========================================================================*/
static void      VkeyCtl_HitNumberLittleCharPad(VkeyCtl* pme, int x, int y,boolean sign)
{
    if (pme)
    {
        int   i;
        for (i = 0; i < MAX_OTHERKEYPAD_NUMBER; i++)
        {
            if (IsInRect(x, y, &pme->GridRect[i]))
            {
                IImage *     pImage;
                pImage = NULL;
                if (sign)
                {
                    pImage = pme->m_KeyPadDown;
                }
                else
                {
                    pImage = pme->m_KeyPadNormal;
                }
                
                if (pImage == NULL)
                {
                    return;
                }                
              
                if(i > 39)
                {
                 IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->GridRect[40].x - pme->m_rc.x,
                               pme->GridRect[40].y - pme->m_rc.y);    
                 IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               65,
                               25);
                 IIMAGE_Draw(pImage,
                            pme->GridRect[40].x,
                            pme->GridRect[40].y);
                }
                else
                {
                 IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->GridRect[i].x - pme->m_rc.x,
                               pme->GridRect[i].y - pme->m_rc.y);    
                IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               30,
                               25);
                 IIMAGE_Draw(pImage,
                            pme->GridRect[i].x,
                            pme->GridRect[i].y);
                }
                
               
                IDISPLAY_Update(pme->m_pIDisplay);
            }
        }
    }
    
    return;
}
/*=======================================================================
VkeyCtl_HitNumberBigCharPad()

Description:
    ���֡�&��д��ĸ���̱�����Ч��

Prototype:
    void VkeyCtl_HitNumberBigCharPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none

========================================================================*/
static void      VkeyCtl_HitNumberBigCharPad(VkeyCtl* pme, int x, int y,boolean sign)
{
    if (pme)
    {
        int   i;
        for (i = 0; i < MAX_OTHERKEYPAD_NUMBER; i++)
        {
            if (IsInRect(x, y, &pme->GridRect[i]))
            {
                IImage *     pImage;
                pImage = NULL;
                if (sign)
                {
                    pImage = pme->m_KeyPadDown;
                }
                else
                {
                    pImage = pme->m_KeyPadNormal;
                }
                
                if (pImage == NULL)
                {
                    return;
                }
                
                
                if(i > 39)
                {    
                 IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->GridRect[40].x - pme->m_rc.x,
                               pme->GridRect[40].y - pme->m_rc.y);    
                 IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               65,
                               25);
                 IIMAGE_Draw(pImage,
                            pme->GridRect[40].x,
                            pme->GridRect[40].y);
                }
                else
                {
                 IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->GridRect[i].x - pme->m_rc.x,
                               pme->GridRect[i].y - pme->m_rc.y);    
                 IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               30,
                               25);
                 IIMAGE_Draw(pImage,
                            pme->GridRect[i].x,
                            pme->GridRect[i].y);
                }
               
                IDISPLAY_Update(pme->m_pIDisplay);
            }
        }
    }
    
    return;
}
/*=======================================================================
VkeyCtl_HitUserKeyPad()

Description:
   �û��û��Լ�����ļ���Ч��

Prototype:
    void VkeyCtl_HitUserKeyPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none


=======================================================================*/
static void      VkeyCtl_HitUserKeyPad(VkeyCtl* pme, int x, int y,boolean sign)
{
    int i = (pme->m_keypad).row;
    int j = (pme->m_keypad).coloum;
    int count = 0;
    int loop_row,loop_coloum;
    //boolean outloop = FALSE;
    AEERect temp_rc;
    
    for (loop_row = 0; loop_row < i; loop_row++)
    {
        for (loop_coloum = 0; loop_coloum < j; loop_coloum++,count++)
        {
            if( IsInRect(x,y,&pme->UsrGridRc[count]) )
            {
                SETAEERECT(&temp_rc, pme->UsrGridRc[count].x, pme->UsrGridRc[count].y,
                           pme->UsrGridRc[count].dx-1, pme->UsrGridRc[count].dy-1);
                if (sign == TRUE)
                {
                    (void)IDISPLAY_FrameButton ( pme->m_pIDisplay, 
                                                 &temp_rc, 
                                                 sign, 
                                                 pme->m_Fillclr );
                    (void)IDISPLAY_DrawText ( pme->m_pIDisplay, 
                                               AEE_FONT_BOLD, 
                                               ((pme->m_keypad).pItem)->name, 
                                               -1, 
                                               0, 
                                               0, 
                                               &pme->UsrGridRc[count], 
                                               IDF_ALIGN_MIDDLE|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT  
                                             ) ;
                    
                }
                else if (sign == FALSE)
                {
                    (void)IDISPLAY_FrameButton ( pme->m_pIDisplay, 
                                                 &temp_rc, 
                                                 sign, 
                                                 pme->m_Bground 
                                               );
                    (void)IDISPLAY_DrawText ( pme->m_pIDisplay, 
                                               AEE_FONT_NORMAL, 
                                               ((pme->m_keypad).pItem)->name, 
                                               -1, 
                                               0, 
                                               0, 
                                               &pme->UsrGridRc[count], 
                                               IDF_ALIGN_MIDDLE|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT  
                                             ) ;
                    
                }                
                //pme->GridRect[count] = temp_rc;
                IDISPLAY_Update(pme->m_pIDisplay) ;
                (pme->m_keypad).pItem -= count;
                return  ;
            }
            (pme->m_keypad).pItem++;            
        }//end   for (j = 0;j < 5;j++,count++)               
    }   
    return  ;
}
/*=======================================================================
VkeyCtl_HitDialNumberPad()

Description:
   ���ż��̱�����Ч��

Prototype:
    void VkeyCtl_HitDialNumberPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none


========================================================================*/

static void      VkeyCtl_HitDialNumberPad(VkeyCtl* pme, int x, int y, boolean sign)
{
    if (pme)
    {
        int   i;
        for (i = 0; i < MAX_DAIL_NUMBER; i++)
        {
            if (IsInRect(x, y, &pme->m_dial_rc[i]))
            {
                IImage *     pImage;
                pImage = NULL;
                if (sign)
                {
                    pImage = ISHELL_LoadImage(pme->m_pIShell,
                                              AEEFS_SHAREDVK_DIR"dailkeydown.bmp");
                }
                else
                {

					boolean rt ;
					AEECLSID  pCLSID;
					uint16 avkkey;

                    pImage = ISHELL_LoadImage(pme->m_pIShell,
                                              AEEFS_SHAREDVK_DIR"dailkeyup.bmp");
					pCLSID = ISHELL_ActiveApplet (pme->m_pIShell);
					switch(i)
					{
						case 0:
							avkkey = AVK_1;
							break;
						case 1:
							avkkey = AVK_2;
							break;
						case 2:
							avkkey = AVK_3;
							break;
						case 3:
							avkkey = AVK_4;
							break;
						case 4:
							avkkey = AVK_5;
							break;
						case 5:
							avkkey = AVK_6;
							break;
						case 6:
							avkkey = AVK_7;
							break;
						case 7:
							avkkey = AVK_8;
							break;
						case 8:
							avkkey = AVK_9;
							break;
						case 9:
							avkkey = AVK_STAR;
							break;
						case 10:
							avkkey = AVK_0;
							break;
						case 11:
							avkkey = AVK_POUND;
							break;
					}
					
					rt =  ISHELL_PostEvent(pme->m_pIShell,pCLSID,EVT_USER,avkkey,0);
                }
                
                if (pImage == NULL)
                {
                    return;
                }
                IIMAGE_Draw(pImage,
                            pme->m_dial_rc[i].x + 20,
                            pme->m_dial_rc[i].y);
                IIMAGE_Release(pImage);
                IDISPLAY_Update(pme->m_pIDisplay);
            }
        }
    }
    
    return;
}

/*=======================================================================
VkeyCtl_HitCalcNumberPad()

Description:
   ���ż��̱�����Ч��

Prototype:
    void VkeyCtl_HitCalcNumberPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none


========================================================================*/

static void    VkeyCtl_HitCalcNumberPad(VkeyCtl* pme, int x, int y, boolean sign)
{
    if (pme)
    {
        int   i;
        for (i = 0; i < MAX_CALC_NUMBER; i++)
        {
            if ((!sign)&&(IsInRect(x, y, &pme->m_calc_rc[1])))
            {
                return;
            }
            if (IsInRect(x, y, &pme->m_calc_rc[i]))
            {
                IImage *     pImage;
                pImage = NULL;
                if (sign)
                {
                    pImage = pme->m_KeyPadDown;
                }
                else
                {
                    pImage = pme->m_KeyPadNormal;
                }
                
                if (pImage == NULL)
                {
                    return;
                }
                
                IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->m_calc_rc[i].x - pme->m_rc.x,
                               pme->m_calc_rc[i].y - pme->m_rc.y);
                IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               31,
                               22);
                IIMAGE_Draw(pImage,
                            pme->m_calc_rc[i].x,
                            pme->m_calc_rc[i].y);
                IDISPLAY_Update(pme->m_pIDisplay);
            }
        }
    }
    
    return;
}
/*=======================================================================
VkeyCtl_HitZhuyinCharPad()

Description:
    ����ע�����̱�����Ч��

Prototype:
    void VkeyCtl_HitZhuyinCharPad(VkeyCtl* pme, int x, int y,boolean sign)

Parameters:
    pme:  Pointer to the IVkeyCtl Interface object.
    x,y: ���ݵĵ������
    sign��ΪTRUE�ǰ���Ч����ΪFALSE ��͹��Ч��
    
Return Value:
    none

========================================================================*/
static void      VkeyCtl_HitZhuyinCharPad(VkeyCtl* pme, int x, int y,boolean sign)
{
    if (pme)
    {
        int   i;
        for (i = 0; i < MAX_OTHERKEYPAD_NUMBER; i++)
        {
            if (IsInRect(x, y, &pme->GridRect[i]))
            {
                IImage *     pImage;
                pImage = NULL;
                if (sign)
                {
                    pImage = pme->m_KeyPadDown;
                }
                else
                {
                    pImage = pme->m_KeyPadNormal;
                }
                
                if (pImage == NULL)
                {
                    return;
                }
                
                IIMAGE_SetParm(pImage,
                               IPARM_OFFSET,
                               pme->GridRect[i].x - pme->m_rc.x,
                               pme->GridRect[i].y - pme->m_rc.y);
                IIMAGE_SetParm(pImage,
                               IPARM_SIZE,
                               15,
                               ((i < 11)?18:19));
                IIMAGE_Draw(pImage,
                            pme->GridRect[i].x,
                            pme->GridRect[i].y);
                IDISPLAY_Update(pme->m_pIDisplay);
            }
        }
    }
    
    return;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
���Ŵ�����
�ּ������²��Ž��棺
      1    2    3
      4    5    6
      7    8    9
      *    0    #
      W    +    P
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++=*/

/*======================================================================

�ػ����Ž���
#define FONT_IMAGE_WIDTH    10
#define FONT_IMAGE_HEIGHT   20
#define BG_IMAGE_WIDTH      55
#define BG_IMAGE_HEIGHT     40
======================================================================*/
static void   VkeyCtl_SetDialNumberPad(VkeyCtl* pme, AEERect *prc)
{
    IImage *         pImage;
    int              i, j, dailnum = 0;
    
    if(pme == NULL)
    {
        return;
    }
    
    pImage = NULL;
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    
    if ((pme->m_rc.dx < DAIL_IMAGE_MINWIDTH)
       || (pme->m_rc.dy < DAIL_IMAGE_MINHEIGHT))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }      
    
    pImage = ISHELL_LoadImage(pme->m_pIShell,
                              AEEFS_SHAREDVK_DIR"dailkeypad.bmp");
    if (pImage == NULL)
    {
        return;
    }
    
    IIMAGE_Draw(pImage, pme->m_rc.x, pme->m_rc.y);
    IIMAGE_Release(pImage);
    
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 3; j++)
        {
            pme->m_dial_rc[dailnum].x = (int16)(pme->m_rc.x + 11 + 55 * j);
            pme->m_dial_rc[dailnum].y = (int16)(pme->m_rc.y + 5 + 25 * i);
            pme->m_dial_rc[dailnum].dx = 44;
            pme->m_dial_rc[dailnum].dy = 23;
            dailnum++;
        }
        
    }
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
}   

/*======================================================================

�ػ����������̽���
#define FONT_IMAGE_WIDTH    10
#define FONT_IMAGE_HEIGHT   20
#define BG_IMAGE_WIDTH      55
#define BG_IMAGE_HEIGHT     40
======================================================================*/
static void   VkeyCtl_SetCalculatorPad(VkeyCtl* pme, AEERect *prc)
{
    int              i, j, count = 0;
    
    if(pme == NULL)
    {
        return;
    }
    
    /*=======================================================================
    �����С�ռ�
    =======================================================================*/
    pme->m_rc = *prc;
    
    if ((pme->m_rc.dx < CALC_IMAGE_MINWIDTH)
       || (pme->m_rc.dy < CALC_IMAGE_MINHEIGHT))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }      
    
    if (pme->m_KeyPadNormal != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadNormal);
    }
    if (pme->m_KeyPadDown != NULL)
    {
        (void)IIMAGE_Release(pme->m_KeyPadDown);
    }
    pme->m_KeyPadNormal = ISHELL_LoadImage(pme->m_pIShell,
                                           AEEFS_SHAREDVK_DIR"calcupkeypad.bmp");
    pme->m_KeyPadDown = ISHELL_LoadImage(pme->m_pIShell,
                                         AEEFS_SHAREDVK_DIR"calcdownkeypad.bmp");
    if ((pme->m_KeyPadNormal == NULL)
       || (pme->m_KeyPadDown == NULL))
    {
        return;
    }
    
    IIMAGE_Draw(pme->m_KeyPadNormal, pme->m_rc.x, pme->m_rc.y);
    
    for(i = 0; i < 5; i++)
    {
        for(j = 0; j < 5; j++)
        {
            pme->m_calc_rc[count].x = (int16)(pme->m_rc.x + 8 + 32 * j);
            pme->m_calc_rc[count].y = (int16)(pme->m_rc.y + 3 + 27 * i);
            pme->m_calc_rc[count].dx = 31;
            pme->m_calc_rc[count].dy = 22;
            count++;
        }
    }
    
    IDISPLAY_Update(pme->m_pIDisplay);
   
    return;
} 

/*========================================================================

�ṩ������Ľӿ�-----���õ������ʾ���������治ͬ��ͼƬ
=========================================================================*/

/*=======================================*/
//�����û��Լ�����ļ�����ɫ
static void  IVkeyCtl_SetColor(IVkeyCtl * po, RGBVAL rgb , RGBVAL  rgbPress)
{
    VkeyCtl* pme=(VkeyCtl*)po;
    
    pme->m_Bground = rgb;
    pme->m_Fillclr = rgbPress;
    return;
}
//�����û��Լ�����ļ���
static int  IVkeyCtl_SetUserKeyPad(IVkeyCtl * po, AEERect *prc, AEEVKeyPad  *pkp)
{
    VkeyCtl* pme = (VkeyCtl*)po;
    
    if ((!prc) || (!pkp))
    {
        return EFAILED;
    }
    pme->m_dwProps = AEE_VKEY_USERKEYPAD;
    pme->m_rc = *prc;
    pme->m_keypad = *pkp;
    
    if (!pme->m_bActive)
    {
       return SUCCESS;
    }
    VkeyCtl_SetUserKeyPad(pme,prc);
    
    return SUCCESS;
}

static void VkeyCtl_SetUserKeyPad(VkeyCtl * pme, AEERect *prc)
{
    int i = (pme->m_keypad).row;
    int j = (pme->m_keypad).coloum;
    int count = 0;
    int loop_row,loop_coloum;
    int rc_x = prc->x;
    int rc_y = prc->y;
    int rc_dx = prc->dx/j;
    int rc_dy = prc->dy/i;
    boolean sign = FALSE;//͹���ʾ
    AEERect temp_rc;
    
    if ((rc_dx < FontWidth) || (rc_dy < FontHeight))
    {
        return ;  //û�дﵽ��С�ռ��Ҫ��
    }
      
    
    for (loop_row = 0; loop_row < i; loop_row++)
    {
        for (loop_coloum = 0; loop_coloum < j; loop_coloum++, count++)      
        {
            SETAEERECT(&pme->UsrGridRc[count], rc_x, rc_y, rc_dx,rc_dy);
            SETAEERECT(&temp_rc, rc_x, rc_y, rc_dx-1,rc_dy-1);
            (void)IDISPLAY_FrameButton ( pme->m_pIDisplay, 
                                         &temp_rc, 
                                         sign, 
                                         pme->m_Bground ); 
            (void)IDISPLAY_DrawText ( pme->m_pIDisplay, 
                                       AEE_FONT_NORMAL, 
                                       ((pme->m_keypad).pItem)->name, 
                                       -1, 
                                       0, 
                                       0, 
                                       &pme->UsrGridRc[count], 
                                       IDF_ALIGN_MIDDLE|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT  
                                       ) ;
            rc_x = rc_x + rc_dx;
            (pme->m_keypad).pItem ++;
        }
        rc_x = prc->x;
        rc_y = rc_y + rc_dy;  
    }
    (pme->m_keypad).pItem -=count;
    IDISPLAY_Update(pme->m_pIDisplay);
}

