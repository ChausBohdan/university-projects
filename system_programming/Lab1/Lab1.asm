.686
.model flat, c
printf proto c: vararg
Extern K : DWORD

.data
msg db 'Output from asm module is:   %d', 0
.code
calc PROC
push ebp
mov ebp, esp
mov edx, K ; Завантаження К
mov eax, dword ptr [ebp+8] ; Завантаження A
mov ebx, dword ptr [ebp+12]  ; Завантаження C
movsx ecx, byte ptr [ebp+16] ; Завантаження E

test ebx, ebx
js negative
shr ebx, 1 ; Ділення C/2
jmp over

negative:
neg ebx
shr ebx, 1
neg ebx 

over:
shl ecx, 3 ; Множення E*8
sub eax, edx ; Віднімання A-K
add eax, ebx ; Додавання A-K+C/2
sub eax, ecx ; Віднімання A-K+C/2-E*8

pop ebp
push eax
invoke printf, offset msg, eax   
pop eax
ret
calc ENDP
END