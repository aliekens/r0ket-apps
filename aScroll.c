#include <sysinit.h>
#include "basic/basic.h"
#include "lcd/render.h"
#include "lcd/allfonts.h"
#include "funk/mesh.h"

#include "basic/config.h"

#include "usetable.h"

/*
void main_walle(void) {
  int blink_it = 0;
  int sine_len = 100;
  //int sine_table[sine_len];
  //for(int i=0;i<sine_len;i++){
  //  sine_table[i] = 
  //}
  int sintable[100]={0,0,1,2,3,4,5,6,7,8,9,10,11,11,12,13,14,15,16,16,17,18,19,19,20,21,21,22,23,23,24,24,25,25,26,26,27,27,27,28,28,28,29,29,29,29,29,29,29,29,30,29,29,29,29,29,29,29,29,28,28,28,27,27,27,26,26,25,25,24,24,23,23,22,21,21,20,19,19,18,17,16,16,15,14,13,12,11,11,10,9,8,7,6,5,4,3,2,1,0}; 

  font=&Font_Orbitron14pt;
  int y_offset=0; 
  int xscroll=0;


  //backlightSetBrightness(GLOBAL(lcdbacklight));
  backlightSetBrightness( 20 );
  
  gpioSetValue( RB_LED3, 1 ); //backlighting
  while(1){
    y_offset++;
    y_offset=y_offset%100;

    if( (y_offset%5) == 0 ) xscroll++;
    xscroll=xscroll%200;
    //if(y_offset>99) y_offset=0;
    //DoString(1,40-sintable[y_offset],"wALLe !");

    DoString(1,  40-sintable[ (y_offset)%100]    , "w" );
    DoString(26, 40-sintable[ (y_offset+8)%100  ], "A" );
    DoString(44, 40-sintable[ (y_offset+16)%100 ], "L" );
    DoString(60, 40-sintable[ (y_offset+24)%100 ], "L" );
    DoString(78, 40-sintable[ (y_offset+32)%100 ], "e" );
    //DoString(92, 40-sintable[ (y_offset+40)%100 ], "!" );
    
    if ( (y_offset%30)==0){ 
      blink_it = 1-blink_it;
    }
    if (y_offset>50) backlightSetBrightness( 10 );
    else backlightSetBrightness( 45 );

    gpioSetValue( RB_LED0, blink_it ); //
    gpioSetValue( RB_LED1, blink_it ); //
    gpioSetValue( RB_LED2, blink_it ); //
    
    //draw_nh_logo(-80+xscroll, 20);
    
    lcdDisplay();
    delayms(10);
    lcdFill(0); //clear screen
  }
*/

static int SINTABLE[100]={0,0,1,2,3,4,5,6,7,8,9,10,11,11,12,13,14,15,16,16,17,18,19,19,20,21,21,22,23,23,24,24,25,25,26,26,27,27,27,28,28,28,29,29,29,29,29,29,29,29,30,29,29,29,29,29,29,29,29,28,28,28,27,27,27,26,26,25,25,24,24,23,23,22,21,21,20,19,19,18,17,16,16,15,14,13,12,11,11,10,9,8,7,6,5,4,3,2,1,0}; 

void ram(void) {
	int NICK_LEN = 0;
	int x = 0;
	int direction = 1; // 0 = right, 1 = left
	int y_offset=0; 
	char * nick = GLOBAL(nickname);

    lcdClear();
	setExtFont(GLOBAL(nickfont));
	NICK_LEN = DoString(0,12, nick);
	x = -NICK_LEN;
	
	while (1) {
		//DoString(x,12,GLOBAL(nickname));
		lcdClear();

		int width = x;
		for (int i = 0; i < strlen(nick); i++) {
			char c = nick[i];
			width = DoChar(width,  (96-getFontHeight())/2-SINTABLE[ (y_offset+i*8)%100] , c );			
		}

	    lcdDisplay();
		
		if (direction) x--; else x++;
		if (x > 96)
			x = -NICK_LEN;
		if (x < -NICK_LEN)
			x = 96;

		y_offset++;
	    y_offset=y_offset%100;
			
		if (x%2 == 0) gpioSetValue (RB_LED0, 1-gpioGetValue(RB_LED0)); 
	    if (x%2 == 1) gpioSetValue (RB_LED2, 1-gpioGetValue(RB_LED2)); 
		
		char key=getInputRaw();
		switch(key) {
	        // Buttons: Right change speed, Up hold scrolling
		case BTN_ENTER:
		  return;
		case BTN_RIGHT:
			direction = 0;
			break; 
		case BTN_UP:
			break;
		case BTN_LEFT:
			direction = 1;
			break;
		case BTN_DOWN:
			break;
		}
	    delayms_queue_plus(5,0);
	}
}