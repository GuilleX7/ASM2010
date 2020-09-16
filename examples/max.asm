.equ valor1 = 7
.equ valor2 = 0x10

main:   LDI R1, valor1  ; Load example values
        LDI R2, valor2
        CALL max        ; Call the subroutine
        STOP            ; Stop CS2010

max:    MOV R0,R1       ; R0 <- R1
        CP R0, R2       ; Check R0 - R2
        BRLT r2g        ; Jumps if result is negative
        JMP end         ; R1 is greater, R0 is correct
r2g:    MOV R0, R2      ; R2 is greater, place it in R0
end:    RET             ; Ends the subroutine