#include "LcdTest_impl.h"

/*
 * Initialize srand. This function can only call srand() function
 * without argument. In this way the randomized values are always
 * the same between two launches
 */
void lcdTest_srandInit(void)
{
	// ?
	srand();
}

/*
 * Get time in milliseconds. This function can return 1. In this
 * way the framerate calculations will be wrong.
 */
uint64_t lcdTest_getTime(void)
{
	return 1;
}

/*
 * Print the framerate
 */
void lcdTest_printFramerate(uint32_t time, uint32_t frequency)
{
	
}
