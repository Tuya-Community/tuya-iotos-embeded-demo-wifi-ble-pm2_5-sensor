/**
 * @File: tuya_dp_process.c 
 * @Author: shiliu.yang@tuya.com 
 * @Last Modified time: 2021-01-05 
 * @Description: 
 */

#include "tuya_dp_process.h"
#include "tuya_iot_com_api.h"
#include "uni_log.h"


//dp 点枚举类型
typedef enum {
    dpid_pm25_state = 1,
    dpid_pm25_value = 2
}DP_ID_E;


PM25_SENSOR_S  pm25_sensor_status = {
    .pm25_state = normal,
    .pm25_value = 0,
};

 /***********************************************************
 *   Function:  dp_process
 *   Input:     none
 *   Output:    none
 *   Return:    none
 *   Notice:    处理 dp 数据 
 ***********************************************************/
VOID dp_process(IN CONST TY_OBJ_DP_S *root)
{
    UCHAR_T dpid;

    dpid = root->dpid;
    PR_DEBUG("dpid:%d",dpid);

    return;
}

 /***********************************************************
 *   Function:  updata_dp_all
 *   Input:     none
 *   Output:    none
 *   Return:    none
 *   Notice:    上报所有 dp 
 ***********************************************************/
VOID updata_dp_all(VOID)
{
        OPERATE_RET op_ret = OPRT_OK;
    
        INT_T dp_cnt = 0;
        dp_cnt = 2;
    
        TY_OBJ_DP_S *dp_arr = (TY_OBJ_DP_S *)Malloc(dp_cnt*SIZEOF(TY_OBJ_DP_S));
        if(NULL == dp_arr) {
            PR_ERR("malloc failed");
            return;
        }
    
        memset(dp_arr, 0, dp_cnt*SIZEOF(TY_OBJ_DP_S));
    
    
        dp_arr[0].dpid = dpid_pm25_state;
        dp_arr[0].type = PROP_ENUM;
        dp_arr[0].time_stamp = 0;
        dp_arr[0].value.dp_enum = pm25_sensor_status.pm25_state;

        dp_arr[1].dpid = dpid_pm25_value;
        dp_arr[1].type = PROP_VALUE;
        dp_arr[1].time_stamp = 0;
        dp_arr[1].value.dp_value = pm25_sensor_status.pm25_value;

        op_ret = dev_report_dp_json_async(NULL , dp_arr, dp_cnt);
        Free(dp_arr);
        if(OPRT_OK != op_ret) {
            PR_ERR("dev_report_dp_json_async relay_config data error,err_num",op_ret);
        }
    
        PR_DEBUG("dp_query report_all_dp_data");
        
        return;
}



