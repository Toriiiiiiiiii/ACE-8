    ; Memory locations for left sprite
    %define PADDLE_LEFT_SPR 0x2004
    %define PADDLE_LEFT_X   0x2005
    %define PADDLE_LEFT_Y   0x2006
    %define PADDLE_LEFT_COL 0x2007

    ; Memory locations for right sprite
    %define PADDLE_RIGHT_SPR 0x2008
    %define PADDLE_RIGHT_X   0x2009
    %define PADDLE_RIGHT_Y   0x200A
    %define PADDLE_RIGHT_COL 0x200B

    %define MISSILE_SPR 0x200C
    %define MISSILE_X   0x200D
    %define MISSILE_Y   0x200E
    %define MISSILE_COL 0x200F

    %define INVADER_LEFT_SPR 0x2010
    %define INVADER_LEFT_X   0x2011
    %define INVADER_LEFT_Y   0x2012
    %define INVADER_LEFT_COL 0x2013

    %define INVADER_RIGHT_SPR 0x2014
    %define INVADER_RIGHT_X   0x2015
    %define INVADER_RIGHT_Y   0x2016
    %define INVADER_RIGHT_COL 0x2017

    %define PADDLE_MOVE_SPEED 4
    %define MISSILE_SPEED 12

_entry:
    cll reset_sprites
    ldi a, 0xe0
    sta a, MISSILE_COL

setup_paddle:
_setup_left:
    ldi a, 1
    sta a, PADDLE_LEFT_SPR 

    ldi a, 124
    sta a, PADDLE_LEFT_X

    ldi a, 240
    sta a, PADDLE_LEFT_Y

    ldi a, 0xa0
    sta a, PADDLE_LEFT_COL

_setup_right:
    ldi a, 2
    sta a, PADDLE_RIGHT_SPR 

    ldi a, 132
    sta a, PADDLE_RIGHT_X

    ldi a, 240
    sta a, PADDLE_RIGHT_Y

    ldi a, 0xa0
    sta a, PADDLE_RIGHT_COL

setup_invader:
    ldi a, 5
    sta a, INVADER_LEFT_SPR
    adi a, 1
    sta a, INVADER_RIGHT_SPR

    ldi a, 123
    sta a, INVADER_LEFT_X
    adi a, 8
    sta a, INVADER_RIGHT_X

    ldi a, 0
    sta a, INVADER_LEFT_Y
    sta a, INVADER_RIGHT_Y

    ldi a, 0xc0
    sta a, INVADER_LEFT_COL
    sta a, INVADER_RIGHT_COL

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; PER-FRAME LOGIC - GAME LOOP &  ;
    ; RENDERING                      ;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
_frame:
    ; Wait for frame to finish drawing
    ; to avoid overwriting video memory
    ; during drawing. Not doing this can
    ; result in ugly artifacts on screen.
    cll waitblank
    
_check_controller:
    lda a, 0x2105
    jiz _controller_done

    psh a
_check_l:
    ani a, 0b00000100
    jiz _check_r

    lda a, PADDLE_LEFT_X
    jiz _check_r

    lda a, PADDLE_LEFT_X
    sbi a, PADDLE_MOVE_SPEED
    sta a, PADDLE_LEFT_X

    lda a, PADDLE_RIGHT_X
    sbi a, PADDLE_MOVE_SPEED
    sta a, PADDLE_RIGHT_X

_check_r:
    pop a
    psh a
    ani a, 0b00001000
    jiz _check_fire

    lda a, PADDLE_LEFT_X
    cpi a, 240
    jiz _check_fire

    lda a, PADDLE_LEFT_X
    adi a, PADDLE_MOVE_SPEED
    sta a, PADDLE_LEFT_X

    lda a, PADDLE_RIGHT_X
    adi a, PADDLE_MOVE_SPEED
    sta a, PADDLE_RIGHT_X

_check_fire:
    lda a, MISSILE_SPR
    jnz _controller_done

    pop a
    psh a
    ani a, 0b00000001
    jiz _controller_done

    lda a, PADDLE_LEFT_X
    adi a, 4
    sta a, MISSILE_X

    lda a, PADDLE_LEFT_Y
    sbi a, 4
    sta a, MISSILE_Y

    ldi a, 3
    sta a, MISSILE_SPR
    jmp _move_invader

_controller_done:
    pop a

_move_missile:
    lda a, MISSILE_SPR
    jiz _move_invader

    lda a, MISSILE_Y
    sbi a, MISSILE_SPEED
    sta a, MISSILE_Y

    lda a, MISSILE_Y
    cpi a, 232
    jin _move_invader

    ldi a, 0
    sta a, MISSILE_SPR

_move_invader:
    lda a, invader_direction
    jiz _inv_move_right

_inv_move_left:
    lda a, INVADER_LEFT_X
    lda b, invader_speed
    sbr a, b
    jic _invader_moveln

    sta a, INVADER_LEFT_X
    adi a, 8
    sta a, INVADER_RIGHT_X
    jmp _move_done

_inv_move_right:
    lda a, INVADER_LEFT_X
    lda b, invader_speed
    adr a, b
    jic _invader_moveln

    sta a, INVADER_LEFT_X
    adi a, 8
    sta a, INVADER_RIGHT_X
    
_move_done:
    jmp _invader_boundchk_done

_invader_moveln:
    lda a, INVADER_LEFT_Y
    adi a, 8
    sta a, INVADER_LEFT_Y
    sta a, INVADER_RIGHT_Y

    lda a, invader_direction
    jiz _inv_dir_left

    ldi a, 0
    jmp _inv_dirchk_done
_inv_dir_left:
    ldi a, 1
_inv_dirchk_done:
    sta a, invader_direction

    lda a, INVADER_LEFT_Y
    dvi a, 8
    mdi a, 2
    jiz _invader_boundchk_done

    lda a, invader_speed
    adi a, 1
    sta a, invader_speed

_invader_boundchk_done:
    ; Wait for next frame to draw to avoid
    ; updating multiple times a frame
    cll waitdraw
    jmp _frame

invader_speed: 0x02
invader_direction: 0x01

    %import "screen.asm"
    %import "background.asm"

    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; BITMAP DATA                    ;
    ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    %org 0x1803
    %incbin "graphics"
