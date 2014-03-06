/***************************************************************************************************

LED Cube - HW routines

Multiplexing interrupt and initialisation subroutines


Based on the code written by Thomas Frey	http://tomscircuits.blogspot.com/
Ported and  optimised to run on MSP430 uC by Rohit Gupta  http://indiantinker.wordpress.com/
$Date: 2013-11-27 22:40:13 +0100 (Di, 27 Nov 2013) $

This work is licensed under the Creative Commons
Attribution-NonCommercial-ShareAlike 3.0 Unported License.
To view a copy of this license, visit
http://creativecommons.org/licenses/by-nc-sa/3.0/ or send a
letter to Creative Commons, 171 Second Street, Suite 300,
San Francisco, California, 94105, USA.

***************************************************************************************************/


#include "HWControl.h"
#include<msp430g2553.h>

/***************************************************************************************************

	timer2 overflow interrupt
	timer2 has 8 bits and generates the multiplexing timing
	
	all it does is to fetch the next port values and to output them
	also, it implements a "long term" software timer

***************************************************************************************************/

volatile uint8_t	delay_counter =0;	// if > 0, decreases in every interrupt
volatile uint8_t	time_scaler =0;


volatile uint8_t	current_group =0;	// current active LED group (0-7)
volatile uint8_t	cycle_counter =	0;	// current PWM cycle (0-63)



/***************************************************************************

// device initialisation

***************************************************************************/

void ioinit (void)
{
	// Set ports

		WDTCTL = WDTPW + WDTHOLD; //Stop Watchdog!
	    P1OUT=0;
		P2OUT=0;
		P2SEL=0;
		P1DIR|=255;
		P2DIR|=255;
		P1OUT=0;
		P2OUT=0;
		BCSCTL1 = CALBC1_8MHZ;                    // Set DCO
		DCOCTL = CALDCO_8MHZ;
	/* timer 2 init
		
		Timer2 generates the timing for multiplexing and dimming

		Timing considerations:
		
		The controller runs at 1MHz; to avoid flicker, the multiplexing
		frequency should be > ~ 100Hz.
		
		With 8 groups of LEDs and 64 levels of brightness we have
		to have an interrupt frequency of ~ 256 x 100Hz.
		So we need to count to 156 or less ((4MHz/100)/256).
		
		NB: The ISR for this timer must be finished in 156 clock cycles!
	*/


	TACCR0 = 320;  // a period of 62,500 cycles is 0 to 320.
	TACCTL0 = CCIE;      // Enable interrupts for CCR0.
	TACTL = TASSEL_2 + MC_1 + TACLR; //smclk,up,div/8 and clear
	__bis_SR_register(GIE); //Global Interrupts enabled
	
	
	

}



/************************************************************************************

	Calculate port values
	Wait
	Clear all

************************************************************************************/

void do_plus(void)		
{
	calculate_IO();
	standard_delay();
	clear_all();
}


void display(void)		
{
//	rotate_cube();
	calculate_IO();
	standard_delay();
}




/***************************************************************************************************
****************************************************************************************************

	delay functions
	
****************************************************************************************************
***************************************************************************************************/


/************************************************************************************

	wait routine
	
	uses a default delay value from a global variable

************************************************************************************/

void standard_delay(void)
{
	egg_timer(current_delay);
}
		

/************************************************************************************

	wait routine
	
	sets a counter which gets decreased in the timer interrupt routine
	
************************************************************************************/

void egg_timer(uint8_t wait_time)		// interrupt gets invoked every ms
{
		delay_counter = wait_time;  	// wait for wait_ms milliseconds
		
		while (delay_counter > 0)	// until delay_counter = 0
		{
		}
}
		



/***************************************************************************************************
****************************************************************************************************

	low level support functions for display
	
****************************************************************************************************
***************************************************************************************************/





/***************************************************************************************************

	function to calculate LED port values from LED_cube ("video RAM") for output
		
***************************************************************************************************/

void calculate_IO(void)				
{
		
	// Define loop counters
	uint8_t i = 0;
	uint8_t j = 0;
	uint8_t current_port = 0;
	
	/* Set all anode values */
	for (i = 0; i < 8; i++)		// repeat for all LED groups
	{	
		current_port = 0;
		for (j = 0; j < 8; j++)	// go through all LEDs
		{	
			current_port = (current_port<<1);
			
			if (LED_cube[i*8+j]>0)
			{
				current_port |= 1;
			}
		}
		
	LED_port[i] = current_port;
		
	}
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void TACCR0_ISR(void)
{

	// Counter 0..255
	cycle_counter++;

	if ((cycle_counter & 0x07) == 0)			// at 0,8,16,...
	{
		// isolate upper 3 bits
		current_group = (cycle_counter>>5);

		// Switch off LEDs
		P2OUT=0;

		/* Set IO pins from memory
		PORTC= LED_port[current_group];
		PORTD= 0b00001100 & (LED_port[current_group]>>4);
*/
		P1OUT=(LED_port[current_group]);

		/* Set LED group (i.e. the common cathode) */
		if (dimmer>0)					// don't set while brightness is 0
			P2OUT =(((1<<current_group)));		// one cathode port is HIGH

		/* delay counter service*/
		if (delay_counter > 0)
		{
		time_scaler++;					// A prescaler /16 to avoid
		time_scaler&=0x0F;				// async 16 bit data exchange

			if (time_scaler == 0)
			{
				delay_counter--;
			}
		}
	}

	if ((cycle_counter & 0x1F) >= dimmer)
	{
		// Switch off LEDs
		P2OUT=0;
	}
}

