RemoveDirectoryA proto
MessageBoxA proto
ExitProcess proto

.data
hello_title db ' Лабораторна робота № 3 ', 0
hello_message db 'Directory deleted', 0
dir_path db 'C:\CPQ', 0

.code
mainCRTStartup proc 
    sub rsp, 28h
    ; Виклик RemoveDirectoryA
    lea rcx, dir_path
    call RemoveDirectoryA

    ; Виклик MessageBoxA
    mov r9, 40h ; стиль вікна - одна кнопка "OK" з піктограмою "і"
    lea r8, hello_title ; адреса рядка із заголовком
    lea rdx, hello_message ; адреса рядка з повідомленням
    mov rcx, 0 ; хендл програми-власника вікна
    call MessageBoxA ; виклик функції API

    ; Виклик ExitProcess
    push 0
    call ExitProcess

add rsp, 28h
ret
mainCRTStartup endp 
end
