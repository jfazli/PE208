/*
 * Copyright 2012 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
#ifndef __LLMJVM_RTX_H
#define __LLMJVM_RTX_H

#include <RTL.h>

// RTX event used to wake up the MicroJvm virtual machine task
#define LLMJVM_RTX_EVT_WAKEUP 1

/*
 * Define to use RTX timer for MicroJvm virtual machine time implementation.
 * This implementation is limited because internal time is stored on a 16 bits
 * unsigned integer (os_time).
 * If an OS tick occurs every 10 ms, the os_time will overflow after less than
 * 11 minutes.
 */
#define LLMJVM_RTX_TIME_RTX 1

/*
 * Define to use an hardware timer MicroJvm virtual machine time implementation
 * instead of internal RTX timer.
 * This implementation is better than the RTX timer implementation but requires
 * an hardware 32-bits timer to run.
 */
#define LLMJVM_RTX_TIME_HARDWARE_TIMER 2

/*
 * By default, time implementation is based on hardware timer.
 * @see LLMJVM_RTX_TIME_RTX
 * @see LLMJVM_RTX_TIME_HARDWARE_TIMER
 */
#ifndef LLMJVM_RTX_TIME_IMPL
#define LLMJVM_RTX_TIME_IMPL LLMJVM_RTX_TIME_HARDWARE_TIMER
#endif

#if LLMJVM_RTX_TIME_IMPL != LLMJVM_RTX_TIME_RTX && LLMJVM_RTX_TIME_IMPL != LLMJVM_RTX_TIME_HARDWARE_TIMER
#error "Please choose between LLMJVM_RTX_TIME_RTX and LLMJVM_RTX_TIME_HARDWARE_TIMER"
#endif

//Warning: this value is the RTX OS_TICK. RTX_Config.c file must include this
//header file in order to get the same value.
#ifndef OS_TICK
 #define OS_TICK        10000
#endif

//Info used by the MicroJvm virtual machine timer
#define LLMJVM_RTX_TIMER_INFO 42

// RTX intern timer call back
void LLMJVM_RTX_tmr_call(void);

/**
 * Memory settings on Cortex-M
 */
#define MEM_CODE_START (0)
#define MEM_CODE_END (0x20000000)

#endif /* __LLMJVM_RTX_H */
