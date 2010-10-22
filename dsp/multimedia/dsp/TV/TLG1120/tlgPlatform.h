#ifndef	__TLG_PLATFORM_H__
#define	__TLG_PLATFORM_H__

#include "msg.h"
//#include "camsensor.h"
//#include "camctrl.h"
#include "i2c.h"
#include "AEEStdLib.h"
#include "gpio_1100.h"


#ifndef TLG_1120
#define TLG_1120
#endif


#define TLG_I2C_USE_BUS   /*to choose the i2c communication with gpio or I2C bus*/

#define TEST_TLG1120_I2C

/*****************************************************************************/
#define	APPEND_CHANNELS		0
//#define	0 //FM_CUTOFF_AT_76MHZ	0
#define TLG_MAX_PROGRAM_NUM 32

#define	H_OFFSET_525		0//9  8
#define	H_REDUCE_525		0//25 24
#define	V_OFFSET_525		0
#define	V_REDUCE_525		0

#define	H_OFFSET_625		0
#define	H_REDUCE_625		0
#define	V_OFFSET_625		0  //8
#define	V_REDUCE_625		0  //16

#define	H_OFFSET_RANGE		0
#define	H_REDUCE_RANGE		0
#define	V_OFFSET_RANGE		0
#define	V_REDUCE_RANGE		0

#define	TV_ACTIVE_IN_MENU		1


/*GPIO MAP*/  //modi by yangdecai
#define ATV_I2C_SDA      GPIO_OUTPUT_39
#define ATV_I2C_SCL      GPIO_OUTPUT_40
#define ATV_RESET_PIN    GPIO_OUTPUT_24
#define ATV_POWER_PIN    GPIO_OUTPUT_25

#define TLG_CMPCLK      GPIO_GENERIC_DEFAULT//CAMIF_PCLK
#define TLG_VSYNC       GPIO_GENERIC_DEFAULT//CAMIF_VSYNC
#define TLG_HSYNC       GPIO_GENERIC_DEFAULT//CAMIF_HSYNC


typedef enum 
{
    I2C_PIN_INPUT = GPIO_INPUT,                /*pin������״̬*/
    I2C_PIN_OUTPUT = GPIO_OUTPUT,               /*pin�����״̬*/
}I2C_PIN_DIR_e;

typedef enum
{
    TLG_SET_REGION,
    TLG_SET_VOLUME,
    TLG_SET_DEFINITION,      /*����������0~63*/    
    TLG_SET_BRIGHT,         /*��������0~15*/
    TLG_SET_CONTRAST,       /*���öԱȶ�0~15*/
    TLG_SET_SATURATION,     /*����ɫ�ʱ��Ͷ�0~255*/
    TLG_SET_DISPLAY_SIZE,   /*���û����С*/     
    TLG_SET_DISPLAY_OFFSET,   /*���û���ƫ����Ԥ��*/
    CAM_SET_DISPLAY_SIZE,
    CAM_SET_SIZE,
}ATV_SET_PARAM_e;

#define TLG_TEST_SENSITIVE
typedef enum
{    
    TLG_GET_CHN_COUNT,
#ifdef TLG_TEST_SENSITIVE
    TLG_GET_TFCODE,
    TLG_GET_TTLGAIN,
    TLG_GET_CHROMLOCK
#endif
}ATV_GET_PARAM_e;


typedef enum
{
    ATV_AUTO_SCAN,               /*�Զ�����ģʽ*/
    ATV_FAST_SET,                /*������̨*/
    ATV_SET_AV_STOP              /*��̨ʱ����Ƶ��ֹ*/   
}ATV_SET_CHN_TYPE_e;


#define INPUT_YUV422
#define INPUT_ORDER_YCbY1Cr

#define TLG_PRINT_0(fmt)                               MSG_FATAL(fmt, 0, 0, 0) 
#define TLG_PRINT_1(fmt,arg1)                          MSG_FATAL(fmt, arg1, 0, 0) 
#define TLG_PRINT_2(fmt,arg1, arg2)                    MSG_FATAL(fmt, arg1, arg2, 0) 
#define TLG_PRINT_3(fmt,arg1, arg2, arg3)              MSG_FATAL(fmt, arg1, arg2,arg3) 
#define TLG_ERR(fmt)                                   MSG_FATAL(fmt, 0, 0, 0) 

/*****************************************************************************/

#ifndef WIN32

typedef	unsigned char		kal_uint8;
typedef	signed char			kal_int8;
typedef	char				kal_char;
typedef	unsigned short		kal_wchar;

typedef	unsigned short int	kal_uint16;
typedef	signed short int	kal_int16;

typedef	unsigned int		kal_uint32;
typedef	signed int			kal_int32;

typedef	unsigned long /*long*/	kal_uint64;
typedef	signed long	/*long*/	kal_int64;

typedef	unsigned int kal_bool;

typedef	struct
{
	kal_uint16 grab_start_x;				/* The first grabed	column data	of the image sensor	in pixel clock count */
	kal_uint16 grab_start_y;				/* The first grabed	row	data of	the	image sensor in	pixel clock	count */
	kal_uint16 exposure_window_width;		/* Exposure	window width of	image sensor */
	kal_uint16 exposure_window_height;		/* Exposure	window height of image sensor */
	kal_uint16 image_target_width;			/* image captured width	*/
	kal_uint16 image_target_height;			/* image captuerd height */
	kal_uint16 exposure_pixel;				/* exposure	window width of	image sensor + dummy pixel */
	kal_uint16 exposure_line;				/* exposure	window width of	image sensor + dummy line */
	kal_uint8  digital_zoom_factor;			/* digital zoom	factor */
} image_sensor_exposure_window_struct;

typedef	struct
{
	kal_bool	enable_shutter_tansfer;		/* capture only	*/
	kal_uint16	capture_shutter;			/* capture only	*/
	kal_uint8	image_mirror;
	kal_uint8	frame_rate;
	kal_bool	meta_mode;					/* capture only, 0:	normal mode, 1:	meta mode */
	kal_bool	enable_flashlight_tansfer;	/* flash light capture only	*/
	kal_uint16	flashlight_duty;			/* flash light capture only	*/
	kal_uint16	flashlight_offset;			/* flash light capture only	*/
} image_sensor_config_struct;

typedef	struct
{
   kal_int8	(* sensor_init)(void);
   void	(* get_sensor_id)(kal_uint8	*sensor_write_id, kal_uint8	*sensor_read_id);
   void	(* get_sensor_size)(kal_uint16 *sensor_width, kal_uint16 *sensor_height);
   void	(* get_sensor_period) (kal_uint16 *pixel_number, kal_uint16	*line_number);
   void	(* sensor_preview_setting)(image_sensor_exposure_window_struct *image_window, image_sensor_config_struct *sensor_config_data);
   void	(* sensor_capture_setting)(image_sensor_exposure_window_struct *image_window, image_sensor_config_struct *sensor_config_data);
   void	(* write_sensor_reg)(kal_uint32	cmd, kal_uint32	para);
   kal_uint32 (* read_sensor_reg)(kal_uint32 cmd);
   void	(* set_sensor_eshutter)(kal_uint16 eshutter);
   void	(* set_camera_night_mode) (kal_bool	enable);
   void	(* sensor_power_off) (void);
   kal_uint16 (* set_sensor_gain) (kal_uint16 gain);
   void	(* set_flashlight) (kal_bool enable);
   kal_uint32 (* yuv_ATV_setting)(kal_uint32 cmd, kal_uint32 para);
} image_sensor_func_struct;

#define	GPIO_ModeSetup(a, b)
#define	GPIO_InitIO(a, b)
#define	GPIO_WriteIO(a, b)
char GPIO_ReadIO(char port);

#define	SET_CMOS_CLOCK_POLARITY_LOW   gpio_out(CAMIF_PCLK, GPIO_LOW_VALUE)
#define	SET_VSYNC_POLARITY_LOW        gpio_out(CAMIF_VSYNC, GPIO_LOW_VALUE)
#define	SET_VSYNC_POLARITY_HIGH       gpio_out(CAMIF_VSYNC, GPIO_HIGH_VALUE)
#define	SET_HSYNC_POLARITY_LOW        gpio_out(CAMIF_HSYNC, GPIO_LOW_VALUE)
#define	SET_HSYNC_POLARITY_HIGH       gpio_out(CAMIF_HSYNC, GPIO_HIGH_VALUE)
#define	ENABLE_CAMERA_INDATA_FORMAT      
#define	SET_CAMERA_INPUT_TYPE(a)
#define	SET_CAMERA_INPUT_ORDER(a)
#define	ENABLE_CAMERA_TG_CLK_48M     camsensor_config_camclk_po(48000000)
#define	DISABLE_CAMERA_TG_CLK_48M    camsensor_unconfig_camclk_po()
#define	set_isp_driving_current(a)
#define	ISP_DRIVING_2MA

#define	SET_TG_OUTPUT_CLK_DIVIDER
#define	SET_CMOS_RISING_EDGE(a)
#define	SET_CMOS_FALLING_EDGE(a)
#define	ENABLE_CAMERA_PIXEL_CLKIN_ENABLE
#define	SET_TG_PIXEL_CLK_DIVIDER(a)
#define	SET_CMOS_DATA_LATCH(a)
#define	DISABLE_CAMERA_TG_PHASE_COUNTER
#define	DISABLE_CAMERA_CLOCK_OUTPUT_TO_CMOS

#define ENABLE_CAMERA_IDLE_INT
#define DISABLE_VIEW_FINDER_MODE
#define ENABLE_VIEW_FINDER_MODE
#define isp_ibw2_cb() 
//void isp_ibw2_cb(void)



typedef void (*FuncPtr)(void);
#define	ATV_TIMER_RESET_DATA_PATH	1
#define	ATV_TIMER_CHANNEL_CHANGE	2
#define ENABLE_CAMERA_FRAME_READY_INT
extern kal_bool isp_digital_zoom_flag;


#define SaveAndSetIRQMask()  0
//kal_uint32 SaveAndSetIRQMask(void);
#define RestoreIRQMask(a) 
//void RestoreIRQMask(kal_uint32 a);
#define StartTimer(a, b,c) do{    \
                            }while(0);
//void StartTimer(kal_uint16 timerid, kal_uint32 delay, FuncPtr funcPtr);
void StopTimer(kal_uint16 timerid);
#else

typedef	unsigned char		kal_uint8;
typedef	signed char			kal_int8;
typedef	char				kal_char;
typedef	unsigned short		kal_wchar;

typedef	unsigned short int	kal_uint16;
typedef	signed short int	kal_int16;

typedef	unsigned int		kal_uint32;
typedef	signed int			kal_int32;

typedef	unsigned long /*long*/	kal_uint64;
typedef	signed long	/*long*/	kal_int64;

typedef	unsigned int kal_bool;

#endif /* WIN32 */

typedef	enum {
	TLG_REGION_USA,				// ����
	TLG_REGION_CANADA,			// ���ô�
	TLG_REGION_KOREA,				// ����
	TLG_REGION_TAIWAN,			// ̨��
	TLG_REGION_MEXICO,			// ī����
	TLG_REGION_CHILE,				// ����
	TLG_REGION_VENEZUELA,			// ί������
	TLG_REGION_PHILIPPINES,		// ���ɱ�
	TLG_REGION_JAMAICA,			// �����
	TLG_REGION_CHINA,				// �й���½
	TLG_REGION_SHENZHEN,			// ����
	TLG_REGION_VIETNAM,			// Խ��
	TLG_REGION_WESTERNEUROPE,	// ��ŷ
	TLG_REGION_TURKEY,			// ������
	TLG_REGION_UAE,				// ������
	TLG_REGION_AFGHANISTAN,		// ������
	TLG_REGION_SINGAPORE,			// �¼���
	TLG_REGION_THAILAND,			// ̩��
	TLG_REGION_CAMBODIA,			// ����կ
	TLG_REGION_INDONESIA,			// ӡ��������
	TLG_REGION_MALAYSIA,			// ��������
	TLG_REGION_LAOS,				// ����
	TLG_REGION_PORTUGAL,			// ������
	TLG_REGION_SWEDEN,			// ���
	TLG_REGION_PAKISTAN,			// �ͻ�˹̹
	TLG_REGION_SPAIN,				// ������
	TLG_REGION_JAPAN,				// �ձ�
	TLG_REGION_UK,					// Ӣ��
	TLG_REGION_HONGKONG,			// ���
	TLG_REGION_SOUTHAFRICA,		// �Ϸ�
	TLG_REGION_BRAZIL,				// ����
	TLG_REGION_ARGENTINA,		// ����͢
	TLG_REGION_INDIA,				// ӡ��
	TLG_REGION_BURMA,				// ���
	TLG_REGION_SUDAN,				// �յ�
	TLG_REGION_YEMEN,				// Ҳ��
	TLG_REGION_PERU,				// ��³
	TLG_REGION_BOLIVIA,			// ����ά��
	TLG_REGION_ECUADOR,			// ��϶��
	TLG_REGION_SURINAME, 			// ������
	TLG_REGION_AUSTRALIA,			// �Ĵ�����
	TLG_REGION_NEWZEALAND,		// ������
#ifdef    TLG_1120
	TLG_REGION_NORTHKOREA, 	/*������*/	
	TLG_REGION_MONGOLIA ,	/*	�ɹ�*/		
	TLG_REGION_AZERBAIJAN ,	/*�����ݽ�*/	
	TLG_REGION_CZETH, 		/*�ݿ�*/		
	TLG_REGION_POLAND, 		/*����*/		
	TLG_REGION_RUSSION,		/*����˹*/	
	TLG_REGION_UKRAINE, 		/*�ڿ���*/	
	TLG_REGIONI_TUNISIA, 	/*	ͻ��˹ */	
	TLG_REGION_EGYPT, 		/*����*/		
	TLG_REGION_MOROCCO, 		/*Ħ���*/	
	TLG_REGION_IRAQ,		/*������*/	
	TLG_REGION_SAUDIARABIA,	/*ɳ��*/		
	TLG_REGION_GREECE,		/*ϣ�� */		
#endif
	TLG_REGION_TOTAL
} TLG_REGION_CODE;

typedef	enum {
	TLG_TV_525_LINES,
	TLG_TV_625_LINES,
	TLG_TV_NONE
} TLG_TV_SCANLINES;

typedef	enum {
	TLG_SCRMD_WINDOW,
	TLG_SCRMD_FULLSCREEN
} TLG_SCREEN_MODE;

typedef enum {
	TLG_TUNEMD_AUTO,
	TLG_TUNEMD_MANUAL
} TLG_TUNE_MODE;

/*
typedef struct {
	p_tlg_channel_t(chnmap);
	//tlg_mode_t mode;
	//tlg_vidstd_t vidstd;
	//tlg_bw_t bw;
	//TLG_TV_SCANLINES scanlines;
	//uint16 chrlma_order, cbcr_order;
} TLG_REGION_MAP;
*/

typedef struct {
	uint8 mBrightness;				// ����
	uint8 mContrast;					// �Աȶ�
	uint8 mSaturation;				// ɫ���Ͷ�
	uint8 mDefinition;				// ������
	uint8 mVolume;					// ����

	uint16 mHOffset,					// ˮƽƫ�Ƶ���
	 	 mHReduce,					// ˮƽ��С����
		 mVOffset,					// ��ֱƫ�Ƶ���
		 mVReduce;					// ��ֱ��С����
} TLG_TV_OPTIONS;

typedef struct {
	TLG_TUNE_MODE mTuneMode;		// ��ǰ��гģʽ
	TLG_SCREEN_MODE mScrMode;		// ��ǰ��Ļģʽ
	TLG_REGION_CODE mRegion;		// ��ǰ���ӵ�������

	uint8 mIdxListCnt;				// Ƶ���б�����Ч��������
	char mCurListIdx;				// ��ǰƵ���б������
	uint8 mCurChnIdx;				// ��ǰ����Ƶ��������
	struct {
		uint8 ChnIdx;					// Ƶ������
		char ChnName[7*2+1];		// Ƶ������
	} mIdxListBuf[TLG_MAX_PROGRAM_NUM];				// Ƶ�������б�
} TLG_TV_STORAGE;

typedef struct {
	uint8 mVolume;					// ����
} TLG_FM_OPTIONS;

typedef struct {
	TLG_TUNE_MODE mTuneMode;		// ��ǰ��гģʽ

	uint8 mIdxListCnt;				// ��̨�б�����Ч��������
	uint8 mCurListIdx;				// ��ǰ��̨�б������
#if 0 //FM_CUTOFF_AT_76MHZ
	uint16
#else /* 0 //FM_CUTOFF_AT_76MHZ */
	uint8
#endif /* 0 //FM_CUTOFF_AT_76MHZ */
		mCurChnIdx;				// ��ǰ������̨������
	struct {
#if 0 //FM_CUTOFF_AT_76MHZ
	uint16
#else /* 0 //FM_CUTOFF_AT_76MHZ */
	uint8
#endif /* 0 //FM_CUTOFF_AT_76MHZ */
				ChnIdx;				// ��̨����
	} mIdxListBuf[32];				// ��̨�����б�
} TLG_FM_STORAGE;

extern int tlg_cur_chn_cnt;
extern TLG_TV_SCANLINES gTvScanLines,gPreScanLines;
extern TLG_TV_OPTIONS gTvOptions;
extern TLG_TV_STORAGE gTvStorage;
extern TLG_FM_OPTIONS gFmOptions;
extern TLG_FM_STORAGE gFmStorage;

void PowerOnFM(void);
void PowerOffFM(void);

int TLGAPP_SetChannelExt(int iChn);
void TLGAPP_InitRegionChannelMap(void);

void TLGMMI_NextTvChannel(void);
void TLGMMI_PrevTvChannel(void);
void TLGMMI_NextFmChannel(void);
void TLGMMI_PrevFmChannel(void);

int TLGMMI_GetRegion(TLG_REGION_CODE *pRegion);
int TLGMMI_SetRegion(TLG_REGION_CODE iRegion);
int TLGMMI_GetBrightness(uint8 *pBrightness);
int TLGMMI_SetBrightness(uint8 iBrightness);
int TLGMMI_GetContrast(uint8 *pContrast);
int TLGMMI_SetContrast(uint8 iContrast);
int TLGMMI_GetSaturation(uint8 *pSaturation);
int TLGMMI_SetSaturation(uint8 iSaturation);
int TLGMMI_GetDefinition(uint8 *pDefinition);
int TLGMMI_SetDefinition(uint8 iDefinition);
int TLGMMI_GetHSize(uint16 *pHsize);
int TLGMMI_GetVSize(uint16 *pVsize);
int TLGMMI_SetHSize(uint16 iHsize);
int TLGMMI_SetVSize(uint16 iVsize);
int TLGMMI_GetHOffset(uint16 *pHoffset);
int TLGMMI_GetVOffset(uint16 *pVoffset);
int TLGMMI_SetHOffset(uint16 iHoffset);
int TLGMMI_SetVOffset(uint16 iVoffset);
void TLGMMI_RestoreTvOption(void);
void TLGMMI_RestoreFmOption(void);
void TLGMMI_ClearTvStorage(void);
void TLGMMI_ClearFmStorage(void);


extern int TLG1120_tv_scan_channel(TLG_TV_STORAGE *tv_storage);    
extern int TLG1120_tv_set_channel(uint16 chn);
extern int TLG1120_tv_set_channel_ext(uint16 chn);
extern int TLG1120_tv_set_fast_channel(uint16 chn);
extern int TLG1120_tv_set_param(ATV_SET_PARAM_e type, int hparam, int lparam);
#if 0   //modi by yangdecai
extern boolean TLG1120_tlg1120_init(camsensor_function_table_type *camsensor_function_table_ptr,
				                                camctrl_tbl_type              *camctrl_tbl_ptr);
#endif

#if 0
extern void TLG1120_fm_power_on(void);
extern void TLG1120_fm_power_off(void);
extern int TLG1120_fm_set_channel(uint16 chn);
extern int TLG1120_fm_scan_channel(TLG_FM_STORAGE *fm_storage);
#endif
#endif /* _PLATFORM_H_ */

