.686
.model flat, C

.data
const_1 dd 1
const_2 dd 2
const_4 dd 4
const_41 dd 41

.code
calc PROC
push ebp
mov ebp, esp

finit 
fld dword ptr [ebp + 16]
fld dword ptr [ebp + 20]

fcompp
fstsw ax
sahf

jb _bigger
jge _lower

_lower:
finit
fild const_41
fld dword ptr[ebp + 20]
fidiv const_4
fsubp st(1), st
fisub const_1

fld dword ptr[ebp + 16]
fld dword ptr[ebp + 20]
fadd qword ptr[ebp + 8]
fptan
fdiv
fdivp st(1), st
fsub dword ptr[ebp + 20]

fdivp st(1), st

jmp _exit

_bigger:
finit
fldln2
fld qword ptr[ebp + 8]
fadd dword ptr[ebp + 20]
fyl2x
fmul dword ptr[ebp + 16]
fld st(0)
fld dword ptr[ebp + 16]
fimul const_2
fsubr st, st(1)

fld dword ptr[ebp + 16]
fidiv const_4
fisub const_1
fdivp st(1), st
fchs

_exit:
pop ebp
ret
calc ENDP
END