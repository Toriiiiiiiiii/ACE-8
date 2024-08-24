entry:
    cll setup_textmode
    cll reset_sprites

    ldi e, 0
_load_paddlesprl_lp:
    ldd a, _paddlesprl_addr
    std a, _paddlesprl_vmem_addr


    lda a, _paddlesprl_vmem_addr
    lda b, _paddlesprl_vmem_addr+1
    ldi c, 1
    ldi d, 0
    cll add16

    sta a, _paddlesprl_vmem_addr
    sta b, _paddlesprl_vmem_addr+1

    lda a, _paddlesprl_addr
    lda b, _paddlesprl_addr+1
    cll add16

    sta a, _paddlesprl_addr
    sta b, _paddlesprl_addr+1

    adi e, 1
    cpi e, 8
    jiz _load_paddlesprl_dn
    jmp _load_paddlesprl_lp

_load_paddlesprl_dn:
    ldi e, 0
_load_paddlesprr_lp:
    ldd a, _paddlesprr_addr
    std a, _paddlesprr_vmem_addr

    lda a, _paddlesprr_vmem_addr
    lda b, _paddlesprr_vmem_addr+1
    ldi c, 1
    ldi d, 0
    cll add16

    sta a, _paddlesprr_vmem_addr
    sta b, _paddlesprr_vmem_addr+1

    lda a, _paddlesprr_addr
    lda b, _paddlesprr_addr+1
    cll add16

    sta a, _paddlesprr_addr
    sta b, _paddlesprr_addr+1

    adi e, 1
    cpi e, 8
    jiz _load_paddlesprr_dn
    jmp _load_paddlesprr_lp

_load_paddlesprr_dn:
    ldi a, 1
    sta a, 0x2004

    ldi a, 2
    sta a, 0x2008

    ldi a, 127
    std a, _paddle_x

    adi a, 8
    std a, _paddle_spr2_x

    ldi a, 240
    std a, _paddle_y
    std a, _paddle_spr2_y

    ldi a, 0xa0
    sta a, 0x2007
    sta a, 0x200B

    ldi a, <str
    ldi b, >str
    cll print

;;;;; Frame Rendering Logic ;;;;;
_FRAME:
    cll waitblank

_CONTROLS:
    lda a, 0x2105 
    jiz _CONTROLS_DONE

_CHK_LEFTBUTTON:
    psh a
    ani a, 0b00000100
    jiz _CHK_RIGHTBUTTON

    ldd a, _paddle_x
    sbi a, 4
    std a, _paddle_x

_CHK_RIGHTBUTTON:
    pop a
    ani a, 0b00001000
    jiz _CONTROLS_DONE

    ldd a, _paddle_x
    adi a, 4
    std a, _paddle_x

_CONTROLS_DONE:

    ldd a, _paddle_x
    adi a, 8
    std a, _paddle_spr2_x

    jmp _FRAME

str:
    "Hello, World!", 0x0a, "This is a test.", 0x00

_paddle_x: 0x2005
_paddle_y: 0x2006

_paddle_spr2_x: 0x2009
_paddle_spr2_y: 0x200A

_paddlesprl_vmem_addr: 0x180B 
_paddlesprl_addr: _paddle_spr_l

_paddlesprr_vmem_addr: 0x1813
_paddlesprr_addr: _paddle_spr_r

_paddle_spr_r:
    0b00000000
    0b00000000
    0b00000001
    0b00000001
    0b00111111
    0b01111111
    0b11111111
    0b11111111

_paddle_spr_l:
    0b00000000
    0b00000000
    0b10000000
    0b10000000
    0b11111100
    0b11111110
    0b11111111
    0b11111111

%import "screen.asm"
