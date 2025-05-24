/*********************************************************************************************************************
 * MM32F527X-E9P Opensource Library 即（MM32F527X-E9P 开源库）是一个基于官方 SDK 接口的第三方开源库
 * Copyright (c) 2022 SEEKFREE 逐飞科技
 *
 * 本文件是 MM32F527X-E9P 开源库的一部分
 *
 * MM32F527X-E9P 开源库 是免费软件
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
 * 文件名称          zf_driver_soft_iic
 * 公司名称          成都逐飞科技有限公司
 * 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
 * 开发环境          MDK 5.37
 * 适用平台          MM32F527X_E9P
 * 店铺链接          https://seekfree.taobao.com/
 *
 * 修改记录
 * 日期              作者                备注
 * 2022-08-10        Teternal            first version
 ********************************************************************************************************************/

#include "soft_iic/zf_driver_soft_iic.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

#define SOFT_IIC_SDA_IO_SWITCH (0)  // 是否需要 SDA 进行 I/O 切换 0-不需要 1-需要

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 延时
// 参数说明     delay           延时次数
// 返回参数     void
// 使用示例     soft_iic_delay(1);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
#define soft_iic_delay(x) for (volatile uint32_t i = x; i--;)

#define gpio_high(GPIOx, GPIO_Pin) ((GPIOx)->BSRR |= (GPIO_Pin))

#define gpio_low(GPIOx, GPIO_Pin) ((GPIOx)->BRR |= (GPIO_Pin))

#define gpio_set_input(GPIOx, GPIO_Pin)                  \
    do {                                                 \
        GPIO_InitTypeDef GPIO_InitStructure = {0};       \
        GPIO_InitStructure.Pin = (GPIO_Pin);             \
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;       \
        GPIO_InitStructure.Pull = GPIO_PULLUP;           \
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; \
        HAL_GPIO_Init((GPIOx), &GPIO_InitStructure);     \
    } while (0)

#define gpio_set_output(GPIOx, GPIO_Pin)                 \
    do {                                                 \
        GPIO_InitTypeDef GPIO_InitStructure = {0};       \
        GPIO_InitStructure.Pin = (GPIO_Pin);             \
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;   \
        GPIO_InitStructure.Pull = GPIO_NOPULL;           \
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; \
        HAL_GPIO_Init((GPIOx), &GPIO_InitStructure);     \
    } while (0)

#define gpio_get_level(GPIOx, GPIO_Pin) ((HAL_GPIO_ReadPin((GPIOx), (GPIO_Pin)) == GPIO_PIN_SET) ? 1 : 0)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC START 信号
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 返回参数     void
// 使用示例     soft_iic_start(soft_iic_obj);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_start(soft_iic_info_struct *soft_iic_obj) {
    gpio_high(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 高电平
    gpio_high(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 高电平

    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 先拉低
    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 再拉低
    soft_iic_delay(soft_iic_obj->delay);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC STOP 信号
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 返回参数     void
// 使用示例     soft_iic_stop(soft_iic_obj);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_stop(soft_iic_info_struct *soft_iic_obj) {
    gpio_low(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 低电平
    gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 低电平

    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 先拉高
    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 再拉高
    soft_iic_delay(soft_iic_obj->delay);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 发送 ACK/NAKC 信号 内部调用
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     ack             ACK 电平
// 返回参数     void
// 使用示例     soft_iic_send_ack(soft_iic_obj, 1);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void soft_iic_send_ack(soft_iic_info_struct *soft_iic_obj, uint8_t ack) {
    gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 低电平

    if (ack) {
        gpio_high(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 拉高
    } else {
        gpio_low(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 拉低
    }

    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 拉高
    soft_iic_delay(soft_iic_obj->delay);
    gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);   // SCL 拉低
    gpio_high(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 拉高
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 获取 ACK/NAKC 信号
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 返回参数     uint8           ACK 状态
// 使用示例     soft_iic_wait_ack(soft_iic_obj);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8_t soft_iic_wait_ack(soft_iic_info_struct *soft_iic_obj) {
    uint8_t temp = 0;
    gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);   // SCL 低电平
    gpio_high(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 高电平 释放 SDA

    gpio_set_input(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 设置为输入模式
    soft_iic_delay(soft_iic_obj->delay);

    gpio_high(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 高电平
    soft_iic_delay(soft_iic_obj->delay);

    if (gpio_get_level(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin)) {
        temp = 1;
    }
    gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 低电平

    gpio_set_output(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);
    soft_iic_delay(soft_iic_obj->delay);

    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 发送 8bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     data            数据
// 返回参数     uint8           ACK 状态
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8_t soft_iic_send_data(soft_iic_info_struct *soft_iic_obj, const uint8_t data) {
    uint8_t temp = 0x80;

    while (temp) {
        //        gpio_set_level(soft_iic_obj->sda_pin, data & temp);
        ((data & temp) ? (gpio_high(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin))
                       : (gpio_low(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin)));
        temp >>= 1;

        soft_iic_delay(soft_iic_obj->delay / 2);
        gpio_high(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 拉高
        soft_iic_delay(soft_iic_obj->delay);
        gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 拉低
        soft_iic_delay(soft_iic_obj->delay / 2);
    }
    return ((soft_iic_wait_ack(soft_iic_obj) == 1) ? 0 : 1);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 读取 8bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     ack             ACK 或 NACK
// 返回参数     uint8           数据
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static uint8_t soft_iic_read_data(soft_iic_info_struct *soft_iic_obj, uint8_t ack) {
    uint8_t data = 0x00;
    uint8_t temp = 8;
    gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 低电平
    soft_iic_delay(soft_iic_obj->delay);
    gpio_high(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);       // SDA 高电平 释放 SDA
    gpio_set_input(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 设置为输入模式

    while (temp--) {
        gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 拉低
        soft_iic_delay(soft_iic_obj->delay);
        gpio_high(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);  // SCL 拉高
        soft_iic_delay(soft_iic_obj->delay);
        data = ((data << 1) | gpio_get_level(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin));
    }
    gpio_low(soft_iic_obj->SCL_GPIOx, soft_iic_obj->SCL_GPIO_Pin);         // SCL 低电平
    gpio_set_output(soft_iic_obj->SDA_GPIOx, soft_iic_obj->SDA_GPIO_Pin);  // SDA 设置为输出模式

    soft_iic_delay(soft_iic_obj->delay);
    soft_iic_send_ack(soft_iic_obj, ack);
    return data;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口写 8bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     data            要写入的数据
// 返回参数     void
// 使用示例     soft_iic_write_8bit_register(soft_iic_obj, 0x01);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit(soft_iic_info_struct *soft_iic_obj, const uint8_t data) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口写 8bit 数组
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     soft_iic_write_8bit_array(soft_iic_obj, data, 6);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_array(soft_iic_info_struct *soft_iic_obj, const uint8_t *data, uint32_t len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    while (len--) {
        soft_iic_send_data(soft_iic_obj, *data++);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口器写 16bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     data            要写入的数据
// 返回参数     void
// 使用示例     soft_iic_write_16bit(soft_iic_obj, 0x0101);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit(soft_iic_info_struct *soft_iic_obj, const uint16_t data) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((data & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(data & 0x00FF));
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口写 16bit 数组
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     soft_iic_write_16bit_array(soft_iic_obj, data, 6);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_array(soft_iic_info_struct *soft_iic_obj, const uint16_t *data, uint32_t len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    while (len--) {
        soft_iic_send_data(soft_iic_obj, (uint8_t)((*data & 0xFF00) >> 8));
        soft_iic_send_data(soft_iic_obj, (uint8_t)(*data++ & 0x00FF));
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口向传感器寄存器写 8bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 参数说明     data            要写入的数据
// 返回参数     void
// 使用示例     soft_iic_write_8bit_register(soft_iic_obj, 0x01, 0x01);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_register(soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, const uint8_t data) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口向传感器寄存器写 8bit 数组
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     soft_iic_write_8bit_registers(soft_iic_obj, 0x01, data, 6);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_8bit_registers(soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, const uint8_t *data,
                                   uint32_t len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    while (len--) {
        soft_iic_send_data(soft_iic_obj, *data++);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口向传感器寄存器写 16bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 参数说明     data            要写入的数据
// 返回参数     void
// 使用示例     soft_iic_write_16bit_register(soft_iic_obj, 0x0101, 0x0101);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_register(soft_iic_info_struct *soft_iic_obj, const uint16_t register_name,
                                   const uint16_t data) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    soft_iic_send_data(soft_iic_obj, (uint8_t)((data & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(data & 0x00FF));
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口向传感器寄存器写 16bit 数组
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 参数说明     *data           数据存放缓冲区
// 参数说明     len             缓冲区长度
// 返回参数     void
// 使用示例     soft_iic_write_16bit_registers(soft_iic_obj, 0x0101, data, 6);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_write_16bit_registers(soft_iic_info_struct *soft_iic_obj, const uint16_t register_name,
                                    const uint16_t *data, uint32_t len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    while (len--) {
        soft_iic_send_data(soft_iic_obj, (uint8_t)((*data & 0xFF00) >> 8));
        soft_iic_send_data(soft_iic_obj, (uint8_t)(*data++ & 0x00FF));
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口读取 8bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 返回参数     uint8           返回读取的 8bit 数据
// 使用示例     soft_iic_read_8bit(soft_iic_obj);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_read_8bit(soft_iic_info_struct *soft_iic_obj) {
    uint8_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口从传感器寄存器读取 8bit 数组
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 参数说明     *data           要读取的数据的缓冲区指针
// 参数说明     len             要读取的数据长度
// 返回参数     void
// 使用示例     soft_iic_read_8bit_array(soft_iic_obj, data, 8);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_8bit_array(soft_iic_info_struct *soft_iic_obj, uint8_t *data, uint32_t len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while (len--) {
        *data++ = soft_iic_read_data(soft_iic_obj, len == 0);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口读取 16bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 返回参数     uint16          返回读取的 16bit 数据
// 使用示例     soft_iic_read_16bit(soft_iic_obj);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint16_t soft_iic_read_16bit(soft_iic_info_struct *soft_iic_obj) {
    uint16_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 0);
    temp = ((temp << 8) | soft_iic_read_data(soft_iic_obj, 1));
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口读取 16bit 数组
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     *data           要读取的数据的缓冲区指针
// 参数说明     len             要读取的数据长度
// 返回参数     void
// 使用示例     soft_iic_read_16bit_array(soft_iic_obj, data, 8);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_16bit_array(soft_iic_info_struct *soft_iic_obj, uint16_t *data, uint32_t len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while (len--) {
        *data = soft_iic_read_data(soft_iic_obj, 0);
        *data = ((*data << 8) | soft_iic_read_data(soft_iic_obj, 0 == len));
        data++;
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口从传感器寄存器读取 8bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 返回参数     uint8           返回读取的 8bit 数据
// 使用示例     soft_iic_read_8bit_register(soft_iic_obj, 0x01);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_read_8bit_register(soft_iic_info_struct *soft_iic_obj, const uint8_t register_name) {
    uint8_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口从传感器寄存器读取 8bit 数组
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 参数说明     *data           要读取的数据的缓冲区指针
// 参数说明     len             要读取的数据长度
// 返回参数     void
// 使用示例     soft_iic_read_8bit_registers(soft_iic_obj, 0x01, data, 8);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_8bit_registers(soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, uint8_t *data,
                                  uint32_t len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while (len--) {
        *data++ = soft_iic_read_data(soft_iic_obj, len == 0);
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口从传感器寄存器读取 16bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 返回参数     uint16          返回读取的 16bit 数据
// 使用示例     soft_iic_read_16bit_register(soft_iic_obj, 0x0101);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint16_t soft_iic_read_16bit_register(soft_iic_info_struct *soft_iic_obj, const uint16_t register_name) {
    uint16_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 0);
    temp = ((temp << 8) | soft_iic_read_data(soft_iic_obj, 1));
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口从传感器寄存器读取 16bit 数组
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 参数说明     *data           要读取的数据的缓冲区指针
// 参数说明     len             要读取的数据长度
// 返回参数     void
// 使用示例     soft_iic_read_16bit_registers(soft_iic_obj, 0x0101, data, 8);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_read_16bit_registers(soft_iic_info_struct *soft_iic_obj, const uint16_t register_name, uint16_t *data,
                                   uint32_t len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, (uint8_t)((register_name & 0xFF00) >> 8));
    soft_iic_send_data(soft_iic_obj, (uint8_t)(register_name & 0x00FF));
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    while (len--) {
        *data = soft_iic_read_data(soft_iic_obj, 0);
        *data = ((*data << 8) | soft_iic_read_data(soft_iic_obj, 0 == len));
        data++;
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口传输 8bit 数组 先写后读取
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     *write_data     发送数据存放缓冲区
// 参数说明     write_len       发送缓冲区长度
// 参数说明     *read_data      读取数据存放缓冲区
// 参数说明     read_len        读取缓冲区长度
// 返回参数     void
// 使用示例     iic_transfer_8bit_array(IIC_1, addr, data, 64, data, 64);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_transfer_8bit_array(soft_iic_info_struct *soft_iic_obj, const uint8_t *write_data, uint32_t write_len,
                                  uint8_t *read_data, uint32_t read_len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    while (write_len--) {
        soft_iic_send_data(soft_iic_obj, *write_data++);
    }
    if (read_len) {
        soft_iic_start(soft_iic_obj);
        soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
        while (read_len--) {
            *read_data++ = soft_iic_read_data(soft_iic_obj, 0 == read_len);
        }
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口传输 16bit 数组 先写后读取
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     *write_data     发送数据存放缓冲区
// 参数说明     write_len       发送缓冲区长度
// 参数说明     *read_data      读取数据存放缓冲区
// 参数说明     read_len        读取缓冲区长度
// 返回参数     void
// 使用示例     iic_transfer_16bit_array(IIC_1, addr, data, 64, data, 64);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_transfer_16bit_array(soft_iic_info_struct *soft_iic_obj, const uint16_t *write_data, uint32_t write_len,
                                   uint16_t *read_data, uint32_t read_len) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    while (write_len--) {
        soft_iic_send_data(soft_iic_obj, (uint8_t)((*write_data & 0xFF00) >> 8));
        soft_iic_send_data(soft_iic_obj, (uint8_t)(*write_data++ & 0x00FF));
    }
    if (read_len) {
        soft_iic_start(soft_iic_obj);
        soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
        while (read_len--) {
            *read_data = soft_iic_read_data(soft_iic_obj, 0);
            *read_data = ((*read_data << 8) | soft_iic_read_data(soft_iic_obj, 0 == read_len));
            read_data++;
        }
    }
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口 SCCB 模式向传感器寄存器写 8bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 参数说明     data            要写入的数据
// 返回参数     void
// 使用示例     soft_iic_sccb_write_register(soft_iic_obj, 0x01, 0x01);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_sccb_write_register(soft_iic_info_struct *soft_iic_obj, const uint8_t register_name, uint8_t data) {
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_send_data(soft_iic_obj, data);
    soft_iic_stop(soft_iic_obj);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口 SCCB 模式从传感器寄存器读取 8bit 数据
// 参数说明     *soft_iic_obj   软件 IIC 指定信息 可以参照 zf_driver_soft_iic.h 里的格式看看
// 参数说明     register_name   传感器的寄存器地址
// 返回参数     uint8           返回读取的 8bit 数据
// 使用示例     soft_iic_sccb_read_register(soft_iic_obj, 0x01);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8_t soft_iic_sccb_read_register(soft_iic_info_struct *soft_iic_obj, const uint8_t register_name) {
    uint8_t temp = 0;
    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1);
    soft_iic_send_data(soft_iic_obj, register_name);
    soft_iic_stop(soft_iic_obj);

    soft_iic_start(soft_iic_obj);
    soft_iic_send_data(soft_iic_obj, soft_iic_obj->addr << 1 | 0x01);
    temp = soft_iic_read_data(soft_iic_obj, 1);
    soft_iic_stop(soft_iic_obj);
    return temp;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     软件 IIC 接口初始化 默认 MASTER 模式 不提供 SLAVE 模式
// 参数说明     *soft_iic_obj   软件 IIC 指定信息存放结构体的指针
// 参数说明     addr            软件 IIC 地址 这里需要注意 标准七位地址 最高位忽略 写入时请务必确认无误
// 参数说明     delay           软件 IIC 延时 就是时钟高电平时间 越短 IIC 速率越高
// 参数说明     scl_pin         软件 IIC 时钟引脚 参照 zf_driver_gpio.h 内 gpio_pin_enum 枚举体定义
// 参数说明     sda_pin         软件 IIC 数据引脚 参照 zf_driver_gpio.h 内 gpio_pin_enum 枚举体定义
// 返回参数     void
// 使用示例     soft_iic_init(&soft_iic_obj, addr, 100, B6, B7);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void soft_iic_init(soft_iic_info_struct *soft_iic_obj, uint8_t addr, uint32_t delay, GPIO_TypeDef *SCL_GPIOx,
                   uint16_t SCL_GPIO_Pin, GPIO_TypeDef *SDA_GPIOx, uint16_t SDA_GPIO_Pin) {
    soft_iic_obj->SDA_GPIOx = SDA_GPIOx;
    soft_iic_obj->SDA_GPIO_Pin = SDA_GPIO_Pin;
    soft_iic_obj->SCL_GPIOx = SCL_GPIOx;
    soft_iic_obj->SCL_GPIO_Pin = SCL_GPIO_Pin;
    soft_iic_obj->addr = addr;
    soft_iic_obj->delay = delay;

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = SDA_GPIO_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SDA_GPIOx, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = SCL_GPIO_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SCL_GPIOx, &GPIO_InitStructure);
}
