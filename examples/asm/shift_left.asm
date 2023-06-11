LDI R1, 0xff
LDI R2, 0xff

; To perform a clean shift-left, we need to clear the carry bit
; before each shift
CLC
ROL R1
CLC
ROL R1
CLC
ROL R1
; This results in R1 = 0b1111 1000

STOP
