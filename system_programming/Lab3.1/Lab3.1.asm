RemoveDirectoryA proto
MessageBoxA proto
ExitProcess proto

.data
hello_title db ' ����������� ������ � 3 ', 0
hello_message db 'Directory deleted', 0
dir_path db 'C:\CPQ', 0

.code
mainCRTStartup proc 
    sub rsp, 28h
    ; ������ RemoveDirectoryA
    lea rcx, dir_path
    call RemoveDirectoryA

    ; ������ MessageBoxA
    mov r9, 40h ; ����� ���� - ���� ������ "OK" � ���������� "�"
    lea r8, hello_title ; ������ ����� �� ����������
    lea rdx, hello_message ; ������ ����� � ������������
    mov rcx, 0 ; ����� ��������-�������� ����
    call MessageBoxA ; ������ ������� API

    ; ������ ExitProcess
    push 0
    call ExitProcess

add rsp, 28h
ret
mainCRTStartup endp 
end
