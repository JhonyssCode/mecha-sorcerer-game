#include <genesis.h>
#include "resources.h"

/**
 * Mecha Sorcerer - Mega Drive Game
 * Project initialized by Antigravity
 */

#define MAX_SHOTS 5

// Estruturas
typedef struct {
    int x, y;
    bool active;
} Shot;

typedef struct {
    Sprite* sprite;
    int x, y;
    int health;
    bool flash;
} Boss;

// Globais
Shot shots[MAX_SHOTS];
Boss boss;
Sprite* player;

int pX = 50, pY = 150;
int velY = 0;
bool jumping = false;

// Constantes
#define FLOOR_Y 160
#define GRAVITY 1

void handleInput() {
    u16 value = JOY_readJoypad(JOY_1);

    if (value & BUTTON_RIGHT) pX += 2;
    if (value & BUTTON_LEFT)  pX -= 2;

    if ((value & BUTTON_B) && !jumping) {
        velY = -10;
        jumping = true;
    }

    if (value & BUTTON_A) {
        for(int i=0; i<MAX_SHOTS; i++) {
            if(!shots[i].active) {
                shots[i].active = true;
                shots[i].x = pX + 20;
                shots[i].y = pY + 10;
                break;
            }
        }
    }
}

void updatePhysics() {
    pY += velY;
    velY += GRAVITY;

    if (pY >= FLOOR_Y) {
        pY = FLOOR_Y;
        velY = 0;
        jumping = false;
    }

    for(int i=0; i<MAX_SHOTS; i++) {
        if(shots[i].active) {
            shots[i].x += 4;
            if(shots[i].x > 320) shots[i].active = false;
        }
    }
}

void handleBoss() {
    static int timer = 0;
    timer++;
    
    // Movimento flutuante do Boss
    boss.y = 80 + (sinFix16(timer << 2) >> 4);
    SPR_setPosition(boss.sprite, boss.x, boss.y);

    // Detecção de Dano
    for(int i=0; i<MAX_SHOTS; i++) {
        if(shots[i].active) {
            if(shots[i].x > boss.x && shots[i].x < boss.x + 64 &&
               shots[i].y > boss.y && shots[i].y < boss.y + 64) {
                
                shots[i].active = false;
                boss.health -= 2;
                boss.flash = true;
            }
        }
    }

    // HUD do Boss
    VDP_drawText("GIGA-BYTE HP:", 2, 2);
    for(int i=0; i<10; i++) {
        if(i < boss.health / 10) VDP_drawText("=", 15+i, 2);
        else VDP_drawText(" ", 15+i, 2);
    }
}

int main(bool hardReset) {
    SPR_init();
    JOY_init();
    VDP_setBackgroundColor(0);

    // Inicializa Jogador e Boss com Sprites reais
    player = SPR_addSprite(&wizard_mecha, pX, pY, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    
    boss.x = 220;
    boss.y = 80;
    boss.health = 100;
    boss.sprite = SPR_addSprite(&boss_gigabyte, boss.x, boss.y, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));

    // Inicia a música
    XGM_startPlay(battle_music);

    while(1) {
        handleInput();
        updatePhysics();
        handleBoss();

        // Atualiza posição do jogador
        SPR_setPosition(player, pX, pY);

        SPR_update();
        SYS_doVBlankProcess();
    }
    return 0;
}
