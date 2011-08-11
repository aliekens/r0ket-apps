#include <sysinit.h>
#include "basic/basic.h"
#include "lcd/render.h"
#include "lcd/allfonts.h"
#include "funk/mesh.h"

#include "basic/config.h"

#include "usetable.h"

#define WIDTH 96
#define HEIGHT 68
#define SCALE 128
static int SINTABLE[100]={0,0,1,2,3,4,5,6,7,8,9,10,11,11,12,13,14,15,16,16,17,18,19,19,20,21,21,22,23,23,24,24,25,25,26,26,27,27,27,28,28,28,29,29,29,29,29,29,29,29,30,29,29,29,29,29,29,29,29,28,28,28,27,27,27,26,26,25,25,24,24,23,23,22,21,21,20,19,19,18,17,16,16,15,14,13,12,11,11,10,9,8,7,6,5,4,3,2,1,0}; 

// key repeat delay stuff
const int KEY_REPEAT_DELAY = 15;
int key_rep[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void ram(void) {
	// setup nick scroll
	bool show_nick = false;
	int NICK_LEN = 0;
	int nick_x = 0;
	int nick_direction = 0;
	int nick_y_offset=0; 
	char * nick = GLOBAL(nickname);

    lcdClear();
	setExtFont(GLOBAL(nickfont));
	int font_heigth = getFontHeight();
	NICK_LEN = DoString(0,12, nick);
	nick_x = (WIDTH-NICK_LEN)/2;

	// setup starfield
	bool show_stars = true;
	int nstars = 100;
	int x[ nstars ];
	int y[ nstars ];
	int age[ nstars ];
	
	for( int i = 0; i < nstars; i++ ) {
		x[ i ] = ( getRandom() % ( WIDTH * SCALE ) );
		y[ i ] = ( getRandom() % ( HEIGHT * SCALE ) );
		age[ i ] = 0;
	}
	
	int step_counter = 0;
	int raw_key, key;
	
	// main loop
	while (1) {
		lcdFill( 0 );

		// ****************************
		// ***  drawing 
		// ****************************
		
		// draw the nick
		if (show_nick) {
			int width = nick_x;
			for (int i = 0; i < strlen(nick); i++) {
				width = DoChar(width,  (96-font_heigth)/2-SINTABLE[ (nick_y_offset+i*8)%100] , nick[i] );			
			}
		}
		
		// draw the stars
		if (show_stars) { 
			for( int i = 0; i < nstars; i++ ) {
				lcdSetPixel( x[ i ] / SCALE + 0, y[ i ] / SCALE + 0, 1 );
				if( age[ i ] > 100 ) {
					lcdSetPixel( x[ i ] / SCALE + 1, y[ i ] / SCALE + 0, 1 );
					lcdSetPixel( x[ i ] / SCALE - 1, y[ i ] / SCALE + 0, 1 );
					lcdSetPixel( x[ i ] / SCALE + 0, y[ i ] / SCALE + 1, 1 );
					lcdSetPixel( x[ i ] / SCALE + 0, y[ i ] / SCALE - 1, 1 );
				}

				if( ( age[ i ] > 200 ) && ( step_counter % 4 == 0 ) ) {
					lcdSetPixel( x[ i ] / SCALE + 1, y[ i ] / SCALE + 1, 1 );
					lcdSetPixel( x[ i ] / SCALE + 1, y[ i ] / SCALE - 1, 1 );
					lcdSetPixel( x[ i ] / SCALE - 1, y[ i ] / SCALE + 1, 1 );
					lcdSetPixel( x[ i ] / SCALE - 1, y[ i ] / SCALE - 1, 1 );
				}
				
				int oldx = x[ i ];
				int oldy = y[ i ]; 
			
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
		}
		
	    lcdDisplay();

		// blinking leds are cool
		if( step_counter == 0 ) {
			gpioSetValue( RB_LED0, 1 - gpioGetValue( RB_LED0 ));
			gpioSetValue( RB_LED2, 1 - gpioGetValue( RB_LED2 ));
		}
		
		// ****************************
		// ***  calculate next step 
		// ****************************
		
		// calculate nick momevements
		if (show_nick) {
			nick_x = nick_x + nick_direction;
			if (nick_x > 96) nick_x = -NICK_LEN;
			if (nick_x < -NICK_LEN) nick_x = 96;
			nick_y_offset++;
	    	nick_y_offset=nick_y_offset%100;
		}
		
		// calculate starfield
//		if (show_stars) { 
//			for (int i=0; i < nstars; i++) {
//			}
//		}
		
		// step counter MOD 50 to trigger the blinking every 50 steps
		step_counter = (step_counter + 1)%50;
		
		// ****************************
		// ***  control
		// ****************************
		
		// check if we need to modify some params
		raw_key = getInputRaw();
		key = 0;
		for(int i = 0; i < 15; i++) {
			if(raw_key & (1 << i)) {
				if(!key_rep[i] || key_rep[i] == KEY_REPEAT_DELAY) key |= 1 << i;
				key_rep[i] += key_rep[i] < KEY_REPEAT_DELAY;
			}
			else key_rep[i] = 0;
		}
		
		switch(key) {
		case BTN_ENTER:
			// ENTER for exiting the app
		  return;
		case BTN_RIGHT:
			// right to make the nick go faster to the right
			if (nick_direction < 5) nick_direction++;
			break; 
		case BTN_UP:
			show_nick = !show_nick;
			if (show_nick) {
				nick_x = (WIDTH-NICK_LEN)/2;
				nick_direction = 0;				
			}
			break;
		case BTN_LEFT:
			// left to make the nick go faster to the left
			if (nick_direction > -5) nick_direction--;
			break;
		case BTN_DOWN:
			// DOWN to switch starfield on or off
			show_stars = !show_stars;
			break;
		}
		
		// wait and process queue
	    //delayms_queue_plus(1,0);
		//delayms(2);
	}
}