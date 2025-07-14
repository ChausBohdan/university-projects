.686 ; ��� ��������� �� ����� INTEL-586
.model flat, STDCALL ; ���������� �� �������� ��� WIN32
option casemap :none ; ��� �������� �� ������� ����
RemoveDirectoryA proto STDCALL, lpPathName : DWORD
MessageBoxA proto STDCALL, h : DWORD, lpText : DWORD, LPCSTR : DWORD, UINT : DWORD
ExitProcess proto STDCALL, uExitCode : DWORD

.data ; ���� �������� �����:
hello_title db ' ����������� ������ � 3 ', 0
hello_message db 'Directory deleted', 0
dir_path db 'E:\CPQ\temp', 0 ; ���� �� ��������, ���� ������� ��������

.code ; ���� �������� ����:

Start:
    ; ���������� ��������� ��� ������� ������ �������
    push offset dir_path ; 1-� ��������: ������ ����� �� ��������
    call RemoveDirectoryA ; ������ ������� API

    ; ���������� ��������� ���� ��� ����������� ����������
    push 40h ; ����� ���� - ���� ������ "OK" � ���������� "�"
    push offset hello_title ; ������ ����� �� ����������
    push offset hello_message ; ������ ����� � ������������
    push 0 ; ����� ��������-�������� ����
    call MessageBoxA ; ������ ������� API

    push 0 ; ��� ������ � ��������
    call ExitProcess ; ���������� ��������

end Start ; ��������� �������� ����
