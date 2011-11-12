/*============================================================================

                 Camera Interface Device Driver Source File for 
                    SP0838 VGA CMOS sensor
                    added by yangty LONGSHANG

   DESCRIPTION
     This file contains the definitions needed for the camera interface
     device driver.

   Copyright (c) 2006 by LCT Driver Team.  All Rights Reserved.
============================================================================*/

/*============================================================================

                      EDIT HISTORY FOR FILE

 This section contains comments describing changes made to this file.
 Notice that changes are listed in reverse chronological order.

$Header: //depot/asic/msm6550/drivers/camsensor/camsensor_ SP0838_ycbcr.c#3 $ $DateTime: 2006/08/11 14:02:49 $ $Author: klee $

 when      who    what, where, why
 --------  -----  ----------------------------------------------------------
  09/30/06  lee    Initial release.
 
============================================================================*/

/*============================================================================
                        INCLUDE FILES
============================================================================*/

#include "camsensor.h"
#if defined (FEATURE_CAMERA) && defined (USE_CAMSENSOR_SP0838)
#include "camerai.h"

#include "camsensor_sp0838.h"
#include "clk.h"

typedef struct register_address_value_pair_struct {
	uint8 register_address;
	uint8 register_value;
} register_address_value_pair;

#ifdef LOCAL
#undef LOCAL
#endif
#define LOCAL static

/*============================================================================
                        CONSTANT DEFINITIONS
============================================================================*/

#define CAMSENSOR_SP0838_I2C_SLAVE_ID              0x30 ///SP0838

#define CAMSENSOR_SP0838_RESET_PIN         GPIO_OUTPUT_10

#define CAMSENSOR_SP0838_ID      0x27
/* 
 * Maximum number of trials before aborting a register write operation
 *
 */
#define MAX_REGISTER_WRITE_TRIALS   3
#define MAX_REGISTER_READ_TRIALS    3
/*begin\bug6192\yangtingyu\20090226\camera�������£�ʵ�����ȵ���*/
#define SP0838_MAX_BRIGHTNESS  9
#define SP0838_MIN_BRIGHTNESS  0
/*end\bug6192\yangtingyu\20090226\camera�������£�ʵ�����ȵ���*/

#define SP0838_MAX_CONTRAST   4
#define SP0838_MIN_CONTRAST   0

#define RETURN_ON_FALSE(rc) if ((rc)==FALSE) return FALSE  ///lee


/* From the logic analyzer measurements */
#define OV_SP0838_YCBCR_FULL_SIZE_WIDTH           640//640
#define OV_SP0838_YCBCR_FULL_SIZE_HEIGHT          480

#define OV_SP0838_YCBCR_QTR_SIZE_WIDTH   		640//640   
#define OV_SP0838_YCBCR_QTR_SIZE_HEIGHT  		480//240//   

/* Strobe Flash Epoch Interrupt time before the end of line count */
static camera_antibanding_type g_iBanding = CAMERA_ANTIBANDING_OFF;
static camera_reflect_type g_reflect = CAMERA_MAX_REFLECT;
static int8 g_effect = CAMERA_EFFECT_MIN_MINUS_1;

/*===========================================================================
                          MACRO DEFINITIONS
============================================================================*/


/*============================================================================
                        STATIC VARIABLES 
============================================================================*/
// modified Longcheer3G_liudan_20070403.........................................
const char camsensor_sp0838_ycbcr_sensor_name[]  = "SP0838 VGA";

// modified end.................................................................
/*============================================================================
                          INTERNAL API PROTOTYPES
============================================================================*/
static boolean camsensor_SP0838_ycbcr_i2c_write_byte(uint8 reg, uint8 data);
static void camsensor_sp0838_ycbcr_register(camsensor_function_table_type *camsensor_function_table_ptr);
static boolean camsensor_SP0838_ycbcr_i2c_read_byte(uint8 reg, uint8 *data); 
static void    camsensor_sp0838_0m3_setup_camctrl_tbl(camctrl_tbl_type *camctrl_tbl_ptr);

static const register_address_value_pair sp0838_preview_5fps[] =
{ 
  {0xfd , 0x00},	//sp0838 24M 1��Ƶ 50Hz VGA 5-5fps 2xgain
  {0x05 , 0x00},
  {0x06 , 0x00},
  {0x09 , 0x00},
  {0x0a , 0x46},
  {0xf0 , 0x19},
  {0xf1 , 0x00},
  {0xf2 , 0x41},
  {0xf5 , 0x5a},
  {0xfd , 0x01},
  {0x00 , 0x9e},
  {0x0f , 0x42},
  {0x16 , 0x42},
  {0x17 , 0x8e},
  {0x18 , 0x96},
  {0x1b , 0x42},
  {0x1c , 0x96},
  {0xb4 , 0x18},
  {0xb5 , 0x18},
  {0xb6 , 0x18},
  {0xb9 , 0x40},
  {0xba , 0x4f},
  {0xbb , 0x47},
  {0xbc , 0x45},
  {0xbd , 0x43},
  {0xbe , 0x42},
  {0xbf , 0x42},
  {0xc0 , 0x42},
  {0xc1 , 0x41},
  {0xc2 , 0x41},
  {0xc3 , 0x41},
  {0xc4 , 0x41},
  {0xc5 , 0x41},
  {0xc6 , 0x41},
  {0xca , 0x70},
  {0xcb , 0x14},
  {0xfd , 0x00},
};

/*============================================================================
                          EXTERNAL API DEFINITIONS
============================================================================*/
LOCAL boolean camsensor_sp0838_sensor_init(void)
{
	int i = 0;
	MSG_FATAL("-----camsensor_SP0838_sensor_init-----enter",0,0,0);

	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x00); //P0
	camsensor_SP0838_ycbcr_i2c_write_byte(0x1B , 0x02);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x27 , 0xe8);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x28 , 0x0B);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x32 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x22 , 0xc0);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x26 , 0x10); 

	camsensor_SP0838_ycbcr_i2c_write_byte(0x31 , 0x10);   //Upside/mirr/Pclk inv/sub

	camsensor_SP0838_ycbcr_i2c_write_byte(0x5f , 0x11);   //Bayer order
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x01);   //P1
	camsensor_SP0838_ycbcr_i2c_write_byte(0x25 , 0x1a);   //Awb start
	camsensor_SP0838_ycbcr_i2c_write_byte(0x26 , 0xfb); 
	camsensor_SP0838_ycbcr_i2c_write_byte(0x28 , 0x75); 
	camsensor_SP0838_ycbcr_i2c_write_byte(0x29 , 0x4e);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x00);   
	camsensor_SP0838_ycbcr_i2c_write_byte(0xe7 , 0x03); 
	camsensor_SP0838_ycbcr_i2c_write_byte(0xe7 , 0x00); 
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x01);

	camsensor_SP0838_ycbcr_i2c_write_byte(0x31 , 0x60);//64
	camsensor_SP0838_ycbcr_i2c_write_byte(0x32 , 0x18);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x4d , 0xdc);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x4e , 0x53);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x41 , 0x8c);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x42 , 0x57);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x55 , 0xff);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x56 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x59 , 0x82);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x5a , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x5d , 0xff);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x5e , 0x6f);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x57 , 0xff);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x58 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x5b , 0xff);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x5c , 0xa8);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x5f , 0x75);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x60 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x2d , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x2e , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x2f , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x30 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x33 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x34 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x37 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x38 , 0x00);  //awb end
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x00);  //P0
	camsensor_SP0838_ycbcr_i2c_write_byte(0x33 , 0x6f);  //LSC BPC EN
	camsensor_SP0838_ycbcr_i2c_write_byte(0x51 , 0x3f);  //BPC debug start
	camsensor_SP0838_ycbcr_i2c_write_byte(0x52 , 0x09);  
	camsensor_SP0838_ycbcr_i2c_write_byte(0x53 , 0x00);  
	camsensor_SP0838_ycbcr_i2c_write_byte(0x54 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x55 , 0x10);  //BPC debug end
	camsensor_SP0838_ycbcr_i2c_write_byte(0x4f , 0x08);  //blueedge
	camsensor_SP0838_ycbcr_i2c_write_byte(0x50 , 0x08);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x57 , 0x10);  //Raw filter debut start
	camsensor_SP0838_ycbcr_i2c_write_byte(0x58 , 0x10);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x59 , 0x10);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x56 , 0x70);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x5a , 0x02);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x5b , 0x02);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x5c , 0x20);  //Raw filter debut end 
	camsensor_SP0838_ycbcr_i2c_write_byte(0x65 , 0x03);  //Sharpness debug start
	camsensor_SP0838_ycbcr_i2c_write_byte(0x66 , 0x01);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x67 , 0x03);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x68 , 0x46);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x69 , 0x7f);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x6a , 0x01);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x6b , 0x04);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x6c , 0x01);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x6d , 0x03);  //Edge gain normal
	camsensor_SP0838_ycbcr_i2c_write_byte(0x6e , 0x46);  //Edge gain normal
	camsensor_SP0838_ycbcr_i2c_write_byte(0x6f , 0x7f);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x70 , 0x01);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x71 , 0x05);   //����ֵ          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x72 , 0x01);   //��������ֵ        
	camsensor_SP0838_ycbcr_i2c_write_byte(0x73 , 0x03);   //��Ե��������ֵ    
	camsensor_SP0838_ycbcr_i2c_write_byte(0x74 , 0x46);   //��Ե��������ֵ    
	camsensor_SP0838_ycbcr_i2c_write_byte(0x75 , 0x7f);   //ʹ��λ            
	camsensor_SP0838_ycbcr_i2c_write_byte(0x76 , 0x01);  
	camsensor_SP0838_ycbcr_i2c_write_byte(0xcb , 0x07);  //HEQ&Saturation debug start 
	camsensor_SP0838_ycbcr_i2c_write_byte(0xcc , 0x04);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xce , 0xff);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xcf , 0x10);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xd0 , 0x20);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xd1 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xd2 , 0x1c);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xd3 , 0x16);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xd4 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xd6 , 0x1c);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xd7 , 0x16);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xdd , 0x70);  //Contrast
	camsensor_SP0838_ycbcr_i2c_write_byte(0xde , 0x90);  //HEQ&Saturation debug end
	camsensor_SP0838_ycbcr_i2c_write_byte(0x7f , 0xd7);  //Color Correction start
	camsensor_SP0838_ycbcr_i2c_write_byte(0x80 , 0xbc);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x81 , 0xed);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x82 , 0xd7);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x83 , 0xd4);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x84 , 0xd6);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x85 , 0xff);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x86 , 0x89);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x87 , 0xf8);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x88 , 0x3c);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x89 , 0x33);                          
	camsensor_SP0838_ycbcr_i2c_write_byte(0x8a , 0x0f);  //Color Correction end 
	camsensor_SP0838_ycbcr_i2c_write_byte(0x8b , 0x00);  //gamma start
	camsensor_SP0838_ycbcr_i2c_write_byte(0x8c , 0x1a);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x8d , 0x29);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x8e , 0x41);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x8f , 0x62);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x90 , 0x7c);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x91 , 0x90);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x92 , 0xa2);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x93 , 0xaf);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x94 , 0xbc);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x95 , 0xc5);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x96 , 0xcd);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x97 , 0xd5);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x98 , 0xdd);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x99 , 0xe5);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x9a , 0xed);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x9b , 0xf5);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x01);  //P1         
	camsensor_SP0838_ycbcr_i2c_write_byte(0x8d , 0xfd);               
	camsensor_SP0838_ycbcr_i2c_write_byte(0x8e , 0xff);  //gamma end  
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x00);  //P0
	camsensor_SP0838_ycbcr_i2c_write_byte(0xca , 0xcf);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xd8 , 0x48);  //UV outdoor
	camsensor_SP0838_ycbcr_i2c_write_byte(0xd9 , 0x48);  //UV indoor 
	camsensor_SP0838_ycbcr_i2c_write_byte(0xda , 0x48);  //UV dummy
	camsensor_SP0838_ycbcr_i2c_write_byte(0xdb , 0x48);  //UV lowlight
	camsensor_SP0838_ycbcr_i2c_write_byte(0xb9 , 0x00);  //Ygamma start
	camsensor_SP0838_ycbcr_i2c_write_byte(0xba , 0x04);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbb , 0x08);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbc , 0x10);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbd , 0x20);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbe , 0x30);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbf , 0x40);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc0 , 0x50);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc1 , 0x60);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc2 , 0x70);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc3 , 0x80);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc4 , 0x90);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc5 , 0xA0);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc6 , 0xB0);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc7 , 0xC0);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc8 , 0xD0);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc9 , 0xE0);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x01);  //P1
	camsensor_SP0838_ycbcr_i2c_write_byte(0x89 , 0xf0);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x8a , 0xff);  //Ygamma end
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x00);  //P0
	camsensor_SP0838_ycbcr_i2c_write_byte(0xe8 , 0x30);  //AEdebug start
	camsensor_SP0838_ycbcr_i2c_write_byte(0xe9 , 0x30);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xea , 0x40);  //Alc Window sel
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf4 , 0x1b);  //outdoor mode sel
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf5 , 0x80);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf7 , 0x78);  //AE target
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf8 , 0x63);  
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf9 , 0x68);  //AE target
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfa , 0x53);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x01);  //P1
	camsensor_SP0838_ycbcr_i2c_write_byte(0x09 , 0x31);  //AE Step 3.0
	camsensor_SP0838_ycbcr_i2c_write_byte(0x0a , 0x85);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x0b , 0x0b);  //AE Step 3.0
	camsensor_SP0838_ycbcr_i2c_write_byte(0x14 , 0x20);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x15 , 0x0f);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x00);  //P0
	camsensor_SP0838_ycbcr_i2c_write_byte(0x05 , 0x00);  
	camsensor_SP0838_ycbcr_i2c_write_byte(0x06 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x09 , 0x01);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x0a , 0x76);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf0 , 0x62);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf1 , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf2 , 0x5f);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf5 , 0x78);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x01);  //P1
	camsensor_SP0838_ycbcr_i2c_write_byte(0x00 , 0xba);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x0f , 0x60);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x16 , 0x60);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x17 , 0xa2);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x18 , 0xaa);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x1b , 0x60);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x1c , 0xaa);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xb4 , 0x20);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xb5 , 0x3a);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xb6 , 0x5e);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xb9 , 0x40);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xba , 0x4f);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbb , 0x47);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbc , 0x45);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbd , 0x43);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbe , 0x42);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbf , 0x42);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc0 , 0x42);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc1 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc2 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc3 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc4 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc5 , 0x78);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc6 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xca , 0x78);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xcb , 0x0c);  //AEdebug end

#if 0//caprure preview daylight 24M 50hz 20-8FPS maxgain:0x70	
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x00);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x05 , 0x0 );
	camsensor_SP0838_ycbcr_i2c_write_byte(0x06 , 0x0 );
	camsensor_SP0838_ycbcr_i2c_write_byte(0x09 , 0x1 );
	camsensor_SP0838_ycbcr_i2c_write_byte(0x0a , 0x76);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf0 , 0x62);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf1 , 0x0 );
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf2 , 0x5f);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xf5 , 0x78);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x01);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x00 , 0xb2);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x0f , 0x60);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x16 , 0x60);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x17 , 0xa2);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x18 , 0xaa);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x1b , 0x60);
	camsensor_SP0838_ycbcr_i2c_write_byte(0x1c , 0xaa);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xb4 , 0x20);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xb5 , 0x3a);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xb6 , 0x5e);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xb9 , 0x40);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xba , 0x4f);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbb , 0x47);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbc , 0x45);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbd , 0x43);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbe , 0x42);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xbf , 0x42);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc0 , 0x42);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc1 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc2 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc3 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc4 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc5 , 0x70);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xc6 , 0x41);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xca , 0x70);
	camsensor_SP0838_ycbcr_i2c_write_byte(0xcb , 0xc );
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x00);
#else
	for(i=0; i<sizeof(sp0838_preview_5fps)/sizeof(register_address_value_pair); i++)
	{
		camsensor_SP0838_ycbcr_i2c_write_byte(sp0838_preview_5fps[i].register_address, sp0838_preview_5fps[i].register_value);
	}
#endif
	camsensor_SP0838_ycbcr_i2c_write_byte(0xfd , 0x00);  //P0
	camsensor_SP0838_ycbcr_i2c_write_byte(0x32 , 0x15);  //Auto_mode set
	camsensor_SP0838_ycbcr_i2c_write_byte(0x34 , 0x66);  //Isp_mode set
	camsensor_SP0838_ycbcr_i2c_write_byte(0x35 , 0x00);  //out format
	return  TRUE;
} /* camsensor_ov7690_write_sensor */




/*===========================================================================

FUNCTION      CAMSENSOR_SP0838_YCBCR_INIT

DESCRIPTION
              Initialize the camera sensor;

DEPENDENCIES
  None

RETURN VALUE
  if successful
    TRUE
  else
    FALSE

SIDE EFFECTS
  None

===========================================================================*/

boolean camsensor_sp0838_init(camsensor_function_table_type *camsensor_function_table_ptr, camctrl_tbl_type *camctrl_tbl_ptr)
{
    uint8  manufacturer_id_msb;

	MSG_FATAL("camsensor_SP0838_ycbcr_init()\n",0,0,0);///yty 


	camsensor_camclk_po_hz =24000000;

	CAMERA_CONFIG_GPIO(CAMSENSOR_SP0838_RESET_PIN);
    
    gpio_out(CAMSENSOR_SP0838_RESET_PIN,1);
    clk_busy_wait(2*1000);
    gpio_out(CAMSENSOR_SP0838_RESET_PIN,0);
    clk_busy_wait(5*1000);
    gpio_out(CAMSENSOR_SP0838_RESET_PIN,1);
    clk_busy_wait(1*1000);

	camsensor_preview_resolution  = CAMSENSOR_QTR_SIZE;
	camsensor_snapshot_resolution = CAMSENSOR_FULL_SIZE;


	/* 1. Initiate I2C:  */
	camsensor_i2c_command.bus_id     = I2C_BUS_HW_CTRL;
	camsensor_i2c_command.slave_addr = CAMSENSOR_SP0838_I2C_SLAVE_ID;
	camsensor_i2c_command.options    = (i2c_options_type) (I2C_REG_DEV | I2C_START_BEFORE_READ); 


	/* 3. Read sensor manufacturer ID MSB:       */

	camsensor_SP0838_ycbcr_i2c_write_byte(0xFD,0x00);

	if (camsensor_SP0838_ycbcr_i2c_read_byte(0x02, &manufacturer_id_msb) == FALSE) ///0x92
	{
		MSG_FATAL("camsensor_SP0838_read REG0x2 fail %x ",manufacturer_id_msb,0,0);///yty 
		return FALSE;
	}

	if ( manufacturer_id_msb != CAMSENSOR_SP0838_ID )
	{
		MSG_FATAL("manufacturer_id_msb:0x%x,manufacturer_id_lsb:0x%x",manufacturer_id_msb,0,0);///yty 
		return FALSE;
	} 

	clk_busy_wait(10*1000);
	//camsensor_sp0838_sensor_init();
	/* Initialize the sensor_function_table */
	camsensor_sp0838_ycbcr_register (camsensor_function_table_ptr);

	camsensor_sp0838_0m3_setup_camctrl_tbl(camctrl_tbl_ptr);

	return TRUE;
} /* camsensor_SP0838_ycbcr_init */

void camsensor_sp0838_0m3_init_camsensor_info()
{    
	MSG_FATAL("    camsensor_sp0838_0m3_init_camsensor_info     ",0,0,0);
	camsensor_info.pclk_invert	=FALSE;
	camsensor_info.preview_dummy_pixels = 0;

	camsensor_camclk_po_hz	=24000000;

	switch (camsensor_info.msm_id)
	{
		case MSM6246:
		case MSM6260:
		case QSC6270:
			camsensor_info.pclk_invert = FALSE;
			break;

		case MSM6280:
		case MSM6290:
			camsensor_info.pclk_invert = FALSE;
			break;

		default:
			break;  
	}
}/* camsensor_sp0838_0m3_init_camsensor_info */
/*===========================================================================

FUNCTION      CAMSENSOR_SP0838_YCBCR_START

DESCRIPTION
              Initialize the camsensor parameters.

DEPENDENCIES
  None

RETURN VALUE
  if successful
    TRUE
  else
    FALSE

SIDE EFFECTS
  None

===========================================================================*/
boolean camsensor_sp0838_ycbcr_start( camsensor_static_params_type *camsensor_params)
{

	MSG_FATAL("camsensor_SP0838_ycbcr_start begin ", 0,0,0);///yty 
	camsensor_sp0838_0m3_init_camsensor_info();
	
	/* Initialize CAMIF operation mode */
	camsensor_params->camif_config.SyncMode         = CAMIF_APS;
	camsensor_params->camif_config.HSyncEdge        = CAMIF_High;
	camsensor_params->camif_config.VSyncEdge        = CAMIF_High;
	camsensor_params->camif_efs_config.EFS_EOL      = 0x0000;
	camsensor_params->camif_efs_config.EFS_SOL      = 0x0000;
	camsensor_params->camif_efs_config.EFS_EOF      = 0x0000;
	camsensor_params->camif_efs_config.EFS_SOF      = 0x0000;

	/* ------------------  Sensor-specific Config -------------- */
	/* Make/model of sensor */
	camsensor_params->sensor_model = CAMSENSOR_ID_SP0838;

	/* CCD or CMOS */
	camsensor_params->sensor_type = CAMSENSOR_CMOS;

	camsensor_params->format = CAMIF_YCbCr_Cb_Y_Cr_Y;
	/* BAYER or YCbCr */
	camsensor_params->output_format = CAMSENSOR_YCBCR;

	/* A pointer to the sensor name for EXIF tags                */
	camsensor_params->sensor_name = camsensor_sp0838_ycbcr_sensor_name;

	/* What is the maximum FPS that can be supported by this sensor
	 in video mode */
	camsensor_params->max_video_fps = 15 * Q8;

	/* Application assigned FPS in video mode */
	camsensor_params->video_fps = 15 * Q8;

	/* Snapshot mode operation */
	camsensor_params->max_preview_fps = 15 * Q8;
	camsensor_params->nightshot_fps = 15 * Q8;

	/* May be assigned with max_preview_fps or nightshot_fps. */
	camsensor_params->preview_fps = 15 * Q8;

	/* PCLK Invert */
	// camsensor_params->pclk_invert = TRUE;

	/* VFE's perception of Sensor output capability */

	/* Snapshot dimensions */
	camsensor_params->full_size_width  = OV_SP0838_YCBCR_FULL_SIZE_WIDTH;
	camsensor_params->full_size_height = OV_SP0838_YCBCR_FULL_SIZE_HEIGHT;

	/* Preview dimensions */
	camsensor_params->qtr_size_width = OV_SP0838_YCBCR_QTR_SIZE_WIDTH;
	camsensor_params->qtr_size_height = OV_SP0838_YCBCR_QTR_SIZE_HEIGHT;

	switch (camsensor_preview_resolution)
	{
		case CAMSENSOR_QTR_SIZE:
			camsensor_params->preview_dx_decimation = camsensor_params->full_size_width * Q12 / camsensor_params->qtr_size_width;
			camsensor_params->preview_dy_decimation = camsensor_params->full_size_height * Q12 / camsensor_params->qtr_size_height;

			/* Set the current dimensions */
			camsensor_params->camsensor_width  = camsensor_params->qtr_size_width;
			camsensor_params->camsensor_height = camsensor_params->qtr_size_height;
			break;

		case CAMSENSOR_FULL_SIZE:
			camsensor_params->preview_dx_decimation = Q12;
			camsensor_params->preview_dy_decimation = Q12;

			/* Set the current dimensions */
			camsensor_params->camsensor_width  = camsensor_params->full_size_width;
			camsensor_params->camsensor_height = camsensor_params->full_size_height;
			break;

		default:
			camsensor_params->preview_dx_decimation = Q12;
			camsensor_params->preview_dy_decimation = Q12;
			break;
	}

	/* This tells camera service the minimum decimation that is supported
	* by the sensor. 
	* Ex: if preview is in quarter size mode, then there is a 
	* sensor decimation of 2, so the minimum is 2 
	*/
	camsensor_params->preview_dx_decimation = (camsensor_params->preview_dx_decimation < Q12) ? Q12 : camsensor_params->preview_dx_decimation;
	camsensor_params->preview_dy_decimation = (camsensor_params->preview_dy_decimation < Q12) ? Q12 : camsensor_params->preview_dy_decimation;

	/* ------------  Auto Exposure Control Config -------------- */
	camsensor_params->aec_enable = FALSE;

	/* ------------  Auto White Balance Config ----------------- */
	/* AWB -Auto White Balance Parameters */
	camsensor_params->awb_enable = FALSE;

	/* ------------  Auto Focus Config ------------------------- */
	/* AF -Auto Focus Parameters */
	camsensor_params->af_enable = FALSE; //TRUE;

	camsensor_params->num_possible_frame_rates = 1;
	/* Define these frame rates */
	/* By convention, the highest frame rate will be first in the
	array (zeroth index) and the lowest last (in order). */
	camsensor_params->frame_rate_array[0].fps = (uint16) (15*256.0); /* Q8 */
	camsensor_params->frame_rate_array[0].use_in_auto_frame_rate = TRUE;

	camsensor_params->frame_rate_array[1].fps = (uint16) (15*256.0); /* Q8 */
	camsensor_params->frame_rate_array[1].use_in_auto_frame_rate = FALSE;

	camsensor_params->pclk_invert = camsensor_info.pclk_invert;
	/* ------------  Default Misc Parmas Config ---------------- */
	/* Does the sensor need/use a flash  */
#if defined FEATURE_WHITE_LED_FLASH || defined FEATURE_STROBE_FLASH
	camsensor_params->support_auto_flash = FALSE;//TRUE;   ///lee test 1004
#else
	camsensor_params->support_auto_flash = FALSE;
#endif

	return TRUE;
} /* camsensor_SP0838_ycbcr_start */


/*===========================================================================

FUNCTION      CAMSENSOR_SP0838_YCBCR_SNAPSHOT_CONFIG

DESCRIPTION
              Configure the camera sensor and the VFE_CAMIF for
              snapshot mode

DEPENDENCIES
  None

RETURN VALUE
  TRUE if successful
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/

boolean camsensor_sp0838_ycbcr_snapshot_config
(
  camsensor_static_params_type *camsensor_params /* Other config params */
) 
{
	/* Sensor output data format */
	camsensor_params->format = CAMIF_YCbCr_Cb_Y_Cr_Y;

	/* Set the current dimensions */
	camsensor_params->camsensor_width = camsensor_params->full_size_width;
	camsensor_params->camsensor_height = camsensor_params->full_size_height;
	/* CAMIF frame */
	camsensor_params->camif_frame_config.pixelsPerLine = OV_SP0838_YCBCR_FULL_SIZE_WIDTH*2;
	camsensor_params->camif_frame_config.linesPerFrame = OV_SP0838_YCBCR_FULL_SIZE_HEIGHT;
	
	/* CAMIF window */
	camsensor_params->camif_window_width_config.firstPixel = 0;
	camsensor_params->camif_window_width_config.lastPixel  = camsensor_params->camsensor_width*2 - 1;
	camsensor_params->camif_window_height_config.firstLine = 0;
	camsensor_params->camif_window_height_config.lastLine = camsensor_params->camsensor_height - 1;
	
	camsensor_current_resolution = camsensor_snapshot_resolution;
	return TRUE;
} /* camsensor_SP0838_ycbcr_snapshot_config */

/*===========================================================================

FUNCTION      CAMSENSOR_SP0838_YCBCR_RAW_SNAPSHOT_CONFIG

DESCRIPTION
              Configure the camera sensor and the VFE_CAMIF for
              raw snapshot mode

DEPENDENCIES
  None

RETURN VALUE
  TRUE if successful
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/

boolean camsensor_sp0838_ycbcr_raw_snapshot_config
(
  camsensor_static_params_type *camsensor_params /* Other config params */
) 
{
	return TRUE;
} /* camsensor_SP0838_ycbcr_raw_snapshot_config */


/*===========================================================================

FUNCTION      CAMSENSOR_SP0838_YCBCR_VIDEO_CONFIG

DESCRIPTION
              Configure the camera sensor and the camera interface
              for Preview mode. Implements all 

DEPENDENCIES
  None

RETURN VALUE
  TRUE if successful
  FALSE otherwise

SIDE EFFECTS
  None

===========================================================================*/

boolean camsensor_sp0838_ycbcr_video_config
(
  camsensor_static_params_type *camsensor_params /* Other config params */
)
{
	camsensor_sp0838_sensor_init();
	
	/* Sensor output data format */
	camsensor_params->discardFirstFrame = TRUE;
	camsensor_params->format = CAMIF_YCbCr_Cb_Y_Cr_Y;

	switch ( camsensor_preview_resolution )
    {
    	case CAMSENSOR_QTR_SIZE:

			/* Set the current dimensions */
			camsensor_params->camsensor_width  = \
			            camsensor_params->qtr_size_width;
			camsensor_params->camsensor_height = \
			            camsensor_params->qtr_size_height;

			/* CAMIF frame */
			camsensor_params->camif_frame_config.pixelsPerLine = \
			            camsensor_params->qtr_size_width*2;

			camsensor_params->camif_frame_config.linesPerFrame = \
			            camsensor_params->qtr_size_height;

			/* CAMIF Window */
			camsensor_params->camif_window_width_config.firstPixel = \
			                 camsensor_info.preview_dummy_pixels*2;

			camsensor_params->camif_window_width_config.lastPixel  = \
			          (camsensor_info.preview_dummy_pixels) \
			      +(camsensor_params->qtr_size_width*2)-1;

			camsensor_params->camif_window_height_config.firstLine = 0;

			camsensor_params->camif_window_height_config.lastLine  = \
			            camsensor_params->qtr_size_height-1;
      		break;


		case CAMSENSOR_FULL_SIZE:
			/* Set the current dimensions */
			camsensor_params->camsensor_width  = \
			            camsensor_params->full_size_width;
			camsensor_params->camsensor_height = \
			            camsensor_params->full_size_height;

			/* CAMIF frame */
			camsensor_params->camif_frame_config.pixelsPerLine = \
			            camsensor_params->full_size_width*2;

			camsensor_params->camif_frame_config.linesPerFrame = \
			            camsensor_params->full_size_width;

			/* CAMIF window */
			camsensor_params->camif_window_width_config.firstPixel = 0;

			camsensor_params->camif_window_width_config.lastPixel  = \
			           (camsensor_params->full_size_width*2)-1;

			camsensor_params->camif_window_height_config.firstLine = 0;

			camsensor_params->camif_window_height_config.lastLine  = \
			           camsensor_params->full_size_height-1;

			break;

    	case CAMSENSOR_INVALID_SIZE:

       		return FALSE;

  	}

	camsensor_current_resolution = camsensor_preview_resolution;

	return TRUE;
} /* camsensor_SP0838_ycbcr_video_config */

/*===========================================================================

FUNCTION      CAMSENSOR_SP0838_YCBCR_POWER_UP

DESCRIPTION
              Power up the sensor.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void camsensor_sp0838_ycbcr_power_up(void)
{
	return;
}
/*===========================================================================

FUNCTION      camsensor_SP0838_ycbcr_POWER_DOWN

DESCRIPTION
              Power down the sensor.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
void camsensor_sp0838_ycbcr_power_down(void)
{
    MSG_FATAL ("camsensor_SP0838_ycbcr_power_down begin", 0,0,0);
	g_reflect = CAMERA_MAX_REFLECT;
	g_effect = CAMERA_EFFECT_MIN_MINUS_1;

}


/*===========================================================================

FUNCTION      camsensor_SP0838_ycbcr_i2c_read_byte

DESCRIPTION
              8-bit I2C read.

DEPENDENCIES
  None

RETURN VALUE
  TRUE - I2C read successful
  FALSE - I2C read failed


SIDE EFFECTS
  None

===========================================================================*/
static boolean camsensor_SP0838_ycbcr_i2c_read_byte(uint8 reg, uint8 *data) 
{
    static uint8 readbyte; 
	uint8  i;

	if (data == NULL)
	{
		return FALSE;
	}

	camsensor_i2c_command.addr.reg = reg;
	camsensor_i2c_command.buf_ptr  = (byte *)(&readbyte);
	camsensor_i2c_command.len      = 1;

	for (i =0; i < 3; ++i)
	{
		if (i2c_read(&camsensor_i2c_command) == I2C_SUCCESS)
		{
			*data  = readbyte;
			MSG_FATAL("camsensor_SP0838_ycbcr_i2c_read_byte: OK %x",reg,0,0);
			return TRUE;
		}
	}

	MSG_FATAL("camsensor_SP0838_ycbcr_i2c_read_byte: false %x",reg,0,0);
	return FALSE;
}

/*===========================================================================

FUNCTION      camsensor_SP0838_ycbcr_i2c_write_byte

DESCRIPTION
              8-bit I2C write.

DEPENDENCIES
  None

RETURN VALUE
  TRUE  - I2C write successful
  FALSE - I2C write failed


SIDE EFFECTS
  None

===========================================================================*/
static boolean camsensor_SP0838_ycbcr_i2c_write_byte(uint8 reg, uint8 data) 
{
    static uint8 writebyte;
	uint8 i;

	writebyte  = data;

	camsensor_i2c_command.addr.reg = reg;
	camsensor_i2c_command.buf_ptr  = (byte *)(&writebyte);
	camsensor_i2c_command.len      = 1;

	for (i = 0; i < 3; ++i)
	{
		if (i2c_write(&camsensor_i2c_command) == I2C_SUCCESS)
		{
			MSG_FATAL("camsensor_SP0838_ycbcr_i2c_read_byte: OK %x,%x",reg,data,0);
			return TRUE;
		}
	}
	MSG_FATAL("camsensor_SP0838_ycbcr_i2c_read_byte: false %x,%x",reg,data,0);
	return FALSE;
} /* camsensor_SP0838_ycbcr_i2c_write_byte */


/* The following are stub functions, (unsupported sensor functions) */
camera_ret_code_type camsensor_sp0838_ycbcr_write_exposure_gain(uint16 gain, uint32 line_count)
{
	return(CAMERA_SUCCESS);
} /* camsensor_SP0838_ycbcr_write_exposure_gain */

//wlr 1114 start
camera_ret_code_type camsensor_sp0838_set_effect(int8 effect) 
{
	camera_ret_code_type ret_val = CAMERA_SUCCESS;
	
	MSG_FATAL ("+++++ camsensor_SP0838_set_effect effect = %d",effect,0,0);

	if ( g_effect == effect )
	{
		return CAMERA_SUCCESS;
	}
	
	switch(effect)
	{
		case CAMERA_EFFECT_OFF://normal
			break;
			
		case CAMERA_EFFECT_MONO:
			break;
			
		case CAMERA_EFFECT_NEGATIVE://��Ƭ
			break;
			
		case CAMERA_EFFECT_SEPIA://�غ�ɫ
			break;

		default:
			ret_val = CAMERA_INVALID_PARM;
			break;
	}

	g_effect = effect;
  	return ret_val;
}/* camsensor_SP0838_set_effect */

camera_ret_code_type camsensor_sp0838_set_wb(int8 wb) 
{
  	camera_ret_code_type ret_val = CAMERA_SUCCESS;
	static int8 m_wb = CAMERA_WB_MIN_MINUS_1;
	
	if ( wb == m_wb )
	{
		return CAMERA_SUCCESS;
	}
	
	MSG_FATAL ("+++++ camsensor_SP0838_set_wb wb = %d",wb,0,0);

  	switch (wb)
  	{
		case  CAMERA_WB_AUTO://�Զ�
	  		break;

		case CAMERA_WB_CLOUDY_DAYLIGHT://����
	  		break;
	
		case CAMERA_WB_INCANDESCENT://�׳��
	  		break;

		case CAMERA_WB_FLUORESCENT: //ӫ��
			ret_val = CAMERA_SUCCESS;
	  		break;	
	  
		case CAMERA_WB_DAYLIGHT:  //�չ�
			break;

		default:
	  		ret_val = CAMERA_INVALID_PARM;
	  		break;
  	}

	m_wb = wb;
  	return ret_val;
}/* camsensor_SP0838_set_wb */
//wlr 1114 end

/*===========================================================================

FUNCTION	  CAMSENSOR_SP0838_YCBCR_2MP_SET_BRIGHTNESS

DESCRIPTION   
  Set brightness.

DEPENDENCIES
  None

INPUT
  positive or negative value to change birghtness.

RETURN VALUE
  CAMERA_SUCCESS - if we changed brightness successfully
  CAMERA_FAILED  - if brightness change failed

SIDE EFFECTS
  This will change the output brgihtness.

===========================================================================*/
camera_ret_code_type camsensor_sp0838_set_brightness (int8 value)
{
  	return CAMERA_SUCCESS;
} /* camsensor_ov2640_set_brightness */

/*===========================================================================

FUNCTION	  CAMSENSOR_SP0838_YCBCR_2MP_SET_CONTRAST

DESCRIPTION   
  Set contrast.

DEPENDENCIES
  None

INPUT
  positive or negative value to change contrast.

RETURN VALUE
  CAMERA_SUCCESS - if we changed contrast successfully
  CAMERA_FAILED  - if contrast change failed

SIDE EFFECTS
  This will change the output contrast.

===========================================================================*/
camera_ret_code_type camsensor_sp0838_set_contrast (int8 contrast)
{
  	return CAMERA_SUCCESS;
} /* camsensor_ov2640_set_contrast */

camera_ret_code_type camsensor_sp0838_set_nightmode (camera_nightshot_mode_type mode)
{
	return CAMERA_SUCCESS;
}


LOCAL camera_ret_code_type camsensor_sp0838_set_frame_rate(uint16 fps)
{
  	return CAMERA_SUCCESS;
} /* camsensor_ov9650_set_frame_rate */

camera_ret_code_type camsensor_sp0838_set_antibanding(int8 antibanding)
{
	if ( g_iBanding == antibanding )
	{
		return CAMERA_SUCCESS;
	}
	
	g_iBanding = antibanding;
	
	switch ((camera_antibanding_type)antibanding)
	{
		case CAMERA_ANTIBANDING_OFF:
			break;

		case CAMERA_ANTIBANDING_60HZ:
			break;

		case CAMERA_ANTIBANDING_50HZ:
			break;

		default:
			return CAMERA_NOT_SUPPORTED;
	}
	
	return CAMERA_SUCCESS;
}

static uint16 camsensor_sp0838_0m3_get_snapshot_fps(uint16 fps)
{
	/* In the current driver, the snapshot fps is always 11 */
	fps = 15 * Q8;

	return fps;
}

static void    camsensor_sp0838_0m3_setup_camctrl_tbl(camctrl_tbl_type *camctrl_tbl_ptr)
{
	camctrl_tbl_ptr->asf_5x5_is_supported         = FALSE; 
	camctrl_tbl_ptr->la_is_supported              = FALSE;
	camctrl_tbl_ptr->cs_is_supported              = FALSE;
	camctrl_tbl_ptr->iso_is_supported             = FALSE;
	camctrl_tbl_ptr->enable_rolloff_correction    = FALSE;
	camctrl_tbl_ptr->hjr_bayer_filtering_enable   = FALSE;
	camctrl_tbl_ptr->nightshot_is_supported       = FALSE;

	camctrl_tbl_ptr->get_snapshot_frame_per_sec = camsensor_sp0838_0m3_get_snapshot_fps;
}
/*===========================================================================

FUNCTION   CAMSENSOR_SP0838_YCBCR_REGISTER 

DESCRIPTION  
  Exports the sensor specific functions to CAMSENSOR

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
static void camsensor_sp0838_ycbcr_register (camsensor_function_table_type *camsensor_function_table_ptr)
{
	camsensor_function_table_ptr->camsensor_start               	= camsensor_sp0838_ycbcr_start;
	camsensor_function_table_ptr->camsensor_video_config        	= camsensor_sp0838_ycbcr_video_config;
	camsensor_function_table_ptr->camsensor_raw_snapshot_config 	= camsensor_sp0838_ycbcr_raw_snapshot_config;
	camsensor_function_table_ptr->camsensor_snapshot_config     	= camsensor_sp0838_ycbcr_snapshot_config;
	camsensor_function_table_ptr->camsensor_power_up            	= camsensor_sp0838_ycbcr_power_up;
	camsensor_function_table_ptr->camsensor_power_down          	= camsensor_sp0838_ycbcr_power_down;
	camsensor_function_table_ptr->camsensor_write_exposure_gain 	= camsensor_sp0838_ycbcr_write_exposure_gain;
	camsensor_function_table_ptr->camsensor_set_frame_rate        	= camsensor_sp0838_set_frame_rate;///lee test
	camsensor_function_table_ptr->camsensor_set_effect				= camsensor_sp0838_set_effect; //wlr1114	 
	camsensor_function_table_ptr->camsensor_set_wb					= camsensor_sp0838_set_wb;//wlr 1114
	camsensor_function_table_ptr->camsensor_set_brightness 			= camsensor_sp0838_set_brightness; //wlr1114	 
	camsensor_function_table_ptr->camsensor_set_contrast 			= camsensor_sp0838_set_contrast;//wlr 1114
	camsensor_function_table_ptr->camsensor_set_antibanding 		= camsensor_sp0838_set_antibanding; 
} /* camsensor_SP0838_ycbcr_register */
#endif /* FEATURE_CAMERA */
