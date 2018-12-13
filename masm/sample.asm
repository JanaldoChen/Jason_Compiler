DOSSEG

_STACK SEGMENT para stack 'stack'
              dw      1000 dup(?)
_STACK ENDS

_DATA SEGMENT word public 'data'
TestResult     dw     ?
n              dw     ?
ans            dw     ?
_t53           dw     ?
_t56           dw     ?
_t58           dw     ?
_t61           dw     ?
_t63           dw     ?
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

; temp_5 := n - 3
         mov       ax, n
         mov       bx, 3
         sub       ax, bx
         mov       _t53, ax
Jump0:

; if temp_5 >= 0 goto _loop55
         cmp       _t53, 0
         jl        Jump1
         jmp       _loop55
Jump1:

; temp_6 := n
         mov       ax, n
         mov       _t56, ax
; if temp_6 <= 0 goto _loop57
         cmp       _t56, 0
         jg        Jump2
         jmp       _loop57
Jump2:

; ans := 1
         mov       ax, 1
         mov       ans, ax
; write ans
         mov       ax, ans
         call      Writeint

_loop57:

; temp_7 := n - 1
         mov       ax, n
         mov       bx, 1
         sub       ax, bx
         mov       _t58, ax
Jump3:

; if temp_7 <= 0 goto _loop59
         cmp       _t58, 0
         jg        Jump4
         jmp       _loop59
Jump4:

; ans := 1
         mov       ax, 1
         mov       ans, ax
; write ans
         mov       ax, ans
         call      Writeint

_loop59:

; goto _loop60
         jmp        _loop60

_loop55:

; temp_8 := n
         mov       ax, n
         mov       _t61, ax
; if temp_8 <= 0 goto _loop62
         cmp       _t61, 0
         jg        Jump5
         jmp       _loop62
Jump5:

; ans := 1
         mov       ax, 1
         mov       ans, ax
; write ans
         mov       ax, ans
         call      Writeint

_loop62:

; temp_9 := n - 1
         mov       ax, n
         mov       bx, 1
         sub       ax, bx
         mov       _t63, ax
Jump6:

; if temp_9 <= 0 goto _loop64
         cmp       _t63, 0
         jg        Jump7
         jmp       _loop64
Jump7:

; ans := 1
         mov       ax, 1
         mov       ans, ax
; write ans
         mov       ax, ans
         call      Writeint

_loop64:

; arg n
         mov       ax, n
         push      ax

; call f
         call        f

; pop these args
         pop       ax

_loop60:

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
         sub       sp, 54
; a := 1
         mov       ax, 1
         mov       word ptr [bp-2], ax
; a := a
         mov       ax, word ptr [bp-2]
         mov       word ptr [bp-2], ax
; b := 1
         mov       ax, 1
         mov       word ptr [bp-4], ax
; b := b + b
         mov       ax, word ptr [bp-4]
         mov       bx, word ptr [bp-4]
         add       ax, bx
         mov       word ptr [bp-4], ax
Jump8:

; b := b - 1
         mov       ax, word ptr [bp-4]
         mov       bx, 1
         sub       ax, bx
         mov       word ptr [bp-4], ax
Jump9:

; n := n - 2
         mov       ax, word ptr [bp+2]
         mov       bx, 2
         sub       ax, bx
         mov       word ptr [bp+2], ax
Jump10:

_loop47:

; temp_0 := n
         mov       ax, word ptr [bp+2]
         mov       word ptr [bp-8], ax
; if temp_0 <= 0 goto _loop48
         cmp       word ptr [bp-8], 0
         jg        Jump11
         jmp       _loop48
Jump11:

; temp_2 := a + b
         mov       ax, word ptr [bp-2]
         mov       bx, word ptr [bp-4]
         add       ax, bx
         mov       word ptr [bp-16], ax
Jump12:

; c := temp_2
         mov       ax, word ptr [bp-16]
         mov       word ptr [bp-6], ax
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
Jump13:

; goto _loop47
         jmp        _loop47

_loop48:

; Return space used by local variables
         add       sp, 54
         pop       bp
         ret       2

_TEXT ENDS


END fib

