; license:MIT License
; copyright-holders:Hitoshi Iwai

; rodata_user
; https://github.com/z88dk/z88dk/blob/master/doc/overview.md#a-quick-note-for-asm-code
; rodata_user if for constant data
; kept in rom if program is in rom
SECTION rodata_user
PUBLIC _MUSIC_00

; 0:TITLE MUSIC
include "../resources/MUSIC_00.asm"
