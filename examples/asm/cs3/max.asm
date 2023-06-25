; We want to check whether 7 is greater than 10,
; so we pass the values as parameters
main:   LDI R1, 7
        LDI R2, 10
        CALL max
        STOP

; This subroutine checks the max number between
; R1 and R2 and places the result in R0
max:    MOV R0, R1
        CP R0, R2
        BRLT r2g
        JMP end
r2g:    MOV R0, R2
end:    RET
