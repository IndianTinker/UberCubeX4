/***************************************************************************************************

LED Cube

Controls a 4 x 4 x 4 LED cube using conventional multiplexing
in a 8x8 multiplexing matrix.

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




#include "LedCube444.h"


volatile uint8_t LED_cube[64] =	{0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0,
						 0,0,0,0,0,0,0,0}; 
                         
                         // Video RAM - one byte per LED

//volatile uint8_t	LED_cube_rot[8] =	{0,0,0,0,0,0,0,0}; // rotated Video RAM

volatile uint8_t	LED_port[8] =		{
						 0,0,0,0,0,0,0,0,
						};
					

volatile uint8_t	current_delay =200;

volatile uint8_t	dimmer =33;
uint8_t i;	// all purpose loop counters
uint8_t j;
uint8_t k;

// cube rotation parameters
//volatile uint8_t	order =			0b00100100; // new order of coordinates
//volatile uint8_t	invert =		0;	    // indicates which coordinates to invert


/************************************************************************************
*************************************************************************************

	main routine

*************************************************************************************
************************************************************************************/
void dimmingface()
{
	current_delay = 100;
			dimmer = 0;

			for(j=0; j<4; j++)
			{
				clear_all();
				set_pattern(j*4, 4, 12, 4);
				calculate_IO();

				for (i=0; i<255; i++)
				{

					dimmer=i/8;
					egg_timer(1);

				}

				for (i=255; i>0; i--)
				{

					dimmer=i/8;
					egg_timer(1);

				}
			}
}
void airpump()
{
	current_delay = 30;


	for (j=0; j<5; j++)
	{

		for (i=0;i<5;i++)
		{

			set_column_xl(0,i);
			set_column_xl(15,i);

			set_column_x(67-(i*16));
			set_column_x(76-(i*16));

			do_plus();	// set IO, wait, clear_all
		}


		for (i=3;i>0;i--)
		{

			set_column_xl(0,i);
			set_column_xl(15,i);

			set_column_x(67-(i*16));
			set_column_x(76-(i*16));

			do_plus();	// set IO, wait, clear_all
		}
	}
}
void movingcorner()
{


	current_delay = 40;
	dimmer = 32;

// 3 Lines from three directions


	for(i=0;i<3;i++)
	{
		clear_all();
		set_column_x(15);
		set_column_y(15);
		set_column_z(15);
		display();

		// move first angle

		clear_all();
		set_column_x(15);
		set_column_y(11);
		set_column_z(11);
		display();

		clear_all();
		set_column_x(15);
		set_column_y(7);
		set_column_z(7);
		display();

		clear_all();
		set_column_x(15);
		set_column_y(3);
		set_column_z(3);
		display();

		// move second angle

		clear_all();
		set_column_x(11);
		set_column_y(3);
		set_column_z(2);
		display();

		clear_all();
		set_column_x(7);
		set_column_y(3);
		set_column_z(1);
		display();

		clear_all();
		set_column_x(3);
		set_column_y(3);
		set_column_z(0);
		display();

		// move third angle

		clear_all();
		set_column_x(2);
		set_column_y(2);
		set_column_z(0);
		display();

		clear_all();
		set_column_x(1);
		set_column_y(1);
		set_column_z(0);
		display();

		clear_all();
		set_column_x(0);
		set_column_y(0);
		set_column_z(0);
		display();


		// move angle 4

		clear_all();
		set_column_x(0);
		set_column_y(4);
		set_column_z(4);
		display();

		clear_all();
		set_column_x(0);
		set_column_y(8);
		set_column_z(8);
		display();

		clear_all();
		set_column_x(0);
		set_column_y(12);
		set_column_z(12);
		display();


		// move angle 5

		clear_all();
		set_column_x(4);
		set_column_y(12);
		set_column_z(13);
		display();

		clear_all();
		set_column_x(8);
		set_column_y(12);
		set_column_z(14);
		display();

		clear_all();
		set_column_x(12);
		set_column_y(12);
		set_column_z(15);
		display();

		// move angle 4

		clear_all();
		set_column_x(13);
		set_column_y(13);
		set_column_z(15);
		display();

		clear_all();
		set_column_x(14);
		set_column_y(14);
		set_column_z(15);
		display();

		clear_all();
		set_column_x(15);
		set_column_y(15);
		set_column_z(15);

	}
}
void randomcloud()
{


	// random activation

	current_delay = 10;
	for ( i=0; i<200; i++)
	{
		set_LED(0x3F & pseudo_random() );		// limit to 0..63
		clear_LED(0x3F & pseudo_random());	// limit to 0..63
		display();
	}

	for ( i=0; i<150; i++)
	{
		clear_LED(0x3F & pseudo_random());	// limit to 0..63
		calculate_IO();
		display();
	}
}

void cycle()
{



	// cycle through planes (xy)

		current_delay = 30;

		for ( i=0; i<3; i++)
		{
			set_pattern_plus(0,  16, 0, 1);
			set_pattern_plus(16, 16, 0, 1);
			set_pattern_plus(32, 16, 0, 1);
			set_pattern_plus(48, 16, 0, 1);

		// cycle through planes (xz)


			set_pattern_plus(0, 4, 12, 4);
			set_pattern_plus(4, 4, 12, 4);
			set_pattern_plus(8, 4, 12, 4);
			set_pattern_plus(12,4, 12, 4);

		// cycle through planes (yz)


			set_pattern_plus(0, 1, 3, 16);
			set_pattern_plus(1, 1, 3, 16);
			set_pattern_plus(2, 1, 3, 16);
			set_pattern_plus(3, 1, 3, 16);
		}



//
}
void cinc()
{


	// cube in cube


	for(i=0; i<3; i++)
	{
		current_delay = 30;

		for ( i=0; i<4; i++)
		{
			set_cube_plus(0);
			set_cube_plus(16);
			set_cube_plus(32);
			set_cube_plus(33);
			set_cube_plus(34);
			set_cube_plus(18);
			set_cube_plus(2);
			set_cube_plus(6);
			set_cube_plus(10);
			set_cube_plus(26);
			set_cube_plus(42);
			set_cube_plus(41);
			set_cube_plus(40);
			set_cube_plus(24);
			set_cube_plus(8);
			set_cube_plus(4);
		}

	}


}

void flip()
{	// the flipping plane
	current_delay = 40;


	static	const uint8_t flipvalues[] = {	0, 1, 2, 3,
							0, 1, 6, 7,
							0, 5, 10, 15,
							0, 4, 9, 13,
							0, 4, 8, 12};

	for ( k=0; k<5; k++)
	{

		for ( i=0; i<4; i++)
		{

			for ( j=0; j<4; j++)
			{
				set_column_x(flipvalues[i*4+j]);
			}
			do_plus();
		}



		for ( i=0; i<4; i++)
		{

			for ( j=0; j<4; j++)
			{
				set_column_y(flipvalues[(4-i)*4+j]);
			}
			do_plus();
		}


		for ( i=0; i<4; i++)
		{

			for ( j=0; j<4; j++)
			{
				set_column_z(flipvalues[i*4+j]);
			}
			do_plus();
		}

	}


}
void upndown()
{


	// jumping up and down

	// set upper plane
	set_pattern(0, 16, 1, 1);

	for ( i=0; i<30; i++)
	{
		uint8_t randomLED;

		//select one LED
		randomLED = (0x0F & pseudo_random());
		current_delay = 15;

		//is it up? yes: move LED down - no: move LED up
		if (get_LED(randomLED) == 1)
		{
			clear_LED(randomLED);
			set_LED(randomLED+16);
			display();

			clear_LED(randomLED+16);
			set_LED(randomLED+48);
			display();
			}
		else
		{
			clear_LED(randomLED+48);
			set_LED(randomLED+32);
			display();

			clear_LED(randomLED+32);
			set_LED(randomLED);
			display();

		}

		current_delay = 50;
		display();


	}
}
 void rotatingtube()
 {
		current_delay = 8;

		uint8_t	l = 0;

		for (l=0; l<3; l++)
		{

			for(k=0;k<7;k++)
			{
				if (k<3)
				{
					i=0;
					j=k+1;
				}
				else
				{
					i=(k-3)*16;
					j=4;
				}

				set_column_xl(0+i,j);
				set_column_xl(15+i,j);

				do_plus();	// set IO, wait, clear_all


				set_column_xl(1+i,j);
				set_column_xl(14+i,j);

				do_plus();	// set IO, wait, clear_all


				set_column_xl(2+i,j);
				set_column_xl(13+i,j);

				do_plus();	// set IO, wait, clear_all


				set_column_xl(3+i,j);
				set_column_xl(12+i,j);

				do_plus();	// set IO, wait, clear_all


				set_column_xl(7+i,j);
				set_column_xl(8+i,j);

				do_plus();	// set IO, wait, clear_all


				set_column_xl(11+i,j);
				set_column_xl(4+i,j);

				do_plus();	// set IO, wait, clear_all

			}
		}
 }

void diagonalplane()
{
	// A diagonal plane moving through the cube

		current_delay = 40;

		for(i=0;i<6;i++)
		{
			for(j=0;j<5;j++)
			{
				set_pattern_plus(j,1,4,32);
			}
		}
}

void cuberotation()
{
	for(i=0; i<6; i++)
	{
		current_delay = 10;


		set_column_x(14);
		set_cube_plus(37);

		set_column_x(15);
		set_cube_plus(37);

		set_column_x(11);
		set_cube_plus(37);

		set_column_x(7);
		set_cube_plus(37);

		set_column_x(3);
		set_cube_plus(37);

		set_column_x(2);
		set_cube_plus(37);

		set_column_x(1);
		set_cube_plus(37);

		set_column_x(0);
		set_cube_plus(37);

		set_column_x(4);
		set_cube_plus(37);

		set_column_x(8);
		set_cube_plus(37);

		set_column_x(12);
		set_cube_plus(37);

		set_column_x(13);
		set_cube_plus(37);

	//	set_cube_plus(21);

	//	set_cube_plus(5);

		set_column_x(14);
		set_cube_plus(5);

		set_column_x(15);
		set_cube_plus(5);

		set_column_x(11);
		set_cube_plus(5);

		set_column_x(7);
		set_cube_plus(5);

		set_column_x(3);
		set_cube_plus(5);

		set_column_x(2);
		set_cube_plus(5);

		set_column_x(1);
		set_cube_plus(5);

		set_column_x(0);
		set_cube_plus(5);

		set_column_x(4);
		set_cube_plus(5);

		set_column_x(8);
		set_cube_plus(5);

		set_column_x(12);
		set_cube_plus(5);

		set_column_x(13);
		set_cube_plus(5);

	}

}
void flip2()
{
	for(i=0;i<3;i++)
	{

		current_delay = 50;

		set_pattern_plus(0,4,12,4);
		set_pattern_plus(4,4,12,4);
		set_pattern_plus(8,4,12,4);
		set_pattern_plus(12,4,12,4);

		set_pattern_plus(12,4,8,4);

		set_pattern_plus(00,16,1,1);
		set_pattern_plus(16,16,1,1);
		set_pattern_plus(32,16,1,1);
		set_pattern_plus(48,16,1,1);

		set_column_y(12);
		set_column_y(9);
		set_column_y(6);
		set_column_y(3);
		do_plus();

		set_pattern_plus(0,1,3,16);
		set_pattern_plus(1,1,3,16);
		set_pattern_plus(2,1,3,16);
		set_pattern_plus(3,1,3,16);

		set_column_x(12);
		set_column_x(9);
		set_column_x(6);
		set_column_x(3);
		do_plus();

	}



}
void lift()
{
	current_delay = 25;


	for(i=0; i<3; i++)
	{
		set_cube(5);

		set_cube(21);

		set_cube(37);

		set_LED(49);
		set_cube(37);

		set_LED(50);
		set_cube(37);

		set_LED(51);
		set_cube(37);

		set_LED(55);
		set_cube(37);

		set_LED(59);
		set_cube(37);

		set_LED(63);
		set_cube(37);

		set_LED(62);
		set_cube(37);

		set_LED(61);
		set_cube(37);

		set_LED(60);
		set_cube(37);

		set_LED(56);
		set_cube(37);

		set_LED(52);
		set_cube(37);

		set_LED(48);
		set_cube(37);

		set_LED(49);
		set_cube(37);

		set_cube(37);

		set_cube(21);

		set_cube(5);

		set_LED(14);
		set_cube(5);

		set_LED(15);
		set_cube(5);

		set_LED(11);
		set_cube(5);

		set_LED(7);
		set_cube(5);

		set_LED(3);
		set_cube(5);

		set_LED(2);
		set_cube(5);

		set_LED(1);
		set_cube(5);

		set_LED(0);
		set_cube(5);

		set_LED(4);
		set_cube(5);

		set_LED(8);
		set_cube(5);

		set_LED(12);
		set_cube(5);

		set_LED(13);
		set_cube(5);

	}

}

void pulsecube()
{

	for (i=0; i<6; i++)
	{

		current_delay = 70;

		set_column_x(0);
		set_column_x(3);
		set_column_x(12);
		set_column_x(15);

		set_column_y(0);
		set_column_y(3);
		set_column_y(12);
		set_column_y(15);

		set_column_z(0);
		set_column_z(3);
		set_column_z(12);
		set_column_z(15);


		do_plus();


		set_cube_plus(21);

	}

}
void spiral()
{
	// downward spiral

	current_delay = 15;
	for ( i=0; i<5; i++)
	{
		for ( j=0; j<4; j++)
		{
			set_LED_plus(j*16+1);
			set_LED_plus(j*16+2);
			set_LED_plus(j*16+7);
			set_LED_plus(j*16+11);
			set_LED_plus(j*16+14);
			set_LED_plus(j*16+13);
			set_LED_plus(j*16+8);
			set_LED_plus(j*16+4);
		}
	}


}
void tube()
{

	// one column, rotating around the cube

	current_delay = 20;
	for ( j=0; j<3; j++)
	{
		for ( i=0; i<5; i++)
		{
			//set_column_plus(j, 0);
			set_column_plus(j, 1);
			set_column_plus(j, 2);
			//set_column_plus(j, 3);
			set_column_plus(j, 7);
			set_column_plus(j, 11);
			//set_column_plus(j, 15);
			set_column_plus(j, 14);
			set_column_plus(j, 13);
			//set_column_plus(j, 12);
			set_column_plus(j, 8);
			set_column_plus(j, 4);
		}
	}


}
void vanishingplane()
{
	//	a shrinking, vanishing plain

		current_delay = 20;

		for(j=0;j<2;j++)
		{
			for(i=16;i>0;i=i-4)
			{

				set_pattern_plus(48,i,1,1);
				set_pattern_plus(32,i,1,1);
				set_pattern_plus(16,i,1,1);
				set_pattern_plus(00,i,1,1);

				set_pattern_plus(00,i,1,1);
				set_pattern_plus(16,i,1,1);
				set_pattern_plus(32,i,1,1);
				set_pattern_plus(48,i,1,1);

			}

			clear_all();
			display();
			egg_timer(120);

		}



	#if 0
	// Show an 'A'
		current_delay = 20;
		clear_all();

		set_LED(0);
		set_LED(16);
		set_LED(32);
		set_LED(48);
		set_LED(4);

		set_LED(3);
		set_LED(19);
		set_LED(35);
		set_LED(51);
		set_LED(7);

		set_LED(9);
		set_LED(10);
		set_LED(17);
		set_LED(18);


		display();
		dimmer = 15;
		standard_delay();
		standard_delay();
		standard_delay();

		dimmer = 32;
		standard_delay();
		standard_delay();
		standard_delay();



	#endif		//
}
void splash()
{
	for(i=0; i<5; i++)
	{
		current_delay = 40;

		set_LED(5);
		do_plus();

		set_LED(21);
		do_plus();

		set_LED(37);
		do_plus();

		set_LED(53);
		do_plus();

		current_delay = 20;

		set_LED(52);
		set_LED(53);
		set_LED(54);
		set_LED(57);
		set_LED(49);

		do_plus();

		set_pattern_plus(48,3,1,3);

		set_pattern_plus(48,16,1,1);
	}

}

int main (void)
{


	ioinit();
	
	while (1)	// loop forever
	{

		//scroll();
vanishingplane();
cinc();
rotatingtube();
movingcorner();
cycle();
flip();
upndown();
randomcloud();
cuberotation();
pulsecube();
tube();
splash();
airpump();
movingcorner();
diagonalplane();
flip2();
lift();
pulsecube();



	}
}




