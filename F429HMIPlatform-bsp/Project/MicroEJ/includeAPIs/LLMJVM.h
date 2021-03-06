/* 
 * Copyright 2011-2012 IS2T. All rights reserved.
 * Modification and distribution is permitted under certain conditions.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */
/* 
 * AUTOGENERATED by COMP-ICETEA-ARMCC-ARM - 1.0.0 - DO NOT EDIT IT
 */
/**
 * MicroJvm virtual machine exit codes.
 */
#ifndef _LLMJVM
#define _LLMJVM 
#include <intern/LLMJVM.h>
#include <stdint.h>
#ifdef __cplusplus
	extern "C" {
#endif
// --------------------------------------------------------------------------------
// -                                  Constants                                   -
// --------------------------------------------------------------------------------

/**
 * Exit code returned when the application ends normally (i.e. all the threads are terminated or <code>System.exit(exitCode)</code> has been called)
 */
#define LLMJVM_E_OK (0)

/**
 * Exit code returned when the SOAR used for the application is not compatible with the MicroJvm virtual machine.
 */
#define LLMJVM_E_SOAR_FILE_INCOMPATIBLE (-1)

/**
 * Exit code returned when the link specific configuration breaks MicroJvm virtual machine requirements.
 */
#define LLMJVM_E_BAD_LINK (-2)

/**
 * Exit code returned when the application ends because of the evaluation version limitations.
 */
#define LLMJVM_E_EVAL_LIMIT (-3)

/**
 * Exit code returned when the MicroJvm virtual machine is badly configured.
 */
#define LLMJVM_E_INV_OPTION (-4)

/**
 * Exit code returned when the Main thread can not be launched (for instance if thread stack can not be allocated).
 */
#define LLMJVM_E_MAIN_THREAD_ALLOC (-5)

/**
 * Exit code returned when the specified maximum number of threads exceeds MicroJvm virtual machine limits.
 */
#define LLMJVM_E_TOO_MANY_THREADS (-12)

/**
 * Exit code returned when the Java heap size is too large.
 */
#define LLMJVM_E_INV_HEAP_SIZE (-13)

/**
 * Exit code returned when the Java stacks memory is invalid. Check the stacks memory alignment or size.
 */
#define LLMJVM_E_INV_JAVA_STACK_MEM (-14)

/**
 * Exit code returned when the Java static fields memory is not large enough.
 */
#define LLMJVM_E_INV_JAVA_STATIC_MEM (-15)

/**
 * Exit code returned when the MicroJvm virtual machine cannot be restarted.
 */
#define LLMJVM_E_CANNOT_RESTART (-16)

#ifdef __cplusplus
	}
#endif
#endif
