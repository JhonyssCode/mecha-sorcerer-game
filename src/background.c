#include "background.h"

static int scrollA = 0;
static int scrollB = 0;

void BG_init() {
    // Limpa os planos
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    
    // Desenha o Piso (Plano A) - Linha de plataforma tecnológica
    for(int i=0; i<40; i++) {
        VDP_drawText("-", i, 21); // Linha superior do chão
        VDP_drawText("=", i, 22); // Preenchimento do chão
    }
    
    // Desenha algumas "estrelas" aleatórias no Plano B (Fundo)
    for(int i=0; i<40; i++) {
        int x = random() % 64;
        int y = random() % 20; // Estrelas apenas acima do chão
        VDP_setTileMapEx(BG_B, TILE_ATTR_FULL(PAL0, 0, 0, 0, TILE_USERINDEX), x, y);
    }
}

void BG_update() {
    // Parallax simples
    scrollA -= 1; // Camada da frente (se houver)
    scrollB -= 2; // Camada do fundo (estrelas)
    
    VDP_setHorizontalScroll(BG_B, scrollB);
}
