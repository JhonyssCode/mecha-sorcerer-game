#include "player.h"
#include "resources.h"
#include "shots.h"

#define FLOOR_Y 160
#define GRAVITY 1

#define ANIM_IDLE 0
#define ANIM_WALK 1

static Sprite* playerSprite;
static int pX = 50, pY = 150;
static int velY = 0;
static int health = 100;
static bool jumping = false;
static bool moving = false;
static int flashTimer = 0;

void PLAYER_init() {
    pX = 50;
    pY = FLOOR_Y;
    velY = 0;
    health = 100;
    jumping = false;
    playerSprite = SPR_addSprite(&wizard_mecha, pX, pY, TILE_ATTR(PAL1, TRUE, FALSE, FALSE));
    SPR_setAnim(playerSprite, ANIM_IDLE);
}

void PLAYER_handleInput() {
    u16 value = JOY_readJoypad(JOY_1);

    moving = false;
    if (value & BUTTON_RIGHT) {
        pX += 2;
        moving = true;
        SPR_setFlip(playerSprite, FALSE, FALSE);
    }
    if (value & BUTTON_LEFT) {
        pX -= 2;
        moving = true;
        SPR_setFlip(playerSprite, TRUE, FALSE);
    }

    if ((value & BUTTON_B) && !jumping) {
        velY = -10;
        jumping = true;
    }

    if (value & BUTTON_A) {
        SHOTS_fire(pX + 20, pY + 10);
    }

    // Atualiza animação
    if (jumping) {
        // Se houver animação de pulo, usar aqui. Por enquanto mantemos walk ou idle.
    } else {
        if (moving) SPR_setAnim(playerSprite, ANIM_WALK);
        else SPR_setAnim(playerSprite, ANIM_IDLE);
    }
}

void PLAYER_damage(int amount) {
    if (flashTimer == 0) { // Invulnerabilidade temporária
        health -= amount;
        if (health < 0) health = 0;
        flashTimer = 30;
    }
}

void PLAYER_update() {
    pY += velY;
    velY += GRAVITY;

    if (pY >= FLOOR_Y) {
        pY = FLOOR_Y;
        velY = 0;
        jumping = false;
    }

    // Efeito de Flash / Invulnerabilidade
    if (flashTimer > 0) {
        flashTimer--;
        if (flashTimer % 2 == 0) SPR_setVisibility(playerSprite, HIDDEN);
        else SPR_setVisibility(playerSprite, VISIBLE);
    } else {
        SPR_setVisibility(playerSprite, VISIBLE);
    }

    SPR_setPosition(playerSprite, pX, pY);

    // HUD do Player
    VDP_drawText("PLAYER:", 2, 26);
    for(int i=0; i<10; i++) {
        if(i < health / 10) VDP_drawText("#", 10+i, 26);
        else VDP_drawText(" ", 10+i, 26);
    }
}

int PLAYER_getX() { return pX; }
int PLAYER_getY() { return pY; }
int PLAYER_getHealth() { return health; }
