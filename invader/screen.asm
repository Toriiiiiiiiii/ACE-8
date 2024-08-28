; Sets the colour of tile at offset 0xBBAA
;
; Parameters:
;   a -> Offset (LSB)
;   b -> Offset (MSB)
;   c -> Colour
setcol:
    psh a
    psh b
    psh c
    psh d

    ldi c, 0x02
    ldi d, 0x14
    cll add16

    sta a, __setcol_tmp
    sta b, __setcol_tmp+1

    pop d
    pop c
    std c, __setcol_tmp

    pop b
    pop a
    ret

__setcol_tmp: 0x0000

waitblank:
    psh a

__waitloop:
    lda a, 0x1000
    jiz __waitloop

__waitdn:
    pop a
    ret

waitdraw:
    psh a

__drawloop:
    lda a, 0x1000
    jnz __drawloop

__drawdn:
    pop a
    ret

reset_sprites:
    psh a
    psh b
    psh c
    psh d
    psh e

    ldi a, 0x04
    ldi b, 0x20
    sta a, __rst_adr
    sta b, __rst_adr+1
    
    ldi e, 0
__rst_lp:
    ldi a, 0x00
    std a, __rst_adr

    lda a, __rst_adr
    lda b, __rst_adr+1
    ldi c, 1
    ldi d, 0
    cll add16

    sta a, __rst_adr
    sta b, __rst_adr+1

    adi e, 1
    jic __rst_spr_dn
    jmp __rst_lp

__rst_spr_dn:
    pop e
    pop d
    pop c
    pop b
    pop a
    ret

__rst_adr: 0x0000

__tilemap: 0x1001
__colrmap: 0x1402
__sprtmap: 0x2004

__tmapw: 0x20
__tmaph: 0x20

%import "math.asm"
