main:   LDI R1, 3
        LDI R2, 4
        CALL mult
        STOP
        
mult:   LDI R0, 0
        CPI R2, 0
        BRZS retorna
        CPI R2, 0
        BRZS retorna
loop:   ADD R0, R1
        SUBI R2, 1
        BRZS retorna
        JMP bucle
end:    RET