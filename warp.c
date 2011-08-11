#include <sysinit.h>
#include "basic/basic.h"
#include "basic/random.h"
#include "lcd/render.h"

#include "usetable.h"

#define WIDTH 96
#define HEIGHT 68
#define SCALE 128

void ram(void) {
	int nstars = 100;
	int x[ nstars ];
	int y[ nstars ];
	int age[ nstars ];
	int i;
	
	for( i = 0; i < nstars; i++ ) {
		x[ i ] = ( getRandom() % ( WIDTH * SCALE ) );
		y[ i ] = ( getRandom() % ( HEIGHT * SCALE ) );
		age[ i ] = 0;
	}
	
	lcdFill( 0 );
	lcdDisplay();

	gpioSetValue( RB_LED0, 1 - gpioGetValue( RB_LED0 ) );
	int counter = 0;
		
	while (1) {

		if( counter % 50 == 0 ) {
			gpioSetValue( RB_LED0, 1 - gpioGetValue( RB_LED0 ) );
			gpioSetValue( RB_LED2, 1 - gpioGetValue( RB_LED2 ));
		}
		
		lcdFill( 0 );
		
		for( int i = 0; i < nstars; i++ ) {
			
			/*if( counter % 100 == 0 ) {
				
				char key=getInputRaw();
				switch(key) {
				
					case BTN_RIGHT:
					break; 
				
					case BTN_LEFT:
					break;
				
					case BTN_UP:
					break;
				
					case BTN_DOWN:
					break;
				
				}
				
			}*/
			
			int oldx = x[ i ];
			int oldy = y[ i ]; 
			
			lcdSetPixel( x[ i ] / SCALE + 0, y[ i ] / SCALE + 0, 1 );
			if( age[ i ] > 100 ) {
				lcdSetPixel( x[ i ] / SCALE + 1, y[ i ] / SCALE + 0, 1 );
				lcdSetPixel( x[ i ] / SCALE - 1, y[ i ] / SCALE + 0, 1 );
				lcdSetPixel( x[ i ] / SCALE + 0, y[ i ] / SCALE + 1, 1 );
				lcdSetPixel( x[ i ] / SCALE + 0, y[ i ] / SCALE - 1, 1 );
			}

			if( ( age[ i ] > 200 ) && ( counter % 4 == 0 ) ) {
				lcdSetPixel( x[ i ] / SCALE + 1, y[ i ] / SCALE + 1, 1 );
				lcdSetPixel( x[ i ] / SCALE + 1, y[ i ] / SCALE - 1, 1 );
				lcdSetPixel( x[ i ] / SCALE - 1, y[ i ] / SCALE + 1, 1 );
				lcdSetPixel( x[ i ] / SCALE - 1, y[ i ] / SCALE - 1, 1 );
			}
			
			int dx = x[ i ] - WIDTH * SCALE / 2;
			int dy = y[ i ] - HEIGHT * SCALE / 2;
			x[ i ] = WIDTH * SCALE / 2 + dx * 1005 / 1000;
			y[ i ] = HEIGHT * SCALE / 2 + dy * 1005 / 1000;
						
			if( ( x[ i ] < 0 ) || ( x[ i ] > WIDTH * SCALE ) || ( y[ i ] < 0 ) || ( y[ i ] > HEIGHT * SCALE ) || ( ( oldx == x[ i ] ) && ( oldy == y[ i ] ) ) ) {
				x[ i ] = ( getRandom() % ( WIDTH * SCALE ) );
				y[ i ] = ( getRandom() % ( HEIGHT * SCALE ) );
				age[ i ] = 0;
			}
			
			age[ i ] = age[ i ] + 1;
			
		}
		
		lcdDisplay();
		delayms(2);
		
		counter = counter + 1;
		
		char key=getInputRaw();
		if (key== BTN_ENTER) return;
	}

}
