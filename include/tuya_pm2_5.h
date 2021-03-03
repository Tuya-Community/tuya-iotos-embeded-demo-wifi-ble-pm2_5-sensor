/**
 * @File: tuya_pm2_5.h 
 * @Author: shiliu.yang@tuya.com 
 * @Last Modified time: 2021-01-05 
 * @Description: vibration sensor demo
 */
#ifndef __TUYA_PM2_5_H__
#define __TUYA_PM2_5_H__

#include "tuya_cloud_types.h"
#include "tuya_gpio.h"

#define PM25_SIGNAL_PIN TY_GPIOA_8

VOID pm25_sensor_init(VOID);

VOID check_pm25_task(VOID);


#endif

