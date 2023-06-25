; We want to carry out 3 times 4, so we pass
; the values as parameters
main:   LDI R1, 3
        LDI R2, 4
        CALL mult
        STOP

; This subroutine multiplies R1 and R2 and stores
; the result in R1
mult:   LDI R0, 0
        CPI R1, 0
        BRZS end
        CPI R2, 0
        BRZS end
loop:   ADD R0, R1
        SUBI R2, 1
        BRZS end
        JMP loop
end:    RET
