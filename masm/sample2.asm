DOSSEG

_STACK SEGMENT para stack 'stack'
              dw      1000 dup(?)
_STACK ENDS

_DATA SEGMENT word public 'data'
TestResult     dw     ?
n              dw     ?
m              dw     ?
t              dw     ?
ans            dw     ?
_t56           dw     ?
_DATA ENDS

_TEXT SEGMENT word public 'code'
         assume    cs: _TEXT

cgcd:

; set up the ds register so that it points to _DATA
         mov       ax, seg _DATA
         mov       ds, ax
         assume    ds: _DATA

; read n
         call      Readint
         mov       n, cx

; read m
         call      Readint
         mov       m, cx

; temp_3 := n - m
         mov       ax, n
         mov       bx, m
         sub       ax, bx
         mov       _t56, ax
Jump0:

; if temp_3 <= 0 goto _loop57
         cmp       _t56, 0
         jg        Jump1
         jmp       _loop57
Jump1:

; t := n
         mov       ax, n
         mov       t, ax
; n := m
         mov       ax, m
         mov       n, ax
; m := t
         mov       ax, t
         mov       m, ax
_loop57:

; arg n
         mov       ax, n
         push      ax

; arg m
         mov       ax, m
         push      ax

; call gcd
         call        gcd

; pop these args
         pop       ax
         pop       ax

; write ans
         mov       ax, ans
         call      Writeint

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
gcd:

         push      bp
         mov       bp, sp
         add       bp, 2

; Allocate space for local variables
         sub       sp, 28
; t := 1
         mov       ax, 1
         mov       word ptr [bp-2], ax
; ans := 1
         mov       ax, 1
         mov       ans, ax
; u := n + 1
         mov       ax, word ptr [bp+4]
         mov       bx, 1
         add       ax, bx
         mov       word ptr [bp-4], ax
Jump2:

_loop49:

; temp_0 := t - u
         mov       ax, word ptr [bp-2]
         mov       bx, word ptr [bp-4]
         sub       ax, bx
         mov       word ptr [bp-10], ax
Jump3:

; if temp_0 >= 0 goto _loop50
         cmp       word ptr [bp-10], 0
         jl        Jump4
         jmp       _loop50
Jump4:

; p := n / t
         xor       dx, dx
         mov       ax, word ptr [bp+4]
         mov       bx, word ptr [bp-2]
         div       bx
         mov       word ptr [bp-6], ax
Jump5:


; q := p * t
         mov       ax, word ptr [bp-6]
         mov       bx, word ptr [bp-2]
         mul       bx
         mov       word ptr [bp-8], ax
Jump6:


; temp_1 := q - n
         mov       ax, word ptr [bp-8]
         mov       bx, word ptr [bp+4]
         sub       ax, bx
         mov       word ptr [bp-16], ax
Jump7:

; if temp_1 != 0 goto _loop52
         cmp       word ptr [bp-16], 0
         je        Jump8
         jmp       _loop52
Jump8:

; p := m / t
         xor       dx, dx
         mov       ax, word ptr [bp+2]
         mov       bx, word ptr [bp-2]
         div       bx
         mov       word ptr [bp-6], ax
Jump9:


; q := p * t
         mov       ax, word ptr [bp-6]
         mov       bx, word ptr [bp-2]
         mul       bx
         mov       word ptr [bp-8], ax
Jump10:


; temp_2 := q - m
         mov       ax, word ptr [bp-8]
         mov       bx, word ptr [bp+2]
         sub       ax, bx
         mov       word ptr [bp-22], ax
Jump11:

; if temp_2 != 0 goto _loop54
         cmp       word ptr [bp-22], 0
         je        Jump12
         jmp       _loop54
Jump12:

; ans := t
         mov       ax, word ptr [bp-2]
         mov       ans, ax
_loop54:

_loop52:

; t := t + 1
         mov       ax, word ptr [bp-2]
         mov       bx, 1
         add       ax, bx
         mov       word ptr [bp-2], ax
Jump13:

; goto _loop49
         jmp        _loop49

_loop50:

; Return space used by local variables
         add       sp, 28
         pop       bp
         ret       2

_TEXT ENDS


END cgcd

