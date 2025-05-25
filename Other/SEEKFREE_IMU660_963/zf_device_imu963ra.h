/*********************************************************************************************************************
* MSPM0G3507 Opensourec Library 即（MSPM0G3507 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
* 
* 本文件是 MSPM0G3507 开源库的一部分
* 
* MSPM0G3507 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
* 
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
* 
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
* 
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
* 
* 文件名称          zf_device_imu963ra
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          MDK 5.38a
* 适用平台          MSPM0G3507
* 店铺链接          https://seekfree.taobao.com/
********************************************************************************************************************/

/*********************************************************************************************************************
* 接线定义          查看 zf_device_imu_interface.h 中接口资源定义
********************************************************************************************************************/

#ifndef _zf_device_imu963ra_h
#define _zf_device_imu963ra_h

// zf_common 层 类型定义 引用
#include "zf_common_typedef.h"

// 此处列举 当前支持的函数列表
// 具体声明在本函数中查看对应注释 具体定义跳转到对应函数定义查看
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// imu963ra_get_physical_acc                                                    // 获取 IMU963RA 加速度计物理量数据
// imu963ra_get_physical_gyro                                                   // 获取 IMU963RA 陀螺仪物理量数据
// imu963ra_get_physical_mag                                                    // 获取 IMU963RA 地磁计物理量数据

// imu963ra_get_acc                                                             // 获取 IMU963RA 加速度计数据
// imu963ra_get_gyro                                                            // 获取 IMU963RA 陀螺仪数据
// imu963ra_get_mag                                                             // 获取 IMU963RA 地磁计数据

// imu963ra_set_config                                                          // IMU963RA 配置模块工作参数
// imu963ra_init                                                                // 初始化 IMU963RA
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 IMU963RA 相关的结构体数据构成细节 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// IMU963RA_ACC_OUTPUT_RATE_BASE 对应默认的 IMU963RA 的加速度计数据输出频率 实际为 52Hz
// 实际会略有偏差 需要去查看对应的芯片手册 因此对应从低到高可以设置为
// IMU963RA_ACC_OUTPUT_RATE_4_DIV       ->  12.5    Hz
// IMU963RA_ACC_OUTPUT_RATE_2_DIV       ->  26      Hz
// IMU963RA_ACC_OUTPUT_RATE_BASE        ->  52      Hz
// IMU963RA_ACC_OUTPUT_RATE_2_MUL       ->  104     Hz
// IMU963RA_ACC_OUTPUT_RATE_4_MUL       ->  208     Hz
// IMU963RA_ACC_OUTPUT_RATE_8_MUL       ->  416     Hz
// IMU963RA_ACC_OUTPUT_RATE_16_MUL      ->  833     Hz
// IMU963RA_ACC_OUTPUT_RATE_32_MUL      ->  1666    Hz
// IMU963RA_ACC_OUTPUT_RATE_64_MUL      ->  3332    Hz
// IMU963RA_ACC_OUTPUT_RATE_128_MUL     ->  6667    Hz
typedef enum
{
    IMU963RA_ACC_OUTPUT_RATE_4_DIV  = 6 ,                                       // 加速度计输出频率 4  分频 (ACC = Accelerometer 加速度计)
    IMU963RA_ACC_OUTPUT_RATE_2_DIV      ,                                       // 加速度计输出频率 2  分频 (ACC = Accelerometer 加速度计)
    IMU963RA_ACC_OUTPUT_RATE_BASE       ,                                       // 加速度计输出频率基础频率 (ACC = Accelerometer 加速度计)
    IMU963RA_ACC_OUTPUT_RATE_2_MUL      ,                                       // 加速度计输出频率 2  倍频 (ACC = Accelerometer 加速度计)
    IMU963RA_ACC_OUTPUT_RATE_4_MUL      ,                                       // 加速度计输出频率 4  倍频 (ACC = Accelerometer 加速度计)
    IMU963RA_ACC_OUTPUT_RATE_8_MUL      ,                                       // 加速度计输出频率 8  倍频 (ACC = Accelerometer 加速度计)
    IMU963RA_ACC_OUTPUT_RATE_16_MUL     ,                                       // 加速度计输出频率 16 倍频 (ACC = Accelerometer 加速度计)
    IMU963RA_ACC_OUTPUT_RATE_32_MUL     ,                                       // 加速度计输出频率 32 倍频 (ACC = Accelerometer 加速度计)
    IMU963RA_ACC_OUTPUT_RATE_64_MUL     ,                                       // 加速度计输出频率 64 倍频 (ACC = Accelerometer 加速度计)
    IMU963RA_ACC_OUTPUT_RATE_128_MUL    ,                                       // 加速度计输出频率 128倍频 (ACC = Accelerometer 加速度计)
}imu963ra_acc_output_rate_enum;

typedef enum
{
    IMU963RA_ACC_RANGE_SGN_2G       = 2 ,                                       // 加速度计量程 ±2 g (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
    IMU963RA_ACC_RANGE_SGN_4G           ,                                       // 加速度计量程 ±4 g (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
    IMU963RA_ACC_RANGE_SGN_8G           ,                                       // 加速度计量程 ±8 g (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
    IMU963RA_ACC_RANGE_SGN_16G          ,                                       // 加速度计量程 ±16g (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
}imu963ra_acc_range_enum;

// IMU963RA_GYRO_OUTPUT_RATE_BASE 对应默认的 IMU963RA 的陀螺仪数据输出频率 实际为 52Hz
// 实际会略有偏差 需要去查看对应的芯片手册 因此对应从低到高可以设置为
// IMU963RA_GYRO_OUTPUT_RATE_4_DIV      ->  12.5    Hz
// IMU963RA_GYRO_OUTPUT_RATE_2_DIV      ->  26      Hz
// IMU963RA_GYRO_OUTPUT_RATE_BASE       ->  52      Hz
// IMU963RA_GYRO_OUTPUT_RATE_2_MUL      ->  104     Hz
// IMU963RA_GYRO_OUTPUT_RATE_4_MUL      ->  208     Hz
// IMU963RA_GYRO_OUTPUT_RATE_8_MUL      ->  416     Hz
// IMU963RA_GYRO_OUTPUT_RATE_16_MUL     ->  833     Hz
// IMU963RA_GYRO_OUTPUT_RATE_32_MUL     ->  1666    Hz
// IMU963RA_GYRO_OUTPUT_RATE_64_MUL     ->  3332    Hz
// IMU963RA_GYRO_OUTPUT_RATE_128_MUL    ->  6667    Hz
typedef enum
{
    IMU963RA_GYRO_OUTPUT_RATE_4_DIV = 6 ,                                       // 陀螺仪输出频率 4  分频 (GYRO = Gyroscope 陀螺仪)
    IMU963RA_GYRO_OUTPUT_RATE_2_DIV     ,                                       // 陀螺仪输出频率 2  分频 (GYRO = Gyroscope 陀螺仪)
    IMU963RA_GYRO_OUTPUT_RATE_BASE      ,                                       // 陀螺仪输出频率基础频率 (GYRO = Gyroscope 陀螺仪)
    IMU963RA_GYRO_OUTPUT_RATE_2_MUL     ,                                       // 陀螺仪输出频率 2  倍频 (GYRO = Gyroscope 陀螺仪)
    IMU963RA_GYRO_OUTPUT_RATE_4_MUL     ,                                       // 陀螺仪输出频率 4  倍频 (GYRO = Gyroscope 陀螺仪)
    IMU963RA_GYRO_OUTPUT_RATE_8_MUL     ,                                       // 陀螺仪输出频率 8  倍频 (GYRO = Gyroscope 陀螺仪)
    IMU963RA_GYRO_OUTPUT_RATE_16_MUL    ,                                       // 陀螺仪输出频率 16 倍频 (GYRO = Gyroscope 陀螺仪)
    IMU963RA_GYRO_OUTPUT_RATE_32_MUL    ,                                       // 陀螺仪输出频率 32 倍频 (GYRO = Gyroscope 陀螺仪)
    IMU963RA_GYRO_OUTPUT_RATE_64_MUL    ,                                       // 陀螺仪输出频率 64 倍频 (GYRO = Gyroscope 陀螺仪)
    IMU963RA_GYRO_OUTPUT_RATE_128_MUL   ,                                       // 陀螺仪输出频率 128倍频 (GYRO = Gyroscope 陀螺仪)
}imu963ra_gyro_output_rate_enum;

typedef enum
{
    IMU963RA_GYRO_RANGE_SGN_125DPS  = 2 ,                                       // 陀螺仪量程 ±125DPS  (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU963RA_GYRO_RANGE_SGN_250DPS      ,                                       // 陀螺仪量程 ±250DPS  (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU963RA_GYRO_RANGE_SGN_500DPS      ,                                       // 陀螺仪量程 ±500DPS  (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU963RA_GYRO_RANGE_SGN_1000DPS     ,                                       // 陀螺仪量程 ±1000DPS (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU963RA_GYRO_RANGE_SGN_2000DPS     ,                                       // 陀螺仪量程 ±2000DPS (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    IMU963RA_GYRO_RANGE_SGN_4000DPS     ,                                       // 陀螺仪量程 ±4000DPS (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
}imu963ra_gyro_range_enum;

// IMU963RA_MAG_OUTPUT_RATE_BASE 对应默认的 IMU963RA 的磁力计数据输出频率 实际为 50Hz
// 实际会略有偏差 需要去查看对应的芯片手册 因此对应从低到高可以设置为
// IMU963RA_MAG_OUTPUT_RATE_4_DIV       ->  10      Hz
// IMU963RA_MAG_OUTPUT_RATE_BASE        ->  50      Hz
// IMU963RA_MAG_OUTPUT_RATE_2_MUL       ->  100     Hz
// IMU963RA_MAG_OUTPUT_RATE_4_MUL       ->  200     Hz
typedef enum
{
    IMU963RA_MAG_OUTPUT_RATE_4_DIV  = 6 ,                                       // 磁力计输出频率 4  分频 (MAG = Magnetometer 磁力计)
    IMU963RA_MAG_OUTPUT_RATE_BASE   = 8 ,                                       // 磁力计输出频率基础频率 (MAG = Magnetometer 磁力计)
    IMU963RA_MAG_OUTPUT_RATE_2_MUL      ,                                       // 磁力计输出频率 2  倍频 (MAG = Magnetometer 磁力计)
    IMU963RA_MAG_OUTPUT_RATE_4_MUL      ,                                       // 磁力计输出频率 4  倍频 (MAG = Magnetometer 磁力计)
}imu963ra_mag_output_rate_enum;

typedef enum
{
    IMU963RA_MAG_RANGE_2G           = 2 ,                                       // 磁力计量程 2G (MAG = Magnetometer 磁力计) (G = Gs 高斯)
    IMU963RA_MAG_RANGE_8G               ,                                       // 磁力计量程 8G (MAG = Magnetometer 磁力计) (G = Gs 高斯)
}imu963ra_mag_range_enum;

#define IMU963RA_TIMEOUT_COUNT              ( 32 )                              // IMU963RA 超时计数

#define IMU963RA_ACC_OUTPUT_RATE_DEFAULT    ( IMU963RA_ACC_OUTPUT_RATE_BASE   ) // 在这设置默认的 加速度计  初始化输出频率
#define IMU963RA_ACC_RANGE_DEFAULT          ( IMU963RA_ACC_RANGE_SGN_8G       ) // 在这设置默认的 加速度计  初始化量程
#define IMU963RA_GYRO_OUTPUT_RATE_DEFAULT   ( IMU963RA_GYRO_OUTPUT_RATE_4_MUL ) // 在这设置默认的 陀螺仪    初始化输出频率
#define IMU963RA_GYRO_RANGE_DEFAULT         ( IMU963RA_GYRO_RANGE_SGN_2000DPS ) // 在这设置默认的 陀螺仪    初始化量程
#define IMU963RA_MAG_OUTPUT_RATE_DEFAULT    ( IMU963RA_MAG_OUTPUT_RATE_BASE   ) // 在这设置默认的 磁力计    初始化输出频率
#define IMU963RA_MAG_RANGE_DEFAULT          ( IMU963RA_MAG_RANGE_8G           ) // 在这设置默认的 磁力计    初始化量程

#define IMU963RA_CONFIG_TYPE_OFFSET         ( 4     )                           // 累计低位的位移后从 16bit 开始
#define IMU963RA_CONFIG_TYPE_MASK           ( 0x0F  )                           // 同样根据需要添加的引脚服用功能掩码 这里也是 4bit 因此掩码为 0xF

#define IMU963RA_CONFIG_VALUE_OFFSET        ( 0     )                           // 累计低位的位移后从 16bit 开始
#define IMU963RA_CONFIG_VALUE_MASK          ( 0x0F  )                           // 同样根据需要添加的引脚服用功能掩码 这里也是 4bit 因此掩码为 0xF

typedef enum                                                                    // 枚举 IMU963RA 配置选项 此枚举定义不允许用户修改
{
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_4_DIV           = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_4_DIV   )   ,
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_2_DIV           = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_2_DIV   )   ,
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_BASE            = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_BASE    )   ,
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_2_MUL           = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_2_MUL   )   ,
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_4_MUL           = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_4_MUL   )   ,
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_8_MUL           = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_8_MUL   )   ,
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_16_MUL          = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_16_MUL  )   ,
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_32_MUL          = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_32_MUL  )   ,
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_64_MUL          = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_64_MUL  )   ,
    IMU963RA_CONFIG_ACC_OUTPUT_RATE_128_MUL         = ((0x00 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_OUTPUT_RATE_128_MUL )   ,

    IMU963RA_CONFIG_ACC_RANGE_SGN_2G                = ((0x01 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_RANGE_SGN_2G    )       ,
    IMU963RA_CONFIG_ACC_RANGE_SGN_4G                = ((0x01 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_RANGE_SGN_4G    )       ,
    IMU963RA_CONFIG_ACC_RANGE_SGN_8G                = ((0x01 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_RANGE_SGN_8G    )       ,
    IMU963RA_CONFIG_ACC_RANGE_SGN_16G               = ((0x01 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_ACC_RANGE_SGN_16G   )       ,

    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_4_DIV          = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_4_DIV  )   ,
    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_2_DIV          = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_2_DIV  )   ,
    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_BASE           = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_BASE   )   ,
    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_2_MUL          = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_2_MUL  )   ,
    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_4_MUL          = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_4_MUL  )   ,
    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_8_MUL          = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_8_MUL  )   ,
    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_16_MUL         = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_16_MUL )   ,
    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_32_MUL         = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_32_MUL )   ,
    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_64_MUL         = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_64_MUL )   ,
    IMU963RA_CONFIG_GYRO_OUTPUT_RATE_128_MUL        = ((0x02 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_OUTPUT_RATE_128_MUL)   ,

    IMU963RA_CONFIG_GYRO_RANGE_SGN_125DPS           = ((0x03 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_RANGE_SGN_125DPS   )   ,
    IMU963RA_CONFIG_GYRO_RANGE_SGN_250DPS           = ((0x03 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_RANGE_SGN_250DPS   )   ,
    IMU963RA_CONFIG_GYRO_RANGE_SGN_500DPS           = ((0x03 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_RANGE_SGN_500DPS   )   ,
    IMU963RA_CONFIG_GYRO_RANGE_SGN_1000DPS          = ((0x03 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_RANGE_SGN_1000DPS  )   ,
    IMU963RA_CONFIG_GYRO_RANGE_SGN_2000DPS          = ((0x03 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_RANGE_SGN_2000DPS  )   ,
    IMU963RA_CONFIG_GYRO_RANGE_SGN_4000DPS          = ((0x03 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_GYRO_RANGE_SGN_4000DPS  )   ,

    IMU963RA_CONFIG_MAG_OUTPUT_RATE_4_DIV           = ((0x04 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_MAG_OUTPUT_RATE_4_DIV   )   ,
    IMU963RA_CONFIG_MAG_OUTPUT_RATE_BASE            = ((0x04 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_MAG_OUTPUT_RATE_BASE    )   ,
    IMU963RA_CONFIG_MAG_OUTPUT_RATE_2_MUL           = ((0x04 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_MAG_OUTPUT_RATE_2_MUL   )   ,
    IMU963RA_CONFIG_MAG_OUTPUT_RATE_4_MUL           = ((0x04 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_MAG_OUTPUT_RATE_4_MUL   )   ,

    IMU963RA_CONFIG_MAG_RANGE_2G                    = ((0x05 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_MAG_RANGE_2G    )           ,
    IMU963RA_CONFIG_MAG_RANGE_8G                    = ((0x05 << IMU963RA_CONFIG_TYPE_OFFSET) | IMU963RA_MAG_RANGE_8G    )           ,
}imu963ra_config_item_enum;

typedef enum
{
    IMU963RA_INTERFACE_SPI      ,
    IMU963RA_INTERFACE_SOFT_IIC ,
    IMU963RA_INTERFACE_IIC      ,
}imu963ra_interface_type_enum;

typedef enum
{
    IMU963RA_STATE_NO_ERROR                     ,

    IMU963RA_STATE_DATA_BUFFER_ERROR            ,
    IMU963RA_STATE_INTERFACE_INIT_ERROR         ,
    IMU963RA_STATE_ID_CHECK_ERROR               ,
    IMU963RA_STATE_SELF_CHECK_ERROR             ,
    IMU963RA_STATE_SET_CONFIG_ERROR             ,
    IMU963RA_STATE_SET_ACC_OUTPUT_RATE_ERROR    ,
    IMU963RA_STATE_SET_ACC_RANGE_ERROR          ,
    IMU963RA_STATE_SET_GYRO_OUTPUT_RATE_ERROR   ,
    IMU963RA_STATE_SET_GYRO_RANGE_ERROR         ,
    IMU963RA_STATE_SET_MAG_OUTPUT_RATE_ERROR    ,
    IMU963RA_STATE_SET_MAG_RANGE_ERROR          ,

    IMU963RA_STATE_MAG_WRITE_RIGESTER_ERROR     ,
    IMU963RA_STATE_MAG_ID_CHECK_ERROR           ,
    IMU963RA_STATE_MAG_SET_CONFIG_ERROR         ,
}imu963ra_state_enum;

typedef struct                                                                  // IMU963RA 数据格式模板 用于存储 IMU963RA 的数据
{
    int16       x   ;                                                           // X 轴的原始数据
    int16       y   ;                                                           // Y 轴的原始数据
    int16       z   ;                                                           // Z 轴的原始数据
}imu963ra_measurement_data_struct;

typedef struct                                                                  // IMU963RA 数据格式模板 用于存储 IMU963RA 的数据
{
    float       x   ;                                                           // X 轴的物理量数据
    float       y   ;                                                           // Y 轴的物理量数据
    float       z   ;                                                           // Z 轴的物理量数据
}imu963ra_physical_data_struct;
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处定义 IMU963RA 驱动所需的内部定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// 定义 IMU963RA 内部地址
#define IMU963RA_DEV_ADDR                           ( 0x6B )                    // 7bit 地址格式 SA0接地：0x6A SA0上拉：0x6B 模块默认上拉
#define IMU963RA_SPI_W                              ( 0x00 )
#define IMU963RA_SPI_R                              ( 0x80 )

#define IMU963RA_FUNC_CFG_ACCESS                    ( 0x01 )
#define IMU963RA_PIN_CTRL                           ( 0x02 )
#define IMU963RA_S4S_TPH_L                          ( 0x04 )
#define IMU963RA_S4S_TPH_H                          ( 0x05 )
#define IMU963RA_S4S_RR                             ( 0x06 )
#define IMU963RA_FIFO_CTRL1                         ( 0x07 )
#define IMU963RA_FIFO_CTRL2                         ( 0x08 )
#define IMU963RA_FIFO_CTRL3                         ( 0x09 )
#define IMU963RA_FIFO_CTRL4                         ( 0x0A )
#define IMU963RA_COUNTER_BDR_REG1                   ( 0x0B )
#define IMU963RA_COUNTER_BDR_REG2                   ( 0x0C )
#define IMU963RA_INT1_CTRL                          ( 0x0D )
#define IMU963RA_INT2_CTRL                          ( 0x0E )
#define IMU963RA_WHO_AM_I                           ( 0x0F )
#define IMU963RA_CTRL1_XL                           ( 0x10 )
#define IMU963RA_CTRL2_G                            ( 0x11 )
#define IMU963RA_CTRL3_C                            ( 0x12 )
#define IMU963RA_CTRL4_C                            ( 0x13 )
#define IMU963RA_CTRL5_C                            ( 0x14 )
#define IMU963RA_CTRL6_C                            ( 0x15 )
#define IMU963RA_CTRL7_G                            ( 0x16 )
#define IMU963RA_CTRL8_XL                           ( 0x17 )
#define IMU963RA_CTRL9_XL                           ( 0x18 )
#define IMU963RA_CTRL10_C                           ( 0x19 )
#define IMU963RA_ALL_INT_SRC                        ( 0x1A )
#define IMU963RA_WAKE_UP_SRC                        ( 0x1B )
#define IMU963RA_TAP_SRC                            ( 0x1C )
#define IMU963RA_D6D_SRC                            ( 0x1D )
#define IMU963RA_STATUS_REG                         ( 0x1E )
#define IMU963RA_OUT_TEMP_L                         ( 0x20 )
#define IMU963RA_OUT_TEMP_H                         ( 0x21 )
#define IMU963RA_OUTX_L_G                           ( 0x22 )
#define IMU963RA_OUTX_H_G                           ( 0x23 )
#define IMU963RA_OUTY_L_G                           ( 0x24 )
#define IMU963RA_OUTY_H_G                           ( 0x25 )
#define IMU963RA_OUTZ_L_G                           ( 0x26 )
#define IMU963RA_OUTZ_H_G                           ( 0x27 )
#define IMU963RA_OUTX_L_A                           ( 0x28 )
#define IMU963RA_OUTX_H_A                           ( 0x29 )
#define IMU963RA_OUTY_L_A                           ( 0x2A )
#define IMU963RA_OUTY_H_A                           ( 0x2B )
#define IMU963RA_OUTZ_L_A                           ( 0x2C )
#define IMU963RA_OUTZ_H_A                           ( 0x2D )
#define IMU963RA_EMB_FUNC_STATUS_MAINPAGE           ( 0x35 )
#define IMU963RA_FSM_STATUS_A_MAINPAGE              ( 0x36 )
#define IMU963RA_FSM_STATUS_B_MAINPAGE              ( 0x37 )
#define IMU963RA_STATUS_MASTER_MAINPAGE             ( 0x39 )
#define IMU963RA_FIFO_STATUS1                       ( 0x3A )
#define IMU963RA_FIFO_STATUS2                       ( 0x3B )
#define IMU963RA_TIMESTAMP0                         ( 0x40 )
#define IMU963RA_TIMESTAMP1                         ( 0x41 )
#define IMU963RA_TIMESTAMP2                         ( 0x42 )
#define IMU963RA_TIMESTAMP3                         ( 0x43 )
#define IMU963RA_TAP_CFG0                           ( 0x56 )
#define IMU963RA_TAP_CFG1                           ( 0x57 )
#define IMU963RA_TAP_CFG2                           ( 0x58 )
#define IMU963RA_TAP_THS_6D                         ( 0x59 )
#define IMU963RA_INT_DUR2                           ( 0x5A )
#define IMU963RA_WAKE_UP_THS                        ( 0x5B )
#define IMU963RA_WAKE_UP_DUR                        ( 0x5C )
#define IMU963RA_FREE_FALL                          ( 0x5D )
#define IMU963RA_MD1_CFG                            ( 0x5E )
#define IMU963RA_MD2_CFG                            ( 0x5F )
#define IMU963RA_S4S_ST_CMD_CODE                    ( 0x60 )
#define IMU963RA_S4S_DT_REG                         ( 0x61 )
#define IMU963RA_I3C_BUS_AVB                        ( 0x62 )
#define IMU963RA_INTERNAL_FREQ_FINE                 ( 0x63 )
#define IMU963RA_INT_OIS                            ( 0x6F )
#define IMU963RA_CTRL1_OIS                          ( 0x70 )
#define IMU963RA_CTRL2_OIS                          ( 0x71 )
#define IMU963RA_CTRL3_OIS                          ( 0x72 )
#define IMU963RA_X_OFS_USR                          ( 0x73 )
#define IMU963RA_Y_OFS_USR                          ( 0x74 )
#define IMU963RA_Z_OFS_USR                          ( 0x75 )
#define IMU963RA_FIFO_DATA_OUT_TAG                  ( 0x78 )
#define IMU963RA_FIFO_DATA_OUT_X_L                  ( 0x79 )
#define IMU963RA_FIFO_DATA_OUT_X_H                  ( 0x7A )
#define IMU963RA_FIFO_DATA_OUT_Y_L                  ( 0x7B )
#define IMU963RA_FIFO_DATA_OUT_Y_H                  ( 0x7C )
#define IMU963RA_FIFO_DATA_OUT_Z_L                  ( 0x7D )
#define IMU963RA_FIFO_DATA_OUT_Z_H                  ( 0x7E )

// 集线器功能相关寄存器 需要将FUNC_CFG_ACCESS的SHUB_REG_ACCESS位设置为1才能正确访问
#define IMU963RA_SENSOR_HUB_1                       ( 0x02 ) 
#define IMU963RA_SENSOR_HUB_2                       ( 0x03 ) 
#define IMU963RA_SENSOR_HUB_3                       ( 0x04 ) 
#define IMU963RA_SENSOR_HUB_4                       ( 0x05 ) 
#define IMU963RA_SENSOR_HUB_5                       ( 0x06 ) 
#define IMU963RA_SENSOR_HUB_6                       ( 0x07 ) 
#define IMU963RA_SENSOR_HUB_7                       ( 0x08 ) 
#define IMU963RA_SENSOR_HUB_8                       ( 0x09 ) 
#define IMU963RA_SENSOR_HUB_9                       ( 0x0A ) 
#define IMU963RA_SENSOR_HUB_10                      ( 0x0B ) 
#define IMU963RA_SENSOR_HUB_11                      ( 0x0C ) 
#define IMU963RA_SENSOR_HUB_12                      ( 0x0D ) 
#define IMU963RA_SENSOR_HUB_13                      ( 0x0E ) 
#define IMU963RA_SENSOR_HUB_14                      ( 0x0F ) 
#define IMU963RA_SENSOR_HUB_15                      ( 0x10 ) 
#define IMU963RA_SENSOR_HUB_16                      ( 0x11 ) 
#define IMU963RA_SENSOR_HUB_17                      ( 0x12 ) 
#define IMU963RA_SENSOR_HUB_18                      ( 0x13 ) 
#define IMU963RA_MASTER_CONFIG                      ( 0x14 ) 
#define IMU963RA_SLV0_ADD                           ( 0x15 ) 
#define IMU963RA_SLV0_SUBADD                        ( 0x16 ) 
#define IMU963RA_SLV0_CONFIG                        ( 0x17 ) 
#define IMU963RA_SLV1_ADD                           ( 0x18 ) 
#define IMU963RA_SLV1_SUBADD                        ( 0x19 ) 
#define IMU963RA_SLV1_CONFIG                        ( 0x1A ) 
#define IMU963RA_SLV2_ADD                           ( 0x1B ) 
#define IMU963RA_SLV2_SUBADD                        ( 0x1C ) 
#define IMU963RA_SLV2_CONFIG                        ( 0x1D ) 
#define IMU963RA_SLV3_ADD                           ( 0x1E ) 
#define IMU963RA_SLV3_SUBADD                        ( 0x1F ) 
#define IMU963RA_SLV3_CONFIG                        ( 0x20 ) 
#define IMU963RA_DATAWRITE_SLV0                     ( 0x21 ) 
#define IMU963RA_STATUS_MASTER                      ( 0x22 )

#define IMU963RA_MAG_ADDR                           ( 0x0D )                    // 7位IIC地址
#define IMU963RA_MAG_OUTX_L                         ( 0x00 )
#define IMU963RA_MAG_CONTROL1                       ( 0x09 )
#define IMU963RA_MAG_CONTROL2                       ( 0x0A )
#define IMU963RA_MAG_FBR                            ( 0x0B )
#define IMU963RA_MAG_CHIP_ID                        ( 0x0D )
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// 此处列举 本文件的所有函数声明 [ 其中包括宏定义函数 ] 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 加速度计物理量数据
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     range                   IMU963RA 加速度量程 详见 zf_device_imu963ra.h 中 imu963ra_acc_range_enum 定义
// 参数说明     *physical_data          IMU963RA 的物理量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_get_physical_acc(imu963_data_buffer, range, physical_data);
// 备注信息     根据量程进行物理量换算
//              加速度计量程 ±2  g   获取到的加速度计数据除以 (1000.0 / 0.061) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//              加速度计量程 ±4  g   获取到的加速度计数据除以 (1000.0 / 0.122) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//              加速度计量程 ±8  g   获取到的加速度计数据除以 (1000.0 / 0.244) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//              加速度计量程 ±16 g   获取到的加速度计数据除以 (1000.0 / 0.488) 可以转化为带物理单位的数据 (g 代表重力加速度 物理学名词 一般情况下 g 取 9.8 m/s^2 为标准值)
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_get_physical_acc (imu963ra_measurement_data_struct *measurement_data, imu963ra_acc_range_enum range,imu963ra_physical_data_struct *physical_data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 陀螺仪物理量数据
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     range                   IMU963RA 加速度量程 详见 zf_device_imu963ra.h 中 imu963ra_gyro_range_enum 定义
// 参数说明     *physical_data          IMU963RA 的物理量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_get_physical_gyro(imu963_data_buffer, range, physical_data);
// 备注信息     根据量程进行物理量换算
//              陀螺仪量程 ±125  dps    获取到的陀螺仪数据除以 (1000.0 / 4.375)  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±250  dps    获取到的陀螺仪数据除以 (1000.0 / 8.75 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±500  dps    获取到的陀螺仪数据除以 (1000.0 / 17.5 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±1000 dps    获取到的陀螺仪数据除以 (1000.0 / 35.0 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±2000 dps    获取到的陀螺仪数据除以 (1000.0 / 70.0 )  可以转化为带物理单位的数据 单位为 °/s
//              陀螺仪量程 ±4000 dps    获取到的陀螺仪数据除以 (1000.0 / 140.0)  可以转化为带物理单位的数据 单位为 °/s
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_get_physical_gyro (imu963ra_measurement_data_struct *measurement_data, imu963ra_gyro_range_enum range,imu963ra_physical_data_struct *physical_data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 地磁计物理量数据
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     range                   IMU963RA 加速度量程 详见 zf_device_imu963ra.h 中 imu963ra_mag_range_enum 定义
// 参数说明     *physical_data          IMU963RA 的物理量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_get_physical_mag(imu963_data_buffer, range, physical_data);
// 备注信息     根据量程进行物理量换算
//              地磁计量程 2G   获取到的地磁计数据除以 12000 可以转化为带物理单位的数据 单位为 G(高斯)
//              地磁计量程 8G   获取到的地磁计数据除以  3000 可以转化为带物理单位的数据 单位为 G(高斯)
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_get_physical_mag (imu963ra_measurement_data_struct *measurement_data, imu963ra_mag_range_enum range,imu963ra_physical_data_struct *physical_data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 加速度计数据
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_get_acc(imu963_data_buffer);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_get_acc (imu963ra_measurement_data_struct *measurement_data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 陀螺仪数据
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_get_gyro(imu963_data_buffer);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_get_gyro (imu963ra_measurement_data_struct *measurement_data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 地磁计数据
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_get_mag(imu963_data_buffer);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_get_mag (imu963ra_measurement_data_struct *measurement_data);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 配置模块工作参数
// 参数说明     item                    IMU963RA 配置参照 zf_device_imu963ra.h 中 imu963ra_config_item_enum 枚举体定义
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_set_config(item);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_set_config (imu963ra_config_item_enum item);

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 IMU963RA
// 参数说明     void
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_init(imu963_data_buffer);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_init (void);
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#endif
