; Type a number in the keyboard and you'll
; see it in the hexadecimal display!

.equ HEX_DISPLAY = 0x00
.equ KEYBOARD = 0x02
.equ ASCII_START = 48       ; This is the position of digit '0' in the ASCII table

main:   LDI R6, HEX_DISPLAY
        LDI R7, KEYBOARD
loop:   LD R2, Z             ; Get pressed key
ascii:  SUBI R2, ASCII_START ; Check if the pressed key is an ASCII digit
        BRLO loop
        CPI R2, 10
        BRLO show
        JMP loop

show:   ST Y, R2             ; Show it
        JMP loop             ; Loop over
