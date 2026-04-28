#include "shots.h"
#include "resources.h"

Shot shots[MAX_SHOTS];

void SHOTS_init() {
    for(int i=0; i<MAX_SHOTS; i++) {
        shots[i].active = false;
        shots[i].sprite = NULL;
    }
}

void SHOTS_fire(int x, int y) {
    for(int i=0; i<MAX_SHOTS; i++) {
        if(!shots[i].active) {
            shots[i].active = true;
            shots[i].x = x;
            shots[i].y = y;
            
            // Já inicializa o sprite se necessário
            if (shots[i].sprite == NULL) {
                shots[i].sprite = SPR_addSprite(&laser_fx, shots[i].x, shots[i].y, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
            } else {
                SPR_setVisibility(shots[i].sprite, VISIBLE);
            }
            break;
        }
    }
}

void SHOTS_update() {
    for(int i=0; i<MAX_SHOTS; i++) {
        if(shots[i].active) {
            shots[i].x += 4;
            
            if(shots[i].x > 320) {
                shots[i].active = false;
                if (shots[i].sprite) SPR_setVisibility(shots[i].sprite, HIDDEN);
            } else {
                if (shots[i].sprite) SPR_setPosition(shots[i].sprite, shots[i].x, shots[i].y);
            }
        }
    }
}
