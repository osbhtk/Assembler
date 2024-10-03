; Write your own assembly language source program.
; The program should contain at least 10 insructions, 5 labels, 5 directives
; No errors: the assembler should generate successfully all output files:
;     myprog.ob, myprog.ext, myprog.ent
.extern CORONA
.entry PEACE
.extern ISRAEL
.extern FOREVER
MAIN:   mov r1 , ISRAEL
        mov r2, FOREVER
        cmp r3,r3
        add #4,r4
        add r1,  r2
LOOP:   lea CORONA, r5
        clr X
        jsr PEACE
        not Y
PEACE:  .string "triple quotation " marks"
        inc X
        dec Y
        rts
        red r1
        sub CORONA, FOREVER
        prn X
        ; have a good day
        jmp &LOOP
        prn Y
X:      .data 1, 2, 3
        .data 4, 5, 6
        .data 7, 8, 9
Y:      .data -19
END:    stop

