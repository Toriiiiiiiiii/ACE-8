_entry:
    cll setup_textmode
    cll reset_sprites

    ldi a, <string
    ldi b, >string
    cll print

    ldi a, 0x40
    sta a, 0x2004

    ldi a, 0x77
    sta a, 0x2005
    sta a, 0x2006

    ldi a, 0xC0
    sta a, 0x2007

_frame:
    cll waitblank

    lda a, 0x2105       ; Load controller status
    psh a

    ani a, 0b00000100
    jiz _chk_r

    lda b, 0x2005
    sbi b, 5
    sta b, 0x2005
_chk_r:
    pop a
    psh a

    ani a, 0b00001000
    jiz _chk_u

    lda b, 0x2005
    adi b, 5
    sta b, 0x2005

_chk_u:
    pop a
    psh a

    ani a, 0b00010000
    jiz _chk_d

    lda b, 0x2006
    sbi b, 5
    sta b, 0x2006
_chk_d:
    pop a
    psh a

    ani a, 0b00100000
    jiz _chk_dn

    lda b, 0x2006
    adi b, 5
    sta b, 0x2006

_chk_dn:
    jmp _frame

string: "Use Directional buttons to move." 0x00
%import "common.asm"
