# Tuya IoTOS Embeded Demo WiFi & BLE Smart PM2.5 Sensor

[English](./README.md) | [中文](./README_zh.md)

## Introduction 

This Demo implements a smart PM2.5 detector through the Tuya Smart Cloud Platform, Tuya Smart APP, IoTOS Embeded WiFi &Ble SDK, using the Tuya WiFi/WiFi+BLE series modules to achieve remote data monitoring intelligent functions.

Implemented functions include.

+ PM2.5 real-time detection 



## Quick start 

### Compile & Burn
+ Download [Tuya IoTOS Embeded WiFi & BLE sdk](https://github.com/tuya/tuya-iotos-embeded-sdk-wifi-ble-bk7231t) 

+ Download the demo to the apps directory of the SDK 

  ```bash
  $ cd apps
  $ git clone https://github.com/Tuya-Community/tuya-iotos-embeded-demo-wifi-ble-pm2_5-sensor
  ```
  
+ Execute the following command in the SDK root directory to start compilation.

  ```bash
  sh build_app.sh apps/tuya-iotos-embeded-demo-wifi-ble-pm2_5-sensor tuya-iotos-embeded-demo-wifi-ble-pm2_5-sensor 1.0.0 
  ```

+ Firmware burn-in license information please refer to: [Wi-Fi + BLE series module burn-in license](https://developer.tuya.com/cn/docs/iot/device-development/burn-and-authorization/burn-and- authorize-wifi-ble-modules/burn-and-authorize-wb-series-modules?id=Ka78f4pttsytd) 



### File introduction 

```
├── include
│   ├── common
│   │   ├── device_config_load.h
│   │   ├── light_printf.h
│   │   └── light_types.h
│   ├── soc
│   │   ├── soc_adc.h
│   │   └── soc_pwm.h
│   ├── tuya_device.h
│   ├── tuya_dp_process.h
│   └── tuya_pm2_5.h
└── src
    ├── soc
    │   ├── soc_adc.c
    │   └── soc_pwm.c
    ├── tuya_device.c		//Application entry file
    ├── tuya_dp_process.c	//DataPoint process file
    └── tuya_pm2_5.c		//PM2.5 sensor data processing file
```



### Demo Information 

|      Product ID      |                       tswil6rgaddniklo                       |
| :------------------: | :----------------------------------------------------------: |
|    Chip Platform     |                           BK7231S                            |
| Compiler Environment | ty_iot_wf_bt_sdk_bk_bk7231t_1.0.2 (Tuya IOT Platform SDK 1.0.2) |
|     Sensor Model     |                          ZPH02-2in1                          |



### Demo entry

Entry file: tuya_device.c

Important function: device_init()

+ Call tuya_iot_wf_soc_dev_init_param() interface to initialize the SDK, configure the operating mode, the wiring mode, and register various callback functions and store the firmware key and PID.

+ Calling the tuya_iot_reg_get_wf_nw_stat_cb() interface to register the device network status callback functions.

+ Call the application layer initialization function air_box_device_init()



### DataPoint related

+ Report dp point interface: dev_report_dp_json_async()

| function name | OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt) |
| ------- | ------------------------------------------------------------ |
| devid | device id (if gateway, MCU, SOC class device then devid = NULL; if sub-device, then devid = sub-device_id) |
| dp_data | dp structure array name |
| cnt | Number of elements of the dp structure array |
| return | OPRT_OK: Success Other: Failure |



### I/O List 

| PM2.5 sensor pins | corresponding connected wb3s pins |
| :-------------: | :----------------: |
| VCC | 3.3V |
| GND | GND |
| TX | RXD2/GPIO_1 |
| RX | TXD2/GPIO_0 |



### Sensor Related Introduction 

| Baud Rate | 9600 |
| ------ | ---- |
| Data bits | 8 bits |
| Stop Bits | 1 bit |
| Check Bit | None |

The module sends the concentration value every 1S, it only sends and does not receive:

| Byte0 | Byte1 | Byte2 | Byte3 | Byte4 | Byte5 | Byte6 | Byte7 | Byte8 |
| :----: | :------: | :--------------: | :--------------: | :--------------: | :---: | :---: | :-------: | :-------: |
| Start Bit | Gas Name | Unit (Low Pulse Rate) | Low Pulse Rate Integer Part | Low Pulse Rate Fractional Part | Reserved | Reserved | VOC Level | Checksum |
| 0xFF | 0x18 | 0x00 | 0x00-0x63 | 0x00 | 0x00 | 0x00 | 0x00 | 0x01-0x04 | 0x00-0xFF |

Calculation of PM2.5:

The sensor sends out a frame of normal data with bit 3 0X12 and bit 4 0X13. This means that the duty cycle of the sensor output is 18.19%. Low pulse rate output in serial mode ranges from 0.5% to 50%.  



Checksum = (Inverse (Byte1+Byte2+......+Byte7)) + 1  



## Related Documents

Tuya Demo Center: https://developer.tuya.com/cn/demo/



## Technical Support

You can get support for Tuya by using the following methods:

- Developer Center: https://developer.tuya.com/cn/
- Help Center: https://support.tuya.com/en/help
- Technical Support Work Order Center: [https://service.console.tuya.com](https://service.console.tuya.com/)