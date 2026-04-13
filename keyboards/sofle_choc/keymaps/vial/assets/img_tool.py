import sys
import os
from PIL import Image

def c_to_bmp(header_file, bmp_file, width, height):
    with open(header_file, 'r') as f:
        content = f.read()

    # Extract hex values
    hex_str = content.split('{')[1].split('}')[0]
    data = [int(x.strip(), 16) for x in hex_str.replace('\n', '').split(',') if x.strip()]

    # Create image
    img = Image.new('1', (width, height))
    pixels = img.load()

    pages = (height + 7) // 8
    for page in range(pages):
        for x in range(width):
            byte_idx = page * width + x
            if byte_idx < len(data):
                byte = data[byte_idx]
                for bit in range(8):
                    y = page * 8 + bit
                    if y < height:
                        # QMK format: LSB is top pixel
                        pixels[x, y] = 1 if (byte & (1 << bit)) else 0

    img.save(bmp_file)

def bmp_to_c(bmp_file, header_file, var_name):
    img = Image.open(bmp_file).convert('1')
    width, height = img.size

    pixels = img.load()
    data = []
    pages = (height + 7) // 8
    for page in range(pages):
        for x in range(width):
            byte = 0
            for bit in range(8):
                y = page * 8 + bit
                if y < height:
                    if pixels[x, y]:
                        byte |= (1 << bit)
            data.append(byte)

    with open(header_file, 'w') as f:
        f.write("#pragma once\n")
        f.write(f"static const char PROGMEM {var_name}[] = {{\n")
        for i in range(0, len(data), 16):
            f.write("    " + ", ".join([f"0x{b:02X}" for b in data[i:i+16]]) + ",\n")
        f.write("};\n")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage:")
        print("  python3 img_tool.py c2bmp <header> <bmp> <width> <height>")
        print("  python3 img_tool.py bmp2c <bmp> <header> <var_name>")
    else:
        cmd = sys.argv[1]
        if cmd == "c2bmp":
            c_to_bmp(sys.argv[2], sys.argv[3], int(sys.argv[4]), int(sys.argv[5]))
        elif cmd == "bmp2c":
            bmp_to_c(sys.argv[2], sys.argv[3], sys.argv[4])
