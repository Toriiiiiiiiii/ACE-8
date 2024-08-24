; Prints a string to the screen.
; No support for escape codes.
; 
; Parameters:
;   a -> LSB of string address
;   b -> MSB of string address
print:
    sta a, __printadr       ; Store the pointer to the string
    sta b, __printadr+1

    psh a                   ; Save register values
    psh b
    psh c
    psh d

__printlp:
    ldd a, __printadr       ; Load the next character
    jiz __printdn           ; End loop if null character is reached

    cpi a, 0x0a
    jnz __print_nonewln

    lda a, __printadr
    lda b, __printadr+1
    ldi c, 1
    ldi d, 0
    cll add16

    sta a, __printadr
    sta b, __printadr+1

    lda a, __vidmemadr
    lda b, __vidmemadr+1

    dvi a, 0x20
    mli a, 0x20

    ldi c, 0x20
    ldi d, 0
    cll add16

    sta a, __vidmemadr
    sta b, __vidmemadr+1
    jmp __print_chkadr

__print_nonewln:
    std a, __vidmemadr      ; Store the character to video memory.

    lda a, __printadr       ; Increment the VMem and string pointers.
    lda b, __printadr+1
    ldi c, 1
    ldi d, 0
    cll add16

    sta a, __printadr
    sta b, __printadr+1

__print_chkadr:
    lda a, __vidmemadr 
    lda b, __vidmemadr+1
    ldi c, 1
    ldi d, 0
    cll add16

    cpi b, 0x14
    jin __print_afterchk

    cpi a, 0x01
    jin __print_afterchk

    ldi a, 0x01
    ldi b, 0x10

__print_afterchk:
    sta a, __vidmemadr
    sta b, __vidmemadr+1
    jmp __printlp

__printdn:
    pop d
    pop c
    pop b                   ; Restore register values
    pop a
    ret

__printadr: 0x0000
__vidmemadr: 0x1001

setup_textmode:
    psh a
    psh b
    psh c
    psh d

    ldi a, 0
    ldi b, 0
__setup_textmode_lp:
    ldi c, 0xf0
    cll setcol

    ldi c, 1
    ldi d, 0
    cll add16

    cpi b, 0x14
    jin __setup_textmode_lp

    cpi a, 0x02
    jin __setup_textmode_lp

__setup_textmode_dn:
    pop d
    pop c
    pop b
    pop a
    ret

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
