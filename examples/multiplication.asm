main:   LDI R1, 3
        LDI R2, 4
        CALL mult
        STOP
        
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