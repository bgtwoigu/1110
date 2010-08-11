#ifndef __G_SENSOR_I_H_
#define __G_SENSOR_I_H_
/*===========================================================================
        
              G-Sensor Driver Internal Header

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
#include "customer.h"
#include "gpio_60x0.h"
#include "bio.h"
#include "i2c.h"
#include "clk.h"
#include "msg.h"

typedef enum
{
    G_SENSOR_NOT_INIT,
    G_SENSOR_POWER_DOWN,
    G_SENSOR_POWER_UP,
    G_SENSOR_IDLE_STATUS = G_SENSOR_POWER_UP,
    
    G_SENSOR_MAX_STATUS = 0xFFFF
}   g_sensor_status_type;

typedef struct
{
    dword       dwStaticValue;      //ˮƽ��ֹʱ�������
    dword       dwOffestPerG;       //ÿ��G���ٶȵ�ƫ�ơ�
} g_sensor_device_info_type;

//��¼G_SENSOR��ǰ��������״̬
extern g_sensor_status_type g_sensor_work_status;

#define G_SENSOR_SUCCESSFUL     0
#define G_SENSOR_FAILED         1

#define delay(t)              clk_busy_wait((t)*1000)

#define G_SENSOR_CMD_TIMER      10

int g_sensor_device_init(void);

int g_sensor_power_up(void);

int g_sensor_power_down(void);


/*==============================================================================
����: 
     g_sensor_get_device_info
             
˵��: 
     ��ȡG_sensorоƬ�Ļ���������Ϣ
       
����: 
     pDeviceInfo:ָ��GSENSORоƬ�������ݵ�ָ��
              
����ֵ: GSENSORоƬ�Ļ�������
       
��ע:
       
==============================================================================*/
void g_sensor_get_device_info(g_sensor_device_info_type *pDeviceInfo);

/*==============================================================================
����: 
      g_sensor_get_acceleration
       
˵��: 
      ��ȡG_SENSOR��X��Y�����ϵ�ԭʼ���
       
����:       
      x_poiner:  ָ��X��ԭʼ�����ָ��
      y_pointer: ָ��Y��ԭʼ�����ָ��
        
����ֵ:
        G_SENSOR_FAILED��0
        G_SENSOR_SUCCESS��1
       
��ע:
      ��ǰ����״̬ӦΪG_SENSOR_POWER_UP
       
==============================================================================*/
int g_sensor_get_acceleration(int16 *x_pointer, int16 *y_pointer);

#endif //__G_SENSOR_I_H_