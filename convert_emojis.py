#!/usr/bin/env python3

import os
from PIL import Image
import struct

def rgb888_to_rgb565(r, g, b):
    """Convert RGB888 to RGB565 format"""
    r = (r >> 3) & 0x1F
    g = (g >> 2) & 0x3F  
    b = (b >> 3) & 0x1F
    return (r << 11) | (g << 5) | b

def convert_png_to_rgb565(input_path, output_path):
    """Convert PNG image to 128x128 RGB565 binary format"""
    with Image.open(input_path) as img:
        # Convert to RGB if not already
        if img.mode != 'RGB':
            img = img.convert('RGB')
        
        # Resize to 128x128
        img = img.resize((128, 128), Image.Resampling.LANCZOS)
        
        # Convert to RGB565 and write binary data
        with open(output_path, 'wb') as f:
            for y in range(128):
                for x in range(128):
                    r, g, b = img.getpixel((x, y))
                    rgb565 = rgb888_to_rgb565(r, g, b)
                    # Write as little-endian 16-bit value
                    f.write(struct.pack('<H', rgb565))

def main():
    input_dir = 'src/assets/emojis'
    output_dir = 'src/assets/emojis_rgb565'
    
    # Create output directory if it doesn't exist
    os.makedirs(output_dir, exist_ok=True)
    
    # Process all PNG files in the input directory
    for filename in os.listdir(input_dir):
        if filename.lower().endswith('.png'):
            input_path = os.path.join(input_dir, filename)
            output_filename = filename.replace('.png', '.rgb565')
            output_path = os.path.join(output_dir, output_filename)
            
            print(f"Converting {filename} -> {output_filename}")
            convert_png_to_rgb565(input_path, output_path)
    
    print("Conversion complete!")

if __name__ == '__main__':
    main()