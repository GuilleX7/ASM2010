; Type a number in the keyboard and you'll
; see it in the hexadecimal display!

.equ HEX_DISPLAY = 0x00
.equ KEYBOARD = 0x02
.equ ASCII_START = 48       ; This is the position of digit '0' in the ASCII table

main:   LDI R0, HEX_DISPLAY
        LDI R1, KEYBOARD
loop:   LD R2, (R1)          ; Get pressed key
ascii:  SUBI R2, ASCII_START ; Check if the pressed key is an ASCII digit
        BRLO loop
        CPI R2, 10
        BRLO show
        JMP loop

show:   LD R3, (R0)          ; Get the currently displayed number
        CLC                  ; Shift left 4 times (hides the left-most number)
        ROL R3
        CLC
        ROL R3
        CLC
        ROL R3
        CLC
        ROL R3
        ADD R3, R2           ; Add the new number (pressed key)
        ST (R0), R3          ; Show it
        JMP loop             ; Loop over
