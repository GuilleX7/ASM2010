main:   CALL count   ; Call the subroutine
        STOP         ; Stop CS2010

count:  LDI R0, 100  ; Set R0 to 100
        LDI R1, 123  ; Set R1 to 123 (base register)
loop:   ST (R1), R0  ; Store at (R1) the content of R0
        ADDI R1, 1   ; Update base register
        SUBI R0, 1   ; Calculate new data
        BRZS end     ; End loop when R0 == 1
        JMP loop     ; Loop over
end:    RET          ; End the subroutine