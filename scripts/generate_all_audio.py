import struct
import os

def vlq(n):
    bytes_ = []
    bytes_.append(n & 0x7F)
    n >>= 7
    while n:
        bytes_.append((n & 0x7F) | 0x80)
        n >>= 7
    return bytes(bytes_[::-1])

def midi_event(delta, data):
    return vlq(delta) + bytes(data)

def generate_track(filename, instrument, main_riff, bass_line, repeats=8):
    division = 480
    track0 = midi_event(0, [0xFF, 0x51, 0x03, 0x07, 0xA1, 0x20]) + midi_event(0, [0xFF, 0x2F, 0x00])
    track0 = b"MTrk" + struct.pack(">I", len(track0)) + track0

    # Guitar Track
    t1 = midi_event(0, [0xC0, instrument])
    for _ in range(repeats):
        for n in main_riff:
            t1 += midi_event(0, [0x90, n, 110])
            t1 += midi_event(division // 2, [0x80, n, 0])
    t1 += midi_event(0, [0xFF, 0x2F, 0x00])
    t1 = b"MTrk" + struct.pack(">I", len(t1)) + t1

    # Bass Track
    t2 = midi_event(0, [0xC1, 0x26])
    for _ in range(repeats * 2):
        for n in bass_line:
            t2 += midi_event(0, [0x91, n, 100])
            t2 += midi_event(division // 2, [0x81, n, 0])
    t2 += midi_event(0, [0xFF, 0x2F, 0x00])
    t2 = b"MTrk" + struct.pack(">I", len(t2)) + t2

    header = b"MThd" + struct.pack(">IHHH", 6, 1, 3, division)
    with open(filename, "wb") as f:
        f.write(header + track0 + t1 + t2)

def generate_all():
    music_dir = os.path.join("mecha-sorcerer-game", "res", "music")
    os.makedirs(music_dir, exist_ok=True)
    
    # 1. MAIN THEME (O seu tema épico, mas com guitarra e longo)
    generate_track(
        os.path.join(music_dir, "main_theme.mid"), 0x1E, 
        [40, 40, 43, 40, 45, 40, 43, 42], [28, 28, 31, 33], repeats=16
    )

    # 2. STAGE THEME (Rápido e Industrial)
    generate_track(
        os.path.join(music_dir, "stage_theme.mid"), 0x1E, 
        [40, 43, 40, 46, 40, 43, 40, 38], [28, 31, 28, 34], repeats=16
    )

    # 3. BATTLE THEME (Frenético)
    generate_track(
        os.path.join(music_dir, "battle_theme.mid"), 0x1E, 
        [40, 40, 40, 40, 52, 52, 55, 50], [28, 28, 28, 28], repeats=24
    )

    # 4. SFX (Sons Curtos)
    def simple_sfx(name, notes):
        data = b""
        for n in notes: data += struct.pack('BBBB', 0, 0x90, n, 120) + struct.pack('BBBB', 40, 0x80, n, 0)
        h = b"MThd" + struct.pack(">IHHH", 6, 0, 1, 480)
        t = b"MTrk" + struct.pack(">I", len(data)+4) + data + b'\x00\xFF\x2F\x00'
        with open(os.path.join(music_dir, name), 'wb') as f: f.write(h + t)

    simple_sfx("sfx_jump.mid", [60, 64, 67])
    simple_sfx("sfx_laser.mid", [80, 78, 76])

    print("Sucesso! Todas as trilhas LONGAS e SFX com guitarra foram gerados.")

if __name__ == "__main__":
    generate_all()
