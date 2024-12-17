.586
.model flat, stdcall
includelib kernel32.lib
includelib ../Debug/IPP-2024L.lib
ExitProcess proto : dword
WriteConsoleA proto : dword, : dword, : dword, : dword, : dword
GetStdHandle proto : dword

.const
STD_OUTPUT_HANDLE equ -11 
error_message_overflow byte "Ошибка 333: Семантический анализатор: переполнение в выражении", 0
error_message_division_by_zero byte "Ошибка 334: Семантический анализатор: деление на ноль", 0
error_message_index byte "Ошибка 324: Семантический анализатор: индекс выходит за границы массива", 0
true_message byte "true", 0
false_message byte "false", 0

.data
newLine db 13, 10, 0 
buffer byte " ", 0

.code
; Обработчик ошибки переполнения. 
; Выводит сообщение об ошибке и завершает программу
overflow_error_message proc
	push offset error_message_overflow
	call PrintConsoleNewLine
	push -1
	call ExitProcess
	ret
overflow_error_message endp

; Обработчик ошибки деления на ноль. 
; Выводит сообщение об ошибке и завершает программу
division_by_zero_error_message proc
	push offset error_message_division_by_zero
	call PrintConsoleNewLine
	push -1
	call ExitProcess
	ret
division_by_zero_error_message endp

; Обработчик ошибки выхода за границы массива. 
; Выводит сообщение об ошибке и завершает программу
index_error_message proc
    push offset error_message_index
    call PrintConsoleNewLine
	push -1
	call ExitProcess
	ret
index_error_message endp

; Получение элемента строкового массива по индексу с проверкой границ
; Входные параметры: индекс, адрес массива, размер массива
; Возвращает адрес элемента или вызывает обработчик ошибки
GetStringArrayElement proc
    push ebp
    mov ebp, esp
    push esi
    push ebx

    mov edx, [ebp + 16]  ; Размер массива в элементах
    mov esi, [ebp + 12]  ; Адрес начала массива
    mov ebx, [ebp + 8]   ; Индекс элемента

    cmp ebx, 0
    jl index_error       ; Перейти к обработке ошибки, если индекс < 0
    cmp ebx, edx
    jge index_error      ; Перейти к обработке ошибки, если индекс >= размера массива

    ; Возвращаем адрес элемента массива
    lea eax, [esi + ebx * 4]  ; Вычисляем адрес элемента массива
    jmp exit_proc

index_error:
    call index_error_message ; Вызов обработчика ошибки
    xor eax, eax             ; Обнуляем возвращаемое значение
    
exit_proc:
    pop ebx
    pop esi
    mov esp, ebp
    pop ebp
    ret 12                   ; Возврат с очисткой стека
GetStringArrayElement endp

; Получение элемента массива по индексу с проверкой границ
; Входные параметры:
;   - Индекс элемента
;   - Адрес массива
;   - Размер массива
; Выход: Адрес элемента или обработка ошибки
GetIntArrayElement proc
    push ebp
    mov ebp, esp
    push esi
    push ebx
    
    mov edx, [ebp + 16]  ; Размер массива
    mov esi, [ebp + 12]  ; Адрес массива
    mov ebx, [ebp + 8]   ; Индекс элемента
    
    cmp ebx, 0
    jl index_error       ; Перейти к обработке ошибки, если индекс < 0
    cmp ebx, edx
    jge index_error      ; Перейти к обработке ошибки, если индекс >= размера массива
    
    ; Вычисляем адрес элемента массива напрямую
    lea eax, [esi + ebx * 4]  ; Адрес элемента в eax
    jmp exit_proc
    
index_error:
    call index_error_message ; Вызов обработчика ошибки
    xor eax, eax             ; Обнуляем возвращаемое значение
    
exit_proc:
    pop ebx
    pop esi
    mov esp, ebp
    pop ebp
    ret 12               ; Возврат с очисткой стека
GetIntArrayElement endp

; Преобразование целого числа в строковое представление
; Входные параметры:
;   - Указатель на буфер строки
;   - Преобразуемое целое число
; Выход: Строковое представление числа в буфере
ToChar proc uses eax ebx ecx edi esi, pstr : dword, intfield : sdword
    mov edi, pstr          ; Указатель на строку
    mov esi, 0             ; Счетчик символов
    mov eax, intfield      ; Загружаем входное число
    cdq                    ; Расширяем знак в edx (для деления)

    ; Обработка отрицательных чисел
    test eax, eax          ; Проверяем знак числа
    jns positive           ; Переходим к обработке положительного числа, если число >= 0

    ; Для отрицательного числа
    mov byte ptr [edi], '-' ; Добавляем знак '-'
    inc edi                ; Сдвигаем указатель на следующий символ
    neg eax                ; Делаем число положительным

positive:
    mov ebx, 10            ; Делитель - основание системы счисления
    xor ecx, ecx           ; Сбрасываем счетчик символов

cycle:
    xor edx, edx           ; Очищаем edx перед делением
    div ebx                ; eax = eax / 10, edx = остаток
    push dx                ; Сохраняем остаток на стек
    inc ecx                ; Увеличиваем количество цифр
    test eax, eax          ; Проверяем, закончились ли цифры
    jnz cycle              ; Если нет, продолжаем цикл

    ; Записываем символы в строку
write:
    pop dx                 ; Вытаскиваем следующую цифру со стека
    add dl, '0'            ; Преобразуем число в ASCII
    mov [edi], dl          ; Записываем символ в строку
    inc edi                ; Сдвигаем указатель
    loop write             ; Повторяем, пока ecx > 0

    ; Завершаем строку нулевым символом
    mov byte ptr [edi], 0  ; Завершаем строку символом '\0'

    ret                    ; Возвращаемся
ToChar endp

; Вывод строки в консоль
; Входные параметры:
;   - Указатель на выводимую строку
; Выполняет печать строки с использованием консольного API Windows
PrintConsole proc uses eax edx ecx, value : dword
    push STD_OUTPUT_HANDLE  ; STD_OUTPUT_HANDLE
    call GetStdHandle       ; Получить дескриптор консоли
    mov ebx, eax            ; Сохраняем дескриптор

    ; Подсчет длины строки
    mov esi, value          ; Указатель на строку
    xor ecx, ecx            ; Сбрасываем счетчик символов

count_loop:
    mov al, [esi]           ; Загружаем текущий символ
    test al, al             ; Проверяем, нулевой ли это байт
    jz count_done           ; Если нулевой, завершаем подсчет
    inc ecx                 ; Увеличиваем счетчик
    inc esi                 ; Переходим к следующему символу
    jmp count_loop          ; Продолжаем подсчет

count_done:
    ; Вывод строки
    push 0                  
    push 0
    push ecx                ; Длина строки
    push value              ; Указатель на строку
    push ebx                ; Дескриптор консоли
    call WriteConsoleA      ; Вызов WriteConsoleA
    ret
PrintConsole endp

PrintConsoleNewLine proc uses eax edx ecx, value : dword
    push value
    call PrintConsole
    push offset newLine    ; Передаем символ новой строки
    call PrintConsole       ; Используем существующий PrintConsole
    ret
PrintConsoleNewLine endp

PrintUnsignedInt proc uses eax ebx edx ecx, value : dword
    push value              ; Число
    push offset buffer
    call ToChar             ; Преобразуем число в строку
    push offset buffer
    call PrintConsole        ; Выводим строку
    ret
PrintUnsignedInt endp

PrintUnsignedIntNewLine proc uses eax edx ecx, value : dword
    push value
    call printUnsignedInt   ; Вывод числа
    push offset newLine     ; Символ новой строки
    call PrintConsole
    ret
PrintUnsignedIntNewLine endp

PrintInt proc uses eax ebx edx ecx, value : sdword
    push value              ; Число
    push offset buffer
    call ToChar             ; Преобразуем число в строку
    push offset buffer
    call PrintConsole        ; Выводим строку
    ret
PrintInt endp

PrintIntNewLine proc uses eax edx ecx, value : sdword
    push value
    call printInt           ; Вывод числа
    push offset newLine     ; Символ новой строки
    call PrintConsole
    ret
PrintIntNewLine endp

PrintBoolean proc uses esi edi, bool_value: dword
    cmp bool_value, 0        ; Проверяем значение
    je print_false           ; Если 0, печатаем "false"
    
    ; Печатаем "true"
    lea esi, true_message    ; Загружаем адрес строки "true"
    jmp print_string         ; Переход к печати строки
    
print_false:
    lea esi, false_message   ; Загружаем адрес строки "false"

print_string:
    push esi                 ; Адрес строки передается в стек
    call PrintConsole         ; Вызов функции для печати строки
    ret
PrintBoolean endp

PrintBooleanNewLine proc uses esi edi, bool_value: dword
    cmp bool_value, 0        ; Проверяем значение
    je print_false_nl        ; Если 0, печатаем "false"
    
    ; Печатаем "true"
    lea esi, true_message    ; Загружаем адрес строки "true"
    jmp print_string_nl      ; Переход к печати строки
    
print_false_nl:
    lea esi, false_message   ; Загружаем адрес строки "false"

print_string_nl:
    push esi                 ; Адрес строки передается в стек
    call PrintConsoleNewLine  ; Вызов функции для печати строки с новой строки
    ret
PrintBooleanNewLine endp

end