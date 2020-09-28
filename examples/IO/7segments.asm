.equ SEG_DISPLAY = 0x04
.equ BUTTONS = 0x02
.equ BTN_NO_REPEAT = 0x01

; 7-segment display codes
;
;       a
;     -----
;  f |     | b
;    |     |
;     -----
;  e |  g  | c
;    |     | 
;     ----- . dp
;       d
;
;              abcdefg.
.equ SEG_A = 0b11101110
.equ SEG_B = 0b00111110
.equ SEG_C = 0b10011100
.equ SEG_D = 0b01111010
.equ SEG_E = 0b10011110
.equ SEG_F = 0b10001110

main: LDI R0, SEG_DISPLAY  ; Load IO addresses
      LDI R1, BUTTONS
      LDI R2, BTN_NO_REPEAT
      ST (R1), R2          ; Disable button repetition
      
loop: LD R3, (R1)          ; Get pressed button
      CPI R3, 0            ; Check for any pressed
      BREQ loop            ; Loop over
      CPI R3, 0x01         ; Button A pressed
      BREQ bt_a
      CPI R3, 0x02         ; Button B pressed
      BREQ bt_b
      CPI R3, 0x04         ; Button C pressed
      BREQ bt_c
      CPI R3, 0x08         ; Button D pressed
      BREQ bt_d
      CPI R3, 0x10         ; Button E pressed
      BREQ bt_e
      CPI R3, 0x20         ; Button F pressed
      BREQ bt_f
      JMP loop             ; Loop over

bt_a: LDI R3, SEG_A
      ST (R0), R3
      JMP loop

bt_b: LDI R3, SEG_B
      ST (R0), R3
      JMP loop

bt_c: LDI R3, SEG_C
      ST (R0), R3
      JMP loop

bt_d: LDI R3, SEG_D
      ST (R0), R3
      JMP loop

bt_e: LDI R3, SEG_E
      ST (R0), R3
      JMP loop

bt_f: LDI R3, SEG_F
      ST (R0), R3
      JMP loop