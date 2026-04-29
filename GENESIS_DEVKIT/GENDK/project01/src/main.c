#include <genesis.h>

int main(u16 hard)
{
    VDP_drawText("Hello SEGA GENESIS World!", 7, 12);

    while(TRUE)
    {
        // read input
        // move sprite
        // update score
        // draw current screen (logo, start screen, settings, game, gameover, credits...)

        // wait for screen refresh and do all SGDK VBlank tasks
        SYS_doVBlankProcess();
    }

    return 0;
}