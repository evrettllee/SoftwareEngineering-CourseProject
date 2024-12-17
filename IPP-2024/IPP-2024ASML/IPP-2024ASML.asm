.586
.model flat, stdcall
includelib kernel32.lib
includelib ../Debug/IPP-2024L.lib
ExitProcess proto : dword
WriteConsoleA proto : dword, : dword, : dword, : dword, : dword
GetStdHandle proto : dword

.const
STD_OUTPUT_HANDLE equ -11 
error_message_overflow byte "������ 333: ������������� ����������: ������������ � ���������", 0
error_message_division_by_zero byte "������ 334: ������������� ����������: ������� �� ����", 0
error_message_index byte "������ 324: ������������� ����������: ������ ������� �� ������� �������", 0
true_message byte "true", 0
false_message byte "false", 0

.data
newLine db 13, 10, 0 
buffer byte " ", 0

.code
; ���������� ������ ������������. 
; ������� ��������� �� ������ � ��������� ���������
overflow_error_message proc
	push offset error_message_overflow
	call PrintConsoleNewLine
	push -1
	call ExitProcess
	ret
overflow_error_message endp

; ���������� ������ ������� �� ����. 
; ������� ��������� �� ������ � ��������� ���������
division_by_zero_error_message proc
	push offset error_message_division_by_zero
	call PrintConsoleNewLine
	push -1
	call ExitProcess
	ret
division_by_zero_error_message endp

; ���������� ������ ������ �� ������� �������. 
; ������� ��������� �� ������ � ��������� ���������
index_error_message proc
    push offset error_message_index
    call PrintConsoleNewLine
	push -1
	call ExitProcess
	ret
index_error_message endp

; ��������� �������� ���������� ������� �� ������� � ��������� ������
; ������� ���������: ������, ����� �������, ������ �������
; ���������� ����� �������� ��� �������� ���������� ������
GetStringArrayElement proc
    push ebp
    mov ebp, esp
    push esi
    push ebx

    mov edx, [ebp + 16]  ; ������ ������� � ���������
    mov esi, [ebp + 12]  ; ����� ������ �������
    mov ebx, [ebp + 8]   ; ������ ��������

    cmp ebx, 0
    jl index_error       ; ������� � ��������� ������, ���� ������ < 0
    cmp ebx, edx
    jge index_error      ; ������� � ��������� ������, ���� ������ >= ������� �������

    ; ���������� ����� �������� �������
    lea eax, [esi + ebx * 4]  ; ��������� ����� �������� �������
    jmp exit_proc

index_error:
    call index_error_message ; ����� ����������� ������
    xor eax, eax             ; �������� ������������ ��������
    
exit_proc:
    pop ebx
    pop esi
    mov esp, ebp
    pop ebp
    ret 12                   ; ������� � �������� �����
GetStringArrayElement endp

; ��������� �������� ������� �� ������� � ��������� ������
; ������� ���������:
;   - ������ ��������
;   - ����� �������
;   - ������ �������
; �����: ����� �������� ��� ��������� ������
GetIntArrayElement proc
    push ebp
    mov ebp, esp
    push esi
    push ebx
    
    mov edx, [ebp + 16]  ; ������ �������
    mov esi, [ebp + 12]  ; ����� �������
    mov ebx, [ebp + 8]   ; ������ ��������
    
    cmp ebx, 0
    jl index_error       ; ������� � ��������� ������, ���� ������ < 0
    cmp ebx, edx
    jge index_error      ; ������� � ��������� ������, ���� ������ >= ������� �������
    
    ; ��������� ����� �������� ������� ��������
    lea eax, [esi + ebx * 4]  ; ����� �������� � eax
    jmp exit_proc
    
index_error:
    call index_error_message ; ����� ����������� ������
    xor eax, eax             ; �������� ������������ ��������
    
exit_proc:
    pop ebx
    pop esi
    mov esp, ebp
    pop ebp
    ret 12               ; ������� � �������� �����
GetIntArrayElement endp

; �������������� ������ ����� � ��������� �������������
; ������� ���������:
;   - ��������� �� ����� ������
;   - ������������� ����� �����
; �����: ��������� ������������� ����� � ������
ToChar proc uses eax ebx ecx edi esi, pstr : dword, intfield : sdword
    mov edi, pstr          ; ��������� �� ������
    mov esi, 0             ; ������� ��������
    mov eax, intfield      ; ��������� ������� �����
    cdq                    ; ��������� ���� � edx (��� �������)

    ; ��������� ������������� �����
    test eax, eax          ; ��������� ���� �����
    jns positive           ; ��������� � ��������� �������������� �����, ���� ����� >= 0

    ; ��� �������������� �����
    mov byte ptr [edi], '-' ; ��������� ���� '-'
    inc edi                ; �������� ��������� �� ��������� ������
    neg eax                ; ������ ����� �������������

positive:
    mov ebx, 10            ; �������� - ��������� ������� ���������
    xor ecx, ecx           ; ���������� ������� ��������

cycle:
    xor edx, edx           ; ������� edx ����� ��������
    div ebx                ; eax = eax / 10, edx = �������
    push dx                ; ��������� ������� �� ����
    inc ecx                ; ����������� ���������� ����
    test eax, eax          ; ���������, ����������� �� �����
    jnz cycle              ; ���� ���, ���������� ����

    ; ���������� ������� � ������
write:
    pop dx                 ; ����������� ��������� ����� �� �����
    add dl, '0'            ; ����������� ����� � ASCII
    mov [edi], dl          ; ���������� ������ � ������
    inc edi                ; �������� ���������
    loop write             ; ���������, ���� ecx > 0

    ; ��������� ������ ������� ��������
    mov byte ptr [edi], 0  ; ��������� ������ �������� '\0'

    ret                    ; ������������
ToChar endp

; ����� ������ � �������
; ������� ���������:
;   - ��������� �� ��������� ������
; ��������� ������ ������ � �������������� ����������� API Windows
PrintConsole proc uses eax edx ecx, value : dword
    push STD_OUTPUT_HANDLE  ; STD_OUTPUT_HANDLE
    call GetStdHandle       ; �������� ���������� �������
    mov ebx, eax            ; ��������� ����������

    ; ������� ����� ������
    mov esi, value          ; ��������� �� ������
    xor ecx, ecx            ; ���������� ������� ��������

count_loop:
    mov al, [esi]           ; ��������� ������� ������
    test al, al             ; ���������, ������� �� ��� ����
    jz count_done           ; ���� �������, ��������� �������
    inc ecx                 ; ����������� �������
    inc esi                 ; ��������� � ���������� �������
    jmp count_loop          ; ���������� �������

count_done:
    ; ����� ������
    push 0                  
    push 0
    push ecx                ; ����� ������
    push value              ; ��������� �� ������
    push ebx                ; ���������� �������
    call WriteConsoleA      ; ����� WriteConsoleA
    ret
PrintConsole endp

PrintConsoleNewLine proc uses eax edx ecx, value : dword
    push value
    call PrintConsole
    push offset newLine    ; �������� ������ ����� ������
    call PrintConsole       ; ���������� ������������ PrintConsole
    ret
PrintConsoleNewLine endp

PrintUnsignedInt proc uses eax ebx edx ecx, value : dword
    push value              ; �����
    push offset buffer
    call ToChar             ; ����������� ����� � ������
    push offset buffer
    call PrintConsole        ; ������� ������
    ret
PrintUnsignedInt endp

PrintUnsignedIntNewLine proc uses eax edx ecx, value : dword
    push value
    call printUnsignedInt   ; ����� �����
    push offset newLine     ; ������ ����� ������
    call PrintConsole
    ret
PrintUnsignedIntNewLine endp

PrintInt proc uses eax ebx edx ecx, value : sdword
    push value              ; �����
    push offset buffer
    call ToChar             ; ����������� ����� � ������
    push offset buffer
    call PrintConsole        ; ������� ������
    ret
PrintInt endp

PrintIntNewLine proc uses eax edx ecx, value : sdword
    push value
    call printInt           ; ����� �����
    push offset newLine     ; ������ ����� ������
    call PrintConsole
    ret
PrintIntNewLine endp

PrintBoolean proc uses esi edi, bool_value: dword
    cmp bool_value, 0        ; ��������� ��������
    je print_false           ; ���� 0, �������� "false"
    
    ; �������� "true"
    lea esi, true_message    ; ��������� ����� ������ "true"
    jmp print_string         ; ������� � ������ ������
    
print_false:
    lea esi, false_message   ; ��������� ����� ������ "false"

print_string:
    push esi                 ; ����� ������ ���������� � ����
    call PrintConsole         ; ����� ������� ��� ������ ������
    ret
PrintBoolean endp

PrintBooleanNewLine proc uses esi edi, bool_value: dword
    cmp bool_value, 0        ; ��������� ��������
    je print_false_nl        ; ���� 0, �������� "false"
    
    ; �������� "true"
    lea esi, true_message    ; ��������� ����� ������ "true"
    jmp print_string_nl      ; ������� � ������ ������
    
print_false_nl:
    lea esi, false_message   ; ��������� ����� ������ "false"

print_string_nl:
    push esi                 ; ����� ������ ���������� � ����
    call PrintConsoleNewLine  ; ����� ������� ��� ������ ������ � ����� ������
    ret
PrintBooleanNewLine endp

end