/**
 * @File: tuya_pm2_5.c 
 * @Author: shiliu.yang@tuya.com 
 * @Last Modified time: 2021-01-07 
 * @Description: tuya app vibration sensor drive demo 
 */

#include "tuya_dp_process.h"
#include "tuya_pm2_5.h"
#include "tuya_iot_com_api.h"
#include "uni_log.h"
#include "tuya_uart.h"
#include "soc_adc.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uni_thread.h"

/* PM2.5 传感器数据输入串口 */
#define PM25_SENSOR_UART    TY_UART2
#define PM25_BUFFER_SIZE    20

#define PM25_ALARM_VALUE    115 //中度污染及以上提示

VOID check_pm25_task(VOID);

VOID pm25_sensor_init(VOID)
{
    /* pm2.5 传感器 串口 初始化 */
    ty_uart_init(PM25_SENSOR_UART, TYU_RATE_9600, TYWL_8B, TYP_NONE, TYS_STOPBIT1, (PM25_BUFFER_SIZE * SIZEOF(UCHAR_T)), TRUE);
    
    tuya_hal_thread_create(NULL, "check_pm25", 512, TRD_PRIO_2, check_pm25_task, NULL);
}

 /***********************************************************
 *   Function:  ch2o_check_sum
 *   Input:     none
 *   Output:    none
 *   Return:    none
 *   Notice:    甲醛数据校验和
 ***********************************************************/
static UCHAR_T ch2o_check_sum(UCHAR_T *data, UCHAR_T len)
{
    UCHAR_T i, tempq = 0;
    data += 1; //指向data[1]

    for(i=0; i<(len-2); i++)
    {
        tempq += *data;
        data++;
    }

    tempq = (~tempq) + 1;

    return (tempq);
}

 /***********************************************************
 *   Function:  get_pm25_sensor_value
 *   Input:     none
 *   Output:    none
 *   Return:    none
 *   Notice:    得到并上传 pm2.5 的数据
 ***********************************************************/
static VOID get_pm25_sensor_value(VOID)
{
    UINT_T  buff_ret, find_head_index = 0;
    //PM2.5 数据高位，低位
    UCHAR_T pm25_data_high, pm25_data_low;
    //校验和
    UCHAR_T check_sums = 0x00;    
    //串口数据缓存区
    UCHAR_T pm25_receive_buffer[PM25_BUFFER_SIZE];
    //指向 PM2.5 数据头部
    UCHAR_T *p_pm25_value = NULL;

    memset(pm25_receive_buffer, 0, sizeof(pm25_receive_buffer));

    //读取串口数据
    ty_uart_read_data(PM25_SENSOR_UART, pm25_receive_buffer, PM25_BUFFER_SIZE);

    // for (find_head_index = 0; find_head_index<PM25_BUFFER_SIZE; find_head_index++) {
    //     PR_NOTICE("pm25_receive_buffer[%d] = %02x", find_head_index, pm25_receive_buffer[find_head_index]);
    // }

    //寻找 PM2.5 传感器发送过来的头部
    for (find_head_index = 0; find_head_index<PM25_BUFFER_SIZE; find_head_index++) {
        if (pm25_receive_buffer[find_head_index] == 0xff && pm25_receive_buffer[find_head_index+1] == 0x18 && pm25_receive_buffer[find_head_index+2] == 0x00){
            //PR_NOTICE("find head is %d", find_head_index);
            break;
        }
    }

    //本次采集数据不完整
    if (find_head_index > 11) { 
        PR_ERR("PM2.5 get uart data no complete!");
        return;
    }

    //将指针指向 PM2.5 数据中的头部
    p_pm25_value = pm25_receive_buffer + find_head_index;

    //检验和，确认读取的数据的准确性
    check_sums = ch2o_check_sum(p_pm25_value, 9);
    if (check_sums != *(p_pm25_value + 8)) {
        PR_ERR("PM2.5 check_sums error");
        return;
    }
    
    pm25_data_high = *(p_pm25_value+3);
    pm25_data_low = *(p_pm25_value+4);

    pm25_sensor_status.pm25_value = pm25_data_high * 100.0 + pm25_data_low;
    if (pm25_sensor_status.pm25_value > PM25_ALARM_VALUE * 10) {
        pm25_sensor_status.pm25_state = alarm;
    } else {
        pm25_sensor_status.pm25_state = normal;
    }

    //PR_NOTICE("PM2.5: %d - %d .", pm25_data_high, pm25_data_low);
    //PR_NOTICE("PM2.5 value is : %d .", gs_air_box.pm25_value);

    return;
}

VOID check_pm25_task(VOID)
{
    while (1) {
        get_pm25_sensor_value();
        updata_dp_all();
        tuya_hal_system_sleep(2000);
    }

}

