    .global assets_font
    .global assets_font_size
    .global assets_sprites
    .global assets_sprites_size
    .section .rodata
assets_font:
    .incbin "../assets/font.bmp"
1:
assets_font_size:
    .int 1b - assets_font
assets_sprites:
    .incbin "../assets/sprites.bmp"
1:
assets_sprites_size:
    .int 1b - assets_sprites
