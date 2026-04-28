import struct

def create_midi(filename):
    # MIDI Header: MThd, length 6, format 0, 1 track, 480 ticks per quarter note
    header = struct.pack('>4sLHHH', b'MThd', 6, 0, 1, 480)
    
    # Track Events (Delta-time, Event)
    # 0x90: Note On, Channel 0, Note 40 (E2), Velocity 100
    # 0x80: Note Off
    
    # A simple Metal Riff: E2, E2, G2, E2, A2
    events = [
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 43, 100), (120, 0x80, 43, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 45, 100), (120, 0x80, 45, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 43, 100), (120, 0x80, 43, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 45, 100), (120, 0x80, 45, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 43, 100), (120, 0x80, 43, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 45, 100), (120, 0x80, 45, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 43, 100), (120, 0x80, 43, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 45, 100), (120, 0x80, 45, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 43, 100), (120, 0x80, 43, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 45, 100), (120, 0x80, 45, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 43, 100), (120, 0x80, 43, 0),
        (0, 0x90, 40, 100), (120, 0x80, 40, 0),
        (0, 0x90, 45, 100), (120, 0x80, 45, 0)
    ]
    
    track_data = b''
    for delta, cmd, note, vel in events:
        # Simplistic MIDI event writer
        track_data += struct.pack('B', delta) # Delta time (simplified, only works < 128)
        track_data += struct.pack('BBB', cmd, note, vel)
    
    # End of Track event
    track_data += b'\x00\xFF\x2F\x00'
    
    track_header = struct.pack('>4sL', b'MTrk', len(track_data))
    
    with open(filename, 'wb') as f:
        f.write(header)
        f.write(track_header)
        f.write(track_data)

if __name__ == "__main__":
    import os
    target = os.path.join("mecha-sorcerer-game", "res", "music", "battle_theme.mid")
    create_midi(target)
    print(f"MIDI gerado com sucesso em: {target}")
