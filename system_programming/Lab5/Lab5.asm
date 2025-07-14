.686
.model flat, stdcall
option casemap:none ; Відключення чутливості до регістру символів в зразках

printf proto c: vararg ; Прототип функції printf з необмеженою кількістю аргументів

include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\msvcrt.inc
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\msvcrt.lib

.data
inputValue db 256 dup(0) ; Буфер для введених даних
hConsoleInput dd 0 ; Дескриптор консолі для вводу
hConsoleOutput dd 0 ; Дескриптор консолі для виводу
NumberOfChars dd 0 ; Кількість введених/виведених символів
msgInput db "Enter input: ", 0 ; Повідомлення для введення

LibName db "C:\Users\Віка\Downloads\CP\MathLibrary\Debug\MathLibrary.dll",0 ; Шлях до DLL
FunctionName db "CountAInLastWord",0 ; Ім'я функції в DLL

DllNotFound db "Cannot load library",0 ; Повідомлення про помилку завантаження DLL
AppName db "Load explicit Library",0 ; Назва додатку
NotFound db "TestFunction function not found",0 ; Повідомлення про відсутність функції в DLL
NotFound2 db "2+2=4",0 

resultFormat db "Count letters 'a': %d",0 ; Формат для виводу результату

.data?

hLib dd ?
TestFunctionAddr dd ?
result db ? ; Змінна для збереження результату функції

.code
start:
invoke GetStdHandle, -11 ; Отримати дескриптор виводу консолі
mov hConsoleOutput, eax

invoke WriteConsoleA, hConsoleOutput, addr msgInput, 13, addr NumberOfChars, 0 ; Вивести повідомлення для введення

invoke GetStdHandle, -10 ; Отримати дескриптор вводу консолі
mov hConsoleInput, eax

invoke ReadConsoleA, hConsoleInput, addr inputValue, 32, addr NumberOfChars, 0 ; Зчитати введені дані

invoke crt_atoi, addr inputValue ; Конвертувати рядок в число

invoke LoadLibrary, addr LibName ; Завантажити DLL

.if eax == NULL ; Якщо завантаження не вдалося
invoke MessageBox, NULL, addr DllNotFound, addr AppName, MB_OK
.else
mov hLib, eax ; Зберегти дескриптор DLL

invoke GetProcAddress, hLib, addr FunctionName ; Отримати адресу функції

.if eax == NULL ; Якщо не вдалося отримати адресу
invoke printf, addr NotFound2
invoke MessageBox, NULL, addr NotFound, addr AppName, MB_OK
.else
push offset inputValue
mov TestFunctionAddr, eax
call [TestFunctionAddr] ; Викликати функцію

invoke printf, addr resultFormat, eax ; Вивести результат на консоль
.endif

invoke FreeLibrary, hLib ; Вивантажити DLL
.endif

invoke ExitProcess, NULL
end start