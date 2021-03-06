#ifndef AIT_INTERFACE_H
#define AIT_INTERFACE_H
#include "a8_common.h"
#include "cam_module.h"
#include "sys_if_ait_api.h"
#include "mmidispsize.h"

//add by yangdecai 09-24
#if defined (FEATURE_DISP_320X240)
#define __MMI_MAINLCD_320X240__
#elif defined (FEATURE_DISP_220X176)
#define __MMI_MAINLCD_220X176__
#else
#define __MMI_MAINLCD_220X176__
#endif


//------------------------------------------------------------------------------------//
// LCD customization definition
//------------------------------------------------------------------------------------//
#if defined(__MMI_MAINLCD_240X320__)
#define A8_MAIN_LCD_WIDTH			(240)
#define A8_MAIN_LCD_HEIGHT		(320)
#elif defined(__MMI_MAINLCD_320X240__)
#define A8_MAIN_LCD_WIDTH			(320)
#define A8_MAIN_LCD_HEIGHT		(240)
#elif defined(__MMI_MAINLCD_176X220__	)
#define A8_MAIN_LCD_WIDTH			(176)
#define A8_MAIN_LCD_HEIGHT		(220)
#elif defined(__MMI_MAINLCD_220X176__)
#define A8_MAIN_LCD_WIDTH			(220)
#define A8_MAIN_LCD_HEIGHT		(176)
#elif defined(__MMI_MAINLCD_128X160__)
#define A8_MAIN_LCD_WIDTH			(128)
#define A8_MAIN_LCD_HEIGHT		(160)
#elif defined(__MMI_MAINLCD_160X128__)  //add by yangdecai 09-24
#define A8_MAIN_LCD_WIDTH			(160)
#define A8_MAIN_LCD_HEIGHT			(128)
#elif defined(__MMI_MAINLCD_128X128__)
#define A8_MAIN_LCD_WIDTH			(128)
#define A8_MAIN_LCD_HEIGHT		(128)
#else
#define A8_MAIN_LCD_WIDTH			(320)
#define A8_MAIN_LCD_HEIGHT		(240)
#endif

//===========================
// External Clock
//===========================
//#define EXTCLK_13M
//#define EXTCLK_26M
//#define EXTCLK_19M2

#if defined(__QSC_TARGET__)

#define AIT_SLEEP_CHECKTIME 	(300)

#endif
// USB EN control 
//#define AIT701_DP_PULLUP_ENABLE		{A800_GetGPIO(AIT_GPIO_USB_DP_CTL,AIT_GPIO_PULL_FLOATING);}
//#define AIT701_DP_PULLUP_DISABLE	{A800_ClearGPIO(AIT_GPIO_USB_DP_CTL);}
//#define AIT701_DP_PULLUP_STANDBY	AIT701_DP_PULLUP_ENABLE
//#define AIT701_DM_PULLUP_ENABLE	    (void)gpio_out(GPIO_OUTPUT_29, GPIO_HIGH_VALUE);
//#define AIT701_DM_PULLUP_DISABLE	(void)gpio_out(GPIO_OUTPUT_29, GPIO_LOW_VALUE);
//#define AIT701_DM_PULLUP_STANDBY	AIT701_DM_PULLUP_DISABLE


//AIT GPIO PIN
#define AIT_GPIO_SD_POWER_CTL 		0xff
#define AIT_GPIO_SLAVE_SENSOR_PWDN_CTL (0xFF)
#define AIT_GPIO_SENSOR_POWER_CTL (18)
#define AIT_GPIO_USB_DP_CTL 	(7)
#define AIT_GPIO_USB_DM_CTL 	(6)



// 1(high): 1.8V Flash, 0(low): 2.8V Flash 
#define AIT_MTK_1_8V_FLASH_HIGH			(1)
#define AIT_MTK_2_8V_FLASH_LOW			(0)
#define AIT_VIDREC_INT_POLARITY 	(AIT_MTK_2_8V_FLASH_LOW) 

//#define AIT_3GP_SUPPORT 1
#define AIT_VIDEO_PHONE_SUPPORT (0)
#define AIT_VIDPHONE_TEST_ENABLE 0

#define AIT_YUV_BUF_VDOPLY_TEST 0
#define AIT_MAX_SENSOR_SUPPORT 10

#define VIDEO_RECORDING_QUALITY 4	//0~4

//Sensor FPS
#define VIDEO_RECORDING_SENSOR_FPS (15)

//Debug Message Enable
#define AIT_LOG_ENABLE_VDO_REC 1
#define AIT_LOG_ENABLE_SD 0

#define OSD_TRANSPARENCY_COLOR 0x1F

//Vin@20091210:Twin Sensor & TV
typedef enum ePREVIEW_SRC_MODE{
	AIT_MASTER_SENSOR_SRC=0,
	AIT_SLAVE_SENSOR_SRC,
	AIT_TV_SRC
}ePREVIEW_SRC_MODE;

typedef enum ePREVIEW_TV_SRC{
	AIT_TV_SRC_PALD = 0,
	AIT_TV_SRC_NTSC	
}ePREVIEW_TV_SRC;

typedef enum eEMI_MODE{
	EMIMODE_BYPASS,
	EMIMODE_ACTIVE_PLL,
	EMIMODE_ACTIVE_NO_PLL
}eEMI_MODE;


//SD Interface
//extern unsigned char* AIT701_SD_Initialize(void);
//extern unsigned short AIT701_SD_ReadSector( unsigned int startsect,unsigned int offset,unsigned char *buf,unsigned int read_size );
//extern unsigned short AIT701_SD_WriteSector( unsigned int startsect,unsigned int offset,unsigned char *buf,unsigned int write_size );

//USB Interface
//extern void AIT701_usb_msdc_start(void);
//extern void AIT701_usb_msdc_stop(void);
//extern void AIT701_USB_WebCAMActive(void);

// Video Recording
//extern void AIT701_Camera_MJPEG_Record_Start(void);
//extern void AIT701_Camera_MJPEG_Record_Pause(void);
//extern void AIT701_Camera_MJPEG_Record_Resume(void);
//extern void AIT701_Camera_MJPEG_Record_Stop(void);
//extern unsigned char* AIT701_Camera_MJPEG_Encode(unsigned char* pFileBuff, unsigned int* pJpegfilesize);
//extern int AIT701_Camera_MJPEG_Interrupt(unsigned char* pJpegFile);

// Video Play Function
//extern u_char AIT701_avi_playback(u_short *frameBuf, u_int frameSize, u_short image_width, u_short image_height, u_char update_to_lcd);
//extern void AIT701_avi_playback_pause(void);
//extern void AIT701_avi_playback_resume(void);
//extern void AIT701_avi_playback_stop(void);
extern u_char AIT701_avi_playback_ChangePlayScreen(u_short mode);


// Others
extern void AIT701_cam_update_osd(unsigned short *osdBuffer, unsigned short start_x, unsigned short start_y, unsigned short width, unsigned short height);
extern unsigned char AIT701_JpegDecode(unsigned short * jpegBufPtr, unsigned int jpegsize, unsigned int panelwidth, unsigned int panelheight, unsigned char* RGBBufPtr,unsigned int RBGBufStartXFill );

//
extern void AIT701_enter_sleep(void);

//BB interface
extern void AIT_ext_cam_cmd_mapping(void);
extern void AIT_ext_Create_Mutex(void);
extern unsigned char AIT_ext_Take_Semaphore(unsigned short mode);
extern void AIT_ext_Give_Semaphore(void);
//AIT Related PIN control
extern void AIT_ext_ResetPinCtl(void);
extern void AIT_ext_BypassPinCtl(unsigned char bEnable);
extern void AIT_ext_ClockPinCtl(unsigned char bEnable);
extern void AIT_ext_Set_EMIMode(eEMI_MODE mode);
//Dummy function for Win32
extern void	CopyMemByteHostToA8(u_int destAddr, u_char *srcAddr, u_int length);
extern void lcd_busy_waiting(void);
extern void Trans565to332ToA8(u_int destAddr, u_short *srcAddr, u_int length);
extern void EINT_Set_Polarity(uint8 eintno, boolean ACT_Polarity);
extern void EINT_UnMask(uint8 eintno);
extern void EINT_Mask(uint8 eintno);
extern void AIT_ext_SetLCDWindow(unsigned short x,unsigned short y,unsigned short w,unsigned short h);
extern void AIT_ext_SetLCDRotate(unsigned char bRotate);
extern uint8 ait_is_active_cam(void);
extern void AIT701_DP_PULLUP_ENABLE(void);
extern void AIT701_DP_PULLUP_DISABLE(void);
extern void AIT701_DP_PULLUP_STANDBY(void);
extern void AIT701_DM_PULLUP_ENABLE(void);
extern void AIT701_DM_PULLUP_DISABLE(void);
extern void AIT701_DM_PULLUP_STANDBY(void);


#endif
