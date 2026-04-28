#include "enemies.h"
#include "resources.h"
#include "shots.h"

Enemy enemies[MAX_ENEMIES];

void ENEMIES_init() {
    for(int i=0; i<MAX_ENEMIES; i++) {
        enemies[i].active = false;
        enemies[i].sprite = NULL;
    }
}

void ENEMIES_spawn() {
    for(int i=0; i<MAX_ENEMIES; i++) {
        if(!enemies[i].active) {
            enemies[i].active = true;
            enemies[i].x = 320;
            enemies[i].y = 160; // No chão
            enemies[i].health = 10;
            
            if (enemies[i].sprite == NULL) {
                enemies[i].sprite = SPR_addSprite(&robot_enemies, enemies[i].x, enemies[i].y, TILE_ATTR(PAL3, TRUE, FALSE, FALSE));
            } else {
                SPR_setVisibility(enemies[i].sprite, VISIBLE);
            }
            break;
        }
    }
}

void ENEMIES_update() {
    for(int i=0; i<MAX_ENEMIES; i++) {
        if(enemies[i].active) {
            enemies[i].x -= 2;
            
            // Colisão com Tiros
            for(int j=0; j<MAX_SHOTS; j++) {
                if(shots[j].active) {
                    if(shots[j].x > enemies[i].x && shots[j].x < enemies[i].x + 32 &&
                       shots[j].y > enemies[i].y && shots[j].y < enemies[i].y + 32) {
                        
                        shots[j].active = false;
                        if (shots[j].sprite) SPR_setVisibility(shots[j].sprite, HIDDEN);
                        
                        enemies[i].health -= 10;
                    }
                }
            }

            if(enemies[i].x < -32 || enemies[i].health <= 0) {
                enemies[i].active = false;
                if (enemies[i].sprite) SPR_setVisibility(enemies[i].sprite, HIDDEN);
            } else {
                if (enemies[i].sprite) SPR_setPosition(enemies[i].sprite, enemies[i].x, enemies[i].y);
            }
        }
    }

    // Spawn aleatório
    if ((random() % 100) < 2) ENEMIES_spawn();
}
