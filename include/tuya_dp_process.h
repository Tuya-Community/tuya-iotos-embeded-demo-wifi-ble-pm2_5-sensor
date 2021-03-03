/**
 * @File: tuya_dp_process.h 
 * @Author: shiliu.yang@tuya.com 
 * @Last Modified time: 2021-01-05 
 * @Description: flame detection demo 
 */
#ifndef __TUYA_DP_PROCESS_H_
#define __TUYA_DP_PROCESS_H_

#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

typedef enum {
    alarm = 0,
    normal = 1
}PM25_STATE_E;

//设备状态结构体
typedef struct {
    PM25_STATE_E    pm25_state; 
    UINT_T          pm25_value;
}PM25_SENSOR_S;


extern PM25_SENSOR_S  pm25_sensor_status;

VOID updata_dp_all(VOID);
VOID dp_process(IN CONST TY_OBJ_DP_S *root);

#endif

