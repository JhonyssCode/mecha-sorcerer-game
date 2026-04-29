#include <genesis.h>
#include "background.h"

// Padrão de um tile "tecnológico alienígena" (8x8 pixels, 4bpp)
// Valores hexadecimais representam as cores (0-15) dos pixels
static const u32 tech_tile[8] = {
    0x11112211,
    0x13333321,
    0x13455431,
    0x23555542,
    0x23555542,
    0x13455431,
    0x13333321,
    0x11221111
};

// Índice seguro na VRAM, longe dos sprites (VRAM tem 2048 tiles no total)
#define ALIEN_TILE_INDEX 1500

static int scrollB_x = 0;
static int scrollB_y = 0;
static u16 color_pulse = 0;
static s16 color_dir = 1;

void BG_init() {
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    
    // Carrega o tile do fundo na VRAM
    VDP_loadTileData(tech_tile, ALIEN_TILE_INDEX, 1, CPU);
    
    // Preenche o Plano B (Fundo) com o tile alienígena
    // Criando um labirinto/padrão orgânico estilo Metroid
    for(int y=0; y<28; y++) {
        for(int x=0; x<40; x++) {
            // Desenha com um padrão para não ficar tudo igual (gira o tile dependendo da posição)
            u16 tile_attr = TILE_ATTR_FULL(PAL0, 0, (x+y)%2, (x*y)%2, ALIEN_TILE_INDEX);
            VDP_setTileMapXY(BG_B, tile_attr, x, y);
        }
    }
    
    // O chão do jogador (Plano A) - Linha sólida e limpa
    for(int i=0; i<40; i++) {
        // Chão sólido usando um tile branco puro (índice 0, modificado na paleta, ou apenas blocos)
        // O caractere '=' da fonte padrão (índice 61 da fonte) serve se pintarmos com a cor certa.
        // Mas para ser profissional, vamos desenhar o tile 'alienígena' achatado
        VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, ALIEN_TILE_INDEX), i, 22);
        VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, ALIEN_TILE_INDEX), i, 23);
        VDP_setTileMapXY(BG_A, TILE_ATTR_FULL(PAL1, 1, 0, 0, ALIEN_TILE_INDEX), i, 24);
    }
    
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    
    // Configura as cores da paleta 0 (usada pelo fundo)
    // Cores de base "vivas" (tons de roxo/ciano escuro)
    PAL_setColor(0, 0x0212); // Fundo escuro
    PAL_setColor(1, 0x0424);
    PAL_setColor(2, 0x0646);
    PAL_setColor(3, 0x0868);
    PAL_setColor(4, 0x0A8A); // Cor que vai pulsar
    PAL_setColor(5, 0x0EAE); // Cor central
}

void BG_update() {
    // Scroll contínuo do Plano B (efeito Parallax/Flutuação)
    scrollB_x -= 1;
    scrollB_y += 1;
    VDP_setHorizontalScroll(BG_B, scrollB_x >> 1);
    VDP_setVerticalScroll(BG_B, scrollB_y >> 2);
    
    // Palette Cycling (O "coração" do fundo vivo)
    color_pulse += color_dir;
    if (color_pulse >= 60) color_dir = -1;
    if (color_pulse <= 0) color_dir = 1;
    
    // Anima a cor 4 da PAL0 para criar um brilho pulsante (como veias orgânicas/tecnológicas)
    u16 red = 2 + (color_pulse / 10);
    u16 blue = 4 + (color_pulse / 10);
    
    // Formato 9-bit BGR: 0x0bbbggggrrrr.
    u16 bgr = (blue << 8) | (0 << 4) | red;
    
    PAL_setColor(4, bgr);
}
