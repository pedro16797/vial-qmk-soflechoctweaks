import sys
import os
from PIL import Image

def c_to_bmp(header_file, bmp_file):
    with open(header_file, 'r') as f:
        content = f.read()

    # Extract hex values
    hex_str = content.split('{')[1].split('}')[0]
    data = [int(x.strip(), 16) for x in hex_str.split(',') if x.strip()]

    # Create image (32x8)
    img = Image.new('1', (32, 8))
    pixels = img.load()

    for x in range(32):
        byte = data[x]
        for y in range(8):
            # QMK format: LSB is top pixel
            pixels[x, y] = 1 if (byte & (1 << y)) else 0

    img.save(bmp_file)

def bmp_to_c(bmp_file, header_file, var_name):
    img = Image.open(bmp_file).convert('1')
    width, height = img.size
    if width != 32 or height != 8:
        print(f"Error: {bmp_file} must be 32x8")
        return

    pixels = img.load()
    data = []
    for x in range(32):
        byte = 0
        for y in range(8):
            if pixels[x, y]:
                byte |= (1 << y)
        data.append(byte)

    with open(header_file, 'w') as f:
        f.write("#pragma once\n")
        f.write(f"static const char PROGMEM {var_name}[] = {{\n    ")
        f.write(", ".join([f"0x{b:02X}" for b in data[:16]]) + ",\n    ")
        f.write(", ".join([f"0x{b:02X}" for b in data[16:]]) + "\n")
        f.write("};\n")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage:")
        print("  python3 img_tool.py c2bmp <header> <bmp>")
        print("  python3 img_tool.py bmp2c <bmp> <header> <var_name>")
    else:
        cmd = sys.argv[1]
        if cmd == "c2bmp":
            c_to_bmp(sys.argv[2], sys.argv[3])
        elif cmd == "bmp2c":
            bmp_to_c(sys.argv[2], sys.argv[3], sys.argv[4])
