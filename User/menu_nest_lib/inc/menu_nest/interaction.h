/**
 * ************************************************************************
 * @file interaction.h
 * @author Thybing (yanbinghao86@gmail.com)
 * @brief 交互模块，通过回调函数对输入进行处理。
 * @version 1.0.0
 * @date 2024-8-11
 * ************************************************************************
 */
#ifndef _MENU_NEST_INTERACTION_H_
#define _MENU_NEST_INTERACTION_H_
#include "stdint.h"
#include "stdbool.h"

/**
 * ************************************************************************
 * @brief 输入类型
 * ************************************************************************
 */
typedef uint32_t input_t;

struct MN_interaction;//前向声明

/**
 * ************************************************************************
 * @brief 交互处理回调函数类型
 * 
 * @return 函数交互模块是否捕获此输入
 * ************************************************************************
 */
typedef bool (* interaction_callback_t)(struct MN_interaction * const,const input_t);

/**
 * ************************************************************************
 * @brief 交互模块
 * @note 会以[get,set]标注用户的直接读写权限，没有标注为的成员变量为内部使用，不建议用户直接操作
 * ************************************************************************
 */
typedef struct MN_interaction
{
    /// @brief 交互处理回调函数
    /// @note  [get,set] 用户可以自定义回调函数并设置，set最好统一使用MN_interaction_set_handle_callback
    interaction_callback_t m_handle_callback;

    /// @brief 交互模块所操作的内存，一般指向持有此交互模块的对象
    /// @note  [get] 用户可以访问到内存空间，不建议用户进行set操作，set建议直接使用MN_interaction_create
    void * mp_memory;
}MN_interaction;

/**
 * ************************************************************************
 * @brief 构造一个交互模块对象并返回
 * 
 * @param[in] p_memory   构造时传入可被交互模块操作的内存空间，一般为其所属对象
 * 
 * @return  申请的交互模块对象
 * ************************************************************************
 */
MN_interaction * MN_interaction_create(void * const p_memory);

/**
 * ************************************************************************
 * @brief 析构交互模块
 * 
 * @param[in] self 指向调用对象
 * 
 * ************************************************************************
 */
void MN_interaction_destroy(MN_interaction * const self);

/**
 * ************************************************************************
 * @brief 设置交互回调函数
 * 
 * @param[in] self  指向调用对象
 * @param[in] handle_callback  交互回调函数
 * 
 * ************************************************************************
 */
void MN_interaction_set_handle_callback(MN_interaction * const self,const interaction_callback_t callback);


#endif //_MENU_NEST_INTERACTION_H_
