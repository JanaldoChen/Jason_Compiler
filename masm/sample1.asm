DOSSEG

_STACK SEGMENT para stack 'stack'
              dw      1000 dup(?)
_STACK ENDS

_DATA SEGMENT word public 'data'
TestResult     dw     ?
n              dw     ?
_DATA ENDS

_TEXT SEGMENT word public 'code'
         assume    cs: _TEXT

prim:

; set up the ds register so that it points to _DATA
         mov       ax, seg _DATA
         mov       ds, ax
         assume    ds: _DATA

; read n
         call      Readint
         mov       n, cx

; arg n
         mov       ax, n
         push      ax

; call pri
         call        pri

; pop these args
         pop       ax

; exit to DOS
         mov       ax, 4C00h
         int       21h

Readint:
         xor       cx, cx
         mov       bx, 10
Re1:
         mov       ah, 01h
         int       21h
         cmp       al, 13
         jz        Re2
         and       ax, 0fh
         xchg      ax, cx
         mul       bx
         add       ax, cx
         xchg      ax, cx
         jmp       Re1
Re2:
         ret
Writeint:
         xor       cx, cx
Wr1:
         mov       dx, 0
         mov       bx, 10
         div       bx
         push      dx
         inc       cx
         cmp       ax, 0
         je        Wr2
         jmp       Wr1
Wr2:
         pop       dx
         add       dl, 30h
         mov       ah, 02h
         int       21h
         loop      Wr2
         mov       dl, 0dh
         mov       ah, 02h
         int       21h
         mov       dl, 0ah
         int       21h
         ret
_TEXT ENDS

_TEXT SEGMENT
pri:

         push      bp
         mov       bp, sp
         add       bp, 2

; Allocate space for local variables
         sub       sp, 50
; m := 2
         mov       ax, 2
         mov       word ptr [bp-2], ax
; n := n + 1
         mov       ax, word ptr [bp+2]
         mov       bx, 1
         add       ax, bx
         mov       word ptr [bp+2], ax
Jump0:

_loop47:

; temp_0 := m - n
         mov       ax, word ptr [bp-2]
         mov       bx, word ptr [bp+2]
         sub       ax, bx
         mov       word ptr [bp-12], ax
Jump1:

; if temp_0 >= 0 goto _loop48
         cmp       word ptr [bp-12], 0
         jl        Jump2
         jmp       _loop48
Jump2:

; x := 2
         mov       ax, 2
         mov       word ptr [bp-4], ax
; f := 1
         mov       ax, 1
         mov       word ptr [bp-10], ax
_loop49:

; temp_1 := x - m
         mov       ax, word ptr [bp-4]
         mov       bx, word ptr [bp-2]
         sub       ax, bx
         mov       word ptr [bp-22], ax
Jump3:

; if temp_1 >= 0 goto _loop51
         cmp       word ptr [bp-22], 0
         jl        Jump4
         jmp       _loop51
Jump4:

; y := 2
         mov       ax, 2
         mov       word ptr [bp-6], ax
_loop52:

; temp_2 := y - m
         mov       ax, word ptr [bp-6]
         mov       bx, word ptr [bp-2]
         sub       ax, bx
         mov       word ptr [bp-32], ax
Jump5:

; if temp_2 >= 0 goto _loop54
         cmp       word ptr [bp-32], 0
         jl        Jump6
         jmp       _loop54
Jump6:

; z := x * y
         mov       ax, word ptr [bp-4]
         mov       bx, word ptr [bp-6]
         mul       bx
         mov       word ptr [bp-8], ax
Jump7:


; temp_3 := z - m
         mov       ax, word ptr [bp-8]
         mov       bx, word ptr [bp-2]
         sub       ax, bx
         mov       word ptr [bp-38], ax
Jump8:

; if temp_3 != 0 goto _loop56
         cmp       word ptr [bp-38], 0
         je        Jump9
         jmp       _loop56
Jump9:

; f := 0
         mov       ax, 0
         mov       word ptr [bp-10], ax
_loop56:

; y := y + 1
         mov       ax, word ptr [bp-6]
         mov       bx, 1
         add       ax, bx
         mov       word ptr [bp-6], ax
Jump10:

; goto _loop52
         jmp        _loop52

_loop54:

; x := x + 1
         mov       ax, word ptr [bp-4]
         mov       bx, 1
         add       ax, bx
         mov       word ptr [bp-4], ax
Jump11:

; goto _loop49
         jmp        _loop49

_loop51:

; temp_4 := f - 1
         mov       ax, word ptr [bp-10]
         mov       bx, 1
         sub       ax, bx
         mov       word ptr [bp-44], ax
Jump12:

; if temp_4 != 0 goto _loop59
         cmp       word ptr [bp-44], 0
         je        Jump13
         jmp       _loop59
Jump13:

; write m
         mov       ax, word ptr [bp-2]
         call      Writeint

_loop59:

; m := m + 1
         mov       ax, word ptr [bp-2]
         mov       bx, 1
         add       ax, bx
         mov       word ptr [bp-2], ax
Jump14:

; goto _loop47
         jmp        _loop47

_loop48:

; Return space used by local variables
         add       sp, 50
         pop       bp
         ret       2

_TEXT ENDS


END prim

