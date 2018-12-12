DOSSEG

_STACK SEGMENT para stack 'stack'
              dw      1000 dup(?)
_STACK ENDS

_DATA SEGMENT word public 'data'
TestResult     dw     ?
n              dw     ?
ans            dw     ?
_t49           dw     ?
_t52           dw     ?
_t54           dw     ?
_t57           dw     ?
_t59           dw     ?
_DATA ENDS

_TEXT SEGMENT word public 'code'
         assume    cs: _TEXT

fib:

; set up the ds register so that it points to _DATA
         mov       ax, seg _DATA
         mov       ds, ax
         assume    ds: _DATA

; read n
         call      Readint
         mov       n, cx

; temp_1 := n - 3
         mov       ax, n
         mov       bx, 3
         sub       ax, bx
         mov       _t49, ax
Jump0:

; if temp_1 >= 0 goto _loop51
         cmp       _t49, 0
         jl        Jump1
         jmp       _loop51
Jump1:

; temp_2 := n
         mov       ax, n
         mov       _t52, ax
; if temp_2 <= 0 goto _loop53
         cmp       _t52, 0
         jg        Jump2
         jmp       _loop53
Jump2:

; ans := 1
         mov       ax, 1
         mov       ans, ax
; write ans
         mov       ax, ans
         call      Writeint

_loop53:

; temp_3 := n - 1
         mov       ax, n
         mov       bx, 1
         sub       ax, bx
         mov       _t54, ax
Jump3:

; if temp_3 <= 0 goto _loop55
         cmp       _t54, 0
         jg        Jump4
         jmp       _loop55
Jump4:

; ans := 1
         mov       ax, 1
         mov       ans, ax
; write ans
         mov       ax, ans
         call      Writeint

_loop55:

; goto _loop56
         jmp        _loop56

_loop51:

; temp_4 := n
         mov       ax, n
         mov       _t57, ax
; if temp_4 <= 0 goto _loop58
         cmp       _t57, 0
         jg        Jump5
         jmp       _loop58
Jump5:

; ans := 1
         mov       ax, 1
         mov       ans, ax
; write ans
         mov       ax, ans
         call      Writeint

_loop58:

; temp_5 := n - 1
         mov       ax, n
         mov       bx, 1
         sub       ax, bx
         mov       _t59, ax
Jump6:

; if temp_5 <= 0 goto _loop60
         cmp       _t59, 0
         jg        Jump7
         jmp       _loop60
Jump7:

; ans := 1
         mov       ax, 1
         mov       ans, ax
; write ans
         mov       ax, ans
         call      Writeint

_loop60:

; arg n
         mov       ax, n
         push      ax

; call f
         call        f

; pop these args
         pop       ax

_loop56:

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
f:

         push      bp
         mov       bp, sp
         add       bp, 2

; Allocate space for local variables
         sub       sp, 46
; a := 1
         mov       ax, 1
         mov       word ptr [bp-2], ax
; b := 1
         mov       ax, 1
         mov       word ptr [bp-4], ax
; n := n - 2
         mov       ax, word ptr [bp+2]
         mov       bx, 2
         sub       ax, bx
         mov       word ptr [bp+2], ax
Jump8:

_loop46:

; temp_0 := n
         mov       ax, word ptr [bp+2]
         mov       word ptr [bp-8], ax
; if temp_0 <= 0 goto _loop48
         cmp       word ptr [bp-8], 0
         jg        Jump9
         jmp       _loop48
Jump9:

; c := a + b
         mov       ax, word ptr [bp-2]
         mov       bx, word ptr [bp-4]
         add       ax, bx
         mov       word ptr [bp-6], ax
Jump10:

; write c
         mov       ax, word ptr [bp-6]
         call      Writeint

; a := b
         mov       ax, word ptr [bp-4]
         mov       word ptr [bp-2], ax
; b := c
         mov       ax, word ptr [bp-6]
         mov       word ptr [bp-4], ax
; n := n - 1
         mov       ax, word ptr [bp+2]
         mov       bx, 1
         sub       ax, bx
         mov       word ptr [bp+2], ax
Jump11:

; goto _loop46
         jmp        _loop46

_loop48:

; Return space used by local variables
         add       sp, 46
         pop       bp
         ret       2

_TEXT ENDS


END fib

