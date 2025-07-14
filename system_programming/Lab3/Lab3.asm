.686 ; для процесора не нижче INTEL-586
.model flat, STDCALL ; компілювати як програму для WIN32
option casemap :none ; код чутливий до регістру літер
RemoveDirectoryA proto STDCALL, lpPathName : DWORD
MessageBoxA proto STDCALL, h : DWORD, lpText : DWORD, LPCSTR : DWORD, UINT : DWORD
ExitProcess proto STDCALL, uExitCode : DWORD

.data ; вміст сегменту даних:
hello_title db ' Лабораторна робота № 3 ', 0
hello_message db 'Directory deleted', 0
dir_path db 'E:\CPQ\temp', 0 ; Шлях до каталогу, який потрібно видалити

.code ; вміст сегменту коду:

Start:
    ; формування параметрів для виклику заданої функції
    push offset dir_path ; 1-й параметр: адреса шляху до каталогу
    call RemoveDirectoryA ; виклик функції API

    ; формування параметрів вікна для відображення результату
    push 40h ; стиль вікна - одна кнопка "OK" з піктограмою "і"
    push offset hello_title ; адреса рядка із заголовком
    push offset hello_message ; адреса рядка з повідомленням
    push 0 ; хендл програми-власника вікна
    call MessageBoxA ; виклик функції API

    push 0 ; код виходу з програми
    call ExitProcess ; завершення програми

end Start ; закінчення сегменту коду
