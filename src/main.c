#include <genesis.h>
#include "resources.h"
#include "player.h"
#include "boss.h"
#include "shots.h"
#include "background.h"
#include "enemies.h"

/**
 * Mecha Sorcerer - Mega Drive Game
 * Refactored with Game States by Antigravity
 */

typedef enum {
    STATE_TITLE,
    STATE_PLAYING,
    STATE_GAMEOVER,
    STATE_VICTORY
} GameState;

GameState currentState = STATE_TITLE;

void handleTitle() {
    VDP_drawText("MECHA SORCERER", 13, 10);
    VDP_drawText("PRESS START", 14, 16);
    
    u16 joy = JOY_readJoypad(JOY_1);
    if (joy & BUTTON_START) {
        VDP_clearPlane(BG_A, TRUE);
        VDP_clearPlane(BG_B, TRUE);
        
        // Inicializa os módulos para começar o jogo
        SHOTS_init();
        PLAYER_init();
        BOSS_init();
        BG_init(); // O BG_init agora cuida de desenhar o chão tecnológico e o fundo orgânico
        ENEMIES_init();
        
        currentState = STATE_PLAYING;
    }
}

void handleVictory() {
    VDP_drawText("VICTORY!", 16, 12);
    VDP_drawText("BOSS DEFEATED", 13, 14);
    waitMs(3000);
    SYS_reset();
}

int main(bool hardReset) {
    // Inicialização do Motor
    SPR_init();
    JOY_init();
    
    // Configurações Globais
    VDP_setBackgroundColor(16); // Usando um índice da paleta
    PAL_setColor(16, 0x0424);   // #322947 convertido para BGR 9-bit (aprox)

    while(1) {
        switch(currentState) {
            case STATE_TITLE:
                handleTitle();
                break;

            case STATE_PLAYING:
                // 1. Entrada do Usuário
                PLAYER_handleInput();

                // 2. Lógica e Física
                PLAYER_update();
                SHOTS_update();
                BOSS_update();
                BG_update();
                ENEMIES_update();

                // Colisão Inimigo -> Player
                for(int i=0; i<MAX_ENEMIES; i++) {
                    if(enemies[i].active) {
                        if(abs(enemies[i].x - PLAYER_getX()) < 20 && abs(enemies[i].y - PLAYER_getY()) < 20) {
                            PLAYER_damage(10);
                        }
                    }
                }

                // Verificação de Condições de Fim de Jogo
                if (PLAYER_getHealth() <= 0) {
                    currentState = STATE_GAMEOVER;
                }
                if (BOSS_getHealth() <= 0) {
                    currentState = STATE_VICTORY;
                }
                break;

            case STATE_GAMEOVER:
                VDP_drawText("GAME OVER", 15, 12);
                waitMs(2000);
                SYS_reset();
                break;

            case STATE_VICTORY:
                handleVictory();
                break;
        }

        // Atualização Gráfica
        SPR_update();
        SYS_doVBlankProcess();
    }
    
    return 0;
}

