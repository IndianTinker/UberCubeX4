/************************************************************************************

	Function declarations for LEDCube444/CubeSupport



************************************************************************************/
#ifndef HWCONTROL_H
#define HWCONTROL_H

#include <stdlib.h>
#include <inttypes.h>


#include "globals.h"
#include "PatternTools.h"


void do_plus(void);
void display(void);


void egg_timer(uint8_t wait_time);
void standard_delay(void);

void ioinit (void);
void calculate_IO(void);

void calculate_IO(void);
void rotate_cube(void);


#endif 
