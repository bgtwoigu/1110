#ifndef __G_SENSOR_H_
#define __G_SENSOR_H_
/*===========================================================================
        
              G-Sensor Driver Public Header

DESCRIPTION
  All the declarations and definitions necessary to support interaction
  with the FM Radio functions for the MSM60x0.

REFERENCES

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

===========================================================================*/

#include "comdef.h"

#define HS_GSENSOR_ENABLE_DELAY 20
#define HS_PEDOMETER_TIME   20
#define HS_SHAKE_TIME       40
#define HS_NAVIGATION_TIME  500


//#define     g_sensor_log_enable

typedef enum
{      
    G_SENSOR_NONE               = 0,
    G_SENSOR_ENABLE             = 0x0001, 
    G_SENSOR_DISABLE            = 0x0002,
    G_SENSOR_PEDOMETER          = 0x0004, 
    G_SENSOR_PEDOMETER_PAUSE    = 0x0008,
    G_SENSOR_PEDOMETER_DISABLE  = 0X0010,   
    G_SENSOR_SHAKE              = 0x0020,
    G_SENSOR_SHAKE_DISABLE      = 0x0040,    
    G_SENSOR_VIDEO_CIRC         = 0x0080,
    G_SENSOR_VIDEO_CIRC_DISABLE = 0x0100,
    G_SENSOR_NAVIGATION_ENABLE  = 0x0200, 
    G_SENSOR_NAVIGATION         = 0x0400,
    G_SENSOR_NAVIGATION_DISABLE = G_SENSOR_DISABLE
    
}   g_sensor_cmd_type;

typedef enum
{   
    MMI_GSENSOR_SHAKE_OPEN,
    MMI_GSENSOR_SHAKE_CLOSE,
    MMI_GSENSOR_SHAKE_OPEN_IN_IDLE,
    MMI_GSENSOR_SHAKE_CLOSE_IN_IDLE,
    
}   mmi_g_sensor_state_type;

extern g_sensor_cmd_type g_sensor_cmd;
extern mmi_g_sensor_state_type mmi_g_sensor_state;

/*==============================================================================
����: 
     mmi_g_sensor_process
             
˵��: 
     g_sensor for MMI�ӿں���
       
����: 
     g_sensor �����¼�����, ����ҪENABLE�¼�����������һ����,��ʹ�����·�ʽ,
     ��:mmi_g_sensor_process(G_SENSOR_ENABLE | G_SENSOR_PEDOMETER)
              
����ֵ:��
       
��ע:
       Ӧ����ɺ���g_sensor_init()
==============================================================================*/
extern void mmi_g_sensor_process(g_sensor_cmd_type);


/*==============================================================================
����: 
     g_sensor_init
             
˵��: 
     ��ʼ��G_sensor
       
����: 
              
����ֵ:��
       
��ע:
       Ӧ�ȵ���g_sensor_device_init()
==============================================================================*/
boolean g_sensor_init(void);

/*==============================================================================
����: 
     g_sensor_enable
             
˵��: 
     ȷ��G_sensor�Ѿ����ڹ���״̬
       
����: 
              
����ֵ: ��
       
��ע:
      Enable��ʱ��ֻ�Ǹı��־,����ʵ�ʴ������豸
==============================================================================*/
void g_sensor_enable(void);

/*==============================================================================
����: 
     g_sensor_disable
             
˵��: 
     ȷ��G_sensor�Ѿ����ڷǹ���״̬
       
����: 
              
����ֵ: ��
       
��ע:
     Disableʱ�ı��־,ͬʱ�ر������豸    
==============================================================================*/
void g_sensor_disable(void);

extern void g_sensor_pedometer(void);

extern void g_sensor_pedometer_pause(void);

extern void g_sensor_shake(void);

extern void g_sensor_video_circ(void);

extern int ReadPedoCounter(void);

extern void ClrPedoCounter(void);

extern void ClrShakeSum(void);

extern void ClrVideoCircSum(void);

extern void g_sensor_nav_get_std(void);

extern void g_sensor_navigation(void);
#endif //__G_SENSOR_H_