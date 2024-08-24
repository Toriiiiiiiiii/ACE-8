_entry:
    cll setup_textmode
    cll reset_sprites

    ldi a, <str
    ldi b, >str
    cll print

    ldi a, 0x00
    ldi c, 0xf0
_lp:
    ldi b, 0
    cll setcol

    adi c, 1
    adi a, 1
    cpi c, 0xfd
    jin _lp

    jmp hlt

%import "common.asm"

str: "Hello, World!", 0x00
