/*=================================================================================
FILE:          localeuk.c

SERVICES:      Locale specific string and text formatting utilities. 

LANGUAGE:      Ukrainian (Ukraine)

PUBLIC CLASSES:ILocale

DESCRIPTION:   This file describes an interface that provides language
               dependent string formatting for a specific language. The
               intent is that there be one of these objects for each
               language your device supports.  Addition implementations
               may be added later to support additional languages.

                Copyright � 1999-2007 QUALCOMM Incorporated.
                           All Rights Reserved.
                        QUALCOMM Proprietary
=================================================================================*/

/*---------------------------------------------------------------------------------
      Include Files
---------------------------------------------------------------------------------*/

#include "localebase.h"
#include "AEECLSID_LOCALEUK.BID"

/*---------------------------------------------------------------------------------
      Type Declarations
---------------------------------------------------------------------------------*/

typedef struct 
{
   LocaleBase base;
} LocaleUK;


/*---------------------------------------------------------------------------------
      Globals
---------------------------------------------------------------------------------*/

// Format variables, strings, characters and patterns for Ukrainian (Ukraine)
static const int    Number_DecimalDigits                  = 0;
static const AECHAR Number_DecimalSeparator[]             = { ',',0 };
static const AECHAR Number_GroupSeparator[]               = { ' ',0 };
static const int    Number_GroupSize                      = 3;
static const AECHAR Number_NegativePattern[]              = { '-','#',0 };

static const int    Currency_DecimalDigits                = 2;
static const AECHAR Currency_DecimalSeparator[]           = { ',',0 };
static const AECHAR Currency_GroupSeparator[]             = { ' ',0 };
static const int    Currenct_GroupSizes                   = 3;
static const AECHAR Currency_NegativePattern []           = { '-','#',' ',0x433,0x440,0x43d,'.',0 };
static const AECHAR Currency_PositivePattern[]            = { '#',' ',0x433,0x440,0x43d,'.',0 }; 
static const AECHAR Currency_CurrencySymbol[]             = { 0x433,0x440,0x43d,'.',0 }; 

static const int    Percent_DecimalDigits                 = 0;
static const AECHAR Percent_DecimalSeparator[]            = { ',',0 };
static const AECHAR Percent_GroupSeparator[]              = { ' ',0 };
static const int    Percent_GroupSize                     = 3;
static const AECHAR Percent_NegativePattern[]             = { '(','#','%',')',0 };
static const AECHAR Percent_PositivePattern[]             = { '#','%',0 };
static const AECHAR Percent_PercentSymbol[]               = { '%',0 };
static const AECHAR Percent_PermilleSymbol[]              = { 0 };

static const AECHAR NANSymbol[]                           = { 'N','A','N',0 };
static const AECHAR NegativeInfinitySymbol[]              = { '-','i','n','f',0 };
static const AECHAR NegativeSign[]                        = { '-',0 };
static const AECHAR PositiveInfinitySymbol[]              = { 'i','n','f',0 };
static const AECHAR PositiveSign[]                        = { '+',0 };

static const AECHAR DateTime_AMDesignator[]               = { 'A','M',0 };
static const AECHAR DateTime_DateSeparator[]              = { '.',0 };
static const AECHAR DateTime_PMDesignator[]               = { 'P','M',0 };
static const AECHAR DateTime_TimeSeparator[]              = { ':',0 };
static const int DateTime_FirstWeekDay                    = 1;

static const AECHAR DateTime_FullDateLongTimePattern[]    = { 'd',' ','M','M','M','M',' ','y','y','y','y',' ','\'',0x440,'.','\'',' ','H',':','m','m',':','s','s', 0 };  
static const AECHAR DateTime_FullDateShortTimePattern[]   = { 'd',' ','M','M','M','M',' ','y','y','y','y',' ','\'',0x440,'.','\'',' ','H',':','m','m',' ',0 };  
static const AECHAR DateTime_GenDateShortTimePattern[]    = { 'd','d','/','M','M','/','y','y','y','y',' ','h',':','m','m',0 };
static const AECHAR DateTime_GenDateLongTimePattern[]     = { 'd','d','/','M','M','/','y','y','y','y',' ','H',':','m','m',':','s','s',0 };

static const AECHAR DateTime_LongDatePattern[]            = { 'd',' ','M','M','M','M',' ','y','y','y','y',' ','\'',0x440,'.','\'', 0 };
static const AECHAR DateTime_LongTimePattern[]            = { 'H',':','m','m',':','s','s',0 };
static const AECHAR DateTime_MonthDayPattern[]            = { 'd','.',' ','M','M','M',0 };
static const AECHAR DateTime_ShortDatePattern[]           = { 'd','d','.','M','M','.','y','y','y','y',0 };
static const AECHAR DateTime_ShortTimePattern[]           = { 'H','H',':','m','m',0 };
static const AECHAR DateTime_YearMonthPattern[]           = { 'M','M','M',' ','y','y','y','y',0 };

static const AECHAR DateTime_RFC1123Pattern[]             = { 'd','d','d',',',' ','d','d',' ','M','M','M',' ','y','y','y','y',' ','H','H',':','m','m',':','s','s',' ','G','\\','M','T',0  };
static const AECHAR DateTime_SortableDatePattern[]        = { 'y','y','y','y','-','M','M','-','d','d','T','H','H',':','m','m',':','s','s',0 };

static const AECHAR String_HardBreak[]                    = { '\r','\n',0 };
static const AECHAR String_SoftBreak[]                    = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,0 };

static const AECHAR DateTime_ShortDayNames[] = {
0x43f,0x43d, 0, 
0x432,0x442, 0,
0x441,0x440, 0,
0x447,0x442, 0,
0x43f,0x442, 0,
0x441,0x431, 0,
0x43d,0x434, 0,
   0 };

static const AECHAR DateTime_ShortMonthNames[] = {
   0x441,0x456,0x447, 0,
   0x43b,0x44e,0x442, 0,
   0x431,0x435,0x440, 0,
   0x43a,0x432,0x456,0x442, 0,
   0x442,0x440,0x430,0x432, 0,
   0x447,0x435,0x440,0x432, 0,
   0x43b,0x438,0x43f, 0,
   0x441,0x435,0x440,0x43f, 0,
   0x432,0x435,0x440, 0,
   0x436,0x43e,0x432,0x442, 0,
   0x43b,0x438,0x441,0x442, 0,
   0x433,0x440,0x443,0x434, 0,
   0 };

static const AECHAR DateTime_DayNames[] = {
0x43f,0x43e,0x43d,0x435,0x434,0x456,0x43b,0x43e,0x43a , 0,
0x432,0x456,0x432,0x442,0x43e,0x440,0x43e,0x43a , 0,
0x441,0x435,0x440,0x435,0x434,0x430 , 0,
0x447,0x435,0x442,0x432,0x435,0x440 , 0,
0x43f,'\'',0x44f,0x442,0x43d,0x438,0x446,0x44f , 0,
0x441,0x443,0x431,0x43e,0x442,0x430 , 0,
0x43d,0x435,0x434,0x456,0x43b,0x44f , 0,
   0 };

static const AECHAR DateTime_MonthNames[] = {
0x441,0x456,0x447,0x43d,0x44f , 0,
0x43b,0x44e,0x442,0x43e,0x433,0x43e , 0,
0x431,0x435,0x440,0x435,0x437,0x43d,0x44f , 0,
0x43a,0x432,0x456,0x442,0x43d,0x44f , 0,
0x442,0x440,0x430,0x432,0x43d,0x44f , 0,
0x447,0x435,0x440,0x432,0x43d,0x44f , 0,
0x43b,0x438,0x43f,0x43d,0x44f , 0,
0x441,0x435,0x440,0x43f,0x43d,0x44f , 0,
0x432,0x435,0x440,0x435,0x441,0x43d,0x44f , 0,
0x436,0x43e,0x432,0x442,0x43d,0x44f , 0,
0x43b,0x438,0x441,0x442,0x43e,0x43f,0x430,0x434,0x430 , 0,
0x433,0x440,0x443,0x434,0x43d,0x44f , 0,
   0 };


/*---------------------------------------------------------------------------------
      Function Prototypes
---------------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------
    LocaleUK_Ctor 
   --- */
static void LocaleUK_Ctor(LocaleUK *pMe, AEEVTBL(ILocale) *pvt, IShell *piShell, IModule *piModule)
{
   LocaleBase_Ctor(LBASE(pMe), pvt, piShell, piModule);

   // these items are settable strings
   LBASE(pMe)->gtFormats[ILOCALE_CURRENCY_DECIMALSEPARATOR].pawData      = Currency_DecimalSeparator;
   LBASE(pMe)->gtFormats[ILOCALE_CURRENCY_GROUPSEPARATOR].pawData        = Currency_GroupSeparator; 
   LBASE(pMe)->gtFormats[ILOCALE_CURRENCY_NEGATIVEPATTERN].pawData       = Currency_NegativePattern;
   LBASE(pMe)->gtFormats[ILOCALE_CURRENCY_POSITIVEPATTERN].pawData       = Currency_PositivePattern;
   LBASE(pMe)->gtFormats[ILOCALE_CURRENCY_SYMBOL].pawData                = Currency_CurrencySymbol;
   LBASE(pMe)->gtFormats[ILOCALE_NUMBER_DECIMALSEPARATOR].pawData        = Number_DecimalSeparator;
   LBASE(pMe)->gtFormats[ILOCALE_NUMBER_GROUPSEPARATOR].pawData          = Number_GroupSeparator;
   LBASE(pMe)->gtFormats[ILOCALE_NUMBER_NEGATIVEPATTERN].pawData         = Number_NegativePattern;
   LBASE(pMe)->gtFormats[ILOCALE_PERCENT_DECIMALSEPARATOR].pawData       = Percent_DecimalSeparator;
   LBASE(pMe)->gtFormats[ILOCALE_PERCENT_GROUPSEPARATOR].pawData         = Percent_GroupSeparator;
   LBASE(pMe)->gtFormats[ILOCALE_PERCENT_NEGATIVEPATTERN].pawData        = Percent_NegativePattern;
   LBASE(pMe)->gtFormats[ILOCALE_PERCENT_POSITIVEPATTERN].pawData        = Percent_PositivePattern;
   LBASE(pMe)->gtFormats[ILOCALE_PERCENT_SYMBOL].pawData                 = Percent_PercentSymbol;
   LBASE(pMe)->gtFormats[ILOCALE_PERMILLE_SYMBOL].pawData                = Percent_PermilleSymbol;
   LBASE(pMe)->gtFormats[ILOCALE_NANSYMBOL].pawData                      = NANSymbol;
   LBASE(pMe)->gtFormats[ILOCALE_NEGATIVEINIFINITYSYMBOL].pawData        = NegativeInfinitySymbol;
   LBASE(pMe)->gtFormats[ILOCALE_NEGATIVESIGN].pawData                   = NegativeSign;
   LBASE(pMe)->gtFormats[ILOCALE_POSITIVEINIFINITYSYMBOL].pawData        = PositiveInfinitySymbol;
   LBASE(pMe)->gtFormats[ILOCALE_POSITIVESIGN].pawData                   = PositiveSign;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_SHORTDAYNAMES].pawData         = DateTime_ShortDayNames;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_SHORTMONTHNAMES].pawData       = DateTime_ShortMonthNames;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_DAYNAMES].pawData              = DateTime_DayNames;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_MONTHNAMES].pawData            = DateTime_MonthNames;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_AMDESIGNATOR].pawData          = DateTime_AMDesignator;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_DATESEPARATOR].pawData         = DateTime_DateSeparator;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_PMDESIGNATOR].pawData          = DateTime_PMDesignator;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_TIMESEPARATOR].pawData         = DateTime_TimeSeparator;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_FULLDATELTIMEPATTERN].pawData  = DateTime_FullDateLongTimePattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_FULLDATESTIMEPATTERN].pawData  = DateTime_FullDateShortTimePattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_GENDATESHORTTIME].pawData      = DateTime_GenDateShortTimePattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_GENDATELONGTIME].pawData       = DateTime_GenDateLongTimePattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_LONGDATEPATTERN].pawData       = DateTime_LongDatePattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_LONGTIMEPATTERN].pawData       = DateTime_LongTimePattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_MONTHDAYPATTERN].pawData       = DateTime_MonthDayPattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_SHORTDATEPATTERN].pawData      = DateTime_ShortDatePattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_SHORTTIMEPATTERN].pawData      = DateTime_ShortTimePattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_YEARMONTHPATTERN].pawData      = DateTime_YearMonthPattern;
   LBASE(pMe)->gtFormats[ILOCALE_STRING_HARDBREAK].pawData               = String_HardBreak;
   LBASE(pMe)->gtFormats[ILOCALE_STRING_SOFTBREAK].pawData               = String_SoftBreak;

   // these are integers
   LBASE(pMe)->gtFormats[ILOCALE_CURRENCY_DECIMALDIGITS].pawData         = (const AECHAR *)Currency_DecimalDigits;
   LBASE(pMe)->gtFormats[ILOCALE_CURRENCY_DECIMALDIGITS].cFlags          = FORMATTABLE_FLAG_INT;
   LBASE(pMe)->gtFormats[ILOCALE_CURRENCY_GROUPSIZES].pawData            = (const AECHAR *)Currenct_GroupSizes;
   LBASE(pMe)->gtFormats[ILOCALE_CURRENCY_GROUPSIZES].cFlags             = FORMATTABLE_FLAG_INT;
   LBASE(pMe)->gtFormats[ILOCALE_NUMBER_DECIMALDIGITS].pawData           = (const AECHAR *)Number_DecimalDigits;
   LBASE(pMe)->gtFormats[ILOCALE_NUMBER_DECIMALDIGITS].cFlags            = FORMATTABLE_FLAG_INT;
   LBASE(pMe)->gtFormats[ILOCALE_NUMBER_GROUPSIZES].pawData              = (const AECHAR *)Number_GroupSize;
   LBASE(pMe)->gtFormats[ILOCALE_NUMBER_GROUPSIZES].cFlags               = FORMATTABLE_FLAG_INT;
   LBASE(pMe)->gtFormats[ILOCALE_PERCENT_DECIMALDIGITS].pawData          = (const AECHAR *)Percent_DecimalDigits;
   LBASE(pMe)->gtFormats[ILOCALE_PERCENT_DECIMALDIGITS].cFlags           = FORMATTABLE_FLAG_INT;
   LBASE(pMe)->gtFormats[ILOCALE_PERCENT_GROUPSIZES].pawData             = (const AECHAR *)Percent_GroupSize;
   LBASE(pMe)->gtFormats[ILOCALE_PERCENT_GROUPSIZES].cFlags              = FORMATTABLE_FLAG_INT;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_FIRSTWEEKDAY].pawData          = (const AECHAR *)DateTime_FirstWeekDay;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_FIRSTWEEKDAY].cFlags           = FORMATTABLE_FLAG_INT;

   // these are constants
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_RFC1123PATTERN].pawData        = DateTime_RFC1123Pattern; 
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_RFC1123PATTERN].cFlags         = FORMATTABLE_FLAG_CONST;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_SORTABLEDATETIME].pawData      = DateTime_SortableDatePattern;
   LBASE(pMe)->gtFormats[ILOCALE_DATETIME_SORTABLEDATETIME].cFlags       = FORMATTABLE_FLAG_CONST;
}

/* -----------------------------------------------------------------------
    LocaleUK_New  
   --- */
int LocaleUK_New(ILocale **ppo, IShell *piShell, IModule *piModule)
{
   LocaleUK *pMe;

   if(!ppo)
		return(EFAILED);

   pMe = MALLOCREC_VTBL(LocaleUK, ILocale);
   if (!pMe) {
      return ENOMEMORY;
   }
   LocaleUK_Ctor(pMe,GETVTBL(pMe,ILocale),piShell,piModule); 
   *ppo = (ILocale*)(void*)pMe;
   return SUCCESS;
}

/*---------------------------------------------------------------------------------
      CreateInstance
---------------------------------------------------------------------------------*/

int LocaleUK_CreateInstance(IModule *module, IShell *shell, AEECLSID idClass, void **pp)
{
   if (idClass == AEECLSID_LOCALEUK) {
      return LocaleUK_New((ILocale **)pp, shell, module);
   }
   *pp = 0;
   return ECLASSNOTSUPPORT;
}
