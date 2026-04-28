import pygame
import sys
import random
import os

# Mecha Sorcerer - High Fidelity Simulator (Python/Pygame)
# Objetivo: Replicar a experiência exata do Mega Drive para testes rápidos.

# Configurações de Resolução Original (Mega Drive)
ORIG_WIDTH = 320
ORIG_HEIGHT = 224
SCALE = 2 # Escala para visualização em monitores modernos
SCREEN_WIDTH = ORIG_WIDTH * SCALE
SCREEN_HEIGHT = ORIG_HEIGHT * SCALE
FPS = 60

# PALETA COMPLETA (36 CORES)
PALETTE = [
    (94, 49, 91), (140, 63, 93), (186, 97, 86), (242, 166, 94), (255, 228, 120),  # Row 1
    (207, 255, 112), (143, 222, 93), (60, 163, 112), (61, 110, 112), (50, 62, 79), # Row 2
    (50, 41, 71), (71, 59, 120), (75, 91, 171), (77, 166, 255), (102, 255, 227),  # Row 3
    (255, 255, 235), (194, 194, 209), (126, 126, 143), (96, 96, 112), (67, 67, 79), # Row 4
    (39, 39, 54), (62, 35, 71), (87, 41, 75), (150, 66, 83), (227, 105, 86),      # Row 5
    (255, 181, 112), (255, 145, 102), (235, 86, 75), (176, 48, 92), (115, 39, 92), # Row 6
    (66, 36, 69), (90, 38, 94), (128, 54, 107), (189, 72, 130), (255, 107, 151),  # Row 7
    (255, 181, 181) # Row 8
]

# Aliases amigáveis para as cores principais
PAL_DARK_BG = PALETTE[10]
PAL_PURPLE  = PALETTE[11]
PAL_YELLOW  = PALETTE[4]
PAL_CYAN    = PALETTE[14]
PAL_ROSE    = PALETTE[28]
PAL_GREY    = PALETTE[18]
PAL_TECH    = PALETTE[19]

BLACK = (20, 20, 25)
WHITE = PALETTE[15]
BASE_PATH = os.path.dirname(os.path.abspath(__file__))
RES_PATH = os.path.join(os.path.dirname(BASE_PATH), "res", "sprites")

class Game:
    def __init__(self):
        pygame.init()
        self.screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
        self.display = pygame.Surface((ORIG_WIDTH, ORIG_HEIGHT))
        pygame.display.set_caption("Mecha Sorcerer - Console Sim")
        self.clock = pygame.time.Clock()
        self.running = True
        
        # Carregar Sprites Reais
        self.load_assets()
        
        # Estado do Jogador
        self.pX = 50
        self.pY = 160
        self.velY = 0
        self.health = 100
        self.jumping = False
        self.facing_right = True
        self.anim_frame = 0
        self.anim_timer = 0
        
        # Estado do Boss
        self.bossX = 220
        self.bossY = 80
        self.boss_health = 100
        self.boss_timer = 0
        self.boss_flash = 0
        
        # Objetos
        self.shots = []
        self.enemies = []
        self.stars = [[random.randint(0, ORIG_WIDTH), random.randint(0, 150)] for _ in range(30)]

    def load_assets(self):
        try:
            # Carrega e recorta os sprites (assumindo frames horizontais)
            self.sheet_player = pygame.image.load(os.path.join(RES_PATH, "wizard_mecha.png")).convert_alpha()
            self.sheet_boss = pygame.image.load(os.path.join(RES_PATH, "boss_gigabyte.png")).convert_alpha()
            self.sheet_enemy = pygame.image.load(os.path.join(RES_PATH, "enemies.png")).convert_alpha()
            self.sheet_fx = pygame.image.load(os.path.join(RES_PATH, "fx.png")).convert_alpha()
            
            # Recorta o primeiro frame (32x32 para player/enemy, 64x64 para boss)
            self.player_img = self.sheet_player.subsurface((0, 0, 32, 32))
            self.boss_img = self.sheet_boss.subsurface((0, 0, 64, 64))
            self.enemy_img = self.sheet_enemy.subsurface((0, 0, 32, 32))
            self.fx_img = self.sheet_fx.subsurface((0, 0, 16, 16))
        except Exception as e:
            print(f"Erro ao carregar assets reais: {e}")
            sys.exit()

    def handle_input(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT: self.running = False
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE and not self.jumping:
                    self.velY = -8
                    self.jumping = True
                if event.key == pygame.K_z: # Botão A do MD
                    self.shots.append([self.pX + 20, self.pY + 10])

        keys = pygame.key.get_pressed()
        if keys[pygame.K_RIGHT]: 
            self.pX += 2
            self.facing_right = True
        if keys[pygame.K_LEFT]: 
            self.pX -= 2
            self.facing_right = False

    def update(self):
        # Gravidade (Sincronizada com C)
        self.pY += self.velY
        self.velY += 0.5
        if self.pY >= 160:
            self.pY = 160
            self.velY = 0
            self.jumping = False

        # Boss (Movimento Senoidal exato)
        self.boss_timer += 0.1
        import math
        self.bossY = 80 + math.sin(self.boss_timer) * 10
        if self.boss_flash > 0: self.boss_flash -= 1

        # Tiros e Colisão Boss
        for s in self.shots[:]:
            s[0] += 4
            if s[0] > ORIG_WIDTH: self.shots.remove(s)
            elif (self.bossX < s[0] < self.bossX + 64) and (self.bossY < s[1] < self.bossY + 64):
                self.boss_health -= 2
                self.boss_flash = 5
                if s in self.shots: self.shots.remove(s)

        # Inimigos
        if random.random() < 0.02:
            self.enemies.append([ORIG_WIDTH, 160])
        for e in self.enemies[:]:
            e[0] -= 1.5
            if e[0] < -32: self.enemies.remove(e)
            # Colisão com Player
            if abs(e[0] - self.pX) < 20 and abs(e[1] - self.pY) < 20:
                self.health -= 0.5
            # Colisão com Tiros
            for s in self.shots:
                if abs(s[0] - e[0]) < 20 and abs(s[1] - e[1]) < 20:
                    if e in self.enemies: self.enemies.remove(e)

        # Estrelas
        for star in self.stars:
            star[0] -= 1
            if star[0] < 0: star[0] = ORIG_WIDTH

    def draw(self):
        self.display.fill(PAL_DARK_BG)
        
        # Nebulosa de Fundo (Simulada com círculos suaves)
        pygame.draw.circle(self.display, PAL_ROSE, (ORIG_WIDTH//4, ORIG_HEIGHT//2), 60)
        pygame.draw.circle(self.display, PAL_PURPLE, (ORIG_WIDTH*3//4, ORIG_HEIGHT//3), 80)
        
        # Grade Tecnológica (Estilo Retro-Future)
        for i in range(0, ORIG_WIDTH, 40):
            pygame.draw.line(self.display, (40, 35, 60), (i, 0), (i, ORIG_HEIGHT), 1)
        for i in range(0, ORIG_HEIGHT, 40):
            pygame.draw.line(self.display, (40, 35, 60), (0, i), (ORIG_WIDTH, i), 1)

        # Estrelas (Plano B)
        for star in self.stars:
            pygame.draw.rect(self.display, PAL_YELLOW, (star[0], star[1], 1, 1))
            
        # Chão (Plano A)
        pygame.draw.rect(self.display, PAL_TECH, (0, 192, ORIG_WIDTH, 32))
        pygame.draw.line(self.display, PAL_CYAN, (0, 192), (ORIG_WIDTH, 192), 2)
        
        # Jogador
        img = self.player_img
        if not self.facing_right: img = pygame.transform.flip(img, True, False)
        self.display.blit(img, (self.pX, self.pY))
        
        # Boss
        if self.boss_flash % 2 == 0:
            self.display.blit(self.boss_img, (self.bossX, self.bossY))
            
        # Tiros
        for s in self.shots:
            self.display.blit(self.fx_img, (s[0], s[1]))
            
        # Inimigos
        for e in self.enemies:
            self.display.blit(self.enemy_img, (e[0], e[1]))
            
        # HUD Premium (Usando a paleta)
        pygame.draw.rect(self.display, WHITE, (10, 10, 102, 10), 1)
        pygame.draw.rect(self.display, PAL_CYAN, (11, 11, max(0, self.health), 8))
        
        pygame.draw.rect(self.display, WHITE, (208, 10, 102, 10), 1)
        pygame.draw.rect(self.display, PAL_ROSE, (209, 11, max(0, self.boss_health), 8))

        # Escalonar para a janela principal
        self.screen.blit(pygame.transform.scale(self.display, (SCREEN_WIDTH, SCREEN_HEIGHT)), (0, 0))
        pygame.display.flip()

    def run(self):
        while self.running:
            self.handle_input()
            self.update()
            self.draw()
            self.clock.tick(FPS)
        pygame.quit()

if __name__ == "__main__":
    game = Game()
    game.run()
