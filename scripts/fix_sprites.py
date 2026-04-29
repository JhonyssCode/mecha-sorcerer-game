import os
from PIL import Image, ImageEnhance

RES_PATH = r"res/sprites"

# Mapeamento dos tamanhos exigidos pelo jogo
SIZES = {
    "wizard_mecha.png": (32, 32),
    "boss_gigabyte.png": (64, 64),
    "enemies.png": (32, 32),
    "fx.png": (16, 16)
}

def process_image(filename):
    file_path = os.path.join(RES_PATH, filename)
    if not os.path.exists(file_path):
        return
        
    print(f"Processando: {filename}")
    try:
        img = Image.open(file_path).convert("RGBA")
        
        # 1. Obter a cor de fundo (assumindo que o pixel 0,0 é o fundo)
        bg_color = img.getpixel((0, 0))
        
        # 2. Criar máscara de transparência removendo a cor de fundo (com tolerância)
        data = img.getdata()
        new_data = []
        tolerance = 30
        for item in data:
            if abs(item[0]-bg_color[0]) < tolerance and abs(item[1]-bg_color[1]) < tolerance and abs(item[2]-bg_color[2]) < tolerance:
                new_data.append((255, 255, 255, 0)) # Fica totalmente transparente
            else:
                new_data.append(item)
        img.putdata(new_data)
        
        # 3. Redimensionar para o tamanho correto do Mega Drive
        target_size = SIZES.get(filename, (32, 32))
        if img.size != target_size:
            img = img.resize(target_size, Image.Resampling.LANCZOS)
            
            # Aumentar um pouco o contraste/nitidez após reduzir
            enhancer = ImageEnhance.Sharpness(img)
            img = enhancer.enhance(2.0)
            enhancer2 = ImageEnhance.Contrast(img)
            img = enhancer2.enhance(1.5)
            
        # 4. Criar paleta de 16 cores (0 = transparente)
        # Separar canal alpha
        alpha = img.split()[-1]
        mask = alpha.point(lambda p: 255 if p < 128 else 0)
        
        # Quantizar a imagem RGB para 15 cores
        indexed = img.convert("RGB").quantize(colors=15, method=2)
        palette = indexed.getpalette()[:45]
        
        # Índice 0 será Magenta Escuro (cor de transparência padrão) ou Preto
        new_palette = [255, 0, 255] + palette
        
        final_img = Image.new("P", img.size)
        final_img.putpalette(new_palette)
        
        import numpy as np
        pixels = np.array(indexed) + 1
        mask_array = np.array(mask)
        pixels[mask_array == 255] = 0 # Força índice 0 onde é transparente
        
        output = Image.fromarray(pixels.astype('uint8'), mode="P")
        output.putpalette(new_palette)
        
        # Salva o arquivo final
        output.save(file_path)
        print(f"Sucesso: {filename} redimensionado para {target_size}")
        
    except Exception as e:
        print(f"Erro em {filename}: {e}")

if __name__ == "__main__":
    for f in SIZES.keys():
        process_image(f)
