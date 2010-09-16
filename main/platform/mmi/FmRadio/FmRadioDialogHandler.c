/*==============================================================================
// �ļ���
//        fmRadioDialogHandler.c
//        2007-11-11 Gemsea����汾(Draft Version)
//        ��Ȩ����(c) 2007 Anylook.,LTD.
//
// �ļ�˵����
//
// ���ߣ�2007-11-11
// �������ڣ�Gemsea
// ��ǰ�汾��Draft
==============================================================================*/

/*==============================================================================
�޸���ʷ��¼��
ʱ       ��      �� �� ��     �޸����ݡ�λ�ü�ԭ��
-----------      ----------     -----------------------------------------------
2007-11-11              ��ʼ�汾
==============================================================================*/


/*==============================================================================
                                 ���ļ��������ⲿ�ļ�
==============================================================================*/
#include "FmRadio_priv.h"
// add these @08.01.22
#include "appscommon.h"
#include "appscommon.brh"
#include "appscommonimages.brh"
#include "fmradiols.brh"
#include "Hs_mb6550.h"
/*==============================================================================
                                 �궨��ͳ���
==============================================================================*/
// �رնԻ����
#define CLOSE_DIALOG(DlgRet)  {    \
                                     pMe->m_eDlgRet = DlgRet; \
                                     (void) ISHELL_EndDialog(pMe->m_pShell);  \
                                 }
// ������ײ���ʾ���궨��
#define DRAW_BOTTOMBAR(x)                           \
{                                                   \
    BottomBar_Param_type BarParam;                  \
    MEMSET(&BarParam, 0, sizeof(BarParam));         \
    BarParam.eBBarType = x;                         \
    DrawBottomBar(pMe->m_pDisplay, &BarParam);      \
}
/*==============================================================================
                                 ���Ͷ���
==============================================================================*/

/*==============================================================================
                                 ��������
==============================================================================*/
// �Ի��� IDD_FM_RADIO_MAIN �¼�������
static boolean  HandleFmRadioMainDialogEvent(CFmRadio *pMe,
                                             AEEEvent eCode,
                                             uint16   wParam,
                                             uint32   dwParam);

// add these @08.01.22
#define CHANNEL_LIST_MENU_ITEM_ID_FIRST     8000

#define CHANGE_CHANNEL_ANTICLOCKWISE        0
#define CHANGE_CHANNEL_CLOCKWISE            1
#define BEGIN_TO_REFRESH_CHANNEL_LIST       TRUE
#define CONTINUE_TO_REFRESH_CHANNEL_LIST    FALSE
#define FM_VOLUME_X                         5
#define FM_VOLUME_Y                         60
static boolean handleKeyEvent( CFmRadio *pMe, uint16 key, uint32 keyModifier);
static boolean handleCommandEvent( CFmRadio *pMe, uint16 itemId);
static void moveOperationModeTo( CFmRadio *pMe, OpModeTypeEnum newMode);

static void changeChannelAnticlockwise( CFmRadio *pMe);
static boolean changeChannelClockwise( CFmRadio *pMe);
static boolean changeChannel( CFmRadio *pMe, int direction);
static boolean setChannelTo( CFmRadio *pMe, uint16 theNewChannel);
static void changeVolume( CFmRadio *pMe, uint16 keyCode);
static boolean hideMenu( CFmRadio *pMe);
static void popTuningModeSelectMenu( CFmRadio *pMe);
static void FMRadioMute( CFmRadio *pMe);
static void hideTuningModeSelectMenu( CFmRadio *pMe);
static void popOptionMenu( CFmRadio *pMe);

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT

static void popChannelListOptionMenu( CFmRadio *pMe);

static void refreshChannelList( CFmRadio *pMe, boolean begin);
static void refreshChannelListCB( void *pme);
static void showChannelList( void *pme);
static boolean hideChannelList( CFmRadio *pMe);
static void showChannelEditingScreen( CFmRadio *pMe);
static void hideChannelEditingScreen( CFmRadio *pMe);
#endif
static int  getIndexByMenuItemId( CFmRadio *pMe, uint16 menuItemId);
static void convertChannelValueToText( int channel, AECHAR *textBuffer, int bufferSizeInBytes);
static int  convertChannelValueFromText( AECHAR *textBuffer);
static boolean channelNumberIsvalid( AECHAR *text);

static void paint( CFmRadio *pMe);
static void repaint( CFmRadio *pMe, boolean immediately);

static void drawBg( CFmRadio *pMe);
static void drawOperationPrompt( CFmRadio *pMe, int16 resId, RGBVAL color);
static void drawChannelIndicator( CFmRadio *pMe);
static void drawVolumeIndicator( CFmRadio *pMe);

static void drawSoftkey( CFmRadio *pMe);
#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
static void drawRefreshListPrompt( void *pMe);
static void stopDrawRefreshListPrompt( CFmRadio *pMe);
#endif
static void drawLedLight( CFmRadio *pMe);

static void drawImage( CFmRadio *pMe, char *resFile, int16 resId, int x, int y);
static void drawImageWithOffset( CFmRadio *pMe, char *resFile, int16 resId, int x, int y, AEERect *pRectOffset);
static void drawText( CFmRadio *pMe,
                  char     *pResFile,
                  int16    resId,
                  AEERect  *rect,
                  RGBVAL   theTextColor,
                  AEEFont  font,
                  uint32   dwAlignFlags
              );

static void drawText2( IDisplay *pDisplay,
                AECHAR   *text,
                AEERect  *rect,
                RGBVAL   theTextColor,
                AEEFont  font,
                uint32   dwAlignFlags
            );

static void drawPrompt( CFmRadio* pMe, uint16 stringResId, BottomBar_e_Type bottomBarType, boolean informative);

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
extern void FmRadio_ClearChanList( CFmRadio* pMe);
extern void FmRadio_ChanList_EnumInit( CFmRadio* pMe);
extern void FmRadio_ChanList_EnumInit_WithLoop( CFmRadio* pMe);
extern boolean FmRadio_ChanList_EnumNext( CFmRadio* pMe);
boolean FmRadio_ChanList_EnumPrevious_WithLoop( CFmRadio* pMe);
boolean FmRadio_ChanList_EnumNext_WithLoop( CFmRadio* pMe);
extern sChanInfo* FmRadio_ChanList_GetCurrent( CFmRadio* pMe);
extern sChanInfo* FmRadio_ChanList_GetCurrent_WithLoop( CFmRadio* pMe);
extern sChanInfo* FmRadio_ChanList_GetByIndex( CFmRadio* pMe, int index);
extern boolean FmRadio_ChanList_DeleteByIndex( CFmRadio* pMe, int index);
extern boolean FmRadio_IsChannelValid( uint16 channel);
#endif
void resume( CFmRadio* pMe);
void FmRadio_ShowDialog(CFmRadio *pMe, uint16 dlgResId);

static void FmRadio_DrawVolumeImage(CFmRadio *pMe, uint16 ResID, int x, int y);
static void FmRadio_RefreshVolumeImage(CFmRadio *pMe);


/*==============================================================================
                                 ��������
==============================================================================*/


/*==============================================================================
����:
       FmRadio_ShowDialog
˵��:
       ������״̬����������������Ӧ�Ի�����ԴID�ĶԻ���

����:
       pMe [In]: ָ��FmRadio Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       dlgResId [in]���Ի�����ԴID���������ݸ�ID�����Ի���

����ֵ:
       ��

��ע:


==============================================================================*/
void FmRadio_ShowDialog(CFmRadio *pMe, uint16 dlgResId)
{
    int nRet = SUCCESS;
    char *resfile = FMRADIOLN_RES_FILE;

    // At most one dialog open at once
    if (ISHELL_GetActiveDialog(pMe->m_pShell) != NULL)
    {
        // Looks like there is one dialog already opened.
        // Flag an error an return without doing anything.
        return;
    }

    nRet = ISHELL_CreateDialog(pMe->m_pShell, resfile, dlgResId, NULL);

//    if (nRet != SUCCESS)
//    {
//    }
}

/*==============================================================================
����:
       FmRadio_RouteDialogEvent
˵��:
       ������BREW�¼�·�ɸ���ǰ��Ի�����¼���������

����:
       pMe [In]: ָ��FmRadio Applet����ṹ��ָ��,�ýṹ����С������ض���Ϣ.
       eCode [in]���¼����롣
       wParam [in]�����¼�eCode���������ݡ�
       dwParam [in]�����¼�eCode���������ݡ�

����ֵ:
       TRUE�������¼�������
       FALSE�������¼�û������

��ע:

==============================================================================*/
boolean FmRadio_RouteDialogEvent(CFmRadio   *pMe,
                                 AEEEvent   eCode,
                                 uint16     wParam,
                                 uint32     dwParam
                                )
{
    if ((NULL == pMe || NULL == pMe->m_pActiveDlg))
    {
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND    
        if((FALSE == pMe->runOnBackground))
        {
            return FALSE;
        }
#endif        
    }

    return HandleFmRadioMainDialogEvent( pMe,eCode,wParam,dwParam);
}

/*==============================================================================
������
       HandleFmRadioMainDialogEvent
˵����
       IDD_FM_RADIO_MAIN�Ի����¼�������

������
       pMe [in]��ָ��FmRadio Applet����ṹ��ָ�롣�ýṹ����С������ض���Ϣ��
       eCode [in]���¼����롣
       wParam���¼�������ݡ�
       dwParam���¼�������ݡ�

����ֵ��
       TRUE�������¼�������
      FALSE�������¼������ԡ�

��ע��

==============================================================================*/
static void closeIncomingSmsPrompt( void* pUser)
{
    CFmRadio *pMe = (CFmRadio*)pUser;

    pMe->newSmsIncoming = FALSE;
    resume(pMe);
}

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT

static void resumeChannelList( CFmRadio *pMe)
{
    switch( pMe->opMode)
    {
        case FM_RADIO_OPMODE_EDIT_CHANNEL_LIST:
        case FM_RADIO_OPMODE_PRMOPT_CHANNEL_LIST_IS_EMPTY:
        {
            showChannelList( pMe);
            IDISPLAY_Update( pMe->m_pDisplay);
        }
        break;
        case FM_RADIO_OPMODE_EDIT_CHANNEL:
            handleCommandEvent( pMe, pMe->CurrentItemId);
        break; 

        case FM_RADIO_OPMODE_EDIT_CHANNEL_LIST_OPTION_SELECTION:
        {
            showChannelList( pMe);
            IDISPLAY_Update( pMe->m_pDisplay);
            popChannelListOptionMenu( pMe);
            IDISPLAY_Update( pMe->m_pDisplay);
        }
        break;
    }

	pMe->m_bAppIsReady = TRUE;
}
#endif

void resume( CFmRadio* pMe)
{
	switch( pMe->opMode)
	{

		case FM_RADIO_OPMODE_AUTO_TUNE:
	#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
		case FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST:
		{
			pMe->opMode = FM_RADIO_OPMODE_PLAY;
		}
	#endif
		case FM_RADIO_OPMODE_PLAY:
	#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
		case FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST_CONFIRM:
	#endif
#if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
		case FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL:
#endif
			repaint( pMe, FALSE);
		break;

		case FM_RADIO_OPMODE_MODE_SELECTION:
		{
			ISHELL_SetTimer( pMe->m_pShell, 300, (PFNNOTIFY)popTuningModeSelectMenu, pMe);
		}
		break;

		case FM_RADIO_OPMODE_OPTION_SELECTION:
		{
			ISHELL_SetTimer( pMe->m_pShell, 300, (PFNNOTIFY)popOptionMenu, pMe);
		}
		break;
	#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
		case FM_RADIO_OPMODE_EDIT_CHANNEL_LIST:
		case FM_RADIO_OPMODE_EDIT_CHANNEL_LIST_OPTION_SELECTION:
		case FM_RADIO_OPMODE_EDIT_CHANNEL:
		case FM_RADIO_OPMODE_PRMOPT_CHANNEL_LIST_IS_EMPTY:
		{
			ISHELL_SetTimer( pMe->m_pShell, 10, (PFNNOTIFY)resumeChannelList, pMe);
		}
		break;
	#endif
	}
}
static boolean  HandleFmRadioMainDialogEvent(CFmRadio *pMe,
                                     AEEEvent eCode,
                                     uint16   wParam,
                                     uint32   dwParam
)
{

    switch (eCode)
    {

        case EVT_DIALOG_INIT:
        {
            pMe->drawRefreshListPrompt  = FALSE;
            pMe->globalSearching        = FALSE;
           return TRUE;
        }

        case EVT_DIALOG_START:
        {
			resume( pMe);
            return TRUE;
        }

        case EVT_USER_REDRAW:
        {
            paint( pMe);
            return TRUE;
        }

        case EVT_DIALOG_END:
        {
        #if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
            if(FM_RADIO_OPMODE_EDIT_CHANNEL == pMe->opMode)
            {
                pMe->edit_chann_interrupt = 1;
                ITEXTCTL_GetText( pMe->pText, pMe->EditChannel, sizeof( pMe->EditChannel));
            }
        #endif
            hideMenu( pMe);
            ISHELL_CancelTimer( pMe->m_pShell, NULL, (void*)pMe);
            return TRUE;
        }

		case EVT_KEY_PRESS:
		case EVT_KEY_RELEASE:
		case EVT_KEY_HELD:
		{
		#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
			if( pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL
#if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
				|| pMe->opMode == FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL
#endif
			)
			{
				if( !( eCode == EVT_KEY_RELEASE && wParam == AVK_STAR))
				{
					ITEXTCTL_HandleEvent( pMe->pText, eCode, wParam, dwParam);
				}
			}
		#else
		#if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
			if ( pMe->opMode == FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL)
			{
				if( !( eCode == EVT_KEY_RELEASE && wParam == AVK_STAR))
				{
					ITEXTCTL_HandleEvent( pMe->pText, eCode, wParam, dwParam);
				}
			}
		#endif
		#endif
		#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
			if( pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL &&
				( eCode == EVT_KEY_RELEASE || eCode == EVT_KEY_HELD)
			)
			{
				   if (WSTRLEN(ITEXTCTL_GetTextPtr(pMe->pText)) > 0)
				   {
					   // Option     Delete
					   //IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_SAVE_DELETE);
					   DRAW_BOTTOMBAR(BTBAR_SAVE_DELETE)
				   }
				   else
				   {
					   //Option     Back
					   // IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_SAVE_BACK);
					   DRAW_BOTTOMBAR(BTBAR_SAVE_BACK)
				   }
			}
		#endif
		}
		return TRUE;


        case EVT_GSENSOR_SHAKE:
        case EVT_KEY:
        {

            if( handleKeyEvent( pMe, wParam, dwParam))
            {
                return TRUE;
            }
        }
		break;

        case EVT_COMMAND:
        {
            if( handleCommandEvent( pMe, wParam))
            {
                return TRUE;
            }
        }
		break;
    }

    return FALSE;
} // HandleMainDialogEvent

// add these @08.01.22

#if defined( FEATURE_FMRADIO_NO_MODE_SELECT)
static void tuneVolumeByLeftRightArrowKey( CFmRadio* pMe)
{

	static int x = 0;

	if( pMe->tuneVolumeByLeftRightArrowKey)
	{
		repaint( pMe, FALSE);
		ISHELL_SetTimer( pMe->m_pShell, 200 + x, (PFNNOTIFY)tuneVolumeByLeftRightArrowKey, pMe);

		x += 10;
	}
	else
	{
		x = 0;
	}
}

static void tuneVolumeByLeftRightArrowKeyCloseCb( CFmRadio* pMe)
{
	pMe->tuneVolumeByLeftRightArrowKey = FALSE;
	repaint( pMe, FALSE);
}
#endif

static boolean handleKeyEvent( CFmRadio *pMe, uint16 key, uint32 keyModifier)
{

    if( ( ( pMe->opMode  == FM_RADIO_OPMODE_MODE_SELECTION   ||
            pMe->opMode  == FM_RADIO_OPMODE_OPTION_SELECTION ||
         #if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
            pMe->opMode  == FM_RADIO_OPMODE_EDIT_CHANNEL_LIST_OPTION_SELECTION ||
         #endif
            pMe->opMode  == FM_RADIO_OPMODE_USER_MSG
         #if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
            || (pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL_LIST && key != AVK_INFO)
         #endif
          )
          &&
          key != AVK_SELECT &&
          key != AVK_SOFT1  &&
          key != AVK_SOFT2  &&
            key != AVK_CLR     &&
            key != AVK_STAR
        )
        &&
        IMENUCTL_HandleEvent( pMe->m_pMenu, EVT_KEY, key, keyModifier)
    )
    {
        return TRUE;
    }

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
	if( pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL
#if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
		|| pMe->opMode == FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL
#endif
	)
#else
#if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
	if ( pMe->opMode == FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL)
#endif
#endif
    {
        //boolean handle_key_success = FALSE;
        if (key == AVK_STAR)
        {
        #if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
			int len = 0;
			ITEXTCTL_GetText( pMe->pText, pMe->directInputChannel, sizeof( pMe->directInputChannel));
			len = WSTRLEN( pMe->directInputChannel);
			pMe->directInputChannel[len ++] = '.';
			pMe->directInputChannel[len]    = 0;

			ITEXTCTL_SetText( pMe->pText, pMe->directInputChannel, len);
			ITEXTCTL_SetCursorPos( pMe->pText, TC_CURSOREND);
		#endif
			return TRUE;
        }
        else if( key == AVK_POUND)
        {
            return TRUE;
        }
        else
        {
			if( ITEXTCTL_HandleEvent( pMe->pText, EVT_KEY, key, keyModifier))
			{
#if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
			  if( pMe->opMode == FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL)
			  {
				  return TRUE;
			  }
#endif

                   if (WSTRLEN(ITEXTCTL_GetTextPtr(pMe->pText)) > 0)
                   {
                       // Option     Delete
                       //IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_SAVE_DELETE);
                       DRAW_BOTTOMBAR(BTBAR_SAVE_DELETE)
                   }
                   else
                   {
                       //Option     Back
                       // IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_SAVE_BACK);
                       DRAW_BOTTOMBAR(BTBAR_SAVE_BACK)
                   }

                   return TRUE;
               }
               else if (ITEXTCTL_GetCursorPos(pMe->pText) == 0 && (WSTRLEN(ITEXTCTL_GetTextPtr(pMe->pText)) > 0 ))
               {
                   return TRUE;
               }
        }

    }

    if(FM_RADIO_OPMODE_USER_MSG == pMe->opMode)
    {
        if(AVK_SELECT == key)
        {
            ISHELL_SendEvent(pMe->m_pShell,
                             AEECLSID_APP_MUSICPLAYER,
                             EVT_CLOSEAPP,
                             0,
                             0);  
            
            pMe->opMode = FM_RADIO_OPMODE_PLAY;
            
            ISHELL_StartApplet(pMe->m_pShell,AEECLSID_APP_FMRADIO);

            return TRUE;              
        }
        else if(AVK_CLR == key)
        {
            ISHELL_CloseApplet(pMe->m_pShell, FALSE);
            return TRUE;   
        }
    }
    
#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT

    if(FM_RADIO_OPMODE_CHANNEL_LIST_DELETE == pMe->opMode)
    {
        if(AVK_SELECT == key)
        {
    		if( FmRadio_ChanList_DeleteByIndex( pMe, pMe->channelListMenuSelectedItemIndex))
    		{
    			IMENUCTL_DeleteItem( pMe->m_pMenu, IMENUCTL_GetItemID( pMe->m_pMenu, pMe->channelListMenuSelectedItemIndex));
    		}
    	    pMe->opMode = FM_RADIO_OPMODE_EDIT_CHANNEL_LIST;
            showChannelList( pMe);
        }
        else if(AVK_CLR == key)
        {
            pMe->opMode = FM_RADIO_OPMODE_EDIT_CHANNEL_LIST;
            showChannelList( pMe);            
        }  
        return TRUE;
    }
    else if(FM_RADIO_OPMODE_CHANNEL_LIST_DELETE_ALL == pMe->opMode)
    {
        if(AVK_SELECT == key)  
        {
    		if( pMe->byChannelMax > 0)
    		{
    			pMe->m_bAppIsReady = FALSE;
    			FmRadio_ClearChanList( pMe);
    			FmRadio_SaveChannelList( pMe);
    			pMe->m_bAppIsReady = TRUE;
    		}
            pMe->opMode = FM_RADIO_OPMODE_EDIT_CHANNEL_LIST;  
    		showChannelList( pMe);            
	    }
        else if(AVK_CLR == key)
        {
            pMe->opMode = FM_RADIO_OPMODE_EDIT_CHANNEL_LIST;
    		showChannelList( pMe);              
        }
        return TRUE;
    }
#endif
    #if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
	if( (key >= AVK_0 && key <= AVK_9) && pMe->opMode == FM_RADIO_OPMODE_PLAY)
        {
            AEERect     rect        = {0};
            int         fontHeight  = IDISPLAY_GetFontMetrics( pMe->m_pDisplay, AEE_FONT_BOLD, 0, 0);
            int         width       = 64;

            pMe->directInputChannel[0] = (AECHAR)( key - AVK_0 + '0');
            pMe->directInputChannel[1] = 0;

		#if defined(FEATURE_DISP_128X128)
			width = 40;
            SETAEERECT( &rect, ( 128 - width) / 2, ( 128 - fontHeight) / 2, width, fontHeight);
        #elif defined(FEATURE_DISP_160X128)
			width = 40;
            SETAEERECT( &rect, ( 160 - width) / 2, ( 128 - fontHeight) / 2, width, fontHeight);
        #else
        	SETAEERECT( &rect, ( 176 - width) / 2, ( 220 - fontHeight) / 2, width, fontHeight);
        #endif
            ITEXTCTL_SetRect( pMe->pText, &rect);
            ITEXTCTL_SetProperties(pMe->pText, TP_FIXSETRECT|TP_FIXOEM|TP_FIXSETRECT|TP_FOCUS_NOSEL);
#if defined FEATURE_FM_SPACE_50KHZ
            ITEXTCTL_SetMaxSize( pMe->pText, 6);
#else
		#if defined(FEATURE_DISP_128X128)
            ITEXTCTL_SetMaxSize( pMe->pText, 4);
        #elif defined(FEATURE_DISP_128X128)
        	ITEXTCTL_SetMaxSize( pMe->pText, 4);
        #else
        	ITEXTCTL_SetMaxSize( pMe->pText, 5);
        #endif
#endif			
            ITEXTCTL_SetActive( pMe->pText, TRUE);
            ITEXTCTL_SetText( pMe->pText, pMe->directInputChannel, WSTRLEN( pMe->directInputChannel));
            ITEXTCTL_SetCursorPos( pMe->pText, TC_CURSOREND);

            moveOperationModeTo( pMe, FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL);
            return TRUE;
    }
    #endif


    switch( key)
    {
#if defined( AEE_SIMULATOR)
        case AVK_SOFT1:
#else
        case AVK_SELECT:
#endif
        {        	
        #if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
            if( pMe->opMode != FM_RADIO_OPMODE_EDIT_CHANNEL_LIST    &&
                IMENUCTL_IsActive( pMe->m_pMenu)
            )
            {
                return handleCommandEvent( pMe, IMENUCTL_GetSel( pMe->m_pMenu));
            }
		#else
			if(IMENUCTL_IsActive( pMe->m_pMenu))
            {
                return handleCommandEvent( pMe, IMENUCTL_GetSel( pMe->m_pMenu));
            }
		#endif

            if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
            {
                popOptionMenu( pMe);
            }
        #if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
            else if( pMe->opMode == FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST_CONFIRM)
            {
                pMe->globalSearching = TRUE;
                refreshChannelList( pMe, BEGIN_TO_REFRESH_CHANNEL_LIST);
            }
            else if(  pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL_LIST)
            {
                popChannelListOptionMenu( pMe);
            }        
            else if( pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL)
            {
                hideChannelEditingScreen( pMe);
            }
			//Add By zzg 2010_9_16
			else if (pMe->opMode == FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST)
			{
				if (HS_HEADSET_ON())
			    {
			        fm_mute(FALSE);
			    }
				ISHELL_CancelTimer( pMe->m_pShell, refreshChannelListCB, (void*)pMe);
				pMe->globalSearching = FALSE;
		        stopDrawRefreshListPrompt( pMe);
		        {
		            sChanInfo *sChannelNode = NULL;
		            sChannelNode = FmRadio_ChanList_GetByIndex(pMe, FmRadio_GetChannelTotal()-1);
		            if(sChannelNode != NULL)
		            {
		                setChannelTo( pMe, sChannelNode->wChannel);
		            }
		            else
		            {
		                setChannelTo( pMe, 0);
		            }
		        }
		        moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);								
				popOptionMenu( pMe);
			}
			//Add End
       #endif
#if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
			else if( pMe->opMode == FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL)
			{
				goto __handleKeyEvent_input_channel_done__;
			}
#endif
        }
        return TRUE;

#if defined( AEE_SIMULATOR)
        case AVK_SELECT:
#else
	#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
        case AVK_INFO:
    #endif
#endif
        {
            if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
            {
#if defined( FEATURE_FMRADIO_NO_MODE_SELECT)
				pMe->tuneVolumeByLeftRightArrowKey = TRUE;
				ISHELL_SetTimer( pMe->m_pShell, 3000, (PFNNOTIFY)tuneVolumeByLeftRightArrowKeyCloseCb, pMe);
				tuneVolumeByLeftRightArrowKey( pMe);
#elif defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE)
                FMRadioMute( pMe);
#else
				popTuningModeSelectMenu( pMe);
#endif
            }
        #if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
            else if(  pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL_LIST)
            {
#if (defined( FEATURE_FMRADIO_NO_MODE_SELECT) || defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE))
				if( IMENUCTL_GetItemCount( pMe->m_pMenu) > 0)
				{
					pMe->channelListMenuSelectedItemId      = IMENUCTL_GetSel( pMe->m_pMenu);
					pMe->channelListMenuSelectedItemIndex   = getIndexByMenuItemId( pMe, pMe->channelListMenuSelectedItemId);
					 handleCommandEvent( pMe, IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_PLAY);
				}
#else
                if( IMENUCTL_GetItemCount( pMe->m_pMenu) == 0)
                {
                //     pMe->channelListIsEmptyHasPrompted = TRUE;
                     handleCommandEvent( pMe, IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_NEW);
                }
                else
                {
                     pMe->m_channellist_bottom_type = FM_SAVE_DELETE;
                     showChannelEditingScreen( pMe);
                }
#endif
            }
            else if( pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL)
            {
                hideChannelEditingScreen( pMe);
            }
        #endif
#if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
			else if( pMe->opMode == FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL)
			{
__handleKeyEvent_input_channel_done__:
				ITEXTCTL_GetText( pMe->pText, pMe->directInputChannel, sizeof( pMe->directInputChannel));
				if( channelNumberIsvalid( pMe->directInputChannel))
				{
					setChannelTo( pMe, convertChannelValueFromText( pMe->directInputChannel));
				}

				ITEXTCTL_SetActive( pMe->pText, FALSE);
				moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
			}
#endif

            return TRUE;
        }

        case AVK_CLR:
        case AVK_SOFT2:
        {			
            if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
            {
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND             
                pMe->runOnBackground = FALSE;
#endif
                ISHELL_CloseApplet( pMe->m_pShell, FALSE);
            }
            else if( pMe->opMode == FM_RADIO_OPMODE_MODE_SELECTION)
            {
                hideTuningModeSelectMenu( pMe);
            }
			
			//Add By zzg 2010_07_15
			else if ((pMe->opMode == FM_RADIO_OPMODE_OPTION_SELECTION) && hideMenu( pMe))
			{
				moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
			}			
			//Add End
			
			//Add By zzg 2010_9_16
			else if (pMe->opMode == FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST)
			{
				if (HS_HEADSET_ON())
			    {
			        fm_mute(FALSE);
			    }
				ISHELL_CancelTimer( pMe->m_pShell, refreshChannelListCB, (void*)pMe);
				pMe->globalSearching = FALSE;
		        stopDrawRefreshListPrompt( pMe);
		        {
		            sChanInfo *sChannelNode = NULL;
		            sChannelNode = FmRadio_ChanList_GetByIndex(pMe, FmRadio_GetChannelTotal()-1);
		            if(sChannelNode != NULL)
		            {
		                setChannelTo( pMe, sChannelNode->wChannel);
		            }
		            else
		            {
		                setChannelTo( pMe, 0);
		            }
		        }
		        moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
				
			}
			//Add End
#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
            else if( pMe->opMode == FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST_CONFIRM    ||
                     ( pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL_LIST && hideChannelList( pMe)) ||
                     (  pMe->opMode == FM_RADIO_OPMODE_OPTION_SELECTION && hideMenu( pMe))
            )
            {
                moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
            }
            else if( pMe->opMode == FM_RADIO_OPMODE_EDIT_CHANNEL)
            {
                showChannelList( pMe);
            }
            else if( pMe->opMode  == FM_RADIO_OPMODE_EDIT_CHANNEL_LIST_OPTION_SELECTION)
            {
                showChannelList( pMe);
            }
#endif

#if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
			else if( pMe->opMode == FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL)
			{
				ITEXTCTL_SetActive( pMe->pText, FALSE);
				moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
			}
#endif
        }
        return TRUE;

#if (defined( FEATURE_FMRADIO_NO_MODE_SELECT) || defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE))
		case AVK_STAR:
		case AVK_POUND:
		{
			if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
			{
				changeVolume( pMe, key);
			}
		}
		return TRUE;
        
		case AVK_LEFT:
		case AVK_RIGHT:
		{
			if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
			{
				pMe->cfg.tuningMode = FM_RADIO_TUNNING_MODE_MANUAL;
				if( key == AVK_LEFT)
				{
					changeChannelAnticlockwise( pMe);
				}
				else
				{
					changeChannelClockwise( pMe);
				}
			}
		}
		return TRUE;

        case AVK_GSENSOR_FORWARD:
        case AVK_GSENSOR_BACKWARD:
    #if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
		case AVK_UP:
		case AVK_DOWN:
		case AVK_I:
		case AVK_O:
	#endif
		{
			if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
			{
#if defined( FEATURE_FMRADIO_NO_MODE_SELECT) 
				if( pMe->tuneVolumeByLeftRightArrowKey)
				{
					ISHELL_CancelTimer( pMe->m_pShell, (PFNNOTIFY)tuneVolumeByLeftRightArrowKeyCloseCb, pMe);
					changeVolume( pMe, key);
					ISHELL_SetTimer( pMe->m_pShell, 3000, (PFNNOTIFY)tuneVolumeByLeftRightArrowKeyCloseCb, pMe);
				}
				else
#endif                   
				{
					pMe->cfg.tuningMode = FM_RADIO_TUNNING_MODE_AUTO;
#if 0                    
					if(( key == AVK_DOWN) || ( key == AVK_GSENSOR_FORWARD))
					{
						changeChannelAnticlockwise( pMe);
					}
					else
					{
						changeChannelClockwise( pMe);
					}
#endif
                    if(( key == AVK_DOWN) || ( key == AVK_UP)|| ( key == AVK_I)|| ( key == AVK_O))
                    {
                        if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
                        {
                            changeVolume( pMe, key);
                        }                    
                    }
				}
			}
		}
		return TRUE;
#else
        case AVK_UP:
        case AVK_DOWN:
		case AVK_I:
		case AVK_O:
        {
            if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
            {
                changeVolume( pMe, key);
            }
        }
        return TRUE;

        case AVK_GSENSOR_FORWARD:
        case AVK_LEFT:
        {
            if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
            {
                changeChannelAnticlockwise( pMe);
            }
        }
        return TRUE;

        case AVK_GSENSOR_BACKWARD:
        case AVK_RIGHT:
        {
            if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
            {
                changeChannelClockwise( pMe);
            }
        }
        return TRUE;
#endif
    }

    return FALSE;
}

static boolean handleCommandEvent( CFmRadio *pMe, uint16 itemId)
{
    pMe->CurrentItemId = itemId;
    if(  itemId < 3)
    {
		pMe->cfg.tuningMode = IMENUCTL_GetSel( pMe->m_pMenu);
        hideTuningModeSelectMenu( pMe);
    }
    else if( itemId == IDS_FMRADIO_OPTION_MENU_QUIT
    #if FEATURE_FMRADIO_SUPPORT_BACKGROUND
    		|| itemId == IDS_FMRADIO_OPTION_MENU_PLAY_ON_BACKGROUND
    #endif
    )
    {
        hideMenu( pMe);
        moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND          
        pMe->runOnBackground = (itemId == IDS_FMRADIO_OPTION_MENU_QUIT) ? FALSE : TRUE;
#endif
        ISHELL_CloseApplet( pMe->m_pShell, (itemId == IDS_FMRADIO_OPTION_MENU_QUIT) ? FALSE : TRUE);
    }
#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
    else if( itemId == IDS_FMRADIO_OPTION_MENU_GLOBAL_SEARCH)
    {
        hideMenu( pMe);
        moveOperationModeTo( pMe, FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST_CONFIRM);
    }

    else if( itemId == IDS_FMRADIO_OPTION_MENU_LIST)
    {
    //    pMe->channelListIsEmptyHasPrompted = FALSE;
        showChannelList( pMe);
    }
    else if( itemId == IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_DELETE)
    {
        pMe->opMode = FM_RADIO_OPMODE_CHANNEL_LIST_DELETE;
        FmRadio_ShowMsgBoxDialog( pMe, 
                            FMRADIOLS_RES_FILE_LANG,
                            IDS_FMRADIO_DELETE,
                            0,                                      
                            NULL,
                            -1);
    }
	else if( itemId == IDS_FMRADIO_DELETE_ALL)
	{
        pMe->opMode = FM_RADIO_OPMODE_CHANNEL_LIST_DELETE_ALL;
        FmRadio_ShowMsgBoxDialog( pMe, 
                            FMRADIOLS_RES_FILE_LANG,
                            IDS_FMRADIO_DELETE_ALL,
                            0,                                      
                            NULL,
                            -1);
	}
    else if( itemId == IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_NEW)
    {
        AECHAR text[] = {0};
        uint16 menuId = CHANNEL_LIST_MENU_ITEM_ID_FIRST + pMe->byChannelMax;
        uint16 oldId  = pMe->channelListMenuSelectedItemId;

        pMe->m_channellist_bottom_type = FM_SAVE_BACK;
        showChannelList( pMe);
        IMENUCTL_AddItem( pMe->m_pMenu, NULL, 0, menuId, text, NULL);
        IMENUCTL_SetSel( pMe->m_pMenu, menuId);
        showChannelEditingScreen( pMe);
        pMe->channelListMenuSelectedItemId = oldId; 
    }
    else if( itemId == IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_PLAY)
    {
        if (IMENUCTL_GetItemCount( pMe->m_pMenu) > 0)
        {
			sChanInfo* node = FmRadio_ChanList_GetByIndex( pMe, pMe->channelListMenuSelectedItemIndex);
			if( node)
			{
				hideMenu( pMe);
				moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
				setChannelTo( pMe, node->wChannel);
            }
            else
            {
                 //pMe->channelListIsEmptyHasPrompted = FALSE;
                 showChannelList( pMe);
             }
        }
        else
        {
             //pMe->channelListIsEmptyHasPrompted = FALSE;
             showChannelList( pMe);
        }
    }

	else if( itemId == IDS_EDIT)
	{
		 pMe->m_channellist_bottom_type = FM_SAVE_DELETE;
		 showChannelList( pMe);
		 showChannelEditingScreen( pMe);
	}
	else if( itemId == IDS_SAVE)
	{
		if( !FmRadio_FindChanListNode( pMe->cfg.channel))
		{
			sChanInfo info = { 0};
			info.wChannel = pMe->cfg.channel;
			FmRadio_AddChanListNode( &info);
			FmRadio_SaveChannelList( pMe);
		}
		hideMenu( pMe);
		moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
	}
#endif
    else
    {
        return FALSE;
    }

    return TRUE;
}

static void moveOperationModeTo( CFmRadio *pMe, OpModeTypeEnum newMode)
{

    pMe->opMode = newMode;
    repaint( pMe, TRUE);
}

static void changeChannelAnticlockwise( CFmRadio *pMe)
{
    
    changeChannel( pMe, CHANGE_CHANNEL_ANTICLOCKWISE);
}

static boolean changeChannelClockwise( CFmRadio *pMe)
{
    return changeChannel( pMe, CHANGE_CHANNEL_CLOCKWISE);
}

static boolean changeChannel( CFmRadio *pMe, int direction)
{
    boolean result = TRUE;
#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
    if( pMe->cfg.tuningMode == FM_RADIO_TUNNING_MODE_LIST)
    {
		if( direction == CHANGE_CHANNEL_ANTICLOCKWISE)
		{
			result = FmRadio_ChanList_EnumPrevious_WithLoop( pMe);
		}
		else
		{
			result = FmRadio_ChanList_EnumNext_WithLoop( pMe);
		}

		if( result)
		{
			result = setChannelTo( pMe, FmRadio_ChanList_GetCurrent_WithLoop( pMe)->wChannel);
        }
        else
        {
            pMe->globalSearching = TRUE;
            refreshChannelList( pMe, BEGIN_TO_REFRESH_CHANNEL_LIST);
        }
    }
    else 
#endif
	if( pMe->cfg.tuningMode == FM_RADIO_TUNNING_MODE_MANUAL)
    {
        int increment[] = { -1, 1};
        int channel     = pMe->cfg.channel;

        channel += increment[direction];
        if( channel * CHANNEL_SPACE > UPPEST_BAND_FREQ - LOWEST_BAND_FREQ)
        {
            channel = 0;
        }
        else if( channel < 0)
        {
            channel = ( UPPEST_BAND_FREQ - LOWEST_BAND_FREQ) / CHANNEL_SPACE;
        }
        result = setChannelTo( pMe, channel);

    }
    else if( pMe->cfg.tuningMode == FM_RADIO_TUNNING_MODE_AUTO)
    {
        pMe->opMode                 = FM_RADIO_OPMODE_AUTO_TUNE;
        pMe->seekChannelClockwise   = direction == CHANGE_CHANNEL_CLOCKWISE;
    #if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
        refreshChannelList( pMe, CONTINUE_TO_REFRESH_CHANNEL_LIST);
    #endif
    }
    return result;
}

static boolean setChannelTo( CFmRadio *pMe, uint16 theNewChannel)
{
    boolean result =FALSE;
    pMe->cfg.channel = theNewChannel;

#if !defined( AEE_SIMULATOR)
    if( fm_tune_channel( (LOWEST_BAND_FREQ + CHANNEL_SPACE * pMe->cfg.channel)/100) == FM_RADIO_SUCCESSFUL)
    {
        result = TRUE;
        pMe->ledLightType = FM_RADIO_LED_LIGHT_PLAYING;
    }
    else
    {
        result = FALSE;
        pMe->ledLightType = FM_RADIO_LED_LIGHT_IDLE;
    }
#endif//#if !defined( AEE_SIMULATOR)

    drawOperationPrompt( pMe, IDS_FMRADIO_PROMPT_PLAYING, RGB_WHITE);
    drawChannelIndicator( pMe);
    drawLedLight( pMe);
    return result;
//    IDISPLAY_UpdateEx( pMe->m_pDisplay, TRUE);
}

static void changeVolume( CFmRadio *pMe, uint16 keyCode)
{

    static const int limitValue[] = { 15, 0};
    static const int increment[]  = { 3, -3};
	int theKey;
	if(keyCode == AVK_I || keyCode == AVK_O)
	{
		if(keyCode == AVK_O)
		{
			theKey = 0;
		}
		else
		{
			theKey = 1;
		}
	}
	else
	{
    	theKey       = keyCode - AVK_UP;
	}
#if 0
#if defined( FEATURE_FMRADIO_NO_MODE_SELECT)
	if( pMe->tuneVolumeByLeftRightArrowKey)
	{
		theKey = keyCode == AVK_LEFT ? 1 : 0;
	}
	else
	{
		theKey = keyCode == AVK_STAR ? 1 : 0;
	}
#elif defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE)
	theKey = keyCode == AVK_STAR ? 1 : 0;
#endif
#endif
   // if( pMe->byVolumeLevel != limitValue[theKey])
    if((pMe->byVolumeLevel == 0) && (theKey == 1))
    {
        //��byVolumeLeveΪ0�Ұ����¼�ʱ������byVolumeLevel����
        //���������������ԭ����byVolumeLevel����Ϊunsigned char��
        //����Ϊ0���ټ�ʱ�������Ϊ255
        pMe->byVolumeLevel = 0;
    }
    else
    {
        pMe->byVolumeLevel += increment[theKey];
    }
    if(pMe->byVolumeLevel > 15)
    {
        pMe->byVolumeLevel = 15;
    }
    else if(pMe->byVolumeLevel < 0)
    {
        pMe->byVolumeLevel = 0;
    }
    if(pMe->byVolumeLevel%3 != 0)
    {
        pMe->byVolumeLevel = 0;
    }
#if !defined( AEE_SIMULATOR)
    //Call driver to set Volume
    if (HS_HEADSET_ON())
    {
        fm_set_volume( pMe->byVolumeLevel);
    }
#endif//#if !defined( AEE_SIMULATOR)
   // FmRadio_RefreshVolumeImage(pMe);
    repaint( pMe, FALSE);
    //drawVolumeIndicator( pMe);
    //IDISPLAY_UpdateEx( pMe->m_pDisplay, TRUE);
    (void) ICONFIG_SetItem(pMe->m_pConfig,
                   CFGI_FMRADIO_VOLUME,
                   &pMe->byVolumeLevel,
                   sizeof(byte)
               );
}

static boolean hideMenu( CFmRadio *pMe)
{

    IMENUCTL_SetActive( pMe->m_pMenu, FALSE);
    IMENUCTL_Reset( pMe->m_pMenu);

    return TRUE;
}
#if defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE)
static void FMRadioMute( CFmRadio *pMe)
{
    pMe->key_to_mute = (pMe->key_to_mute ? 0 : 1);
#ifndef WIN32
    //fm_mute(pMe->key_to_mute);		//Del By zzg 2010_07_18
#endif
	repaint( pMe, TRUE);
}

#endif 

static void popTuningModeSelectMenu( CFmRadio *pMe)
{
#ifndef FEATURE_FMRADIO_SIMPLE_VERSION
    int         i   = 0;
    CtlAddItem  ai;
    AEERect     rect;

    ai.pText        = NULL;
    ai.pImage       = NULL;
    ai.pszResImage  = AEE_APPSCOMMONRES_IMAGESFILE;
    ai.pszResText   = FMRADIOLS_RES_FILE_LANG;
    ai.wFont        = AEE_FONT_NORMAL;
    ai.dwData       = 0;

    for( i = 0; i < FM_RADIO_TUNNING_MODE_NUMBER; i ++)
    {
        ai.wText    = IDS_FMRADIO_TUNING_MODE_PROMPT_LIST + i;
        ai.wImage   = i == pMe->cfg.tuningMode ? IDB_RADIO_FILLED : IDB_RADIO_UNFILLED;
        ai.wItemID  = i;

        IMENUCTL_AddItemEx( pMe->m_pMenu, &ai);
    }

    drawImage( pMe, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BGMASK, 0, 0);
#if defined(FEATURE_DISP_128X128)
	SETAEERECT( &rect, (128 >> 3) + 6, (128 >> 3) + 6, 128 * 3 >> 2, 128 * 3 >> 2);
    IDISPLAY_FillRect( pMe->m_pDisplay, &rect, 0);

    SETAEERECT( &rect, 128 >> 3, 128 >> 3, 128 * 3 >> 2, 128 * 3 >> 2);
    IMENUCTL_SetRect( pMe->m_pMenu, &rect);
#elif defined(FEATURE_DISP_160X128)
		SETAEERECT( &rect, (128 >> 3) + 6, (128 >> 3) + 6, 128 * 3 >> 2, 128 * 3 >> 2);
		IDISPLAY_FillRect( pMe->m_pDisplay, &rect, 0);
	
		SETAEERECT( &rect, 128 >> 3, 128 >> 3, 128 * 3 >> 2, 128 * 3 >> 2);
		IMENUCTL_SetRect( pMe->m_pMenu, &rect);
#else
    SETAEERECT( &rect, (128 >> 3) + 6, (128 >> 3) + 6, 128 * 3 >> 2, 128 * 3 >> 2);
    IDISPLAY_FillRect( pMe->m_pDisplay, &rect, 0);

    SETAEERECT( &rect, 128 >> 3, 128 >> 3, 128 * 3 >> 2, 128 * 3 >> 2);
    IMENUCTL_SetRect( pMe->m_pMenu, &rect);
#endif
//    IMENUCTL_SetProperties( pMe->m_pMenu, IMENUCTL_GetProperties( pMe->m_pMenu) | MP_BIND_ITEM_TO_NUMBER_KEY);
	IMENUCTL_SetOemProperties(pMe->m_pMenu, IMENUCTL_GetOemProperties( pMe->m_pMenu)&~OEMMP_DISTINGUISH_INFOKEY_SELECTKEY);
    IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_SELECT_BACK);
	#if 0
    IMENUCTL_SetTitle( pMe->m_pMenu, FMRADIOLS_RES_FILE_LANG, IDS_FMRADIO_SOFTKEY_MODE, NULL);
	#else
    {
  		AECHAR WTitle[40] = {0};
		(void)ISHELL_LoadResString(pMe->m_pShell,
                FMRADIOLS_RES_FILE_LANG,                                
                IDS_FMRADIO_SOFTKEY_MODE,
                WTitle,
                sizeof(WTitle));
		IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
    }
	#endif
    IMENUCTL_SetSel( pMe->m_pMenu, pMe->cfg.tuningMode);

	repaint( pMe, TRUE);
    IMENUCTL_SetActive( pMe->m_pMenu, TRUE);

    pMe->opMode = FM_RADIO_OPMODE_MODE_SELECTION;
#endif
}

static void hideTuningModeSelectMenu( CFmRadio *pMe)
{

    hideMenu( pMe);
    moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
}

static void setOptionMenuProperties( CFmRadio* pMe)
{

    IMENUCTL_SetProperties( pMe->m_pMenu, IMENUCTL_GetProperties( pMe->m_pMenu) | MP_BIND_ITEM_TO_NUMBER_KEY);
	IMENUCTL_SetOemProperties(pMe->m_pMenu, IMENUCTL_GetOemProperties( pMe->m_pMenu)&~OEMMP_DISTINGUISH_INFOKEY_SELECTKEY);
    IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_SELECT_BACK);
    IMENUCTL_SetPopMenuRect( pMe->m_pMenu);
    IMENUCTL_SetActive( pMe->m_pMenu, TRUE);
}

static void popOptionMenu( CFmRadio *pMe)
{

	int i = 0;
	char*   resFile[] = {
	#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
							// FMRADIOLS_RES_FILE_LANG,
						  FMRADIOLS_RES_FILE_LANG,
						  FMRADIOLS_RES_FILE_LANG,
						  AEE_APPSCOMMONRES_LANGFILE,
	#endif
						  FMRADIOLS_RES_FILE_LANG
					};
					
	uint16  resId[] = { 
	#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
						//IDS_FMRADIO_OPTION_MENU_QUIT,
						IDS_FMRADIO_OPTION_MENU_GLOBAL_SEARCH,
						IDS_FMRADIO_OPTION_MENU_LIST,
						IDS_SAVE,
	#endif
	#if FEATURE_FMRADIO_SUPPORT_BACKGROUND
						IDS_FMRADIO_OPTION_MENU_PLAY_ON_BACKGROUND
	#endif
						};

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND
	for( i = 0; i < 4; i ++)
#else
	for( i = 0; i < 3; i ++)
#endif
	{

#if defined( FEATURE_CARRIER_THAILAND_HUTCH)
		if( i == 1)
		{
			continue;
		}
#endif

		IMENUCTL_AddItem( pMe->m_pMenu, resFile[i], resId[i], resId[i], 0, 0);
	}
#else
	IMENUCTL_AddItem( pMe->m_pMenu, resFile[0], resId[0], resId[0], 0, 0);
#endif
	repaint( pMe, TRUE);
    pMe->opMode = FM_RADIO_OPMODE_OPTION_SELECTION;
    setOptionMenuProperties( pMe);
}

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT

static void popChannelListOptionMenu( CFmRadio *pMe)
{

    int     i       = 0;
#if (defined( FEATURE_FMRADIO_NO_MODE_SELECT) || defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE))
	char*   resFile[] = { FMRADIOLS_RES_FILE_LANG,
						  FMRADIOLS_RES_FILE_LANG,
						  AEE_APPSCOMMONRES_LANGFILE,
						  FMRADIOLS_RES_FILE_LANG,
						  FMRADIOLS_RES_FILE_LANG
					};
	uint16  resId[] = { IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_PLAY,
						IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_NEW,
						IDS_EDIT,
						IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_DELETE,
						IDS_FMRADIO_DELETE_ALL
					};
#else
	uint16  resId[] = { IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_NEW,
						IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_DELETE,
						IDS_FMRADIO_CHANNEL_LIST_OPTION_MENU_PLAY
					};
#endif

    hideChannelList( pMe);

#if (defined( FEATURE_FMRADIO_NO_MODE_SELECT) || defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE))
    if(TRUE == pMe->channelListIsEmpty)
    {
        IMENUCTL_AddItem( pMe->m_pMenu, resFile[1], resId[1], resId[1], 0, 0); 
    }
    else
    {
    	for( i = 0; i < 5; i ++)
    	{
    		if( ( i == 1 && pMe->byChannelMax >= MAX_FMRADIO_STORED_CHANNEL) ||
    			( i == 2 && pMe->byChannelMax == 0)
    		)
    		{
    			continue;
    		}
    		IMENUCTL_AddItem( pMe->m_pMenu, resFile[i], resId[i], resId[i], 0, 0);
    	}    
    }
#else
	if( pMe->byChannelMax < MAX_FMRADIO_STORED_CHANNEL)
	{
		IMENUCTL_AddItem( pMe->m_pMenu, FMRADIOLS_RES_FILE_LANG, resId[0], resId[0], 0, 0);
	}
	for( i = 1; i < 3; i ++)
    {
        IMENUCTL_AddItem( pMe->m_pMenu, FMRADIOLS_RES_FILE_LANG, resId[i], resId[i], 0, 0);
    }
#endif

    pMe->opMode = FM_RADIO_OPMODE_EDIT_CHANNEL_LIST_OPTION_SELECTION;
    setOptionMenuProperties( pMe);
}

static void refreshChannelList( CFmRadio *pMe, boolean begin)
{
    if (HS_HEADSET_ON())
    {
        fm_mute(TRUE);//add by xuhui
    }
    pMe->ledLightType = FM_RADIO_LED_LIGHT_SEARCHING;
    if( begin == BEGIN_TO_REFRESH_CHANNEL_LIST)
    {
        pMe->seekChannelClockwise = TRUE;
        moveOperationModeTo( pMe, FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST);
#if !defined( AEE_SIMULATOR)
        fm_tune_channel((LOWEST_BAND_FREQ)/100);
#endif//#if !defined( AEE_SIMULATOR)

        if( pMe->globalSearching)
        {
			FmRadio_ClearChanList( pMe);
        }
    }

    if( !pMe->drawRefreshListPrompt)
    {
        pMe->drawRefreshListPrompt = TRUE;
        pMe->demitintColor = 255;
        drawLedLight( pMe);
        drawRefreshListPrompt( pMe);
    }
	 pMe->cfg.channel=0;
#if !defined( AEE_SIMULATOR)
    fm_seek_start( pMe->seekChannelClockwise, FALSE);
#endif//#if !defined( AEE_SIMULATOR)

    ISHELL_SetTimer( pMe->m_pShell, 200, refreshChannelListCB, (void*)pMe);
}
#if 1
static void refreshChannelListCB( void *pme)
{
    CFmRadio *pMe = pme;

#if defined( AEE_SIMULATOR)

    pMe->cfg.channel ++;

	if( pMe->cfg.channel < FM_MAX_CHANNEL)
    {
        ISHELL_SetTimer(pMe->m_pShell, 200, refreshChannelListCB, (void*)pMe);
        drawChannelIndicator( pMe);

        {
            sChanInfo   sChannelNode = {0};
            sChannelNode.wChannel = pMe->cfg.channel;
            FmRadio_AddChanListNode( &sChannelNode);
        }

    }
    else
    {
        moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
    }

#else

    boolean  ready                  = FALSE;
    boolean  reachBandLimit         = FALSE;
    static   int numberOfloop       = 0;
    static   int countSearchChanne  = 0;
    fm_get_seek_status( &ready, &reachBandLimit, NULL);
    pMe->cfg.tuningMode = FM_RADIO_TUNNING_MODE_MANUAL;
    if(((pMe->cfg.channel * CHANNEL_SPACE + LOWEST_BAND_FREQ) >= LOWEST_BAND_FREQ) && 
        ((pMe->cfg.channel * CHANNEL_SPACE + LOWEST_BAND_FREQ) <= UPPEST_BAND_FREQ) &&
        (countSearchChanne  < (UPPEST_BAND_FREQ - LOWEST_BAND_FREQ)/CHANNEL_SPACE) )
    {
        countSearchChanne++;
        ready = TRUE;
    }
    else
    {
        countSearchChanne = 0;
        ready = FALSE;        
    }
    if(ready)
    {
        if( changeChannelClockwise(pme))
        {
            if( pMe->globalSearching)
            {
                sChanInfo sChannelNode = {0};
                sChannelNode.wChannel = pMe->cfg.channel;
				if( FmRadio_AddChanListNode( &sChannelNode))
				{
					drawChannelIndicator( pMe);
					//refreshChannelList( pMe, CONTINUE_TO_REFRESH_CHANNEL_LIST);
				}
				else // reach number limit
				{
					goto __refreshChannelListCB_complete;
				}
			}
			else
			{
                goto __refreshChannelListCB_no_channel_found;
            }
        }
       // else
        {
            //If Seek don't finish, check status after 200 ms
            ISHELL_SetTimer( pMe->m_pShell, 200, refreshChannelListCB, (void*)pMe);
        }
    }
    else
    {
        if (HS_HEADSET_ON())
        {
            fm_mute(FALSE);//add by xuhui
        }
        //OK, Search Band Completed, tune back to pCurChanNode
        if( pMe->globalSearching)
        {
__refreshChannelListCB_complete:
            //pMe->cfg.channel = 0;
            FmRadio_ChanList_EnumInit( pMe);
            if( FmRadio_ChanList_EnumNext( pMe))
            {
                //pMe->cfg.channel = FmRadio_ChanList_GetCurrent( pMe)->wChannel;
            }
            FmRadio_ChanList_EnumInit_WithLoop( pMe);
            goto __refreshChannelListCB_no_channel_found;
        }
        else
        {
            numberOfloop ++;
            if( numberOfloop > 2)
            {
                numberOfloop = 0;
                goto __refreshChannelListCB_no_channel_found;
            }
            refreshChannelList( pMe, CONTINUE_TO_REFRESH_CHANNEL_LIST);
        }


__refreshChannelListCB_no_channel_found:
        pMe->globalSearching = FALSE;
        stopDrawRefreshListPrompt( pMe);
        {
            sChanInfo *sChannelNode = NULL;
            sChannelNode = FmRadio_ChanList_GetByIndex(pMe, FmRadio_GetChannelTotal()-1);
            if(sChannelNode != NULL)
            {
                setChannelTo( pMe, sChannelNode->wChannel);
            }
            else
            {
                setChannelTo( pMe, 0);
            }
        }
        moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
    }
#endif//#if !defined( AEE_SIMULATOR)
}

#else
static void refreshChannelListCB( void *pme)
{

    CFmRadio *pMe = pme;
    
#if defined( AEE_SIMULATOR)

    pMe->cfg.channel ++;

	if( pMe->cfg.channel < FM_MAX_CHANNEL)
    {
        ISHELL_SetTimer(pMe->m_pShell, 200, refreshChannelListCB, (void*)pMe);
        drawChannelIndicator( pMe);

        {
            sChanInfo   sChannelNode = {0};
            sChannelNode.wChannel = pMe->cfg.channel;
            FmRadio_AddChanListNode( &sChannelNode);
        }

    }
    else
    {
        moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
    }

#else

    boolean  ready                  = FALSE;
    boolean  reachBandLimit         = FALSE;
    word     wChannel               = 0;
    static   int numberOfloop       = 0;
    MSG_FATAL("refreshChannelListCB Start",0,0,0);
    if( fm_get_seek_status( &ready, &reachBandLimit, &wChannel) == FM_RADIO_SUCCESSFUL)
    {

        if( ready)
        {

            if( reachBandLimit)
            {
                //OK, Search Band Completed, tune back to pCurChanNode
                if( pMe->globalSearching)
				{
__refreshChannelListCB_complete:
                {

						pMe->cfg.channel = 0;
						FmRadio_ChanList_EnumInit( pMe);
						if( FmRadio_ChanList_EnumNext( pMe))
						{
							pMe->cfg.channel = FmRadio_ChanList_GetCurrent( pMe)->wChannel;
						}
                    FmRadio_ChanList_EnumInit_WithLoop( pMe);
                    goto __refreshChannelListCB_no_channel_found;
					}
                }
                else
                {
                    numberOfloop ++;
                    if( numberOfloop > 2)
                    {
                        numberOfloop = 0;
                        goto __refreshChannelListCB_no_channel_found;
                    }
                    refreshChannelList( pMe, CONTINUE_TO_REFRESH_CHANNEL_LIST);
                }
            }
            else
            {

                if( pMe->globalSearching)
                {
                    sChanInfo sChannelNode = {0};

                    sChannelNode.wChannel = wChannel;

					if( FmRadio_AddChanListNode( &sChannelNode))
					{
						pMe->cfg.channel = wChannel;
						drawChannelIndicator( pMe);

						refreshChannelList( pMe, CONTINUE_TO_REFRESH_CHANNEL_LIST);
					}
					else // reach number limit
					{
						goto __refreshChannelListCB_complete;
					}
				}
				else
				{
				    if( FmRadio_IsChannelValid( wChannel))
				    {
                        pMe->cfg.channel = wChannel;
				    }
                    goto __refreshChannelListCB_no_channel_found;
                }
            }
        }
        else
        {
            //If Seek don't finish, check status after 200 ms
            ISHELL_SetTimer( pMe->m_pShell, 200, refreshChannelListCB, (void*)pMe);
        }
    }
    else
    {

__refreshChannelListCB_no_channel_found:
        pMe->globalSearching = FALSE;
        stopDrawRefreshListPrompt( pMe);
        setChannelTo( pMe, pMe->cfg.channel);
        moveOperationModeTo( pMe, FM_RADIO_OPMODE_PLAY);
    }
    MSG_FATAL("refreshChannelListCB End",0,0,0);
#endif//#if !defined( AEE_SIMULATOR)
}
#endif

static void showChannelList( void* pme)
{

    CFmRadio *pMe = pme;

	AEERect         rect  = pMe->m_rc;
    AECHAR          text[32];
    uint16          menuItemId = CHANNEL_LIST_MENU_ITEM_ID_FIRST;

    hideMenu( pMe);

    FmRadio_ChanList_EnumInit( pMe);
	while( FmRadio_ChanList_EnumNext( pMe))
    {
		convertChannelValueToText( FmRadio_ChanList_GetCurrent( pMe)->wChannel, text, 64);
        IMENUCTL_AddItem( pMe->m_pMenu, NULL, 0, menuItemId ++, text, NULL);
    }

    IMENUCTL_SetRect( pMe->m_pMenu, &rect);
	#if 0
    IMENUCTL_SetTitle( pMe->m_pMenu, FMRADIOLS_RES_FILE_LANG, IDS_FMRADIO_OPTION_MENU_LIST, NULL);
	#else
    {
  		AECHAR WTitle[40] = {0};
		(void)ISHELL_LoadResString(pMe->m_pShell,
                FMRADIOLS_RES_FILE_LANG,                                
                IDS_FMRADIO_OPTION_MENU_LIST,
                WTitle,
                sizeof(WTitle));
		IANNUNCIATOR_SetFieldText(pMe->m_pIAnn,WTitle);
    }
	#endif
    IMENUCTL_SetProperties( pMe->m_pMenu, MP_WRAPSCROLL);
#if (defined( FEATURE_FMRADIO_NO_MODE_SELECT) || defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE))
	IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_OPTION_BACK);
#else
	IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_OPTION_EDIT_BACK);
#endif
    IMENUCTL_SetOemProperties(pMe->m_pMenu, OEMMP_DISTINGUISH_INFOKEY_SELECTKEY);
    IMENUCTL_SetActive( pMe->m_pMenu, TRUE);
    IMENUCTL_SetSel( pMe->m_pMenu, pMe->channelListMenuSelectedItemId);

    if( IMENUCTL_GetItemCount( pMe->m_pMenu) == 0) 
        //&& !pMe->channelListIsEmptyHasPrompted)
    {
        {
            AECHAR wstrText[16] = {0};
            RGBVAL fontColor;
            
            (void)ISHELL_LoadResString(pMe->m_pShell,
                          FMRADIOLS_RES_FILE_LANG,
                          IDS_FMRADIO_CHANNEL_LIST_IS_EMPTY,
                          wstrText,
                          sizeof(wstrText));
            fontColor = IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, RGB_WHITE);
            (void)IDISPLAY_DrawText(pMe->m_pDisplay,
                          AEE_FONT_NORMAL,
                          wstrText, 
                          -1, 
                          6,
                          6 + TITLEBAR_HEIGHT,
                          NULL,
                          IDF_TEXT_TRANSPARENT);
            IDISPLAY_SetColor(pMe->m_pDisplay, CLR_USER_TEXT, fontColor);

        }
        pMe->channelListIsEmpty = TRUE;
        pMe->opMode = FM_RADIO_OPMODE_EDIT_CHANNEL_LIST;
    }
    else
    {
        pMe->channelListIsEmpty = FALSE;
        pMe->opMode = FM_RADIO_OPMODE_EDIT_CHANNEL_LIST;
    }
}

static boolean hideChannelList( CFmRadio *pMe)
{

    pMe->channelListMenuSelectedItemId      = IMENUCTL_GetSel( pMe->m_pMenu);
    pMe->channelListMenuSelectedItemIndex   = getIndexByMenuItemId( pMe, pMe->channelListMenuSelectedItemId);
    hideMenu( pMe);
    return TRUE;
}
#endif

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT

static void showChannelEditingScreen( CFmRadio *pMe)
{

    CtlAddItem  menuItem;
    AEERect     rect;

    if( IMENUCTL_GetItem( pMe->m_pMenu, IMENUCTL_GetSel( pMe->m_pMenu), &menuItem))
    {
        if(1 == pMe->edit_chann_interrupt)
        {
            menuItem.pText = pMe->EditChannel;
            pMe->edit_chann_interrupt = 0;
        }
        ITEXTCTL_SetText( pMe->pText, menuItem.pText, WSTRLEN( menuItem.pText) - 4);
        ITEXTCTL_GetText( pMe->pText, pMe->originalChannelNumber, 16);
    }

    IMENUCTL_GetSelItemRect( pMe->m_pMenu, &rect);
    rect.x  = 0;
    rect.dx = pMe->m_rc.dx;
    ITEXTCTL_SetRect( pMe->pText, &rect);

#if defined FEATURE_FM_SPACE_50KHZ
    ITEXTCTL_SetMaxSize(pMe->pText, 6/*bw:5*/);
#else
	#if defined(FEATURE_DISP_128X128)
		ITEXTCTL_SetMaxSize(pMe->pText, 4);
	#elif defined(FEATURE_DISP_128X128)
		ITEXTCTL_SetMaxSize(pMe->pText, 4);
	#else
    	ITEXTCTL_SetMaxSize(pMe->pText, 5);
    #endif
#endif
    ITEXTCTL_SetInputMode(pMe->pText, AEE_TM_NUMBERS);

    if (pMe->m_channellist_bottom_type == FM_SAVE_BACK)
    {
         IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_SAVE_BACK);
    }
    else if (pMe->m_channellist_bottom_type == FM_SAVE_DELETE)
    {
        IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_SAVE_DELETE);
    }
    else
    {
        IMENUCTL_SetBottomBarType( pMe->m_pMenu, BTBAR_OPTION_EDIT_BACK);
        IMENUCTL_SetOemProperties(pMe->m_pMenu, OEMMP_DISTINGUISH_INFOKEY_SELECTKEY);
    }

    hideChannelList( pMe);
    IDISPLAY_SetColor( pMe->m_pDisplay, CLR_USER_TEXT, RGB_BLACK);
    ITEXTCTL_SetActive( pMe->pText, TRUE);

    pMe->opMode = FM_RADIO_OPMODE_EDIT_CHANNEL;
}
#endif
static boolean isDigital( AECHAR theChar)
{

    return ( theChar >= '0' && theChar <= '9');
}
 
static boolean channelNumberIsvalid( AECHAR *text)
{

    if( text == NULL)
    {
        return FALSE;
    }
    else
    {
        int length      = WSTRLEN( text);
        int i           = 0;
        int dotNumber   = 0;
        int lastChar    = length - 1;

        if( length          == 0    ||
            text[0]         == '.'  ||
            text[1]         == '.'  ||
            text[lastChar]  == '.'
        )
        {
            return FALSE;
        }

        for( i = 0; i < length; i ++)
        {

            if( !isDigital( text[i]) && text[i] != '.')
            {
                return FALSE;
            }

            if( text[i] == '.')
            {
                dotNumber ++;
            }
        }

        if( dotNumber > 1)
        {
            return FALSE;
        }
        else
        {
            int channel = convertChannelValueFromText( text);
            return channel >= 0 &&
               channel <= ( ( UPPEST_BAND_FREQ - LOWEST_BAND_FREQ) / CHANNEL_SPACE);
        }
    }
}

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT

static void hideChannelEditingScreen( CFmRadio *pMe)
{

    AECHAR text[16]         = {0};

    ITEXTCTL_GetText( pMe->pText, text, 16);
#if defined FEATURE_FM_SPACE_50KHZ 
    if( WSTRLEN( text) <= 6 &&
        channelNumberIsvalid( text)
#else
    if( WSTRLEN( text) <= 5 &&
        channelNumberIsvalid( text)
#endif //#if defined FEATURE_FM_SPACE_50KHZ
    )
    {
        int            channel = convertChannelValueFromText( text);
        /*if( FmRadio_FindChanListNodeByIndex( pMe->channelListMenuSelectedItemIndex, &pnode) == TRUE)
        {
            pnode->chanInfo.wChannel = channel;
            FmRadio_SaveChannelList( (void*)pMe);
        }
        else
        {
            sChanInfo info;
            info.wChannel = channel;
            FmRadio_AddChanListNode( &info);
        }*/

        if( WSTRCMP( text, pMe->originalChannelNumber) == 0)
        {
            ITEXTCTL_SetActive( pMe->pText, FALSE);
            showChannelList( pMe);
        }
        else
        {
            if( FmRadio_FindChanListNode(channel))
            {
				ITEXTCTL_SetActive( pMe->pText, FALSE);
                //prompt
                drawPrompt( pMe,
                    IDS_FMRADIO_CHANNEL_NODE_HAS_EXISTED,
                    BTBAR_NONE,
                    TRUE
                );

                ISHELL_SetTimer( pMe->m_pShell, 1200, showChannelList, (void*)pMe);
            }
            else
			{

				sChanInfo* pnode = FmRadio_ChanList_GetByIndex( pMe, pMe->channelListMenuSelectedItemIndex);
				if( pnode)
				{
					if( WSTRCMP( text, pMe->originalChannelNumber) != 0)
					{
						pnode->wChannel = channel;
					}
				}
				else if (FmRadio_FindChanListNode(channel) == FALSE)
				{
					sChanInfo info = { 0};
                    info.wChannel = channel;
                    FmRadio_AddChanListNode( &info);
                }

                ITEXTCTL_SetActive( pMe->pText, FALSE);
                pMe->channelListMenuSelectedItemId = CHANNEL_LIST_MENU_ITEM_ID_FIRST + pMe->byChannelMax - 1;
                showChannelList( pMe);
            }
        }


    }
    else
    {
           ITEXTCTL_SetActive( pMe->pText, FALSE);
           drawPrompt( pMe,
                       IDS_FMRADIO_CHANNEL_NODE_IS_INVALID,
                       BTBAR_NONE,
                       TRUE
                       );

           ISHELL_SetTimer( pMe->m_pShell, 1200, showChannelList, (void*)pMe);
    }

}
#endif

static void paint( CFmRadio *pMe)
{
#if FEATURE_FMRADIO_SUPPORT_BACKGROUND  
    if( pMe->runOnBackground)
    {
        return;
    }
#endif
    drawBg( pMe);
    FmRadio_RefreshVolumeImage(pMe);
    //drawLedLight( pMe);
    drawChannelIndicator( pMe);
    //drawVolumeIndicator( pMe);
	if( pMe->refuseReason == FM_RADIO_REFUSE_REASON_NOT_REFUSE)	//Add By zzg 2010_07_14
    {
    	drawSoftkey( pMe);
	}
	
    drawOperationPrompt( pMe, IDS_FMRADIO_PROMPT_PLAYING, RGB_WHITE);

//#ifndef FEATURE_FMRADIO_SIMPLE_VERSION
#if 0
#if defined( FEATURE_FMRADIO_NO_MODE_SELECT)
	#if defined(FEATURE_DISP_128X128)
		if( pMe->tuneVolumeByLeftRightArrowKey)
		{
			static int x = 0;

			if( (x % 2) == 0)
			{
				drawImage( pMe, FMRADIOLN_RES_FILE, IDI_VOLUME1, 49, 80);
			}
			x++;
		}
	#elif defined(FEATURE_DISP_160X128)
		if( pMe->tuneVolumeByLeftRightArrowKey)
		{
			static int x = 0;

			if( (x % 2) == 0)
			{
				drawImage( pMe, FMRADIOLN_RES_FILE, IDI_VOLUME1, 49, 80);
			}
			x++;
		}
	#else
		if( pMe->tuneVolumeByLeftRightArrowKey)
		{
			static int x = 0;

			if( (x % 2) == 0)
			{
				drawImage( pMe, FMRADIOLN_RES_FILE, IDI_VOLUME1, 49, 113);
			}
			x++;
		}
	#endif
#endif
#if defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE)
#ifndef FEATURE_FMRADIO_SIMPLE_VERSION// draw cursor
	#if defined(FEATURE_DISP_128X128)
		if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
		{
			if(pMe->key_to_mute)
			{
		    	drawImage( pMe, FMRADIOLN_RES_FILE, IDI_MUTE, 60, 80); 
			}
			else
			{
			    drawImage( pMe, FMRADIOLN_RES_FILE, IDI_UNMUTE, 60, 80);     
			} 
		}
	#elif defined(FEATURE_DISP_160X128)
		if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
		{
			if(pMe->key_to_mute)
			{
		    	drawImage( pMe, FMRADIOLN_RES_FILE, IDI_MUTE, 60, 80); 
			}
			else
			{
			    drawImage( pMe, FMRADIOLN_RES_FILE, IDI_UNMUTE, 60, 80);     
			} 
		}
	#else
		if( pMe->opMode == FM_RADIO_OPMODE_PLAY)
		{
			if(pMe->key_to_mute)
			{
		    	drawImage( pMe, FMRADIOLN_RES_FILE, IDI_MUTE, 81, 146); 
			}
			else
			{
			    drawImage( pMe, FMRADIOLN_RES_FILE, IDI_UNMUTE, 81, 146);     
			} 
		}
	#endif
#endif
#endif
#endif

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT

    if( pMe->opMode == FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST_CONFIRM)
    {
#ifdef FEATURE_CARRIER_THAILAND_HUTCH          
        drawPrompt( pMe,
                    IDS_FMRADIO_REFRESH_CHANNEL_LIST_CONFIRM,
                    BTBAR_OK_BACK,
                    FALSE
                );
#else
        drawPrompt( pMe,
                    IDS_FMRADIO_REFRESH_CHANNEL_LIST_CONFIRM,
                    BTBAR_OK_CANCEL,
                    FALSE
                );
#endif //#if defined FEATURE_CARRIER_THAILAND_HUTCH  
    }
#endif
    if( pMe->refuseReason != FM_RADIO_REFUSE_REASON_NOT_REFUSE)
    {

        int16   resId[] = { IDS_FMRADIO_REFUSE_VOICECALL_IS_CONNECTED,
							IDS_FMRADIO_REFUSE_NO_HEADSET,
							IDS_FMRADIO_REFUSE_HEADSET_PLUG_OUT
                        };

        drawImage( pMe, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BGMASK, 0, 0);
        drawText( pMe,
                FMRADIOLS_RES_FILE_LANG,
                resId[pMe->refuseReason],
                &pMe->m_rc,
                0xff00,
                AEE_FONT_BOLD,
                IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE
            );
        drawText( pMe,
                FMRADIOLS_RES_FILE_LANG,
                IDS_FMRADIO_OPTION_MENU_QUIT,
                &pMe->m_rc,
                RGB_WHITE,
                AEE_FONT_BOLD,
                IDF_TEXT_TRANSPARENT | IDF_ALIGN_LEFT | IDF_ALIGN_BOTTOM
            );
        drawText( pMe,
                APPSCOMMON_RES_LANG_FILE,
                IDS_BACK,
                &pMe->m_rc,
                RGB_WHITE,
                AEE_FONT_BOLD,
                IDF_TEXT_TRANSPARENT | IDF_ALIGN_RIGHT | IDF_ALIGN_BOTTOM
            );
    }

    #if FEATURE_DIRECT_INPUT_CHANNEL_NUMBER
    if( pMe->opMode == FM_RADIO_OPMODE_DIRECT_INPUT_CHANNEL)
    {

        drawImage( pMe, AEE_APPSCOMMONRES_IMAGESFILE, IDB_BGMASK, 0, 0);
        ITEXTCTL_Redraw( pMe->pText);
        {
            AEERect     rect        = {0};
            int         fontHeight  = IDISPLAY_GetFontMetrics( pMe->m_pDisplay, AEE_FONT_BOLD, 0, 0);
        #if defined(FEATURE_DISP_128X128)
            int         width       = 46;
		
            SETAEERECT( &rect, ( 128 - width) / 2, (( 128 - fontHeight) / 2) - fontHeight, width, fontHeight);
            drawText( pMe,
                        FMRADIOLS_RES_FILE_LANG,
                        IDS_FMRADIO_FREQ_INPUT,
                        &rect,
                        MAKE_RGB(255,105,0),
                        AEE_FONT_NORMAL,
                        IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE
                    );
        #elif defined(FEATURE_DISP_160X128)
            int         width       = 70;
		
            SETAEERECT( &rect, ( 160 - width) / 2, (( 128 - fontHeight) / 2) - fontHeight, width, fontHeight);
            drawText( pMe,
                        FMRADIOLS_RES_FILE_LANG,
                        IDS_FMRADIO_FREQ_INPUT,
                        &rect,
                        MAKE_RGB(255,105,0),
                        AEE_FONT_NORMAL,
                        IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE
                    );
        #else
        	int         width       = 86;
		
            SETAEERECT( &rect, ( 176 - width) / 2, (( 220 - fontHeight) / 2) - fontHeight, width, fontHeight);
            drawText( pMe,
                        FMRADIOLS_RES_FILE_LANG,
                        IDS_FMRADIO_FREQ_INPUT,
                        &rect,
                        MAKE_RGB(255,105,0),
                        AEE_FONT_NORMAL,
                        IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE
                    );
        #endif
            //IDISPLAY_UpdateEx( pMe->m_pDisplay, FALSE);            
        }
    }
    #endif

    IDISPLAY_UpdateEx( pMe->m_pDisplay, FALSE);
}

static void repaint( CFmRadio *pMe, boolean immediately)
{

    if( immediately)
    {
        ISHELL_HandleEvent( pMe->m_pShell,
                     EVT_USER_REDRAW,
                     0,
                     0
                 );
    }
    else
    {
        ISHELL_PostEvent( pMe->m_pShell,
                     AEECLSID_APP_FMRADIO,
                     EVT_USER_REDRAW,
                     0,
                     0
                 );
    }
}

static void drawBg( CFmRadio *pMe)
{

    drawImage( pMe, FMRADIOLN_RES_FILE, IDI_BG, 0, 0);
}

static void drawOperationPrompt( CFmRadio *pMe, int16 resId, RGBVAL color)
{
//wangliang close!
#ifndef FEATURE_FMRADIO_SIMPLE_VERSION
    static AEERect rect = {0};
    int nFontHeight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_BOLD, NULL, NULL);

    SETAEERECT(&rect,
                        pMe->m_rc.x + FMRADIO_CHANNEL_XOFFSET + FMRADIO_LED_LIGHT_SIZE, 
                        pMe->m_rc.y + FMRADIO_OPERATION_YOFFSET*2, 
                        pMe->m_rc.dx - 2*(FMRADIO_CHANNEL_XOFFSET + FMRADIO_LED_LIGHT_SIZE), 
                        nFontHeight);
    drawImageWithOffset( pMe, FMRADIOLN_RES_FILE, IDI_BG, rect.x, rect.y, &rect);
    drawText( pMe,
            FMRADIOLS_RES_FILE_LANG,
            resId,
            &rect,
            color,
            AEE_FONT_BOLD,
            IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE
        );
    IDISPLAY_UpdateEx( pMe->m_pDisplay, TRUE);
#endif
}

static int getIndexByMenuItemId( CFmRadio *pMe, uint16 menuItemId)
{

    int index = 0;
    int total = IMENUCTL_GetItemCount( pMe->m_pMenu);

    for( index = 0; index < total; index ++)
    {
        if( IMENUCTL_GetItemID( pMe->m_pMenu, index) == menuItemId)
        {
            return index;
        }
    }

    return -1;
}

static void convertChannelValueToText( int channel, AECHAR *textBuffer, int bufferSizeInBytes)
{
#if defined FEATURE_FM_SPACE_50KHZ
	AECHAR  format[]    = { '%', 'd', '.', '%','0','2','d', ' ', 'M', 'H', 'z', 0};
    uint32  iFreqInt    = 0;              //Ƶ�ʵ���������
    uint32  iFreqFra    = 0;              //Ƶ�ʵ�С������

    iFreqInt = ( LOWEST_BAND_FREQ + CHANNEL_SPACE * channel);
    iFreqFra = ( iFreqInt % 1000) / 10;/*bw:100;*/
    iFreqInt /= 1000;
    WSPRINTF( textBuffer, bufferSizeInBytes, format, iFreqInt, iFreqFra);
#else
	AECHAR  format[]    = { '%', 'd', '.', '%', 'd', ' ', 'M', 'H', 'z', 0};
    uint32  iFreqInt    = 0;              //Ƶ�ʵ���������
    uint32  iFreqFra    = 0;              //Ƶ�ʵ�С������

    iFreqInt = ( LOWEST_BAND_FREQ + CHANNEL_SPACE * channel);
    iFreqFra = ( iFreqInt % 1000) / 100;
    iFreqInt /= 1000;
    WSPRINTF( textBuffer, bufferSizeInBytes, format, iFreqInt, iFreqFra);
#endif //#if defined FEATURE_FM_SPACE_50KHZ
}
static int multipliedBy10( int multiplicand, int number)
{

    int i = 0;

    for( i = 0; i < number; i ++)
    {
        multiplicand *= 10;
    }

    return multiplicand;
}

static int convertChannelValueFromText( AECHAR *textBuffer)
{
#ifndef WIN32
    AECHAR  text[16]    = {0};
    int     integral    = 0;
    int     fraction    = 0;
    int     result      = 0;

    int     length      = WSTRLEN( textBuffer);
    AECHAR* fractionStr = WSTRCHR( textBuffer, (AECHAR)'.');

    WSTRLCPY( text, textBuffer, fractionStr == NULL ? length + 1 : fractionStr - textBuffer + 1);
    integral = WSTRTOFLOAT( text);

    if( fractionStr)
    {
#ifndef min
#define min(a,b) (((a)<(b)) ? (a) : (b))
#endif

        int i  = 0;
        fractionStr ++;

#if defined FEATURE_FM_SPACE_50KHZ
        length = min( 2, WSTRLEN( fractionStr));
#else
        length = min( 1, WSTRLEN( fractionStr));
#endif

        for( i = 0; i < length; i ++)
        {
            fraction += multipliedBy10( fractionStr[i] - '0', 2 - i);
        }
    }

    result = ( integral * 1000 + fraction - LOWEST_BAND_FREQ) / CHANNEL_SPACE;
    return result;
#else
	return 0;
#endif//WIN32
}

static void drawChannelIndicator( CFmRadio *pMe)
{
    AEERect rect;
    AECHAR  szBuf[20] = {0};            //�������
    int nFontHeight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_BOLD, NULL, NULL);

//#ifndef FEATURE_FMRADIO_SIMPLE_VERSION// draw cursor
#if 0
    SETAEERECT(&rect, 
                pMe->m_rc.x + FMRADIO_CHANNEL_XOFFSET, 
                pMe->m_rc.y + FMRADIO_CHANNEL_YOFFSET, 
                pMe->m_rc.dx - 2*FMRADIO_CHANNEL_XOFFSET, 
                FMRADIO_CURSOR_DY);
    drawImageWithOffset( pMe, FMRADIOLN_RES_FILE, IDI_BG, rect.x, rect.y, &rect);
    drawImage( pMe,
            FMRADIOLN_RES_FILE,
            IDI_CURSOR,
            pMe->m_rc.x + FMRADIO_CHANNEL_XOFFSET  + 7* pMe->cfg.channel/10,//(CHANNEL_SPACE * pMe->cfg.channel / 200),
            pMe->m_rc.y + FMRADIO_CHANNEL_YOFFSET);
#endif

#ifdef FEATURE_FMRADIO_SIMPLE_VERSION
	// draw Channel Name, Channel Freq
    convertChannelValueToText( pMe->cfg.channel, szBuf, 40);
    SETAEERECT(&rect, 
                pMe->m_rc.x, 
                pMe->m_rc.y + FMRADIO_CHANNEL_FREQ_YOFFSET, 
                pMe->m_rc.dx,
                nFontHeight);
    drawImageWithOffset( pMe, FMRADIOLN_RES_FILE, IDI_BG, rect.x, rect.y, &rect);
    drawText2( pMe->m_pDisplay,
            szBuf,
            &rect,
            RGB_WHITE,
            AEE_FONT_BOLD,
            IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE
        );
#else
    // draw Channel Name, Channel Freq
    convertChannelValueToText( pMe->cfg.channel, szBuf, 40);
/*    SETAEERECT(&rect, 
                pMe->m_rc.x + FMRADIO_CHANNEL_XOFFSET, 
                pMe->m_rc.y + FMRADIO_CHANNEL_FREQ_YOFFSET, 
                pMe->m_rc.dx,
                nFontHeight);*/
    SETAEERECT(&rect, 
                pMe->m_rc.x, 
                pMe->m_rc.y + FMRADIO_CHANNEL_FREQ_YOFFSET, 
                pMe->m_rc.dx,
                nFontHeight);                
    drawImageWithOffset( pMe, FMRADIOLN_RES_FILE, IDI_BG, rect.x, rect.y, &rect);
    drawText2( pMe->m_pDisplay,
            szBuf,
            &rect,
            RGB_WHITE,
            AEE_FONT_BOLD,
            IDF_TEXT_TRANSPARENT | IDF_ALIGN_CENTER | IDF_ALIGN_MIDDLE
        );
#endif
    IDISPLAY_UpdateEx( pMe->m_pDisplay, TRUE);
}

static void drawVolumeIndicator( CFmRadio *pMe)
{
//#ifndef FEATURE_FMRADIO_SIMPLE_VERSION
#if 0
    AEERect rect;

    // draw background
    SETAEERECT(&rect,
                pMe->m_rc.x + FMRADIO_VOLUME_XOFFSET, 
                pMe->m_rc.y + FMRADIO_VOLUME_YOFFSET, 
                FMRADIO_VOLUME_LENGTH, 
                FMRADIO_VOLUME_BLOCK_SIZE);
    drawImageWithOffset( pMe, FMRADIOLN_RES_FILE, IDI_BG, rect.x, rect.y, &rect);

    // draw volume
    SETAEERECT(&rect,
                        0, 
                        0, 
                        FMRADIO_VOLUME_BLOCK_SIZE, 
                        FMRADIO_VOLUME_BLOCK_SIZE);
    drawImageWithOffset( pMe, 
                        FMRADIOLN_RES_FILE, 
                        IDI_VOLUME, 
                        pMe->m_rc.x + FMRADIO_VOLUME_XOFFSET+ (FMRADIO_VOLUME_LENGTH - FMRADIO_VOLUME_BLOCK_SIZE)*pMe->byVolumeLevel/7, 
                        pMe->m_rc.y + FMRADIO_VOLUME_YOFFSET, 
                        &rect);
    IDISPLAY_UpdateEx( pMe->m_pDisplay, TRUE);
#endif
}

static void drawSoftkey( CFmRadio *pMe)
{
    int i = 0;
    int nFontHeight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_BOLD, NULL, NULL);
    AEERect rectMask = {0};

    SETAEERECT(&rectMask,
                pMe->m_rc.x + 3, 
                pMe->m_rc.dy - nFontHeight, 
                pMe->m_rc.dx - 5, 
                nFontHeight);

    for( i = 0; i < 3; i ++)
    {

        static       char*  resFile[]    = { APPSCOMMON_RES_LANG_FILE, FMRADIOLS_RES_FILE_LANG, APPSCOMMON_RES_LANG_FILE};
        static const int16  resourceId[] = { IDS_OPTION, IDS_FMRADIO_SOFTKEY_MODE, IDS_FM_BACK}; // IDS_BACK};
        static const uint32 alignFlag[]  = { IDF_ALIGN_LEFT, IDF_ALIGN_CENTER, IDF_ALIGN_RIGHT};

#if (defined( FEATURE_FMRADIO_NO_MODE_SELECT) || defined( FEATURE_FMRADIO_KEY_OK_TO_MUTE))
		if( i == 1)
		{
			continue;
		}
#endif

        drawText( pMe,
              resFile[i],
              resourceId[i],
              &rectMask,
              RGB_WHITE,
              AEE_FONT_BOLD,
              IDF_TEXT_TRANSPARENT | IDF_ALIGN_MIDDLE | alignFlag[i]
          );
    }

    IDISPLAY_UpdateEx( pMe->m_pDisplay, TRUE);
}

#if FEATURE_FMRADIO_CHANNEL_LIST_SUPPORT

static void drawRefreshListPrompt( void *pme)
{
#ifndef FEATURE_FMRADIO_SIMPLE_VERSION
    RGBVAL   color = 0;
    CFmRadio *pMe  = (CFmRadio*)pme;

    color = MAKE_RGB( pMe->demitintColor, pMe->demitintColor, pMe->demitintColor);
    pMe->demitintColor = ( pMe->demitintColor + 205) % 255;

    drawOperationPrompt( pMe,
            pMe->opMode == FM_RADIO_OPMODE_REFRESH_CHANNEL_LIST ? IDS_FMRADIO_PROMPT_SEARCHING : IDS_FMRADIO_PROMPT_TUNING,
            color
        );
    drawLedLight( pMe);
    IDISPLAY_UpdateEx( pMe->m_pDisplay, FALSE);

    if( pMe->drawRefreshListPrompt)
    {
        ISHELL_SetTimer( pMe->m_pShell, 200, drawRefreshListPrompt, pme);
    }
#endif
}

static void stopDrawRefreshListPrompt( CFmRadio *pMe)
{

    ISHELL_CancelTimer( pMe->m_pShell, drawRefreshListPrompt, (void*)pMe);
    pMe->drawRefreshListPrompt = FALSE;
}
#endif

static void drawLedLight( CFmRadio *pMe)
{
//#ifndef FEATURE_FMRADIO_SIMPLE_VERSION
#if 0
    int nFontHeight = IDISPLAY_GetFontMetrics(pMe->m_pDisplay, AEE_FONT_BOLD, NULL, NULL);
    AEERect rect[] = {
            { 0, FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE}, //FM_RADIO_LED_LIGHT_PLAYING, green1
            { FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE}, //FM_RADIO_LED_LIGHT_IDLE, green2
            { 0, 0, FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE},  //FM_RADIO_LED_LIGHT_SEARCHING, red1

            { 0, FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE}, //FM_RADIO_TUNNING_MODE_LIST, green1
            { 0, 2*FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE}, //FM_RADIO_TUNNING_MODE_MANUAL, blue1
            { 0, 0, FMRADIO_LED_LIGHT_SIZE, FMRADIO_LED_LIGHT_SIZE}  //FM_RADIO_TUNNING_MODE_AUTO, red1
    };
    static int switcher = 0;

    rect[2].x = FMRADIO_LED_LIGHT_SIZE * switcher;
    switcher ++;
    switcher %= 2;

    drawImageWithOffset( pMe, 
                        FMRADIOLN_RES_FILE, 
                        IDI_LED_LIGHT, 
                        pMe->m_rc.x + FMRADIO_CHANNEL_XOFFSET, 
                        pMe->m_rc.y + FMRADIO_OPERATION_YOFFSET + (nFontHeight - FMRADIO_LED_LIGHT_SIZE)/2, 
                        &rect[pMe->cfg.tuningMode + 3]);
    drawImageWithOffset( pMe, 
                        FMRADIOLN_RES_FILE, 
                        IDI_LED_LIGHT, 
                        pMe->m_rc.dx - FMRADIO_CHANNEL_XOFFSET - FMRADIO_LED_LIGHT_SIZE, 
                        pMe->m_rc.y + FMRADIO_OPERATION_YOFFSET + (nFontHeight - FMRADIO_LED_LIGHT_SIZE)/2, 
                        &rect[pMe->ledLightType]);
    IDISPLAY_UpdateEx( pMe->m_pDisplay, TRUE);
#endif
}

static void drawImage( CFmRadio *pMe, char *resFile, int16 resId, int x, int y)
{

    IImage *image = ISHELL_LoadResImage( pMe->m_pShell, resFile, resId);
    if( image != NULL)
    {

        IIMAGE_Draw( image, x, y);
        IIMAGE_Release( image);
    }
}

static void drawImageWithOffset( CFmRadio *pMe, char *resFile, int16 resId, int x, int y, AEERect *pRectOffset)
{

    IImage *image = ISHELL_LoadResImage( pMe->m_pShell, resFile, resId);
    if( image != NULL)
    {

        AEERect oldClip;
        AEERect clip;

        SETAEERECT( &clip, x, y, pRectOffset->dx, pRectOffset->dy);
        IDISPLAY_GetClipRect( pMe->m_pDisplay, &oldClip);
        IDISPLAY_SetClipRect( pMe->m_pDisplay, &clip);

        IIMAGE_SetOffset( image, pRectOffset->x, pRectOffset->y);
        IIMAGE_SetDrawSize( image, pRectOffset->dx, pRectOffset->dy);
        IIMAGE_Draw( image, x, y);
        IIMAGE_Release( image);

        IDISPLAY_SetClipRect( pMe->m_pDisplay, &oldClip);
    }
}

static void drawText( CFmRadio *pMe,
          char     *pResFile,
          int16    resId,
          AEERect  *rect,
          RGBVAL   theTextColor,
          AEEFont  font,
          uint32   dwAlignFlags
)
{

    if( NULL == pMe || NULL == rect || NULL == pResFile)
    {
        return;
    }
    else
    {

        AECHAR text[256];
        ISHELL_LoadResString( pMe->m_pShell,
                              pResFile,
                              resId,
                              text,
                              256 * sizeof( AECHAR)
                          );

        drawText2( pMe->m_pDisplay, text, rect, theTextColor, font, dwAlignFlags);
    }
}

static void drawText2( IDisplay *pDisplay,
        AECHAR   *text,
        AEERect  *rect,
        RGBVAL   theTextColor,
        AEEFont  font,
        uint32   dwAlignFlags
)
{

    RGBVAL  oldColor    = 0;
    int     textLength  = ( text == NULL) ? 0 : WSTRLEN( text);
    int     textOffset  = 0;
    int     fontHeight  = 0;
    int     y           = rect->y;
    AEERect oldClip;
    AEERect rectDrawing = *rect;

    if( pDisplay == NULL || rect == NULL || textLength == 0)
    {
        return;
    }

    fontHeight = IDISPLAY_GetFontMetrics( pDisplay, font, 0, 0);
    oldColor = IDISPLAY_SetColor( pDisplay, CLR_USER_TEXT, theTextColor);
    IDISPLAY_GetClipRect( pDisplay, &oldClip);
    IDISPLAY_SetClipRect( pDisplay, rect);

    if( dwAlignFlags & IDF_ALIGN_MIDDLE)
    {
        fontHeight *= 2;
    }

    do
    {

        int     fits = 0;
        AECHAR  line[64];
        boolean newLineFound = FALSE;

        if( ( y >= rectDrawing.y + rectDrawing.dy) ||
            IDISPLAY_MeasureTextEx( pDisplay,
                              font,
                              text + textOffset,
                              textLength - textOffset,
                              rectDrawing.dx,
                              &fits
                       ) == 0
        )
        {
            break;
        }

        {
            AECHAR* result = WSTRCHR( text + textOffset, '\\');
            int     length = result - text - textOffset;
            if( result != NULL && (length + textOffset < textLength) && result[1] == 'n')
            {
                newLineFound = TRUE;
                fits = min( fits, length);
            }
            else
            {
                newLineFound = FALSE;
            }
        }
        WSTRNCOPYN( line, 64, text + textOffset, fits);
        IDISPLAY_DrawText( pDisplay,
                    font,
                    line,
                    -1,
                    rectDrawing.x,
                    y,
                    &rectDrawing,
                    dwAlignFlags | IDF_TEXT_TRANSPARENT
                );

        textOffset      += fits;
        y               += fontHeight;
        rectDrawing.y   += fontHeight;
        rectDrawing.dy  -= fontHeight;

        if( newLineFound)
        {
            textOffset += 2;
        }
    }
    while( textOffset < textLength);

    IDISPLAY_SetColor( pDisplay, CLR_USER_TEXT, oldColor);
    IDISPLAY_SetClipRect( pDisplay, &oldClip);
}

static void drawPrompt( CFmRadio* pMe, uint16 stringResId, BottomBar_e_Type bottomBarType, boolean informative)
{

    PromptMsg_Param_type    parm;
    AECHAR                  text[128];

    MEMSET( &parm, 0, sizeof( parm));
    parm.ePMsgType  = informative ? MESSAGE_INFORMATIVE : MESSAGE_CONFIRM;
    parm.eBBarType  = bottomBarType;

    ISHELL_LoadResString( pMe->m_pShell,
                       FMRADIOLS_RES_FILE_LANG,
                       stringResId,
                       text,
                       256
                   );
    parm.pwszMsg = text;

    DrawPromptMessage( pMe->m_pDisplay,
                pMe->pStatic,
                &parm
            );
}

static void FmRadio_DrawVolumeImage(CFmRadio *pMe, uint16 ResID, int x, int y)
{
    //���ԭ���ڴ�δ�ͷţ��ͷ�֮
    if (pMe->m_pVolumeImage)
    {
       IIMAGE_Release(pMe->m_pVolumeImage);
       pMe->m_pVolumeImage = NULL;
    }
    //loadͼƬ
    pMe->m_pVolumeImage = ISHELL_LoadResImage(pMe->m_pShell, 
                                      FMRADIOLN_RES_FILE, 
                                      ResID);
    if(pMe->m_pVolumeImage == NULL)
    {
        return;
    }
    
    
    //��ͼ  
    if(pMe->m_pVolumeImage)
    {
       IIMAGE_Draw(pMe->m_pVolumeImage,x,y);
    } 
     if (pMe->m_pVolumeImage)
    {
       IIMAGE_Release(pMe->m_pVolumeImage);
       pMe->m_pVolumeImage = NULL;
    }
    //repaint( pMe, FALSE);
}

static void FmRadio_RefreshVolumeImage(CFmRadio *pMe)
{
    uint16 ResID;
    switch (pMe->byVolumeLevel)
    {
        case 0:
            ResID = IDI_FM_SIMPLEVOL_OFF;
            break;
        case 3:
            ResID = IDI_FM_SIMPLEVOL_ONE;
            break;
        case 6:
            ResID = IDI_FM_SIMPLEVOL_TWO;
            break;
        case 9:
            ResID = IDI_FM_SIMPLEVOL_THREE;
            break;
        case 12:
            ResID = IDI_FM_SIMPLEVOL_FOUR;
            break;
        case 15:
            ResID = IDI_FM_SIMPLEVOL_FIVE;
            break;
        default :
           //ResID = IDI_FM_SIMPLEVOL_THREE;    
            break;
    }
    FmRadio_DrawVolumeImage( pMe, ResID, FM_VOLUME_X, FM_VOLUME_Y);
}

