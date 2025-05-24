# IMU660RA_STM32

该仓库实现了STM32F103(ZET6)对逐飞IMU660Ra六轴姿态传感器的驱动及对获取的数据进行姿态解算。

## 环境准备
- Stm32CubeMX
- Arm Keilv5

## 快速开始

1. Stm32CubeMX打开F103ZET6.ioc文件，点击Generate Code生成工程。
2. 使用Keil打开 MDK-ARM/F103ZET6.uvprojx
3. 编译，烧录

## 第三方库声明

### 本项目使用了以下 GPLv3 授权的第三方库：

**MSPM0G3507 Opensourec Library (SEEKFREE 逐飞科技)**
User/imu660ra/和User/soft_iic/由该库修改得到
项目原地址暂时未知，代码来源2024年电赛省赛时逐飞科技群的开源。

### 本项目使用了以下 MIT 授权的第三方库：

**[MenuNest](https://github.com/Thybing/MenuNest)**
之前写的菜单框架，该项目中用于展示数据。

**[stm32-ssd1306](https://github.com/afiskon/stm32-ssd1306)**
接入菜单框架中的ssd1306驱动。位于User/ssd1306

### NO LICENSE

**[Pose](https://github.com/diceTZ/Pose)**
代码中使用的姿态解算的代码来源。User/attitude_algorithm/pose.c/h和User/attitude_algorithm/pose_math.c/h。
文件夹中User/attitude_algorithm/attitude_algorighm.c/h，是对该库的二次封装，添加了去除零飘等操作。

## 许可证

由于本项目包含 GPLv3 授权组件(MSPM0G3507 Opensourec Library SEEKFREE)并基于其开发，故**本项目整体授权协议为 GNU GPLv3**，详见仓库中的 `LICENSE` 文件。
