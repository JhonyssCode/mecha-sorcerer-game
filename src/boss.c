#include "boss.h"
#include "resources.h"
#include "shots.h"

static Boss boss;
static int timer = 0;

void BOSS_init() {
    boss.x = 220;
    boss.y = 80;
    boss.health = 100;
    boss.flash = FALSE;
    PAL_setPalette(PAL2, boss_gigabyte.palette->data, CPU);
    boss.sprite = SPR_addSprite(&boss_gigabyte, boss.x, boss.y, TILE_ATTR(PAL2, TRUE, FALSE, FALSE));
}

void BOSS_update() {
    timer++;
    
    // Movimento flutuante
    boss.y = 80 + (sinFix16(timer << 2) >> 4);
    SPR_setPosition(boss.sprite, boss.x, boss.y);

    // Efeito de Flash de Dano
    if (boss.flash) {
        static int flashTimer = 0;
        flashTimer++;
        if (flashTimer < 5) {
            // Muda a paleta para cores claras (simulando flash)
            PAL_setPalette(PAL2, palette_grey, CPU); 
        } else {
            PAL_setPalette(PAL2, boss_gigabyte.palette->data, CPU);
            boss.flash = FALSE;
            flashTimer = 0;
        }
    }

    // Detecção de Dano (Colisão com Tiros)
    for(int i=0; i<MAX_SHOTS; i++) {
        if(shots[i].active) {
            if(shots[i].x > boss.x && shots[i].x < boss.x + 64 &&
               shots[i].y > boss.y && shots[i].y < boss.y + 64) {
                
                shots[i].active = FALSE;
                if (shots[i].sprite) SPR_setVisibility(shots[i].sprite, HIDDEN);
                
                boss.health -= 2;
                boss.flash = TRUE;
            }
        }
    }

    // HUD Gráfico (Barra de Vida)
    VDP_drawText("GIGA-BYTE:", 2, 2);
    // Desenha uma barra de 20 segmentos
    for(int i=0; i<20; i++) {
        if(i < boss.health / 5) {
            // Usa o tile seguro 1500 para a barra cheia
            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, 1, 0, 0, 1500), 12 + i, 2);
        } else {
            // Usa o tile 0 (transparente/vazio) para a barra vazia
            VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL0, 1, 0, 0, 0), 12 + i, 2);
        }
    }
}

int BOSS_getHealth() { return boss.health; }
