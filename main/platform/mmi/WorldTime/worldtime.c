/*=============================================================================

  $Header: //tsg/target/CRP/v1.0/UI/Games/src/Space.c#2 $
$DateTime: 2007/11/28 16:40:02 $
  $Author:  $
  $Change:  $
                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when         who            what, where, why
----------   ---            ---------------------------------------------------
=============================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "AEE.h"
#include "AEEShell.h"
#include "AEEMenu.h"
#include "AEEModGen.h"
#include "AEEAppGen.h"
#include "AEEStdLib.h"
#include "OEMClassIDs.h"
#include "worldtime.brh"
#include "AppComFunc.h"
#include "Appscommon.h"
#if defined(FEATURE_DISP_160X128)
#include "Appscommon_160x128.brh"
#elif defined(FEATURE_DISP_128X128)
#include "Appscommon_color.brh"
#else
#include "Appscommon_color.brh"
#endif


#include "OEMCFGI.h"
#include "OEMOS.h"

#ifndef WIN32
#if !defined( AEE_SIMULATOR)
    #include "db.h"
#endif
#endif//WIN32
#include "worldtime_images.brh"
#include "err.h"
#if defined( AEE_SIMULATOR)
    #define WORLDTIME_IMAGES_RES_FILE "fs:/mod/worldtime/worldtime_images.bar"
    #define WORLDTIME_RES_FILE_LANG   ("fs:/mod/worldtime/en/" WORLDTIME_RES_FILE)
#else
    #define WORLDTIME_RES_FILE_LANG   (AEE_RES_LANGDIR WORLDTIME_RES_FILE)
#endif
/*===========================================================================

                    MACRO DEFINITIONS

===========================================================================*/
//For BREW3.0 support the muti-language re-define the RES maco

#define PT_IN_RECT(a,b,rct)      (boolean)( ((a) >= (rct).x && (a) <= ((rct).x + (rct).dx)) && ((b) >= (rct).y && (b) <= ((rct).y + (rct).dy)) )
#define SPACE_BETWEEN_MENU                  (3)
#define SPACE_BETWEEN_MENU_AND_EDGE (0)

/*===========================================================================

                    TYPE DECLARATIONs

===========================================================================*/
typedef struct _CWorldTime CWorldTime;
struct _CWorldTime {
    AEEApplet   a;
    AEERect     m_rectScreen;// Image Rect containing screen

    IMenuCtl*   m_pMenuCity;
    IImage*     m_pImageBg;
    IImage*     m_pImageBar;
    int         m_xBar;
    int         m_xBg;
    int         m_yBg;
    int         m_widthBg;
    int         m_timeZone;
    int         m_xMenu;//x coordinate of menucity
    int         m_yMenu;//y coordinate of menucity
    int         m_dxMenu;//width of menucity
    int         m_dyMenu;//height of menucity
    IImage*  m_backGround;
    IImage*  m_leftArrow;
    IImage*  m_rightArrow;
};

/*===========================================================================

                       FUNCTION DECLARATIONS

===========================================================================*/
static int  WorldTimeMod_CreateInstance(AEECLSID ClsId,
                                    IShell *pIShell,
                                    IModule *po,
                                    void **ppObj);
int  WorldTimeMod_Load(IShell *ps,
                          void *pHelpers,
                          IModule **pMod);


static boolean WorldTime_HandleEvent(CWorldTime * pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam);
static void CWorldTime_Free(CWorldTime *pme);
static void CWorldTime_DrawCityTime(CWorldTime *pme);
static void Draw_WorldTimeContent(CWorldTime *pme);
static void WorldTime_DrawNextCity(CWorldTime * pMe, boolean left);
static int get_timezone(void);
static void WorldTime_DrawBackGround(CWorldTime * pMe);
/*===========================================================================

                    LOCAL/STATIC DATA

===========================================================================*/
/*=============================================================================
FUNCTION: WorldTimeMod_Load

DESCRIPTION:

PARAMETERS:
   *ps:
   *pHelpers:
   **pMod:

RETURN VALUE:
   int:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
int WorldTimeMod_Load(IShell *ps,
                         void *pHelpers,
                         IModule **pMod)
{
   return AEEStaticMod_New(sizeof(AEEMod),
                           ps,
                           pHelpers,
                           pMod,
                           (PFNMODCREATEINST) WorldTimeMod_CreateInstance,
                           (PFNFREEMODDATA) NULL);


}

/*=============================================================================
FUNCTION: SpaceMod_CreateInstance

DESCRIPTION:

PARAMETERS:
   ClsId:
   *pIShell:
   *po:
   **ppObj:

RETURN VALUE:
   int:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
#if defined(BREW_STATIC_APP)
static int WorldTimeMod_CreateInstance(AEECLSID ClsId,
                                   IShell *pIShell,
                                   IModule *po,
                                   void **ppObj)
#else
int AEEClsCreateInstance(AEECLSID ClsId,
                         IShell *pIShell,
                         IModule *po,
                         void **ppObj)
#endif
{
   CWorldTime     *pme;

   // Allocate the applet class.

   if (!AEEApplet_New(sizeof(CWorldTime),
                      ClsId,
                      pIShell,
                      po,
                      (IApplet **) ppObj,
                      (AEEHANDLER) WorldTime_HandleEvent,
                      (PFNFREEAPPDATA) CWorldTime_Free)) {
      return ENOMEMORY;
   }

   pme = (CWorldTime *) *ppObj;

   return AEE_SUCCESS;
}


/*=============================================================================
FUNCTION:  getCursorPosX

DESCRIPTION: Get the X coordinate of the timezone slection bar

PARAMETERS:
   *pme:

RETURN VALUE:
   None:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void getCursorPosX( CWorldTime* pme)
{
    AEEImageInfo ii         = {0};
    int          timeZone   = pme->m_timeZone;

    IIMAGE_GetInfo( pme->m_pImageBar, &ii);
#ifndef WIN32//wlh ��ʱ�޸�
    switch( timeZone)
    {
        case 12:
            pme->m_xBar = pme->m_widthBg / 2 + ii.cx / 2;
            break;

        case -12:
            pme->m_xBar = pme->m_widthBg / 2 + ii.cx;
            break;

        case -1:
            pme->m_xBar = 0;
            break;

        case -2:
            pme->m_xBar = pme->m_widthBg - ii.cx / 2;
            break;

        default:
            if( timeZone < 0)
            {
                timeZone += 25;
            }
            timeZone ++;
            pme->m_xBar =(int)( (pme->m_widthBg / 24.0 * timeZone ) + 0.5);
            break;
    }
    pme->m_xBar  = pme->m_xBar < 0 ? 0 : pme->m_xBar;
#endif//WIN32
}

/*=============================================================================
FUNCTION:  InitWorldTime

DESCRIPTION: Returns module information

PARAMETERS:
   *pme:

RETURN VALUE:
   PFNMODENTRY:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean InitWorldTime(CWorldTime *pme)
{
    AEEDeviceInfo   di      = {0};
    AEEImageInfo    ii      = {0};
    AEERect          rc;
    AEEImageInfo   iInfo = {0};
    int                  i = 0;
    int                  yArrow = 0;

    ISHELL_GetDeviceInfo( pme->a.m_pIShell, &di);
    SETAEERECT( &pme->m_rectScreen, 0, 0, di.cxScreen, di.cyScreen);

    if( SUCCESS != ISHELL_CreateInstance(pme->a.m_pIShell, AEECLSID_LISTCTL, (void **) &pme->m_pMenuCity) ||
        NULL    == ( pme->m_pImageBg = ISHELL_LoadResImage(pme->a.m_pIShell, WORLDTIME_IMAGES_RES_FILE, IDI_MAP)) ||
        NULL    == ( pme->m_pImageBar = ISHELL_LoadResImage(pme->a.m_pIShell, WORLDTIME_IMAGES_RES_FILE, IDI_BAR)) ||
#ifdef FEATURE_CARRIER_CHINA_VERTU
        NULL    == ( pme->m_backGround= ISHELL_LoadResImage(pme->a.m_pIShell, AEE_APPSCOMMONRES_IMAGESFILE, IDI_MENU_BACKGROUND)) ||
#else
        NULL    == ( pme->m_backGround= ISHELL_LoadResImage(pme->a.m_pIShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BACKGROUND)) ||
#endif
        NULL    == ( pme->m_leftArrow  = ISHELL_LoadResImage(pme->a.m_pIShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_LEFTARROW)) ||
        NULL    == ( pme->m_rightArrow = ISHELL_LoadResImage(pme->a.m_pIShell, AEE_APPSCOMMONRES_IMAGESFILE, IDB_RIGHTARROW))
        )
    {
        CWorldTime_Free( pme);
        return FALSE;
    }

    {
        IIMAGE_GetInfo( pme->m_pImageBg, &ii);
        pme->m_dyMenu = MENUITEM_HEIGHT - 2;
        pme->m_yBg      = TITLEBAR_HEIGHT + (di.cyScreen - BOTTOMBAR_HEIGHT - TITLEBAR_HEIGHT - ii.cy - 2*(pme->m_dyMenu + SPACE_BETWEEN_MENU))/2;
        //pme->m_yBg      = ( di.cyScreen - GetBottomBarHeight( pme->a.m_pIDisplay) - ii.cy) / 2 - (pme->m_dyMenu / 2);
        pme->m_xBg      = ( di.cxScreen - ii.cx) / 2;
        pme->m_widthBg  = ii.cx;
        pme->m_yMenu = pme->m_yBg + ii.cy + SPACE_BETWEEN_MENU;

        IIMAGE_GetInfo(pme->m_leftArrow, &iInfo);
        yArrow = pme->m_yMenu + (pme->m_dyMenu - iInfo.cy)/2;
        pme->m_xMenu = SPACE_BETWEEN_MENU_AND_EDGE + iInfo.cx;
        pme->m_dxMenu = di.cxScreen - (SPACE_BETWEEN_MENU_AND_EDGE*2) - (2*iInfo.cx);

        IIMAGE_Draw(pme->m_backGround, 0, 0);
        IIMAGE_Draw( pme->m_leftArrow, SPACE_BETWEEN_MENU_AND_EDGE, yArrow);

        IIMAGE_Draw( pme->m_rightArrow, di.cxScreen - SPACE_BETWEEN_MENU_AND_EDGE - iInfo.cx, yArrow);
    }


    {
        AEEMenuColors color  = {0};
        color.wMask = MC_BACK | MC_SEL_BACK;
        color.cBack = RGB_WHITE;
        color.cSelBack = RGB_WHITE;
        IMENUCTL_SetColors( pme->m_pMenuCity, &color);

        SETAEERECT(&rc, 
                        pme->m_xMenu, 
                        pme->m_yMenu, 
                        pme->m_dxMenu, 
                        pme->m_dyMenu);
        IMENUCTL_SetRect( pme->m_pMenuCity,&rc);

        for(i=0;i<=24;i++)
        {
            (void)IMENUCTL_AddItem( pme->m_pMenuCity,
                                                WORLDTIME_RES_FILE_LANG,
                                                IDS_CITY_0 + i,
                                                IDS_CITY_0 + i,
                                                NULL,
                                                0);
        }
        IMENUCTL_SetOemProperties(pme->m_pMenuCity, OEMMP_IDF_ALIGN_CENTER);
        IMENUCTL_SetActive( pme->m_pMenuCity, TRUE);
    }
    pme->m_timeZone = get_timezone();
    getCursorPosX( pme);

    return TRUE;
}

/*=============================================================================
FUNCTION: CWorldTime_Free

DESCRIPTION:
   Called by AEEAppGen when the applet's memory is being released.

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void CWorldTime_Free(CWorldTime *pme)
{

    if( pme->m_pMenuCity != NULL)
    {
        IMENUCTL_Release( pme->m_pMenuCity);
    }
    if( pme->m_pImageBg != NULL)
    {
        IIMAGE_Release( pme->m_pImageBg);
    }

    if( pme->m_pImageBar != NULL)
    {
        IIMAGE_Release( pme->m_pImageBar);
    }
    if( pme->m_backGround != NULL)
    {
        IIMAGE_Release( pme->m_backGround);
    }
    if( pme->m_leftArrow != NULL)
    {
        IIMAGE_Release( pme->m_leftArrow);
    }
    if( pme->m_rightArrow != NULL)
    {
        IIMAGE_Release( pme->m_rightArrow);
    }
    //end added
}



/*==============================================================================
����:
    Calendar_FormatDate

˵��:
    �������ʾ��ʱ��ת��Ϊ�ַ�����ʽ��ֻ�������ա�

����:
    seconds                 [in] : �Լ����ʱ�䡣
    resultString            [out]: ���ڴ��ʱ���ַ����Ļ�������
    resultStringLength      [in] : resultString�Ĵ�С(��λ:AECHAR)��

����ֵ:
    TRUE : success
    FALSE: failed

��ע:

==============================================================================*/
boolean Calendar_FormatDate2( uint16 year, uint16 month, uint16 day, AECHAR* resultString, int resultStringLength)
{

    if( NULL == resultString || resultStringLength < 22)
    {
        return FALSE;
    }
    else
    {

        AECHAR          formatString[16];
#ifdef FEATURE_TIME_DATA_SETTING
        byte            dateFormatType      = 0;
        extern int OEM_GetConfig(AEEConfigItem i, void * pBuff, int nSize);
#endif

        MEMSET( resultString, 0, resultStringLength);
        MEMSET( formatString, 0, sizeof(formatString));
        year %= 100; //BUGFIX_V0.1_PR43234, ( 60, 61)

#ifdef FEATURE_TIME_DATA_SETTING

        OEM_GetConfig( CFGI_DATE_FORMAT, &dateFormatType, sizeof( byte));

        switch( dateFormatType)
        {
            case OEMNV_DATEFORM_MDY:
            {
                STRTOWSTR( "%02d/%02d/%02d", formatString, sizeof( formatString));
                WSPRINTF( resultString,
                          resultStringLength,
                          formatString,
                          month,
                          day,
                          year
                      );
            }
            break;

            case OEMNV_DATEFORM_DMY:
            {
                STRTOWSTR( "%02d/%02d/%02d", formatString, sizeof( formatString));
                WSPRINTF( resultString,
                          resultStringLength,
                          formatString,
                          day,
                          month,
                          year
                      );
            }
            break;

            case OEMNV_DATEFORM_YMD:
            default:
            {
                STRTOWSTR( "%02d/%02d/%02d", formatString, sizeof( formatString));
                WSPRINTF( resultString,
                          resultStringLength,
                          formatString,
                          year,
                          month,
                          day
                      );
            }
            break;
        }


#else

        STRTOWSTR( "%02d/%02d/%02d", formatString, sizeof(formatString));
        WSPRINTF( resultString,
                  resultStringLength,
                  formatString,
                  year,
                  month,
                  day
              );

#endif // FEATURE_TIME_DATA_SETTING

        return TRUE;
    }

}

/*==============================================================================
����:
    Calendar_FormatDate

˵��:
    �������ʾ��ʱ��ת��Ϊ�ַ�����ʽ��ֻ�������ա�

����:
    seconds                 [in] : �Լ����ʱ�䡣
    resultString            [out]: ���ڴ��ʱ���ַ����Ļ�������
    resultStringLength      [in] : resultString�Ĵ�С(AECHAR)��

����ֵ:
    TRUE : success
    FALSE: failed

��ע:

==============================================================================*/
boolean Calendar_FormatDate( uint32 seconds, AECHAR* resultString, int resultStringLength)
{

    if( NULL == resultString || resultStringLength < 22)
    {
        return FALSE;
    }
    else
    {

        JulianType      julianInfo;
        GETJULIANDATE( seconds, &julianInfo);

        return Calendar_FormatDate2( julianInfo.wYear,
                            julianInfo.wMonth,
                            julianInfo.wDay,
                            resultString,
                            resultStringLength
                        );
    }
}

/*==============================================================================
����:
    Calendar_FormatTime

˵��:
    �������ʾ��ʱ��ת��Ϊ�ַ�����ʽ��ֻ��ʱ�֡�

����:
    seconds                 [in] : �Լ����ʱ�䡣
    resultString            [out]: ���ڴ��ʱ���ַ����Ļ�������
    resultStringLength      [in] : pBuf �Ŀռ��С(��λ:AECHAR)��

����ֵ:
    TRUE : success
    FALSE: failed

��ע:

==============================================================================*/
boolean Calendar_FormatTime( uint32 seconds, AECHAR* resultString, int resultStringLength)
{

    if( NULL == resultString || resultStringLength < 18)
    {
        return FALSE;
    }
    else
    {

        JulianType      julianInfo       = {0};
        AECHAR         formatString[16] = {0};
        byte              timeFormatType      = 0;

        extern int OEM_GetConfig( AEEConfigItem i, void * pBuff, int nSize);

        if( seconds > 0)
        {
            GETJULIANDATE( seconds, &julianInfo);
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
                STRTOWSTR( "%02d:%02d PM    ", formatString, sizeof( formatString));
            }
            else
            {
                STRTOWSTR( "%02d:%02d AM    ", formatString, sizeof( formatString));
            }

            julianInfo.wHour = ( julianInfo.wHour + 12) % 12;
            if( julianInfo.wHour == 0)
            {
                julianInfo.wHour = 12;
            }
        }
        else
        {
            STRTOWSTR( "%02d:%02d         ", formatString, sizeof( formatString));
        }

        // ��ʽ��ʱ���ַ���
        MEMSET( resultString, 0, resultStringLength);
        WSPRINTF( resultString, resultStringLength, formatString, julianInfo.wHour, julianInfo.wMinute);

        return TRUE;
    }

}

/*==============================================================================
����:
    Calendar_FormatDateTime

˵��:
    �������ʾ��ʱ��ת��Ϊ�ַ�����ʽ, ����������ʱ�֡�

����:
    seconds                 [in] : �Լ����ʱ�䡣
    resultString            [out]: ���ڴ��ʱ���ַ����Ļ�������
    resultStringLength      [in] : pBuf �Ŀռ��С(��λ:AECHAR)��

����ֵ:
    TRUE : success
    FALSE: failed

��ע:

==============================================================================*/
boolean Calendar_FormatDateTime( uint32 seconds, AECHAR* resultString, int resultStringLength)
{

    if( NULL == resultString || resultStringLength < 40)
    {
        return FALSE;
    }
    else
    {

        int len = 0;
        if( !Calendar_FormatTime( seconds, resultString, resultStringLength))
        {
            return FALSE;
        }

        // �ӿո�
        len = WSTRLEN( resultString);
        resultString[len ++] = (AECHAR)' ';

        if( !Calendar_FormatDate( seconds, &resultString[len], resultStringLength - len*2))
        {
            return FALSE;
        }

        return TRUE;
    }

}
/*=============================================================================
FUNCTION: drawImageWithOffset

DESCRIPTION:
   Draw image with appointed offset at appointed position.

PARAMETERS:
   *pme:
   *image:the object image going to draw
   x        :X coordinate of the position to draw the image
   y        :Y coordinate of the position to draw the image
   *pRectOffset:offset rectangle
   
RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void drawImageWithOffset( CWorldTime *pMe, IImage* image, int x, int y, AEERect *pRectOffset)
{

    if( image != NULL)
    {

        AEERect oldClip = {0};
        AEERect clip    = {0};

        SETAEERECT( &clip, x, y, pRectOffset->dx, pRectOffset->dy);
        IDISPLAY_GetClipRect( pMe->a.m_pIDisplay, &oldClip);
        IDISPLAY_SetClipRect( pMe->a.m_pIDisplay, &clip);

        IIMAGE_SetOffset( image, pRectOffset->x, pRectOffset->y);
        IIMAGE_SetDrawSize( image, pRectOffset->dx, pRectOffset->dy);
        IIMAGE_Draw( image, x, y);
        IIMAGE_SetOffset( image, 0, 0);
        IIMAGE_SetDrawSize( image, 9999, 9999);

        IDISPLAY_SetClipRect( pMe->a.m_pIDisplay, &oldClip);
    }
}
/*=============================================================================
FUNCTION: CWorldTime_DrawCityTime

DESCRIPTION:
   Draw city name and current time.

PARAMETERS:
   *pme:

RETURN VALUE:
   None

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void CWorldTime_DrawCityTime(CWorldTime *pme)
{
    AECHAR    text[32];
    uint32      sec;
    int           timeZone = pme->m_timeZone;
    int           local    = get_timezone();
    AEERect   rc = {0};

    if( timeZone < 0)
    {
        timeZone += 25;
    }
    IMENUCTL_SetSel(pme->m_pMenuCity,timeZone + IDS_CITY_0);

    sec = GETUTCSECONDS() + (local==pme->m_timeZone?LOCALTIMEOFFSET( 0):pme->m_timeZone*3600);
    Calendar_FormatDateTime( sec, text, sizeof( text));
    SETAEERECT(&rc, 
                        pme->m_xMenu, 
                        pme->m_yMenu + SPACE_BETWEEN_MENU + pme->m_dyMenu,
                        pme->m_dxMenu, 
                        pme->m_dyMenu);

    drawImageWithOffset(pme, pme->m_backGround, rc.x, rc.y, &rc);
    IDISPLAY_SetColor(pme->a.m_pIDisplay, CLR_USER_TEXT, RGB_WHITE);
    IDISPLAY_DrawText(pme->a.m_pIDisplay, 
                                AEE_FONT_NORMAL, 
                                text, 
                                -1, 
                                rc.x, 
                                rc.y, 
                                &rc, 
                                IDF_ALIGN_CENTER | IDF_TEXT_TRANSPARENT);

    IMENUCTL_Redraw( pme->m_pMenuCity);
    IDISPLAY_UpdateEx(pme->a.m_pIDisplay, FALSE);
}

/*=============================================================================
FUNCTION:  Draw_WorldTimeContent

DESCRIPTION: Returns module information

PARAMETERS:
   *pme:
   offset

RETURN VALUE:
   PFNMODENTRY:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static void Draw_WorldTimeContent(CWorldTime *pme)
{
    if( pme->m_pImageBg && pme->m_pImageBar)
    {
        //IDISPLAY_FillRect( pme->a.m_pIDisplay, &pme->m_rectScreen, RGB_BLACK);
        IIMAGE_Draw( pme->m_pImageBg, pme->m_xBg, pme->m_yBg);
        ERR("pme->m_yBg::::%d,",pme->m_yBg,0,0);
        IIMAGE_Draw( pme->m_pImageBar, pme->m_xBg + pme->m_xBar, pme->m_yBg);
        
        //draw title
        {
        TitleBar_Param_type tBarParam = {0};
        AECHAR title[20];
        ISHELL_LoadResString( pme->a.m_pIShell,
                                        WORLDTIME_RES_FILE_LANG,
                                        IDS_TITLE,
                                        title,
                                        sizeof( title));
        tBarParam.dwAlignFlags = IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE;
        tBarParam.pwszTitle = title;
        DrawTitleBar(pme->a.m_pIDisplay, &tBarParam);
        }//end added
        // draw bottom bar
        {
            BottomBar_Param_type BarParam = { 0 };
            BarParam.eBBarType = BTBAR_BACK;
            DrawBottomBar(pme->a.m_pIDisplay, &BarParam);
        }

        CWorldTime_DrawCityTime(pme);
    }
}



static void WorldTime_DrawNextCity(CWorldTime * pme, boolean left)
{

    if( pme->m_pImageBg && pme->m_pImageBar)
    {
        AEERect         rect = {0};
        AEEImageInfo    ii   = {0};

        IIMAGE_GetInfo( pme->m_pImageBar, &ii);
        SETAEERECT( &rect, pme->m_xBar, 0, ii.cx, ii.cy);
        ERR("ii.cy:::::%d",pme->m_yBg,0,0);
        drawImageWithOffset( pme, pme->m_pImageBg, pme->m_xBg + pme->m_xBar, pme->m_yBg, &rect);

        pme->m_timeZone += left ? -1 : 1;
        if( pme->m_timeZone == -13)
        {
            pme->m_timeZone = 12;
        }
        else if( pme->m_timeZone == 13)
        {
            pme->m_timeZone = -12;
        }
        getCursorPosX( pme);

        SETAEERECT( &rect, pme->m_xBg, pme->m_yBg, pme->m_widthBg, pme->m_rectScreen.dy);
        IDISPLAY_SetClipRect( pme->a.m_pIDisplay, &rect);
        IIMAGE_Draw( pme->m_pImageBar, pme->m_xBg + pme->m_xBar, pme->m_yBg);
        ERR("pme->m_yBg:::::%d",pme->m_yBg,0,0);
        IDISPLAY_SetClipRect( pme->a.m_pIDisplay, &pme->m_rectScreen);

        CWorldTime_DrawCityTime(pme);
        IDISPLAY_Update(pme->a.m_pIDisplay);
    }
}

/*=============================================================================
FUNCTION:  WorldTime_HandleEvent

DESCRIPTION:

PARAMETERS:
   *pme:

RETURN VALUE:
   boolean:

COMMENTS:

SIDE EFFECTS:

SEE ALSO:

=============================================================================*/
static boolean WorldTime_HandleEvent(CWorldTime * pMe, AEEEvent eCode, uint16 wParam, uint32 dwParam)
{
    switch (eCode)
    {
        case EVT_APP_START:
            InitWorldTime(pMe);
            Draw_WorldTimeContent(pMe);
            return TRUE;

        case EVT_APP_SUSPEND:
            IMENUCTL_SetActive( pMe->m_pMenuCity, FALSE);
            return TRUE;

        case EVT_APP_RESUME:
            WorldTime_DrawBackGround(pMe);
            IMENUCTL_SetActive( pMe->m_pMenuCity, TRUE);

            Draw_WorldTimeContent(pMe);
            return TRUE;

        case EVT_APP_STOP:
            return (TRUE);

        case EVT_KEY:
            switch (wParam)
            {
                case AVK_SOFT2:
                case AVK_CLR:
                    CWorldTime_Free( pMe);
                    ISHELL_CloseApplet(pMe->a.m_pIShell, FALSE);
                    return TRUE;

                case AVK_LEFT:
                case AVK_UP:
                    WorldTime_DrawNextCity(pMe, TRUE);
                    return TRUE;

                case AVK_RIGHT:
                case AVK_DOWN:
                    WorldTime_DrawNextCity(pMe, FALSE);
                    return TRUE;
            }
            return TRUE;

        default:
            break;
    }

    return (FALSE);
}
/*=======================================================================
get timezone

offset 1h per timezone!
=======================================================================*/
static int get_timezone(void)
{
#if defined( AEE_SIMULATOR)
    return 8;
#else
    boolean daylight = 0;
    int32   local    = LOCALTIMEOFFSET( &daylight);
    int     timezone = local / 3600;
    if( daylight)
    {
        timezone --;
    }
    return timezone;
#endif
}

static void WorldTime_DrawBackGround(CWorldTime * pMe)
{
    AEEImageInfo   iInfo = {0};
    int yArrow = 0;

    IIMAGE_GetInfo(pMe->m_leftArrow, &iInfo);
    yArrow = pMe->m_yMenu + (pMe->m_dyMenu - iInfo.cy)/2;
    if(pMe->m_backGround)
        IIMAGE_Draw(pMe->m_backGround, 0, 0);
    if(pMe->m_leftArrow)
        IIMAGE_Draw( pMe->m_leftArrow, SPACE_BETWEEN_MENU_AND_EDGE, yArrow);
    if(pMe->m_rightArrow)
        IIMAGE_Draw( pMe->m_rightArrow, pMe->m_rectScreen.dx - SPACE_BETWEEN_MENU_AND_EDGE - iInfo.cx, yArrow);
}
