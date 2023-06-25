; This is invalid assembly and will make the assembler
; to throw some errors :)

main:   LDI R1, 3
        LDI R2          ; Error: missing operand
        CALL mult
        ROL R5          ; Error: unsupported instruction
        STOP
        
mult:	LDI R0, 0
        CPI R2, 0
        BRZS end
loop:	ADD R0, R1
        SUBI R2, 1
        BRZS end
        JMP bucle       ; Error: missing line identifier
end:	RET
