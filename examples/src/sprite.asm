_entry:
    ldi a, 0x40
    ldi b, 0x00
    ldi c, 0xC0
    sta a, 0x2004
    sta b, 0x2005
    sta b, 0x2006
    sta c, 0x2007

_lp:
    cll waitblank

    lda a, 0x2005
    adi a, 1
    sta a, 0x2005
    sta a, 0x2006

    jmp _lp

%import "common.asm"
