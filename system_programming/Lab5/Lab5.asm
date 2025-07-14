.686
.model flat, stdcall
option casemap:none ; ³��������� ��������� �� ������� ������� � �������

printf proto c: vararg ; �������� ������� printf � ����������� ������� ���������

include \masm32\include\windows.inc
include \masm32\include\kernel32.inc
include \masm32\include\user32.inc
include \masm32\include\msvcrt.inc
includelib \masm32\lib\kernel32.lib
includelib \masm32\lib\msvcrt.lib

.data
inputValue db 256 dup(0) ; ����� ��� �������� �����
hConsoleInput dd 0 ; ���������� ������ ��� �����
hConsoleOutput dd 0 ; ���������� ������ ��� ������
NumberOfChars dd 0 ; ʳ������ ��������/��������� �������
msgInput db "Enter input: ", 0 ; ����������� ��� ��������

LibName db "C:\Users\³��\Downloads\CP\MathLibrary\Debug\MathLibrary.dll",0 ; ���� �� DLL
FunctionName db "CountAInLastWord",0 ; ��'� ������� � DLL

DllNotFound db "Cannot load library",0 ; ����������� ��� ������� ������������ DLL
AppName db "Load explicit Library",0 ; ����� �������
NotFound db "TestFunction function not found",0 ; ����������� ��� ��������� ������� � DLL
NotFound2 db "2+2=4",0 

resultFormat db "Count letters 'a': %d",0 ; ������ ��� ������ ����������

.data?

hLib dd ?
TestFunctionAddr dd ?
result db ? ; ����� ��� ���������� ���������� �������

.code
start:
invoke GetStdHandle, -11 ; �������� ���������� ������ ������
mov hConsoleOutput, eax

invoke WriteConsoleA, hConsoleOutput, addr msgInput, 13, addr NumberOfChars, 0 ; ������� ����������� ��� ��������

invoke GetStdHandle, -10 ; �������� ���������� ����� ������
mov hConsoleInput, eax

invoke ReadConsoleA, hConsoleInput, addr inputValue, 32, addr NumberOfChars, 0 ; ������� ������ ���

invoke crt_atoi, addr inputValue ; ������������ ����� � �����

invoke LoadLibrary, addr LibName ; ����������� DLL

.if eax == NULL ; ���� ������������ �� �������
invoke MessageBox, NULL, addr DllNotFound, addr AppName, MB_OK
.else
mov hLib, eax ; �������� ���������� DLL

invoke GetProcAddress, hLib, addr FunctionName ; �������� ������ �������

.if eax == NULL ; ���� �� ������� �������� ������
invoke printf, addr NotFound2
invoke MessageBox, NULL, addr NotFound, addr AppName, MB_OK
.else
push offset inputValue
mov TestFunctionAddr, eax
call [TestFunctionAddr] ; ��������� �������

invoke printf, addr resultFormat, eax ; ������� ��������� �� �������
.endif

invoke FreeLibrary, hLib ; ����������� DLL
.endif

invoke ExitProcess, NULL
end start