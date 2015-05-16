    .global assets_font
    .global assets_font_size
    .section .rodata
assets_font:
    .incbin "../assets/font.bmp"
1:
assets_font_size:
    .int 1b - assets_font
