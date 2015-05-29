#ifdef __WIN32__
    .global _assets_font
    .global _assets_font_size
    .global _assets_sprites
    .global _assets_sprites_size
#else
    .global assets_font
    .global assets_font_size
    .global assets_sprites
    .global assets_sprites_size
#endif

    .section .rodata

#ifdef __WIN32__
_assets_font:
#else
assets_font:
#endif
    .incbin "../assets/font.bmp"
1:
#ifdef __WIN32__
_assets_font_size:
    .int 1b - _assets_font
#else
assets_font_size:
    .int 1b - assets_font
#endif

#if __WIN32__
_assets_sprites:
#else
assets_sprites:
#endif
    .incbin "../assets/sprites.bmp"
1:
#if __WIN32__
_assets_sprites_size:
    .int 1b - _assets_sprites
#else
assets_sprites_size:
    .int 1b - assets_sprites
#endif
