#!/bin/bash
# Convert border image to header file
xxd -i jolteon_border.jpg > include/jolteon_border.h
sed -i 's/unsigned char jolteon_border_jpg/const uint8_t jolteon_border/g' include/jolteon_border.h
sed -i 's/unsigned int jolteon_border_jpg_len/const uint32_t jolteon_border_size/g' include/jolteon_border.h
echo "Border converted to header file"
