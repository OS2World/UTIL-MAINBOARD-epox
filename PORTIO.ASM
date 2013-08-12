; base if for the parameter passing, 4 for near calls, 6 for far.
BASE    EQU     6

.386

_TEXTIOPL SEGMENT WORD USE16 PUBLIC 'CODE'
        ASSUME  CS:_TEXTIOPL,DS:NOTHING

; inp (byte)

        PUBLIC  INP
INP     PROC    FAR
        PUSH    BP
        MOV     BP,SP

        PUSH    DX

        MOV     DX,WORD PTR [BP+BASE]   ; get port address
        IN      AL,DX                   ; read from port
        XOR     AH,AH                   ; clear out the high part of ax
        POP     DX

        POP     BP
        RET     2
INP     ENDP

; outp (byte)

        PUBLIC  OUTP
OUTP    PROC    FAR
        PUSH    BP
        MOV     BP,SP

        PUSH    DX
        MOV     DX,WORD PTR [BP+BASE+2] ; get port address
        MOV     AX,WORD PTR [BP+BASE]   ; get value to write
        OUT     DX,AL                   ; write to port
        POP     DX

        POP     BP
        RET     4
OUTP    ENDP

; inpw (word)

        PUBLIC  INPW
INPW    PROC    FAR
        PUSH    BP
        MOV     BP,SP

        PUSH    DX

        MOV     DX,WORD PTR [BP+BASE]   ; get port address
        IN      AX,DX                   ; read from port
        POP     DX

        POP     BP
        RET     2
INPW    ENDP

; outpw (word)

        PUBLIC  OUTPW
OUTPW   PROC    FAR
        PUSH    BP
        MOV     BP,SP

        PUSH    DX
        MOV     DX,WORD PTR [BP+BASE+2] ; get port address
        MOV     AX,WORD PTR [BP+BASE]   ; get value to write
        OUT     DX,AX                   ; write to port
        POP     DX
        POP     BP
        RET     4
OUTPW   ENDP

_TEXTIOPL ENDS
        END
