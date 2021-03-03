# Tuya IoTOS Embeded Demo WiFi & BLE Smart PM2.5 Sensor 

[English](./README.md) | [中文](./README_zh.md)

## 简介 

本Demo通过涂鸦智能云平台、涂鸦智能APP、IoTOS Embeded WiFi &Ble SDK实现一款智能PM2.5检测仪，使用涂鸦WiFi/WiFi+BLE系列模组，实现远程数据监测智能化功能。

已实现功能包括：

+ PM2.5实时检测




## 快速上手 

### 编译与烧录
+ 下载[Tuya IoTOS Embeded WiFi & BLE sdk](https://github.com/tuya/tuya-iotos-embeded-sdk-wifi-ble-bk7231t) 

+ 下载Demo至SDK目录的apps目录下 

  ```bash
  $ cd apps
  $ git clone https://github.com/Tuya-Community/tuya-iotos-embeded-demo-wifi-ble-pm2_5-sensor.git
  ```
  
+ 在SDK根目录下执行以下命令开始编译：

  ```bash
  sh build_app.sh apps/btuya-iotos-embeded-demo-wifi-ble-pm2_5-sensor tuya-iotos-embeded-demo-wifi-ble-pm2_5-sensor 1.0.0 
  ```

+ 固件烧录授权相关信息请参考：[Wi-Fi + BLE 系列模组烧录授权](https://developer.tuya.com/cn/docs/iot/device-development/burn-and-authorization/burn-and-authorize-wifi-ble-modules/burn-and-authorize-wb-series-modules?id=Ka78f4pttsytd) 



### 文件介绍 

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
    ├── tuya_device.c		//应用入口文件
    ├── tuya_dp_process.c	//DP点处理文件
    └── tuya_pm2_5.c		//PM2.5传感器数据处理文件
```



### Demo信息 

|   产品ID   |                      tswil6rgaddniklo                      |
| :--------: | :--------------------------------------------------------: |
|  芯片平台  |                          BK7231S                           |
|  编译环境  | ty_iot_wf_bt_sdk_bk_bk7231t_1.0.2（涂鸦IOT平台 SDK 1.0.2） |
| 传感器型号 |                         ZPH02-2in1                         |



### Demo入口

入口文件：tuya_device.c 

重要函数：device_init()

+ 调用 tuya_iot_wf_soc_dev_init_param() 接口进行SDK初始化，配置了工作模式、配网模式，同时注册了各种回调函数并存入了固件key和PID。

+ 调用 tuya_iot_reg_get_wf_nw_stat_cb() 接口注册设备网络状态回调函数。

+ 调用应用层初始化函数 air_box_device_init()



### DP点相关

+ 上报dp点接口: dev_report_dp_json_async()

| 函数名  | OPERATE_RET dev_report_dp_json_async(IN CONST CHAR_T *dev_id,IN CONST TY_OBJ_DP_S *dp_data,IN CONST UINT_T cnt) |
| ------- | ------------------------------------------------------------ |
| devid   | 设备id（若为网关、MCU、SOC类设备则devid = NULL;若为子设备，则devid = sub-device_id) |
| dp_data | dp结构体数组名                                               |
| cnt     | dp结构体数组的元素个数                                       |
| Return  | OPRT_OK: 成功  Other: 失败                                   |



### I/O 列表 

| PM2.5传感器引脚 | 对应连接的wb3s引脚 |
| :-------------: | :----------------: |
|       VCC       |        3.3V        |
|       GND       |        GND         |
|       TX        |    RXD2/GPIO_1     |
|       RX        |    TXD2/GPIO_0     |



### 传感器相关介绍 


| 波特率 | 9600 |
| ------ | ---- |
| 数据位 | 8位  |
| 停止位 | 1位  |
| 校验位 | 无   |

模块每隔1S发送一次浓度值，只发送不接收 ：

| Byte0  |  Byte1   |      Byte2       |      Byte3       |      Byte4       | Byte5 | Byte6 |   Byte7   |   Byte8   |
| :----: | :------: | :--------------: | :--------------: | :--------------: | :---: | :---: | :-------: | :-------: |
| 起始位 | 气体名称 | 单位（低脉冲率） | 低脉冲率整数部分 | 低脉冲率小数部分 | 预留  | 预留  |  VOC等级  |  校验值   |
|  0xFF  |   0x18   |       0x00       |    0x00-0x63     |    0x00-0x63     | 0x00  | 0x00  | 0x01-0x04 | 0x00-0xFF |

对 PM2.5 的计算：

传感器发出一帧正常数据中， 第 3 位 0X12， 第 4 位 0X13。 则表示： 传感器输出的占空比为 18.19%。串口模式下低脉冲率输出范围 0.5%-50%。  



校验和 = （取反（Byte1+Byte2+……+Byte7）） + 1  



## 相关文档

涂鸦Demo中心：https://developer.tuya.com/cn/demo/



## 技术支持

您可以通过以下方法获得涂鸦的支持:

- 开发者中心：https://developer.tuya.com/cn/
- 帮助中心: https://support.tuya.com/en/help
- 技术支持工单中心: [https://service.console.tuya.com](https://service.console.tuya.com/)