#!/usr/bin/env python3
"""
Convert PNG images to RGB565 format for Arduino/ESP32 OLED displays
"""

from PIL import Image
import os
import sys

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565 format"""
    r5 = (r >> 3) & 0x1F  # 5 bits for red
    g6 = (g >> 2) & 0x3F  # 6 bits for green  
    b5 = (b >> 3) & 0x1F  # 5 bits for blue
    
    return (r5 << 11) | (g6 << 5) | b5

def convert_png_to_rgb565_header(png_path, output_path, array_name):
    """Convert PNG to C header file with RGB565 data array"""
    try:
        # Open and convert image to RGB
        img = Image.open(png_path)
        img = img.convert('RGB')
        
        width, height = img.size
        pixels = img.load()
        
        # Generate header file
        with open(output_path, 'w') as f:
            f.write(f"// Auto-generated from {os.path.basename(png_path)}\n")
            f.write(f"// Image dimensions: {width}x{height}\n")
            f.write(f"#ifndef {array_name.upper()}_H\n")
            f.write(f"#define {array_name.upper()}_H\n\n")
            f.write(f"#include <Arduino.h>\n\n")
            f.write(f"#define {array_name.upper()}_WIDTH {width}\n")
            f.write(f"#define {array_name.upper()}_HEIGHT {height}\n\n")
            f.write(f"static const uint16_t {array_name}[] PROGMEM = {{\n")
            
            # Convert each pixel to RGB565
            for y in range(height):
                f.write("  ")
                for x in range(width):
                    r, g, b = pixels[x, y]
                    rgb565 = rgb888_to_rgb565(r, g, b)
                    f.write(f"0x{rgb565:04X}")
                    
                    if y < height - 1 or x < width - 1:
                        f.write(", ")
                    
                    if (x + 1) % 8 == 0:  # 8 values per line
                        f.write("\n  ")
                
                if y < height - 1:
                    f.write("\n")
            
            f.write("\n};\n\n")
            f.write(f"#endif // {array_name.upper()}_H\n")
        
        print(f"Converted {png_path} -> {output_path}")
        print(f"Array name: {array_name}")
        print(f"Dimensions: {width}x{height}")
        print(f"Total pixels: {width * height}")
        
    except Exception as e:
        print(f"Error converting {png_path}: {e}")

def convert_all_pngs_in_directory(input_dir, output_dir):
    """Convert all PNG files in a directory"""
    os.makedirs(output_dir, exist_ok=True)
    
    for filename in os.listdir(input_dir):
        if filename.lower().endswith('.png'):
            png_path = os.path.join(input_dir, filename)
            array_name = filename.replace('.png', '').replace('-', '_').replace(' ', '_')
            header_filename = f"{array_name}.h"
            output_path = os.path.join(output_dir, header_filename)
            
            convert_png_to_rgb565_header(png_path, output_path, array_name)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python png_to_rgb565.py <input_directory> <output_directory>")
        print("Example: python png_to_rgb565.py src/assets/emojis src/assets/emojis_rgb565")
        sys.exit(1)
    
    input_dir = sys.argv[1]
    output_dir = sys.argv[2]
    
    if not os.path.exists(input_dir):
        print(f"Input directory {input_dir} does not exist")
        sys.exit(1)
    
    convert_all_pngs_in_directory(input_dir, output_dir)
    print("Conversion complete!")