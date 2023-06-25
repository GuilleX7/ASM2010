; We want to fill the memory from address 123 onwards
; with numbers starting from 100 and decreasing until
; it reaches 0 

        LDI R0, 100
        LDI R7, 123
loop:   ST Z, R0
        SUBI R7, -1
        SUBI R0, 1
        BRZS end
        JMP loop
end:    STOP
