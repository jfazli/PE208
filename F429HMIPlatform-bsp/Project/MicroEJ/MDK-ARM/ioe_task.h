/*
 * C
 *
 * Copyright 2012-2013 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
 
#ifndef IOE_TASK_H
#define IOE_TASK_H

#include "microej.h"

/**
 * @brief  Create and start the IO Expander task that call IOExpander_interrupt() when it is notified
 * @retval 0 if an error occurred, a non null value otherwise
 */
uint8_t IOE_TASK_create_task(void);

/**
 * @brief  Notify the IO Expander task to call IOExpander_interrupt()
 * @retval 0 if an error occurred, a non null value otherwise
 */
uint8_t IOE_TASK_handle_interrupt(void);

#endif
