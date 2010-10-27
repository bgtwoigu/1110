/*=============================================================================

FILE: OEMText.c

SERVICES: OEM Functions for Text Control support

GENERAL DESCRIPTION:
A sample implementation of the OEMTextCtl functions declared in
OEMTextCtl.h that are used in the implementation of AEETextCtl.

PUBLIC CLASSES AND STATIC FUNCTIONS:
OEM_TextCreate
OEM_TextDelete
OEM_TextSet
OEM_TextGet
OEM_TextDraw
OEM_TextGetModeString
OEM_TextSetEdit
OEM_TextGetCurrentMode
OEM_TextAddChar
OEM_TextQueryModes
OEM_TextKeyPress
OEM_TextEnumModesInit
OEM_TextEnumMode
OEM_TextGetCursorPos
OEM_TextSetCursorPos

INITIALIZATION AND SEQUENCING REQUIREMENTS:
No additional module-level initialization or sequencing requirements.

        Copyright ?2000,2001,2002 QUALCOMM Incorporated.
               All Rights Reserved.
            QUALCOMM Proprietary/GTDR
=============================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  $Header: //tsg/UI/main/TSGBREWOEM/src/OEMText.c#6 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/01/08   bw      Build for T9 Alphabet
15/01/08   bw      Add T9 Traditional Chiese

===========================================================================*/

// Suppress "ANSI limit of 511 'external identifiers' exceeded"
//lint -"esym(793,external identifiers)"

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "OEMFeatures.h"

#ifdef OEMTEXT
#include "OEMText.h"
#include "OEMHeap.h"
#include "OEMClassIDs.h"
#include "OEMITSIM.h"
#include "AEE_OEM.h"

#ifdef AEE_DEBUG
#include "msg.h"
#endif

#ifdef FEATURE_T9_INPUT
#include "t9api.h"  
#include "chinconv.h"
#endif //#ifdef FEATURE_T9_INPUT
#include "Appscommon.h"
#include "appscommonimages.brh"
#include "err.h"
#include "OEMQuertkey.h"
/*===========================================================================

                    DEFINITIONS AND CONSTANTS

===========================================================================*/
#define LINEBREAK               ((AECHAR) '\n')
                                
#ifdef FEATURE_T9_INPUT
#define MAX_BUFFER_LENGTH       MAX_INPUTTEXT_SIZE
#define OEM_IME_DIALOG_CTL_ITSIM    (0x7ffc)         //the ID of the ITSIM control.

#define OEM_IME_DIALOG_CTL_IMUI     (0x7FFE)         //the ID of the IMUI control.


#define  PTRCK_HIT_ABOVE   (0x01)      // Above the thumb, y position < thumb.y OR x position < thumb.x
#define  PTRCK_HIT_THUMB   (0x02)      // On the thumb
#define  PTRCK_HIT_ITEM    (0x02)      // Matches above, overloaded for non SB entities
#define  PTRCK_HIT_BELOW   (0x04)      // Below the thumb, y position > thumb.y + thumb.dy OR x position < thumb.x + thumb.dx
#define  PTRCK_HIT_SCRL    (0x08)      // 1: Hit Scrollbar; 0: Didn't hit Scrollbar
#define  PTRCK_HIT_ABTRI   (0x10)
#define  PTRCK_HIT_BLTRI   (0x20)
#define  PTRCK_NOTTHUMB    (PTRCK_HIT_BELOW | PTRCK_HIT_ABOVE)
#define  PTRCK_HIT_TRIAN   (PTRCK_HIT_ABTRI | PTRCK_HIT_BLTRI)

#ifdef FEATURE_T9_CHINESE
#define SELECTION_BUFFER_SIZE   (8)
#define CAUDB_SIZE              (110)
#define T9KEYTYPE_NORMAL        (0)
#define T9KEYTYPE_SELECT        (1)
#define T9KEYTYPE_CONTROL       (2)
#define T9KEYTYPE_UNKNOWN       (6)
#define T9_FONT_WIDTH           (20)
#define T9_STROKE_FONT_WIDTH    (12) // (10) 
#define T9_STROKE_LEFT_ARROW    (10) 
#define MAX_STROKES             (9) // (10) // the max count which display in the screen

#ifndef AEE_SIMULATOR
#define SYLLABLEWIDTH  15
#define SEPARATORWIDTH 6
#define PSYLLABLEWIDTH  8   
#define PSEPARATORWIDTH 3   // 4
#define SPELLMAX  10
#define SPACESIZE  5  //6
#define CHINESE_FONT_HEIGHT 14
#define CHINESE_FONT_WIDTH 14    
#else 
//add by ydc
#define SYLLABLEWIDTH  15//9
#define SEPARATORWIDTH  6//3
#define PSYLLABLEWIDTH  8   
#define PSEPARATORWIDTH 3  // 4
#define SPELLMAX  10
#define SPACESIZE  5  //7
// Gemsea end
#define CHINESE_FONT_HEIGHT 17
#define CHINESE_FONT_WIDTH  16
#endif //AEE_SIMULATOR

#endif //#ifdef FEATURE_T9_CHINESE
#endif //#ifdef FEATURE_T9_INPUT

static int snTextModeIndex = 0;
#define TIMEOUT   460
#define TEXT_BETWEEN_LINE_PIXEL   (3)
#define INPUT_BETWEEN_LINE_PIXEL   (2)
#define  PTRCK_GEN_TMRSET  (0x01)   // Timer is set
#define TSIM_MODE      (2)

typedef struct
{
	uint8    cbHit;
	uint8    cbFlags;
	uint16   wData;
	AEEPoint ptPosition;
} PenTracker;

OBJECT(T9ChineseCxt){
	IDialog *      pDlg;
};

#define PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
/*===========================================================================

                    MACROS

===========================================================================*/

/*===========================================================================

                    TYPE DEFINITIONS

===========================================================================*/

typedef struct _MultitapStateInfo {
   AVKType kLast;    // Last key pressed
   int nSubChar;     // Which character in multitap string currently
                     // displayed and highlighted
   int nMax;         // Number of characters in multitap string
   int nCapsState;   // Capslock state, 0=>lower, 1=>upper
} MultitapStateInfo;

typedef union _ModeStateInfo {
   // Add other substate info as required
   MultitapStateInfo mtap;
} ModeStateInfo;

typedef struct _TextCtlContext {
   IShell               *pIShell;
   IDisplay             *pIDisplay;
   AEECLSID             clsMe;             // To keep old behaviors.
   uint32               dwProperties;
   AEERect              rectDisplay;
   AECHAR               *pszContents;
   unsigned short       wContentsChars;
   unsigned char        byMode;            // index into sTextModes
   unsigned short       wSelStart;
   unsigned short       wSelEnd;
   unsigned short       wDisplayStartLine;
   unsigned short       wMaxChars;        // 0 => no maximum
   uint16               wScrollBarWidth;  // Includes 1-pixel white border
   AEEFont              font;
   short                nFontAscent;
   short                nFontDescent;
   short                nFontLeading;
   short                nDisplayLines;
   short                nExtraPixels;
   unsigned short       wLines;
   unsigned short       *pwLineStarts;    // Array of wLines+1 entries
   int32                nLineHeight;

   //add by ydc
   PenTracker        ptTracker;
   IDialog *         pIDialog;
   AEEAutoRepeat     arPenRepeat;
   flg               bdowntsim:1;//record if the pen has hit the textrange in tsim
   flg               binorig:1;//record if it is in the original state
   AEERect           rc_text;
   CoordiRange       textrange;//the range of the text area
   T9ChineseCxt      T9Cxt;
   flg               flgPenDown;       
   //end ydc
   flg                  bValid:1;
   flg                  bEditable:1;
   flg                  bNeedsDraw:1;
   //flag added to indicate when the maximum char i.e the last char has timed
   // out. Used in multitap so that the char after the max char should not
   // replace the last char, instead they should be ignored
   flg                  bMaxCharTimedOut:1;
   flg                  bSwap:1;         // Determines if we swapped SelStart with SelEnd
                                         // Useful to track actual selection beginning point
                                         // without rewriting a bunch of start/end logics.

   ModeStateInfo        uModeInfo;         // Text mode entry state info   
   MultitapCapsState    nMultitapCaps;
#ifdef FEATURE_FUNCS_THEME    
   RGBVAL               m_themeColor;
#endif //FEATURE_FUNCS_THEME 
#ifdef FEATURE_T9_INPUT  
   sFocusType           sFocus;  
#ifdef FEATURE_T9_ALPHABETIC
   T9AWFieldInfo        sT9awFieldInfo;
#endif //#ifdef FEATURE_T9_ALPHABETIC   
#ifdef FEATURE_T9_CHINESE
   T9CCFieldInfo        sT9ccFieldInfo;
   short                nSelectionSelectd; // The current HanZi which in the Selection is selected.
   AEERect              rectChineseInput;
   AEERect              rectChineseSyllableInput;
   AEERect              rectChineseTextInput;   
#endif //#ifdef FEATURE_T9_CHINESE  
#endif //FEATURE_T9_INPUT

#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
   IFont                *m_pMyFont;
   uint32               dwAlignFlags ;
   AEERect             CursorDrawRectTimerPara;
#endif
   IImage               *m_pImageBg;
   boolean              is_isShift;
   boolean              m_bCaplk;
} TextCtlContext;

typedef boolean         (*PFN_ModeCharHandler)(TextCtlContext *,AEEEvent, AVKType);
typedef const AECHAR *  (*PFN_ModeGetString)(TextCtlContext *);
typedef void            (*PFN_ModeChange)(TextCtlContext *);

typedef struct _ModeInfo {
   PFN_ModeChange       pfn_restart;   // NULL means no restart handling required
   PFN_ModeCharHandler  pfn_char;
   PFN_ModeGetString    pfn_string; // NULL means use standard name
   PFN_ModeChange       pfn_exit;      // NULL means no exit handling required
   AEETextMode          info;
} ModeInfo;

/*===========================================================================

                    EXTERNAL DATA

===========================================================================*/

/*===========================================================================

                    LOCAL FUNCTION PROTOTYPES

===========================================================================*/
// Normal Number
static void TextCtl_NumbersRestart(TextCtlContext *);
static boolean TextCtl_NumbersKey(TextCtlContext *,AEEEvent, AVKType);
static void TextCtl_NumbersExit(TextCtlContext *);
static void TextCtl_MultitapTimer(void *pUser);

#ifdef FEATURE_T9_INPUT
#ifdef FEATURE_T9_ALPHABETIC
// T9 Latin Rapid
static void T9TextCtl_Latin_Rapid_Restart(TextCtlContext *pContext);
static boolean T9TextCtl_Latin_Rapid_Key(TextCtlContext *,AEEEvent,AVKType);
static void T9TextCtl_Latin_Rapid_Exit(TextCtlContext *pContext);

// T9 Latin Multitap
static void T9TextCtl_MultitapRestart(TextCtlContext *pContext);
static boolean T9TextCtl_MultitapKey(TextCtlContext *,AEEEvent,AVKType);
static void T9TextCtl_MultitapExit(TextCtlContext *pContext);

//T9 Cap Lower   yangdecai 2010-09-09
static void T9TextCtl_Cap_Lower_Restart(TextCtlContext *pContext);
static boolean T9TextCtl_Cap_Lower_Rapid_Key(TextCtlContext *,AEEEvent,AVKType);
static void T9TextCtl_Cap_Lower_Rapid_Exit(TextCtlContext *pContext);


// T9 Commone function
static void T9_AW_Init(TextCtlContext *pContext);
static void T9_AW_Destroy(TextCtlContext *pContext);
static boolean T9_AW_DisplayText(TextCtlContext *pContext, AVKType key);
static T9KEY T9_BrewKeyToT9AlphabeticKey(TextCtlContext *pContext,AEEEvent, AVKType cKey);

/* This is static because the only use of it is in setting a pointer.
   It is T9FARCALL because it might be called from anywhere. */
static T9STATUS T9FARCALL T9_AW_HandleRequest(T9FieldInfo *pFieldInfo, T9Request *pRequest)
{return (0);}
extern T9U32 T9FARCALL T9ReadLdbData(T9FieldInfo *pFieldInfo, T9U32 dwOffset, T9FARPOINTER *ppbDst, T9U32 *pdwStart, T9UINT nSize, T9MINOR mCacheIndex);
#endif //#ifdef FEATURE_T9_ALPHABETIC

#ifdef FEATURE_T9_CHINESE
// CJK CHINESE
static void T9TextCtl_CJK_CHINESE_Restart(TextCtlContext *pContext);
static boolean T9TextCtl_CJK_CHINESE_Key(TextCtlContext *,AEEEvent,AVKType);
static void T9TextCtl_CJK_CHINESE_Exit(TextCtlContext *pContext);

static void T9_CJK_CHINESE_DisplaySelection(TextCtlContext *pContext);
static void T9_CJK_CHINESE_DrawStrokeString(TextCtlContext *pContext);

static void T9_CJK_CHINESE_DisplaySyllable ( TextCtlContext *pContext );
static void T9_CJK_CHINESE_DrawSyllableString ( TextCtlContext *pContext );

static T9STATUS T9_CJK_CHINESE_Init(TextCtlContext *pContext);
static void T9_CJK_CHINESE_Destroy(TextCtlContext *pContext);
static boolean T9_CJK_CHINESE_DisplayText(TextCtlContext *pContext);
static T9KEY T9_CJK_CHINESE_BrewKeyToT9Key(TextCtlContext *pContext,AEEEvent, AVKType cKey);
T9STATUS T9FARCALL T9_CJK_CHINESE_HandleRequest(T9FieldInfo *pFieldInfo, T9Request *pRequest);
#endif // #ifdef FEATURE_T9_CHINESE
#endif //FEATURE_T9_INPUT


// All Commone function
static void TextCtl_RestartEdit(TextCtlContext *);
static void TextCtl_NoSelection(TextCtlContext *);
static void TextCtl_AddChar(TextCtlContext *, AECHAR ch);
static void TextCtl_AddString(TextCtlContext *, AECHAR *);
static boolean TextCtl_SetSel(TextCtlContext *, int selStart, int selEnd);
static void TextCtl_TextChanged(TextCtlContext *);
static boolean TextCtl_CalText(TextCtlContext *);
static boolean TextCtl_CalText2(TextCtlContext *, boolean);
static boolean TextCtl_AutoScroll(TextCtlContext *);
static void TextCtl_DrawScrollBar(TextCtlContext *);
static void TextCtl_DrawTextPart(TextCtlContext *, boolean, boolean);
static unsigned char FindModeIndex(TextCtlContext *, AEETextInputMode);
static uint16 TextCtl_GetLine(TextCtlContext *pContext, uint16 nSelIndex);

//add by ydc
static void      TextCtl_StepTimerCB(TextCtlContext *pme);
static void      TextCtl_ScrollTimerCB(TextCtlContext *pme);
static boolean   TextCtl_StepByPos(TextCtlContext * pme, int nDir, int16 wXPos, int16 wYPos);
static boolean   TextCtl_ScrollByPos(TextCtlContext * pme, int nDir, int16 wXPos, int16 wYPos);
static uint8     TextCtl_PenHitToTrack(TextCtlContext * pme, int16 wXPos, int16 wYPos, uint16 * pwData, AEEPoint * pptPosition);
static int       TextCtl_GetScrollBarRects(TextCtlContext * pme, AEERect * prcFrame, AEERect * prcThumb);
static void      TSIM_CreateDlg(TextCtlContext *pContext);
static boolean   TSIM_DlgHandleEvent(void * pUser, AEEEvent evt, uint16 wparam, uint32 dwparam);
static boolean   TextCtl_IsInRange(int16 xpos, int16 ypos, CoordiRange* range);
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
static void TextCtl_DrawCursorTimer(TextCtlContext *pContext);
extern uint32 ParagraphAlignment(AECHAR *pszText, int nChars);                 
#endif

#ifdef FEATURE_T9_MT_THAI  
//̩���ַ��Ƿ����ڸ���
static boolean isThaiCnsntChar(AECHAR inputChar);
//̩���ַ��Ƿ�������Ԫ��
static boolean isThaiAboveVowelChar(AECHAR inputChar);
//̩���ַ��Ƿ�������Ԫ��
static boolean isThaiBelowVowelChar(AECHAR inputChar);
//̩���ַ��Ƿ���������
static boolean isThaiToneChar(AECHAR inputChar);
//�ж�ǰһ���ַ��Ƿ�����Ԫ��������,��������ƶ�
static int moveleftselThaiChar(AECHAR pretwoChar,AECHAR preChar);
//�жϺ�һ���ַ��Ƿ�����Ԫ��������,��������ƶ�
static int moverightselThaiChar(AECHAR nexttwoChar,AECHAR nextChar);
#endif /*FEATURE_T9_MT_THAI*/

#if defined(FEATURE_WMS_APP)
extern int WMSUtil_CalculateMessagesCount(AECHAR *pWstr, 
                                          int *pencoding,
                                          int *pnMaxcharsOneitem);
#endif                                          
 /*quit the number method in add the contact number and ip number set on 080903*/
// define the function 
static void TextCtl_NumbersTimer(void *pUser);
/* add the code end */
static void TextCtl_DrawBackGround(TextCtlContext *pContext, AEERect *pRect);
/*===========================================================================

                    STATIC/LOCAL DATA

===========================================================================*/
#ifdef FEATURE_T9_INPUT
    typedef struct T9KeyMap_s 
    {
        T9KEY       mKey;
        AVKType     cKey;
    } T9KeyMap;
    
#ifdef FEATURE_T9_ALPHABETIC
/* Translates alphabetic keys to T9 key codes */
T9KeyMap Alphabetic2T9Map[] = 
{
    {T9KEYAMBIG1,    AVK_1}, {T9KEYAMBIG2,    AVK_2}, {T9KEYAMBIG3,    AVK_3},
    {T9KEYAMBIG4,    AVK_4}, {T9KEYAMBIG5,    AVK_5}, {T9KEYAMBIG6,    AVK_6}, 
    {T9KEYAMBIG7,    AVK_7}, {T9KEYAMBIG8,    AVK_8}, {T9KEYAMBIG9,    AVK_9}, 
    {T9KEYSPACE,     AVK_0}, {T9KEYNONE,   AVK_STAR}, {T9KEYNONE,  AVK_POUND},
    {T9KEYNONE, AVK_SELECT}, {T9KEYCLEAR,   AVK_CLR}, {T9KEYNEXT,   AVK_DOWN}, 
    {T9KEYPREV,     AVK_UP}, {T9KEYLEFT,   AVK_LEFT}, {T9KEYRIGHT, AVK_RIGHT},  
    {T9KEYNONE,   AVK_SEND}, {T9KEYNONE,    AVK_END}, {0,0}
};

T9KeyMap MultitapAlphabetic2T9Map[] = 
{
    {T9KEYAMBIG1,    AVK_1}, {T9KEYAMBIG2,    AVK_2}, {T9KEYAMBIG3,    AVK_3},
    {T9KEYAMBIG4,    AVK_4}, {T9KEYAMBIG5,    AVK_5}, {T9KEYAMBIG6,    AVK_6}, 
    {T9KEYAMBIG7,    AVK_7}, {T9KEYAMBIG8,    AVK_8}, {T9KEYAMBIG9,    AVK_9}, 
    {T9KEYAMBIGA,    AVK_0}, {T9KEYNONE,   AVK_STAR}, {T9KEYNONE,  AVK_POUND},
    {T9KEYNONE, AVK_SELECT}, {T9KEYCLEAR,   AVK_CLR}, {T9KEYNEXT,   AVK_DOWN}, 
    {T9KEYPREV,     AVK_UP}, {T9KEYLEFT,   AVK_LEFT}, {T9KEYRIGHT, AVK_RIGHT},  
    {T9KEYNONE,   AVK_SEND}, {T9KEYNONE,    AVK_END}, {0,0}
};

T9KeyMap ThaiAlphabetic2T9Map[] = 
{
    {T9KEYAMBIG1,    AVK_1}, {T9KEYAMBIG2,    AVK_2}, {T9KEYAMBIG3,    AVK_3},
    {T9KEYAMBIG4,    AVK_4}, {T9KEYAMBIG5,    AVK_5}, {T9KEYAMBIG6,    AVK_6}, 
    {T9KEYAMBIG7,    AVK_7}, {T9KEYAMBIG8,    AVK_8}, {T9KEYAMBIG9,    AVK_9}, 
    {T9KEYAMBIGA,    AVK_0}, {T9KEYAMBIGB,  AVK_STAR},{T9KEYAMBIGC, AVK_POUND},
    {T9KEYNONE, AVK_SELECT}, {T9KEYCLEAR,   AVK_CLR}, {T9KEYNEXT,   AVK_DOWN}, 
    {T9KEYPREV,     AVK_UP}, {T9KEYLEFT,   AVK_LEFT}, {T9KEYRIGHT, AVK_RIGHT},  
    {T9KEYNONE,   AVK_SEND}, {T9KEYNONE,    AVK_END}, {0,0}
};
#endif //#ifdef FEATURE_T9_ALPHABETIC
    
    
#ifdef FEATURE_T9_CHINESE
/* Translates Zhuyin keys to T9 key codes */
T9KeyMap Pinyin2T9Map[] = 
{
    {T9KEYAMBIG1,    AVK_1}, {T9KEYAMBIG2,    AVK_2}, {T9KEYAMBIG3,    AVK_3},
    {T9KEYAMBIG4,    AVK_4}, {T9KEYAMBIG5,    AVK_5}, {T9KEYAMBIG6,    AVK_6}, 
    {T9KEYAMBIG7,    AVK_7}, {T9KEYAMBIG8,    AVK_8}, {T9KEYAMBIG9,    AVK_9}, 
    {T9KEYSPACE,     AVK_0}, {T9KEYNONE,   AVK_STAR}, {T9KEYNONE,  AVK_POUND},
    {T9KEYNONE, AVK_SELECT}, {T9KEYCLEAR,   AVK_CLR}, {T9KEYNEXT,   AVK_DOWN}, 
    {T9KEYPREV,     AVK_UP}, {T9KEYLEFT,   AVK_LEFT}, {T9KEYRIGHT, AVK_RIGHT}, 
    {T9KEYNONE,   AVK_SEND}, {T9KEYNONE,    AVK_END}, {0,0}
};

T9KeyMap Zhuyin2T9Map[] = 
{
    {T9KEYAMBIG1,    AVK_1}, {T9KEYAMBIG2,    AVK_2}, {T9KEYAMBIG3,    AVK_3},
    {T9KEYAMBIG4,    AVK_4}, {T9KEYAMBIG5,    AVK_5}, {T9KEYAMBIG6,    AVK_6}, 
    {T9KEYAMBIG7,    AVK_7}, {T9KEYAMBIG8,    AVK_8}, {T9KEYAMBIG9,    AVK_9}, 
    {T9KEYSPACE,     AVK_0}, {T9KEYAMBIGA,  AVK_STAR},{T9KEYAMBIGC,AVK_POUND},
    {T9KEYNONE, AVK_SELECT}, {T9KEYCLEAR,   AVK_CLR}, {T9KEYNEXT,   AVK_DOWN}, 
    {T9KEYPREV,     AVK_UP}, {T9KEYLEFT,   AVK_LEFT}, {T9KEYRIGHT, AVK_RIGHT}, 
    {T9KEYNONE,   AVK_SEND}, {T9KEYNONE,    AVK_END}, {0,0}
};

T9KeyMap Stroke2T9Map[] = 
{
    {T9KEYAMBIG1,    AVK_1}, {T9KEYAMBIG2,    AVK_2}, {T9KEYAMBIG3,    AVK_3},
    {T9KEYAMBIG4,    AVK_4}, {T9KEYAMBIG5,    AVK_5}, {T9KEYAMBIG6,    AVK_6}, 
    {T9KEYNONE,      AVK_7}, {T9KEYNONE,      AVK_8}, {T9KEYNONE,      AVK_9}, 
    {T9KEYNONE,      AVK_0}, {T9KEYSHIFT,  AVK_STAR}, {T9KEYAMBIGC,AVK_POUND},
    {T9KEYNONE, AVK_SELECT}, {T9KEYCLEAR,   AVK_CLR}, {T9KEYNEXT,   AVK_DOWN}, 
    {T9KEYPREV,     AVK_UP}, {T9KEYLEFT,   AVK_LEFT}, {T9KEYRIGHT, AVK_RIGHT}, 
    {T9KEYNONE,   AVK_SEND}, {T9KEYNONE,    AVK_END}, {0,0}
};



T9CCAudbInfo         *gpSimpCAUdbInfo = NULL;
T9CCAudbInfo         *gpTradCAUdbInfo = NULL;
extern T9STATUS T9FARCALL T9CCLoadLdb(T9CCFieldInfo *pFieldInfo, T9ChineseData T9FARDATA *pT9CCLdbHeader);
#endif //FEATURE_T9_CHINESE
    
    
static ModeInfo sTextModes[NUM_OF_MODES] =
{ 
    {TextCtl_NumbersRestart,
      TextCtl_NumbersKey,
      NULL, /* Use default name for Numbers mode */
      TextCtl_NumbersExit,
      {TEXT_MODE_NUMBERS, {0}}}

#ifdef FEATURE_T9_MT_ENGLISH
    ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_MULTITAP, {0}}}
#endif //FEATURE_T9_MT_ENGLISH      

#ifdef FEATURE_T9_RAPID_ENGLISH
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_ENGLISH, {0}}}
#endif //FEATURE_T9_RAPID_ENGLISH

#ifdef FEATURE_T9_CAP_LOWER_ENGLISH   //add by yangdecai  2010-09-09
	,{T9TextCtl_Cap_Lower_Restart,
      T9TextCtl_Cap_Lower_Rapid_Key,
      NULL, 
      T9TextCtl_Cap_Lower_Rapid_Exit ,
      {TEXT_MODE_T9_CAP_LOWER_ENGLISH, {0}}}
#endif


#ifdef FEATURE_T9_PINYIN
   ,{T9TextCtl_CJK_CHINESE_Restart,
      T9TextCtl_CJK_CHINESE_Key,
      NULL, 
      T9TextCtl_CJK_CHINESE_Exit ,
      {TEXT_MODE_T9_PINYIN, {0}}}
#endif //FEATURE_T9_PINYIN

#ifdef FEATURE_T9_STROKE
   ,{T9TextCtl_CJK_CHINESE_Restart,
      T9TextCtl_CJK_CHINESE_Key,
      NULL, 
      T9TextCtl_CJK_CHINESE_Exit ,
      {TEXT_MODE_T9_STROKE, {0}}}
#endif //FEATURE_T9_STROKE

#ifdef FEATURE_T9_ZHUYIN
   ,{T9TextCtl_CJK_CHINESE_Restart,
      T9TextCtl_CJK_CHINESE_Key,
      NULL, 
      T9TextCtl_CJK_CHINESE_Exit ,
      {TEXT_MODE_T9_ZHUYIN, {0}}}
#endif //FEATURE_T9_ZHUYIN

#ifdef FEATURE_T9_MT_ARABIC
   ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_T9_MT_ARABIC, {0}}}
#endif //FEATURE_T9_MT_ARABIC

#ifdef FEATURE_T9_RAPID_ARABIC
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_ARABIC, {0}}}
#endif //FEATURE_T9_RAPID_ARABIC

#ifdef FEATURE_T9_MT_HEBREW
   ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_T9_MT_HEBREW, {0}}}
#endif //FEATURE_T9_MT_HEBREW

#ifdef FEATURE_T9_RAPID_HEBREW
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_HEBREW, {0}}}
#endif //FEATURE_T9_RAPID_HEBREW

#ifdef FEATURE_T9_MT_HINDI
   ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_T9_MT_HINDI, {0}}}
#endif //FEATURE_T9_MT_HINDI

#ifdef FEATURE_T9_RAPID_HINDI
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_HINDI, {0}}}
#endif //FEATURE_T9_RAPID_HINDI

#ifdef FEATURE_T9_MT_THAI
   ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_T9_MT_THAI, {0}}}
#endif //FEATURE_T9_MT_THAI

#ifdef FEATURE_T9_RAPID_THAI
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_THAI, {0}}}
#endif //FEATURE_T9_RAPID_THAI

#ifdef FEATURE_T9_MT_SPANISH
   ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_T9_MT_SPANISH, {0}}}
#endif //FEATURE_T9_MT_SPANISH

#ifdef FEATURE_T9_RAPID_SPANISH
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_SPANISH, {0}}}
#endif //FEATURE_T9_RAPID_SPANISH

#ifdef FEATURE_T9_MT_PORTUGUESE
   ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_T9_MT_PORTUGUESE, {0}}}
#endif //FEATURE_T9_MT_PORTUGUESE

#ifdef FEATURE_T9_RAPID_PORTUGUESE
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_PORTUGUESE, {0}}}
#endif //FEATURE_T9_RAPID_PORTUGUESE

#ifdef FEATURE_T9_MT_INDONESIAN
   ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_T9_MT_INDONESIAN, {0}}}
#endif //FEATURE_T9_MT_INDONESIAN

#ifdef FEATURE_T9_RAPID_INDONESIAN
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_INDONESIAN, {0}}}
#endif //FEATURE_T9_RAPID_INDONESIAN

#ifdef FEATURE_T9_MT_VIETNAMESE
   ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_T9_MT_VIETNAMESE, {0}}}
#endif //FEATURE_T9_MT_VIETNAMESE

#ifdef FEATURE_T9_RAPID_VIETNAMESE
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_VIETNAMESE, {0}}}
#endif //FEATURE_T9_RAPID_VIETNAMESE

#ifdef FEATURE_T9_MT_FRENCH
   ,{T9TextCtl_MultitapRestart,
      T9TextCtl_MultitapKey,
      NULL,
      T9TextCtl_MultitapExit,
      {TEXT_MODE_T9_MT_FRENCH, {0}}}
#endif //FEATURE_T9_MT_FRENCH

#ifdef FEATURE_T9_RAPID_FRENCH
   ,{T9TextCtl_Latin_Rapid_Restart,
      T9TextCtl_Latin_Rapid_Key,
      NULL, 
      T9TextCtl_Latin_Rapid_Exit ,
      {TEXT_MODE_T9_RAPID_FRENCH, {0}}}
#endif //FEATURE_T9_RAPID_FRENCH

};/*lint !e785 */
#endif //FEATURE_T9_INPUT

/*=============================================================================
FUNCTION: OEM_TextCreate

DESCRIPTION:

PARAMETERS:
   pIShell:
   pIDisplay:
   *pRect:

RETURN VALUE:
   OEMCONTEXT:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
OEMCONTEXT OEM_TextCreate(const IShell* pIShell,
                          const IDisplay* pIDisplay,
                          const AEERect *pRect,
                          AEECLSID cls)
{
   TextCtlContext *pNewContext;
   AEEDeviceInfo devInfo;
   short i,j;
   uint32 curLngMode ;

   // Validate input parameters
   if (!pIShell || !pIDisplay || /* !pITextCtl || */ !pRect) return NULL;
   //if (!pRect->dx || !pRect->dy) return NULL;

   // Get device info
   ISHELL_GetDeviceInfo((IShell *)pIShell, &devInfo);

   // Allocate our new context
   pNewContext = (TextCtlContext *) OEM_Malloc(sizeof(TextCtlContext));

   if (!pNewContext)
      return(NULL);

   MEMSET(pNewContext, 0, sizeof(*pNewContext));

   // Initialize our new context
   pNewContext->pIShell = (IShell *) pIShell;
   pNewContext->pIDisplay = (IDisplay *) pIDisplay;
   pNewContext->rectDisplay = *pRect;
#ifdef FEATURE_SCROLLBAR_USE_STYLE
   pNewContext->wScrollBarWidth = SCROLLBAR_WIDTH;
#else
   pNewContext->wScrollBarWidth = devInfo.cxScrollBar + 1;  // Add border pixel
#endif

   pNewContext->bEditable = 1;
   pNewContext->dwProperties = TP_FRAME | TP_MULTILINE;

   pNewContext->bMaxCharTimedOut = 0;
   pNewContext->clsMe = cls;
   pNewContext->bSwap = FALSE;

   // By default we have these settings until the API is enhanced
   pNewContext->font = AEE_FONT_NORMAL;
   pNewContext->nFontLeading = TEXT_BETWEEN_LINE_PIXEL; // 0 pixels of leading between rows //modified by chengxiao 2009.03.23
   curLngMode = LNG_ENGLISH;//OEM_Lang();  
   pNewContext->m_pImageBg = NULL;

   {
      for (i=0;i<NUM_OF_MODES;i++) {
         for (j = 0; ; j++) {
            sTextModes[i].info.pszTitle[j] = englishTitle[i][j];
            if (0 == sTextModes[i].info.pszTitle[j]) {
               break;
            }
         }
      }
      // Default mode is Multitap
      pNewContext->byMode = 0;
   }
   pNewContext->is_isShift = FALSE;
   pNewContext->nMultitapCaps = MULTITAP_FIRST_CAP;
   pNewContext->m_bCaplk = FALSE;

   pNewContext->nLineHeight =
                     IDISPLAY_GetFontMetrics((IDisplay*)pNewContext->pIDisplay,
                                              AEE_FONT_NORMAL,
                                              NULL,
                                              NULL);

   // KLUDGE
   //
   // A multiline,framed text control requires a display area of at least
   // "nLineHeight + 4" pixels on the y plane.
   //
   // When the ITextCtl creates a new OEM Text Control, it adjusts the OEM
   // display area as follows:
   //    - subtract nLineHeight for a title
   //    - subtract nLineHeight if it is multiline (?!)
   //
   // This means that if the ITextCtl actually has a rectangle of
   // 3 * nLineHeight y pixels assigned to it, the OEM Text Control will
   // only get nLineHeight pixels even though there is enough physical
   // room for 2 * nLineHeight pixels!!!
   //
   // This workaround will ensure there is always enough room to display
   // a single line regardless of the rectangle that the ITextCtl gives us.
   //
   if (pNewContext->rectDisplay.dy < pNewContext->nLineHeight + 4) {
      SETAEERECT(&pNewContext->rectDisplay,
                  pNewContext->rectDisplay.x,
                  pNewContext->rectDisplay.y,
                  pNewContext->rectDisplay.dx,
                  pNewContext->nLineHeight + 4);
   }
#ifdef FEATURE_T9_CHINESE
   pNewContext->rectChineseInput.x = pNewContext->rectDisplay.x;
   pNewContext->rectChineseInput.dx = 0;
   pNewContext->rectChineseInput.y = pNewContext->rectDisplay.y + pNewContext->rectDisplay.dy;
   pNewContext->rectChineseInput.dy = 0;
#endif //#ifdef FEATURE_T9_CHINESE

#ifdef FEATURE_FUNCS_THEME
    {
        // ��ȡ�������
        Theme_Param_type    Theme_Param;
        Appscom_GetThemeParameters(&Theme_Param);
        pNewContext->m_themeColor = Theme_Param.themeColor;        
    }
#endif /* FEATURE_FUNCS_THEME */   


   // Perform any housekeeping
   TextCtl_TextChanged(pNewContext);

   // Restart the edit if editable
   TextCtl_RestartEdit(pNewContext);

// bw:20080925
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
    if ( SUCCESS != ISHELL_CreateInstance(pNewContext->pIShell,
                                          AEECLSID_FONTSYSNORMAL,//AEECLSID_FONT,
                                          (void **)&pNewContext->m_pMyFont))
    {
        pNewContext->m_pMyFont = NULL;
        return NULL;
    }  

#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
   // Return the newly created context
   return pNewContext;
}

/*=============================================================================
FUNCTION: OEM_TextDelete

DESCRIPTION:

PARAMETERS:
   hContext:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextDelete(OEMCONTEXT hContext)
{
   register TextCtlContext *pTextCtlContext = (TextCtlContext *) hContext;

   if (pTextCtlContext) {
      // Cleanup and release our context

#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
    (void) ISHELL_CancelTimer((IShell *) (pTextCtlContext->pIShell),  
                                            (PFNNOTIFY)TextCtl_DrawCursorTimer, 
                                            pTextCtlContext);
#endif                                            
      // Exit the edit now so that any outstanding timers, or db
      // storage can be processed.
      if (pTextCtlContext->bEditable
          && sTextModes[pTextCtlContext->byMode].pfn_exit) {
         // Exit the current text mode before we go bye-bye!
         (*sTextModes[pTextCtlContext->byMode].pfn_exit)(pTextCtlContext);
      }

      // Free storage

      if (pTextCtlContext->pszContents) {
         sys_free(pTextCtlContext->pszContents);
      }

      if (pTextCtlContext->pwLineStarts) {
         sys_free(pTextCtlContext->pwLineStarts);
      }  
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
      if( pTextCtlContext->m_pMyFont )
      {
         IFONT_Release(pTextCtlContext->m_pMyFont);
         pTextCtlContext->m_pMyFont = NULL;
      }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
      sys_free(pTextCtlContext);
   }
}

/*=============================================================================
FUNCTION: OEM_TextEnumModesInit

DESCRIPTION:

PARAMETERS:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextEnumModesInit()
{
   snTextModeIndex = 0;
}

/*=============================================================================
FUNCTION: OEM_TextEnumMode

DESCRIPTION:

PARAMETERS:
   *pMode:

RETURN VALUE:
   boolean:  Return true if this is a valid mode.
             Return false if we already reached end

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
boolean OEM_TextEnumMode(AEETextMode *pMode)
{
   int sTextModesSize;

   if (!pMode) return FALSE;
   
   sTextModesSize = ARRAY_SIZE(sTextModes);
   
   if (snTextModeIndex < sTextModesSize) {
      *pMode = sTextModes[snTextModeIndex++].info;
      return TRUE;
   } else {
      return FALSE;
   }
}

/*=============================================================================
FUNCTION: OEM_TextGetCurrentMode

DESCRIPTION:

PARAMETERS:
   hTextCtl:

RETURN VALUE:
   AEETextInputMode:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
AEETextInputMode OEM_TextGetCurrentMode(OEMCONTEXT hTextCtl)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   return sTextModes[pContext?pContext->byMode:0].info.wID;
}

/*=============================================================================
FUNCTION: OEM_TextGetModeString

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   *szBuf:
   nLen:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextGetModeString(OEMCONTEXT hTextCtl, AECHAR *szBuf, uint16 nLen)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   nLen >>= 1; // Convert to AECHAR len
   if (pContext && szBuf && nLen) {
      const AECHAR *pszName = NULL;

      --nLen;  // Reserve room for NUL terminator

      {

         // If we're in edit mode, use the appropriate mode function to
         // get the string name of this mode
         if (pContext->bEditable && sTextModes[pContext->byMode].pfn_string)
            pszName = (*sTextModes[pContext->byMode].pfn_string)(pContext);

         // If we're not in edit mode or the mode function is NULL or
         // it returned the NULL string, use the default name
         if (!pszName)
            pszName = sTextModes[pContext->byMode].info.pszTitle;
      }

      // Now copy as many characters as fits
      for (; nLen && *pszName; --nLen, ++pszName) {
         *szBuf++ = *pszName;
      }

      // Be sure it's nul terminated!
      *szBuf = 0;
      return;
   }
}

/*=============================================================================
FUNCTION: OEM_TextSetEdit

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   bIsEditable:
   wmode:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextSetEdit(OEMCONTEXT hTextCtl,
                     boolean bIsEditable,
                     AEETextInputMode wmode)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   if (pContext) {
      unsigned char byModeIndex = FindModeIndex(pContext, wmode);
      boolean bRedraw=FALSE, bRestartEdit=FALSE, bExitEdit=FALSE;

      if (pContext->bEditable != bIsEditable) {
         // We must redraw here after changing mode
         bRedraw=TRUE;
         bRestartEdit=bIsEditable;// Must restart new mode if becoming editable
         bExitEdit=!bIsEditable; // Must exit mode if no longer editable
      }
      if (byModeIndex != pContext->byMode) {
         if (bIsEditable) {
            bRestartEdit=TRUE;// Must restart new mode
            bExitEdit=pContext->bEditable; // Must exit old mode if was editable
         }
      }
      // May need to redraw and/or exit/restart edit

      if (bExitEdit && sTextModes[pContext->byMode].pfn_exit) {
         // Exit the old mode before changing the mode number
         // or the edit flag
         (*sTextModes[pContext->byMode].pfn_exit)(pContext);
      }
      // Update the mode and editable flags
      pContext->byMode = byModeIndex;
      pContext->bEditable = bIsEditable;

      if (bRestartEdit) {
         TextCtl_RestartEdit(pContext);
         (void) TextCtl_AutoScroll(pContext);
      }

      if (bRedraw) {
         pContext->bNeedsDraw = TRUE;
      }
   }
}

/*=============================================================================
FUNCTION: OEM_TextUpdate

DESCRIPTION:

PARAMETERS:
   hTextCtl:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextUpdate(OEMCONTEXT hTextCtl)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;
   if (pContext && pContext->bNeedsDraw) {
      OEM_TextDraw(pContext);
   }
}

/*=============================================================================
FUNCTION: OEM_TextSet

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   *pszText:
   ignored:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextSet(OEMCONTEXT hTextCtl, AECHAR *pszText, uint16 ignored)
{
   //PARAM_NOT_REF(ignored)

   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;
   uint32                   nLen;
   AECHAR                  *pszNewText;

   if (pContext) {
      nLen = pszText ? WSTRLEN(pszText) : 0;

      // Restrict length to number of characters we want
      // nChars < 0 specifies string length
      if (ignored > 0 && ignored < nLen)
         nLen = ignored;

      // Make room for NULL
      if (pszText ||!nLen)
         nLen++;
      pszNewText = (AECHAR *)
                   OEM_Realloc(pContext->pszContents, nLen * sizeof(AECHAR));
      if (!pszNewText && nLen) {
         // The buffer allocation failed, do nothing?
         return;
      }
      // Update the text with the new contents
      pContext->pszContents = pszNewText;

      if (nLen)
      {
         pContext->wContentsChars = nLen - 1;
         MEMCPY(pContext->pszContents, pszText, pContext->wContentsChars * sizeof(AECHAR));
         pContext->pszContents[pContext->wContentsChars] = 0;
      }
      else
         pContext->wContentsChars = 0;

      // Set the cursor position to the rightmost position.
      // TextCtl_TextChanged() will adjust these values to be in range
      // for the current text string
      pContext->wSelStart = 0xFFFF;
      pContext->wSelEnd = 0xFFFF;
      
      // Text changed, so update selection, etc. if necessary
      TextCtl_TextChanged(pContext);

      // If we're editable, we must now restart the edit to accomodate
      // the changed text
      TextCtl_RestartEdit(pContext);

      // Autoscroll if necessary
      (void) TextCtl_AutoScroll(pContext);
   }
}

/*=============================================================================
FUNCTION: OEM_TextGet

DESCRIPTION:

PARAMETERS:
   hTextCtl:

RETURN VALUE:
   AECHAR *:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
AECHAR* OEM_TextGet(OEMCONTEXT hTextCtl)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;
#ifdef FEATURE_TOUCHPAD
   if(pContext)
   {
	   //Add the Get Text when TSIM
	   if(pContext->pIDialog)
	   {
		   ITSIM   *pITSIM;

		   pITSIM = (ITSIM*)IDIALOG_GetControl(pContext->pIDialog,
			   OEM_IME_DIALOG_CTL_ITSIM);

		   if(pITSIM)
		   {
			   return TSIM_TextGet(pITSIM);
		   }
	   }
   }
#endif
   return pContext ? pContext->pszContents : NULL;
}

/*=============================================================================
FUNCTION: OEM_TextGetProperties

DESCRIPTION:

PARAMETERS:
   hTextCtl:

RETURN VALUE:
   uint32:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
uint32 OEM_TextGetProperties(OEMCONTEXT hTextCtl)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   return(pContext ? pContext->dwProperties : 0);
}

/*=============================================================================
FUNCTION: OEM_TextSetProperties

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   dwProperties:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextSetProperties(OEMCONTEXT hTextCtl, uint32 dwProperties)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   if (pContext && dwProperties != pContext->dwProperties) {
      pContext->dwProperties = dwProperties;

      if (dwProperties & TP_LARGE_FONT) {
         pContext->font = AEE_FONT_LARGE;
      } else {
         pContext->font = AEE_FONT_NORMAL;
      }


      // Text calculations changed, so update selection, etc. if necessary
      TextCtl_TextChanged(pContext);
   }
}

/*=============================================================================
FUNCTION: OEM_TextGetMaxChars

DESCRIPTION:

PARAMETERS:
   hTextCtl:

RETURN VALUE:
   uint16:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
uint16 OEM_TextGetMaxChars(OEMCONTEXT hTextCtl)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   return pContext ? pContext->wMaxChars : 0;
}

/*=============================================================================
FUNCTION: OEM_TextSetMaxChars

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   wMaxChars:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextSetMaxChars(OEMCONTEXT hTextCtl, uint16 wMaxChars)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;
   if(wMaxChars > MAX_INPUTTEXT_SIZE)
   {
       wMaxChars = MAX_INPUTTEXT_SIZE;
   }
   if (pContext && wMaxChars != pContext->wMaxChars) {
      if (!wMaxChars || wMaxChars >= pContext->wContentsChars) {
         // Just pick up the new value, no changes necessary
         pContext->wMaxChars = wMaxChars;
      } else {
         // Must truncate the text to wMaxChars
         unsigned short selStart = pContext->wSelStart;
         unsigned short selEnd = pContext->wSelEnd;

         // Set selection to stuff to be truncated
         OEM_TextSetSel(pContext, wMaxChars, (unsigned short) 65535U);

         // Replace it with NUL (effectively deleting it)
         TextCtl_AddChar(pContext, 0);

         // Pick up the new max value
         pContext->wMaxChars = wMaxChars;

         // Restore the selection to as close as possible
         OEM_TextSetSel(pContext, selStart, selEnd);

         // Restart the edit
         TextCtl_RestartEdit(pContext);
      }
   }
}

/*=============================================================================
FUNCTION: OEM_TextGetRect

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   *pOutRect:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextGetRect(OEMCONTEXT hTextCtl, AEERect *pOutRect)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   if (pContext && pOutRect) {
      *pOutRect = pContext->rectDisplay;
   }
}

/*=============================================================================
FUNCTION: OEM_TextSetRect

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   *pInRect:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextSetRect(OEMCONTEXT hTextCtl, const AEERect *pInRect)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   if (pContext && pInRect) {
      if (pInRect->x != pContext->rectDisplay.x
          || pInRect->y != pContext->rectDisplay.y
          || pInRect->dx != pContext->rectDisplay.dx
          || pInRect->dy != pContext->rectDisplay.dy) {
         // Size is changing
         pContext->rectDisplay = *pInRect;

         TextCtl_TextChanged(pContext);
      }
   }
}

/*=============================================================================
FUNCTION: OEM_TextSetSel

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   selStart:
   selEnd:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextSetSel(OEMCONTEXT hTextCtl, int selStart, int selEnd)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   if (pContext) {
      if (TextCtl_SetSel(pContext, selStart, selEnd)) {
         (void) TextCtl_AutoScroll(pContext);
         pContext->bNeedsDraw = TRUE;
      }
   }
}

/*=============================================================================
FUNCTION: OEM_TextGetSel

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   *pSelStart:
   *pSelEnd:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextGetSel(OEMCONTEXT hTextCtl, int *pSelStart, int *pSelEnd)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

   if (pContext) {
      if (pSelStart)
         *pSelStart = (pContext->bSwap ? pContext->wSelEnd : pContext->wSelStart);
      if (pSelEnd)
         *pSelEnd = (pContext->bSwap ? pContext->wSelStart : pContext->wSelEnd);
   }
   else {
      if (pSelStart)
         *pSelStart = 0;
      if (pSelEnd)
         *pSelEnd = 0;
   }
}

/*=============================================================================
FUNCTION: OEM_TextKeyPress

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   eCode:
   dwKeyCode:
   dwKeySyms:

RETURN VALUE:
   boolean:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
boolean OEM_TextKeyPress(OEMCONTEXT hTextCtl,
                         AEEEvent eCode,
                         uint32 dwKeyCode,
                         uint32 dwKeySyms)
{
    //PARAM_NOT_REF(dwKeySyms)
    
    register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;
    AVKType key = (AVKType) dwKeyCode;
    
    // Press and hold the number key to get the number
	#if 0
    if ((eCode != EVT_KEY) 
        /*&&(!(key == AVK_STAR) && (eCode == EVT_KEY_RELEASE))
        &&(!(key == AVK_POUND) && (eCode == EVT_KEY_RELEASE))*/ //modi by yangdecai 2010-08-07
        &&(!((key == AVK_CLR) && (eCode == EVT_KEY_HELD))))
    {
        return FALSE; // We only want key events or CLR held events
    }
	#else
	if ((eCode == EVT_KEY_RELEASE)|| 
        /*&&(!(key == AVK_STAR) && (eCode == EVT_KEY_RELEASE))
        &&(!(key == AVK_POUND) && (eCode == EVT_KEY_RELEASE))*/ //modi by yangdecai 2010-08-07
         (eCode == EVT_KEY_PRESS))
    {
        return FALSE; // We only want key events or CLR held events
    }
	#endif
    if (pContext) 
    {
        // Ignore Up/Down if we're not multiline
        if ((key == AVK_UP || key == AVK_DOWN)
            && ((pContext->dwProperties & TP_PASSWORD)||(pContext->dwProperties & TP_USELESS_UPDOWN)) // kai.yao add TP_USELESS_UPDOWN
            && !(pContext->dwProperties & TP_MULTILINE)) 
        {
            return FALSE;
        }
    
        if (pContext->bEditable) 
        {
            switch (key) 
            {
                case AVK_UP:
                    if (pContext->sFocus == FOCUS_TEXT || 
                        sTextModes[pContext->byMode].info.wID == TEXT_MODE_NUMBERS)
                    {
                        uint16 nLine, nSel, wSelOld;
                        boolean bSel = (boolean)(pContext->wSelEnd == pContext->wSelStart ? FALSE : TRUE);
                        wSelOld = (uint16)(pContext->bSwap ? pContext->wSelEnd : pContext->wSelStart);
                        nLine = TextCtl_GetLine(pContext, wSelOld);
                            
                        if (!((nLine == 0 || !pContext->pwLineStarts) && !bSel))
                        {
                            if(bSel)
                            {
                                nSel = wSelOld;
                            
                            OEM_TextSetSel(pContext, nSel,nSel);
                            TextCtl_AutoScroll(pContext);
                            OEM_TextUpdate(pContext);
                            
                            return TRUE;
                            }
                        }
                    }
                    goto OEM_TextKeyPress_COMM;
                    
                case AVK_DOWN:
                    if (pContext->sFocus == FOCUS_TEXT || 
                        sTextModes[pContext->byMode].info.wID == TEXT_MODE_NUMBERS)
                    {
                        uint16 nSel, wSelOld;
                        //uint32   nLen = (pContext->pszContents ? WSTRLEN(pContext->pszContents) : 0);
                        boolean bSel = (boolean)(pContext->wSelEnd == pContext->wSelStart ? FALSE : TRUE);
                        
                        wSelOld = (uint16)(pContext->bSwap ? pContext->wSelEnd : pContext->wSelStart);
                            

                        if (bSel)
                        {
                            nSel = wSelOld;
                            OEM_TextSetSel(pContext, nSel,nSel);
                            TextCtl_AutoScroll(pContext);
                            OEM_TextUpdate(pContext);
                          
                            return TRUE;
                        }
                    }
                    goto OEM_TextKeyPress_COMM;
                        #if 0
                        }
                        else
                        {
                            if (!pContext->pwLineStarts)
                            {
                                goto OEM_TextKeyPress_COMM;
                            }
                            nLine = TextCtl_GetLine(pContext, wSelOld);

                            if ((uint32)OEM_TextGetCursorPos(pContext) == nLen)
                            {
                                goto OEM_TextKeyPress_COMM;
                            }
                            // If the cursor is on the last line and the line's last character is not
                            // a LF, then FALSE is returned as nothing can be done. A LF on the end of a line
                            // Does not tell the wLines member that there is another line, hence this extra check.
                            if(nLine == (pContext->wLines-1) && pContext->pszContents[nLen-1] != (AECHAR)'\n')
                            {
                                goto OEM_TextKeyPress_COMM;
                            }
                            // See how many characters into the current line the cursor is
                            nCharsIn = wSelOld - pContext->pwLineStarts[nLine];
                            // If the cursor is more characters in than the next line...
                            // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
                            if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
                            {
                                // If it is the last line, don't subtract the LINEBREAK from selection spot
                                if( nLine+2 == pContext->wLines )
                                {
                                    nSel = pContext->pwLineStarts[nLine+2];
                                }
                                else
                                {
                                    nSel = pContext->pwLineStarts[nLine+2]-1;
                                }
                            }
                            else
                            {
                                // Selection spot is number of chars into the next line
                                nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
                                // If this is not the beginning of a line 
                                // and the selection point is a LINEBREAK, subtract one
                                // Otherwise the selection overshoots to the first character
                                // of the following line.
                                if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
                                {
                                    nSel--;
                                }
                            }
                        }
                        OEM_TextSetSel(pContext, nSel,nSel);
                        TextCtl_AutoScroll(pContext);
                        OEM_TextUpdate(pContext);
                        return TRUE;
                    }
                    goto OEM_TextKeyPress_COMM;
                   #endif  
                    
                case AVK_LEFT:
                    if (pContext->sFocus == FOCUS_TEXT || 
                        sTextModes[pContext->byMode].info.wID == TEXT_MODE_NUMBERS)
                    {
                        boolean bSel = (boolean)(pContext->wSelEnd == pContext->wSelStart ? FALSE : TRUE);
                        uint16 wNewSel = (pContext->bSwap ? pContext->wSelEnd : pContext->wSelStart);
                        if (wNewSel && bSel)
                        {
                            --wNewSel;
                            OEM_TextSetSel(pContext, wNewSel,wNewSel);
                            TextCtl_AutoScroll(pContext);
                            OEM_TextUpdate(pContext);
                            
                            return TRUE;
                        }
                    }
                    goto OEM_TextKeyPress_COMM;
                
                case AVK_RIGHT:
                    if (pContext->sFocus == FOCUS_TEXT || 
                        sTextModes[pContext->byMode].info.wID == TEXT_MODE_NUMBERS)
                    {
                        boolean bSel = (boolean)(pContext->wSelEnd == pContext->wSelStart ? FALSE : TRUE);
                        uint16 wNewSel = ( pContext->bSwap ? pContext->wSelEnd : pContext->wSelStart);
                        if (bSel)
                        {
                            wNewSel++; // Checks bounds below.
                            OEM_TextSetSel(pContext, wNewSel, wNewSel);
                            TextCtl_AutoScroll(pContext);
                            OEM_TextUpdate(pContext);
                            
                            return TRUE;
                        }
                    }
                    goto OEM_TextKeyPress_COMM;
                
                case AVK_CLR:
                    if (eCode == EVT_KEY_HELD) 
                    {
                        // Erase the whole buffer
                        (void) TextCtl_SetSel(pContext, 0, (uint16) 65535U);
                        OEM_TextAddChar(pContext, 0, FALSE);
                        (void) TextCtl_AutoScroll(pContext);
                        OEM_TextUpdate(pContext);
                        break;
                    }
                    
OEM_TextKeyPress_COMM:
                /* fall through */
                default:
                    if (sTextModes[pContext->byMode].pfn_char) 
                    {
                    	
                        boolean ans = (*sTextModes[pContext->byMode].pfn_char)(pContext,eCode, key);
                        if (ans)
                        {
                            OEM_TextUpdate(pContext);
                        }
                        return(ans);
                    }
                    return(FALSE);
            }
        } 
        else 
        {
            // Up/Down keys move the scroll position and other keys are ignored
            // Don't process anything unless we're multiline
            if (key != AVK_UP && key != AVK_DOWN) 
            {
                return FALSE;
            }
        
            // Do nothing if there's no scroll bar
            if (!pContext->bValid || pContext->wLines <= pContext->nDisplayLines)
            {
                return TRUE;   // Nothing to scroll if they all fit!
            }
        
            if (key == AVK_UP) 
            {
                /* The window we're looking through is moving up,
                * which means we start displaying at a lower line
                * value so decrement wDisplayStartLine if it's > 0
                */
                if (pContext->wDisplayStartLine) 
                {
                    --pContext->wDisplayStartLine;
                    OEM_TextDraw(pContext);
                    // Move the cursor now?
                }
            } 
            else 
            {
                /* The window we're looking through is moving down,
                * which means we start displaying at a greater line
                * value, so increment wDisplayStartLine and then
                * pin it to maximum
                */
                int wNewScroll = pContext->wDisplayStartLine + 1;
                if (wNewScroll + pContext->nDisplayLines > pContext->wLines) 
                {
                    /* Pin the value so we don't scroll farther than we need to */
                    wNewScroll = pContext->wLines - pContext->nDisplayLines;
                }
                if (wNewScroll != pContext->wDisplayStartLine) 
                {
                    pContext->wDisplayStartLine = (uint16) wNewScroll;
                    OEM_TextDraw(pContext);
                    // Move the cursor now?
                }
            }
        }
    }
    
    return TRUE;   // handled always TRUE for now
}

/*=============================================================================
FUNCTION: OEM_TextAddChar

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   ch:
   bOverStrike:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextAddChar(OEMCONTEXT hTextCtl, AECHAR ch, boolean bOverStrike)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;
#ifdef AEE_DEBUG
   MSG_MED("OEM_TextAddChar 0x%04lX", (unsigned long) ch, 0, 0);
#endif

   if (pContext) {
      if (bOverStrike && pContext->wSelStart == pContext->wSelEnd
          && pContext->wSelStart < pContext->wContentsChars) {
         // Overstrike is only meaningful if we have an insertion point
         // rather than a selection AND the insertion point is not
         // at the very end of the text.

         // Set the selection to the next character so it will
         // be replaced.
         pContext->wSelEnd = pContext->wSelStart + 1;      
      }

      // Add the new character
      TextCtl_AddChar(pContext, ch);

      // Restart the edit if applicable
      TextCtl_RestartEdit(pContext);

      // Autoscroll
      (void) TextCtl_AutoScroll(pContext);
   }
}
/*=============================================================================
FUNCTION: OEM_TextAddString

DESCRIPTION:

PARAMETERS:
   hTextCtl:
   string:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextAddString(OEMCONTEXT hTextCtl, AECHAR* string)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;
   
   if (pContext) {
      // Add the new character
      TextCtl_AddString(pContext, string);
      
      // Restart the edit if applicable
      TextCtl_RestartEdit(pContext);
      
      // Autoscroll
      (void)TextCtl_AutoScroll(pContext);
   }
}
/*==========================================================================

Function: OEM_TextDraw()

Description:
   This function draws the text associated with a given text control object
   on the screen. This function also draws the associated items such as
   Scroll Bar, Border, Cursor etc. if  necessary and if supported.

Prototype:
   void OEM_TextDraw(OEMCONTEXT hTextField)

Parameters:
   htextField:    OEMCONTEXT   Handle for the text control object

Return Value:
   None.

Comments:
   None.

Side Effects:
   If the TP_DISPLAY_COUNT bit is set in the properities field then this
   function will display a charcater remain count on the top line.
   This was done because ITEXTCTL_SetTitle does not work correctly.
   The ITEXTCTL_SetTitle deletes the text of the main buffer and
   set it to non editable.
   This is only used by the SMS.c and only from MO message's text entry.

See Also:
   None

==========================================================================*/
void OEM_TextDraw(OEMCONTEXT hTextCtl)
{
    register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;
    boolean bScroll = FALSE;
    boolean bFrame = FALSE;
    RGBVAL nOldFontColor;

#ifdef FEATURE_FUNCS_THEME 
    RGBVAL dwOldBkClr;
#endif

    uint32 dwFlags = (IDF_ALIGN_LEFT|IDF_ALIGN_MIDDLE|IDF_TEXT_TRANSPARENT);
    if (!pContext)
    {
        return;
    }
    if (pContext->bValid)
    {
        if (pContext->dwProperties & TP_FRAME) 
        {
            AEERect rect = pContext->rectDisplay;

            // See side effects above!
            if (pContext->dwProperties & TP_DISPLAY_COUNT) 
            {
                // calculate the count and display it
                AECHAR          szFormat[16];
                AECHAR          szRemainingCount[16];
                uint16          szsmscount;
                uint16          wRemainingChars = pContext->wMaxChars;
                wRemainingChars = (uint16)(wRemainingChars - WSTRLEN(pContext->pszContents));
#if defined(FEATURE_WMS_APP)
                if(pContext->dwProperties & TP_DISPLAY_SMSCOUNT)
                {
                    AECHAR   * pszText;
                    pszText = OEM_TextGet(pContext);
                    szsmscount = 1;
                    szsmscount = WMSUtil_CalculateMessagesCount(pszText, NULL, NULL);
                    // Draw the number of remaining characters.
                    STR_TO_WSTR("%d/%d   ", szFormat, sizeof(szFormat));
                    WSPRINTF(szRemainingCount, sizeof(szRemainingCount), szFormat, wRemainingChars, szsmscount);                    

                    /*
                    WSPRINTF(szRemainingCount, 
                                    (WSTRLEN(szFormat) + wRemainingChars + szsmscount) * sizeof(AECHAR),
                                    szFormat, 
                                    wRemainingChars,
                                    szsmscount);
                    */
                }
                else
#endif
                {
                    // Draw the number of remaining characters.
                    STR_TO_WSTR("%d        ", szFormat, sizeof(szFormat));
                    WSPRINTF(szRemainingCount, sizeof(szRemainingCount), szFormat, wRemainingChars);                    

                    /*
                    WSPRINTF(szRemainingCount, 
                                    (WSTRLEN(szFormat) + wRemainingChars) * sizeof(AECHAR),
                                    szFormat, 
                                    wRemainingChars);                    
                    */
                }
#ifdef FEATURE_FUNCS_THEME 
                dwOldBkClr = IDISPLAY_SetColor((IDisplay *)pContext->pIDisplay,CLR_USER_BACKGROUND,RGB_BLACK);//MAKE_RGB(255,255,255)

                {
                    AEERect  pRect;
                    
                    SETAEERECT(&pRect,
                                rect.x,
                                rect.y-pContext->nLineHeight,
                                60,
                                pContext->nLineHeight);
                    IDISPLAY_EraseRect(pContext->pIDisplay, &pRect);
                      
                    IDISPLAY_FillRect(pContext->pIDisplay, &pRect,  pContext->m_themeColor);

  
                    (void)IDISPLAY_DrawText((IDisplay *)pContext->pIDisplay,
                            AEE_FONT_NORMAL,
                            szRemainingCount, -1, rect.x+2, rect.y-pContext->nLineHeight,
                            &pRect,
                            dwFlags);

                    (void) IDISPLAY_SetColor((IDisplay *)pContext->pIDisplay,CLR_USER_BACKGROUND,dwOldBkClr);
                }
#else
#ifndef WIN32
                {
                    AEERect rc;
                    AEERect oldrc;
                    TitleBar_Param_type TitleBar;
                    static int m_nPixels = 0;
                    int  nBarH = GetTitleBarHeight((IDisplay *)pContext->pIDisplay);
                    int nPixels = IDISPLAY_MeasureText(pContext->pIDisplay,pContext->font,szRemainingCount);
                    IDISPLAY_GetClipRect(pContext->pIDisplay, &oldrc);
                    if ( nPixels > m_nPixels)
                    {
						m_nPixels = nPixels;
                    }
                    
                    if(nPixels != 0)
                    {
                        SETAEERECT(&rc,
                                    rect.x,
                                    rect.y-nBarH,
                                    nPixels,//��128*128��С���Ϲ̶���Ȳ��У���Ϊ��Ȳ����������ַ��Ƕ����ֻ�ܸ����
                                    nBarH);  
                    }
                    else
                    {
                        SETAEERECT(&rc,
                                    rect.x,
                                    rect.y-nBarH,
                                    50,
                                    nBarH);  
                    }

                    IDISPLAY_SetClipRect(pContext->pIDisplay, &rc);
                    MEMSET(&TitleBar, 0, sizeof(TitleBar_Param_type));
                    TitleBar.prc = &rc;
                    DrawTitleBar(pContext->pIDisplay, &TitleBar);
                    nOldFontColor = IDISPLAY_SetColor(pContext->pIDisplay, CLR_USER_TEXT, RGB_WHITE);
                    
                    (void)IDISPLAY_DrawText(pContext->pIDisplay,
                            AEE_FONT_NORMAL,
                            szRemainingCount, -1, 0, 0,
                            &rc,
                            dwFlags);
                    IDISPLAY_SetClipRect(pContext->pIDisplay, &oldrc);
                    (void)IDISPLAY_SetColor(pContext->pIDisplay, CLR_USER_TEXT, 
                                                        nOldFontColor/*RGB_BLACK*/);//��ԭ�ı���ɫ
                }
#endif
#endif //FEATURE_FUNCS_THEME

            }
            bFrame = TRUE;
        }
        if (pContext->wLines > pContext->nDisplayLines && (pContext->dwProperties & TP_MULTILINE)) 
        {
            // Draw the scroll bar
            TextCtl_DrawScrollBar(pContext);
            bScroll = TRUE;
        }
        if(pContext->dwProperties & TP_GRAPHIC_BG)
        {
            nOldFontColor = TEXT_GRAPHIC_FONT_COLOR;
        }
        else
        {
            nOldFontColor = TEXT_FONT_COLOR;
        }
        nOldFontColor = IDISPLAY_SetColor(pContext->pIDisplay, CLR_USER_TEXT, nOldFontColor); 
        TextCtl_DrawTextPart(pContext, bScroll, bFrame);
        (void)IDISPLAY_SetColor(pContext->pIDisplay, CLR_USER_TEXT, nOldFontColor);
    } 
    else 
    {
        // Not valid, just draw all white
        if(pContext->dwProperties & TP_GRAPHIC_BG)
        {
            TextCtl_DrawBackGround(pContext, &pContext->rectDisplay);
        }
        else
        {
            IDISPLAY_FillRect(pContext->pIDisplay,
                    &pContext->rectDisplay, RGB_WHITE);
        }
    }
    pContext->bNeedsDraw = FALSE;
}


/*=============================================================================
FUNCTION: OEM_TextGetCursorPos

DESCRIPTION:

PARAMETERS:
   hTextField:

RETURN VALUE:
   int32:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int32 OEM_TextGetCursorPos(OEMCONTEXT hTextField)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextField;

   //return pContext->wSelStart;
   return (pContext->bSwap ? pContext->wSelEnd : pContext->wSelStart);
}

/*=============================================================================
FUNCTION: OEM_TextSetCursorPos

DESCRIPTION:

PARAMETERS:
   hTextField:
   nOffset:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
void OEM_TextSetCursorPos(OEMCONTEXT hTextField, int32 nOffset)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextField;
   uint16   nSel;

   if( nOffset < 0 )
      nSel  = 0;
   else
      nSel  = (uint16) nOffset;
   OEM_TextSetSel(hTextField, nSel, nSel);
#ifdef FEATURE_T9_INPUT  
#ifdef FEATURE_T9_ALPHABETIC
   if(&pContext->sT9awFieldInfo.G != NULL)
   {
      T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, pContext->wSelEnd );  
   }
#endif //FEATURE_T9_ALPHABETIC 

#ifdef FEATURE_T9_CHINESE
   if(&pContext->sT9ccFieldInfo.G != NULL)
   {
      T9Cursor ( &pContext->sT9ccFieldInfo.G, T9CA_FROMBEG, pContext->wSelEnd );   
   }
#endif //FEATURE_T9_CHINESE  
#endif //FEATURE_T9_INPUT   
   (void) TextCtl_AutoScroll(pContext);
   OEM_TextUpdate(pContext);
}

// add the code for when the cursor in begin, the button should be changed
/*=============================================================================
FUNCTION: OEM_TextGetT9End

DESCRIPTION:

PARAMETERS:
   hTextField:

RETURN VALUE:
   int32:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int32 OEM_TextGetT9End(OEMCONTEXT hTextField)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextField;

   return pContext->wSelEnd;
}
// add end for cursor and button
int OEM_TextGetFocusState(OEMCONTEXT hTextField)
{
   register TextCtlContext *pContext = (TextCtlContext *) hTextField;

   return pContext ? pContext->sFocus: NULL;
}

/*get the key buffer length in the chinese input method*/
int OEM_TextGetKeyBufLen(OEMCONTEXT hTextField)
{
#ifdef FEATURE_T9_CHINESE
   register TextCtlContext *pContext = (TextCtlContext *) hTextField;
   return pContext ? pContext->sT9ccFieldInfo.nKeyBufLen : NULL;
#else
   return ( NULL);
#endif
}

/*quit the number method in add the contact number and ip number set on 080903*/
int32 OEM_ExitIputMethod(OEMCONTEXT hTextField)
{
    TextCtlContext *pContext = (TextCtlContext *) hTextField;
	if(NULL == pContext)
		return EFAILED;
	if(pContext->dwProperties & TP_EDITNUMBER_PTSTRING)
	{
		 ISHELL_CancelTimer((IShell *) pContext->pIShell,
                             TextCtl_MultitapTimer, pContext);
	}
	return SUCCESS;
}
/*add the code end*/
void OEM_TextSetBackGround(OEMCONTEXT hTextField, const IImage * pImageBg)
{
    TextCtlContext *pContext = (TextCtlContext *) hTextField;
    
    if(pContext != NULL)
    {
        pContext->m_pImageBg = (IImage*)pImageBg;
    }
}

/*===========================================================================

                     LOCAL/STATIC FUNCTION IMPLEMENTATIONS

===========================================================================*/

/*=============================================================================
FUNCTION: FindModeIndex

DESCRIPTION:

PARAMETERS:
   *pContext:
   wMode:

RETURN VALUE:
   char:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static unsigned char FindModeIndex(TextCtlContext   *pContext,
                                   AEETextInputMode  wMode)
{
   // Return the matching mode index by searching the table sTextModes.
   // If there is no match, return index 0 as the default.

   int   sTextModesSize;
   uint8 i;

   if (wMode == AEE_TM_CURRENT && pContext) {
      // Special case
      return pContext->byMode;
   }
   
   sTextModesSize = ARRAY_SIZE(sTextModes);
   
   for (i=0; i<sTextModesSize; ++i) {
      if (sTextModes[i].info.wID == wMode) {
         return i;
      }
   }
   return 0;
}

/*=============================================================================
FUNCTION: TextCtl_TextChanged

DESCRIPTION:

PARAMETERS:
   *pContext:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_TextChanged(TextCtlContext *pContext)
{
   // Assume we're never called with a NULL pointer as this
   // is a private function.
   int iFontAscent, iFontDescent;

   // Start off assumine we're not valid for display
   pContext->bValid = FALSE;

   // We must validate various aspects of the text control
   // and determine whether or not the display needs to
   // be re-calculated/re-drawn.
   // First normalize the selection if necessary
   if (pContext->wSelEnd < pContext->wSelStart) {
      // Swap start and end of selection
      unsigned short swap=pContext->wSelEnd;
      pContext->wSelEnd = pContext->wSelStart;
      pContext->wSelStart = swap;
      pContext->bSwap = TRUE;
   }
   else
   {
      pContext->bSwap = FALSE;
   }

   // Now check start for being in range
   if (pContext->wSelStart > pContext->wContentsChars) {
      pContext->wSelStart = pContext->wContentsChars;
   }

   // Now check end for being in range
   if (pContext->wSelEnd > pContext->wContentsChars) {
      pContext->wSelEnd = pContext->wContentsChars;
   }

   // Update font characteristics
   if (IDISPLAY_GetFontMetrics((IDisplay *)pContext->pIDisplay, pContext->font,
                               &iFontAscent, &iFontDescent) == EFAILED) {
      // Can't function like this, leave bValid set to false
      // and just return.
      return;
   }

   pContext->nFontAscent = (short) iFontAscent;
   pContext->nFontDescent = (short) iFontDescent;
   if (pContext->nFontAscent < 0 || pContext->nFontDescent < 0 ||
       pContext->nFontLeading < 0) {
      // Invalid values, leave bValid false and return
      return;
   }
   if (pContext->nFontAscent + pContext->nFontDescent == 0) {
      // Invalid values, leave bValid false and return
      return;
   }

   // Calculate maximum number of lines
   if (pContext->dwProperties & TP_FRAME) {
      pContext->nDisplayLines = ( (pContext->rectDisplay.dy - 4) +
                                  pContext->nFontLeading          ) /
                                     (pContext->nFontAscent +
                                      pContext->nFontDescent +
                                      pContext->nFontLeading);
   } else {
      pContext->nDisplayLines = ((short) pContext->rectDisplay.dy + pContext->nFontLeading) /
                                (pContext->nFontAscent + pContext->nFontDescent + pContext->nFontLeading);
   }
   if (!(pContext->dwProperties & TP_MULTILINE) && pContext->nDisplayLines > 1) {
      // Restrict to at most 1 line
      pContext->nDisplayLines = 1;
   }

   if (pContext->nDisplayLines <= 0)
      return;

   // Calculate the number of extra pixels that don't fit a whole line
   // Only count the leading BETWEEN lines
   pContext->nExtraPixels = (int16) ( pContext->rectDisplay.dy +
                                      pContext->nFontLeading -
                                       pContext->nDisplayLines *
                                         (pContext->nFontAscent  +
                                          pContext->nFontDescent +
                                          pContext->nFontLeading)
                                    );
   if (!(pContext->dwProperties & TP_MULTILINE) && (pContext->dwProperties & TP_FIXOEM)) {
      // Adjust for frame
      pContext->nExtraPixels = pContext->nExtraPixels/2 + 1;
   }
   if (pContext->dwProperties & TP_FRAME) {
      // Adjust for frame
      pContext->nExtraPixels -= 4;
   }

   // We're valid now pending recalc.
   pContext->bValid = TRUE;

   // Last step is to re-calculate the line starts
   (void) TextCtl_CalText(pContext);

   // And possibly auto-scroll the selection into view
   (void) TextCtl_AutoScroll(pContext);

   pContext->bNeedsDraw = TRUE;
}

/*=============================================================================
FUNCTION: TextCtl_CalText

DESCRIPTION:

PARAMETERS:
   *pContext:

RETURN VALUE:
   boolean:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean TextCtl_CalText(TextCtlContext *pContext)
{
   // Start off assuming it fits without scroll bars.
   // If it doesn't, a recursive call will automatically
   // be generated re-trying it with scroll bars.
   return TextCtl_CalText2(pContext, FALSE);
}

/*=============================================================================
FUNCTION: TextCtl_CalText2

DESCRIPTION:

PARAMETERS:
   *pContext:
   bScroll:

RETURN VALUE:
   boolean:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean TextCtl_CalText2(TextCtlContext *pContext, boolean bScroll)
{
   AEERect  textRect;
   int16    wWidth;
   uint16   wChars = pContext->wContentsChars;
   AECHAR  *psText = pContext->pszContents;

   // Do nothing if not valid
   if (!pContext->bValid) return FALSE;

   // Calculate the text rect and pixel width for lines
   if (pContext->dwProperties & TP_FRAME) {
      textRect = pContext->rectDisplay;
      textRect.x += 2;
      textRect.y += 2;
      textRect.dx -= 4;
      textRect.dy -= 4;
      if (bScroll) {
         // Subtract scroll bar width, but it shares 2 pixels with the frame
         textRect.dx -= (int16) (pContext->wScrollBarWidth - 2);
      }
   } else {
      textRect = pContext->rectDisplay;
      if (bScroll) {
         // Subtract scroll bar width, no sharing
         textRect.dx -= (int16) pContext->wScrollBarWidth;
      }
   }
   wWidth = textRect.dx;

   // First toss any old line starts
   if (pContext->pwLineStarts) OEM_Free(pContext->pwLineStarts);
   pContext->wLines = 0;
   pContext->pwLineStarts = (unsigned short *) OEM_Malloc(sizeof(unsigned short));
   if (!pContext->pwLineStarts) {
      pContext->bValid = FALSE;
      return FALSE;
   }
   pContext->pwLineStarts[0] = 0;
   for (; wChars;) {
      // Start a new line
      if (!bScroll && pContext->wLines >= pContext->nDisplayLines
          && (pContext->dwProperties & TP_MULTILINE)) {
         // We now need a scroll bar, so restart with scroll bar
         // set to true!
         return TextCtl_CalText2(pContext, TRUE);
      } else {
         // Total width of characters that fit on this line
         int wLineWidth = 0;

         AECHAR *newStarts = (AECHAR *) OEM_Realloc(pContext->pwLineStarts,
                                                    (++pContext->wLines + 1) *
                                                               sizeof(AECHAR));
         AECHAR *breakSpace;
         uint16  nBackupChars;
         boolean bBackupNeeded = FALSE;

         if (!newStarts) {
            pContext->bValid = FALSE;
            return FALSE;
         }
         pContext->pwLineStarts = newStarts;
         // Accumulate characters into the line
         for (; wChars; --wChars, ++psText) {
            int nFitsChars;
            int nWidthPixels;
            uint16   nCharSpan = 1; // Default is single character, Thai can span to 3

            if (*psText == LINEBREAK) {
               // Special handling.  This is the last character of this line
               // It always fits and it's never drawn
               ++psText;
               --wChars;
               break;
            }
            // Add the character width
            nWidthPixels = IDISPLAY_MeasureTextEx(pContext->pIDisplay,
                                                  pContext->font,
                                                  psText,
                                                  nCharSpan,
                                                  -1,
                                                  &nFitsChars);

            if (nWidthPixels > wWidth) {
               // If a character is wider than the total width of the text
               // control, we are invalid for display (we'd get stuck in an
               // infinite loop trying to calculate the line breaks)
               pContext->bValid = FALSE;
               return FALSE;
            }

            if (wLineWidth + nWidthPixels > wWidth) {
               // We want the scroll to work even for space
               bBackupNeeded = TRUE;   // Request "nice" break
               break;
            }
            // Accumulate the character's width into the line's width
            wLineWidth += nWidthPixels;

             // Post Op conditionals
             wChars -= (nCharSpan-1);
             psText += (nCharSpan-1);

         }
         // Do "nice" word-breaking by backing up until we find a space.  If
         // no space is found on this line leave the break where it is.
         breakSpace = psText-1;
         nBackupChars = 0;

         // Only try to backup for "nice" break if requested
         if (bBackupNeeded) {
            while (*breakSpace != ' ' && breakSpace >
                   (pContext->pszContents +
                                 pContext->pwLineStarts[pContext->wLines-1])) {
               --breakSpace;
               ++nBackupChars;
            }
         }
         if (breakSpace >
             (pContext->pszContents +
                                 pContext->pwLineStarts[pContext->wLines-1])) {
            // We found a space in this line, so break there instead
            psText -= nBackupChars;
            wChars += nBackupChars;
         }
         // Set the line start value for the beginning of the next line
         pContext->pwLineStarts[pContext->wLines] =
                                      (uint16)(psText - pContext->pszContents);
         // psText is now left pointing at the first character of the next
         // line with wChars indicating how many characters are left to examine
      }
   }
   return TRUE;
}


/*=============================================================================
FUNCTION: TextCtl_AddChar

DESCRIPTION:

PARAMETERS:
   *pContext:
   ch:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_AddChar(TextCtlContext *pContext, AECHAR ch)
{
   boolean bModified = FALSE;
   // Don't allow the null character to be inserted.
   if (pContext) {
      AECHAR *pNewContents;

      /* handle when textcontent reaches the limit */
      // effect: delete all if contents is full.
      /*   if (pContext->wContentsChars == pContext->wMaxChars){
               pContext->wSelEnd   = pContext->wMaxChars;
               pContext->wSelStart =0;
           }
       */
      // First delete the selection if any
      if (pContext->wSelEnd > pContext->wSelStart) {
         // Be sure not to lose the trailing NUL character!
         MEMMOVE(pContext->pszContents+pContext->wSelStart,
                 pContext->pszContents+pContext->wSelEnd,
                 sizeof(AECHAR) *
                         ((pContext->wContentsChars - pContext->wSelEnd) + 1));
         pContext->wContentsChars -= pContext->wSelEnd - pContext->wSelStart;
         pContext->wSelEnd = pContext->wSelStart;
         bModified = TRUE;
      }
      if (ch) {
         if (!pContext->wMaxChars
             || pContext->wContentsChars < pContext->wMaxChars) {
            // Now grow the contents bl0ock to make sure it will fit the
            // additional character (and don't forget to include the
            // NULL character!)
            pNewContents = (AECHAR *) OEM_Realloc(pContext->pszContents,
                                           sizeof(AECHAR) *
                                           (pContext->wContentsChars + 1 + 1));
            if (!pNewContents) {
               // Bad, out of memory, so just ignore the character
               return;
            }
            pContext->pszContents = pNewContents;

            // Now move text around to make room for the new character
            MEMMOVE(pContext->pszContents+pContext->wSelStart+1,
                    pContext->pszContents+pContext->wSelEnd,
                    sizeof(AECHAR) *
                         ((pContext->wContentsChars - pContext->wSelEnd) + 1));

            // Write in the new character
            pContext->pszContents[pContext->wSelStart] = ch;
            ++pContext->wContentsChars;

            // Update the selection to be after the new character
            ++pContext->wSelStart;
            pContext->wSelEnd = pContext->wSelStart;

            bModified = TRUE;
         } else {
            // Maybe beep at the user because maximum length exceeded?
         }
      }
      
      if (bModified) {
         // Now re-calc and re-draw
         TextCtl_TextChanged(pContext);
      }
   }
}

/*=============================================================================
FUNCTION: TextCtl_AddString

DESCRIPTION:

PARAMETERS:
   *pContext:
   sz:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_AddString(TextCtlContext *pContext, AECHAR * sz)
{
   boolean bModified = FALSE;
   if( pContext && sz && *sz ){
      AECHAR * pNewContents;

      // First delete the selection if any
      if( pContext->wSelEnd > pContext->wSelStart ){
         // Be sure not to lose the trailing NULL character!
         MEMMOVE(pContext->pszContents+pContext->wSelStart,
                 pContext->pszContents+pContext->wSelEnd,
                 sizeof(AECHAR) * (pContext->wContentsChars - pContext->wSelEnd + 1));
         pContext->wContentsChars -= pContext->wSelEnd - pContext->wSelStart;
         pContext->wSelEnd = pContext->wSelStart;
         bModified = TRUE;
      }

      if( !pContext->wMaxChars
          || pContext->wContentsChars < pContext->wMaxChars ){
         int   nAllocSize;
         // Check if the full substring can fit.
         if( pContext->wMaxChars && WSTRLEN(sz)+pContext->wContentsChars > pContext->wMaxChars ){
            nAllocSize = (pContext->wMaxChars+1)*sizeof(AECHAR);
         }else{
            nAllocSize = (pContext->wContentsChars + WSTRLEN(sz) + 1) * sizeof(AECHAR);
         }
         // Now grow the contents block to make sure it will fit the
         // additional character (and don't forget to include the NUL character!)
         pNewContents = (AECHAR *) OEM_Realloc(pContext->pszContents, nAllocSize);

         if (!pNewContents) {
            // Bad, out of memory, so just ignore the character
            return;
         }
         pContext->pszContents = pNewContents;

         // Now move text around to make room for the new character
         if ((!pContext->wMaxChars) ||
             ((pContext->wContentsChars + WSTRLEN(sz))
                < pContext->wMaxChars))
          {
              MEMMOVE(pContext->pszContents+pContext->wSelStart+WSTRLEN(sz),
                      pContext->pszContents+pContext->wSelEnd,
                      sizeof(AECHAR) * ((pContext->wContentsChars - pContext->wSelEnd) + 1));
          }
          else
          {
              MEMMOVE(pContext->pszContents+pContext->wSelStart
                      +(pContext->wMaxChars - pContext->wContentsChars),
                      pContext->pszContents+pContext->wSelEnd,
                      sizeof(AECHAR) * ((pContext->wContentsChars - pContext->wSelEnd) + 1));
          }

         // Write in the new character
         if ((!pContext->wMaxChars) ||
             ((pContext->wContentsChars + WSTRLEN(sz))
               < pContext->wMaxChars))
          {
              MEMCPY((void *)&pContext->pszContents[pContext->wSelStart],(void *)sz, WSTRLEN(sz)*sizeof(AECHAR));
              pContext->wContentsChars += WSTRLEN(sz);

              // Update the selection to be after the new character
              pContext->wSelStart += WSTRLEN(sz);
              pContext->wSelEnd = pContext->wSelStart;
          }
          else
          {
              uint16       nadd;
              // Write in the new character
              MEMCPY((void *)&pContext->pszContents[pContext->wSelStart],
                    (void *)sz,
                    (pContext->wMaxChars - pContext->wContentsChars)*sizeof(AECHAR));
              nadd = pContext->wMaxChars - pContext->wContentsChars;
              pContext->wContentsChars += nadd;

              // Update the selection to be after the new character
              pContext->wSelStart += nadd;
              pContext->wSelEnd = pContext->wSelStart;
          }
         
         bModified = TRUE;
      }
      else {
         // Maybe beep at the user because maximum length exceeded?
      }

      if (bModified) {
         // Now re-calc and re-draw
         TextCtl_TextChanged(pContext);
      }
   }
}

/*=============================================================================
FUNCTION: TextCtl_SetSel

DESCRIPTION:

PARAMETERS:
   *pContext:
   selStart:
   selEnd:

RETURN VALUE:
   boolean:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean TextCtl_SetSel(TextCtlContext *pContext,
                              int             selStart,
                              int             selEnd)
{
   boolean bChanged = FALSE;
   if (pContext) {
      int selSwap;

      if (selStart < 0)
         selStart = (int) pContext->wContentsChars + 1 + selStart;

      if (selEnd < 0)
         selEnd = (int) pContext->wContentsChars + 1 + selEnd;

      if (selStart < 0)
         selStart = 0;

      if (selEnd < 0)
         selEnd = 0;

      if (selStart > pContext->wContentsChars)
         selStart = pContext->wContentsChars;

      if (selEnd > pContext->wContentsChars)
         selEnd = pContext->wContentsChars;

      if (selStart > selEnd) {
         selSwap = selStart;
         selStart = selEnd;
         selEnd = selSwap;
         pContext->bSwap = TRUE;
      }
      else
      {
         pContext->bSwap = FALSE;
      }
      if ((uint16) selStart != pContext->wSelStart) {
         pContext->wSelStart = (uint16) selStart;
         bChanged = TRUE;
      }
      if ((uint16) selEnd != pContext->wSelEnd) {
         pContext->wSelEnd = (uint16) selEnd;
         bChanged = TRUE;
      }
      if (bChanged) {
         pContext->bNeedsDraw = TRUE;
      }
   }
   return bChanged;
}

/*=============================================================================
FUNCTION: TextCtl_GetLine

DESCRIPTION:

PARAMETERS:
   *pContext:
   nSelIndex:

RETURN VALUE:
   uint16:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static uint16 TextCtl_GetLine(TextCtlContext *pContext, uint16 nSelIndex)
{
   if (pContext && pContext->wLines) {
      uint16 i;

      if (nSelIndex > pContext->wContentsChars) nSelIndex = pContext->wContentsChars;
      for (i=1; i<=pContext->wLines; ++i) {
         if (nSelIndex < pContext->pwLineStarts[i]) {
            // Definitely Line i-1
            return i-1;
         }
         if (nSelIndex == pContext->pwLineStarts[i]) {
            // Could be Line i-1 or Line i, check for LINEBREAK
            if (pContext->pszContents[nSelIndex-1] == LINEBREAK)
               return i;// Definitely next line unless end of text
            // Could still be that line if adding characters
            if( WSTRLEN(pContext->pszContents) == nSelIndex )
               return(i-1);
            else
               return i;
         }
      }
   }
   return 0;
}

/*=============================================================================
FUNCTION: TextCtl_AutoScroll

DESCRIPTION:

PARAMETERS:
   *pContext:

RETURN VALUE:
   boolean:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean TextCtl_AutoScroll(TextCtlContext *pContext)
{
   // Scroll the text so that the selection is visible.
   // If all of the selection doesn't fit, make the starting point fit.

   uint16 wStartSelLine, wEndSelLine;
   boolean bChanged = FALSE;

   if (pContext && pContext->bValid && pContext->wLines) {
      if (pContext->wLines <= pContext->nDisplayLines) {
         // wDisplayStartLine should be 0 if everything fits
         if (pContext->wDisplayStartLine) {
            pContext->wDisplayStartLine = 0;
            bChanged = TRUE;
         }
      } else {
         wStartSelLine = TextCtl_GetLine(pContext, pContext->wSelStart);
         wEndSelLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

         // Make sure wDisplayStartLine is valid
         if (pContext->wDisplayStartLine > pContext->wLines - pContext->nDisplayLines) {
            // Must have deleted some text, pin wDisplayStartLine
            pContext->wDisplayStartLine =
                          (uint16)(pContext->wLines - pContext->nDisplayLines);
            bChanged = TRUE;
         }

         // Check selection start
         if (wStartSelLine < pContext->wDisplayStartLine) {
            pContext->wDisplayStartLine = wStartSelLine;
            bChanged = TRUE;
         }

         if (wEndSelLine >
                 (pContext->wDisplayStartLine + pContext->nDisplayLines - 1)) {
            int newStart = (wEndSelLine - pContext->nDisplayLines) + 1;

            if (newStart > wStartSelLine) {
               // Always include start of selection
               newStart = wStartSelLine;
            }
            if (newStart != pContext->wDisplayStartLine) {
               pContext->wDisplayStartLine = (uint16) newStart;
               bChanged = TRUE;
            }
         }
      }
   }
   if (bChanged && pContext) pContext->bNeedsDraw = TRUE;
   return bChanged;
}

/*=============================================================================
FUNCTION: TextCtl_DrawScrollBar

DESCRIPTION:

PARAMETERS:
   *pContext:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_DrawScrollBar(TextCtlContext *pContext)
{
   // Assume this is only called internally when bValid is true and
   // scroll bars need to be draw.  This function will ALWAYS draw
   // a scroll bar if called.

   AEERect aRect,
           scratchRect;
   int32   wBlackPixels;
   int32   wTopWhitePixels;
   int32   wBottomWhitePixels;
   int16   wScrollBarHeight = pContext->rectDisplay.dy - 2;

   // Draw the clear 1-pixel border to the left of the scroll bar
   aRect.x = pContext->rectDisplay.x +
                 (int16)(pContext->rectDisplay.dx - pContext->wScrollBarWidth);
   aRect.y = pContext->rectDisplay.y;
#ifdef FEATURE_SCROLLBAR_USE_STYLE
   aRect.dx = pContext->wScrollBarWidth - 1;
#else
   aRect.dx = 1;
#endif
   aRect.dy = pContext->rectDisplay.dy;
   scratchRect = aRect;
   if (pContext->dwProperties & TP_FRAME) {
      /* Don't erase part of the frame! */
#ifdef FEATURE_SCROLLBAR_USE_STYLE
      scratchRect.x -= 2;
      scratchRect.y += 2;
      scratchRect.dy -= 4;
      wScrollBarHeight -= 2;
      aRect = scratchRect;
#else
      scratchRect.y += 1;
      scratchRect.dy -= 2;
#endif
   }
   if(pContext->dwProperties & TP_GRAPHIC_BG)
   {
      TextCtl_DrawBackGround(pContext, &scratchRect);
   }
   else
   {
      IDISPLAY_FillRect(pContext->pIDisplay, &scratchRect, RGB_WHITE);
   }

   // Now calculate the length of the black portion of the scroll bar
   if (pContext->wLines > pContext->nDisplayLines) {
      int32 wWhitePixels;
      int16 wLinesOffScreen;

      wBlackPixels = (pContext->nDisplayLines *
                      wScrollBarHeight /
                      pContext->wLines);

      if (wBlackPixels < 3) {
         wBlackPixels = 3; // Minimum size of black part
      }

      if (wBlackPixels > wScrollBarHeight) {
         wBlackPixels = wScrollBarHeight;
      }

      wLinesOffScreen = (int16)(pContext->wLines - pContext->nDisplayLines);
      wWhitePixels = wScrollBarHeight - wBlackPixels;
      wTopWhitePixels = (wWhitePixels *
                         pContext->wDisplayStartLine /
                         wLinesOffScreen);

      if (wWhitePixels && !wTopWhitePixels && pContext->wDisplayStartLine) {
         // Be sure to show at least 1 pixel of white on the top if we're not
         // really scrolled all the way to the top
         wTopWhitePixels = 1;
      }
      wBottomWhitePixels = wWhitePixels - wTopWhitePixels;
      if (!wBottomWhitePixels && wWhitePixels && pContext->wDisplayStartLine <
          wLinesOffScreen) {
         // Really we should show at least 1 pixel of white on the bottom
         if (wTopWhitePixels >= 2) {
            // Get it from the top white area
            --wTopWhitePixels;
            ++wBottomWhitePixels;
         } else if (wBlackPixels > 3) {
            // Squeeze it out of the black area
            --wBlackPixels;
            ++wBottomWhitePixels;
         }
      }
   } else {
      wBlackPixels = wScrollBarHeight;
      wTopWhitePixels = 0;
      wBottomWhitePixels = 0;
   }

   // Re-adjust aRect for scroll bar area itself
   ++aRect.x;
   aRect.dx = (int16)pContext->wScrollBarWidth - 1;

   // Frame the outer area of the scroll bar
   IDISPLAY_FrameRect((IDisplay *)pContext->pIDisplay, &aRect);

   // Now adjust aRect for just the inside of the scroll bar
   ++aRect.x;
#ifdef FEATURE_SCROLLBAR_USE_STYLE
   --aRect.dx;
#else
   aRect.dx -= 2;
#endif
   ++aRect.y;
   aRect.dy -= 2;

   // Draw the top white pixels
   if (wTopWhitePixels) {
      aRect.dy = (int16) wTopWhitePixels;
#ifdef FEATURE_SCROLLBAR_USE_STYLE
      IDISPLAY_FillRect(pContext->pIDisplay,
                        &aRect,
                        MAKE_RGB(0xDE, 0xDE, 0xDE));
#else
      IDISPLAY_FillRect(pContext->pIDisplay,
                        &aRect,
                        RGB_WHITE);
#endif

      aRect.y += (int16) wTopWhitePixels;
   }
   // Draw the black pixels (we always have some of these)
   aRect.dy = (int16) wBlackPixels;
   if (aRect.dy + aRect.y > pContext->rectDisplay.y + wScrollBarHeight/*pContext->rectDisplay.dy*/)
     aRect.dy = pContext->rectDisplay.y + wScrollBarHeight/*pContext->rectDisplay.dy*/ - aRect.y;
#ifdef FEATURE_SCROLLBAR_USE_STYLE
   IDISPLAY_FillRect(pContext->pIDisplay, &aRect, MAKE_RGB(0xFF, 0x70, 0x00));
#else
   IDISPLAY_FillRect(pContext->pIDisplay, &aRect, CLR_SYS_SCROLLBAR_FILL);//IDISPLAY_FillRect(pContext->pIDisplay, &aRect, RGB_BLACK);
#endif

   // Draw the bottom white pixels
   if (wBottomWhitePixels) {
      aRect.y += (int16) wBlackPixels;
      aRect.dy = (int16) wBottomWhitePixels;
      
#ifdef FEATURE_SCROLLBAR_USE_STYLE
      IDISPLAY_FillRect(pContext->pIDisplay, &aRect, MAKE_RGB(0xDE, 0xDE, 0xDE));
#else
      IDISPLAY_FillRect(pContext->pIDisplay, &aRect, RGB_WHITE);
#endif
   }
}

/*=============================================================================
FUNCTION: IntersectRect

DESCRIPTION:

PARAMETERS:
   *pOutRect:
   *pRect1:
   *pRect2:

RETURN VALUE:
   boolean:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean IntersectRect(AEERect *pOutRect, const AEERect *pRect1,
                             const AEERect *pRect2)
{
   long ans_x, ans_y, ans_dx, ans_dy;

   if (!pRect1 || !pRect2) return FALSE;

   // Get furthest right x
   if (pRect1->x >= pRect2->x)
      ans_x = pRect1->x;
   else
      ans_x = pRect2->x;

   // Get furthest down y
   if (pRect1->y >= pRect2->y)
      ans_y = pRect1->y;
   else
      ans_y = pRect2->y;

   // Get furthest left dx
   if (pRect1->x+pRect1->dx <= pRect2->x+pRect2->dx)
      ans_dx = pRect1->x+pRect1->dx - ans_x;
   else
      ans_dx = pRect2->x+pRect2->dx - ans_x;

   // Get furthest up dy
   if (pRect1->y+pRect1->dy <= pRect2->y+pRect2->dy)
      ans_dy = pRect1->y+pRect1->dy - ans_y;
   else
      ans_dy = pRect2->y+pRect2->dy - ans_y;

   // See if result is non-empty
   if (ans_dx > 0 && ans_dy > 0) {
      // Return intersection if requested
      if (pOutRect) {
         pOutRect->x = (int16) ans_x;
         pOutRect->y = (int16) ans_y;
         pOutRect->dx = (int16) ans_dx;
         pOutRect->dy = (int16) ans_dy;
      }
      return TRUE;
   } else
      return FALSE;
}
/*=============================================================================
FUNCTION: TextCtl_DrawCursorTimer

DESCRIPTION:

PARAMETERS:
   *pContext:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
static void TextCtl_DrawCursorTimer(TextCtlContext *pContext)
{
    static boolean staCursorFlag = TRUE;
    AEERect *drawCursor = NULL;
    
    if(NULL == pContext)//pContext
    {
        return ;
    }
    
    drawCursor = &(pContext->CursorDrawRectTimerPara);
    if((drawCursor->dx >0) && (drawCursor->dy > 0))
    {
        if(staCursorFlag)
        {
            staCursorFlag = FALSE;
            //IDISPLAY_FillRect(pContext->pIDisplay, drawCursor, RGB_BLACK);
            IDISPLAY_InvertRect(pContext->pIDisplay, drawCursor);
        }
        else
        {
            staCursorFlag = TRUE;
            //IDISPLAY_FillRect(pContext->pIDisplay, drawCursor, RGB_WHITE);
            IDISPLAY_InvertRect(pContext->pIDisplay, drawCursor);
        }
        IDISPLAY_UpdateEx( pContext->pIDisplay, FALSE);
        (void) ISHELL_SetTimer((IShell *) (pContext->pIShell),
                                                TIMEOUT,
                                                (PFNNOTIFY)TextCtl_DrawCursorTimer,
                                                pContext);
    }
}
#endif //FEATURE_ARPHIC_LAYOUT_ENGINE
/*=============================================================================
FUNCTION: TextCtl_DrawCursor

DESCRIPTION:

PARAMETERS:
   *pContext:
   *cursRect:
   *clipRect:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_DrawCursor(TextCtlContext *pContext,
                               const AEERect  *cursRect,
                               const AEERect  *clipRect)
{
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
    AEERect draw, scratch  = *cursRect;

    scratch.dx = 1;
    scratch.dy = 14; 
    
    if (IntersectRect(&draw, &scratch, clipRect))
    {
        pContext->CursorDrawRectTimerPara = draw;
        //memcpy((char *)&(pContext->CursorDrawRectTimerPara), (char *)&draw, sizeof(AEERect));
        TextCtl_DrawCursorTimer(pContext);
    }
#else    

   // Draw a cursor by drawing a horizontal line at the top and bottom of cursRect,
   // A vertical line in the middle of cursRect with a 1-pixel hole poked out on
   //   the top and bottom of it.

   // Unfortunately, this drawing needs to be clipped to the display
   // rectangle and there's no easy way to do it.

   AEERect draw, scratch = *cursRect;
/*  
   scratch.dy = 1;

   // Top bar
   if (IntersectRect(&draw, &scratch, clipRect))
      IDISPLAY_FillRect(pContext->pIDisplay, &draw, RGB_BLACK);
   scratch.y = cursRect->y + cursRect->dy - 1;
   // Bottom bar
   if (IntersectRect(&draw, &scratch, clipRect))
      IDISPLAY_FillRect(pContext->pIDisplay, &draw, RGB_BLACK);
   scratch.x += (int16)( (uint16) scratch.dx >> 1 );
   scratch.dx = 1;
   // Bottom hole
   if (IntersectRect(&draw, &scratch, clipRect))
      IDISPLAY_FillRect(pContext->pIDisplay, &draw, RGB_WHITE);
   scratch.y = cursRect->y;
   // Top hole
   if (IntersectRect(&draw, &scratch, clipRect))
      IDISPLAY_FillRect(pContext->pIDisplay, &draw, RGB_WHITE);
   ++scratch.y;
   scratch.dy = cursRect->dy - 2;
*/ 
   scratch.x += (int16)( (uint16) scratch.dx >> 1 ) + 1;
   scratch.dx = 1;
   scratch.dy = 14; 
   // Vertical bar
   // ���д�ֱ������ж���
   if (IntersectRect(&draw, &scratch, clipRect))
   {
       if(!(pContext->dwProperties & TP_MULTILINE) && 
                         (pContext->dwProperties & TP_FIXOEM))
       {
            draw.x++;
            draw.y += pContext->nExtraPixels;
       }
       if(pContext->dwProperties & TP_GRAPHIC_BG)
       {
           IDISPLAY_FillRect(pContext->pIDisplay, &draw, TEXT_GRAPHIC_FONT_COLOR); 
       }
       else
       {
           IDISPLAY_FillRect(pContext->pIDisplay, &draw, TEXT_FONT_COLOR/*RGB_BLACK*/); 
       }
   }
#endif //FEATURE_ARPHIC_LAYOUT_ENGINE
}

/*=============================================================================
FUNCTION: TextCtl_DrawTextPart

DESCRIPTION:

PARAMETERS:
   *pContext:
   bScroll:
   bFrame:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_DrawTextPart(TextCtlContext *pContext,
                                 boolean         bScroll,
                                 boolean         bFrame)
{
   // Assume we're not called unless bValid is true

   // Very simple loop here, starting with the wDisplayLineStart
   // line, draw up to wDisplayLines of text erasing the leading
   // in-between lines (if leading is != 0)

   RGBVAL dwOldBkClr;

   unsigned i             = pContext->wDisplayStartLine;
   int16    cnt           = pContext->nDisplayLines;
   uint16   wSelStartLine = TextCtl_GetLine(pContext, pContext->wSelStart);
   uint16   wSelEndLine   = TextCtl_GetLine(pContext, pContext->wSelEnd);
   boolean  bDrawCursor   = FALSE,
            bCursor       = (pContext->wSelStart == pContext->wSelEnd);
   AEERect  rectText,
            rectLeading,
            cursRect,
            rectClip;
   AECHAR  *wszHide = NULL;

#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
   //IBitmap       *pDestBmp = NULL;
   int            cursorx1    = 0,  cursorx2    = 0;
   int            LineCursor1 = -1, LineCursor2 = -1;
   pContext->dwAlignFlags = ParagraphAlignment(pContext->pszContents, WSTRLEN(pContext->pszContents)); 
   //pDestBmp = IDISPLAY_GetDestination ( pContext->pIDisplay );

   (void) ISHELL_CancelTimer((IShell *) (pContext->pIShell), 
                                         (PFNNOTIFY)TextCtl_DrawCursorTimer, 
                                         pContext);
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE

   /*��Ϊ���ַ�������һ�л�������βʱ�����ַ�������ո�
   ��Ѻܳ��Ŀռ䷴��(multitap input mode)*/
   if ( (wSelEndLine != wSelStartLine) &&
        ((pContext->sT9awFieldInfo.G.dwStateBits & T9STATEMULTITAPMASK) == T9STATEMULTITAPMASK))
   {
      wSelEndLine = wSelStartLine;
   }

   memset(&cursRect, 0, sizeof(cursRect));

   rectClip.x  = pContext->rectDisplay.x;
   rectClip.y  = pContext->rectDisplay.y;
   rectClip.dx = pContext->rectDisplay.dx;
   rectClip.dy = pContext->rectDisplay.dy;

   if (bFrame) {
      rectClip.x  += 2;
      rectClip.y  += 2;
      rectClip.dx -= 4;
      rectClip.dy -= 4;

      // If we have a frame, the scroll bar overlaps it by 2 pixels
      if (bScroll) {
         rectClip.dx -= (int16) (pContext->wScrollBarWidth - 2);
      }

   } else {
      if (bScroll) {
         rectClip.dx -= (int16) pContext->wScrollBarWidth;
      }
   }
   rectText = rectClip;
   rectText.dy = pContext->nFontAscent + pContext->nFontDescent;

   rectLeading = rectText;
   rectLeading.y += rectText.dy;
   rectLeading.dy = pContext->nFontLeading;

   if(pContext->dwProperties & TP_GRAPHIC_BG)
   {
       dwOldBkClr = TEXT_GRAPHIC_BG_COLOR;
   }
   else
   {
       dwOldBkClr = TEXT_BG_COLOR;
   }
   dwOldBkClr = IDISPLAY_SetColor((IDisplay *)pContext->pIDisplay,CLR_USER_BACKGROUND,
                        dwOldBkClr);//MAKE_RGB(255,255,255)); 
   for (; cnt > 0; ++i, --cnt) 
   {
      if (cnt > 1 && pContext->nFontLeading) 
      {
         // Draw the leading area first so it's ready to be inverted if
         // we need to draw a selection
         if(pContext->dwProperties & TP_GRAPHIC_BG)
         {
            TextCtl_DrawBackGround(pContext, &rectLeading);
         }
         else
         {
            IDISPLAY_FillRect(pContext->pIDisplay, &rectLeading, RGB_WHITE);
         }
      }
      if (i < pContext->wLines) 
      {
         unsigned short lineChars = pContext->pwLineStarts[i+1] -
                                    pContext->pwLineStarts[i];
         if (pContext->pszContents[pContext->pwLineStarts[i+1]-1]
             == LINEBREAK) 
         {
            // Don't include the line break character in the count!
            --lineChars;
         }

         if( pContext->dwProperties & TP_PASSWORD ) 
         {
            wszHide = WSTRDUP(pContext->pwLineStarts[i] +
                                     pContext->pszContents);
            if (wszHide) 
            {
               int     q;
               boolean bShowLast = FALSE;

               // If in multitap mode show the last character while the
               // selection is active (so they can see what has been entered)
               
               // if (sTextModes[pContext->byMode].info.wID == TEXT_MODE_MULTITAP)
              if ((sTextModes[pContext->byMode].info.wID == TEXT_MODE_MULTITAP)
#ifdef FEATURE_T9_MT_SPANISH
			  	||(sTextModes[pContext->byMode].info.wID == TEXT_MODE_T9_MT_SPANISH)
#endif
#ifdef FEATURE_T9_MT_FRENCH
                ||(sTextModes[pContext->byMode].info.wID == TEXT_MODE_T9_MT_FRENCH)
#endif

                )
               {
                  bShowLast = !bCursor;
               }

               for(q = 0;
                   q < (bShowLast ? WSTRLEN(wszHide) - 1 : WSTRLEN(wszHide));
                   q++ ) 
               {
                  wszHide[q]  = '*';
               }
               if(pContext->dwProperties & TP_GRAPHIC_BG)
               {
                   TextCtl_DrawBackGround(pContext, &rectText);
                   (void) IDISPLAY_DrawText(pContext->pIDisplay,
                                       pContext->font,
                                       wszHide,
                                       lineChars,
                                       rectText.x,
                                       rectText.y,
                                       &rectText,
                                       IDF_TEXT_TRANSPARENT);
               }
               else
               {
                    (void) IDISPLAY_DrawText(pContext->pIDisplay,
                                        pContext->font,
                                        wszHide,
                                        lineChars,
                                        rectText.x,
                                        rectText.y/*+pContext->nFontAscent*/,
                                        &rectText,
                                        IDF_RECT_FILL);//dwFlags);
               }
            }
         } 
         // ���д�ֱ������ж���
         else if (!(pContext->dwProperties & TP_MULTILINE) && 
                  (pContext->dwProperties & TP_FIXOEM))
         {
            int offSet = 0;
            if(AEE_TM_NUMBERS == sTextModes[pContext?pContext->byMode:0].info.wID)
            {
                offSet = 1;
            }
            if(pContext->dwProperties & TP_GRAPHIC_BG)
            {
                TextCtl_DrawBackGround(pContext, &rectClip);

                (void) IDISPLAY_DrawText(pContext->pIDisplay,
                                      pContext->font,
                                      pContext->pwLineStarts[i] +
                                      pContext->pszContents,
                                      lineChars,
                                      rectText.x + 1,
                                      rectText.y + pContext->nExtraPixels + offSet,
                                      &rectClip,
                                      IDF_TEXT_TRANSPARENT);
            }
            else
            {
                (void) IDISPLAY_DrawText(pContext->pIDisplay,
                                      pContext->font,
                                      pContext->pwLineStarts[i] +
                                      pContext->pszContents,
                                      lineChars,
                                      rectText.x + 1,
                                      rectText.y + pContext->nExtraPixels + offSet/*+pContext->nFontAscent*/,
                                      &rectClip,
                                      IDF_RECT_FILL/*|IDF_ALIGN_MIDDLE*/);//dwFlags);
            }
         }
         else 
         {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
                if(pContext->dwProperties & TP_GRAPHIC_BG)
                {
                    TextCtl_DrawBackGround(pContext, &rectText);
                }
                (void) IDISPLAY_DrawText(pContext->pIDisplay,
                                         pContext->font,
                                         pContext->pwLineStarts[i] + pContext->pszContents,
                                         lineChars,
                                         rectText.x,
                                         rectText.y/*+pContext->nFontAscent*/,
                                         &rectText,
                                        IDF_RECT_FILL | pContext->dwAlignFlags);

                if ( NULL != pContext->m_pMyFont )
                {
                    LineCursor1 = pContext->wSelStart - pContext->pwLineStarts[i];
                    IFONT_MeasureTextCursorPos(pContext->m_pMyFont, rectText.x, 
                                                         pContext->pszContents+pContext->pwLineStarts[i], 
                                                        lineChars, &rectText, &cursorx1, LineCursor1, pContext->dwAlignFlags);
                }
#else //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
                if(pContext->dwProperties & TP_GRAPHIC_BG)
                {
                    TextCtl_DrawBackGround(pContext, &rectText);
                    (void) IDISPLAY_DrawText(pContext->pIDisplay,
                                             pContext->font,
                                             pContext->pwLineStarts[i] +
                                             pContext->pszContents,
                                             lineChars,
                                             rectText.x,
                                             rectText.y,
                                             &rectText,
                                             IDF_TEXT_TRANSPARENT);
                }
                else
                {
                    (void) IDISPLAY_DrawText(pContext->pIDisplay,
                                             pContext->font,
                                             pContext->pwLineStarts[i] +
                                             pContext->pszContents,
                                             lineChars,
                                             rectText.x,
                                             rectText.y/*+pContext->nFontAscent*/,
                                             &rectText,
                                             IDF_RECT_FILL);//dwFlags);
                }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
         }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE

         if ( bCursor ) 
         {
            if (wSelStartLine == i && pContext->bEditable) 
            {
               cursRect.x = (int16)cursorx1;//rectText.x + (int16)(cursorx1);
               cursRect.y = rectText.y; // Would subtract 1, but vertical leading
                                        // is embedded in our fonts too
               cursRect.dx = 1;
               cursRect.dy = pContext->nFontAscent + pContext->nFontDescent + 1;
               bDrawCursor = TRUE;  // Draw the cursor at the end
            }
         }
         else 
         {
            if (wSelStartLine <= i && i <= wSelEndLine) 
            {
               /* Must draw some kind of selection on this line */
               AEERect invertRect;
               int16 dy = rectText.dy;
               uint32 cur_text_mode = OEM_TextGetCurrentMode(pContext);
               uint32 startAlign = ParagraphAlignment(pContext->wSelStart + pContext->pszContents, 
                                    lineChars - (pContext->wSelStart - pContext->pwLineStarts[i]));
               uint32 endAlign = ParagraphAlignment(pContext->pwLineStarts[i] + pContext->pszContents, 
                                    pContext->wSelEnd - pContext->pwLineStarts[i]);
               int32 startX = (IDF_ALIGN_LEFT == startAlign)?(rectText.x):(rectText.x + (int16) rectText.dx);
               int32 endX = (wSelEndLine == wSelStartLine)
                                    ?((IDF_ALIGN_LEFT == startAlign)?(rectText.x + (int16) rectText.dx):(rectText.x))
                                    :((IDF_ALIGN_LEFT == endAlign)?(rectText.x + (int16) rectText.dx):(rectText.x));
               if (wSelEndLine == i) 
               {
                  if (pContext->wSelEnd > pContext->pwLineStarts[i]) 
                  {
                        if ( NULL != pContext->m_pMyFont )
                        {
                            LineCursor2 = pContext->wSelEnd - pContext->pwLineStarts[i];
                            IFONT_MeasureTextCursorPos(pContext->m_pMyFont, rectText.x, 
                                                                    pContext->pwLineStarts[i] + pContext->pszContents, 
                                                                    lineChars, &rectText, &cursorx2, LineCursor2, pContext->dwAlignFlags);
                        }
                        endX = (int16)cursorx2;
                  } 
                  else
                        endX = startX;
               }
               if ((wSelStartLine == i) && (pContext->wSelStart > pContext->pwLineStarts[i])) 
               {
                    if ( NULL != pContext->m_pMyFont )
                    {
                        LineCursor1 = pContext->wSelStart - pContext->pwLineStarts[i];
                        IFONT_MeasureTextCursorPos(pContext->m_pMyFont, rectText.x, 
                                                            pContext->pwLineStarts[i] + pContext->pszContents, 
                                                            lineChars, &rectText, &cursorx1, LineCursor1, pContext->dwAlignFlags);
                    }
                    /* Must adjust the left edge */
                    startX = (int16)cursorx1;
               }

               if (i < wSelEndLine) 
               {
                  /* Must include the leading */
                  dy += rectLeading.dy;
               }
               
               if ( startX > endX )
               {
                  int32 tmp = endX;
                  endX = startX;
                  startX = tmp;
               }
               
#ifdef TEXT_MODE_MULTITAP
               if((TEXT_MODE_MULTITAP == cur_text_mode) 
                    && (IDF_ALIGN_RIGHT == pContext->dwAlignFlags) 
                    && (startX == rectText.x))
               {
                    startX = (rectText.x + (int16) rectText.dx) -  IDISPLAY_MeasureTextEx(pContext->pIDisplay, 
                                                                                            pContext->font,
                                                                                            pContext->pszContents + pContext->pwLineStarts[i],
                                                                                            lineChars,
                                                                                            -1,
                                                                                            NULL);
               }
#endif
#ifdef TEXT_MODE_T9_RAPID_ENGLISH
               if((TEXT_MODE_T9_RAPID_ENGLISH == cur_text_mode) 
                   && (IDF_ALIGN_RIGHT == pContext->dwAlignFlags) 
                   && (startX == rectText.x))
               {
                    startX = (rectText.x + (int16) rectText.dx) -  IDISPLAY_MeasureTextEx(pContext->pIDisplay, 
                                                                                            pContext->font,
                                                                                            pContext->pszContents + pContext->pwLineStarts[i],
                                                                                            lineChars,
                                                                                            -1,
                                                                                            NULL);
               }
#endif

               invertRect.x = (int16) startX;
               invertRect.y = rectText.y;
               invertRect.dx = (int16)(endX - startX);
               invertRect.dy = dy;
               if(0
#ifdef TEXT_MODE_MULTITAP               
               || ((TEXT_MODE_MULTITAP == cur_text_mode) && ((int16)cursorx1 > (int16)cursorx2) && (wSelEndLine == wSelStartLine))
#endif
#ifdef TEXT_MODE_T9_MT_HEBREW               
               || ((TEXT_MODE_T9_MT_HEBREW == cur_text_mode) && ((int16)cursorx1 < (int16)cursorx2) && (wSelEndLine == wSelStartLine))
#endif
#ifdef TEXT_MODE_T9_MT_ARABIC
               || ((TEXT_MODE_T9_MT_ARABIC == cur_text_mode)  && ((int16)cursorx1 < (int16)cursorx2) && (wSelEndLine == wSelStartLine))
#endif               
               )
               {
                    ;// no invert
               }
               else
               {
                    IDISPLAY_InvertRect(pContext->pIDisplay, &invertRect);
               }
            }
         }
#else //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE

         if ( bCursor ) 
         {
            if (wSelStartLine == i && pContext->bEditable) 
            {
               /* Must draw a cursor now */
               int32 cursX = rectText.x;
               if (pContext->wSelStart > pContext->pwLineStarts[i]) 
               {
                  if( pContext->dwProperties & TP_PASSWORD ) 
                  {
                     cursX += IDISPLAY_MeasureTextEx(pContext->pIDisplay,
                                                     pContext->font,
                                                     wszHide,
                                                     pContext->wSelStart -
                                                     pContext->pwLineStarts[i],
                                                     -1,
                                                     NULL);

                  } 
                  else 
                  {
                     cursX += IDISPLAY_MeasureTextEx(pContext->pIDisplay,
                                                     pContext->font,
                                                     pContext->pszContents +
                                                     pContext->pwLineStarts[i],
                                                     pContext->wSelStart -
                                                     pContext->pwLineStarts[i],
                                                     -1,
                                                     NULL);
                  }

                  --cursX; // Take advantage of knowledge about where to find
                           // horizontal leading in our fonts.  If we didn't do
                           // this, we'd have to do it anyway for the special
                           // case of ending up 1-pixel outside of the clipping
                           // rectangle.
                  if (cursX >= rectText.x + rectText.dx) 
                  {
                     // If the line ends with a lot of spaces, they will
                     // stick to the end of the line even though technically
                     // they exceed the displayable width, so we can greatly
                     // exceed the displayable pixels when we MeasureText
                     // although what gets chopped is only blank space.
                     // If this happens, we just stick the cursor at the
                     // right edge.
                     cursX = rectText.x + rectText.dx - 1;
                  }
               }
               // else We compromise a bit if we're at the left edge and
               //     don't move left 1 pixel since it would leave the vertical
               //     bar of the cursor outside the clipping rectangle!

               cursRect.x = (int16) (cursX-2);
               cursRect.y = rectText.y; // Would subtract 1, but vertical leading
                                        // is embedded in our fonts too
               cursRect.dx = 5;
               cursRect.dy = pContext->nFontAscent + pContext->nFontDescent + 1;
               bDrawCursor = TRUE;  // Draw the cursor at the end
            }
         } 
         else 
         {
            if (wSelStartLine <= i && i <= wSelEndLine) 
            {
               /* Must draw some kind of selection on this line */
               int32 startX = rectText.x;
               int32 endX = rectText.x + (int16) rectText.dx;
               int16 dy = rectText.dy;
               AEERect invertRect;

               if (wSelEndLine == i) 
               {
                  /* Must adjust the right edge */
                  /* We MUST adjust the right edge BEFORE the left edge because
                   * the forumula uses startX and gets the wrong answer if
                   * the select starts and ends on the same line because startX
                   * would then be adjusted first and throw off this calculation
                   */
                  if (pContext->wSelEnd > pContext->pwLineStarts[i]) 
                  {
                     if( pContext->dwProperties & TP_PASSWORD )
                     {
                        endX = startX +
                               IDISPLAY_MeasureTextEx(pContext->pIDisplay,
                                                      pContext->font,
                                                      wszHide,
                                                      pContext->wSelEnd -
                                                      pContext->pwLineStarts[i],
                                                      -1,
                                                      NULL);
                     } 
                     else 
                     {
                        endX = startX +
                               IDISPLAY_MeasureTextEx(pContext->pIDisplay,
                                                      pContext->font,
                                                      pContext->pszContents +
                                                      pContext->pwLineStarts[i],
                                                      pContext->wSelEnd -
                                                      pContext->pwLineStarts[i],
                                                      -1,
                                                      NULL);
                     }
                  } 
                  else
                     endX = startX;
               }

               if (wSelStartLine == i &&
                   pContext->wSelStart > pContext->pwLineStarts[i]) 
               {

                  if( pContext->dwProperties & TP_PASSWORD )
                  {
                     /* Must adjust the left edge */
                     startX += IDISPLAY_MeasureTextEx(pContext->pIDisplay,
                                                      pContext->font,
                                                      wszHide,
                                                      pContext->wSelStart -
                                                      pContext->pwLineStarts[i],
                                                      -1,
                                                      NULL)
                                              - 1; /* Include 1-pixel leading */
                  } 
                  else 
                  {
                     /* Must adjust the left edge */
                     startX += IDISPLAY_MeasureTextEx(pContext->pIDisplay,
                                                      pContext->font,
                                                      pContext->pszContents +
                                                      pContext->pwLineStarts[i],
                                                      pContext->wSelStart -
                                                      pContext->pwLineStarts[i],
                                                      -1,
                                                      NULL)
                                                 - 1; // Include 1-pixel leading
                  }
               }

               if (i < wSelEndLine) 
               {
                  /* Must include the leading */
                  dy += rectLeading.dy;
               }
               invertRect.x = (int16) startX;
               invertRect.y = rectText.y; 
               if((!(pContext->dwProperties & TP_MULTILINE) && 
                           (pContext->dwProperties & TP_FIXOEM)))
               {
                    invertRect.x++;
                    invertRect.y = (invertRect.y + pContext->nExtraPixels - 1);
               }
               invertRect.dx = (int16)(endX - startX);
               invertRect.dy = dy;
               IDISPLAY_InvertRect(pContext->pIDisplay, &invertRect);
            }
         }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
      } 
      else
      {
         // Draw an empty box, there's no text
         if (!(pContext->dwProperties & TP_MULTILINE) && 
                  (pContext->dwProperties & TP_FIXOEM))
         {
             if(pContext->dwProperties & TP_GRAPHIC_BG)
             {
                TextCtl_DrawBackGround(pContext, &rectClip);
             }
             else
             {
                IDISPLAY_FillRect(pContext->pIDisplay, &rectClip, RGB_WHITE);
             }
         }
         else
         {
             if(pContext->dwProperties & TP_GRAPHIC_BG)
             {
                TextCtl_DrawBackGround(pContext, &rectText);
             }
             else
             {
                IDISPLAY_FillRect(pContext->pIDisplay, &rectText, RGB_WHITE);
             }
         }

         if (bCursor && wSelStartLine == i && pContext->bEditable) 
         {
            // Must draw a cursor.  We can only get here if the text
            // is completely empty, so just use a nice cursor rectangle
            // at where the start of the text would be

            cursRect.x = rectText.x - 2;
            cursRect.y = rectText.y;
            cursRect.dx = 5;
            cursRect.dy = pContext->nFontAscent + pContext->nFontDescent + 1;
            bDrawCursor = TRUE;
         }
      }
      // Adjust rects for next line
      rectText.y += rectText.dy + rectLeading.dy;
      rectLeading.y = rectText.y + rectText.dy;
// v2.0.2 Add
      // Free Hide Buffer
      if( wszHide )
      {
         FREE( wszHide );
         wszHide = NULL;
      }
// End Add
   }//end for

   if (pContext->nExtraPixels) 
   {
      // Draw the area at the bottom that was left over
      rectText.y -= rectLeading.dy;
      rectText.dy = pContext->nExtraPixels;
      if(!(!(pContext->dwProperties & TP_MULTILINE) && 
                  (pContext->dwProperties & TP_FIXOEM)))
      {
         if(pContext->dwProperties & TP_GRAPHIC_BG)
         {
             TextCtl_DrawBackGround(pContext, &rectText);
         }
         else
         {
             IDISPLAY_FillRect(pContext->pIDisplay, &rectText, RGB_WHITE);
         }
      }
   }
   if (bDrawCursor) 
   {
      TextCtl_DrawCursor(pContext, &cursRect, &rectClip);
   }
   
   (void) IDISPLAY_SetColor((IDisplay *)pContext->pIDisplay,CLR_USER_BACKGROUND,dwOldBkClr);
}

/*=============================================================================
FUNCTION: TextCtl_RestartEdit

DESCRIPTION:

PARAMETERS:
   *pContext:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_RestartEdit(TextCtlContext *pContext)
{
   if (pContext && pContext->bEditable) {
    if (sTextModes[pContext->byMode].pfn_exit) 
    {
        (*sTextModes[pContext->byMode].pfn_exit)(pContext); 
    }
      if (sTextModes[pContext->byMode].pfn_restart)
    {
        (*sTextModes[pContext->byMode].pfn_restart)(pContext);
    }
   }
}

/*=============================================================================
FUNCTION: TextCtl_NoSelection

DESCRIPTION:

PARAMETERS:
   *pContext:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_NoSelection(TextCtlContext *pContext)
{
   // This is a common behavior for the various text modes
   // Make sure the selection is an insertion point rather than
   // a selection.  If it is a selection, change it to an
   // insertion point at the right edge of the selection.

   if (pContext) {
      if (pContext->wSelStart != pContext->wSelEnd) {
         // Must change the selection
         pContext->wSelStart = pContext->wSelEnd;
         pContext->bSwap = FALSE;
         (void) TextCtl_AutoScroll(pContext);
         pContext->bNeedsDraw = TRUE;
      }
   }
}

static void TextCtl_DrawBackGround(TextCtlContext *pContext, AEERect *pRect)
{
    Appscommon_ResetBackground(pContext->pIDisplay, pContext->m_pImageBg, APPSCOMMON_BG_COLOR, pRect, 0, 0);
}

#ifdef FEATURE_T9_INPUT
/*------------------------------------------------------------------------
 *
 *  Function name	: SymbToAECHARNCopy
 *  Description	    : Copys a wide string to a character string
 *
 *  Pass            : *pdDisplay        - Buffer for the destination string
 *                    *psBuffer         - Source string
 *                     iLength           - Number of characters to copy
 *
 *  return          : int - Number of characters copied
 *
 *-----------------------------------------------------------------------*/
int SymbToAECHARNCopy(AECHAR *pdDisplay, T9SYMB *psBuffer, unsigned int iLength)
{
    unsigned int i;

        for (i = 0; i < iLength;i++) {
            *(pdDisplay + i) = *(psBuffer + i);
        }

    return i;
}

#ifdef FEATURE_T9_ALPHABETIC
/*------------------------------------------------------------------------
 *
 *  Function name   : T9TextCtl_EnglishRestart
 *  Description     : Restart English prediction mode.
 *
 *  Pass            : void
 *
 *  Return          : void
 *
 *-----------------------------------------------------------------------*/
static void T9TextCtl_Latin_Rapid_Restart(TextCtlContext *pContext)
{
    T9STATUS sT9Status = T9STATERROR;
    
    // Init Alphabet input
    T9_AW_Init ( pContext );

    switch ( OEM_TextGetCurrentMode((OEMCONTEXT)pContext) )
    {
#ifdef FEATURE_T9_RAPID_ARABIC
                     case TEXT_MODE_T9_RAPID_ARABIC:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDArabic, 0, 0 );   
                        break;
#endif //FEATURE_T9_RAPID_ARABIC

#ifdef FEATURE_T9_RAPID_HEBREW
                     case TEXT_MODE_T9_RAPID_HEBREW:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDHebrew, 0, 0 );   
                        break;    
#endif //FEATURE_T9_RAPID_HEBREW

#ifdef FEATURE_T9_RAPID_HINDI
                     case TEXT_MODE_T9_RAPID_HINDI:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDHindi, 0, 0 );   
                        break;        
#endif //FEATURE_T9_RAPID_HINDI

#ifdef FEATURE_T9_RAPID_THAI
                     case TEXT_MODE_T9_RAPID_THAI:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDThai, 0, 0 );                           
                        break;         
#endif //FEATURE_T9_RAPID_THAI

#ifdef FEATURE_T9_RAPID_SPANISH
                     case TEXT_MODE_T9_RAPID_SPANISH:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDSpanish, 0, 0 );     
                        break;       
#endif //FEATURE_T9_RAPID_SPANISH

#ifdef FEATURE_T9_RAPID_PORTUGUESE
                     case TEXT_MODE_T9_RAPID_PORTUGUESE:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDPortuguese, 0, 0 );             
                        break;         
#endif //FEATURE_T9_RAPID_PORTUGUESE

#ifdef FEATURE_T9_RAPID_INDONESIAN
                     case TEXT_MODE_T9_RAPID_INDONESIAN:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDIndonesian, 0, 0 );                       
                        break;         
#endif //FEATURE_T9_RAPID_INDONESIAN

#ifdef FEATURE_T9_RAPID_VIETNAMESE
                     case TEXT_MODE_T9_RAPID_VIETNAMESE:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDVietnamese, 0, 0 );    
                        break;           
#endif //FEATURE_T9_RAPID_VIETNAMESE

#ifdef FEATURE_T9_RAPID_FRENCH
                     case TEXT_MODE_T9_RAPID_FRENCH:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDFrench, 0, 0 );       
                        break;       
#endif //FEATURE_T9_RAPID_FRENCH       

#ifdef FEATURE_T9_RAPID_ENGLISH            
                    case TEXT_MODE_T9_RAPID_ENGLISH:
#endif //FEATURE_T9_RAPID_ENGLISH   
                    default:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                  T9PIDEnglish, 0, 0 );
                        break;
    }   
    
    //set to Ambiguous mode
    if ( T9STATNONE == sT9Status )
    {
        sT9Status = T9AWClrMultitapMode(&pContext->sT9awFieldInfo);
    }
    
    // re-setup Text Buffer
    if ( T9STATNONE == sT9Status )
    {
        sT9Status = T9ResetTextBuf(&pContext->sT9awFieldInfo.G, MAX_BUFFER_LENGTH);
    }
    if ( T9STATNONE == sT9Status )
    {        
        sT9Status = T9AddTextString ( &pContext->sT9awFieldInfo.G, 
                                      pContext->pszContents, 
                                      pContext->wContentsChars );
    }
    
    // set Cursor
    if ( T9STATNONE == sT9Status )
    {
        T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, pContext->wSelEnd );
    }
    
    TextCtl_NoSelection(pContext);
    TextCtl_TextChanged(pContext);
    pContext->sFocus = FOCUS_TEXT;
    
}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9TextCtl_Latin_Rapid_Key
 *  Description     : Handle English prediction Key.
 *
 *  Pass            : AVKType
 *
 *  Return          : boolean
 *
 *-----------------------------------------------------------------------*/
static boolean T9TextCtl_Latin_Rapid_Key(TextCtlContext *pContext, AEEEvent eCode,AVKType key)
{
    boolean  bRet       = FALSE;
    T9STATUS sT9Status = T9STATERROR;     
    T9KEY    t9Key;
    uint16   nBufLen   = pContext->sT9awFieldInfo.G.nBufLen;
    sFocusType sFocus = pContext->sFocus;     
    boolean bFlag = GetArrowFlagonIM();

#if defined (FEATURE_ALL_KEY_PAD)
	if(eCode == EVT_KEY_HELD)
		{
            int i;
            AECHAR ch = 0;
			switch(key){
			case AVK_0:
			case AVK_1:
			case AVK_2:
			case AVK_3:
			case AVK_4:
			case AVK_5:
			case AVK_6:
			case AVK_7:
			case AVK_8:
			case AVK_9:
            case AVK_POUND:
	        case AVK_STAR:
	        case AVK_T:
	        case AVK_Y:
	        case AVK_U:
	        case AVK_I:
	        case AVK_O:
	        case AVK_P:
	        case AVK_G:
	        case AVK_H:
	        case AVK_J:
	        case AVK_K:
	        case AVK_L:
	        case AVK_V:
	        case AVK_B:
	        case AVK_N:
	        case AVK_M:
                for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
    			{           
            		if (key == VLCharKeyItem[i].wParam)
            		{
                        ch = VLCharKeyItem[i].wp;
                        break;
            		}
                }
                break;
                
			default:
			    break;
			}
            
            if(ch != 0)
            {
                if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
				{
					/* Set selection to the character before the insertion point */
					--pContext->wSelStart;
				}
				else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
				{
					 return FALSE;
				}
		
				/* Insert a "NUL" to just delete and insert nothing */
				TextCtl_AddChar(pContext, 0);
				if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
				{
					// meet the max count of the text.
					if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
					{
						sT9Status = T9STATERROR; 
						return FALSE;
					}
				}
				TextCtl_NoSelection(pContext);
				TextCtl_AddChar(pContext,ch);
                return TRUE;
            }
		}
		else
		{
		    if(pContext->uModeInfo.mtap.kLast == AVK_UNDEFINED)
		    {
		        pContext->sFocus = FOCUS_TEXT;            
		    }

		    if(AVK_SELECT == key || AVK_INFO == key)
		    {
		        if(FOCUS_SELECTION == pContext->sFocus)
		        {       
		            t9Key = T9KEYRIGHT;
		        }
		        else
		        {
		            sT9Status = T9STATERROR;         
		            return FALSE;          
		        }
		    }     
		    switch ( key) 
		    {
		    	case AVK_0:
		        case AVK_1:
		        case AVK_2:
		        case AVK_3:
		        case AVK_4:
		        case AVK_5:
		        case AVK_6:
		        case AVK_7:
		        case AVK_8:
		        case AVK_9:
		        case AVK_POUND:
		        case AVK_STAR: 
		        case AVK_T: 
		        case AVK_Y:
		        case AVK_U:
		        case AVK_I:
		        case AVK_O:
		        case AVK_P:
		        case AVK_G:
		        case AVK_H:
		        case AVK_J:
		        case AVK_K:
		        case AVK_L:
		        case AVK_V: 
		        case AVK_B: 
		        case AVK_N:
		        case AVK_M:
		        case AVK_ENTER:
				case AVK_SPACE:
		        case AVK_RWD:
		            {
		                int i = 0;
		                if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
		                {
		                    // meet the max count of the text.
		                    if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
		                    {
		                        sT9Status = T9STATERROR; 
		                        return FALSE;
		                    }
		                }
		                for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
		    			{    
		    				switch(OEM_TextGetCurrentMode((OEMCONTEXT)pContext))
		    				{
		    				#ifdef FEATURE_T9_RAPID_ARABIC
		    					case TEXT_MODE_T9_RAPID_ARABIC:
									{
										if (key == VLCharArabicKeyItem[i].wParam)
					            		{
					            			TextCtl_NoSelection(pContext);
											TextCtl_AddChar(pContext,(AECHAR)(VLCharArabicKeyItem[i].wp));
										}
									}
									break;
							#endif
								default:
									{
					            		if (key == VLCharKeyItem[i].wParam)
					            		{
					        			    if(pContext->is_isShift)
					                        { 
					                            TextCtl_NoSelection(pContext);
					                            TextCtl_AddChar(pContext,(AECHAR)(VLCharKeyItem[i].wp));
					                            pContext->is_isShift = FALSE;
					                        }
					                        else
					                        {
					                            TextCtl_NoSelection(pContext);
												if(pContext->m_bCaplk)
												{
													TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
													pContext->m_bCaplk = FALSE;
												}
												else
												{
					                            	TextCtl_AddChar(pContext,(AECHAR)(VLCharLowKeyItem[i].wp));
												}
					                        }
					                     }
										break;
									}
		    				}
		                 }
		            }
		            return TRUE;
		            break;
		         case AVK_SHIFT:
		              {
		                 if(pContext->is_isShift)
		                 {
		                    pContext->is_isShift = FALSE;
		                 }
		                 else
		                 {
		                    pContext->is_isShift = TRUE;
		                 }
		              }
		              break;
				case AVK_CAPLK:
					{
						pContext->m_bCaplk = !pContext->m_bCaplk;
					}
					break;
		        case AVK_LEFT:
		            {
		            if(FOCUS_SELECTION == pContext->sFocus)
		            {
		                pContext->sFocus = FOCUS_TEXT;             
		                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYLEFT );  
		            }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
		            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
		            {
		                   uint16 wNewSel;
		                   wNewSel = pContext->wSelEnd + 1;
		                   if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
		                   {
		                       OEM_TextSetCursorPos(pContext, 0);
		                   }
		                   else 
		                   {  
		                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
		                   }
		            }
#else//change by xuhui
//#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
		            if (OEM_TextGetCursorPos(pContext) == 0)
		                {
		                    OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
		                }
		                else
		                {
		                    uint16 wNewSel;
		                    wNewSel = pContext->wSelStart;
		                    if (wNewSel)
		                    {
		                        --wNewSel;
		                    }               
#ifdef FEATURE_LANG_THAI
		                    {
		                        int count=0;
		                        count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
		                                                    pContext->pszContents[pContext->wSelStart-1]);
		                        if(count!= 0)
		                        {
		                            wNewSel = wNewSel - count;
		                        }
		                    }
#endif //FEATURE_LANG_THAI                 
		                OEM_TextSetSel(pContext, wNewSel, wNewSel);
		                    (void) TextCtl_AutoScroll(pContext);                        
		                }
#endif                  //change by xuhui      
		                return TRUE;
		            }            
		            break;     

		        case AVK_RIGHT:
		             {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
		            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
		                {
		                   uint16 wNewSel;
		                   wNewSel = pContext->wSelStart ;
		                   if ( OEM_TextGetCursorPos(pContext) == 0 )
		                   {
		                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
		                       //OEM_TextSetSel(pContext, 0, 0);
		                       OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
		                   }
		                   else 
		                   {  
		                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );  
		                       wNewSel --;          
		                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
		                   }
		                }
#else//change by xuhui
//#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
		             if (OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
		                {
		                    OEM_TextSetCursorPos(pContext, -1);
		                }                
		                else
		                {
		                    uint16 wNewSel;
		                    wNewSel = pContext->wSelEnd + 1;                  
#ifdef FEATURE_LANG_THAI  
		                    {
		                        int count=0;
		                        count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
		                                                     pContext->pszContents[pContext->wSelStart+1]);
		                        if(count!= 0)
		                        {
		                            wNewSel = wNewSel + count;
		                        }
		                    }
#endif //FEATURE_LANG_THAI                                
		                 OEM_TextSetSel(pContext, wNewSel, wNewSel);
		                    (void) TextCtl_AutoScroll(pContext);                          
		                }
#endif                        //change by xuhui
		                return TRUE;  
		            }
		            break;   

		         case AVK_UP:
		              {
		                uint16 nLine, nCharsIn,nSel;
		                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

		                // If it is on the first line, return false
		                if(nLine == 0 || !pContext->pwLineStarts)
		                    return FALSE;

		                // Otherwise figure out how many characters from the start
		                // of the line the cursor is and try to put the cursor in a
		                // similar position on previous line. Or, if not enough
		                // chars, at the end of the line
		                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
		                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
		                                               pContext->pwLineStarts[nLine]) 
		                {
		                    nSel = pContext->pwLineStarts[nLine]-1;
		                } 
		                else 
		                {
		                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
		                }
		                OEM_TextSetSel(pContext, nSel,nSel);
		                (void) TextCtl_AutoScroll(pContext);
		                return TRUE;
		            }            
		            break;   

		        case AVK_DOWN:
		             {
		                uint16 nLine, nCharsIn,nSel;

		                if((!pContext->pwLineStarts)||(!pContext->wLines))
		                    return FALSE;
		                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

		                // If the cursor is on the last line and the line's last
		                // character is not a LF, then FALSE is returned as nothing
		                // can be done. A LF on the end of a line does not tell the
		                // wLines member that there is another line, hence this
		                // extra check.
		                if ( nLine == (pContext->wLines-1) &&
		                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
		                {
		                    return FALSE;
		                }

		                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
		                // If the cursor is more characters in than the next line...
		                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
		                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
		                {
		                    // If it is the last line, don't subtract the LINEBREAK from selection spot
		                    if( nLine+2 == pContext->wLines )
		                    {
		                        nSel = pContext->pwLineStarts[nLine+2];
		                    }
		                    else
		                    {
		                        nSel = pContext->pwLineStarts[nLine+2]-1;
		                    }
		                }
		                else
		                {
		                    // Selection spot is number of chars into the next line
		                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
		                    // If this is not the beginning of a line 
		                    // and the selection point is a LINEBREAK, subtract one
		                    // Otherwise the selection overshoots to the first character
		                    // of the following line.
		                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
		                    {
		                        nSel--;
		                    }
		                }
		                OEM_TextSetSel(pContext, nSel,nSel);
		                (void) TextCtl_AutoScroll(pContext);

		                return TRUE;
		            }            
		            break;    

		            
		        case AVK_CLR:
		            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
		            {
		                 /* Set selection to the character before the insertion point */
		                 --pContext->wSelStart;
		            }
		            else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
		            {
		                  return FALSE;
		            }
		            
		            /* Insert a "NUL" to just delete and insert nothing */
		            TextCtl_AddChar(pContext, 0);
		            return TRUE;                

		        default:
		            pContext->sFocus = FOCUS_TEXT;   
		            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
		            break;  
		    }   
		}
#elif defined(FEATURE_DISP_220X176)
				if(eCode == EVT_KEY_HELD)
				{
					int i;
					AECHAR ch = 0;
					switch(key){
					case AVK_0:
					case AVK_1:
					case AVK_2:
					case AVK_3:
					case AVK_4:
					case AVK_5:
					case AVK_6:
					case AVK_7:
					case AVK_8:
					case AVK_9:
					case AVK_POUND:
					case AVK_STAR:
					case AVK_T:
					case AVK_Y:
					case AVK_U:
					case AVK_I:
					case AVK_O:
					case AVK_P:
					case AVK_G:
					case AVK_H:
					case AVK_J:
					case AVK_K:
					case AVK_L:
					case AVK_V:
					case AVK_B:
					case AVK_N:
					case AVK_M:
						for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
						{			
							if (key == VLCharKeyItem[i].wParam)
							{
								ch = VLCharKeyItem[i].wp;
								break;
							}
						}
						break;
						
					default:
						break;
					}
					
					if(ch != 0)
					{
						if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
						{
							/* Set selection to the character before the insertion point */
							--pContext->wSelStart;
						}
						else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
						{
							 return FALSE;
						}
				
						/* Insert a "NUL" to just delete and insert nothing */
						TextCtl_AddChar(pContext, 0);
						if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
						{
							// meet the max count of the text.
							if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
							{
								sT9Status = T9STATERROR; 
								return FALSE;
							}
						}
						TextCtl_NoSelection(pContext);
						TextCtl_AddChar(pContext,ch);
						return TRUE;
					}
				}
				else
				{
					if(pContext->uModeInfo.mtap.kLast == AVK_UNDEFINED)
					{
						pContext->sFocus = FOCUS_TEXT;			  
					}
		
					if(AVK_SELECT == key || AVK_INFO == key)
					{
						if(FOCUS_SELECTION == pContext->sFocus)
						{		
							t9Key = T9KEYRIGHT;
						}
						else
						{
							sT9Status = T9STATERROR;		 
							return FALSE;		   
						}
					}	  
					switch ( key) 
					{
						case AVK_0:
						case AVK_1:
						case AVK_2:
						case AVK_3:
						case AVK_4:
						case AVK_5:
						case AVK_6:
						case AVK_7:
						case AVK_8:
						case AVK_9:
						case AVK_POUND:
						case AVK_STAR: 
						case AVK_T: 
						case AVK_Y:
						case AVK_U:
						case AVK_I:
						case AVK_O:
						case AVK_P:
						case AVK_G:
						case AVK_H:
						case AVK_J:
						case AVK_K:
						case AVK_L:
						case AVK_V: 
						case AVK_B: 
						case AVK_N:
						case AVK_M:
						case AVK_ENTER:
						case AVK_SPACE:
						case AVK_RWD:
							{
								int i = 0;
								if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
								{
									// meet the max count of the text.
									if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
									{
										sT9Status = T9STATERROR; 
										return FALSE;
									}
								}
								for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
								{			
									if (key == VLCharKeyItem[i].wParam)
									{
										if(pContext->is_isShift)
										{ 
											TextCtl_NoSelection(pContext);
											TextCtl_AddChar(pContext,(AECHAR)(VLCharKeyItem[i].wp));
											pContext->is_isShift = FALSE;
										}
										else
										{
											TextCtl_NoSelection(pContext);
											if(pContext->m_bCaplk)
											{
												TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
												pContext->m_bCaplk = FALSE;
											}
											else
											{
												TextCtl_AddChar(pContext,(AECHAR)(VLCharLowKeyItem[i].wp));
											}
										}
									 }
								  }
							}
							return TRUE;
							break;
						 case AVK_SHIFT:
							  {
								 if(pContext->is_isShift)
								 {
									pContext->is_isShift = FALSE;
								 }
								 else
								 {
									pContext->is_isShift = TRUE;
								 }
							  }
							  break;
						case AVK_CAPLK:
							{
								pContext->m_bCaplk = !pContext->m_bCaplk;
							}
							break;
						case AVK_LEFT:
							{
							if(FOCUS_SELECTION == pContext->sFocus)
							{
								pContext->sFocus = FOCUS_TEXT;			   
								sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYLEFT );  
							}
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
							if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
							{
								   uint16 wNewSel;
								   wNewSel = pContext->wSelEnd + 1;
								   if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
								   {
									   OEM_TextSetCursorPos(pContext, 0);
								   }
								   else 
								   {  
									   OEM_TextSetSel(pContext, wNewSel, wNewSel);						 
								   }
							}
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
							if (OEM_TextGetCursorPos(pContext) == 0)
								{
									OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
								}
								else
								{
									uint16 wNewSel;
									wNewSel = pContext->wSelStart;
									if (wNewSel)
									{
										--wNewSel;
									}				
#ifdef FEATURE_LANG_THAI
									{
										int count=0;
										count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
																	pContext->pszContents[pContext->wSelStart-1]);
										if(count!= 0)
										{
											wNewSel = wNewSel - count;
										}
									}
#endif //FEATURE_LANG_THAI                   
								OEM_TextSetSel(pContext, wNewSel, wNewSel);
									(void) TextCtl_AutoScroll(pContext);
								}
								return TRUE;
							}			 
							break;	   
		
						case AVK_RIGHT:
							 {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
							if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
								{
								   uint16 wNewSel;
								   wNewSel = pContext->wSelStart ;
								   if ( OEM_TextGetCursorPos(pContext) == 0 )
								   {
									   //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
									   //OEM_TextSetSel(pContext, 0, 0);
									   OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
								   }
								   else 
								   {  
									   //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );	
									   wNewSel --;			
									   OEM_TextSetSel(pContext, wNewSel, wNewSel);						 
								   }
								}
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
							 if (OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
								{
									OEM_TextSetCursorPos(pContext, -1);
								}				 
								else
								{
									uint16 wNewSel;
									wNewSel = pContext->wSelEnd + 1;				  
#ifdef FEATURE_LANG_THAI  
									{
										int count=0;
										count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
																	 pContext->pszContents[pContext->wSelStart+1]);
										if(count!= 0)
										{
											wNewSel = wNewSel + count;
										}
									}
#endif //FEATURE_LANG_THAI                                
								 OEM_TextSetSel(pContext, wNewSel, wNewSel);
									(void) TextCtl_AutoScroll(pContext);
								}
								return TRUE;  
							}
							break;	 
		
						 case AVK_UP:
							  {
								uint16 nLine, nCharsIn,nSel;
								nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);
		
								// If it is on the first line, return false
								if(nLine == 0 || !pContext->pwLineStarts)
									return FALSE;
		
								// Otherwise figure out how many characters from the start
								// of the line the cursor is and try to put the cursor in a
								// similar position on previous line. Or, if not enough
								// chars, at the end of the line
								nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
								if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
															   pContext->pwLineStarts[nLine]) 
								{
									nSel = pContext->pwLineStarts[nLine]-1;
								} 
								else 
								{
									nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
								}
								OEM_TextSetSel(pContext, nSel,nSel);
								(void) TextCtl_AutoScroll(pContext);
								return TRUE;
							}			 
							break;	 
		
						case AVK_DOWN:
							 {
								uint16 nLine, nCharsIn,nSel;
		
								if((!pContext->pwLineStarts)||(!pContext->wLines))
									return FALSE;
								nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);
		
								// If the cursor is on the last line and the line's last
								// character is not a LF, then FALSE is returned as nothing
								// can be done. A LF on the end of a line does not tell the
								// wLines member that there is another line, hence this
								// extra check.
								if ( nLine == (pContext->wLines-1) &&
									pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
								{
									return FALSE;
								}
		
								nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
								// If the cursor is more characters in than the next line...
								// This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
								if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
								{
									// If it is the last line, don't subtract the LINEBREAK from selection spot
									if( nLine+2 == pContext->wLines )
									{
										nSel = pContext->pwLineStarts[nLine+2];
									}
									else
									{
										nSel = pContext->pwLineStarts[nLine+2]-1;
									}
								}
								else
								{
									// Selection spot is number of chars into the next line
									nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
									// If this is not the beginning of a line 
									// and the selection point is a LINEBREAK, subtract one
									// Otherwise the selection overshoots to the first character
									// of the following line.
									if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
									{
										nSel--;
									}
								}
								OEM_TextSetSel(pContext, nSel,nSel);
								(void) TextCtl_AutoScroll(pContext);
		
								return TRUE;
							}			 
							break;	  
		
							
						case AVK_CLR:
							if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
							{
								 /* Set selection to the character before the insertion point */
								 --pContext->wSelStart;
							}
							else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
							{
								  return FALSE;
							}
							
							/* Insert a "NUL" to just delete and insert nothing */
							TextCtl_AddChar(pContext, 0);
							return TRUE;				
		
						default:
							pContext->sFocus = FOCUS_TEXT;	 
							sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
							break;	
					}	
				}
#else
	//handle key
    t9Key     = T9_BrewKeyToT9AlphabeticKey (pContext,eCode, key );
    if(key == AVK_SELECT|| key == AVK_INFO)
    {
        if(FOCUS_SELECTION == pContext->sFocus)
        {       
            t9Key = T9KEYSPACE;	  
        }
        else
        {
            sT9Status = T9STATERROR;         
            return FALSE;          
        }
    }
            
    if ( pContext->wMaxChars != 0 && 
         nBufLen >= pContext->wMaxChars &&
         (( t9Key >= T9KEYAMBIG1 && t9Key <= T9KEYAMBIGC) || 
         ((FOCUS_TEXT == pContext->sFocus)  && (T9KEYSPACE == t9Key)) ) )
    { 
        // meet the max count of the text.
        sT9Status = T9STATERROR; 
        return FALSE;       
    }
    
    switch ( t9Key) 
    {
        case T9KEYAMBIG1:
        case T9KEYAMBIG2:
        case T9KEYAMBIG3:
        case T9KEYAMBIG4:
        case T9KEYAMBIG5:
        case T9KEYAMBIG6:
        case T9KEYAMBIG7:
        case T9KEYAMBIG8:
        case T9KEYAMBIG9:
        case T9KEYAMBIGA:
        case T9KEYAMBIGB: 
        case T9KEYAMBIGC: 
             if (TEXT_MODE_T9_RAPID_ENGLISH== OEM_TextGetCurrentMode(pContext)
                 && MULTITAP_FIRST_CAP == pContext->nMultitapCaps
                 && !OEM_isFirstCap(pContext)
                 && FOCUS_TEXT == pContext->sFocus)
            {
               pContext->nMultitapCaps = MULTITAP_ALL_SMALL;
            } 
#ifdef FEATURE_T9_RAPID_SPANISH
		 if (TEXT_MODE_T9_RAPID_SPANISH== OEM_TextGetCurrentMode(pContext)
                 && MULTITAP_FIRST_CAP == pContext->nMultitapCaps
                 && !OEM_isFirstCap(pContext)
                 && FOCUS_TEXT == pContext->sFocus)
            {
               pContext->nMultitapCaps = MULTITAP_ALL_SMALL;
            } 
#endif
            pContext->sFocus = FOCUS_SELECTION;

            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );             
           
            break;        

        case T9KEYLEFT:
            if(FOCUS_SELECTION == pContext->sFocus)
            {   
                pContext->sFocus = FOCUS_TEXT;               
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key ); 
            }
            //bw: <<for Hebrew
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
            else if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags && FOCUS_TEXT == pContext->sFocus )
            {
                if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
                }
                else 
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_MOVERIGHT, 1 );                   
                }
            }
            // bw:>>
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
            else if(FOCUS_TEXT == pContext->sFocus)
            {
                if(OEM_TextGetCursorPos(pContext) == 0)
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMEND, 0 ); 
                }
                else
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_MOVELEFT, 1 );                
#ifdef FEATURE_LANG_THAI   
                    {
                        int count=0;
                        count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
                                                    pContext->pszContents[pContext->wSelStart-1]);
                        if(count!= 0)
                        {
                            sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_MOVELEFT, count );  
                        }
                    }
#endif //FEATURE_LANG_THAI                   
                }             
            }
            break;

        case T9KEYRIGHT:
            if(FOCUS_SELECTION == pContext->sFocus)
            {   
                pContext->sFocus = FOCUS_TEXT;             
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key);
            }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
            else if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags && FOCUS_TEXT == pContext->sFocus )
            {
                if ( OEM_TextGetCursorPos(pContext) == 0 )
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMEND, 0 );
                }
                else 
                {  
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVELEFT, 1 );                   
                                             
                }
            }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
            else if(FOCUS_TEXT == pContext->sFocus)
            {
                if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
                }
                else 
                {  
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_MOVERIGHT, 1 );                   
#ifdef FEATURE_LANG_THAI                 
                    {
                        int count=0;
                        count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
                                                     pContext->pszContents[pContext->wSelStart+1]);
                        if(count!= 0)
                        {
                            sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_MOVERIGHT, count );  
                        }
                    }
#endif //FEATURE_LANG_THAI                                               
                }
            }
            break;

         case T9KEYPREV:
            if(FOCUS_SELECTION == pContext->sFocus)
            {            
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key);   
            }
            else if(FOCUS_TEXT == pContext->sFocus)
            {
                uint16 nLine, nCharsIn,nSel,lineChars;
                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);
        
                // If it is on the first line, return false
                if(nLine == 0 || !pContext->pwLineStarts)
                    return FALSE;
        
                // Otherwise figure out how many characters from the start
                // of the line the cursor is and try to put the cursor in a
                // similar position on previous line. Or, if not enough
                // chars, at the end of the line
                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
                                               pContext->pwLineStarts[nLine]) 
                {
                    nSel = pContext->pwLineStarts[nLine]-1;
                } 
                else 
                {
                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
                }
                lineChars = pContext->wSelEnd - nSel;
                sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_MOVELEFT, lineChars );                            
            }             
            break;

        case T9KEYNEXT:
            if(FOCUS_SELECTION == pContext->sFocus)
            {            
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key);  
            }
            else if(FOCUS_TEXT == pContext->sFocus)
            {
                uint16 nLine, nCharsIn,nSel,lineChars;
            
                if((!pContext->pwLineStarts)||(!pContext->wLines))
                    return FALSE;
                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);
            
                // If the cursor is on the last line and the line's last
                // character is not a LF, then FALSE is returned as nothing
                // can be done. A LF on the end of a line does not tell the
                // wLines member that there is another line, hence this
                // extra check.
                if ( nLine == (pContext->wLines-1) &&
                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
                {
                    return FALSE;
                }
            
                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
                // If the cursor is more characters in than the next line...
                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
                {
                    // If it is the last line, don't subtract the LINEBREAK from selection spot
                    if( nLine+2 == pContext->wLines )
                    {
                        nSel = pContext->pwLineStarts[nLine+2];
                    }
                    else
                    {
                        nSel = pContext->pwLineStarts[nLine+2]-1;
                    }
                }
                else
                {
                    // Selection spot is number of chars into the next line
                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
                    // If this is not the beginning of a line 
                    // and the selection point is a LINEBREAK, subtract one
                    // Otherwise the selection overshoots to the first character
                    // of the following line.
                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
                    {
                        nSel--;
                    }
                }
                lineChars = nSel - pContext->wSelEnd;
                sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_MOVERIGHT, lineChars );                                 
            }          
            break; 
            
        case T9KEYCLEAR:
            if ((0 == pContext->wSelStart) && (pContext->wSelStart == pContext->wSelEnd))
            {
                return FALSE;
            }
            else
            {
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key ); 
            }
            break;
            
        case T9KEYSPACE:  
            if((FOCUS_SELECTION == pContext->sFocus)&&
                (pContext->wMaxChars == nBufLen))
            {
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYRIGHT );  
            }
            else
            {         
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
            }
             pContext->sFocus = FOCUS_TEXT;  
            break;  

        default: 
            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
            break;  
    }  
    if( pContext->sT9awFieldInfo.G.nCurSelObj == 0)
    {
       bFlag =  FALSE;
    } else if( !(T9KEYPREV == t9Key  || T9KEYNEXT == t9Key) && 
                    (FOCUS_SELECTION == sFocus  || FOCUS_SELECTION == pContext->sFocus) )
    {        
       T9HandleKey ( &pContext->sT9awFieldInfo.G,  T9KEYNEXT); 
        if( pContext->sT9awFieldInfo.G.nCurSelObj > 1)
        {
            T9HandleKey ( &pContext->sT9awFieldInfo.G,  T9KEYPREV);
            bFlag = TRUE;
        }else{
            bFlag = FALSE; }
    }
    if(bFlag != GetArrowFlagonIM() )
    {
        SetArrowFlagonIM( bFlag);
        ISHELL_HandleEvent(pContext->pIShell, EVT_UPDATE_ARROW_BUTTON, 0, 0);	                
    }

    //display strings
    //if ( T9STATNONE == sT9Status )
    {          
        bRet = T9_AW_DisplayText ( pContext, key);  
    }
#endif


    return bRet;
}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9TextCtl_Latin_Rapid_Exit
 *  Description     : Exit English prediction mode.
 *
 *  Pass            : void
 *
 *  Return          : void
 *
 *-----------------------------------------------------------------------*/
static void T9TextCtl_Latin_Rapid_Exit(TextCtlContext *pContext)
{
    T9_AW_Destroy ( pContext );
    TextCtl_NoSelection(pContext);
	pContext->m_bCaplk = FALSE;
}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9TextCtl_MultitapRestart
 *  Description     : set to Multitap mode.
 *
 *  Pass            : void
 *
 *  Return          : void
 *
 *-----------------------------------------------------------------------*/
static void T9TextCtl_MultitapRestart(TextCtlContext *pContext)
{
    T9STATUS sT9Status = T9STATERROR;
    
    // Init Alphabet input
    T9_AW_Init ( pContext );

    switch ( OEM_TextGetCurrentMode((OEMCONTEXT)pContext) )
    {
#ifdef FEATURE_T9_MT_ARABIC
                     case TEXT_MODE_T9_MT_ARABIC:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDArabic, 0, 0 );   
                        break;
#endif //FEATURE_T9_MT_ARABIC

#ifdef FEATURE_T9_MT_HEBREW
                     case TEXT_MODE_T9_MT_HEBREW:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDHebrew, 0, 0 );   
                        break;    
#endif //FEATURE_T9_MT_HEBREW

#ifdef FEATURE_T9_MT_HINDI
                     case TEXT_MODE_T9_MT_HINDI:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDHindi, 0, 0 );   
                        break;        
#endif //FEATURE_T9_MT_HINDI

#ifdef FEATURE_T9_MT_THAI
                     case TEXT_MODE_T9_MT_THAI:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDThai, 0, 0 );                           
                        break;         
#endif //FEATURE_T9_MT_THAI

#ifdef FEATURE_T9_MT_SPANISH
                     case TEXT_MODE_T9_MT_SPANISH:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDSpanish, 0, 0 );     
                        break;       
#endif //FEATURE_T9_MT_SPANISH

#ifdef FEATURE_T9_MT_PORTUGUESE
                     case TEXT_MODE_T9_MT_PORTUGUESE:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDPortuguese, 0, 0 );             
                        break;         
#endif //FEATURE_T9_MT_PORTUGUESE

#ifdef FEATURE_T9_MT_INDONESIAN
                     case TEXT_MODE_T9_MT_INDONESIAN:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDIndonesian, 0, 0 );                       
                        break;         
#endif //FEATURE_T9_MT_INDONESIAN

#ifdef FEATURE_T9_MT_VIETNAMESE
                     case TEXT_MODE_T9_MT_VIETNAMESE:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDVietnamese, 0, 0 );    
                        break;           
#endif //FEATURE_T9_MT_VIETNAMESE

#ifdef FEATURE_T9_MT_FRENCH
                     case TEXT_MODE_T9_MT_FRENCH:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                      T9PIDFrench, 0, 0 );       
                        break;       
#endif //FEATURE_T9_MT_FRENCH       

#ifdef FEATURE_T9_MT_ENGLISH            
                    case TEXT_MODE_MULTITAP:
#endif //FEATURE_T9_MT_ENGLISH   
                    default:
                        sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                                  T9PIDEnglish, 0, 0 );
                        break;
    }   
    
    //set to Ambiguous mode
    if ( T9STATNONE == sT9Status )
    {
        //set to Multitap mode
        sT9Status = T9AWSetMultitapMode(&pContext->sT9awFieldInfo);
    }
    
    // re-setup Text Buffer
    if ( T9STATNONE == sT9Status )
    {
        sT9Status = T9ResetTextBuf(&pContext->sT9awFieldInfo.G, MAX_BUFFER_LENGTH);
    }
    if ( T9STATNONE == sT9Status )
    {        
        sT9Status = T9AddTextString ( &pContext->sT9awFieldInfo.G, 
                                      pContext->pszContents, 
                                      pContext->wContentsChars );
    }
    
    // set Cursor
    if ( T9STATNONE == sT9Status )
    {
        T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, pContext->wSelEnd );
    }
    
    TextCtl_NoSelection(pContext);
    pContext->uModeInfo.mtap.kLast = AVK_FIRST;  
    TextCtl_TextChanged(pContext);
    pContext->sFocus = FOCUS_TEXT;

}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9TextCtl_MultitapKey
 *  Description     : handle Multitap Key.
 *
 *  Pass            : AVKType key
 *
 *  Return          : boolean
 *
 *-----------------------------------------------------------------------*/
static boolean T9TextCtl_MultitapKey(TextCtlContext *pContext,AEEEvent eCode, AVKType key)
{
    boolean  bRet       = FALSE;
    T9STATUS sT9Status = T9STATERROR;  
    T9KEY    t9Key;
    uint16   nBufLen = pContext->sT9awFieldInfo.G.nBufLen;
    ERR("T9TextCtl_MultitapKey::start",0,0,0);

#if defined (FEATURE_ALL_KEY_PAD)
	if(eCode == EVT_KEY_HELD)
    {
        int i;
        AECHAR ch = 0;
        switch(key){
        case AVK_0:
        case AVK_1:
        case AVK_2:
        case AVK_3:
        case AVK_4:
        case AVK_5:
        case AVK_6:
        case AVK_7:
        case AVK_8:
        case AVK_9:
        case AVK_POUND:
        case AVK_STAR:
        case AVK_T:
        case AVK_Y:
        case AVK_U:
        case AVK_I:
        case AVK_O:
        case AVK_P:
        case AVK_G:
        case AVK_H:
        case AVK_J:
        case AVK_K:
        case AVK_L:
        case AVK_V:
        case AVK_B:
        case AVK_N:
        case AVK_M:
            for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
            {           
                if (key == VLCharKeyItem[i].wParam)
                {
                    ch = VLCharKeyItem[i].wp;
                    break;
                }
            }
            break;
            
        default:
            break;
        }
        
        if(ch != 0)
        {
            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
            {
                /* Set selection to the character before the insertion point */
                --pContext->wSelStart;
            }
            else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
            {
                 return FALSE;
            }
    
            /* Insert a "NUL" to just delete and insert nothing */
            TextCtl_AddChar(pContext, 0);
            if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
            {
                // meet the max count of the text.
                if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
                {
                    sT9Status = T9STATERROR; 
                    return FALSE;
                }
            }
            TextCtl_NoSelection(pContext);
            TextCtl_AddChar(pContext,ch);
            return TRUE;
        }
    }
	else
	{
	    if(pContext->uModeInfo.mtap.kLast == AVK_UNDEFINED)
	    {
	        pContext->sFocus = FOCUS_TEXT;            
	    }

	    if(AVK_SELECT == key || AVK_INFO == key)
	    {
	        if(FOCUS_SELECTION == pContext->sFocus)
	        {       
	            t9Key = T9KEYRIGHT;
	        }
	        else
	        {
	            sT9Status = T9STATERROR;         
	            return FALSE;          
	        }
	    }     
		
	    switch ( key) 
	    {
	    	case AVK_0:
	        case AVK_1:
	        case AVK_2:
	        case AVK_3:
	        case AVK_4:
	        case AVK_5:
	        case AVK_6:
	        case AVK_7:
	        case AVK_8:
	        case AVK_9:
	        case AVK_POUND:
	        case AVK_STAR: 
	        case AVK_T: 
	        case AVK_Y:
	        case AVK_U:
	        case AVK_I:
	        case AVK_O:
	        case AVK_P:
	        case AVK_G:
	        case AVK_H:
	        case AVK_J:
	        case AVK_K:
	        case AVK_L:
	        case AVK_V: 
	        case AVK_B: 
	        case AVK_N:
	        case AVK_M:
	        case AVK_ENTER:
			case AVK_SPACE:
	        case AVK_RWD:
	            {
	                int i = 0;
	                if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
	                {
	                    // meet the max count of the text.
	                    if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
	                    {
	                        sT9Status = T9STATERROR; 
	                        return FALSE;
	                    }
	                }
	                for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
	    			{       
	            		if (key == VLCharKeyItem[i].wParam)
	            		{
	        			    if(pContext->is_isShift)
	                        { 
	                            TextCtl_NoSelection(pContext);
	                            TextCtl_AddChar(pContext,(AECHAR)(VLCharKeyItem[i].wp));
	                            pContext->is_isShift = FALSE;
	                        }
	                        else
	                        {
	                            TextCtl_NoSelection(pContext);
								if(pContext->m_bCaplk)
								{
									TextCtl_AddChar(pContext,(AECHAR)(VLCharLowKeyItem[i].wp));
									pContext->m_bCaplk = FALSE;
								}
								else
								{
	                            	TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
								}
	                        }
	                     }
	                  }
	            }
	            return TRUE;
	            break;
	         case AVK_SHIFT:
	              {
	                 if(pContext->is_isShift)
	                 {
	                    pContext->is_isShift = FALSE;
	                 }
	                 else
	                 {
	                    pContext->is_isShift = TRUE;
	                 }
	              }
	              break;
	        case AVK_CAPLK:
	             {
				 	
	                pContext->m_bCaplk = !pContext->m_bCaplk;
	             }
	             return TRUE;
	             break;
	        case AVK_LEFT:
	            {
	            if(FOCUS_SELECTION == pContext->sFocus)
	            {
	                pContext->sFocus = FOCUS_TEXT;             
	                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYLEFT );  
	            }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	            {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelEnd + 1;
	                   if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                   {
	                       OEM_TextSetCursorPos(pContext, 0);
	                   }
	                   else 
	                   {  
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	            }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if (OEM_TextGetCursorPos(pContext) == 0)
	                {
	                    OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                }
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelStart;
	                    if (wNewSel)
	                    {
	                        --wNewSel;
	                    }               
#ifdef FEATURE_LANG_THAI
	                    {
	                        int count=0;
	                        count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
	                                                    pContext->pszContents[pContext->wSelStart-1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel - count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI     
	                OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;
	            }            
	            break;     

	        case AVK_RIGHT:
	             {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	                {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelStart ;
	                   if ( OEM_TextGetCursorPos(pContext) == 0 )
	                   {
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
	                       //OEM_TextSetSel(pContext, 0, 0);
	                       OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                   }
	                   else 
	                   {  
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );  
	                       wNewSel --;         
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	                }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	             if (OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                {
	                    OEM_TextSetCursorPos(pContext, -1);
	                }                
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelEnd + 1;                  
#ifdef FEATURE_LANG_THAI  
	                    {
	                        int count=0;
	                        count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
	                                                     pContext->pszContents[pContext->wSelStart+1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel + count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI         
	                 OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;  
	            }
	            break;   

	         case AVK_UP:
	              {
	                uint16 nLine, nCharsIn,nSel;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If it is on the first line, return false
	                if(nLine == 0 || !pContext->pwLineStarts)
	                    return FALSE;

	                // Otherwise figure out how many characters from the start
	                // of the line the cursor is and try to put the cursor in a
	                // similar position on previous line. Or, if not enough
	                // chars, at the end of the line
	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
	                                               pContext->pwLineStarts[nLine]) 
	                {
	                    nSel = pContext->pwLineStarts[nLine]-1;
	                } 
	                else 
	                {
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);
	                return TRUE;
	            }            
	            break;   

	        case AVK_DOWN:
	             {
	                uint16 nLine, nCharsIn,nSel;

	                if((!pContext->pwLineStarts)||(!pContext->wLines))
	                    return FALSE;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If the cursor is on the last line and the line's last
	                // character is not a LF, then FALSE is returned as nothing
	                // can be done. A LF on the end of a line does not tell the
	                // wLines member that there is another line, hence this
	                // extra check.
	                if ( nLine == (pContext->wLines-1) &&
	                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
	                {
	                    return FALSE;
	                }

	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                // If the cursor is more characters in than the next line...
	                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
	                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
	                {
	                    // If it is the last line, don't subtract the LINEBREAK from selection spot
	                    if( nLine+2 == pContext->wLines )
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2];
	                    }
	                    else
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2]-1;
	                    }
	                }
	                else
	                {
	                    // Selection spot is number of chars into the next line
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
	                    // If this is not the beginning of a line 
	                    // and the selection point is a LINEBREAK, subtract one
	                    // Otherwise the selection overshoots to the first character
	                    // of the following line.
	                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
	                    {
	                        nSel--;
	                    }
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);

	                return TRUE;
	            }            
	            break;    

	            
	        case AVK_CLR:
	            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
	            {
	                 /* Set selection to the character before the insertion point */
	                 --pContext->wSelStart;
	            }
	            else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
	            {
	                  return FALSE;
	            }
	            
	            /* Insert a "NUL" to just delete and insert nothing */
	            TextCtl_AddChar(pContext, 0);
	            return TRUE;                

	        default:
	            pContext->sFocus = FOCUS_TEXT;   
	            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
	            break;  
	    }   
	}
#else
	ERR("T9TextCtl_MultitapKey::1",0,0,0);
    t9Key     = T9_BrewKeyToT9AlphabeticKey (pContext, eCode,key );
    
    if(pContext->uModeInfo.mtap.kLast == AVK_UNDEFINED)
    {
        pContext->sFocus = FOCUS_TEXT;            
    }

    if(AVK_SELECT == key || AVK_INFO == key)
    {
        if(FOCUS_SELECTION == pContext->sFocus)
        {       
            t9Key = T9KEYRIGHT;
        }
        else
        {
            sT9Status = T9STATERROR;         
            return FALSE;          
        }
    }    
    ERR("T9TextCtl_MultitapKey::2",0,0,0);
    if ( pContext->wMaxChars != 0 && 
         nBufLen >= pContext->wMaxChars &&
         (( t9Key >= T9KEYAMBIG1 && t9Key <= T9KEYAMBIGC) || T9KEYSPACE == t9Key ) )
    { 
        // meet the max count of the text.
        if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
        {
            sT9Status = T9STATERROR; 
            return FALSE;
        }
    }    
    ERR("T9TextCtl_MultitapKey::3",0,0,0);
    switch ( t9Key) 
    {
        case T9KEYAMBIG1:
        case T9KEYAMBIG2:
        case T9KEYAMBIG3:
        case T9KEYAMBIG4:
        case T9KEYAMBIG5:
        case T9KEYAMBIG6:
        case T9KEYAMBIG7:
        case T9KEYAMBIG8:
        case T9KEYAMBIG9:
        case T9KEYAMBIGA:
        case T9KEYAMBIGB: 
        case T9KEYAMBIGC: 
            ERR("T9TextCtl_MultitapKey::4",0,0,0);
            pContext->sFocus = FOCUS_SELECTION;              
            if(pContext->uModeInfo.mtap.kLast == AVK_UNDEFINED)
            { 
                T9TimeOut(&pContext->sT9awFieldInfo.G, 1);         
                pContext->uModeInfo.mtap.nSubChar = 0;                 
            } 
            else if(pContext->uModeInfo.mtap.kLast != key)
            {
                // when key != kLast    
                pContext->uModeInfo.mtap.nSubChar = 0;         
                pContext->wSelStart = pContext->sT9awFieldInfo.G.nCursor;      
                if (pContext->uModeInfo.mtap.kLast >= AVK_0 
                     && pContext->uModeInfo.mtap.kLast <= AVK_9 
                     //&& TEXT_MODE_MULTITAP == OEM_TextGetCurrentMode(pContext)
                     && (TEXT_MODE_MULTITAP == OEM_TextGetCurrentMode(pContext)
#ifdef FEATURE_T9_MT_SPANISH
                     ||TEXT_MODE_T9_MT_SPANISH== OEM_TextGetCurrentMode(pContext)
#endif
#ifdef FEATURE_T9_MT_FRENCH
                     ||TEXT_MODE_T9_MT_FRENCH== OEM_TextGetCurrentMode(pContext)
#endif

                      )
                     && MULTITAP_FIRST_CAP == pContext->nMultitapCaps
                     && !OEM_isFirstCap(pContext))
                {
                   pContext->nMultitapCaps = MULTITAP_ALL_SMALL;
                }                                        
            } 
            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );              
            break;        

        case T9KEYLEFT:
            ERR("T9TextCtl_MultitapKey::5",0,0,0);
            if(FOCUS_SELECTION == pContext->sFocus)
            {
                pContext->sFocus = FOCUS_TEXT;             
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYLEFT );  
            }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
            else if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags && FOCUS_TEXT == pContext->sFocus )
            {
                if ( OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents) )
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
                }
                else 
                {  
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );                   
                }
            }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
            else if(FOCUS_TEXT == pContext->sFocus)
            {
                if(OEM_TextGetCursorPos(pContext) == 0)
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMEND, 0 ); 
                }
                else
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_MOVELEFT, 1 );                
#ifdef FEATURE_LANG_THAI
                    {
                        int count=0;
                        count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
                                                    pContext->pszContents[pContext->wSelStart-1]);
                        if(count!= 0)
                        {
                            sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_MOVELEFT, count );  
                        }
                    }
#endif //FEATURE_LANG_THAI                   
                }             
            }
            break;

        case T9KEYRIGHT:
            ERR("T9TextCtl_MultitapKey::6",0,0,0);
            if(FOCUS_SELECTION == pContext->sFocus)
            {
                pContext->sFocus = FOCUS_TEXT;             
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYRIGHT); 
            }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
            else if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags && FOCUS_TEXT == pContext->sFocus )
            {
                if ( OEM_TextGetCursorPos(pContext) == 0 )
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMEND, 0 );
                }
                else 
                {  
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVELEFT, 1 );                   
                }
            }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
            else if(FOCUS_TEXT == pContext->sFocus)
            {
                if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
                {
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
                }
                else 
                {  
                    sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );                   
#ifdef FEATURE_LANG_THAI  
                    {
                        int count=0;
                        count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
                                                     pContext->pszContents[pContext->wSelStart+1]);
                        if(count!= 0)
                        {
                            sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, count );  
                        }
                    }
#endif //FEATURE_LANG_THAI                                               
                }

            }
            break;

         case T9KEYPREV:
            ERR("T9TextCtl_MultitapKey::7",0,0,0);
            if(FOCUS_SELECTION == pContext->sFocus)
            {
                pContext->sFocus = FOCUS_TEXT;             
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYRIGHT);   
            }
            else if(FOCUS_TEXT == pContext->sFocus)
            {
                uint16 nLine, nCharsIn,nSel,lineChars;
                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);
        
                // If it is on the first line, return false
                if(nLine == 0 || !pContext->pwLineStarts)
                    return FALSE;
        
                // Otherwise figure out how many characters from the start
                // of the line the cursor is and try to put the cursor in a
                // similar position on previous line. Or, if not enough
                // chars, at the end of the line
                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
                                               pContext->pwLineStarts[nLine]) 
                {
                    nSel = pContext->pwLineStarts[nLine]-1;
                } 
                else 
                {
                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
                }
                lineChars = pContext->wSelEnd - nSel;
                sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, 
                            T9CA_MOVELEFT, 
                            lineChars );                            
            }            
            break;

        case T9KEYNEXT:
            ERR("T9TextCtl_MultitapKey::8",0,0,0);
            if(FOCUS_SELECTION == pContext->sFocus)
            {
                pContext->sFocus = FOCUS_TEXT;             
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYRIGHT);  
            }
            else if(FOCUS_TEXT == pContext->sFocus)
            {
                uint16 nLine, nCharsIn,nSel,lineChars;
            
                if((!pContext->pwLineStarts)||(!pContext->wLines))
                    return FALSE;
                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);
            
                // If the cursor is on the last line and the line's last
                // character is not a LF, then FALSE is returned as nothing
                // can be done. A LF on the end of a line does not tell the
                // wLines member that there is another line, hence this
                // extra check.
                if ( nLine == (pContext->wLines-1) &&
                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
                {
                    return FALSE;
                }
            
                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
                // If the cursor is more characters in than the next line...
                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
                {
                    // If it is the last line, don't subtract the LINEBREAK from selection spot
                    if( nLine+2 == pContext->wLines )
                    {
                        nSel = pContext->pwLineStarts[nLine+2];
                    }
                    else
                    {
                        nSel = pContext->pwLineStarts[nLine+2]-1;
                    }
                }
                else
                {
                    // Selection spot is number of chars into the next line
                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
                    // If this is not the beginning of a line 
                    // and the selection point is a LINEBREAK, subtract one
                    // Otherwise the selection overshoots to the first character
                    // of the following line.
                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
                    {
                        nSel--;
                    }
                }
                lineChars = nSel - pContext->wSelEnd;
                sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, 
                            T9CA_MOVERIGHT, 
                            lineChars );                                 
            }            
            break; 
            
        case T9KEYCLEAR:
            ERR("T9TextCtl_MultitapKey::9",0,0,0);
            pContext->sFocus = FOCUS_TEXT;                
            // Turn off the timer until another numeric key is pressed
            (void) ISHELL_CancelTimer((IShell *)pContext->pIShell,
                                       TextCtl_MultitapTimer, pContext);
            if ((FOCUS_TEXT == pContext->sFocus) &&
                (0 == pContext->wSelStart) && (pContext->wSelStart == pContext->wSelEnd))
            {
                return FALSE;
            }
            else
            {
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key ); 
            }
            break;               

        case T9KEYNONE:
            ERR("T9TextCtl_MultitapKey::10",0,0,0);
            if(FOCUS_SELECTION == pContext->sFocus)
            {
                pContext->sFocus = FOCUS_TEXT;             
                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYRIGHT );
                break;
            }

        default:
            ERR("T9TextCtl_MultitapKey::11",0,0,0);
            pContext->sFocus = FOCUS_TEXT;   
            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
            break;  
    }   
    ERR("T9TextCtl_MultitapKey::12",0,0,0);
    pContext->uModeInfo.mtap.kLast = key;   

    //display strings
    //if ( T9STATNONE == sT9Status )
    {     
        bRet = T9_AW_DisplayText ( pContext, key);  
    }
    
    //Set timer 
    if(TRUE == bRet)
    {
        // Set timer to deselect it
        (void) ISHELL_SetTimer((IShell *) pContext->pIShell,
                        MULTITAP_TIMEOUT,
                        TextCtl_MultitapTimer,
                        pContext);  
    }
    ERR("T9TextCtl_MultitapKey::13",0,0,0);
#endif	
    
    return bRet;
}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9TextCtl_MultitapExit
 *  Description     : Exit Multitap Mode.
 *
 *  Pass            : void
 *
 *  Return          : void
 *
 *-----------------------------------------------------------------------*/
static void T9TextCtl_MultitapExit(TextCtlContext *pContext)
{
    T9_AW_Destroy ( pContext );
    (void) ISHELL_CancelTimer((IShell *) pContext->pIShell,
                            TextCtl_MultitapTimer, pContext);    
    TextCtl_NoSelection(pContext);
	pContext->m_bCaplk = FALSE;

}

//add by yangdecai 2010-09-09
static void T9TextCtl_Cap_Lower_Restart(TextCtlContext *pContext)
{
	T9STATUS sT9Status = T9STATERROR;
    
    // Init Alphabet input
    T9_AW_Init ( pContext );

    switch ( OEM_TextGetCurrentMode((OEMCONTEXT)pContext) )
    {
#ifdef FEATURE_T9_CAP_LOWER_ENGLISH
         case TEXT_MODE_T9_CAP_LOWER_ENGLISH:
            sT9Status = T9AWSetLanguage ( &pContext->sT9awFieldInfo, 
                                          T9PIDEnglish, 0, 0 );       
            break;       
#endif //FEATURE_T9_CAP_LOWER_ENGLISH    
		default:
			break;

    }
	//set to Ambiguous mode
    if ( T9STATNONE == sT9Status )
    {
        //set to Multitap mode
        sT9Status = T9AWSetMultitapMode(&pContext->sT9awFieldInfo);
    }
    
    // re-setup Text Buffer
    if ( T9STATNONE == sT9Status )
    {
        sT9Status = T9ResetTextBuf(&pContext->sT9awFieldInfo.G, MAX_BUFFER_LENGTH);
    }
    if ( T9STATNONE == sT9Status )
    {        
        sT9Status = T9AddTextString ( &pContext->sT9awFieldInfo.G, 
                                      pContext->pszContents, 
                                      pContext->wContentsChars );
    }
    
    // set Cursor
    if ( T9STATNONE == sT9Status )
    {
        T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, pContext->wSelEnd );
    }
    
    TextCtl_NoSelection(pContext);
    pContext->uModeInfo.mtap.kLast = AVK_FIRST;  
    TextCtl_TextChanged(pContext);
    pContext->sFocus = FOCUS_TEXT;
}
static boolean T9TextCtl_Cap_Lower_Rapid_Key(TextCtlContext *pContext,AEEEvent eCode,AVKType key)
{
	boolean  bRet       = FALSE;
    T9STATUS sT9Status = T9STATERROR;  
    T9KEY    t9Key;
    uint16   nBufLen = pContext->sT9awFieldInfo.G.nBufLen;
    ERR("T9TextCtl_MultitapKey::start",0,0,0);
    //handle key
#if defined (FEATURE_DISP_160X128) || defined (FEATURE_DISP_220X176)
    
    if(eCode == EVT_KEY_HELD)
    {
        int i;
        AECHAR ch = 0;
		switch(key){
		case AVK_0:
		case AVK_1:
		case AVK_2:
		case AVK_3:
		case AVK_4:
		case AVK_5:
		case AVK_6:
		case AVK_7:
		case AVK_8:
		case AVK_9:
        case AVK_POUND:
        case AVK_STAR:
        case AVK_T:
        case AVK_Y:
        case AVK_U:
        case AVK_I:
        case AVK_O:
        case AVK_P:
        case AVK_G:
        case AVK_H:
        case AVK_J:
        case AVK_K:
        case AVK_L:
        case AVK_V:
        case AVK_B:
        case AVK_N:
        case AVK_M:
            for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
			{           
        		if (key == VLCharKeyItem[i].wParam)
        		{
                    ch = VLCharKeyItem[i].wp;
                    break;
        		}
            }
            break;
            
		default:
		    break;
		}
        
        if(ch != 0)
        {
            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
			{
				/* Set selection to the character before the insertion point */
				--pContext->wSelStart;
			}
			else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
			{
				 return FALSE;
			}
	
			/* Insert a "NUL" to just delete and insert nothing */
			TextCtl_AddChar(pContext, 0);
			if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
			{
				// meet the max count of the text.
				if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
				{
					sT9Status = T9STATERROR; 
					return FALSE;
				}
			}
			TextCtl_NoSelection(pContext);
			TextCtl_AddChar(pContext,ch);
            return TRUE;
        }
    }
	else
	{
	    if(pContext->uModeInfo.mtap.kLast == AVK_UNDEFINED)
	    {
	        pContext->sFocus = FOCUS_TEXT;            
	    }

	    if(AVK_SELECT == key || AVK_INFO == key)
	    {
	        if(FOCUS_SELECTION == pContext->sFocus)
	        {       
	            t9Key = T9KEYRIGHT;
	        }
	        else
	        {
	            sT9Status = T9STATERROR;         
	            return FALSE;          
	        }
	    }     
		
	    switch ( key) 
	    {
	    	case AVK_0:
	        case AVK_1:
	        case AVK_2:
	        case AVK_3:
	        case AVK_4:
	        case AVK_5:
	        case AVK_6:
	        case AVK_7:
	        case AVK_8:
	        case AVK_9:
	        case AVK_POUND:
	        case AVK_STAR: 
	        case AVK_T: 
	        case AVK_Y:
	        case AVK_U:
	        case AVK_I:
	        case AVK_O:
	        case AVK_P:
	        case AVK_G:
	        case AVK_H:
	        case AVK_J:
	        case AVK_K:
	        case AVK_L:
	        case AVK_V: 
	        case AVK_B: 
	        case AVK_N:
	        case AVK_M:
	        case AVK_ENTER:
			case AVK_SPACE:
	        case AVK_RWD:
	            {
	                int i = 0;
	                if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
	                {
	                    // meet the max count of the text.
	                    if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
	                    {
	                        sT9Status = T9STATERROR; 
	                        return FALSE;
	                    }
	                }
	                for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
	    			{       
	            		if (key == VLCharKeyItem[i].wParam)
	            		{
	        			    if(pContext->is_isShift)
	                        { 
	                            TextCtl_NoSelection(pContext);
	                            TextCtl_AddChar(pContext,(AECHAR)(VLCharKeyItem[i].wp));
	                            pContext->is_isShift = FALSE;
	                        }
	                        else
	                        {
	                        	AECHAR Tempstr[5] = {L". "};
								AECHAR Tempstrp[5] = {L"."};
	                            TextCtl_NoSelection(pContext);
								nBufLen = WSTRLEN(pContext->pszContents);
								if(nBufLen == 0)
								{
									
	                            	TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
									
								}
								else
								{
									if((pContext->m_bCaplk) || (!WSTRCMP(pContext->pszContents+(nBufLen-2),Tempstr))||
										(!WSTRCMP(pContext->pszContents+(nBufLen-1),Tempstrp)))
									{
										TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
										pContext->m_bCaplk = FALSE;
									}
									else
									{
	                            		TextCtl_AddChar(pContext,(AECHAR)(VLCharLowKeyItem[i].wp));
									}
								}
	                        }
	                     }
	                  }
	            }
	            return TRUE;
	            break;
	         case AVK_SHIFT:
	              {
	                 if(pContext->is_isShift)
	                 {
	                    pContext->is_isShift = FALSE;
	                 }
	                 else
	                 {
	                    pContext->is_isShift = TRUE;
	                 }
	              }
	              break;
	        case AVK_CAPLK:
	             {
				 	
	                pContext->m_bCaplk = !pContext->m_bCaplk;
	             }
	             return TRUE;
	             break;
	        case AVK_LEFT:
	            {
	            if(FOCUS_SELECTION == pContext->sFocus)
	            {
	                pContext->sFocus = FOCUS_TEXT;             
	                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYLEFT );  
	            }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	            {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelEnd + 1;
	                   if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                   {
	                       OEM_TextSetCursorPos(pContext, 0);
	                   }
	                   else 
	                   {  
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	            }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if (OEM_TextGetCursorPos(pContext) == 0)
	                {
	                    OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                }
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelStart;
	                    if (wNewSel)
	                    {
	                        --wNewSel;
	                    }               
#ifdef FEATURE_LANG_THAI
	                    {
	                        int count=0;
	                        count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
	                                                    pContext->pszContents[pContext->wSelStart-1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel - count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI             
	                OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;
	            }            
	            break;     

	        case AVK_RIGHT:
	             {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	                {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelStart ;
	                   if ( OEM_TextGetCursorPos(pContext) == 0 )
	                   {
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
	                       //OEM_TextSetSel(pContext, 0, 0);
	                       OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                   }
	                   else 
	                   {  
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );  
	                       wNewSel --;           
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	                }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	             if (OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                {
	                    OEM_TextSetCursorPos(pContext, -1);
	                }                
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelEnd + 1;                  
#ifdef FEATURE_LANG_THAI  
	                    {
	                        int count=0;
	                        count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
	                                                     pContext->pszContents[pContext->wSelStart+1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel + count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI      
	                 OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;  
	            }
	            break;   

	         case AVK_UP:
	              {
	                uint16 nLine, nCharsIn,nSel;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If it is on the first line, return false
	                if(nLine == 0 || !pContext->pwLineStarts)
	                    return FALSE;

	                // Otherwise figure out how many characters from the start
	                // of the line the cursor is and try to put the cursor in a
	                // similar position on previous line. Or, if not enough
	                // chars, at the end of the line
	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
	                                               pContext->pwLineStarts[nLine]) 
	                {
	                    nSel = pContext->pwLineStarts[nLine]-1;
	                } 
	                else 
	                {
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);
	                return TRUE;
	            }            
	            break;   

	        case AVK_DOWN:
	             {
	                uint16 nLine, nCharsIn,nSel;

	                if((!pContext->pwLineStarts)||(!pContext->wLines))
	                    return FALSE;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If the cursor is on the last line and the line's last
	                // character is not a LF, then FALSE is returned as nothing
	                // can be done. A LF on the end of a line does not tell the
	                // wLines member that there is another line, hence this
	                // extra check.
	                if ( nLine == (pContext->wLines-1) &&
	                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
	                {
	                    return FALSE;
	                }

	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                // If the cursor is more characters in than the next line...
	                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
	                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
	                {
	                    // If it is the last line, don't subtract the LINEBREAK from selection spot
	                    if( nLine+2 == pContext->wLines )
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2];
	                    }
	                    else
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2]-1;
	                    }
	                }
	                else
	                {
	                    // Selection spot is number of chars into the next line
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
	                    // If this is not the beginning of a line 
	                    // and the selection point is a LINEBREAK, subtract one
	                    // Otherwise the selection overshoots to the first character
	                    // of the following line.
	                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
	                    {
	                        nSel--;
	                    }
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);

	                return TRUE;
	            }            
	            break;    

	            
	        case AVK_CLR:
	            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
	            {
	                 /* Set selection to the character before the insertion point */
	                 --pContext->wSelStart;
	            }
	            else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
	            {
	                  return FALSE;
	            }
	            
	            /* Insert a "NUL" to just delete and insert nothing */
	            TextCtl_AddChar(pContext, 0);
	            return TRUE;                

	        default:
	            pContext->sFocus = FOCUS_TEXT;   
	            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
	            break;  
	    }   
	}		
#elif defined (FEATURE_DISP_128X160)
    if(eCode == EVT_KEY_HELD)
    {
        int i;
        AECHAR ch = 0;
		switch(key){
		case AVK_0:
		case AVK_1:
		case AVK_2:
		case AVK_3:
		case AVK_4:
		case AVK_5:
		case AVK_6:
		case AVK_7:
		case AVK_8:
		case AVK_9:
        case AVK_POUND:
        case AVK_STAR:
        case AVK_T:
        case AVK_Y:
        case AVK_U:
        case AVK_I:
        case AVK_O:
        case AVK_P:
        case AVK_G:
        case AVK_H:
        case AVK_J:
        case AVK_K:
        case AVK_L:
        case AVK_V:
        case AVK_B:
        case AVK_N:
        case AVK_M:
            for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
			{           
        		if (key == VLCharKeyItem[i].wParam)
        		{
                    ch = VLCharKeyItem[i].wp;
                    break;
        		}
            }
            break;
            
		default:
		    break;
		}
        
        if(ch != 0)
        {
            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
			{
				/* Set selection to the character before the insertion point */
				--pContext->wSelStart;
			}
			else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
			{
				 return FALSE;
			}
	
			/* Insert a "NUL" to just delete and insert nothing */
			TextCtl_AddChar(pContext, 0);
			if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
			{
				// meet the max count of the text.
				if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
				{
					sT9Status = T9STATERROR; 
					return FALSE;
				}
			}
			TextCtl_NoSelection(pContext);
			TextCtl_AddChar(pContext,ch);
            return TRUE;
        }
    }
	else
	{
	    if(pContext->uModeInfo.mtap.kLast == AVK_UNDEFINED)
	    {
	        pContext->sFocus = FOCUS_TEXT;            
	    }

	    if(AVK_SELECT == key || AVK_INFO == key)
	    {
	        if(FOCUS_SELECTION == pContext->sFocus)
	        {       
	            t9Key = T9KEYRIGHT;
	        }
	        else
	        {
	            sT9Status = T9STATERROR;         
	            return FALSE;          
	        }
	    }     
		
	    switch ( key) 
	    {
	    	case AVK_0:
	        case AVK_1:
	        case AVK_2:
	        case AVK_3:
	        case AVK_4:
	        case AVK_5:
	        case AVK_6:
	        case AVK_7:
	        case AVK_8:
	        case AVK_9:
	        case AVK_POUND:
	        case AVK_STAR: 
	        case AVK_T: 
	        case AVK_Y:
	        case AVK_U:
	        case AVK_I:
	        case AVK_O:
	        case AVK_P:
	        case AVK_G:
	        case AVK_H:
	        case AVK_J:
	        case AVK_K:
	        case AVK_L:
	        case AVK_V: 
	        case AVK_B: 
	        case AVK_N:
	        case AVK_M:
	        case AVK_ENTER:
			case AVK_SPACE:
	        case AVK_RWD:
	            {
	                int i = 0;
	                if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
	                {
	                    // meet the max count of the text.
	                    if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
	                    {
	                        sT9Status = T9STATERROR; 
	                        return FALSE;
	                    }
	                }
	                for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
	    			{       
	            		if (key == VLCharKeyItem[i].wParam)
	            		{
	        			    if(pContext->is_isShift)
	                        { 
	                            TextCtl_NoSelection(pContext);
	                            TextCtl_AddChar(pContext,(AECHAR)(VLCharKeyItem[i].wp));
	                            pContext->is_isShift = FALSE;
	                        }
	                        else
	                        {
	                        	AECHAR Tempstr[5] = {L". "};
								AECHAR Tempstrp[5] = {L"."};
	                            TextCtl_NoSelection(pContext);
								nBufLen = WSTRLEN(pContext->pszContents);
								if(nBufLen == 0)
								{
									
	                            	TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
									
								}
								else
								{
									if((pContext->m_bCaplk) || (!WSTRCMP(pContext->pszContents+(nBufLen-2),Tempstr))||
										(!WSTRCMP(pContext->pszContents+(nBufLen-1),Tempstrp)))
									{
										TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
										pContext->m_bCaplk = FALSE;
									}
									else
									{
	                            		TextCtl_AddChar(pContext,(AECHAR)(VLCharLowKeyItem[i].wp));
									}
								}
	                        }
	                     }
	                  }
	            }
	            return TRUE;
	            break;
	         case AVK_SHIFT:
	              {
	                 if(pContext->is_isShift)
	                 {
	                    pContext->is_isShift = FALSE;
	                 }
	                 else
	                 {
	                    pContext->is_isShift = TRUE;
	                 }
	              }
	              break;
	        case AVK_CAPLK:
	             {
				 	
	                pContext->m_bCaplk = !pContext->m_bCaplk;
	             }
	             return TRUE;
	             break;
	        case AVK_LEFT:
	            {
	            if(FOCUS_SELECTION == pContext->sFocus)
	            {
	                pContext->sFocus = FOCUS_TEXT;             
	                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYLEFT );  
	            }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	            {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelEnd + 1;
	                   if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                   {
	                       OEM_TextSetCursorPos(pContext, 0);
	                   }
	                   else 
	                   {  
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	            }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if (OEM_TextGetCursorPos(pContext) == 0)
	                {
	                    OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                }
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelStart;
	                    if (wNewSel)
	                    {
	                        --wNewSel;
	                    }               
#ifdef FEATURE_LANG_THAI
	                    {
	                        int count=0;
	                        count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
	                                                    pContext->pszContents[pContext->wSelStart-1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel - count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI        
	                OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;
	            }            
	            break;     

	        case AVK_RIGHT:
	             {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	                {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelStart ;
	                   if ( OEM_TextGetCursorPos(pContext) == 0 )
	                   {
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
	                       //OEM_TextSetSel(pContext, 0, 0);
	                       OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                   }
	                   else 
	                   {  
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );  
	                       wNewSel --;        
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	                }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	             if (OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                {
	                    OEM_TextSetCursorPos(pContext, -1);
	                }                
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelEnd + 1;                  
#ifdef FEATURE_LANG_THAI  
	                    {
	                        int count=0;
	                        count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
	                                                     pContext->pszContents[pContext->wSelStart+1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel + count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI                  
	                 OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;  
	            }
	            break;   

	         case AVK_UP:
	              {
	                uint16 nLine, nCharsIn,nSel;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If it is on the first line, return false
	                if(nLine == 0 || !pContext->pwLineStarts)
	                    return FALSE;

	                // Otherwise figure out how many characters from the start
	                // of the line the cursor is and try to put the cursor in a
	                // similar position on previous line. Or, if not enough
	                // chars, at the end of the line
	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
	                                               pContext->pwLineStarts[nLine]) 
	                {
	                    nSel = pContext->pwLineStarts[nLine]-1;
	                } 
	                else 
	                {
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);
	                return TRUE;
	            }            
	            break;   

	        case AVK_DOWN:
	             {
	                uint16 nLine, nCharsIn,nSel;

	                if((!pContext->pwLineStarts)||(!pContext->wLines))
	                    return FALSE;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If the cursor is on the last line and the line's last
	                // character is not a LF, then FALSE is returned as nothing
	                // can be done. A LF on the end of a line does not tell the
	                // wLines member that there is another line, hence this
	                // extra check.
	                if ( nLine == (pContext->wLines-1) &&
	                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
	                {
	                    return FALSE;
	                }

	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                // If the cursor is more characters in than the next line...
	                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
	                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
	                {
	                    // If it is the last line, don't subtract the LINEBREAK from selection spot
	                    if( nLine+2 == pContext->wLines )
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2];
	                    }
	                    else
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2]-1;
	                    }
	                }
	                else
	                {
	                    // Selection spot is number of chars into the next line
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
	                    // If this is not the beginning of a line 
	                    // and the selection point is a LINEBREAK, subtract one
	                    // Otherwise the selection overshoots to the first character
	                    // of the following line.
	                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
	                    {
	                        nSel--;
	                    }
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);

	                return TRUE;
	            }            
	            break;    

	            
	        case AVK_CLR:
	            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
	            {
	                 /* Set selection to the character before the insertion point */
	                 --pContext->wSelStart;
	            }
	            else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
	            {
	                  return FALSE;
	            }
	            
	            /* Insert a "NUL" to just delete and insert nothing */
	            TextCtl_AddChar(pContext, 0);
	            return TRUE;                

	        default:
	            pContext->sFocus = FOCUS_TEXT;   
	            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
	            break;  
	    }   
	}	
#elif defined (FEATURE_DISP_176X220)
    if(eCode == EVT_KEY_HELD)
    {
        int i;
        AECHAR ch = 0;
		switch(key){
		case AVK_0:
		case AVK_1:
		case AVK_2:
		case AVK_3:
		case AVK_4:
		case AVK_5:
		case AVK_6:
		case AVK_7:
		case AVK_8:
		case AVK_9:
        case AVK_POUND:
        case AVK_STAR:
        case AVK_T:
        case AVK_Y:
        case AVK_U:
        case AVK_I:
        case AVK_O:
        case AVK_P:
        case AVK_G:
        case AVK_H:
        case AVK_J:
        case AVK_K:
        case AVK_L:
        case AVK_V:
        case AVK_B:
        case AVK_N:
        case AVK_M:
            for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
			{           
        		if (key == VLCharKeyItem[i].wParam)
        		{
                    ch = VLCharKeyItem[i].wp;
                    break;
        		}
            }
            break;
            
		default:
		    break;
		}
        
        if(ch != 0)
        {
            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
			{
				/* Set selection to the character before the insertion point */
				--pContext->wSelStart;
			}
			else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
			{
				 return FALSE;
			}
	
			/* Insert a "NUL" to just delete and insert nothing */
			TextCtl_AddChar(pContext, 0);
			if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
			{
				// meet the max count of the text.
				if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
				{
					sT9Status = T9STATERROR; 
					return FALSE;
				}
			}
			TextCtl_NoSelection(pContext);
			TextCtl_AddChar(pContext,ch);
            return TRUE;
        }
    }
	else
	{
	    if(pContext->uModeInfo.mtap.kLast == AVK_UNDEFINED)
	    {
	        pContext->sFocus = FOCUS_TEXT;            
	    }

	    if(AVK_SELECT == key || AVK_INFO == key)
	    {
	        if(FOCUS_SELECTION == pContext->sFocus)
	        {       
	            t9Key = T9KEYRIGHT;
	        }
	        else
	        {
	            sT9Status = T9STATERROR;         
	            return FALSE;          
	        }
	    }     
		
	    switch ( key) 
	    {
	    	case AVK_0:
	        case AVK_1:
	        case AVK_2:
	        case AVK_3:
	        case AVK_4:
	        case AVK_5:
	        case AVK_6:
	        case AVK_7:
	        case AVK_8:
	        case AVK_9:
	        case AVK_POUND:
	        case AVK_STAR: 
	        case AVK_T: 
	        case AVK_Y:
	        case AVK_U:
	        case AVK_I:
	        case AVK_O:
	        case AVK_P:
	        case AVK_G:
	        case AVK_H:
	        case AVK_J:
	        case AVK_K:
	        case AVK_L:
	        case AVK_V: 
	        case AVK_B: 
	        case AVK_N:
	        case AVK_M:
	        case AVK_ENTER:
			case AVK_SPACE:
	        case AVK_RWD:
	            {
	                int i = 0;
	                if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
	                {
	                    // meet the max count of the text.
	                    if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
	                    {
	                        sT9Status = T9STATERROR; 
	                        return FALSE;
	                    }
	                }
	                for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
	    			{       
	            		if (key == VLCharKeyItem[i].wParam)
	            		{
	        			    if(pContext->is_isShift)
	                        { 
	                            TextCtl_NoSelection(pContext);
	                            TextCtl_AddChar(pContext,(AECHAR)(VLCharKeyItem[i].wp));
	                            pContext->is_isShift = FALSE;
	                        }
	                        else
	                        {
	                        	AECHAR Tempstr[5] = {L". "};
								AECHAR Tempstrp[5] = {L"."};
	                            TextCtl_NoSelection(pContext);
								nBufLen = WSTRLEN(pContext->pszContents);
								if(nBufLen == 0)
								{
									
	                            	TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
									
								}
								else
								{
									if((pContext->m_bCaplk) || (!WSTRCMP(pContext->pszContents+(nBufLen-2),Tempstr))||
										(!WSTRCMP(pContext->pszContents+(nBufLen-1),Tempstrp)))
									{
										TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
										pContext->m_bCaplk = FALSE;
									}
									else
									{
	                            		TextCtl_AddChar(pContext,(AECHAR)(VLCharLowKeyItem[i].wp));
									}
								}
	                        }
	                     }
	                  }
	            }
	            return TRUE;
	            break;
	         case AVK_SHIFT:
	              {
	                 if(pContext->is_isShift)
	                 {
	                    pContext->is_isShift = FALSE;
	                 }
	                 else
	                 {
	                    pContext->is_isShift = TRUE;
	                 }
	              }
	              break;
	        case AVK_CAPLK:
	             {
				 	
	                pContext->m_bCaplk = !pContext->m_bCaplk;
	             }
	             return TRUE;
	             break;
	        case AVK_LEFT:
	            {
	            if(FOCUS_SELECTION == pContext->sFocus)
	            {
	                pContext->sFocus = FOCUS_TEXT;             
	                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYLEFT );  
	            }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	            {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelEnd + 1;
	                   if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                   {
	                       OEM_TextSetCursorPos(pContext, 0);
	                   }
	                   else 
	                   {  
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	            }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if (OEM_TextGetCursorPos(pContext) == 0)
	                {
	                    OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                }
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelStart;
	                    if (wNewSel)
	                    {
	                        --wNewSel;
	                    }               
#ifdef FEATURE_LANG_THAI
	                    {
	                        int count=0;
	                        count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
	                                                    pContext->pszContents[pContext->wSelStart-1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel - count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI           
	                OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;
	            }            
	            break;     

	        case AVK_RIGHT:
	             {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	                {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelStart ;
	                   if ( OEM_TextGetCursorPos(pContext) == 0 )
	                   {
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
	                       //OEM_TextSetSel(pContext, 0, 0);
	                       OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                   }
	                   else 
	                   {  
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );  
	                       wNewSel --;     
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	                }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	             if (OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                {
	                    OEM_TextSetCursorPos(pContext, -1);
	                }                
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelEnd + 1;                  
#ifdef FEATURE_LANG_THAI  
	                    {
	                        int count=0;
	                        count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
	                                                     pContext->pszContents[pContext->wSelStart+1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel + count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI      
	                 OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;  
	            }
	            break;   

	         case AVK_UP:
	              {
	                uint16 nLine, nCharsIn,nSel;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If it is on the first line, return false
	                if(nLine == 0 || !pContext->pwLineStarts)
	                    return FALSE;

	                // Otherwise figure out how many characters from the start
	                // of the line the cursor is and try to put the cursor in a
	                // similar position on previous line. Or, if not enough
	                // chars, at the end of the line
	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
	                                               pContext->pwLineStarts[nLine]) 
	                {
	                    nSel = pContext->pwLineStarts[nLine]-1;
	                } 
	                else 
	                {
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);
	                return TRUE;
	            }            
	            break;   

	        case AVK_DOWN:
	             {
	                uint16 nLine, nCharsIn,nSel;

	                if((!pContext->pwLineStarts)||(!pContext->wLines))
	                    return FALSE;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If the cursor is on the last line and the line's last
	                // character is not a LF, then FALSE is returned as nothing
	                // can be done. A LF on the end of a line does not tell the
	                // wLines member that there is another line, hence this
	                // extra check.
	                if ( nLine == (pContext->wLines-1) &&
	                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
	                {
	                    return FALSE;
	                }

	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                // If the cursor is more characters in than the next line...
	                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
	                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
	                {
	                    // If it is the last line, don't subtract the LINEBREAK from selection spot
	                    if( nLine+2 == pContext->wLines )
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2];
	                    }
	                    else
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2]-1;
	                    }
	                }
	                else
	                {
	                    // Selection spot is number of chars into the next line
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
	                    // If this is not the beginning of a line 
	                    // and the selection point is a LINEBREAK, subtract one
	                    // Otherwise the selection overshoots to the first character
	                    // of the following line.
	                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
	                    {
	                        nSel--;
	                    }
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);

	                return TRUE;
	            }            
	            break;    

	            
	        case AVK_CLR:
	            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
	            {
	                 /* Set selection to the character before the insertion point */
	                 --pContext->wSelStart;
	            }
	            else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
	            {
	                  return FALSE;
	            }
	            
	            /* Insert a "NUL" to just delete and insert nothing */
	            TextCtl_AddChar(pContext, 0);
	            return TRUE;                

	        default:
	            pContext->sFocus = FOCUS_TEXT;   
	            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
	            break;  
	    }   
	}	
#elif defined (FEATURE_DISP_240X320) || defined (FEATURE_DISP_320X240)
    if(eCode == EVT_KEY_HELD)
    {
        int i;
        AECHAR ch = 0;
		switch(key){
		case AVK_0:
		case AVK_1:
		case AVK_2:
		case AVK_3:
		case AVK_4:
		case AVK_5:
		case AVK_6:
		case AVK_7:
		case AVK_8:
		case AVK_9:
        case AVK_POUND:
        case AVK_STAR:
        case AVK_T:
        case AVK_Y:
        case AVK_U:
        case AVK_I:
        case AVK_O:
        case AVK_P:
        case AVK_G:
        case AVK_H:
        case AVK_J:
        case AVK_K:
        case AVK_L:
        case AVK_V:
        case AVK_B:
        case AVK_N:
        case AVK_M:
            for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
			{           
        		if (key == VLCharKeyItem[i].wParam)
        		{
                    ch = VLCharKeyItem[i].wp;
                    break;
        		}
            }
            break;
            
		default:
		    break;
		}
        
        if(ch != 0)
        {
            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
			{
				/* Set selection to the character before the insertion point */
				--pContext->wSelStart;
			}
			else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
			{
				 return FALSE;
			}
	
			/* Insert a "NUL" to just delete and insert nothing */
			TextCtl_AddChar(pContext, 0);
			if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
			{
				// meet the max count of the text.
				if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
				{
					sT9Status = T9STATERROR; 
					return FALSE;
				}
			}
			TextCtl_NoSelection(pContext);
			TextCtl_AddChar(pContext,ch);
            return TRUE;
        }
    }
	else
	{
	    if(pContext->uModeInfo.mtap.kLast == AVK_UNDEFINED)
	    {
	        pContext->sFocus = FOCUS_TEXT;            
	    }

	    if(AVK_SELECT == key || AVK_INFO == key)
	    {
	        if(FOCUS_SELECTION == pContext->sFocus)
	        {       
	            t9Key = T9KEYRIGHT;
	        }
	        else
	        {
	            sT9Status = T9STATERROR;         
	            return FALSE;          
	        }
	    }     
		
	    switch ( key) 
	    {
	    	case AVK_0:
	        case AVK_1:
	        case AVK_2:
	        case AVK_3:
	        case AVK_4:
	        case AVK_5:
	        case AVK_6:
	        case AVK_7:
	        case AVK_8:
	        case AVK_9:
	        case AVK_POUND:
	        case AVK_STAR: 
	        case AVK_T: 
	        case AVK_Y:
	        case AVK_U:
	        case AVK_I:
	        case AVK_O:
	        case AVK_P:
	        case AVK_G:
	        case AVK_H:
	        case AVK_J:
	        case AVK_K:
	        case AVK_L:
	        case AVK_V: 
	        case AVK_B: 
	        case AVK_N:
	        case AVK_M:
	        case AVK_ENTER:
			case AVK_SPACE:
	        case AVK_RWD:
	            {
	                int i = 0;
	                if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
	                {
	                    // meet the max count of the text.
	                    if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
	                    {
	                        sT9Status = T9STATERROR; 
	                        return FALSE;
	                    }
	                }
	                for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
	    			{       
	            		if (key == VLCharKeyItem[i].wParam)
	            		{
	        			    if(pContext->is_isShift)
	                        { 
	                            TextCtl_NoSelection(pContext);
	                            TextCtl_AddChar(pContext,(AECHAR)(VLCharKeyItem[i].wp));
	                            pContext->is_isShift = FALSE;
	                        }
	                        else
	                        {
	                        	AECHAR Tempstr[5] = {L". "};
								AECHAR Tempstrp[5] = {L"."};
	                            TextCtl_NoSelection(pContext);
								nBufLen = WSTRLEN(pContext->pszContents);
								if(nBufLen == 0)
								{
									
	                            	TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
									
								}
								else
								{
									if((pContext->m_bCaplk) || (!WSTRCMP(pContext->pszContents+(nBufLen-2),Tempstr))||
										(!WSTRCMP(pContext->pszContents+(nBufLen-1),Tempstrp)))
									{
										TextCtl_AddChar(pContext,(AECHAR)(VLCharCapKeyItem[i].wp));
										pContext->m_bCaplk = FALSE;
									}
									else
									{
	                            		TextCtl_AddChar(pContext,(AECHAR)(VLCharLowKeyItem[i].wp));
									}
								}
	                        }
	                     }
	                  }
	            }
	            return TRUE;
	            break;
	         case AVK_SHIFT:
	              {
	                 if(pContext->is_isShift)
	                 {
	                    pContext->is_isShift = FALSE;
	                 }
	                 else
	                 {
	                    pContext->is_isShift = TRUE;
	                 }
	              }
	              break;
	        case AVK_CAPLK:
	             {
				 	
	                pContext->m_bCaplk = !pContext->m_bCaplk;
	             }
	             return TRUE;
	             break;
	        case AVK_LEFT:
	            {
	            if(FOCUS_SELECTION == pContext->sFocus)
	            {
	                pContext->sFocus = FOCUS_TEXT;             
	                sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, T9KEYLEFT );  
	            }
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	            {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelEnd + 1;
	                   if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                   {
	                       OEM_TextSetCursorPos(pContext, 0);
	                   }
	                   else 
	                   {  
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	            }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if (OEM_TextGetCursorPos(pContext) == 0)
	                {
	                    OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                }
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelStart;
	                    if (wNewSel)
	                    {
	                        --wNewSel;
	                    }               
#ifdef FEATURE_LANG_THAI
	                    {
	                        int count=0;
	                        count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
	                                                    pContext->pszContents[pContext->wSelStart-1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel - count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI            
	                OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;
	            }            
	            break;     

	        case AVK_RIGHT:
	             {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	            if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	                {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelStart ;
	                   if ( OEM_TextGetCursorPos(pContext) == 0 )
	                   {
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
	                       //OEM_TextSetSel(pContext, 0, 0);
	                       OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                   }
	                   else 
	                   {  
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );  
	                       wNewSel --;   
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	                }
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	             if (OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                {
	                    OEM_TextSetCursorPos(pContext, -1);
	                }                
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelEnd + 1;                  
#ifdef FEATURE_LANG_THAI  
	                    {
	                        int count=0;
	                        count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
	                                                     pContext->pszContents[pContext->wSelStart+1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel + count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI    
	                 OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;  
	            }
	            break;   

	         case AVK_UP:
	              {
	                uint16 nLine, nCharsIn,nSel;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If it is on the first line, return false
	                if(nLine == 0 || !pContext->pwLineStarts)
	                    return FALSE;

	                // Otherwise figure out how many characters from the start
	                // of the line the cursor is and try to put the cursor in a
	                // similar position on previous line. Or, if not enough
	                // chars, at the end of the line
	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
	                                               pContext->pwLineStarts[nLine]) 
	                {
	                    nSel = pContext->pwLineStarts[nLine]-1;
	                } 
	                else 
	                {
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);
	                return TRUE;
	            }            
	            break;   

	        case AVK_DOWN:
	             {
	                uint16 nLine, nCharsIn,nSel;

	                if((!pContext->pwLineStarts)||(!pContext->wLines))
	                    return FALSE;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If the cursor is on the last line and the line's last
	                // character is not a LF, then FALSE is returned as nothing
	                // can be done. A LF on the end of a line does not tell the
	                // wLines member that there is another line, hence this
	                // extra check.
	                if ( nLine == (pContext->wLines-1) &&
	                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
	                {
	                    return FALSE;
	                }

	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                // If the cursor is more characters in than the next line...
	                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
	                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
	                {
	                    // If it is the last line, don't subtract the LINEBREAK from selection spot
	                    if( nLine+2 == pContext->wLines )
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2];
	                    }
	                    else
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2]-1;
	                    }
	                }
	                else
	                {
	                    // Selection spot is number of chars into the next line
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
	                    // If this is not the beginning of a line 
	                    // and the selection point is a LINEBREAK, subtract one
	                    // Otherwise the selection overshoots to the first character
	                    // of the following line.
	                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
	                    {
	                        nSel--;
	                    }
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);

	                return TRUE;
	            }            
	            break;    

	            
	        case AVK_CLR:
	            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
	            {
	                 /* Set selection to the character before the insertion point */
	                 --pContext->wSelStart;
	            }
	            else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
	            {
	                  return FALSE;
	            }
	            
	            /* Insert a "NUL" to just delete and insert nothing */
	            TextCtl_AddChar(pContext, 0);
	            return TRUE;                

	        default:
	            pContext->sFocus = FOCUS_TEXT;   
	            sT9Status = T9HandleKey ( &pContext->sT9awFieldInfo.G, t9Key );  
	            break;  
	    }   
	}	
#endif
    return bRet;
}
static void T9TextCtl_Cap_Lower_Rapid_Exit(TextCtlContext *pContext)
{
	T9_AW_Destroy ( pContext );  
    TextCtl_NoSelection(pContext);
	pContext->m_bCaplk = FALSE;
}

//add by yangdecai end




/*------------------------------------------------------------------------
 *
 *  Function name   : T9_AW_Init
 *  Description     : Allocates and initializes all data structures
 *                    necessary for T9 operation.  Calls Init API function
 *                    to start T9 in the proper mode.
 *
 *                    This routine should be called during the initialization
 *                    process of the system.
 *
 *  Pass            : void
 *
 *  Return          : void
 *
 *-----------------------------------------------------------------------*/
static void T9_AW_Init(TextCtlContext *pContext)
{
    T9STATUS sT9Status = T9STATERROR;
    MEMSET ( &pContext->sT9awFieldInfo, 0, sizeof(T9AWFieldInfo) );

    /* Initialize the T9 system */
    sT9Status = T9AWInitialize(&pContext->sT9awFieldInfo,
                             (T9SYMB*)MALLOC(MAX_BUFFER_LENGTH * sizeof(T9SYMB)),
                             (T9AuxType*)MALLOC(MAX_BUFFER_LENGTH * sizeof(T9AuxType)),
                             MAX_BUFFER_LENGTH,
                             0 /* nBufLen */,
                             T9_AW_HandleRequest,
                             T9ReadLdbData);
                             
    if ( T9STATNONE == sT9Status )
    {
        T9ResetTextBuf(&pContext->sT9awFieldInfo.G, MAX_BUFFER_LENGTH);
    }

    if ( T9STATNONE != sT9Status )
    {
        MSG_FATAL("...Failed T9 Init. Status code = %d", sT9Status,0,0);
    }
}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9_AW_Destroy
 *  Description     : De-allocates memory allcoated for T9.
 *
 *                    Should be called during shutdown phase of system.
 *
 *  Pass            : void
 *
 *  Return          : void
 *
 *-----------------------------------------------------------------------*/
static void T9_AW_Destroy(TextCtlContext *pContext)
{
    FREEIF(pContext->sT9awFieldInfo.G.psTxtBuf);
    FREEIF(pContext->sT9awFieldInfo.G.paAuxBuf);
}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9_AW_DisplayText
 *  Description     : .
 *
 *                   
 *
 *  Pass            : void
 *
 *  Return          : boolean
 *
 *-----------------------------------------------------------------------*/
static boolean T9_AW_DisplayText(TextCtlContext *pContext, AVKType key)
{
    T9UINT  nBufLen;
    T9UINT  nCursor;
    boolean bModified = FALSE;
    AECHAR kInsert[2] = {0,0};

    nCursor = pContext->sT9awFieldInfo.G.nCursor;
    nBufLen = pContext->sT9awFieldInfo.G.nBufLen;

     if ( !pContext->wMaxChars || nBufLen <= pContext->wMaxChars ) 
     {
        // don't forget to include the
        // NULL character!0
        AECHAR *pNewContents;
        pNewContents = (AECHAR *) OEM_Realloc(pContext->pszContents,
                                       sizeof(AECHAR) *
                                       (nBufLen + 1));
        if ( !pNewContents )
        {
           // out of memory, so just ignore the character
           return FALSE;
        }
        pContext->pszContents = pNewContents;
        
        if((TEXT_MODE_MULTITAP == OEM_TextGetCurrentMode((OEMCONTEXT)pContext))
            &&(key >= AVK_0 && key <= AVK_9))
        {
            if(pContext->dwProperties & TP_STARKEY_SWITCH)  // ��ĸ���뷨�°�*�������л�
            {
         #ifndef FEATURE_LANG_PORTUGUESE
                if(AVK_0 == key)    
         #endif
                {
                    pContext->uModeInfo.mtap.nMax = WSTRLEN((AECHAR*)saMultitapStrings[(int)key-(int)AVK_0]);               
                    kInsert[0] = saMultitapStrings[(int)key - (int)AVK_0][pContext->uModeInfo.mtap.nSubChar];  
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                    if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                    {
                        pContext->uModeInfo.mtap.nSubChar = 0;
                    } 
                }
#ifdef FEATURE_CARRIER_CHINA_TELCOM
                if (AVK_1== key)
                {
                   pContext->uModeInfo.mtap.nMax = WSTRLEN(sszT9Multitap1SpecialUnicom);
                   kInsert[0] = sszT9Multitap1SpecialUnicom[pContext->uModeInfo.mtap.nSubChar];  
                   pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                   if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                   {
                       pContext->uModeInfo.mtap.nSubChar = 0;
                   }
                }
#endif // FEATURE_CARRIER_CHINA_TELCOM
                
                if(MULTITAP_ALL_SMALL != pContext->nMultitapCaps)
                {
                    kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart];
                    WSTRUPPER(kInsert);
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];                
                }
          #ifdef FEATURE_LANG_PORTUGUESE  
                else if(MULTITAP_ALL_SMALL == pContext->nMultitapCaps)
                {
                    kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart];
                    WSTRLOWER(kInsert);
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];                
                }
          #endif
            }
            else
            {                
                pContext->uModeInfo.mtap.nMax = WSTRLEN((AECHAR*)saMultitapStrings[(int)key-(int)AVK_0]);               
                kInsert[0] = saMultitapStrings[(int)key - (int)AVK_0][pContext->uModeInfo.mtap.nSubChar];  
                pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                {
                    pContext->uModeInfo.mtap.nSubChar = 0;
                }            
            }
        }        
        // �������뷨
        if((TEXT_MODE_T9_RAPID_ENGLISH == OEM_TextGetCurrentMode((OEMCONTEXT)pContext))
            &&((FOCUS_SELECTION== pContext->sFocus) 
                ||(AVK_0 == key) 
                ||(AVK_SELECT == key) 
                ||(AVK_INFO == key)))
            {
                if(MULTITAP_ALL_CAPS == pContext->nMultitapCaps)
                {
                    int i;
                    for(i=0; i<pContext->sT9awFieldInfo.G.nBufLen; i++)
                    {
                        kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart+i];
                        WSTRUPPER(kInsert);
                        pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart+i] = kInsert[0];  
                    }
                }
                else if(MULTITAP_ALL_SMALL != pContext->nMultitapCaps)
                {
                    kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart];
                    WSTRUPPER(kInsert);
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];  
                }
            }

#ifdef FEATURE_T9_MT_SPANISH
 	 if((TEXT_MODE_T9_MT_SPANISH == OEM_TextGetCurrentMode((OEMCONTEXT)pContext))
            &&(key >= AVK_0 && key <= AVK_9))
        {
            if(pContext->dwProperties & TP_STARKEY_SWITCH)  // ��ĸ���뷨�°�*�������л�
            {
                if(AVK_0 == key)    
//         #endif
                {
                    pContext->uModeInfo.mtap.nMax = WSTRLEN((AECHAR*)esaMultitapStrings[(int)key-(int)AVK_0]);               
                    kInsert[0] = esaMultitapStrings[(int)key - (int)AVK_0][pContext->uModeInfo.mtap.nSubChar];  
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                    if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                    {
                        pContext->uModeInfo.mtap.nSubChar = 0;
                    } 
                }
                
                if(MULTITAP_ALL_SMALL != pContext->nMultitapCaps)
                {
                    kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart];
                    WSTRUPPER(kInsert);
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];                
                }
            }
            else
            {                
                pContext->uModeInfo.mtap.nMax = WSTRLEN((AECHAR*)esaMultitapStrings[(int)key-(int)AVK_0]);               
                kInsert[0] = esaMultitapStrings[(int)key - (int)AVK_0][pContext->uModeInfo.mtap.nSubChar];  
                pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                {
                    pContext->uModeInfo.mtap.nSubChar = 0;
                }            
            }
        }
#endif

#ifdef FEATURE_T9_RAPID_SPANISH
	if((TEXT_MODE_T9_RAPID_SPANISH== OEM_TextGetCurrentMode((OEMCONTEXT)pContext))
            &&((FOCUS_SELECTION== pContext->sFocus) 
                ||(AVK_0 == key) 
                ||(AVK_SELECT == key) 
                ||(AVK_INFO == key)))
            {
                if(MULTITAP_ALL_CAPS == pContext->nMultitapCaps)
                {
                    int i;
                    for(i=0; i<pContext->sT9awFieldInfo.G.nWordLen; i++)
                    {
                        kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart+i];
                        WSTRUPPER(kInsert);
                        pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart+i] = kInsert[0];  
                    }
                }
                else if(MULTITAP_ALL_SMALL != pContext->nMultitapCaps)
                {
                    kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart];
                    WSTRUPPER(kInsert);
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];  
                }
            }
#endif
	
#ifdef FEATURE_T9_MT_FRENCH
 	 if((TEXT_MODE_T9_MT_FRENCH == OEM_TextGetCurrentMode((OEMCONTEXT)pContext))
            &&(key >= AVK_0 && key <= AVK_9))
        {
            if(pContext->dwProperties & TP_STARKEY_SWITCH)  // ��ĸ���뷨�°�*�������л�
            {
                if(AVK_0 == key)
                {
                    pContext->uModeInfo.mtap.nMax = WSTRLEN((AECHAR*)fraMultitapStrings[(int)key-(int)AVK_0]);               
                    kInsert[0] = fraMultitapStrings[(int)key - (int)AVK_0][pContext->uModeInfo.mtap.nSubChar];  
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                    if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                    {
                        pContext->uModeInfo.mtap.nSubChar = 0;
                    } 
                }
                
                if(MULTITAP_ALL_SMALL != pContext->nMultitapCaps)
                {
                    kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart];
                    WSTRUPPER(kInsert);
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];                
                }
            }
            else
            {                
                pContext->uModeInfo.mtap.nMax = WSTRLEN((AECHAR*)fraMultitapStrings[(int)key-(int)AVK_0]);               
                kInsert[0] = fraMultitapStrings[(int)key - (int)AVK_0][pContext->uModeInfo.mtap.nSubChar];  
                pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                {
                    pContext->uModeInfo.mtap.nSubChar = 0;
                }            
            }
        }
#endif

#ifdef FEATURE_T9_RAPID_FRENCH
	if((TEXT_MODE_T9_RAPID_FRENCH== OEM_TextGetCurrentMode((OEMCONTEXT)pContext))
            &&((FOCUS_SELECTION== pContext->sFocus) 
                ||(AVK_0 == key) 
                ||(AVK_SELECT == key) 
                ||(AVK_INFO == key)))
            {
                if(MULTITAP_ALL_CAPS == pContext->nMultitapCaps)
                {
                    int i;
                    for(i=0; i<pContext->sT9awFieldInfo.G.nWordLen; i++)
                    {
                        kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart+i];
                        WSTRUPPER(kInsert);
                        pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart+i] = kInsert[0];  
                    }
                }
                else if(MULTITAP_ALL_SMALL != pContext->nMultitapCaps)
                {
                    kInsert[0] = pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart];
                    WSTRUPPER(kInsert);
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];  
                }
            }
#endif

#ifdef FEATURE_T9_MT_THAI  
        if(TEXT_MODE_T9_MT_THAI == OEM_TextGetCurrentMode((OEMCONTEXT)pContext))
        {
#ifdef FEATURE_CARRIER_THAILAND_HUTCH           
            if (AVK_2 == key)
            {
                    pContext->uModeInfo.mtap.nMax = WSTRLEN(sszT9Multitap2SpecialThai);
                    kInsert[0] = sszT9Multitap2SpecialThai[pContext->uModeInfo.mtap.nSubChar];  
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                    if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                    {
                        pContext->uModeInfo.mtap.nSubChar = 0;
                    }
            }
#endif //FEATURE_CARRIER_THAILAND_HUTCH
            if (AVK_0 == key)
            {
                if(!isThaiCnsntChar(pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart-1]))
                {
                    pContext->uModeInfo.mtap.nMax = WSTRLEN(sszT9Multitap0SpecialThai);
                    kInsert[0] = sszT9Multitap0SpecialThai[pContext->uModeInfo.mtap.nSubChar];  
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                    if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                    {
                        pContext->uModeInfo.mtap.nSubChar = 0;
                    }
                }
            }
            
            if(AVK_POUND == key)
            {
                if(!isThaiCnsntChar(pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart-1]) 
                    && !isThaiAboveVowelChar(pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart-1]) 
                    && !isThaiBelowVowelChar(pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart-1]))
                {                        
                    pContext->uModeInfo.mtap.nMax = WSTRLEN(sszT9MultitapPound1SpecialThai);
                    kInsert[0] = sszT9MultitapPound1SpecialThai[pContext->uModeInfo.mtap.nSubChar];  
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                    if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                    {
                        pContext->uModeInfo.mtap.nSubChar = 0;
                    }
                } 
                else if(!isThaiCnsntChar(pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart-1]))
                {
                    pContext->uModeInfo.mtap.nMax = WSTRLEN(sszT9MultitapPound2SpecialThai);
                    kInsert[0] = sszT9MultitapPound2SpecialThai[pContext->uModeInfo.mtap.nSubChar];  
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                    if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                    {
                        pContext->uModeInfo.mtap.nSubChar = 0;
                    }                    
                }
            }
        }
#endif  /*FEATURE_T9_MT_THAI*/   

#ifdef FEATURE_PREPAID_ISRAEL_HEBREW
 	     if (AVK_1== key)
            {
                    pContext->uModeInfo.mtap.nMax = WSTRLEN(sszT9Multitap1SpecialHebrew);
                    kInsert[0] = sszT9Multitap1SpecialHebrew[pContext->uModeInfo.mtap.nSubChar];  
                    pContext->sT9awFieldInfo.G.psTxtBuf[pContext->wSelStart] = kInsert[0];
                    if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
                    {
                        pContext->uModeInfo.mtap.nSubChar = 0;
                    }
            }
#endif  // FEATURE_PREPAID_ISRAEL_HEBREW

        SymbToAECHARNCopy ( pContext->pszContents, 
                            pContext->sT9awFieldInfo.G.psTxtBuf, 
                            nBufLen );
                            
        pContext->pszContents[nBufLen] = 0; 
        pContext->wContentsChars = nBufLen;   

        // Update the selection to be after the new character
        if(FOCUS_TEXT == pContext->sFocus)
        {
            pContext->wSelEnd = nCursor ;
            pContext->wSelStart = nCursor ;
        }
        else
        {
            pContext->wSelEnd = nCursor ;            
        }
        
        bModified = TRUE;
    }
      
    if ( bModified ) 
    {         
        // Now re-calc and re-draw
        TextCtl_TextChanged(pContext);
    }
    return bModified;

}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9_BrewKeyToT9AlphabeticKey
 *  Description     : Convert Brew Key to T9 Key.
 *
 *
 *  Pass            : AVKType key
 *
 *  Return          : T9KEY
 *
 *-----------------------------------------------------------------------*/
static T9KEY T9_BrewKeyToT9AlphabeticKey(TextCtlContext *pContext,AEEEvent eCode, AVKType cKey)
{
    uint16 i;
    switch ( OEM_TextGetCurrentMode((OEMCONTEXT)pContext) )
    {    
#ifdef FEATURE_T9_MT_THAI     
        case TEXT_MODE_T9_MT_THAI:
           for (i = 0; ThaiAlphabetic2T9Map[i].cKey != 0; i++) 
            {
                if (ThaiAlphabetic2T9Map[i].cKey == cKey)
                {
                    return ThaiAlphabetic2T9Map[i].mKey;
                }
            }            
            break;            
#endif //FEATURE_T9_MT_THAI  

#ifdef FEATURE_T9_RAPID_THAI    
        case TEXT_MODE_T9_RAPID_THAI:    
           for (i = 0; ThaiAlphabetic2T9Map[i].cKey != 0; i++) 
            {
                if (ThaiAlphabetic2T9Map[i].cKey == cKey)
                {
                    return ThaiAlphabetic2T9Map[i].mKey;
                }
            }            
            break;
#endif //FEATURE_T9_RAPID_THAI

#ifdef FEATURE_T9_MULTITAP    
        case TEXT_MODE_MULTITAP:    
#ifdef FEATURE_T9_MT_SPANISH
	 case TEXT_MODE_T9_MT_SPANISH:
#endif
#ifdef FEATURE_T9_MT_FRENCH
        case TEXT_MODE_T9_MT_FRENCH:
#endif

           for (i = 0; MultitapAlphabetic2T9Map[i].cKey != 0; i++) 
            {
                if (MultitapAlphabetic2T9Map[i].cKey == cKey)
                {
                    return MultitapAlphabetic2T9Map[i].mKey;
                }
            }            
            break;
#endif //FEATURE_T9_MULTITAP

        default:  
            for (i = 0; Alphabetic2T9Map[i].cKey != 0; i++) 
            {
                if (Alphabetic2T9Map[i].cKey == cKey)
                {
                    return Alphabetic2T9Map[i].mKey;
                }
            }        
            break;
    }
       
    return T9KEYNONE;    
}
#endif //#ifdef FEATURE_T9_ALPHABETIC

#ifdef FEATURE_T9_CHINESE
/*------------------------------------------------------------------------
 *
 *  Function name   : 
 *  Description     : 
 *
 *  Pass            : 
 *
 *  Return          : 
 *
 *-----------------------------------------------------------------------*/
static void T9TextCtl_CJK_CHINESE_Restart(TextCtlContext *pContext)
{
    T9STATUS sT9Status = T9STATERROR;  
    // TRI Chinese input Init
    sT9Status = T9_CJK_CHINESE_Init ( pContext );

    // set rectChinese input Rect
    pContext->rectChineseSyllableInput.x = pContext->rectDisplay.x;
    pContext->rectChineseSyllableInput.dx = pContext->rectDisplay.dx -2;
    pContext->rectChineseSyllableInput.dy = pContext->nLineHeight + 4;    
    pContext->rectChineseSyllableInput.y = pContext->rectDisplay.y + pContext->rectDisplay.dy - pContext->rectChineseSyllableInput.dy*2;

    pContext->rectChineseTextInput.x = pContext->rectChineseSyllableInput.x;
    pContext->rectChineseTextInput.dx = pContext->rectChineseSyllableInput.dx;
    pContext->rectChineseTextInput.dy = pContext->nLineHeight + 5;    
    pContext->rectChineseTextInput.y = pContext->rectDisplay.y + pContext->rectDisplay.dy - pContext->rectChineseTextInput.dy;    

    pContext->rectChineseInput.x = pContext->rectChineseSyllableInput.x;
    pContext->rectChineseInput.dx = pContext->rectChineseSyllableInput.dx;
    pContext->rectChineseInput.dy = pContext->rectChineseSyllableInput.dy + pContext->rectChineseTextInput.dy;    
    pContext->rectChineseInput.y = pContext->rectDisplay.y + pContext->rectDisplay.dy - pContext->rectChineseInput.dy;            
    
    // re-setup Text Buffer
    if ( T9STATNONE == sT9Status )
    {
        sT9Status = T9ResetTextBuf(&pContext->sT9ccFieldInfo.G, MAX_BUFFER_LENGTH);
    }
    if ( T9STATNONE == sT9Status )
    {
        sT9Status = T9AddTextString ( &pContext->sT9ccFieldInfo.G, 
                                      pContext->pszContents, 
                                      pContext->wContentsChars );
    }
    
    // set Cursor
    if ( T9STATNONE == sT9Status )
    {
        T9Cursor ( &pContext->sT9ccFieldInfo.G, T9CA_FROMBEG, pContext->wSelEnd );
    }
        
    TextCtl_NoSelection(pContext);
    TextCtl_TextChanged(pContext);
    pContext->nSelectionSelectd = 0;       // no default selected word   
    pContext->sFocus = FOCUS_TEXT;
}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9TextCtl_CJK_CHINESE_Key
 *  Description     : handle key event
 *
 *  Pass            : 
 *
 *  Return          : 
 *
 *-----------------------------------------------------------------------*/
static boolean T9TextCtl_CJK_CHINESE_Key(TextCtlContext *pContext, AEEEvent eCode,AVKType key)
{
    T9U16  wT9KeyType = T9KEYTYPE_UNKNOWN;
    T9KEY  mKey = T9KEYNONE;
    T9SYMB sExplSym;
    unsigned int uWordCount = 0;
    T9STATUS sStatus = T9STATERROR;
    boolean bRet = TRUE;

    // discard the event that we don't handle
    if ( key == AVK_SEND )
    {
        return FALSE;
    }  

    mKey = T9_CJK_CHINESE_BrewKeyToT9Key (pContext, key );

    if ( pContext->wMaxChars != 0 && 
         pContext->sT9ccFieldInfo.G.nBufLen >= pContext->wMaxChars &&
         (( mKey >= T9KEYAMBIG1 && mKey <= T9KEYAMBIGC) 
         || T9KEYSPACE == mKey
         || AVK_SELECT == key 
         || AVK_INFO == key ) )
    { 
        // meet the max count of the text.
        sStatus = T9STATERROR; 
        return FALSE;
    }        

    switch ( key ) 
    {
        /* Assign zhuyin */
        case AVK_1:
        case AVK_2:
        case AVK_3:
        case AVK_4:
        case AVK_5:
        case AVK_6:        
        case AVK_7:    
        case AVK_8:    
        case AVK_9:    
        case AVK_0:     
        case AVK_POUND:            
        case AVK_STAR:    
            if(T9KEYSPACE == mKey)
            {
                pContext->sFocus = FOCUS_TEXT;                
                wT9KeyType = T9KEYTYPE_CONTROL;   
            }            
            else if(T9KEYNONE != mKey)
            {
                wT9KeyType = T9KEYTYPE_NORMAL;
            }
            break;       
        
        case AVK_CLR:
            if ( FOCUS_SELECTION == pContext->sFocus )
            {
                if ( pContext->sT9ccFieldInfo.nKeyBufLen == 0  )
                {
                    pContext->sFocus = FOCUS_TEXT_PREDICTION;
                }
                else 
                {
                    pContext->sFocus = FOCUS_SYLLABLE;
                }
                pContext->nSelectionSelectd = 0; 
                T9_CJK_CHINESE_DisplaySyllable ( pContext );
                T9_CJK_CHINESE_DisplaySelection ( pContext );
            }    
             if ( FOCUS_TEXT_PREDICTION == pContext->sFocus )  
            {
                pContext->sFocus = FOCUS_TEXT;
                T9_CJK_CHINESE_DisplayText ( pContext );// update cursor.
                T9_CJK_CHINESE_DisplaySelection ( pContext );
            }
            else if ( FOCUS_TEXT == pContext->sFocus &&
                      0 == pContext->wSelStart && 
                      pContext->wSelStart == pContext->wSelEnd )
            {
                // need to retrue FALSE;
                return FALSE;
            }
            else // focus on BPMF text or on TEXT
            {
                mKey = T9KEYCLEAR;
                wT9KeyType = T9KEYTYPE_CONTROL;   
            }
            
            break;
        
        case AVK_SELECT:   
        case AVK_INFO: 	  
            if ( FOCUS_SYLLABLE == pContext->sFocus )
            {
                pContext->sFocus = FOCUS_SELECTION;
                pContext->nSelectionSelectd = 0; 
                T9_CJK_CHINESE_DisplaySyllable  ( pContext );
                T9_CJK_CHINESE_DisplaySelection ( pContext );
            }                      
            else if ( FOCUS_SELECTION == pContext->sFocus ) // focus on Candidate text
            {
                mKey = pContext->nSelectionSelectd;
                wT9KeyType = T9KEYTYPE_SELECT;   
            }
            else if ( FOCUS_TEXT_PREDICTION == pContext->sFocus )
            {
                pContext->sFocus = FOCUS_SELECTION;
                pContext->nSelectionSelectd = 0;
                T9_CJK_CHINESE_DisplaySelection ( pContext );
            }            
            else 
                bRet = FALSE;
            break;
        
        case AVK_RIGHT:
            // the focus is in BPMF stirng, no Selectedin Selection string
            if ( FOCUS_SYLLABLE == pContext->sFocus ) 
            {
                if ( pContext->sT9ccFieldInfo.nKeyBufLen == 0 ) 
                {
                    return FALSE;
                }
                uWordCount = (STRLEN((char*)pContext->sT9ccFieldInfo.pbSpellBuf)+1)/
                             (pContext->sT9ccFieldInfo.nKeyBufLen+1);
                if ( uWordCount > 1 ) 
                {
                    T9UINT nValue = pContext->sT9ccFieldInfo.bSpellCode;
                    if ( nValue < 1 ) 
                    {
                        nValue = 1;
                    }
                    
                    nValue++;
                    if ( nValue > uWordCount )
                    {
                        nValue = 1;
                    }
                    sStatus = T9CCSpecifySpell(&pContext->sT9ccFieldInfo, nValue);
                    
                }
                T9_CJK_CHINESE_DisplaySyllable  (pContext);
                T9_CJK_CHINESE_DisplaySelection (pContext);
            }
            else if ( FOCUS_SELECTION == pContext->sFocus )
            {
                if  ((T9UINT) pContext->nSelectionSelectd < pContext->sT9ccFieldInfo.nSelectPageLen-1 )
                {
                    pContext->nSelectionSelectd ++;
                }
                else
                    pContext->nSelectionSelectd = 0;
                T9_CJK_CHINESE_DisplaySelection (pContext);
            }            
            else if ( FOCUS_TEXT_PREDICTION == pContext->sFocus || FOCUS_TEXT == pContext->sFocus )
            {
                if (OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
                {
                    T9Cursor ( &pContext->sT9ccFieldInfo.G, T9CA_FROMBEG, 0 );
                    T9_CJK_CHINESE_DisplayText ( pContext );
                }
                else
                {
                    mKey = T9KEYRIGHT;
                    wT9KeyType = T9KEYTYPE_CONTROL;   
                }
            }        
            break;        
        
        case AVK_LEFT:            
            if ( FOCUS_SYLLABLE == pContext->sFocus ) 
            {
                if (pContext->sT9ccFieldInfo.nKeyBufLen == 0) 
                {
                    return FALSE;
                }
                uWordCount = (STRLEN((char*)pContext->sT9ccFieldInfo.pbSpellBuf)+1)/
                             (pContext->sT9ccFieldInfo.nKeyBufLen+1);
                if ( uWordCount > 1 ) 
                {
                    T9UINT nValue = pContext->sT9ccFieldInfo.bSpellCode;
                    nValue--;
                    if (nValue < 1) 
                    {
                        nValue = uWordCount; /* wrap arround */
                    }                
                    sStatus = T9CCSpecifySpell ( &pContext->sT9ccFieldInfo, nValue );
                }
                T9_CJK_CHINESE_DisplaySyllable  (pContext);
                T9_CJK_CHINESE_DisplaySelection (pContext);
            }                    
            else if ( FOCUS_SELECTION == pContext->sFocus )
            {
                if ( pContext->nSelectionSelectd > 0 )
                {
                    pContext->nSelectionSelectd --;
                }
                else
                    pContext->nSelectionSelectd = pContext->sT9ccFieldInfo.nSelectPageLen-1;
                T9_CJK_CHINESE_DisplaySelection (pContext);
            }                
            else if ( FOCUS_TEXT_PREDICTION == pContext->sFocus || 
                      FOCUS_TEXT == pContext->sFocus )
            {
                if (OEM_TextGetCursorPos(pContext) == 0)
                {
                    T9Cursor ( &pContext->sT9ccFieldInfo.G, T9CA_FROMEND, 0 );
                    T9_CJK_CHINESE_DisplayText ( pContext );
                }
                else    
                {
                    mKey = T9KEYLEFT;
                    wT9KeyType = T9KEYTYPE_CONTROL;   
                }
            }
            break;
            
        case AVK_UP:       
            if ( FOCUS_SYLLABLE == pContext->sFocus ) 
            {
                pContext->sFocus = FOCUS_SELECTION;
                pContext->nSelectionSelectd = 0; 
                T9_CJK_CHINESE_DisplaySyllable  ( pContext );
                T9_CJK_CHINESE_DisplaySelection ( pContext );
            }                      
            else if ( FOCUS_TEXT_PREDICTION == pContext->sFocus ||FOCUS_TEXT == pContext->sFocus)
            {
                uint16 nLine, nCharsIn,nSel,lineChars;
                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);
        
                // If it is on the first line, return false
                if(nLine == 0 || !pContext->pwLineStarts)
                    return FALSE;
        
                // Otherwise figure out how many characters from the start
                // of the line the cursor is and try to put the cursor in a
                // similar position on previous line. Or, if not enough
                // chars, at the end of the line
                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
                                               pContext->pwLineStarts[nLine]) 
                {
                    nSel = pContext->pwLineStarts[nLine]-1;
                } 
                else 
                {
                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
                }
                lineChars = pContext->wSelEnd - nSel;
                T9Cursor ( &pContext->sT9ccFieldInfo.G, 
                            T9CA_MOVELEFT, 
                            lineChars );                                           
                T9_CJK_CHINESE_DisplayText ( pContext );
            }
            else
            {
                mKey = T9KEYPREV;
                wT9KeyType = T9KEYTYPE_CONTROL;   
            }
            break;
            
        case AVK_DOWN:
            if ( FOCUS_SYLLABLE == pContext->sFocus )
            {
                pContext->sFocus = FOCUS_SELECTION;
                pContext->nSelectionSelectd = 0; 
                T9_CJK_CHINESE_DisplaySyllable  ( pContext );
                T9_CJK_CHINESE_DisplaySelection ( pContext );
            }                      
           else if ( FOCUS_TEXT_PREDICTION == pContext->sFocus ||   
                 FOCUS_TEXT == pContext->sFocus )
            {
                uint16 nLine, nCharsIn,nSel,lineChars;
            
                if((!pContext->pwLineStarts)||(!pContext->wLines))
                    return FALSE;
                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);
            
                // If the cursor is on the last line and the line's last
                // character is not a LF, then FALSE is returned as nothing
                // can be done. A LF on the end of a line does not tell the
                // wLines member that there is another line, hence this
                // extra check.
                if ( nLine == (pContext->wLines-1) &&
                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
                {
                    return FALSE;
                }
            
                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
                // If the cursor is more characters in than the next line...
                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
                {
                    // If it is the last line, don't subtract the LINEBREAK from selection spot
                    if( nLine+2 == pContext->wLines )
                    {
                        nSel = pContext->pwLineStarts[nLine+2];
                    }
                    else
                    {
                        nSel = pContext->pwLineStarts[nLine+2]-1;
                    }
                }
                else
                {
                    // Selection spot is number of chars into the next line
                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
                    // If this is not the beginning of a line 
                    // and the selection point is a LINEBREAK, subtract one
                    // Otherwise the selection overshoots to the first character
                    // of the following line.
                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
                    {
                        nSel--;
                    }
                }
                lineChars = nSel - pContext->wSelEnd;
                T9Cursor ( &pContext->sT9ccFieldInfo.G, 
                            T9CA_MOVERIGHT, 
                            lineChars );                                 
                T9_CJK_CHINESE_DisplayText ( pContext );
            }
            else
            {    
                mKey = T9KEYNEXT;    
                wT9KeyType = T9KEYTYPE_CONTROL;   
            }
            break;

        default:
            break;
    }

    /* Now take action depending on what type of key was pressed */
    switch ( wT9KeyType ) 
    {
        /* This was an explicit select key event.  Select one of the characters from the
         * character selection matrix using the T9EVTEXPL event */
        case T9KEYTYPE_SELECT:

            if ( (T9UINT) mKey < pContext->sT9ccFieldInfo.nSelectPageLen &&
                 (T9UINT) mKey < pContext->sT9ccFieldInfo.nSelectPageMax ) 
            {
                sExplSym = pContext->sT9ccFieldInfo.pwSelectPage[mKey];
                
                // add Char to TextCtrl
                sStatus = T9AddExplicitChar ( &pContext->sT9ccFieldInfo.G, sExplSym );
                if ( T9STATNONE == sStatus ) 
                {
                    pContext->sFocus = FOCUS_SELECTION;
                    pContext->nSelectionSelectd = 0;
                    T9_CJK_CHINESE_DisplayText ( pContext );
                    T9_CJK_CHINESE_DisplaySyllable  ( pContext );
                    T9_CJK_CHINESE_DisplaySelection ( pContext );
                }
            }
            else 
            {
                sStatus = T9STATNOWRD;
            }
            
            break;

        case T9KEYTYPE_CONTROL:            
            sStatus = T9HandleKey ( &pContext->sT9ccFieldInfo.G, mKey );
            
            // nKeyBufLen is 0, and the clr pressed and the sFocus is SYLLABLE ,
            // then transfer to FOCUS_TEXT_PREDICTION
            if ( mKey == T9KEYCLEAR &&
                 FOCUS_SYLLABLE == pContext->sFocus &&
                 0 == pContext->sT9ccFieldInfo.nKeyBufLen )
            {
                pContext->sFocus = FOCUS_TEXT;
            }
            
            // when sFocus on TEXT, TEXT_PRIDICTION,
            if ( FOCUS_TEXT == pContext->sFocus )
            {
                T9_CJK_CHINESE_DisplayText ( pContext );// update cursor.
            }
            T9_CJK_CHINESE_DisplaySyllable  (pContext);
            T9_CJK_CHINESE_DisplaySelection (pContext);
            break;
            
        /* All other keystrokes go directly to the T9 core via the T9EVTKEY event */
        case T9KEYTYPE_NORMAL:
            /* since the core will wrap to the last page and we don't want
             * that behavior, we pre check if we already reach the first page. if
             * we already reach the first page, we'll just set the status to end
             * and do nothing
             */
             
            pContext->nSelectionSelectd = 0;
            
            sStatus = T9HandleKey ( &pContext->sT9ccFieldInfo.G, mKey );
            
            // sFocus is TEXT and nKeyBufLen have changed to non zero.
            // this case, the nKeyBufLen should be 1.
            if ( pContext->sT9ccFieldInfo.nKeyBufLen >= 1 )
            {
                pContext->sFocus = FOCUS_SYLLABLE;
                T9_CJK_CHINESE_DisplayText ( pContext );// update cursor.                
            }
            // when sFocus on SELECTION, TEXT, TEXT_PRIDICTION, SYLLABLE
            T9_CJK_CHINESE_DisplaySyllable  (pContext);
            T9_CJK_CHINESE_DisplaySelection (pContext);
            
            break;

        default:
            break;
    }
    
    return bRet;
}

/*------------------------------------------------------------------------
 *
 *  Function name   : 
 *  Description     : 
 *
 *  Pass            : 
 *
 *  Return          : 
 *
 *-----------------------------------------------------------------------*/
static void T9TextCtl_CJK_CHINESE_Exit(TextCtlContext *pContext)
{
    T9_CJK_CHINESE_Destroy ( pContext );    
    TextCtl_NoSelection(pContext);
}

/*------------------------------------------------------------------------
 *
 *    Callback routines necessary to link to the T9 Core.
 *
 *------------------------------------------------------------------------*/
T9STATUS T9FARCALL T9_CJK_CHINESE_HandleRequest(T9FieldInfo *pFieldInfo, T9Request *pRequest)
{

    T9STATUS sStatus = T9STATERROR;

    switch(pRequest->eType) 
    {
       case T9REQRELOADCAUDB:
            /* note: in the multi-thread environment, need to do some thread safe
            * here */
            if ((pFieldInfo->nLdbNum & T9PIDMASK) == T9PIDChinese) 
            {
                    if ((pFieldInfo->nLdbNum & T9SIDMASK) == T9SIDChineseTraditional) 
                    {
                        ((T9CCFieldInfo*) pFieldInfo)->pCAUdbInfo = gpTradCAUdbInfo;
                    }
                    else if ((pFieldInfo->nLdbNum & T9SIDMASK) == T9SIDChineseSimplified) 
                    {
                        ((T9CCFieldInfo*) pFieldInfo)->pCAUdbInfo = gpSimpCAUdbInfo;
                    }
            }
            sStatus = T9STATNONE;
            break;
            
       default:
           sStatus = T9STATNONE;

           break;
    }

    return sStatus;

}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9_CJK_CHINESE_Init
 *  Description     : Allocates and initializes all data structures
 *                    necessary for T9 operation.  Calls Init API function
 *                    to start T9 in the proper mode.
 *
 *                    This routine should be called during the initialization
 *                    process of the system.
 *
 *  Pass            : void
 *
 *  Return          : void
 *
 *-----------------------------------------------------------------------*/
static T9STATUS T9_CJK_CHINESE_Init(TextCtlContext *pContext)
{
    T9STATUS sStatus = T9STATERROR;
    
    /* for CAUDB initialization */
    gpSimpCAUdbInfo = (T9CCAudbInfo *)malloc(CAUDB_SIZE);
    gpTradCAUdbInfo = (T9CCAudbInfo *)malloc(CAUDB_SIZE);

    /* Allocate and initialize the field structure */
    MEMSET(&pContext->sT9ccFieldInfo, 0, sizeof(T9CCFieldInfo));

    sStatus = T9CCInitialize(&pContext->sT9ccFieldInfo, 
                            (T9SYMB*)malloc(MAX_BUFFER_LENGTH * sizeof(T9SYMB)),
                            (T9AuxType*)malloc(MAX_BUFFER_LENGTH * sizeof(T9AuxType)),
                            MAX_BUFFER_LENGTH, 
                            0 /* nBufLen */, 
                            T9_CJK_CHINESE_HandleRequest, 
                            SELECTION_BUFFER_SIZE, 
                            T9CCLoadLdb);
    
    if ( T9STATNONE == sStatus ) 
    {
        switch ( OEM_TextGetCurrentMode((OEMCONTEXT)pContext) )
        {    
#ifdef  FEATURE_T9_PINYIN     
            case TEXT_MODE_T9_PINYIN:
                    sStatus = T9CCSetLanguage(&pContext->sT9ccFieldInfo, T9PIDChinese | T9SIDChineseSimplified, 0, 0);
                    if ( T9STATNONE == sStatus ) 
                    {
                        sStatus = T9CCAudbActivate(&pContext->sT9ccFieldInfo, gpSimpCAUdbInfo,
                                                       CAUDB_SIZE, 1);
                    }   
                    //T9_CJK_CHINESE_GetCoreVerString(pContext);   
                     if ( T9STATNONE == sStatus ) 
                    {
                        sStatus = T9CCSetAmbigSpellMode(&pContext->sT9ccFieldInfo);
                    }   
                    break;
#endif //FEATURE_T9_PINYIN

#ifdef  FEATURE_T9_ZHUYIN     
            case TEXT_MODE_T9_ZHUYIN:
                    sStatus = T9CCSetLanguage(&pContext->sT9ccFieldInfo, T9PIDChinese | T9SIDChineseTraditional, 0, 0);
                    if ( T9STATNONE == sStatus ) 
                    {
                        sStatus = T9CCAudbActivate(&pContext->sT9ccFieldInfo, gpTradCAUdbInfo,
                                                       CAUDB_SIZE, 1);
                    }    
                    //T9_CJK_CHINESE_GetCoreVerString(pContext);   
                     if ( T9STATNONE == sStatus ) 
                    {
                        sStatus = T9CCSetAmbigSpellMode(&pContext->sT9ccFieldInfo);
                    }                       
                    break;
#endif //FEATURE_T9_ZHUYIN

#ifdef  FEATURE_T9_STROKE     
            case TEXT_MODE_T9_STROKE:
#if defined FEATURE_LANG_TCHINESE                
                    sStatus = T9CCSetLanguage(&pContext->sT9ccFieldInfo, T9PIDChinese | T9SIDChineseTraditional, 0, 0);
                    if ( T9STATNONE == sStatus ) 
                    {
                        sStatus = T9CCAudbActivate(&pContext->sT9ccFieldInfo, gpTradCAUdbInfo,
                                                       CAUDB_SIZE, 1);
                    }                                                                           
#elif defined FEATURE_LANG_CHINESE //AEE_SIMULATOR                        
                    sStatus = T9CCSetLanguage(&pContext->sT9ccFieldInfo, T9PIDChinese | T9SIDChineseSimplified, 0, 0);
                    if ( T9STATNONE == sStatus ) 
                    {
                        sStatus = T9CCAudbActivate(&pContext->sT9ccFieldInfo, gpSimpCAUdbInfo,
                                                       CAUDB_SIZE, 1);
                    }                              
#endif //FEATURE_LANG_TCHINESE
                    //T9_CJK_CHINESE_GetCoreVerString(pContext);            
                    break;
#endif //FEATURE_T9_STROKE

            default:
                    sStatus = T9STATERROR;                     
                    break;
        }
    }    
    return sStatus;
}

/*------------------------------------------------------------------------
 *
 *  Function name   : DestroyT9
 *  Description     : De-allocates memory allcoated for T9.
 *
 *                    Should be called during shutdown phase of system.
 *
 *  Pass            : void
 *
 *  Return          : void
 *
 *-----------------------------------------------------------------------*/
static void T9_CJK_CHINESE_Destroy(TextCtlContext *pContext)
{
    FREEIF(pContext->sT9ccFieldInfo.G.psTxtBuf);
    FREEIF(pContext->sT9ccFieldInfo.G.paAuxBuf);
    FREEIF(gpSimpCAUdbInfo);
    FREEIF(gpTradCAUdbInfo);
}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9_CJK_CHINESE_BrewKeyToT9Key
 *  Description     : Convert Brew Key to T9 Key.
 *
 *
 *  Pass            : AVKType key
 *
 *  Return          : T9KEY
 *
 *-----------------------------------------------------------------------*/
static T9KEY T9_CJK_CHINESE_BrewKeyToT9Key(TextCtlContext *pContext, AVKType cKey)
{
    uint16 i;

    switch ( OEM_TextGetCurrentMode((OEMCONTEXT)pContext) )
    {    
#ifdef  FEATURE_T9_PINYIN     
        case TEXT_MODE_T9_PINYIN:
               for (i = 0; Pinyin2T9Map[i].cKey != 0; i++) 
                {
                    if (Pinyin2T9Map[i].cKey == cKey)
                    {
                        return Pinyin2T9Map[i].mKey;
                    }
                }            
                break;
#endif //FEATURE_T9_PINYIN

#ifdef  FEATURE_T9_ZHUYIN     
        case TEXT_MODE_T9_ZHUYIN:
               for (i = 0; Zhuyin2T9Map[i].cKey != 0; i++) 
                {
                    if (Zhuyin2T9Map[i].cKey == cKey)
                    {
                        return Zhuyin2T9Map[i].mKey;
                    }
                }                                
                break;
#endif //FEATURE_T9_ZHUYIN

#ifdef  FEATURE_T9_STROKE     
        case TEXT_MODE_T9_STROKE:
               for (i = 0; Stroke2T9Map[i].cKey != 0; i++) 
                {
                    if (Stroke2T9Map[i].cKey == cKey)
                    {
                        return Stroke2T9Map[i].mKey;
                    }
                }                                
                break;
#endif //FEATURE_T9_STROKE

        default:                        
                break;
    }
       
    return T9KEYNONE;

}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9_CJK_CHINESE_DisplayText
 *  Description     : .
 *
 *                   
 *
 *  Pass            : void
 *
 *  Return          : boolean
 *
 *-----------------------------------------------------------------------*/
static boolean T9_CJK_CHINESE_DisplayText(TextCtlContext *pContext)
{
    T9UINT  nBufLen;
    T9UINT  nCursor;
    boolean bModified = FALSE;
    boolean bAdjust = FALSE;
   
    if(pContext->rectDisplay.dy > pContext->rectChineseSyllableInput.dy + pContext->rectChineseTextInput.dy)
    {
    if((FOCUS_SELECTION == pContext->sFocus) ||(FOCUS_TEXT_PREDICTION == pContext->sFocus))
    {
        pContext->rectDisplay.dy = pContext->rectDisplay.dy -pContext->rectChineseTextInput.dy+3;     
            bAdjust = TRUE;
    }
    if(FOCUS_SYLLABLE == pContext->sFocus)
    {
        pContext->rectDisplay.dy = pContext->rectDisplay.dy -pContext->rectChineseSyllableInput.dy - pContext->rectChineseTextInput.dy+4;     
            bAdjust = TRUE;
        }
    }

    nCursor = pContext->sT9ccFieldInfo.G.nCursor;
    nBufLen = pContext->sT9ccFieldInfo.G.nBufLen;
    
    if ( !pContext->wMaxChars || nBufLen <= pContext->wMaxChars ) 
    {
        // don't forget to include the
        // NULL character!0
        AECHAR *pNewContents;
        pNewContents = (AECHAR *) OEM_Realloc(pContext->pszContents,
                                       sizeof(AECHAR) *
                                       (nBufLen + 1));
        if ( !pNewContents )
        {
           // out of memory, so just ignore the character
           return FALSE;
        }
        pContext->pszContents = pNewContents;
                     
        SymbToAECHARNCopy ( pContext->pszContents, 
                            pContext->sT9ccFieldInfo.G.psTxtBuf, 
                            nBufLen );
                            
        pContext->pszContents[nBufLen] = 0; 
        pContext->wContentsChars = nBufLen;
        
        // Update the selection to be after the new character
        pContext->wSelEnd = nCursor ;
        pContext->wSelStart = nCursor ;
        
        bModified = TRUE;

    }
      
    if ( bModified ) 
    {         
        // Now re-calc and re-draw
        TextCtl_TextChanged(pContext);
    }

    if((FOCUS_SELECTION == pContext->sFocus) ||(FOCUS_TEXT_PREDICTION == pContext->sFocus))
    {
        if(bAdjust == TRUE)
        {
        pContext->rectDisplay.dy = pContext->rectDisplay.dy + pContext->rectChineseTextInput.dy-3;     
        }
    }
    if(FOCUS_SYLLABLE == pContext->sFocus)
    {
        if(bAdjust == TRUE)
        {
        pContext->rectDisplay.dy = pContext->rectDisplay.dy + pContext->rectChineseSyllableInput.dy + pContext->rectChineseTextInput.dy-4;     
        }
    }    
    return bModified;

}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9_CJK_CHINESE_AdjustInputInfoLocation
 *  Description     : adjust syllable and string location when the height of  text field  is very narrow
 *
 *  Pass            : 
 *
 *  Return          : 
 *
 *-----------------------------------------------------------------------*/
static void T9_CJK_CHINESE_AdjustInputInfoLocation(TextCtlContext *pContext, 
                                                                                                                                unsigned int *pWindX, 
                                                                                                                                unsigned int *pWindY,
                                                                                                                                unsigned int *pWindDx,
                                                                                                                                unsigned int *pWindDy)
{
    if(pContext == NULL ||pWindX ==NULL ||pWindY == NULL)
    {
        return ;
    }
    
    if(pContext->rectDisplay.dy < pContext->rectChineseSyllableInput.dy + pContext->rectChineseTextInput.dy)
    {
       AEEDeviceInfo DeviceInfo;    
       OEM_GetDeviceInfo(&DeviceInfo);
        if(pContext->rectDisplay.y +
            pContext->rectDisplay.dy + 
            pContext->rectChineseSyllableInput.dy +
            pContext->rectChineseTextInput.dy +
            GetBottomBarHeight(pContext->pIDisplay) < DeviceInfo.cyScreen)
        {
            *pWindY += pContext->rectDisplay.dy ;
        }
        else
        {
            *pWindY = MAX(0, (int16)(*pWindY) - pContext->rectDisplay.dy );
        }

        *pWindX =-1;
        *pWindDx = DeviceInfo.cxScreen - 1;
    }
}

/*------------------------------------------------------------------------
 *
 *  Function name   : 
 *  Description     : 
 *
 *  Pass            : 
 *
 *  Return          : 
 *
 *-----------------------------------------------------------------------*/
static void T9_CJK_CHINESE_DrawStrokeString(TextCtlContext *pContext)
{
    unsigned int    k = 0;
    //unsigned int    iWindX = pContext->rectChineseInput.x + 1;
    //unsigned int    iWindY = pContext->rectChineseInput.y + 1;
    unsigned int     iWindX = pContext->rectChineseSyllableInput.x;
    unsigned int     iWindY = pContext->rectChineseSyllableInput.y ;
    unsigned int     iWindDx = pContext->rectChineseSyllableInput.dx;
    unsigned int     iWindDy = pContext->rectChineseSyllableInput.dy ;
    T9U8            *pbBuffer;
    T9U8            bNumOfComp = 0;
    T9UINT           nStrokeLen = 0;
    T9UINT           nStart = 0;
    T9UINT           nStrokeDisLen = 0;

    AECHAR ch[2]={0,0}; 
    uint32 format;
    AEERect  pRect;

    T9_CJK_CHINESE_AdjustInputInfoLocation(pContext, &iWindX, &iWindY, &iWindDx, &iWindDy);
    
/*
    RGBVAL dwOldBkClr;
              
    SETAEERECT(&pRect,
              iWindX,
              iWindY,
              pContext->rectChineseInput.dx-2,
              pContext->nLineHeight);
              
    // ���ñ���ɫ
    dwOldBkClr = IDISPLAY_SetColor((IDisplay *)pContext->pIDisplay,
                           CLR_USER_BACKGROUND,
                           MAKE_RGB(255, 255, 255));
*/
    // setup the Stroke Rect
       SETAEERECT ( &pRect,
                     iWindX+2,   //+T9_FONT_WIDTH,   
                     iWindY+1,
                     iWindDx -4,  
                     iWindDy); 
       // Erase Stroke Rect
       IDISPLAY_EraseRect ( pContext->pIDisplay, &pRect );

    /* len of the stroke buffer, extra 1 for a component */
    nStrokeLen = pContext->sT9ccFieldInfo.nKeyBufLen;

    /*nStrokeDisLen is the length of the strokes and components in spell buffer */
    nStrokeDisLen = pContext->sT9ccFieldInfo.nKeyBufLen;

#ifdef FEATURE_FUNCS_THEME                       
    IDISPLAY_DrawRect(pContext->pIDisplay,
            &pRect,
            pContext->m_themeColor,
            RGB_WHITE,
            IDF_RECT_FRAME);
#else //FEATURE_FUNCS_THEME 
    IDISPLAY_DrawRect(pContext->pIDisplay,
            &pRect,
            RGB_BLACK,
            RGB_WHITE,
            IDF_RECT_FRAME);
#endif //FEATURE_FUNCS_THEME  
    
    /* padding the elipse ..., if stroke len is bigger than the display window */
    if (nStrokeDisLen > MAX_STROKES) 
    {
        format = IDF_ALIGN_NONE;
        ch[0] = 0x003C; //'<'
        (void) IDISPLAY_DrawText((IDisplay *)pContext->pIDisplay,
                               AEE_FONT_NORMAL,
                               ch,
                               -1,
                               iWindX+4,  // iWindX,
                               iWindY+3,  // iWindY,
                               NULL,
                               format);
        iWindX += T9_STROKE_LEFT_ARROW; // T9_STROKE_FONT_WIDTH;

        {
            /* otherwise just move the window to the right */
            nStart = nStrokeLen - MAX_STROKES + 1;
        }
    }

    /* Point to the strokes to draw */
    pbBuffer = pContext->sT9ccFieldInfo.pbKeyBuf + bNumOfComp;

    /* Draw each stroke, starting fromt the nstart */
    for (k = nStart; k < pContext->sT9ccFieldInfo.nKeyBufLen - bNumOfComp; k++) 
    {
        format = IDF_ALIGN_NONE;
        ch[0] = *(pbBuffer+k) + 0x3129;
        (void) IDISPLAY_DrawText((IDisplay *)pContext->pIDisplay,
                               AEE_FONT_NORMAL,
                               ch,
                               -1,
                               iWindX+4,  // iWindX,
                               iWindY+3,  // iWindY,
                               NULL,
                               format);

        /* If this character is a NULL terminator, then stop drawing */
        if (*(pbBuffer + k ) == 0) break;

        /* Advance X position to after this character. */
        iWindX += T9_STROKE_FONT_WIDTH;
    }
}

/*------------------------------------------------------------------------
 *
 *  Function name   :T9_CJK_CHINESE_DisplaySyllable
 *  Description     : Draws the specified syllable to the specified location
 *                    on the graphics screen.
 *                  when KeyBufLen = 2; the Max count of words is 4
 *                  when KeyBufLen = 3; the Max count of words is 3
 *  Pass            : 
 *
 *  Return          : 
 *
 *-----------------------------------------------------------------------*/
static void T9_CJK_CHINESE_DrawSyllableString ( TextCtlContext *pContext )
{
    
    int     iWindX          = pContext->rectChineseSyllableInput.x;
    int     iWindY          = pContext->rectChineseSyllableInput.y ;
    unsigned int     iWindDx = pContext->rectChineseSyllableInput.dx;
    unsigned int     iWindDy = pContext->rectChineseSyllableInput.dy ;
    int     nWordCount      = 0;
    int     nWordCountDisp  = 0;
    int     nSelectedCode   = 0;            // the number of selected in displayed spell code list(base-0)
    int     nSpellBufLen    = 0;
    char   *pbSpellBuffer   = NULL;
    T9UINT  nKeyBufLen      = pContext->sT9ccFieldInfo.nKeyBufLen ;
    uint32  format          = IDF_ALIGN_NONE;
    
    T9UINT  bSpellCode      = 0;
    AECHAR *wszSpellBuf     = NULL;
    AECHAR *wszSpellBufDisp = NULL;
    AECHAR *wszTmp          = NULL;
    AEERect pRect;
    int    iLenTemp[T9CCSPELLBUFMAXSIZE] = {0};	// �洢ÿ���ַ��ĳ���
    int    iCount = 0; // Ϊ�ַ�����
    int    iwszlen = 0;    // �ַ��ĳ���
    int32  iSpellLenTemp[SPELLMAX] = {0};   // �洢��ĸ��ϵĳ���
    int32  iSpellCursX[SPELLMAX] = {0}; // �洢��ĸ��ϵĺ�����λ��
    int     nSelectedCodeTemp = 0;  // ��ĸ������
    static int nSpellCodeStart = 0;         // the start index in the Zhuyin Code list (base-0)
    static int nKeyBufLenOrig  = 0;         // the original len of Key buffer

    T9_CJK_CHINESE_AdjustInputInfoLocation(pContext, (unsigned int *)&iWindX, (unsigned int *)&iWindY, &iWindDx, &iWindDy);
    
    // setup the BMPF Rect
    SETAEERECT ( &pRect,
                  iWindX+2,   //+T9_FONT_WIDTH,   
                  iWindY+1,
                  iWindDx -6,  
                  iWindDy);  

    // Erase BMPF Rect
    IDISPLAY_EraseRect ( pContext->pIDisplay, &pRect );
    
    // no syllable to drew
    if ( 0 == nKeyBufLen )
    {
        return;
    }

    // Point to the Spell to draw 
    pbSpellBuffer = (char*)pContext->sT9ccFieldInfo.pbSpellBuf;
                
    // number of words in Spell buffer 
    nWordCount = ( STRLEN(pbSpellBuffer) + 1 ) / ( nKeyBufLen + 1 );
    
    // number of spell words that can be displayed
    if (pContext->sT9ccFieldInfo.G.nLdbNum == (T9PIDChinese | T9SIDChineseSimplified))
    {
        nWordCountDisp = ( pContext->rectChineseInput.dx - 2 ) / 
                     ( nKeyBufLen * PSYLLABLEWIDTH + PSEPARATORWIDTH );
    }
    else
    {
        nWordCountDisp = ( pContext->rectChineseInput.dx - 2 ) / 
                     ( nKeyBufLen * SYLLABLEWIDTH + SEPARATORWIDTH );
    }
    
    if ( 0 == nWordCountDisp )
    {
        nWordCountDisp = 1;
    }
    else if ( nWordCountDisp > nWordCount )
    {
        nWordCountDisp = nWordCount ;
    }
        
    // Length of AECHAR Spell buffer 
    nSpellBufLen = (nWordCount * (nKeyBufLen+2) + 1) * sizeof(AECHAR);
    
    // get some memory to work with 
    wszSpellBuf     = (AECHAR *)MALLOC ( nSpellBufLen );
    wszSpellBufDisp = (AECHAR *)MALLOC ( nSpellBufLen );
    MEMSET ( wszSpellBuf, 0, nSpellBufLen ); 
    MEMSET ( wszSpellBufDisp, 0, nSpellBufLen ); 
    
    // Base-1 value indicating which phonetic spelling is currently selected.
    bSpellCode = pContext->sT9ccFieldInfo.bSpellCode;
    // base-0
    nSelectedCode = bSpellCode - 1;
    wszTmp = wszSpellBuf;
    
    // Copy syllables
    while ( *pbSpellBuffer )
    {
        if (pContext->sT9ccFieldInfo.G.nLdbNum == (T9PIDChinese | T9SIDChineseSimplified))
        {
            *wszTmp = *pbSpellBuffer;
        }
        else if ((pContext->sT9ccFieldInfo.G.nLdbNum == (T9PIDChinese | T9SIDChineseTraditional)) && (IsBPMFChar(*pbSpellBuffer)))
        {
            *wszTmp = (AECHAR)BPMFInternalToUnicode ( *pbSpellBuffer );      
        }
        else
        {
            *wszTmp = 0x0020;
        }
        iwszlen = IDISPLAY_MeasureTextEx(pContext->pIDisplay,
                                                      pContext->font,
                                                      wszTmp,
                                                      1,
                                                      -1,
                                                      NULL);
        iLenTemp[iCount] = iwszlen;
        pbSpellBuffer ++;
        wszTmp   ++;
        iCount ++;
    }

    
    // the Len changed
    if ( nKeyBufLenOrig != nKeyBufLen )
    {
        nSpellCodeStart = 0;
    }
    else
    {
        // adjust the Start code
        if ( nSpellCodeStart < nSelectedCode - nWordCountDisp + 1 )
        {
            nSpellCodeStart = nSelectedCode - nWordCountDisp + 1;
        }
        else if ( nSpellCodeStart > nSelectedCode )
        {
            nSpellCodeStart = nSelectedCode;
        }
        if ( nSpellCodeStart > nWordCount - 1 )
        {
            nSpellCodeStart = nWordCount - 1;
        }
        // set Selected index for the display Spell code
        nSelectedCode = nSelectedCode - nSpellCodeStart;
    }

    // copy the displaying part from wszSpellBuf
    SymbToAECHARNCopy ( wszSpellBufDisp, 
                        wszSpellBuf + nSpellCodeStart * (nKeyBufLen+1) ,
                        nWordCountDisp * (nKeyBufLen+1) - 1 );
#ifdef FEATURE_FUNCS_THEME                       
    IDISPLAY_DrawRect(pContext->pIDisplay,
            &pRect,
            pContext->m_themeColor,
            RGB_WHITE,
            IDF_RECT_FRAME);
#else //FEATURE_FUNCS_THEME 
    IDISPLAY_DrawRect(pContext->pIDisplay,
            &pRect,
            RGB_BLACK,
            RGB_WHITE,
            IDF_RECT_FRAME);
#endif //FEATURE_FUNCS_THEME  

    (void) IDISPLAY_DrawText ((IDisplay *)pContext->pIDisplay,
                               AEE_FONT_NORMAL,
                               wszSpellBufDisp,  
                               -1,  
                               pRect.x+1,
                               pRect.y+1,
                               NULL,
                               format );
    // Add the character width
    for ( nSelectedCodeTemp=0; nSelectedCodeTemp < SPELLMAX; nSelectedCodeTemp++)
    {
        int32  cursTempX = 0;   // ��ĸ��ϵĺ�����λ��
        int    iSpellCount; // ��ĸ����е���ĸ���
        int    nSpellCountTemp;  // ��ĸ�ļ���
        for ( nSpellCountTemp=nSelectedCodeTemp*(nKeyBufLen+1), iSpellCount=0; 
              nSpellCountTemp < iCount && iSpellCount < nKeyBufLen;
              nSpellCountTemp++, iSpellCount++ )
        {
            cursTempX += iLenTemp[nSpellCountTemp];
        }
        iSpellLenTemp[nSelectedCodeTemp] = cursTempX;   // ÿ����ĸ��ϵĳ���
        if (nSelectedCodeTemp == 0)
        {   // ��һ����ĸ��ϵ�����λ��
            iSpellCursX[nSelectedCodeTemp] = pRect.x+1;
        }
        else
        {   // �������ĸ��ϵĴ������λ��
            iSpellCursX[nSelectedCodeTemp] = iSpellCursX[nSelectedCodeTemp-1]+
                iSpellLenTemp[nSelectedCodeTemp-1]+iLenTemp[nSelectedCodeTemp*(nKeyBufLen+1)-1];
        }
    }                                                    
    // draw Select Rect.
    // bSpellCode's value of 0 indicates there is no active phonetic spelling
    if ( nSelectedCode >= 0 )
    {
        AEERect  invertRect;
        invertRect.dx = iSpellLenTemp[nSelectedCode]+2;
        if (pContext->sT9ccFieldInfo.G.nLdbNum == (T9PIDChinese | T9SIDChineseTraditional))
        {   // ����������ĸ��ϵ�λ��
            invertRect.x = iSpellCursX[nSelectedCode];
        }
        else
        {
            if (nSelectedCode == 0)
            {   // ���ĵĵ�һ����ĸ��ϵ�λ��
                invertRect.x = iSpellCursX[nSelectedCode];
            }
            else
            {   // ���ĵĺ������ĸ��ϵľ�ȷλ��
                invertRect.x = iSpellCursX[nSelectedCode]-2;
            }
        }
        invertRect.y  = pRect.y+1;
        invertRect.dy = CHINESE_FONT_HEIGHT;
        IDISPLAY_InvertRect ( pContext->pIDisplay, &invertRect );
    }                              

    nKeyBufLenOrig = nKeyBufLen;
    
    if ( wszSpellBuf )
    {
        FREE ( wszSpellBuf );
    }
    if ( wszSpellBufDisp )
    {
        FREE ( wszSpellBufDisp );
    }
}

/*------------------------------------------------------------------------
 *
 *  Function name   : 
 *  Description     : 
 *
 *  Pass            : 
 *
 *  Return          : 
 *
 *-----------------------------------------------------------------------*/
static void T9_CJK_CHINESE_DisplaySelection(TextCtlContext *pContext)
{

    AECHAR          ch[2] = {0,0};
    uint32          format;
    AEERect         pRect;
    AEERect         invertRect;
    unsigned int    k = 0;
    unsigned int    iWindX = pContext->rectChineseTextInput.x;
    unsigned int    iWindY = pContext->rectChineseTextInput.y;   
    unsigned int     iWindDx = pContext->rectChineseSyllableInput.dx;
    unsigned int     iWindDy = pContext->rectChineseSyllableInput.dy ;
    T9SYMB          *psBuffer;

    if(!((FOCUS_SELECTION == pContext->sFocus) ||
          (FOCUS_SYLLABLE == pContext->sFocus) ||
          (FOCUS_TEXT_PREDICTION == pContext->sFocus)))
    {
        return;
    }
    T9_CJK_CHINESE_AdjustInputInfoLocation(pContext, &iWindX, &iWindY, &iWindDx, &iWindDy);
    
    // setup the text Rect
    SETAEERECT(&pRect,
              iWindX+2,
              iWindY+1, // at the bottom line
              iWindDx -6,     
              iWindDy -1);    
    IDISPLAY_EraseRect(pContext->pIDisplay, &pRect);
    
    // blank the selection when focus on TEXT
    if ( FOCUS_TEXT != pContext->sFocus )
    {
#ifdef FEATURE_FUNCS_THEME     
        IDISPLAY_DrawRect(pContext->pIDisplay,   
                &pRect,
                pContext->m_themeColor,
                RGB_WHITE,
                IDF_RECT_FRAME);  
#else //FEATURE_FUNCS_THEME 
        IDISPLAY_DrawRect(pContext->pIDisplay,   
                &pRect,
                RGB_BLACK,
                RGB_WHITE,
                IDF_RECT_FRAME);  
#endif //FEATURE_FUNCS_THEME 
    
        /* Point to the buffer to draw */
        psBuffer = pContext->sT9ccFieldInfo.pwSelectPage;

        /* Draw each character */
        for (k = 0; k < pContext->sT9ccFieldInfo.nSelectPageMax; k++) 
        {
            format = IDF_ALIGN_NONE;
             
            ch[0] = *(psBuffer+k); // use GBcode for EVB board    
            (void) IDISPLAY_DrawText((IDisplay *)pContext->pIDisplay,
                                   AEE_FONT_NORMAL,
                                   ch,
                                   -1,
                                   pRect.x+1+(T9_FONT_WIDTH)*k,
                                   pRect.y+1,//SCREEN_HEIGHT - pContext->nLineHeight,
                                   NULL,
                                   format);
            /* If this character is a NULL terminator, then stop drawing */
            if (*(psBuffer + k ) == 0)  break;
            
        };
        
        // draw Select Rect.
        if ( FOCUS_SELECTION == pContext->sFocus &&
             pContext->nSelectionSelectd >= 0 )
        {
            if ( pContext->nSelectionSelectd + 1 > pContext->sT9ccFieldInfo.nSelectPageLen)
            {
                pContext->nSelectionSelectd = pContext->sT9ccFieldInfo.nSelectPageLen - 1;
            }
            invertRect.x = pRect.x+1+(T9_FONT_WIDTH)*pContext->nSelectionSelectd;
            invertRect.y = pRect.y+1;
            invertRect.dx = CHINESE_FONT_WIDTH;
            invertRect.dy = CHINESE_FONT_HEIGHT;
            IDISPLAY_InvertRect(pContext->pIDisplay, &invertRect);
        }
    }   
}

/*------------------------------------------------------------------------
 *
 *  Function name   : T9_CJK_CHINESE_DisplaySyllable
 *  Description     : Draws the specified syllable to the specified location
 *                    on the graphics screen.
 *
 *  Pass            : 
 *
 *  Return          : void
 *
 *-----------------------------------------------------------------------*/
static void T9_CJK_CHINESE_DisplaySyllable ( TextCtlContext *pContext )
{
    if(FOCUS_SYLLABLE == pContext->sFocus)
    {
        switch ( OEM_TextGetCurrentMode((OEMCONTEXT)pContext) )
        {    
#ifdef  FEATURE_T9_PINYIN     
            case TEXT_MODE_T9_PINYIN:
                    T9_CJK_CHINESE_DrawSyllableString(pContext);
                    break;
#endif //FEATURE_T9_PINYIN

#ifdef  FEATURE_T9_ZHUYIN     
            case TEXT_MODE_T9_ZHUYIN:
                    T9_CJK_CHINESE_DrawSyllableString(pContext);                 
                    break;
#endif //FEATURE_T9_ZHUYIN

#ifdef  FEATURE_T9_STROKE     
            case TEXT_MODE_T9_STROKE:
                    T9_CJK_CHINESE_DrawStrokeString(pContext);              
                    break;
#endif //FEATURE_T9_ZHUYIN

            default:             
                    break;
        }        
    }
}

#endif //#ifdef FEATURE_T9_CHINESE

#endif //#ifdef FEATURE_T9_INPUT

//-----------------------------------------------------------------------------
// These functions handle Numbers text mode entry
//-----------------------------------------------------------------------------
/*=============================================================================
FUNCTION: TextCtl_NumbersRestart

DESCRIPTION:

PARAMETERS:
   *pContext:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_NumbersRestart(TextCtlContext *pContext)
{
   TextCtl_NoSelection(pContext);
   TextCtl_TextChanged(pContext);
}

/*=============================================================================
FUNCTION: TextCtl_NumbersTimer

DESCRIPTION:

PARAMETERS:
   *pUser:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_NumbersTimer(void *pUser)
{
   register TextCtlContext *pContext = (TextCtlContext *) pUser;
   TextCtl_NoSelection(pContext);
   OEM_TextDraw(pContext);
   IDISPLAY_Update((IDisplay *)pContext->pIDisplay);
}

/*=============================================================================
FUNCTION: TextCtl_NumbersKey

DESCRIPTION:

PARAMETERS:
   *pContext:
   key:

RETURN VALUE:
   boolean:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean TextCtl_NumbersKey(TextCtlContext *pContext, AEEEvent eCode,AVKType key)
{
   uint16   nBufLen = pContext->sT9awFieldInfo.G.nBufLen;
   boolean  bRet       = FALSE;
   T9STATUS sT9Status = T9STATERROR;  
   
   if (key >= AVK_1 && key <= AVK_9) 
   {
      TextCtl_NoSelection(pContext);
      TextCtl_AddChar(pContext,(AECHAR) ( ( (int)key - (int)AVK_0 ) + '0'));
      return TRUE;
   }
   else
   {   
	    switch (key)
	    {   
#if !defined (FEATURE_ALL_KEY_PAD)
	        case AVK_STAR:
	          //TextCtl_AddChar(pContext, (AECHAR) '*');
	          return TRUE;            

	        case AVK_POUND:
	            if(  (!(pContext->dwProperties & TP_STARKEY_SWITCH)))	           
	            {
	                TextCtl_AddChar(pContext, (AECHAR) '#');
	                return TRUE;
	            }
	            else
	            {
	                TextCtl_AddChar(pContext, (AECHAR) ' ');
	                return TRUE;
	            }            
	            break;	
#else			
			case AVK_POUND:
	        case AVK_STAR: 
#endif
	        
	        case AVK_T: 
	        case AVK_Y:
	        case AVK_U:
	        case AVK_I:
	        case AVK_O:
	        case AVK_P:
	        case AVK_G:
	        case AVK_H:
	        case AVK_J:
	        case AVK_K:
	        case AVK_L:
	        case AVK_V: 
	        case AVK_B: 
	        case AVK_N:
	        case AVK_M:
	        case AVK_RWD:
	            {
	                int i = 0;
	                if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
	                {
	                    if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
	                    {
	                        sT9Status = T9STATERROR; 
	                        return FALSE;
	                    }
	                }
	                for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
	    			{
	   
	        			if (key == VLCharKeyItem[i].wParam)
	        			{

	                        if(pContext->is_isShift)
	                        { 
	                            TextCtl_NoSelection(pContext);
	                            TextCtl_AddChar(pContext,(AECHAR)(VLCharKeyItem[i].wp));
	                            pContext->is_isShift = FALSE;
	                        }
	                    }
	                }
	            }
	            return TRUE;
	            break;
	        case AVK_ENTER:
			case AVK_SPACE:
	            {
	                int i = 0;
	                if ( pContext->wMaxChars != 0 && nBufLen >= pContext->wMaxChars)
	                {
	                    if(nBufLen > pContext->wMaxChars || pContext->uModeInfo.mtap.kLast != key)
	                    {
	                        sT9Status = T9STATERROR; 
	                        return FALSE;
	                    }
	                }
	                for(i = 0;i<MAX_SHEFTKEYPAD_NUMBER;i++)
	    			{
	        			if (key == VLCharKeyItem[i].wParam)
	        			{
	                        TextCtl_NoSelection(pContext);
	                        TextCtl_AddChar(pContext,(AECHAR)(VLCharKeyItem[i].wp));
	                    }
	                }
	            }
	            return TRUE;
	            break;
	        case AVK_SHIFT:
	              {
	                 if(pContext->is_isShift)
	                 {
	                    pContext->is_isShift = FALSE;
	                 }
	                 else
	                 {
	                    pContext->is_isShift = TRUE;
	                 }
	              }
	              break;
		  
	        case AVK_0:
	            if(pContext->dwProperties & TP_EDITNUMBER_PTSTRING)     
	            {
	                boolean ans = TRUE;
	                boolean canInsertFlg = TRUE;
	                AECHAR kInsert[2] = {0,0};

	                for (;;)
	                {
	                    if(key == AVK_0 )
	                    {
	                        if ( AVK_0 != pContext->uModeInfo.mtap.kLast) 
	                        {
	                            if(OEM_TextGetCursorPos(pContext) == 0)
	                            {
	                                pContext->uModeInfo.mtap.nMax = WSTRLEN(sszMultitap0TP0);                     
	                            }
	                            else
	                            {
	                                pContext->uModeInfo.mtap.nMax = WSTRLEN(sszMultitap0TP1);   
	                            }   
	                            pContext->uModeInfo.mtap.nSubChar = 0;

	                            if (!pContext->uModeInfo.mtap.nMax) 
	                            {
	                                ans = FALSE;  // Pass key up
	                                break;
	                            }
	                            TextCtl_NoSelection(pContext);
	                            if (pContext->uModeInfo.mtap.nCapsState) 
	                            {
	                                WSTRUPPER(kInsert);
	                            }
	                            TextCtl_AddChar(pContext, kInsert[0]);
	                        }
	                        if(OEM_TextGetCursorPos(pContext) == 0)
	                        {
	                            kInsert[0] = sszMultitap0TP0[pContext->uModeInfo.mtap.nSubChar];                       
	                        }
	                        else
	                        {
	                            kInsert[0] = sszMultitap0TP1[pContext->uModeInfo.mtap.nSubChar];                            
	                        }
	                        
	                        if (pContext->uModeInfo.mtap.nCapsState) 
	                        {
	                            // Capitalize it
	                            WSTRUPPER(kInsert);
	                        }
	                        
	                        if (canInsertFlg)
	                        {
	                            TextCtl_AddChar(pContext, kInsert[0]);
	                            (void) TextCtl_SetSel(pContext, pContext->wSelEnd - 1, pContext->wSelEnd);
	                        } 
	                        
	                        (void) ISHELL_SetTimer((IShell *) pContext->pIShell,
	                                                                TIMEOUT,
	                                                                TextCtl_MultitapTimer,
	                                                                pContext);

	                        if (++pContext->uModeInfo.mtap.nSubChar >= pContext->uModeInfo.mtap.nMax) 
	                        {
	                            pContext->uModeInfo.mtap.nSubChar = 0;
	                        }
	                    } 
	                    else 
	                    {
	                        ans = FALSE;
	                    }
	                    break;
	                } 
	                if (ans) 
	                {
	                    pContext->uModeInfo.mtap.kLast = key; 
	                }
	                return ans;
	            }    
	            else
	            {
	                TextCtl_AddChar(pContext, (AECHAR) '0');
	                return TRUE;
	            }           
	            break;   

	        case AVK_LEFT:
	            {                
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	                if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	                {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelEnd + 1;
	                   if(OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                   {
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
	                       //OEM_TextSetSel(pContext, 0, 0);
	                       OEM_TextSetCursorPos(pContext, 0);
	                   }
	                   else 
	                   {  
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );          
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	                }
	                else
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	                if (OEM_TextGetCursorPos(pContext) == 0)
	                {
	                    OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                }
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelStart;
	                    if (wNewSel)
	                    {
	                        --wNewSel;
	                    }
#ifdef FEATURE_LANG_THAI  
	                    {
	                        int count=0;
	                        count = moveleftselThaiChar(pContext->pszContents[pContext->wSelStart-2],
	                                                    pContext->pszContents[pContext->wSelStart-1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel - count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI           
	                    OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;
	            }            
	            break;     

	        case AVK_RIGHT:
	            {
#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	                if ( IDF_ALIGN_RIGHT == pContext->dwAlignFlags )
	                {
	                   uint16 wNewSel;
	                   wNewSel = pContext->wSelStart ;
	                   if ( OEM_TextGetCursorPos(pContext) == 0 )
	                   {
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G, T9CA_FROMBEG, 0 );
	                       //OEM_TextSetSel(pContext, 0, 0);
	                       OEM_TextSetCursorPos(pContext, WSTRLEN(pContext->pszContents)); 
	                   }
	                   else 
	                   {  
	                       //sT9Status = T9Cursor ( &pContext->sT9awFieldInfo.G,  T9CA_MOVERIGHT, 1 );  
	                       wNewSel --;            
	                       OEM_TextSetSel(pContext, wNewSel, wNewSel);                       
	                   }
	                }
	                else
#endif //#ifdef FEATURE_ARPHIC_LAYOUT_ENGINE
	                if (OEM_TextGetCursorPos(pContext) == WSTRLEN(pContext->pszContents))
	                {
	                    OEM_TextSetCursorPos(pContext, -1);
	                }                
	                else
	                {
	                    uint16 wNewSel;
	                    wNewSel = pContext->wSelEnd + 1;
#ifdef FEATURE_LANG_THAI  
	                    {
	                        int count=0;
	                        count = moverightselThaiChar(pContext->pszContents[pContext->wSelStart+2],
	                                                     pContext->pszContents[pContext->wSelStart+1]);
	                        if(count!= 0)
	                        {
	                            wNewSel = wNewSel + count;
	                        }
	                    }
#endif //FEATURE_LANG_THAI                       
	                    OEM_TextSetSel(pContext, wNewSel, wNewSel);
	                    (void) TextCtl_AutoScroll(pContext);
	                }
	                return TRUE;
	            }            
	            break;   

	        case AVK_UP:
	            {
	                uint16 nLine, nCharsIn,nSel;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If it is on the first line, return false
	                if(nLine == 0 || !pContext->pwLineStarts)
	                    return FALSE;

	                // Otherwise figure out how many characters from the start
	                // of the line the cursor is and try to put the cursor in a
	                // similar position on previous line. Or, if not enough
	                // chars, at the end of the line
	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                if(nCharsIn + pContext->pwLineStarts[nLine-1] >=
	                                               pContext->pwLineStarts[nLine]) 
	                {
	                    nSel = pContext->pwLineStarts[nLine]-1;
	                } 
	                else 
	                {
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine-1];
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);
	                return TRUE;
	            }            
	            break;     

	        case AVK_DOWN:
	            {
	                uint16 nLine, nCharsIn,nSel;

	                if((!pContext->pwLineStarts)||(!pContext->wLines))
	                    return FALSE;
	                nLine = TextCtl_GetLine(pContext, pContext->wSelEnd);

	                // If the cursor is on the last line and the line's last
	                // character is not a LF, then FALSE is returned as nothing
	                // can be done. A LF on the end of a line does not tell the
	                // wLines member that there is another line, hence this
	                // extra check.
	                if ( nLine == (pContext->wLines-1) &&
	                    pContext->pszContents[WSTRLEN(pContext->pszContents)-1] != LINEBREAK ) 
	                {
	                    return FALSE;
	                }

	                nCharsIn = pContext->wSelEnd - pContext->pwLineStarts[nLine];
	                // If the cursor is more characters in than the next line...
	                // This can happen because the LINEBREAK may be immediate, or at least < nCharsIn
	                if(nCharsIn + pContext->pwLineStarts[nLine+1] > pContext->pwLineStarts[nLine+2])
	                {
	                    // If it is the last line, don't subtract the LINEBREAK from selection spot
	                    if( nLine+2 == pContext->wLines )
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2];
	                    }
	                    else
	                    {
	                        nSel = pContext->pwLineStarts[nLine+2]-1;
	                    }
	                }
	                else
	                {
	                    // Selection spot is number of chars into the next line
	                    nSel = nCharsIn + pContext->pwLineStarts[nLine+1];
	                    // If this is not the beginning of a line 
	                    // and the selection point is a LINEBREAK, subtract one
	                    // Otherwise the selection overshoots to the first character
	                    // of the following line.
	                    if( nCharsIn && nSel && pContext->pszContents[nSel-1] == LINEBREAK )
	                    {
	                        nSel--;
	                    }
	                }
	                OEM_TextSetSel(pContext, nSel,nSel);
	                (void) TextCtl_AutoScroll(pContext);

	                return TRUE;
	            }            
	            break;    
	        case AVK_CLR:
	            if (pContext->wSelStart && pContext->wSelStart == pContext->wSelEnd) 
	            {
	                 /* Set selection to the character before the insertion point */
	                 --pContext->wSelStart;
	            }
	            else if ((pContext->wSelStart == 0) && (pContext->wSelStart == pContext->wSelEnd))
	            {
	                  return FALSE;
	            }
	            
	            /* Insert a "NUL" to just delete and insert nothing */
	            TextCtl_AddChar(pContext, 0);
	            return TRUE;                         
	    }
   }
   return FALSE;
}

/*=============================================================================
FUNCTION: TextCtl_NumbersExit

DESCRIPTION:

PARAMETERS:
   *pContext:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void TextCtl_NumbersExit(TextCtlContext *pContext)
{
   (void) ISHELL_CancelTimer((IShell *) pContext->pIShell,
                             TextCtl_NumbersTimer, pContext);

   TextCtl_NoSelection(pContext);
}

/*=============================================================================
FUNCTION: OEM_TextSetMultiCaps
DESCRIPTION: 
    ����FEATURE_3STYLE_MULTITAP��ȷ����ǰmultitap���뷨������style֮һ��

PARAMETERS:
   *pContext:
RETURN VALUE:
   None
COMMENTS:
SIDE EFFECTS:
SEE ALSO:
=============================================================================*/
void OEM_TextSetMultiCaps(OEMCONTEXT hTextField,MultitapCapsState nCaps)
{
    register TextCtlContext *pContext = (TextCtlContext *) hTextField;
    pContext->nMultitapCaps = nCaps;
}
/*=============================================================================
FUNCTION: OEM_TextGetMultiCaps
DESCRIPTION: 
    ����FEATURE_3STYLE_MULTITAP���õ���ǰmultitap���뷨������style֮һ��

PARAMETERS:
   *pContext:
RETURN VALUE:
   None
COMMENTS:
SIDE EFFECTS:
SEE ALSO:
=============================================================================*/
MultitapCapsState OEM_TextGetMultiCaps(OEMCONTEXT hTextField)
{
    register TextCtlContext *pContext = (TextCtlContext *) hTextField;
    return pContext->nMultitapCaps;
}

/*=============================================================================
FUNCTION: TextCtl_MultitapTimer

DESCRIPTION:

PARAMETERS:
   *pUser:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/

static void TextCtl_MultitapTimer(void *pUser)
{
   register TextCtlContext *pContext = (TextCtlContext *) pUser;

   // If the timer goes off, deselect the character and forget
   // the last key pressed
   TextCtl_NoSelection(pContext);

   pContext->uModeInfo.mtap.kLast = AVK_UNDEFINED;

   if (pContext->bNeedsDraw ) {
      // Force drawing now or the selection won't be removed!
      if (pContext->bNeedsDraw) {
         OEM_TextDraw(pContext);
      }
      IDISPLAY_Update(pContext->pIDisplay);
   }
   //if this was the max char that was timing out,
   //then the flag so that next char wont replace the last char
   if(pContext->wContentsChars == pContext->wMaxChars){
      pContext->bMaxCharTimedOut = TRUE;
   }
   
   // �����first mode�Ĵ�дtimer�󣬾�תΪall small, ע���ǿո��timerʱ����Ӧset Ϊall small��
   //if ( TEXT_MODE_MULTITAP == OEM_TextGetCurrentMode(pContext)
   if ( (TEXT_MODE_MULTITAP == OEM_TextGetCurrentMode(pContext)
#ifdef FEATURE_T9_MT_SPANISH
   	||TEXT_MODE_T9_MT_SPANISH == OEM_TextGetCurrentMode(pContext)
#endif
#ifdef FEATURE_T9_MT_FRENCH
    ||TEXT_MODE_T9_MT_FRENCH== OEM_TextGetCurrentMode(pContext)
#endif
        )
        && MULTITAP_FIRST_CAP == pContext->nMultitapCaps
        && !OEM_isFirstCap(pContext))
   {
      pContext->nMultitapCaps = MULTITAP_ALL_SMALL;
   }
}

/*=================================================================
Function: TextCtl_isFirstCap

Description:
   Check the position where need First cap mode 

Parameter(s):
   pme:  pointer to ITextCtl interface to text control object

Return Value:
   TRUE  - need First cap mode
   FALSE - not need First cap mode
Comments: none

Side Effects: none

=================================================================*/
boolean OEM_isFirstCap (OEMCONTEXT hTextField)
{
    register TextCtlContext *pContext = (TextCtlContext *) hTextField;
    boolean bRet = FALSE;
    int maxsymbolcount;
    int i,j;
    
    maxsymbolcount = WSTRLEN(saMultitapStrings_3sdtyle_endsymbol);

    if ( pContext->wSelStart == pContext->wSelEnd )
    {
        if ( pContext->wSelStart == 0 )
            bRet = TRUE;

        if ( pContext->wSelStart > 1 )
        {
            if(' ' == pContext->pszContents[pContext->wSelStart-1])
            { 
                for(j=0; j<(pContext->wSelStart-2); j++)
                {
                    if(' ' != pContext->pszContents[pContext->wSelStart-2-j])
                    {
                         break;
                    }
                }
                
                for(i=0; i<maxsymbolcount; i++)
                {
                    if(saMultitapStrings_3sdtyle_endsymbol[i] ==pContext->pszContents[pContext->wSelStart-2-j])
                    {
                        bRet = TRUE;
                        break;
                    }
                }
            }
            else
            {
                for(i=0; i<maxsymbolcount; i++)
                {
                    if(saMultitapStrings_3sdtyle_endsymbol[i] ==pContext->pszContents[pContext->wSelStart-1])
                    {
                        bRet = TRUE;
                        break;
                    }
                }          
             }
         }
    }
    else if ( pContext->wSelStart == pContext->wSelEnd-1 )
    {
        if ( pContext->wSelStart > 1 )
        {
            if(' ' == pContext->pszContents[pContext->wSelStart])
            {
                for(j=0; j<(pContext->wSelStart-1); j++)
                {
                    if(' ' != pContext->pszContents[pContext->wSelStart-1-j])
                    {
                         break;
                    }
                }
                            
                for(i=0; i<maxsymbolcount; i++)
                {
                    if(saMultitapStrings_3sdtyle_endsymbol[i] ==pContext->pszContents[pContext->wSelStart-1-j])
                    {
                        bRet = TRUE;
                        break;
                    }
                }
            }
            else
            {
                for(i=0; i<maxsymbolcount; i++)
                {
                    if(saMultitapStrings_3sdtyle_endsymbol[i] ==pContext->pszContents[pContext->wSelStart])
                    {
                        bRet = TRUE;
                        break;
                    }
                }          
             }
         }
    }
    return bRet;
}

void OEM_TextDrawIMEDlg(OEMCONTEXT hTextField)
{
//modi by ydc nf
//remark next line
//#ifdef FEATURE_ZICORP_CHINESE // {
	TextCtlContext * pme = (TextCtlContext *)hTextField;

	if( pme->T9Cxt.pDlg ){
		IDIALOG_Redraw(pme->T9Cxt.pDlg);
	}
	if( pme->pIDialog )
	{
		IDIALOG_Redraw(pme->pIDialog);
	}
//modi by ydc nf
//remark next line
//#endif // } FEATURE_ZICORP_CHINESE 
}


boolean OEM_HandlePenEvt(OEMCONTEXT hTextField,AEEEvent eCode,uint16 wLwrTime,int16 wXPos, int16 wYPos)
{
	TextCtlContext *  pme = (TextCtlContext *)hTextField;
	boolean           bRet = FALSE;
	//add by ydc

	if( eCode == EVT_PEN_STALE_MOVE ){
		// Potentially the greatest flood of pen messages, so quickly do not handle
		// NOTE that you will probably need to handle this in your dlg event handler
		// for handwriting recognition capabilities...
		// If it is inline handwriting recognition, then handle here!!!!
		return bRet;
	}
	if( !pme || !pme->bValid || !pme->bEditable){
		return bRet;
	}

	switch( eCode ){
   case EVT_PEN_MOVE:
	   {
		   // Moves come here a lot, don't track if there is nothing to track
		   if( pme->ptTracker.cbHit ){
			   uint8    cbHit;
			   uint16   wData;
			   AEEPoint ptPosition;

			   if( pme->ptTracker.cbHit == PTRCK_HIT_ITEM )
			   {
				   AEERect     rctGeneric;
				   boolean     bScroll = (pme->wLines > pme->nDisplayLines
					   && (pme->dwProperties & TP_MULTILINE)) ? TRUE : FALSE;

				   // Calculate the text rect and pixel width for lines
				   if (pme->dwProperties & TP_FRAME)
				   {
					   rctGeneric = pme->rectDisplay;
					   rctGeneric.x += 2;
					   rctGeneric.y += 2;
					   rctGeneric.dx -= 4;
					   rctGeneric.dy -= 4;
					   // Subtract scroll bar width, but it shares 2 pixels with the frame
					   if (bScroll)
					   {
						   rctGeneric.dx -= (pme->wScrollBarWidth - 2);
					   }
				   }
				   else
				   {
					   rctGeneric = pme->rectDisplay;
					   // Subtract scroll bar width, no sharing
					   if (bScroll)
					   {
						   rctGeneric.dx -= pme->wScrollBarWidth;
					   }
				   }
				   if(wYPos <= rctGeneric.y)
				   {
					   wYPos = rctGeneric.y;
				   }
				   if(wYPos >= rctGeneric.y + rctGeneric.dy)
				   {
					   wYPos = rctGeneric.y + rctGeneric.dy;
				   }
			   }       
			   if( (cbHit = TextCtl_PenHitToTrack(pme, wXPos, wYPos, &wData, &ptPosition)) != 0 ){
				   if( pme->ptTracker.cbHit == PTRCK_HIT_ITEM ){
					   if( wData <= pme->ptTracker.wData ){
						   OEM_TextSetSel(pme, wData, pme->ptTracker.wData);
					   }else{
						   OEM_TextSetSel(pme, pme->ptTracker.wData, wData);
					   }
					   if( !(pme->dwProperties & TP_NODRAW) ){
						   OEM_TextUpdate(pme);
					   }
				   }else{
					   if( pme->ptTracker.cbHit & PTRCK_NOTTHUMB ){
						   if( !(pme->ptTracker.cbFlags & PTRCK_GEN_TMRSET) ){
							   TextCtl_ScrollByPos(pme, ((pme->ptTracker.cbHit & PTRCK_HIT_BELOW) ? 1 : -1), wXPos, wYPos);
						   }
					   }else if(pme->ptTracker.cbHit & PTRCK_HIT_TRIAN)
					   {
						   if( !(pme->ptTracker.cbFlags & PTRCK_GEN_TMRSET) ){
							   TextCtl_StepByPos(pme, ((pme->ptTracker.cbHit & PTRCK_HIT_BLTRI) ? 1 : -1), wXPos, wYPos);
						   }
					   }
					   else{
						   AEERect aRect;
						   uint16 wBlackPixels;
						   uint16 wTopWhitePixels;
						   uint16 wScrollBarHeight;
						   uint16 wWhitePixels, wNewStart = pme->wDisplayStartLine;
						   uint16 wLinesOffScreen, triheight;

						   if(pme->rectDisplay.dy > 2 * pme->wScrollBarWidth)
						   {
							   wScrollBarHeight = (uint16)(pme->rectDisplay.dy
								   - 2 * pme->wScrollBarWidth);
							   triheight = pme->wScrollBarWidth;
						   }
						   else
						   {
							   wScrollBarHeight = ((pme->rectDisplay.dy%2==0)?2:1);
							   triheight = (pme->rectDisplay.dy - wScrollBarHeight)/2;
						   }

						   // Skip the pad from text to SB Frame
						   aRect.x = ((pme->rectDisplay.x + pme->rectDisplay.dx) - pme->wScrollBarWidth);
						   aRect.y = pme->rectDisplay.y + triheight;
						   aRect.dx = pme->wScrollBarWidth;
						   aRect.dy = pme->rectDisplay.dy - 2 * triheight;

						   // Consider allowing slippage of pen!
						   if( PT_IN_RECT(wXPos, wYPos, aRect) ){
							   // Now calculate the length of the black portion of the scroll bar
							   wBlackPixels = (uint16) ((uint16)pme->nDisplayLines *
								   wScrollBarHeight
								   /  pme->wLines);

							   if (wBlackPixels < 3) wBlackPixels = 3; // Minimum size of black part
							   if (wBlackPixels > wScrollBarHeight) wBlackPixels = wScrollBarHeight;
							   wLinesOffScreen = (uint16) pme->wLines
								   - (uint16) pme->nDisplayLines;
							   wWhitePixels = wScrollBarHeight - wBlackPixels;
							   wTopWhitePixels = (uint16) ((uint32) wWhitePixels
								   * (uint32) pme->wDisplayStartLine
								   / (uint32) wLinesOffScreen);
							   if (wWhitePixels)
							   {
								   if(((wLinesOffScreen*((wYPos - aRect.y)
									   -pme->ptTracker.wData))+(wWhitePixels-1)) > 0)
								   {
									   wNewStart = (((wLinesOffScreen*((wYPos - aRect.y)
										   -pme->ptTracker.wData))+(wWhitePixels-1))/wWhitePixels);
								   }
								   else
								   {
									   wNewStart = 0;
								   }
							   }
							   if( wNewStart != pme->wDisplayStartLine ){
								   if( wNewStart+pme->nDisplayLines > pme->wLines ){
									   wNewStart = pme->wLines - pme->nDisplayLines;
								   }
								   pme->wDisplayStartLine = wNewStart;
								   if( !(pme->dwProperties & TP_NODRAW) )
								   {
									   OEM_TextDraw(pme);
									   IDISPLAY_Update((IDisplay *)pme->pIDisplay);
								   }
							   }
						   }
					   }
				   } // End of scrollbar work for moved pen event
			   } // End of Current Point hit inside an understandable component
			   // It was handled it in some manner (explicitely by doing something, 
			   // implicitely since the pen down caused beginnin gof tracking)
			   // AKA: Pen capture, do not cause needless cascading of this event
			   bRet = TRUE;
		   } // End of tracking whether pen hit an understandable component on pen down
	   }
	   break;
   case EVT_PEN_DOWN :
	   {
		   if (/*(pme->byMode == (byte)TSIM_MODE)
			   && */(pme->pIDialog == NULL))
		   {
			   //(void)TSIM_ProcPenDown(pme, wXPos, wYPos);  //modi ydc 090519
		   }
		   if( PT_IN_RECT(wXPos, wYPos, pme->rectDisplay) )
		   {
			   if( (pme->ptTracker.cbHit = TextCtl_PenHitToTrack(pme, wXPos, wYPos, &pme->ptTracker.wData, &pme->ptTracker.ptPosition)) != 0 )
			   {
				   if( pme->ptTracker.cbHit & PTRCK_HIT_SCRL )
				   {
					   if( pme->ptTracker.cbHit & PTRCK_NOTTHUMB )
					   {
						   TextCtl_ScrollTimerCB(pme);  // Sets cursor and scrolls!
						   ISHELL_SetTimer((IShell *)pme->pIShell, (int32)pme->arPenRepeat.dwStart, (PFNNOTIFY)TextCtl_ScrollTimerCB, (void *)pme);
						   pme->ptTracker.cbFlags |= PTRCK_GEN_TMRSET;
					   }
					   if( pme->ptTracker.cbHit & PTRCK_HIT_TRIAN )
					   {
						   TextCtl_StepTimerCB(pme);  // Sets cursor and scrolls!
						   ISHELL_SetTimer((IShell *)pme->pIShell, (int32)pme->arPenRepeat.dwStart, (PFNNOTIFY)TextCtl_StepTimerCB, (void *)pme);
						   pme->ptTracker.cbFlags |= PTRCK_GEN_TMRSET;
					   }
				   }
				   else
				   {
					   // Set the cursor position changed, but no selection yet!
					   OEM_TextSetSel(pme, pme->ptTracker.wData, pme->ptTracker.wData);            
					   TextCtl_AutoScroll(pme);
					   if( !(pme->dwProperties & TP_NODRAW) )
					   {
						   OEM_TextUpdate(pme);
					   }
				   }
			   }
			   // In the rect of this control, so handled ot prevent inefficient cascades
			   // Not so important with this event, but a safe specification to adhere with
			   bRet = TRUE;
		   }
	   }
	   break;
   case EVT_PEN_UP:
	   if( pme->ptTracker.cbHit )
	   {
		   if( pme->ptTracker.cbFlags & PTRCK_GEN_TMRSET )
		   {
			   if(pme->ptTracker.cbHit & PTRCK_NOTTHUMB)
			   {
				   ISHELL_CancelTimer((IShell *)pme->pIShell, (PFNNOTIFY)TextCtl_ScrollTimerCB, (void *)pme);
			   }
			   if(pme->ptTracker.cbHit & PTRCK_HIT_TRIAN)
			   {
				   ISHELL_CancelTimer((IShell *)pme->pIShell, (PFNNOTIFY)TextCtl_StepTimerCB, (void *)pme);
			   }
		   }
		   // TODO: Return Values
		   bRet = TRUE;
	   }
	   // Pen is up, can't store these
	   MEMSET((void *)&pme->ptTracker, 0, sizeof(pme->ptTracker));
	   if ((pme->pIDialog == NULL)&&(1)&&(1)&&(1)&&(1))
	   {
		   return(TSIM_ProcPenUp(pme,wXPos,wYPos));    //modi 090519
	   }
	   break;
   default:
	   break;
	}

	return bRet;
}

/*=============================================================================
FUNCTION: TSIM_ProcPenUp

DESCRIPTION:when received a EVT_PEN_UP,we invoke this function to handle.

PARAMETERS:
*pTextCtl:a OEMCONTEXT type parameter is passed in
dwparam: the word parameter 

RETURN VALUE:
boolean:true if successful,otherwise false

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
boolean TSIM_ProcPenUp(OEMCONTEXT hTextCtl,int16 xpos,int16 ypos)
{
	register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

	pContext->rc_text.x = 0;
	pContext->rc_text.y = 20;
	pContext->rc_text.dx = 170;
	pContext->rc_text.dy = 164;
	pContext ->binorig = TRUE;

	//if the pendown point is in the pinyin keypad, then pass the event to the virtualkey controls
	//set the range of the text now
	SETCoordiRange(&pContext->textrange,
		pContext->rc_text.x,
		pContext->rc_text.y,
		(pContext->rc_text.x + pContext->rc_text.dx) - 1,
		(pContext->rc_text.y + pContext->rc_text.dy) - 1);

	if ((TextCtl_IsInRange(xpos, ypos, &pContext->textrange))
		&& pContext->binorig)
	{
		pContext->bdowntsim = TRUE;
		if (pContext->pIDialog == NULL)
		{
			pContext->bdowntsim = FALSE;
			pContext->binorig = FALSE;
			TSIM_CreateDlg(pContext);
		}
		return TRUE;
	}
	return FALSE;
}
static void TextCtl_StepTimerCB(TextCtlContext *pme)
{
	pme->ptTracker.cbFlags &= ~PTRCK_GEN_TMRSET;
	TextCtl_StepByPos(pme, ((pme->ptTracker.cbHit & PTRCK_HIT_BLTRI) ? 1 : -1), pme->ptTracker.ptPosition.x, pme->ptTracker.ptPosition.y);
}

static void TextCtl_ScrollTimerCB(TextCtlContext *pme)
{
	pme->ptTracker.cbFlags &= ~PTRCK_GEN_TMRSET;
	TextCtl_ScrollByPos(pme, ((pme->ptTracker.cbHit & PTRCK_HIT_BELOW) ? 1 : -1), pme->ptTracker.ptPosition.x, pme->ptTracker.ptPosition.y);
}

static boolean TextCtl_StepByPos(TextCtlContext * pme, int nDir, int16 wXPos, int16 wYPos)
{
	AEERect  rctFrame, rctThumb;

	if( (pme->wLines > pme->nDisplayLines && (pme->dwProperties & TP_MULTILINE))
		&& !TextCtl_GetScrollBarRects(pme, &rctFrame, &rctThumb) ){
			boolean  bMoved = FALSE;

			if( PT_IN_RECT(wXPos, wYPos, rctFrame) ){
				uint16 triheight;

				if(pme->rectDisplay.dy > 2 * pme->wScrollBarWidth)
				{
					triheight = pme->wScrollBarWidth;
				}
				else
				{
					if(pme->rectDisplay.dy%2 == 0)
					{
						triheight = (pme->rectDisplay.dy - 2)/2;
					}
					else
					{
						triheight = (pme->rectDisplay.dy - 1)/2;
					}
				}

				// If the point is in the segment expected
				if( nDir > 0 ){
					if( wYPos > (pme->rectDisplay.y + pme->rectDisplay.dy - triheight) )
					{   // Already bound by SB Frame
						if( pme->wLines > (pme->wDisplayStartLine + pme->nDisplayLines) )
						{
							pme->wDisplayStartLine++;
							bMoved = TRUE;
						}
					}
				}else{
					if( wYPos < pme->rectDisplay.y + triheight){  // Already bound by SB Frame
						if( pme->wDisplayStartLine > 0 ){
							pme->wDisplayStartLine--;
							bMoved = TRUE;
						}
					}
				}
				if( bMoved ){
					ISHELL_SetTimer((IShell*)pme->pIShell, (int)pme->arPenRepeat.dwRate, (PFNNOTIFY)TextCtl_StepTimerCB, (void *)pme);
					pme->ptTracker.cbFlags |= PTRCK_GEN_TMRSET;
					if( !(pme->dwProperties & TP_NODRAW) ){
						OEM_TextDraw(pme);
						IDISPLAY_Update((IDisplay *)pme->pIDisplay);
					}
					return bMoved;
				}
			}
	}
	return FALSE;
}

static boolean TextCtl_ScrollByPos(TextCtlContext * pme, int nDir, int16 wXPos, int16 wYPos)
{
	AEERect  rctFrame, rctThumb;

	if( (pme->wLines > pme->nDisplayLines && (pme->dwProperties & TP_MULTILINE))
		&& !TextCtl_GetScrollBarRects(pme, &rctFrame, &rctThumb) ){
			boolean  bMoved = FALSE;

			if( PT_IN_RECT(wXPos, wYPos, rctFrame) ){
				uint16 triheight;

				if(pme->rectDisplay.dy > 2 * pme->wScrollBarWidth)
				{
					triheight = pme->wScrollBarWidth;
				}
				else
				{
					if(pme->rectDisplay.dy%2 == 0)
					{
						triheight = (pme->rectDisplay.dy - 2)/2;
					}
					else
					{
						triheight = (pme->rectDisplay.dy - 1)/2;
					}
				}

				// If the point is in the segment expected
				if( nDir > 0 ){
					if( (wYPos > (rctThumb.y+rctThumb.dy))
						&& (wYPos < (pme->rectDisplay.y + pme->rectDisplay.dy - triheight))){   // Already bound by SB Frame
							if( pme->wLines > (pme->wDisplayStartLine + pme->nDisplayLines) ){
								pme->wDisplayStartLine += (uint16)pme->nDisplayLines;
								if(pme->wDisplayStartLine + pme->nDisplayLines
				  > pme->wLines)
								{
									pme->wDisplayStartLine = pme->wLines - pme->nDisplayLines;
								}
								bMoved = TRUE;
							}
					}
				}else{
					if(( wYPos < rctThumb.y )
						&& (wYPos > pme->rectDisplay.y + triheight)){  // Already bound by SB Frame
							if( (pme->wDisplayStartLine - pme->nDisplayLines) > 0 ){
								pme->wDisplayStartLine -= (uint16)pme->nDisplayLines;
								bMoved = TRUE;
							}else{
								if( pme->wDisplayStartLine != 0 ){
									pme->wDisplayStartLine = 0;
									bMoved = TRUE;
								}
							}
					}
				}
				if( bMoved ){
					ISHELL_SetTimer((IShell*)pme->pIShell, (int)pme->arPenRepeat.dwRate, (PFNNOTIFY)TextCtl_ScrollTimerCB, (void *)pme);
					pme->ptTracker.cbFlags |= PTRCK_GEN_TMRSET;
					if( !(pme->dwProperties & TP_NODRAW) ){
						OEM_TextDraw(pme);
						IDISPLAY_Update((IDisplay *)pme->pIDisplay);
					}
					return bMoved;
				}
			}
	}
	return FALSE;
}

static uint8 TextCtl_PenHitToTrack(TextCtlContext * pme, int16 wXPos, int16 wYPos, uint16 * pwData, AEEPoint * pptPosition)
{
	AEERect        rctGeneric;
	uint16         wWidth, wHeight;
	//wChars = pme->wContentsChars;
	AECHAR *       psText = pme->pszContents;
	uint16         wLines = pme->wLines;
	boolean        bScroll = (wLines > pme->nDisplayLines && (pme->dwProperties & TP_MULTILINE)) ? TRUE : FALSE;
	uint16 *       pwLineStart = pme->pwLineStarts;
	int            nFits,
		nAdd = 0,
		nEnd = -1;
	// Calculate the text rect and pixel width for lines
	if (pme->dwProperties & TP_FRAME) {
		rctGeneric = pme->rectDisplay;
		rctGeneric.x += 2;
		rctGeneric.y += 2;
		rctGeneric.dx -= 4;
		rctGeneric.dy -= 4;
		// Subtract scroll bar width, but it shares 2 pixels with the frame
		if (bScroll) 
			rctGeneric.dx -= (pme->wScrollBarWidth - 2);
	}
	else {
		rctGeneric = pme->rectDisplay;
		// Subtract scroll bar width, no sharing
		if (bScroll)
			rctGeneric.dx -= pme->wScrollBarWidth;
	}
	// Consider 'Pen Capture', whereby select-to-scroll notices points in a given range
	// above and below the text area to select prev or next line point.
	// (Negative values would be therefore OK)
	if( PT_IN_RECT(wXPos, wYPos, rctGeneric) ){
		wWidth   = (uint16)rctGeneric.dx;
		wHeight  = 0;
		if( !pwLineStart ){
			return FALSE;
		}
		if( pme->dwProperties & TP_MULTILINE ){
			wHeight  = (uint16)(wYPos - rctGeneric.y)/(uint16)(pme->nFontAscent + pme->nFontDescent + pme->nFontLeading);
			if(wYPos == rctGeneric.y){
				// For scrolling back via pen selection
				// Consider adding padding as 1 pixel is difficult to land on with slippery pens
				if( pme->wDisplayStartLine ){
					pme->wDisplayStartLine--;
				}
			}
			if(wYPos == (rctGeneric.y + rctGeneric.dy))
			{
				if((pme->wDisplayStartLine + pme->nDisplayLines) < pme->wLines)
				{
					pme->wDisplayStartLine++;
				} 
			}
		}
		if(wHeight == pme->nDisplayLines)
		{
			wHeight = pme->nDisplayLines - 1;
		}
		if( (wHeight + pme->wDisplayStartLine) >= wLines ){
			// For scrolling forward via pen selection
			// Consider adding padding as 1 pixel is difficult to land on with slippery pens
			wHeight = MAX(wLines-pme->wDisplayStartLine-1, 0);
		}
		nAdd = pwLineStart[wHeight+pme->wDisplayStartLine];
		if( (pme->wDisplayStartLine+wHeight+1) < wLines ){
			nEnd = pwLineStart[pme->wDisplayStartLine+wHeight+1];
		}
		if( nEnd == -1 ){
			//nEnd = (((AECHAR *)(psText+nAdd) ) ? WSTRLEN((AECHAR *)(psText+nAdd)) : 0);
			nEnd = (((AECHAR *)(psText + nAdd) ) ? WSTRLEN((AECHAR *)(psText + nAdd)) : 0);
		}
		(void)IDISPLAY_MeasureTextEx((IDisplay *)pme->pIDisplay, pme->font, (psText+nAdd), nEnd, wXPos-rctGeneric.x, &nFits);
		nAdd += nFits;
		if (nAdd > 0)
		{
			if( psText[nAdd-1] == LINEBREAK ){
				nAdd--; // Clicking after a line keeps us on the line please!
			}
		}
		*pwData = (uint16)nAdd;
		return PTRCK_HIT_ITEM;
	}else{
		if( bScroll ){
			AEERect rctFrame, rctThumb;
			if( !TextCtl_GetScrollBarRects(pme, &rctFrame, &rctThumb) ){
				if( PT_IN_RECT(wXPos, wYPos, rctFrame) ){
					uint8 cbHit = 0;
					uint16 triheight;

					if(pme->rectDisplay.dy > 2 * pme->wScrollBarWidth)
					{
						triheight = pme->wScrollBarWidth;
					}
					else
					{
						if(pme->rectDisplay.dy%2 == 0)
						{
							triheight = (pme->rectDisplay.dy - 2)/2;
						}
						else
						{
							triheight = (pme->rectDisplay.dy - 1)/2;
						}
					}

					// Check the thumb first!
					// Don't want to restrict the thumb from the scroll edges, 
					// so entire width is valid given it is in vertical thumb range
					if( wYPos >= rctThumb.y && wYPos <= (rctThumb.y + rctThumb.dy) )
					{
						//ERR("PTRCK_HIT_THUMB", 0, 0, 0);
						cbHit = PTRCK_HIT_THUMB;
					}else if(wYPos <= pme->rectDisplay.y + triheight)
					{
						//ERR("PTRCK_HIT_ABTRI", 0, 0, 0);
						cbHit = PTRCK_HIT_ABTRI;
					}else if(( wYPos < rctThumb.y ) 
						&& (wYPos > pme->rectDisplay.y + triheight)){
							//ERR("PTRCK_HIT_ABOVE", 0, 0, 0);
							cbHit = PTRCK_HIT_ABOVE;
					}else if(wYPos >= (pme->rectDisplay.y
						+ pme->rectDisplay.dy - triheight))
					{
						//ERR("PTRCK_HIT_BLTRI", 0, 0, 0);
						cbHit = PTRCK_HIT_BLTRI;
					}
					else{
						//ERR("PTRCK_HIT_BELOW", 0, 0, 0);
						cbHit = PTRCK_HIT_BELOW;
					}
					cbHit |= PTRCK_HIT_SCRL;
					*pwData = (uint16)(wYPos - rctThumb.y);
					if( pptPosition ){
						pptPosition->x = wXPos;
						pptPosition->y = wYPos;
					}
					return cbHit;
				}
			}
		}
	}
	*pwData = 0;
	return 0;
}


static int TextCtl_GetScrollBarRects(TextCtlContext * pme, AEERect * prcFrame, AEERect * prcThumb)
{
	AEERect aRect;
	uint16 wBlackPixels;
	uint16 wTopWhitePixels;
	uint16 wBottomWhitePixels;
	uint16 wScrollBarHeight, triheight;

	if(pme->rectDisplay.dy > 2 * pme->wScrollBarWidth)
	{
		wScrollBarHeight = (uint16)(pme->rectDisplay.dy
			- 2 * pme->wScrollBarWidth);
		triheight = pme->wScrollBarWidth;
	}
	else
	{
		wScrollBarHeight = ((pme->rectDisplay.dy%2==0)?2:1);
		triheight = (pme->rectDisplay.dy - wScrollBarHeight)/2;
	}

	// Skip the clear 1-pixel border to the left of the scroll bar
	aRect.x = pme->rectDisplay.x + pme->rectDisplay.dx - pme->wScrollBarWidth;
	aRect.y = pme->rectDisplay.y;
	aRect.dx = pme->wScrollBarWidth;
	aRect.dy = pme->rectDisplay.dy;

	// Now calculate the length of the black portion of the scroll bar
	if (pme->wLines > pme->nDisplayLines) {
		uint16 wWhitePixels;
		uint16 wLinesOffScreen;
		wBlackPixels = (uint16) ( (uint16)pme->nDisplayLines *
			wScrollBarHeight
			/  pme->wLines);
		if (wBlackPixels < 3) wBlackPixels = 3; // Minimum size of black part
		if (wBlackPixels > wScrollBarHeight) wBlackPixels = wScrollBarHeight;
		wLinesOffScreen = (uint16) pme->wLines
			- (uint16) pme->nDisplayLines;
		wWhitePixels = wScrollBarHeight - wBlackPixels;
		wTopWhitePixels = (uint16) ((uint32) wWhitePixels
			* (uint32) pme->wDisplayStartLine
			/ (uint32) wLinesOffScreen);
		if (wWhitePixels && !wTopWhitePixels && pme->wDisplayStartLine) {
			// Be sure to show at least 1 pixel of white on the top if we're not
			// really scrolled all the way to the top
			wTopWhitePixels = 1;
		}
		wBottomWhitePixels = wWhitePixels - wTopWhitePixels;
		if (!wBottomWhitePixels && wWhitePixels && pme->wDisplayStartLine <
			wLinesOffScreen) {
				// Really we should show at least 1 pixel of white on the bottom
				if (wTopWhitePixels >= 2) {
					// Get it from the top white area
					--wTopWhitePixels;
					++wBottomWhitePixels;
				}
				else if (wBlackPixels > 3) {
					// Squeeze it out of the black area
					--wBlackPixels;
					++wBottomWhitePixels;
				}
		}
	}
	else {
		wBlackPixels = wScrollBarHeight;
		wTopWhitePixels = 0;
		wBottomWhitePixels = 0;
	}

	// Re-adjust aRect for scroll bar area itself
	//   ++aRect.x;
	//   aRect.dx = pme->wScrollBarWidth - 1;

	// Frame Position the outer area of the scroll bar
	*prcFrame = aRect;

	// Now adjust aRect for just the inside of the scroll bar
	//   ++aRect.x;
	//   aRect.dx -= 2;
	//   ++aRect.y;
	//   aRect.dy -= 2;

	// Skip the top white pixels
	aRect.y += wTopWhitePixels + triheight;
	// Skip the black pixels (we always have some of these)
	aRect.dy = (int16)wBlackPixels;
	*prcThumb = aRect;

	return AEE_SUCCESS;
}

/*=============================================================================
FUNCTION: TSIM_ProcPenDown

DESCRIPTION:when received a EVT_PEN_UP ,we invoke this function to handle.

PARAMETERS:
*pTextCtl:a OEMCONTEXT type parameter is passed in
dwparam: the word parameter 

RETURN VALUE:
boolean:true if successful,otherwise false

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
boolean TSIM_ProcPenDown(OEMCONTEXT hTextCtl, int16 xpos, int16 ypos)
{
	register TextCtlContext *pContext = (TextCtlContext *) hTextCtl;

	//if it is not in pinyin inputmode, then exit
	//if (pContext->byMode != (byte)TSIM_MODE)
	//{
		//ERR("BYMODE != TSIM_MODE", 0, 0, 0);
	//	return FALSE;
	//}
	//add by ydc  2009/04/02
	pContext->rc_text.x = 0;
	pContext->rc_text.y = 20;
	pContext->rc_text.dx = 170;
	pContext->rc_text.dy = 164;
	pContext ->binorig = TRUE;

	//if the pendown point is in the pinyin keypad, then pass the event to the virtualkey controls
	//set the range of the text now
	SETCoordiRange(&pContext->textrange,
		pContext->rc_text.x,
		pContext->rc_text.y,
		(pContext->rc_text.x + pContext->rc_text.dx) - 1,
		(pContext->rc_text.y + pContext->rc_text.dy) - 1);

	if ((TextCtl_IsInRange(xpos, ypos, &pContext->textrange))
		&& pContext->binorig)
	{
		pContext->bdowntsim = TRUE;
		if (pContext->pIDialog == NULL)
		{
			pContext->bdowntsim = FALSE;
			pContext->binorig = FALSE;
			TSIM_CreateDlg(pContext);
		}
		return TRUE;
	}
	return FALSE;
}

/*=============================================================================
FUNCTION: TSIM_CreateDlg

DESCRIPTION:This function is to create a dialog for TSIM inputmode

PARAMETERS:
*pContext:the pointer to the TextCtlContext.

RETURN VALUE:NULL

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void  TSIM_CreateDlg(TextCtlContext *pContext)
{
	DialogInfo     di;
	IBitmap *      pBmp;
	AEEBitmapInfo  bi;
	ITSIM   *      pITSIM;

	//get the bitmap
	pBmp = IDISPLAY_GetDestination((IDisplay *)pContext->pIDisplay);
	if (!pBmp)
	{
		//ERR("IDISPLAY_GetDestination FAIL", 0, 0, 0);
		return;
	}

	//get the information of the bitmap gotten
	(void)IBITMAP_GetInfo(pBmp, &bi, sizeof(AEEBitmapInfo));

	//fill the di with the information of bitmap
	SETAEERECT(&di.h.rc,
		0,
		0,
		bi.cx,
		bi.cy);

	//set the parameter of di
	di.h.dwProps   = DLG_HANDLE_ALL_EVENTS;
	di.h.wID       = OEM_IME_DIALOG;
	di.h.wFocusID  = OEM_IME_DIALOG_CTL_ITSIM;
	di.h.nControls = 1;
	di.h.wTitle    = 0;

	di.controls[0].h.cls       = AEECLSID_OEMITSIM;
	di.controls[0].h.dwProps   = TP_FRAME|TP_MULTILINE;
	di.controls[0].h.nItems    = 0;
	di.controls[0].h.wID       = OEM_IME_DIALOG_CTL_ITSIM;
	di.controls[0].h.wTextID   = 0;
	di.controls[0].h.wTitleID  = 0;

	// (0, 0, bitmap width, 18 is abritrary just to keep things from being too smart on creation)
	// The rect will self calculate when IDisplay is set to it.
	SETAEERECT(&di.controls[0].h.rc, 0, 0, bi.cx, bi.cy);

	// These lines are to work around a context-less dialog event
	// So the text control can work the same when launched inside a dialog
	// and when flat on screen.
	// Wherever these are referenced in this scope are also workaround material.
	// modi by ydc nf
	//pac = AEE_SetAppContext(AEE_GetStackedAppContext());
	//create the dialog

	if (AEE_SUCCESS != ISHELL_CreateDialog((IShell *)pContext->pIShell,
		NULL,
		0,
		&di))
	{
		IBITMAP_Release( pBmp );
		return;
	}
/*
	if (AEE_SUCCESS != ISHELL_CreateDialog((IShell *)pContext->pIShell,
		AEE_APPSTISIMAPP_RES_FILE,
		IDD_IME,
		NULL))
	{
		IBITMAP_Release( pBmp );
		return;
	}
	*/

	pContext->pIDialog = ISHELL_GetActiveDialog((IShell *)pContext->pIShell);

	// modi by ydc nf
	//(void)AEE_SetAppContext(pac);

	if (!pContext->pIDialog)
	{
		//ERR("NO ACTIVE DIALOG", 0, 0, 0);
		return;
	}

	IDIALOG_SetEventHandler(pContext->pIDialog, TSIM_DlgHandleEvent, (void*)pContext);
	pITSIM = (ITSIM*)IDIALOG_GetControl(pContext->pIDialog, OEM_IME_DIALOG_CTL_ITSIM);
	if (NULL == pITSIM)
	{
		(void)ISHELL_EndDialog((IShell*)pContext->pIShell);
		pContext->pIDialog = NULL;
		return;
	}

	ITSIM_SetMaxSize(pITSIM, pContext->wMaxChars);
	(void)ITSIM_SetText(pITSIM, pContext->pszContents, pContext->wContentsChars);
	ITSIM_SetCursorPos(pITSIM, (int32)pContext->wSelEnd);
	//ITSIM_SetActive(pITSIM, TRUE);
	//   ERR("TSIM_CreateDlg 8", 0, 0, 0);
	//ITSIM_Redraw(pITSIM);
	//   ERR("TSIM_CreateDlg 9", 0, 0, 0);
	IBITMAP_Release( pBmp );
	return;
}
/*=============================================================================
FUNCTION: TSIM_DlgHandleEvent

DESCRIPTION:This handleevent function of the pinyin inputmode dialog
which will be invoked when there is some event to be passed into.

PARAMETERS:
*pUser:the pointer to the user data.
evt:the event being passed into.
wparam:the uint16 parameter passed into.
dwparam:the uint32 parameter passed into.

RETURN VALUE:TRUE if successful, otherwise FALSE

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean TSIM_DlgHandleEvent(void * pUser, AEEEvent evt, uint16 wparam, uint32 dwparam)
{
	TextCtlContext * pContext = (TextCtlContext *)pUser;

	if (NULL == pContext)
	{
		return FALSE;
	}
	switch( evt )
	{
	case EVT_DIALOG_INIT:
	case EVT_DIALOG_START:
		return TRUE;

	case EVT_DIALOG_END:
		{
/*
			ITSIM   *pITSIM;
			AECHAR *pcontents;
			int     selstart;
			IDialog   *pdlg;
			if(pContext->pIDialog == NULL)
			{
				return FALSE;
			}

			pITSIM = (ITSIM*)IDIALOG_GetControl(pContext->pIDialog,
				OEM_IME_DIALOG_CTL_ITSIM);
			pContext->binorig = TRUE;
			pContext->wDisplayStartLine = 0;
			pcontents = ITSIM_GetTextPtr(pITSIM);
			(void)OEM_TextSet((OEMCONTEXT)pContext,
				pcontents,
				pcontents?WSTRLEN(pcontents):0);
			selstart = (int)ITSIM_GetCursorPos(pITSIM);
			//(void)TextCtl_SetSel(pContext, selstart, selstart);
			pdlg = pContext->pIDialog;
			pContext->pIDialog = NULL;
			IDIALOG_SetEventHandler(pdlg, NULL, (void*)pContext);
			IDIALOG_Release(pdlg);
			//IDISPLAY_ClearScreen((IDisplay *)pContext->pIDisplay);
			//OEM_TextDraw((OEMCONTEXT)pContext);
			//IDISPLAY_Update((IDisplay *)pContext->pIDisplay);
			*/
			IDialog   *pdlg;
			pdlg = pContext->pIDialog;
			pContext->pIDialog = NULL;
			IDIALOG_SetEventHandler(pdlg, NULL, (void*)pContext);
			IDIALOG_Release(pdlg);
			return TRUE;
		}
	case EVT_TSIM_READY:
		{
			ITSIM   *pITSIM;
			AECHAR *pcontents;
			int     selstart;
			IDialog   *pdlg;

			pITSIM = (ITSIM*)IDIALOG_GetControl(pContext->pIDialog,
				OEM_IME_DIALOG_CTL_ITSIM);
			pContext->binorig = TRUE;
			pContext->wDisplayStartLine = 0;
			pcontents = ITSIM_GetTextPtr(pITSIM);
			(void)OEM_TextSet((OEMCONTEXT)pContext,
				pcontents,
				pcontents?WSTRLEN(pcontents):0);
			selstart = (int)ITSIM_GetCursorPos(pITSIM);
			(void)TextCtl_SetSel(pContext, selstart, selstart);
			pdlg = pContext->pIDialog;
			pContext->pIDialog = NULL;
			IDIALOG_SetEventHandler(pdlg, NULL, (void*)pContext);
			IDIALOG_Release(pdlg);
			return TRUE;
		}

	case EVT_KEY:
		{
			switch((AVKType)wparam)
			{
			case AVK_SELECT:
			case AVK_F0:
			case AVK_CLR:
				{
					ITSIM   *pITSIM;
					AECHAR *pcontents;
					int     selstart;
					IDialog   *pdlg;


					pITSIM = (ITSIM*)IDIALOG_GetControl(pContext->pIDialog,
						OEM_IME_DIALOG_CTL_ITSIM);
					pContext->binorig = TRUE;
					pContext->wDisplayStartLine = 0;
					pcontents = ITSIM_GetTextPtr(pITSIM);
					(void)OEM_TextSet((OEMCONTEXT)pContext,
						pcontents,
						pcontents?WSTRLEN(pcontents):0);
					selstart = (int)ITSIM_GetCursorPos(pITSIM);
					(void)TextCtl_SetSel(pContext, selstart, selstart);

					//delete by ydc
					pdlg = pContext->pIDialog;
					pContext->pIDialog = NULL;
					IDIALOG_SetEventHandler(pdlg, NULL, (void*)pContext);
					IDIALOG_Release(pdlg);
					//end
					//ISHELL_EndDialog(pContext->pIShell);


					//AEE_Event(EVT_USER_REDRAW, 0, 0);
					return TRUE;
				}

			default:
				break;
			}
			return TRUE;
		}

	case EVT_PEN_DOWN :
	case EVT_PEN_MOVE:
	case EVT_PEN_UP:
	case EVT_KEY_PRESS:
	case EVT_KEY_RELEASE:
		return TRUE;

	default:
		return FALSE;
	}
}

/*=================================================================
Function: OEM_TextGetSelEnd

Description:
get Select End position

Parameter(s):

Return Value:

Comments: none

Side Effects: none

=================================================================*/
int32 OEM_TextGetSelEnd(OEMCONTEXT hTextField) 
{
register TextCtlContext *pContext = (TextCtlContext *) hTextField;

return pContext->wSelEnd;
}

//*****************************************************************************
// touch screen  Input integration
//  
//*****************************************************************************
/*=============================================================================
FUNCTION: TextCtl_IsInRange

DESCRIPTION:This function check if the coordinate passed into is in the coordirange.

PARAMETERS:
dwparam:the coordinate passed into will de checked.
range:  the pointer to the coordirange.

RETURN VALUE:true if successful, otherwise false

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean TextCtl_IsInRange(int16 xpos, int16 ypos, CoordiRange* range)
{
	if (!range)
	{
		//ERR("range is NULL", 0, 0, 0);
	}

	if ((range) &&
		(xpos>= range->xmin) && 
		(xpos <= range->xmax) &&
		(ypos >= range->ymin) &&
		(ypos <= range->ymax))
	{
		return TRUE;
	}
	return FALSE;
}


#ifdef FEATURE_LANG_THAI  //T9 ����MULTTAP��̩������ģʽ
//̩���ַ��Ƿ����ڸ���
static boolean isThaiCnsntChar(AECHAR inputChar)
{
    //�ֲ��ڰ���2~9��
    if( ( inputChar >= 0x0e01 ) && ( inputChar <= 0x0e2e ) )
        return TRUE;
    else
        return FALSE;
}

//̩���ַ��Ƿ�������Ԫ��
static boolean isThaiAboveVowelChar(AECHAR inputChar)
{
    //�ֲ��ڡ�0������ǰ����͡�#����5,6��λ������(��)��׼��Ԫ��
    if( (( inputChar >= 0x0e34 ) && ( inputChar <= 0x0e37 ))
        ||(inputChar == 0x0e31) ||(inputChar == 0x0e47 ) || (inputChar == 0x0e4c ) )
        return TRUE;
    else
        return FALSE;
}

//̩���ַ��Ƿ�������Ԫ��
static boolean isThaiBelowVowelChar(AECHAR inputChar)
{
    //�ֲ��ڡ�0�����ĺ�����
    if( ( inputChar >= 0x0e38 ) && ( inputChar <= 0x0e39 ) )
        return TRUE;
    else
        return FALSE;
}

//̩���ַ��Ƿ���������
static boolean isThaiToneChar(AECHAR inputChar)
{
    //����������,�ֲ��ڡ�#������ǰ�ĸ�
    if( ( inputChar == 0x0e48 ) || ( inputChar == 0x0e49 ) ||
        ( inputChar == 0x0e4a ) || ( inputChar == 0x0e4b ) )
        return TRUE;
    else
        return FALSE;
}

//�ж�ǰһ���ַ��Ƿ�����Ԫ��������
static int moveleftselThaiChar(AECHAR pretwoChar,AECHAR preChar)
{  
    int count = 0 ;
    
    //��Ԫ��������ֻ��һ������� ��ǰ��ĸ��̩�︨��
    if(isThaiToneChar(preChar) && (isThaiAboveVowelChar(pretwoChar) || isThaiBelowVowelChar(pretwoChar)))
    {
        count = 2;
    }    
    else if(isThaiToneChar(preChar) || isThaiAboveVowelChar(preChar) || isThaiBelowVowelChar(preChar))
    {
        count = 1;    
    }         
    return count;       
} //moveleftselThaiChar()

//�жϺ�һ���ַ��Ƿ�����Ԫ��������
static int moverightselThaiChar(AECHAR nexttwoChar,AECHAR nextChar)
{  
    int count = 0 ;
    
    //��Ԫ��������ֻ��һ������� ��ǰ��ĸ��̩�︨��
    if(isThaiToneChar(nexttwoChar) && (isThaiAboveVowelChar(nextChar) || isThaiBelowVowelChar(nextChar)))
    {
        count = 2;
    }    
    else if(isThaiToneChar(nextChar) || isThaiAboveVowelChar(nextChar) || isThaiBelowVowelChar(nextChar))
    {
        count = 1;    
    }         
    return count;       
} //moverightselThaiChar()


#endif /*FEATURE_LANG_THAI*/
 
#endif /* OEMTEXT */
/*---------------------------------- eof ---------------------------------*/
