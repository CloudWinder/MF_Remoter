#include <stdint.h>
#include "debug_assert.h"
#include "led.h"
#include "stm32f1xx_hal.h"


void assertFail(char *exp, char *file, int line)
{
	//printf("Assert failed %s:%d\n", file, line);
	while (1);
}


