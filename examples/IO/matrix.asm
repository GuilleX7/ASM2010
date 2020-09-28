.equ MATRIX1 = 0x05
.equ MATRIX2 = 0x06
.equ BUTTONS = 0x02
.equ BTN_NO_REPEAT = 0x01

main:   LDI R0, MATRIX1       ; Load IO addresses
        LDI R1, MATRIX2
        LDI R2, BUTTONS
        LDI R3, BTN_NO_REPEAT ; Disable button repetition
        ST (R2), R3        
        JMP reset             ; Reset and start

loop:   LD R7, (R2)           ; Check for a button pressed
        CPI R7, 0
        BREQ loop

first:  CPI R3, 0x80          ; Check for the first overflow (at delta)
        BREQ r2strt
        CPI R5, 0xff          ; Or maybe it has already overflowed!
        BREQ second
        ADD R3, R3            ; Go for the next pixel at left side
        ADD R5, R3
        JMP update            ; Loop over

r2strt: LDI R3, 0             ; Set R3 to 0 so next time it won't appear
        LDI R6, 1             ; to be an overflow there
        JMP update

second: CPI R6, 0xff          ; Check for the second overflow
        BREQ reset            ; Reset it all
        ADD R4, R4            ; Go for the next pixel at right side
        ADD R6, R4

update: ST (R0), R5           ; Update matrix registers
        ST (R1), R6
        JMP loop              ; Loop over

reset:  LDI R3, 1             ; Delta for left side (R0)
        LDI R4, 1             ; Delta for right side (R1)
        LDI R5, 1             ; Counter for left side (R0)
        LDI R6, 0             ; Counter for right side (R1)
        JMP update