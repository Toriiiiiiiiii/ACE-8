_entry:
	ldi a, 0xcc

	ldi b, 0xff
	ldi c, 0x1
	adr b, c
	jic success
	jmp done

success:
	ldi a, 0xaa

done:
	sta a, 0x1402

_hlt:
	jmp _hlt
