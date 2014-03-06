/************************************************************************************

	Function declarations for LEDCube444/CubeSupport

Based on the code written by Thomas Frey	http://tomscircuits.blogspot.com/
Ported and  optimised to run on MSP430 uC by Rohit Gupta  http://indiantinker.wordpress.com/
$Date: 2013-11-27 22:40:13 +0100 (Di, 27 Nov 2013) $

************************************************************************************/
#ifndef PATTERNTOOLS_H
#define PATTERNTOOLS_H

#include <stdlib.h>
#include <inttypes.h>


#include "globals.h"
#include "HWControl.h"


void clear_all(void);
void set_all(void);
void set_LED(uint8_t LED);
void set_LED3(uint8_t X, uint8_t y, uint8_t z);
void set_LED_plus(uint8_t LED);
void clear_LED(uint8_t LED);
uint8_t get_LED(uint8_t current_LED);

void set_pattern(uint8_t start, uint8_t LED_on, uint8_t LED_off, uint8_t repeat);
void set_pattern_plus(uint8_t start, uint8_t LED_on, uint8_t LED_off, uint8_t repeat);

void set_cube(uint8_t position);
void set_cube_plus(uint8_t position);
void set_column(uint8_t direction, uint8_t position);
void set_column_plus(uint8_t direction, uint8_t position);
void set_column_x(uint8_t position);
void set_column_y(uint8_t position);
void set_column_z(uint8_t position);
void set_column_xl(uint8_t position, uint8_t length);
void set_column_yl(uint8_t position, uint8_t length);
void set_column_zl(uint8_t position, uint8_t length);

void set_level(uint8_t level);

void scroll_text(uint8_t byte);

uint8_t pseudo_random (void);


#endif 
