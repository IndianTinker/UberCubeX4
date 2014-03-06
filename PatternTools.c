/***************************************************************************************************

LED Cube - Pattern generation tools

Base routines to generate patterns


Based on the code written by Thomas Frey	http://tomscircuits.blogspot.com/
Ported and  optimised to run on MSP430 uC by Rohit Gupta  http://indiantinker.wordpress.com/
$Date: 2013-11-27 22:40:13 +0100 (Di, 27 Nov 2013) $

$Date: 2012-11-09 23:31:31 +0100 (Fr, 09 Nov 2012) $

This work is licensed under the Creative Commons
Attribution-NonCommercial-ShareAlike 3.0 Unported License.
To view a copy of this license, visit
http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a
letter to Creative Commons, 171 Second Street, Suite 300,
San Francisco, California, 94105, USA.

***************************************************************************************************/


#include "PatternTools.h"



/************************************************************************************

	universal LED manipulation rotine for 3D patterns

	start:		an offset for the pattern to set
	LED_on:		number of LEDs to set in a row
	LED_off:	number of LEDs to keep off in a row
	repeat: 	how often to repeat that sequence

	Base for many regular patterns
	Example: A column starts at "start" (an LED in the first layer. One
	LED is activated and then 15 are not. This is repeated 4 times

************************************************************************************/



void set_pattern(uint8_t start, uint8_t LED_on, uint8_t LED_off, uint8_t repeat)
{

	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t current_LED = start;

	for ( i = repeat; i > 0; i-- )	// repeat several times
	{

		for ( j = LED_on; j > 0; j--)	// set zero or more consecutive LEDs
		{
			set_LED(current_LED++);
		}
		current_LED = current_LED + LED_off;
	}
}


/************************************************************************************

	As before; For improved efficiency includes a clear all and a delay call

************************************************************************************/

void set_pattern_plus(uint8_t start, uint8_t LED_on, uint8_t LED_off, uint8_t repeat)
{
	set_pattern(start, LED_on, LED_off, repeat);
	do_plus();
}




/************************************************************************************

	sets a column of LEDs in the cube / direction as parameter

	position:	0..15 - the column number
	direction:	0 = x / 1 = y / 2 = z
	
************************************************************************************/

void set_column(uint8_t direction, uint8_t position)
{
	switch(direction)
	{
		case 0:
			set_column_x(position);
			break;
			
		case 1:
			set_column_y(position);
			break;
			
		default:
			set_column_z(position);
	}
}

void set_column_plus(uint8_t direction, uint8_t position)
{
	set_column(direction, position);
	do_plus();
}

/************************************************************************************

	sets a vertical column of LEDs in the cube / x direction

	position:	0..15 - the column number
	
************************************************************************************/

void set_column_x(uint8_t position)
{
	set_pattern(position,1,15,4);
}



// set column with variable length
void set_column_xl(uint8_t position, uint8_t length)
{
	set_pattern(position,1,15,length);
}


/************************************************************************************

	sets a horizontal column of LEDs in the cube / y direction

	position:	0..15 - the column number
	
************************************************************************************/

void set_column_y(uint8_t position)
{
	position = (position & 0x03) + ((position & 0xFC)<<2);
	set_pattern(position,1,3,4);
}

void set_column_yl(uint8_t position, uint8_t length)
{
	position = (position & 0x03) + ((position & 0xFC)<<2);
	set_pattern(position,1,3,length);
}

			
/************************************************************************************

	sets a horizontal column of LEDs in the cube / z direction

	position:	0..15 - the column number
	
************************************************************************************/

void set_column_z(uint8_t position)
{
	position = position * 4;
	set_pattern(position,4,1,1);
}

void set_column_zl(uint8_t position, uint8_t length)
{
	position = position * 4;
	set_pattern(position,length,1,1);
}





/************************************************************************************

	sets a 2x2 cube at a given starting point

	position:	0..64 - the LED number where to start
	
************************************************************************************/


void set_cube_plus(uint8_t position)
{
	set_cube(position);
	do_plus();
}

void set_cube(uint8_t position)
{
	set_pattern (position,2,2,2);
	set_pattern (position+16,2,2,2);
}





/************************************************************************************

	set LED
	
	sets a LED in the cube, input is the LED number

************************************************************************************/

void set_LED(uint8_t LED)		
{
	if (LED<64)
	LED_cube[LED] = 1;
}


/************************************************************************************

	set LED
	
	sets a LED in the cube, input are 3 coordinates.

************************************************************************************/

void set_LED3(uint8_t x, uint8_t y, uint8_t z)		
{
	set_LED(x+4*y+16*z);
}



/************************************************************************************

	set LED with delay and clear_all
	
	sets a LED in the cube, input is the LED number

************************************************************************************/

void set_LED_plus(uint8_t LED)		
{
	set_LED(LED);
	do_plus();
}


/************************************************************************************

	clear LED
	
	sets a LED in the cube, input is the LED number

************************************************************************************/

void clear_LED(uint8_t LED)		
{
	LED_cube[LED] = 0;
}


uint8_t get_LED(uint8_t current_LED)
{
	return (LED_cube[current_LED]);
}

/************************************************************************************

	clear all LEDs

************************************************************************************/

void clear_all(void)
{
	uint8_t i = 0;
  
	/* clear all elements of array 'LED_cube'  */
	
	for ( i = 0; i < 64; i++ )
   		{
			LED_cube[i] = 0;
		}
}


/************************************************************************************

	set all LEDs

************************************************************************************/

void set_all(void)
{
	set_pattern(0, 64, 0, 1);
}


/************************************************************************************

	scroll all the cube

************************************************************************************/

void scroll_text(uint8_t byte)
{
	uint8_t	i=0;
	uint8_t	j=0;

	for(j=0; j<16; j++)
	{
		for(i=0; i<3; i++)
		{
			if (get_LED((j*4)+i+1)>0)
			{
				set_LED((j*4)+i);
			}
			else
			{
				clear_LED((j*4)+i);
			}
		}
		clear_LED((j)*4+3);
	}

	if (byte&0x40) set_LED(15);
	if (byte&0x20) set_LED(11);
	if (byte&0x10) set_LED(7);
	if (byte&0x08) set_LED(3);
	if (byte&0x04) set_LED(19);
	if (byte&0x02) set_LED(35);
	if (byte&0x01) set_LED(51);

	display();
}



/************************************************************************************

	a pseudo random generator
	implemented as Galois LFSR (from Wikipedia)

************************************************************************************/

uint8_t pseudo_random (void)
{
	static uint16_t lfsr=1;

	/* characteristic polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
	lfsr = (lfsr >> 1) ^ (-(lfsr & 1) & 0xB400);
	return lfsr & 0xFF;   
}



/***************************************************************************************************

	function to rotate the contents of the cube ("video RAM")
	
	in order to rotate a coordinate system you need to swap and
	invert coordinates, see http://en.wikipedia.org/wiki/Rotation_matrix - rotation by
	90 degree is a special case (sin, cos is either 0 or +/-1)
	
	reads in LED_cube, calculates LED_cube_rot from order and invert
	
		
***************************************************************************************************/

/*void rotate_cube(void)				
{
	uint8_t LED_state = 0;		// LED is on or off
	uint8_t LED_out = 0;		// LED number of output LED
	
	uint8_t i = 0;			// loop counter
	uint8_t j = 0;			// loop counter
	
	uint8_t coord_src[3];		// source coordinates
	uint8_t coord_tgt[3];		// target coordinates


	for (j = 0; j <= 7; j++)	// delete all
	{
		LED_cube_rot[j] = LED_cube[j];
//		LED_cube_rot[j] = 0;
	}

	// Set all anode values
	for (j = 0; j <= 63; j++)	// repeat for all LEDs in the cube
	{	
		// fetch current LED state	
		LED_state = (LED_cube[(j>>3)]>>(j & 0x07))&1;



		for (i=0; i <= 2; i++)
		{
			// convert LED number to coordinates
			coord_src[i] = (j>>(2*i)) & 0b00000011;

			// invert coordinates conditionally
			if (((invert>>i) & 1) == 1)
			{
				coord_src[i]=coord_src[i] ^ 0b00000011;	// only invert two relevant bits
			}

		}

		for (i=0; i <= 2; i++)
		{
			//swap coordinates
			coord_tgt[i]=coord_src[(order>>(2*i))&0b00000011];
//			coord_tgt[i]=coord_src[i];

	}


		// convert coordinates to LED number
		LED_out = 0;
		
		for (i=0; i <= 2; i++)
		{		
			LED_out = LED_out + (coord_tgt[i]<<(2*i));
		}

		
		// move LED state to rotated cube
		LED_cube_rot[(LED_out>>3)] |= (LED_state<<(LED_out & 0x07));
		
	}

}
*/
