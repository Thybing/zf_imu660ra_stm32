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

// zf_device 层头文件引用

#include "zf_device_imu_interface.h"

// 自身头文件
#include "zf_device_imu963ra.h"

// 此处定义 本文件的所有函数具体定义 这里不允许用户修改
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Part Start >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 作为 IIC 主机向磁力计写数据
// 参数说明     *interface              IMU963RA 的通信接口对象指针
// 参数说明     addr                    目标地址
// 参数说明     reg                     目标寄存器
// 参数说明     data                    数据
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_write_mag_register(interface, addr, reg, data);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static imu963ra_state_enum imu963ra_write_mag_register (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 data)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    uint16 timeout_count = 0;
    do{
        addr = addr << 1;
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG   , 0x00      ); // 从机0配置清除
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD      , addr | 0  ); // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD   , reg       ); // 需要写入的寄存器地址
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_DATAWRITE_SLV0, data      ); // 需要写入的数据
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG , 0x4C      ); // 仅在第一个周期启用通讯 开启上拉 I2C主机使能

        // 等待通讯成功
        while(!(0x80 & interface->read_8bit_register(
            IMU963RA_DEV_ADDR, 
                (IMU_INTERFACE_SPI == interface->interface_type)
                ? (IMU963RA_STATUS_MASTER | IMU963RA_SPI_R)
                : IMU963RA_STATUS_MASTER)))
        {
            if(IMU963RA_TIMEOUT_COUNT < timeout_count ++)
            {
                return_state = IMU963RA_STATE_MAG_WRITE_RIGESTER_ERROR;
                break;
            }
            interface->delay_ms(2);
        }
    }while(0);
    return return_state;
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 作为 IIC 主机向磁力计读数据
// 参数说明     *interface              IMU963RA 的通信接口对象指针
// 参数说明     addr                    目标地址
// 参数说明     reg                     目标寄存器
// 参数说明     *data                   数据指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_read_mag_register(IMU963RA_MAG_ADDR, IMU963RA_MAG_CHIP_ID);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static imu963ra_state_enum imu963ra_read_mag_register (imu_interface_obj_struct *interface, uint8 addr, uint8 reg, uint8 *data)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    uint16              timeout_count = 0;

    do{
        addr = addr << 1;
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD,        addr | 1);  // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD,     reg);       // 需要读取的寄存器地址
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG,     0x01);    
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG,   0x4C);      // 仅在第一个周期启用通讯 开启上拉 I2C主机使能
        
        // 等待通讯成功
        while(!(0x01 & interface->read_8bit_register(
            IMU963RA_DEV_ADDR, 
                (IMU_INTERFACE_SPI == interface->interface_type)
                ? (IMU963RA_STATUS_MASTER | IMU963RA_SPI_R)
                : IMU963RA_STATUS_MASTER)))
        {
            if(IMU963RA_TIMEOUT_COUNT < timeout_count ++)
            {
                return_state = IMU963RA_STATE_MAG_WRITE_RIGESTER_ERROR;
                break;
            }
            interface->delay_ms(2);
        }

        *data = interface->read_8bit_register(
            IMU963RA_DEV_ADDR, 
                (IMU_INTERFACE_SPI == interface->interface_type)
                ? (IMU963RA_SENSOR_HUB_1 | IMU963RA_SPI_R)
                : IMU963RA_SENSOR_HUB_1);
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 加速度计数据 拓展接口
// 参数说明     *interface              IMU963RA 的通信接口对象指针
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_expansion_get_acc(interface, data_buffer);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static imu963ra_state_enum imu963ra_expansion_get_acc (imu_interface_obj_struct *interface, imu963ra_measurement_data_struct *measurement_data)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    do{
        if( interface->assert(
                NULL != measurement_data,                                       // 缓冲区指针 不能为空
                "*measurement_data error.",                                     // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_DATA_BUFFER_ERROR;
            break;
        }
        uint8 data[6];
        interface->read_8bit_registers(
            IMU963RA_DEV_ADDR, 
                (IMU_INTERFACE_SPI == interface->interface_type)
                ?   (IMU963RA_OUTX_L_A | IMU963RA_SPI_R)
                :   IMU963RA_OUTX_L_A,
            data, 6);
        measurement_data->x = (int16)(((uint16)data[1] << 8 | data[0]));
        measurement_data->y = (int16)(((uint16)data[3] << 8 | data[2]));
        measurement_data->z = (int16)(((uint16)data[5] << 8 | data[4]));
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 陀螺仪数据 拓展接口
// 参数说明     *interface              IMU963RA 的通信接口对象指针
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_expansion_get_gyro(interface, data_buffer);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static imu963ra_state_enum imu963ra_expansion_get_gyro (imu_interface_obj_struct *interface, imu963ra_measurement_data_struct *measurement_data)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    do{
        if( interface->assert(
                NULL != measurement_data,                                       // 缓冲区指针 不能为空
                "*measurement_data error.",                                     // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_DATA_BUFFER_ERROR;
            break;
        }
        uint8 data[6];
        interface->read_8bit_registers(
            IMU963RA_DEV_ADDR, 
                (IMU_INTERFACE_SPI == interface->interface_type)
                ?   (IMU963RA_OUTX_L_G | IMU963RA_SPI_R)
                :   IMU963RA_OUTX_L_G,
            data, 6);
        measurement_data->x = (int16)(((uint16)data[1] << 8 | data[0]));
        measurement_data->y = (int16)(((uint16)data[3] << 8 | data[2]));
        measurement_data->z = (int16)(((uint16)data[5] << 8 | data[4]));
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 地磁计数据 拓展接口
// 参数说明     *interface              IMU963RA 的通信接口对象指针
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_expansion_get_mag(interface, data_buffer);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static imu963ra_state_enum imu963ra_expansion_get_mag (imu_interface_obj_struct *interface, imu963ra_measurement_data_struct *measurement_data)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    uint16 timeout_count = 0;
    do{
        if( interface->assert(
                NULL != measurement_data,                                       // 缓冲区指针 不能为空
                "*measurement_data error.",                                     // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_DATA_BUFFER_ERROR;
            break;
        }
        uint8 data[6];
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
        do
        {
            if(0x01 & interface->read_8bit_register(
                IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_SPI == interface->interface_type)
                    ? (IMU963RA_STATUS_MASTER | IMU963RA_SPI_R)
                    : IMU963RA_STATUS_MASTER))
            {
                interface->read_8bit_registers(
                    IMU963RA_DEV_ADDR, 
                        (IMU_INTERFACE_SPI == interface->interface_type)
                        ?   (IMU963RA_SENSOR_HUB_1 | IMU963RA_SPI_R)
                        :   IMU963RA_SENSOR_HUB_1,
                    data, 6);
                measurement_data->x = (int16)(((uint16)data[1] << 8 | data[0]));
                measurement_data->y = (int16)(((uint16)data[3] << 8 | data[2]));
                measurement_data->z = (int16)(((uint16)data[5] << 8 | data[4]));
                break;
            }
            interface->delay_ms(2);
        }while(IMU963RA_TIMEOUT_COUNT < timeout_count ++);
        if(IMU963RA_TIMEOUT_COUNT < timeout_count)
        {
            return_state = IMU963RA_STATE_ID_CHECK_ERROR;
        }
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 配置模块工作参数 拓展接口
// 参数说明     *interface              IMU963RA 的通信接口对象指针
// 参数说明     item                    IMU963RA 配置参照 zf_device_imu963ra.h 中 imu963ra_config_item_enum 枚举体定义
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_expansion_set_config(interface, interface);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static imu963ra_state_enum imu963ra_expansion_set_config (imu_interface_obj_struct *interface, imu963ra_config_item_enum item)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    uint8               register_temp   = 0;
    do{
        switch((item >> IMU963RA_CONFIG_TYPE_OFFSET) & IMU963RA_CONFIG_TYPE_MASK)
        {
            case 0x00:
            {
                if( interface->assert(
                            IMU963RA_ACC_OUTPUT_RATE_4_DIV      <= (item & 0x0F)
                        &&  IMU963RA_ACC_OUTPUT_RATE_128_MUL    >= (item & 0x0F),
                        "item set error.",                                      // 输出预设报错信息
                        ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
                {
                    return_state = IMU963RA_STATE_SET_ACC_OUTPUT_RATE_ERROR;
                    break;
                }
                register_temp = interface->read_8bit_register(
                IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_SPI == interface->interface_type)
                    ? (IMU963RA_CTRL1_XL | IMU963RA_SPI_R)
                    : IMU963RA_CTRL1_XL);
                register_temp = ((register_temp & ~0xF0) |(((item & 0x0F) - 5) << 4));
                interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL1_XL, register_temp);
            }break;
            case 0x01:
            {
                if( interface->assert(
                            IMU963RA_ACC_RANGE_SGN_2G <= (item & 0x0F)
                        &&  IMU963RA_ACC_RANGE_SGN_16G >= (item & 0x0F),
                        "item set error.",                                      // 输出预设报错信息
                        ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
                {
                    return_state = IMU963RA_STATE_SET_ACC_RANGE_ERROR;
                    break;
                }
                register_temp = interface->read_8bit_register(
                IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_SPI == interface->interface_type)
                    ? (IMU963RA_CTRL1_XL | IMU963RA_SPI_R)
                    : IMU963RA_CTRL1_XL);
                const uint8 acc_range_list[6] = {0x00, 0x00, 0x00, 0x08, 0x0C, 0x04};
                register_temp = ((register_temp & ~0x0C) | acc_range_list[(item & 0x0F)]);
                interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL1_XL, register_temp);
            }break;
            case 0x02:
            {
                if( interface->assert(
                            IMU963RA_GYRO_OUTPUT_RATE_4_DIV     <= (item & 0x0F)
                        &&  IMU963RA_GYRO_OUTPUT_RATE_128_MUL   >= (item & 0x0F),
                        "item set error.",                                      // 输出预设报错信息
                        ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
                {
                    return_state = IMU963RA_STATE_SET_GYRO_OUTPUT_RATE_ERROR;
                    break;
                }
                register_temp = interface->read_8bit_register(
                IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_SPI == interface->interface_type)
                    ? (IMU963RA_CTRL2_G | IMU963RA_SPI_R)
                    : IMU963RA_CTRL2_G);
                register_temp = ((register_temp & ~0xF0) |(((item & 0x0F) - 5) << 4));
                interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL2_G, register_temp);
            }break;
            case 0x03:
            {
                if( interface->assert(
                            IMU963RA_GYRO_RANGE_SGN_125DPS  <= (item & 0x0F)
                        &&  IMU963RA_GYRO_RANGE_SGN_4000DPS >= (item & 0x0F),
                        "item set error.",                                      // 输出预设报错信息
                        ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
                {
                    return_state = IMU963RA_STATE_SET_GYRO_RANGE_ERROR;
                    break;
                }
                register_temp = interface->read_8bit_register(
                IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_SPI == interface->interface_type)
                    ? (IMU963RA_CTRL2_G | IMU963RA_SPI_R)
                    : IMU963RA_CTRL2_G);
                const uint8 gyro_range_list[8] = {0x00, 0x00, 0x02, 0x00, 0x04, 0x08, 0x0C, 0x01};
                register_temp = ((register_temp & ~0x0F) | gyro_range_list[(item & 0x0F)]);
                interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL2_G, register_temp);
            }break;
            case 0x04:
            {
                if( interface->assert(
                            IMU963RA_MAG_OUTPUT_RATE_4_DIV  <= (item & 0x0F)
                        &&  IMU963RA_MAG_OUTPUT_RATE_4_MUL  >= (item & 0x0F),
                        "item set error.",                                      // 输出预设报错信息
                        ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
                {
                    return_state = IMU963RA_STATE_SET_MAG_OUTPUT_RATE_ERROR;
                    break;
                }
                interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
                do
                {
                    imu963ra_read_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, &register_temp);
                    register_temp = (register_temp & ~0x0C)
                        | (((item & 0x0F) == IMU963RA_MAG_OUTPUT_RATE_4_DIV) ? (0) : (((item & 0x0F) - 7) << 2));
                    imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, register_temp | 0x01);

                    interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD, (IMU963RA_MAG_ADDR << 1) | 1); // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
                    interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD, IMU963RA_MAG_OUTX_L);       // 需要读取的寄存器地址
                    interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG, 0x06);    
                    interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x6C);                    // 仅在第一个周期启用通讯 开启上拉 I2C主机使能
                }while(0);
                interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
            }break;
            case 0x05:
            {
                if( interface->assert(
                            IMU963RA_MAG_RANGE_2G   <= (item & 0x0F)
                        &&  IMU963RA_MAG_RANGE_8G   >= (item & 0x0F),
                        "item set error.",                                      // 输出预设报错信息
                        ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
                {
                    return_state = IMU963RA_STATE_SET_MAG_RANGE_ERROR;
                    break;
                }
                interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
                do
                {
                    imu963ra_read_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, &register_temp);
                    register_temp = (register_temp & ~0x30) | (((item & 0x0F) - 2) << 4);
                    imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, register_temp | 0x01);

                    interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD, (IMU963RA_MAG_ADDR << 1) | 1); // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
                    interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD, IMU963RA_MAG_OUTX_L);       // 需要读取的寄存器地址
                    interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG, 0x06);    
                    interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x6C);                    // 仅在第一个周期启用通讯 开启上拉 I2C主机使能
                }while(0);
                interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
            }break;
            default:
            {
                if( interface->assert(
                        0,
                        "item set error.",                                      // 输出预设报错信息
                        ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
                {
                    return_state = IMU963RA_STATE_SET_CONFIG_ERROR;
                    break;
                }
            }break;
        }
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 六轴 检测 ID 是否匹配 拓展接口
// 参数说明     *interface              IMU963RA 的通信接口对象指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_expansion_ckeck_id(interface);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static imu963ra_state_enum imu963ra_expansion_ckeck_id (imu_interface_obj_struct *interface)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    uint8 data = 0;
    uint16 timeout_count = 0;

    do
    {
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL3_C, 0x01);               // 复位设备
        interface->delay_ms(2);                             
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
        do
        {
            if(IMU963RA_TIMEOUT_COUNT < timeout_count ++)
            {
                return_state = IMU963RA_STATE_ID_CHECK_ERROR;
                break;
            }
            data = interface->read_8bit_register(
                IMU963RA_DEV_ADDR, 
                    (IMU_INTERFACE_SPI == interface->interface_type)
                    ? (IMU963RA_WHO_AM_I | IMU963RA_SPI_R)
                    : IMU963RA_WHO_AM_I);
            interface->delay_ms(10);
        }while(0x6B != data);

        // 必须要给 加速度计 或 地磁计 任意一个设置输出速率 否则六轴处于 power-down 模式不能跟地磁计通信
        // interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL1_XL, 0x10);
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL2_G,  0x10);

        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x80);         // 复位I2C主机
        interface->delay_ms(2);
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x00);         // 清除复位标志
        interface->delay_ms(2);
        imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL2, 0x80);// 复位连接的外设
        interface->delay_ms(2);
        imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL2, 0x00);
        interface->delay_ms(2);

        data = 0;
        imu963ra_read_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CHIP_ID, &data);
        if(0xff != data)
        {
            return_state = IMU963RA_STATE_ID_CHECK_ERROR;
        }
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 IMU963RA 拓展接口
// 参数说明     *interface              IMU963RA 的通信接口对象指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_expansion_init(interface);
// 备注信息     本接口为传感器框架接口 并不直接对用户开放
//-------------------------------------------------------------------------------------------------------------------
static imu963ra_state_enum imu963ra_expansion_init (imu_interface_obj_struct *interface)
{
    imu963ra_state_enum return_state    = IMU963RA_STATE_NO_ERROR;
    uint8               register_temp   = 0;
    do{
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_INT1_CTRL, 0x03);    // 开启陀螺仪 加速度数据就绪中断

        if( interface->assert(
                    IMU963RA_ACC_OUTPUT_RATE_4_DIV      <= IMU963RA_ACC_OUTPUT_RATE_DEFAULT
                &&  IMU963RA_ACC_OUTPUT_RATE_128_MUL    >= IMU963RA_ACC_OUTPUT_RATE_DEFAULT,
                "IMU963RA_ACC_OUTPUT_RATE_DEFAULT set error.",                  // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_SET_ACC_OUTPUT_RATE_ERROR;
            break;
        }
        register_temp = ((IMU963RA_ACC_OUTPUT_RATE_DEFAULT - 5) << 4);
        if( interface->assert(
                    IMU963RA_ACC_RANGE_SGN_2G   <= IMU963RA_ACC_RANGE_DEFAULT
                &&  IMU963RA_ACC_RANGE_SGN_16G  >= IMU963RA_ACC_RANGE_DEFAULT,
                "IMU963RA_ACC_RANGE_DEFAULT set error.",                  // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_SET_ACC_RANGE_ERROR;
            break;
        }
        const uint8 acc_range_list[6] = {0x00, 0x00, 0x00, 0x08, 0x0C, 0x04};
        register_temp |= acc_range_list[IMU963RA_ACC_RANGE_DEFAULT];
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL1_XL, register_temp);

        if( interface->assert(
                    IMU963RA_GYRO_OUTPUT_RATE_4_DIV     <= IMU963RA_GYRO_OUTPUT_RATE_DEFAULT
                &&  IMU963RA_GYRO_OUTPUT_RATE_128_MUL   >= IMU963RA_GYRO_OUTPUT_RATE_DEFAULT,
                "IMU963RA_GYRO_OUTPUT_RATE_DEFAULT set error.",                  // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_SET_GYRO_OUTPUT_RATE_ERROR;
            break;
        }
        register_temp = ((IMU963RA_GYRO_OUTPUT_RATE_DEFAULT - 5) << 4);
        if( interface->assert(
                    IMU963RA_GYRO_RANGE_SGN_125DPS  <= IMU963RA_GYRO_RANGE_DEFAULT
                &&  IMU963RA_GYRO_RANGE_SGN_4000DPS >= IMU963RA_GYRO_RANGE_DEFAULT,
                "IMU963RA_GYRO_RANGE_DEFAULT set error.",                  // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_SET_GYRO_RANGE_ERROR;
            break;
        }
        const uint8 gyro_range_list[8] = {0x00, 0x00, 0x02, 0x00, 0x04, 0x08, 0x0C, 0x01};
        register_temp |= gyro_range_list[IMU963RA_GYRO_RANGE_DEFAULT];
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL2_G, register_temp);

        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL3_C, 0x44);               // 使能陀螺仪数字低通滤波器
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL4_C, 0x02);               // 使能数字低通滤波器
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL5_C, 0x00);               // 加速度计与陀螺仪四舍五入
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL6_C, 0x00);               // 开启加速度计高性能模式 陀螺仪低通滤波 133hz
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL7_G, 0x00);               // 开启陀螺仪高性能模式 关闭高通滤波
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_CTRL9_XL, 0x01);              // 关闭I3C接口

        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x40);
        do
        {
            if( interface->assert(
                        IMU963RA_MAG_OUTPUT_RATE_4_DIV  <= IMU963RA_MAG_OUTPUT_RATE_DEFAULT
                    &&  IMU963RA_MAG_OUTPUT_RATE_4_MUL  >= IMU963RA_MAG_OUTPUT_RATE_DEFAULT,
                    "IMU963RA_MAG_OUTPUT_RATE_DEFAULT set error.",                  // 输出预设报错信息
                    ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
            {
                return_state = IMU963RA_STATE_SET_MAG_OUTPUT_RATE_ERROR;
                break;
            }
            register_temp =
                    (IMU963RA_MAG_OUTPUT_RATE_DEFAULT == IMU963RA_MAG_OUTPUT_RATE_4_DIV)
                ?   (0)
                :   (IMU963RA_MAG_OUTPUT_RATE_DEFAULT - 7);
            register_temp = (register_temp << 2);
            if( interface->assert(
                        IMU963RA_MAG_RANGE_2G   <= IMU963RA_MAG_RANGE_DEFAULT
                    &&  IMU963RA_MAG_RANGE_8G   >= IMU963RA_MAG_RANGE_DEFAULT,
                    "IMU963RA_MAG_RANGE_DEFAULT set error.",                  // 输出预设报错信息
                    ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
            {
                return_state = IMU963RA_STATE_SET_MAG_RANGE_ERROR;
                break;
            }
            register_temp |= ((IMU963RA_MAG_RANGE_DEFAULT - 2) << 4);
            imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_CONTROL1, register_temp | 0x01);
            imu963ra_write_mag_register(interface, IMU963RA_MAG_ADDR, IMU963RA_MAG_FBR, 0x01);

            interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_ADD, (IMU963RA_MAG_ADDR << 1) | 1); // 设置地磁计地址（注意这里需要设置8位的I2C地址） 0x2C
            interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_SUBADD, IMU963RA_MAG_OUTX_L);       // 需要读取的寄存器地址
            interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_SLV0_CONFIG, 0x06);    
            interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_MASTER_CONFIG, 0x6C);                    // 仅在第一个周期启用通讯 开启上拉 I2C主机使能
        }while(0);
        interface->write_8bit_register(IMU963RA_DEV_ADDR, IMU963RA_FUNC_CFG_ACCESS, 0x00);
    }while(0);
    return return_state;
}

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
imu963ra_state_enum imu963ra_get_physical_acc (imu963ra_measurement_data_struct *measurement_data, imu963ra_acc_range_enum range,imu963ra_physical_data_struct *physical_data)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    do{
        if( imu_assert(
                NULL != measurement_data,                                       // 缓冲区指针 不能为空
                "*measurement_data error.",                                     // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_DATA_BUFFER_ERROR;
            break;
        }
        if( imu_assert(
                    IMU963RA_ACC_RANGE_SGN_2G   <= range
                &&  IMU963RA_ACC_RANGE_SGN_16G  >= range,
                "range error.",                                                 // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_SET_ACC_RANGE_ERROR;
            break;
        }
        if( imu_assert(
                NULL != physical_data,                                          // 缓冲区指针 不能为空
                "*physical_data error.",                                        // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_DATA_BUFFER_ERROR;
            break;
        }
        const float transition_factor[4] = {1000.0 / 0.061, 1000.0 / 0.122, 1000.0 / 0.244, 1000.0 / 0.488};
        physical_data->x  = (float)measurement_data->x / transition_factor[range - IMU963RA_ACC_RANGE_SGN_2G];
        physical_data->y  = (float)measurement_data->y / transition_factor[range - IMU963RA_ACC_RANGE_SGN_2G];
        physical_data->z  = (float)measurement_data->z / transition_factor[range - IMU963RA_ACC_RANGE_SGN_2G];
    }while(0);
    return return_state;
}

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
imu963ra_state_enum imu963ra_get_physical_gyro (imu963ra_measurement_data_struct *measurement_data, imu963ra_gyro_range_enum range,imu963ra_physical_data_struct *physical_data)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    do{
        if( imu_assert(
                NULL != measurement_data,                                       // 缓冲区指针 不能为空
                "*measurement_data error.",                                     // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_DATA_BUFFER_ERROR;
            break;
        }
        if( imu_assert(
                    IMU963RA_GYRO_RANGE_SGN_125DPS  <= range
                &&  IMU963RA_GYRO_RANGE_SGN_4000DPS >= range,
                "range error.",                                                 // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_SET_GYRO_RANGE_ERROR;
            break;
        }
        if( imu_assert(
                NULL != physical_data,                                          // 缓冲区指针 不能为空
                "*physical_data error.",                                        // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_DATA_BUFFER_ERROR;
            break;
        }
        const float transition_factor[6] = 
        {
            1000.0 / 4.375, 1000.0 / 8.75,  1000.0 / 17.5,
            1000.0 / 35.0,  1000.0 / 70.0,  1000.0 / 140.0,
        };
        physical_data->x  = (float)measurement_data->x / transition_factor[range - IMU963RA_GYRO_RANGE_SGN_125DPS];
        physical_data->y  = (float)measurement_data->y / transition_factor[range - IMU963RA_GYRO_RANGE_SGN_125DPS];
        physical_data->z  = (float)measurement_data->z / transition_factor[range - IMU963RA_GYRO_RANGE_SGN_125DPS];
    }while(0);
    return return_state;
}

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
imu963ra_state_enum imu963ra_get_physical_mag (imu963ra_measurement_data_struct *measurement_data, imu963ra_mag_range_enum range,imu963ra_physical_data_struct *physical_data)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    do{
        if( imu_assert(
                NULL != measurement_data,                                       // 缓冲区指针 不能为空
                "*measurement_data error.",                                     // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_DATA_BUFFER_ERROR;
            break;
        }
        if( imu_assert(
                    IMU963RA_MAG_RANGE_2G   <= range
                &&  IMU963RA_MAG_RANGE_8G   >= range,
                "range error.",                                                 // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_SET_MAG_RANGE_ERROR;
            break;
        }
        if( imu_assert(
                NULL != physical_data,                                          // 缓冲区指针 不能为空
                "*physical_data error.",                                        // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_DATA_BUFFER_ERROR;
            break;
        }
        const float transition_factor[2] = {12000, 3000};
        physical_data->x  = (float)measurement_data->x / transition_factor[range - IMU963RA_MAG_RANGE_2G];
        physical_data->y  = (float)measurement_data->y / transition_factor[range - IMU963RA_MAG_RANGE_2G];
        physical_data->z  = (float)measurement_data->z / transition_factor[range - IMU963RA_MAG_RANGE_2G];
    }while(0);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 加速度计数据
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_get_acc(imu963_data_buffer);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_get_acc (imu963ra_measurement_data_struct *measurement_data)
{
    return imu963ra_expansion_get_acc(&imu_interface_default_obj, measurement_data);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 陀螺仪数据
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_get_gyro(imu963_data_buffer);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_get_gyro (imu963ra_measurement_data_struct *measurement_data)
{
    return imu963ra_expansion_get_gyro(&imu_interface_default_obj, measurement_data);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 IMU963RA 地磁计数据
// 参数说明     *measurement_data       IMU963RA 的测量数据缓冲指针
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_get_mag(imu963_data_buffer);
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_get_mag (imu963ra_measurement_data_struct *measurement_data)
{
    return imu963ra_expansion_get_mag(&imu_interface_default_obj, measurement_data);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     IMU963RA 配置模块工作参数
// 参数说明     item                    IMU963RA 配置参照 zf_device_imu963ra.h 中 imu963ra_config_item_enum 枚举体定义
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_set_config(item);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_set_config (imu963ra_config_item_enum item)
{
    return imu963ra_expansion_set_config(&imu_interface_default_obj, item);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 IMU963RA
// 参数说明     void
// 返回参数     imu963ra_state_enum     IMU963RA 状态码 详见 zf_device_imu963ra.h 中 imu963ra_state_enum 定义
// 使用示例     imu963ra_init(imu963_data_buffer);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
imu963ra_state_enum imu963ra_init (void)
{
    imu963ra_state_enum return_state = IMU963RA_STATE_NO_ERROR;
    imu_delay_ms(20);                                                           // 等待设备上电成功
    do{
        if( imu_assert(
                !imu_interface_init(),                                          // 如果没有异常 那么接口初始化返回为 0
                "IMU interface init error.",                                    // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            return_state = IMU963RA_STATE_INTERFACE_INIT_ERROR;
            break;
        }

        if( imu_assert(
                !imu963ra_expansion_ckeck_id(&imu_interface_default_obj),       // IMU963RA 自检 如果没有异常 那么接口初始化返回为 0
                "imu963ra self check error.",                                   // 输出预设报错信息
                ZF_FILE_MESSAGE, ZF_LINE_MESSAGE))
        {
            // 如果程序在输出了断言信息 并且提示出错位置在这里
            // 那么就是 IMU963RA 自检出错并超时退出了
            // 检查一下接线有没有问题 如果没问题可能就是坏了
            return_state = IMU963RA_STATE_SELF_CHECK_ERROR;
            break;
        }

        return_state = imu963ra_expansion_init(&imu_interface_default_obj);
    }while(0);
    if(return_state)
    {
        imu_interface_deinit();
    }
    return return_state;
}
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Part   End <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
