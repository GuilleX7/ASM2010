main:   LDI R0, 0xfa
        MOV R1, R0
        MOV R2, R0
        SEC
        ROL R1
        CLC
        ROL R2
        CP R1, R2
        BREQ jump
        
jump:   LDI R3, 16
        LDI R4, 1
        ST R4, R3
        JMP jump2
        
jump2:  LD R5, R4
        STS 0x02, R5
        SUBI R5, 1
        CPI R5, 15
        BRLO jump3
        MOV R7, R5
        STOP
        
jump3:  STOP