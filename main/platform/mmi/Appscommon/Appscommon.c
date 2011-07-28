/*==============================================================================
//           A P P S   C O M M O N   F U N C T I O N S
// �ļ���
//        Appscommon.c
//        ��Ȩ����(c) 2009 Anylook Tech. CO.,LTD.
//        
// �ļ�˵����
//        ���ļ��ṩ��̬ Applet �õ��Ĺ���������ʵ�֡�         
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
#include "Appscommon.h"

#include "AEE_OEM.h"
#include "AEE_OEMEvent.h"

// �ַ�����Դ�ļ�
#include "appscommon.brh"

// ͼƬ��Դ�ļ�
#include "appscommonimages.brh"
#include "CallApp.h"

#define APPSCOMMON_STR2KEY_INCLUDE
#include "Appscommon_Str2Key.h"
#if !defined( AEE_SIMULATOR)
#include "OEMConfig.h"
#include "OEMCFGI.h"
#endif//#if !defined( AEE_SIMULATOR)

#include "AEEGraphics.h"
#ifndef WIN32
#include "disp.h"
#else
#include "oemhelperfunctype.h"
#endif//WIN32
#include "OEMClassIDs.h"
#include "msg.h"
/*==============================================================================
                                 
                                 �궨��ͳ���
                                 
==============================================================================*/
const char GsmToUcs2[GSM_CHARACTER_SET_SIZE] =
{            /*+0x0        +0x1        +0x2        +0x3        +0x4        +0x5        +0x6        +0x7*/
/*0x00*/ (char)0x40, (char)0xa3, (char)0x24, (char)0xa5, (char)0xe8, (char)0xe9, (char)0xf9, (char)0xec,
/*0x08*/ (char)0xf2, (char)0xc7, (char)0x0a, (char)0xd8, (char)0xf8, (char)0x0d, (char)0xc5, (char)0xe5,
/*0x10*/ (char)0,    (char)0x5f, (char)0,    (char)0,    (char)0,    (char)0,    (char)0,    (char)0,
/*0x18*/ (char)0,    (char)0,    (char)0,    (char)0x20, (char)0xc6, (char)0xe6, (char)0xdf, (char)0xc9,
/*0x20*/ (char)0x20, (char)0x21, (char)0x22, (char)0x23, (char)0x24, (char)0x25, (char)0x26, (char)0x27,
/*0x28*/ (char)0x28, (char)0x29, (char)0x2a, (char)0x2b, (char)0x2c, (char)0x2d, (char)0x2e, (char)0x2f,
/*0x30*/ (char)0x30, (char)0x31, (char)0x32, (char)0x33, (char)0x34, (char)0x35, (char)0x36, (char)0x37,
/*0x37*/ (char)0x38, (char)0x39, (char)0x3a, (char)0x3b, (char)0x3c, (char)0x3d, (char)0x3e, (char)0x3f,
/*0x40*/ (char)0x40, (char)0x41, (char)0x42, (char)0x43, (char)0x44, (char)0x45, (char)0x46, (char)0x47,
/*0x48*/ (char)0x48, (char)0x49, (char)0x4a, (char)0x4b, (char)0x4c, (char)0x4d, (char)0x4e, (char)0x4f,
/*0x50*/ (char)0x50, (char)0x51, (char)0x52, (char)0x53, (char)0x54, (char)0x55, (char)0x56, (char)0x57,
/*0x58*/ (char)0x58, (char)0x59, (char)0x5a, (char)0xc4, (char)0xd6, (char)0xd1, (char)0xdc, (char)0x5f,
/*0x60*/ (char)0xbf, (char)0x61, (char)0x62, (char)0x63, (char)0x64, (char)0x65, (char)0x66, (char)0x67,
/*0x68*/ (char)0x68, (char)0x69, (char)0x6a, (char)0x6b, (char)0x6c, (char)0x6d, (char)0x6e, (char)0x6f,
/*0x70*/ (char)0x70, (char)0x71, (char)0x72, (char)0x73, (char)0x74, (char)0x75, (char)0x76, (char)0x77,
/*0x78*/ (char)0x78, (char)0x79, (char)0x7a, (char)0xe4, (char)0xf6, (char)0xf1, (char)0xfc, (char)0xe0
};

const AECHAR  Ucs2ToGsm[UCS2_TO_GSM_TABLE_SIZE] =
{           /*+0x0      +0x1        +0x2        +0x3        +0x4        +0x5        +0x6        +0x7*/
/*0x00*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0x08*/    NON_GSM,    NON_GSM,    0x0a,       NON_GSM,    NON_GSM,    0x0d,       NON_GSM,    NON_GSM,
/*0x10*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0x18*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0x20*/    0x20,       0x21,       0x22,       0x23,       0x24,       0x25,       0x26,       0x27,
/*0x28*/    0x28,       0x29,       0x2a,       0x2b,       0x2c,       0x2d,       0x2e,       0x2f,
/*0x30*/    0x30,       0x31,       0x32,       0x33,       0x34,       0x35,       0x36,       0x37,
/*0x38*/    0x38,       0x39,       0x3a,       0x3b,       0x3c,       0x3d,       0x3e,       0x3f,
/*0x40*/    0x40,       0x41,       0x42,       0x43,       0x44,       0x45,       0x46,       0x47,
/*0x48*/    0x48,       0x49,       0x4a,       0x4b,       0x4c,       0x4d,       0x4e,       0x4f,
/*0x50*/    0x50,       0x51,       0x52,       0x53,       0x54,       0x55,       0x56,       0x57,
/*0x58*/    0x58,       0x59,       0x5a,       NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    0x5f,
/*0x60*/    NON_GSM,    0x61,       0x62,       0x63,       0x64,       0x65,       0x66,       0x67,
/*0x68*/    0x68,       0x69,       0x6a,       0x6b,       0x6c,       0x6d,       0x6e,       0x6f,
/*0x70*/    0x70,       0x71,       0x72,       0x73,       0x74,       0x75,       0x76,       0x77,
/*0x78*/    0x78,       0x79,       0x7a,       NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0x80*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0x88*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0x90*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0x98*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0xa0*/    NON_GSM,    0x21,       NON_GSM,    0x01,       NON_GSM,       0x03,       NON_GSM,    NON_GSM,
/*0xa8*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0xb0*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,
/*0xb8*/    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    NON_GSM,    0x3f,
/*0xc0*/    0x41,       0x41,       0x41,       0x41,       0x5b,       0x0e,       0x1c,       0x43,
/*0xc8*/    0x45,       0x45,       0x45,       0x45,       0x49,       0x49,       0x49,       0x60,
/*0xd0*/    NON_GSM,    0x4e,       0x4f,       0x4f,       0x4f,       0x4f,       0x5c,       0x2a,
/*0xd8*/    0x0b,       0x55,       0x55,       0x55,       0x55,       0x59,       NON_GSM,    0x1e,
/*0xe0*/    0x61,       0x61,       0x61,       0x61,       0x7b,       0x0f,       0x1d,       0x63,
/*0xe8*/    0x65,       0x65,       0x65,       0x65,       0x69,       0x69,       0x69,       0x69,
/*0xf0*/    NON_GSM,    0x6e,       0x6f,       0x6f,       0x6f,       0x6f,       0x7c,       0x2f,
/*0xf8*/    0x0c,       0x06,       0x75,       0x75,       0x75,       0x79,       NON_GSM,    0x79
};

#ifdef FEATURE_LANG_HEBREW
// Ucs2ToHebrew ��ʹ��˵��: 
// ��Ҫ����UNICODE�ַ� wChar ת��Ϊ Hebrew ��ֵ����ʹ�� Ucs2ToHebrew[wChar & 0x00FF]
// �����ַ� 0x203E ��0x00D7 ����ʹ�ô˱�0x203E-->0xAF ��0x00D7-->0xAA �� ����ֵ NON_HEB
// ��ʾ����Ӧ�ַ���ת��ʧ��
const AECHAR  Ucs2ToHebrew[HEBREW_CONVERT_TABLE_SIZE] =
{           /*+0x0      +0x1        +0x2        +0x3        +0x4        +0x5        +0x6        +0x7*/
/*0x00*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0x08*/    NON_HEB,    NON_HEB,    0x0a,       NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0x10*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    0xdf,
/*0x18*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0x20*/    0x20,       0x21,       0x22,       0x23,       0x24,       0x25,       0x26,       0x27,
/*0x28*/    0x28,       0x29,       0x2a,       0x2b,       0x2c,       0x2d,       0x2e,       0x2f,
/*0x30*/    0x30,       0x31,       0x32,       0x33,       0x34,       0x35,       0x36,       0x37,
/*0x38*/    0x38,       0x39,       0x3a,       0x3b,       0x3c,       0x3d,       0x3e,       0x3f,
/*0x40*/    0x40,       0x41,       0x42,       0x43,       0x44,       0x45,       0x46,       0x47,
/*0x48*/    0x48,       0x49,       0x4a,       0x4b,       0x4c,       0x4d,       0x4e,       0x4f,
/*0x50*/    0x50,       0x51,       0x52,       0x53,       0x54,       0x55,       0x56,       0x57,
/*0x58*/    0x58,       0x59,       0x5a,       0x5b,       0x5c,       0x5d,       0x5e,       0x5f,
/*0x60*/    0x60,       0x61,       0x62,       0x63,       0x64,       0x65,       0x66,       0x67,
/*0x68*/    0x68,       0x69,       0x6a,       0x6b,       0x6c,       0x6d,       0x6e,       0x6f,
/*0x70*/    0x70,       0x71,       0x72,       0x73,       0x74,       0x75,       0x76,       0x77,
/*0x78*/    0x78,       0x79,       0x7a,       0x7b,       0x7c,       0x7d,       0x7e,       NON_HEB,
/*0x80*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0x88*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0x90*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0x98*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0xa0*/    0xa0,       NON_HEB,    0xa2,       0xa3,       0xa4,       0xa5,       0xa6,       0xa7,
/*0xa8*/    0xa8,       0xa9,       NON_HEB,    0xab,       0xac,       0xad,       0xae,       NON_HEB,
/*0xb0*/    0xb0,       0xb1,       0xb2,       0xb3,       0xb4,       0xb5,       0xb6,       0xb7,
/*0xb8*/    0xb8,       0xb9,       NON_HEB,    0xbb,       0xbc,       0xbd,       0xbe,       NON_HEB,
/*0xc0*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0xc8*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0xd0*/    0xe0,       0xe1,       0xe2,       0xe3,       0xe4,       0xe5,       0xe6,       0xe7,
/*0xd8*/    0xe8,       0xe9,       0xea,       0xeb,       0xec,       0xed,       0xee,       0xef,
/*0xe0*/    0xf0,       0xf1,       0xf2,       0xf3,       0xf4,       0xf5,       0xf6,       0xf7,
/*0xe8*/    0xf8,       0xf9,       0xfa,       NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,
/*0xf0*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    0xba,
/*0xf8*/    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB,    NON_HEB
};

// HebrewToUcs2 ��ʹ��˵��: 
// ��Ҫ�����޷��� char �ַ� c ת��Ϊ Hebrew UNICODE ��ֵ����ʹ�� Ucs2ToHebrew[c] . ����ֵΪ 0����ʾת��
// ���ɹ�
const AECHAR  HebrewToUcs2[HEBREW_CONVERT_TABLE_SIZE] =
{           /*+0x0      +0x1        +0x2        +0x3        +0x4        +0x5        +0x6        +0x7*/
/*0x00*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0x08*/    0,          0,          0x0a,       0,          0,          0,          0,          0,
/*0x10*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0x18*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0x20*/    0x20,       0x21,       0x22,       0x23,       0x24,       0x25,       0x26,       0x27,
/*0x28*/    0x28,       0x29,       0x2a,       0x2b,       0x2c,       0x2d,       0x2e,       0x2f,
/*0x30*/    0x30,       0x31,       0x32,       0x33,       0x34,       0x35,       0x36,       0x37,
/*0x38*/    0x38,       0x39,       0x3a,       0x3b,       0x3c,       0x3d,       0x3e,       0x3f,
/*0x40*/    0x40,       0x41,       0x42,       0x43,       0x44,       0x45,       0x46,       0x47,
/*0x48*/    0x48,       0x49,       0x4a,       0x4b,       0x4c,       0x4d,       0x4e,       0x4f,
/*0x50*/    0x50,       0x51,       0x52,       0x53,       0x54,       0x55,       0x56,       0x57,
/*0x58*/    0x58,       0x59,       0x5a,       0x5b,       0x5c,       0x5d,       0x5e,       0x5f,
/*0x60*/    0x60,       0x61,       0x62,       0x63,       0x64,       0x65,       0x66,       0x67,
/*0x68*/    0x68,       0x69,       0x6a,       0x6b,       0x6c,       0x6d,       0x6e,       0x6f,
/*0x70*/    0x70,       0x71,       0x72,       0x73,       0x74,       0x75,       0x76,       0x77,
/*0x78*/    0x78,       0x79,       0x7a,       0x7b,       0x7c,       0x7d,       0x7e,       0,
/*0x80*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0x88*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0x90*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0x98*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0xa0*/    0xa0,       0,          0xa2,       0xa3,       0xa4,       0xa5,       0xa6,       0xa7,
/*0xa8*/    0xa8,       0xa9,       0xd7,       0xab,       0xac,       0xad,       0xae,       0x203e,
/*0xb0*/    0xb0,       0xb1,       0xb2,       0xb3,       0xb4,       0xb5,       0xb6,       0xb7,
/*0xb8*/    0xb8,       0xb9,       0xf7,       0xbb,       0xbc,       0xbd,       0xbe,       0,
/*0xc0*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0xc8*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0xd0*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0xd8*/    0,          0,          0,          0,          0,          0,          0,          0,
/*0xe0*/    0x05d0,     0x05d1,     0x05d2,     0x05d3,     0x05d4,     0x05d5,     0x05d6,     0x05d7,
/*0xe8*/    0x05d8,     0x05d9,     0x05da,     0x05db,     0x05dc,     0x05dd,     0x05de,     0x05df,
/*0xf0*/    0x05e0,     0x05e1,     0x05e2,     0x05e3,     0x05e4,     0x05e5,     0x05e6,     0x05e7,
/*0xf8*/    0x05e8,     0x05e9,     0x05ea,     0,          0,          0,          0,          0
};
#endif

/*==============================================================================
                                 
                                �ڲ��������Ͷ���
                                 
==============================================================================*/

/*=============================================================================
                                ��������
==============================================================================*/
static void ALKWSTRLOWER(AECHAR * pszDest);

/*==============================================================================
                                 
                                 ����ȫ�ֱ�������
                                 
==============================================================================*/
#if !defined( AEE_SIMULATOR)
//static call_start_info_type call_start_info_call;
#endif

 static boolean gbArrowFlag = FALSE;
/*==============================================================================

                                 ��������
                                 
==============================================================================*/

/*==============================================================================
����:
    MenuAutoScroll

˵��:
    ʵ�ְ����·����ʱ���˵��Զ�ѭ���������ܡ�

����:
    pMenu [in]��ָ�� IMenuCtl �ӿڵ�ָ�롣
    eCode [in]���¼����롣
    wParam���¼�����

����ֵ:
    none

��ע:

==============================================================================*/
void MenuAutoScroll(IMenuCtl *pMenu, AEEEvent eCode, uint16 wParam)
{
    // �˹����ֿ�ͨ�����ò˵��ؼ�����ʵ�֣�����������ֱ�����ɾ���˺���
#if 0    
    static uint16 wSelItem = 0;
    int nCount;
    uint16 wLastItem, wFirstItem;

    if ((pMenu == NULL) || 
        ((wParam != AVK_UP) && (wParam != AVK_DOWN)))
    {
        return;
    }

    nCount = IMENUCTL_GetItemCount(pMenu);
    if (nCount <= 1)
    {
        return;
    }
    wLastItem  = IMENUCTL_GetItemID(pMenu,nCount - 1);
    wFirstItem = IMENUCTL_GetItemID(pMenu, 0);

    switch (eCode) 
    {
        case EVT_KEY_PRESS:
            wSelItem = IMENUCTL_GetSel(pMenu);
            break;
            
        case EVT_KEY_RELEASE:
            switch (wParam)
            {
                case AVK_UP:
                    if (wSelItem == wFirstItem)
                    {
                        IMENUCTL_SetSel(pMenu, wLastItem);
                        (void)IMENUCTL_Redraw(pMenu);
                    }
                    break;
                    
                case AVK_DOWN:
                    if (wSelItem == wLastItem)
                    {
                        IMENUCTL_SetSel(pMenu, wFirstItem);
                        (void)IMENUCTL_Redraw(pMenu);
                    }
                    break;
                    
                default:
                    break;
            }
            break;
            
        default:
            break;
    }
#endif    
}
static boolean AppsCommon_GetTxtIDFromBarType(BottomBar_Param_type *pBTBarParam, 
      uint16 *pnLeftTxtID,
      uint16 *pnMidTxtID,
      uint16 *pnRightTxtID)
{
   BottomBar_e_Type    eBBarType;
   uint16      nResID_L = 0;// ��
   uint16      nResID_R = 0;// ��
   uint16      nResID_M = 0;// ��

   if(!pBTBarParam || !(pnLeftTxtID || pnMidTxtID || pnRightTxtID))
      return FALSE;

   eBBarType = pBTBarParam->eBBarType;

   if(eBBarType & BTBAR_SINGLE_MASK) {
      switch (eBBarType) {
           // -----����
         case BTBAR_BACK:
            nResID_R = IDS_BACK;
            break;

            // -----ȡ��
         case BTBAR_CANCEL:
            nResID_R = IDS_STRING_CANCEL;
            break;

            // -----�ر�
         case BTBAR_CLOSE:
            nResID_R = IDS_CLOSE;
            break;

            /*guoys add @2008.10.20 for no match dont use select*/    
         case BTBAR_DELETE:
            nResID_R = IDS_DEL;
            break;

            // -----�һ�: 
         case BTBAR_END:
            nResID_R = IDS_END;
            break;

            // ---------��һ��
         case BTBAR_NEXT:
            nResID_R = IDS_NEXT;
            break;

            //ADD XIAO.CHENG
         case BTBAR_OK:
            nResID_L = IDS_OK;
            break; 
         
         case BTBAR_PROMPT_OK:
            nResID_M = IDS_OK;
            break;

            // -----��������
         case BTBAR_SOS:
            nResID_R = IDS_SOS;
            break;

            // -----ֹͣ
         case BTBAR_STOP:
            nResID_R = IDS_STOP;
            break;
			
		case BTBAR_OPTION:
			nResID_L = IDS_OPTION;
            break;
		case BTBAR_SNOOZE:    
			nResID_L = IDS_SNOOZE;
            nResID_R = IDS_STOP;
            break;

         default:
            break;
      }
   }
   else if(eBBarType & BTBAR_DOUBLE_MASK) {
      switch (eBBarType) {
         /*jinqing add,2009/01/08*/
         case BTBAR_ACCEPT_REJECT:
            nResID_L = IDS_BLUETOOTH_ACCEPT;
            nResID_R = IDS_REJECT;  
            break;

         case BTBAR_ADD_BACK:
            nResID_L=IDS_ADD;
            nResID_R=IDS_BACK;
            break;

         case BTBAR_ANSWER_IGNORE:
            nResID_L = IDS_ANSWER;
            nResID_R = IDS_IGNORE;
            break;

         case BTBAR_ANSWER_REJECT:
            nResID_L = IDS_ANSWER;
            nResID_R = IDS_REJECT;
            break;

         case BTBAR_ANSWER_MUTE:
            nResID_L = IDS_ANSWER;
            nResID_R = IDS_MUTE;
            break;

         case BTBAR_ANSWER_UNMUTE:
            nResID_L = IDS_ANSWER;
            nResID_R = IDS_UNMUTE;
            break;

         case BTBAR_BACK_DELETE:
            nResID_L = IDS_BACK;
            nResID_R = IDS_DEL;
            break;

            // ����-----��������: 
         case BTBAR_BACK_SOS:
            nResID_L = IDS_BACK;
            nResID_R = IDS_SOS;
            break;
         case BTBAR_MENU_SOS:
         	nResID_L = IDS_MENU;
            nResID_R = IDS_SOS;
         	break;
         case BTBAR_STOP_BACK:
            nResID_R = IDS_BACK; 
            nResID_L = IDS_STOP;
            break;     

         case BTBAR_CALL_BACK:
            nResID_L = IDS_CALL;
            nResID_R = IDS_BACK;  
            break;

            // ����-----����
         case BTBAR_CONTINU_BACK:
            nResID_L = IDS_CONTINU;
            nResID_R = IDS_BACK;
            break;

            // ����-----ɾ��
         case BTBAR_CONTINU_DELETE:
            nResID_L = IDS_CONTINU;
            nResID_R = IDS_DEL;
            break;

         case BTBAR_CREATE_BACK:
            nResID_L=IDS_CREATE;
            nResID_R=IDS_BACK;
            break;             

            // ɾ��---����
         case BTBAR_EARSE_BACK:
            nResID_L = IDS_DEL;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_EXCHANGE_BACK:
            nResID_L = IDS_EXCHANGE;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_EXCHANGE_DELETE:
            nResID_L = IDS_EXCHANGE;
            nResID_R = IDS_DEL;  
            break;          

            // �༭-----����
         case BTBAR_EDIT_BACK:
            nResID_L = IDS_EDIT;
            nResID_R = IDS_BACK;
            break;

            // ����-----����
         case BTBAR_FIND_BACK:
            nResID_L = IDS_FIND;
            nResID_R = IDS_BACK;
            break;
         case BTBAR_GALLERY_BACK:
            nResID_L = IDS_GALLERY;            
            nResID_R = IDS_BACK;
            break;              
            
         case BTBAR_LEFT_START_NULL:
            nResID_M = IDS_START;
            nResID_R = IDS_BACK;
            break;
                
            //�˵�---�绰��
         case BTBAR_MENU_CONTACTS:
            nResID_L = IDS_MENU;
            nResID_R = IDS_STRING_CONTACTS;
            break;
					
		case BTBAR_MENU_FRENDUO:
			nResID_L = IDS_MENU;
			nResID_R = IDS_FRENDUO_LIST;
			break;
		
		case BTBAR_FACEBOOK_CHAT:
			nResID_L = IDS_FACEBOOK;
			nResID_R = IDS_CHAT;
			break;
		
		case BTBAR_VIEWMORE_BACK:
            nResID_L = IDS_MORE;
			nResID_M = IDS_OK;
			nResID_R = IDS_BACK;
			break;		

            // �˵�-----����
         case BTBAR_MENU_BACK:
            nResID_L = IDS_MENU;
            nResID_R = IDS_BACK;
            break;

            // ȷ��-----����
         case BTBAR_OK_BACK:
            nResID_L = IDS_OK;
            nResID_R = IDS_BACK;
            break;

            // ȷ��-----ȡ��
         case BTBAR_OK_CANCEL:
            nResID_L = IDS_OK;
            nResID_R = IDS_CANCEL;
            break;
                      
            // ȷ��-----ɾ��
         case BTBAR_OK_DEL:
         case BTBAR_OK_DELETE:
            nResID_L = IDS_OK;
            nResID_R = IDS_DEL;
            break;

            // ѡ��-----����
         case BTBAR_OPTION_BACK:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_OPTION_STOP:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_STOP;
            break;
            
            // ѡ��-----ɾ��
         case BTBAR_OPTION_DELETE:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_DEL;
            break; 

         case BTBAR_OPTION_DEL:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_DEL;
            break;   

            //ѡ��---�һ�:
         case BTBAR_OPTION_ENDCALL:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_STRING_END_CALL;
            break;

         case BTBAR_OPTION_HANDSFREEON:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_HANDSFREE_ON;  
            break;
		

         case BTBAR_OPTION_NORMAL:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_HANDSFREE_OFF;  
            break;

         case BTBAR_OPTION_UP:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_UP;
            break;

         case BTBAR_PAUSE_BACK:
            nResID_L = IDS_PAUSE;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_PLAY_BACK:
            nResID_M = IDS_PLAY;            
            nResID_R = IDS_BACK;
            break;  
			
		 case BTBAR_0PTION_PLAY_BACK:
		 	nResID_L = IDS_OPTION;
		 	nResID_M = IDS_PLAY;            
            nResID_R = IDS_BACK;
		 	break;

         case BTBAR_PLAY_STOP:
            nResID_M = IDS_PLAY;            
            nResID_R = IDS_STOP;
            break; 

         case BTBAR_GALLERY_PLAY_BACK:
            nResID_L = IDS_GALLERY;
            nResID_M = IDS_PLAY;            
            nResID_R = IDS_BACK;
            break;

            // �Ķ�-----����
         case BTBAR_READ_BACK:
            nResID_L = IDS_READ;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_RECORD_STOP:
            nResID_L = IDS_RECORD;
            nResID_R = IDS_STOP;
            break;

         case BTBAR_REPLAY_BACK:
            nResID_L = IDS_REPLAY;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_RESET_BACK:
            nResID_L = IDS_RESET;
            nResID_R = IDS_BACK;
            break; 

         case BTBAR_RESUME_BACK:
            nResID_L = IDS_RESUME;
            nResID_R = IDS_BACK;  
            break;

         case BTBAR_RETRY_CANCEL:
            nResID_L = IDS_RETRY;
            nResID_R = IDS_STRING_CANCEL;
            break;

            // ����-----����
         case BTBAR_SAVE_BACK:
            nResID_L = IDS_SAVE;
            nResID_R = IDS_BACK;
            break;

            // ����-----ȡ��
         case BTBAR_SAVE_CANCEL:
            nResID_L = IDS_SAVE;
            nResID_R = IDS_BACK;
            break;            

            // ����-----ɾ��
         case BTBAR_SAVE_DELETE:
            nResID_L = IDS_SAVE;
            nResID_R = IDS_DEL;
            break;

            // ѡ��-----����
         case BTBAR_SELECT_BACK:
            nResID_L = IDS_OK;	//SELECT;
            nResID_R = IDS_BACK;
            break;
            //ѡ��--------ɾ��
         case BTBAR_SELECT_DEL:
            nResID_L = IDS_OK; //IDS_SELECT;
            nResID_R = IDS_DEL;
            break;

         case BTBAR_SELECT_STOP:
            nResID_L = IDS_OK; //IDS_SELECT;
            nResID_R = IDS_STOP;
            break;

            // ����---ɾ��
         case BTBAR_SEND_EARSE:
            nResID_L = IDS_SEND;
            nResID_R = IDS_DEL;
            break;

            // ����---����
         case BTBAR_SEND_BACK:
            nResID_L = IDS_SEND;
            nResID_R = IDS_BACK;
            break;  

         case BTBAR_SNOOZE_STOP:
            nResID_L = IDS_SNOOZE;
            nResID_R = IDS_STOP;
            break;

         case BTBAR_START_BACK:
            nResID_L = IDS_START;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_SNOOZE_DONE:
            nResID_L = IDS_SNOOZE;
            nResID_R = IDS_POP_DONE;
            break;

         case BTBAR_UNLOCK_SOS:
            nResID_R = IDS_STRING_UNLOCK;
            nResID_L = IDS_SOS;
            break;
		case BTBAR_UNLOCK:
			nResID_R = IDS_STRING_UNLOCK;
			break;
        case BTBAR_LUNLOCK:
            nResID_L = IDS_STRING_UNLOCK;
            nResID_R = IDS_SOS;
            break;
#ifdef FEATURE_SPORTS_APP
         case BTBAR_PAUSE_STOP:
            nResID_L = IDS_PAUSE;
            nResID_R = IDS_STOP;
            break;

         case BTBAR_RESUME_STOP:
            nResID_L = IDS_RESUME;
            nResID_R = IDS_STOP;
            break;

         case BTBAR_SPORT_STOP:
            nResID_L = IDS_SPORT;
            nResID_R = IDS_STOP;
            break;            
#endif

         case BTBAR_VIEW_SNOOZE:         
            nResID_L = IDS_VIEW;
            nResID_R = IDS_SNOOZE;
            break;

         case BTBAR_VIEW_STOP:         
            nResID_L = IDS_VIEW;
            nResID_R = IDS_STOP;
            break;
#ifdef FEATURE_FLEXI_STATIC_BREW_APP
    	case BTBAR_FNASRANI_FPORTAL:
    		nResID_L = IDS_FNASRANI;
    		nResID_R = IDS_FPORTAL;
    		break;

    	case BTBAR_FMUSLIM_FPORTAL:
    		nResID_L = IDS_FMUSLIM;
    		nResID_R = IDS_FPORTAL;
    		break;

    	case BTBAR_FGURU_FPORTAL:
    		nResID_L = IDS_FGURU;
    		nResID_R = IDS_FPORTAL;
    		break;
    		
    	case BTBAR_CONTACTS_FPORTAL:
    		nResID_L = IDS_STRING_CONTACTS;
    		nResID_R = IDS_FPORTAL;
    		break;            
    	case BTBAR_OPTION_SAVE_BACK:
            nResID_L = IDS_OPTION;
            nResID_M = IDS_SAVE;
            nResID_R = IDS_BACK;
            break;
#endif
    	case BTBAR_FMENU_CANTACT:
    		nResID_L = IDS_FMENU;
    		nResID_R = IDS_STRING_CONTACTS;
    		break; 
    	case BTBAR_MENU_FMENU:
    		nResID_L = IDS_MENU;
    		nResID_R = IDS_FMENU;
    		break;
		case BTBAR_YES_NO:
            nResID_L = IDS_YES;
            nResID_R = IDS_NO;
            break;
         default:
            break;
      }
   }
   else if(eBBarType & BTBAR_TRI_MASK) {
      switch (eBBarType) {
         case BTBAR_CANCEL_RESUME_NULL:
            nResID_L = IDS_RESET;
            nResID_M = IDS_RESUME;
            nResID_R = IDS_CANCEL;
            break;
            
         case BTBAR_FULLSCREEN_PAUSE_STOP:
            nResID_L = IDS_FULLSCREEN; 
            nResID_M = IDS_PAUSE;            
            nResID_R = IDS_STOP;
            break;  
            
         case BTBAR_JEWISH_TODAY_BACK:
            nResID_L = IDS_JEWISH;
            nResID_M = IDS_TOTODAY;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_OPTION_CREATE_BACK:
            nResID_L = IDS_OPTION;
            nResID_M = IDS_CREATE;
            nResID_R = IDS_BACK;
            break;

            // for FM Radio list using
         case BTBAR_OPTION_EDIT_BACK:
            {
               nResID_L = IDS_OPTION_FM; 
               nResID_M = IDS_EDIT;
               nResID_R = IDS_FM_BACK; // IDS_BACK;
            }
            break;

         case BTBAR_OPTION_OK_BACK:
            nResID_L = IDS_OPTION;
            nResID_M = IDS_OK;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_OPTION_OK_DEL:
            nResID_L = IDS_OPTION;
            nResID_M = IDS_OK;
            nResID_R = IDS_DEL;
            break;

         case BTBAR_OPTION_PAUSE_STOP:
            nResID_L = IDS_OPTION;
            nResID_M = IDS_PAUSE;
            nResID_R = IDS_STOP;
            break;
            
         case BTBAR_OPTION_PLAY_BACK:
            nResID_L=IDS_OPTION;
            nResID_M=IDS_PLAY;
            nResID_R=IDS_BACK;
            break;

         case BTBAR_OPTION_PLAY_STOP:
            nResID_L = IDS_OPTION;
            nResID_M = IDS_PLAY;
            nResID_R = IDS_STOP;
            break;

         case BTBAR_OPTION_SAVE_DEL:
            nResID_L = IDS_OPTION;
            nResID_M = IDS_SAVE;
            nResID_R = IDS_DEL;
            break;
		case BTBAR_OPTION_SAVE_BACK:
			nResID_L = IDS_OPTION;
            nResID_M = IDS_SAVE;
            nResID_R = IDS_BACK;
            break;
            // option---����---����
         case BTBAR_OPTION_TODAY_BACK:
            nResID_L = IDS_OPTION;
            nResID_M = IDS_TOTODAY;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_SAVE_AM_BACK:
            nResID_L = IDS_SAVE;
            nResID_M = IDS_APPSCOMMON_AM;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_SAVE_IP_DELETE:
            nResID_L = IDS_SAVE;
            nResID_M = IDS_STRING_IP;
            nResID_R = IDS_DEL;
            break;

         case BTBAR_SAVE_PM_BACK:
            nResID_L = IDS_SAVE;
            nResID_M = IDS_APPSCOMMON_PM;
            nResID_R = IDS_BACK;
            break;

            // ѡ��---ɾ��---����
         case BTBAR_SELECT_DEL_BACK:
            nResID_L = IDS_SELECT; //IDS_SELECT;
            nResID_M = IDS_DEL; //IDS_DEL;
            nResID_R = IDS_BACK;
            break;

            // ѡ��---ȷ��---����
         case BTBAR_SELECT_OK_BACK:
            nResID_L = IDS_SELECT; //IDS_SELECT;
            nResID_M = IDS_OK; //IDS_OK;
            nResID_R = IDS_BACK;
            break;

            // ѡ��---ȷ��---ɾ��
         case BTBAR_SELECT_OK_DEL:
            nResID_L = IDS_SELECT; //IDS_SELECT;
            nResID_M = IDS_OK; //IDS_OK;
            nResID_R = IDS_DEL;
            break;      

            // ѡ��---����---����
         case BTBAR_SELECT_SET_BACK:
            nResID_L = IDS_SELECT; //IDS_SELECT;
            nResID_M = IDS_SETTING;
            nResID_R = IDS_BACK;
            break;

         case BTBAR_SNOOZE_CONTINUE_STOP:
            nResID_L = IDS_SNOOZE;
            nResID_M = IDS_CONTINU;
            nResID_R = IDS_STOP;
            break;

         case BTBAR_CONTINUE_RESET_BACK:
            nResID_L = IDS_CONTINU;
            nResID_M = IDS_RESET;
            nResID_R = IDS_BACK;
            break;
            
        case BTBAR_CALL_MODIFY_BACK:
            nResID_L = IDS_CALL;
            nResID_M = IDS_MODIFY;            
            nResID_R = IDS_BACK;  
            break;

        case BTBAR_YES_NO_CANCEL:
            nResID_L = IDS_YES;
            nResID_M = IDS_NO;
            nResID_R = IDS_CANCEL;
         default:
            break;
      }
   }
   else {
      return FALSE;
   }

   if(pnLeftTxtID )
      *pnLeftTxtID = nResID_L;

   if(pnMidTxtID)
      *pnMidTxtID = nResID_M;

   if(pnRightTxtID)
      *pnRightTxtID = nResID_R;

   if(nResID_L != 0 || nResID_M != 0 || nResID_R != 0)
      return  TRUE;
   else
      return FALSE;

}//AppsCommon_GetTxtIDFromBarType

/*==============================================================================
����:
    DrawBottomBar

˵��:
    ���������ڻ���������صĵײ���ʾ����

����:
    pIDisplay [in]: ָ�� IDisplay �ӿڵ�ָ�롣
    BParam [in]: �ײ���ʾ����ز���ָ�롣

����ֵ:
    none

��ע:
    ���������ڻ���������صĵײ���ʾ���������޹صĵײ���ʾͼƬ�������л��ƣ���
    Ҫ���ô˺�����ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ����
    ֧�ֵײ���3����ť��ʾ��
==============================================================================*/
void DrawBottomBar(IDisplay  * pIDisplay, BottomBar_Param_type *BParam)
{
#ifdef FEATURE_FUNCS_BOTTOM_BAR
    IImage      *pBarImg = NULL;
    uint16      nResID_L = 0;// ��
    uint16      nResID_R = 0;// ��
    uint16      nResID_M = 0;// ��
    AEERect     rc;
    AECHAR      wszBar_L[20]={0};// ��
    AECHAR      wszBar_R[20]={0};// ��
    AECHAR      wszBar_M[20]={0};// ��
#ifdef FEATURE_FUNCS_THEME    
    RGBVAL      oldColor;
    Theme_Param_type Theme_Param;
#endif /* FEATURE_FUNCS_THEME */    
    IShell      *pShell = AEE_GetShell();
#ifdef FEATURE_RANDOM_MENU_COLOR
    RGBVAL  nBgColor = APPSCOMMON_BG_COLOR;
#endif
    
    if ((NULL == pShell) || (NULL == pIDisplay) || (NULL == BParam))
    {
        return;
    }

#ifdef FEATURE_FUNCS_THEME    
    // ��ȡ�������
    Appscom_GetThemeParameters(&Theme_Param);
#endif /* FEATURE_FUNCS_THEME */    
    
    // ȷ�������ľ���
    if (NULL != BParam->prc)
    {
        rc = *(BParam->prc);
        /* ǿ�ƾ���*/
        rc.x = 0;
        rc.dx = DISP_WIDTH;
    }
    else
    {
        AEEDeviceInfo devinfo;
        int nBarH = GetBottomBarHeight(pIDisplay);
        
        MEMSET(&devinfo, 0, sizeof(devinfo));
        ISHELL_GetDeviceInfo(pShell, &devinfo);
        SETAEERECT(&rc, 0, devinfo.cyScreen-nBarH, devinfo.cxScreen, nBarH);
    }
   
    AppsCommon_GetTxtIDFromBarType(BParam, &nResID_L, &nResID_M, &nResID_R); 

   // ���ذ�ť�ı�
    if (nResID_L != 0)
    {
        (void) ISHELL_LoadResString(pShell,
                                    AEE_APPSCOMMONRES_LANGFILE,
                                    nResID_L,
                                    wszBar_L,
                                    sizeof(wszBar_L));
    }
    if (nResID_R != 0)
    {
        (void) ISHELL_LoadResString(pShell,
                                    AEE_APPSCOMMONRES_LANGFILE,
                                    nResID_R,
                                    wszBar_R,
                                    sizeof(wszBar_R));
    }
    if (nResID_M != 0)
    {
        (void) ISHELL_LoadResString(pShell,
                                    AEE_APPSCOMMONRES_LANGFILE,
                                    nResID_M,
                                    wszBar_M,
                                    sizeof(wszBar_M));
    }

//Del By zzg 2010_07_14
/*	
#ifdef FEATURE_RANDOM_MENU_COLOR
    if(SUCCESS == OEM_GetConfig(CFGI_MENU_BGCOLOR, &nBgColor, sizeof(nBgColor)))
    {
        int i, nColorHeight = 14;
        AEERect rect;
        uint32 nRVal = GET_RGB_R(nBgColor),
                 nGVal = GET_RGB_G(nBgColor),
                 nBVal = GET_RGB_B(nBgColor);

        SETAEERECT(&rect, 0, rc.y + nColorHeight, DISP_WIDTH, rc.dy - nColorHeight);

        IDISPLAY_FillRect(pIDisplay, &rect, nBgColor);
        rect.y = rc.y + nColorHeight - 1;
        rect.dy = 1;

        for(i = 0; i < nColorHeight; i++, rect.y--)
        {
            nRVal = MIN(255, (nRVal+8));
            nGVal = MIN(255, (nGVal+8));
            nBVal = MIN(255, (nBVal+8));
            nBgColor = MAKE_RGB(nRVal, nGVal, nBVal);
            IDISPLAY_FillRect(pIDisplay, &rect, nBgColor);
        }
    }
    // ��ȡ��ɫ���ɹ�ʱ�ż���ͼƬ
    else
#endif// FEATURE_RANDOM_MENU_COLOR
*/
    {
        // ���ص�������ͼƬ
        if ((STRLEN(BParam->strImgResFile) > 0) && 
            (BParam->nImgResID != 0))
        {// �û������������
            pBarImg = ISHELL_LoadResImage(pShell, 
                                          BParam->strImgResFile, 
                                          BParam->nImgResID);
        }
#ifdef FEATURE_FUNCS_THEME

        else if ((STRLEN(Theme_Param.strBBarImgResFile) > 0) && 
                 (0 != Theme_Param.nBBarImgResID))
        {// ���������֮
            pBarImg = ISHELL_LoadResImage(pShell, 
                                          Theme_Param.strBBarImgResFile, 
                                          Theme_Param.nBBarImgResID);
        }
         else
        {// ���������֮
            pBarImg = ISHELL_LoadResImage(pShell, 
                                          AEE_APPSCOMMONRES_IMAGESFILE, 
                                          IDI_BOTTOMBAR);
        }
#else
        else
        {
            pBarImg = ISHELL_LoadResImage(pShell,
                                          AEE_APPSCOMMONRES_IMAGESFILE,
                                          IDI_BOTTOMBAR);
        }
#endif /* FEATURE_FUNCS_THEME */   

        // ���Ƶײ���ʾ��
        if (NULL != pBarImg)
        {
            IIMAGE_Draw(pBarImg, 0, rc.y);
            
            IIMAGE_Release(pBarImg);
            pBarImg = NULL;
        } 

#ifdef FEATURE_FUNCS_THEME
        else
        {
            IDISPLAY_FillRect(pIDisplay, &rc, Theme_Param.themeColor);
        }
#endif
    }

#ifdef FEATURE_FUNCS_THEME
    oldColor = IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, Theme_Param.textColor);
#else

    (void)IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_WHITE);

#endif /* FEATURE_FUNCS_THEME */    
    
#if !defined( FEATURE_CARRIER_ISRAEL_PELEPHONE)  // modify the code on 080922
    rc.x+=5;
    rc.dx-=10;
#endif
    (void)IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_WHITE);
    // �����ı�-���
    if (WSTRLEN(wszBar_L)>0)
    {
        (void) IDISPLAY_DrawText(pIDisplay, 
                    AEE_FONT_BOLD, 
                    wszBar_L, 
                    -1, 
                    0, 
                    0, 
                    &rc, 
                    IDF_ALIGN_BOTTOM | IDF_ALIGN_LEFT | IDF_TEXT_TRANSPARENT);
    }
    
    // �����ı�-�Ҽ�
    if (WSTRLEN(wszBar_R)>0)
    {
        (void) IDISPLAY_DrawText(pIDisplay, 
                    AEE_FONT_BOLD, 
                    wszBar_R, 
                    -1, 
                    0, 
                    0, 
                    &rc, 
                    IDF_ALIGN_BOTTOM | IDF_ALIGN_RIGHT | IDF_TEXT_TRANSPARENT);
    }
    
    // �����ı�-�м��
    if (WSTRLEN(wszBar_M)>0)
    {
#ifdef FEATURE_FUNCS_THEME
        (void) IDISPLAY_DrawText(pIDisplay, 
                    AEE_FONT_BOLD, 
                    wszBar_M, 
                    -1, 
                    0, 
                    0, 
                    &rc, 
                    IDF_ALIGN_BOTTOM | IDF_ALIGN_CENTER | IDF_TEXT_TRANSPARENT);
#else
/*
        rc.y -= 2;
        (void)IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_BLACK);
        DrawTextWithProfile(pShell, 
                            pIDisplay, 
                            RGB_WHITE_NO_TRANS, 
                            AEE_FONT_BOLD, 
                            wszBar_M, 
                            -1, 
                            0, 
                            0, 
                            &rc, 
                            IDF_ALIGN_BOTTOM|IDF_ALIGN_CENTER|IDF_TEXT_TRANSPARENT);
        (void)IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_WHITE);
        rc.y += 2;
        */  //modi by yangdecai
            (void) IDISPLAY_DrawText(pIDisplay, 
                    AEE_FONT_BOLD, 
                    wszBar_M, 
                    -1, 
                    0, 
                    0, 
                    &rc, 
                    IDF_ALIGN_BOTTOM | IDF_ALIGN_CENTER | IDF_TEXT_TRANSPARENT);
#endif
    }

    if (gbArrowFlag)
    {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE 
        // ���������֮
        pBarImg = ISHELL_LoadResImage(pShell, 
                                      AEE_APPSCOMMONRES_IMAGESFILE,
                                      IDB_EDIT_UP_DOWN_BMP);

        // ���Ƶײ���ʾ�������¼�ͷ
        if (NULL != pBarImg)
        {
            IIMAGE_Draw(pBarImg, 61, 117);
            IIMAGE_Release(pBarImg);
            pBarImg = NULL;
        }
#else
    	wszBar_M[0] = 0x2195;
   	    wszBar_M[1] = 0;
    	(void) IDISPLAY_DrawText(pIDisplay, 
                    AEE_FONT_BOLD, 
                    wszBar_M, 
                    -1, 
                    0, 
                    0, 
                    &rc, 
                    IDF_ALIGN_BOTTOM | IDF_ALIGN_CENTER | IDF_TEXT_TRANSPARENT);
#endif
    }    
    
#ifdef FEATURE_FUNCS_THEME    
    // �ָ���ʾ�ı���ɫ
    (void)IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, oldColor);
#else
    (void)IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_BLACK);
#endif /* FEATURE_FUNCS_THEME */    
#endif /* FEATURE_FUNCS_BOTTOM_BAR */
	IDisplay_Update(pIDisplay);
}                       

/*==============================================================================
����:
    DrawTitleBar

˵��:
    ���������ڻ�������ʾ����

����:
    pIDisplay [in]: ָ�� IDisplay �ӿڵ�ָ�롣
    TParam [in]: ��������ز���ָ�롣

����ֵ:
    none

��ע:
    ����������?�����������ʾ��ͼƬ���Է���Դ�ļ����������ص��ֻ������Ƶı����ı?
    ���ɲ������롣ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ�
==============================================================================*/
void DrawTitleBar(IDisplay  * pIDisplay, TitleBar_Param_type *TParam)
{
#ifdef FEATURE_FUNCS_TITLE_BAR
    AECHAR      *pwszTitle = NULL;
    IImage      *pBarImg = NULL;
    AEERect     rc;
    uint32      dwFlags;
#ifdef FEATURE_FUNCS_THEME    
    Theme_Param_type    Theme_Param;
#endif /* FEATURE_FUNCS_THEME */    
    IShell  *pShell = AEE_GetShell();
#ifdef FEATURE_RANDOM_MENU_COLOR
    RGBVAL  nBgColor = APPSCOMMON_BG_COLOR;
#endif
    
    if ((NULL == pShell) || (NULL == pIDisplay) || (NULL == TParam))
    {
        return;
    }

#ifdef FEATURE_FUNCS_THEME    
    // ��ȡ�������
    Appscom_GetThemeParameters(&Theme_Param);
#endif /* FEATURE_FUNCS_THEME */    
    
    // ȷ���������ľ���
    if (NULL != TParam->prc)
    {
        rc = *(TParam->prc);
        /* ǿ�ƾ���*/
        rc.x = 0;
        rc.dx = DISP_WIDTH;
    }
    else
    {
        AEEDeviceInfo devinfo;
        int     nBarH = GetTitleBarHeight(pIDisplay);
        
        MEMSET(&devinfo, 0, sizeof(devinfo));
        ISHELL_GetDeviceInfo(pShell, &devinfo);
        SETAEERECT(&rc, 0, 0, devinfo.cxScreen, nBarH);
    }

//Del By zzg 2010_07_14
/*
#ifdef FEATURE_RANDOM_MENU_COLOR
    if(SUCCESS == OEM_GetConfig(CFGI_MENU_BGCOLOR, &nBgColor, sizeof(nBgColor)))
    {
        int i, nColorHeight = 19;
        AEERect rect;
        uint32 nRVal = GET_RGB_R(nBgColor),
                 nGVal = GET_RGB_G(nBgColor),
                 nBVal = GET_RGB_B(nBgColor);

        SETAEERECT(&rect, 0, rc.y + nColorHeight, DISP_WIDTH, rc.dy - nColorHeight);

        IDISPLAY_FillRect(pIDisplay, &rect, nBgColor);
        rect.y = rc.y + nColorHeight - 1;
        rect.dy = 1;

        for(i = 0; i < nColorHeight; i++, rect.y--)
        {
            nRVal = MIN(255, (nRVal+6));
            nGVal = MIN(255, (nGVal+6));
            nBVal = MIN(255, (nBVal+6));
            nBgColor = MAKE_RGB(nRVal, nGVal, nBVal);
            IDISPLAY_FillRect(pIDisplay, &rect, nBgColor);
        }
    }
    // ��ȡ��ɫ���ɹ�ʱ�ż���ͼƬ
    else
#endif// FEATURE_RANDOM_MENU_COLOR
*/
    {
        if ((STRLEN(TParam->strImgResFile) > 0) && 
            (0 != TParam->nImgResID))
        {// �û������������
            pBarImg = ISHELL_LoadResImage(pShell, 
                                          TParam->strImgResFile, 
                                          TParam->nImgResID);
        }
#ifdef FEATURE_FUNCS_THEME

        else if ((STRLEN(Theme_Param.strTBarImgResFile) > 0) && 
                 (0 != Theme_Param.nTBarImgResID))
        {// ���������֮
            pBarImg = ISHELL_LoadResImage(pShell, 
                                          Theme_Param.strBBarImgResFile, 
                                          Theme_Param.nBBarImgResID);
        }
         else
        {// ���������֮
            pBarImg = ISHELL_LoadResImage(pShell, 
                                          AEE_APPSCOMMONRES_IMAGESFILE, 
                                          IDI_TITLEBAR);
        }
#else
        else
        {
            pBarImg = ISHELL_LoadResImage(pShell,
                                          AEE_APPSCOMMONRES_IMAGESFILE,
                                          IDI_TITLEBAR);
        }
#endif /* FEATURE_FUNCS_THEME */    
//IDISPLAY_UpdateEx(pIDisplay, FALSE);//wlh
        // ���Ʊ���������
        if (NULL != pBarImg)
        {
            IIMAGE_Draw(pBarImg, 0, rc.y);
            IIMAGE_Release(pBarImg);
            pBarImg = NULL;
        }
//IDISPLAY_UpdateEx(pIDisplay, FALSE);//wlh 
#ifdef FEATURE_FUNCS_THEME    
        else
        {
            IDISPLAY_FillRect(pIDisplay, &rc, Theme_Param.themeColor);
        }
#endif /* FEATURE_FUNCS_THEME */
    }

    /*��������������ͷ*/
    if(TParam->eTBarType == TBAR_ARROW)
    {
        IImage *pArrow = NULL;
        AEEImageInfo iInfo = {0};

        pArrow = ISHELL_LoadResImage(pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_LEFTARROW);
        if(pArrow != NULL)
        {
            IIMAGE_GetInfo(pArrow, &iInfo);
            IImage_Draw(pArrow, 5, (rc.dy - iInfo.cy)/2);
            IImage_Release(pArrow);
            pArrow = NULL;
        }
        pArrow = ISHELL_LoadResImage(pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_RIGHTARROW);
        if(pArrow != NULL)
        {
            IIMAGE_GetInfo(pArrow, &iInfo);
            IImage_Draw(pArrow, rc.dx - 5 - iInfo.cx, (rc.dy - iInfo.cy)/2);
            IImage_Release(pArrow);
            pArrow = NULL;
        }
    }
//IDISPLAY_UpdateEx(pIDisplay, FALSE);//wlh 
    // ���Ʊ������ı�
    if (NULL != TParam->pwszTitle)
    {
        pwszTitle = TParam->pwszTitle;
    }
    else if ((TParam->strTitleResFile != NULL) && (0 != TParam->nTitleResID))
    {
        int32  dwSize = 0;
        
        dwSize = 64 * sizeof(AECHAR);
        pwszTitle = (AECHAR *)MALLOC(dwSize);
        if (NULL != pwszTitle)
        {
            (void)ISHELL_LoadResString(pShell, 
                                       TParam->strTitleResFile, 
                                       TParam->nTitleResID, 
                                       pwszTitle, 
                                       dwSize);
        }
    }
    if (NULL != pwszTitle)
    {
#ifdef FEATURE_FUNCS_THEME    
        RGBVAL oldColor = IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, Theme_Param.textColor);
#else
        IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_WHITE);
#endif /* FEATURE_FUNCS_THEME */    
        
        dwFlags = TParam->dwAlignFlags | 
                  IDF_TEXT_TRANSPARENT |
                  IDF_ALIGN_MIDDLE;

        // ���Ʊ����ı�
        (void) IDISPLAY_DrawText(pIDisplay, 
                    AEE_FONT_NORMAL, 
                    pwszTitle, 
                    -1, 
                    0, 
                    0, 
                    &rc, 
                    dwFlags);
       
        if (NULL == TParam->pwszTitle)
        {
            FREE(pwszTitle);
        }
        
#ifdef FEATURE_FUNCS_THEME    
        // �ָ���ʾ�ı���ɫ
        (void)IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, oldColor);
#else
        (void)IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_BLACK);
#endif /* FEATURE_FUNCS_THEME */    
    }
#endif /* FEATURE_FUNCS_TITLE_BAR */
//IDISPLAY_UpdateEx(pIDisplay, FALSE);//wlh 
}
/*==============================================================================
����:
    DrawBackground

˵��:
    ���������ڻ��ؼ�����ͼ��

����:
    pDisplay   [in]��IDisplayָ�롣
    pRect      [in]��Ŀ�������С��

����ֵ:
    none

��ע:
    ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ�
==============================================================================*/
void drawTheImage( IImage *image, AEERect *pRect)
{

    IIMAGE_SetOffset( image, pRect->x, pRect->y);
    IIMAGE_SetDrawSize( image, pRect->dx, pRect->dy);
    IIMAGE_Draw( image, pRect->x, pRect->y);
    IIMAGE_Release( image);
}

void DrawBackground( IDisplay *pDisplay, AEERect *pRect)
{     
    IImage *image = ISHELL_LoadResImage( AEE_GetShell(),
                            AEE_APPSCOMMONRES_IMAGESFILE,
                            IDB_BACKGROUND
                           );
    if( image == NULL)
    {
        MSG_FATAL( ";DrawBackground, load wall paper failed",0,0,0);
    }
    else
    {
        drawTheImage( image, pRect);
    }
}

/*==============================================================================
����:
    DrawGreyBitTextWithProfile

˵��:
    �������ڻ��ƴ��������ı���

����:
    pShell [in]��IShell �ӿ�ָ�롣
    pDisplay [in]��IDisplay �ӿ�ָ�롣
    ProfileColor [in]���ı�������RGBVAL��ɫֵ��
    ���� Font��pcText��nChars��x��y��prcBackground��dwFlags ����ͬ�ӿں���
    IDISPLAY_DrawText ��

����ֵ:
    none

��ע:
    ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ�
==============================================================================*/
extern int GreyBitBrewFont_DrawText(IDisplay *p, int nSize, const AECHAR *psz, int nl, int x, int y, const AEERect *prcb, uint32 flags);

void DrawGreyBitTextWithProfile(IShell* pShell, 
    IDisplay * pDisplay, 
    RGBVAL ProfileColor, 
    int Font, 
    const AECHAR * pcText, 
    int nChars,
    int x, 
    int y, 
    const AEERect * prcBackground, 
    uint32 dwFlags
)
{
	RGBVAL oldTextClr;
	AEERect ClipRc;
	AEERect rc;
	AEEDeviceInfo di;

	// ��������������������Ӧ��������
	if ((NULL == pDisplay) || 
		(NULL == pcText) || 
		(NULL == pShell))
	{
		return;
	}

	

	#if 1
	// ������ı�ʱ�ļ��о���
	ISHELL_GetDeviceInfo(pShell, &di);
	ClipRc.x = 0;
	ClipRc.y = 0;
	ClipRc.dx = di.cxScreen;
	ClipRc.dy = di.cyScreen;
	if (NULL != prcBackground)	
	{
		ClipRc = *prcBackground;
	}
	
	rc = ClipRc;
	
	// �����ı��ı�
	if (dwFlags & IDF_ALIGN_RIGHT)
	{
		rc.dx = ClipRc.dx - 1;
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y, 
					&rc, 
					dwFlags);
					
		rc.dx = ClipRc.dx + 1;
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y, 
					&rc, 
					dwFlags);
	}
	else if ( (dwFlags & IDF_ALIGN_LEFT) || (dwFlags & IDF_ALIGN_CENTER) )
	{
		rc.x = ClipRc.x - 1;
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y, 
					&rc, 
					dwFlags);
		rc.x = ClipRc.x + 1;
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y, 
					&rc, 
					dwFlags);
	}
	else
	{
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x-1, 
					y, 
					NULL, 
					dwFlags);
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x+1, 
					y, 
					NULL, 
					dwFlags);
	}
	
	rc = (prcBackground ? *prcBackground : ClipRc); 
	
	if (dwFlags & IDF_ALIGN_BOTTOM)
	{
		rc.dy = ClipRc.dy - 1;
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y, 
					&rc,
					dwFlags);
		rc.dy = ClipRc.dy + 1;
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y, 
					&rc, 
					dwFlags);
	}
	else if ( (dwFlags & IDF_ALIGN_TOP) || (dwFlags & IDF_ALIGN_MIDDLE) )
	{
		rc.y = ClipRc.y - 1;
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y, 
					&rc, 
					dwFlags);
		rc.y = ClipRc.y + 1;
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y, 
					&rc, 
					dwFlags);
	}
	else
	{
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y - 1, 
					NULL, 
					dwFlags);
		(void) GreyBitBrewFont_DrawText(pDisplay, 
					Font, 
					pcText, 
					nChars, 
					x, 
					y + 1, 
					NULL, 
					dwFlags);
	}
	#else
	// �����ı�
	rc.dx = prcBackground->dx;
	rc.dy = prcBackground->dy;
	rc.x = prcBackground->x-1;
	rc.y = prcBackground->y-1;
	(void) GreyBitBrewFont_DrawText(pDisplay, 
				Font+2, 
				pcText, 
				nChars, 
				x, 
				y, 
				&rc,//prcBackground, 
				dwFlags);
	#endif
	// �����ı����ʱ�ߵ���ɫ��ͬʱ����ԭ���ı���ɫֵ
	oldTextClr = IDISPLAY_SetColor(pDisplay, CLR_USER_TEXT, ProfileColor);
	
	
	// �����ı�
	(void) GreyBitBrewFont_DrawText(pDisplay, 
				Font, 
				pcText, 
				nChars, 
				x, 
				y, 
				prcBackground, 
				dwFlags);
	// �ָ���ʼ�ı���ɫ
	(void)IDISPLAY_SetColor(pDisplay, CLR_USER_TEXT, oldTextClr);
}

/*==============================================================================
����:
    DrawTextWithProfile

˵��:
    �������ڻ��ƴ��������ı���

����:
    pShell [in]��IShell �ӿ�ָ�롣
    pDisplay [in]��IDisplay �ӿ�ָ�롣
    ProfileColor [in]���ı�������RGBVAL��ɫֵ��
    ���� Font��pcText��nChars��x��y��prcBackground��dwFlags ����ͬ�ӿں���
    IDISPLAY_DrawText ��

����ֵ:
    none

��ע:
    ͬʱ���������������ĸ��£�����ĸ����ɵ�����������ɡ�
==============================================================================*/
void DrawTextWithProfile(IShell* pShell, 
    IDisplay * pDisplay, 
    RGBVAL ProfileColor, 
    AEEFont Font, 
    const AECHAR * pcText, 
    int nChars,
    int x, 
    int y, 
    const AEERect * prcBackground, 
    uint32 dwFlags
)
{
    RGBVAL oldTextClr;
    AEERect ClipRc;
    AEERect rc;
    AEEDeviceInfo di;

    // ��������������������Ӧ��������
    if ((NULL == pDisplay) || 
        (NULL == pcText) || 
        (NULL == pShell))
    {
        return;
    }

	
    // �����ı����ʱ�ߵ���ɫ��ͬʱ����ԭ���ı���ɫֵ
    oldTextClr = IDISPLAY_SetColor(pDisplay, CLR_USER_TEXT, ~ProfileColor);
	
    // ������ı�ʱ�ļ��о���
    ISHELL_GetDeviceInfo(pShell, &di);
    ClipRc.x = 0;
    ClipRc.y = 0;
    ClipRc.dx = di.cxScreen;
    ClipRc.dy = di.cyScreen;
    if (NULL != prcBackground)  
    {
        ClipRc = *prcBackground;
    }
    
    rc = ClipRc;
    
    // �����ı��ı�
    if (dwFlags & IDF_ALIGN_RIGHT)
    {
        rc.dx = ClipRc.dx - 1;
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y, 
                    &rc, 
                    dwFlags);
                    
        rc.dx = ClipRc.dx + 1;
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y, 
                    &rc, 
                    dwFlags);
        MSG_FATAL("rc.dx=%d", rc.dx,0,0);
    }
    else if ( (dwFlags & IDF_ALIGN_LEFT) || (dwFlags & IDF_ALIGN_CENTER) )
    {
        MSG_FATAL("rc.dx=%d", rc.dx,0,0);
        rc.x = ClipRc.x - 1;
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y, 
                    &rc, 
                    dwFlags);
        rc.x = ClipRc.x + 1;
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y, 
                    &rc, 
                    dwFlags);
    }
    else
    {
        MSG_FATAL("rc.dx=%d", rc.dx,0,0);
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x-1, 
                    y, 
                    NULL, 
                    dwFlags);
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x+1, 
                    y, 
                    NULL, 
                    dwFlags);
    }
    
    rc = (prcBackground ? *prcBackground : ClipRc); 
    
    if (dwFlags & IDF_ALIGN_BOTTOM)
    {
        rc.dy = ClipRc.dy - 1;
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y, 
                    &rc,
                    dwFlags);
        rc.dy = ClipRc.dy + 1;
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y, 
                    &rc, 
                    dwFlags);
        MSG_FATAL("rc.dx=%d", rc.dx,0,0);
    }
    else if ( (dwFlags & IDF_ALIGN_TOP) || (dwFlags & IDF_ALIGN_MIDDLE) )
    {
        rc.y = ClipRc.y - 1;
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y, 
                    &rc, 
                    dwFlags);
        rc.y = ClipRc.y + 1;
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y, 
                    &rc, 
                    dwFlags);
        MSG_FATAL("rc.dx=%d", rc.dx,0,0);
    }
    else
    {
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y - 1, 
                    NULL, 
                    dwFlags);
        (void) IDISPLAY_DrawText(pDisplay, 
                    Font, 
                    pcText, 
                    nChars, 
                    x, 
                    y + 1, 
                    NULL, 
                    dwFlags);
        MSG_FATAL("rc.dx=%d", rc.dx,0,0);
    }
	
	(void)IDISPLAY_SetColor(pDisplay, CLR_USER_TEXT, ProfileColor);
    // �����ı�
    (void) IDISPLAY_DrawText(pDisplay, 
                Font, 
                pcText, 
                nChars, 
                x, 
                y, 
                prcBackground, 
                dwFlags);
	// �ָ���ʼ�ı���ɫ
    (void)IDISPLAY_SetColor(pDisplay, CLR_USER_TEXT, oldTextClr);
}

/*==============================================================================
����:
    EncodePWDToUint16

˵��:
    �������ַ���ת��Ϊ�޷�����������

����:
    pszPWD [in]�������ַ�����

����ֵ:
    none

��ע:
    �����ַ����������Ϊ8����ֻ���ַ�0-9��*��#
==============================================================================*/
uint16 EncodePWDToUint16(char *pszPWD)
{
    int  index, nLen;
    float wPow = 0.1;
    uint16 wRet = 0;
    uint16 wTep = 10000;
    uint16  wPSD[10];
    
    if (NULL == pszPWD)
    {
        return wRet;
    }
    
    MEMSET(wPSD, 0, sizeof(wPSD));     
    
    nLen = STRLEN(pszPWD);

    if (nLen > 8)
    {
        return wRet;
    }       

    for (index = 0; index < nLen; index++)
    {
        wPow = wPow * 10; 
             
        switch(pszPWD[index])
        {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                wPSD[index] = (uint16)(pszPWD[index] - 48); 
                wTep += wPSD[index] * wPow;                                         
                break;

            case '*':
                wTep += (uint16)(index + 1) * 37;
                
                break;

            case '#':
                wTep += (uint16)(index + 1) * 97;
                break;
                
            case '0':
               wTep += (uint16)(index + 1) * 67;
               break;
            default:
                break;
        }           
                 
    }  

    wRet = wTep;  
    return wRet;
}

/*==============================================================================
����:
    Appscom_GetThemeParameters

˵��:
    �������ڻ�ȡ��ǰ���õ�������صĲ�����

����:
    TParam [in/out]: ���ڷ��ؽ���Ĳ���ָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
void Appscom_GetThemeParameters(Theme_Param_type *TParam)
{
#if defined( AEE_SIMULATOR)
    if (NULL == TParam)
    {
        return;
    }
    
    MEMSET(TParam, 0, sizeof(Theme_Param_type));
    
    // ���в�����δ���������
    TParam->textColor = MAKE_RGB( 0xde, 0xde, 0xde);
    TParam->seltextColor = RGB_BLACK;
    TParam->bkcolor = RGB_BLACK;
    TParam->themeColor = MAKE_RGB(0xfe, 0x61, 0x01);
    return;
#else
    
#ifdef FEATURE_FUNCS_THEME    
    byte ThemeType = DESKTOP_THEME_ORANGE;
    
    if (NULL == TParam)
    {
        return;
    }
    
    MEMSET(TParam, 0, sizeof(Theme_Param_type));
    
    // ���в�����δ���������
    TParam->textColor = MAKE_RGB( 0xde, 0xde, 0xde);
    TParam->seltextColor = RGB_BLACK;
    TParam->bkcolor = RGB_BLACK;
    
    // ���в�����δ����
    //TParam->selbkcolor = ?;

    (void)OEM_GetConfig(CFGI_DESKTOP_THEME, &ThemeType, sizeof(ThemeType));
    switch (ThemeType)
    {
        case DESKTOP_THEME_GREEN:
            TParam->themeColor = MAKE_RGB(0x75, 0xb1, 0x17);
            break;
            
        case DESKTOP_THEME_BLUE:
            TParam->themeColor = MAKE_RGB(0x00, 0x99, 0xff);
            break;

        case DESKTOP_THEME_DEEP_BLUE:
            TParam->themeColor = MAKE_RGB(0x38, 0x65, 0xdf);
            break;
            
        case DESKTOP_THEME_RED:
            TParam->themeColor = MAKE_RGB(0xfe, 0x41, 0x31);
            break;
        
        case DESKTOP_THEME_ORANGE:
            TParam->themeColor = MAKE_RGB(0xfe, 0x61, 0x01);
            break;
            
        case DESKTOP_THEME_YELLOW:
            TParam->themeColor = MAKE_RGB(0xfe, 0x8e, 0x00);
            break;
            
        default:
            break;
    }

#else//���ִ����л�ȡ�����Լ�����ɫ��������ر�feature����һЩ����

    TParam->textColor = MAKE_RGB( 0xde, 0xde, 0xde);
    TParam->seltextColor = RGB_BLACK;
    TParam->bkcolor = RGB_BLACK;
    TParam->themeColor = RGB_BLACK;
    
#endif /* FEATURE_FUNCS_THEME */    
#endif //#if defined( AEE_SIMULATOR)
}

/*==============================================================================
����:
    GetTitleBarHeight

˵��:
    ���������ڻ�ȡ������ʾ���ĸ߶ȡ�

����:
    pIDisplay [in]: ָ�� IDisplay �ӿڵ�ָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
int GetTitleBarHeight(IDisplay  * pIDisplay)
{
#ifndef FEATURE_FUNCS_TITLE_BAR
    return 0;
#else    
    if (NULL == pIDisplay)
    {
        return 0;
    }
    
    return TITLEBAR_HEIGHT;//���ﲻ����Ӧ���еĳߴ磬���Կ���ʹ��FEATURE�ֿ���ͬ����Ļ�ߴ硣(IDISPLAY_GetFontMetrics(pIDisplay, AEE_FONT_BOLD, NULL, NULL));
#endif /* FEATURE_FUNCS_TITLE_BAR */    
}

/*==============================================================================
����:
    GetBottomBarHeight

˵��:
    ���������ڻ�ȡ�ײ���ʾ���ĸ߶ȡ�

����:
    pIDisplay [in]: ָ�� IDisplay �ӿڵ�ָ�롣

����ֵ:
    none

��ע:

==============================================================================*/
int GetBottomBarHeight(IDisplay  * pIDisplay)
{
#ifndef FEATURE_FUNCS_BOTTOM_BAR
    return 0;
#else    
    if (NULL == pIDisplay)
    {
        return 0;
    }
    
    return BOTTOMBAR_HEIGHT;//���ﲻ����Ӧ���еĳߴ磬���Կ���ʹ��FEATURE�ֿ���ͬ����Ļ�ߴ硣(IDISPLAY_GetFontMetrics(pIDisplay, AEE_FONT_BOLD, NULL, NULL));
#endif /* FEATURE_FUNCS_BOTTOM_BAR */    
}

/*==============================================================================
����:
    DrawPromptMessage

˵��:

����:
    pIDisplay  [in]��IDisplayָ�롣
    pStatic    [in]: IStatic�ؼ���    
    PromptMsg_Param_type [in]����ʾ��Ϣ��ز���ָ��

����ֵ:
    none

��ע:
==============================================================================*/
void DrawPromptMessage (IDisplay *pIDisplay,
                        IStatic *pStatic,
                        PromptMsg_Param_type *PParam)
{      
    IShell          *pShell;     
    uint16          pMsgImgResID = 0;// warning
    IImage          *pMsgImg; 
    IImage          *StringBgMsgImg; 
    IImage          *TitleBgMsgImg;      
    AEERect         totalrect;
    AEERect         strrect;
    AEERect         titlerect; 
    BottomBar_Param_type bottomParam;
    TitleBar_Param_type  titleParam;
    AEEDeviceInfo   devinfo;         
    boolean         drawbottomStr;  
    boolean         drawbgimage = FALSE;  
    AECHAR         *pwszMsg = NULL;  
               
    pShell = AEE_GetShell();                       
    
    if ((NULL == pShell) 
        || (NULL == pIDisplay) 
        || (NULL == pStatic) 
        || (NULL == PParam))
    {
        return;
    }   
        
    switch (PParam->ePMsgType)
    {
        case MESSAGE_CONFIRM:
            drawbottomStr = TRUE;
            pMsgImgResID = IDB_CONFIRM;        
            break;
            
        case MESSAGE_NONE:
        default:        
            drawbottomStr = FALSE;
            pMsgImgResID = IDB_INFORMATION;
            break;
#ifndef FEATURE_USES_LOWMEM
//��������ͼƬ�Լ�spec���޸ģ����½�ͼƬ�����м䣬���������û���������(��cs0x��ͬ)
        case MESSAGE_ERR:
            drawbottomStr = FALSE;
            pMsgImgResID = IDB_ERROR;
            break;
            
        case MESSAGE_WARNNING:
            drawbottomStr = FALSE;
            pMsgImgResID = IDB_WARNNING;        
            break;
            
        case MESSAGE_WAITING:
            drawbottomStr = FALSE;
            pMsgImgResID = IDB_WAITING;        
            break;
            
        case MESSAGE_INFORMATION:
            drawbottomStr = FALSE;
            pMsgImgResID = IDB_INFORMATION;        
            break;   
            
        case MESSAGE_INFORMATIVE:
            drawbottomStr = FALSE;
            pMsgImgResID = IDB_INFORMATIVE;        
            break;               
#endif
    }  
    
    //Draw shadow for screen
    pMsgImg = ISHELL_LoadResImage(pShell,
                            AEE_APPSCOMMONRES_IMAGESFILE,
                            IDB_BACKGROUND);
    if(pMsgImg != NULL)
    {                  
        IIMAGE_Draw(pMsgImg, 0, 0);
        IIMAGE_Release(pMsgImg);
        pMsgImg = NULL;
    }
    else
    {
        return;
    }

    if (NULL != PParam->prc)
    {
        totalrect = *(PParam->prc);

        titlerect.dy = GetBottomBarHeight(pIDisplay);
        titlerect.dx = totalrect.dx;
        titlerect.x = totalrect.x;
        titlerect.y = totalrect.y;

        strrect.dy = totalrect.dy - titlerect.dy;
        strrect.dx = totalrect.dx;
        strrect.x = totalrect.x;
        strrect.y = totalrect.y - titlerect.dy;  
        IDISPLAY_EraseRect(pIDisplay, &totalrect);  
    }
    else
    {
        AEEImageInfo StringBgImgInfo = {0};
        AEEImageInfo TitleBgImgInfo = {0};
        
        //Draw back ground picture   
        MEMSET(&devinfo, 0, sizeof(devinfo));
        ISHELL_GetDeviceInfo(pShell, &devinfo);     
        
#if 0 
        totalrect.x = devinfo.cxScreen/5;
        if(devinfo.cxScreen == devinfo.cyScreen)
        {
            totalrect.y = devinfo.cyScreen/5; 
        }
        else
        {
            totalrect.y = devinfo.cyScreen/5- GetBottomBarHeight(pIDisplay); 
        }        
#endif    
/*
        //Draw title background picture
        TitleBgMsgImg = ISHELL_LoadResImage(pShell,
                                    AEE_APPSCOMMONRES_IMAGESFILE,
                                    pMsgImgResID); 
        if(TitleBgMsgImg != NULL)
        {
            IIMAGE_GetInfo(TitleBgMsgImg, &TitleBgImgInfo);
            IIMAGE_Draw(TitleBgMsgImg, totalrect.x, totalrect.y);
            IIMAGE_Release(TitleBgMsgImg);
            TitleBgMsgImg = NULL; 
            
            //Draw string background picture
            StringBgMsgImg = ISHELL_LoadResImage(pShell,
                                AEE_APPSCOMMONRES_IMAGESFILE,
                                IDB_PROMPT_MSG_BG); 
            if(StringBgMsgImg != NULL)
            {
                IIMAGE_GetInfo(StringBgMsgImg, &StringBgImgInfo);
                IIMAGE_Draw(StringBgMsgImg, totalrect.x, totalrect.y+TitleBgImgInfo.cy);
                IIMAGE_Release(StringBgMsgImg);
                StringBgMsgImg = NULL; 
            }             
        }       
*/
/*
        totalrect.x = 0;
        totalrect.y = devinfo.cyScreen - GetBottomBarHeight(pIDisplay);
*/

        totalrect.x = devinfo.cxScreen / 2;
        //totalrect.y = (devinfo.cyScreen - TITLEBAR_HEIGHT - BOTTOMBAR_HEIGHT)/2 + TITLEBAR_HEIGHT;
        totalrect.y = (devinfo.cyScreen  - BOTTOMBAR_HEIGHT)/2;	//Add By zzg 2010_07_16
		
        //Draw string background picture
        StringBgMsgImg = ISHELL_LoadResImage(pShell,
                            AEE_APPSCOMMONRES_IMAGESFILE,
                            IDB_PROMPT_MSG_BG);
        if(StringBgMsgImg != NULL)
        {
            IIMAGE_GetInfo(StringBgMsgImg, &StringBgImgInfo);
            //totalrect.x = (devinfo.cxScreen - StringBgImgInfo.cx)/2;
            IIMAGE_Draw(StringBgMsgImg, totalrect.x - StringBgImgInfo.cx/2, totalrect.y - StringBgImgInfo.cy/2);
            IIMAGE_Release(StringBgMsgImg);
            StringBgMsgImg = NULL;  
            
            //Draw title background picture
            TitleBgMsgImg = ISHELL_LoadResImage(pShell,
                                        AEE_APPSCOMMONRES_IMAGESFILE,
                                        pMsgImgResID);
            if(TitleBgMsgImg != NULL)
            {
                IIMAGE_GetInfo(TitleBgMsgImg, &TitleBgImgInfo);
                
                IIMAGE_Draw(TitleBgMsgImg, totalrect.x - TitleBgImgInfo.cx/2, totalrect.y - StringBgImgInfo.cy/2 + TitleBgImgInfo.cy/2);
                IIMAGE_Release(TitleBgMsgImg);
                TitleBgMsgImg = NULL;               
            }
                                   
        }
        if((TitleBgImgInfo.cy!= 0) && (StringBgImgInfo.cy!= 0))
        {              
            totalrect.dy = StringBgImgInfo.cy + TitleBgImgInfo.cy; 
            totalrect.dx = TitleBgImgInfo.cx;        

            titlerect.dy = TitleBgImgInfo.cy;
            titlerect.dx = TitleBgImgInfo.cx;
            titlerect.x = totalrect.x - TitleBgImgInfo.cx/2;
            titlerect.y = totalrect.y  - StringBgImgInfo.cy/2 + TitleBgImgInfo.cy/2;
            
            strrect.dy = StringBgImgInfo.cy/2;
            strrect.dx = StringBgImgInfo.cx;
            strrect.x = totalrect.x - StringBgImgInfo.cx/2;
            strrect.y = totalrect.y;

            
            drawbgimage = TRUE;                           
        }
        else
        {     
            //Draw totalrect  
            totalrect.dx  = devinfo.cxScreen * 75 / 100;
            totalrect.dy = devinfo.cyScreen * 75 / 100;

            titlerect.dy = GetBottomBarHeight(pIDisplay);
            titlerect.dx = totalrect.dx;
            titlerect.x = totalrect.x;
            titlerect.y = totalrect.y;

            strrect.dy = totalrect.dy - titlerect.dy;
            strrect.dx = totalrect.dx;
            strrect.x = totalrect.x;
            strrect.y = totalrect.y - titlerect.dy;  
            IDISPLAY_EraseRect(pIDisplay, &totalrect);     
        } 
    }
        
    //Draw title 
    if(PParam->pwszTitle != NULL)
    {
        MEMSET(&titleParam,0,sizeof(TitleBar_Param_type));
        titleParam.prc = &titlerect;
        titleParam.pwszTitle = PParam->pwszTitle;
        DrawTitleBar(pIDisplay, &titleParam);
    }
    
    //Draw bottom
    MEMSET(&bottomParam,0,sizeof(BottomBar_Param_type));
    if(NULL != PParam->eBBarType)
    {
        bottomParam.eBBarType = PParam->eBBarType;
    }
    else
    {
        if(TRUE == drawbottomStr)
        {
            bottomParam.eBBarType = BTBAR_BACK; //BTBAR_SELECT_BACK;            

        }  
    }
    DrawBottomBar(pIDisplay, &bottomParam);  
     
    //Draw string  
    ISTATIC_SetRect(pStatic, &strrect);
    if(NULL != PParam->pwszMsg)
    {
        pwszMsg = PParam->pwszMsg;
    }
    else if((NULL != PParam->strMsgResFile) && (0 != PParam->nMsgResID))
    {
        int32  dwSize = 0;
        
        dwSize = 128 * sizeof(AECHAR);
        pwszMsg = (AECHAR *)MALLOC(dwSize);
        if (NULL != pwszMsg)
        {
            (void)ISHELL_LoadResString(pShell, 
                                       PParam->strMsgResFile, 
                                       PParam->nMsgResID, 
                                       pwszMsg, 
                                       dwSize);
        }            
    }       
    if (NULL != pwszMsg)
    {
#ifndef FEATURE_USES_LOWMEM 
        IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_WHITE);
#else
        IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_BLACK);
#endif
        if (TRUE== drawbgimage)
        {
            ISTATIC_SetProperties(pStatic, ST_CENTERTEXT|ST_MIDDLETEXT|ST_TRANSPARENTBACK);   
        }
        else
        {
            ISTATIC_SetProperties(pStatic, ST_CENTERTEXT|ST_MIDDLETEXT); 
        }   
        (void)ISTATIC_SetText(pStatic, 
                              NULL, 
                              pwszMsg, 
                              AEE_FONT_NORMAL, 
                              AEE_FONT_NORMAL);
   
        //Redraw
        (void)ISTATIC_Redraw(pStatic);
        if (NULL == PParam->pwszMsg)
        {
            FREE(pwszMsg);
        }
        IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, RGB_BLACK);
    } 
}
#ifdef FEATURE_KEYGUARD
void Appscomm_Draw_Keyguard_Msg(IDisplay *pIDisplay,IStatic *pStatic,boolean unlockkey)
{
    PromptMsg_Param_type m_PromptMsg;
    AEERect rect = {0};
    ISTATIC_SetRect(pStatic, &rect);
    MEMSET(&m_PromptMsg,0,sizeof(PromptMsg_Param_type));
    //SETAEERECT(&rect, 0, 16, 128, 112);
    //m_PromptMsg.prc = &rect;

	if ( (NULL == pIDisplay) 
        || (NULL == pStatic))
    {
        return;
    } 
//Add By zzg 2010_11_23
#ifdef FEATURE_UNLOCK_KEY_SPACE
	m_PromptMsg.nMsgResID = IDS_MSG_KEYGUARD_SPACE;
#else
	if(TRUE == unlockkey)
	{
	    m_PromptMsg.nMsgResID = IDS_MSG_KEYGUARD_UNLOCKKEY;
	}
	else
	{
	    m_PromptMsg.nMsgResID = IDS_MSG_KEYGUARD;        
	}
#endif
//Add End

	/*
    if(TRUE == unlockkey)
    {
        m_PromptMsg.nMsgResID = IDS_MSG_KEYGUARD_UNLOCKKEY;
    }
    else
    {
        m_PromptMsg.nMsgResID = IDS_MSG_KEYGUARD;        
    }
    */
    
    STRLCPY(m_PromptMsg.strMsgResFile, AEE_APPSCOMMONRES_LANGFILE,MAX_FILE_NAME);
    m_PromptMsg.ePMsgType = MESSAGE_WARNNING;        
    m_PromptMsg.eBBarType = BTBAR_NONE;        
    DrawPromptMessage(pIDisplay,pStatic,&m_PromptMsg);
    IDISPLAY_UpdateEx(pIDisplay,FALSE);
}
#endif
#ifdef FEATURE_KEYGUARD
void Appscommon_Draw_Keyguard_Time(IDisplay *pIDisplay)
{
    JulianType  jDate;
    AEERect rect = {0};
    AECHAR  wFormat[32] = {0}, wstrDisp[32] = {0}, wstrWeekDay[16] = {0};
    AEEDeviceInfo devinfo = {0};
    IShell      *pShell = AEE_GetShell();
    int         nLineWidth = 4, nNumberWidth = 20, nNumberHeight = 40, nOffset = 5,
                xStartPos = 0, yStartPos = 0, nTextLen = 0, 
                nFontHeight = IDISPLAY_GetFontMetrics(pIDisplay, AEE_FONT_NORMAL, NULL, NULL);
    byte      bTFmt = 0;
    uint16    nWeekResID = 0, nHour = 0;
    RGBVAL  nOldFontColor = RGB_WHITE;

    if (NULL == pIDisplay)
    {
        return;
    } 
    ISHELL_GetDeviceInfo(pShell, &devinfo);
    //clear screen
    SETAEERECT(&rect, 0, 0, devinfo.cxScreen, devinfo.cyScreen);
    IDISPLAY_FillRect(pIDisplay, &rect, RGB_BLACK);
    
    GetJulianDate(GETTIMESECONDS(), &jDate);
    
    // draw day
#ifdef FEATURE_TIME_DATA_SETTING
    (void)OEM_GetConfig(CFGI_DATE_FORMAT, &bTFmt, sizeof(bTFmt));

    switch(bTFmt)
    {
        case OEMNV_DATEFORM_DMY:
            WSTRLCPY(wFormat,L"%02d/%02d/%04d",32);
            WSPRINTF(wstrDisp, sizeof(wstrDisp), wFormat, jDate.wDay, jDate.wMonth, jDate.wYear);
            break;

        case OEMNV_DATEFORM_MDY:
            WSTRLCPY(wFormat,L"%02d/%02d/%04d",32);
            WSPRINTF(wstrDisp, sizeof(wstrDisp), wFormat, jDate.wMonth, jDate.wDay, jDate.wYear);
            break;
            
        default:
        case OEMNV_DATEFORM_YMD:
            WSTRLCPY(wFormat,L"%04d/%02d/%02d",32);
            WSPRINTF(wstrDisp, sizeof(wstrDisp), wFormat, jDate.wYear, jDate.wMonth, jDate.wDay);
            break;
    }
#else
    WSTRLCPY(wFormat,L"%04d-%02d-%02d",32);
    WSPRINTF(wstrDisp, sizeof(wstrDisp), wFormat, jDate.wYear, jDate.wMonth, jDate.wDay);
#endif
    nTextLen = WSTRLEN(wstrDisp);
    wstrDisp[nTextLen] = L' ';
    wstrDisp[nTextLen+1] = L' ';
    wstrDisp[nTextLen+2] = L'\0';
    
    switch (jDate.wWeekDay)
    {
        case 0: // ����һ
            nWeekResID = AEE_IDS_MONDAY;
            break;
            
        case 1: // ���ڶ�
            nWeekResID = AEE_IDS_TUESDAY;
            break;

        case 2: // ������
            nWeekResID = AEE_IDS_WEDNESDAY;
            break;

        case 3: // ������
            nWeekResID = AEE_IDS_THURSDAY;
            break;

        case 4: // ������
            nWeekResID = AEE_IDS_FRIDAY;
            break;

        case 5: // ������
            nWeekResID = AEE_IDS_SATURDAY;
            break;

        default:  // 6:������
            nWeekResID = AEE_IDS_SUNDAY;
            break;
    }
    (void) ISHELL_LoadResString(pShell, OEMAEECONTROLS_LNGRES_FILE, nWeekResID, wstrWeekDay, sizeof(wstrWeekDay));
    
    wstrWeekDay[3] = L'\0';
    nTextLen = WSTRLEN(wstrDisp);
    WSTRCPY(wstrDisp+nTextLen, wstrWeekDay);
    
    nTextLen = IDISPLAY_MeasureText(pIDisplay, AEE_FONT_NORMAL, wstrDisp);
    xStartPos = (devinfo.cxScreen - nTextLen)/2;
    yStartPos = (devinfo.cyScreen*2/5) - nFontHeight - 2*nOffset;
    
    nOldFontColor = IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, nOldFontColor);
    
    IDISPLAY_DrawText(pIDisplay, AEE_FONT_NORMAL, wstrDisp, -1, xStartPos, yStartPos, NULL, IDF_TEXT_TRANSPARENT);

    nTextLen = (nNumberWidth + nOffset)*4 + nLineWidth;
    xStartPos = (devinfo.cxScreen - nTextLen)/2;
    yStartPos = (devinfo.cyScreen*2/5);

    // get time format
    (void)OEM_GetConfig(CFGI_TIME_FORMAT, &bTFmt, sizeof(bTFmt));

    if (bTFmt == OEMNV_TIMEFORM_AMPM)
    {
        nHour = (jDate.wHour > 12) ? (jDate.wHour - 12) : jDate.wHour;
        if(nHour/10 == 1)
        {
            xStartPos -= (nNumberWidth - nLineWidth)/2;
        }
        MEMSET(wstrDisp, 0, sizeof(wstrDisp));
        wstrDisp[0] = (jDate.wHour > 12)?(L'P'):(L'A');
        wstrDisp[1] = L'M';
        wstrDisp[2] = L'\0';
        nTextLen = IDISPLAY_MeasureText(pIDisplay, AEE_FONT_NORMAL, wstrDisp);
        xStartPos -= (nTextLen + nOffset)/2;
        IDISPLAY_DrawText(pIDisplay, AEE_FONT_NORMAL, wstrDisp, -1, 
                       xStartPos + (nNumberWidth + nOffset)*4 + nLineWidth + nOffset, 
                       yStartPos + nNumberHeight - nFontHeight, 
                       NULL, IDF_TEXT_TRANSPARENT);
    }
    else
    {
        nHour = jDate.wHour;
        if(nHour/10 == 1)
        {
            xStartPos -= (nNumberWidth - nLineWidth)/2;
        }
    }

    // draw hour
    SETAEERECT(&rect, xStartPos, yStartPos, nNumberWidth, nNumberHeight);
    Appscommon_DrawDigitalNumber(pIDisplay, (nHour/10), nLineWidth, &rect, RGB_WHITE);
    rect.x += nNumberWidth + nOffset;
    Appscommon_DrawDigitalNumber(pIDisplay, (nHour%10), nLineWidth, &rect, RGB_WHITE);

    // draw colon
    SETAEERECT(&rect, xStartPos + 2*(nNumberWidth + nOffset), yStartPos + nNumberHeight/2 - nLineWidth, nLineWidth, nLineWidth);
    IDISPLAY_FillRect(pIDisplay, &rect, RGB_WHITE);
    rect.y = yStartPos + nNumberHeight*4/5 - nLineWidth;
    IDISPLAY_FillRect(pIDisplay, &rect, RGB_WHITE);
    
    // draw minute
    SETAEERECT(&rect, xStartPos + 2*(nNumberWidth + nOffset) + nLineWidth + nOffset, yStartPos, nNumberWidth, nNumberHeight);
    Appscommon_DrawDigitalNumber(pIDisplay, (jDate.wMinute/10), nLineWidth, &rect, RGB_WHITE);
    rect.x += nNumberWidth + nOffset;
    Appscommon_DrawDigitalNumber(pIDisplay, (jDate.wMinute%10), nLineWidth, &rect, RGB_WHITE);
    
    (void)IDISPLAY_SetColor(pIDisplay, CLR_USER_TEXT, nOldFontColor);

    IDISPLAY_Update(pIDisplay);
}
#endif
void DrawBottomBar_Ex(IShell    *m_pIShell, IDisplay  * pIDisplay, BottomBar_e_Type    eBBarType)
{
    AEEDeviceInfo devinfo;
    AEERect rc;
    uint16      nResID_L = 0;// ��
    uint16      nResID_R = 0;// ��
    uint16      nResID_M = 0;// ��   ////modi by yangdecai 2010-08-04
    AECHAR      wszBuf[16]= {(AECHAR)'\0'};
    int nBarH = 0;
    int nFontH = 0;
    if ((NULL == m_pIShell) || (NULL == pIDisplay))
    {
        return;
    }

    nBarH = GetBottomBarHeight(pIDisplay);
    nFontH = IDISPLAY_GetFontMetrics(pIDisplay, AEE_FONT_NORMAL, NULL, NULL);

    MEMSET(&devinfo, 0, sizeof(devinfo));
    ISHELL_GetDeviceInfo(m_pIShell, &devinfo);
    //SETAEERECT(&rc, 2, devinfo.cyScreen-nBarH, devinfo.cxScreen-4, nBarH);
    if(nBarH < nFontH + 2)
    {
        //��Ϊ�Ҷ�������ұ��Զ��ճ�һ�����أ�������ߵľ��α߽�Ӧ�����ұ߶�һ������
        SETAEERECT(&rc, 3, devinfo.cyScreen-nBarH, devinfo.cxScreen-5, nBarH);
    }
    else
    {
        SETAEERECT(&rc, 3, devinfo.cyScreen-nFontH-2, devinfo.cxScreen-5, nFontH);
    }

    switch (eBBarType)
    {
        case BTBAR_MESSAGES_CONTACTS:
            nResID_L = IDS_MESSAGES;
            nResID_R = IDS_STRING_CONTACTS;
            break;
            
        case BTBAR_MENU_CONTACTS:
            nResID_L = IDS_MENU;
            nResID_R = IDS_STRING_CONTACTS;
            break;
            
		case BTBAR_MENU_SOS:
         	nResID_L = IDS_MENU;
            nResID_R = IDS_SOS;
         	break;
         	
		case BTBAR_MENU_FRENDUO:
			nResID_L = IDS_MENU;
			nResID_R = IDS_FRENDUO_LIST;
			break;
		
		case BTBAR_FACEBOOK_CHAT:
			nResID_L = IDS_FACEBOOK;
			nResID_R = IDS_CHAT;
			break;
            
		case BTBAR_VIEWMORE_BACK:
            nResID_L = IDS_MORE;
			nResID_M = IDS_OK;   ////add by yangdecai 2010-08-04
			nResID_R = IDS_BACK;
			break;
            
        case BTBAR_UNLOCK_SOS:
            nResID_R = IDS_STRING_UNLOCK;
            nResID_L = IDS_SOS;
            break;
		case BTBAR_UNLOCK:
			nResID_R = IDS_STRING_UNLOCK;
			break;
        case BTBAR_LUNLOCK:
            nResID_L = IDS_STRING_UNLOCK;
            nResID_R = IDS_SOS;
            break;
        case BTBAR_BACK:
            nResID_R = IDS_BACK;
            break;

        case BTBAR_OK:
            nResID_L = IDS_OK;
            break;
        
        case BTBAR_OPTION_ENDCALL:
            nResID_L = IDS_OPTION;
            nResID_R = IDS_STRING_END_CALL;
            break;
               
		case BTBAR_OPTION:
			nResID_L = IDS_OPTION;
            break;
        // ѡ��-----����
         case BTBAR_SELECT_BACK:
            nResID_L = IDS_OK;	//SELECT;
            nResID_R = IDS_BACK;
            break;
            
        case BTBAR_SAVE_BACK:
            nResID_L = IDS_SAVE;
            nResID_R = IDS_BACK;
            break;
#ifdef FEATURE_FLEXI_STATIC_BREW_APP
        case BTBAR_FNASRANI_FPORTAL:
            nResID_L = IDS_FNASRANI;
            nResID_R = IDS_FPORTAL;
            break;
        case BTBAR_FMUSLIM_FPORTAL:
            nResID_L = IDS_FMUSLIM;
            nResID_R = IDS_FPORTAL;
            break;  
        case BTBAR_FGURU_FPORTAL:
            nResID_L = IDS_FGURU;
            nResID_R = IDS_FPORTAL;
            break;
            
        case BTBAR_CONTACTS_FPORTAL:
            nResID_L = IDS_STRING_CONTACTS;
            nResID_R = IDS_FPORTAL;
            break;
#endif  /*FEATURE_FLEXI_STATIC_BREW_APP*/
		case BTBAR_FMENU_CANTACT:
			nResID_L = IDS_FMENU;
			nResID_R = IDS_STRING_CONTACTS;
			break; 
		case BTBAR_MENU_FMENU:
			nResID_L = IDS_MENU;
			nResID_R = IDS_FMENU;
			break;	
    }
    if(nResID_L)
    {
        ISHELL_LoadResString(m_pIShell,AEE_APPSCOMMONRES_LANGFILE,nResID_L,wszBuf, sizeof(wszBuf));
        DrawTextWithProfile(m_pIShell, pIDisplay,RGB_WHITE_NO_TRANS,AEE_FONT_NORMAL, wszBuf, -1,0, 0, &rc,  IDF_ALIGN_MIDDLE | IDF_ALIGN_LEFT  | IDF_TEXT_TRANSPARENT);
    } 
    if(nResID_R)
    {
        ISHELL_LoadResString(m_pIShell,AEE_APPSCOMMONRES_LANGFILE,nResID_R,wszBuf, sizeof(wszBuf));
        DrawTextWithProfile(m_pIShell, pIDisplay,RGB_WHITE_NO_TRANS,AEE_FONT_NORMAL, wszBuf, -1,0, 0, &rc,  IDF_ALIGN_MIDDLE | IDF_ALIGN_RIGHT  | IDF_TEXT_TRANSPARENT);
    }
	//add by yangdecai 2010-08-04
	if(nResID_M)
	{
		ISHELL_LoadResString(m_pIShell,AEE_APPSCOMMONRES_LANGFILE,nResID_M,wszBuf, sizeof(wszBuf));
        DrawTextWithProfile(m_pIShell, pIDisplay,RGB_WHITE_NO_TRANS,AEE_FONT_NORMAL, wszBuf, -1,0, 0, &rc,  IDF_ALIGN_MIDDLE | IDF_ALIGN_CENTER | IDF_TEXT_TRANSPARENT);
	}
}

/*==============================================================================
����: 
    DecodeAlphaString
       
˵��: 
    ���������� Alpha ���ַ�����
       
����: 
    pdata [in]��ָ�����������ֽ����С�
    nLen [in]�����������ֽ����г���,��λ�ֽڡ�
    wstrOut [out]�� ���Buffer
    nBufSize [in]:  ���Buffer��С,��λ AECHAR
       
����ֵ:
    none
       
��ע:
       
==============================================================================*/
void DecodeAlphaString(byte *pdata, int nLen, AECHAR *wstrOut, int nBufSize)
{
    int nChars = 0,i;
    AECHAR  wBase;
    byte bTep;
    
    if ((NULL == pdata) || (NULL == wstrOut))
    {
        return;
    }
    
    if (nLen<=0 || nBufSize<=0)
    {
        return;
    }
    
    switch(pdata[0])
    {
        // Unicode ����,�޻�ַ
        case 0x80:
            if (nBufSize*sizeof(AECHAR)>nLen)
            {
                uint16 *pUs;
                
                MEMCPY((void*)wstrOut, (void*)&pdata[1], nLen-1);
                pUs = wstrOut;
                for (i=0; i<(nLen-1)/2; i++)
                {
                   if ((*pUs == 0xFFFF) || (*pUs == 0x0000))
                   {
                       break;
                   }
                   *pUs = NTOHS(*pUs);
                   pUs++;
                }
                wstrOut[i] = 0;
            }
            break;
            
        // Unicode ����1,������һ�����ַ:
        // ����˵��: �ֽ�pdata[2]������16λ��������wBase 0xxxxxxxx0000000��xxxxxxxx����,
        //           �������ֽڵ����λΪ1,����ֽں�7λ��wBase�ĺͱ�ʾһ��Unicode ����
        //           �ַ�,������ַ�ΪGSM default alphabet characters
        case 0x81:
            if (nLen>3)
            {
                nChars = pdata[1];
                wBase = pdata[2];
                wBase = wBase<<7;
                
                nChars = ((nChars <= nLen-3) ? nChars : (nLen-3));
                for (i=0; i<nChars && i<(nBufSize-1); i++)
                {
                    bTep = pdata[i+3];
                    if (bTep & 0x80)
                    {
                        wstrOut[i] = (bTep & 0x7F) + wBase;
                    }
                    else
                    {
                        wstrOut[i] = (AECHAR)GsmToUcs2[bTep];
                    }
                }
                wstrOut[i] = 0;
            }
            break;
        
        // Unicode ����2,������һ����ַ: 
        // ����˵��: �ֽ�pdata[2]��pdata[3]������һ��16λUnicode �����ַ�ַ�,
        //           �������ֽڵ����λΪ1,����ֽں�7λ��wBase�ĺͱ�ʾһ��Unicode ����
        //           �ַ�,������ַ�ΪGSM default alphabet characters
        case 0x82:
            if (nLen>4)
            {
                nChars = pdata[1];
                
                MEMCPY((void*)&wBase, &pdata[2], sizeof(wBase));
                wBase = HTONS(wBase);
                
                nChars = ((nChars <= nLen-4) ? nChars : (nLen-4));
                for (i=0; i<nChars&& i<(nBufSize-1); i++)
                {
                    bTep = pdata[i+4];
                    if (bTep & 0x80)
                    {
                        wstrOut[i] = (bTep & 0x7F) + wBase;
                    }
                    else
                    {
                        wstrOut[i] = (AECHAR)GsmToUcs2[bTep];
                    }
                }
                wstrOut[i] = 0;
            }
            break;
            
        // Ĭ�� GSM default alphabet characters only
        default:
            for (i=0; i<nLen && i<(nBufSize-1); i++)
            {
                if (pdata[i]==0xFF || pdata[i]==0x00)
                {
                    break;
                }
                wstrOut[i] = (AECHAR)GsmToUcs2[pdata[i]];
            }
            wstrOut[i] = 0;
            break;
    }
}

int Appscomm_is_incoming_state(int bb)
{
    static int g_is_incoming = 0;
    if (bb>=0)
    {
        g_is_incoming = bb;
    }
    return g_is_incoming;   
}

void SetArrowFlagonIM(boolean bFlag)
{
    gbArrowFlag = bFlag;
}
boolean GetArrowFlagonIM()
{
     return (gbArrowFlag);
}


/*==============================================================================
����: 
    Appscommon_DrawDialogBoxFrame
       
˵��: 
    ���Ƶ����Ի���߿򼰱���ɫ��
       
����: 
    pBoxRect [in]: ���뽫Ҫ���ƶԻ���ľ��η�Χ�� 
    bUpward [in]: �Ի����ͷ�������Ƿ����Ϸ���
    nFrameColor [in]: �Ի���߿���ɫ��
    nBgColor [in]: �Ի��򱳾���ɫ
       
����ֵ:
    TRUE: ���Ƴɹ�
    FALSE: ����ʧ��
       
��ע:
       
==============================================================================*/
boolean Appscommon_DrawDialogBoxFrame(AEERect *pBoxRect, boolean bUpward, RGBVAL nFrameColor, RGBVAL nBgColor)
{
    AEERect        rect = *pBoxRect;
    int               nTriangleHeight = 4; /* �Ի����ͷС���Ǹ߶�*/
    int               nMaxXOffset = MAX(15, pBoxRect->dx/4); /* ��ͷ���x�᷽�����ƫ��*/
    IShell           *pShell = AEE_GetShell();
    IGraphics      *pGraphic;
    AEETriangle   triangle;
    
    if (NULL == pShell)
    {
        return FALSE;
    }
    
    if(SUCCESS != ISHELL_CreateInstance(pShell, AEECLSID_GRAPHICS, (void **)&pGraphic))
    {
        return FALSE;
    }
    
    IGRAPHICS_SetFillMode(pGraphic, TRUE);
    /* ���öԻ��򱳾���ɫ*/
    IGRAPHICS_SetFillColor(pGraphic, GET_RGB_R(nBgColor), GET_RGB_G(nBgColor), GET_RGB_B(nBgColor), 0);
    /* ���öԻ���߿���ɫ*/
    IGRAPHICS_SetColor(pGraphic, GET_RGB_R(nFrameColor), GET_RGB_G(nFrameColor), GET_RGB_B(nFrameColor), 0);

    /* �����Ի���߶ȣ�������ͷ�����θ߶�*/
    if(bUpward)
    {
        rect.y += (nTriangleHeight - 1);
    }
    rect.dy -= (nTriangleHeight - 1);

    /* ���ƶԻ������屳��*/
    IGRAPHICS_DrawRoundRectangle(pGraphic, &rect, 6, 6);

    triangle.x0 = MIN(pBoxRect->x + nMaxXOffset, pBoxRect->x + pBoxRect->dx/2 - nTriangleHeight/2);
    triangle.x1 = triangle.x0;
    triangle.x2 = triangle.x0 + nTriangleHeight;
    
    triangle.y0 = (bUpward) ? (pBoxRect->y) : (pBoxRect->y + pBoxRect->dy - 1);
    triangle.y1 = (bUpward) ? (triangle.y0 + nTriangleHeight) : (triangle.y0 - nTriangleHeight);
    triangle.y2 = triangle.y1;

    /* ���Ƽ�ͷ������*/
    IGRAPHICS_DrawTriangle(pGraphic, &triangle);
    
    /* ������Ҫ�ѶԻ��򱳾���ɫ�����߿��ڵ���Ե���紦�߶�*/
    IGRAPHICS_SetColor(pGraphic, GET_RGB_R(nBgColor), GET_RGB_G(nBgColor), GET_RGB_B(nBgColor), 0);
    
    SETAEERECT(&rect, triangle.x0, triangle.y1, nTriangleHeight + 1, 1);
    IGRAPHICS_DrawRect(pGraphic, &rect);
    
    if(pGraphic != NULL)
    {
        IGRAPHICS_Release(pGraphic);
        pGraphic = NULL;
    }
    return TRUE;
}

/*==============================================================================
����: 
    Appscommon_DrawPopUpDialogBox
       
˵��: 
    ���Ƶ����Ի���(���У����ı�)��
       
����: 
    pIDisplay [in]:
    x [in]: ���뽫Ҫ���ƶԻ������Ͻ�x���ꡣ
    y [in]: ���뽫Ҫ���ƶԻ������Ͻ�y���ꡣ
    strDisplay[in]:���뽫Ҫ���ƶԻ����ı����ݡ�
       
����ֵ:
    TRUE: ���Ƴɹ�
    FALSE: ����ʧ��
       
��ע:
    �����ں�������֮ǰʹ��IDISPLAY_SetColor�����ı���ɫ�������ʾ����Ϊ
    ���֮����Ļ����ʾ���x��С�����֮һ��Ļ����ʾ���y������
    menu���һ������ʾ����ʼ���꣬ �����Ľ��Զ�������
       
==============================================================================*/
boolean Appscommon_DrawPopUpDialogBox(IDisplay *pIDisplay, int x, int y, AECHAR *strDisplay)
{
    AEERect                rect;
    boolean                 bUpward;
    int                        nLength = IDISPLAY_MeasureText(pIDisplay, AEE_FONT_BOLD, strDisplay) + 5;
    int                        nFontHeight = IDISPLAY_GetFontMetrics(pIDisplay, AEE_FONT_BOLD, NULL, NULL);
    int                        nDialogHeight = MENUITEM_HEIGHT;
    int                        xPos, yPos, minXOffset, maxYOffset;
    AEEDeviceInfo        di = {0};
    IShell                   *pShell = AEE_GetShell();
    
    if ((NULL == pShell) || (NULL == pIDisplay))
    {
        return FALSE;
    }
    
    ISHELL_GetDeviceInfo(pShell, &di);
    minXOffset = di.cxScreen/5;
    maxYOffset = di.cyScreen - BOTTOMBAR_HEIGHT - nDialogHeight;
    bUpward = (boolean)(y <= maxYOffset);
    nLength = MIN(di.cxScreen - minXOffset, nLength);
    xPos = MIN(di.cxScreen - nLength, MAX(minXOffset, x));
    yPos = (bUpward)?(y):(maxYOffset - nDialogHeight);
    
    SETAEERECT(&rect, xPos, yPos, nLength, nDialogHeight);
    
    if(Appscommon_DrawDialogBoxFrame(&rect, bUpward, MAKE_RGB(0xFF, 0x70, 0x00), RGB_WHITE) != TRUE)
    {
        return FALSE;
    }

    SETAEERECT(&rect, xPos + 3, yPos + (nDialogHeight - 3 - nFontHeight)/2 + 1, nLength - 5, nFontHeight);

    if(bUpward)
    {
        rect.y += 3;
    }
    
    IDISPLAY_DrawText(pIDisplay, 
                    AEE_FONT_BOLD, 
                    strDisplay, 
                    -1, 
                    rect.x, 
                    rect.y, 
                    &rect, 
                    IDF_TEXT_TRANSPARENT);
    
    return TRUE;
}

/*==============================================================================
����: 
    Appscommon_DrawDigitalNumber
       
˵��: 
    ���Ʒ��߶����������
       
����: 
    pDisplay [in]: 
    number[in]: ��Ҫ���Ƶ����֡�
    nLineWidth[in]: ��Ҫ���Ƶ�����������ȡ�
    fontRect [in]: �������ֵı߽���ο�
    fontColor[in]: ��Ҫ���Ƶ�������ɫ��
       
����ֵ:
    FALSE: ����ʧ��
    TRUE: ���Ƴɹ�
       
��ע:
       
==============================================================================*/
boolean Appscommon_DrawDigitalNumber (IDisplay *pDisplay, int number, int nLineWidth, AEERect *fontRect, RGBVAL fontColor)
{
	if (NULL == pDisplay)
    {
        return;
    } 
    
    if(number > 9 || number < 0 || pDisplay == NULL)
    {
        return FALSE;
    }
    else
    {
        AEERect rect = {0};
        int i = 0;
        int nWidth = nLineWidth;
        int nLength = fontRect->dx - 2;
        int nHeight = (fontRect->dy - 2)/2;
        int nHeight2 = (nWidth%2 == 0)? (nHeight - 1) : (nHeight);
        
        if(number == 0 ||
            number == 2 ||
            number == 3 ||
            number == 5 ||
            number == 6 ||
            number == 7 ||
            number == 8 ||
            number == 9)
        {
            SETAEERECT(&rect, fontRect->x + 1, fontRect->y, nLength, 1);
            for(i = 0; i < nWidth; i++)
            {
                IDISPLAY_FillRect(pDisplay, &rect, fontColor);
                rect.x++;
                rect.y++;
                rect.dx -= 2;
            }
        }
        if(number == 2 ||
            number == 3 ||
            number == 4 ||
            number == 5 ||
            number == 6 ||
            number == 8 ||
            number == 9)
        {
            SETAEERECT(&rect, fontRect->x + nWidth, fontRect->y + fontRect->dy - nHeight2 - 1 - (nWidth + 1)/2, nLength - 2*(nWidth - 1), 1);
            if(nWidth%2 == 0)
            {
                rect.y--;
            }
            //�����м�����ĳ���
            {
                rect.x -=  (nWidth - 1)/2;
                rect.dx +=  ((nWidth - 1)/2)*2;
            }
            for(i = 0; i < ((nWidth + 1)/2); i++)
            {
                IDISPLAY_FillRect(pDisplay, &rect, fontColor);
                rect.y += (nWidth - 2*i - 1);
                IDISPLAY_FillRect(pDisplay, &rect, fontColor);
                rect.y -= (nWidth - 2*i - 2);
                rect.x--;
                rect.dx += 2;
            }
        }
        if(number == 0 ||
            number == 2 ||
            number == 3 ||
            number == 5 ||
            number == 6 ||
            number == 8 ||
            number == 9)
        {
            SETAEERECT(&rect, fontRect->x + 1, fontRect->y + fontRect->dy - 1, nLength, 1);
            for(i = 0; i < nWidth; i++)
            {
                IDISPLAY_FillRect(pDisplay, &rect, fontColor);
                rect.x++;
                rect.y--;
                rect.dx -= 2;
            }
        }
        if(number == 0 ||
            number == 4 ||
            number == 5 ||
            number == 6 ||
            number == 8 ||
            number == 9)
        {
            SETAEERECT(&rect, fontRect->x, fontRect->y + 1, 1, nHeight);
            for(i = 0; i < nWidth; i++)
            {
                IDISPLAY_FillRect(pDisplay, &rect, fontColor);
                rect.x++;
                rect.y++;
                rect.dy -= 2;
            }
        }
        if(number == 0 ||
            number == 2 ||
            number == 6 ||
            number == 8)
        {
            SETAEERECT(&rect, fontRect->x, fontRect->y + fontRect->dy - nHeight2 - 1, 1, nHeight2);
            if(number == 0 ||
                number == 1 ||
                number == 7)
            {
                if(nWidth%2 == 0 && fontRect->dy%2 != 0)
                {
                    rect.y--;
                    rect.dy++;
                }
            }
            for(i = 0; i < nWidth; i++)
            {
                IDISPLAY_FillRect(pDisplay, &rect, fontColor);
                rect.x++;
                rect.y++;
                rect.dy -= 2;
            }
        }
        if(number == 0 ||
            number == 1 ||
            number == 2 ||
            number == 3 ||
            number == 4 ||
            number == 7 ||
            number == 8 ||
            number == 9)
        {
            SETAEERECT(&rect, fontRect->x + nLength + 1, fontRect->y + 1, 1, nHeight);
            for(i = 0; i < nWidth; i++)
            {
                IDISPLAY_FillRect(pDisplay, &rect, fontColor);
                rect.x--;
                rect.y++;
                rect.dy -= 2;
            }
        }
        if(number == 0 ||
            number == 1 ||
            number == 3 ||
            number == 4 ||
            number == 5 ||
            number == 6 ||
            number == 7 ||
            number == 8 ||
            number == 9)
        {
            SETAEERECT(&rect, fontRect->x + nLength + 1, fontRect->y + fontRect->dy - nHeight2 - 1, 1, nHeight2);
            if(number == 0 ||
                number == 1 ||
                number == 7)
            {
                if(nWidth%2 == 0 && fontRect->dy%2 != 0)
                {
                    rect.y--;
                    rect.dy++;
                }
            }
            for(i = 0; i < nWidth; i++)
            {
                IDISPLAY_FillRect(pDisplay, &rect, fontColor);
                rect.x--;
                rect.y++;
                rect.dy -= 2;
            }
        }
    }
    return TRUE;
}
/*==============================================================================
����: 
    Appscommon_CompareName

˵��: 
    ���������ڱȽ������ַ����Ա�����

����: 
    wstrNam1 [in]: �ַ���1��
    wstrNam2 [in]: �ַ���2��
����ֵ: 
    1 : wstrNam1>wstrNam2
    0 : wstrNam1==wstrNam2
    -1: wstrNam1<wstrNam2

��ע:
    ����ǵ绰������ OEM ���ڳ�ʼ�� Cache ʱ����ô˺������� Cache ����UI �㲻��������
    ����ģ���������������

==============================================================================*/
int Appscommon_CompareName(AECHAR *wstrNam1,AECHAR *wstrNam2)
{

    AECHAR  alpha1[MAX_STR +1]={0,};
    AECHAR  alpha2[MAX_STR +1]={0,};
    AECHAR  wTemp = 0;

     if (wstrNam1 && !wstrNam2)
    {
        return(1);
    }
    
    if (!wstrNam1 && wstrNam2)
    {
        return(-1);
    }
    
    if (!wstrNam1 && !wstrNam2)
    {
        return(0);
    }
    

    // Convert to letter
    WStr2Letter(alpha1, MAX_STR, wstrNam1);
    ALKWSTRLOWER(alpha1); //brew �Դ����������ַ����к���0xA1���ַ�ʱ������
    
    // Note:Ϊ������ ASCII ��������ַ�����Ե�һ��ת���ַ�����ƫ�����Ĵ���
    //      ������ASCII �������
    if (wstrNam1[0] > OFFSET_ASCII)
    {
        // CHN Unicode
        wTemp = wstrNam1[0];
    }
    
    // Convert to letter
    WStr2Letter(alpha2, MAX_STR, wstrNam2);
    ALKWSTRLOWER(alpha2);   //brew �Դ����������ַ����к���0xA1���ַ�ʱ������
    
    // Note:Ϊ������ ASCII ��������ַ�����Ե�һ��ת���ַ�����ƫ�����Ĵ���
    //      ������ASCII �������
    if (wstrNam2[0] > OFFSET_ASCII)
    {
        if (wTemp)
        {
            // CHN Unicode vs CHN Unicode
            if (alpha1[0] == alpha2[0])
            {
                // �������ĸ��ȣ��򰴱ʻ���������
                if (wTemp > wstrNam2[0])
                {
                    return(1);
                }
                else if (wTemp < wstrNam2[0])
                {
                    return(-1);
                }
                else
                {
                    // Continue to cmp
                }
            }
        }
        else
        {
            // ASCII vs CHN Unicode
#if defined(FEATURE_CARRIER_SUDAN_SUDATEL)
            return (1);
#endif
            return (-1);
        }
    }
    else
    {
        if (wTemp)
        {
            // CHN Unicode vs ASCII
#if defined(FEATURE_CARRIER_SUDAN_SUDATEL)
            return (-1);
#endif
            return(1);
        }
    }
    
    return WSTRCMP(alpha1, alpha2);
}
static void ALKWSTRLOWER(AECHAR * pszDest)
{
   AECHAR   ch;

   if(pszDest)
   {
        while((ch = *pszDest) != (AECHAR)0)
       {
              if(ch >= (AECHAR)'A' && ch <= (AECHAR)'Z') 
             {
                     ch += (AECHAR)('a' - 'A');
              }
              else if(ch >= CON_ACCENT_UPPER_START && ch <= CON_ACCENT_UPPER_END)
		      {          
                     ch += (AECHAR)(CON_ACCENT_LOWER_START - CON_ACCENT_UPPER_START);
              }
              else 
              {
                break;
              }
               *pszDest = ch;
                pszDest++;
         }
   }
   else
   {
   	   return;
   }
}


int GetSingleNumberWidth(IDisplay *pDisplay, AEEFont Font)
{
    AECHAR testNumber[] = {'0','\0'};
    int nNumberWidth = 0;

	if (NULL == pDisplay)
    {
        return;
    } 
    
    nNumberWidth = IDISPLAY_MeasureText(pDisplay, Font, testNumber);

    return nNumberWidth;
}
/*==============================================================================
������
       IsDoubleBytesIME

˵����
       ��������Ϣ��ʽ������ʾ���û���

������

����ֵ��

��ע��:
        
==============================================================================*/
boolean IsDoubleBytesIME(AEETextInputMode imnputmode)
{
    switch(imnputmode)
    {
#if defined(FEATURE_LANG_CHINESE)
    case AEE_TM_PINYIN:
    case AEE_TM_STROKE:
    case AEE_TM_RAPID:
        return TRUE;
#endif// defined(FEATURE_LANG_CHINESE)

#if defined(FEATURE_LANG_ARABIC)
    case AEE_TM_ARABIC:
    case AEE_TM_ARABIC_R:
        return TRUE;
#endif// defined(FEATURE_LANG_ARABIC)

#if defined(FEATURE_LANG_HEBREW)
    case AEE_TM_HEBREW:
    case AEE_TM_HEBREW_R:
        return TRUE;
#endif// defined(FEATURE_LANG_HEBREW)

#if defined(FEATURE_LANG_HINDI)
    case AEE_TM_HINDI:
        return TRUE;
#endif// defined(FEATURE_LANG_HINDI)

#if defined(FEATURE_LANG_RUSSIAN)
    case AEE_TM_RUSSIAN:
    case AEE_TM_RUSSIAN_R:
        return TRUE;
#endif// defined(FEATURE_LANG_RUSSIAN)

#if defined(FEATURE_LANG_THAI)
    case AEE_TM_THAI:
    case AEE_TM_THAI_R:
        return TRUE;
#endif// defined(FEATURE_LANG_THAI)

#if defined(FEATURE_LANG_SPANISH)
    case AEE_TM_SPANISH_EU:
    case AEE_TM_SPANISH_EU_R:
    case AEE_TM_SPANISH_LA:
    case AEE_TM_SPANISH_LA_R:
        return TRUE;
#endif// defined(FEATURE_LANG_SPANISH)

#if defined(FEATURE_LANG_PORTUGUESE)
    case AEE_TM_PORTUGUESE_BR:
    case AEE_TM_PORTUGUESE_BR_R:
    case AEE_TM_PORTUGUESE_EU:
    case AEE_TM_PORTUGUESE_EU_R:
        return TRUE;
#endif// defined(FEATURE_LANG_PORTUGUESE)

#if defined(FEATURE_LANG_ITALIAN)
    case AEE_TM_ITALIAN:
    case AEE_TM_ITALIAN_R:
        return TRUE;
#endif// defined(FEATURE_LANG_ITALIAN)

#if defined(FEATURE_LANG_ROMANIAN)
    case AEE_TM_ROMANIAN:
    case AEE_TM_ROMANIAN_R:
        return TRUE;
#endif// defined(FEATURE_LANG_ROMANIAN)

#if defined(FEATURE_LANG_VIETNAMESE)
    case AEE_TM_VIETNAMESE:
    case AEE_TM_VIETNAMESE_R:
        return TRUE;
#endif// defined(FEATURE_LANG_VIETNAMESE)

//-------------------------------------------------------------------
//             Single bytes
//-------------------------------------------------------------------
#if defined(FEATURE_INPUTMODE_INDONESIAN)
    case AEE_TM_INDONESIAN:
    case AEE_TM_INDONESIAN_R:
        return FALSE;
#endif // defined(FEATURE_INPUTMODE_INDONESIAN)

    case AEE_TM_NUMBERS:
    case AEE_TM_LETTERS:
    case AEE_TM_EZTEXT:
    //case AEE_TM_ENGLISH_UK:
    //case AEE_TM_ENGLISH_UK_R:
    //case AEE_TM_ENGLISH_US:
    //case AEE_TM_ENGLISH_US_R:
    default:
        return FALSE;
    }
}


/*==============================================================================
����: 
    HaveNoneASCIIChar

˵��: 
    �жϿ��ַ������Ƿ��з�ASCII�ַ���

����: 
    pWstr [in]: ָ����ַ�����
    nPostion [out]: ��ASCII�ַ���һ�γ��ֵ�λ�á�

����ֵ: 
    �����з�ASCII�ַ�������TRUE������FALSE��

��ע: 

==============================================================================*/
boolean HaveNoneASCIIChar ( AECHAR * pWstr, int * pPostion)
{
    int i ,len;
    AECHAR *pWstrTep = pWstr;

    if (NULL == pWstrTep)
    {
        return FALSE;
    }
    
    len = WSTRLEN(pWstrTep);
    if (len <= 0)
    {
        return FALSE;
    }
#if defined(FEATURE_SMS_GSM7BIT)
    for (i=0; ((i < len) && ((*pWstr) < UCS2_TO_GSM_TABLE_SIZE)); i++,pWstr++)
#else
    for (i=0; ((i < len) && ((*pWstr) < 128)); i++,pWstr++)
#endif    
    {
        //ERR("i='%c',=%d",*pWstr,*pWstr,0);//Azlun
        ;
    }
    
    if (i < len)
    {
        if (NULL != pPostion)
        {
            *pPostion = i;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*==============================================================================
����: 
    Appscommon_ResetBackground

˵��: 
    ����ĳ������ı���ͼƬ��

����: 
    pDisplay [in]: 
    BgImage [in]: ����ͼƬָ��
    BgColor [in]: ������ɫ
    rect [in]: ��Ҫ���Ƶľ��η�Χ
    x,y[in]: ͼƬ���Ͻ�����Ļ�е�����������������
    
����ֵ: 
    ��

��ע:
    �������ͼƬ��Ϊ�գ������ͼƬ��ָ�����η�Χ��
    ����ָ����Χ���Ϊ����ɫ��

==============================================================================*/
void Appscommon_ResetBackground(IDisplay *pDisplay, IImage *BgImage, RGBVAL BgColor, AEERect * rect, int x, int y)
{
    if(pDisplay == NULL)
    {
        return;
    }
    
    if(NULL != BgImage)
    {
        AEERect oldClip = {0};
        
        IDisplay_GetClipRect(pDisplay, &oldClip);
        IDisplay_SetClipRect(pDisplay, rect);
        IImage_Draw(BgImage, x, y);
        IDisplay_SetClipRect(pDisplay, &oldClip);
    }
    else
    {
        IDisplay_FillRect(pDisplay, rect, BgColor);
    }
}


/*==============================================================================
����: 
    Appscommon_DrawScrollBar

˵��: 
    ���ƹ�������

����: 
    pDisplay [in]: 
    nCurrentIdx [in]: ��ǰҳ��������һ������ֵ(��1Ϊ���)
    nTotalItem [in]: ��Ŀ����
    nPageItem[in]: ����Ļ�����ʾ��Ŀ��
    ScrollRect [in]: ��Ҫ���Ƶľ��η�Χ
    
����ֵ: 
    ��

��ע:

==============================================================================*/
#include "msg.h"
void Appscommon_DrawScrollBar(IDisplay *pDisplay, int nCurrentIdx, int nTotalItem, int nPageItem, AEERect *ScrollRect)
{
    AEERect rctThumb, rectScrollBar = *ScrollRect;
    int   x, y, dx, dy, nRange;
    RGBVAL cScrollbar = MAKE_RGB(0xDE, 0xDE, 0xDE), 
    cScrollbarFill = /*MAKE_RGB(0xFF, 0xC0, 0x60)*/MAKE_RGB(0xFF, 0x70, 0x00), 
               cBackGround = APPSCOMMON_BG_COLOR;

    if(nTotalItem <= nPageItem || pDisplay == NULL)
    {
        return;
    }
#ifdef FEATURE_RANDOM_MENU_COLOR
    OEM_GetConfig(CFGI_MENU_BGCOLOR, &cBackGround, sizeof(cBackGround));
#endif

    IDISPLAY_FillRect(pDisplay, ScrollRect, cBackGround);
    rectScrollBar.x += 2*AEE_FRAME_SIZE;
    rectScrollBar.dx -= 2*AEE_FRAME_SIZE;
#ifdef FEATURE_SCROLLBAR_USE_STYLE
    nRange = ScrollRect->dy - 4*AEE_FRAME_SIZE;
    y = ScrollRect->y + 2*AEE_FRAME_SIZE;
    x = rectScrollBar.x;
    dx = rectScrollBar.dx;
#else
    nRange = ScrollRect->dy - 2*AEE_FRAME_SIZE;
    y = ScrollRect->y + AEE_FRAME_SIZE;
    x = rectScrollBar.x + AEE_FRAME_SIZE;
    dx = rectScrollBar.dx - 2*AEE_FRAME_SIZE;
#endif
    dy = MAX(3,(nRange * nPageItem)/nTotalItem);
    if(nCurrentIdx > nPageItem)
    {
        y = MIN(y + nRange - dy, y + ((nRange - dy) * (nCurrentIdx - nPageItem))/(nTotalItem - nPageItem));
    }
    SETAEERECT(&rctThumb, x, y, dx, dy);
    
#ifdef FEATURE_SCROLLBAR_USE_STYLE
    IDISPLAY_FillRect(pDisplay, &rectScrollBar, cScrollbar);
    IDISPLAY_FillRect(pDisplay, &rctThumb, cScrollbarFill);
    {
        AEERect rectScrollbarDark;
        RGBVAL  cScrollbarDark = MAKE_RGB(0xFF, 0x70, 0x00);
        
        x += AEE_FRAME_SIZE;
        y -= AEE_FRAME_SIZE;
        dx -= 2*AEE_FRAME_SIZE;
        dy += 2*AEE_FRAME_SIZE;
        SETAEERECT(&rectScrollbarDark, x, y, dx, dy);
        IDISPLAY_FillRect(pDisplay, &rectScrollbarDark, cScrollbarDark);
    }
#else
    IDISPLAY_DrawFrame(pDisplay, &rectScrollBar, AEE_FT_BOX, cScrollbar);
    IDISPLAY_FillRect(pDisplay, &rctThumb, cScrollbarFill);
#endif
}
#ifdef FEATURE_LANG_CHINESE
AECHAR CNUnicode2Letter(AECHAR c)
{
    CNUni2Letter(c);
    
    return c;
}

/*=============================================================================

Macro : Letter2Alpha

DESCRIPTION: 
    ��Ӣ����ĸת��Ϊ��Ӧ�İ����ַ�
    
PARAMETERS:
    letter:
    
=============================================================================*/
AECHAR CNLetter2Alpha(AECHAR letter)                                        
{                                                           
    AECHAR  temp[2] = {'\0', '\0'};                         
    temp[0] = letter;                                       
    WSTRLOWER(temp);                                        
    letter = temp[0];                                       
    if(letter >= 'a' && letter <= 'z')                      
    {                                                       
        letter = (AECHAR)letter2alphaTable[letter -'a'];    
    }       

    return letter;
}

#endif
/*==============================================================================
����: 
    Appscommon_GetRandomColor

˵��: 
    ���ƹ�������

����: 
    nMaxVal [in]: ����ƽ��ֵ���ֵ���������ƻ�ȡɫ�ʵ����ȣ�
    �粻��Ҫ���ƣ�����Ϊ255����
    
����ֵ: 
    ��õ���ɫRGBֵ

��ע:

==============================================================================*/
RGBVAL Appscommon_GetRandomColor(uint32 nMaxVal) 
{
    byte          nRGBVal[3];
    RGBVAL      nColor;

    GETRAND(nRGBVal, sizeof(nRGBVal));
    
    if(nRGBVal[0] + nRGBVal[1] + nRGBVal[2] > nMaxVal*3)
    {
        nRGBVal[0] = (nRGBVal[0] > nMaxVal)?(nRGBVal[0]*nMaxVal/255):(nRGBVal[0]);
        nRGBVal[1] = (nRGBVal[1] > nMaxVal)?(nRGBVal[1]*nMaxVal/255):(nRGBVal[1]);
        nRGBVal[2] = (nRGBVal[2] > nMaxVal)?(nRGBVal[2]*nMaxVal/255):(nRGBVal[2]);
    }

    nColor = MAKE_RGB((uint32)nRGBVal[0], (uint32)nRGBVal[1], (uint32)nRGBVal[2]);
    return nColor;
}

/*==============================================================================
����: 
    Appscommon_ResetBackgroundEx

˵��: 
    ����ĳ������ı���ɫ�Լ�ͼƬ��

����: 
    pDisplay [in]: 
    rect [in]: ��Ҫ���Ƶľ��η�Χ
    bDrawImage[in] : �Ƿ����ͼƬ��FALSE����䱳��ɫ
    
����ֵ: 
    ��

��ע:
    ��ָ����Χ���ΪĬ�ϵı���ɫAPPSCOMMON_BG_COLOR������������ΪTRUEʱ��
    ����Ĭ�ϱ���ͼƬ��

==============================================================================*/
void Appscommon_ResetBackgroundEx(IDisplay *pDisplay, AEERect * rect, boolean bDrawImage)
{
    RGBVAL nBgColor = APPSCOMMON_BG_COLOR;

    if(pDisplay == NULL)
    {
        return;
    }
    
#ifdef FEATURE_RANDOM_MENU_COLOR
    (void)OEM_GetConfig(CFGI_MENU_BGCOLOR, &nBgColor, sizeof(nBgColor));
#endif
        
    if(!bDrawImage)
    {
        IDisplay_FillRect(pDisplay, rect, nBgColor);
    }
    else
    {
        IImage *pImageBg = NULL;
        IShell  *pShell = AEE_GetShell();
        int      xPos = 0, yPos = 0;
#ifdef FEATURE_RANDOM_MENU_COLOR
        byte     nRandomMenu = 0;

        (void)OEM_GetConfig(CFGI_RANDOM_MENU, (void*)&nRandomMenu, sizeof(nRandomMenu));
		#if 0
        if(nRandomMenu != 0)
        {
            /* ��͸��ͨ����ͼƬ������С�ߴ磬����������ɫ�Ĳ��֣���ʼ��������*/
            xPos = APPSCOMMON_MENUBG_XPOS;
            yPos = APPSCOMMON_MENUBG_YPOS;
            pImageBg = ISHELL_LoadResImage(pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_MENU_BACKGROUND_TRANS);
            IDisplay_FillRect(pDisplay, rect, nBgColor);
        }
        else
	    #endif
#endif
        {
            /* ����͸��ͨ�����󽵵�ͼ�����ʾ�ٶȣ�Ĭ��ʹ��һ����͸��ɫ��ͼƬ*/
            pImageBg = ISHELL_LoadResImage(pShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BACKGROUND);  //moci by yangdecai
        }

        Appscommon_ResetBackground(pDisplay, pImageBg, nBgColor, rect, xPos, yPos);

        if(pImageBg != NULL)
        {
            IImage_Release(pImageBg);
            pImageBg = NULL;
        }
    }
}
/*******************************************************************************
**
** Function         Appscommon_bluetooth_used_sd
**
** Description
**                  
**
** Returns          int
*******************************************************************************/
int Appscommon_bluetooth_used_sd(int aa)
{
#ifdef FEATURE_SUPPORT_BT_APP
    static int g_bluetoothused_sd = 0;
    if (aa>=0)
    {
        g_bluetoothused_sd = aa;
    }
    return g_bluetoothused_sd;
#else
    return 0;
#endif
}
/*==============================================================================
����:
    IPAdd_FromNetValueToString

˵��:
    ������IP��ַ������ֵת��Ϊ�ַ�����ʽ��

����:
    dwAddNet [in]��IP��ַ����ֵ��
    pszbuf [in/out]�����ؽ���� Buffer��
    nSize [in]��Buffer ��С��

����ֵ:
    TRUE: ת���ɹ�
    FALSE:ת��ʧ��

��ע:

==============================================================================*/
boolean IPAdd_FromNetValueToString(uint32 dwAddNet, char *pszbuf, int nSize)
{
    uint8 b1, b2, b3, b4;
    
    if (!pszbuf || (nSize < 16))
    {
        return FALSE;
    }
    
    MEMSET(pszbuf, 0, nSize);
    
    b4 = (dwAddNet >> 24);
    b3 = (dwAddNet >> 16) & 0xff;
    b2 = (dwAddNet >> 8) & 0xff;
    b1 = dwAddNet & 0xff;
    
    SPRINTF(pszbuf, "%d.%d.%d.%d", b1, b2, b3, b4);
    
    return TRUE;
}

/*==============================================================================
����:
    IPAdd_FromStringToNetValue

˵��:
    ������IP��ַ���ַ�����ʽת��Ϊ����ֵ��

����:
    pdwAddNet [in/out]������IP��ַ����ֵ�� Buffer��
    pszbuf [in]���ַ�����ʽ��IP��ַ��

����ֵ:
    TRUE: ת���ɹ�
    FALSE:ת��ʧ��

��ע:

==============================================================================*/
boolean IPAdd_FromStringToNetValue(uint32 *pdwAddNet, char *pszbuf)
{
    int     nByte = 0;
    char    c;
    char    *pstrTep = NULL;   
    
    if (!pdwAddNet || !pszbuf)
    {
        return FALSE;
    }
    pstrTep = pszbuf;
    c = *pstrTep;

    while ((c >= '0') && (c <= '9')) 
    {
        int n = 0;
        
        while ((c >= '0') && (c <= '9'))
        {
            n = n*10 + (c - '0');
            ++pstrTep;
            c = *pstrTep;
        }
    
        ((char*)pdwAddNet)[nByte++] = n;
    
        if (nByte == 4 || *pstrTep != '.')
        {
            break;
        }
    
        ++pstrTep;
        c = *pstrTep;
    }
    
    c = *pstrTep;

    if (nByte < 4 || 
        ((c >= '0') && (c <= '9')) ||
        (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'))))  
    {
        *pdwAddNet = 0xFFFFFFFF;    // Invalid address
        return FALSE;
    }

    return TRUE;
}

/*==============================================================================
����:
    Appscommon_FormatTimeString

˵��:
    ������ʱ��ת��Ϊ�ַ�����ʾ��ʽ��

����:
    nSeconds [in]����Ҫת����ʱ�䡣
    wstrTime [in]������ַ��������ַ��
    nTimeStringLength[in]������ַ������������ȡ�

����ֵ:
    TRUE: ת���ɹ�
    FALSE:ת��ʧ��

��ע:

==============================================================================*/
boolean Appscommon_FormatTimeString(uint32 nSeconds, AECHAR* wstrTime, int nTimeStringLength)
{
    if( NULL == wstrTime || nTimeStringLength < 9*(sizeof(AECHAR)))
    {
        return FALSE;
    }
    else
    {
        JulianType      julianInfo       = {0};
        AECHAR         formatString[16] = {0};
        byte              timeFormatType      = 0;

        if(nSeconds > 0)
        {
            GETJULIANDATE( nSeconds, &julianInfo);
        }
        MEMSET( formatString, 0, sizeof(formatString));

        OEM_GetConfig( CFGI_TIME_FORMAT, &timeFormatType, sizeof( byte));
        
#if defined( AEE_SIMULATOR)
        timeFormatType = OEMNV_TIMEFORM_AMPM;
#endif

        if (timeFormatType == OEMNV_TIMEFORM_AMPM)
        {
            if( julianInfo.wHour >= 12)
            {
                STRTOWSTR( "%02d:%02d PM", formatString, sizeof( formatString));
            }
            else
            {
                STRTOWSTR( "%02d:%02d AM", formatString, sizeof( formatString));
            }

            julianInfo.wHour = (julianInfo.wHour > 12) ? (julianInfo.wHour - 12) : julianInfo.wHour;
        }
        else
        {
            STRTOWSTR( "%02d:%02d", formatString, sizeof( formatString));
        }

        // ��ʽ��ʱ���ַ���
        MEMSET( wstrTime, 0, nTimeStringLength);
        WSPRINTF( wstrTime, nTimeStringLength, formatString, julianInfo.wHour, julianInfo.wMinute);

        return TRUE;
    }
}

/*==============================================================================
����: 
    app_media_scheduler
       
˵��: 
    �жϵ�ǰ��Ҫ�Ķ�ý���豸(VC0848)�Ƿ��ѱ�ռ��
       
����: 
    none
����ֵ:
    APP_MEDIA_ALLOW:            ����ʹ��
    APP_MEDIA_IMPACT_BY_FM:     �ѱ�FMռ��
    APP_MEDIA_IMPACT_BY_MP3:    �ѱ�MP3ռ��
       
��ע:
    
==============================================================================*/
int app_media_scheduler(void)
{
    boolean b_FMBackground = FALSE;
  
    OEM_GetConfig(CFGI_FM_BACKGROUND,&b_FMBackground, sizeof(b_FMBackground));
    if((TRUE == b_FMBackground) && (AEECLSID_APP_FMRADIO != ISHELL_ActiveApplet(AEE_GetShell())))
    {
        return APP_MEDIA_IMPACT_BY_FM;
    }
#ifdef FEATURE_APP_MUSICPLAYER    
    else if(IsMp3PlayerStatusOnBG()) 
    {
        return APP_MEDIA_IMPACT_BY_MP3;        
    }
#endif    
    return APP_MEDIA_ALLOW;


}

