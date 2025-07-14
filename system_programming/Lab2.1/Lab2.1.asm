.data
const_1 dd 1
const_2 dd 2
const_4 dd 4
const_41 dd 41

.code
calc PROC
movlps qword ptr[rsp+8], xmm0 ; a
movss dword ptr[rsp + 16], xmm1 ; c
movss dword ptr[rsp + 20], xmm2 ; d

finit 
fld dword ptr [rsp + 16]
fld dword ptr [rsp + 20]

fcompp
fstsw ax
sahf

jb _bigger
jge _lower

_lower:
finit
fild const_41
fld dword ptr[rsp + 20]
fidiv const_4
fsubp st(1), st
fisub const_1

fld dword ptr[rsp + 16]
fld dword ptr[rsp + 20]
fadd qword ptr[rsp + 8]
fptan
fdiv
fdivp st(1), st
fsub dword ptr[rsp + 20]

fdivp st(1), st

jmp _exit

_bigger:
finit
fldln2
fld qword ptr[rsp + 8]
fadd dword ptr[rsp + 20]
fyl2x
fmul dword ptr[rsp + 16]
fld st(0)
fld dword ptr[rsp + 16]
fimul const_2
fsubr st, st(1)

fld dword ptr[rsp + 16]
fidiv const_4
fisub const_1
fdivp st(1), st
fchs

_exit:
fst qword ptr[rsp + 8]
movlps xmm0, qword ptr[rsp+8]
ret
calc ENDP
END