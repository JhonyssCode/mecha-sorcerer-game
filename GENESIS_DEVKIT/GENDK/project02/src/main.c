#include <genesis.h>
#include "sprite.h"

// position and movement variables
int posX, posY;
bool paused;

int main(u16 hard)
{
    //inicializacao da VDP (Video Display Processor)
	SYS_disableInts();
	VDP_init();
	VDP_setScreenWidth320();
	VDP_setScreenHeight224();
	SYS_enableInts();
	
	//inicializacao de variaveis
	posX = 25;
	posY = 25;
	paused = FALSE;
	
	//inicializacao de sprites
	SPR_init();
	Sprite* player;
	player = SPR_addSprite(&spr_dan, posX, posY, PAL0);
	VDP_setPalette(PAL0, spr_dan.palette->data);
		
    while(TRUE)
    {
        // read input
        // move sprite
        // update score
		VDP_setPaletteColor(15,RGB24_TO_VDPCOLOR(0xff0000));
		VDP_drawText("_GDB_ SEGA GENESIS GAME ENGINE", 0, 0);
		
        // draw screen
		SPR_update();

        // wait for screen refresh and do all SGDK VBlank tasks
        SYS_doVBlankProcess();
    }

    return 0;
}