.equ HEX_DISPLAY1 = 0x00
.equ HEX_DISPLAY2 = 0x01
.equ KEYBOARD = 0x03
.equ ASCII_START = 48

main:   LDI R0, HEX_DISPLAY1 ; Load IO addresses
        LDI R1, HEX_DISPLAY2
        LDI R1, KEYBOARD
loop:   LD R3, (R1)          ; Get pressed key
ascii:  SUBI R3, ASCII_START ; Check whether 48 <= key < 58,
        BRLT loop            ; as the key would be an ascii decimal
        CPI R3, 10           ; character
        BRLT show
        JMP loop             ; Loop over
show:   LD R4, (R0)          ; Get actual number on hex display 1
        CLC                  ; Shift left 4 times
        ROL R4
        CLC
        ROL R4
        CLC
        ROL R4
        CLC
        ROL R4
        ADD R3, R4           ; Add the new number (pressed key)
        ST (R0), R3          ; Show it
        JMP loop             ; Loop over