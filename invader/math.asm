; Adds 2 16-bit numbers.
;
; Parameters:
;   a -> A (LSB)
;   b -> A (MSB)
;   c -> B (LSB)
;   d -> B (MSB)
;
; Returns:
;   a -> LSB
;   b -> MSB
add16:
    adr a, c
    jic __add16_carry

    adr b, d
    jmp __add16dn 

__add16_carry:
    adr b, d
    adi b, 1

__add16dn:
    ret
