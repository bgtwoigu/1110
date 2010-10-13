/*===========================================================================

        G-Sensor Driver for Common Service

DESCRIPTION
  All the declarations and definitions necessary to support interaction
  with the G-Sensor functions for the MSM60x0.

REFERENCES

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

===========================================================================*/
#include "g_sensor.h"
#include "g_sensor_i.h"
#include "hs.h"
#include "keypad.h"

/*--------------------------------------------------------------------------------
              
              �����豸��ȡ��ֵ����Ƭ��λ���й�ϵ����Ҫ������׼
              
--------------------------------------------------------------------------------*/
//#define FEATURE_POSITION_A           //�������߼�λ����ͬ
#define FEATURE_POSITION_B           //оƬ��λ��˳ʱ����ת90��
//#define FEATURE_POSITION_C           //оƬ��λ��˳ʱ����ת180��
//#define FEATURE_POSITION_D             //оƬ��λ��˳ʱ����ת270��

#define GSENSOR_SAMPLE_MIN  0x450
#define GSENSOR_SAMPLE_MAX  0xBB0  
#define MAN_STEPS       0x0050 
#define SAMPLE_TIME		0x000001
#define HPF_RC			0x00000F
#define LPF_RC          0x000001

//shake
#define DATA_SUM                10
#define SHAKE_COMP_NUM          (DATA_SUM-6)
#define GS_OUTPUT_CRT_LEFT      0x02A0
#define GS_OUTPUT_CRT_RIGHT     0x0240 
#define SHAKE_OFFSET            0xB0
#define SHAKE_COMP_OFFSET       0X0E0
#define SHAKE_DELAY_TIME_SUM    700    //700ms


#define SHAKE_UNLOCK        0
#define SHAKE_LOCK          1

#define SHAKE_DIR_MIDDLE    0
#define SHAKE_DIR_LEFT      1
#define SHAKE_DIR_RIGHT     2

#define SHAKE_WAIT_TICK     5

#define GS_CRT_DATA     0x0095

#define NAV_OFFSET      0x10

//��¼G_SENSOR��ǰ��������״̬
g_sensor_status_type g_sensor_work_status = G_SENSOR_NOT_INIT;

//G_SENSOR��ǰ����Ƶ״̬
static hs_key_type g_sensor_vedio_direction = HS_GSENSOR_STAND_K;

static g_sensor_device_info_type g_sensor_device_info = {0};

//�Ʋ���
static word PedoCounter = 0;
static int16 g_pedometer_delay_sum = 0;

static unsigned char bUp = 0, bDown = 0;
static unsigned char bStatus_axis_bc = 0;
static int	stc_iAcce1_LPFcount1,stc_iAcce0_LPFcount1;
static int	stc_iAcce1_HPFcount1,stc_iAcce0_HPFcount1;
static int	stc_iAcceFilter1;
static int 	stc_iG_CountUP,stc_iG_CountDOWN;

//Shake
static int16 g_sensor_x_acc[DATA_SUM] = {0};
static int16 g_sensor_y_acc[DATA_SUM] = {0};
static int16 g_shake_delay_sum = 0;
static int shake_sum = 0;  
static unsigned char shake_lock = SHAKE_UNLOCK;
static unsigned char shake_dir_bc = SHAKE_DIR_MIDDLE;
static unsigned char shake_dir_once = SHAKE_DIR_MIDDLE;
static int16 shake_offset_once = 0;
static int16 wait_more_value_shake = 0;

//video circumrotate
static int video_circ_sum=0; 

//Navigation
static int16 x_acc_nav_std,y_acc_nav_std;


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
boolean g_sensor_init()
{
    int16 g_sensor_cmd_timer;
    gpio_i2c_config();  //set i2c mode
    for(g_sensor_cmd_timer=0; g_sensor_cmd_timer<G_SENSOR_CMD_TIMER; g_sensor_cmd_timer++)
    {
#ifdef g_sensor_log_enable    
        MSG_ERROR("g_sensor_device_init timer = %d", g_sensor_cmd_timer, 0, 0); 
#endif    
        if(g_sensor_device_init() == G_SENSOR_SUCCESSFUL)
        {
            break;
        }   
        rex_sleep(5);
    }
    if(G_SENSOR_CMD_TIMER == g_sensor_cmd_timer)
    {
#ifdef g_sensor_log_enable    
        MSG_ERROR("g_sensor_init error", 0, 0, 0); 
#endif
        return FALSE;
    }
    
    g_sensor_get_device_info(&g_sensor_device_info);    

    return TRUE;
}

/*==============================================================================
����: 
     g_sensor_enable
             
˵��: 
     ȷ��G_sensor�Ѿ����ڹ���״̬
       
����: 
              
����ֵ: ��
       
��ע: 
      Enable��ʱ��ֻ�Ǹı��־,����ʵ�ʴ������豸��
      APP Call It to Enable g_sensor Working.
==============================================================================*/
void g_sensor_enable()
{
    int16 g_sensor_cmd_timer;

    if(G_SENSOR_POWER_DOWN < g_sensor_work_status) 
    {
        return;
    }
    
    if(G_SENSOR_NOT_INIT == g_sensor_work_status)
    {    
        g_sensor_init();        
    }

    ASSERT(G_SENSOR_NOT_INIT != g_sensor_work_status)

    for(g_sensor_cmd_timer=0; g_sensor_cmd_timer<G_SENSOR_CMD_TIMER; g_sensor_cmd_timer++)
    {
#ifdef g_sensor_log_enable    
          MSG_ERROR("g_sensor_power_up timer = %d", g_sensor_cmd_timer, 0, 0); 
#endif    
        if(g_sensor_power_up() == G_SENSOR_SUCCESSFUL)
        {
            break;
        }
        rex_sleep(5);
    }
    if(G_SENSOR_CMD_TIMER == g_sensor_cmd_timer)
    {
#ifdef g_sensor_log_enable    
        MSG_ERROR("g_sensor_power_up error", 0, 0, 0); 
#endif
    }
}



/*==============================================================================
����: 
     g_sensor_disable
             
˵��: 
     ͣ��G_sensor
       
����: 
              
����ֵ:
       
��ע: 
      Disableʱ�ı��־,ͬʱ�ر������豸��
      APP Call It to Disable g_sensor Working.
==============================================================================*/
void g_sensor_disable()
{
    int16 g_sensor_cmd_timer;
    
    if(G_SENSOR_POWER_DOWN >= g_sensor_work_status) 
    {
        return;
    }
    
    for(g_sensor_cmd_timer=0; g_sensor_cmd_timer<G_SENSOR_CMD_TIMER; g_sensor_cmd_timer++)
    {
#ifdef g_sensor_log_enable    
          MSG_ERROR("g_sensor_power_down timer = %d", g_sensor_cmd_timer, 0, 0); 
#endif    
        if(g_sensor_power_down() == G_SENSOR_SUCCESSFUL)
        {
            break;
        }
        rex_sleep(5);    
    }
    if(G_SENSOR_CMD_TIMER == g_sensor_cmd_timer)
    {
#ifdef g_sensor_log_enable    
        MSG_ERROR("g_sensor_power_down error", 0, 0, 0); 
#endif
    }

}


//APP Call It to register/de-register service
/*==============================================================================
����: 
      g_sensor_get_logic_acc
             
˵��: 
      ȡ������
       
����: 
      x_sample_poiner:   ָ��X�������ָ��
      y_sample_pointer:  ָ��Y�������ָ��
              
����ֵ:BOOL
       
��ע:
      
==============================================================================*/
boolean g_sensor_get_logic_acc(int16 *x_sample_pointer, int16 *y_sample_pointer)
{
    int16   g_sensor_x_acc1; 
    int16   g_sensor_y_acc1;
        
    //��ȡ����������ݡ�
    if(g_sensor_get_acceleration(&g_sensor_x_acc1, &g_sensor_y_acc1) == G_SENSOR_FAILED)
    {
        return FALSE;
    }
        
    //ԭʼ���ݼ�ȥ��ֹ״̬��BASEֵ
    g_sensor_x_acc1 -= g_sensor_device_info.dwStaticValue;
    g_sensor_y_acc1 -= g_sensor_device_info.dwStaticValue;
    
    //������Ƭ��λ�ã�����Ϊ�߼����
    #ifdef FEATURE_POSITION_A
        *x_sample_pointer = g_sensor_x_acc1;
        *y_sample_pointer = g_sensor_y_acc1;  
    #elif defined FEATURE_POSITION_B
        *x_sample_pointer = g_sensor_y_acc1;
        *y_sample_pointer = -g_sensor_x_acc1;    
    #elif defined  FEATURE_POSITION_C
        *x_sample_pointer = -g_sensor_x_acc1;
        *y_sample_pointer = -g_sensor_y_acc1;   
    #elif defined FEATURE_POSITION_D
        *x_sample_pointer = -g_sensor_y_acc1;
        *y_sample_pointer = g_sensor_x_acc1;    
    #endif
    
    *x_sample_pointer += g_sensor_device_info.dwStaticValue;
    *y_sample_pointer += g_sensor_device_info.dwStaticValue;

    return TRUE;
}


/*==============================================================================
����: 
       g_sensor_get_ave
             
˵��: 
       �õ���ȡ��ʮ�����ݵ�ƽ��ֵ
       
����: 
     x_sample_poiner:   ָ��X�������ָ��
     y_sample_pointer:  ָ��Y�������ָ��
     x_ave_pointer:     ָ��X�����ƽ��ֵ��ָ��
     y_ave_pointer:     ָ��Y�����ƽ��ֵ��ָ��
              
����ֵ:��
       
��ע:
      
==============================================================================*/
void g_sensor_get_ave(int16 *x_sample_poiner, int16 *y_sample_pointer, int16 *x_ave_pointer, int16 *y_ave_pointer)
{
    int i,x_sample_data = 0, y_sample_data = 0;
    
    for(i=0; i<10; i++)
    {
        x_sample_data += (int)*x_sample_poiner++;
        y_sample_data += (int)*y_sample_pointer++;   
    }
    
    *x_ave_pointer = (int16)x_sample_data/10;
    *y_ave_pointer = (int16)y_sample_data/10;
}

/*==============================================================================
����: 
     g_sensor_shake
             
˵��: 
     �����¼���ͼƬ������
       
����: 
              
����ֵ: ��
       
��ע:�ֻ�����ͨ����ǰ�����Ӷ������õ����ֲ�ͬ������
      
==============================================================================*/
void g_sensor_shake()
{ 
    int16 x_shake_acc_temp[DATA_SUM] = {0};
    int16 temp;
    int i,j, mid_num = 0,add_num = 0, reduce_num = 0;
    int16 x_acc_temp;
    int16 y_acc_temp;
    int16 shake_offset_temp = 0;

    if(G_SENSOR_NOT_INIT == g_sensor_work_status)
    {    
        g_sensor_init();        
        return;
    }
   
    //��������豸û�д�,��ѯ�ĵ�һ���ȴ������豸
    if(G_SENSOR_POWER_DOWN == g_sensor_work_status)
    {
        g_sensor_power_up();
#ifdef g_sensor_log_enable           
        MSG_ERROR("g_sensor_power_up in g_sensor_pedometer", 0, 0, 0);
#endif
        return;
    }

    // just to sample data when SHAKE has been starting. 
    if(DATA_SUM > shake_sum)
    {
        if(FALSE == g_sensor_get_logic_acc(&g_sensor_x_acc[shake_sum], &g_sensor_y_acc[shake_sum]))//ȡ������   ����X��ֵ
        {
            return;            
        }
#ifdef g_sensor_log_enable           
        MSG_ERROR("gsensor shake_sum is %d", shake_sum, 0, 0); 
#endif
        shake_sum++;
        return;
    }

    //sampling in normal state
    if(FALSE == g_sensor_get_logic_acc(&x_acc_temp, &y_acc_temp))
    {
        return;
    }

    //shift data, X_data is used.
    for(i=0; i<DATA_SUM; i++)
    {   
        if((DATA_SUM-1) == i)
        {
            g_sensor_x_acc[i] = x_shake_acc_temp[i] = x_acc_temp;             
        }
        else
        {
            g_sensor_x_acc[i] = x_shake_acc_temp[i] = g_sensor_x_acc[i+1]; 
        }
    }

#ifdef g_sensor_log_enable      
    MSG_ERROR("g_sensor_x_acc[DATA_SUM-1] is 0x%X", g_sensor_x_acc[DATA_SUM-1], 0, 0);
#endif

    //try to unlock shake if it has locked.
    if((SHAKE_DIR_LEFT == shake_dir_bc) && (SHAKE_LOCK == shake_lock))
    {
        for(i=(DATA_SUM*2/3); i<DATA_SUM; i++)
        {
            if((g_sensor_device_info.dwStaticValue - SHAKE_OFFSET) < g_sensor_x_acc[i])
            {
                mid_num++;
            }
        }
        
        if((DATA_SUM/5) <= mid_num)
        {
            shake_lock = SHAKE_UNLOCK;
        }
    }
    else if((SHAKE_DIR_RIGHT == shake_dir_bc) && (SHAKE_LOCK == shake_lock))
    {
        for(i=(DATA_SUM*2/3); i<DATA_SUM; i++)
        {
            if((g_sensor_x_acc[i] < (g_sensor_device_info.dwStaticValue + SHAKE_OFFSET)))
            {
                mid_num++;
            }
        }
        if((DATA_SUM/5) <= mid_num)
        {
            shake_lock = SHAKE_UNLOCK;
        }
    }
    
    // return if open shake delay, but sampling data is still must!
    if(0 != g_shake_delay_sum)
    {
        g_shake_delay_sum--;
        return;
    }

/***** shake calculation start as below. *****/
    
    //data arrange, the max data is in data[SUM-1].
    for(i=(DATA_SUM-1); i>0; i--)
    {
        for(j=0; j<i; j++)
        {
            if(x_shake_acc_temp[j] > x_shake_acc_temp[j+1])
            {
                temp = x_shake_acc_temp[j];
                x_shake_acc_temp[j] = x_shake_acc_temp[j+1];
                x_shake_acc_temp[j+1] = temp;
            }
        }
    }

    //calculate the direction of sampling array.
    for(i=0; i<SHAKE_COMP_NUM; i++)
    {
        if(g_sensor_x_acc[i] > (g_sensor_x_acc[i+(DATA_SUM-SHAKE_COMP_NUM)] + SHAKE_COMP_OFFSET))
        {
            reduce_num++;
        }
        else if(g_sensor_x_acc[i] < (g_sensor_x_acc[i+(DATA_SUM-SHAKE_COMP_NUM)] - SHAKE_COMP_OFFSET))
        {
            add_num++;
        }
    }
            
#ifdef g_sensor_log_enable      
    MSG_ERROR("g_sensor_shake reduce_num is %d,add_num is %d", reduce_num, add_num, 0);  
    MSG_ERROR("g_sensor_shake shake_lock is %d", shake_lock, 0, 0);
#endif

    //if shake is happen ?
    //direct to left.
    if((((SHAKE_DIR_MIDDLE == shake_dir_once) ? SHAKE_COMP_NUM : (SHAKE_COMP_NUM-1)) <= reduce_num) 
        && (SHAKE_UNLOCK == shake_lock)) 
    {
        if(((g_sensor_device_info.dwStaticValue - x_shake_acc_temp[1]) > GS_OUTPUT_CRT_LEFT)
            && (g_sensor_device_info.dwStaticValue > x_shake_acc_temp[1]))
        {
            if(SHAKE_DIR_LEFT != shake_dir_once)
            {    
                shake_dir_once = SHAKE_DIR_LEFT;
                
                //backup the offset value.
                shake_offset_once = g_sensor_device_info.dwStaticValue - x_shake_acc_temp[1];
                shake_offset_once = shake_offset_once - GS_OUTPUT_CRT_LEFT;
                
                // open shake delay timer for waiting whether that shake of more value is detected.
                wait_more_value_shake = SHAKE_WAIT_TICK + 1; 
            }
            else
            {
                shake_offset_temp = g_sensor_device_info.dwStaticValue - x_shake_acc_temp[1];
                shake_offset_temp = shake_offset_temp - GS_OUTPUT_CRT_LEFT;
                if(shake_offset_temp >= shake_offset_once)
                {                   
                    //backup the offset value.
                    shake_offset_once = g_sensor_device_info.dwStaticValue - x_shake_acc_temp[1];
                    shake_offset_once = shake_offset_once - GS_OUTPUT_CRT_LEFT;
                    
                    // open shake delay timer for waiting whether that shake of more value is detected.
                    wait_more_value_shake = SHAKE_WAIT_TICK + 1;  
                }
            }
#ifdef g_sensor_log_enable
                for(i=0; i<DATA_SUM; i++)
                {   
                    MSG_ERROR("gsensor g_sensor_x_acc[%d] = 0x%X --llllll--%d--lllllllllll", i, g_sensor_x_acc[i], shake_dir_once);           
                }                         
#endif            

        }
    }
    //direct to right.
    else if((((SHAKE_DIR_MIDDLE == shake_dir_once) ? SHAKE_COMP_NUM : (SHAKE_COMP_NUM-1)) <= add_num)
        && (SHAKE_UNLOCK == shake_lock))
    {
        if(((x_shake_acc_temp[DATA_SUM-2] - g_sensor_device_info.dwStaticValue) > GS_OUTPUT_CRT_RIGHT)
            && (x_shake_acc_temp[DATA_SUM-2]  > g_sensor_device_info.dwStaticValue))
        {
           if(SHAKE_DIR_RIGHT != shake_dir_once)
            {    
                shake_dir_once = SHAKE_DIR_RIGHT;
                
                //backup the offset value.
                shake_offset_once = x_shake_acc_temp[DATA_SUM-2] - g_sensor_device_info.dwStaticValue;
                shake_offset_once = shake_offset_once - GS_OUTPUT_CRT_RIGHT;
                
                // open shake delay timer for waiting whether that shake of more value is detected.
                wait_more_value_shake = SHAKE_WAIT_TICK + 1;  
            }
            else
            {
                shake_offset_temp = x_shake_acc_temp[DATA_SUM-2] - g_sensor_device_info.dwStaticValue;
                shake_offset_temp = shake_offset_temp - GS_OUTPUT_CRT_LEFT;
                if(shake_offset_temp >= shake_offset_once)
                {
                    //backup the offset value.
                    shake_offset_once = x_shake_acc_temp[DATA_SUM-2] - g_sensor_device_info.dwStaticValue;
                    shake_offset_once = shake_offset_once - GS_OUTPUT_CRT_RIGHT;
                    
                    // open shake delay timer for waiting whether that shake of more value is detected.
                    wait_more_value_shake = SHAKE_WAIT_TICK + 1;  
                }
            }  
#ifdef g_sensor_log_enable    
            for(i=0; i<DATA_SUM; i++)
            {   
                MSG_ERROR("gsensor g_sensor_x_acc[%d] = 0x%X --rrrrrrr--%d--rrrrrrrrrr", i, g_sensor_x_acc[i], shake_dir_once);           
            }
#endif            

        }
    }
#ifdef g_sensor_log_enable      
    MSG_ERROR("g_sensor_shake result: shake_dir_once %d,wait_more_value_shake %d", shake_dir_once, wait_more_value_shake, 0);  
#endif
    //last result
    if(SHAKE_DIR_MIDDLE != shake_dir_once)
    {
        if(0 == (--wait_more_value_shake))
        {
            if(SHAKE_DIR_LEFT == shake_dir_once)
            {
                KEYPAD_PASS_KEY_CODE(HS_GSENSOR_FORWARD_K, HS_NONE_K);
                KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_FORWARD_K); 
                g_shake_delay_sum = (SHAKE_DELAY_TIME_SUM / HS_SHAKE_TIME); //delay SHAKE_DELAY_TIME_SUM,if keypad pass.
                shake_dir_bc = SHAKE_DIR_LEFT; 
                shake_lock = SHAKE_LOCK;   
#ifdef g_sensor_log_enable 
                for(i=0; i<DATA_SUM; i++)
                {   
                    MSG_ERROR("gsensor g_sensor_x_acc[%d] = 0x%X --llllllllllllllllllll", i, g_sensor_x_acc[i], 0);           
                } 
#endif            
            }
            else if(SHAKE_DIR_RIGHT== shake_dir_once)
            {
                KEYPAD_PASS_KEY_CODE(HS_GSENSOR_BACKWARD_K, HS_NONE_K);
                KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_BACKWARD_K);  
                g_shake_delay_sum = (SHAKE_DELAY_TIME_SUM / HS_SHAKE_TIME); //delay SHAKE_DELAY_TIME_SUM,if keypad pass.
                shake_dir_bc = SHAKE_DIR_RIGHT;        
                shake_lock = SHAKE_LOCK; 
#ifdef g_sensor_log_enable    
                for(i=0; i<DATA_SUM; i++)
                {   
                    MSG_ERROR("gsensor g_sensor_x_acc[%d] = 0x%X --rrrrrrrrrrrrrrrrrrrrr", i, g_sensor_x_acc[i], 0);           
                }
#endif            
            } 
            
            shake_dir_once = SHAKE_DIR_MIDDLE;
            
       }
    }
}

void ClrShakeSum(void)
{
    shake_sum = 0;
    g_shake_delay_sum = 0;
}

/*==============================================================================
����: 
     g_sensor_change_video
             
˵��: 
     ��Ƶת���ж�  
       
����: 
              
����ֵ:��
       
��ע:  �������ݷ�����״̬�ı�󣬹ʵ�ǰ״̬�����ı����Ҫ�ȴ�1�����ҲŻ�ת����Ƶ
      
==============================================================================*/
void g_sensor_video_circ()
{
    int16 x_ave_sample_data, y_ave_sample_data;//ȡ������ʮ�����ݵ�ƽ��ֵ     
    int k, same_num = 0, diff_num = 0;
    int16 x_acc_temp;
    int16 y_acc_temp; 
    
    if(G_SENSOR_NOT_INIT == g_sensor_work_status)
    {    
        g_sensor_init();        
        return;
    }
   
    //��������豸û�д�,��ѯ�ĵ�һ���ȴ������豸
    if(G_SENSOR_POWER_DOWN == g_sensor_work_status)
    {
        g_sensor_power_up();
#ifdef g_sensor_log_enable   
        MSG_ERROR("g_sensor_power_up in g_sensor_pedometer", 0, 0, 0);
#endif
        return;
    }
   
   if(DATA_SUM > video_circ_sum)
    {
        if(FALSE == g_sensor_get_logic_acc(&g_sensor_x_acc[video_circ_sum], &g_sensor_y_acc[video_circ_sum]))//ȡ������ 
        {
            return;
        }
#ifdef g_sensor_log_enable   
        MSG_ERROR("gsensor video_circ_sum is %d", video_circ_sum, 0, 0);    
#endif
        video_circ_sum++;
        return;
    }
    if(FALSE == g_sensor_get_logic_acc(&x_acc_temp, &y_acc_temp))//ȡ������
    {
        return;
    }
    for(k=0; k<DATA_SUM; k++)   //��λ����
    {   
        if((DATA_SUM-1) == k)
        {
            g_sensor_x_acc[k] = x_acc_temp;    
            g_sensor_y_acc[k] = y_acc_temp;   
        }
        else
        {
            g_sensor_x_acc[k] = g_sensor_x_acc[k+1]; 
            g_sensor_y_acc[k] = g_sensor_y_acc[k+1];             
        }
    }
    
    for(k=0; k<5; k++)     //�ж�������ݵ��ȶ���                   //�жϵ�ǰʮ�������Ƿ��ȶ�
    {
        if(-GS_CRT_DATA < (g_sensor_x_acc[k] - g_sensor_x_acc[5+k])
            && (g_sensor_x_acc[k] - g_sensor_x_acc[5+k]) < GS_CRT_DATA 
            && -GS_CRT_DATA < (g_sensor_y_acc[k] - g_sensor_y_acc[5+k]) 
            && (g_sensor_y_acc[k] - g_sensor_y_acc[5+k]) < GS_CRT_DATA )
        {
            same_num++;  
        }
        else
        {
            diff_num++;
        }
    }
    
    if(same_num > diff_num)
    {
        g_sensor_get_ave(g_sensor_x_acc, g_sensor_y_acc, &x_ave_sample_data, &y_ave_sample_data);
        
        if(g_sensor_device_info.dwStaticValue - GS_CRT_DATA < x_ave_sample_data
            && x_ave_sample_data < g_sensor_device_info.dwStaticValue + GS_CRT_DATA
            && g_sensor_device_info.dwStaticValue  + g_sensor_device_info.dwOffestPerG - GS_CRT_DATA < y_ave_sample_data
            && y_ave_sample_data < g_sensor_device_info.dwStaticValue + g_sensor_device_info.dwOffestPerG + GS_CRT_DATA)
        {
            if(g_sensor_vedio_direction != HS_GSENSOR_STAND_K)
            {
                g_sensor_vedio_direction = HS_GSENSOR_STAND_K;
                KEYPAD_PASS_KEY_CODE(HS_GSENSOR_STAND_K, HS_NONE_K);
                KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_STAND_K);
                
#ifdef g_sensor_log_enable 
                MSG_ERROR("gsensor g_sensor_video_circ is HS_GSENSOR_STAND_K", 0, 0, 0);           
#endif  
            }
        }
        else if(g_sensor_device_info.dwStaticValue  - g_sensor_device_info.dwOffestPerG - GS_CRT_DATA < x_ave_sample_data
                 && x_ave_sample_data < g_sensor_device_info.dwStaticValue  - g_sensor_device_info.dwOffestPerG + GS_CRT_DATA
                 && g_sensor_device_info.dwStaticValue - GS_CRT_DATA < y_ave_sample_data
                 && y_ave_sample_data < g_sensor_device_info.dwStaticValue + GS_CRT_DATA)
        {
            if(g_sensor_vedio_direction != HS_GSENSOR_LEFT_K)
            {
                g_sensor_vedio_direction = HS_GSENSOR_LEFT_K;
                KEYPAD_PASS_KEY_CODE(HS_GSENSOR_LEFT_K, HS_NONE_K);
                KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_LEFT_K);
                
#ifdef g_sensor_log_enable 
                MSG_ERROR("gsensor g_sensor_video_circ is HS_GSENSOR_LEFT_K", 0, 0, 0);           
#endif
            }          
        }
        else if(g_sensor_device_info.dwStaticValue  + g_sensor_device_info.dwOffestPerG-GS_CRT_DATA < x_ave_sample_data
                 && x_ave_sample_data < g_sensor_device_info.dwStaticValue  + g_sensor_device_info.dwOffestPerG + GS_CRT_DATA
                 && g_sensor_device_info.dwStaticValue - GS_CRT_DATA < y_ave_sample_data
                 && y_ave_sample_data < g_sensor_device_info.dwStaticValue + GS_CRT_DATA)
        {
            if(g_sensor_vedio_direction != HS_GSENSOR_RIGHT_K)
            {
                g_sensor_vedio_direction = HS_GSENSOR_RIGHT_K;
                KEYPAD_PASS_KEY_CODE(HS_GSENSOR_RIGHT_K, HS_NONE_K);
                KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_RIGHT_K);
                
#ifdef g_sensor_log_enable 
                MSG_ERROR("gsensor g_sensor_video_circ is HS_GSENSOR_RIGHT_K", 0, 0, 0);           
#endif
            }
        }
        else if(g_sensor_device_info.dwStaticValue - GS_CRT_DATA < x_ave_sample_data
                 && x_ave_sample_data < g_sensor_device_info.dwStaticValue + GS_CRT_DATA
                 && g_sensor_device_info.dwStaticValue  - g_sensor_device_info.dwOffestPerG - GS_CRT_DATA < y_ave_sample_data
                 && y_ave_sample_data < g_sensor_device_info.dwStaticValue  - g_sensor_device_info.dwOffestPerG + GS_CRT_DATA)
        {
            if(g_sensor_vedio_direction != HS_GSENSOR_UPEND_K)
            {
                g_sensor_vedio_direction = HS_GSENSOR_UPEND_K;
                KEYPAD_PASS_KEY_CODE(HS_GSENSOR_UPEND_K, HS_NONE_K);
                KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_UPEND_K);
                
#ifdef g_sensor_log_enable 
                MSG_ERROR("gsensor g_sensor_video_circ is HS_GSENSOR_UPEND_K", 0, 0, 0);           
#endif                
            }
        }
        else
        {
            return;
        }
    }
}

void ClrVideoCircSum(void)
{
    video_circ_sum=0;
}

 int AccLPF(int iX0, int iY1)
{
iY1*=LPF_RC;					//RC = 0x000003;
iY1+=((iX0*SAMPLE_TIME)+(LPF_RC+SAMPLE_TIME)/2);
iY1/=(LPF_RC+SAMPLE_TIME);
return iY1;
}

int AccHPF(int iX0, int iX1, int iY1)
{
iY1+=(iX0-iX1);
iY1*=HPF_RC;
iY1+=0x008000+(HPF_RC+SAMPLE_TIME)/2;					//RC = 0x000003;
iY1/=(HPF_RC+SAMPLE_TIME);							//	0x000004;		T+RC   T=Timer*100,  
return iY1;
}

void StepLoops(int16 x_adc,int16 y_adc)
{
    unsigned char 	bStatus_axis = 1;
    int	 	iAccXData, iAccYData,iG_CountPEAK;
    int  	iAcce_Sample;
    int		iAcce1_LPFcount0,iAcce0_LPFcount0,iAcce1_HPFcount0,iAcce0_HPFcount0,iAcceFilter0;
    
#ifdef g_sensor_log_enable       
   MSG_ERROR("StepLoops x_adc is 0x%X", x_adc, 0, 0);
   MSG_ERROR("StepLoops y_adc is 0x%X", y_adc, 0, 0);
#endif

   	iAccXData=(int)x_adc;
   	iAccYData=(int)y_adc;
   
    if (iAccYData>0X0900) bStatus_axis=1;
    if (iAccYData<0X0700) bStatus_axis=1;
    if (iAccXData>0X0900) bStatus_axis=0;
    if (iAccXData<0X0700) bStatus_axis=0;

    if(bStatus_axis != bStatus_axis_bc)
    {
        bStatus_axis_bc = bStatus_axis;
        return;
    }
    
    if(bStatus_axis)
    {     
#ifdef g_sensor_log_enable              
        MSG_ERROR("gsensor bStatus_axis = YYYYYYYY", 0, 0, 0);
#endif
		iAcce_Sample=iAccYData;  
    }       
	else
    {
#ifdef g_sensor_log_enable              
        MSG_ERROR("gsensor bStatus_axis = XXXXXXXX", 0, 0, 0);
#endif      
		iAcce_Sample=iAccXData;    
    }   
    
    iAcce_Sample*=0x10;
    iAcce1_LPFcount0=AccLPF(iAcce_Sample,stc_iAcce1_LPFcount1);
    iAcce0_LPFcount0=AccLPF(iAcce1_LPFcount0,stc_iAcce0_LPFcount1);
    iAcce1_HPFcount0=AccHPF(iAcce0_LPFcount0,stc_iAcce0_LPFcount1,stc_iAcce1_HPFcount1);
    iAcce0_HPFcount0=AccHPF(iAcce1_HPFcount0,stc_iAcce1_HPFcount1,stc_iAcce0_HPFcount1);   
    iAcceFilter0=iAcce0_HPFcount0/0x10;
    
#ifdef g_sensor_log_enable              
    MSG_ERROR("gsensor iAcce1_LPFcount0 = 0x%X", iAcce1_LPFcount0, 0, 0);
    MSG_ERROR("gsensor iAcce0_LPFcount0 = 0x%X", iAcce0_LPFcount0, 0, 0);
    MSG_ERROR("gsensor iAcce1_HPFcount0 = 0x%X", iAcce1_HPFcount0, 0, 0);
    MSG_ERROR("gsensor iAcce0_HPFcount0 = 0x%X", iAcce0_HPFcount0, 0, 0);
    MSG_ERROR("gsensor iAcceFilter0 = 0x%X", iAcceFilter0, 0, 0);
#endif            
		
    if (iAcceFilter0>0x000800 && iAcceFilter0>stc_iG_CountUP) 
		stc_iG_CountUP=iAcceFilter0;
    if (iAcceFilter0<0x000800 && iAcceFilter0<stc_iG_CountDOWN) 
		stc_iG_CountDOWN=iAcceFilter0;
    if (iAcceFilter0>=0x000800 && stc_iAcceFilter1<0x000800)
    {
        stc_iG_CountDOWN=0x000800-stc_iG_CountDOWN;
        iG_CountPEAK=stc_iG_CountDOWN;
        stc_iG_CountDOWN=0x000800;
       if (iG_CountPEAK>MAN_STEPS)
        {
    	   	bUp=1;   
#ifdef g_sensor_log_enable              
            MSG_ERROR("gsensor bUp=1", 0, 0, 0);
#endif            
        }
	   else
	   	bUp=0;
    }
   if (iAcceFilter0<=0x000800 && stc_iAcceFilter1>0x000800)
    {
        stc_iG_CountUP=stc_iG_CountUP-0x000800;
        iG_CountPEAK=stc_iG_CountUP;
        stc_iG_CountUP=0x000800;
        if (iG_CountPEAK>MAN_STEPS) 
        {
			bDown=1; 
#ifdef g_sensor_log_enable              
            MSG_ERROR("gsensor bDown=1", 0, 0, 0);
#endif             
        }
		else
			bDown=0;
    }
   if ( bUp && bDown )
    {
        PedoCounter++; //������1	
        bUp=0;bDown=0;
    }
    stc_iAcce0_LPFcount1=iAcce0_LPFcount0;
    stc_iAcce1_LPFcount1=iAcce1_LPFcount0;
    stc_iAcce0_HPFcount1=iAcce0_HPFcount0;
    stc_iAcce1_HPFcount1=iAcce1_HPFcount0;
    stc_iAcceFilter1=iAcceFilter0;				
}

/*==============================================================================
����: 
     g_sensor_refresh_status
             
˵��: 
     ���ݵ�ǰ��ע��ķ��������ݡ�
     
����: 
              
����ֵ:��
       
��ע:
      ��ѯ�ĵ�һ��������ʼ�������������豸
==============================================================================*/
void g_sensor_pedometer()
{
    int16 x_pedometer_acc = 0;
    int16 y_pedometer_acc = 0;
    word cout_temp;
    
        //��������豸û�г�ʼ������ѯ�ĵ�һ��������ʼ����
    if(G_SENSOR_NOT_INIT == g_sensor_work_status)
    {    
        g_sensor_init();        
        return;
    }
   
    //��������豸û�д�,��ѯ�ĵ�һ���ȴ������豸
    if(G_SENSOR_POWER_DOWN == g_sensor_work_status)
    {
        g_sensor_power_up();
        
 #ifdef g_sensor_log_enable          
        MSG_ERROR("g_sensor_power_up in g_sensor_pedometer", 0, 0, 0);
 #endif
        return;
    }
    
    // return if open pedometer delay!
    if(0 != g_pedometer_delay_sum)
    {
#ifdef g_sensor_log_enable           
        MSG_ERROR("pedometer delay = %d", g_pedometer_delay_sum, 0, 0); 
#endif
        g_pedometer_delay_sum--;
        return;
    }

    if(FALSE == g_sensor_get_logic_acc(&x_pedometer_acc, &y_pedometer_acc))//ȡ������
    {
        return;
    }
#ifdef g_sensor_log_enable       
    MSG_ERROR("gsensor x_pedometer_acc is 0x%X", x_pedometer_acc, 0, 0);
    MSG_ERROR("gsensor y_pedometer_acc is 0x%X", y_pedometer_acc, 0, 0);
#endif

    cout_temp = PedoCounter;
    StepLoops(x_pedometer_acc,y_pedometer_acc);
    if(PedoCounter > cout_temp)
    {
        KEYPAD_PASS_KEY_CODE(HS_GSENSOR_STEP_K, HS_NONE_K);
        KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_STEP_K); 
        g_pedometer_delay_sum = (100 / HS_PEDOMETER_TIME); //delay 100ms,if keypad pass.
#ifdef g_sensor_log_enable        
        MSG_ERROR("gsensor PedoCounter is %d", PedoCounter, 0, 0);
#endif  
    }         
}

void g_sensor_pedometer_pause(void) 
{
    bUp = 0;
    bDown = 0;

}

int ReadPedoCounter(void)
{
    return PedoCounter;
}

void ClrPedoCounter(void)
{
    PedoCounter = 0;
    g_pedometer_delay_sum = 0;
}

// get X&Y data of navigation benchmark
void g_sensor_nav_get_std(void)
{
    int16 x_acc_temp,y_acc_temp;
    int16 x_acc_temp_sum = 0,y_acc_temp_sum = 0;
    int16 nav_get_std_sum;
    int16 get_data_times = 0;
    
    if(G_SENSOR_NOT_INIT == g_sensor_work_status)
    {    
        g_sensor_init();        
        return;
    }
   
    //��������豸û�д�,��ѯ�ĵ�һ���ȴ������豸
    if(G_SENSOR_POWER_DOWN == g_sensor_work_status)
    {
        g_sensor_power_up();
#ifdef g_sensor_log_enable           
        MSG_ERROR("g_sensor_power_up in g_sensor_nav_get_std", 0, 0, 0);
#endif
        return;
    }
    for(nav_get_std_sum=10;nav_get_std_sum>0;nav_get_std_sum--)
    {
        if(FALSE == g_sensor_get_logic_acc(&x_acc_temp, &y_acc_temp))//ȡ������
        {
#ifdef g_sensor_log_enable           
            MSG_ERROR("g_sensor_navigation get data fail!", 0, 0, 0);
#endif    
            continue;
        }
        x_acc_temp_sum += x_acc_temp;
        y_acc_temp_sum += y_acc_temp;  
        get_data_times++;
        rex_sleep(10);
    }
    x_acc_nav_std = x_acc_temp_sum / get_data_times;
    y_acc_nav_std = y_acc_temp_sum / get_data_times;
    
#ifdef g_sensor_log_enable           
        MSG_ERROR("g_sensor_navigation x_acc_nav_std = 0x%X", x_acc_nav_std, 0, 0);
        MSG_ERROR("g_sensor_navigation y_acc_nav_std = 0x%X", y_acc_nav_std, 0, 0);
#endif    
}

void g_sensor_navigation(void)
{ 
    int16 x_acc_temp;
    int16 y_acc_temp;

    if(G_SENSOR_NOT_INIT == g_sensor_work_status)
    {    
        g_sensor_init();        
        return;
    }
   
    //��������豸û�д�,��ѯ�ĵ�һ���ȴ������豸
    if(G_SENSOR_POWER_DOWN == g_sensor_work_status)
    {
        g_sensor_power_up();
#ifdef g_sensor_log_enable           
        MSG_ERROR("g_sensor_power_up in g_sensor_navigation", 0, 0, 0);
#endif
        return;
    }
    
    if(FALSE == g_sensor_get_logic_acc(&x_acc_temp, &y_acc_temp))//ȡ������
    {
        return;
    }
    if(((y_acc_temp > y_acc_nav_std) ? (y_acc_temp - y_acc_nav_std) : (y_acc_nav_std - y_acc_temp))
        >= ((x_acc_temp > x_acc_nav_std)?(x_acc_temp - x_acc_nav_std) : (x_acc_nav_std - x_acc_temp)))
    {
        if(y_acc_temp > (y_acc_nav_std + NAV_OFFSET))
        {
            //down
            KEYPAD_PASS_KEY_CODE(HS_GSENSOR_UPEND_K, HS_NONE_K);
            KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_UPEND_K); 
#ifdef g_sensor_log_enable           
            MSG_ERROR("g_sensor_navigation down", 0, 0, 0);
#endif
        }
        if(y_acc_temp < (y_acc_nav_std - NAV_OFFSET))
        {
            //up
            KEYPAD_PASS_KEY_CODE(HS_GSENSOR_STAND_K, HS_NONE_K);
            KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_STAND_K);  
#ifdef g_sensor_log_enable           
            MSG_ERROR("g_sensor_navigation up", 0, 0, 0);
#endif        
        }
    }
    else
    {
        if(x_acc_temp > (x_acc_nav_std + NAV_OFFSET)) 
        {
            //right
            KEYPAD_PASS_KEY_CODE(HS_GSENSOR_RIGHT_K, HS_NONE_K);
            KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_RIGHT_K); 
#ifdef g_sensor_log_enable           
            MSG_ERROR("g_sensor_navigation right", 0, 0, 0);
#endif        
        }
        if(x_acc_temp < (x_acc_nav_std - NAV_OFFSET))
        {
            //left
            KEYPAD_PASS_KEY_CODE(HS_GSENSOR_LEFT_K, HS_NONE_K);
            KEYPAD_PASS_KEY_CODE(HS_RELEASE_K, HS_GSENSOR_LEFT_K);   
#ifdef g_sensor_log_enable           
            MSG_ERROR("g_sensor_navigation left", 0, 0, 0);
#endif        
        }
    }    
 }

